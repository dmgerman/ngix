begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (C) 2002-2004 Igor Sysoev  */
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
file|<ngx_event.h>
end_include

begin_include
include|#
directive|include
file|<ngx_http.h>
end_include

begin_function_decl
specifier|static
name|void
name|ngx_http_read_client_request_body_handler
parameter_list|(
name|ngx_event_t
modifier|*
name|rev
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_do_read_client_request_body
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
function_decl|;
end_function_decl

begin_function
DECL|function|ngx_http_read_client_request_body (ngx_http_request_t * r)
name|ngx_int_t
name|ngx_http_read_client_request_body
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
block|{
name|ssize_t
name|size
decl_stmt|;
name|ngx_buf_t
modifier|*
name|b
decl_stmt|;
name|ngx_chain_t
modifier|*
name|cl
decl_stmt|;
name|ngx_http_core_loc_conf_t
modifier|*
name|clcf
decl_stmt|;
name|size
operator|=
name|r
operator|->
name|header_in
operator|->
name|last
operator|-
name|r
operator|->
name|header_in
operator|->
name|pos
expr_stmt|;
if|if
condition|(
name|size
condition|)
block|{
comment|/* there is the pre-read part of the request body */
name|ngx_test_null
argument_list|(
name|b
argument_list|,
name|ngx_calloc_buf
argument_list|(
name|r
operator|->
name|pool
argument_list|)
argument_list|,
name|NGX_HTTP_INTERNAL_SERVER_ERROR
argument_list|)
expr_stmt|;
name|b
operator|->
name|temporary
operator|=
literal|1
expr_stmt|;
name|b
operator|->
name|start
operator|=
name|b
operator|->
name|pos
operator|=
name|r
operator|->
name|header_in
operator|->
name|pos
expr_stmt|;
name|b
operator|->
name|end
operator|=
name|b
operator|->
name|last
operator|=
name|r
operator|->
name|header_in
operator|->
name|last
expr_stmt|;
name|ngx_alloc_link_and_set_buf
argument_list|(
name|r
operator|->
name|request_body
operator|->
name|bufs
argument_list|,
name|b
argument_list|,
name|r
operator|->
name|pool
argument_list|,
name|NGX_HTTP_INTERNAL_SERVER_ERROR
argument_list|)
expr_stmt|;
if|if
condition|(
name|size
operator|>=
name|r
operator|->
name|headers_in
operator|.
name|content_length_n
condition|)
block|{
comment|/* the whole request body was pre-read */
name|r
operator|->
name|header_in
operator|->
name|pos
operator|+=
name|r
operator|->
name|headers_in
operator|.
name|content_length_n
expr_stmt|;
name|r
operator|->
name|request_body
operator|->
name|handler
argument_list|(
name|r
operator|->
name|request_body
operator|->
name|data
argument_list|)
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
name|r
operator|->
name|header_in
operator|->
name|pos
operator|=
name|r
operator|->
name|header_in
operator|->
name|last
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
name|r
operator|->
name|request_body
operator|->
name|rest
operator|=
name|r
operator|->
name|headers_in
operator|.
name|content_length_n
operator|-
name|size
expr_stmt|;
if|if
condition|(
name|r
operator|->
name|request_body
operator|->
name|rest
operator|<
name|clcf
operator|->
name|client_body_buffer_size
operator|+
operator|(
name|clcf
operator|->
name|client_body_buffer_size
operator|>>
literal|2
operator|)
condition|)
block|{
name|size
operator|=
name|r
operator|->
name|request_body
operator|->
name|rest
expr_stmt|;
block|}
else|else
block|{
name|size
operator|=
name|clcf
operator|->
name|client_body_buffer_size
expr_stmt|;
block|}
name|ngx_test_null
argument_list|(
name|r
operator|->
name|request_body
operator|->
name|buf
argument_list|,
name|ngx_create_temp_buf
argument_list|(
name|r
operator|->
name|pool
argument_list|,
name|size
argument_list|)
argument_list|,
name|NGX_HTTP_INTERNAL_SERVER_ERROR
argument_list|)
expr_stmt|;
name|ngx_alloc_link_and_set_buf
argument_list|(
name|cl
argument_list|,
name|r
operator|->
name|request_body
operator|->
name|buf
argument_list|,
name|r
operator|->
name|pool
argument_list|,
name|NGX_HTTP_INTERNAL_SERVER_ERROR
argument_list|)
expr_stmt|;
if|if
condition|(
name|r
operator|->
name|request_body
operator|->
name|bufs
condition|)
block|{
name|r
operator|->
name|request_body
operator|->
name|bufs
operator|->
name|next
operator|=
name|cl
expr_stmt|;
block|}
else|else
block|{
name|r
operator|->
name|request_body
operator|->
name|bufs
operator|=
name|cl
expr_stmt|;
block|}
name|r
operator|->
name|connection
operator|->
name|read
operator|->
name|event_handler
operator|=
name|ngx_http_read_client_request_body_handler
expr_stmt|;
return|return
name|ngx_http_do_read_client_request_body
argument_list|(
name|r
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_read_client_request_body_handler (ngx_event_t * rev)
specifier|static
name|void
name|ngx_http_read_client_request_body_handler
parameter_list|(
name|ngx_event_t
modifier|*
name|rev
parameter_list|)
block|{
name|ngx_int_t
name|rc
decl_stmt|;
name|ngx_connection_t
modifier|*
name|c
decl_stmt|;
name|ngx_http_request_t
modifier|*
name|r
decl_stmt|;
name|c
operator|=
name|rev
operator|->
name|data
expr_stmt|;
name|r
operator|=
name|c
operator|->
name|data
expr_stmt|;
if|if
condition|(
name|rev
operator|->
name|timedout
condition|)
block|{
name|ngx_http_finalize_request
argument_list|(
name|r
argument_list|,
name|NGX_HTTP_REQUEST_TIME_OUT
argument_list|)
expr_stmt|;
return|return;
block|}
name|rc
operator|=
name|ngx_http_do_read_client_request_body
argument_list|(
name|r
argument_list|)
expr_stmt|;
if|if
condition|(
name|rc
operator|>=
name|NGX_HTTP_SPECIAL_RESPONSE
condition|)
block|{
name|ngx_http_finalize_request
argument_list|(
name|r
argument_list|,
name|rc
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|ngx_http_do_read_client_request_body (ngx_http_request_t * r)
specifier|static
name|ngx_int_t
name|ngx_http_do_read_client_request_body
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
block|{
name|size_t
name|size
decl_stmt|;
name|ssize_t
name|n
decl_stmt|;
name|ngx_buf_t
modifier|*
name|b
decl_stmt|;
name|ngx_connection_t
modifier|*
name|c
decl_stmt|;
name|ngx_http_core_loc_conf_t
modifier|*
name|clcf
decl_stmt|;
name|c
operator|=
name|r
operator|->
name|connection
expr_stmt|;
name|ngx_log_debug0
argument_list|(
name|NGX_LOG_DEBUG_HTTP
argument_list|,
name|c
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"http read client request body"
argument_list|)
expr_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
if|if
condition|(
name|r
operator|->
name|request_body
operator|->
name|buf
operator|->
name|last
operator|==
name|r
operator|->
name|request_body
operator|->
name|buf
operator|->
name|end
condition|)
block|{
name|n
operator|=
name|ngx_write_chain_to_temp_file
argument_list|(
name|r
operator|->
name|request_body
operator|->
name|temp_file
argument_list|,
name|r
operator|->
name|request_body
operator|->
name|bufs
operator|->
name|next
condition|?
name|r
operator|->
name|request_body
operator|->
name|bufs
operator|->
name|next
else|:
name|r
operator|->
name|request_body
operator|->
name|bufs
argument_list|)
expr_stmt|;
comment|/* TODO: n == 0 or not complete and level event */
if|if
condition|(
name|n
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
name|r
operator|->
name|request_body
operator|->
name|temp_file
operator|->
name|offset
operator|+=
name|n
expr_stmt|;
name|r
operator|->
name|request_body
operator|->
name|buf
operator|->
name|pos
operator|=
name|r
operator|->
name|request_body
operator|->
name|buf
operator|->
name|start
expr_stmt|;
name|r
operator|->
name|request_body
operator|->
name|buf
operator|->
name|last
operator|=
name|r
operator|->
name|request_body
operator|->
name|buf
operator|->
name|start
expr_stmt|;
block|}
name|size
operator|=
name|r
operator|->
name|request_body
operator|->
name|buf
operator|->
name|end
operator|-
name|r
operator|->
name|request_body
operator|->
name|buf
operator|->
name|last
expr_stmt|;
if|if
condition|(
name|size
operator|>
name|r
operator|->
name|request_body
operator|->
name|rest
condition|)
block|{
name|size
operator|=
name|r
operator|->
name|request_body
operator|->
name|rest
expr_stmt|;
block|}
name|n
operator|=
name|c
operator|->
name|recv
argument_list|(
name|c
argument_list|,
name|r
operator|->
name|request_body
operator|->
name|buf
operator|->
name|last
argument_list|,
name|size
argument_list|)
expr_stmt|;
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_HTTP
argument_list|,
name|c
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"http client request body recv "
name|SIZE_T_FMT
argument_list|,
name|n
argument_list|)
expr_stmt|;
if|if
condition|(
name|n
operator|==
name|NGX_AGAIN
condition|)
block|{
name|clcf
operator|=
name|ngx_http_get_module_loc_conf
argument_list|(
name|r
argument_list|,
name|ngx_http_core_module
argument_list|)
expr_stmt|;
name|ngx_add_timer
argument_list|(
name|c
operator|->
name|read
argument_list|,
name|clcf
operator|->
name|client_body_timeout
argument_list|)
expr_stmt|;
if|if
condition|(
name|ngx_handle_read_event
argument_list|(
name|c
operator|->
name|read
argument_list|,
literal|0
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
return|return
name|NGX_AGAIN
return|;
block|}
if|if
condition|(
name|n
operator|==
literal|0
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_INFO
argument_list|,
name|c
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"client closed prematurely connection"
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|n
operator|==
literal|0
operator|||
name|n
operator|==
name|NGX_ERROR
condition|)
block|{
name|r
operator|->
name|closed
operator|=
literal|1
expr_stmt|;
return|return
name|NGX_HTTP_BAD_REQUEST
return|;
block|}
name|r
operator|->
name|request_body
operator|->
name|buf
operator|->
name|last
operator|+=
name|n
expr_stmt|;
name|r
operator|->
name|request_body
operator|->
name|rest
operator|-=
name|n
expr_stmt|;
if|if
condition|(
name|r
operator|->
name|request_body
operator|->
name|rest
operator|==
literal|0
condition|)
block|{
break|break;
block|}
if|if
condition|(
name|r
operator|->
name|request_body
operator|->
name|buf
operator|->
name|last
operator|<
name|r
operator|->
name|request_body
operator|->
name|buf
operator|->
name|end
condition|)
block|{
break|break;
block|}
block|}
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_HTTP
argument_list|,
name|c
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"http client request body rest "
name|SIZE_T_FMT
argument_list|,
name|r
operator|->
name|request_body
operator|->
name|rest
argument_list|)
expr_stmt|;
if|if
condition|(
name|r
operator|->
name|request_body
operator|->
name|rest
condition|)
block|{
return|return
name|NGX_AGAIN
return|;
block|}
if|if
condition|(
name|r
operator|->
name|request_body
operator|->
name|temp_file
operator|->
name|file
operator|.
name|fd
operator|!=
name|NGX_INVALID_FILE
condition|)
block|{
comment|/* save the last part */
name|n
operator|=
name|ngx_write_chain_to_temp_file
argument_list|(
name|r
operator|->
name|request_body
operator|->
name|temp_file
argument_list|,
name|r
operator|->
name|request_body
operator|->
name|bufs
operator|->
name|next
condition|?
name|r
operator|->
name|request_body
operator|->
name|bufs
operator|->
name|next
else|:
name|r
operator|->
name|request_body
operator|->
name|bufs
argument_list|)
expr_stmt|;
comment|/* TODO: n == 0 or not complete and level event */
if|if
condition|(
name|n
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
if|if
condition|(
operator|!
operator|(
name|b
operator|=
name|ngx_calloc_buf
argument_list|(
name|r
operator|->
name|pool
argument_list|)
operator|)
condition|)
block|{
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
name|b
operator|->
name|in_file
operator|=
literal|1
expr_stmt|;
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
name|r
operator|->
name|request_body
operator|->
name|temp_file
operator|->
name|file
operator|.
name|offset
expr_stmt|;
name|b
operator|->
name|file
operator|=
operator|&
name|r
operator|->
name|request_body
operator|->
name|temp_file
operator|->
name|file
expr_stmt|;
if|if
condition|(
name|r
operator|->
name|request_body
operator|->
name|bufs
operator|->
name|next
condition|)
block|{
name|r
operator|->
name|request_body
operator|->
name|bufs
operator|->
name|next
operator|->
name|buf
operator|=
name|b
expr_stmt|;
block|}
else|else
block|{
name|r
operator|->
name|request_body
operator|->
name|bufs
operator|->
name|buf
operator|=
name|b
expr_stmt|;
block|}
block|}
name|r
operator|->
name|request_body
operator|->
name|handler
argument_list|(
name|r
operator|->
name|request_body
operator|->
name|data
argument_list|)
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

end_unit

