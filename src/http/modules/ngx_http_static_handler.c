begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|<ngx_config.h>
end_include

begin_include
include|#
directive|include
file|<ngx_string.h>
end_include

begin_include
include|#
directive|include
file|<ngx_file.h>
end_include

begin_include
include|#
directive|include
file|<ngx_hunk.h>
end_include

begin_include
include|#
directive|include
file|<ngx_http.h>
end_include

begin_include
include|#
directive|include
file|<ngx_http_output_filter.h>
end_include

begin_decl_stmt
DECL|variable|ngx_http_static_module
name|ngx_http_module_t
name|ngx_http_static_module
decl_stmt|;
end_decl_stmt

begin_if
if|#
directive|if
literal|0
end_if

begin_comment
comment|/* STUB */
end_comment

begin_comment
unit|static ngx_http_static_ctx_t module_ctx;  void ngx_http_static_init() {      module_ctx.out = NULL;       ngx_http_static_module.ctx =&module_ctx; }
comment|/* */
end_comment

begin_endif
endif|#
directive|endif
end_endif

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
decl_stmt|;
name|ngx_err_t
name|err
decl_stmt|;
name|ngx_hunk_t
modifier|*
name|h
decl_stmt|;
name|ngx_http_log_ctx_t
modifier|*
name|ctx
decl_stmt|;
if|#
directive|if
literal|0
block_content|ngx_http_event_static_handler_loc_conf_t  *cf;      cf = (ngx_http_event_static_handler_loc_conf_t *)              ngx_get_module_loc_conf(r,&ngx_http_event_static_handler_module);
endif|#
directive|endif
name|ngx_http_discard_body
argument_list|(
name|r
argument_list|)
expr_stmt|;
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
name|ngx_log_error
argument_list|(
name|NGX_LOG_ERR
argument_list|,
name|r
operator|->
name|connection
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"ngx_http_static_handler: "
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
if|if
condition|(
name|err
operator|==
name|NGX_ENOENT
condition|)
return|return
name|NGX_HTTP_NOT_FOUND
return|;
if|#
directive|if
operator|(
name|WIN32
operator|)
if|else if
condition|(
name|err
operator|==
name|ERROR_PATH_NOT_FOUND
condition|)
return|return
name|NGX_HTTP_NOT_FOUND
return|;
endif|#
directive|endif
else|else
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
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
name|NGX_LOG_ERR
argument_list|,
name|r
operator|->
name|connection
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"ngx_http_static_handler: "
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
name|NGX_LOG_ERR
argument_list|,
name|r
operator|->
name|connection
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"ngx_http_static_handler: "
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
comment|/* it's probably Unix specific */
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
name|NGX_LOG_ERR
argument_list|,
name|r
operator|->
name|connection
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"ngx_http_static_handler: "
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
name|NGX_LOG_ERR
argument_list|,
name|r
operator|->
name|connection
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"ngx_http_static_handler: "
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
literal|12
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
literal|"Content-Type"
expr_stmt|;
comment|/* STUB */
if|if
condition|(
name|r
operator|->
name|exten
operator|.
name|len
condition|)
block|{
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
literal|"html"
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
literal|25
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
literal|"text/html; charset=koi8-r"
expr_stmt|;
block|}
if|else if
condition|(
name|ngx_strcasecmp
argument_list|(
name|r
operator|->
name|exten
operator|.
name|data
argument_list|,
literal|"gif"
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
literal|9
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
literal|"image/gif"
expr_stmt|;
block|}
if|else if
condition|(
name|ngx_strcasecmp
argument_list|(
name|r
operator|->
name|exten
operator|.
name|data
argument_list|,
literal|"jpg"
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
literal|10
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
literal|"image/jpeg"
expr_stmt|;
block|}
if|else if
condition|(
name|ngx_strcasecmp
argument_list|(
name|r
operator|->
name|exten
operator|.
name|data
argument_list|,
literal|"pdf"
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
literal|15
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
literal|"application/pdf"
expr_stmt|;
block|}
block|}
else|else
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
literal|25
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
literal|"text/html; charset=koi8-r"
expr_stmt|;
block|}
comment|/**/
comment|/* we need to allocate them before header would be sent */
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
return|return
name|rc
return|;
if|#
directive|if
literal|1
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
name|pos
operator|.
name|file
operator|=
literal|0
expr_stmt|;
name|h
operator|->
name|last
operator|.
name|file
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
name|ngx_log_debug
argument_list|(
argument|r->connection->log
argument_list|,
literal|"0 output_filter: %d"
argument|_ rc
argument_list|)
empty_stmt|;
else|#
directive|else
DECL|macro|BLK
define|#
directive|define
name|BLK
value|10000
block|{
name|int
name|i
decl_stmt|,
name|s
decl_stmt|;
name|s
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
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|s
condition|;
name|i
operator|+=
name|BLK
control|)
block|{
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
name|h
operator|->
name|type
operator|=
name|NGX_HUNK_FILE
expr_stmt|;
if|if
condition|(
name|s
operator|-
name|i
operator|<=
name|BLK
condition|)
block|{
name|h
operator|->
name|type
operator||=
name|NGX_HUNK_LAST
expr_stmt|;
block|}
name|h
operator|->
name|pos
operator|.
name|file
operator|=
name|i
expr_stmt|;
name|h
operator|->
name|last
operator|.
name|file
operator|=
name|i
operator|+
name|BLK
expr_stmt|;
if|if
condition|(
name|h
operator|->
name|last
operator|.
name|file
operator|>
name|s
condition|)
block|{
name|h
operator|->
name|last
operator|.
name|file
operator|=
name|s
expr_stmt|;
block|}
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
name|ngx_log_debug
argument_list|(
argument|r->connection->log
argument_list|,
literal|"0 output_filter: %d"
argument|_ rc
argument_list|)
empty_stmt|;
block|}
block|}
endif|#
directive|endif
return|return
name|rc
return|;
block|}
end_function

end_unit

