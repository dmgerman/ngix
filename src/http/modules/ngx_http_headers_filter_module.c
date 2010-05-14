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
DECL|typedef|ngx_http_header_val_t
typedef|typedef
name|struct
name|ngx_http_header_val_s
name|ngx_http_header_val_t
typedef|;
end_typedef

begin_typedef
DECL|typedef|ngx_http_set_header_pt
typedef|typedef
name|ngx_int_t
function_decl|(
modifier|*
name|ngx_http_set_header_pt
function_decl|)
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_header_val_t
modifier|*
name|hv
parameter_list|,
name|ngx_str_t
modifier|*
name|value
parameter_list|)
function_decl|;
end_typedef

begin_typedef
DECL|struct|__anon28d8d5100108
typedef|typedef
struct|struct
block|{
DECL|member|name
name|ngx_str_t
name|name
decl_stmt|;
DECL|member|offset
name|ngx_uint_t
name|offset
decl_stmt|;
DECL|member|handler
name|ngx_http_set_header_pt
name|handler
decl_stmt|;
DECL|typedef|ngx_http_set_header_t
block|}
name|ngx_http_set_header_t
typedef|;
end_typedef

begin_struct
DECL|struct|ngx_http_header_val_s
struct|struct
name|ngx_http_header_val_s
block|{
DECL|member|value
name|ngx_http_complex_value_t
name|value
decl_stmt|;
DECL|member|hash
name|ngx_uint_t
name|hash
decl_stmt|;
DECL|member|key
name|ngx_str_t
name|key
decl_stmt|;
DECL|member|handler
name|ngx_http_set_header_pt
name|handler
decl_stmt|;
DECL|member|offset
name|ngx_uint_t
name|offset
decl_stmt|;
block|}
struct|;
end_struct

begin_define
DECL|macro|NGX_HTTP_EXPIRES_OFF
define|#
directive|define
name|NGX_HTTP_EXPIRES_OFF
value|0
end_define

begin_define
DECL|macro|NGX_HTTP_EXPIRES_EPOCH
define|#
directive|define
name|NGX_HTTP_EXPIRES_EPOCH
value|1
end_define

begin_define
DECL|macro|NGX_HTTP_EXPIRES_MAX
define|#
directive|define
name|NGX_HTTP_EXPIRES_MAX
value|2
end_define

begin_define
DECL|macro|NGX_HTTP_EXPIRES_ACCESS
define|#
directive|define
name|NGX_HTTP_EXPIRES_ACCESS
value|3
end_define

begin_define
DECL|macro|NGX_HTTP_EXPIRES_MODIFIED
define|#
directive|define
name|NGX_HTTP_EXPIRES_MODIFIED
value|4
end_define

begin_define
DECL|macro|NGX_HTTP_EXPIRES_DAILY
define|#
directive|define
name|NGX_HTTP_EXPIRES_DAILY
value|5
end_define

begin_typedef
DECL|struct|__anon28d8d5100208
typedef|typedef
struct|struct
block|{
DECL|member|expires
name|ngx_uint_t
name|expires
decl_stmt|;
DECL|member|expires_time
name|time_t
name|expires_time
decl_stmt|;
DECL|member|headers
name|ngx_array_t
modifier|*
name|headers
decl_stmt|;
DECL|typedef|ngx_http_headers_conf_t
block|}
name|ngx_http_headers_conf_t
typedef|;
end_typedef

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_set_expires
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_headers_conf_t
modifier|*
name|conf
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_add_cache_control
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_header_val_t
modifier|*
name|hv
parameter_list|,
name|ngx_str_t
modifier|*
name|value
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_set_last_modified
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_header_val_t
modifier|*
name|hv
parameter_list|,
name|ngx_str_t
modifier|*
name|value
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|void
modifier|*
name|ngx_http_headers_create_conf
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
name|ngx_http_headers_merge_conf
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
name|ngx_int_t
name|ngx_http_headers_filter_init
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
name|ngx_http_headers_expires
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
name|ngx_http_headers_add
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

begin_decl_stmt
DECL|variable|ngx_http_set_headers
specifier|static
name|ngx_http_set_header_t
name|ngx_http_set_headers
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"Cache-Control"
argument_list|)
block|,
literal|0
block|,
name|ngx_http_add_cache_control
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"Last-Modified"
argument_list|)
block|,
name|offsetof
argument_list|(
name|ngx_http_headers_out_t
argument_list|,
name|last_modified
argument_list|)
block|,
name|ngx_http_set_last_modified
block|}
block|,
block|{
name|ngx_null_string
block|,
literal|0
block|,
name|NULL
block|}
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_headers_filter_commands
specifier|static
name|ngx_command_t
name|ngx_http_headers_filter_commands
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"expires"
argument_list|)
block|,
name|NGX_HTTP_MAIN_CONF
operator||
name|NGX_HTTP_SRV_CONF
operator||
name|NGX_HTTP_LOC_CONF
operator||
name|NGX_HTTP_LIF_CONF
operator||
name|NGX_CONF_TAKE12
block|,
name|ngx_http_headers_expires
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
literal|"add_header"
argument_list|)
block|,
name|NGX_HTTP_MAIN_CONF
operator||
name|NGX_HTTP_SRV_CONF
operator||
name|NGX_HTTP_LOC_CONF
operator||
name|NGX_HTTP_LIF_CONF
operator||
name|NGX_CONF_TAKE2
block|,
name|ngx_http_headers_add
block|,
name|NGX_HTTP_LOC_CONF_OFFSET
block|,
literal|0
block|,
name|NULL
block|}
block|,
name|ngx_null_command
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_headers_filter_module_ctx
specifier|static
name|ngx_http_module_t
name|ngx_http_headers_filter_module_ctx
init|=
block|{
name|NULL
block|,
comment|/* preconfiguration */
name|ngx_http_headers_filter_init
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
name|ngx_http_headers_create_conf
block|,
comment|/* create location configuration */
name|ngx_http_headers_merge_conf
comment|/* merge location configuration */
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_headers_filter_module
name|ngx_module_t
name|ngx_http_headers_filter_module
init|=
block|{
name|NGX_MODULE_V1
block|,
operator|&
name|ngx_http_headers_filter_module_ctx
block|,
comment|/* module context */
name|ngx_http_headers_filter_commands
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

begin_decl_stmt
DECL|variable|ngx_http_next_header_filter
specifier|static
name|ngx_http_output_header_filter_pt
name|ngx_http_next_header_filter
decl_stmt|;
end_decl_stmt

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_headers_filter (ngx_http_request_t * r)
name|ngx_http_headers_filter
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
block|{
name|ngx_str_t
name|value
decl_stmt|;
name|ngx_uint_t
name|i
decl_stmt|;
name|ngx_http_header_val_t
modifier|*
name|h
decl_stmt|;
name|ngx_http_headers_conf_t
modifier|*
name|conf
decl_stmt|;
name|conf
operator|=
name|ngx_http_get_module_loc_conf
argument_list|(
name|r
argument_list|,
name|ngx_http_headers_filter_module
argument_list|)
expr_stmt|;
if|if
condition|(
operator|(
name|conf
operator|->
name|expires
operator|==
name|NGX_HTTP_EXPIRES_OFF
operator|&&
name|conf
operator|->
name|headers
operator|==
name|NULL
operator|)
operator|||
name|r
operator|!=
name|r
operator|->
expr|main
operator|||
operator|(
name|r
operator|->
name|headers_out
operator|.
name|status
operator|!=
name|NGX_HTTP_OK
operator|&&
name|r
operator|->
name|headers_out
operator|.
name|status
operator|!=
name|NGX_HTTP_NO_CONTENT
operator|&&
name|r
operator|->
name|headers_out
operator|.
name|status
operator|!=
name|NGX_HTTP_MOVED_PERMANENTLY
operator|&&
name|r
operator|->
name|headers_out
operator|.
name|status
operator|!=
name|NGX_HTTP_MOVED_TEMPORARILY
operator|&&
name|r
operator|->
name|headers_out
operator|.
name|status
operator|!=
name|NGX_HTTP_NOT_MODIFIED
operator|)
condition|)
block|{
return|return
name|ngx_http_next_header_filter
argument_list|(
name|r
argument_list|)
return|;
block|}
if|if
condition|(
name|conf
operator|->
name|expires
operator|!=
name|NGX_HTTP_EXPIRES_OFF
condition|)
block|{
if|if
condition|(
name|ngx_http_set_expires
argument_list|(
name|r
argument_list|,
name|conf
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
block|}
if|if
condition|(
name|conf
operator|->
name|headers
condition|)
block|{
name|h
operator|=
name|conf
operator|->
name|headers
operator|->
name|elts
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|conf
operator|->
name|headers
operator|->
name|nelts
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|ngx_http_complex_value
argument_list|(
name|r
argument_list|,
operator|&
name|h
index|[
name|i
index|]
operator|.
name|value
argument_list|,
operator|&
name|value
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|h
index|[
name|i
index|]
operator|.
name|handler
argument_list|(
name|r
argument_list|,
operator|&
name|h
index|[
name|i
index|]
argument_list|,
operator|&
name|value
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
block|}
block|}
return|return
name|ngx_http_next_header_filter
argument_list|(
name|r
argument_list|)
return|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_set_expires (ngx_http_request_t * r,ngx_http_headers_conf_t * conf)
name|ngx_http_set_expires
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_headers_conf_t
modifier|*
name|conf
parameter_list|)
block|{
name|size_t
name|len
decl_stmt|;
name|time_t
name|now
decl_stmt|,
name|expires_time
decl_stmt|,
name|max_age
decl_stmt|;
name|ngx_uint_t
name|i
decl_stmt|;
name|ngx_table_elt_t
modifier|*
name|expires
decl_stmt|,
modifier|*
name|cc
decl_stmt|,
modifier|*
modifier|*
name|ccp
decl_stmt|;
name|expires
operator|=
name|r
operator|->
name|headers_out
operator|.
name|expires
expr_stmt|;
if|if
condition|(
name|expires
operator|==
name|NULL
condition|)
block|{
name|expires
operator|=
name|ngx_list_push
argument_list|(
operator|&
name|r
operator|->
name|headers_out
operator|.
name|headers
argument_list|)
expr_stmt|;
if|if
condition|(
name|expires
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|r
operator|->
name|headers_out
operator|.
name|expires
operator|=
name|expires
expr_stmt|;
name|expires
operator|->
name|hash
operator|=
literal|1
expr_stmt|;
name|ngx_str_set
argument_list|(
operator|&
name|expires
operator|->
name|key
argument_list|,
literal|"Expires"
argument_list|)
expr_stmt|;
block|}
name|len
operator|=
sizeof|sizeof
argument_list|(
literal|"Mon, 28 Sep 1970 06:00:00 GMT"
argument_list|)
expr_stmt|;
name|expires
operator|->
name|value
operator|.
name|len
operator|=
name|len
operator|-
literal|1
expr_stmt|;
name|ccp
operator|=
name|r
operator|->
name|headers_out
operator|.
name|cache_control
operator|.
name|elts
expr_stmt|;
if|if
condition|(
name|ccp
operator|==
name|NULL
condition|)
block|{
if|if
condition|(
name|ngx_array_init
argument_list|(
operator|&
name|r
operator|->
name|headers_out
operator|.
name|cache_control
argument_list|,
name|r
operator|->
name|pool
argument_list|,
literal|1
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_table_elt_t
operator|*
argument_list|)
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|ccp
operator|=
name|ngx_array_push
argument_list|(
operator|&
name|r
operator|->
name|headers_out
operator|.
name|cache_control
argument_list|)
expr_stmt|;
if|if
condition|(
name|ccp
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|cc
operator|=
name|ngx_list_push
argument_list|(
operator|&
name|r
operator|->
name|headers_out
operator|.
name|headers
argument_list|)
expr_stmt|;
if|if
condition|(
name|cc
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|cc
operator|->
name|hash
operator|=
literal|1
expr_stmt|;
name|ngx_str_set
argument_list|(
operator|&
name|cc
operator|->
name|key
argument_list|,
literal|"Cache-Control"
argument_list|)
expr_stmt|;
operator|*
name|ccp
operator|=
name|cc
expr_stmt|;
block|}
else|else
block|{
for|for
control|(
name|i
operator|=
literal|1
init|;
name|i
operator|<
name|r
operator|->
name|headers_out
operator|.
name|cache_control
operator|.
name|nelts
condition|;
name|i
operator|++
control|)
block|{
name|ccp
index|[
name|i
index|]
operator|->
name|hash
operator|=
literal|0
expr_stmt|;
block|}
name|cc
operator|=
name|ccp
index|[
literal|0
index|]
expr_stmt|;
block|}
if|if
condition|(
name|conf
operator|->
name|expires
operator|==
name|NGX_HTTP_EXPIRES_EPOCH
condition|)
block|{
name|expires
operator|->
name|value
operator|.
name|data
operator|=
operator|(
name|u_char
operator|*
operator|)
literal|"Thu, 01 Jan 1970 00:00:01 GMT"
expr_stmt|;
name|ngx_str_set
argument_list|(
operator|&
name|cc
operator|->
name|value
argument_list|,
literal|"no-cache"
argument_list|)
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
if|if
condition|(
name|conf
operator|->
name|expires
operator|==
name|NGX_HTTP_EXPIRES_MAX
condition|)
block|{
name|expires
operator|->
name|value
operator|.
name|data
operator|=
operator|(
name|u_char
operator|*
operator|)
literal|"Thu, 31 Dec 2037 23:55:55 GMT"
expr_stmt|;
comment|/* 10 years */
name|ngx_str_set
argument_list|(
operator|&
name|cc
operator|->
name|value
argument_list|,
literal|"max-age=315360000"
argument_list|)
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
name|expires
operator|->
name|value
operator|.
name|data
operator|=
name|ngx_pnalloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
name|len
argument_list|)
expr_stmt|;
if|if
condition|(
name|expires
operator|->
name|value
operator|.
name|data
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|conf
operator|->
name|expires_time
operator|==
literal|0
condition|)
block|{
name|ngx_memcpy
argument_list|(
name|expires
operator|->
name|value
operator|.
name|data
argument_list|,
name|ngx_cached_http_time
operator|.
name|data
argument_list|,
name|ngx_cached_http_time
operator|.
name|len
operator|+
literal|1
argument_list|)
expr_stmt|;
name|ngx_str_set
argument_list|(
operator|&
name|cc
operator|->
name|value
argument_list|,
literal|"max-age=0"
argument_list|)
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
name|now
operator|=
name|ngx_time
argument_list|()
expr_stmt|;
if|if
condition|(
name|conf
operator|->
name|expires
operator|==
name|NGX_HTTP_EXPIRES_ACCESS
operator|||
name|r
operator|->
name|headers_out
operator|.
name|last_modified_time
operator|==
operator|-
literal|1
condition|)
block|{
name|expires_time
operator|=
name|now
operator|+
name|conf
operator|->
name|expires_time
expr_stmt|;
name|max_age
operator|=
name|conf
operator|->
name|expires_time
expr_stmt|;
block|}
if|else if
condition|(
name|conf
operator|->
name|expires
operator|==
name|NGX_HTTP_EXPIRES_DAILY
condition|)
block|{
name|expires_time
operator|=
name|ngx_next_time
argument_list|(
name|conf
operator|->
name|expires_time
argument_list|)
expr_stmt|;
name|max_age
operator|=
name|expires_time
operator|-
name|now
expr_stmt|;
block|}
else|else
block|{
name|expires_time
operator|=
name|r
operator|->
name|headers_out
operator|.
name|last_modified_time
operator|+
name|conf
operator|->
name|expires_time
expr_stmt|;
name|max_age
operator|=
name|expires_time
operator|-
name|now
expr_stmt|;
block|}
name|ngx_http_time
argument_list|(
name|expires
operator|->
name|value
operator|.
name|data
argument_list|,
name|expires_time
argument_list|)
expr_stmt|;
if|if
condition|(
name|conf
operator|->
name|expires_time
operator|<
literal|0
operator|||
name|max_age
operator|<
literal|0
condition|)
block|{
name|ngx_str_set
argument_list|(
operator|&
name|cc
operator|->
name|value
argument_list|,
literal|"no-cache"
argument_list|)
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
name|cc
operator|->
name|value
operator|.
name|data
operator|=
name|ngx_pnalloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
literal|"max-age="
argument_list|)
operator|+
name|NGX_TIME_T_LEN
operator|+
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|cc
operator|->
name|value
operator|.
name|data
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|cc
operator|->
name|value
operator|.
name|len
operator|=
name|ngx_sprintf
argument_list|(
name|cc
operator|->
name|value
operator|.
name|data
argument_list|,
literal|"max-age=%T"
argument_list|,
name|max_age
argument_list|)
operator|-
name|cc
operator|->
name|value
operator|.
name|data
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_add_header (ngx_http_request_t * r,ngx_http_header_val_t * hv,ngx_str_t * value)
name|ngx_http_add_header
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_header_val_t
modifier|*
name|hv
parameter_list|,
name|ngx_str_t
modifier|*
name|value
parameter_list|)
block|{
name|ngx_table_elt_t
modifier|*
name|h
decl_stmt|;
if|if
condition|(
name|value
operator|->
name|len
condition|)
block|{
name|h
operator|=
name|ngx_list_push
argument_list|(
operator|&
name|r
operator|->
name|headers_out
operator|.
name|headers
argument_list|)
expr_stmt|;
if|if
condition|(
name|h
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|h
operator|->
name|hash
operator|=
name|hv
operator|->
name|hash
expr_stmt|;
name|h
operator|->
name|key
operator|=
name|hv
operator|->
name|key
expr_stmt|;
name|h
operator|->
name|value
operator|=
operator|*
name|value
expr_stmt|;
block|}
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_add_cache_control (ngx_http_request_t * r,ngx_http_header_val_t * hv,ngx_str_t * value)
name|ngx_http_add_cache_control
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_header_val_t
modifier|*
name|hv
parameter_list|,
name|ngx_str_t
modifier|*
name|value
parameter_list|)
block|{
name|ngx_table_elt_t
modifier|*
name|cc
decl_stmt|,
modifier|*
modifier|*
name|ccp
decl_stmt|;
name|ccp
operator|=
name|r
operator|->
name|headers_out
operator|.
name|cache_control
operator|.
name|elts
expr_stmt|;
if|if
condition|(
name|ccp
operator|==
name|NULL
condition|)
block|{
if|if
condition|(
name|ngx_array_init
argument_list|(
operator|&
name|r
operator|->
name|headers_out
operator|.
name|cache_control
argument_list|,
name|r
operator|->
name|pool
argument_list|,
literal|1
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_table_elt_t
operator|*
argument_list|)
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
block|}
name|ccp
operator|=
name|ngx_array_push
argument_list|(
operator|&
name|r
operator|->
name|headers_out
operator|.
name|cache_control
argument_list|)
expr_stmt|;
if|if
condition|(
name|ccp
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|cc
operator|=
name|ngx_list_push
argument_list|(
operator|&
name|r
operator|->
name|headers_out
operator|.
name|headers
argument_list|)
expr_stmt|;
if|if
condition|(
name|cc
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|cc
operator|->
name|hash
operator|=
literal|1
expr_stmt|;
name|ngx_str_set
argument_list|(
operator|&
name|cc
operator|->
name|key
argument_list|,
literal|"Cache-Control"
argument_list|)
expr_stmt|;
name|cc
operator|->
name|value
operator|=
operator|*
name|value
expr_stmt|;
operator|*
name|ccp
operator|=
name|cc
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_set_last_modified (ngx_http_request_t * r,ngx_http_header_val_t * hv,ngx_str_t * value)
name|ngx_http_set_last_modified
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_header_val_t
modifier|*
name|hv
parameter_list|,
name|ngx_str_t
modifier|*
name|value
parameter_list|)
block|{
name|ngx_table_elt_t
modifier|*
name|h
decl_stmt|,
modifier|*
modifier|*
name|old
decl_stmt|;
if|if
condition|(
name|hv
operator|->
name|offset
condition|)
block|{
name|old
operator|=
operator|(
name|ngx_table_elt_t
operator|*
operator|*
operator|)
operator|(
operator|(
name|char
operator|*
operator|)
operator|&
name|r
operator|->
name|headers_out
operator|+
name|hv
operator|->
name|offset
operator|)
expr_stmt|;
block|}
else|else
block|{
name|old
operator|=
name|NULL
expr_stmt|;
block|}
name|r
operator|->
name|headers_out
operator|.
name|last_modified_time
operator|=
operator|-
literal|1
expr_stmt|;
if|if
condition|(
name|old
operator|==
name|NULL
operator|||
operator|*
name|old
operator|==
name|NULL
condition|)
block|{
if|if
condition|(
name|value
operator|->
name|len
operator|==
literal|0
condition|)
block|{
return|return
name|NGX_OK
return|;
block|}
name|h
operator|=
name|ngx_list_push
argument_list|(
operator|&
name|r
operator|->
name|headers_out
operator|.
name|headers
argument_list|)
expr_stmt|;
if|if
condition|(
name|h
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
block|}
else|else
block|{
name|h
operator|=
operator|*
name|old
expr_stmt|;
if|if
condition|(
name|value
operator|->
name|len
operator|==
literal|0
condition|)
block|{
name|h
operator|->
name|hash
operator|=
literal|0
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
block|}
name|h
operator|->
name|hash
operator|=
name|hv
operator|->
name|hash
expr_stmt|;
name|h
operator|->
name|key
operator|=
name|hv
operator|->
name|key
expr_stmt|;
name|h
operator|->
name|value
operator|=
operator|*
name|value
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
DECL|function|ngx_http_headers_create_conf (ngx_conf_t * cf)
name|ngx_http_headers_create_conf
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
block|{
name|ngx_http_headers_conf_t
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
name|ngx_http_headers_conf_t
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
comment|/*      * set by ngx_pcalloc():      *      *     conf->headers = NULL;      *     conf->expires_time = 0;      */
name|conf
operator|->
name|expires
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
DECL|function|ngx_http_headers_merge_conf (ngx_conf_t * cf,void * parent,void * child)
name|ngx_http_headers_merge_conf
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
name|ngx_http_headers_conf_t
modifier|*
name|prev
init|=
name|parent
decl_stmt|;
name|ngx_http_headers_conf_t
modifier|*
name|conf
init|=
name|child
decl_stmt|;
if|if
condition|(
name|conf
operator|->
name|expires
operator|==
name|NGX_CONF_UNSET_UINT
condition|)
block|{
name|conf
operator|->
name|expires
operator|=
name|prev
operator|->
name|expires
expr_stmt|;
name|conf
operator|->
name|expires_time
operator|=
name|prev
operator|->
name|expires_time
expr_stmt|;
if|if
condition|(
name|conf
operator|->
name|expires
operator|==
name|NGX_CONF_UNSET_UINT
condition|)
block|{
name|conf
operator|->
name|expires
operator|=
name|NGX_HTTP_EXPIRES_OFF
expr_stmt|;
block|}
block|}
if|if
condition|(
name|conf
operator|->
name|headers
operator|==
name|NULL
condition|)
block|{
name|conf
operator|->
name|headers
operator|=
name|prev
operator|->
name|headers
expr_stmt|;
block|}
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_headers_filter_init (ngx_conf_t * cf)
name|ngx_http_headers_filter_init
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
block|{
name|ngx_http_next_header_filter
operator|=
name|ngx_http_top_header_filter
expr_stmt|;
name|ngx_http_top_header_filter
operator|=
name|ngx_http_headers_filter
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
specifier|static
name|char
modifier|*
DECL|function|ngx_http_headers_expires (ngx_conf_t * cf,ngx_command_t * cmd,void * conf)
name|ngx_http_headers_expires
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
name|ngx_http_headers_conf_t
modifier|*
name|hcf
init|=
name|conf
decl_stmt|;
name|ngx_uint_t
name|minus
decl_stmt|,
name|n
decl_stmt|;
name|ngx_str_t
modifier|*
name|value
decl_stmt|;
if|if
condition|(
name|hcf
operator|->
name|expires
operator|!=
name|NGX_CONF_UNSET_UINT
condition|)
block|{
return|return
literal|"is duplicate"
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
if|if
condition|(
name|cf
operator|->
name|args
operator|->
name|nelts
operator|==
literal|2
condition|)
block|{
if|if
condition|(
name|ngx_strcmp
argument_list|(
name|value
index|[
literal|1
index|]
operator|.
name|data
argument_list|,
literal|"epoch"
argument_list|)
operator|==
literal|0
condition|)
block|{
name|hcf
operator|->
name|expires
operator|=
name|NGX_HTTP_EXPIRES_EPOCH
expr_stmt|;
return|return
name|NGX_CONF_OK
return|;
block|}
if|if
condition|(
name|ngx_strcmp
argument_list|(
name|value
index|[
literal|1
index|]
operator|.
name|data
argument_list|,
literal|"max"
argument_list|)
operator|==
literal|0
condition|)
block|{
name|hcf
operator|->
name|expires
operator|=
name|NGX_HTTP_EXPIRES_MAX
expr_stmt|;
return|return
name|NGX_CONF_OK
return|;
block|}
if|if
condition|(
name|ngx_strcmp
argument_list|(
name|value
index|[
literal|1
index|]
operator|.
name|data
argument_list|,
literal|"off"
argument_list|)
operator|==
literal|0
condition|)
block|{
name|hcf
operator|->
name|expires
operator|=
name|NGX_HTTP_EXPIRES_OFF
expr_stmt|;
return|return
name|NGX_CONF_OK
return|;
block|}
name|hcf
operator|->
name|expires
operator|=
name|NGX_HTTP_EXPIRES_ACCESS
expr_stmt|;
name|n
operator|=
literal|1
expr_stmt|;
block|}
else|else
block|{
comment|/* cf->args->nelts == 3 */
if|if
condition|(
name|ngx_strcmp
argument_list|(
name|value
index|[
literal|1
index|]
operator|.
name|data
argument_list|,
literal|"modified"
argument_list|)
operator|!=
literal|0
condition|)
block|{
return|return
literal|"invalid value"
return|;
block|}
name|hcf
operator|->
name|expires
operator|=
name|NGX_HTTP_EXPIRES_MODIFIED
expr_stmt|;
name|n
operator|=
literal|2
expr_stmt|;
block|}
if|if
condition|(
name|value
index|[
name|n
index|]
operator|.
name|data
index|[
literal|0
index|]
operator|==
literal|'@'
condition|)
block|{
name|value
index|[
name|n
index|]
operator|.
name|data
operator|++
expr_stmt|;
name|value
index|[
name|n
index|]
operator|.
name|len
operator|--
expr_stmt|;
name|minus
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|hcf
operator|->
name|expires
operator|==
name|NGX_HTTP_EXPIRES_MODIFIED
condition|)
block|{
return|return
literal|"daily time can not be used with \"modified\" parameter"
return|;
block|}
name|hcf
operator|->
name|expires
operator|=
name|NGX_HTTP_EXPIRES_DAILY
expr_stmt|;
block|}
if|else if
condition|(
name|value
index|[
name|n
index|]
operator|.
name|data
index|[
literal|0
index|]
operator|==
literal|'+'
condition|)
block|{
name|value
index|[
name|n
index|]
operator|.
name|data
operator|++
expr_stmt|;
name|value
index|[
name|n
index|]
operator|.
name|len
operator|--
expr_stmt|;
name|minus
operator|=
literal|0
expr_stmt|;
block|}
if|else if
condition|(
name|value
index|[
name|n
index|]
operator|.
name|data
index|[
literal|0
index|]
operator|==
literal|'-'
condition|)
block|{
name|value
index|[
name|n
index|]
operator|.
name|data
operator|++
expr_stmt|;
name|value
index|[
name|n
index|]
operator|.
name|len
operator|--
expr_stmt|;
name|minus
operator|=
literal|1
expr_stmt|;
block|}
else|else
block|{
name|minus
operator|=
literal|0
expr_stmt|;
block|}
name|hcf
operator|->
name|expires_time
operator|=
name|ngx_parse_time
argument_list|(
operator|&
name|value
index|[
name|n
index|]
argument_list|,
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|hcf
operator|->
name|expires_time
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
literal|"invalid value"
return|;
block|}
if|if
condition|(
name|hcf
operator|->
name|expires
operator|==
name|NGX_HTTP_EXPIRES_DAILY
operator|&&
name|hcf
operator|->
name|expires_time
operator|>
literal|24
operator|*
literal|60
operator|*
literal|60
condition|)
block|{
return|return
literal|"daily time value must be less than 24 hours"
return|;
block|}
if|if
condition|(
name|hcf
operator|->
name|expires_time
operator|==
name|NGX_PARSE_LARGE_TIME
condition|)
block|{
return|return
literal|"value must be less than 68 years"
return|;
block|}
if|if
condition|(
name|minus
condition|)
block|{
name|hcf
operator|->
name|expires_time
operator|=
operator|-
name|hcf
operator|->
name|expires_time
expr_stmt|;
block|}
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

begin_function
specifier|static
name|char
modifier|*
DECL|function|ngx_http_headers_add (ngx_conf_t * cf,ngx_command_t * cmd,void * conf)
name|ngx_http_headers_add
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
name|ngx_http_headers_conf_t
modifier|*
name|hcf
init|=
name|conf
decl_stmt|;
name|ngx_str_t
modifier|*
name|value
decl_stmt|;
name|ngx_uint_t
name|i
decl_stmt|;
name|ngx_http_header_val_t
modifier|*
name|hv
decl_stmt|;
name|ngx_http_set_header_t
modifier|*
name|set
decl_stmt|;
name|ngx_http_compile_complex_value_t
name|ccv
decl_stmt|;
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
name|hcf
operator|->
name|headers
operator|==
name|NULL
condition|)
block|{
name|hcf
operator|->
name|headers
operator|=
name|ngx_array_create
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
literal|1
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_header_val_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|hcf
operator|->
name|headers
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
block|}
name|hv
operator|=
name|ngx_array_push
argument_list|(
name|hcf
operator|->
name|headers
argument_list|)
expr_stmt|;
if|if
condition|(
name|hv
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|hv
operator|->
name|hash
operator|=
literal|1
expr_stmt|;
name|hv
operator|->
name|key
operator|=
name|value
index|[
literal|1
index|]
expr_stmt|;
name|hv
operator|->
name|handler
operator|=
name|ngx_http_add_header
expr_stmt|;
name|hv
operator|->
name|offset
operator|=
literal|0
expr_stmt|;
name|set
operator|=
name|ngx_http_set_headers
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|set
index|[
name|i
index|]
operator|.
name|name
operator|.
name|len
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|ngx_strcasecmp
argument_list|(
name|value
index|[
literal|1
index|]
operator|.
name|data
argument_list|,
name|set
index|[
name|i
index|]
operator|.
name|name
operator|.
name|data
argument_list|)
operator|!=
literal|0
condition|)
block|{
continue|continue;
block|}
name|hv
operator|->
name|offset
operator|=
name|set
index|[
name|i
index|]
operator|.
name|offset
expr_stmt|;
name|hv
operator|->
name|handler
operator|=
name|set
index|[
name|i
index|]
operator|.
name|handler
expr_stmt|;
break|break;
block|}
if|if
condition|(
name|value
index|[
literal|2
index|]
operator|.
name|len
operator|==
literal|0
condition|)
block|{
name|ngx_memzero
argument_list|(
operator|&
name|hv
operator|->
name|value
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_complex_value_t
argument_list|)
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_OK
return|;
block|}
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
literal|2
index|]
expr_stmt|;
name|ccv
operator|.
name|complex_value
operator|=
operator|&
name|hv
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
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

end_unit

