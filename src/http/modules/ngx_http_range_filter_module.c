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

begin_comment
comment|/*  * the single part format:  *  * "HTTP/1.0 206 Partial Content" CRLF  * ... header ...  * "Content-Type: image/jpeg" CRLF  * "Content-Length: SIZE" CRLF  * "Content-Range: bytes START-END/SIZE" CRLF  * CRLF  * ... data ...  *  *  * the mutlipart format:  *  * "HTTP/1.0 206 Partial Content" CRLF  * ... header ...  * "Content-Type: multipart/byteranges; boundary=0123456789" CRLF  * CRLF  * CRLF  * "--0123456789" CRLF  * "Content-Type: image/jpeg" CRLF  * "Content-Range: bytes START0-END0/SIZE" CRLF  * CRLF  * ... data ...  * CRLF  * "--0123456789" CRLF  * "Content-Type: image/jpeg" CRLF  * "Content-Range: bytes START1-END1/SIZE" CRLF  * CRLF  * ... data ...  * CRLF  * "--0123456789--" CRLF  */
end_comment

begin_typedef
DECL|struct|__anon2a5525640108
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
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_range_body_filter_init
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
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
comment|/* preconfiguration */
name|ngx_http_range_header_filter_init
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
DECL|variable|ngx_http_range_header_filter_module
name|ngx_module_t
name|ngx_http_range_header_filter_module
init|=
block|{
name|NGX_MODULE_V1
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
DECL|variable|ngx_http_range_body_filter_module_ctx
specifier|static
name|ngx_http_module_t
name|ngx_http_range_body_filter_module_ctx
init|=
block|{
name|NULL
block|,
comment|/* preconfiguration */
name|ngx_http_range_body_filter_init
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
DECL|variable|ngx_http_range_body_filter_module
name|ngx_module_t
name|ngx_http_range_body_filter_module
init|=
block|{
name|NGX_MODULE_V1
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
DECL|function|ngx_http_range_header_filter (ngx_http_request_t * r)
name|ngx_http_range_header_filter
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
name|off_t
name|start
decl_stmt|,
name|end
decl_stmt|;
name|ngx_int_t
name|rc
decl_stmt|;
name|ngx_uint_t
name|suffix
decl_stmt|,
name|i
decl_stmt|;
name|ngx_atomic_uint_t
name|boundary
decl_stmt|;
name|ngx_table_elt_t
modifier|*
name|content_range
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
operator|!=
name|r
operator|->
expr|main
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
name|allow_ranges
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
name|r
operator|->
name|headers_out
operator|.
name|accept_ranges
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
name|r
operator|->
name|headers_out
operator|.
name|accept_ranges
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
name|headers_out
operator|.
name|accept_ranges
operator|->
name|hash
operator|=
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
if|if
condition|(
name|ngx_array_init
argument_list|(
operator|&
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
literal|2
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_range_t
argument_list|)
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
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
name|range
operator|=
name|ngx_array_push
argument_list|(
operator|&
name|r
operator|->
name|headers_out
operator|.
name|ranges
argument_list|)
expr_stmt|;
if|if
condition|(
name|range
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
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
name|range
operator|=
name|ngx_array_push
argument_list|(
operator|&
name|r
operator|->
name|headers_out
operator|.
name|ranges
argument_list|)
expr_stmt|;
if|if
condition|(
name|range
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
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
name|content_range
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
name|content_range
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
name|headers_out
operator|.
name|content_range
operator|=
name|content_range
expr_stmt|;
name|content_range
operator|->
name|hash
operator|=
literal|1
expr_stmt|;
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
name|content_range
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
sizeof|sizeof
argument_list|(
literal|"bytes */"
argument_list|)
operator|-
literal|1
operator|+
name|NGX_OFF_T_LEN
argument_list|)
expr_stmt|;
if|if
condition|(
name|content_range
operator|->
name|value
operator|.
name|data
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|content_range
operator|->
name|value
operator|.
name|len
operator|=
name|ngx_sprintf
argument_list|(
name|content_range
operator|->
name|value
operator|.
name|data
argument_list|,
literal|"bytes */%O"
argument_list|,
name|r
operator|->
name|headers_out
operator|.
name|content_length_n
argument_list|)
operator|-
name|content_range
operator|->
name|value
operator|.
name|data
expr_stmt|;
name|ngx_http_clear_content_length
argument_list|(
name|r
argument_list|)
expr_stmt|;
return|return
name|rc
return|;
block|}
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
name|content_range
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
name|content_range
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
name|headers_out
operator|.
name|content_range
operator|=
name|content_range
expr_stmt|;
name|content_range
operator|->
name|hash
operator|=
literal|1
expr_stmt|;
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
name|content_range
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
sizeof|sizeof
argument_list|(
literal|"bytes -/"
argument_list|)
operator|-
literal|1
operator|+
literal|3
operator|*
name|NGX_OFF_T_LEN
argument_list|)
expr_stmt|;
if|if
condition|(
name|content_range
operator|->
name|value
operator|.
name|data
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
comment|/* "Content-Range: bytes SSSS-EEEE/TTTT" header */
name|content_range
operator|->
name|value
operator|.
name|len
operator|=
name|ngx_sprintf
argument_list|(
name|content_range
operator|->
name|value
operator|.
name|data
argument_list|,
literal|"bytes %O-%O/%O"
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
operator|-
name|content_range
operator|->
name|value
operator|.
name|data
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
return|return
name|ngx_http_next_header_filter
argument_list|(
name|r
argument_list|)
return|;
block|}
comment|/* TODO: what if no content_type ?? */
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
name|ngx_http_range_filter_ctx_t
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
name|ngx_http_range_body_filter_module
argument_list|)
expr_stmt|;
name|len
operator|=
sizeof|sizeof
argument_list|(
name|CRLF
literal|"--"
argument_list|)
operator|-
literal|1
operator|+
name|NGX_ATOMIC_T_LEN
operator|+
sizeof|sizeof
argument_list|(
name|CRLF
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
sizeof|sizeof
argument_list|(
name|CRLF
literal|"Content-Range: bytes "
argument_list|)
operator|-
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
name|ctx
operator|->
name|boundary_header
operator|.
name|data
operator|=
name|ngx_palloc
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
name|ctx
operator|->
name|boundary_header
operator|.
name|data
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|boundary
operator|=
name|ngx_next_temp_number
argument_list|(
literal|0
argument_list|)
expr_stmt|;
comment|/*      * The boundary header of the range:      * CRLF      * "--0123456789" CRLF      * "Content-Type: image/jpeg" CRLF      * "Content-Range: bytes "      */
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
name|ngx_sprintf
argument_list|(
name|ctx
operator|->
name|boundary_header
operator|.
name|data
argument_list|,
name|CRLF
literal|"--%0muA"
name|CRLF
literal|"Content-Type: %V; charset=%V"
name|CRLF
literal|"Content-Range: bytes "
argument_list|,
name|boundary
argument_list|,
operator|&
name|r
operator|->
name|headers_out
operator|.
name|content_type
argument_list|,
operator|&
name|r
operator|->
name|headers_out
operator|.
name|charset
argument_list|)
operator|-
name|ctx
operator|->
name|boundary_header
operator|.
name|data
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
name|ngx_sprintf
argument_list|(
name|ctx
operator|->
name|boundary_header
operator|.
name|data
argument_list|,
name|CRLF
literal|"--%0muA"
name|CRLF
literal|"Content-Type: %V"
name|CRLF
literal|"Content-Range: bytes "
argument_list|,
name|boundary
argument_list|,
operator|&
name|r
operator|->
name|headers_out
operator|.
name|content_type
argument_list|)
operator|-
name|ctx
operator|->
name|boundary_header
operator|.
name|data
expr_stmt|;
block|}
name|r
operator|->
name|headers_out
operator|.
name|content_type
operator|.
name|data
operator|=
name|ngx_palloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
literal|"Content-Type: multipart/byteranges; boundary="
argument_list|)
operator|-
literal|1
operator|+
name|NGX_ATOMIC_T_LEN
argument_list|)
expr_stmt|;
if|if
condition|(
name|r
operator|->
name|headers_out
operator|.
name|content_type
operator|.
name|data
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
comment|/* "Content-Type: multipart/byteranges; boundary=0123456789" */
name|r
operator|->
name|headers_out
operator|.
name|content_type
operator|.
name|len
operator|=
name|ngx_sprintf
argument_list|(
name|r
operator|->
name|headers_out
operator|.
name|content_type
operator|.
name|data
argument_list|,
literal|"multipart/byteranges; boundary=%0muA"
argument_list|,
name|boundary
argument_list|)
operator|-
name|r
operator|->
name|headers_out
operator|.
name|content_type
operator|.
name|data
expr_stmt|;
comment|/* the size of the last boundary CRLF "--0123456789--" CRLF */
name|len
operator|=
sizeof|sizeof
argument_list|(
name|CRLF
literal|"--"
argument_list|)
operator|-
literal|1
operator|+
name|NGX_ATOMIC_T_LEN
operator|+
sizeof|sizeof
argument_list|(
literal|"--"
name|CRLF
argument_list|)
operator|-
literal|1
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
comment|/* the size of the range: "SSSS-EEEE/TTTT" CRLF CRLF */
name|range
index|[
name|i
index|]
operator|.
name|content_range
operator|.
name|data
operator|=
name|ngx_palloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
literal|3
operator|*
name|NGX_OFF_T_LEN
operator|+
literal|2
operator|+
literal|4
argument_list|)
expr_stmt|;
if|if
condition|(
name|range
index|[
name|i
index|]
operator|.
name|content_range
operator|.
name|data
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|range
index|[
name|i
index|]
operator|.
name|content_range
operator|.
name|len
operator|=
name|ngx_sprintf
argument_list|(
argument|range[i].content_range.data
argument_list|,
literal|"%O-%O/%O"
argument|CRLF CRLF
argument_list|,
argument|range[i].start
argument_list|,
argument|range[i].end -
literal|1
argument_list|,
argument|r->headers_out.content_length_n
argument_list|)
operator|-
name|range
index|[
name|i
index|]
operator|.
name|content_range
operator|.
name|data
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
return|return
name|ngx_http_next_header_filter
argument_list|(
name|r
argument_list|)
return|;
block|}
end_function

begin_comment
comment|/*  * TODO:  *     buffer or buffers is in memory  *     range or ranges are overlapped in buffers  */
end_comment

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_range_body_filter (ngx_http_request_t * r,ngx_chain_t * in)
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
decl_stmt|,
modifier|*
name|buf
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
name|in
operator|==
name|NULL
operator|||
name|r
operator|->
name|headers_out
operator|.
name|ranges
operator|.
name|nelts
operator|==
literal|0
operator|||
name|ngx_buf_special
argument_list|(
name|in
operator|->
name|buf
argument_list|)
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
name|buf
operator|=
name|in
operator|->
name|buf
expr_stmt|;
if|if
condition|(
operator|!
name|buf
operator|->
name|in_file
condition|)
block|{
goto|goto
name|in_memory
goto|;
block|}
if|if
condition|(
operator|!
name|buf
operator|->
name|last_buf
condition|)
block|{
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
if|if
condition|(
name|buf
operator|->
name|file_pos
operator|>
name|range
index|[
name|i
index|]
operator|.
name|start
operator|||
name|buf
operator|->
name|file_last
operator|<
name|range
index|[
name|i
index|]
operator|.
name|end
condition|)
block|{
goto|goto
name|overlapped
goto|;
block|}
block|}
block|}
comment|/*      * the optimized version for the static files only      * that are passed in the single file buffer      */
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
name|buf
operator|->
name|file_pos
operator|=
name|range
operator|->
name|start
expr_stmt|;
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
comment|/*          * The boundary header of the range:          * CRLF          * "--0123456789" CRLF          * "Content-Type: image/jpeg" CRLF          * "Content-Range: bytes "          */
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
name|hcl
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
name|hcl
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|hcl
operator|->
name|buf
operator|=
name|b
expr_stmt|;
comment|/* "SSSS-EEEE/TTTT" CRLF CRLF */
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
name|rcl
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
name|rcl
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|rcl
operator|->
name|buf
operator|=
name|b
expr_stmt|;
comment|/* the range data */
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
name|buf
operator|->
name|file
expr_stmt|;
name|dcl
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
name|dcl
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|dcl
operator|->
name|buf
operator|=
name|b
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
name|b
operator|->
name|pos
operator|=
name|ngx_palloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|CRLF
literal|"--"
argument_list|)
operator|-
literal|1
operator|+
name|NGX_ATOMIC_T_LEN
operator|+
sizeof|sizeof
argument_list|(
literal|"--"
name|CRLF
argument_list|)
operator|-
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|b
operator|->
name|pos
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
name|hcl
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
name|hcl
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|hcl
operator|->
name|buf
operator|=
name|b
expr_stmt|;
name|hcl
operator|->
name|next
operator|=
name|NULL
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
name|in_memory
label|:
name|overlapped
label|:
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|r
operator|->
name|connection
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"range in memory buffer or in overlapped buffers"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_range_header_filter_init (ngx_conf_t * cf)
name|ngx_http_range_header_filter_init
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
name|ngx_http_range_header_filter
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_range_body_filter_init (ngx_conf_t * cf)
name|ngx_http_range_body_filter_init
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
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

