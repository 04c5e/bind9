/*
 * Copyright (C) 2015  Internet Systems Consortium, Inc. ("ISC")
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND ISC DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS.  IN NO EVENT SHALL ISC BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
 * OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#include <string.h>

#include <isc/hash.h>
#include <isc/ht.h>
#include <isc/types.h>
#include <isc/magic.h>
#include <isc/mem.h>
#include <isc/result.h>
#include <isc/util.h>


typedef struct isc_ht_node isc_ht_node_t;

#define ISC_HT_MAGIC			ISC_MAGIC('H', 'T', 'a', 'b')
#define ISC_HT_VALID(ht)		ISC_MAGIC_VALID(ht, ISC_HT_MAGIC)

struct isc_ht_node {
	void *value;
	isc_ht_node_t *next;
	size_t keysize;
	unsigned char key[];
};

struct isc_ht {
	unsigned int magic;
	isc_mem_t *mctx;
	size_t size;
	size_t mask;
	unsigned int count;
	isc_ht_node_t **table;
};

struct isc_ht_iter {
	isc_ht_t *ht;
	isc_uint32_t i;
	isc_ht_node_t *cur;
};

isc_result_t
isc_ht_init(isc_ht_t **htp, isc_mem_t *mctx, isc_uint8_t bits) {
	isc_ht_t *ht = NULL;
	isc_uint32_t i;

	REQUIRE(htp != NULL && *htp == NULL);
	REQUIRE(mctx != NULL);
	REQUIRE(bits >= 1 && bits <= (sizeof(size_t)*8 - 1));

	ht = isc_mem_get(mctx, sizeof(struct isc_ht));
	if (ht == NULL) {
		return (ISC_R_NOMEMORY);
	}

	ht->mctx = NULL;
	isc_mem_attach(mctx, &ht->mctx);

	ht->size = (1<<bits);
	ht->mask = (1<<bits)-1;
	ht->count = 0;

	ht->table = isc_mem_get(ht->mctx, ht->size * sizeof(isc_ht_node_t*));
	if (ht->table == NULL) {
		isc_mem_putanddetach(&ht->mctx, ht, sizeof(struct isc_ht));
		return (ISC_R_NOMEMORY);
	}

	for (i = 0; i < ht->size; i++) {
		ht->table[i] = NULL;
	}

	ht->magic = ISC_HT_MAGIC;

	*htp = ht;
	return (ISC_R_SUCCESS);
}

void
isc_ht_destroy(isc_ht_t **htp) {
	isc_ht_t *ht;
	isc_uint32_t i;

	REQUIRE(htp != NULL);

	ht = *htp;
	REQUIRE(ISC_HT_VALID(ht));

	ht->magic = 0;

	for (i = 0; i < ht->size; i++) {
		isc_ht_node_t *node = ht->table[i];
		while (node != NULL) {
			isc_ht_node_t *next = node->next;
			ht->count--;
			isc_mem_put(ht->mctx, node,
					sizeof(isc_ht_node_t) + node->keysize);
			node = next;
		}
	}

	INSIST(ht->count == 0);

	isc_mem_put(ht->mctx, ht->table, ht->size * sizeof(isc_ht_node_t*));
	isc_mem_putanddetach(&ht->mctx, ht, sizeof(struct isc_ht));

	*htp = NULL;
}

isc_result_t
isc_ht_add(isc_ht_t *ht, const unsigned char *key,
	   isc_uint32_t keysize, void *value)
{
	isc_ht_node_t *node;
	isc_uint32_t hash;

	REQUIRE(ISC_HT_VALID(ht));
	REQUIRE(key != NULL && keysize > 0);

	hash = isc_hash_function(key, keysize, ISC_TRUE, NULL);
	node = ht->table[hash & ht->mask];
	while (node != NULL) {
		if (keysize == node->keysize &&
		    memcmp(key, node->key, keysize) == 0)
		{
			return (ISC_R_EXISTS);
		}
		node = node->next;
	}

	node = isc_mem_get(ht->mctx, sizeof(isc_ht_node_t) + keysize);
	if (node == NULL) {
		return (ISC_R_NOMEMORY);
	}

	memmove(node->key, key, keysize);
	node->keysize = keysize;
	node->next = ht->table[hash & ht->mask];
	node->value = value;

	ht->count++;
	ht->table[hash & ht->mask] = node;
	return (ISC_R_SUCCESS);
}

isc_result_t
isc_ht_find(const isc_ht_t *ht, const unsigned char *key,
	    isc_uint32_t keysize, void **valuep)
{
	isc_ht_node_t *node;
	isc_uint32_t hash;

	REQUIRE(ISC_HT_VALID(ht));
	REQUIRE(key != NULL && keysize > 0);
	REQUIRE(valuep != NULL);

	hash = isc_hash_function(key, keysize, ISC_TRUE, NULL);
	node = ht->table[hash & ht->mask];
	while (node != NULL) {
		if (keysize == node->keysize &&
		    memcmp(key, node->key, keysize) == 0)
		{
			*valuep = node->value;
			return (ISC_R_SUCCESS);
		}
		node = node->next;
	}

	return (ISC_R_NOTFOUND);
}

isc_result_t
isc_ht_delete(isc_ht_t *ht, const unsigned char *key, isc_uint32_t keysize) {
	isc_ht_node_t *node, *prev;
	isc_uint32_t hash;

	REQUIRE(ISC_HT_VALID(ht));
	REQUIRE(key != NULL && keysize > 0);

	prev = NULL;
	hash = isc_hash_function(key, keysize, ISC_TRUE, NULL);
	node = ht->table[hash & ht->mask];
	while (node != NULL) {
		if (keysize == node->keysize &&
		    memcmp(key, node->key, keysize) == 0)
		{
			if (prev == NULL) {
				ht->table[hash & ht->mask] = node->next;
			} else {
				prev->next = node->next;
			}
			isc_mem_put(ht->mctx, node,
					sizeof(isc_ht_node_t) + node->keysize);
			ht->count--;

			return (ISC_R_SUCCESS);
		}

		prev = node;
		node = node->next;
	}
	return (ISC_R_NOTFOUND);
}

isc_result_t
isc_ht_iter_create(isc_ht_t *ht, isc_ht_iter_t **itp) {
	isc_ht_iter_t *it;

	REQUIRE(ISC_HT_VALID(ht));
	REQUIRE(itp != NULL && *itp == NULL);

	if (ht->count == 0)
		return (ISC_R_NOMORE);

	it = isc_mem_get(ht->mctx, sizeof(isc_ht_iter_t));
	if (it == NULL)
		return (ISC_R_NOMEMORY);

	it->ht = ht;
	it->i = 0;
	it->cur = NULL;

	*itp = it;

	return (ISC_R_SUCCESS);
}

void
isc_ht_iter_destroy(isc_ht_iter_t **itp) {
	isc_ht_iter_t *it;
	isc_ht_t *ht;

	REQUIRE(itp != NULL && *itp != NULL);

	it = *itp;
	ht = it->ht;
	isc_mem_put(ht->mctx, it, sizeof(isc_ht_iter_t));

	*itp = NULL;
}

isc_result_t
isc_ht_iter_first(isc_ht_iter_t *it) {
	REQUIRE(it != NULL);

	it->i = 0;
	while (it->i < it->ht->size && it->ht->table[it->i] == NULL)
		it->i++;

	if(it->i == it->ht->size)
		return (ISC_R_NOMORE);

	it->cur = it->ht->table[it->i];

	return (ISC_R_SUCCESS);
}

isc_result_t
isc_ht_iter_next(isc_ht_iter_t *it) {
	REQUIRE(it != NULL);
	REQUIRE(it->cur != NULL);

	it->cur = it->cur->next;
	if (it->cur == NULL) {
		while (it->i < it->ht->size && it->ht->table[it->i] == NULL)
			it->i++;
		if (it->i < it->ht->size)
			return (ISC_R_NOMORE);
		it->cur = it->ht->table[it->i];
	}

	return (ISC_R_SUCCESS);
}

void
isc_ht_iter_current(isc_ht_iter_t *it, void **valuep) {
	REQUIRE(it != NULL);
	REQUIRE(it->cur != NULL);
	*valuep = it->cur->value;
}

unsigned int
isc_ht_count(isc_ht_t *ht) {
	REQUIRE(ISC_HT_VALID(ht));

	return(ht->count);
}

isc_result_t
isc_ht_walk(isc_ht_t *ht, isc_ht_walkfn walkfn, void *udata) {
	isc_uint32_t i;
	isc_result_t res;

	REQUIRE(ISC_HT_VALID(ht));

	for (i = 0; i < ht->size; i++) {
		isc_ht_node_t *cur = ht->table[i];
		isc_ht_node_t *prev = NULL;
		while (cur != NULL) {
			if (walkfn == NULL) {
				continue;
			}
			res = walkfn(udata, cur->key, cur->keysize, cur->value);
			if (res != ISC_R_SUCCESS && res != ISC_R_EXISTS) {
				return (res);
			}
			if (res == ISC_R_EXISTS) { /* remove this node */
				isc_ht_node_t *tmp = cur;
				cur = cur->next;
				if (prev == NULL) {
					ht->table[i] = cur;
				} else {
					prev->next = cur;
				}
				isc_mem_put(ht->mctx, tmp,
				        sizeof(isc_ht_node_t) + tmp->keysize);
				ht->count--;
			} else {
				prev = cur;
				cur = cur->next;
			}
		}
	}

	return (ISC_R_SUCCESS);
}
