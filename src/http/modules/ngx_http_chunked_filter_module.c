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
DECL|struct|__anon291908770108
typedef|typedef
struct|struct
block|{
DECL|member|free
name|ngx_chain_t
modifier|*
name|free
decl_stmt|;
DECL|member|busy
name|ngx_chain_t
modifier|*
name|busy
decl_stmt|;
DECL|typedef|ngx_http_chunked_filter_ctx_t
block|}
name|ngx_http_chunked_filter_ctx_t
typedef|;
end_typedef

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_chunked_filter_init
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
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
comment|/* preconfiguration */
name|ngx_http_chunked_filter_init
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
DECL|variable|ngx_http_chunked_filter_module
name|ngx_module_t
name|ngx_http_chunked_filter_module
init|=
block|{
name|NGX_MODULE_V1
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

begin_decl_stmt
DECL|variable|ngx_http_next_body_filter
specifier|static
name|ngx_http_output_body_filter_pt
name|ngx_http_next_body_filter
decl_stmt|;
end_decl_stmt

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_chunked_header_filter (ngx_http_request_t * r)
name|ngx_http_chunked_header_filter
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
block|{
name|ngx_http_core_loc_conf_t
modifier|*
name|clcf
decl_stmt|;
name|ngx_http_chunked_filter_ctx_t
modifier|*
name|ctx
decl_stmt|;
if|if
condition|(
name|r
operator|->
name|headers_out
operator|.
name|status
operator|==
name|NGX_HTTP_NOT_MODIFIED
operator|||
name|r
operator|->
name|headers_out
operator|.
name|status
operator|==
name|NGX_HTTP_NO_CONTENT
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
name|method
operator|&
name|NGX_HTTP_HEAD
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
name|clcf
operator|->
name|chunked_transfer_encoding
condition|)
block|{
name|r
operator|->
name|chunked
operator|=
literal|1
expr_stmt|;
name|ctx
operator|=
name|ngx_pcalloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_chunked_filter_ctx_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|ctx
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|ngx_http_set_ctx
argument_list|(
name|r
argument_list|,
name|ctx
argument_list|,
name|ngx_http_chunked_filter_module
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|r
operator|->
name|keepalive
operator|=
literal|0
expr_stmt|;
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
DECL|function|ngx_http_chunked_body_filter (ngx_http_request_t * r,ngx_chain_t * in)
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
name|ngx_int_t
name|rc
decl_stmt|;
name|ngx_buf_t
modifier|*
name|b
decl_stmt|;
name|ngx_chain_t
modifier|*
name|out
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
name|ngx_http_chunked_filter_ctx_t
modifier|*
name|ctx
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
operator|||
name|r
operator|->
name|header_only
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
name|ctx
operator|=
name|ngx_http_get_module_ctx
argument_list|(
name|r
argument_list|,
name|ngx_http_chunked_filter_module
argument_list|)
expr_stmt|;
name|out
operator|=
name|NULL
expr_stmt|;
name|ll
operator|=
operator|&
name|out
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
name|cl
operator|->
name|buf
operator|->
name|sync
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
name|tl
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
name|tl
operator|==
name|NULL
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
name|tl
operator|=
name|ngx_chain_get_free_buf
argument_list|(
name|r
operator|->
name|pool
argument_list|,
operator|&
name|ctx
operator|->
name|free
argument_list|)
expr_stmt|;
if|if
condition|(
name|tl
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|b
operator|=
name|tl
operator|->
name|buf
expr_stmt|;
name|chunk
operator|=
name|b
operator|->
name|start
expr_stmt|;
if|if
condition|(
name|chunk
operator|==
name|NULL
condition|)
block|{
comment|/* the "0000000000000000" is 64-bit hexadecimal string */
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
name|start
operator|=
name|chunk
expr_stmt|;
name|b
operator|->
name|end
operator|=
name|chunk
operator|+
sizeof|sizeof
argument_list|(
literal|"0000000000000000"
name|CRLF
argument_list|)
operator|-
literal|1
expr_stmt|;
block|}
name|b
operator|->
name|tag
operator|=
operator|(
name|ngx_buf_tag_t
operator|)
operator|&
name|ngx_http_chunked_filter_module
expr_stmt|;
name|b
operator|->
name|memory
operator|=
literal|0
expr_stmt|;
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
name|tl
operator|->
name|next
operator|=
name|out
expr_stmt|;
name|out
operator|=
name|tl
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
name|tl
operator|=
name|ngx_chain_get_free_buf
argument_list|(
name|r
operator|->
name|pool
argument_list|,
operator|&
name|ctx
operator|->
name|free
argument_list|)
expr_stmt|;
if|if
condition|(
name|tl
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|b
operator|=
name|tl
operator|->
name|buf
expr_stmt|;
name|b
operator|->
name|tag
operator|=
operator|(
name|ngx_buf_tag_t
operator|)
operator|&
name|ngx_http_chunked_filter_module
expr_stmt|;
name|b
operator|->
name|temporary
operator|=
literal|0
expr_stmt|;
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
operator|*
name|ll
operator|=
name|tl
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
block|}
block|}
if|else if
condition|(
name|size
operator|>
literal|0
condition|)
block|{
name|tl
operator|=
name|ngx_chain_get_free_buf
argument_list|(
name|r
operator|->
name|pool
argument_list|,
operator|&
name|ctx
operator|->
name|free
argument_list|)
expr_stmt|;
if|if
condition|(
name|tl
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|b
operator|=
name|tl
operator|->
name|buf
expr_stmt|;
name|b
operator|->
name|tag
operator|=
operator|(
name|ngx_buf_tag_t
operator|)
operator|&
name|ngx_http_chunked_filter_module
expr_stmt|;
name|b
operator|->
name|temporary
operator|=
literal|0
expr_stmt|;
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
operator|*
name|ll
operator|=
name|tl
expr_stmt|;
block|}
else|else
block|{
operator|*
name|ll
operator|=
name|NULL
expr_stmt|;
block|}
name|rc
operator|=
name|ngx_http_next_body_filter
argument_list|(
name|r
argument_list|,
name|out
argument_list|)
expr_stmt|;
name|ngx_chain_update_chains
argument_list|(
name|r
operator|->
name|pool
argument_list|,
operator|&
name|ctx
operator|->
name|free
argument_list|,
operator|&
name|ctx
operator|->
name|busy
argument_list|,
operator|&
name|out
argument_list|,
operator|(
name|ngx_buf_tag_t
operator|)
operator|&
name|ngx_http_chunked_filter_module
argument_list|)
expr_stmt|;
return|return
name|rc
return|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_chunked_filter_init (ngx_conf_t * cf)
name|ngx_http_chunked_filter_init
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

