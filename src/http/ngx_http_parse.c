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

begin_function
DECL|function|ngx_http_parse_request_line (ngx_http_request_t * r)
name|ngx_int_t
name|ngx_http_parse_request_line
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
block|{
name|u_char
name|ch
decl_stmt|,
modifier|*
name|p
decl_stmt|,
modifier|*
name|m
decl_stmt|;
DECL|enum|__anon28ebfb140103
enum|enum
block|{
DECL|enumerator|sw_start
name|sw_start
init|=
literal|0
block|,
DECL|enumerator|sw_method
name|sw_method
block|,
DECL|enumerator|sw_space_after_method
name|sw_space_after_method
block|,
DECL|enumerator|sw_spaces_before_uri
name|sw_spaces_before_uri
block|,
DECL|enumerator|sw_schema
name|sw_schema
block|,
DECL|enumerator|sw_schema_slash
name|sw_schema_slash
block|,
DECL|enumerator|sw_schema_slash_slash
name|sw_schema_slash_slash
block|,
DECL|enumerator|sw_host
name|sw_host
block|,
DECL|enumerator|sw_port
name|sw_port
block|,
DECL|enumerator|sw_after_slash_in_uri
name|sw_after_slash_in_uri
block|,
DECL|enumerator|sw_check_uri
name|sw_check_uri
block|,
DECL|enumerator|sw_uri
name|sw_uri
block|,
DECL|enumerator|sw_http_09
name|sw_http_09
block|,
DECL|enumerator|sw_http_H
name|sw_http_H
block|,
DECL|enumerator|sw_http_HT
name|sw_http_HT
block|,
DECL|enumerator|sw_http_HTT
name|sw_http_HTT
block|,
DECL|enumerator|sw_http_HTTP
name|sw_http_HTTP
block|,
DECL|enumerator|sw_first_major_digit
name|sw_first_major_digit
block|,
DECL|enumerator|sw_major_digit
name|sw_major_digit
block|,
DECL|enumerator|sw_first_minor_digit
name|sw_first_minor_digit
block|,
DECL|enumerator|sw_minor_digit
name|sw_minor_digit
block|,
DECL|enumerator|sw_almost_done
name|sw_almost_done
block|,
DECL|enumerator|sw_done
name|sw_done
block|}
name|state
enum|;
name|state
operator|=
name|r
operator|->
name|state
expr_stmt|;
name|p
operator|=
name|r
operator|->
name|header_in
operator|->
name|pos
expr_stmt|;
while|while
condition|(
name|p
operator|<
name|r
operator|->
name|header_in
operator|->
name|last
operator|&&
name|state
operator|<
name|sw_done
condition|)
block|{
name|ch
operator|=
operator|*
name|p
operator|++
expr_stmt|;
comment|/* gcc 2.95.2 and vc 6.0 compile this switch as an jump table */
switch|switch
condition|(
name|state
condition|)
block|{
comment|/* HTTP methods: GET, HEAD, POST */
case|case
name|sw_start
case|:
name|r
operator|->
name|request_start
operator|=
name|p
operator|-
literal|1
expr_stmt|;
if|if
condition|(
name|ch
operator|==
name|CR
operator|||
name|ch
operator|==
name|LF
condition|)
block|{
break|break;
block|}
if|if
condition|(
name|ch
argument_list|<
literal|'A'
operator|||
name|ch
argument_list|>
literal|'Z'
condition|)
block|{
return|return
name|NGX_HTTP_PARSE_INVALID_METHOD
return|;
block|}
name|state
operator|=
name|sw_method
expr_stmt|;
break|break;
case|case
name|sw_method
case|:
if|if
condition|(
name|ch
operator|==
literal|' '
condition|)
block|{
name|r
operator|->
name|method_end
operator|=
name|p
operator|-
literal|1
expr_stmt|;
name|m
operator|=
name|r
operator|->
name|request_start
expr_stmt|;
if|if
condition|(
name|r
operator|->
name|method_end
operator|-
name|m
operator|==
literal|3
condition|)
block|{
if|if
condition|(
operator|*
name|m
operator|==
literal|'G'
operator|&&
operator|*
operator|(
name|m
operator|+
literal|1
operator|)
operator|==
literal|'E'
operator|&&
operator|*
operator|(
name|m
operator|+
literal|2
operator|)
operator|==
literal|'T'
condition|)
block|{
name|r
operator|->
name|method
operator|=
name|NGX_HTTP_GET
expr_stmt|;
block|}
block|}
if|else if
condition|(
name|r
operator|->
name|method_end
operator|-
name|m
operator|==
literal|4
condition|)
block|{
if|if
condition|(
operator|*
name|m
operator|==
literal|'P'
operator|&&
operator|*
operator|(
name|m
operator|+
literal|1
operator|)
operator|==
literal|'O'
operator|&&
operator|*
operator|(
name|m
operator|+
literal|2
operator|)
operator|==
literal|'T'
operator|&&
operator|*
operator|(
name|m
operator|+
literal|3
operator|)
operator|==
literal|'T'
condition|)
block|{
name|r
operator|->
name|method
operator|=
name|NGX_HTTP_POST
expr_stmt|;
block|}
if|else if
condition|(
operator|*
name|m
operator|==
literal|'H'
operator|&&
operator|*
operator|(
name|m
operator|+
literal|1
operator|)
operator|==
literal|'E'
operator|&&
operator|*
operator|(
name|m
operator|+
literal|2
operator|)
operator|==
literal|'A'
operator|&&
operator|*
operator|(
name|m
operator|+
literal|3
operator|)
operator|==
literal|'D'
condition|)
block|{
name|r
operator|->
name|method
operator|=
name|NGX_HTTP_HEAD
expr_stmt|;
block|}
block|}
name|state
operator|=
name|sw_spaces_before_uri
expr_stmt|;
break|break;
block|}
if|if
condition|(
name|ch
argument_list|<
literal|'A'
operator|||
name|ch
argument_list|>
literal|'Z'
condition|)
block|{
return|return
name|NGX_HTTP_PARSE_INVALID_METHOD
return|;
block|}
break|break;
comment|/* single space after method */
case|case
name|sw_space_after_method
case|:
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
literal|' '
case|:
name|state
operator|=
name|sw_spaces_before_uri
expr_stmt|;
break|break;
default|default:
return|return
name|NGX_HTTP_PARSE_INVALID_METHOD
return|;
block|}
break|break;
comment|/* space* before URI */
case|case
name|sw_spaces_before_uri
case|:
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
literal|'/'
case|:
name|r
operator|->
name|uri_start
operator|=
name|p
operator|-
literal|1
expr_stmt|;
name|state
operator|=
name|sw_after_slash_in_uri
expr_stmt|;
break|break;
case|case
literal|' '
case|:
break|break;
default|default:
if|if
condition|(
operator|(
name|ch
operator|>=
literal|'A'
operator|&&
name|ch
operator|<=
literal|'Z'
operator|)
operator|||
operator|(
name|ch
operator|>=
literal|'a'
operator|&&
name|ch
operator|<=
literal|'z'
operator|)
condition|)
block|{
name|r
operator|->
name|schema_start
operator|=
name|p
operator|-
literal|1
expr_stmt|;
name|state
operator|=
name|sw_schema
expr_stmt|;
break|break;
block|}
return|return
name|NGX_HTTP_PARSE_INVALID_REQUEST
return|;
block|}
break|break;
case|case
name|sw_schema
case|:
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
literal|':'
case|:
name|r
operator|->
name|schema_end
operator|=
name|p
operator|-
literal|1
expr_stmt|;
name|state
operator|=
name|sw_schema_slash
expr_stmt|;
break|break;
default|default:
if|if
condition|(
operator|(
name|ch
operator|>=
literal|'A'
operator|&&
name|ch
operator|<=
literal|'Z'
operator|)
operator|||
operator|(
name|ch
operator|>=
literal|'a'
operator|&&
name|ch
operator|<=
literal|'z'
operator|)
condition|)
block|{
break|break;
block|}
return|return
name|NGX_HTTP_PARSE_INVALID_REQUEST
return|;
block|}
break|break;
case|case
name|sw_schema_slash
case|:
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
literal|'/'
case|:
name|state
operator|=
name|sw_schema_slash_slash
expr_stmt|;
break|break;
default|default:
return|return
name|NGX_HTTP_PARSE_INVALID_REQUEST
return|;
block|}
break|break;
case|case
name|sw_schema_slash_slash
case|:
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
literal|'/'
case|:
name|r
operator|->
name|host_start
operator|=
name|p
operator|-
literal|1
expr_stmt|;
name|state
operator|=
name|sw_host
expr_stmt|;
break|break;
default|default:
return|return
name|NGX_HTTP_PARSE_INVALID_REQUEST
return|;
block|}
break|break;
case|case
name|sw_host
case|:
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
literal|':'
case|:
name|r
operator|->
name|host_end
operator|=
name|p
operator|-
literal|1
expr_stmt|;
name|state
operator|=
name|sw_port
expr_stmt|;
break|break;
case|case
literal|'/'
case|:
name|r
operator|->
name|host_end
operator|=
name|p
operator|-
literal|1
expr_stmt|;
name|r
operator|->
name|uri_start
operator|=
name|p
operator|-
literal|1
expr_stmt|;
name|state
operator|=
name|sw_after_slash_in_uri
expr_stmt|;
break|break;
default|default:
if|if
condition|(
operator|(
name|ch
operator|>=
literal|'A'
operator|&&
name|ch
operator|<=
literal|'Z'
operator|)
operator|||
operator|(
name|ch
operator|>=
literal|'a'
operator|&&
name|ch
operator|<=
literal|'z'
operator|)
operator|||
operator|(
name|ch
operator|>=
literal|'0'
operator|&&
name|ch
operator|<=
literal|'9'
operator|)
operator|||
name|ch
operator|==
literal|'.'
operator|||
name|ch
operator|==
literal|'-'
condition|)
block|{
break|break;
block|}
return|return
name|NGX_HTTP_PARSE_INVALID_REQUEST
return|;
block|}
break|break;
case|case
name|sw_port
case|:
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
literal|'/'
case|:
name|r
operator|->
name|port_end
operator|=
name|p
operator|-
literal|1
expr_stmt|;
name|r
operator|->
name|uri_start
operator|=
name|p
operator|-
literal|1
expr_stmt|;
name|state
operator|=
name|sw_after_slash_in_uri
expr_stmt|;
break|break;
default|default:
if|if
condition|(
name|ch
argument_list|<
literal|'0'
operator|&&
name|ch
argument_list|>
literal|'9'
condition|)
block|{
return|return
name|NGX_HTTP_PARSE_INVALID_REQUEST
return|;
block|}
break|break;
block|}
break|break;
comment|/* check "/.", "//", and "%" in URI */
case|case
name|sw_after_slash_in_uri
case|:
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
name|CR
case|:
name|r
operator|->
name|uri_end
operator|=
name|p
operator|-
literal|1
expr_stmt|;
name|r
operator|->
name|http_minor
operator|=
literal|9
expr_stmt|;
name|state
operator|=
name|sw_almost_done
expr_stmt|;
break|break;
case|case
name|LF
case|:
name|r
operator|->
name|uri_end
operator|=
name|p
operator|-
literal|1
expr_stmt|;
name|r
operator|->
name|http_minor
operator|=
literal|9
expr_stmt|;
name|state
operator|=
name|sw_done
expr_stmt|;
break|break;
case|case
literal|' '
case|:
name|r
operator|->
name|uri_end
operator|=
name|p
operator|-
literal|1
expr_stmt|;
name|state
operator|=
name|sw_http_09
expr_stmt|;
break|break;
case|case
literal|'.'
case|:
case|case
literal|'%'
case|:
name|r
operator|->
name|complex_uri
operator|=
literal|1
expr_stmt|;
name|state
operator|=
name|sw_uri
expr_stmt|;
break|break;
case|case
literal|'/'
case|:
name|r
operator|->
name|complex_uri
operator|=
literal|1
expr_stmt|;
break|break;
case|case
literal|'?'
case|:
name|r
operator|->
name|args_start
operator|=
name|p
expr_stmt|;
name|state
operator|=
name|sw_uri
expr_stmt|;
break|break;
default|default:
name|state
operator|=
name|sw_check_uri
expr_stmt|;
break|break;
block|}
break|break;
comment|/* check "/" and "%" in URI */
case|case
name|sw_check_uri
case|:
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
name|CR
case|:
name|r
operator|->
name|uri_end
operator|=
name|p
operator|-
literal|1
expr_stmt|;
name|r
operator|->
name|http_minor
operator|=
literal|9
expr_stmt|;
name|state
operator|=
name|sw_almost_done
expr_stmt|;
break|break;
case|case
name|LF
case|:
name|r
operator|->
name|uri_end
operator|=
name|p
operator|-
literal|1
expr_stmt|;
name|r
operator|->
name|http_minor
operator|=
literal|9
expr_stmt|;
name|state
operator|=
name|sw_done
expr_stmt|;
break|break;
case|case
literal|' '
case|:
name|r
operator|->
name|uri_end
operator|=
name|p
operator|-
literal|1
expr_stmt|;
name|state
operator|=
name|sw_http_09
expr_stmt|;
break|break;
case|case
literal|'.'
case|:
name|r
operator|->
name|uri_ext
operator|=
name|p
expr_stmt|;
break|break;
case|case
literal|'/'
case|:
name|r
operator|->
name|uri_ext
operator|=
name|NULL
expr_stmt|;
name|state
operator|=
name|sw_after_slash_in_uri
expr_stmt|;
break|break;
case|case
literal|'%'
case|:
name|r
operator|->
name|complex_uri
operator|=
literal|1
expr_stmt|;
name|state
operator|=
name|sw_uri
expr_stmt|;
break|break;
case|case
literal|'?'
case|:
name|r
operator|->
name|args_start
operator|=
name|p
expr_stmt|;
name|state
operator|=
name|sw_uri
expr_stmt|;
break|break;
block|}
break|break;
comment|/* URI */
case|case
name|sw_uri
case|:
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
name|CR
case|:
name|r
operator|->
name|uri_end
operator|=
name|p
operator|-
literal|1
expr_stmt|;
name|r
operator|->
name|http_minor
operator|=
literal|9
expr_stmt|;
name|state
operator|=
name|sw_almost_done
expr_stmt|;
break|break;
case|case
name|LF
case|:
name|r
operator|->
name|uri_end
operator|=
name|p
operator|-
literal|1
expr_stmt|;
name|r
operator|->
name|http_minor
operator|=
literal|9
expr_stmt|;
name|state
operator|=
name|sw_done
expr_stmt|;
break|break;
case|case
literal|' '
case|:
name|r
operator|->
name|uri_end
operator|=
name|p
operator|-
literal|1
expr_stmt|;
name|state
operator|=
name|sw_http_09
expr_stmt|;
break|break;
block|}
break|break;
comment|/* space+ after URI */
case|case
name|sw_http_09
case|:
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
literal|' '
case|:
break|break;
case|case
name|CR
case|:
name|r
operator|->
name|http_minor
operator|=
literal|9
expr_stmt|;
name|state
operator|=
name|sw_almost_done
expr_stmt|;
break|break;
case|case
name|LF
case|:
name|r
operator|->
name|http_minor
operator|=
literal|9
expr_stmt|;
name|state
operator|=
name|sw_done
expr_stmt|;
break|break;
case|case
literal|'H'
case|:
name|state
operator|=
name|sw_http_H
expr_stmt|;
break|break;
default|default:
return|return
name|NGX_HTTP_PARSE_INVALID_REQUEST
return|;
block|}
break|break;
case|case
name|sw_http_H
case|:
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
literal|'T'
case|:
name|state
operator|=
name|sw_http_HT
expr_stmt|;
break|break;
default|default:
return|return
name|NGX_HTTP_PARSE_INVALID_REQUEST
return|;
block|}
break|break;
case|case
name|sw_http_HT
case|:
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
literal|'T'
case|:
name|state
operator|=
name|sw_http_HTT
expr_stmt|;
break|break;
default|default:
return|return
name|NGX_HTTP_PARSE_INVALID_REQUEST
return|;
block|}
break|break;
case|case
name|sw_http_HTT
case|:
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
literal|'P'
case|:
name|state
operator|=
name|sw_http_HTTP
expr_stmt|;
break|break;
default|default:
return|return
name|NGX_HTTP_PARSE_INVALID_REQUEST
return|;
block|}
break|break;
case|case
name|sw_http_HTTP
case|:
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
literal|'/'
case|:
name|state
operator|=
name|sw_first_major_digit
expr_stmt|;
break|break;
default|default:
return|return
name|NGX_HTTP_PARSE_INVALID_REQUEST
return|;
block|}
break|break;
comment|/* first digit of major HTTP version */
case|case
name|sw_first_major_digit
case|:
if|if
condition|(
name|ch
argument_list|<
literal|'1'
operator|||
name|ch
argument_list|>
literal|'9'
condition|)
block|{
return|return
name|NGX_HTTP_PARSE_INVALID_REQUEST
return|;
block|}
name|r
operator|->
name|http_major
operator|=
name|ch
operator|-
literal|'0'
expr_stmt|;
name|state
operator|=
name|sw_major_digit
expr_stmt|;
break|break;
comment|/* major HTTP version or dot */
case|case
name|sw_major_digit
case|:
if|if
condition|(
name|ch
operator|==
literal|'.'
condition|)
block|{
name|state
operator|=
name|sw_first_minor_digit
expr_stmt|;
break|break;
block|}
if|if
condition|(
name|ch
argument_list|<
literal|'0'
operator|||
name|ch
argument_list|>
literal|'9'
condition|)
block|{
return|return
name|NGX_HTTP_PARSE_INVALID_REQUEST
return|;
block|}
name|r
operator|->
name|http_major
operator|=
name|r
operator|->
name|http_major
operator|*
literal|10
operator|+
name|ch
operator|-
literal|'0'
expr_stmt|;
break|break;
comment|/* first digit of minor HTTP version */
case|case
name|sw_first_minor_digit
case|:
if|if
condition|(
name|ch
argument_list|<
literal|'0'
operator|||
name|ch
argument_list|>
literal|'9'
condition|)
block|{
return|return
name|NGX_HTTP_PARSE_INVALID_REQUEST
return|;
block|}
name|r
operator|->
name|http_minor
operator|=
name|ch
operator|-
literal|'0'
expr_stmt|;
name|state
operator|=
name|sw_minor_digit
expr_stmt|;
break|break;
comment|/* minor HTTP version or end of request line */
case|case
name|sw_minor_digit
case|:
if|if
condition|(
name|ch
operator|==
name|CR
condition|)
block|{
name|state
operator|=
name|sw_almost_done
expr_stmt|;
break|break;
block|}
if|if
condition|(
name|ch
operator|==
name|LF
condition|)
block|{
name|state
operator|=
name|sw_done
expr_stmt|;
break|break;
block|}
if|if
condition|(
name|ch
argument_list|<
literal|'0'
operator|||
name|ch
argument_list|>
literal|'9'
condition|)
block|{
return|return
name|NGX_HTTP_PARSE_INVALID_REQUEST
return|;
block|}
name|r
operator|->
name|http_minor
operator|=
name|r
operator|->
name|http_minor
operator|*
literal|10
operator|+
name|ch
operator|-
literal|'0'
expr_stmt|;
break|break;
comment|/* end of request line */
case|case
name|sw_almost_done
case|:
name|r
operator|->
name|request_end
operator|=
name|p
operator|-
literal|2
expr_stmt|;
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
name|LF
case|:
name|state
operator|=
name|sw_done
expr_stmt|;
break|break;
default|default:
return|return
name|NGX_HTTP_PARSE_INVALID_REQUEST
return|;
block|}
break|break;
comment|/* suppress warning */
case|case
name|sw_done
case|:
break|break;
block|}
block|}
name|r
operator|->
name|header_in
operator|->
name|pos
operator|=
name|p
expr_stmt|;
if|if
condition|(
name|state
operator|==
name|sw_done
condition|)
block|{
if|if
condition|(
name|r
operator|->
name|request_end
operator|==
name|NULL
condition|)
block|{
name|r
operator|->
name|request_end
operator|=
name|p
operator|-
literal|1
expr_stmt|;
block|}
name|r
operator|->
name|http_version
operator|=
name|r
operator|->
name|http_major
operator|*
literal|1000
operator|+
name|r
operator|->
name|http_minor
expr_stmt|;
name|r
operator|->
name|state
operator|=
name|sw_start
expr_stmt|;
if|if
condition|(
name|r
operator|->
name|http_version
operator|==
literal|9
operator|&&
name|r
operator|->
name|method
operator|!=
name|NGX_HTTP_GET
condition|)
block|{
return|return
name|NGX_HTTP_PARSE_INVALID_09_METHOD
return|;
block|}
return|return
name|NGX_OK
return|;
block|}
else|else
block|{
name|r
operator|->
name|state
operator|=
name|state
expr_stmt|;
return|return
name|NGX_AGAIN
return|;
block|}
block|}
end_function

begin_function
DECL|function|ngx_http_parse_header_line (ngx_http_request_t * r,ngx_hunk_t * h)
name|ngx_int_t
name|ngx_http_parse_header_line
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_hunk_t
modifier|*
name|h
parameter_list|)
block|{
name|u_char
name|c
decl_stmt|,
name|ch
decl_stmt|,
modifier|*
name|p
decl_stmt|;
DECL|enum|__anon28ebfb140203
enum|enum
block|{
DECL|enumerator|sw_start
name|sw_start
init|=
literal|0
block|,
DECL|enumerator|sw_name
name|sw_name
block|,
DECL|enumerator|sw_space_before_value
name|sw_space_before_value
block|,
DECL|enumerator|sw_value
name|sw_value
block|,
DECL|enumerator|sw_space_after_value
name|sw_space_after_value
block|,
DECL|enumerator|sw_almost_done
name|sw_almost_done
block|,
DECL|enumerator|sw_header_almost_done
name|sw_header_almost_done
block|,
DECL|enumerator|sw_ignore_line
name|sw_ignore_line
block|,
DECL|enumerator|sw_done
name|sw_done
block|,
DECL|enumerator|sw_header_done
name|sw_header_done
block|}
name|state
enum|;
name|state
operator|=
name|r
operator|->
name|state
expr_stmt|;
name|p
operator|=
name|h
operator|->
name|pos
expr_stmt|;
while|while
condition|(
name|p
operator|<
name|h
operator|->
name|last
operator|&&
name|state
operator|<
name|sw_done
condition|)
block|{
name|ch
operator|=
operator|*
name|p
operator|++
expr_stmt|;
switch|switch
condition|(
name|state
condition|)
block|{
comment|/* first char */
case|case
name|sw_start
case|:
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
name|CR
case|:
name|r
operator|->
name|header_end
operator|=
name|p
operator|-
literal|1
expr_stmt|;
name|state
operator|=
name|sw_header_almost_done
expr_stmt|;
break|break;
case|case
name|LF
case|:
name|r
operator|->
name|header_end
operator|=
name|p
operator|-
literal|1
expr_stmt|;
name|state
operator|=
name|sw_header_done
expr_stmt|;
break|break;
default|default:
name|state
operator|=
name|sw_name
expr_stmt|;
name|r
operator|->
name|header_name_start
operator|=
name|p
operator|-
literal|1
expr_stmt|;
name|c
operator|=
operator|(
name|u_char
operator|)
operator|(
name|ch
operator||
literal|0x20
operator|)
expr_stmt|;
if|if
condition|(
name|c
operator|>=
literal|'a'
operator|&&
name|c
operator|<=
literal|'z'
condition|)
block|{
break|break;
block|}
if|if
condition|(
name|ch
operator|==
literal|'-'
operator|||
name|ch
operator|==
literal|'_'
operator|||
name|ch
operator|==
literal|'~'
operator|||
name|ch
operator|==
literal|'.'
condition|)
block|{
break|break;
block|}
if|if
condition|(
name|ch
operator|>=
literal|'0'
operator|&&
name|ch
operator|<=
literal|'9'
condition|)
block|{
break|break;
block|}
return|return
name|NGX_HTTP_PARSE_INVALID_HEADER
return|;
block|}
break|break;
comment|/* header name */
case|case
name|sw_name
case|:
name|c
operator|=
operator|(
name|u_char
operator|)
operator|(
name|ch
operator||
literal|0x20
operator|)
expr_stmt|;
if|if
condition|(
name|c
operator|>=
literal|'a'
operator|&&
name|c
operator|<=
literal|'z'
condition|)
block|{
break|break;
block|}
if|if
condition|(
name|ch
operator|==
literal|':'
condition|)
block|{
name|r
operator|->
name|header_name_end
operator|=
name|p
operator|-
literal|1
expr_stmt|;
name|state
operator|=
name|sw_space_before_value
expr_stmt|;
break|break;
block|}
if|if
condition|(
name|ch
operator|==
literal|'-'
operator|||
name|ch
operator|==
literal|'_'
operator|||
name|ch
operator|==
literal|'~'
operator|||
name|ch
operator|==
literal|'.'
condition|)
block|{
break|break;
block|}
if|if
condition|(
name|ch
operator|>=
literal|'0'
operator|&&
name|ch
operator|<=
literal|'9'
condition|)
block|{
break|break;
block|}
comment|/* IIS can send duplicate "HTTP/1.1 ..." lines */
if|if
condition|(
name|ch
operator|==
literal|'/'
operator|&&
name|r
operator|->
name|proxy
operator|&&
name|p
operator|-
name|r
operator|->
name|header_start
operator|==
literal|5
operator|&&
name|ngx_strncmp
argument_list|(
name|r
operator|->
name|header_start
argument_list|,
literal|"HTTP"
argument_list|,
literal|4
argument_list|)
operator|==
literal|0
condition|)
block|{
name|state
operator|=
name|sw_ignore_line
expr_stmt|;
break|break;
block|}
return|return
name|NGX_HTTP_PARSE_INVALID_HEADER
return|;
comment|/* space* before header value */
case|case
name|sw_space_before_value
case|:
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
literal|' '
case|:
break|break;
case|case
name|CR
case|:
name|r
operator|->
name|header_start
operator|=
name|r
operator|->
name|header_end
operator|=
name|p
operator|-
literal|1
expr_stmt|;
name|state
operator|=
name|sw_almost_done
expr_stmt|;
break|break;
case|case
name|LF
case|:
name|r
operator|->
name|header_start
operator|=
name|r
operator|->
name|header_end
operator|=
name|p
operator|-
literal|1
expr_stmt|;
name|state
operator|=
name|sw_done
expr_stmt|;
break|break;
default|default:
name|r
operator|->
name|header_start
operator|=
name|p
operator|-
literal|1
expr_stmt|;
name|state
operator|=
name|sw_value
expr_stmt|;
break|break;
block|}
break|break;
comment|/* header value */
case|case
name|sw_value
case|:
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
literal|' '
case|:
name|r
operator|->
name|header_end
operator|=
name|p
operator|-
literal|1
expr_stmt|;
name|state
operator|=
name|sw_space_after_value
expr_stmt|;
break|break;
case|case
name|CR
case|:
name|r
operator|->
name|header_end
operator|=
name|p
operator|-
literal|1
expr_stmt|;
name|state
operator|=
name|sw_almost_done
expr_stmt|;
break|break;
case|case
name|LF
case|:
name|r
operator|->
name|header_end
operator|=
name|p
operator|-
literal|1
expr_stmt|;
name|state
operator|=
name|sw_done
expr_stmt|;
break|break;
block|}
break|break;
comment|/* space* before end of header line */
case|case
name|sw_space_after_value
case|:
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
literal|' '
case|:
break|break;
case|case
name|CR
case|:
name|state
operator|=
name|sw_almost_done
expr_stmt|;
break|break;
case|case
name|LF
case|:
name|state
operator|=
name|sw_done
expr_stmt|;
break|break;
default|default:
name|state
operator|=
name|sw_value
expr_stmt|;
break|break;
block|}
break|break;
comment|/* ignore header line */
case|case
name|sw_ignore_line
case|:
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
name|LF
case|:
name|state
operator|=
name|sw_start
expr_stmt|;
break|break;
default|default:
break|break;
block|}
break|break;
comment|/* end of header line */
case|case
name|sw_almost_done
case|:
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
name|LF
case|:
name|state
operator|=
name|sw_done
expr_stmt|;
break|break;
default|default:
return|return
name|NGX_HTTP_PARSE_INVALID_HEADER
return|;
block|}
break|break;
comment|/* end of header */
case|case
name|sw_header_almost_done
case|:
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
name|LF
case|:
name|state
operator|=
name|sw_header_done
expr_stmt|;
break|break;
default|default:
return|return
name|NGX_HTTP_PARSE_INVALID_HEADER
return|;
block|}
break|break;
comment|/* suppress warning */
case|case
name|sw_done
case|:
case|case
name|sw_header_done
case|:
break|break;
block|}
block|}
name|h
operator|->
name|pos
operator|=
name|p
expr_stmt|;
if|if
condition|(
name|state
operator|==
name|sw_done
condition|)
block|{
name|r
operator|->
name|state
operator|=
name|sw_start
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
if|else if
condition|(
name|state
operator|==
name|sw_header_done
condition|)
block|{
name|r
operator|->
name|state
operator|=
name|sw_start
expr_stmt|;
return|return
name|NGX_HTTP_PARSE_HEADER_DONE
return|;
block|}
else|else
block|{
name|r
operator|->
name|state
operator|=
name|state
expr_stmt|;
return|return
name|NGX_AGAIN
return|;
block|}
block|}
end_function

begin_function
DECL|function|ngx_http_parse_complex_uri (ngx_http_request_t * r)
name|ngx_int_t
name|ngx_http_parse_complex_uri
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
block|{
name|u_char
name|c
decl_stmt|,
name|ch
decl_stmt|,
name|decoded
decl_stmt|,
modifier|*
name|p
decl_stmt|,
modifier|*
name|u
decl_stmt|;
DECL|enum|__anon28ebfb140303
enum|enum
block|{
DECL|enumerator|sw_usual
name|sw_usual
init|=
literal|0
block|,
DECL|enumerator|sw_slash
name|sw_slash
block|,
DECL|enumerator|sw_dot
name|sw_dot
block|,
DECL|enumerator|sw_dot_dot
name|sw_dot_dot
block|,
if|#
directive|if
operator|(
name|WIN32
operator|)
DECL|enumerator|sw_dot_dot_dot
name|sw_dot_dot_dot
block|,
endif|#
directive|endif
DECL|enumerator|sw_quoted
name|sw_quoted
block|,
DECL|enumerator|sw_quoted_second
name|sw_quoted_second
block|}
name|state
enum|,
name|quoted_state
enum|;
name|decoded
operator|=
literal|'\0'
expr_stmt|;
name|quoted_state
operator|=
name|sw_usual
expr_stmt|;
name|state
operator|=
name|sw_usual
expr_stmt|;
name|p
operator|=
name|r
operator|->
name|uri_start
expr_stmt|;
name|u
operator|=
name|r
operator|->
name|uri
operator|.
name|data
expr_stmt|;
name|r
operator|->
name|uri_ext
operator|=
name|NULL
expr_stmt|;
name|ch
operator|=
operator|*
name|p
operator|++
expr_stmt|;
while|while
condition|(
name|p
operator|<
name|r
operator|->
name|uri_start
operator|+
name|r
operator|->
name|uri
operator|.
name|len
operator|+
literal|1
condition|)
block|{
name|ngx_log_debug4
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
literal|"s:%d in:'%x:%c', out:'%c'"
argument_list|,
name|state
argument_list|,
name|ch
argument_list|,
name|ch
argument_list|,
operator|*
name|u
argument_list|)
expr_stmt|;
switch|switch
condition|(
name|state
condition|)
block|{
case|case
name|sw_usual
case|:
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
literal|'/'
case|:
name|r
operator|->
name|uri_ext
operator|=
name|NULL
expr_stmt|;
name|state
operator|=
name|sw_slash
expr_stmt|;
operator|*
name|u
operator|++
operator|=
name|ch
expr_stmt|;
break|break;
case|case
literal|'%'
case|:
name|quoted_state
operator|=
name|state
expr_stmt|;
name|state
operator|=
name|sw_quoted
expr_stmt|;
break|break;
case|case
literal|'.'
case|:
name|r
operator|->
name|uri_ext
operator|=
name|u
operator|+
literal|1
expr_stmt|;
default|default:
operator|*
name|u
operator|++
operator|=
name|ch
expr_stmt|;
break|break;
block|}
name|ch
operator|=
operator|*
name|p
operator|++
expr_stmt|;
break|break;
case|case
name|sw_slash
case|:
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
literal|'/'
case|:
break|break;
case|case
literal|'.'
case|:
name|state
operator|=
name|sw_dot
expr_stmt|;
operator|*
name|u
operator|++
operator|=
name|ch
expr_stmt|;
break|break;
case|case
literal|'%'
case|:
name|quoted_state
operator|=
name|state
expr_stmt|;
name|state
operator|=
name|sw_quoted
expr_stmt|;
break|break;
default|default:
name|state
operator|=
name|sw_usual
expr_stmt|;
operator|*
name|u
operator|++
operator|=
name|ch
expr_stmt|;
break|break;
block|}
name|ch
operator|=
operator|*
name|p
operator|++
expr_stmt|;
break|break;
case|case
name|sw_dot
case|:
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
literal|'/'
case|:
name|state
operator|=
name|sw_slash
expr_stmt|;
name|u
operator|--
expr_stmt|;
break|break;
case|case
literal|'.'
case|:
name|state
operator|=
name|sw_dot_dot
expr_stmt|;
operator|*
name|u
operator|++
operator|=
name|ch
expr_stmt|;
break|break;
case|case
literal|'%'
case|:
name|quoted_state
operator|=
name|state
expr_stmt|;
name|state
operator|=
name|sw_quoted
expr_stmt|;
break|break;
default|default:
name|state
operator|=
name|sw_usual
expr_stmt|;
operator|*
name|u
operator|++
operator|=
name|ch
expr_stmt|;
break|break;
block|}
name|ch
operator|=
operator|*
name|p
operator|++
expr_stmt|;
break|break;
case|case
name|sw_dot_dot
case|:
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
literal|'/'
case|:
name|state
operator|=
name|sw_slash
expr_stmt|;
name|u
operator|-=
literal|4
expr_stmt|;
if|if
condition|(
name|u
operator|<
name|r
operator|->
name|uri
operator|.
name|data
condition|)
block|{
return|return
name|NGX_HTTP_PARSE_INVALID_REQUEST
return|;
block|}
while|while
condition|(
operator|*
operator|(
name|u
operator|-
literal|1
operator|)
operator|!=
literal|'/'
condition|)
block|{
name|u
operator|--
expr_stmt|;
block|}
break|break;
case|case
literal|'%'
case|:
name|quoted_state
operator|=
name|state
expr_stmt|;
name|state
operator|=
name|sw_quoted
expr_stmt|;
break|break;
if|#
directive|if
operator|(
name|WIN32
operator|)
case|case
literal|'.'
case|:
name|state
operator|=
name|sw_dot_dot_dot
expr_stmt|;
operator|*
name|u
operator|++
operator|=
name|ch
expr_stmt|;
break|break;
endif|#
directive|endif
default|default:
name|state
operator|=
name|sw_usual
expr_stmt|;
operator|*
name|u
operator|++
operator|=
name|ch
expr_stmt|;
break|break;
block|}
name|ch
operator|=
operator|*
name|p
operator|++
expr_stmt|;
break|break;
if|#
directive|if
operator|(
name|WIN32
operator|)
case|case
name|sw_dot_dot_dot
case|:
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
literal|'/'
case|:
name|state
operator|=
name|sw_slash
expr_stmt|;
name|u
operator|-=
literal|5
expr_stmt|;
if|if
condition|(
name|u
operator|<
name|r
operator|->
name|uri
operator|.
name|data
condition|)
block|{
return|return
name|NGX_HTTP_PARSE_INVALID_REQUEST
return|;
block|}
while|while
condition|(
operator|*
name|u
operator|!=
literal|'/'
condition|)
block|{
name|u
operator|--
expr_stmt|;
block|}
if|if
condition|(
name|u
operator|<
name|r
operator|->
name|uri
operator|.
name|data
condition|)
block|{
return|return
name|NGX_HTTP_PARSE_INVALID_REQUEST
return|;
block|}
while|while
condition|(
operator|*
operator|(
name|u
operator|-
literal|1
operator|)
operator|!=
literal|'/'
condition|)
block|{
name|u
operator|--
expr_stmt|;
block|}
break|break;
case|case
literal|'%'
case|:
name|quoted_state
operator|=
name|state
expr_stmt|;
name|state
operator|=
name|sw_quoted
expr_stmt|;
break|break;
default|default:
name|state
operator|=
name|sw_usual
expr_stmt|;
operator|*
name|u
operator|++
operator|=
name|ch
expr_stmt|;
break|break;
block|}
name|ch
operator|=
operator|*
name|p
operator|++
expr_stmt|;
break|break;
endif|#
directive|endif
case|case
name|sw_quoted
case|:
if|if
condition|(
name|ch
operator|>=
literal|'0'
operator|&&
name|ch
operator|<=
literal|'9'
condition|)
block|{
name|decoded
operator|=
operator|(
name|u_char
operator|)
operator|(
name|ch
operator|-
literal|'0'
operator|)
expr_stmt|;
name|state
operator|=
name|sw_quoted_second
expr_stmt|;
name|ch
operator|=
operator|*
name|p
operator|++
expr_stmt|;
break|break;
block|}
name|c
operator|=
operator|(
name|u_char
operator|)
operator|(
name|ch
operator||
literal|0x20
operator|)
expr_stmt|;
if|if
condition|(
name|c
operator|>=
literal|'a'
operator|&&
name|c
operator|<=
literal|'f'
condition|)
block|{
name|decoded
operator|=
operator|(
name|u_char
operator|)
operator|(
name|c
operator|-
literal|'a'
operator|+
literal|10
operator|)
expr_stmt|;
name|state
operator|=
name|sw_quoted_second
expr_stmt|;
name|ch
operator|=
operator|*
name|p
operator|++
expr_stmt|;
break|break;
block|}
return|return
name|NGX_HTTP_PARSE_INVALID_REQUEST
return|;
case|case
name|sw_quoted_second
case|:
if|if
condition|(
name|ch
operator|>=
literal|'0'
operator|&&
name|ch
operator|<=
literal|'9'
condition|)
block|{
name|ch
operator|=
operator|(
name|u_char
operator|)
operator|(
operator|(
name|decoded
operator|<<
literal|4
operator|)
operator|+
name|ch
operator|-
literal|'0'
operator|)
expr_stmt|;
if|if
condition|(
name|ch
operator|==
literal|'%'
condition|)
block|{
name|state
operator|=
name|sw_usual
expr_stmt|;
operator|*
name|u
operator|++
operator|=
name|ch
expr_stmt|;
name|ch
operator|=
operator|*
name|p
operator|++
expr_stmt|;
break|break;
block|}
name|state
operator|=
name|quoted_state
expr_stmt|;
break|break;
block|}
name|c
operator|=
operator|(
name|u_char
operator|)
operator|(
name|ch
operator||
literal|0x20
operator|)
expr_stmt|;
if|if
condition|(
name|c
operator|>=
literal|'a'
operator|&&
name|c
operator|<=
literal|'f'
condition|)
block|{
name|ch
operator|=
operator|(
name|u_char
operator|)
operator|(
operator|(
name|decoded
operator|<<
literal|4
operator|)
operator|+
name|c
operator|-
literal|'a'
operator|+
literal|10
operator|)
expr_stmt|;
if|if
condition|(
name|ch
operator|==
literal|'%'
condition|)
block|{
name|state
operator|=
name|sw_usual
expr_stmt|;
operator|*
name|u
operator|++
operator|=
name|ch
expr_stmt|;
name|ch
operator|=
operator|*
name|p
operator|++
expr_stmt|;
break|break;
block|}
name|state
operator|=
name|quoted_state
expr_stmt|;
break|break;
block|}
return|return
name|NGX_HTTP_PARSE_INVALID_REQUEST
return|;
block|}
block|}
name|r
operator|->
name|uri
operator|.
name|len
operator|=
name|u
operator|-
name|r
operator|->
name|uri
operator|.
name|data
expr_stmt|;
name|r
operator|->
name|uri
operator|.
name|data
index|[
name|r
operator|->
name|uri
operator|.
name|len
index|]
operator|=
literal|'\0'
expr_stmt|;
if|if
condition|(
name|r
operator|->
name|uri_ext
condition|)
block|{
name|r
operator|->
name|exten
operator|.
name|len
operator|=
name|u
operator|-
name|r
operator|->
name|uri_ext
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|r
operator|->
name|exten
operator|.
name|data
operator|=
name|ngx_palloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
name|r
operator|->
name|exten
operator|.
name|len
operator|+
literal|1
argument_list|)
operator|)
condition|)
block|{
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
name|ngx_cpystrn
argument_list|(
name|r
operator|->
name|exten
operator|.
name|data
argument_list|,
name|r
operator|->
name|uri_ext
argument_list|,
name|r
operator|->
name|exten
operator|.
name|len
operator|+
literal|1
argument_list|)
expr_stmt|;
block|}
name|r
operator|->
name|uri_ext
operator|=
name|NULL
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

end_unit

