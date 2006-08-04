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

begin_include
include|#
directive|include
file|<nginx.h>
end_include

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_header_filter_init
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_header_filter
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
function_decl|;
end_function_decl

begin_decl_stmt
DECL|variable|ngx_http_header_filter_module_ctx
specifier|static
name|ngx_http_module_t
name|ngx_http_header_filter_module_ctx
init|=
block|{
name|NULL
block|,
comment|/* preconfiguration */
name|NULL
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
block|,
comment|/* merge location configuration */
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_header_filter_module
name|ngx_module_t
name|ngx_http_header_filter_module
init|=
block|{
name|NGX_MODULE_V1
block|,
operator|&
name|ngx_http_header_filter_module_ctx
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
name|ngx_http_header_filter_init
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
DECL|variable|ngx_http_server_string
specifier|static
name|char
name|ngx_http_server_string
index|[]
init|=
literal|"Server: "
name|NGINX_VER
name|CRLF
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_status_lines
specifier|static
name|ngx_str_t
name|ngx_http_status_lines
index|[]
init|=
block|{
name|ngx_string
argument_list|(
literal|"200 OK"
argument_list|)
block|,
name|ngx_string
argument_list|(
literal|"201 Created"
argument_list|)
block|,
name|ngx_null_string
block|,
comment|/* "202 Accepted" */
name|ngx_null_string
block|,
comment|/* "203 Non-Authoritative Information" */
name|ngx_string
argument_list|(
literal|"204 No Content"
argument_list|)
block|,
name|ngx_null_string
block|,
comment|/* "205 Reset Content" */
name|ngx_string
argument_list|(
literal|"206 Partial Content"
argument_list|)
block|,
comment|/* ngx_null_string, */
comment|/* "207 Multi-Status" */
DECL|macro|NGX_HTTP_LEVEL_200
define|#
directive|define
name|NGX_HTTP_LEVEL_200
value|7
comment|/* ngx_null_string, */
comment|/* "300 Multiple Choices" */
name|ngx_string
argument_list|(
literal|"301 Moved Permanently"
argument_list|)
block|,
name|ngx_string
argument_list|(
literal|"302 Moved Temporarily"
argument_list|)
block|,
name|ngx_null_string
block|,
comment|/* "303 See Other" */
name|ngx_string
argument_list|(
literal|"304 Not Modified"
argument_list|)
block|,
comment|/* ngx_null_string, */
comment|/* "305 Use Proxy" */
comment|/* ngx_null_string, */
comment|/* "306 unused" */
comment|/* ngx_null_string, */
comment|/* "307 Temporary Redirect" */
DECL|macro|NGX_HTTP_LEVEL_300
define|#
directive|define
name|NGX_HTTP_LEVEL_300
value|4
name|ngx_string
argument_list|(
literal|"400 Bad Request"
argument_list|)
block|,
name|ngx_string
argument_list|(
literal|"401 Unauthorized"
argument_list|)
block|,
name|ngx_string
argument_list|(
literal|"402 Payment Required"
argument_list|)
block|,
name|ngx_string
argument_list|(
literal|"403 Forbidden"
argument_list|)
block|,
name|ngx_string
argument_list|(
literal|"404 Not Found"
argument_list|)
block|,
name|ngx_string
argument_list|(
literal|"405 Not Allowed"
argument_list|)
block|,
name|ngx_string
argument_list|(
literal|"406 Not Acceptable"
argument_list|)
block|,
name|ngx_null_string
block|,
comment|/* "407 Proxy Authentication Required" */
name|ngx_string
argument_list|(
literal|"408 Request Time-out"
argument_list|)
block|,
name|ngx_string
argument_list|(
literal|"409 Conflict"
argument_list|)
block|,
name|ngx_string
argument_list|(
literal|"410 Gone"
argument_list|)
block|,
name|ngx_string
argument_list|(
literal|"411 Length Required"
argument_list|)
block|,
name|ngx_null_string
block|,
comment|/* "412 Precondition Failed" */
name|ngx_string
argument_list|(
literal|"413 Request Entity Too Large"
argument_list|)
block|,
name|ngx_null_string
block|,
comment|/* "414 Request-URI Too Large", but we never send it                        * because we treat such requests as the HTTP/0.9                        * requests and send only a body without a header                        */
name|ngx_string
argument_list|(
literal|"415 Unsupported Media Type"
argument_list|)
block|,
name|ngx_string
argument_list|(
literal|"416 Requested Range Not Satisfiable"
argument_list|)
block|,
comment|/* ngx_null_string, */
comment|/* "417 Expectation Failed" */
comment|/* ngx_null_string, */
comment|/* "418 unused" */
comment|/* ngx_null_string, */
comment|/* "419 unused" */
comment|/* ngx_null_string, */
comment|/* "420 unused" */
comment|/* ngx_null_string, */
comment|/* "421 unused" */
comment|/* ngx_null_string, */
comment|/* "422 Unprocessable Entity" */
comment|/* ngx_null_string, */
comment|/* "423 Locked" */
comment|/* ngx_null_string, */
comment|/* "424 Failed Dependency" */
DECL|macro|NGX_HTTP_LEVEL_400
define|#
directive|define
name|NGX_HTTP_LEVEL_400
value|17
name|ngx_string
argument_list|(
literal|"500 Internal Server Error"
argument_list|)
block|,
name|ngx_string
argument_list|(
literal|"501 Method Not Implemented"
argument_list|)
block|,
name|ngx_string
argument_list|(
literal|"502 Bad Gateway"
argument_list|)
block|,
name|ngx_string
argument_list|(
literal|"503 Service Temporarily Unavailable"
argument_list|)
block|,
name|ngx_string
argument_list|(
literal|"504 Gateway Time-out"
argument_list|)
block|,
name|ngx_null_string
block|,
comment|/* "505 HTTP Version Not Supported" */
name|ngx_null_string
block|,
comment|/* "506 Variant Also Negotiates" */
name|ngx_string
argument_list|(
literal|"507 Insufficient Storage"
argument_list|)
block|,
comment|/* ngx_null_string, */
comment|/* "508 unused" */
comment|/* ngx_null_string, */
comment|/* "509 unused" */
comment|/* ngx_null_string, */
comment|/* "510 Not Extended" */
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_headers_out
name|ngx_http_header_out_t
name|ngx_http_headers_out
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"Server"
argument_list|)
block|,
name|offsetof
argument_list|(
argument|ngx_http_headers_out_t
argument_list|,
argument|server
argument_list|)
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"Date"
argument_list|)
block|,
name|offsetof
argument_list|(
argument|ngx_http_headers_out_t
argument_list|,
argument|date
argument_list|)
block|}
block|,
if|#
directive|if
literal|0
block|{ ngx_string("Content-Type"),                  offsetof(ngx_http_headers_out_t, content_type) },
endif|#
directive|endif
block|{
name|ngx_string
argument_list|(
literal|"Content-Length"
argument_list|)
block|,
name|offsetof
argument_list|(
argument|ngx_http_headers_out_t
argument_list|,
argument|content_length
argument_list|)
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"Content-Encoding"
argument_list|)
block|,
name|offsetof
argument_list|(
argument|ngx_http_headers_out_t
argument_list|,
argument|content_encoding
argument_list|)
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"Location"
argument_list|)
block|,
name|offsetof
argument_list|(
argument|ngx_http_headers_out_t
argument_list|,
argument|location
argument_list|)
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"Last-Modified"
argument_list|)
block|,
name|offsetof
argument_list|(
argument|ngx_http_headers_out_t
argument_list|,
argument|last_modified
argument_list|)
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"Accept-Ranges"
argument_list|)
block|,
name|offsetof
argument_list|(
argument|ngx_http_headers_out_t
argument_list|,
argument|accept_ranges
argument_list|)
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"Expires"
argument_list|)
block|,
name|offsetof
argument_list|(
argument|ngx_http_headers_out_t
argument_list|,
argument|expires
argument_list|)
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"Cache-Control"
argument_list|)
block|,
name|offsetof
argument_list|(
argument|ngx_http_headers_out_t
argument_list|,
argument|cache_control
argument_list|)
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"ETag"
argument_list|)
block|,
name|offsetof
argument_list|(
argument|ngx_http_headers_out_t
argument_list|,
argument|etag
argument_list|)
block|}
block|,
block|{
name|ngx_null_string
block|,
literal|0
block|}
block|}
decl_stmt|;
end_decl_stmt

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_header_filter (ngx_http_request_t * r)
name|ngx_http_header_filter
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
block|{
name|u_char
modifier|*
name|p
decl_stmt|;
name|size_t
name|len
decl_stmt|;
name|ngx_uint_t
name|status
decl_stmt|,
name|i
decl_stmt|;
name|ngx_buf_t
modifier|*
name|b
decl_stmt|;
name|ngx_chain_t
name|out
decl_stmt|;
name|ngx_list_part_t
modifier|*
name|part
decl_stmt|;
name|ngx_table_elt_t
modifier|*
name|header
decl_stmt|;
name|ngx_http_core_loc_conf_t
modifier|*
name|clcf
decl_stmt|;
name|r
operator|->
name|header_sent
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|r
operator|!=
name|r
operator|->
expr|main
condition|)
block|{
return|return
name|NGX_OK
return|;
block|}
if|if
condition|(
name|r
operator|->
name|http_version
operator|<
name|NGX_HTTP_VERSION_10
condition|)
block|{
return|return
name|NGX_OK
return|;
block|}
if|if
condition|(
name|r
operator|->
name|method
operator|==
name|NGX_HTTP_HEAD
condition|)
block|{
name|r
operator|->
name|header_only
operator|=
literal|1
expr_stmt|;
block|}
if|if
condition|(
name|r
operator|->
name|headers_out
operator|.
name|last_modified_time
operator|!=
operator|-
literal|1
condition|)
block|{
if|if
condition|(
name|r
operator|->
name|headers_out
operator|.
name|status
operator|!=
name|NGX_HTTP_OK
operator|&&
name|r
operator|->
name|headers_out
operator|.
name|status
operator|!=
name|NGX_HTTP_PARTIAL_CONTENT
operator|&&
name|r
operator|->
name|headers_out
operator|.
name|status
operator|!=
name|NGX_HTTP_NOT_MODIFIED
condition|)
block|{
name|r
operator|->
name|headers_out
operator|.
name|last_modified_time
operator|=
operator|-
literal|1
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|last_modified
operator|=
name|NULL
expr_stmt|;
block|}
block|}
name|len
operator|=
sizeof|sizeof
argument_list|(
literal|"HTTP/1.x "
argument_list|)
operator|-
literal|1
operator|+
sizeof|sizeof
argument_list|(
name|CRLF
argument_list|)
operator|-
literal|1
comment|/* the end of the header */
operator|+
sizeof|sizeof
argument_list|(
name|CRLF
argument_list|)
operator|-
literal|1
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
if|#
directive|if
operator|(
name|NGX_SUPPRESS_WARN
operator|)
name|status
operator|=
name|NGX_INVALID_ARRAY_INDEX
expr_stmt|;
endif|#
directive|endif
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
block|{
comment|/* 2XX */
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
if|if
condition|(
name|r
operator|->
name|headers_out
operator|.
name|status
operator|==
name|NGX_HTTP_NO_CONTENT
condition|)
block|{
name|r
operator|->
name|header_only
operator|=
literal|1
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|content_type
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
operator|.
name|data
operator|=
name|NULL
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|last_modified_time
operator|=
operator|-
literal|1
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|last_modified
operator|=
name|NULL
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|content_length
operator|=
name|NULL
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|content_length_n
operator|=
operator|-
literal|1
expr_stmt|;
block|}
block|}
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
block|{
comment|/* 3XX */
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
name|NGX_HTTP_LEVEL_200
expr_stmt|;
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
name|r
operator|->
name|header_only
operator|=
literal|1
expr_stmt|;
block|}
block|}
if|else if
condition|(
name|r
operator|->
name|headers_out
operator|.
name|status
operator|<
name|NGX_HTTP_INTERNAL_SERVER_ERROR
condition|)
block|{
comment|/* 4XX */
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
name|NGX_HTTP_LEVEL_200
operator|+
name|NGX_HTTP_LEVEL_300
expr_stmt|;
block|}
else|else
block|{
comment|/* 5XX */
name|status
operator|=
name|r
operator|->
name|headers_out
operator|.
name|status
operator|-
name|NGX_HTTP_INTERNAL_SERVER_ERROR
operator|+
name|NGX_HTTP_LEVEL_200
operator|+
name|NGX_HTTP_LEVEL_300
operator|+
name|NGX_HTTP_LEVEL_400
expr_stmt|;
block|}
name|len
operator|+=
name|ngx_http_status_lines
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
operator|==
name|NULL
condition|)
block|{
name|len
operator|+=
sizeof|sizeof
argument_list|(
name|ngx_http_server_string
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
operator|==
name|NULL
condition|)
block|{
name|len
operator|+=
sizeof|sizeof
argument_list|(
literal|"Date: Mon, 28 Sep 1970 06:00:00 GMT"
name|CRLF
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
name|content_type
operator|.
name|len
condition|)
block|{
name|len
operator|+=
sizeof|sizeof
argument_list|(
literal|"Content-Type: "
argument_list|)
operator|-
literal|1
operator|+
name|r
operator|->
name|headers_out
operator|.
name|content_type
operator|.
name|len
operator|+
literal|2
expr_stmt|;
if|if
condition|(
name|r
operator|->
name|headers_out
operator|.
name|content_type_len
operator|==
name|r
operator|->
name|headers_out
operator|.
name|content_type
operator|.
name|len
operator|&&
name|r
operator|->
name|headers_out
operator|.
name|charset
operator|.
name|len
condition|)
block|{
name|len
operator|+=
sizeof|sizeof
argument_list|(
literal|"; charset="
argument_list|)
operator|-
literal|1
operator|+
name|r
operator|->
name|headers_out
operator|.
name|charset
operator|.
name|len
expr_stmt|;
block|}
block|}
if|if
condition|(
name|r
operator|->
name|headers_out
operator|.
name|content_length
operator|==
name|NULL
operator|&&
name|r
operator|->
name|headers_out
operator|.
name|content_length_n
operator|>=
literal|0
condition|)
block|{
name|len
operator|+=
sizeof|sizeof
argument_list|(
literal|"Content-Length: "
argument_list|)
operator|-
literal|1
operator|+
name|NGX_OFF_T_LEN
operator|+
literal|2
expr_stmt|;
block|}
if|if
condition|(
name|r
operator|->
name|headers_out
operator|.
name|last_modified
operator|==
name|NULL
operator|&&
name|r
operator|->
name|headers_out
operator|.
name|last_modified_time
operator|!=
operator|-
literal|1
condition|)
block|{
name|len
operator|+=
sizeof|sizeof
argument_list|(
literal|"Last-Modified: Mon, 28 Sep 1970 06:00:00 GMT"
name|CRLF
argument_list|)
operator|-
literal|1
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
if|if
condition|(
name|r
operator|->
name|headers_out
operator|.
name|location
operator|&&
name|r
operator|->
name|headers_out
operator|.
name|location
operator|->
name|value
operator|.
name|len
operator|&&
name|r
operator|->
name|headers_out
operator|.
name|location
operator|->
name|value
operator|.
name|data
index|[
literal|0
index|]
operator|==
literal|'/'
condition|)
block|{
name|r
operator|->
name|headers_out
operator|.
name|location
operator|->
name|hash
operator|=
literal|0
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_HTTP_SSL
operator|)
if|if
condition|(
name|r
operator|->
name|connection
operator|->
name|ssl
condition|)
block|{
name|len
operator|+=
sizeof|sizeof
argument_list|(
literal|"Location: https://"
argument_list|)
operator|-
literal|1
operator|+
name|r
operator|->
name|server_name
operator|.
name|len
operator|+
name|r
operator|->
name|headers_out
operator|.
name|location
operator|->
name|value
operator|.
name|len
operator|+
literal|2
expr_stmt|;
if|if
condition|(
name|clcf
operator|->
name|port_in_redirect
operator|&&
name|r
operator|->
name|port
operator|!=
literal|443
condition|)
block|{
name|len
operator|+=
name|r
operator|->
name|port_text
operator|->
name|len
expr_stmt|;
block|}
block|}
else|else
endif|#
directive|endif
block|{
name|len
operator|+=
sizeof|sizeof
argument_list|(
literal|"Location: http://"
argument_list|)
operator|-
literal|1
operator|+
name|r
operator|->
name|server_name
operator|.
name|len
operator|+
name|r
operator|->
name|headers_out
operator|.
name|location
operator|->
name|value
operator|.
name|len
operator|+
literal|2
expr_stmt|;
if|if
condition|(
name|clcf
operator|->
name|port_in_redirect
operator|&&
name|r
operator|->
name|port
operator|!=
literal|80
condition|)
block|{
name|len
operator|+=
name|r
operator|->
name|port_text
operator|->
name|len
expr_stmt|;
block|}
block|}
block|}
if|if
condition|(
name|r
operator|->
name|chunked
condition|)
block|{
name|len
operator|+=
sizeof|sizeof
argument_list|(
literal|"Transfer-Encoding: chunked"
name|CRLF
argument_list|)
operator|-
literal|1
expr_stmt|;
block|}
if|if
condition|(
name|r
operator|->
name|keepalive
condition|)
block|{
name|len
operator|+=
sizeof|sizeof
argument_list|(
literal|"Connection: keep-alive"
name|CRLF
argument_list|)
operator|-
literal|1
expr_stmt|;
comment|/*          * MSIE and Opera ignore the "Keep-Alive: timeout=<N>" header.          * MSIE keeps the connection alive for about 60-65 seconds.          * Opera keeps the connection alive very long.          * Mozilla keeps the connection alive for N plus about 1-10 seconds.          * Konqueror keeps the connection alive for about N seconds.          */
if|if
condition|(
name|clcf
operator|->
name|keepalive_header
condition|)
block|{
name|len
operator|+=
sizeof|sizeof
argument_list|(
literal|"Keep-Alive: timeout="
argument_list|)
operator|-
literal|1
operator|+
name|NGX_TIME_T_LEN
operator|+
literal|2
expr_stmt|;
block|}
block|}
else|else
block|{
name|len
operator|+=
sizeof|sizeof
argument_list|(
literal|"Connection: closed"
name|CRLF
argument_list|)
operator|-
literal|1
expr_stmt|;
block|}
name|part
operator|=
operator|&
name|r
operator|->
name|headers_out
operator|.
name|headers
operator|.
name|part
expr_stmt|;
name|header
operator|=
name|part
operator|->
name|elts
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
comment|/* void */
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|i
operator|>=
name|part
operator|->
name|nelts
condition|)
block|{
if|if
condition|(
name|part
operator|->
name|next
operator|==
name|NULL
condition|)
block|{
break|break;
block|}
name|part
operator|=
name|part
operator|->
name|next
expr_stmt|;
name|header
operator|=
name|part
operator|->
name|elts
expr_stmt|;
name|i
operator|=
literal|0
expr_stmt|;
block|}
if|if
condition|(
name|header
index|[
name|i
index|]
operator|.
name|hash
operator|==
literal|0
condition|)
block|{
continue|continue;
block|}
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
sizeof|sizeof
argument_list|(
literal|": "
argument_list|)
operator|-
literal|1
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
sizeof|sizeof
argument_list|(
name|CRLF
argument_list|)
operator|-
literal|1
expr_stmt|;
block|}
name|b
operator|=
name|ngx_create_temp_buf
argument_list|(
name|r
operator|->
name|pool
argument_list|,
name|len
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
name|NGX_ERROR
return|;
block|}
comment|/* "HTTP/1.x " */
name|b
operator|->
name|last
operator|=
name|ngx_cpymem
argument_list|(
name|b
operator|->
name|last
argument_list|,
literal|"HTTP/1.1 "
argument_list|,
sizeof|sizeof
argument_list|(
literal|"HTTP/1.x "
argument_list|)
operator|-
literal|1
argument_list|)
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
name|b
operator|->
name|last
operator|=
name|ngx_copy
argument_list|(
name|b
operator|->
name|last
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
block|}
else|else
block|{
name|b
operator|->
name|last
operator|=
name|ngx_copy
argument_list|(
name|b
operator|->
name|last
argument_list|,
name|ngx_http_status_lines
index|[
name|status
index|]
operator|.
name|data
argument_list|,
name|ngx_http_status_lines
index|[
name|status
index|]
operator|.
name|len
argument_list|)
expr_stmt|;
block|}
operator|*
name|b
operator|->
name|last
operator|++
operator|=
name|CR
expr_stmt|;
operator|*
name|b
operator|->
name|last
operator|++
operator|=
name|LF
expr_stmt|;
if|if
condition|(
name|r
operator|->
name|headers_out
operator|.
name|server
operator|==
name|NULL
condition|)
block|{
name|b
operator|->
name|last
operator|=
name|ngx_cpymem
argument_list|(
name|b
operator|->
name|last
argument_list|,
name|ngx_http_server_string
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_server_string
argument_list|)
operator|-
literal|1
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|r
operator|->
name|headers_out
operator|.
name|date
operator|==
name|NULL
condition|)
block|{
name|b
operator|->
name|last
operator|=
name|ngx_cpymem
argument_list|(
name|b
operator|->
name|last
argument_list|,
literal|"Date: "
argument_list|,
sizeof|sizeof
argument_list|(
literal|"Date: "
argument_list|)
operator|-
literal|1
argument_list|)
expr_stmt|;
name|b
operator|->
name|last
operator|=
name|ngx_cpymem
argument_list|(
name|b
operator|->
name|last
argument_list|,
name|ngx_cached_http_time
operator|.
name|data
argument_list|,
name|ngx_cached_http_time
operator|.
name|len
argument_list|)
expr_stmt|;
operator|*
name|b
operator|->
name|last
operator|++
operator|=
name|CR
expr_stmt|;
operator|*
name|b
operator|->
name|last
operator|++
operator|=
name|LF
expr_stmt|;
block|}
if|if
condition|(
name|r
operator|->
name|headers_out
operator|.
name|content_type
operator|.
name|len
condition|)
block|{
name|b
operator|->
name|last
operator|=
name|ngx_cpymem
argument_list|(
name|b
operator|->
name|last
argument_list|,
literal|"Content-Type: "
argument_list|,
sizeof|sizeof
argument_list|(
literal|"Content-Type: "
argument_list|)
operator|-
literal|1
argument_list|)
expr_stmt|;
name|p
operator|=
name|b
operator|->
name|last
expr_stmt|;
name|b
operator|->
name|last
operator|=
name|ngx_copy
argument_list|(
name|b
operator|->
name|last
argument_list|,
name|r
operator|->
name|headers_out
operator|.
name|content_type
operator|.
name|data
argument_list|,
name|r
operator|->
name|headers_out
operator|.
name|content_type
operator|.
name|len
argument_list|)
expr_stmt|;
if|if
condition|(
name|r
operator|->
name|headers_out
operator|.
name|content_type_len
operator|==
name|r
operator|->
name|headers_out
operator|.
name|content_type
operator|.
name|len
operator|&&
name|r
operator|->
name|headers_out
operator|.
name|charset
operator|.
name|len
condition|)
block|{
name|b
operator|->
name|last
operator|=
name|ngx_cpymem
argument_list|(
name|b
operator|->
name|last
argument_list|,
literal|"; charset="
argument_list|,
sizeof|sizeof
argument_list|(
literal|"; charset="
argument_list|)
operator|-
literal|1
argument_list|)
expr_stmt|;
name|b
operator|->
name|last
operator|=
name|ngx_copy
argument_list|(
name|b
operator|->
name|last
argument_list|,
name|r
operator|->
name|headers_out
operator|.
name|charset
operator|.
name|data
argument_list|,
name|r
operator|->
name|headers_out
operator|.
name|charset
operator|.
name|len
argument_list|)
expr_stmt|;
comment|/* update r->headers_out.content_type for possible logging */
name|r
operator|->
name|headers_out
operator|.
name|content_type
operator|.
name|len
operator|=
name|b
operator|->
name|last
operator|-
name|p
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|content_type
operator|.
name|data
operator|=
name|p
expr_stmt|;
block|}
operator|*
name|b
operator|->
name|last
operator|++
operator|=
name|CR
expr_stmt|;
operator|*
name|b
operator|->
name|last
operator|++
operator|=
name|LF
expr_stmt|;
block|}
if|if
condition|(
name|r
operator|->
name|headers_out
operator|.
name|content_length
operator|==
name|NULL
operator|&&
name|r
operator|->
name|headers_out
operator|.
name|content_length_n
operator|>=
literal|0
condition|)
block|{
name|b
operator|->
name|last
operator|=
name|ngx_sprintf
argument_list|(
name|b
operator|->
name|last
argument_list|,
literal|"Content-Length: %O"
name|CRLF
argument_list|,
name|r
operator|->
name|headers_out
operator|.
name|content_length_n
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|r
operator|->
name|headers_out
operator|.
name|last_modified
operator|==
name|NULL
operator|&&
name|r
operator|->
name|headers_out
operator|.
name|last_modified_time
operator|!=
operator|-
literal|1
condition|)
block|{
name|b
operator|->
name|last
operator|=
name|ngx_cpymem
argument_list|(
name|b
operator|->
name|last
argument_list|,
literal|"Last-Modified: "
argument_list|,
sizeof|sizeof
argument_list|(
literal|"Last-Modified: "
argument_list|)
operator|-
literal|1
argument_list|)
expr_stmt|;
name|b
operator|->
name|last
operator|=
name|ngx_http_time
argument_list|(
name|b
operator|->
name|last
argument_list|,
name|r
operator|->
name|headers_out
operator|.
name|last_modified_time
argument_list|)
expr_stmt|;
operator|*
name|b
operator|->
name|last
operator|++
operator|=
name|CR
expr_stmt|;
operator|*
name|b
operator|->
name|last
operator|++
operator|=
name|LF
expr_stmt|;
block|}
if|if
condition|(
name|r
operator|->
name|headers_out
operator|.
name|location
operator|&&
name|r
operator|->
name|headers_out
operator|.
name|location
operator|->
name|value
operator|.
name|len
operator|&&
name|r
operator|->
name|headers_out
operator|.
name|location
operator|->
name|value
operator|.
name|data
index|[
literal|0
index|]
operator|==
literal|'/'
condition|)
block|{
name|p
operator|=
name|b
operator|->
name|last
operator|+
sizeof|sizeof
argument_list|(
literal|"Location: "
argument_list|)
operator|-
literal|1
expr_stmt|;
name|b
operator|->
name|last
operator|=
name|ngx_cpymem
argument_list|(
name|b
operator|->
name|last
argument_list|,
literal|"Location: http"
argument_list|,
sizeof|sizeof
argument_list|(
literal|"Location: http"
argument_list|)
operator|-
literal|1
argument_list|)
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_HTTP_SSL
operator|)
if|if
condition|(
name|r
operator|->
name|connection
operator|->
name|ssl
condition|)
block|{
operator|*
name|b
operator|->
name|last
operator|++
operator|=
literal|'s'
expr_stmt|;
block|}
endif|#
directive|endif
operator|*
name|b
operator|->
name|last
operator|++
operator|=
literal|':'
expr_stmt|;
operator|*
name|b
operator|->
name|last
operator|++
operator|=
literal|'/'
expr_stmt|;
operator|*
name|b
operator|->
name|last
operator|++
operator|=
literal|'/'
expr_stmt|;
name|b
operator|->
name|last
operator|=
name|ngx_copy
argument_list|(
name|b
operator|->
name|last
argument_list|,
name|r
operator|->
name|server_name
operator|.
name|data
argument_list|,
name|r
operator|->
name|server_name
operator|.
name|len
argument_list|)
expr_stmt|;
if|if
condition|(
name|clcf
operator|->
name|port_in_redirect
condition|)
block|{
if|#
directive|if
operator|(
name|NGX_HTTP_SSL
operator|)
if|if
condition|(
name|r
operator|->
name|connection
operator|->
name|ssl
condition|)
block|{
if|if
condition|(
name|r
operator|->
name|port
operator|!=
literal|443
condition|)
block|{
name|b
operator|->
name|last
operator|=
name|ngx_copy
argument_list|(
name|b
operator|->
name|last
argument_list|,
name|r
operator|->
name|port_text
operator|->
name|data
argument_list|,
name|r
operator|->
name|port_text
operator|->
name|len
argument_list|)
expr_stmt|;
block|}
block|}
else|else
endif|#
directive|endif
block|{
if|if
condition|(
name|r
operator|->
name|port
operator|!=
literal|80
condition|)
block|{
name|b
operator|->
name|last
operator|=
name|ngx_copy
argument_list|(
name|b
operator|->
name|last
argument_list|,
name|r
operator|->
name|port_text
operator|->
name|data
argument_list|,
name|r
operator|->
name|port_text
operator|->
name|len
argument_list|)
expr_stmt|;
block|}
block|}
block|}
name|b
operator|->
name|last
operator|=
name|ngx_copy
argument_list|(
name|b
operator|->
name|last
argument_list|,
name|r
operator|->
name|headers_out
operator|.
name|location
operator|->
name|value
operator|.
name|data
argument_list|,
name|r
operator|->
name|headers_out
operator|.
name|location
operator|->
name|value
operator|.
name|len
argument_list|)
expr_stmt|;
comment|/* update r->headers_out.location->value for possible logging */
name|r
operator|->
name|headers_out
operator|.
name|location
operator|->
name|value
operator|.
name|len
operator|=
name|b
operator|->
name|last
operator|-
name|p
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|location
operator|->
name|value
operator|.
name|data
operator|=
name|p
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|location
operator|->
name|key
operator|.
name|len
operator|=
sizeof|sizeof
argument_list|(
literal|"Location: "
argument_list|)
operator|-
literal|1
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|location
operator|->
name|key
operator|.
name|data
operator|=
operator|(
name|u_char
operator|*
operator|)
literal|"Location: "
expr_stmt|;
operator|*
name|b
operator|->
name|last
operator|++
operator|=
name|CR
expr_stmt|;
operator|*
name|b
operator|->
name|last
operator|++
operator|=
name|LF
expr_stmt|;
block|}
if|if
condition|(
name|r
operator|->
name|chunked
condition|)
block|{
name|b
operator|->
name|last
operator|=
name|ngx_cpymem
argument_list|(
name|b
operator|->
name|last
argument_list|,
literal|"Transfer-Encoding: chunked"
name|CRLF
argument_list|,
sizeof|sizeof
argument_list|(
literal|"Transfer-Encoding: chunked"
name|CRLF
argument_list|)
operator|-
literal|1
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|r
operator|->
name|keepalive
condition|)
block|{
name|b
operator|->
name|last
operator|=
name|ngx_cpymem
argument_list|(
name|b
operator|->
name|last
argument_list|,
literal|"Connection: keep-alive"
name|CRLF
argument_list|,
sizeof|sizeof
argument_list|(
literal|"Connection: keep-alive"
name|CRLF
argument_list|)
operator|-
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|clcf
operator|->
name|keepalive_header
condition|)
block|{
name|b
operator|->
name|last
operator|=
name|ngx_sprintf
argument_list|(
name|b
operator|->
name|last
argument_list|,
literal|"Keep-Alive: timeout=%T"
name|CRLF
argument_list|,
name|clcf
operator|->
name|keepalive_header
argument_list|)
expr_stmt|;
block|}
block|}
else|else
block|{
name|b
operator|->
name|last
operator|=
name|ngx_cpymem
argument_list|(
name|b
operator|->
name|last
argument_list|,
literal|"Connection: close"
name|CRLF
argument_list|,
sizeof|sizeof
argument_list|(
literal|"Connection: close"
name|CRLF
argument_list|)
operator|-
literal|1
argument_list|)
expr_stmt|;
block|}
name|part
operator|=
operator|&
name|r
operator|->
name|headers_out
operator|.
name|headers
operator|.
name|part
expr_stmt|;
name|header
operator|=
name|part
operator|->
name|elts
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
comment|/* void */
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|i
operator|>=
name|part
operator|->
name|nelts
condition|)
block|{
if|if
condition|(
name|part
operator|->
name|next
operator|==
name|NULL
condition|)
block|{
break|break;
block|}
name|part
operator|=
name|part
operator|->
name|next
expr_stmt|;
name|header
operator|=
name|part
operator|->
name|elts
expr_stmt|;
name|i
operator|=
literal|0
expr_stmt|;
block|}
if|if
condition|(
name|header
index|[
name|i
index|]
operator|.
name|hash
operator|==
literal|0
condition|)
block|{
continue|continue;
block|}
name|b
operator|->
name|last
operator|=
name|ngx_copy
argument_list|(
name|b
operator|->
name|last
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
operator|*
name|b
operator|->
name|last
operator|++
operator|=
literal|':'
expr_stmt|;
operator|*
name|b
operator|->
name|last
operator|++
operator|=
literal|' '
expr_stmt|;
name|b
operator|->
name|last
operator|=
name|ngx_copy
argument_list|(
name|b
operator|->
name|last
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
operator|*
name|b
operator|->
name|last
operator|++
operator|=
name|CR
expr_stmt|;
operator|*
name|b
operator|->
name|last
operator|++
operator|=
name|LF
expr_stmt|;
block|}
if|#
directive|if
operator|(
name|NGX_DEBUG
operator|)
operator|*
name|b
operator|->
name|last
operator|=
literal|'\0'
expr_stmt|;
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
literal|"%s\n"
argument_list|,
name|b
operator|->
name|pos
argument_list|)
expr_stmt|;
endif|#
directive|endif
comment|/* the end of HTTP header */
operator|*
name|b
operator|->
name|last
operator|++
operator|=
name|CR
expr_stmt|;
operator|*
name|b
operator|->
name|last
operator|++
operator|=
name|LF
expr_stmt|;
name|r
operator|->
name|header_size
operator|=
name|b
operator|->
name|last
operator|-
name|b
operator|->
name|pos
expr_stmt|;
if|if
condition|(
name|r
operator|->
name|header_only
condition|)
block|{
name|b
operator|->
name|last_buf
operator|=
literal|1
expr_stmt|;
block|}
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
name|ngx_http_write_filter
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
specifier|static
name|ngx_int_t
DECL|function|ngx_http_header_filter_init (ngx_cycle_t * cycle)
name|ngx_http_header_filter_init
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
block|{
name|ngx_http_top_header_filter
operator|=
name|ngx_http_header_filter
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

end_unit

