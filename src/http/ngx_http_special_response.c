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
name|ngx_http_send_error_page
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_err_page_t
modifier|*
name|err_page
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_send_special_response
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_core_loc_conf_t
modifier|*
name|clcf
parameter_list|,
name|ngx_uint_t
name|err
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_send_refresh
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
function_decl|;
end_function_decl

begin_decl_stmt
DECL|variable|ngx_http_error_full_tail
specifier|static
name|u_char
name|ngx_http_error_full_tail
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
DECL|variable|ngx_http_error_tail
specifier|static
name|u_char
name|ngx_http_error_tail
index|[]
init|=
literal|"<hr><center>nginx</center>"
name|CRLF
literal|"</body>"
name|CRLF
literal|"</html>"
name|CRLF
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_msie_stub
specifier|static
name|u_char
name|ngx_http_msie_stub
index|[]
init|=
literal|"<!-- The padding to disable MSIE's friendly error page -->"
name|CRLF
literal|"<!-- The padding to disable MSIE's friendly error page -->"
name|CRLF
literal|"<!-- The padding to disable MSIE's friendly error page -->"
name|CRLF
literal|"<!-- The padding to disable MSIE's friendly error page -->"
name|CRLF
literal|"<!-- The padding to disable MSIE's friendly error page -->"
name|CRLF
literal|"<!-- The padding to disable MSIE's friendly error page -->"
name|CRLF
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_msie_refresh_head
specifier|static
name|u_char
name|ngx_http_msie_refresh_head
index|[]
init|=
literal|"<html><head><meta http-equiv=\"Refresh\" content=\"0; URL="
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_msie_refresh_tail
specifier|static
name|u_char
name|ngx_http_msie_refresh_tail
index|[]
init|=
literal|"\"></head><body></body></html>"
name|CRLF
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_error_301_page
specifier|static
name|char
name|ngx_http_error_301_page
index|[]
init|=
literal|"<html>"
name|CRLF
literal|"<head><title>301 Moved Permanently</title></head>"
name|CRLF
literal|"<body bgcolor=\"white\">"
name|CRLF
literal|"<center><h1>301 Moved Permanently</h1></center>"
name|CRLF
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_error_302_page
specifier|static
name|char
name|ngx_http_error_302_page
index|[]
init|=
literal|"<html>"
name|CRLF
literal|"<head><title>302 Found</title></head>"
name|CRLF
literal|"<body bgcolor=\"white\">"
name|CRLF
literal|"<center><h1>302 Found</h1></center>"
name|CRLF
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_error_400_page
specifier|static
name|char
name|ngx_http_error_400_page
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
DECL|variable|ngx_http_error_401_page
specifier|static
name|char
name|ngx_http_error_401_page
index|[]
init|=
literal|"<html>"
name|CRLF
literal|"<head><title>401 Authorization Required</title></head>"
name|CRLF
literal|"<body bgcolor=\"white\">"
name|CRLF
literal|"<center><h1>401 Authorization Required</h1></center>"
name|CRLF
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_error_402_page
specifier|static
name|char
name|ngx_http_error_402_page
index|[]
init|=
literal|"<html>"
name|CRLF
literal|"<head><title>402 Payment Required</title></head>"
name|CRLF
literal|"<body bgcolor=\"white\">"
name|CRLF
literal|"<center><h1>402 Payment Required</h1></center>"
name|CRLF
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_error_403_page
specifier|static
name|char
name|ngx_http_error_403_page
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
DECL|variable|ngx_http_error_404_page
specifier|static
name|char
name|ngx_http_error_404_page
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
DECL|variable|ngx_http_error_405_page
specifier|static
name|char
name|ngx_http_error_405_page
index|[]
init|=
literal|"<html>"
name|CRLF
literal|"<head><title>405 Not Allowed</title></head>"
name|CRLF
literal|"<body bgcolor=\"white\">"
name|CRLF
literal|"<center><h1>405 Not Allowed</h1></center>"
name|CRLF
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_error_406_page
specifier|static
name|char
name|ngx_http_error_406_page
index|[]
init|=
literal|"<html>"
name|CRLF
literal|"<head><title>406 Not Acceptable</title></head>"
name|CRLF
literal|"<body bgcolor=\"white\">"
name|CRLF
literal|"<center><h1>406 Not Acceptable</h1></center>"
name|CRLF
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_error_408_page
specifier|static
name|char
name|ngx_http_error_408_page
index|[]
init|=
literal|"<html>"
name|CRLF
literal|"<head><title>408 Request Time-out</title></head>"
name|CRLF
literal|"<body bgcolor=\"white\">"
name|CRLF
literal|"<center><h1>408 Request Time-out</h1></center>"
name|CRLF
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_error_409_page
specifier|static
name|char
name|ngx_http_error_409_page
index|[]
init|=
literal|"<html>"
name|CRLF
literal|"<head><title>409 Conflict</title></head>"
name|CRLF
literal|"<body bgcolor=\"white\">"
name|CRLF
literal|"<center><h1>409 Conflict</h1></center>"
name|CRLF
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_error_410_page
specifier|static
name|char
name|ngx_http_error_410_page
index|[]
init|=
literal|"<html>"
name|CRLF
literal|"<head><title>410 Gone</title></head>"
name|CRLF
literal|"<body bgcolor=\"white\">"
name|CRLF
literal|"<center><h1>410 Gone</h1></center>"
name|CRLF
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_error_411_page
specifier|static
name|char
name|ngx_http_error_411_page
index|[]
init|=
literal|"<html>"
name|CRLF
literal|"<head><title>411 Length Required</title></head>"
name|CRLF
literal|"<body bgcolor=\"white\">"
name|CRLF
literal|"<center><h1>411 Length Required</h1></center>"
name|CRLF
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_error_412_page
specifier|static
name|char
name|ngx_http_error_412_page
index|[]
init|=
literal|"<html>"
name|CRLF
literal|"<head><title>412 Precondition Failed</title></head>"
name|CRLF
literal|"<body bgcolor=\"white\">"
name|CRLF
literal|"<center><h1>412 Precondition Failed</h1></center>"
name|CRLF
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_error_413_page
specifier|static
name|char
name|ngx_http_error_413_page
index|[]
init|=
literal|"<html>"
name|CRLF
literal|"<head><title>413 Request Entity Too Large</title></head>"
name|CRLF
literal|"<body bgcolor=\"white\">"
name|CRLF
literal|"<center><h1>413 Request Entity Too Large</h1></center>"
name|CRLF
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_error_414_page
specifier|static
name|char
name|ngx_http_error_414_page
index|[]
init|=
literal|"<html>"
name|CRLF
literal|"<head><title>414 Request-URI Too Large</title></head>"
name|CRLF
literal|"<body bgcolor=\"white\">"
name|CRLF
literal|"<center><h1>414 Request-URI Too Large</h1></center>"
name|CRLF
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_error_415_page
specifier|static
name|char
name|ngx_http_error_415_page
index|[]
init|=
literal|"<html>"
name|CRLF
literal|"<head><title>415 Unsupported Media Type</title></head>"
name|CRLF
literal|"<body bgcolor=\"white\">"
name|CRLF
literal|"<center><h1>415 Unsupported Media Type</h1></center>"
name|CRLF
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_error_416_page
specifier|static
name|char
name|ngx_http_error_416_page
index|[]
init|=
literal|"<html>"
name|CRLF
literal|"<head><title>416 Requested Range Not Satisfiable</title></head>"
name|CRLF
literal|"<body bgcolor=\"white\">"
name|CRLF
literal|"<center><h1>416 Requested Range Not Satisfiable</h1></center>"
name|CRLF
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_error_495_page
specifier|static
name|char
name|ngx_http_error_495_page
index|[]
init|=
literal|"<html>"
name|CRLF
literal|"<head><title>400 The SSL certificate error</title></head>"
name|CRLF
literal|"<body bgcolor=\"white\">"
name|CRLF
literal|"<center><h1>400 Bad Request</h1></center>"
name|CRLF
literal|"<center>The SSL certificate error</center>"
name|CRLF
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_error_496_page
specifier|static
name|char
name|ngx_http_error_496_page
index|[]
init|=
literal|"<html>"
name|CRLF
literal|"<head><title>400 No required SSL certificate was sent</title></head>"
name|CRLF
literal|"<body bgcolor=\"white\">"
name|CRLF
literal|"<center><h1>400 Bad Request</h1></center>"
name|CRLF
literal|"<center>No required SSL certificate was sent</center>"
name|CRLF
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_error_497_page
specifier|static
name|char
name|ngx_http_error_497_page
index|[]
init|=
literal|"<html>"
name|CRLF
literal|"<head><title>400 The plain HTTP request was sent to HTTPS port</title></head>"
name|CRLF
literal|"<body bgcolor=\"white\">"
name|CRLF
literal|"<center><h1>400 Bad Request</h1></center>"
name|CRLF
literal|"<center>The plain HTTP request was sent to HTTPS port</center>"
name|CRLF
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_error_500_page
specifier|static
name|char
name|ngx_http_error_500_page
index|[]
init|=
literal|"<html>"
name|CRLF
literal|"<head><title>500 Internal Server Error</title></head>"
name|CRLF
literal|"<body bgcolor=\"white\">"
name|CRLF
literal|"<center><h1>500 Internal Server Error</h1></center>"
name|CRLF
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_error_501_page
specifier|static
name|char
name|ngx_http_error_501_page
index|[]
init|=
literal|"<html>"
name|CRLF
literal|"<head><title>501 Method Not Implemented</title></head>"
name|CRLF
literal|"<body bgcolor=\"white\">"
name|CRLF
literal|"<center><h1>501 Method Not Implemented</h1></center>"
name|CRLF
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_error_502_page
specifier|static
name|char
name|ngx_http_error_502_page
index|[]
init|=
literal|"<html>"
name|CRLF
literal|"<head><title>502 Bad Gateway</title></head>"
name|CRLF
literal|"<body bgcolor=\"white\">"
name|CRLF
literal|"<center><h1>502 Bad Gateway</h1></center>"
name|CRLF
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_error_503_page
specifier|static
name|char
name|ngx_http_error_503_page
index|[]
init|=
literal|"<html>"
name|CRLF
literal|"<head><title>503 Service Temporarily Unavailable</title></head>"
name|CRLF
literal|"<body bgcolor=\"white\">"
name|CRLF
literal|"<center><h1>503 Service Temporarily Unavailable</h1></center>"
name|CRLF
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_error_504_page
specifier|static
name|char
name|ngx_http_error_504_page
index|[]
init|=
literal|"<html>"
name|CRLF
literal|"<head><title>504 Gateway Time-out</title></head>"
name|CRLF
literal|"<body bgcolor=\"white\">"
name|CRLF
literal|"<center><h1>504 Gateway Time-out</h1></center>"
name|CRLF
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_error_507_page
specifier|static
name|char
name|ngx_http_error_507_page
index|[]
init|=
literal|"<html>"
name|CRLF
literal|"<head><title>507 Insufficient Storage</title></head>"
name|CRLF
literal|"<body bgcolor=\"white\">"
name|CRLF
literal|"<center><h1>507 Insufficient Storage</h1></center>"
name|CRLF
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_error_pages
specifier|static
name|ngx_str_t
name|ngx_http_error_pages
index|[]
init|=
block|{
name|ngx_null_string
block|,
comment|/* 201, 204 */
DECL|macro|NGX_HTTP_LEVEL_200
define|#
directive|define
name|NGX_HTTP_LEVEL_200
value|1
comment|/* ngx_null_string, */
comment|/* 300 */
name|ngx_string
argument_list|(
name|ngx_http_error_301_page
argument_list|)
block|,
name|ngx_string
argument_list|(
name|ngx_http_error_302_page
argument_list|)
block|,
name|ngx_null_string
block|,
comment|/* 303 */
DECL|macro|NGX_HTTP_LEVEL_300
define|#
directive|define
name|NGX_HTTP_LEVEL_300
value|3
name|ngx_string
argument_list|(
name|ngx_http_error_400_page
argument_list|)
block|,
name|ngx_string
argument_list|(
name|ngx_http_error_401_page
argument_list|)
block|,
name|ngx_string
argument_list|(
name|ngx_http_error_402_page
argument_list|)
block|,
name|ngx_string
argument_list|(
name|ngx_http_error_403_page
argument_list|)
block|,
name|ngx_string
argument_list|(
name|ngx_http_error_404_page
argument_list|)
block|,
name|ngx_string
argument_list|(
name|ngx_http_error_405_page
argument_list|)
block|,
name|ngx_string
argument_list|(
name|ngx_http_error_406_page
argument_list|)
block|,
name|ngx_null_string
block|,
comment|/* 407 */
name|ngx_string
argument_list|(
name|ngx_http_error_408_page
argument_list|)
block|,
name|ngx_string
argument_list|(
name|ngx_http_error_409_page
argument_list|)
block|,
name|ngx_string
argument_list|(
name|ngx_http_error_410_page
argument_list|)
block|,
name|ngx_string
argument_list|(
name|ngx_http_error_411_page
argument_list|)
block|,
name|ngx_string
argument_list|(
name|ngx_http_error_412_page
argument_list|)
block|,
name|ngx_string
argument_list|(
name|ngx_http_error_413_page
argument_list|)
block|,
name|ngx_string
argument_list|(
name|ngx_http_error_414_page
argument_list|)
block|,
name|ngx_string
argument_list|(
name|ngx_http_error_415_page
argument_list|)
block|,
name|ngx_string
argument_list|(
name|ngx_http_error_416_page
argument_list|)
block|,
DECL|macro|NGX_HTTP_LEVEL_400
define|#
directive|define
name|NGX_HTTP_LEVEL_400
value|17
name|ngx_string
argument_list|(
name|ngx_http_error_495_page
argument_list|)
block|,
comment|/* 495, https certificate error */
name|ngx_string
argument_list|(
name|ngx_http_error_496_page
argument_list|)
block|,
comment|/* 496, https no certificate */
name|ngx_string
argument_list|(
name|ngx_http_error_497_page
argument_list|)
block|,
comment|/* 497, http to https */
name|ngx_string
argument_list|(
name|ngx_http_error_404_page
argument_list|)
block|,
comment|/* 498, canceled */
name|ngx_null_string
block|,
comment|/* 499, client has closed connection */
name|ngx_string
argument_list|(
name|ngx_http_error_500_page
argument_list|)
block|,
name|ngx_string
argument_list|(
name|ngx_http_error_501_page
argument_list|)
block|,
name|ngx_string
argument_list|(
name|ngx_http_error_502_page
argument_list|)
block|,
name|ngx_string
argument_list|(
name|ngx_http_error_503_page
argument_list|)
block|,
name|ngx_string
argument_list|(
name|ngx_http_error_504_page
argument_list|)
block|,
name|ngx_null_string
block|,
comment|/* 505 */
name|ngx_null_string
block|,
comment|/* 506 */
name|ngx_string
argument_list|(
argument|ngx_http_error_507_page
argument_list|)
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_get_name
specifier|static
name|ngx_str_t
name|ngx_http_get_name
init|=
block|{
literal|3
block|,
operator|(
name|u_char
operator|*
operator|)
literal|"GET "
block|}
decl_stmt|;
end_decl_stmt

begin_function
name|ngx_int_t
DECL|function|ngx_http_special_response_handler (ngx_http_request_t * r,ngx_int_t error)
name|ngx_http_special_response_handler
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_int_t
name|error
parameter_list|)
block|{
name|ngx_uint_t
name|i
decl_stmt|,
name|err
decl_stmt|;
name|ngx_http_err_page_t
modifier|*
name|err_page
decl_stmt|;
name|ngx_http_core_loc_conf_t
modifier|*
name|clcf
decl_stmt|;
name|ngx_log_debug3
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
literal|"http special response: %d, \"%V?%V\""
argument_list|,
name|error
argument_list|,
operator|&
name|r
operator|->
name|uri
argument_list|,
operator|&
name|r
operator|->
name|args
argument_list|)
expr_stmt|;
name|r
operator|->
name|err_status
operator|=
name|error
expr_stmt|;
if|if
condition|(
name|r
operator|->
name|keepalive
condition|)
block|{
switch|switch
condition|(
name|error
condition|)
block|{
case|case
name|NGX_HTTP_BAD_REQUEST
case|:
case|case
name|NGX_HTTP_REQUEST_ENTITY_TOO_LARGE
case|:
case|case
name|NGX_HTTP_REQUEST_URI_TOO_LARGE
case|:
case|case
name|NGX_HTTP_TO_HTTPS
case|:
case|case
name|NGX_HTTPS_CERT_ERROR
case|:
case|case
name|NGX_HTTPS_NO_CERT
case|:
case|case
name|NGX_HTTP_INTERNAL_SERVER_ERROR
case|:
name|r
operator|->
name|keepalive
operator|=
literal|0
expr_stmt|;
block|}
block|}
if|if
condition|(
name|r
operator|->
name|lingering_close
operator|==
literal|1
condition|)
block|{
switch|switch
condition|(
name|error
condition|)
block|{
case|case
name|NGX_HTTP_BAD_REQUEST
case|:
case|case
name|NGX_HTTP_TO_HTTPS
case|:
case|case
name|NGX_HTTPS_CERT_ERROR
case|:
case|case
name|NGX_HTTPS_NO_CERT
case|:
name|r
operator|->
name|lingering_close
operator|=
literal|0
expr_stmt|;
block|}
block|}
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
operator|!
name|r
operator|->
name|error_page
operator|&&
name|clcf
operator|->
name|error_pages
operator|&&
name|r
operator|->
name|uri_changes
operator|!=
literal|0
condition|)
block|{
if|if
condition|(
name|clcf
operator|->
name|recursive_error_pages
operator|==
literal|0
condition|)
block|{
name|r
operator|->
name|error_page
operator|=
literal|1
expr_stmt|;
block|}
name|err_page
operator|=
name|clcf
operator|->
name|error_pages
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
name|clcf
operator|->
name|error_pages
operator|->
name|nelts
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|err_page
index|[
name|i
index|]
operator|.
name|status
operator|==
name|error
condition|)
block|{
return|return
name|ngx_http_send_error_page
argument_list|(
name|r
argument_list|,
operator|&
name|err_page
index|[
name|i
index|]
argument_list|)
return|;
block|}
block|}
block|}
name|r
operator|->
name|expect_tested
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|ngx_http_discard_request_body
argument_list|(
name|r
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
name|error
operator|=
name|NGX_HTTP_INTERNAL_SERVER_ERROR
expr_stmt|;
block|}
if|if
condition|(
name|clcf
operator|->
name|msie_refresh
operator|&&
name|r
operator|->
name|headers_in
operator|.
name|msie
operator|&&
operator|(
name|error
operator|==
name|NGX_HTTP_MOVED_PERMANENTLY
operator|||
name|error
operator|==
name|NGX_HTTP_MOVED_TEMPORARILY
operator|)
condition|)
block|{
return|return
name|ngx_http_send_refresh
argument_list|(
name|r
argument_list|)
return|;
block|}
if|if
condition|(
name|error
operator|==
name|NGX_HTTP_CREATED
condition|)
block|{
comment|/* 201 */
name|err
operator|=
literal|0
expr_stmt|;
name|r
operator|->
name|header_only
operator|=
literal|1
expr_stmt|;
block|}
if|else if
condition|(
name|error
operator|==
name|NGX_HTTP_NO_CONTENT
condition|)
block|{
comment|/* 204 */
name|err
operator|=
literal|0
expr_stmt|;
block|}
if|else if
condition|(
name|error
operator|<
name|NGX_HTTP_BAD_REQUEST
condition|)
block|{
comment|/* 3XX */
name|err
operator|=
name|error
operator|-
name|NGX_HTTP_MOVED_PERMANENTLY
operator|+
name|NGX_HTTP_LEVEL_200
expr_stmt|;
block|}
if|else if
condition|(
name|error
operator|<
name|NGX_HTTP_OWN_CODES
condition|)
block|{
comment|/* 4XX */
name|err
operator|=
name|error
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
comment|/* 49X, 5XX */
name|err
operator|=
name|error
operator|-
name|NGX_HTTP_OWN_CODES
operator|+
name|NGX_HTTP_LEVEL_200
operator|+
name|NGX_HTTP_LEVEL_300
operator|+
name|NGX_HTTP_LEVEL_400
expr_stmt|;
switch|switch
condition|(
name|error
condition|)
block|{
case|case
name|NGX_HTTP_TO_HTTPS
case|:
case|case
name|NGX_HTTPS_CERT_ERROR
case|:
case|case
name|NGX_HTTPS_NO_CERT
case|:
name|r
operator|->
name|err_status
operator|=
name|NGX_HTTP_BAD_REQUEST
expr_stmt|;
break|break;
block|}
block|}
return|return
name|ngx_http_send_special_response
argument_list|(
name|r
argument_list|,
name|clcf
argument_list|,
name|err
argument_list|)
return|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_send_error_page (ngx_http_request_t * r,ngx_http_err_page_t * err_page)
name|ngx_http_send_error_page
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_err_page_t
modifier|*
name|err_page
parameter_list|)
block|{
name|ngx_int_t
name|overwrite
decl_stmt|;
name|ngx_str_t
name|uri
decl_stmt|,
name|args
decl_stmt|;
name|ngx_table_elt_t
modifier|*
name|location
decl_stmt|;
name|ngx_http_core_loc_conf_t
modifier|*
name|clcf
decl_stmt|;
name|overwrite
operator|=
name|err_page
operator|->
name|overwrite
expr_stmt|;
if|if
condition|(
name|overwrite
operator|&&
name|overwrite
operator|!=
name|NGX_HTTP_OK
condition|)
block|{
name|r
operator|->
name|expect_tested
operator|=
literal|1
expr_stmt|;
block|}
name|r
operator|->
name|err_status
operator|=
name|overwrite
expr_stmt|;
name|r
operator|->
name|zero_in_uri
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|ngx_http_complex_value
argument_list|(
name|r
argument_list|,
operator|&
name|err_page
operator|->
name|value
argument_list|,
operator|&
name|uri
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|uri
operator|.
name|data
index|[
literal|0
index|]
operator|==
literal|'/'
condition|)
block|{
if|if
condition|(
name|err_page
operator|->
name|value
operator|.
name|lengths
condition|)
block|{
name|ngx_http_split_args
argument_list|(
name|r
argument_list|,
operator|&
name|uri
argument_list|,
operator|&
name|args
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|args
operator|=
name|err_page
operator|->
name|args
expr_stmt|;
block|}
if|if
condition|(
name|r
operator|->
name|method
operator|!=
name|NGX_HTTP_HEAD
condition|)
block|{
name|r
operator|->
name|method
operator|=
name|NGX_HTTP_GET
expr_stmt|;
name|r
operator|->
name|method_name
operator|=
name|ngx_http_get_name
expr_stmt|;
block|}
return|return
name|ngx_http_internal_redirect
argument_list|(
name|r
argument_list|,
operator|&
name|uri
argument_list|,
operator|&
name|args
argument_list|)
return|;
block|}
if|if
condition|(
name|uri
operator|.
name|data
index|[
literal|0
index|]
operator|==
literal|'@'
condition|)
block|{
return|return
name|ngx_http_named_location
argument_list|(
name|r
argument_list|,
operator|&
name|uri
argument_list|)
return|;
block|}
name|location
operator|=
name|ngx_list_push
argument_list|(
operator|&
name|r
operator|->
name|headers_out
operator|.
name|headers
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
name|r
operator|->
name|err_status
operator|=
name|NGX_HTTP_MOVED_TEMPORARILY
expr_stmt|;
name|location
operator|->
name|hash
operator|=
literal|1
expr_stmt|;
name|location
operator|->
name|key
operator|.
name|len
operator|=
sizeof|sizeof
argument_list|(
literal|"Location"
argument_list|)
operator|-
literal|1
expr_stmt|;
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
literal|"Location"
expr_stmt|;
name|location
operator|->
name|value
operator|=
name|uri
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|location
operator|=
name|location
expr_stmt|;
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
name|msie_refresh
operator|&&
name|r
operator|->
name|headers_in
operator|.
name|msie
condition|)
block|{
return|return
name|ngx_http_send_refresh
argument_list|(
name|r
argument_list|)
return|;
block|}
return|return
name|ngx_http_send_special_response
argument_list|(
name|r
argument_list|,
name|clcf
argument_list|,
name|NGX_HTTP_MOVED_TEMPORARILY
operator|-
name|NGX_HTTP_MOVED_PERMANENTLY
operator|+
name|NGX_HTTP_LEVEL_200
argument_list|)
return|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_send_special_response (ngx_http_request_t * r,ngx_http_core_loc_conf_t * clcf,ngx_uint_t err)
name|ngx_http_send_special_response
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_core_loc_conf_t
modifier|*
name|clcf
parameter_list|,
name|ngx_uint_t
name|err
parameter_list|)
block|{
name|u_char
modifier|*
name|tail
decl_stmt|;
name|size_t
name|len
decl_stmt|;
name|ngx_int_t
name|rc
decl_stmt|;
name|ngx_buf_t
modifier|*
name|b
decl_stmt|;
name|ngx_uint_t
name|msie_padding
decl_stmt|;
name|ngx_chain_t
name|out
index|[
literal|3
index|]
decl_stmt|;
if|if
condition|(
name|clcf
operator|->
name|server_tokens
condition|)
block|{
name|len
operator|=
sizeof|sizeof
argument_list|(
name|ngx_http_error_full_tail
argument_list|)
operator|-
literal|1
expr_stmt|;
name|tail
operator|=
name|ngx_http_error_full_tail
expr_stmt|;
block|}
else|else
block|{
name|len
operator|=
sizeof|sizeof
argument_list|(
name|ngx_http_error_tail
argument_list|)
operator|-
literal|1
expr_stmt|;
name|tail
operator|=
name|ngx_http_error_tail
expr_stmt|;
block|}
name|msie_padding
operator|=
literal|0
expr_stmt|;
if|if
condition|(
operator|!
name|r
operator|->
name|zero_body
condition|)
block|{
if|if
condition|(
name|ngx_http_error_pages
index|[
name|err
index|]
operator|.
name|len
condition|)
block|{
name|r
operator|->
name|headers_out
operator|.
name|content_length_n
operator|=
name|ngx_http_error_pages
index|[
name|err
index|]
operator|.
name|len
operator|+
name|len
expr_stmt|;
if|if
condition|(
name|clcf
operator|->
name|msie_padding
operator|&&
name|r
operator|->
name|headers_in
operator|.
name|msie
operator|&&
name|r
operator|->
name|http_version
operator|>=
name|NGX_HTTP_VERSION_10
operator|&&
name|err
operator|>=
name|NGX_HTTP_LEVEL_300
condition|)
block|{
name|r
operator|->
name|headers_out
operator|.
name|content_length_n
operator|+=
sizeof|sizeof
argument_list|(
name|ngx_http_msie_stub
argument_list|)
operator|-
literal|1
expr_stmt|;
name|msie_padding
operator|=
literal|1
expr_stmt|;
block|}
name|r
operator|->
name|headers_out
operator|.
name|content_type_len
operator|=
sizeof|sizeof
argument_list|(
literal|"text/html"
argument_list|)
operator|-
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
sizeof|sizeof
argument_list|(
literal|"text/html"
argument_list|)
operator|-
literal|1
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|content_type
operator|.
name|data
operator|=
operator|(
name|u_char
operator|*
operator|)
literal|"text/html"
expr_stmt|;
block|}
else|else
block|{
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
else|else
block|{
name|r
operator|->
name|headers_out
operator|.
name|content_length_n
operator|=
literal|0
expr_stmt|;
name|err
operator|=
literal|0
expr_stmt|;
block|}
if|if
condition|(
name|r
operator|->
name|headers_out
operator|.
name|content_length
condition|)
block|{
name|r
operator|->
name|headers_out
operator|.
name|content_length
operator|->
name|hash
operator|=
literal|0
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|content_length
operator|=
name|NULL
expr_stmt|;
block|}
name|ngx_http_clear_accept_ranges
argument_list|(
name|r
argument_list|)
expr_stmt|;
name|ngx_http_clear_last_modified
argument_list|(
name|r
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
name|rc
operator|==
name|NGX_ERROR
operator|||
name|r
operator|->
name|header_only
condition|)
block|{
return|return
name|rc
return|;
block|}
if|if
condition|(
name|ngx_http_error_pages
index|[
name|err
index|]
operator|.
name|len
operator|==
literal|0
condition|)
block|{
return|return
name|NGX_OK
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
name|ngx_http_error_pages
index|[
name|err
index|]
operator|.
name|data
expr_stmt|;
name|b
operator|->
name|last
operator|=
name|ngx_http_error_pages
index|[
name|err
index|]
operator|.
name|data
operator|+
name|ngx_http_error_pages
index|[
name|err
index|]
operator|.
name|len
expr_stmt|;
name|out
index|[
literal|0
index|]
operator|.
name|buf
operator|=
name|b
expr_stmt|;
name|out
index|[
literal|0
index|]
operator|.
name|next
operator|=
operator|&
name|out
index|[
literal|1
index|]
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
name|tail
expr_stmt|;
name|b
operator|->
name|last
operator|=
name|tail
operator|+
name|len
expr_stmt|;
name|out
index|[
literal|1
index|]
operator|.
name|buf
operator|=
name|b
expr_stmt|;
name|out
index|[
literal|1
index|]
operator|.
name|next
operator|=
name|NULL
expr_stmt|;
if|if
condition|(
name|msie_padding
condition|)
block|{
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
name|ngx_http_msie_stub
expr_stmt|;
name|b
operator|->
name|last
operator|=
name|ngx_http_msie_stub
operator|+
sizeof|sizeof
argument_list|(
name|ngx_http_msie_stub
argument_list|)
operator|-
literal|1
expr_stmt|;
name|out
index|[
literal|1
index|]
operator|.
name|next
operator|=
operator|&
name|out
index|[
literal|2
index|]
expr_stmt|;
name|out
index|[
literal|2
index|]
operator|.
name|buf
operator|=
name|b
expr_stmt|;
name|out
index|[
literal|2
index|]
operator|.
name|next
operator|=
name|NULL
expr_stmt|;
block|}
if|if
condition|(
name|r
operator|==
name|r
operator|->
expr|main
condition|)
block|{
name|b
operator|->
name|last_buf
operator|=
literal|1
expr_stmt|;
block|}
name|b
operator|->
name|last_in_chain
operator|=
literal|1
expr_stmt|;
return|return
name|ngx_http_output_filter
argument_list|(
name|r
argument_list|,
operator|&
name|out
index|[
literal|0
index|]
argument_list|)
return|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_send_refresh (ngx_http_request_t * r)
name|ngx_http_send_refresh
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
block|{
name|u_char
modifier|*
name|p
decl_stmt|,
modifier|*
name|location
decl_stmt|;
name|size_t
name|len
decl_stmt|,
name|size
decl_stmt|;
name|uintptr_t
name|escape
decl_stmt|;
name|ngx_int_t
name|rc
decl_stmt|;
name|ngx_buf_t
modifier|*
name|b
decl_stmt|;
name|ngx_chain_t
name|out
decl_stmt|;
name|len
operator|=
name|r
operator|->
name|headers_out
operator|.
name|location
operator|->
name|value
operator|.
name|len
expr_stmt|;
name|location
operator|=
name|r
operator|->
name|headers_out
operator|.
name|location
operator|->
name|value
operator|.
name|data
expr_stmt|;
name|escape
operator|=
literal|2
operator|*
name|ngx_escape_uri
argument_list|(
name|NULL
argument_list|,
name|location
argument_list|,
name|len
argument_list|,
name|NGX_ESCAPE_REFRESH
argument_list|)
expr_stmt|;
name|size
operator|=
sizeof|sizeof
argument_list|(
name|ngx_http_msie_refresh_head
argument_list|)
operator|-
literal|1
operator|+
name|escape
operator|+
name|len
operator|+
sizeof|sizeof
argument_list|(
name|ngx_http_msie_refresh_tail
argument_list|)
operator|-
literal|1
expr_stmt|;
name|r
operator|->
name|err_status
operator|=
name|NGX_HTTP_OK
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|content_type_len
operator|=
sizeof|sizeof
argument_list|(
literal|"text/html"
argument_list|)
operator|-
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
sizeof|sizeof
argument_list|(
literal|"text/html"
argument_list|)
operator|-
literal|1
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|content_type
operator|.
name|data
operator|=
operator|(
name|u_char
operator|*
operator|)
literal|"text/html"
expr_stmt|;
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
name|r
operator|->
name|headers_out
operator|.
name|location
operator|=
name|NULL
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|content_length_n
operator|=
name|size
expr_stmt|;
if|if
condition|(
name|r
operator|->
name|headers_out
operator|.
name|content_length
condition|)
block|{
name|r
operator|->
name|headers_out
operator|.
name|content_length
operator|->
name|hash
operator|=
literal|0
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|content_length
operator|=
name|NULL
expr_stmt|;
block|}
name|ngx_http_clear_accept_ranges
argument_list|(
name|r
argument_list|)
expr_stmt|;
name|ngx_http_clear_last_modified
argument_list|(
name|r
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
name|rc
operator|==
name|NGX_ERROR
operator|||
name|r
operator|->
name|header_only
condition|)
block|{
return|return
name|rc
return|;
block|}
name|b
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
name|b
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|p
operator|=
name|ngx_cpymem
argument_list|(
name|b
operator|->
name|pos
argument_list|,
name|ngx_http_msie_refresh_head
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_msie_refresh_head
argument_list|)
operator|-
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|escape
operator|==
literal|0
condition|)
block|{
name|p
operator|=
name|ngx_cpymem
argument_list|(
name|p
argument_list|,
name|location
argument_list|,
name|len
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|p
operator|=
operator|(
name|u_char
operator|*
operator|)
name|ngx_escape_uri
argument_list|(
name|p
argument_list|,
name|location
argument_list|,
name|len
argument_list|,
name|NGX_ESCAPE_REFRESH
argument_list|)
expr_stmt|;
block|}
name|b
operator|->
name|last
operator|=
name|ngx_cpymem
argument_list|(
name|p
argument_list|,
name|ngx_http_msie_refresh_tail
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_msie_refresh_tail
argument_list|)
operator|-
literal|1
argument_list|)
expr_stmt|;
name|b
operator|->
name|last_buf
operator|=
literal|1
expr_stmt|;
name|b
operator|->
name|last_in_chain
operator|=
literal|1
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
name|ngx_http_output_filter
argument_list|(
name|r
argument_list|,
operator|&
name|out
argument_list|)
return|;
block|}
end_function

end_unit

