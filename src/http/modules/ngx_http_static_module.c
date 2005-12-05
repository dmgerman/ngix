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
DECL|struct|__anon2b1a37980108
typedef|typedef
struct|struct
block|{
DECL|member|redirect_cache
name|ngx_http_cache_hash_t
modifier|*
name|redirect_cache
decl_stmt|;
DECL|typedef|ngx_http_static_loc_conf_t
block|}
name|ngx_http_static_loc_conf_t
typedef|;
end_typedef

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
name|void
modifier|*
name|ngx_http_static_create_loc_conf
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
name|ngx_http_static_merge_loc_conf
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
name|ngx_http_static_init
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
function_decl|;
end_function_decl

begin_decl_stmt
DECL|variable|ngx_http_static_commands
specifier|static
name|ngx_command_t
name|ngx_http_static_commands
index|[]
init|=
block|{
if|#
directive|if
operator|(
name|NGX_HTTP_CACHE
operator|)
block|{
name|ngx_string
argument_list|(
literal|"redirect_cache"
argument_list|)
block|,
name|NGX_HTTP_MAIN_CONF
operator||
name|NGX_HTTP_SRV_CONF
operator||
name|NGX_HTTP_LOC_CONF
operator||
name|NGX_CONF_TAKE3
block|,
name|ngx_http_set_cache_slot
block|,
name|NGX_HTTP_LOC_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_http_static_loc_conf_t
argument_list|,
name|redirect_cache
argument_list|)
block|,
name|NULL
block|}
block|,
endif|#
directive|endif
name|ngx_null_command
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_static_module_ctx
name|ngx_http_module_t
name|ngx_http_static_module_ctx
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
name|ngx_http_static_create_loc_conf
block|,
comment|/* create location configuration */
name|ngx_http_static_merge_loc_conf
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
name|ngx_http_static_commands
block|,
comment|/* module directives */
name|NGX_HTTP_MODULE
block|,
comment|/* module type */
name|NULL
block|,
comment|/* init master */
name|ngx_http_static_init
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
name|ngx_fd_t
name|fd
decl_stmt|;
name|ngx_int_t
name|rc
decl_stmt|;
name|ngx_uint_t
name|level
decl_stmt|;
name|ngx_str_t
name|path
decl_stmt|;
name|ngx_err_t
name|err
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
name|ngx_file_info_t
name|fi
decl_stmt|;
name|ngx_pool_cleanup_t
modifier|*
name|cln
decl_stmt|;
name|ngx_pool_cleanup_file_t
modifier|*
name|clnf
decl_stmt|;
name|ngx_http_core_loc_conf_t
modifier|*
name|clcf
decl_stmt|;
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
comment|/* TODO: Win32 */
if|if
condition|(
name|r
operator|->
name|zero_in_uri
condition|)
block|{
return|return
name|NGX_DECLINED
return|;
block|}
if|if
condition|(
name|r
operator|->
name|method
operator|!=
name|NGX_HTTP_GET
operator|&&
name|r
operator|->
name|method
operator|!=
name|NGX_HTTP_HEAD
condition|)
block|{
return|return
name|NGX_HTTP_NOT_ALLOWED
return|;
block|}
name|rc
operator|=
name|ngx_http_discard_body
argument_list|(
name|r
argument_list|)
expr_stmt|;
if|if
condition|(
name|rc
operator|!=
name|NGX_OK
operator|&&
name|rc
operator|!=
name|NGX_AGAIN
condition|)
block|{
return|return
name|rc
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
name|cln
operator|=
name|ngx_pool_cleanup_add
argument_list|(
name|r
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_pool_cleanup_file_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|cln
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
name|fd
operator|=
name|ngx_open_file
argument_list|(
name|path
operator|.
name|data
argument_list|,
name|NGX_FILE_RDONLY
argument_list|,
name|NGX_FILE_OPEN
argument_list|)
expr_stmt|;
if|if
condition|(
name|fd
operator|==
name|NGX_INVALID_FILE
condition|)
block|{
name|err
operator|=
name|ngx_errno
expr_stmt|;
if|if
condition|(
name|err
operator|==
name|NGX_ENOENT
operator|||
name|err
operator|==
name|NGX_ENOTDIR
operator|||
name|err
operator|==
name|NGX_ENAMETOOLONG
condition|)
block|{
name|level
operator|=
name|NGX_LOG_ERR
expr_stmt|;
name|rc
operator|=
name|NGX_HTTP_NOT_FOUND
expr_stmt|;
block|}
if|else if
condition|(
name|err
operator|==
name|NGX_EACCES
condition|)
block|{
name|level
operator|=
name|NGX_LOG_ERR
expr_stmt|;
name|rc
operator|=
name|NGX_HTTP_FORBIDDEN
expr_stmt|;
block|}
else|else
block|{
name|level
operator|=
name|NGX_LOG_CRIT
expr_stmt|;
name|rc
operator|=
name|NGX_HTTP_INTERNAL_SERVER_ERROR
expr_stmt|;
block|}
name|clcf
operator|=
name|ngx_http_get_module_loc_conf
argument_list|(
name|r
argument_list|,
name|ngx_http_core_module
argument_list|)
expr_stmt|;
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
name|err
argument_list|,
name|ngx_open_file_n
literal|" \"%s\" failed"
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
name|fd
argument_list|)
expr_stmt|;
if|if
condition|(
name|ngx_fd_info
argument_list|(
name|fd
argument_list|,
operator|&
name|fi
argument_list|)
operator|==
name|NGX_FILE_ERROR
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_CRIT
argument_list|,
name|log
argument_list|,
name|ngx_errno
argument_list|,
name|ngx_fd_info_n
literal|" \"%s\" failed"
argument_list|,
name|path
operator|.
name|data
argument_list|)
expr_stmt|;
if|if
condition|(
name|ngx_close_file
argument_list|(
name|fd
argument_list|)
operator|==
name|NGX_FILE_ERROR
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|log
argument_list|,
name|ngx_errno
argument_list|,
name|ngx_close_file_n
literal|" \"%s\" failed"
argument_list|,
name|path
operator|.
name|data
argument_list|)
expr_stmt|;
block|}
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
if|if
condition|(
name|ngx_is_dir
argument_list|(
operator|&
name|fi
argument_list|)
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
if|if
condition|(
name|ngx_close_file
argument_list|(
name|fd
argument_list|)
operator|==
name|NGX_FILE_ERROR
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|log
argument_list|,
name|ngx_errno
argument_list|,
name|ngx_close_file_n
literal|" \"%s\" failed"
argument_list|,
name|path
operator|.
name|data
argument_list|)
expr_stmt|;
block|}
name|r
operator|->
name|headers_out
operator|.
name|location
operator|=
name|ngx_palloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_table_elt_t
argument_list|)
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
name|clcf
operator|=
name|ngx_http_get_module_loc_conf
argument_list|(
name|r
argument_list|,
name|ngx_http_core_module
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|clcf
operator|->
name|alias
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
block|}
else|else
block|{
name|location
operator|=
name|ngx_palloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
name|r
operator|->
name|uri
operator|.
name|len
operator|+
literal|1
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
block|}
operator|*
name|last
operator|=
literal|'/'
expr_stmt|;
comment|/*          * we do not need to set the r->headers_out.location->hash and          * r->headers_out.location->key fields          */
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
name|r
operator|->
name|uri
operator|.
name|len
operator|+
literal|1
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
name|ngx_is_file
argument_list|(
operator|&
name|fi
argument_list|)
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_CRIT
argument_list|,
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"\"%s\" is not a regular file"
argument_list|,
name|path
operator|.
name|data
argument_list|)
expr_stmt|;
if|if
condition|(
name|ngx_close_file
argument_list|(
name|fd
argument_list|)
operator|==
name|NGX_FILE_ERROR
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|log
argument_list|,
name|ngx_errno
argument_list|,
name|ngx_close_file_n
literal|" \"%s\" failed"
argument_list|,
name|path
operator|.
name|data
argument_list|)
expr_stmt|;
block|}
return|return
name|NGX_HTTP_NOT_FOUND
return|;
block|}
endif|#
directive|endif
name|log
operator|->
name|action
operator|=
literal|"sending response to client"
expr_stmt|;
name|cln
operator|->
name|handler
operator|=
name|ngx_pool_cleanup_file
expr_stmt|;
name|clnf
operator|=
name|cln
operator|->
name|data
expr_stmt|;
name|clnf
operator|->
name|fd
operator|=
name|fd
expr_stmt|;
name|clnf
operator|->
name|name
operator|=
name|path
operator|.
name|data
expr_stmt|;
name|clnf
operator|->
name|log
operator|=
name|r
operator|->
name|pool
operator|->
name|log
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
name|ngx_file_size
argument_list|(
operator|&
name|fi
argument_list|)
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|last_modified_time
operator|=
name|ngx_file_mtime
argument_list|(
operator|&
name|fi
argument_list|)
expr_stmt|;
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
name|r
operator|->
name|allow_ranges
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|r
operator|->
name|header_only
operator|||
operator|(
name|r
operator|->
expr|main
operator|!=
name|r
operator|&&
name|ngx_file_size
argument_list|(
operator|&
name|fi
argument_list|)
operator|==
literal|0
operator|)
condition|)
block|{
return|return
name|ngx_http_send_header
argument_list|(
name|r
argument_list|)
return|;
block|}
comment|/* we need to allocate all before the header would be sent */
name|b
operator|=
name|ngx_pcalloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_buf_t
argument_list|)
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
name|ngx_file_size
argument_list|(
operator|&
name|fi
argument_list|)
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
operator|->
expr|main
operator|==
name|r
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
name|void
modifier|*
DECL|function|ngx_http_static_create_loc_conf (ngx_conf_t * cf)
name|ngx_http_static_create_loc_conf
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
block|{
name|ngx_http_static_loc_conf_t
modifier|*
name|conf
decl_stmt|;
name|conf
operator|=
name|ngx_palloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_static_loc_conf_t
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
name|NGX_CONF_ERROR
return|;
block|}
name|conf
operator|->
name|redirect_cache
operator|=
name|NULL
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
DECL|function|ngx_http_static_merge_loc_conf (ngx_conf_t * cf,void * parent,void * child)
name|ngx_http_static_merge_loc_conf
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
name|ngx_http_static_loc_conf_t
modifier|*
name|prev
init|=
name|parent
decl_stmt|;
name|ngx_http_static_loc_conf_t
modifier|*
name|conf
init|=
name|child
decl_stmt|;
if|if
condition|(
name|conf
operator|->
name|redirect_cache
operator|==
name|NULL
condition|)
block|{
name|conf
operator|->
name|redirect_cache
operator|=
name|prev
operator|->
name|redirect_cache
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
DECL|function|ngx_http_static_init (ngx_cycle_t * cycle)
name|ngx_http_static_init
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
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
name|ngx_http_cycle_get_module_main_conf
argument_list|(
name|cycle
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

