/*
 * Copyright (C) 1999-2001  Internet Software Consortium.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND INTERNET SOFTWARE CONSORTIUM
 * DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL
 * INTERNET SOFTWARE CONSORTIUM BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING
 * FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION
 * WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/* $Id: named-checkconf.c,v 1.12.12.1 2003/08/01 07:09:50 marka Exp $ */

#include <config.h>

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#include <isc/commandline.h>
#include <isc/dir.h>
#include <isc/log.h>
#include <isc/mem.h>
#include <isc/result.h>
#include <isc/string.h>
#include <isc/util.h>

#include <isccfg/cfg.h>
#include <isccfg/check.h>

#include <dns/log.h>
#include <dns/result.h>

#include "check-tool.h"

isc_log_t *logc = NULL;

#define CHECK(r)\
	do { \
		result = (r); \
		if (result != ISC_R_SUCCESS) \
			goto cleanup; \
	} while (0)

static void
usage(void) {
        fprintf(stderr, "usage: named-checkconf [-v] [-z] [-t directory] "
		"[named.conf]\n");
        exit(1);
}

static isc_result_t
directory_callback(const char *clausename, cfg_obj_t *obj, void *arg) {
	isc_result_t result;
	char *directory;

	REQUIRE(strcasecmp("directory", clausename) == 0);

	UNUSED(arg);
	UNUSED(clausename);

	/*
	 * Change directory.
	 */
	directory = cfg_obj_asstring(obj);
	result = isc_dir_chdir(directory);
	if (result != ISC_R_SUCCESS) {
		cfg_obj_log(obj, logc, ISC_LOG_ERROR,
			    "change directory to '%s' failed: %s\n",
			    directory, isc_result_totext(result));
		return (result);
	}

	return (ISC_R_SUCCESS);
}

static isc_result_t
configure_zone(const char *vclass, const char *view, cfg_obj_t *zconfig,
	       isc_mem_t *mctx)
{
	isc_result_t result;
	const char *zclass;
	const char *zname;
	const char *zfile;
	cfg_obj_t *zoptions = NULL;
	cfg_obj_t *classobj = NULL;
	cfg_obj_t *typeobj = NULL;
	cfg_obj_t *fileobj = NULL;
	cfg_obj_t *dbobj = NULL;

	zname = cfg_obj_asstring(cfg_tuple_get(zconfig, "name"));
	classobj = cfg_tuple_get(zconfig, "class");
        if (!cfg_obj_isstring(classobj))
                zclass = vclass;
        else
		zclass = cfg_obj_asstring(classobj);
	zoptions = cfg_tuple_get(zconfig, "options");
	cfg_map_get(zoptions, "type", &typeobj);
	if (typeobj == NULL)
		return (ISC_R_FAILURE);
	if (strcasecmp(cfg_obj_asstring(typeobj), "master") != 0)
		return (ISC_R_SUCCESS);
        cfg_map_get(zoptions, "database", &dbobj);
        if (dbobj != NULL)
                return (ISC_R_SUCCESS);
	cfg_map_get(zoptions, "file", &fileobj);
	if (fileobj == NULL)
		return (ISC_R_FAILURE);
	zfile = cfg_obj_asstring(fileobj);
	result = load_zone(mctx, zname, zfile, zclass, NULL);
	if (result != ISC_R_SUCCESS)
		fprintf(stderr, "%s/%s/%s: %s\n", view, zname, zclass,
			dns_result_totext(result));
	return(result);
}

static isc_result_t
configure_view(const char *vclass, const char *view, cfg_obj_t *config,
	       cfg_obj_t *vconfig, isc_mem_t *mctx)
{
	cfg_listelt_t *element;
	cfg_obj_t *voptions;
	cfg_obj_t *zonelist;
	isc_result_t result = ISC_R_SUCCESS;

	voptions = NULL;
	if (vconfig != NULL)
		voptions = cfg_tuple_get(vconfig, "options");

	zonelist = NULL;
	if (voptions != NULL)
		(void)cfg_map_get(voptions, "zone", &zonelist);
	else
		(void)cfg_map_get(config, "zone", &zonelist);

	for (element = cfg_list_first(zonelist);
	     element != NULL;
	     element = cfg_list_next(element))
	{
		cfg_obj_t *zconfig = cfg_listelt_value(element);
		CHECK(configure_zone(vclass, view, zconfig, mctx));
	}
 cleanup:
	return (result);
}


static isc_result_t
load_zones_fromconfig(cfg_obj_t *config, isc_mem_t *mctx) {
	cfg_listelt_t *element;
	cfg_obj_t *classobj;
	cfg_obj_t *views;
	cfg_obj_t *vconfig;
	const char *vclass;
	isc_result_t result = ISC_R_SUCCESS;

	views = NULL;

	(void)cfg_map_get(config, "view", &views);
	for (element = cfg_list_first(views);
	     element != NULL;
	     element = cfg_list_next(element))
	{
		const char *vname;

		vclass = "IN";
		vconfig = cfg_listelt_value(element);
		if (vconfig != NULL) {
			classobj = cfg_tuple_get(vconfig, "class");
			if (cfg_obj_isstring(classobj))
				vclass = cfg_obj_asstring(classobj);
		}
		vname = cfg_obj_asstring(cfg_tuple_get(vconfig, "name"));
		CHECK(configure_view(vclass, vname, config, vconfig, mctx));
	}

	if (views == NULL)
		CHECK(configure_view("IN", "_default", config, NULL, mctx));
 cleanup:
	return (result);
}

int
main(int argc, char **argv) {
	int c;
	cfg_parser_t *parser = NULL;
	cfg_obj_t *config = NULL;
	const char *conffile = NULL;
	isc_mem_t *mctx = NULL;
	isc_result_t result;
	int exit_status = 0;
	isc_boolean_t load_zones = ISC_FALSE;

	while ((c = isc_commandline_parse(argc, argv, "dt:vz")) != EOF) {
		switch (c) {
		case 'd':
			debug++;
			break;

		case 't':
			result = isc_dir_chroot(isc_commandline_argument);
			if (result != ISC_R_SUCCESS) {
				fprintf(stderr, "isc_dir_chroot: %s\n",
					isc_result_totext(result));
				exit(1);
			}
			result = isc_dir_chdir("/");
			if (result != ISC_R_SUCCESS) {
				fprintf(stderr, "isc_dir_chdir: %s\n",
					isc_result_totext(result));
				exit(1);
			}
			break;

		case 'v':
			printf(VERSION "\n");
			exit(0);

		case 'z':
			load_zones = ISC_TRUE;
			break;

		default:
			usage();
		}
	}

	if (argv[isc_commandline_index] != NULL)
		conffile = argv[isc_commandline_index];
	if (conffile == NULL || conffile[0] == '\0')
		conffile = NAMED_CONFFILE;

	RUNTIME_CHECK(isc_mem_create(0, 0, &mctx) == ISC_R_SUCCESS);

	RUNTIME_CHECK(setup_logging(mctx, &logc) == ISC_R_SUCCESS);

	dns_result_register();

	RUNTIME_CHECK(cfg_parser_create(mctx, logc, &parser) == ISC_R_SUCCESS);

	cfg_parser_setcallback(parser, directory_callback, NULL);

	if (cfg_parse_file(parser, conffile, &cfg_type_namedconf, &config) !=
	    ISC_R_SUCCESS)
		exit(1);

	result = cfg_check_namedconf(config, logc, mctx);
	if (result != ISC_R_SUCCESS)
		exit_status = 1;

	if (result == ISC_R_SUCCESS && load_zones) {
		dns_log_init(logc);
                dns_log_setcontext(logc);
		result = load_zones_fromconfig(config, mctx);
		if (result != ISC_R_SUCCESS)
			exit_status = 1;
	}

	cfg_obj_destroy(parser, &config);

	cfg_parser_destroy(&parser);

	isc_log_destroy(&logc);

	isc_mem_destroy(&mctx);

	return (exit_status);
}
