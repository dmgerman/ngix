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

begin_function_decl
specifier|static
name|int
name|ngx_http_chunked_filter_init
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|,
name|ngx_log_t
modifier|*
name|log
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
block|,
comment|/* commit module */
name|NULL
comment|/* rollback module */
block|}
decl_stmt|;
end_decl_stmt

begin_function_decl
DECL|variable|next_header_filter
specifier|static
name|int
function_decl|(
modifier|*
name|next_header_filter
function_decl|)
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
DECL|variable|next_body_filter
specifier|static
name|int
function_decl|(
modifier|*
name|next_body_filter
function_decl|)
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_chain_t
modifier|*
name|ch
parameter_list|)
function_decl|;
end_function_decl

begin_function
DECL|function|ngx_http_chunked_header_filter (ngx_http_request_t * r)
specifier|static
name|int
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
name|content_length
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
name|next_header_filter
argument_list|(
name|r
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_chunked_body_filter (ngx_http_request_t * r,ngx_chain_t * in)
specifier|static
name|int
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
name|char
modifier|*
name|chunk
decl_stmt|;
name|size_t
name|size
decl_stmt|,
name|len
decl_stmt|;
name|ngx_hunk_t
modifier|*
name|h
decl_stmt|;
name|ngx_chain_t
modifier|*
name|out
decl_stmt|,
modifier|*
name|ce
decl_stmt|,
modifier|*
name|te
decl_stmt|,
modifier|*
modifier|*
name|le
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
name|next_body_filter
argument_list|(
name|r
argument_list|,
name|in
argument_list|)
return|;
block|}
name|ngx_test_null
argument_list|(
name|out
argument_list|,
name|ngx_alloc_chain_entry
argument_list|(
name|r
operator|->
name|pool
argument_list|)
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
name|le
operator|=
operator|&
name|out
operator|->
name|next
expr_stmt|;
name|size
operator|=
literal|0
expr_stmt|;
name|ce
operator|=
name|in
expr_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
if|if
condition|(
name|ce
operator|->
name|hunk
operator|->
name|type
operator|&
name|NGX_HUNK_IN_MEMORY
condition|)
block|{
name|size
operator|+=
name|ce
operator|->
name|hunk
operator|->
name|last
operator|-
name|ce
operator|->
name|hunk
operator|->
name|pos
expr_stmt|;
block|}
else|else
block|{
name|size
operator|+=
operator|(
name|size_t
operator|)
operator|(
name|ce
operator|->
name|hunk
operator|->
name|file_last
operator|-
name|ce
operator|->
name|hunk
operator|->
name|file_pos
operator|)
expr_stmt|;
block|}
name|ngx_test_null
argument_list|(
name|te
argument_list|,
name|ngx_alloc_chain_entry
argument_list|(
name|r
operator|->
name|pool
argument_list|)
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
name|te
operator|->
name|hunk
operator|=
name|ce
operator|->
name|hunk
expr_stmt|;
operator|*
name|le
operator|=
name|te
expr_stmt|;
name|le
operator|=
operator|&
name|te
operator|->
name|next
expr_stmt|;
if|if
condition|(
name|ce
operator|->
name|next
operator|==
name|NULL
condition|)
block|{
break|break;
block|}
name|ce
operator|=
name|ce
operator|->
name|next
expr_stmt|;
block|}
name|ngx_test_null
argument_list|(
name|chunk
argument_list|,
name|ngx_palloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
literal|11
argument_list|)
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
name|len
operator|=
name|ngx_snprintf
argument_list|(
argument|chunk
argument_list|,
literal|11
argument_list|,
argument|SIZEX_FMT CRLF
argument_list|,
argument|size
argument_list|)
expr_stmt|;
name|ngx_test_null
argument_list|(
name|h
argument_list|,
name|ngx_calloc_hunk
argument_list|(
name|r
operator|->
name|pool
argument_list|)
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
name|h
operator|->
name|type
operator|=
name|NGX_HUNK_IN_MEMORY
operator||
name|NGX_HUNK_TEMP
expr_stmt|;
name|h
operator|->
name|pos
operator|=
name|chunk
expr_stmt|;
name|h
operator|->
name|last
operator|=
name|chunk
operator|+
name|len
expr_stmt|;
name|out
operator|->
name|hunk
operator|=
name|h
expr_stmt|;
name|ngx_test_null
argument_list|(
name|h
argument_list|,
name|ngx_calloc_hunk
argument_list|(
name|r
operator|->
name|pool
argument_list|)
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
if|if
condition|(
name|ce
operator|->
name|hunk
operator|->
name|type
operator|&
name|NGX_HUNK_LAST
condition|)
block|{
name|ce
operator|->
name|hunk
operator|->
name|type
operator|&=
operator|~
name|NGX_HUNK_LAST
expr_stmt|;
name|h
operator|->
name|type
operator|=
name|NGX_HUNK_IN_MEMORY
operator||
name|NGX_HUNK_MEMORY
operator||
name|NGX_HUNK_LAST
expr_stmt|;
name|h
operator|->
name|pos
operator|=
name|CRLF
literal|"0"
name|CRLF
name|CRLF
expr_stmt|;
name|h
operator|->
name|last
operator|=
name|h
operator|->
name|pos
operator|+
literal|7
expr_stmt|;
block|}
else|else
block|{
name|h
operator|->
name|type
operator|=
name|NGX_HUNK_IN_MEMORY
operator||
name|NGX_HUNK_MEMORY
expr_stmt|;
name|h
operator|->
name|pos
operator|=
name|CRLF
expr_stmt|;
name|h
operator|->
name|last
operator|=
name|h
operator|->
name|pos
operator|+
literal|2
expr_stmt|;
block|}
name|ngx_test_null
argument_list|(
name|te
argument_list|,
name|ngx_alloc_chain_entry
argument_list|(
name|r
operator|->
name|pool
argument_list|)
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
name|te
operator|->
name|hunk
operator|=
name|h
expr_stmt|;
name|te
operator|->
name|next
operator|=
name|NULL
expr_stmt|;
operator|*
name|le
operator|=
name|te
expr_stmt|;
return|return
name|next_body_filter
argument_list|(
name|r
argument_list|,
name|out
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_chunked_filter_init (ngx_cycle_t * cycle,ngx_log_t * log)
specifier|static
name|int
name|ngx_http_chunked_filter_init
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|,
name|ngx_log_t
modifier|*
name|log
parameter_list|)
block|{
name|next_header_filter
operator|=
name|ngx_http_top_header_filter
expr_stmt|;
name|ngx_http_top_header_filter
operator|=
name|ngx_http_chunked_header_filter
expr_stmt|;
name|next_body_filter
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

