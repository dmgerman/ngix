begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (C) 2002-2004 Igor Sysoev  */
end_comment

begin_ifndef
ifndef|#
directive|ifndef
name|_NGX_HTTP_REQUEST_H_INCLUDED_
end_ifndef

begin_define
DECL|macro|_NGX_HTTP_REQUEST_H_INCLUDED_
define|#
directive|define
name|_NGX_HTTP_REQUEST_H_INCLUDED_
end_define

begin_define
DECL|macro|NGX_HTTP_DISCARD_BUFFER_SIZE
define|#
directive|define
name|NGX_HTTP_DISCARD_BUFFER_SIZE
value|4096
end_define

begin_define
DECL|macro|NGX_HTTP_LINGERING_BUFFER_SIZE
define|#
directive|define
name|NGX_HTTP_LINGERING_BUFFER_SIZE
value|4096
end_define

begin_define
DECL|macro|NGX_HTTP_VERSION_9
define|#
directive|define
name|NGX_HTTP_VERSION_9
value|9
end_define

begin_define
DECL|macro|NGX_HTTP_VERSION_10
define|#
directive|define
name|NGX_HTTP_VERSION_10
value|1000
end_define

begin_define
DECL|macro|NGX_HTTP_VERSION_11
define|#
directive|define
name|NGX_HTTP_VERSION_11
value|1001
end_define

begin_define
DECL|macro|NGX_HTTP_GET
define|#
directive|define
name|NGX_HTTP_GET
value|1
end_define

begin_define
DECL|macro|NGX_HTTP_HEAD
define|#
directive|define
name|NGX_HTTP_HEAD
value|2
end_define

begin_define
DECL|macro|NGX_HTTP_POST
define|#
directive|define
name|NGX_HTTP_POST
value|3
end_define

begin_define
DECL|macro|NGX_HTTP_CONNECTION_CLOSE
define|#
directive|define
name|NGX_HTTP_CONNECTION_CLOSE
value|1
end_define

begin_define
DECL|macro|NGX_HTTP_CONNECTION_KEEP_ALIVE
define|#
directive|define
name|NGX_HTTP_CONNECTION_KEEP_ALIVE
value|2
end_define

begin_define
DECL|macro|NGX_NONE
define|#
directive|define
name|NGX_NONE
value|1
end_define

begin_define
DECL|macro|NGX_HTTP_PARSE_HEADER_DONE
define|#
directive|define
name|NGX_HTTP_PARSE_HEADER_DONE
value|1
end_define

begin_define
DECL|macro|NGX_HTTP_CLIENT_ERROR
define|#
directive|define
name|NGX_HTTP_CLIENT_ERROR
value|10
end_define

begin_define
DECL|macro|NGX_HTTP_PARSE_INVALID_METHOD
define|#
directive|define
name|NGX_HTTP_PARSE_INVALID_METHOD
value|10
end_define

begin_define
DECL|macro|NGX_HTTP_PARSE_INVALID_REQUEST
define|#
directive|define
name|NGX_HTTP_PARSE_INVALID_REQUEST
value|11
end_define

begin_define
DECL|macro|NGX_HTTP_PARSE_TOO_LONG_URI
define|#
directive|define
name|NGX_HTTP_PARSE_TOO_LONG_URI
value|12
end_define

begin_define
DECL|macro|NGX_HTTP_PARSE_INVALID_09_METHOD
define|#
directive|define
name|NGX_HTTP_PARSE_INVALID_09_METHOD
value|13
end_define

begin_define
DECL|macro|NGX_HTTP_PARSE_HEADER_ERROR
define|#
directive|define
name|NGX_HTTP_PARSE_HEADER_ERROR
value|14
end_define

begin_define
DECL|macro|NGX_HTTP_PARSE_INVALID_HEADER
define|#
directive|define
name|NGX_HTTP_PARSE_INVALID_HEADER
value|14
end_define

begin_define
DECL|macro|NGX_HTTP_PARSE_TOO_LONG_HEADER
define|#
directive|define
name|NGX_HTTP_PARSE_TOO_LONG_HEADER
value|15
end_define

begin_define
DECL|macro|NGX_HTTP_PARSE_TOO_MANY_HEADERS
define|#
directive|define
name|NGX_HTTP_PARSE_TOO_MANY_HEADERS
value|16
end_define

begin_define
DECL|macro|NGX_HTTP_PARSE_NO_HOST_HEADER
define|#
directive|define
name|NGX_HTTP_PARSE_NO_HOST_HEADER
value|17
end_define

begin_define
DECL|macro|NGX_HTTP_PARSE_INVALID_CL_HEADER
define|#
directive|define
name|NGX_HTTP_PARSE_INVALID_CL_HEADER
value|18
end_define

begin_define
DECL|macro|NGX_HTTP_PARSE_POST_WO_CL_HEADER
define|#
directive|define
name|NGX_HTTP_PARSE_POST_WO_CL_HEADER
value|19
end_define

begin_define
DECL|macro|NGX_HTTP_PARSE_HTTP_TO_HTTPS
define|#
directive|define
name|NGX_HTTP_PARSE_HTTP_TO_HTTPS
value|20
end_define

begin_define
DECL|macro|NGX_HTTP_PARSE_INVALID_HOST
define|#
directive|define
name|NGX_HTTP_PARSE_INVALID_HOST
value|21
end_define

begin_define
DECL|macro|NGX_HTTP_OK
define|#
directive|define
name|NGX_HTTP_OK
value|200
end_define

begin_define
DECL|macro|NGX_HTTP_PARTIAL_CONTENT
define|#
directive|define
name|NGX_HTTP_PARTIAL_CONTENT
value|206
end_define

begin_define
DECL|macro|NGX_HTTP_SPECIAL_RESPONSE
define|#
directive|define
name|NGX_HTTP_SPECIAL_RESPONSE
value|300
end_define

begin_define
DECL|macro|NGX_HTTP_MOVED_PERMANENTLY
define|#
directive|define
name|NGX_HTTP_MOVED_PERMANENTLY
value|301
end_define

begin_define
DECL|macro|NGX_HTTP_MOVED_TEMPORARILY
define|#
directive|define
name|NGX_HTTP_MOVED_TEMPORARILY
value|302
end_define

begin_define
DECL|macro|NGX_HTTP_NOT_MODIFIED
define|#
directive|define
name|NGX_HTTP_NOT_MODIFIED
value|304
end_define

begin_define
DECL|macro|NGX_HTTP_BAD_REQUEST
define|#
directive|define
name|NGX_HTTP_BAD_REQUEST
value|400
end_define

begin_define
DECL|macro|NGX_HTTP_FORBIDDEN
define|#
directive|define
name|NGX_HTTP_FORBIDDEN
value|403
end_define

begin_define
DECL|macro|NGX_HTTP_NOT_FOUND
define|#
directive|define
name|NGX_HTTP_NOT_FOUND
value|404
end_define

begin_define
DECL|macro|NGX_HTTP_NOT_ALLOWED
define|#
directive|define
name|NGX_HTTP_NOT_ALLOWED
value|405
end_define

begin_define
DECL|macro|NGX_HTTP_REQUEST_TIME_OUT
define|#
directive|define
name|NGX_HTTP_REQUEST_TIME_OUT
value|408
end_define

begin_define
DECL|macro|NGX_HTTP_REQUEST_ENTITY_TOO_LARGE
define|#
directive|define
name|NGX_HTTP_REQUEST_ENTITY_TOO_LARGE
value|413
end_define

begin_define
DECL|macro|NGX_HTTP_REQUEST_URI_TOO_LARGE
define|#
directive|define
name|NGX_HTTP_REQUEST_URI_TOO_LARGE
value|414
end_define

begin_define
DECL|macro|NGX_HTTP_RANGE_NOT_SATISFIABLE
define|#
directive|define
name|NGX_HTTP_RANGE_NOT_SATISFIABLE
value|416
end_define

begin_comment
comment|/* Our own HTTP codes */
end_comment

begin_define
DECL|macro|NGX_HTTP_NGX_CODES
define|#
directive|define
name|NGX_HTTP_NGX_CODES
value|NGX_HTTP_TO_HTTPS
end_define

begin_comment
comment|/*  * We use the special code for the plain HTTP requests that are sent to  * HTTPS port to distinguish it from 4XX in an error page redirection   */
end_comment

begin_define
DECL|macro|NGX_HTTP_TO_HTTPS
define|#
directive|define
name|NGX_HTTP_TO_HTTPS
value|497
end_define

begin_comment
comment|/*  * We use the special code for the requests with invalid host name  * to distinguish it from 4XX in an error page redirection   */
end_comment

begin_define
DECL|macro|NGX_HTTP_INVALID_HOST
define|#
directive|define
name|NGX_HTTP_INVALID_HOST
value|498
end_define

begin_comment
comment|/*  * HTTP does not define the code for the case when a client closed  * the connection while we are processing its request so we introduce  * own code to log such situation when a client has closed the connection  * before we even try to send the HTTP header to it  */
end_comment

begin_define
DECL|macro|NGX_HTTP_CLIENT_CLOSED_REQUEST
define|#
directive|define
name|NGX_HTTP_CLIENT_CLOSED_REQUEST
value|499
end_define

begin_define
DECL|macro|NGX_HTTP_INTERNAL_SERVER_ERROR
define|#
directive|define
name|NGX_HTTP_INTERNAL_SERVER_ERROR
value|500
end_define

begin_define
DECL|macro|NGX_HTTP_NOT_IMPLEMENTED
define|#
directive|define
name|NGX_HTTP_NOT_IMPLEMENTED
value|501
end_define

begin_define
DECL|macro|NGX_HTTP_BAD_GATEWAY
define|#
directive|define
name|NGX_HTTP_BAD_GATEWAY
value|502
end_define

begin_define
DECL|macro|NGX_HTTP_SERVICE_UNAVAILABLE
define|#
directive|define
name|NGX_HTTP_SERVICE_UNAVAILABLE
value|503
end_define

begin_define
DECL|macro|NGX_HTTP_GATEWAY_TIME_OUT
define|#
directive|define
name|NGX_HTTP_GATEWAY_TIME_OUT
value|504
end_define

begin_typedef
DECL|enum|__anon293e63170103
typedef|typedef
enum|enum
block|{
DECL|enumerator|NGX_HTTP_RESTRICT_HOST_OFF
name|NGX_HTTP_RESTRICT_HOST_OFF
init|=
literal|0
block|,
DECL|enumerator|NGX_HTTP_RESTRICT_HOST_ON
name|NGX_HTTP_RESTRICT_HOST_ON
block|,
DECL|enumerator|NGX_HTTP_RESTRICT_HOST_CLOSE
name|NGX_HTTP_RESTRICT_HOST_CLOSE
DECL|typedef|ngx_http_restrict_host_e
block|}
name|ngx_http_restrict_host_e
typedef|;
end_typedef

begin_typedef
DECL|enum|__anon293e63170203
typedef|typedef
enum|enum
block|{
DECL|enumerator|NGX_HTTP_INITING_REQUEST_STATE
name|NGX_HTTP_INITING_REQUEST_STATE
init|=
literal|0
block|,
DECL|enumerator|NGX_HTTP_READING_REQUEST_STATE
name|NGX_HTTP_READING_REQUEST_STATE
block|,
DECL|enumerator|NGX_HTTP_PROCESS_REQUEST_STATE
name|NGX_HTTP_PROCESS_REQUEST_STATE
block|,
DECL|enumerator|NGX_HTTP_CONNECT_UPSTREAM_STATE
name|NGX_HTTP_CONNECT_UPSTREAM_STATE
block|,
DECL|enumerator|NGX_HTTP_WRITING_UPSTREAM_STATE
name|NGX_HTTP_WRITING_UPSTREAM_STATE
block|,
DECL|enumerator|NGX_HTTP_READING_UPSTREAM_STATE
name|NGX_HTTP_READING_UPSTREAM_STATE
block|,
DECL|enumerator|NGX_HTTP_WRITING_REQUEST_STATE
name|NGX_HTTP_WRITING_REQUEST_STATE
block|,
DECL|enumerator|NGX_HTTP_LINGERING_CLOSE_STATE
name|NGX_HTTP_LINGERING_CLOSE_STATE
block|,
DECL|enumerator|NGX_HTTP_KEEPALIVE_STATE
name|NGX_HTTP_KEEPALIVE_STATE
DECL|typedef|ngx_http_state_e
block|}
name|ngx_http_state_e
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon293e63170308
typedef|typedef
struct|struct
block|{
DECL|member|name
name|ngx_str_t
name|name
decl_stmt|;
DECL|member|offset
name|ngx_uint_t
name|offset
decl_stmt|;
DECL|typedef|ngx_http_header_t
block|}
name|ngx_http_header_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon293e63170408
typedef|typedef
struct|struct
block|{
DECL|member|headers
name|ngx_list_t
name|headers
decl_stmt|;
DECL|member|host
name|ngx_table_elt_t
modifier|*
name|host
decl_stmt|;
DECL|member|connection
name|ngx_table_elt_t
modifier|*
name|connection
decl_stmt|;
DECL|member|if_modified_since
name|ngx_table_elt_t
modifier|*
name|if_modified_since
decl_stmt|;
DECL|member|user_agent
name|ngx_table_elt_t
modifier|*
name|user_agent
decl_stmt|;
DECL|member|referer
name|ngx_table_elt_t
modifier|*
name|referer
decl_stmt|;
DECL|member|content_length
name|ngx_table_elt_t
modifier|*
name|content_length
decl_stmt|;
DECL|member|range
name|ngx_table_elt_t
modifier|*
name|range
decl_stmt|;
if|#
directive|if
operator|(
name|NGX_HTTP_GZIP
operator|)
DECL|member|accept_encoding
name|ngx_table_elt_t
modifier|*
name|accept_encoding
decl_stmt|;
DECL|member|via
name|ngx_table_elt_t
modifier|*
name|via
decl_stmt|;
endif|#
directive|endif
DECL|member|authorization
name|ngx_table_elt_t
modifier|*
name|authorization
decl_stmt|;
DECL|member|keep_alive
name|ngx_table_elt_t
modifier|*
name|keep_alive
decl_stmt|;
if|#
directive|if
operator|(
name|NGX_HTTP_PROXY
operator|)
DECL|member|x_forwarded_for
name|ngx_table_elt_t
modifier|*
name|x_forwarded_for
decl_stmt|;
endif|#
directive|endif
DECL|member|cookies
name|ngx_array_t
name|cookies
decl_stmt|;
DECL|member|host_name_len
name|size_t
name|host_name_len
decl_stmt|;
DECL|member|content_length_n
name|ssize_t
name|content_length_n
decl_stmt|;
DECL|member|connection_type
name|size_t
name|connection_type
decl_stmt|;
DECL|member|keep_alive_n
name|ssize_t
name|keep_alive_n
decl_stmt|;
DECL|member|msie
name|unsigned
name|msie
range|:
literal|1
decl_stmt|;
DECL|member|msie4
name|unsigned
name|msie4
range|:
literal|1
decl_stmt|;
DECL|member|opera
name|unsigned
name|opera
range|:
literal|1
decl_stmt|;
DECL|member|gecko
name|unsigned
name|gecko
range|:
literal|1
decl_stmt|;
DECL|member|konqueror
name|unsigned
name|konqueror
range|:
literal|1
decl_stmt|;
DECL|typedef|ngx_http_headers_in_t
block|}
name|ngx_http_headers_in_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon293e63170508
typedef|typedef
struct|struct
block|{
DECL|member|start
name|off_t
name|start
decl_stmt|;
DECL|member|end
name|off_t
name|end
decl_stmt|;
DECL|member|content_range
name|ngx_str_t
name|content_range
decl_stmt|;
DECL|typedef|ngx_http_range_t
block|}
name|ngx_http_range_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon293e63170608
typedef|typedef
struct|struct
block|{
DECL|member|headers
name|ngx_list_t
name|headers
decl_stmt|;
DECL|member|status
name|ngx_uint_t
name|status
decl_stmt|;
DECL|member|status_line
name|ngx_str_t
name|status_line
decl_stmt|;
DECL|member|server
name|ngx_table_elt_t
modifier|*
name|server
decl_stmt|;
DECL|member|date
name|ngx_table_elt_t
modifier|*
name|date
decl_stmt|;
DECL|member|content_type
name|ngx_table_elt_t
modifier|*
name|content_type
decl_stmt|;
DECL|member|content_length
name|ngx_table_elt_t
modifier|*
name|content_length
decl_stmt|;
DECL|member|content_encoding
name|ngx_table_elt_t
modifier|*
name|content_encoding
decl_stmt|;
DECL|member|location
name|ngx_table_elt_t
modifier|*
name|location
decl_stmt|;
DECL|member|last_modified
name|ngx_table_elt_t
modifier|*
name|last_modified
decl_stmt|;
DECL|member|content_range
name|ngx_table_elt_t
modifier|*
name|content_range
decl_stmt|;
DECL|member|accept_ranges
name|ngx_table_elt_t
modifier|*
name|accept_ranges
decl_stmt|;
DECL|member|expires
name|ngx_table_elt_t
modifier|*
name|expires
decl_stmt|;
DECL|member|cache_control
name|ngx_table_elt_t
modifier|*
name|cache_control
decl_stmt|;
DECL|member|etag
name|ngx_table_elt_t
modifier|*
name|etag
decl_stmt|;
DECL|member|charset
name|ngx_str_t
name|charset
decl_stmt|;
DECL|member|ranges
name|ngx_array_t
name|ranges
decl_stmt|;
DECL|member|content_length_n
name|off_t
name|content_length_n
decl_stmt|;
DECL|member|date_time
name|time_t
name|date_time
decl_stmt|;
DECL|member|last_modified_time
name|time_t
name|last_modified_time
decl_stmt|;
DECL|typedef|ngx_http_headers_out_t
block|}
name|ngx_http_headers_out_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon293e63170708
typedef|typedef
struct|struct
block|{
DECL|member|temp_file
name|ngx_temp_file_t
modifier|*
name|temp_file
decl_stmt|;
DECL|member|bufs
name|ngx_chain_t
modifier|*
name|bufs
decl_stmt|;
DECL|member|buf
name|ngx_buf_t
modifier|*
name|buf
decl_stmt|;
DECL|member|rest
name|size_t
name|rest
decl_stmt|;
DECL|member|handler
name|void
function_decl|(
modifier|*
name|handler
function_decl|)
parameter_list|(
name|void
modifier|*
name|data
parameter_list|)
function_decl|;
DECL|member|data
name|void
modifier|*
name|data
decl_stmt|;
DECL|typedef|ngx_http_request_body_t
block|}
name|ngx_http_request_body_t
typedef|;
end_typedef

begin_struct
DECL|struct|ngx_http_cleanup_s
struct|struct
name|ngx_http_cleanup_s
block|{
DECL|union|__anon293e6317080a
union|union
block|{
DECL|struct|__anon293e63170908
struct|struct
block|{
DECL|member|fd
name|ngx_fd_t
name|fd
decl_stmt|;
DECL|member|name
name|u_char
modifier|*
name|name
decl_stmt|;
DECL|member|file
block|}
name|file
struct|;
DECL|struct|__anon293e63170a08
struct|struct
block|{
DECL|member|hash
name|ngx_http_cache_hash_t
modifier|*
name|hash
decl_stmt|;
DECL|member|cache
name|ngx_http_cache_t
modifier|*
name|cache
decl_stmt|;
DECL|member|cache
block|}
name|cache
struct|;
DECL|member|data
block|}
name|data
union|;
DECL|member|valid
name|unsigned
name|valid
range|:
literal|1
decl_stmt|;
DECL|member|cache
name|unsigned
name|cache
range|:
literal|1
decl_stmt|;
block|}
struct|;
end_struct

begin_typedef
DECL|struct|__anon293e63170b08
typedef|typedef
struct|struct
block|{
DECL|member|request
name|ngx_http_request_t
modifier|*
name|request
decl_stmt|;
DECL|member|busy
name|ngx_buf_t
modifier|*
modifier|*
name|busy
decl_stmt|;
DECL|member|nbusy
name|ngx_int_t
name|nbusy
decl_stmt|;
DECL|member|free
name|ngx_buf_t
modifier|*
modifier|*
name|free
decl_stmt|;
DECL|member|nfree
name|ngx_int_t
name|nfree
decl_stmt|;
DECL|member|pipeline
name|ngx_uint_t
name|pipeline
decl_stmt|;
comment|/* unsigned  pipeline:1; */
DECL|typedef|ngx_http_connection_t
block|}
name|ngx_http_connection_t
typedef|;
end_typedef

begin_typedef
DECL|typedef|ngx_http_handler_pt
typedef|typedef
name|ngx_int_t
function_decl|(
modifier|*
name|ngx_http_handler_pt
function_decl|)
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
function_decl|;
end_typedef

begin_struct
DECL|struct|ngx_http_request_s
struct|struct
name|ngx_http_request_s
block|{
DECL|member|signature
name|uint32_t
name|signature
decl_stmt|;
comment|/* "HTTP" */
DECL|member|connection
name|ngx_connection_t
modifier|*
name|connection
decl_stmt|;
DECL|member|ctx
name|void
modifier|*
modifier|*
name|ctx
decl_stmt|;
DECL|member|main_conf
name|void
modifier|*
modifier|*
name|main_conf
decl_stmt|;
DECL|member|srv_conf
name|void
modifier|*
modifier|*
name|srv_conf
decl_stmt|;
DECL|member|loc_conf
name|void
modifier|*
modifier|*
name|loc_conf
decl_stmt|;
DECL|member|cache
name|ngx_http_cache_t
modifier|*
name|cache
decl_stmt|;
DECL|member|file
name|ngx_file_t
name|file
decl_stmt|;
DECL|member|pool
name|ngx_pool_t
modifier|*
name|pool
decl_stmt|;
DECL|member|header_in
name|ngx_buf_t
modifier|*
name|header_in
decl_stmt|;
DECL|member|headers_in
name|ngx_http_headers_in_t
name|headers_in
decl_stmt|;
DECL|member|headers_out
name|ngx_http_headers_out_t
name|headers_out
decl_stmt|;
DECL|member|request_body
name|ngx_http_request_body_t
modifier|*
name|request_body
decl_stmt|;
DECL|member|lingering_time
name|time_t
name|lingering_time
decl_stmt|;
DECL|member|method
name|ngx_uint_t
name|method
decl_stmt|;
DECL|member|http_version
name|ngx_uint_t
name|http_version
decl_stmt|;
DECL|member|http_major
name|ngx_uint_t
name|http_major
decl_stmt|;
DECL|member|http_minor
name|ngx_uint_t
name|http_minor
decl_stmt|;
DECL|member|request_line
name|ngx_str_t
name|request_line
decl_stmt|;
DECL|member|uri
name|ngx_str_t
name|uri
decl_stmt|;
DECL|member|args
name|ngx_str_t
name|args
decl_stmt|;
DECL|member|exten
name|ngx_str_t
name|exten
decl_stmt|;
DECL|member|unparsed_uri
name|ngx_str_t
name|unparsed_uri
decl_stmt|;
DECL|member|method_name
name|ngx_str_t
name|method_name
decl_stmt|;
DECL|member|main
name|ngx_http_request_t
modifier|*
decl|main
decl_stmt|;
DECL|member|in_addr
name|uint32_t
name|in_addr
decl_stmt|;
DECL|member|port
name|ngx_uint_t
name|port
decl_stmt|;
DECL|member|port_text
name|ngx_str_t
modifier|*
name|port_text
decl_stmt|;
comment|/* ":80" */
DECL|member|server_name
name|ngx_str_t
modifier|*
name|server_name
decl_stmt|;
DECL|member|virtual_names
name|ngx_array_t
modifier|*
name|virtual_names
decl_stmt|;
DECL|member|phase
name|ngx_uint_t
name|phase
decl_stmt|;
DECL|member|phase_handler
name|ngx_int_t
name|phase_handler
decl_stmt|;
DECL|member|content_handler
name|ngx_http_handler_pt
name|content_handler
decl_stmt|;
DECL|member|cleanup
name|ngx_array_t
name|cleanup
decl_stmt|;
comment|/* used to learn the Apache compatible response length without a header */
DECL|member|header_size
name|size_t
name|header_size
decl_stmt|;
DECL|member|discarded_buffer
name|u_char
modifier|*
name|discarded_buffer
decl_stmt|;
DECL|member|err_ctx
name|void
modifier|*
modifier|*
name|err_ctx
decl_stmt|;
DECL|member|err_status
name|ngx_uint_t
name|err_status
decl_stmt|;
DECL|member|http_connection
name|ngx_http_connection_t
modifier|*
name|http_connection
decl_stmt|;
DECL|member|http_state
name|unsigned
name|http_state
range|:
literal|4
decl_stmt|;
if|#
directive|if
literal|0
comment|/* URI is not started with '/' - "GET http://" */
block|unsigned             unusual_uri:1;
endif|#
directive|endif
comment|/* URI with "/.", "%" and on Win32 with "//" */
DECL|member|complex_uri
name|unsigned
name|complex_uri
range|:
literal|1
decl_stmt|;
DECL|member|header_timeout_set
name|unsigned
name|header_timeout_set
range|:
literal|1
decl_stmt|;
DECL|member|proxy
name|unsigned
name|proxy
range|:
literal|1
decl_stmt|;
DECL|member|bypass_cache
name|unsigned
name|bypass_cache
range|:
literal|1
decl_stmt|;
DECL|member|no_cache
name|unsigned
name|no_cache
range|:
literal|1
decl_stmt|;
if|#
directive|if
literal|0
block|unsigned             cachable:1;
endif|#
directive|endif
DECL|member|pipeline
name|unsigned
name|pipeline
range|:
literal|1
decl_stmt|;
comment|/* can we use sendfile ? */
DECL|member|sendfile
name|unsigned
name|sendfile
range|:
literal|1
decl_stmt|;
DECL|member|plain_http
name|unsigned
name|plain_http
range|:
literal|1
decl_stmt|;
DECL|member|chunked
name|unsigned
name|chunked
range|:
literal|1
decl_stmt|;
DECL|member|header_only
name|unsigned
name|header_only
range|:
literal|1
decl_stmt|;
DECL|member|keepalive
name|unsigned
name|keepalive
range|:
literal|1
decl_stmt|;
DECL|member|lingering_close
name|unsigned
name|lingering_close
range|:
literal|1
decl_stmt|;
DECL|member|closed
name|unsigned
name|closed
range|:
literal|1
decl_stmt|;
DECL|member|filter_need_in_memory
name|unsigned
name|filter_need_in_memory
range|:
literal|1
decl_stmt|;
DECL|member|filter_ssi_need_in_memory
name|unsigned
name|filter_ssi_need_in_memory
range|:
literal|1
decl_stmt|;
DECL|member|filter_need_temporary
name|unsigned
name|filter_need_temporary
range|:
literal|1
decl_stmt|;
DECL|member|filter_allow_ranges
name|unsigned
name|filter_allow_ranges
range|:
literal|1
decl_stmt|;
if|#
directive|if
operator|(
name|NGX_STAT_STUB
operator|)
DECL|member|stat_reading
name|unsigned
name|stat_reading
range|:
literal|1
decl_stmt|;
DECL|member|stat_writing
name|unsigned
name|stat_writing
range|:
literal|1
decl_stmt|;
endif|#
directive|endif
DECL|member|headers_n
name|ngx_uint_t
name|headers_n
decl_stmt|;
comment|/* used to parse HTTP headers */
DECL|member|state
name|ngx_uint_t
name|state
decl_stmt|;
DECL|member|uri_start
name|u_char
modifier|*
name|uri_start
decl_stmt|;
DECL|member|uri_end
name|u_char
modifier|*
name|uri_end
decl_stmt|;
DECL|member|uri_ext
name|u_char
modifier|*
name|uri_ext
decl_stmt|;
DECL|member|args_start
name|u_char
modifier|*
name|args_start
decl_stmt|;
DECL|member|request_start
name|u_char
modifier|*
name|request_start
decl_stmt|;
DECL|member|request_end
name|u_char
modifier|*
name|request_end
decl_stmt|;
DECL|member|method_end
name|u_char
modifier|*
name|method_end
decl_stmt|;
DECL|member|schema_start
name|u_char
modifier|*
name|schema_start
decl_stmt|;
DECL|member|schema_end
name|u_char
modifier|*
name|schema_end
decl_stmt|;
DECL|member|host_start
name|u_char
modifier|*
name|host_start
decl_stmt|;
DECL|member|host_end
name|u_char
modifier|*
name|host_end
decl_stmt|;
DECL|member|port_start
name|u_char
modifier|*
name|port_start
decl_stmt|;
DECL|member|port_end
name|u_char
modifier|*
name|port_end
decl_stmt|;
DECL|member|header_name_start
name|u_char
modifier|*
name|header_name_start
decl_stmt|;
DECL|member|header_name_end
name|u_char
modifier|*
name|header_name_end
decl_stmt|;
DECL|member|header_start
name|u_char
modifier|*
name|header_start
decl_stmt|;
DECL|member|header_end
name|u_char
modifier|*
name|header_end
decl_stmt|;
block|}
struct|;
end_struct

begin_decl_stmt
specifier|extern
name|ngx_http_header_t
name|ngx_http_headers_in
index|[]
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|ngx_http_header_t
name|ngx_http_headers_out
index|[]
decl_stmt|;
end_decl_stmt

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* _NGX_HTTP_REQUEST_H_INCLUDED_ */
end_comment

end_unit

