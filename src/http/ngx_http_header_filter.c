begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|<nginx.h>
end_include

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
file|<ngx_string.h>
end_include

begin_include
include|#
directive|include
file|<ngx_table.h>
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

begin_if
if|#
directive|if
literal|0
end_if

begin_comment
unit|ngx_http_module_t  ngx_http_header_filter_module = {     NGX_HTTP_MODULE,      NULL,
comment|/* create server config */
end_comment

begin_comment
unit|NULL,
comment|/* create location config */
end_comment

begin_comment
unit|NULL,
comment|/* module directives */
end_comment

begin_comment
unit|NULL,
comment|/* init module */
end_comment

begin_comment
unit|NULL,
comment|/* translate handler */
end_comment

begin_comment
unit|ngx_http_header_filter_init
comment|/* init output header filter */
end_comment

begin_comment
unit|NULL
comment|/* init output body filter */
end_comment

begin_endif
unit|};
endif|#
directive|endif
end_endif

begin_decl_stmt
DECL|variable|server_string
specifier|static
name|char
name|server_string
index|[]
init|=
literal|"Server: "
name|NGINX_VER
name|CRLF
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|http_codes
specifier|static
name|ngx_str_t
name|http_codes
index|[]
init|=
block|{
block|{
literal|6
block|,
literal|"200 OK"
block|}
block|,
block|{
literal|21
block|,
literal|"301 Moved Permanently"
block|}
block|,
block|{
literal|15
block|,
literal|"400 Bad Request"
block|}
block|,
block|{
literal|0
block|,
name|NULL
block|}
block|,
block|{
literal|0
block|,
name|NULL
block|}
block|,
block|{
literal|13
block|,
literal|"403 Forbidden"
block|}
block|,
block|{
literal|13
block|,
literal|"404 Not Found"
block|}
block|}
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|ngx_http_header_filter (ngx_http_request_t * r)
name|int
name|ngx_http_header_filter
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
block|{
name|int
name|len
decl_stmt|,
name|status
decl_stmt|,
name|i
decl_stmt|;
name|ngx_hunk_t
modifier|*
name|h
decl_stmt|;
name|ngx_chain_t
modifier|*
name|ch
decl_stmt|;
name|ngx_table_elt_t
modifier|*
name|header
decl_stmt|;
if|if
condition|(
name|r
operator|->
name|http_version
operator|<
name|NGX_HTTP_VERSION_10
condition|)
return|return
name|NGX_OK
return|;
comment|/* 9 is for "HTTP/1.1 ", 2 is for trailing "\r\n"        and 2 is for end of header */
name|len
operator|=
literal|9
operator|+
literal|2
operator|+
literal|2
expr_stmt|;
comment|/* status line */
if|if
condition|(
name|r
operator|->
name|headers_out
operator|.
name|status_line
operator|.
name|len
condition|)
block|{
name|len
operator|+=
name|r
operator|->
name|headers_out
operator|.
name|status_line
operator|.
name|len
expr_stmt|;
block|}
else|else
block|{
if|if
condition|(
name|r
operator|->
name|headers_out
operator|.
name|status
operator|<
name|NGX_HTTP_MOVED_PERMANENTLY
condition|)
name|status
operator|=
name|r
operator|->
name|headers_out
operator|.
name|status
operator|-
name|NGX_HTTP_OK
expr_stmt|;
if|else if
condition|(
name|r
operator|->
name|headers_out
operator|.
name|status
operator|<
name|NGX_HTTP_BAD_REQUEST
condition|)
name|status
operator|=
name|r
operator|->
name|headers_out
operator|.
name|status
operator|-
name|NGX_HTTP_MOVED_PERMANENTLY
operator|+
literal|1
expr_stmt|;
else|else
name|status
operator|=
name|r
operator|->
name|headers_out
operator|.
name|status
operator|-
name|NGX_HTTP_BAD_REQUEST
operator|+
literal|1
operator|+
literal|1
expr_stmt|;
name|len
operator|+=
name|http_codes
index|[
name|status
index|]
operator|.
name|len
expr_stmt|;
block|}
if|if
condition|(
name|r
operator|->
name|headers_out
operator|.
name|server
operator|&&
name|r
operator|->
name|headers_out
operator|.
name|server
operator|->
name|key
operator|.
name|len
condition|)
block|{
name|len
operator|+=
name|r
operator|->
name|headers_out
operator|.
name|server
operator|->
name|key
operator|.
name|len
operator|+
name|r
operator|->
name|headers_out
operator|.
name|server
operator|->
name|value
operator|.
name|len
operator|+
literal|2
expr_stmt|;
block|}
else|else
block|{
name|len
operator|+=
sizeof|sizeof
argument_list|(
name|server_string
argument_list|)
operator|-
literal|1
expr_stmt|;
block|}
if|if
condition|(
name|r
operator|->
name|headers_out
operator|.
name|date
operator|&&
name|r
operator|->
name|headers_out
operator|.
name|date
operator|->
name|key
operator|.
name|len
condition|)
block|{
name|len
operator|+=
name|r
operator|->
name|headers_out
operator|.
name|date
operator|->
name|key
operator|.
name|len
operator|+
name|r
operator|->
name|headers_out
operator|.
name|date
operator|->
name|value
operator|.
name|len
operator|+
literal|2
expr_stmt|;
block|}
else|else
block|{
comment|/* "Date: ... \r\n"; */
name|len
operator|+=
literal|37
expr_stmt|;
block|}
comment|/* 2^64 is 20 characters */
if|if
condition|(
name|r
operator|->
name|headers_out
operator|.
name|content_length
operator|>=
literal|0
condition|)
name|len
operator|+=
literal|48
expr_stmt|;
if|#
directive|if
literal|0
block_content|if (r->headers_out.content_type.len)         len += r->headers_out.content_type.len + 16;
endif|#
directive|endif
if|if
condition|(
name|r
operator|->
name|keepalive
condition|)
name|len
operator|+=
literal|24
expr_stmt|;
else|else
name|len
operator|+=
literal|19
expr_stmt|;
name|header
operator|=
operator|(
name|ngx_table_elt_t
operator|*
operator|)
name|r
operator|->
name|headers_out
operator|.
name|headers
operator|->
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
name|r
operator|->
name|headers_out
operator|.
name|headers
operator|->
name|nelts
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|header
index|[
name|i
index|]
operator|.
name|key
operator|.
name|len
operator|==
literal|0
condition|)
continue|continue;
name|len
operator|+=
name|header
index|[
name|i
index|]
operator|.
name|key
operator|.
name|len
operator|+
literal|2
operator|+
name|header
index|[
name|i
index|]
operator|.
name|value
operator|.
name|len
operator|+
literal|2
expr_stmt|;
block|}
name|ngx_test_null
argument_list|(
name|h
argument_list|,
name|ngx_create_temp_hunk
argument_list|(
name|r
operator|->
name|pool
argument_list|,
name|len
argument_list|,
literal|0
argument_list|,
literal|64
argument_list|)
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
comment|/* "HTTP/1.1 " */
name|ngx_memcpy
argument_list|(
name|h
operator|->
name|last
operator|.
name|mem
argument_list|,
literal|"HTTP/1.1 "
argument_list|,
literal|9
argument_list|)
expr_stmt|;
name|h
operator|->
name|last
operator|.
name|mem
operator|+=
literal|9
expr_stmt|;
comment|/* status line */
if|if
condition|(
name|r
operator|->
name|headers_out
operator|.
name|status_line
operator|.
name|len
condition|)
block|{
name|ngx_memcpy
argument_list|(
name|h
operator|->
name|last
operator|.
name|mem
argument_list|,
name|r
operator|->
name|headers_out
operator|.
name|status_line
operator|.
name|data
argument_list|,
name|r
operator|->
name|headers_out
operator|.
name|status_line
operator|.
name|len
argument_list|)
expr_stmt|;
name|h
operator|->
name|last
operator|.
name|mem
operator|+=
name|r
operator|->
name|headers_out
operator|.
name|status_line
operator|.
name|len
expr_stmt|;
block|}
else|else
block|{
name|ngx_memcpy
argument_list|(
name|h
operator|->
name|last
operator|.
name|mem
argument_list|,
name|http_codes
index|[
name|status
index|]
operator|.
name|data
argument_list|,
name|http_codes
index|[
name|status
index|]
operator|.
name|len
argument_list|)
expr_stmt|;
name|h
operator|->
name|last
operator|.
name|mem
operator|+=
name|http_codes
index|[
name|status
index|]
operator|.
name|len
expr_stmt|;
block|}
operator|*
operator|(
name|h
operator|->
name|last
operator|.
name|mem
operator|++
operator|)
operator|=
name|CR
expr_stmt|;
operator|*
operator|(
name|h
operator|->
name|last
operator|.
name|mem
operator|++
operator|)
operator|=
name|LF
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|r
operator|->
name|headers_out
operator|.
name|server
operator|&&
name|r
operator|->
name|headers_out
operator|.
name|server
operator|->
name|key
operator|.
name|len
operator|)
condition|)
block|{
name|ngx_memcpy
argument_list|(
name|h
operator|->
name|last
operator|.
name|mem
argument_list|,
name|server_string
argument_list|,
sizeof|sizeof
argument_list|(
name|server_string
argument_list|)
operator|-
literal|1
argument_list|)
expr_stmt|;
name|h
operator|->
name|last
operator|.
name|mem
operator|+=
sizeof|sizeof
argument_list|(
name|server_string
argument_list|)
operator|-
literal|1
expr_stmt|;
block|}
if|if
condition|(
operator|!
operator|(
name|r
operator|->
name|headers_out
operator|.
name|date
operator|&&
name|r
operator|->
name|headers_out
operator|.
name|date
operator|->
name|key
operator|.
name|len
operator|)
condition|)
block|{
name|ngx_memcpy
argument_list|(
name|h
operator|->
name|last
operator|.
name|mem
argument_list|,
literal|"Date: "
argument_list|,
literal|6
argument_list|)
expr_stmt|;
name|h
operator|->
name|last
operator|.
name|mem
operator|+=
literal|6
expr_stmt|;
name|h
operator|->
name|last
operator|.
name|mem
operator|+=
name|ngx_http_get_time
argument_list|(
name|h
operator|->
name|last
operator|.
name|mem
argument_list|,
name|time
argument_list|(
name|NULL
argument_list|)
argument_list|)
expr_stmt|;
operator|*
operator|(
name|h
operator|->
name|last
operator|.
name|mem
operator|++
operator|)
operator|=
name|CR
expr_stmt|;
operator|*
operator|(
name|h
operator|->
name|last
operator|.
name|mem
operator|++
operator|)
operator|=
name|LF
expr_stmt|;
block|}
comment|/* 2^64 is 20 characters  */
if|if
condition|(
name|r
operator|->
name|headers_out
operator|.
name|content_length
operator|>=
literal|0
condition|)
name|h
operator|->
name|last
operator|.
name|mem
operator|+=
name|ngx_snprintf
argument_list|(
name|h
operator|->
name|last
operator|.
name|mem
argument_list|,
literal|49
argument_list|,
literal|"Content-Length: %u"
name|CRLF
argument_list|,
name|r
operator|->
name|headers_out
operator|.
name|content_length
argument_list|)
expr_stmt|;
if|#
directive|if
literal|0
block_content|if (r->headers_out.content_type.len) {         ngx_memcpy(h->last.mem, "Content-Type: ", 14);         h->last.mem += 14;         ngx_memcpy(h->last.mem, r->headers_out.content_type.data,                    r->headers_out.content_type.len);         h->last.mem += r->headers_out.content_type.len;         *(h->last.mem++) = CR; *(h->last.mem++) = LF;     }
endif|#
directive|endif
if|if
condition|(
name|r
operator|->
name|keepalive
condition|)
block|{
name|ngx_memcpy
argument_list|(
name|h
operator|->
name|last
operator|.
name|mem
argument_list|,
literal|"Connection: keep-alive"
name|CRLF
argument_list|,
literal|24
argument_list|)
expr_stmt|;
name|h
operator|->
name|last
operator|.
name|mem
operator|+=
literal|24
expr_stmt|;
block|}
else|else
block|{
name|ngx_memcpy
argument_list|(
name|h
operator|->
name|last
operator|.
name|mem
argument_list|,
literal|"Connection: close"
name|CRLF
argument_list|,
literal|19
argument_list|)
expr_stmt|;
name|h
operator|->
name|last
operator|.
name|mem
operator|+=
literal|19
expr_stmt|;
block|}
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|r
operator|->
name|headers_out
operator|.
name|headers
operator|->
name|nelts
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|header
index|[
name|i
index|]
operator|.
name|key
operator|.
name|len
operator|==
literal|0
condition|)
continue|continue;
name|ngx_memcpy
argument_list|(
name|h
operator|->
name|last
operator|.
name|mem
argument_list|,
name|header
index|[
name|i
index|]
operator|.
name|key
operator|.
name|data
argument_list|,
name|header
index|[
name|i
index|]
operator|.
name|key
operator|.
name|len
argument_list|)
expr_stmt|;
name|h
operator|->
name|last
operator|.
name|mem
operator|+=
name|header
index|[
name|i
index|]
operator|.
name|key
operator|.
name|len
expr_stmt|;
operator|*
operator|(
name|h
operator|->
name|last
operator|.
name|mem
operator|++
operator|)
operator|=
literal|':'
expr_stmt|;
operator|*
operator|(
name|h
operator|->
name|last
operator|.
name|mem
operator|++
operator|)
operator|=
literal|' '
expr_stmt|;
name|ngx_memcpy
argument_list|(
name|h
operator|->
name|last
operator|.
name|mem
argument_list|,
name|header
index|[
name|i
index|]
operator|.
name|value
operator|.
name|data
argument_list|,
name|header
index|[
name|i
index|]
operator|.
name|value
operator|.
name|len
argument_list|)
expr_stmt|;
name|h
operator|->
name|last
operator|.
name|mem
operator|+=
name|header
index|[
name|i
index|]
operator|.
name|value
operator|.
name|len
expr_stmt|;
operator|*
operator|(
name|h
operator|->
name|last
operator|.
name|mem
operator|++
operator|)
operator|=
name|CR
expr_stmt|;
operator|*
operator|(
name|h
operator|->
name|last
operator|.
name|mem
operator|++
operator|)
operator|=
name|LF
expr_stmt|;
block|}
comment|/* end of HTTP header */
operator|*
operator|(
name|h
operator|->
name|last
operator|.
name|mem
operator|++
operator|)
operator|=
name|CR
expr_stmt|;
operator|*
operator|(
name|h
operator|->
name|last
operator|.
name|mem
operator|++
operator|)
operator|=
name|LF
expr_stmt|;
name|ngx_test_null
argument_list|(
name|ch
argument_list|,
name|ngx_palloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_chain_t
argument_list|)
argument_list|)
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
name|ch
operator|->
name|hunk
operator|=
name|h
expr_stmt|;
name|ch
operator|->
name|next
operator|=
name|NULL
expr_stmt|;
return|return
name|ngx_http_write_filter
argument_list|(
name|r
argument_list|,
name|ch
argument_list|)
return|;
block|}
end_function

end_unit

