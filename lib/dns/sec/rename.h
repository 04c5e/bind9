/*
 * Copyright (C) 2000  Internet Software Consortium.
 * 
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND INTERNET SOFTWARE CONSORTIUM DISCLAIMS
 * ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL INTERNET SOFTWARE
 * CONSORTIUM BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 */

#ifdef DST_USE_PRIVATE_OPENSSL
#define BN_CTX_end dst_BN_CTX_end
#define BN_CTX_free dst_BN_CTX_free
#define BN_CTX_get dst_BN_CTX_get
#define BN_CTX_init dst_BN_CTX_init
#define BN_CTX_new dst_BN_CTX_new
#define BN_CTX_start dst_BN_CTX_start
#define BN_MONT_CTX_copy dst_BN_MONT_CTX_copy
#define BN_MONT_CTX_free dst_BN_MONT_CTX_free
#define BN_MONT_CTX_init dst_BN_MONT_CTX_init
#define BN_MONT_CTX_new dst_BN_MONT_CTX_new
#define BN_MONT_CTX_set dst_BN_MONT_CTX_set
#define BN_RECP_CTX_free dst_BN_RECP_CTX_free
#define BN_RECP_CTX_init dst_BN_RECP_CTX_init
#define BN_RECP_CTX_new dst_BN_RECP_CTX_new
#define BN_RECP_CTX_set dst_BN_RECP_CTX_set
#define BN_add dst_BN_add
#define BN_add_word dst_BN_add_word
#define BN_bin2bn dst_BN_bin2bn
#define BN_bn2bin dst_BN_bn2bin
#define BN_bn2hex dst_BN_bn2hex
#define BN_clear dst_BN_clear
#define BN_clear_bit dst_BN_clear_bit
#define BN_clear_free dst_BN_clear_free
#define BN_cmp dst_BN_cmp
#define BN_copy dst_BN_copy
#define BN_div dst_BN_div
#define BN_div_recp dst_BN_div_recp
#define BN_div_word dst_BN_div_word
#define BN_dup dst_BN_dup
#define BN_exp dst_BN_exp
#define BN_free dst_BN_free
#define BN_from_montgomery dst_BN_from_montgomery
#define BN_gcd dst_BN_gcd
#define BN_generate_prime dst_BN_generate_prime
#define BN_get_params dst_BN_get_params
#define BN_get_word dst_BN_get_word
#define BN_hex2bn dst_BN_hex2bn
#define BN_init dst_BN_init
#define BN_is_bit_set dst_BN_is_bit_set
#define BN_is_prime dst_BN_is_prime
#define BN_is_prime_fasttest dst_BN_is_prime_fasttest
#define BN_lshift dst_BN_lshift
#define BN_lshift1 dst_BN_lshift1
#define BN_mask_bits dst_BN_mask_bits
#define BN_mod dst_BN_mod
#define BN_mod_exp dst_BN_mod_exp
#define BN_mod_exp2_mont dst_BN_mod_exp2_mont
#define BN_mod_exp_mont dst_BN_mod_exp_mont
#define BN_mod_exp_recp dst_BN_mod_exp_recp
#define BN_mod_exp_simple dst_BN_mod_exp_simple
#define BN_mod_inverse dst_BN_mod_inverse
#define BN_mod_mul dst_BN_mod_mul
#define BN_mod_mul_montgomery dst_BN_mod_mul_montgomery
#define BN_mod_mul_reciprocal dst_BN_mod_mul_reciprocal
#define BN_mod_word dst_BN_mod_word
#define BN_mul dst_BN_mul
#define BN_mul_word dst_BN_mul_word
#define BN_new dst_BN_new
#define BN_num_bits dst_BN_num_bits
#define BN_num_bits_word dst_BN_num_bits_word
#define BN_options dst_BN_options
#define BN_pseudo_rand dst_BN_pseudo_rand
#define BN_rand dst_BN_rand
#define BN_reciprocal dst_BN_reciprocal
#define BN_rshift dst_BN_rshift
#define BN_rshift1 dst_BN_rshift1
#define BN_set_bit dst_BN_set_bit
#define BN_set_params dst_BN_set_params
#define BN_set_word dst_BN_set_word
#define BN_sqr dst_BN_sqr
#define BN_sub dst_BN_sub
#define BN_sub_word dst_BN_sub_word
#define BN_uadd dst_BN_uadd
#define BN_ucmp dst_BN_ucmp
#define BN_usub dst_BN_usub
#define BN_value_one dst_BN_value_one
#define BUF_MEM_free dst_BUF_MEM_free
#define BUF_MEM_grow dst_BUF_MEM_grow
#define BUF_MEM_new dst_BUF_MEM_new
#define BUF_strdup dst_BUF_strdup
#define CRYPTO_add_lock dst_CRYPTO_add_lock
#define CRYPTO_dbg_free dst_CRYPTO_dbg_free
#define CRYPTO_dbg_get_options dst_CRYPTO_dbg_get_options
#define CRYPTO_dbg_malloc dst_CRYPTO_dbg_malloc
#define CRYPTO_dbg_realloc dst_CRYPTO_dbg_realloc
#define CRYPTO_dbg_set_options dst_CRYPTO_dbg_set_options
#define CRYPTO_dup_ex_data dst_CRYPTO_dup_ex_data
#define CRYPTO_free dst_CRYPTO_free
#define CRYPTO_free_ex_data dst_CRYPTO_free_ex_data
#define CRYPTO_free_locked dst_CRYPTO_free_locked
#define CRYPTO_get_add_lock_callback dst_CRYPTO_get_add_lock_callback
#define CRYPTO_get_ex_data dst_CRYPTO_get_ex_data
#define CRYPTO_get_ex_new_index dst_CRYPTO_get_ex_new_index
#define CRYPTO_get_id_callback dst_CRYPTO_get_id_callback
#define CRYPTO_get_lock_name dst_CRYPTO_get_lock_name
#define CRYPTO_get_locked_mem_functions dst_CRYPTO_get_locked_mem_functions
#define CRYPTO_get_locking_callback dst_CRYPTO_get_locking_callback
#define CRYPTO_get_mem_debug_functions dst_CRYPTO_get_mem_debug_functions
#define CRYPTO_get_mem_debug_options dst_CRYPTO_get_mem_debug_options
#define CRYPTO_get_mem_functions dst_CRYPTO_get_mem_functions
#define CRYPTO_get_new_lockid dst_CRYPTO_get_new_lockid
#define CRYPTO_is_mem_check_on dst_CRYPTO_is_mem_check_on
#define CRYPTO_lock dst_CRYPTO_lock
#define CRYPTO_malloc dst_CRYPTO_malloc
#define CRYPTO_malloc_locked dst_CRYPTO_malloc_locked
#define CRYPTO_mem_ctrl dst_CRYPTO_mem_ctrl
#define CRYPTO_new_ex_data dst_CRYPTO_new_ex_data
#define CRYPTO_num_locks dst_CRYPTO_num_locks
#define CRYPTO_pop_info dst_CRYPTO_pop_info
#define CRYPTO_push_info_ dst_CRYPTO_push_info_
#define CRYPTO_realloc dst_CRYPTO_realloc
#define CRYPTO_remalloc dst_CRYPTO_remalloc
#define CRYPTO_remove_all_info dst_CRYPTO_remove_all_info
#define CRYPTO_set_add_lock_callback dst_CRYPTO_set_add_lock_callback
#define CRYPTO_set_ex_data dst_CRYPTO_set_ex_data
#define CRYPTO_set_id_callback dst_CRYPTO_set_id_callback
#define CRYPTO_set_locked_mem_functions dst_CRYPTO_set_locked_mem_functions
#define CRYPTO_set_locking_callback dst_CRYPTO_set_locking_callback
#define CRYPTO_set_mem_debug_functions dst_CRYPTO_set_mem_debug_functions
#define CRYPTO_set_mem_debug_options dst_CRYPTO_set_mem_debug_options
#define CRYPTO_set_mem_functions dst_CRYPTO_set_mem_functions
#define CRYPTO_thread_id dst_CRYPTO_thread_id
#define DH_OpenSSL dst_DH_OpenSSL
#define DH_compute_key dst_DH_compute_key
#define DH_free dst_DH_free
#define DH_generate_key dst_DH_generate_key
#define DH_generate_parameters dst_DH_generate_parameters
#define DH_get_default_method dst_DH_get_default_method
#define DH_get_ex_data dst_DH_get_ex_data
#define DH_get_ex_new_index dst_DH_get_ex_new_index
#define DH_new dst_DH_new
#define DH_new_method dst_DH_new_method
#define DH_set_default_method dst_DH_set_default_method
#define DH_set_ex_data dst_DH_set_ex_data
#define DH_set_method dst_DH_set_method
#define DH_size dst_DH_size
#define DSA_OpenSSL dst_DSA_OpenSSL
#define DSA_SIG_free dst_DSA_SIG_free
#define DSA_SIG_new dst_DSA_SIG_new
#define DSA_do_sign dst_DSA_do_sign
#define DSA_do_verify dst_DSA_do_verify
#define DSA_free dst_DSA_free
#define DSA_generate_key dst_DSA_generate_key
#define DSA_generate_parameters dst_DSA_generate_parameters
#define DSA_get_default_method dst_DSA_get_default_method
#define DSA_get_ex_data dst_DSA_get_ex_data
#define DSA_get_ex_new_index dst_DSA_get_ex_new_index
#define DSA_new dst_DSA_new
#define DSA_new_method dst_DSA_new_method
#define DSA_set_default_method dst_DSA_set_default_method
#define DSA_set_ex_data dst_DSA_set_ex_data
#define DSA_set_method dst_DSA_set_method
#define DSA_sign_setup dst_DSA_sign_setup
#define ERR_add_error_data dst_ERR_add_error_data
#define ERR_clear_error dst_ERR_clear_error
#define ERR_error_string dst_ERR_error_string
#define ERR_free_strings dst_ERR_free_strings
#define ERR_func_error_string dst_ERR_func_error_string
#define ERR_get_err_state_table dst_ERR_get_err_state_table
#define ERR_get_error dst_ERR_get_error
#define ERR_get_error_line dst_ERR_get_error_line
#define ERR_get_error_line_data dst_ERR_get_error_line_data
#define ERR_get_next_error_library dst_ERR_get_next_error_library
#define ERR_get_state dst_ERR_get_state
#define ERR_get_string_table dst_ERR_get_string_table
#define ERR_lib_error_string dst_ERR_lib_error_string
#define ERR_load_BN_strings dst_ERR_load_BN_strings
#define ERR_load_DH_strings dst_ERR_load_DH_strings
#define ERR_load_DSA_strings dst_ERR_load_DSA_strings
#define ERR_load_ERR_strings dst_ERR_load_ERR_strings
#define ERR_load_strings dst_ERR_load_strings
#define ERR_peek_error dst_ERR_peek_error
#define ERR_peek_error_line dst_ERR_peek_error_line
#define ERR_peek_error_line_data dst_ERR_peek_error_line_data
#define ERR_put_error dst_ERR_put_error
#define ERR_reason_error_string dst_ERR_reason_error_string
#define ERR_remove_state dst_ERR_remove_state
#define ERR_set_error_data dst_ERR_set_error_data
#define MD5_Final dst_MD5_Final
#define MD5_Init dst_MD5_Init
#define MD5_Transform dst_MD5_Transform
#define MD5_Update dst_MD5_Update
#define RAND_SSLeay dst_RAND_SSLeay
#define RAND_add dst_RAND_add
#define RAND_bytes dst_RAND_bytes
#define RAND_cleanup dst_RAND_cleanup
#define RAND_get_rand_method dst_RAND_get_rand_method
#define RAND_pseudo_bytes dst_RAND_pseudo_bytes
#define RAND_seed dst_RAND_seed
#define RAND_set_rand_method dst_RAND_set_rand_method
#define RAND_status dst_RAND_status
#define rand_ssleay_meth dst_rand_ssleay_meth
#define SHA1 dst_SHA1
#define SHA1_Final dst_SHA1_Final
#define SHA1_Init dst_SHA1_Init
#define SHA1_Transform dst_SHA1_Transform
#define SHA1_Update dst_SHA1_Update
#define bn_add_words dst_bn_add_words
#define bn_cmp_words dst_bn_cmp_words
#define bn_div_words dst_bn_div_words
#define bn_expand2 dst_bn_expand2
#define bn_mul_add_words dst_bn_mul_add_words
#define bn_mul_comba4 dst_bn_mul_comba4
#define bn_mul_comba8 dst_bn_mul_comba8
#define bn_mul_high dst_bn_mul_high
#define bn_mul_low_normal dst_bn_mul_low_normal
#define bn_mul_low_recursive dst_bn_mul_low_recursive
#define bn_mul_normal dst_bn_mul_normal
#define bn_mul_part_recursive dst_bn_mul_part_recursive
#define bn_mul_recursive dst_bn_mul_recursive
#define bn_mul_words dst_bn_mul_words
#define bn_sqr_comba4 dst_bn_sqr_comba4
#define bn_sqr_comba8 dst_bn_sqr_comba8
#define bn_sqr_normal dst_bn_sqr_normal
#define bn_sqr_recursive dst_bn_sqr_recursive
#define bn_sqr_words dst_bn_sqr_words
#define bn_sub_words dst_bn_sub_words
#define lh_delete dst_lh_delete
#define lh_doall dst_lh_doall
#define lh_doall_arg dst_lh_doall_arg
#define lh_free dst_lh_free
#define lh_insert dst_lh_insert
#define lh_new dst_lh_new
#define lh_retrieve dst_lh_retrieve
#define lh_strhash dst_lh_strhash
#define md5_block_host_order dst_md5_block_host_order
#define sha1_block_data_order dst_sha1_block_data_order
#define sha1_block_host_order dst_sha1_block_host_order
#define sk_CRYPTO_EX_DATA_FUNCS_delete dst_sk_CRYPTO_EX_DATA_FUNCS_delete
#define sk_CRYPTO_EX_DATA_FUNCS_delete_ptr dst_sk_CRYPTO_EX_DATA_FUNCS_delete_ptr
#define sk_CRYPTO_EX_DATA_FUNCS_dup dst_sk_CRYPTO_EX_DATA_FUNCS_dup
#define sk_CRYPTO_EX_DATA_FUNCS_find dst_sk_CRYPTO_EX_DATA_FUNCS_find
#define sk_CRYPTO_EX_DATA_FUNCS_free dst_sk_CRYPTO_EX_DATA_FUNCS_free
#define sk_CRYPTO_EX_DATA_FUNCS_insert dst_sk_CRYPTO_EX_DATA_FUNCS_insert
#define sk_CRYPTO_EX_DATA_FUNCS_new dst_sk_CRYPTO_EX_DATA_FUNCS_new
#define sk_CRYPTO_EX_DATA_FUNCS_new_null dst_sk_CRYPTO_EX_DATA_FUNCS_new_null
#define sk_CRYPTO_EX_DATA_FUNCS_num dst_sk_CRYPTO_EX_DATA_FUNCS_num
#define sk_CRYPTO_EX_DATA_FUNCS_pop dst_sk_CRYPTO_EX_DATA_FUNCS_pop
#define sk_CRYPTO_EX_DATA_FUNCS_pop_free dst_sk_CRYPTO_EX_DATA_FUNCS_pop_free
#define sk_CRYPTO_EX_DATA_FUNCS_push dst_sk_CRYPTO_EX_DATA_FUNCS_push
#define sk_CRYPTO_EX_DATA_FUNCS_set dst_sk_CRYPTO_EX_DATA_FUNCS_set
#define sk_CRYPTO_EX_DATA_FUNCS_set_cmp_func dst_sk_CRYPTO_EX_DATA_FUNCS_set_cmp_func
#define sk_CRYPTO_EX_DATA_FUNCS_shift dst_sk_CRYPTO_EX_DATA_FUNCS_shift
#define sk_CRYPTO_EX_DATA_FUNCS_sort dst_sk_CRYPTO_EX_DATA_FUNCS_sort
#define sk_CRYPTO_EX_DATA_FUNCS_unshift dst_sk_CRYPTO_EX_DATA_FUNCS_unshift
#define sk_CRYPTO_EX_DATA_FUNCS_value dst_sk_CRYPTO_EX_DATA_FUNCS_value
#define sk_CRYPTO_EX_DATA_FUNCS_zero dst_sk_CRYPTO_EX_DATA_FUNCS_zero
#define sk_delete dst_sk_delete
#define sk_delete_ptr dst_sk_delete_ptr
#define sk_dup dst_sk_dup
#define sk_find dst_sk_find
#define sk_free dst_sk_free
#define sk_insert dst_sk_insert
#define sk_new dst_sk_new
#define sk_num dst_sk_num
#define sk_pop dst_sk_pop
#define sk_pop_free dst_sk_pop_free
#define sk_push dst_sk_push
#define sk_set dst_sk_set
#define sk_set_cmp_func dst_sk_set_cmp_func
#define sk_shift dst_sk_shift
#define sk_sort dst_sk_sort
#define sk_unshift dst_sk_unshift
#define sk_value dst_sk_value
#define sk_zero dst_sk_zero
#endif
