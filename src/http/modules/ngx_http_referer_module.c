begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (C) Igor Sysoev  * Copyright (C) Nginx, Inc.  */
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

begin_define
DECL|macro|NGX_HTTP_REFERER_NO_URI_PART
define|#
directive|define
name|NGX_HTTP_REFERER_NO_URI_PART
value|((void *) 4)
end_define

begin_typedef
DECL|struct|__anon2b1c75720108
typedef|typedef
struct|struct
block|{
DECL|member|hash
name|ngx_hash_combined_t
name|hash
decl_stmt|;
if|#
directive|if
operator|(
name|NGX_PCRE
operator|)
DECL|member|regex
name|ngx_array_t
modifier|*
name|regex
decl_stmt|;
DECL|member|server_name_regex
name|ngx_array_t
modifier|*
name|server_name_regex
decl_stmt|;
endif|#
directive|endif
DECL|member|no_referer
name|ngx_flag_t
name|no_referer
decl_stmt|;
DECL|member|blocked_referer
name|ngx_flag_t
name|blocked_referer
decl_stmt|;
DECL|member|server_names
name|ngx_flag_t
name|server_names
decl_stmt|;
DECL|member|keys
name|ngx_hash_keys_arrays_t
modifier|*
name|keys
decl_stmt|;
DECL|member|referer_hash_max_size
name|ngx_uint_t
name|referer_hash_max_size
decl_stmt|;
DECL|member|referer_hash_bucket_size
name|ngx_uint_t
name|referer_hash_bucket_size
decl_stmt|;
DECL|typedef|ngx_http_referer_conf_t
block|}
name|ngx_http_referer_conf_t
typedef|;
end_typedef

begin_function_decl
specifier|static
name|void
modifier|*
name|ngx_http_referer_create_conf
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
name|ngx_http_referer_merge_conf
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|void
modifier|*
name|parent
parameter_list|,
name|void
modifier|*
name|child
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|char
modifier|*
name|ngx_http_valid_referers
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
name|ngx_int_t
name|ngx_http_add_referer
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|ngx_hash_keys_arrays_t
modifier|*
name|keys
parameter_list|,
name|ngx_str_t
modifier|*
name|value
parameter_list|,
name|ngx_str_t
modifier|*
name|uri
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_add_regex_referer
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|ngx_http_referer_conf_t
modifier|*
name|rlcf
parameter_list|,
name|ngx_str_t
modifier|*
name|name
parameter_list|)
function_decl|;
end_function_decl

begin_if
if|#
directive|if
operator|(
name|NGX_PCRE
operator|)
end_if

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_add_regex_server_name
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|ngx_http_referer_conf_t
modifier|*
name|rlcf
parameter_list|,
name|ngx_http_regex_t
modifier|*
name|regex
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_function_decl
specifier|static
name|int
name|ngx_libc_cdecl
name|ngx_http_cmp_referer_wildcards
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

begin_decl_stmt
DECL|variable|ngx_http_referer_commands
specifier|static
name|ngx_command_t
name|ngx_http_referer_commands
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"valid_referers"
argument_list|)
block|,
name|NGX_HTTP_SRV_CONF
operator||
name|NGX_HTTP_LOC_CONF
operator||
name|NGX_CONF_1MORE
block|,
name|ngx_http_valid_referers
block|,
name|NGX_HTTP_LOC_CONF_OFFSET
block|,
literal|0
block|,
name|NULL
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"referer_hash_max_size"
argument_list|)
block|,
name|NGX_HTTP_SRV_CONF
operator||
name|NGX_HTTP_LOC_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_conf_set_num_slot
block|,
name|NGX_HTTP_LOC_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_http_referer_conf_t
argument_list|,
name|referer_hash_max_size
argument_list|)
block|,
name|NULL
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"referer_hash_bucket_size"
argument_list|)
block|,
name|NGX_HTTP_SRV_CONF
operator||
name|NGX_HTTP_LOC_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_conf_set_num_slot
block|,
name|NGX_HTTP_LOC_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_http_referer_conf_t
argument_list|,
name|referer_hash_bucket_size
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
DECL|variable|ngx_http_referer_module_ctx
specifier|static
name|ngx_http_module_t
name|ngx_http_referer_module_ctx
init|=
block|{
name|NULL
block|,
comment|/* preconfiguration */
name|NULL
block|,
comment|/* postconfiguration */
name|NULL
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
name|ngx_http_referer_create_conf
block|,
comment|/* create location configuration */
name|ngx_http_referer_merge_conf
comment|/* merge location configuration */
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_referer_module
name|ngx_module_t
name|ngx_http_referer_module
init|=
block|{
name|NGX_MODULE_V1
block|,
operator|&
name|ngx_http_referer_module_ctx
block|,
comment|/* module context */
name|ngx_http_referer_commands
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
DECL|function|ngx_http_referer_variable (ngx_http_request_t * r,ngx_http_variable_value_t * v,uintptr_t data)
name|ngx_http_referer_variable
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
name|u_char
modifier|*
name|p
decl_stmt|,
modifier|*
name|ref
decl_stmt|,
modifier|*
name|last
decl_stmt|;
name|size_t
name|len
decl_stmt|;
name|ngx_str_t
modifier|*
name|uri
decl_stmt|;
name|ngx_uint_t
name|i
decl_stmt|,
name|key
decl_stmt|;
name|ngx_http_referer_conf_t
modifier|*
name|rlcf
decl_stmt|;
name|u_char
name|buf
index|[
literal|256
index|]
decl_stmt|;
if|#
directive|if
operator|(
name|NGX_PCRE
operator|)
name|ngx_int_t
name|rc
decl_stmt|;
name|ngx_str_t
name|referer
decl_stmt|;
endif|#
directive|endif
name|rlcf
operator|=
name|ngx_http_get_module_loc_conf
argument_list|(
name|r
argument_list|,
name|ngx_http_referer_module
argument_list|)
expr_stmt|;
if|if
condition|(
name|rlcf
operator|->
name|hash
operator|.
name|hash
operator|.
name|buckets
operator|==
name|NULL
operator|&&
name|rlcf
operator|->
name|hash
operator|.
name|wc_head
operator|==
name|NULL
operator|&&
name|rlcf
operator|->
name|hash
operator|.
name|wc_tail
operator|==
name|NULL
if|#
directive|if
operator|(
name|NGX_PCRE
operator|)
operator|&&
name|rlcf
operator|->
name|regex
operator|==
name|NULL
operator|&&
name|rlcf
operator|->
name|server_name_regex
operator|==
name|NULL
endif|#
directive|endif
condition|)
block|{
goto|goto
name|valid
goto|;
block|}
if|if
condition|(
name|r
operator|->
name|headers_in
operator|.
name|referer
operator|==
name|NULL
condition|)
block|{
if|if
condition|(
name|rlcf
operator|->
name|no_referer
condition|)
block|{
goto|goto
name|valid
goto|;
block|}
goto|goto
name|invalid
goto|;
block|}
name|len
operator|=
name|r
operator|->
name|headers_in
operator|.
name|referer
operator|->
name|value
operator|.
name|len
expr_stmt|;
name|ref
operator|=
name|r
operator|->
name|headers_in
operator|.
name|referer
operator|->
name|value
operator|.
name|data
expr_stmt|;
if|if
condition|(
name|len
operator|>=
sizeof|sizeof
argument_list|(
literal|"http://i.ru"
argument_list|)
operator|-
literal|1
condition|)
block|{
name|last
operator|=
name|ref
operator|+
name|len
expr_stmt|;
if|if
condition|(
name|ngx_strncasecmp
argument_list|(
name|ref
argument_list|,
operator|(
name|u_char
operator|*
operator|)
literal|"http://"
argument_list|,
literal|7
argument_list|)
operator|==
literal|0
condition|)
block|{
name|ref
operator|+=
literal|7
expr_stmt|;
name|len
operator|-=
literal|7
expr_stmt|;
goto|goto
name|valid_scheme
goto|;
block|}
if|else if
condition|(
name|ngx_strncasecmp
argument_list|(
name|ref
argument_list|,
operator|(
name|u_char
operator|*
operator|)
literal|"https://"
argument_list|,
literal|8
argument_list|)
operator|==
literal|0
condition|)
block|{
name|ref
operator|+=
literal|8
expr_stmt|;
name|len
operator|-=
literal|8
expr_stmt|;
goto|goto
name|valid_scheme
goto|;
block|}
block|}
if|if
condition|(
name|rlcf
operator|->
name|blocked_referer
condition|)
block|{
goto|goto
name|valid
goto|;
block|}
goto|goto
name|invalid
goto|;
name|valid_scheme
label|:
name|i
operator|=
literal|0
expr_stmt|;
name|key
operator|=
literal|0
expr_stmt|;
for|for
control|(
name|p
operator|=
name|ref
init|;
name|p
operator|<
name|last
condition|;
name|p
operator|++
control|)
block|{
if|if
condition|(
operator|*
name|p
operator|==
literal|'/'
operator|||
operator|*
name|p
operator|==
literal|':'
condition|)
block|{
break|break;
block|}
if|if
condition|(
name|i
operator|==
literal|256
condition|)
block|{
goto|goto
name|invalid
goto|;
block|}
name|buf
index|[
name|i
index|]
operator|=
name|ngx_tolower
argument_list|(
operator|*
name|p
argument_list|)
expr_stmt|;
name|key
operator|=
name|ngx_hash
argument_list|(
name|key
argument_list|,
name|buf
index|[
name|i
operator|++
index|]
argument_list|)
expr_stmt|;
block|}
name|uri
operator|=
name|ngx_hash_find_combined
argument_list|(
operator|&
name|rlcf
operator|->
name|hash
argument_list|,
name|key
argument_list|,
name|buf
argument_list|,
name|p
operator|-
name|ref
argument_list|)
expr_stmt|;
if|if
condition|(
name|uri
condition|)
block|{
goto|goto
name|uri
goto|;
block|}
if|#
directive|if
operator|(
name|NGX_PCRE
operator|)
if|if
condition|(
name|rlcf
operator|->
name|server_name_regex
condition|)
block|{
name|referer
operator|.
name|len
operator|=
name|p
operator|-
name|ref
expr_stmt|;
name|referer
operator|.
name|data
operator|=
name|buf
expr_stmt|;
name|rc
operator|=
name|ngx_regex_exec_array
argument_list|(
name|rlcf
operator|->
name|server_name_regex
argument_list|,
operator|&
name|referer
argument_list|,
name|r
operator|->
name|connection
operator|->
name|log
argument_list|)
expr_stmt|;
if|if
condition|(
name|rc
operator|==
name|NGX_OK
condition|)
block|{
goto|goto
name|valid
goto|;
block|}
if|if
condition|(
name|rc
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|rc
return|;
block|}
comment|/* NGX_DECLINED */
block|}
if|if
condition|(
name|rlcf
operator|->
name|regex
condition|)
block|{
name|referer
operator|.
name|len
operator|=
name|len
expr_stmt|;
name|referer
operator|.
name|data
operator|=
name|ref
expr_stmt|;
name|rc
operator|=
name|ngx_regex_exec_array
argument_list|(
name|rlcf
operator|->
name|regex
argument_list|,
operator|&
name|referer
argument_list|,
name|r
operator|->
name|connection
operator|->
name|log
argument_list|)
expr_stmt|;
if|if
condition|(
name|rc
operator|==
name|NGX_OK
condition|)
block|{
goto|goto
name|valid
goto|;
block|}
if|if
condition|(
name|rc
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|rc
return|;
block|}
comment|/* NGX_DECLINED */
block|}
endif|#
directive|endif
name|invalid
label|:
operator|*
name|v
operator|=
name|ngx_http_variable_true_value
expr_stmt|;
return|return
name|NGX_OK
return|;
name|uri
label|:
for|for
control|(
comment|/* void */
init|;
name|p
operator|<
name|last
condition|;
name|p
operator|++
control|)
block|{
if|if
condition|(
operator|*
name|p
operator|==
literal|'/'
condition|)
block|{
break|break;
block|}
block|}
name|len
operator|=
name|last
operator|-
name|p
expr_stmt|;
if|if
condition|(
name|uri
operator|==
name|NGX_HTTP_REFERER_NO_URI_PART
condition|)
block|{
goto|goto
name|valid
goto|;
block|}
if|if
condition|(
name|len
operator|<
name|uri
operator|->
name|len
operator|||
name|ngx_strncmp
argument_list|(
name|uri
operator|->
name|data
argument_list|,
name|p
argument_list|,
name|uri
operator|->
name|len
argument_list|)
operator|!=
literal|0
condition|)
block|{
goto|goto
name|invalid
goto|;
block|}
name|valid
label|:
operator|*
name|v
operator|=
name|ngx_http_variable_null_value
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
DECL|function|ngx_http_referer_create_conf (ngx_conf_t * cf)
name|ngx_http_referer_create_conf
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
block|{
name|ngx_http_referer_conf_t
modifier|*
name|conf
decl_stmt|;
name|conf
operator|=
name|ngx_pcalloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_referer_conf_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|conf
operator|==
name|NULL
condition|)
block|{
return|return
name|NULL
return|;
block|}
comment|/*      * set by ngx_pcalloc():      *      *     conf->hash = { NULL };      *     conf->server_names = 0;      *     conf->keys = NULL;      */
if|#
directive|if
operator|(
name|NGX_PCRE
operator|)
name|conf
operator|->
name|regex
operator|=
name|NGX_CONF_UNSET_PTR
expr_stmt|;
name|conf
operator|->
name|server_name_regex
operator|=
name|NGX_CONF_UNSET_PTR
expr_stmt|;
endif|#
directive|endif
name|conf
operator|->
name|no_referer
operator|=
name|NGX_CONF_UNSET
expr_stmt|;
name|conf
operator|->
name|blocked_referer
operator|=
name|NGX_CONF_UNSET
expr_stmt|;
name|conf
operator|->
name|referer_hash_max_size
operator|=
name|NGX_CONF_UNSET_UINT
expr_stmt|;
name|conf
operator|->
name|referer_hash_bucket_size
operator|=
name|NGX_CONF_UNSET_UINT
expr_stmt|;
return|return
name|conf
return|;
block|}
end_function

begin_function
specifier|static
name|char
modifier|*
DECL|function|ngx_http_referer_merge_conf (ngx_conf_t * cf,void * parent,void * child)
name|ngx_http_referer_merge_conf
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|void
modifier|*
name|parent
parameter_list|,
name|void
modifier|*
name|child
parameter_list|)
block|{
name|ngx_http_referer_conf_t
modifier|*
name|prev
init|=
name|parent
decl_stmt|;
name|ngx_http_referer_conf_t
modifier|*
name|conf
init|=
name|child
decl_stmt|;
name|ngx_uint_t
name|n
decl_stmt|;
name|ngx_hash_init_t
name|hash
decl_stmt|;
name|ngx_http_server_name_t
modifier|*
name|sn
decl_stmt|;
name|ngx_http_core_srv_conf_t
modifier|*
name|cscf
decl_stmt|;
if|if
condition|(
name|conf
operator|->
name|keys
operator|==
name|NULL
condition|)
block|{
name|conf
operator|->
name|hash
operator|=
name|prev
operator|->
name|hash
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_PCRE
operator|)
name|ngx_conf_merge_ptr_value
argument_list|(
name|conf
operator|->
name|regex
argument_list|,
name|prev
operator|->
name|regex
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|ngx_conf_merge_ptr_value
argument_list|(
name|conf
operator|->
name|server_name_regex
argument_list|,
name|prev
operator|->
name|server_name_regex
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
endif|#
directive|endif
name|ngx_conf_merge_value
argument_list|(
name|conf
operator|->
name|no_referer
argument_list|,
name|prev
operator|->
name|no_referer
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|ngx_conf_merge_value
argument_list|(
name|conf
operator|->
name|blocked_referer
argument_list|,
name|prev
operator|->
name|blocked_referer
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|ngx_conf_merge_uint_value
argument_list|(
name|conf
operator|->
name|referer_hash_max_size
argument_list|,
name|prev
operator|->
name|referer_hash_max_size
argument_list|,
literal|2048
argument_list|)
expr_stmt|;
name|ngx_conf_merge_uint_value
argument_list|(
name|conf
operator|->
name|referer_hash_bucket_size
argument_list|,
name|prev
operator|->
name|referer_hash_bucket_size
argument_list|,
literal|64
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_OK
return|;
block|}
if|if
condition|(
name|conf
operator|->
name|server_names
operator|==
literal|1
condition|)
block|{
name|cscf
operator|=
name|ngx_http_conf_get_module_srv_conf
argument_list|(
name|cf
argument_list|,
name|ngx_http_core_module
argument_list|)
expr_stmt|;
name|sn
operator|=
name|cscf
operator|->
name|server_names
operator|.
name|elts
expr_stmt|;
for|for
control|(
name|n
operator|=
literal|0
init|;
name|n
operator|<
name|cscf
operator|->
name|server_names
operator|.
name|nelts
condition|;
name|n
operator|++
control|)
block|{
if|#
directive|if
operator|(
name|NGX_PCRE
operator|)
if|if
condition|(
name|sn
index|[
name|n
index|]
operator|.
name|regex
condition|)
block|{
if|if
condition|(
name|ngx_http_add_regex_server_name
argument_list|(
name|cf
argument_list|,
name|conf
argument_list|,
name|sn
index|[
name|n
index|]
operator|.
name|regex
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
continue|continue;
block|}
endif|#
directive|endif
if|if
condition|(
name|ngx_http_add_referer
argument_list|(
name|cf
argument_list|,
name|conf
operator|->
name|keys
argument_list|,
operator|&
name|sn
index|[
name|n
index|]
operator|.
name|name
argument_list|,
name|NULL
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
block|}
if|if
condition|(
operator|(
name|conf
operator|->
name|no_referer
operator|==
literal|1
operator|||
name|conf
operator|->
name|blocked_referer
operator|==
literal|1
operator|)
operator|&&
name|conf
operator|->
name|keys
operator|->
name|keys
operator|.
name|nelts
operator|==
literal|0
operator|&&
name|conf
operator|->
name|keys
operator|->
name|dns_wc_head
operator|.
name|nelts
operator|==
literal|0
operator|&&
name|conf
operator|->
name|keys
operator|->
name|dns_wc_tail
operator|.
name|nelts
operator|==
literal|0
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_EMERG
argument_list|,
name|cf
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"the \"none\" or \"blocked\" referers are specified "
literal|"in the \"valid_referers\" directive "
literal|"without any valid referer"
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|ngx_conf_merge_uint_value
argument_list|(
name|conf
operator|->
name|referer_hash_max_size
argument_list|,
name|prev
operator|->
name|referer_hash_max_size
argument_list|,
literal|2048
argument_list|)
expr_stmt|;
name|ngx_conf_merge_uint_value
argument_list|(
name|conf
operator|->
name|referer_hash_bucket_size
argument_list|,
name|prev
operator|->
name|referer_hash_bucket_size
argument_list|,
literal|64
argument_list|)
expr_stmt|;
name|conf
operator|->
name|referer_hash_bucket_size
operator|=
name|ngx_align
argument_list|(
name|conf
operator|->
name|referer_hash_bucket_size
argument_list|,
name|ngx_cacheline_size
argument_list|)
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
name|conf
operator|->
name|referer_hash_max_size
expr_stmt|;
name|hash
operator|.
name|bucket_size
operator|=
name|conf
operator|->
name|referer_hash_bucket_size
expr_stmt|;
name|hash
operator|.
name|name
operator|=
literal|"referer_hash"
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
name|conf
operator|->
name|keys
operator|->
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
name|conf
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
name|conf
operator|->
name|keys
operator|->
name|keys
operator|.
name|elts
argument_list|,
name|conf
operator|->
name|keys
operator|->
name|keys
operator|.
name|nelts
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
if|if
condition|(
name|conf
operator|->
name|keys
operator|->
name|dns_wc_head
operator|.
name|nelts
condition|)
block|{
name|ngx_qsort
argument_list|(
name|conf
operator|->
name|keys
operator|->
name|dns_wc_head
operator|.
name|elts
argument_list|,
operator|(
name|size_t
operator|)
name|conf
operator|->
name|keys
operator|->
name|dns_wc_head
operator|.
name|nelts
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_hash_key_t
argument_list|)
argument_list|,
name|ngx_http_cmp_referer_wildcards
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
name|cf
operator|->
name|temp_pool
expr_stmt|;
if|if
condition|(
name|ngx_hash_wildcard_init
argument_list|(
operator|&
name|hash
argument_list|,
name|conf
operator|->
name|keys
operator|->
name|dns_wc_head
operator|.
name|elts
argument_list|,
name|conf
operator|->
name|keys
operator|->
name|dns_wc_head
operator|.
name|nelts
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|conf
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
name|conf
operator|->
name|keys
operator|->
name|dns_wc_tail
operator|.
name|nelts
condition|)
block|{
name|ngx_qsort
argument_list|(
name|conf
operator|->
name|keys
operator|->
name|dns_wc_tail
operator|.
name|elts
argument_list|,
operator|(
name|size_t
operator|)
name|conf
operator|->
name|keys
operator|->
name|dns_wc_tail
operator|.
name|nelts
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_hash_key_t
argument_list|)
argument_list|,
name|ngx_http_cmp_referer_wildcards
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
name|cf
operator|->
name|temp_pool
expr_stmt|;
if|if
condition|(
name|ngx_hash_wildcard_init
argument_list|(
operator|&
name|hash
argument_list|,
name|conf
operator|->
name|keys
operator|->
name|dns_wc_tail
operator|.
name|elts
argument_list|,
name|conf
operator|->
name|keys
operator|->
name|dns_wc_tail
operator|.
name|nelts
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|conf
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
if|#
directive|if
operator|(
name|NGX_PCRE
operator|)
name|ngx_conf_merge_ptr_value
argument_list|(
name|conf
operator|->
name|regex
argument_list|,
name|prev
operator|->
name|regex
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|ngx_conf_merge_ptr_value
argument_list|(
name|conf
operator|->
name|server_name_regex
argument_list|,
name|prev
operator|->
name|server_name_regex
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
endif|#
directive|endif
if|if
condition|(
name|conf
operator|->
name|no_referer
operator|==
name|NGX_CONF_UNSET
condition|)
block|{
name|conf
operator|->
name|no_referer
operator|=
literal|0
expr_stmt|;
block|}
if|if
condition|(
name|conf
operator|->
name|blocked_referer
operator|==
name|NGX_CONF_UNSET
condition|)
block|{
name|conf
operator|->
name|blocked_referer
operator|=
literal|0
expr_stmt|;
block|}
name|conf
operator|->
name|keys
operator|=
name|NULL
expr_stmt|;
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

begin_function
specifier|static
name|char
modifier|*
DECL|function|ngx_http_valid_referers (ngx_conf_t * cf,ngx_command_t * cmd,void * conf)
name|ngx_http_valid_referers
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
name|ngx_http_referer_conf_t
modifier|*
name|rlcf
init|=
name|conf
decl_stmt|;
name|u_char
modifier|*
name|p
decl_stmt|;
name|ngx_str_t
modifier|*
name|value
decl_stmt|,
name|uri
decl_stmt|,
name|name
decl_stmt|;
name|ngx_uint_t
name|i
decl_stmt|;
name|ngx_http_variable_t
modifier|*
name|var
decl_stmt|;
name|ngx_str_set
argument_list|(
operator|&
name|name
argument_list|,
literal|"invalid_referer"
argument_list|)
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
name|ngx_http_referer_variable
expr_stmt|;
if|if
condition|(
name|rlcf
operator|->
name|keys
operator|==
name|NULL
condition|)
block|{
name|rlcf
operator|->
name|keys
operator|=
name|ngx_pcalloc
argument_list|(
name|cf
operator|->
name|temp_pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_hash_keys_arrays_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|rlcf
operator|->
name|keys
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|rlcf
operator|->
name|keys
operator|->
name|pool
operator|=
name|cf
operator|->
name|pool
expr_stmt|;
name|rlcf
operator|->
name|keys
operator|->
name|temp_pool
operator|=
name|cf
operator|->
name|pool
expr_stmt|;
if|if
condition|(
name|ngx_hash_keys_array_init
argument_list|(
name|rlcf
operator|->
name|keys
argument_list|,
name|NGX_HASH_SMALL
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
name|value
operator|=
name|cf
operator|->
name|args
operator|->
name|elts
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|1
init|;
name|i
operator|<
name|cf
operator|->
name|args
operator|->
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
name|i
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
literal|"invalid referer \"%V\""
argument_list|,
operator|&
name|value
index|[
name|i
index|]
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
name|i
index|]
operator|.
name|data
argument_list|,
literal|"none"
argument_list|)
operator|==
literal|0
condition|)
block|{
name|rlcf
operator|->
name|no_referer
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|ngx_strcmp
argument_list|(
name|value
index|[
name|i
index|]
operator|.
name|data
argument_list|,
literal|"blocked"
argument_list|)
operator|==
literal|0
condition|)
block|{
name|rlcf
operator|->
name|blocked_referer
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|ngx_strcmp
argument_list|(
name|value
index|[
name|i
index|]
operator|.
name|data
argument_list|,
literal|"server_names"
argument_list|)
operator|==
literal|0
condition|)
block|{
name|rlcf
operator|->
name|server_names
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|value
index|[
name|i
index|]
operator|.
name|data
index|[
literal|0
index|]
operator|==
literal|'~'
condition|)
block|{
if|if
condition|(
name|ngx_http_add_regex_referer
argument_list|(
name|cf
argument_list|,
name|rlcf
argument_list|,
operator|&
name|value
index|[
name|i
index|]
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
continue|continue;
block|}
name|ngx_str_null
argument_list|(
operator|&
name|uri
argument_list|)
expr_stmt|;
name|p
operator|=
operator|(
name|u_char
operator|*
operator|)
name|ngx_strchr
argument_list|(
name|value
index|[
name|i
index|]
operator|.
name|data
argument_list|,
literal|'/'
argument_list|)
expr_stmt|;
if|if
condition|(
name|p
condition|)
block|{
name|uri
operator|.
name|len
operator|=
operator|(
name|value
index|[
name|i
index|]
operator|.
name|data
operator|+
name|value
index|[
name|i
index|]
operator|.
name|len
operator|)
operator|-
name|p
expr_stmt|;
name|uri
operator|.
name|data
operator|=
name|p
expr_stmt|;
name|value
index|[
name|i
index|]
operator|.
name|len
operator|=
name|p
operator|-
name|value
index|[
name|i
index|]
operator|.
name|data
expr_stmt|;
block|}
if|if
condition|(
name|ngx_http_add_referer
argument_list|(
name|cf
argument_list|,
name|rlcf
operator|->
name|keys
argument_list|,
operator|&
name|value
index|[
name|i
index|]
argument_list|,
operator|&
name|uri
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
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_add_referer (ngx_conf_t * cf,ngx_hash_keys_arrays_t * keys,ngx_str_t * value,ngx_str_t * uri)
name|ngx_http_add_referer
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|ngx_hash_keys_arrays_t
modifier|*
name|keys
parameter_list|,
name|ngx_str_t
modifier|*
name|value
parameter_list|,
name|ngx_str_t
modifier|*
name|uri
parameter_list|)
block|{
name|ngx_int_t
name|rc
decl_stmt|;
name|ngx_str_t
modifier|*
name|u
decl_stmt|;
if|if
condition|(
name|uri
operator|==
name|NULL
operator|||
name|uri
operator|->
name|len
operator|==
literal|0
condition|)
block|{
name|u
operator|=
name|NGX_HTTP_REFERER_NO_URI_PART
expr_stmt|;
block|}
else|else
block|{
name|u
operator|=
name|ngx_palloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_str_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|u
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
operator|*
name|u
operator|=
operator|*
name|uri
expr_stmt|;
block|}
name|rc
operator|=
name|ngx_hash_add_key
argument_list|(
name|keys
argument_list|,
name|value
argument_list|,
name|u
argument_list|,
name|NGX_HASH_WILDCARD_KEY
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
name|NGX_OK
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
name|value
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
name|value
argument_list|)
expr_stmt|;
block|}
return|return
name|NGX_ERROR
return|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_add_regex_referer (ngx_conf_t * cf,ngx_http_referer_conf_t * rlcf,ngx_str_t * name)
name|ngx_http_add_regex_referer
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|ngx_http_referer_conf_t
modifier|*
name|rlcf
parameter_list|,
name|ngx_str_t
modifier|*
name|name
parameter_list|)
block|{
if|#
directive|if
operator|(
name|NGX_PCRE
operator|)
name|ngx_regex_elt_t
modifier|*
name|re
decl_stmt|;
name|ngx_regex_compile_t
name|rc
decl_stmt|;
name|u_char
name|errstr
index|[
name|NGX_MAX_CONF_ERRSTR
index|]
decl_stmt|;
if|if
condition|(
name|name
operator|->
name|len
operator|==
literal|1
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
literal|"empty regex in \"%V\""
argument_list|,
name|name
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|rlcf
operator|->
name|regex
operator|==
name|NGX_CONF_UNSET_PTR
condition|)
block|{
name|rlcf
operator|->
name|regex
operator|=
name|ngx_array_create
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
literal|2
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_regex_elt_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|rlcf
operator|->
name|regex
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
block|}
name|re
operator|=
name|ngx_array_push
argument_list|(
name|rlcf
operator|->
name|regex
argument_list|)
expr_stmt|;
if|if
condition|(
name|re
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|name
operator|->
name|len
operator|--
expr_stmt|;
name|name
operator|->
name|data
operator|++
expr_stmt|;
name|ngx_memzero
argument_list|(
operator|&
name|rc
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_regex_compile_t
argument_list|)
argument_list|)
expr_stmt|;
name|rc
operator|.
name|pattern
operator|=
operator|*
name|name
expr_stmt|;
name|rc
operator|.
name|pool
operator|=
name|cf
operator|->
name|pool
expr_stmt|;
name|rc
operator|.
name|options
operator|=
name|NGX_REGEX_CASELESS
expr_stmt|;
name|rc
operator|.
name|err
operator|.
name|len
operator|=
name|NGX_MAX_CONF_ERRSTR
expr_stmt|;
name|rc
operator|.
name|err
operator|.
name|data
operator|=
name|errstr
expr_stmt|;
if|if
condition|(
name|ngx_regex_compile
argument_list|(
operator|&
name|rc
argument_list|)
operator|!=
name|NGX_OK
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
literal|"%V"
argument_list|,
operator|&
name|rc
operator|.
name|err
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
name|re
operator|->
name|regex
operator|=
name|rc
operator|.
name|regex
expr_stmt|;
name|re
operator|->
name|name
operator|=
name|name
operator|->
name|data
expr_stmt|;
return|return
name|NGX_OK
return|;
else|#
directive|else
name|ngx_conf_log_error
argument_list|(
name|NGX_LOG_EMERG
argument_list|,
name|cf
argument_list|,
literal|0
argument_list|,
literal|"the using of the regex \"%V\" requires PCRE library"
argument_list|,
name|name
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
endif|#
directive|endif
block|}
end_function

begin_if
if|#
directive|if
operator|(
name|NGX_PCRE
operator|)
end_if

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_add_regex_server_name (ngx_conf_t * cf,ngx_http_referer_conf_t * rlcf,ngx_http_regex_t * regex)
name|ngx_http_add_regex_server_name
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|ngx_http_referer_conf_t
modifier|*
name|rlcf
parameter_list|,
name|ngx_http_regex_t
modifier|*
name|regex
parameter_list|)
block|{
name|ngx_regex_elt_t
modifier|*
name|re
decl_stmt|;
if|if
condition|(
name|rlcf
operator|->
name|server_name_regex
operator|==
name|NGX_CONF_UNSET_PTR
condition|)
block|{
name|rlcf
operator|->
name|server_name_regex
operator|=
name|ngx_array_create
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
literal|2
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_regex_elt_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|rlcf
operator|->
name|server_name_regex
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
block|}
name|re
operator|=
name|ngx_array_push
argument_list|(
name|rlcf
operator|->
name|server_name_regex
argument_list|)
expr_stmt|;
if|if
condition|(
name|re
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|re
operator|->
name|regex
operator|=
name|regex
operator|->
name|regex
expr_stmt|;
name|re
operator|->
name|name
operator|=
name|regex
operator|->
name|name
operator|.
name|data
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_endif
endif|#
directive|endif
end_endif

begin_function
specifier|static
name|int
name|ngx_libc_cdecl
DECL|function|ngx_http_cmp_referer_wildcards (const void * one,const void * two)
name|ngx_http_cmp_referer_wildcards
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

end_unit

