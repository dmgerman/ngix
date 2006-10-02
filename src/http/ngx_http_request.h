begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (C) Igor Sysoev  */
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
DECL|macro|NGX_HTTP_MAX_URI_CHANGES
define|#
directive|define
name|NGX_HTTP_MAX_URI_CHANGES
value|10
end_define

begin_define
DECL|macro|NGX_HTTP_MAX_SUBREQUESTS
define|#
directive|define
name|NGX_HTTP_MAX_SUBREQUESTS
value|50
end_define

begin_comment
comment|/* must be 2^n */
end_comment

begin_define
DECL|macro|NGX_HTTP_LC_HEADER_LEN
define|#
directive|define
name|NGX_HTTP_LC_HEADER_LEN
value|32
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
DECL|macro|NGX_HTTP_UNKNOWN
define|#
directive|define
name|NGX_HTTP_UNKNOWN
value|0x0001
end_define

begin_define
DECL|macro|NGX_HTTP_GET
define|#
directive|define
name|NGX_HTTP_GET
value|0x0002
end_define

begin_define
DECL|macro|NGX_HTTP_HEAD
define|#
directive|define
name|NGX_HTTP_HEAD
value|0x0004
end_define

begin_define
DECL|macro|NGX_HTTP_POST
define|#
directive|define
name|NGX_HTTP_POST
value|0x0008
end_define

begin_define
DECL|macro|NGX_HTTP_PUT
define|#
directive|define
name|NGX_HTTP_PUT
value|0x0010
end_define

begin_define
DECL|macro|NGX_HTTP_DELETE
define|#
directive|define
name|NGX_HTTP_DELETE
value|0x0020
end_define

begin_define
DECL|macro|NGX_HTTP_MKCOL
define|#
directive|define
name|NGX_HTTP_MKCOL
value|0x0040
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
DECL|macro|NGX_HTTP_PARSE_INVALID_09_METHOD
define|#
directive|define
name|NGX_HTTP_PARSE_INVALID_09_METHOD
value|12
end_define

begin_define
DECL|macro|NGX_HTTP_PARSE_INVALID_HEADER
define|#
directive|define
name|NGX_HTTP_PARSE_INVALID_HEADER
value|13
end_define

begin_define
DECL|macro|NGX_HTTP_ZERO_IN_URI
define|#
directive|define
name|NGX_HTTP_ZERO_IN_URI
value|1
end_define

begin_define
DECL|macro|NGX_HTTP_OK
define|#
directive|define
name|NGX_HTTP_OK
value|200
end_define

begin_define
DECL|macro|NGX_HTTP_CREATED
define|#
directive|define
name|NGX_HTTP_CREATED
value|201
end_define

begin_define
DECL|macro|NGX_HTTP_NO_CONTENT
define|#
directive|define
name|NGX_HTTP_NO_CONTENT
value|204
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
DECL|macro|NGX_HTTP_UNAUTHORIZED
define|#
directive|define
name|NGX_HTTP_UNAUTHORIZED
value|401
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
DECL|macro|NGX_HTTP_CONFLICT
define|#
directive|define
name|NGX_HTTP_CONFLICT
value|409
end_define

begin_define
DECL|macro|NGX_HTTP_LENGTH_REQUIRED
define|#
directive|define
name|NGX_HTTP_LENGTH_REQUIRED
value|411
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
DECL|macro|NGX_HTTP_UNSUPPORTED_MEDIA_TYPE
define|#
directive|define
name|NGX_HTTP_UNSUPPORTED_MEDIA_TYPE
value|415
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

begin_comment
comment|/* The special code to close connection without any response */
end_comment

begin_define
DECL|macro|NGX_HTTP_CLOSE
define|#
directive|define
name|NGX_HTTP_CLOSE
value|444
end_define

begin_define
DECL|macro|NGX_HTTP_OWN_CODES
define|#
directive|define
name|NGX_HTTP_OWN_CODES
value|495
end_define

begin_define
DECL|macro|NGX_HTTPS_CERT_ERROR
define|#
directive|define
name|NGX_HTTPS_CERT_ERROR
value|495
end_define

begin_define
DECL|macro|NGX_HTTPS_NO_CERT
define|#
directive|define
name|NGX_HTTPS_NO_CERT
value|496
end_define

begin_comment
comment|/*  * We use the special code for the plain HTTP requests that are sent to  * HTTPS port to distinguish it from 4XX in an error page redirection  */
end_comment

begin_define
DECL|macro|NGX_HTTP_TO_HTTPS
define|#
directive|define
name|NGX_HTTP_TO_HTTPS
value|497
end_define

begin_comment
comment|/* 498 is the canceled code for the requests with invalid host name */
end_comment

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

begin_define
DECL|macro|NGX_HTTP_INSUFFICIENT_STORAGE
define|#
directive|define
name|NGX_HTTP_INSUFFICIENT_STORAGE
value|507
end_define

begin_define
DECL|macro|NGX_HTTP_LOWLEVEL_BUFFERED
define|#
directive|define
name|NGX_HTTP_LOWLEVEL_BUFFERED
value|0x000000f0
end_define

begin_define
DECL|macro|NGX_HTTP_WRITE_BUFFERED
define|#
directive|define
name|NGX_HTTP_WRITE_BUFFERED
value|0x00000010
end_define

begin_define
DECL|macro|NGX_HTTP_GZIP_BUFFERED
define|#
directive|define
name|NGX_HTTP_GZIP_BUFFERED
value|0x00000020
end_define

begin_define
DECL|macro|NGX_HTTP_SSI_BUFFERED
define|#
directive|define
name|NGX_HTTP_SSI_BUFFERED
value|0x00000100
end_define

begin_define
DECL|macro|NGX_HTTP_COPY_BUFFERED
define|#
directive|define
name|NGX_HTTP_COPY_BUFFERED
value|0x00000200
end_define

begin_typedef
DECL|enum|__anon2ab31b590103
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
DECL|struct|__anon2ab31b590208
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
DECL|member|handler
name|ngx_http_header_handler_pt
name|handler
decl_stmt|;
DECL|typedef|ngx_http_header_t
block|}
name|ngx_http_header_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon2ab31b590308
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
DECL|typedef|ngx_http_header_out_t
block|}
name|ngx_http_header_out_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon2ab31b590408
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
DECL|member|content_type
name|ngx_table_elt_t
modifier|*
name|content_type
decl_stmt|;
DECL|member|range
name|ngx_table_elt_t
modifier|*
name|range
decl_stmt|;
DECL|member|transfer_encoding
name|ngx_table_elt_t
modifier|*
name|transfer_encoding
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
operator|||
name|NGX_HTTP_REALIP
operator|)
DECL|member|x_forwarded_for
name|ngx_table_elt_t
modifier|*
name|x_forwarded_for
decl_stmt|;
endif|#
directive|endif
if|#
directive|if
operator|(
name|NGX_HTTP_REALIP
operator|)
DECL|member|x_real_ip
name|ngx_table_elt_t
modifier|*
name|x_real_ip
decl_stmt|;
endif|#
directive|endif
if|#
directive|if
operator|(
name|NGX_HTTP_HEADERS
operator|)
DECL|member|accept
name|ngx_table_elt_t
modifier|*
name|accept
decl_stmt|;
DECL|member|accept_language
name|ngx_table_elt_t
modifier|*
name|accept_language
decl_stmt|;
endif|#
directive|endif
if|#
directive|if
operator|(
name|NGX_HTTP_DAV
operator|)
DECL|member|depth
name|ngx_table_elt_t
modifier|*
name|depth
decl_stmt|;
DECL|member|destination
name|ngx_table_elt_t
modifier|*
name|destination
decl_stmt|;
DECL|member|date
name|ngx_table_elt_t
modifier|*
name|date
decl_stmt|;
endif|#
directive|endif
DECL|member|user
name|ngx_str_t
name|user
decl_stmt|;
DECL|member|passwd
name|ngx_str_t
name|passwd
decl_stmt|;
DECL|member|cookies
name|ngx_array_t
name|cookies
decl_stmt|;
DECL|member|host_name_len
name|size_t
name|host_name_len
decl_stmt|;
DECL|member|content_length_n
name|off_t
name|content_length_n
decl_stmt|;
DECL|member|keep_alive_n
name|time_t
name|keep_alive_n
decl_stmt|;
DECL|member|connection_type
name|unsigned
name|connection_type
range|:
literal|2
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
DECL|struct|__anon2ab31b590508
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
DECL|struct|__anon2ab31b590608
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
DECL|member|www_authenticate
name|ngx_table_elt_t
modifier|*
name|www_authenticate
decl_stmt|;
DECL|member|expires
name|ngx_table_elt_t
modifier|*
name|expires
decl_stmt|;
DECL|member|etag
name|ngx_table_elt_t
modifier|*
name|etag
decl_stmt|;
DECL|member|override_charset
name|ngx_str_t
modifier|*
name|override_charset
decl_stmt|;
DECL|member|content_type_len
name|size_t
name|content_type_len
decl_stmt|;
DECL|member|content_type
name|ngx_str_t
name|content_type
decl_stmt|;
DECL|member|charset
name|ngx_str_t
name|charset
decl_stmt|;
DECL|member|ranges
name|ngx_array_t
name|ranges
decl_stmt|;
DECL|member|cache_control
name|ngx_array_t
name|cache_control
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
DECL|typedef|ngx_http_client_body_handler_pt
typedef|typedef
name|void
function_decl|(
modifier|*
name|ngx_http_client_body_handler_pt
function_decl|)
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
function_decl|;
end_typedef

begin_typedef
DECL|struct|__anon2ab31b590708
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
name|off_t
name|rest
decl_stmt|;
DECL|member|to_write
name|ngx_chain_t
modifier|*
name|to_write
decl_stmt|;
DECL|member|post_handler
name|ngx_http_client_body_handler_pt
name|post_handler
decl_stmt|;
DECL|typedef|ngx_http_request_body_t
block|}
name|ngx_http_request_body_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon2ab31b590808
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
DECL|struct|__anon2ab31b590908
typedef|typedef
struct|struct
block|{
DECL|member|hash
name|ngx_hash_t
name|hash
decl_stmt|;
DECL|member|dns_wildcards
name|ngx_hash_wildcard_t
modifier|*
name|dns_wildcards
decl_stmt|;
DECL|typedef|ngx_http_virtual_names_t
block|}
name|ngx_http_virtual_names_t
typedef|;
end_typedef

begin_typedef
DECL|typedef|ngx_http_cleanup_pt
typedef|typedef
name|void
function_decl|(
modifier|*
name|ngx_http_cleanup_pt
function_decl|)
parameter_list|(
name|void
modifier|*
name|data
parameter_list|)
function_decl|;
end_typedef

begin_typedef
DECL|typedef|ngx_http_cleanup_t
typedef|typedef
name|struct
name|ngx_http_cleanup_s
name|ngx_http_cleanup_t
typedef|;
end_typedef

begin_struct
DECL|struct|ngx_http_cleanup_s
struct|struct
name|ngx_http_cleanup_s
block|{
DECL|member|handler
name|ngx_http_cleanup_pt
name|handler
decl_stmt|;
DECL|member|data
name|void
modifier|*
name|data
decl_stmt|;
DECL|member|next
name|ngx_http_cleanup_t
modifier|*
name|next
decl_stmt|;
block|}
struct|;
end_struct

begin_typedef
DECL|typedef|ngx_http_postponed_request_t
typedef|typedef
name|struct
name|ngx_http_postponed_request_s
name|ngx_http_postponed_request_t
typedef|;
end_typedef

begin_struct
DECL|struct|ngx_http_postponed_request_s
struct|struct
name|ngx_http_postponed_request_s
block|{
DECL|member|request
name|ngx_http_request_t
modifier|*
name|request
decl_stmt|;
DECL|member|out
name|ngx_chain_t
modifier|*
name|out
decl_stmt|;
DECL|member|next
name|ngx_http_postponed_request_t
modifier|*
name|next
decl_stmt|;
block|}
struct|;
end_struct

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

begin_typedef
DECL|typedef|ngx_http_event_handler_pt
typedef|typedef
name|void
function_decl|(
modifier|*
name|ngx_http_event_handler_pt
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
DECL|member|read_event_handler
name|ngx_http_event_handler_pt
name|read_event_handler
decl_stmt|;
DECL|member|write_event_handler
name|ngx_http_event_handler_pt
name|write_event_handler
decl_stmt|;
DECL|member|cache
name|ngx_http_cache_t
modifier|*
name|cache
decl_stmt|;
DECL|member|upstream
name|ngx_http_upstream_t
modifier|*
name|upstream
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
DECL|member|start_time
name|time_t
name|start_time
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
DECL|member|http_protocol
name|ngx_str_t
name|http_protocol
decl_stmt|;
DECL|member|out
name|ngx_chain_t
modifier|*
name|out
decl_stmt|;
DECL|member|main
name|ngx_http_request_t
modifier|*
decl|main
decl_stmt|;
DECL|member|parent
name|ngx_http_request_t
modifier|*
name|parent
decl_stmt|;
DECL|member|postponed
name|ngx_http_postponed_request_t
modifier|*
name|postponed
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
name|server_name
decl_stmt|;
DECL|member|virtual_names
name|ngx_http_virtual_names_t
modifier|*
name|virtual_names
decl_stmt|;
DECL|member|phase_handler
name|ngx_int_t
name|phase_handler
decl_stmt|;
DECL|member|content_handler
name|ngx_http_handler_pt
name|content_handler
decl_stmt|;
DECL|member|access_code
name|ngx_uint_t
name|access_code
decl_stmt|;
DECL|member|variables
name|ngx_http_variable_value_t
modifier|*
name|variables
decl_stmt|;
DECL|member|root_length
name|size_t
name|root_length
decl_stmt|;
DECL|member|limit_rate
name|size_t
name|limit_rate
decl_stmt|;
comment|/* used to learn the Apache compatible response length without a header */
DECL|member|header_size
name|size_t
name|header_size
decl_stmt|;
DECL|member|request_length
name|off_t
name|request_length
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
DECL|member|log_handler
name|ngx_http_log_handler_pt
name|log_handler
decl_stmt|;
DECL|member|cleanup
name|ngx_http_cleanup_t
modifier|*
name|cleanup
decl_stmt|;
DECL|member|http_state
name|unsigned
name|http_state
range|:
literal|4
decl_stmt|;
comment|/* URI with "/." and on Win32 with "//" */
DECL|member|complex_uri
name|unsigned
name|complex_uri
range|:
literal|1
decl_stmt|;
comment|/* URI with "%" */
DECL|member|quoted_uri
name|unsigned
name|quoted_uri
range|:
literal|1
decl_stmt|;
comment|/* URI with "+" */
DECL|member|plus_in_uri
name|unsigned
name|plus_in_uri
range|:
literal|1
decl_stmt|;
comment|/* URI with "\0" or "%00" */
DECL|member|zero_in_uri
name|unsigned
name|zero_in_uri
range|:
literal|1
decl_stmt|;
DECL|member|invalid_header
name|unsigned
name|invalid_header
range|:
literal|1
decl_stmt|;
DECL|member|valid_location
name|unsigned
name|valid_location
range|:
literal|1
decl_stmt|;
DECL|member|valid_unparsed_uri
name|unsigned
name|valid_unparsed_uri
range|:
literal|1
decl_stmt|;
DECL|member|uri_changed
name|unsigned
name|uri_changed
range|:
literal|1
decl_stmt|;
DECL|member|uri_changes
name|unsigned
name|uri_changes
range|:
literal|4
decl_stmt|;
DECL|member|request_body_in_single_buf
name|unsigned
name|request_body_in_single_buf
range|:
literal|1
decl_stmt|;
DECL|member|request_body_in_file_only
name|unsigned
name|request_body_in_file_only
range|:
literal|1
decl_stmt|;
DECL|member|request_body_in_persistent_file
name|unsigned
name|request_body_in_persistent_file
range|:
literal|1
decl_stmt|;
DECL|member|request_body_delete_incomplete_file
name|unsigned
name|request_body_delete_incomplete_file
range|:
literal|1
decl_stmt|;
DECL|member|request_body_file_group_access
name|unsigned
name|request_body_file_group_access
range|:
literal|1
decl_stmt|;
DECL|member|request_body_file_log_level
name|unsigned
name|request_body_file_log_level
range|:
literal|3
decl_stmt|;
DECL|member|fast_subrequest
name|unsigned
name|fast_subrequest
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
operator|(
name|NGX_HTTP_REALIP
operator|)
comment|/*      * instead of using the request context data in ngx_http_realip_module      * we use the single bit in the request structure      */
DECL|member|realip_set
name|unsigned
name|realip_set
range|:
literal|1
decl_stmt|;
endif|#
directive|endif
if|#
directive|if
literal|0
block|unsigned                          cachable:1;
endif|#
directive|endif
DECL|member|pipeline
name|unsigned
name|pipeline
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
DECL|member|zero_body
name|unsigned
name|zero_body
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
DECL|member|discard_body
name|unsigned
name|discard_body
range|:
literal|1
decl_stmt|;
DECL|member|internal
name|unsigned
name|internal
range|:
literal|1
decl_stmt|;
DECL|member|error_page
name|unsigned
name|error_page
range|:
literal|1
decl_stmt|;
DECL|member|post_action
name|unsigned
name|post_action
range|:
literal|1
decl_stmt|;
DECL|member|request_complete
name|unsigned
name|request_complete
range|:
literal|1
decl_stmt|;
DECL|member|request_output
name|unsigned
name|request_output
range|:
literal|1
decl_stmt|;
DECL|member|header_sent
name|unsigned
name|header_sent
range|:
literal|1
decl_stmt|;
DECL|member|done
name|unsigned
name|done
range|:
literal|1
decl_stmt|;
DECL|member|utf8
name|unsigned
name|utf8
range|:
literal|1
decl_stmt|;
DECL|member|main_filter_need_in_memory
name|unsigned
name|main_filter_need_in_memory
range|:
literal|1
decl_stmt|;
DECL|member|filter_need_in_memory
name|unsigned
name|filter_need_in_memory
range|:
literal|1
decl_stmt|;
DECL|member|filter_need_temporary
name|unsigned
name|filter_need_temporary
range|:
literal|1
decl_stmt|;
DECL|member|allow_ranges
name|unsigned
name|allow_ranges
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
DECL|member|subrequests
name|unsigned
name|subrequests
range|:
literal|8
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
DECL|member|header_hash
name|ngx_uint_t
name|header_hash
decl_stmt|;
DECL|member|lowcase_index
name|ngx_uint_t
name|lowcase_index
decl_stmt|;
DECL|member|lowcase_header
name|u_char
name|lowcase_header
index|[
name|NGX_HTTP_LC_HEADER_LEN
index|]
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
name|ngx_http_header_out_t
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

