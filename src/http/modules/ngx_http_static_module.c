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

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_static_handler
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_static_init
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
function_decl|;
end_function_decl

begin_decl_stmt
DECL|variable|ngx_http_static_module_ctx
specifier|static
name|ngx_http_module_t
name|ngx_http_static_module_ctx
init|=
block|{
name|NULL
block|,
comment|/* preconfiguration */
name|ngx_http_static_init
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
name|NULL
block|,
comment|/* create location configuration */
name|NULL
comment|/* merge location configuration */
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_static_module
name|ngx_module_t
name|ngx_http_static_module
init|=
block|{
name|NGX_MODULE_V1
block|,
operator|&
name|ngx_http_static_module_ctx
block|,
comment|/* module context */
name|NULL
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
DECL|function|ngx_http_static_handler (ngx_http_request_t * r)
name|ngx_http_static_handler
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
block|{
name|u_char
modifier|*
name|last
decl_stmt|,
modifier|*
name|location
decl_stmt|;
name|size_t
name|root
decl_stmt|,
name|len
decl_stmt|;
name|ngx_str_t
name|path
decl_stmt|;
name|ngx_int_t
name|rc
decl_stmt|;
name|ngx_uint_t
name|level
decl_stmt|;
name|ngx_log_t
modifier|*
name|log
decl_stmt|;
name|ngx_buf_t
modifier|*
name|b
decl_stmt|;
name|ngx_chain_t
name|out
decl_stmt|;
name|ngx_open_file_info_t
name|of
decl_stmt|;
name|ngx_http_core_loc_conf_t
modifier|*
name|clcf
decl_stmt|;
if|if
condition|(
operator|!
operator|(
name|r
operator|->
name|method
operator|&
operator|(
name|NGX_HTTP_GET
operator||
name|NGX_HTTP_HEAD
operator||
name|NGX_HTTP_POST
operator|)
operator|)
condition|)
block|{
return|return
name|NGX_HTTP_NOT_ALLOWED
return|;
block|}
if|if
condition|(
name|r
operator|->
name|uri
operator|.
name|data
index|[
name|r
operator|->
name|uri
operator|.
name|len
operator|-
literal|1
index|]
operator|==
literal|'/'
condition|)
block|{
return|return
name|NGX_DECLINED
return|;
block|}
name|log
operator|=
name|r
operator|->
name|connection
operator|->
name|log
expr_stmt|;
comment|/*      * ngx_http_map_uri_to_path() allocates memory for terminating '\0'      * so we do not need to reserve memory for '/' for possible redirect      */
name|last
operator|=
name|ngx_http_map_uri_to_path
argument_list|(
name|r
argument_list|,
operator|&
name|path
argument_list|,
operator|&
name|root
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|last
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
name|path
operator|.
name|len
operator|=
name|last
operator|-
name|path
operator|.
name|data
expr_stmt|;
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_HTTP
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"http filename: \"%s\""
argument_list|,
name|path
operator|.
name|data
argument_list|)
expr_stmt|;
name|clcf
operator|=
name|ngx_http_get_module_loc_conf
argument_list|(
name|r
argument_list|,
name|ngx_http_core_module
argument_list|)
expr_stmt|;
name|ngx_memzero
argument_list|(
operator|&
name|of
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_open_file_info_t
argument_list|)
argument_list|)
expr_stmt|;
name|of
operator|.
name|read_ahead
operator|=
name|clcf
operator|->
name|read_ahead
expr_stmt|;
name|of
operator|.
name|directio
operator|=
name|clcf
operator|->
name|directio
expr_stmt|;
name|of
operator|.
name|valid
operator|=
name|clcf
operator|->
name|open_file_cache_valid
expr_stmt|;
name|of
operator|.
name|min_uses
operator|=
name|clcf
operator|->
name|open_file_cache_min_uses
expr_stmt|;
name|of
operator|.
name|errors
operator|=
name|clcf
operator|->
name|open_file_cache_errors
expr_stmt|;
name|of
operator|.
name|events
operator|=
name|clcf
operator|->
name|open_file_cache_events
expr_stmt|;
if|if
condition|(
name|ngx_http_set_disable_symlinks
argument_list|(
name|r
argument_list|,
name|clcf
argument_list|,
operator|&
name|path
argument_list|,
operator|&
name|of
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
if|if
condition|(
name|ngx_open_cached_file
argument_list|(
name|clcf
operator|->
name|open_file_cache
argument_list|,
operator|&
name|path
argument_list|,
operator|&
name|of
argument_list|,
name|r
operator|->
name|pool
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
switch|switch
condition|(
name|of
operator|.
name|err
condition|)
block|{
case|case
literal|0
case|:
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
case|case
name|NGX_ENOENT
case|:
case|case
name|NGX_ENOTDIR
case|:
case|case
name|NGX_ENAMETOOLONG
case|:
name|level
operator|=
name|NGX_LOG_ERR
expr_stmt|;
name|rc
operator|=
name|NGX_HTTP_NOT_FOUND
expr_stmt|;
break|break;
case|case
name|NGX_EACCES
case|:
if|#
directive|if
operator|(
name|NGX_HAVE_OPENAT
operator|)
case|case
name|NGX_EMLINK
case|:
case|case
name|NGX_ELOOP
case|:
endif|#
directive|endif
name|level
operator|=
name|NGX_LOG_ERR
expr_stmt|;
name|rc
operator|=
name|NGX_HTTP_FORBIDDEN
expr_stmt|;
break|break;
default|default:
name|level
operator|=
name|NGX_LOG_CRIT
expr_stmt|;
name|rc
operator|=
name|NGX_HTTP_INTERNAL_SERVER_ERROR
expr_stmt|;
break|break;
block|}
if|if
condition|(
name|rc
operator|!=
name|NGX_HTTP_NOT_FOUND
operator|||
name|clcf
operator|->
name|log_not_found
condition|)
block|{
name|ngx_log_error
argument_list|(
name|level
argument_list|,
name|log
argument_list|,
name|of
operator|.
name|err
argument_list|,
literal|"%s \"%s\" failed"
argument_list|,
name|of
operator|.
name|failed
argument_list|,
name|path
operator|.
name|data
argument_list|)
expr_stmt|;
block|}
return|return
name|rc
return|;
block|}
name|r
operator|->
name|root_tested
operator|=
operator|!
name|r
operator|->
name|error_page
expr_stmt|;
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_HTTP
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"http static fd: %d"
argument_list|,
name|of
operator|.
name|fd
argument_list|)
expr_stmt|;
if|if
condition|(
name|of
operator|.
name|is_dir
condition|)
block|{
name|ngx_log_debug0
argument_list|(
name|NGX_LOG_DEBUG_HTTP
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"http dir"
argument_list|)
expr_stmt|;
name|ngx_http_clear_location
argument_list|(
name|r
argument_list|)
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|location
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
name|r
operator|->
name|headers_out
operator|.
name|location
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
name|len
operator|=
name|r
operator|->
name|uri
operator|.
name|len
operator|+
literal|1
expr_stmt|;
if|if
condition|(
operator|!
name|clcf
operator|->
name|alias
operator|&&
name|clcf
operator|->
name|root_lengths
operator|==
name|NULL
operator|&&
name|r
operator|->
name|args
operator|.
name|len
operator|==
literal|0
condition|)
block|{
name|location
operator|=
name|path
operator|.
name|data
operator|+
name|clcf
operator|->
name|root
operator|.
name|len
expr_stmt|;
operator|*
name|last
operator|=
literal|'/'
expr_stmt|;
block|}
else|else
block|{
if|if
condition|(
name|r
operator|->
name|args
operator|.
name|len
condition|)
block|{
name|len
operator|+=
name|r
operator|->
name|args
operator|.
name|len
operator|+
literal|1
expr_stmt|;
block|}
name|location
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
name|location
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
name|last
operator|=
name|ngx_copy
argument_list|(
name|location
argument_list|,
name|r
operator|->
name|uri
operator|.
name|data
argument_list|,
name|r
operator|->
name|uri
operator|.
name|len
argument_list|)
expr_stmt|;
operator|*
name|last
operator|=
literal|'/'
expr_stmt|;
if|if
condition|(
name|r
operator|->
name|args
operator|.
name|len
condition|)
block|{
operator|*
operator|++
name|last
operator|=
literal|'?'
expr_stmt|;
name|ngx_memcpy
argument_list|(
operator|++
name|last
argument_list|,
name|r
operator|->
name|args
operator|.
name|data
argument_list|,
name|r
operator|->
name|args
operator|.
name|len
argument_list|)
expr_stmt|;
block|}
block|}
name|r
operator|->
name|headers_out
operator|.
name|location
operator|->
name|hash
operator|=
literal|1
expr_stmt|;
name|ngx_str_set
argument_list|(
operator|&
name|r
operator|->
name|headers_out
operator|.
name|location
operator|->
name|key
argument_list|,
literal|"Location"
argument_list|)
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|location
operator|->
name|value
operator|.
name|len
operator|=
name|len
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|location
operator|->
name|value
operator|.
name|data
operator|=
name|location
expr_stmt|;
return|return
name|NGX_HTTP_MOVED_PERMANENTLY
return|;
block|}
if|#
directive|if
operator|!
operator|(
name|NGX_WIN32
operator|)
comment|/* the not regular files are probably Unix specific */
if|if
condition|(
operator|!
name|of
operator|.
name|is_file
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_CRIT
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"\"%s\" is not a regular file"
argument_list|,
name|path
operator|.
name|data
argument_list|)
expr_stmt|;
return|return
name|NGX_HTTP_NOT_FOUND
return|;
block|}
endif|#
directive|endif
if|if
condition|(
name|r
operator|->
name|method
operator|==
name|NGX_HTTP_POST
condition|)
block|{
return|return
name|NGX_HTTP_NOT_ALLOWED
return|;
block|}
name|rc
operator|=
name|ngx_http_discard_request_body
argument_list|(
name|r
argument_list|)
expr_stmt|;
if|if
condition|(
name|rc
operator|!=
name|NGX_OK
condition|)
block|{
return|return
name|rc
return|;
block|}
name|log
operator|->
name|action
operator|=
literal|"sending response to client"
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|status
operator|=
name|NGX_HTTP_OK
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|content_length_n
operator|=
name|of
operator|.
name|size
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|last_modified_time
operator|=
name|of
operator|.
name|mtime
expr_stmt|;
if|if
condition|(
name|ngx_http_set_etag
argument_list|(
name|r
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
if|if
condition|(
name|ngx_http_set_content_type
argument_list|(
name|r
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
if|if
condition|(
name|r
operator|!=
name|r
operator|->
expr|main
operator|&&
name|of
operator|.
name|size
operator|==
literal|0
condition|)
block|{
return|return
name|ngx_http_send_header
argument_list|(
name|r
argument_list|)
return|;
block|}
name|r
operator|->
name|allow_ranges
operator|=
literal|1
expr_stmt|;
comment|/* we need to allocate all before the header would be sent */
name|b
operator|=
name|ngx_calloc_buf
argument_list|(
name|r
operator|->
name|pool
argument_list|)
expr_stmt|;
if|if
condition|(
name|b
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
name|b
operator|->
name|file
operator|=
name|ngx_pcalloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_file_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|b
operator|->
name|file
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
name|rc
operator|=
name|ngx_http_send_header
argument_list|(
name|r
argument_list|)
expr_stmt|;
if|if
condition|(
name|rc
operator|==
name|NGX_ERROR
operator|||
name|rc
operator|>
name|NGX_OK
operator|||
name|r
operator|->
name|header_only
condition|)
block|{
return|return
name|rc
return|;
block|}
name|b
operator|->
name|file_pos
operator|=
literal|0
expr_stmt|;
name|b
operator|->
name|file_last
operator|=
name|of
operator|.
name|size
expr_stmt|;
name|b
operator|->
name|in_file
operator|=
name|b
operator|->
name|file_last
condition|?
literal|1
else|:
literal|0
expr_stmt|;
name|b
operator|->
name|last_buf
operator|=
operator|(
name|r
operator|==
name|r
operator|->
expr|main
operator|)
condition|?
literal|1
else|:
literal|0
expr_stmt|;
name|b
operator|->
name|last_in_chain
operator|=
literal|1
expr_stmt|;
name|b
operator|->
name|file
operator|->
name|fd
operator|=
name|of
operator|.
name|fd
expr_stmt|;
name|b
operator|->
name|file
operator|->
name|name
operator|=
name|path
expr_stmt|;
name|b
operator|->
name|file
operator|->
name|log
operator|=
name|log
expr_stmt|;
name|b
operator|->
name|file
operator|->
name|directio
operator|=
name|of
operator|.
name|is_directio
expr_stmt|;
name|out
operator|.
name|buf
operator|=
name|b
expr_stmt|;
name|out
operator|.
name|next
operator|=
name|NULL
expr_stmt|;
return|return
name|ngx_http_output_filter
argument_list|(
name|r
argument_list|,
operator|&
name|out
argument_list|)
return|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_static_init (ngx_conf_t * cf)
name|ngx_http_static_init
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
block|{
name|ngx_http_handler_pt
modifier|*
name|h
decl_stmt|;
name|ngx_http_core_main_conf_t
modifier|*
name|cmcf
decl_stmt|;
name|cmcf
operator|=
name|ngx_http_conf_get_module_main_conf
argument_list|(
name|cf
argument_list|,
name|ngx_http_core_module
argument_list|)
expr_stmt|;
name|h
operator|=
name|ngx_array_push
argument_list|(
operator|&
name|cmcf
operator|->
name|phases
index|[
name|NGX_HTTP_CONTENT_PHASE
index|]
operator|.
name|handlers
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
operator|*
name|h
operator|=
name|ngx_http_static_handler
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

end_unit

