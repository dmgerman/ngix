begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (C) Igor Sysoev  */
end_comment

begin_include
include|#
directive|include
file|<ngx_config.h>
end_include

begin_include
include|#
directive|include
file|<ngx_core.h>
end_include

begin_include
include|#
directive|include
file|<ngx_http.h>
end_include

begin_typedef
DECL|struct|__anon2ae86da80108
typedef|typedef
struct|struct
block|{
DECL|member|hash_max_size
name|ngx_uint_t
name|hash_max_size
decl_stmt|;
DECL|member|hash_bucket_size
name|ngx_uint_t
name|hash_bucket_size
decl_stmt|;
DECL|typedef|ngx_http_map_conf_t
block|}
name|ngx_http_map_conf_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon2ae86da80208
typedef|typedef
struct|struct
block|{
DECL|member|keys
name|ngx_hash_keys_arrays_t
name|keys
decl_stmt|;
DECL|member|values_hash
name|ngx_array_t
modifier|*
name|values_hash
decl_stmt|;
DECL|member|default_value
name|ngx_http_variable_value_t
modifier|*
name|default_value
decl_stmt|;
DECL|member|hostnames
name|ngx_uint_t
name|hostnames
decl_stmt|;
comment|/* unsigned  hostnames:1 */
DECL|typedef|ngx_http_map_conf_ctx_t
block|}
name|ngx_http_map_conf_ctx_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon2ae86da80308
typedef|typedef
struct|struct
block|{
DECL|member|hash
name|ngx_hash_combined_t
name|hash
decl_stmt|;
DECL|member|value
name|ngx_http_complex_value_t
name|value
decl_stmt|;
DECL|member|default_value
name|ngx_http_variable_value_t
modifier|*
name|default_value
decl_stmt|;
DECL|member|hostnames
name|ngx_uint_t
name|hostnames
decl_stmt|;
comment|/* unsigned  hostnames:1 */
DECL|typedef|ngx_http_map_ctx_t
block|}
name|ngx_http_map_ctx_t
typedef|;
end_typedef

begin_function_decl
specifier|static
name|int
name|ngx_libc_cdecl
name|ngx_http_map_cmp_dns_wildcards
parameter_list|(
specifier|const
name|void
modifier|*
name|one
parameter_list|,
specifier|const
name|void
modifier|*
name|two
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|void
modifier|*
name|ngx_http_map_create_conf
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|char
modifier|*
name|ngx_http_map_block
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|ngx_command_t
modifier|*
name|cmd
parameter_list|,
name|void
modifier|*
name|conf
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|char
modifier|*
name|ngx_http_map
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|ngx_command_t
modifier|*
name|dummy
parameter_list|,
name|void
modifier|*
name|conf
parameter_list|)
function_decl|;
end_function_decl

begin_decl_stmt
DECL|variable|ngx_http_map_commands
specifier|static
name|ngx_command_t
name|ngx_http_map_commands
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"map"
argument_list|)
block|,
name|NGX_HTTP_MAIN_CONF
operator||
name|NGX_CONF_BLOCK
operator||
name|NGX_CONF_TAKE2
block|,
name|ngx_http_map_block
block|,
name|NGX_HTTP_MAIN_CONF_OFFSET
block|,
literal|0
block|,
name|NULL
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"map_hash_max_size"
argument_list|)
block|,
name|NGX_HTTP_MAIN_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_conf_set_num_slot
block|,
name|NGX_HTTP_MAIN_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_http_map_conf_t
argument_list|,
name|hash_max_size
argument_list|)
block|,
name|NULL
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"map_hash_bucket_size"
argument_list|)
block|,
name|NGX_HTTP_MAIN_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_conf_set_num_slot
block|,
name|NGX_HTTP_MAIN_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_http_map_conf_t
argument_list|,
name|hash_bucket_size
argument_list|)
block|,
name|NULL
block|}
block|,
name|ngx_null_command
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_map_module_ctx
specifier|static
name|ngx_http_module_t
name|ngx_http_map_module_ctx
init|=
block|{
name|NULL
block|,
comment|/* preconfiguration */
name|NULL
block|,
comment|/* postconfiguration */
name|ngx_http_map_create_conf
block|,
comment|/* create main configuration */
name|NULL
block|,
comment|/* init main configuration */
name|NULL
block|,
comment|/* create server configuration */
name|NULL
block|,
comment|/* merge server configuration */
name|NULL
block|,
comment|/* create location configuration */
name|NULL
comment|/* merge location configuration */
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_map_module
name|ngx_module_t
name|ngx_http_map_module
init|=
block|{
name|NGX_MODULE_V1
block|,
operator|&
name|ngx_http_map_module_ctx
block|,
comment|/* module context */
name|ngx_http_map_commands
block|,
comment|/* module directives */
name|NGX_HTTP_MODULE
block|,
comment|/* module type */
name|NULL
block|,
comment|/* init master */
name|NULL
block|,
comment|/* init module */
name|NULL
block|,
comment|/* init process */
name|NULL
block|,
comment|/* init thread */
name|NULL
block|,
comment|/* exit thread */
name|NULL
block|,
comment|/* exit process */
name|NULL
block|,
comment|/* exit master */
name|NGX_MODULE_V1_PADDING
block|}
decl_stmt|;
end_decl_stmt

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_map_variable (ngx_http_request_t * r,ngx_http_variable_value_t * v,uintptr_t data)
name|ngx_http_map_variable
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_variable_value_t
modifier|*
name|v
parameter_list|,
name|uintptr_t
name|data
parameter_list|)
block|{
name|ngx_http_map_ctx_t
modifier|*
name|map
init|=
operator|(
name|ngx_http_map_ctx_t
operator|*
operator|)
name|data
decl_stmt|;
name|size_t
name|len
decl_stmt|;
name|ngx_str_t
name|val
decl_stmt|;
name|ngx_uint_t
name|key
decl_stmt|;
name|ngx_http_variable_value_t
modifier|*
name|value
decl_stmt|;
name|ngx_log_debug0
argument_list|(
name|NGX_LOG_DEBUG_HTTP
argument_list|,
name|r
operator|->
name|connection
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"http map started"
argument_list|)
expr_stmt|;
if|if
condition|(
name|ngx_http_complex_value
argument_list|(
name|r
argument_list|,
operator|&
name|map
operator|->
name|value
argument_list|,
operator|&
name|val
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|len
operator|=
name|val
operator|.
name|len
expr_stmt|;
if|if
condition|(
name|len
operator|&&
name|map
operator|->
name|hostnames
operator|&&
name|val
operator|.
name|data
index|[
name|len
operator|-
literal|1
index|]
operator|==
literal|'.'
condition|)
block|{
name|len
operator|--
expr_stmt|;
block|}
if|if
condition|(
name|len
operator|==
literal|0
condition|)
block|{
operator|*
name|v
operator|=
operator|*
name|map
operator|->
name|default_value
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
name|key
operator|=
name|ngx_hash_strlow
argument_list|(
name|val
operator|.
name|data
argument_list|,
name|val
operator|.
name|data
argument_list|,
name|len
argument_list|)
expr_stmt|;
name|value
operator|=
name|ngx_hash_find_combined
argument_list|(
operator|&
name|map
operator|->
name|hash
argument_list|,
name|key
argument_list|,
name|val
operator|.
name|data
argument_list|,
name|len
argument_list|)
expr_stmt|;
if|if
condition|(
name|value
condition|)
block|{
operator|*
name|v
operator|=
operator|*
name|value
expr_stmt|;
block|}
else|else
block|{
operator|*
name|v
operator|=
operator|*
name|map
operator|->
name|default_value
expr_stmt|;
block|}
name|ngx_log_debug2
argument_list|(
name|NGX_LOG_DEBUG_HTTP
argument_list|,
name|r
operator|->
name|connection
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"http map: \"%v\" \"%v\""
argument_list|,
operator|&
name|val
argument_list|,
name|v
argument_list|)
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
specifier|static
name|void
modifier|*
DECL|function|ngx_http_map_create_conf (ngx_conf_t * cf)
name|ngx_http_map_create_conf
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
block|{
name|ngx_http_map_conf_t
modifier|*
name|mcf
decl_stmt|;
name|mcf
operator|=
name|ngx_palloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_map_conf_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|mcf
operator|==
name|NULL
condition|)
block|{
return|return
name|NULL
return|;
block|}
name|mcf
operator|->
name|hash_max_size
operator|=
name|NGX_CONF_UNSET_UINT
expr_stmt|;
name|mcf
operator|->
name|hash_bucket_size
operator|=
name|NGX_CONF_UNSET_UINT
expr_stmt|;
return|return
name|mcf
return|;
block|}
end_function

begin_function
specifier|static
name|char
modifier|*
DECL|function|ngx_http_map_block (ngx_conf_t * cf,ngx_command_t * cmd,void * conf)
name|ngx_http_map_block
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|ngx_command_t
modifier|*
name|cmd
parameter_list|,
name|void
modifier|*
name|conf
parameter_list|)
block|{
name|ngx_http_map_conf_t
modifier|*
name|mcf
init|=
name|conf
decl_stmt|;
name|char
modifier|*
name|rv
decl_stmt|;
name|ngx_str_t
modifier|*
name|value
decl_stmt|,
name|name
decl_stmt|;
name|ngx_conf_t
name|save
decl_stmt|;
name|ngx_pool_t
modifier|*
name|pool
decl_stmt|;
name|ngx_hash_init_t
name|hash
decl_stmt|;
name|ngx_http_map_ctx_t
modifier|*
name|map
decl_stmt|;
name|ngx_http_variable_t
modifier|*
name|var
decl_stmt|;
name|ngx_http_map_conf_ctx_t
name|ctx
decl_stmt|;
name|ngx_http_compile_complex_value_t
name|ccv
decl_stmt|;
if|if
condition|(
name|mcf
operator|->
name|hash_max_size
operator|==
name|NGX_CONF_UNSET_UINT
condition|)
block|{
name|mcf
operator|->
name|hash_max_size
operator|=
literal|2048
expr_stmt|;
block|}
if|if
condition|(
name|mcf
operator|->
name|hash_bucket_size
operator|==
name|NGX_CONF_UNSET_UINT
condition|)
block|{
name|mcf
operator|->
name|hash_bucket_size
operator|=
name|ngx_cacheline_size
expr_stmt|;
block|}
else|else
block|{
name|mcf
operator|->
name|hash_bucket_size
operator|=
name|ngx_align
argument_list|(
name|mcf
operator|->
name|hash_bucket_size
argument_list|,
name|ngx_cacheline_size
argument_list|)
expr_stmt|;
block|}
name|map
operator|=
name|ngx_pcalloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_map_ctx_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|map
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|value
operator|=
name|cf
operator|->
name|args
operator|->
name|elts
expr_stmt|;
name|ngx_memzero
argument_list|(
operator|&
name|ccv
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_compile_complex_value_t
argument_list|)
argument_list|)
expr_stmt|;
name|ccv
operator|.
name|cf
operator|=
name|cf
expr_stmt|;
name|ccv
operator|.
name|value
operator|=
operator|&
name|value
index|[
literal|1
index|]
expr_stmt|;
name|ccv
operator|.
name|complex_value
operator|=
operator|&
name|map
operator|->
name|value
expr_stmt|;
if|if
condition|(
name|ngx_http_compile_complex_value
argument_list|(
operator|&
name|ccv
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|name
operator|=
name|value
index|[
literal|2
index|]
expr_stmt|;
name|name
operator|.
name|len
operator|--
expr_stmt|;
name|name
operator|.
name|data
operator|++
expr_stmt|;
name|var
operator|=
name|ngx_http_add_variable
argument_list|(
name|cf
argument_list|,
operator|&
name|name
argument_list|,
name|NGX_HTTP_VAR_CHANGEABLE
argument_list|)
expr_stmt|;
if|if
condition|(
name|var
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|var
operator|->
name|get_handler
operator|=
name|ngx_http_map_variable
expr_stmt|;
name|var
operator|->
name|data
operator|=
operator|(
name|uintptr_t
operator|)
name|map
expr_stmt|;
name|pool
operator|=
name|ngx_create_pool
argument_list|(
literal|16384
argument_list|,
name|cf
operator|->
name|log
argument_list|)
expr_stmt|;
if|if
condition|(
name|pool
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|ctx
operator|.
name|keys
operator|.
name|pool
operator|=
name|cf
operator|->
name|pool
expr_stmt|;
name|ctx
operator|.
name|keys
operator|.
name|temp_pool
operator|=
name|pool
expr_stmt|;
if|if
condition|(
name|ngx_hash_keys_array_init
argument_list|(
operator|&
name|ctx
operator|.
name|keys
argument_list|,
name|NGX_HASH_LARGE
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
name|ngx_destroy_pool
argument_list|(
name|pool
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|ctx
operator|.
name|values_hash
operator|=
name|ngx_pcalloc
argument_list|(
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_array_t
argument_list|)
operator|*
name|ctx
operator|.
name|keys
operator|.
name|hsize
argument_list|)
expr_stmt|;
if|if
condition|(
name|ctx
operator|.
name|values_hash
operator|==
name|NULL
condition|)
block|{
name|ngx_destroy_pool
argument_list|(
name|pool
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|ctx
operator|.
name|default_value
operator|=
name|NULL
expr_stmt|;
name|ctx
operator|.
name|hostnames
operator|=
literal|0
expr_stmt|;
name|save
operator|=
operator|*
name|cf
expr_stmt|;
name|cf
operator|->
name|pool
operator|=
name|pool
expr_stmt|;
name|cf
operator|->
name|ctx
operator|=
operator|&
name|ctx
expr_stmt|;
name|cf
operator|->
name|handler
operator|=
name|ngx_http_map
expr_stmt|;
name|cf
operator|->
name|handler_conf
operator|=
name|conf
expr_stmt|;
name|rv
operator|=
name|ngx_conf_parse
argument_list|(
name|cf
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
operator|*
name|cf
operator|=
name|save
expr_stmt|;
if|if
condition|(
name|rv
operator|!=
name|NGX_CONF_OK
condition|)
block|{
name|ngx_destroy_pool
argument_list|(
name|pool
argument_list|)
expr_stmt|;
return|return
name|rv
return|;
block|}
name|map
operator|->
name|default_value
operator|=
name|ctx
operator|.
name|default_value
condition|?
name|ctx
operator|.
name|default_value
else|:
operator|&
name|ngx_http_variable_null_value
expr_stmt|;
name|hash
operator|.
name|key
operator|=
name|ngx_hash_key_lc
expr_stmt|;
name|hash
operator|.
name|max_size
operator|=
name|mcf
operator|->
name|hash_max_size
expr_stmt|;
name|hash
operator|.
name|bucket_size
operator|=
name|mcf
operator|->
name|hash_bucket_size
expr_stmt|;
name|hash
operator|.
name|name
operator|=
literal|"map_hash"
expr_stmt|;
name|hash
operator|.
name|pool
operator|=
name|cf
operator|->
name|pool
expr_stmt|;
if|if
condition|(
name|ctx
operator|.
name|keys
operator|.
name|keys
operator|.
name|nelts
condition|)
block|{
name|hash
operator|.
name|hash
operator|=
operator|&
name|map
operator|->
name|hash
operator|.
name|hash
expr_stmt|;
name|hash
operator|.
name|temp_pool
operator|=
name|NULL
expr_stmt|;
if|if
condition|(
name|ngx_hash_init
argument_list|(
operator|&
name|hash
argument_list|,
name|ctx
operator|.
name|keys
operator|.
name|keys
operator|.
name|elts
argument_list|,
name|ctx
operator|.
name|keys
operator|.
name|keys
operator|.
name|nelts
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
name|ngx_destroy_pool
argument_list|(
name|pool
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
block|}
if|if
condition|(
name|ctx
operator|.
name|keys
operator|.
name|dns_wc_head
operator|.
name|nelts
condition|)
block|{
name|ngx_qsort
argument_list|(
name|ctx
operator|.
name|keys
operator|.
name|dns_wc_head
operator|.
name|elts
argument_list|,
operator|(
name|size_t
operator|)
name|ctx
operator|.
name|keys
operator|.
name|dns_wc_head
operator|.
name|nelts
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_hash_key_t
argument_list|)
argument_list|,
name|ngx_http_map_cmp_dns_wildcards
argument_list|)
expr_stmt|;
name|hash
operator|.
name|hash
operator|=
name|NULL
expr_stmt|;
name|hash
operator|.
name|temp_pool
operator|=
name|pool
expr_stmt|;
if|if
condition|(
name|ngx_hash_wildcard_init
argument_list|(
operator|&
name|hash
argument_list|,
name|ctx
operator|.
name|keys
operator|.
name|dns_wc_head
operator|.
name|elts
argument_list|,
name|ctx
operator|.
name|keys
operator|.
name|dns_wc_head
operator|.
name|nelts
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
name|ngx_destroy_pool
argument_list|(
name|pool
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|map
operator|->
name|hash
operator|.
name|wc_head
operator|=
operator|(
name|ngx_hash_wildcard_t
operator|*
operator|)
name|hash
operator|.
name|hash
expr_stmt|;
block|}
if|if
condition|(
name|ctx
operator|.
name|keys
operator|.
name|dns_wc_tail
operator|.
name|nelts
condition|)
block|{
name|ngx_qsort
argument_list|(
name|ctx
operator|.
name|keys
operator|.
name|dns_wc_tail
operator|.
name|elts
argument_list|,
operator|(
name|size_t
operator|)
name|ctx
operator|.
name|keys
operator|.
name|dns_wc_tail
operator|.
name|nelts
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_hash_key_t
argument_list|)
argument_list|,
name|ngx_http_map_cmp_dns_wildcards
argument_list|)
expr_stmt|;
name|hash
operator|.
name|hash
operator|=
name|NULL
expr_stmt|;
name|hash
operator|.
name|temp_pool
operator|=
name|pool
expr_stmt|;
if|if
condition|(
name|ngx_hash_wildcard_init
argument_list|(
operator|&
name|hash
argument_list|,
name|ctx
operator|.
name|keys
operator|.
name|dns_wc_tail
operator|.
name|elts
argument_list|,
name|ctx
operator|.
name|keys
operator|.
name|dns_wc_tail
operator|.
name|nelts
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
name|ngx_destroy_pool
argument_list|(
name|pool
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|map
operator|->
name|hash
operator|.
name|wc_tail
operator|=
operator|(
name|ngx_hash_wildcard_t
operator|*
operator|)
name|hash
operator|.
name|hash
expr_stmt|;
block|}
name|ngx_destroy_pool
argument_list|(
name|pool
argument_list|)
expr_stmt|;
return|return
name|rv
return|;
block|}
end_function

begin_function
specifier|static
name|int
name|ngx_libc_cdecl
DECL|function|ngx_http_map_cmp_dns_wildcards (const void * one,const void * two)
name|ngx_http_map_cmp_dns_wildcards
parameter_list|(
specifier|const
name|void
modifier|*
name|one
parameter_list|,
specifier|const
name|void
modifier|*
name|two
parameter_list|)
block|{
name|ngx_hash_key_t
modifier|*
name|first
decl_stmt|,
modifier|*
name|second
decl_stmt|;
name|first
operator|=
operator|(
name|ngx_hash_key_t
operator|*
operator|)
name|one
expr_stmt|;
name|second
operator|=
operator|(
name|ngx_hash_key_t
operator|*
operator|)
name|two
expr_stmt|;
return|return
name|ngx_dns_strcmp
argument_list|(
name|first
operator|->
name|key
operator|.
name|data
argument_list|,
name|second
operator|->
name|key
operator|.
name|data
argument_list|)
return|;
block|}
end_function

begin_function
specifier|static
name|char
modifier|*
DECL|function|ngx_http_map (ngx_conf_t * cf,ngx_command_t * dummy,void * conf)
name|ngx_http_map
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|ngx_command_t
modifier|*
name|dummy
parameter_list|,
name|void
modifier|*
name|conf
parameter_list|)
block|{
name|ngx_int_t
name|rc
decl_stmt|;
name|ngx_str_t
modifier|*
name|value
decl_stmt|,
name|file
decl_stmt|;
name|ngx_uint_t
name|i
decl_stmt|,
name|key
decl_stmt|;
name|ngx_http_map_conf_ctx_t
modifier|*
name|ctx
decl_stmt|;
name|ngx_http_variable_value_t
modifier|*
name|var
decl_stmt|,
modifier|*
modifier|*
name|vp
decl_stmt|;
name|ctx
operator|=
name|cf
operator|->
name|ctx
expr_stmt|;
name|value
operator|=
name|cf
operator|->
name|args
operator|->
name|elts
expr_stmt|;
if|if
condition|(
name|cf
operator|->
name|args
operator|->
name|nelts
operator|==
literal|1
operator|&&
name|ngx_strcmp
argument_list|(
name|value
index|[
literal|0
index|]
operator|.
name|data
argument_list|,
literal|"hostnames"
argument_list|)
operator|==
literal|0
condition|)
block|{
name|ctx
operator|->
name|hostnames
operator|=
literal|1
expr_stmt|;
return|return
name|NGX_CONF_OK
return|;
block|}
if|else if
condition|(
name|cf
operator|->
name|args
operator|->
name|nelts
operator|!=
literal|2
condition|)
block|{
name|ngx_conf_log_error
argument_list|(
name|NGX_LOG_EMERG
argument_list|,
name|cf
argument_list|,
literal|0
argument_list|,
literal|"invalid number of the map parameters"
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
if|else if
condition|(
name|value
index|[
literal|0
index|]
operator|.
name|len
operator|==
literal|0
condition|)
block|{
name|ngx_conf_log_error
argument_list|(
name|NGX_LOG_EMERG
argument_list|,
name|cf
argument_list|,
literal|0
argument_list|,
literal|"invalid first parameter"
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
if|if
condition|(
name|ngx_strcmp
argument_list|(
name|value
index|[
literal|0
index|]
operator|.
name|data
argument_list|,
literal|"include"
argument_list|)
operator|==
literal|0
condition|)
block|{
name|file
operator|=
name|value
index|[
literal|1
index|]
expr_stmt|;
if|if
condition|(
name|ngx_conf_full_name
argument_list|(
name|cf
operator|->
name|cycle
argument_list|,
operator|&
name|file
argument_list|,
literal|1
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_CORE
argument_list|,
name|cf
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"include %s"
argument_list|,
name|file
operator|.
name|data
argument_list|)
expr_stmt|;
return|return
name|ngx_conf_parse
argument_list|(
name|cf
argument_list|,
operator|&
name|file
argument_list|)
return|;
block|}
name|key
operator|=
literal|0
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|value
index|[
literal|1
index|]
operator|.
name|len
condition|;
name|i
operator|++
control|)
block|{
name|key
operator|=
name|ngx_hash
argument_list|(
name|key
argument_list|,
name|value
index|[
literal|1
index|]
operator|.
name|data
index|[
name|i
index|]
argument_list|)
expr_stmt|;
block|}
name|key
operator|%=
name|ctx
operator|->
name|keys
operator|.
name|hsize
expr_stmt|;
name|vp
operator|=
name|ctx
operator|->
name|values_hash
index|[
name|key
index|]
operator|.
name|elts
expr_stmt|;
if|if
condition|(
name|vp
condition|)
block|{
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|ctx
operator|->
name|values_hash
index|[
name|key
index|]
operator|.
name|nelts
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|value
index|[
literal|1
index|]
operator|.
name|len
operator|!=
operator|(
name|size_t
operator|)
name|vp
index|[
name|i
index|]
operator|->
name|len
condition|)
block|{
continue|continue;
block|}
if|if
condition|(
name|ngx_strncmp
argument_list|(
name|value
index|[
literal|1
index|]
operator|.
name|data
argument_list|,
name|vp
index|[
name|i
index|]
operator|->
name|data
argument_list|,
name|value
index|[
literal|1
index|]
operator|.
name|len
argument_list|)
operator|==
literal|0
condition|)
block|{
name|var
operator|=
name|vp
index|[
name|i
index|]
expr_stmt|;
goto|goto
name|found
goto|;
block|}
block|}
block|}
else|else
block|{
if|if
condition|(
name|ngx_array_init
argument_list|(
operator|&
name|ctx
operator|->
name|values_hash
index|[
name|key
index|]
argument_list|,
name|cf
operator|->
name|pool
argument_list|,
literal|4
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_variable_value_t
operator|*
argument_list|)
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
block|}
name|var
operator|=
name|ngx_palloc
argument_list|(
name|ctx
operator|->
name|keys
operator|.
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_variable_value_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|var
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|var
operator|->
name|len
operator|=
name|value
index|[
literal|1
index|]
operator|.
name|len
expr_stmt|;
name|var
operator|->
name|data
operator|=
name|ngx_pstrdup
argument_list|(
name|ctx
operator|->
name|keys
operator|.
name|pool
argument_list|,
operator|&
name|value
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
if|if
condition|(
name|var
operator|->
name|data
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|var
operator|->
name|valid
operator|=
literal|1
expr_stmt|;
name|var
operator|->
name|no_cacheable
operator|=
literal|0
expr_stmt|;
name|var
operator|->
name|not_found
operator|=
literal|0
expr_stmt|;
name|vp
operator|=
name|ngx_array_push
argument_list|(
operator|&
name|ctx
operator|->
name|values_hash
index|[
name|key
index|]
argument_list|)
expr_stmt|;
if|if
condition|(
name|vp
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
operator|*
name|vp
operator|=
name|var
expr_stmt|;
name|found
label|:
if|if
condition|(
name|ngx_strcmp
argument_list|(
name|value
index|[
literal|0
index|]
operator|.
name|data
argument_list|,
literal|"default"
argument_list|)
operator|==
literal|0
condition|)
block|{
if|if
condition|(
name|ctx
operator|->
name|default_value
condition|)
block|{
name|ngx_conf_log_error
argument_list|(
name|NGX_LOG_EMERG
argument_list|,
name|cf
argument_list|,
literal|0
argument_list|,
literal|"duplicate default map parameter"
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|ctx
operator|->
name|default_value
operator|=
name|var
expr_stmt|;
return|return
name|NGX_CONF_OK
return|;
block|}
if|if
condition|(
name|value
index|[
literal|0
index|]
operator|.
name|len
operator|&&
name|value
index|[
literal|0
index|]
operator|.
name|data
index|[
literal|0
index|]
operator|==
literal|'!'
condition|)
block|{
name|value
index|[
literal|0
index|]
operator|.
name|len
operator|--
expr_stmt|;
name|value
index|[
literal|0
index|]
operator|.
name|data
operator|++
expr_stmt|;
block|}
name|rc
operator|=
name|ngx_hash_add_key
argument_list|(
operator|&
name|ctx
operator|->
name|keys
argument_list|,
operator|&
name|value
index|[
literal|0
index|]
argument_list|,
name|var
argument_list|,
operator|(
name|ctx
operator|->
name|hostnames
operator|)
condition|?
name|NGX_HASH_WILDCARD_KEY
else|:
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|rc
operator|==
name|NGX_OK
condition|)
block|{
return|return
name|NGX_CONF_OK
return|;
block|}
if|if
condition|(
name|rc
operator|==
name|NGX_DECLINED
condition|)
block|{
name|ngx_conf_log_error
argument_list|(
name|NGX_LOG_EMERG
argument_list|,
name|cf
argument_list|,
literal|0
argument_list|,
literal|"invalid hostname or wildcard \"%V\""
argument_list|,
operator|&
name|value
index|[
literal|0
index|]
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|rc
operator|==
name|NGX_BUSY
condition|)
block|{
name|ngx_conf_log_error
argument_list|(
name|NGX_LOG_EMERG
argument_list|,
name|cf
argument_list|,
literal|0
argument_list|,
literal|"conflicting parameter \"%V\""
argument_list|,
operator|&
name|value
index|[
literal|0
index|]
argument_list|)
expr_stmt|;
block|}
return|return
name|NGX_CONF_ERROR
return|;
block|}
end_function

end_unit

