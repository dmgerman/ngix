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
file|<ngx_http.h>
end_include

begin_decl_stmt
DECL|variable|error_tail
specifier|static
name|char
name|error_tail
index|[]
init|=
literal|"<hr><center>"
name|NGINX_VER
literal|"</center>"
name|CRLF
literal|"</body>"
name|CRLF
literal|"</html>"
name|CRLF
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|error_400_page
specifier|static
name|char
name|error_400_page
index|[]
init|=
literal|"<html>"
name|CRLF
literal|"<head><title>400 Bad Request</title></head>"
name|CRLF
literal|"<body bgcolor=\"white\">"
name|CRLF
literal|"<center><h1>400 Bad Request</h1></center>"
name|CRLF
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|error_403_page
specifier|static
name|char
name|error_403_page
index|[]
init|=
literal|"<html>"
name|CRLF
literal|"<head><title>403 Forbidden</title></head>"
name|CRLF
literal|"<body bgcolor=\"white\">"
name|CRLF
literal|"<center><h1>403 Forbidden</h1></center>"
name|CRLF
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|error_404_page
specifier|static
name|char
name|error_404_page
index|[]
init|=
literal|"<html>"
name|CRLF
literal|"<head><title>404 Not Found</title></head>"
name|CRLF
literal|"<body bgcolor=\"white\">"
name|CRLF
literal|"<center><h1>404 Not Found</h1></center>"
name|CRLF
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|error_pages
specifier|static
name|ngx_str_t
name|error_pages
index|[]
init|=
block|{
block|{
literal|0
block|,
name|NULL
block|}
block|,
comment|/* 301 */
block|{
literal|0
block|,
name|NULL
block|}
block|,
comment|/* 302 */
block|{
literal|0
block|,
name|NULL
block|}
block|,
comment|/* 303 */
block|{
literal|0
block|,
name|NULL
block|}
block|,
comment|/* 304 */
block|{
sizeof|sizeof
argument_list|(
name|error_400_page
argument_list|)
operator|-
literal|1
block|,
name|error_400_page
block|}
block|,
block|{
literal|0
block|,
name|NULL
block|}
block|,
comment|/* 401 */
block|{
literal|0
block|,
name|NULL
block|}
block|,
comment|/* 402 */
block|{
sizeof|sizeof
argument_list|(
name|error_403_page
argument_list|)
operator|-
literal|1
block|,
name|error_403_page
block|}
block|,
block|{
sizeof|sizeof
argument_list|(
name|error_404_page
argument_list|)
operator|-
literal|1
block|,
name|error_404_page
block|}
block|,
block|{
literal|0
block|,
name|NULL
block|}
comment|/* 500 */
block|}
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|ngx_http_special_response (ngx_http_request_t * r,int error)
name|int
name|ngx_http_special_response
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|int
name|error
parameter_list|)
block|{
name|int
name|rc
decl_stmt|,
name|err
decl_stmt|,
name|len
decl_stmt|;
name|ngx_hunk_t
modifier|*
name|message
decl_stmt|,
modifier|*
name|tail
decl_stmt|;
name|len
operator|=
literal|0
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|status
operator|=
name|error
expr_stmt|;
if|if
condition|(
name|error
operator|<
name|NGX_HTTP_BAD_REQUEST
condition|)
name|err
operator|=
name|error
operator|-
name|NGX_HTTP_MOVED_PERMANENTLY
expr_stmt|;
if|else if
condition|(
name|error
operator|<
name|NGX_HTTP_INTERNAL_SERVER_ERROR
condition|)
name|err
operator|=
name|error
operator|-
name|NGX_HTTP_BAD_REQUEST
operator|+
literal|4
expr_stmt|;
else|else
name|err
operator|=
name|NGX_HTTP_INTERNAL_SERVER_ERROR
operator|+
literal|4
operator|+
literal|5
expr_stmt|;
if|if
condition|(
name|error_pages
index|[
name|err
index|]
operator|.
name|len
operator|==
literal|0
condition|)
name|r
operator|->
name|headers_out
operator|.
name|content_length
operator|=
operator|-
literal|1
expr_stmt|;
else|else
name|r
operator|->
name|headers_out
operator|.
name|content_length
operator|=
name|error_pages
index|[
name|err
index|]
operator|.
name|len
operator|+
name|len
operator|+
sizeof|sizeof
argument_list|(
name|error_tail
argument_list|)
expr_stmt|;
name|ngx_http_send_header
argument_list|(
name|r
argument_list|)
expr_stmt|;
if|if
condition|(
name|error_pages
index|[
name|err
index|]
operator|.
name|len
operator|==
literal|0
condition|)
return|return
name|NGX_OK
return|;
name|ngx_test_null
argument_list|(
name|message
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
name|message
operator|->
name|type
operator|=
name|NGX_HUNK_MEMORY
expr_stmt|;
name|message
operator|->
name|pos
operator|.
name|mem
operator|=
name|error_pages
index|[
name|err
index|]
operator|.
name|data
expr_stmt|;
name|message
operator|->
name|last
operator|.
name|mem
operator|=
name|error_pages
index|[
name|err
index|]
operator|.
name|data
operator|+
name|error_pages
index|[
name|err
index|]
operator|.
name|len
expr_stmt|;
name|rc
operator|=
name|ngx_http_output_filter
argument_list|(
name|r
argument_list|,
name|message
argument_list|)
expr_stmt|;
name|ngx_test_null
argument_list|(
name|tail
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
name|tail
operator|->
name|type
operator|=
name|NGX_HUNK_MEMORY
operator||
name|NGX_HUNK_LAST
expr_stmt|;
name|tail
operator|->
name|pos
operator|.
name|mem
operator|=
name|error_tail
expr_stmt|;
name|tail
operator|->
name|last
operator|.
name|mem
operator|=
name|error_tail
operator|+
sizeof|sizeof
argument_list|(
name|error_tail
argument_list|)
expr_stmt|;
name|rc
operator|=
name|ngx_http_output_filter
argument_list|(
name|r
argument_list|,
name|tail
argument_list|)
expr_stmt|;
block|}
end_function

end_unit

