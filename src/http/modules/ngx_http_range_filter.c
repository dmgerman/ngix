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

begin_comment
comment|/*  * the single part format:  *  * "HTTP/1.0 206 Partial Content" CRLF  * ... header ...  * "Content-Type: image/jpeg" CRLF  * "Content-Length: SIZE" CRLF  * "Content-Range: bytes START-END/SIZE" CRLF  * CRLF  * ... data ...  *  *  * the mutlipart format:  *  * "HTTP/1.0 206 Partial Content" CRLF  * ... header ...  * "Content-Type: multipart/byteranges; boundary=0123456789" CRLF  * CRLF  * CRLF  * "--0123456789" CRLF  * "Content-Type: image/jpeg" CRLF  * "Content-Range: bytes START0-END0/SIZE" CRLF  * CRLF  * ... data ...  * CRLF  * "--0123456789" CRLF  * "Content-Type: image/jpeg" CRLF  * "Content-Range: bytes START1-END1/SIZE" CRLF  * CRLF  * ... data ...  * CRLF  * "--0123456789--" CRLF  */
end_comment

begin_typedef
DECL|struct|__anon299078ab0108
typedef|typedef
struct|struct
block|{
DECL|member|boundary_header
name|ngx_str_t
name|boundary_header
decl_stmt|;
DECL|typedef|ngx_http_range_filter_ctx_t
block|}
name|ngx_http_range_filter_ctx_t
typedef|;
end_typedef

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_range_header_filter_init
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
name|ngx_http_range_body_filter_init
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
function_decl|;
end_function_decl

begin_decl_stmt
DECL|variable|ngx_http_range_header_filter_module_ctx
specifier|static
name|ngx_http_module_t
name|ngx_http_range_header_filter_module_ctx
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
DECL|variable|ngx_http_range_header_filter_module
name|ngx_module_t
name|ngx_http_range_header_filter_module
init|=
block|{
name|NGX_MODULE
block|,
operator|&
name|ngx_http_range_header_filter_module_ctx
block|,
comment|/* module context */
name|NULL
block|,
comment|/* module directives */
name|NGX_HTTP_MODULE
block|,
comment|/* module type */
name|ngx_http_range_header_filter_init
block|,
comment|/* init module */
name|NULL
comment|/* init child */
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_range_body_filter_module_ctx
specifier|static
name|ngx_http_module_t
name|ngx_http_range_body_filter_module_ctx
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
DECL|variable|ngx_http_range_body_filter_module
name|ngx_module_t
name|ngx_http_range_body_filter_module
init|=
block|{
name|NGX_MODULE
block|,
operator|&
name|ngx_http_range_body_filter_module_ctx
block|,
comment|/* module context */
name|NULL
block|,
comment|/* module directives */
name|NGX_HTTP_MODULE
block|,
comment|/* module type */
name|ngx_http_range_body_filter_init
block|,
comment|/* init module */
name|NULL
comment|/* init child */
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
DECL|function|ngx_http_range_header_filter (ngx_http_request_t * r)
specifier|static
name|ngx_int_t
name|ngx_http_range_header_filter
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
block|{
name|ngx_int_t
name|rc
decl_stmt|;
name|ngx_uint_t
name|boundary
decl_stmt|,
name|suffix
decl_stmt|,
name|i
decl_stmt|;
name|u_char
modifier|*
name|p
decl_stmt|;
name|size_t
name|len
decl_stmt|;
name|off_t
name|start
decl_stmt|,
name|end
decl_stmt|;
name|ngx_http_range_t
modifier|*
name|range
decl_stmt|;
name|ngx_http_range_filter_ctx_t
modifier|*
name|ctx
decl_stmt|;
if|if
condition|(
name|r
operator|->
name|http_version
operator|<
name|NGX_HTTP_VERSION_10
operator|||
name|r
operator|->
name|headers_out
operator|.
name|status
operator|!=
name|NGX_HTTP_OK
operator|||
name|r
operator|->
name|headers_out
operator|.
name|content_length_n
operator|==
operator|-
literal|1
operator|||
operator|!
name|r
operator|->
name|filter_allow_ranges
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
name|headers_in
operator|.
name|range
operator|==
name|NULL
operator|||
name|r
operator|->
name|headers_in
operator|.
name|range
operator|->
name|value
operator|.
name|len
operator|<
literal|7
operator|||
name|ngx_strncasecmp
argument_list|(
name|r
operator|->
name|headers_in
operator|.
name|range
operator|->
name|value
operator|.
name|data
argument_list|,
literal|"bytes="
argument_list|,
literal|6
argument_list|)
operator|!=
literal|0
condition|)
block|{
if|if
condition|(
operator|!
operator|(
name|r
operator|->
name|headers_out
operator|.
name|accept_ranges
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
name|r
operator|->
name|headers_out
operator|.
name|accept_ranges
operator|->
name|key
operator|.
name|len
operator|=
sizeof|sizeof
argument_list|(
literal|"Accept-Ranges"
argument_list|)
operator|-
literal|1
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|accept_ranges
operator|->
name|key
operator|.
name|data
operator|=
operator|(
name|u_char
operator|*
operator|)
literal|"Accept-Ranges"
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|accept_ranges
operator|->
name|value
operator|.
name|len
operator|=
sizeof|sizeof
argument_list|(
literal|"bytes"
argument_list|)
operator|-
literal|1
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|accept_ranges
operator|->
name|value
operator|.
name|data
operator|=
operator|(
name|u_char
operator|*
operator|)
literal|"bytes"
expr_stmt|;
return|return
name|ngx_http_next_header_filter
argument_list|(
name|r
argument_list|)
return|;
block|}
name|ngx_init_array
argument_list|(
name|r
operator|->
name|headers_out
operator|.
name|ranges
argument_list|,
name|r
operator|->
name|pool
argument_list|,
literal|5
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_range_t
argument_list|)
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
name|rc
operator|=
literal|0
expr_stmt|;
name|range
operator|=
name|NULL
expr_stmt|;
name|p
operator|=
name|r
operator|->
name|headers_in
operator|.
name|range
operator|->
name|value
operator|.
name|data
operator|+
literal|6
expr_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
name|start
operator|=
literal|0
expr_stmt|;
name|end
operator|=
literal|0
expr_stmt|;
name|suffix
operator|=
literal|0
expr_stmt|;
while|while
condition|(
operator|*
name|p
operator|==
literal|' '
condition|)
block|{
name|p
operator|++
expr_stmt|;
block|}
if|if
condition|(
operator|*
name|p
operator|!=
literal|'-'
condition|)
block|{
if|if
condition|(
operator|*
name|p
argument_list|<
literal|'0'
operator|||
operator|*
name|p
argument_list|>
literal|'9'
condition|)
block|{
name|rc
operator|=
name|NGX_HTTP_RANGE_NOT_SATISFIABLE
expr_stmt|;
break|break;
block|}
while|while
condition|(
operator|*
name|p
operator|>=
literal|'0'
operator|&&
operator|*
name|p
operator|<=
literal|'9'
condition|)
block|{
name|start
operator|=
name|start
operator|*
literal|10
operator|+
operator|*
name|p
operator|++
operator|-
literal|'0'
expr_stmt|;
block|}
while|while
condition|(
operator|*
name|p
operator|==
literal|' '
condition|)
block|{
name|p
operator|++
expr_stmt|;
block|}
if|if
condition|(
operator|*
name|p
operator|++
operator|!=
literal|'-'
condition|)
block|{
name|rc
operator|=
name|NGX_HTTP_RANGE_NOT_SATISFIABLE
expr_stmt|;
break|break;
block|}
if|if
condition|(
name|start
operator|>=
name|r
operator|->
name|headers_out
operator|.
name|content_length_n
condition|)
block|{
name|rc
operator|=
name|NGX_HTTP_RANGE_NOT_SATISFIABLE
expr_stmt|;
break|break;
block|}
while|while
condition|(
operator|*
name|p
operator|==
literal|' '
condition|)
block|{
name|p
operator|++
expr_stmt|;
block|}
if|if
condition|(
operator|*
name|p
operator|==
literal|','
operator|||
operator|*
name|p
operator|==
literal|'\0'
condition|)
block|{
name|ngx_test_null
argument_list|(
name|range
argument_list|,
name|ngx_push_array
argument_list|(
operator|&
name|r
operator|->
name|headers_out
operator|.
name|ranges
argument_list|)
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
name|range
operator|->
name|start
operator|=
name|start
expr_stmt|;
name|range
operator|->
name|end
operator|=
name|r
operator|->
name|headers_out
operator|.
name|content_length_n
expr_stmt|;
if|if
condition|(
operator|*
name|p
operator|++
operator|!=
literal|','
condition|)
block|{
break|break;
block|}
continue|continue;
block|}
block|}
else|else
block|{
name|suffix
operator|=
literal|1
expr_stmt|;
name|p
operator|++
expr_stmt|;
block|}
if|if
condition|(
operator|*
name|p
argument_list|<
literal|'0'
operator|||
operator|*
name|p
argument_list|>
literal|'9'
condition|)
block|{
name|rc
operator|=
name|NGX_HTTP_RANGE_NOT_SATISFIABLE
expr_stmt|;
break|break;
block|}
while|while
condition|(
operator|*
name|p
operator|>=
literal|'0'
operator|&&
operator|*
name|p
operator|<=
literal|'9'
condition|)
block|{
name|end
operator|=
name|end
operator|*
literal|10
operator|+
operator|*
name|p
operator|++
operator|-
literal|'0'
expr_stmt|;
block|}
while|while
condition|(
operator|*
name|p
operator|==
literal|' '
condition|)
block|{
name|p
operator|++
expr_stmt|;
block|}
if|if
condition|(
operator|*
name|p
operator|!=
literal|','
operator|&&
operator|*
name|p
operator|!=
literal|'\0'
condition|)
block|{
name|rc
operator|=
name|NGX_HTTP_RANGE_NOT_SATISFIABLE
expr_stmt|;
break|break;
block|}
if|if
condition|(
name|suffix
condition|)
block|{
name|start
operator|=
name|r
operator|->
name|headers_out
operator|.
name|content_length_n
operator|-
name|end
expr_stmt|;
name|end
operator|=
name|r
operator|->
name|headers_out
operator|.
name|content_length_n
operator|-
literal|1
expr_stmt|;
block|}
if|if
condition|(
name|start
operator|>
name|end
condition|)
block|{
name|rc
operator|=
name|NGX_HTTP_RANGE_NOT_SATISFIABLE
expr_stmt|;
break|break;
block|}
name|ngx_test_null
argument_list|(
name|range
argument_list|,
name|ngx_push_array
argument_list|(
operator|&
name|r
operator|->
name|headers_out
operator|.
name|ranges
argument_list|)
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
name|range
operator|->
name|start
operator|=
name|start
expr_stmt|;
if|if
condition|(
name|end
operator|>=
name|r
operator|->
name|headers_out
operator|.
name|content_length_n
condition|)
block|{
comment|/*              * Download Accelerator sends the last byte position              * that equals to the file length              */
name|range
operator|->
name|end
operator|=
name|r
operator|->
name|headers_out
operator|.
name|content_length_n
expr_stmt|;
block|}
else|else
block|{
name|range
operator|->
name|end
operator|=
name|end
operator|+
literal|1
expr_stmt|;
block|}
if|if
condition|(
operator|*
name|p
operator|++
operator|!=
literal|','
condition|)
block|{
break|break;
block|}
block|}
if|if
condition|(
name|rc
condition|)
block|{
comment|/* rc == NGX_HTTP_RANGE_NOT_SATISFIABLE */
name|r
operator|->
name|headers_out
operator|.
name|status
operator|=
name|rc
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|ranges
operator|.
name|nelts
operator|=
literal|0
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|r
operator|->
name|headers_out
operator|.
name|content_range
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
name|r
operator|->
name|headers_out
operator|.
name|content_range
operator|->
name|key
operator|.
name|len
operator|=
sizeof|sizeof
argument_list|(
literal|"Content-Range"
argument_list|)
operator|-
literal|1
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|content_range
operator|->
name|key
operator|.
name|data
operator|=
operator|(
name|u_char
operator|*
operator|)
literal|"Content-Range"
expr_stmt|;
name|ngx_test_null
argument_list|(
name|r
operator|->
name|headers_out
operator|.
name|content_range
operator|->
name|value
operator|.
name|data
argument_list|,
name|ngx_palloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
literal|8
operator|+
literal|20
operator|+
literal|1
argument_list|)
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|content_range
operator|->
name|value
operator|.
name|len
operator|=
name|ngx_snprintf
argument_list|(
operator|(
name|char
operator|*
operator|)
name|r
operator|->
name|headers_out
operator|.
name|content_range
operator|->
name|value
operator|.
name|data
argument_list|,
literal|8
operator|+
literal|20
operator|+
literal|1
argument_list|,
literal|"bytes */"
name|OFF_T_FMT
argument_list|,
name|r
operator|->
name|headers_out
operator|.
name|content_length_n
argument_list|)
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
name|key
operator|.
name|len
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
return|return
name|rc
return|;
block|}
else|else
block|{
name|r
operator|->
name|headers_out
operator|.
name|status
operator|=
name|NGX_HTTP_PARTIAL_CONTENT
expr_stmt|;
if|if
condition|(
name|r
operator|->
name|headers_out
operator|.
name|ranges
operator|.
name|nelts
operator|==
literal|1
condition|)
block|{
if|if
condition|(
operator|!
operator|(
name|r
operator|->
name|headers_out
operator|.
name|content_range
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
name|r
operator|->
name|headers_out
operator|.
name|content_range
operator|->
name|key
operator|.
name|len
operator|=
sizeof|sizeof
argument_list|(
literal|"Content-Range"
argument_list|)
operator|-
literal|1
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|content_range
operator|->
name|key
operator|.
name|data
operator|=
operator|(
name|u_char
operator|*
operator|)
literal|"Content-Range"
expr_stmt|;
name|ngx_test_null
argument_list|(
name|r
operator|->
name|headers_out
operator|.
name|content_range
operator|->
name|value
operator|.
name|data
argument_list|,
name|ngx_palloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
literal|6
operator|+
literal|20
operator|+
literal|1
operator|+
literal|20
operator|+
literal|1
operator|+
literal|20
operator|+
literal|1
argument_list|)
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
comment|/* "Content-Range: bytes SSSS-EEEE/TTTT" header */
name|r
operator|->
name|headers_out
operator|.
name|content_range
operator|->
name|value
operator|.
name|len
operator|=
name|ngx_snprintf
argument_list|(
operator|(
name|char
operator|*
operator|)
name|r
operator|->
name|headers_out
operator|.
name|content_range
operator|->
name|value
operator|.
name|data
argument_list|,
literal|6
operator|+
literal|20
operator|+
literal|1
operator|+
literal|20
operator|+
literal|1
operator|+
literal|20
operator|+
literal|1
argument_list|,
literal|"bytes "
name|OFF_T_FMT
literal|"-"
name|OFF_T_FMT
literal|"/"
name|OFF_T_FMT
argument_list|,
name|range
operator|->
name|start
argument_list|,
name|range
operator|->
name|end
operator|-
literal|1
argument_list|,
name|r
operator|->
name|headers_out
operator|.
name|content_length_n
argument_list|)
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|content_length_n
operator|=
name|range
operator|->
name|end
operator|-
name|range
operator|->
name|start
expr_stmt|;
block|}
else|else
block|{
if|#
directive|if
literal|0
comment|/* TODO: what if no content_type ?? */
block_content|if (!(r->headers_out.content_type =                    ngx_http_add_header(&r->headers_out, ngx_http_headers_out)))             {                 return NGX_ERROR;             }
endif|#
directive|endif
name|ngx_http_create_ctx
argument_list|(
name|r
argument_list|,
name|ctx
argument_list|,
name|ngx_http_range_body_filter_module
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_range_filter_ctx_t
argument_list|)
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
name|len
operator|=
literal|4
operator|+
literal|10
operator|+
literal|2
operator|+
literal|14
operator|+
name|r
operator|->
name|headers_out
operator|.
name|content_type
operator|->
name|value
operator|.
name|len
operator|+
literal|2
operator|+
literal|21
operator|+
literal|1
expr_stmt|;
if|if
condition|(
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
literal|10
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
name|ngx_test_null
argument_list|(
name|ctx
operator|->
name|boundary_header
operator|.
name|data
argument_list|,
name|ngx_palloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
name|len
argument_list|)
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
name|boundary
operator|=
name|ngx_next_temp_number
argument_list|(
literal|0
argument_list|)
expr_stmt|;
comment|/*              * The boundary header of the range:              * CRLF              * "--0123456789" CRLF              * "Content-Type: image/jpeg" CRLF              * "Content-Range: bytes "              */
if|if
condition|(
name|r
operator|->
name|headers_out
operator|.
name|charset
operator|.
name|len
condition|)
block|{
name|ctx
operator|->
name|boundary_header
operator|.
name|len
operator|=
name|ngx_snprintf
argument_list|(
argument|(char *) ctx->boundary_header.data
argument_list|,
argument|len
argument_list|,
argument|CRLF
literal|"--%010"
argument|NGX_UINT_T_FMT CRLF
literal|"Content-Type: %s; charset=%s"
argument|CRLF
literal|"Content-Range: bytes "
argument_list|,
argument|boundary
argument_list|,
argument|r->headers_out.content_type->value.data
argument_list|,
argument|r->headers_out.charset.data
argument_list|)
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|charset
operator|.
name|len
operator|=
literal|0
expr_stmt|;
block|}
else|else
block|{
name|ctx
operator|->
name|boundary_header
operator|.
name|len
operator|=
name|ngx_snprintf
argument_list|(
argument|(char *) ctx->boundary_header.data
argument_list|,
argument|len
argument_list|,
argument|CRLF
literal|"--%010"
argument|NGX_UINT_T_FMT CRLF
literal|"Content-Type: %s"
argument|CRLF
literal|"Content-Range: bytes "
argument_list|,
argument|boundary
argument_list|,
argument|r->headers_out.content_type->value.data
argument_list|)
expr_stmt|;
block|}
name|ngx_test_null
argument_list|(
name|r
operator|->
name|headers_out
operator|.
name|content_type
operator|->
name|value
operator|.
name|data
argument_list|,
name|ngx_palloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
literal|31
operator|+
literal|10
operator|+
literal|1
argument_list|)
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
comment|/* "Content-Type: multipart/byteranges; boundary=0123456789" */
name|r
operator|->
name|headers_out
operator|.
name|content_type
operator|->
name|value
operator|.
name|len
operator|=
name|ngx_snprintf
argument_list|(
operator|(
name|char
operator|*
operator|)
name|r
operator|->
name|headers_out
operator|.
name|content_type
operator|->
name|value
operator|.
name|data
argument_list|,
literal|31
operator|+
literal|10
operator|+
literal|1
argument_list|,
literal|"multipart/byteranges; boundary=%010"
name|NGX_UINT_T_FMT
argument_list|,
name|boundary
argument_list|)
expr_stmt|;
comment|/* the size of the last boundary CRLF "--0123456789--" CRLF */
name|len
operator|=
literal|4
operator|+
literal|10
operator|+
literal|4
expr_stmt|;
name|range
operator|=
name|r
operator|->
name|headers_out
operator|.
name|ranges
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
name|r
operator|->
name|headers_out
operator|.
name|ranges
operator|.
name|nelts
condition|;
name|i
operator|++
control|)
block|{
name|ngx_test_null
argument_list|(
name|range
index|[
name|i
index|]
operator|.
name|content_range
operator|.
name|data
argument_list|,
name|ngx_palloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
literal|20
operator|+
literal|1
operator|+
literal|20
operator|+
literal|1
operator|+
literal|20
operator|+
literal|5
argument_list|)
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
comment|/* the size of the range: "SSSS-EEEE/TTTT" CRLF CRLF */
name|range
index|[
name|i
index|]
operator|.
name|content_range
operator|.
name|len
operator|=
name|ngx_snprintf
argument_list|(
argument|(char *) range[i].content_range.data
argument_list|,
literal|20
argument|+
literal|1
argument|+
literal|20
argument|+
literal|1
argument|+
literal|20
argument|+
literal|5
argument_list|,
argument|OFF_T_FMT
literal|"-"
argument|OFF_T_FMT
literal|"/"
argument|OFF_T_FMT CRLF CRLF
argument_list|,
argument|range[i].start
argument_list|,
argument|range[i].end -
literal|1
argument_list|,
argument|r->headers_out.content_length_n
argument_list|)
expr_stmt|;
name|len
operator|+=
name|ctx
operator|->
name|boundary_header
operator|.
name|len
operator|+
name|range
index|[
name|i
index|]
operator|.
name|content_range
operator|.
name|len
operator|+
operator|(
name|size_t
operator|)
operator|(
name|range
index|[
name|i
index|]
operator|.
name|end
operator|-
name|range
index|[
name|i
index|]
operator|.
name|start
operator|)
expr_stmt|;
block|}
name|r
operator|->
name|headers_out
operator|.
name|content_length_n
operator|=
name|len
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
DECL|function|ngx_http_range_body_filter (ngx_http_request_t * r,ngx_chain_t * in)
specifier|static
name|ngx_int_t
name|ngx_http_range_body_filter
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
name|ngx_uint_t
name|i
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
name|hcl
decl_stmt|,
modifier|*
name|rcl
decl_stmt|,
modifier|*
name|dcl
decl_stmt|,
modifier|*
modifier|*
name|ll
decl_stmt|;
name|ngx_http_range_t
modifier|*
name|range
decl_stmt|;
name|ngx_http_range_filter_ctx_t
modifier|*
name|ctx
decl_stmt|;
if|if
condition|(
name|r
operator|->
name|headers_out
operator|.
name|ranges
operator|.
name|nelts
operator|==
literal|0
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
comment|/*      * the optimized version for the static files only      * that are passed in the single file buf      */
if|if
condition|(
name|in
operator|&&
name|in
operator|->
name|buf
operator|->
name|in_file
operator|&&
name|in
operator|->
name|buf
operator|->
name|last_buf
condition|)
block|{
name|range
operator|=
name|r
operator|->
name|headers_out
operator|.
name|ranges
operator|.
name|elts
expr_stmt|;
if|if
condition|(
name|r
operator|->
name|headers_out
operator|.
name|ranges
operator|.
name|nelts
operator|==
literal|1
condition|)
block|{
name|in
operator|->
name|buf
operator|->
name|file_pos
operator|=
name|range
operator|->
name|start
expr_stmt|;
name|in
operator|->
name|buf
operator|->
name|file_last
operator|=
name|range
operator|->
name|end
expr_stmt|;
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
name|ngx_http_range_body_filter_module
argument_list|)
expr_stmt|;
name|ll
operator|=
operator|&
name|out
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
name|ranges
operator|.
name|nelts
condition|;
name|i
operator|++
control|)
block|{
comment|/*              * The boundary header of the range:              * CRLF              * "--0123456789" CRLF              * "Content-Type: image/jpeg" CRLF              * "Content-Range: bytes "              */
name|ngx_test_null
argument_list|(
name|b
argument_list|,
name|ngx_calloc_buf
argument_list|(
name|r
operator|->
name|pool
argument_list|)
argument_list|,
name|NGX_ERROR
argument_list|)
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
name|ctx
operator|->
name|boundary_header
operator|.
name|data
expr_stmt|;
name|b
operator|->
name|last
operator|=
name|ctx
operator|->
name|boundary_header
operator|.
name|data
operator|+
name|ctx
operator|->
name|boundary_header
operator|.
name|len
expr_stmt|;
name|ngx_test_null
argument_list|(
name|hcl
argument_list|,
name|ngx_alloc_chain_link
argument_list|(
name|r
operator|->
name|pool
argument_list|)
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
name|hcl
operator|->
name|buf
operator|=
name|b
expr_stmt|;
comment|/* "SSSS-EEEE/TTTT" CRLF CRLF */
name|ngx_test_null
argument_list|(
name|b
argument_list|,
name|ngx_calloc_buf
argument_list|(
name|r
operator|->
name|pool
argument_list|)
argument_list|,
name|NGX_ERROR
argument_list|)
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
name|range
index|[
name|i
index|]
operator|.
name|content_range
operator|.
name|data
expr_stmt|;
name|b
operator|->
name|last
operator|=
name|range
index|[
name|i
index|]
operator|.
name|content_range
operator|.
name|data
operator|+
name|range
index|[
name|i
index|]
operator|.
name|content_range
operator|.
name|len
expr_stmt|;
name|ngx_test_null
argument_list|(
name|rcl
argument_list|,
name|ngx_alloc_chain_link
argument_list|(
name|r
operator|->
name|pool
argument_list|)
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
name|rcl
operator|->
name|buf
operator|=
name|b
expr_stmt|;
comment|/* the range data */
name|ngx_test_null
argument_list|(
name|b
argument_list|,
name|ngx_calloc_buf
argument_list|(
name|r
operator|->
name|pool
argument_list|)
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
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
name|range
index|[
name|i
index|]
operator|.
name|start
expr_stmt|;
name|b
operator|->
name|file_last
operator|=
name|range
index|[
name|i
index|]
operator|.
name|end
expr_stmt|;
name|b
operator|->
name|file
operator|=
name|in
operator|->
name|buf
operator|->
name|file
expr_stmt|;
name|ngx_alloc_link_and_set_buf
argument_list|(
name|dcl
argument_list|,
name|b
argument_list|,
name|r
operator|->
name|pool
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
operator|*
name|ll
operator|=
name|hcl
expr_stmt|;
name|hcl
operator|->
name|next
operator|=
name|rcl
expr_stmt|;
name|rcl
operator|->
name|next
operator|=
name|dcl
expr_stmt|;
name|ll
operator|=
operator|&
name|dcl
operator|->
name|next
expr_stmt|;
block|}
comment|/* the last boundary CRLF "--0123456789--" CRLF  */
name|ngx_test_null
argument_list|(
name|b
argument_list|,
name|ngx_calloc_buf
argument_list|(
name|r
operator|->
name|pool
argument_list|)
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
name|b
operator|->
name|temporary
operator|=
literal|1
expr_stmt|;
name|b
operator|->
name|last_buf
operator|=
literal|1
expr_stmt|;
name|ngx_test_null
argument_list|(
name|b
operator|->
name|pos
argument_list|,
name|ngx_palloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
literal|4
operator|+
literal|10
operator|+
literal|4
argument_list|)
argument_list|,
name|NGX_ERROR
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
name|pos
argument_list|,
name|ctx
operator|->
name|boundary_header
operator|.
name|data
argument_list|,
literal|4
operator|+
literal|10
argument_list|)
expr_stmt|;
operator|*
name|b
operator|->
name|last
operator|++
operator|=
literal|'-'
expr_stmt|;
operator|*
name|b
operator|->
name|last
operator|++
operator|=
literal|'-'
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
name|ngx_alloc_link_and_set_buf
argument_list|(
name|hcl
argument_list|,
name|b
argument_list|,
name|r
operator|->
name|pool
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
operator|*
name|ll
operator|=
name|hcl
expr_stmt|;
return|return
name|ngx_http_next_body_filter
argument_list|(
name|r
argument_list|,
name|out
argument_list|)
return|;
block|}
comment|/* TODO: alert */
return|return
name|ngx_http_next_body_filter
argument_list|(
name|r
argument_list|,
name|in
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_range_header_filter_init (ngx_cycle_t * cycle)
specifier|static
name|ngx_int_t
name|ngx_http_range_header_filter_init
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
name|ngx_http_range_header_filter
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_range_body_filter_init (ngx_cycle_t * cycle)
specifier|static
name|ngx_int_t
name|ngx_http_range_body_filter_init
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
block|{
name|ngx_http_next_body_filter
operator|=
name|ngx_http_top_body_filter
expr_stmt|;
name|ngx_http_top_body_filter
operator|=
name|ngx_http_range_body_filter
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

end_unit

