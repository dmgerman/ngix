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
name|ngx_int_t
name|ngx_http_chunked_filter_init
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
function_decl|;
end_function_decl

begin_decl_stmt
DECL|variable|ngx_http_chunked_filter_module_ctx
specifier|static
name|ngx_http_module_t
name|ngx_http_chunked_filter_module_ctx
init|=
block|{
name|NULL
block|,
comment|/* pre conf */
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
block|,
comment|/* merge location configuration */
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_chunked_filter_module
name|ngx_module_t
name|ngx_http_chunked_filter_module
init|=
block|{
name|NGX_MODULE
block|,
operator|&
name|ngx_http_chunked_filter_module_ctx
block|,
comment|/* module context */
name|NULL
block|,
comment|/* module directives */
name|NGX_HTTP_MODULE
block|,
comment|/* module type */
name|ngx_http_chunked_filter_init
block|,
comment|/* init module */
name|NULL
comment|/* init process */
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

begin_decl_stmt
DECL|variable|ngx_http_next_body_filter
specifier|static
name|ngx_http_output_body_filter_pt
name|ngx_http_next_body_filter
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|ngx_http_chunked_header_filter (ngx_http_request_t * r)
specifier|static
name|ngx_int_t
name|ngx_http_chunked_header_filter
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
block|{
if|if
condition|(
name|r
operator|->
name|headers_out
operator|.
name|status
operator|==
name|NGX_HTTP_NOT_MODIFIED
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
name|r
operator|->
name|headers_out
operator|.
name|content_length_n
operator|==
operator|-
literal|1
condition|)
block|{
if|if
condition|(
name|r
operator|->
name|http_version
operator|<
name|NGX_HTTP_VERSION_11
condition|)
block|{
name|r
operator|->
name|keepalive
operator|=
literal|0
expr_stmt|;
block|}
else|else
block|{
name|r
operator|->
name|chunked
operator|=
literal|1
expr_stmt|;
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
DECL|function|ngx_http_chunked_body_filter (ngx_http_request_t * r,ngx_chain_t * in)
specifier|static
name|ngx_int_t
name|ngx_http_chunked_body_filter
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_chain_t
modifier|*
name|in
parameter_list|)
block|{
name|u_char
modifier|*
name|chunk
decl_stmt|;
name|off_t
name|size
decl_stmt|;
name|ngx_buf_t
modifier|*
name|b
decl_stmt|;
name|ngx_chain_t
name|out
decl_stmt|,
name|tail
decl_stmt|,
modifier|*
name|cl
decl_stmt|,
modifier|*
name|tl
decl_stmt|,
modifier|*
modifier|*
name|ll
decl_stmt|;
if|if
condition|(
name|in
operator|==
name|NULL
operator|||
operator|!
name|r
operator|->
name|chunked
condition|)
block|{
return|return
name|ngx_http_next_body_filter
argument_list|(
name|r
argument_list|,
name|in
argument_list|)
return|;
block|}
name|out
operator|.
name|buf
operator|=
name|NULL
expr_stmt|;
name|ll
operator|=
operator|&
name|out
operator|.
name|next
expr_stmt|;
name|size
operator|=
literal|0
expr_stmt|;
name|cl
operator|=
name|in
expr_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
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
literal|"http chunk: %d"
argument_list|,
name|ngx_buf_size
argument_list|(
name|cl
operator|->
name|buf
argument_list|)
argument_list|)
expr_stmt|;
name|size
operator|+=
name|ngx_buf_size
argument_list|(
name|cl
operator|->
name|buf
argument_list|)
expr_stmt|;
if|if
condition|(
name|cl
operator|->
name|buf
operator|->
name|flush
operator|||
name|ngx_buf_in_memory
argument_list|(
name|cl
operator|->
name|buf
argument_list|)
operator|||
name|cl
operator|->
name|buf
operator|->
name|in_file
condition|)
block|{
if|if
condition|(
operator|!
operator|(
name|tl
operator|=
name|ngx_alloc_chain_link
argument_list|(
name|r
operator|->
name|pool
argument_list|)
operator|)
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|tl
operator|->
name|buf
operator|=
name|cl
operator|->
name|buf
expr_stmt|;
operator|*
name|ll
operator|=
name|tl
expr_stmt|;
name|ll
operator|=
operator|&
name|tl
operator|->
name|next
expr_stmt|;
block|}
if|if
condition|(
name|cl
operator|->
name|next
operator|==
name|NULL
condition|)
block|{
break|break;
block|}
name|cl
operator|=
name|cl
operator|->
name|next
expr_stmt|;
block|}
if|if
condition|(
name|size
condition|)
block|{
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
name|NGX_ERROR
return|;
block|}
name|chunk
operator|=
name|ngx_palloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
literal|"0000000000000000"
name|CRLF
argument_list|)
operator|-
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|chunk
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
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
name|pos
operator|=
name|chunk
expr_stmt|;
name|b
operator|->
name|last
operator|=
name|ngx_sprintf
argument_list|(
name|chunk
argument_list|,
literal|"%xO"
name|CRLF
argument_list|,
name|size
argument_list|)
expr_stmt|;
name|out
operator|.
name|buf
operator|=
name|b
expr_stmt|;
block|}
if|if
condition|(
name|cl
operator|->
name|buf
operator|->
name|last_buf
condition|)
block|{
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
name|NGX_ERROR
return|;
block|}
name|b
operator|->
name|memory
operator|=
literal|1
expr_stmt|;
name|b
operator|->
name|last_buf
operator|=
literal|1
expr_stmt|;
name|b
operator|->
name|pos
operator|=
operator|(
name|u_char
operator|*
operator|)
name|CRLF
literal|"0"
name|CRLF
name|CRLF
expr_stmt|;
name|b
operator|->
name|last
operator|=
name|b
operator|->
name|pos
operator|+
literal|7
expr_stmt|;
name|cl
operator|->
name|buf
operator|->
name|last_buf
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|size
operator|==
literal|0
condition|)
block|{
name|b
operator|->
name|pos
operator|+=
literal|2
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
name|ngx_http_next_body_filter
argument_list|(
name|r
argument_list|,
operator|&
name|out
argument_list|)
return|;
block|}
block|}
else|else
block|{
if|if
condition|(
name|size
operator|==
literal|0
condition|)
block|{
operator|*
name|ll
operator|=
name|NULL
expr_stmt|;
return|return
name|ngx_http_next_body_filter
argument_list|(
name|r
argument_list|,
name|out
operator|.
name|next
argument_list|)
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
name|NGX_ERROR
return|;
block|}
name|b
operator|->
name|memory
operator|=
literal|1
expr_stmt|;
name|b
operator|->
name|pos
operator|=
operator|(
name|u_char
operator|*
operator|)
name|CRLF
expr_stmt|;
name|b
operator|->
name|last
operator|=
name|b
operator|->
name|pos
operator|+
literal|2
expr_stmt|;
block|}
name|tail
operator|.
name|buf
operator|=
name|b
expr_stmt|;
name|tail
operator|.
name|next
operator|=
name|NULL
expr_stmt|;
operator|*
name|ll
operator|=
operator|&
name|tail
expr_stmt|;
return|return
name|ngx_http_next_body_filter
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
DECL|function|ngx_http_chunked_filter_init (ngx_cycle_t * cycle)
specifier|static
name|ngx_int_t
name|ngx_http_chunked_filter_init
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
block|{
name|ngx_http_next_header_filter
operator|=
name|ngx_http_top_header_filter
expr_stmt|;
name|ngx_http_top_header_filter
operator|=
name|ngx_http_chunked_header_filter
expr_stmt|;
name|ngx_http_next_body_filter
operator|=
name|ngx_http_top_body_filter
expr_stmt|;
name|ngx_http_top_body_filter
operator|=
name|ngx_http_chunked_body_filter
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

end_unit

