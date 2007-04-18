begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (C) Igor Sysoev  */
end_comment

begin_ifndef
ifndef|#
directive|ifndef
name|_NGX_HTTP_UPSTREAM_H_INCLUDED_
end_ifndef

begin_define
DECL|macro|_NGX_HTTP_UPSTREAM_H_INCLUDED_
define|#
directive|define
name|_NGX_HTTP_UPSTREAM_H_INCLUDED_
end_define

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
file|<ngx_event.h>
end_include

begin_include
include|#
directive|include
file|<ngx_event_connect.h>
end_include

begin_include
include|#
directive|include
file|<ngx_event_pipe.h>
end_include

begin_include
include|#
directive|include
file|<ngx_http.h>
end_include

begin_define
DECL|macro|NGX_HTTP_UPSTREAM_FT_ERROR
define|#
directive|define
name|NGX_HTTP_UPSTREAM_FT_ERROR
value|0x00000002
end_define

begin_define
DECL|macro|NGX_HTTP_UPSTREAM_FT_TIMEOUT
define|#
directive|define
name|NGX_HTTP_UPSTREAM_FT_TIMEOUT
value|0x00000004
end_define

begin_define
DECL|macro|NGX_HTTP_UPSTREAM_FT_INVALID_HEADER
define|#
directive|define
name|NGX_HTTP_UPSTREAM_FT_INVALID_HEADER
value|0x00000008
end_define

begin_define
DECL|macro|NGX_HTTP_UPSTREAM_FT_HTTP_500
define|#
directive|define
name|NGX_HTTP_UPSTREAM_FT_HTTP_500
value|0x00000010
end_define

begin_define
DECL|macro|NGX_HTTP_UPSTREAM_FT_HTTP_503
define|#
directive|define
name|NGX_HTTP_UPSTREAM_FT_HTTP_503
value|0x00000020
end_define

begin_define
DECL|macro|NGX_HTTP_UPSTREAM_FT_HTTP_404
define|#
directive|define
name|NGX_HTTP_UPSTREAM_FT_HTTP_404
value|0x00000040
end_define

begin_define
DECL|macro|NGX_HTTP_UPSTREAM_FT_BUSY_LOCK
define|#
directive|define
name|NGX_HTTP_UPSTREAM_FT_BUSY_LOCK
value|0x00000080
end_define

begin_define
DECL|macro|NGX_HTTP_UPSTREAM_FT_MAX_WAITING
define|#
directive|define
name|NGX_HTTP_UPSTREAM_FT_MAX_WAITING
value|0x00000100
end_define

begin_define
DECL|macro|NGX_HTTP_UPSTREAM_FT_OFF
define|#
directive|define
name|NGX_HTTP_UPSTREAM_FT_OFF
value|0x80000000
end_define

begin_define
DECL|macro|NGX_HTTP_UPSTREAM_INVALID_HEADER
define|#
directive|define
name|NGX_HTTP_UPSTREAM_INVALID_HEADER
value|40
end_define

begin_typedef
DECL|struct|__anon2b2dee830108
typedef|typedef
struct|struct
block|{
DECL|member|bl_time
name|ngx_msec_t
name|bl_time
decl_stmt|;
DECL|member|bl_state
name|ngx_uint_t
name|bl_state
decl_stmt|;
DECL|member|status
name|ngx_uint_t
name|status
decl_stmt|;
DECL|member|response_sec
name|time_t
name|response_sec
decl_stmt|;
DECL|member|response_msec
name|ngx_uint_t
name|response_msec
decl_stmt|;
DECL|member|peer
name|ngx_str_t
modifier|*
name|peer
decl_stmt|;
DECL|typedef|ngx_http_upstream_state_t
block|}
name|ngx_http_upstream_state_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon2b2dee830208
typedef|typedef
struct|struct
block|{
DECL|member|headers_in_hash
name|ngx_hash_t
name|headers_in_hash
decl_stmt|;
DECL|member|upstreams
name|ngx_array_t
name|upstreams
decl_stmt|;
comment|/* ngx_http_upstream_srv_conf_t */
DECL|typedef|ngx_http_upstream_main_conf_t
block|}
name|ngx_http_upstream_main_conf_t
typedef|;
end_typedef

begin_typedef
DECL|typedef|ngx_http_upstream_srv_conf_t
typedef|typedef
name|struct
name|ngx_http_upstream_srv_conf_s
name|ngx_http_upstream_srv_conf_t
typedef|;
end_typedef

begin_typedef
DECL|typedef|ngx_http_upstream_init_pt
typedef|typedef
name|ngx_int_t
function_decl|(
modifier|*
name|ngx_http_upstream_init_pt
function_decl|)
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|ngx_http_upstream_srv_conf_t
modifier|*
name|us
parameter_list|)
function_decl|;
end_typedef

begin_typedef
DECL|typedef|ngx_http_upstream_init_peer_pt
typedef|typedef
name|ngx_int_t
function_decl|(
modifier|*
name|ngx_http_upstream_init_peer_pt
function_decl|)
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_upstream_srv_conf_t
modifier|*
name|us
parameter_list|)
function_decl|;
end_typedef

begin_typedef
DECL|struct|__anon2b2dee830308
typedef|typedef
struct|struct
block|{
DECL|member|init_upstream
name|ngx_http_upstream_init_pt
name|init_upstream
decl_stmt|;
DECL|member|init
name|ngx_http_upstream_init_peer_pt
name|init
decl_stmt|;
DECL|member|data
name|void
modifier|*
name|data
decl_stmt|;
DECL|typedef|ngx_http_upstream_peer_t
block|}
name|ngx_http_upstream_peer_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon2b2dee830408
typedef|typedef
struct|struct
block|{
DECL|member|addrs
name|ngx_peer_addr_t
modifier|*
name|addrs
decl_stmt|;
DECL|member|naddrs
name|ngx_uint_t
name|naddrs
decl_stmt|;
DECL|member|weight
name|ngx_uint_t
name|weight
decl_stmt|;
DECL|member|max_fails
name|ngx_uint_t
name|max_fails
decl_stmt|;
DECL|member|fail_timeout
name|time_t
name|fail_timeout
decl_stmt|;
DECL|member|down
name|unsigned
name|down
range|:
literal|1
decl_stmt|;
DECL|member|backup
name|unsigned
name|backup
range|:
literal|1
decl_stmt|;
DECL|typedef|ngx_http_upstream_server_t
block|}
name|ngx_http_upstream_server_t
typedef|;
end_typedef

begin_define
DECL|macro|NGX_HTTP_UPSTREAM_CREATE
define|#
directive|define
name|NGX_HTTP_UPSTREAM_CREATE
value|0x0001
end_define

begin_define
DECL|macro|NGX_HTTP_UPSTREAM_WEIGHT
define|#
directive|define
name|NGX_HTTP_UPSTREAM_WEIGHT
value|0x0002
end_define

begin_define
DECL|macro|NGX_HTTP_UPSTREAM_MAX_FAILS
define|#
directive|define
name|NGX_HTTP_UPSTREAM_MAX_FAILS
value|0x0004
end_define

begin_define
DECL|macro|NGX_HTTP_UPSTREAM_FAIL_TIMEOUT
define|#
directive|define
name|NGX_HTTP_UPSTREAM_FAIL_TIMEOUT
value|0x0008
end_define

begin_define
DECL|macro|NGX_HTTP_UPSTREAM_DOWN
define|#
directive|define
name|NGX_HTTP_UPSTREAM_DOWN
value|0x0010
end_define

begin_define
DECL|macro|NGX_HTTP_UPSTREAM_BACKUP
define|#
directive|define
name|NGX_HTTP_UPSTREAM_BACKUP
value|0x0020
end_define

begin_struct
DECL|struct|ngx_http_upstream_srv_conf_s
struct|struct
name|ngx_http_upstream_srv_conf_s
block|{
DECL|member|peer
name|ngx_http_upstream_peer_t
name|peer
decl_stmt|;
DECL|member|srv_conf
name|void
modifier|*
modifier|*
name|srv_conf
decl_stmt|;
DECL|member|servers
name|ngx_array_t
modifier|*
name|servers
decl_stmt|;
comment|/* ngx_http_upstream_server_t */
DECL|member|flags
name|ngx_uint_t
name|flags
decl_stmt|;
DECL|member|host
name|ngx_str_t
name|host
decl_stmt|;
DECL|member|file_name
name|ngx_str_t
name|file_name
decl_stmt|;
DECL|member|line
name|ngx_uint_t
name|line
decl_stmt|;
DECL|member|port
name|in_port_t
name|port
decl_stmt|;
DECL|member|default_port
name|in_port_t
name|default_port
decl_stmt|;
block|}
struct|;
end_struct

begin_typedef
DECL|struct|__anon2b2dee830508
typedef|typedef
struct|struct
block|{
DECL|member|upstream
name|ngx_http_upstream_srv_conf_t
modifier|*
name|upstream
decl_stmt|;
DECL|member|connect_timeout
name|ngx_msec_t
name|connect_timeout
decl_stmt|;
DECL|member|send_timeout
name|ngx_msec_t
name|send_timeout
decl_stmt|;
DECL|member|read_timeout
name|ngx_msec_t
name|read_timeout
decl_stmt|;
DECL|member|timeout
name|ngx_msec_t
name|timeout
decl_stmt|;
DECL|member|send_lowat
name|size_t
name|send_lowat
decl_stmt|;
DECL|member|buffer_size
name|size_t
name|buffer_size
decl_stmt|;
DECL|member|busy_buffers_size
name|size_t
name|busy_buffers_size
decl_stmt|;
DECL|member|max_temp_file_size
name|size_t
name|max_temp_file_size
decl_stmt|;
DECL|member|temp_file_write_size
name|size_t
name|temp_file_write_size
decl_stmt|;
DECL|member|busy_buffers_size_conf
name|size_t
name|busy_buffers_size_conf
decl_stmt|;
DECL|member|max_temp_file_size_conf
name|size_t
name|max_temp_file_size_conf
decl_stmt|;
DECL|member|temp_file_write_size_conf
name|size_t
name|temp_file_write_size_conf
decl_stmt|;
DECL|member|next_upstream
name|ngx_uint_t
name|next_upstream
decl_stmt|;
DECL|member|bufs
name|ngx_bufs_t
name|bufs
decl_stmt|;
DECL|member|buffering
name|ngx_flag_t
name|buffering
decl_stmt|;
DECL|member|pass_request_headers
name|ngx_flag_t
name|pass_request_headers
decl_stmt|;
DECL|member|pass_request_body
name|ngx_flag_t
name|pass_request_body
decl_stmt|;
DECL|member|ignore_client_abort
name|ngx_flag_t
name|ignore_client_abort
decl_stmt|;
DECL|member|intercept_errors
name|ngx_flag_t
name|intercept_errors
decl_stmt|;
DECL|member|cyclic_temp_file
name|ngx_flag_t
name|cyclic_temp_file
decl_stmt|;
DECL|member|temp_path
name|ngx_path_t
modifier|*
name|temp_path
decl_stmt|;
DECL|member|hide_headers_hash
name|ngx_hash_t
name|hide_headers_hash
decl_stmt|;
DECL|member|hide_headers
name|ngx_array_t
modifier|*
name|hide_headers
decl_stmt|;
DECL|member|pass_headers
name|ngx_array_t
modifier|*
name|pass_headers
decl_stmt|;
DECL|member|schema
name|ngx_str_t
name|schema
decl_stmt|;
DECL|member|uri
name|ngx_str_t
name|uri
decl_stmt|;
DECL|member|location
name|ngx_str_t
name|location
decl_stmt|;
DECL|member|url
name|ngx_str_t
name|url
decl_stmt|;
comment|/* used in proxy_rewrite_location */
DECL|member|intercept_404
name|unsigned
name|intercept_404
range|:
literal|1
decl_stmt|;
DECL|member|change_buffering
name|unsigned
name|change_buffering
range|:
literal|1
decl_stmt|;
if|#
directive|if
operator|(
name|NGX_HTTP_SSL
operator|)
DECL|member|ssl
name|ngx_ssl_t
modifier|*
name|ssl
decl_stmt|;
endif|#
directive|endif
DECL|typedef|ngx_http_upstream_conf_t
block|}
name|ngx_http_upstream_conf_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon2b2dee830608
typedef|typedef
struct|struct
block|{
DECL|member|name
name|ngx_str_t
name|name
decl_stmt|;
DECL|member|handler
name|ngx_http_header_handler_pt
name|handler
decl_stmt|;
DECL|member|offset
name|ngx_uint_t
name|offset
decl_stmt|;
DECL|member|copy_handler
name|ngx_http_header_handler_pt
name|copy_handler
decl_stmt|;
DECL|member|conf
name|ngx_uint_t
name|conf
decl_stmt|;
DECL|member|redirect
name|ngx_uint_t
name|redirect
decl_stmt|;
comment|/* unsigned   redirect:1; */
DECL|typedef|ngx_http_upstream_header_t
block|}
name|ngx_http_upstream_header_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon2b2dee830708
typedef|typedef
struct|struct
block|{
DECL|member|headers
name|ngx_list_t
name|headers
decl_stmt|;
DECL|member|status_n
name|ngx_uint_t
name|status_n
decl_stmt|;
DECL|member|status_line
name|ngx_str_t
name|status_line
decl_stmt|;
DECL|member|status
name|ngx_table_elt_t
modifier|*
name|status
decl_stmt|;
DECL|member|date
name|ngx_table_elt_t
modifier|*
name|date
decl_stmt|;
DECL|member|server
name|ngx_table_elt_t
modifier|*
name|server
decl_stmt|;
DECL|member|connection
name|ngx_table_elt_t
modifier|*
name|connection
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
DECL|member|x_accel_expires
name|ngx_table_elt_t
modifier|*
name|x_accel_expires
decl_stmt|;
DECL|member|x_accel_redirect
name|ngx_table_elt_t
modifier|*
name|x_accel_redirect
decl_stmt|;
DECL|member|x_accel_limit_rate
name|ngx_table_elt_t
modifier|*
name|x_accel_limit_rate
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
DECL|member|last_modified
name|ngx_table_elt_t
modifier|*
name|last_modified
decl_stmt|;
DECL|member|location
name|ngx_table_elt_t
modifier|*
name|location
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
if|#
directive|if
operator|(
name|NGX_HTTP_GZIP
operator|)
DECL|member|content_encoding
name|ngx_table_elt_t
modifier|*
name|content_encoding
decl_stmt|;
endif|#
directive|endif
DECL|member|cache_control
name|ngx_array_t
name|cache_control
decl_stmt|;
DECL|typedef|ngx_http_upstream_headers_in_t
block|}
name|ngx_http_upstream_headers_in_t
typedef|;
end_typedef

begin_struct
DECL|struct|ngx_http_upstream_s
struct|struct
name|ngx_http_upstream_s
block|{
DECL|member|peer
name|ngx_peer_connection_t
name|peer
decl_stmt|;
DECL|member|pipe
name|ngx_event_pipe_t
modifier|*
name|pipe
decl_stmt|;
DECL|member|request_bufs
name|ngx_chain_t
modifier|*
name|request_bufs
decl_stmt|;
DECL|member|output
name|ngx_output_chain_ctx_t
name|output
decl_stmt|;
DECL|member|writer
name|ngx_chain_writer_ctx_t
name|writer
decl_stmt|;
DECL|member|conf
name|ngx_http_upstream_conf_t
modifier|*
name|conf
decl_stmt|;
DECL|member|headers_in
name|ngx_http_upstream_headers_in_t
name|headers_in
decl_stmt|;
DECL|member|buffer
name|ngx_buf_t
name|buffer
decl_stmt|;
DECL|member|length
name|size_t
name|length
decl_stmt|;
DECL|member|out_bufs
name|ngx_chain_t
modifier|*
name|out_bufs
decl_stmt|;
DECL|member|busy_bufs
name|ngx_chain_t
modifier|*
name|busy_bufs
decl_stmt|;
DECL|member|free_bufs
name|ngx_chain_t
modifier|*
name|free_bufs
decl_stmt|;
DECL|member|input_filter_init
name|ngx_int_t
function_decl|(
modifier|*
name|input_filter_init
function_decl|)
parameter_list|(
name|void
modifier|*
name|data
parameter_list|)
function_decl|;
DECL|member|input_filter
name|ngx_int_t
function_decl|(
modifier|*
name|input_filter
function_decl|)
parameter_list|(
name|void
modifier|*
name|data
parameter_list|,
name|ssize_t
name|bytes
parameter_list|)
function_decl|;
DECL|member|input_filter_ctx
name|void
modifier|*
name|input_filter_ctx
decl_stmt|;
DECL|member|create_request
name|ngx_int_t
function_decl|(
modifier|*
name|create_request
function_decl|)
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
function_decl|;
DECL|member|reinit_request
name|ngx_int_t
function_decl|(
modifier|*
name|reinit_request
function_decl|)
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
function_decl|;
DECL|member|process_header
name|ngx_int_t
function_decl|(
modifier|*
name|process_header
function_decl|)
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
function_decl|;
DECL|member|abort_request
name|void
function_decl|(
modifier|*
name|abort_request
function_decl|)
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
function_decl|;
DECL|member|finalize_request
name|void
function_decl|(
modifier|*
name|finalize_request
function_decl|)
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_int_t
name|rc
parameter_list|)
function_decl|;
DECL|member|rewrite_redirect
name|ngx_int_t
function_decl|(
modifier|*
name|rewrite_redirect
function_decl|)
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_table_elt_t
modifier|*
name|h
parameter_list|,
name|size_t
name|prefix
parameter_list|)
function_decl|;
DECL|member|timeout
name|ngx_msec_t
name|timeout
decl_stmt|;
DECL|member|method
name|ngx_str_t
name|method
decl_stmt|;
DECL|member|state
name|ngx_http_upstream_state_t
modifier|*
name|state
decl_stmt|;
DECL|member|uri
name|ngx_str_t
name|uri
decl_stmt|;
DECL|member|cleanup
name|ngx_http_cleanup_pt
modifier|*
name|cleanup
decl_stmt|;
DECL|member|cachable
name|unsigned
name|cachable
range|:
literal|1
decl_stmt|;
DECL|member|accel
name|unsigned
name|accel
range|:
literal|1
decl_stmt|;
DECL|member|buffering
name|unsigned
name|buffering
range|:
literal|1
decl_stmt|;
DECL|member|request_sent
name|unsigned
name|request_sent
range|:
literal|1
decl_stmt|;
DECL|member|header_sent
name|unsigned
name|header_sent
range|:
literal|1
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
name|ngx_int_t
name|ngx_http_upstream_header_variable
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_variable_value_t
modifier|*
name|v
parameter_list|,
name|uintptr_t
name|data
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|ngx_http_upstream_init
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_http_upstream_srv_conf_t
modifier|*
name|ngx_http_upstream_add
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|ngx_url_t
modifier|*
name|u
parameter_list|,
name|ngx_uint_t
name|flags
parameter_list|)
function_decl|;
end_function_decl

begin_define
DECL|macro|ngx_http_conf_upstream_srv_conf (uscf,module)
define|#
directive|define
name|ngx_http_conf_upstream_srv_conf
parameter_list|(
name|uscf
parameter_list|,
name|module
parameter_list|)
define|\
value|uscf->srv_conf[module.ctx_index]
end_define

begin_decl_stmt
specifier|extern
name|ngx_module_t
name|ngx_http_upstream_module
decl_stmt|;
end_decl_stmt

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* _NGX_HTTP_UPSTREAM_H_INCLUDED_ */
end_comment

end_unit

