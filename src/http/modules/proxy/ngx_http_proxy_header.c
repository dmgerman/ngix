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
file|<ngx_http_proxy_handler.h>
end_include

begin_function_decl
specifier|static
name|int
name|ngx_http_proxy_rewrite_location_header
parameter_list|(
name|ngx_http_proxy_ctx_t
modifier|*
name|p
parameter_list|,
name|ngx_table_elt_t
modifier|*
name|loc
parameter_list|)
function_decl|;
end_function_decl

begin_function
DECL|function|ngx_http_proxy_copy_header (ngx_http_proxy_ctx_t * p,ngx_http_proxy_headers_in_t * headers_in)
name|int
name|ngx_http_proxy_copy_header
parameter_list|(
name|ngx_http_proxy_ctx_t
modifier|*
name|p
parameter_list|,
name|ngx_http_proxy_headers_in_t
modifier|*
name|headers_in
parameter_list|)
block|{
name|ngx_uint_t
name|i
decl_stmt|;
name|ngx_table_elt_t
modifier|*
name|ho
decl_stmt|,
modifier|*
name|h
decl_stmt|;
name|ngx_http_request_t
modifier|*
name|r
decl_stmt|;
name|r
operator|=
name|p
operator|->
name|request
expr_stmt|;
name|h
operator|=
name|headers_in
operator|->
name|headers
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
name|headers_in
operator|->
name|headers
operator|.
name|nelts
condition|;
name|i
operator|++
control|)
block|{
comment|/* ignore some headers */
if|if
condition|(
operator|&
name|h
index|[
name|i
index|]
operator|==
name|headers_in
operator|->
name|connection
condition|)
block|{
continue|continue;
block|}
if|if
condition|(
operator|&
name|h
index|[
name|i
index|]
operator|==
name|headers_in
operator|->
name|x_pad
condition|)
block|{
continue|continue;
block|}
if|if
condition|(
name|p
operator|->
name|accel
condition|)
block|{
if|if
condition|(
operator|&
name|h
index|[
name|i
index|]
operator|==
name|headers_in
operator|->
name|date
operator|||
operator|&
name|h
index|[
name|i
index|]
operator|==
name|headers_in
operator|->
name|accept_ranges
condition|)
block|{
continue|continue;
block|}
if|if
condition|(
operator|&
name|h
index|[
name|i
index|]
operator|==
name|headers_in
operator|->
name|x_accel_expires
operator|&&
operator|!
name|p
operator|->
name|lcf
operator|->
name|pass_x_accel_expires
condition|)
block|{
continue|continue;
block|}
if|if
condition|(
operator|&
name|h
index|[
name|i
index|]
operator|==
name|headers_in
operator|->
name|server
operator|&&
operator|!
name|p
operator|->
name|lcf
operator|->
name|pass_server
condition|)
block|{
continue|continue;
block|}
if|if
condition|(
operator|&
name|h
index|[
name|i
index|]
operator|==
name|headers_in
operator|->
name|location
condition|)
block|{
if|if
condition|(
name|ngx_http_proxy_rewrite_location_header
argument_list|(
name|p
argument_list|,
operator|&
name|h
index|[
name|i
index|]
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
continue|continue;
block|}
block|}
comment|/* "Content-Type" is handled specially */
if|if
condition|(
operator|&
name|h
index|[
name|i
index|]
operator|==
name|headers_in
operator|->
name|content_type
condition|)
block|{
name|r
operator|->
name|headers_out
operator|.
name|content_type
operator|=
operator|&
name|h
index|[
name|i
index|]
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
continue|continue;
block|}
comment|/* copy some header pointers and set up r->headers_out */
if|if
condition|(
operator|!
operator|(
name|ho
operator|=
name|ngx_http_add_header
argument_list|(
operator|&
name|r
operator|->
name|headers_out
argument_list|,
name|ngx_http_headers_out
argument_list|)
operator|)
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
operator|*
name|ho
operator|=
name|h
index|[
name|i
index|]
expr_stmt|;
if|if
condition|(
operator|&
name|h
index|[
name|i
index|]
operator|==
name|headers_in
operator|->
name|expires
condition|)
block|{
name|r
operator|->
name|headers_out
operator|.
name|expires
operator|=
name|ho
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|&
name|h
index|[
name|i
index|]
operator|==
name|headers_in
operator|->
name|cache_control
condition|)
block|{
name|r
operator|->
name|headers_out
operator|.
name|cache_control
operator|=
name|ho
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|&
name|h
index|[
name|i
index|]
operator|==
name|headers_in
operator|->
name|etag
condition|)
block|{
name|r
operator|->
name|headers_out
operator|.
name|etag
operator|=
name|ho
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|&
name|h
index|[
name|i
index|]
operator|==
name|headers_in
operator|->
name|last_modified
condition|)
block|{
name|r
operator|->
name|headers_out
operator|.
name|last_modified
operator|=
name|ho
expr_stmt|;
comment|/* TODO: update r->headers_out.last_modified_time */
continue|continue;
block|}
comment|/*          * ngx_http_header_filter() passes the following headers as is          * and does not handle them specially if they are set:          *     r->headers_out.server,          *     r->headers_out.date,          *     r->headers_out.content_length          */
if|if
condition|(
operator|&
name|h
index|[
name|i
index|]
operator|==
name|headers_in
operator|->
name|server
condition|)
block|{
name|r
operator|->
name|headers_out
operator|.
name|server
operator|=
name|ho
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|&
name|h
index|[
name|i
index|]
operator|==
name|headers_in
operator|->
name|date
condition|)
block|{
name|r
operator|->
name|headers_out
operator|.
name|date
operator|=
name|ho
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|&
name|h
index|[
name|i
index|]
operator|==
name|headers_in
operator|->
name|content_length
condition|)
block|{
name|r
operator|->
name|headers_out
operator|.
name|content_length
operator|=
name|ho
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|content_length_n
operator|=
name|ngx_atoi
argument_list|(
name|ho
operator|->
name|value
operator|.
name|data
argument_list|,
name|ho
operator|->
name|value
operator|.
name|len
argument_list|)
expr_stmt|;
continue|continue;
block|}
block|}
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_proxy_rewrite_location_header (ngx_http_proxy_ctx_t * p,ngx_table_elt_t * loc)
specifier|static
name|int
name|ngx_http_proxy_rewrite_location_header
parameter_list|(
name|ngx_http_proxy_ctx_t
modifier|*
name|p
parameter_list|,
name|ngx_table_elt_t
modifier|*
name|loc
parameter_list|)
block|{
name|u_char
modifier|*
name|last
decl_stmt|;
name|ngx_table_elt_t
modifier|*
name|location
decl_stmt|;
name|ngx_http_request_t
modifier|*
name|r
decl_stmt|;
name|ngx_http_proxy_upstream_conf_t
modifier|*
name|uc
decl_stmt|;
name|r
operator|=
name|p
operator|->
name|request
expr_stmt|;
name|uc
operator|=
name|p
operator|->
name|lcf
operator|->
name|upstream
expr_stmt|;
name|location
operator|=
name|ngx_http_add_header
argument_list|(
operator|&
name|r
operator|->
name|headers_out
argument_list|,
name|ngx_http_headers_out
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
name|NGX_ERROR
return|;
block|}
comment|/*      * we do not set r->headers_out.location to avoid the handling      * the local redirects without a host name by ngx_http_header_filter()      */
if|#
directive|if
literal|0
block_content|r->headers_out.location = location;
endif|#
directive|endif
if|if
condition|(
name|uc
operator|->
name|url
operator|.
name|len
operator|>
name|loc
operator|->
name|value
operator|.
name|len
operator|||
name|ngx_rstrncmp
argument_list|(
name|loc
operator|->
name|value
operator|.
name|data
argument_list|,
name|uc
operator|->
name|url
operator|.
name|data
argument_list|,
name|uc
operator|->
name|url
operator|.
name|len
argument_list|)
operator|!=
literal|0
condition|)
block|{
operator|*
name|location
operator|=
operator|*
name|loc
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
comment|/* TODO: proxy_reverse */
name|location
operator|->
name|value
operator|.
name|len
operator|=
name|uc
operator|->
name|location
operator|->
name|len
operator|+
operator|(
name|loc
operator|->
name|value
operator|.
name|len
operator|-
name|uc
operator|->
name|url
operator|.
name|len
operator|)
operator|+
literal|1
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|location
operator|->
name|value
operator|.
name|data
operator|=
name|ngx_palloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
name|location
operator|->
name|value
operator|.
name|len
argument_list|)
operator|)
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|last
operator|=
name|ngx_cpymem
argument_list|(
name|location
operator|->
name|value
operator|.
name|data
argument_list|,
name|uc
operator|->
name|location
operator|->
name|data
argument_list|,
name|uc
operator|->
name|location
operator|->
name|len
argument_list|)
expr_stmt|;
name|ngx_cpystrn
argument_list|(
name|last
argument_list|,
name|loc
operator|->
name|value
operator|.
name|data
operator|+
name|uc
operator|->
name|url
operator|.
name|len
argument_list|,
name|loc
operator|->
name|value
operator|.
name|len
operator|-
name|uc
operator|->
name|url
operator|.
name|len
operator|+
literal|1
argument_list|)
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

end_unit

