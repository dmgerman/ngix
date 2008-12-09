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

begin_function_decl
specifier|static
name|void
name|ngx_http_read_client_request_body_handler
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
name|ngx_http_do_read_client_request_body
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
name|ngx_http_write_request_body
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_chain_t
modifier|*
name|body
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|void
name|ngx_http_read_discarded_request_body_handler
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
name|ngx_http_read_discarded_request_body
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * on completion ngx_http_read_client_request_body() adds to  * r->request_body->bufs one or two bufs:  *    *) one memory buf that was preread in r->header_in;  *    *) one memory or file buf that contains the rest of the body  */
end_comment

begin_function
name|ngx_int_t
DECL|function|ngx_http_read_client_request_body (ngx_http_request_t * r,ngx_http_client_body_handler_pt post_handler)
name|ngx_http_read_client_request_body
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_client_body_handler_pt
name|post_handler
parameter_list|)
block|{
name|size_t
name|preread
decl_stmt|;
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
decl_stmt|,
modifier|*
modifier|*
name|next
decl_stmt|;
name|ngx_temp_file_t
modifier|*
name|tf
decl_stmt|;
name|ngx_http_request_body_t
modifier|*
name|rb
decl_stmt|;
name|ngx_http_core_loc_conf_t
modifier|*
name|clcf
decl_stmt|;
if|if
condition|(
name|r
operator|->
name|request_body
operator|||
name|r
operator|->
name|discard_body
condition|)
block|{
name|post_handler
argument_list|(
name|r
argument_list|)
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
name|rb
operator|=
name|ngx_pcalloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_request_body_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|rb
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
name|r
operator|->
name|request_body
operator|=
name|rb
expr_stmt|;
if|if
condition|(
name|r
operator|->
name|headers_in
operator|.
name|content_length_n
operator|<
literal|0
condition|)
block|{
name|post_handler
argument_list|(
name|r
argument_list|)
expr_stmt|;
return|return
name|NGX_OK
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
name|r
operator|->
name|headers_in
operator|.
name|content_length_n
operator|==
literal|0
condition|)
block|{
if|if
condition|(
name|r
operator|->
name|request_body_in_file_only
condition|)
block|{
name|tf
operator|=
name|ngx_pcalloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_temp_file_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|tf
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
name|tf
operator|->
name|file
operator|.
name|fd
operator|=
name|NGX_INVALID_FILE
expr_stmt|;
name|tf
operator|->
name|file
operator|.
name|log
operator|=
name|r
operator|->
name|connection
operator|->
name|log
expr_stmt|;
name|tf
operator|->
name|path
operator|=
name|clcf
operator|->
name|client_body_temp_path
expr_stmt|;
name|tf
operator|->
name|pool
operator|=
name|r
operator|->
name|pool
expr_stmt|;
name|tf
operator|->
name|warn
operator|=
literal|"a client request body is buffered to a temporary file"
expr_stmt|;
name|tf
operator|->
name|log_level
operator|=
name|r
operator|->
name|request_body_file_log_level
expr_stmt|;
name|tf
operator|->
name|persistent
operator|=
name|r
operator|->
name|request_body_in_persistent_file
expr_stmt|;
name|tf
operator|->
name|clean
operator|=
name|r
operator|->
name|request_body_in_clean_file
expr_stmt|;
if|if
condition|(
name|r
operator|->
name|request_body_file_group_access
condition|)
block|{
name|tf
operator|->
name|access
operator|=
literal|0660
expr_stmt|;
block|}
name|rb
operator|->
name|temp_file
operator|=
name|tf
expr_stmt|;
if|if
condition|(
name|ngx_create_temp_file
argument_list|(
operator|&
name|tf
operator|->
name|file
argument_list|,
name|tf
operator|->
name|path
argument_list|,
name|tf
operator|->
name|pool
argument_list|,
name|tf
operator|->
name|persistent
argument_list|,
name|tf
operator|->
name|clean
argument_list|,
name|tf
operator|->
name|access
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
block|}
name|post_handler
argument_list|(
name|r
argument_list|)
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
name|rb
operator|->
name|post_handler
operator|=
name|post_handler
expr_stmt|;
comment|/*      * set by ngx_pcalloc():      *      *     rb->bufs = NULL;      *     rb->buf = NULL;      *     rb->rest = 0;      */
name|preread
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
name|preread
condition|)
block|{
comment|/* there is the pre-read part of the request body */
name|ngx_log_debug1
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
literal|"http client request body preread %uz"
argument_list|,
name|preread
argument_list|)
expr_stmt|;
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
name|temporary
operator|=
literal|1
expr_stmt|;
name|b
operator|->
name|start
operator|=
name|r
operator|->
name|header_in
operator|->
name|pos
expr_stmt|;
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
name|last
operator|=
name|r
operator|->
name|header_in
operator|->
name|last
expr_stmt|;
name|b
operator|->
name|end
operator|=
name|r
operator|->
name|header_in
operator|->
name|end
expr_stmt|;
name|rb
operator|->
name|bufs
operator|=
name|ngx_alloc_chain_link
argument_list|(
name|r
operator|->
name|pool
argument_list|)
expr_stmt|;
if|if
condition|(
name|rb
operator|->
name|bufs
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
name|rb
operator|->
name|bufs
operator|->
name|buf
operator|=
name|b
expr_stmt|;
name|rb
operator|->
name|bufs
operator|->
name|next
operator|=
name|NULL
expr_stmt|;
name|rb
operator|->
name|buf
operator|=
name|b
expr_stmt|;
if|if
condition|(
operator|(
name|off_t
operator|)
name|preread
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
operator|(
name|size_t
operator|)
name|r
operator|->
name|headers_in
operator|.
name|content_length_n
expr_stmt|;
name|r
operator|->
name|request_length
operator|+=
name|r
operator|->
name|headers_in
operator|.
name|content_length_n
expr_stmt|;
if|if
condition|(
name|r
operator|->
name|request_body_in_file_only
condition|)
block|{
if|if
condition|(
name|ngx_http_write_request_body
argument_list|(
name|r
argument_list|,
name|rb
operator|->
name|bufs
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
block|}
name|post_handler
argument_list|(
name|r
argument_list|)
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
comment|/*          * to not consider the body as pipelined request in          * ngx_http_set_keepalive()          */
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
name|r
operator|->
name|request_length
operator|+=
name|preread
expr_stmt|;
name|rb
operator|->
name|rest
operator|=
name|r
operator|->
name|headers_in
operator|.
name|content_length_n
operator|-
name|preread
expr_stmt|;
if|if
condition|(
name|rb
operator|->
name|rest
operator|<=
operator|(
name|off_t
operator|)
operator|(
name|b
operator|->
name|end
operator|-
name|b
operator|->
name|last
operator|)
condition|)
block|{
comment|/* the whole request body may be placed in r->header_in */
name|rb
operator|->
name|to_write
operator|=
name|rb
operator|->
name|bufs
expr_stmt|;
name|r
operator|->
name|read_event_handler
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
name|next
operator|=
operator|&
name|rb
operator|->
name|bufs
operator|->
name|next
expr_stmt|;
block|}
else|else
block|{
name|b
operator|=
name|NULL
expr_stmt|;
name|rb
operator|->
name|rest
operator|=
name|r
operator|->
name|headers_in
operator|.
name|content_length_n
expr_stmt|;
name|next
operator|=
operator|&
name|rb
operator|->
name|bufs
expr_stmt|;
block|}
name|size
operator|=
name|clcf
operator|->
name|client_body_buffer_size
expr_stmt|;
name|size
operator|+=
name|size
operator|>>
literal|2
expr_stmt|;
if|if
condition|(
name|rb
operator|->
name|rest
operator|<
name|size
condition|)
block|{
name|size
operator|=
operator|(
name|ssize_t
operator|)
name|rb
operator|->
name|rest
expr_stmt|;
if|if
condition|(
name|r
operator|->
name|request_body_in_single_buf
condition|)
block|{
name|size
operator|+=
name|preread
expr_stmt|;
block|}
block|}
else|else
block|{
name|size
operator|=
name|clcf
operator|->
name|client_body_buffer_size
expr_stmt|;
comment|/* disable copying buffer for r->request_body_in_single_buf */
name|b
operator|=
name|NULL
expr_stmt|;
block|}
name|rb
operator|->
name|buf
operator|=
name|ngx_create_temp_buf
argument_list|(
name|r
operator|->
name|pool
argument_list|,
name|size
argument_list|)
expr_stmt|;
if|if
condition|(
name|rb
operator|->
name|buf
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
name|cl
operator|=
name|ngx_alloc_chain_link
argument_list|(
name|r
operator|->
name|pool
argument_list|)
expr_stmt|;
if|if
condition|(
name|cl
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
name|cl
operator|->
name|buf
operator|=
name|rb
operator|->
name|buf
expr_stmt|;
name|cl
operator|->
name|next
operator|=
name|NULL
expr_stmt|;
if|if
condition|(
name|b
operator|&&
name|r
operator|->
name|request_body_in_single_buf
condition|)
block|{
name|size
operator|=
name|b
operator|->
name|last
operator|-
name|b
operator|->
name|pos
expr_stmt|;
name|ngx_memcpy
argument_list|(
name|rb
operator|->
name|buf
operator|->
name|pos
argument_list|,
name|b
operator|->
name|pos
argument_list|,
name|size
argument_list|)
expr_stmt|;
name|rb
operator|->
name|buf
operator|->
name|last
operator|+=
name|size
expr_stmt|;
name|next
operator|=
operator|&
name|rb
operator|->
name|bufs
expr_stmt|;
block|}
operator|*
name|next
operator|=
name|cl
expr_stmt|;
if|if
condition|(
name|r
operator|->
name|request_body_in_file_only
operator|||
name|r
operator|->
name|request_body_in_single_buf
condition|)
block|{
name|rb
operator|->
name|to_write
operator|=
name|rb
operator|->
name|bufs
expr_stmt|;
block|}
else|else
block|{
name|rb
operator|->
name|to_write
operator|=
name|rb
operator|->
name|bufs
operator|->
name|next
condition|?
name|rb
operator|->
name|bufs
operator|->
name|next
else|:
name|rb
operator|->
name|bufs
expr_stmt|;
block|}
name|r
operator|->
name|read_event_handler
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
specifier|static
name|void
DECL|function|ngx_http_read_client_request_body_handler (ngx_http_request_t * r)
name|ngx_http_read_client_request_body_handler
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
block|{
name|ngx_int_t
name|rc
decl_stmt|;
if|if
condition|(
name|r
operator|->
name|connection
operator|->
name|read
operator|->
name|timedout
condition|)
block|{
name|r
operator|->
name|connection
operator|->
name|timedout
operator|=
literal|1
expr_stmt|;
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
specifier|static
name|ngx_int_t
DECL|function|ngx_http_do_read_client_request_body (ngx_http_request_t * r)
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
name|ngx_http_request_body_t
modifier|*
name|rb
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
name|rb
operator|=
name|r
operator|->
name|request_body
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
for|for
control|(
init|;
condition|;
control|)
block|{
if|if
condition|(
name|rb
operator|->
name|buf
operator|->
name|last
operator|==
name|rb
operator|->
name|buf
operator|->
name|end
condition|)
block|{
if|if
condition|(
name|ngx_http_write_request_body
argument_list|(
name|r
argument_list|,
name|rb
operator|->
name|to_write
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
name|rb
operator|->
name|to_write
operator|=
name|rb
operator|->
name|bufs
operator|->
name|next
condition|?
name|rb
operator|->
name|bufs
operator|->
name|next
else|:
name|rb
operator|->
name|bufs
expr_stmt|;
name|rb
operator|->
name|buf
operator|->
name|last
operator|=
name|rb
operator|->
name|buf
operator|->
name|start
expr_stmt|;
block|}
name|size
operator|=
name|rb
operator|->
name|buf
operator|->
name|end
operator|-
name|rb
operator|->
name|buf
operator|->
name|last
expr_stmt|;
if|if
condition|(
operator|(
name|off_t
operator|)
name|size
operator|>
name|rb
operator|->
name|rest
condition|)
block|{
name|size
operator|=
operator|(
name|size_t
operator|)
name|rb
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
name|rb
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
literal|"http client request body recv %z"
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
break|break;
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
name|c
operator|->
name|error
operator|=
literal|1
expr_stmt|;
return|return
name|NGX_HTTP_BAD_REQUEST
return|;
block|}
name|rb
operator|->
name|buf
operator|->
name|last
operator|+=
name|n
expr_stmt|;
name|rb
operator|->
name|rest
operator|-=
name|n
expr_stmt|;
name|r
operator|->
name|request_length
operator|+=
name|n
expr_stmt|;
if|if
condition|(
name|rb
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
name|rb
operator|->
name|buf
operator|->
name|last
operator|<
name|rb
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
literal|"http client request body rest %O"
argument_list|,
name|rb
operator|->
name|rest
argument_list|)
expr_stmt|;
if|if
condition|(
name|rb
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
operator|!
name|c
operator|->
name|read
operator|->
name|ready
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
operator|!=
name|NGX_OK
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
block|}
if|if
condition|(
name|c
operator|->
name|read
operator|->
name|timer_set
condition|)
block|{
name|ngx_del_timer
argument_list|(
name|c
operator|->
name|read
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|rb
operator|->
name|temp_file
operator|||
name|r
operator|->
name|request_body_in_file_only
condition|)
block|{
comment|/* save the last part */
if|if
condition|(
name|ngx_http_write_request_body
argument_list|(
name|r
argument_list|,
name|rb
operator|->
name|to_write
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
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
name|rb
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
name|rb
operator|->
name|temp_file
operator|->
name|file
expr_stmt|;
if|if
condition|(
name|rb
operator|->
name|bufs
operator|->
name|next
condition|)
block|{
name|rb
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
name|rb
operator|->
name|bufs
operator|->
name|buf
operator|=
name|b
expr_stmt|;
block|}
block|}
if|if
condition|(
name|r
operator|->
name|request_body_in_file_only
operator|&&
name|rb
operator|->
name|bufs
operator|->
name|next
condition|)
block|{
name|rb
operator|->
name|bufs
operator|=
name|rb
operator|->
name|bufs
operator|->
name|next
expr_stmt|;
block|}
name|rb
operator|->
name|post_handler
argument_list|(
name|r
argument_list|)
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_write_request_body (ngx_http_request_t * r,ngx_chain_t * body)
name|ngx_http_write_request_body
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_chain_t
modifier|*
name|body
parameter_list|)
block|{
name|ssize_t
name|n
decl_stmt|;
name|ngx_temp_file_t
modifier|*
name|tf
decl_stmt|;
name|ngx_http_request_body_t
modifier|*
name|rb
decl_stmt|;
name|ngx_http_core_loc_conf_t
modifier|*
name|clcf
decl_stmt|;
name|rb
operator|=
name|r
operator|->
name|request_body
expr_stmt|;
if|if
condition|(
name|rb
operator|->
name|temp_file
operator|==
name|NULL
condition|)
block|{
name|tf
operator|=
name|ngx_pcalloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_temp_file_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|tf
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
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
name|tf
operator|->
name|file
operator|.
name|fd
operator|=
name|NGX_INVALID_FILE
expr_stmt|;
name|tf
operator|->
name|file
operator|.
name|log
operator|=
name|r
operator|->
name|connection
operator|->
name|log
expr_stmt|;
name|tf
operator|->
name|path
operator|=
name|clcf
operator|->
name|client_body_temp_path
expr_stmt|;
name|tf
operator|->
name|pool
operator|=
name|r
operator|->
name|pool
expr_stmt|;
name|tf
operator|->
name|warn
operator|=
literal|"a client request body is buffered to a temporary file"
expr_stmt|;
name|tf
operator|->
name|log_level
operator|=
name|r
operator|->
name|request_body_file_log_level
expr_stmt|;
name|tf
operator|->
name|persistent
operator|=
name|r
operator|->
name|request_body_in_persistent_file
expr_stmt|;
name|tf
operator|->
name|clean
operator|=
name|r
operator|->
name|request_body_in_clean_file
expr_stmt|;
if|if
condition|(
name|r
operator|->
name|request_body_file_group_access
condition|)
block|{
name|tf
operator|->
name|access
operator|=
literal|0660
expr_stmt|;
block|}
name|rb
operator|->
name|temp_file
operator|=
name|tf
expr_stmt|;
block|}
name|n
operator|=
name|ngx_write_chain_to_temp_file
argument_list|(
name|rb
operator|->
name|temp_file
argument_list|,
name|body
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
name|NGX_ERROR
return|;
block|}
name|rb
operator|->
name|temp_file
operator|->
name|offset
operator|+=
name|n
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
name|ngx_int_t
DECL|function|ngx_http_discard_request_body (ngx_http_request_t * r)
name|ngx_http_discard_request_body
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
block|{
name|ssize_t
name|size
decl_stmt|;
name|ngx_event_t
modifier|*
name|rev
decl_stmt|;
if|if
condition|(
name|r
operator|!=
name|r
operator|->
expr|main
operator|||
name|r
operator|->
name|discard_body
condition|)
block|{
return|return
name|NGX_OK
return|;
block|}
name|rev
operator|=
name|r
operator|->
name|connection
operator|->
name|read
expr_stmt|;
name|ngx_log_debug0
argument_list|(
name|NGX_LOG_DEBUG_HTTP
argument_list|,
name|rev
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"http set discard body"
argument_list|)
expr_stmt|;
if|if
condition|(
name|rev
operator|->
name|timer_set
condition|)
block|{
name|ngx_del_timer
argument_list|(
name|rev
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|r
operator|->
name|headers_in
operator|.
name|content_length_n
operator|<=
literal|0
operator|||
name|r
operator|->
name|request_body
condition|)
block|{
return|return
name|NGX_OK
return|;
block|}
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
if|if
condition|(
name|r
operator|->
name|headers_in
operator|.
name|content_length_n
operator|>
name|size
condition|)
block|{
name|r
operator|->
name|headers_in
operator|.
name|content_length_n
operator|-=
name|size
expr_stmt|;
block|}
else|else
block|{
name|r
operator|->
name|header_in
operator|->
name|pos
operator|+=
operator|(
name|size_t
operator|)
name|r
operator|->
name|headers_in
operator|.
name|content_length_n
expr_stmt|;
name|r
operator|->
name|headers_in
operator|.
name|content_length_n
operator|=
literal|0
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
block|}
name|r
operator|->
name|discard_body
operator|=
literal|1
expr_stmt|;
name|r
operator|->
name|read_event_handler
operator|=
name|ngx_http_read_discarded_request_body_handler
expr_stmt|;
if|if
condition|(
name|ngx_handle_read_event
argument_list|(
name|rev
argument_list|,
literal|0
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
operator|(
name|void
operator|)
name|ngx_http_read_discarded_request_body
argument_list|(
name|r
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
DECL|function|ngx_http_read_discarded_request_body_handler (ngx_http_request_t * r)
name|ngx_http_read_discarded_request_body_handler
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
block|{
name|ngx_int_t
name|rc
decl_stmt|;
name|ngx_msec_t
name|timer
decl_stmt|;
name|ngx_event_t
modifier|*
name|rev
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
name|rev
operator|=
name|c
operator|->
name|read
expr_stmt|;
if|if
condition|(
name|rev
operator|->
name|timedout
condition|)
block|{
name|c
operator|->
name|timedout
operator|=
literal|1
expr_stmt|;
name|c
operator|->
name|error
operator|=
literal|1
expr_stmt|;
name|ngx_http_finalize_request
argument_list|(
name|r
argument_list|,
literal|0
argument_list|)
expr_stmt|;
return|return;
block|}
if|if
condition|(
name|r
operator|->
name|lingering_time
condition|)
block|{
name|timer
operator|=
operator|(
name|ngx_msec_t
operator|)
operator|(
name|r
operator|->
name|lingering_time
operator|-
name|ngx_time
argument_list|()
operator|)
expr_stmt|;
if|if
condition|(
name|timer
operator|<=
literal|0
condition|)
block|{
name|r
operator|->
name|discard_body
operator|=
literal|0
expr_stmt|;
name|ngx_http_finalize_request
argument_list|(
name|r
argument_list|,
literal|0
argument_list|)
expr_stmt|;
return|return;
block|}
block|}
else|else
block|{
name|timer
operator|=
literal|0
expr_stmt|;
block|}
name|rc
operator|=
name|ngx_http_read_discarded_request_body
argument_list|(
name|r
argument_list|)
expr_stmt|;
if|if
condition|(
name|rc
operator|==
name|NGX_OK
condition|)
block|{
name|r
operator|->
name|discard_body
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|r
operator|->
name|done
condition|)
block|{
name|ngx_http_finalize_request
argument_list|(
name|r
argument_list|,
literal|0
argument_list|)
expr_stmt|;
block|}
return|return;
block|}
comment|/* rc == NGX_AGAIN */
if|if
condition|(
name|ngx_handle_read_event
argument_list|(
name|rev
argument_list|,
literal|0
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
name|c
operator|->
name|error
operator|=
literal|1
expr_stmt|;
name|ngx_http_finalize_request
argument_list|(
name|r
argument_list|,
name|rc
argument_list|)
expr_stmt|;
return|return;
block|}
if|if
condition|(
name|timer
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
name|timer
operator|*=
literal|1000
expr_stmt|;
if|if
condition|(
name|timer
operator|>
name|clcf
operator|->
name|lingering_timeout
condition|)
block|{
name|timer
operator|=
name|clcf
operator|->
name|lingering_timeout
expr_stmt|;
block|}
name|ngx_add_timer
argument_list|(
name|rev
argument_list|,
name|timer
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_read_discarded_request_body (ngx_http_request_t * r)
name|ngx_http_read_discarded_request_body
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
name|u_char
name|buffer
index|[
name|NGX_HTTP_DISCARD_BUFFER_SIZE
index|]
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
literal|"http read discarded body"
argument_list|)
expr_stmt|;
do|do
block|{
if|if
condition|(
name|r
operator|->
name|headers_in
operator|.
name|content_length_n
operator|==
literal|0
condition|)
block|{
name|r
operator|->
name|read_event_handler
operator|=
name|ngx_http_block_reading
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
name|size
operator|=
operator|(
name|r
operator|->
name|headers_in
operator|.
name|content_length_n
operator|>
name|NGX_HTTP_DISCARD_BUFFER_SIZE
operator|)
condition|?
name|NGX_HTTP_DISCARD_BUFFER_SIZE
else|:
operator|(
name|size_t
operator|)
name|r
operator|->
name|headers_in
operator|.
name|content_length_n
expr_stmt|;
name|n
operator|=
name|r
operator|->
name|connection
operator|->
name|recv
argument_list|(
name|r
operator|->
name|connection
argument_list|,
name|buffer
argument_list|,
name|size
argument_list|)
expr_stmt|;
if|if
condition|(
name|n
operator|==
name|NGX_ERROR
condition|)
block|{
name|r
operator|->
name|connection
operator|->
name|error
operator|=
literal|1
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
if|if
condition|(
name|n
operator|==
name|NGX_AGAIN
condition|)
block|{
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
return|return
name|NGX_OK
return|;
block|}
name|r
operator|->
name|headers_in
operator|.
name|content_length_n
operator|-=
name|n
expr_stmt|;
block|}
do|while
condition|(
name|r
operator|->
name|connection
operator|->
name|read
operator|->
name|ready
condition|)
do|;
return|return
name|NGX_AGAIN
return|;
block|}
end_function

end_unit

