begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
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

begin_include
include|#
directive|include
file|<ngx_http_config.h>
end_include

begin_include
include|#
directive|include
file|<ngx_http_core_module.h>
end_include

begin_include
include|#
directive|include
file|<ngx_http_output_filter.h>
end_include

begin_function
DECL|function|ngx_http_static_handler (ngx_http_request_t * r)
name|int
name|ngx_http_static_handler
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
block|{
name|int
name|rc
decl_stmt|,
name|key
decl_stmt|,
name|i
decl_stmt|;
name|ngx_log_e
name|level
decl_stmt|;
name|ngx_err_t
name|err
decl_stmt|;
name|ngx_hunk_t
modifier|*
name|h
decl_stmt|;
name|ngx_http_type_t
modifier|*
name|type
decl_stmt|;
name|ngx_http_log_ctx_t
modifier|*
name|ctx
decl_stmt|;
name|ngx_http_core_loc_conf_t
modifier|*
name|clcf
decl_stmt|;
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
condition|)
block|{
return|return
name|rc
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
name|ctx
operator|=
name|r
operator|->
name|connection
operator|->
name|log
operator|->
name|data
expr_stmt|;
name|ctx
operator|->
name|action
operator|=
literal|"sending response"
expr_stmt|;
if|if
condition|(
name|r
operator|->
name|file
operator|.
name|fd
operator|==
name|NGX_INVALID_FILE
condition|)
block|{
name|r
operator|->
name|file
operator|.
name|fd
operator|=
name|ngx_open_file
argument_list|(
name|r
operator|->
name|file
operator|.
name|name
operator|.
name|data
argument_list|,
name|NGX_FILE_RDONLY
argument_list|)
expr_stmt|;
if|if
condition|(
name|r
operator|->
name|file
operator|.
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
name|ngx_log_error
argument_list|(
name|level
argument_list|,
name|r
operator|->
name|connection
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
name|ngx_open_file_n
literal|" %s failed"
argument_list|,
name|r
operator|->
name|file
operator|.
name|name
operator|.
name|data
argument_list|)
expr_stmt|;
return|return
name|rc
return|;
block|}
block|}
if|if
condition|(
operator|!
name|r
operator|->
name|file
operator|.
name|info_valid
condition|)
block|{
if|if
condition|(
name|ngx_stat_fd
argument_list|(
name|r
operator|->
name|file
operator|.
name|fd
argument_list|,
operator|&
name|r
operator|->
name|file
operator|.
name|info
argument_list|)
operator|==
name|NGX_FILE_ERROR
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_CRIT
argument_list|,
name|r
operator|->
name|connection
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
name|ngx_stat_fd_n
literal|" %s failed"
argument_list|,
name|r
operator|->
name|file
operator|.
name|name
operator|.
name|data
argument_list|)
expr_stmt|;
if|if
condition|(
name|ngx_close_file
argument_list|(
name|r
operator|->
name|file
operator|.
name|fd
argument_list|)
operator|==
name|NGX_FILE_ERROR
condition|)
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|r
operator|->
name|connection
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
name|ngx_close_file_n
literal|" %s failed"
argument_list|,
name|r
operator|->
name|file
operator|.
name|name
operator|.
name|data
argument_list|)
expr_stmt|;
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
name|r
operator|->
name|file
operator|.
name|info_valid
operator|=
literal|1
expr_stmt|;
block|}
if|#
directive|if
operator|!
operator|(
name|WIN32
operator|)
comment|/* the not regular files are probably Unix specific */
if|if
condition|(
operator|!
name|ngx_is_file
argument_list|(
name|r
operator|->
name|file
operator|.
name|info
argument_list|)
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_CRIT
argument_list|,
name|r
operator|->
name|connection
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"%s is not regular file"
argument_list|,
name|r
operator|->
name|file
operator|.
name|name
operator|.
name|data
argument_list|)
expr_stmt|;
if|if
condition|(
name|ngx_close_file
argument_list|(
name|r
operator|->
name|file
operator|.
name|fd
argument_list|)
operator|==
name|NGX_FILE_ERROR
condition|)
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|r
operator|->
name|connection
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
name|ngx_close_file_n
literal|" %s failed"
argument_list|,
name|r
operator|->
name|file
operator|.
name|name
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
name|content_length
operator|=
name|ngx_file_size
argument_list|(
name|r
operator|->
name|file
operator|.
name|info
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
name|r
operator|->
name|file
operator|.
name|info
argument_list|)
expr_stmt|;
name|ngx_test_null
argument_list|(
name|r
operator|->
name|headers_out
operator|.
name|content_type
argument_list|,
name|ngx_push_table
argument_list|(
name|r
operator|->
name|headers_out
operator|.
name|headers
argument_list|)
argument_list|,
name|NGX_HTTP_INTERNAL_SERVER_ERROR
argument_list|)
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|content_type
operator|->
name|key
operator|.
name|len
operator|=
literal|0
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|content_type
operator|->
name|key
operator|.
name|data
operator|=
name|NULL
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|content_type
operator|->
name|value
operator|.
name|len
operator|=
literal|0
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|content_type
operator|->
name|value
operator|.
name|data
operator|=
name|NULL
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
if|if
condition|(
name|r
operator|->
name|exten
operator|.
name|len
condition|)
block|{
name|ngx_http_types_hash_key
argument_list|(
name|key
argument_list|,
name|r
operator|->
name|exten
argument_list|)
expr_stmt|;
name|type
operator|=
operator|(
name|ngx_http_type_t
operator|*
operator|)
name|clcf
operator|->
name|types
index|[
name|key
index|]
operator|.
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
name|clcf
operator|->
name|types
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
name|r
operator|->
name|exten
operator|.
name|len
operator|!=
name|type
index|[
name|i
index|]
operator|.
name|exten
operator|.
name|len
condition|)
block|{
continue|continue;
block|}
if|if
condition|(
name|ngx_strcasecmp
argument_list|(
name|r
operator|->
name|exten
operator|.
name|data
argument_list|,
name|type
index|[
name|i
index|]
operator|.
name|exten
operator|.
name|data
argument_list|)
operator|==
literal|0
condition|)
block|{
name|r
operator|->
name|headers_out
operator|.
name|content_type
operator|->
name|value
operator|.
name|len
operator|=
name|type
index|[
name|i
index|]
operator|.
name|type
operator|.
name|len
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|content_type
operator|->
name|value
operator|.
name|data
operator|=
name|type
index|[
name|i
index|]
operator|.
name|type
operator|.
name|data
expr_stmt|;
break|break;
block|}
block|}
block|}
if|if
condition|(
name|r
operator|->
name|headers_out
operator|.
name|content_type
operator|->
name|value
operator|.
name|len
operator|==
literal|0
condition|)
block|{
name|r
operator|->
name|headers_out
operator|.
name|content_type
operator|->
name|value
operator|.
name|len
operator|=
name|clcf
operator|->
name|default_type
operator|.
name|len
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|content_type
operator|->
name|value
operator|.
name|data
operator|=
name|clcf
operator|->
name|default_type
operator|.
name|data
expr_stmt|;
block|}
comment|/* we need to allocate all before the header would be sent */
name|ngx_test_null
argument_list|(
name|h
argument_list|,
name|ngx_pcalloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_hunk_t
argument_list|)
argument_list|)
argument_list|,
name|NGX_HTTP_INTERNAL_SERVER_ERROR
argument_list|)
expr_stmt|;
name|ngx_test_null
argument_list|(
name|h
operator|->
name|file
argument_list|,
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
argument_list|,
name|NGX_HTTP_INTERNAL_SERVER_ERROR
argument_list|)
expr_stmt|;
name|rc
operator|=
name|ngx_http_send_header
argument_list|(
name|r
argument_list|)
expr_stmt|;
if|if
condition|(
name|r
operator|->
name|header_only
condition|)
block|{
if|if
condition|(
name|rc
operator|==
name|NGX_AGAIN
condition|)
block|{
name|ngx_http_set_write_handler
argument_list|(
name|r
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|ngx_http_finalize_request
argument_list|(
name|r
argument_list|,
literal|0
argument_list|)
expr_stmt|;
block|}
return|return
name|NGX_OK
return|;
block|}
name|h
operator|->
name|type
operator|=
name|NGX_HUNK_FILE
operator||
name|NGX_HUNK_LAST
expr_stmt|;
name|h
operator|->
name|file_pos
operator|=
literal|0
expr_stmt|;
name|h
operator|->
name|file_last
operator|=
name|ngx_file_size
argument_list|(
name|r
operator|->
name|file
operator|.
name|info
argument_list|)
expr_stmt|;
name|h
operator|->
name|file
operator|->
name|fd
operator|=
name|r
operator|->
name|file
operator|.
name|fd
expr_stmt|;
name|h
operator|->
name|file
operator|->
name|log
operator|=
name|r
operator|->
name|connection
operator|->
name|log
expr_stmt|;
name|rc
operator|=
name|ngx_http_output_filter
argument_list|(
name|r
argument_list|,
name|h
argument_list|)
expr_stmt|;
if|if
condition|(
name|r
operator|->
expr|main
operator|==
name|NULL
condition|)
block|{
if|if
condition|(
name|rc
operator|==
name|NGX_AGAIN
condition|)
block|{
name|ngx_http_set_write_handler
argument_list|(
name|r
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|ngx_http_finalize_request
argument_list|(
name|r
argument_list|,
literal|0
argument_list|)
expr_stmt|;
block|}
block|}
return|return
name|NGX_OK
return|;
block|}
end_function

end_unit

