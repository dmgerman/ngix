begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (C) Igor Sysoev  */
end_comment

begin_ifndef
ifndef|#
directive|ifndef
name|_NGX_HTTP_CORE_H_INCLUDED_
end_ifndef

begin_define
DECL|macro|_NGX_HTTP_CORE_H_INCLUDED_
define|#
directive|define
name|_NGX_HTTP_CORE_H_INCLUDED_
end_define

begin_include
include|#
directive|include
file|<ngx_string.h>
end_include

begin_include
include|#
directive|include
file|<ngx_array.h>
end_include

begin_include
include|#
directive|include
file|<ngx_http.h>
end_include

begin_define
DECL|macro|NGX_HTTP_GZIP_PROXIED_OFF
define|#
directive|define
name|NGX_HTTP_GZIP_PROXIED_OFF
value|0x0002
end_define

begin_define
DECL|macro|NGX_HTTP_GZIP_PROXIED_EXPIRED
define|#
directive|define
name|NGX_HTTP_GZIP_PROXIED_EXPIRED
value|0x0004
end_define

begin_define
DECL|macro|NGX_HTTP_GZIP_PROXIED_NO_CACHE
define|#
directive|define
name|NGX_HTTP_GZIP_PROXIED_NO_CACHE
value|0x0008
end_define

begin_define
DECL|macro|NGX_HTTP_GZIP_PROXIED_NO_STORE
define|#
directive|define
name|NGX_HTTP_GZIP_PROXIED_NO_STORE
value|0x0010
end_define

begin_define
DECL|macro|NGX_HTTP_GZIP_PROXIED_PRIVATE
define|#
directive|define
name|NGX_HTTP_GZIP_PROXIED_PRIVATE
value|0x0020
end_define

begin_define
DECL|macro|NGX_HTTP_GZIP_PROXIED_NO_LM
define|#
directive|define
name|NGX_HTTP_GZIP_PROXIED_NO_LM
value|0x0040
end_define

begin_define
DECL|macro|NGX_HTTP_GZIP_PROXIED_NO_ETAG
define|#
directive|define
name|NGX_HTTP_GZIP_PROXIED_NO_ETAG
value|0x0080
end_define

begin_define
DECL|macro|NGX_HTTP_GZIP_PROXIED_AUTH
define|#
directive|define
name|NGX_HTTP_GZIP_PROXIED_AUTH
value|0x0100
end_define

begin_define
DECL|macro|NGX_HTTP_GZIP_PROXIED_ANY
define|#
directive|define
name|NGX_HTTP_GZIP_PROXIED_ANY
value|0x0200
end_define

begin_define
DECL|macro|NGX_HTTP_SATISFY_ALL
define|#
directive|define
name|NGX_HTTP_SATISFY_ALL
value|0
end_define

begin_define
DECL|macro|NGX_HTTP_SATISFY_ANY
define|#
directive|define
name|NGX_HTTP_SATISFY_ANY
value|1
end_define

begin_typedef
DECL|struct|__anon2b2065660108
typedef|typedef
struct|struct
block|{
DECL|member|default_server
name|unsigned
name|default_server
range|:
literal|1
decl_stmt|;
DECL|member|bind
name|unsigned
name|bind
range|:
literal|1
decl_stmt|;
DECL|member|backlog
name|int
name|backlog
decl_stmt|;
DECL|member|rcvbuf
name|int
name|rcvbuf
decl_stmt|;
DECL|member|sndbuf
name|int
name|sndbuf
decl_stmt|;
if|#
directive|if
operator|(
name|NGX_HAVE_DEFERRED_ACCEPT
operator|&&
name|defined
name|SO_ACCEPTFILTER
operator|)
DECL|member|accept_filter
name|char
modifier|*
name|accept_filter
decl_stmt|;
endif|#
directive|endif
if|#
directive|if
operator|(
name|NGX_HAVE_DEFERRED_ACCEPT
operator|&&
name|defined
name|TCP_DEFER_ACCEPT
operator|)
DECL|member|deferred_accept
name|ngx_uint_t
name|deferred_accept
decl_stmt|;
endif|#
directive|endif
DECL|member|addr
name|u_char
name|addr
index|[
name|INET_ADDRSTRLEN
operator|+
literal|6
index|]
decl_stmt|;
DECL|typedef|ngx_http_listen_conf_t
block|}
name|ngx_http_listen_conf_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon2b2065660208
typedef|typedef
struct|struct
block|{
DECL|member|addr
name|in_addr_t
name|addr
decl_stmt|;
DECL|member|port
name|in_port_t
name|port
decl_stmt|;
DECL|member|family
name|int
name|family
decl_stmt|;
DECL|member|file_name
name|u_char
modifier|*
name|file_name
decl_stmt|;
DECL|member|line
name|ngx_uint_t
name|line
decl_stmt|;
DECL|member|conf
name|ngx_http_listen_conf_t
name|conf
decl_stmt|;
DECL|typedef|ngx_http_listen_t
block|}
name|ngx_http_listen_t
typedef|;
end_typedef

begin_typedef
DECL|enum|__anon2b2065660303
typedef|typedef
enum|enum
block|{
DECL|enumerator|NGX_HTTP_POST_READ_PHASE
name|NGX_HTTP_POST_READ_PHASE
init|=
literal|0
block|,
DECL|enumerator|NGX_HTTP_SERVER_REWRITE_PHASE
name|NGX_HTTP_SERVER_REWRITE_PHASE
block|,
DECL|enumerator|NGX_HTTP_FIND_CONFIG_PHASE
name|NGX_HTTP_FIND_CONFIG_PHASE
block|,
DECL|enumerator|NGX_HTTP_REWRITE_PHASE
name|NGX_HTTP_REWRITE_PHASE
block|,
DECL|enumerator|NGX_HTTP_POST_REWRITE_PHASE
name|NGX_HTTP_POST_REWRITE_PHASE
block|,
DECL|enumerator|NGX_HTTP_PREACCESS_PHASE
name|NGX_HTTP_PREACCESS_PHASE
block|,
DECL|enumerator|NGX_HTTP_ACCESS_PHASE
name|NGX_HTTP_ACCESS_PHASE
block|,
DECL|enumerator|NGX_HTTP_POST_ACCESS_PHASE
name|NGX_HTTP_POST_ACCESS_PHASE
block|,
DECL|enumerator|NGX_HTTP_CONTENT_PHASE
name|NGX_HTTP_CONTENT_PHASE
block|,
DECL|enumerator|NGX_HTTP_LOG_PHASE
name|NGX_HTTP_LOG_PHASE
DECL|typedef|ngx_http_phases
block|}
name|ngx_http_phases
typedef|;
end_typedef

begin_typedef
DECL|typedef|ngx_http_phase_handler_t
typedef|typedef
name|struct
name|ngx_http_phase_handler_s
name|ngx_http_phase_handler_t
typedef|;
end_typedef

begin_typedef
DECL|typedef|ngx_http_phase_handler_pt
typedef|typedef
name|ngx_int_t
function_decl|(
modifier|*
name|ngx_http_phase_handler_pt
function_decl|)
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_phase_handler_t
modifier|*
name|ph
parameter_list|)
function_decl|;
end_typedef

begin_struct
DECL|struct|ngx_http_phase_handler_s
struct|struct
name|ngx_http_phase_handler_s
block|{
DECL|member|checker
name|ngx_http_phase_handler_pt
name|checker
decl_stmt|;
DECL|member|handler
name|ngx_http_handler_pt
name|handler
decl_stmt|;
DECL|member|next
name|ngx_uint_t
name|next
decl_stmt|;
block|}
struct|;
end_struct

begin_typedef
DECL|struct|__anon2b2065660408
typedef|typedef
struct|struct
block|{
DECL|member|handlers
name|ngx_http_phase_handler_t
modifier|*
name|handlers
decl_stmt|;
DECL|member|server_rewrite_index
name|ngx_uint_t
name|server_rewrite_index
decl_stmt|;
DECL|member|location_rewrite_index
name|ngx_uint_t
name|location_rewrite_index
decl_stmt|;
DECL|typedef|ngx_http_phase_engine_t
block|}
name|ngx_http_phase_engine_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon2b2065660508
typedef|typedef
struct|struct
block|{
DECL|member|handlers
name|ngx_array_t
name|handlers
decl_stmt|;
DECL|typedef|ngx_http_phase_t
block|}
name|ngx_http_phase_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon2b2065660608
typedef|typedef
struct|struct
block|{
DECL|member|servers
name|ngx_array_t
name|servers
decl_stmt|;
comment|/* ngx_http_core_srv_conf_t */
DECL|member|phase_engine
name|ngx_http_phase_engine_t
name|phase_engine
decl_stmt|;
DECL|member|headers_in_hash
name|ngx_hash_t
name|headers_in_hash
decl_stmt|;
DECL|member|variables_hash
name|ngx_hash_t
name|variables_hash
decl_stmt|;
DECL|member|variables
name|ngx_array_t
name|variables
decl_stmt|;
comment|/* ngx_http_variable_t */
DECL|member|server_names_hash_max_size
name|ngx_uint_t
name|server_names_hash_max_size
decl_stmt|;
DECL|member|server_names_hash_bucket_size
name|ngx_uint_t
name|server_names_hash_bucket_size
decl_stmt|;
DECL|member|variables_hash_max_size
name|ngx_uint_t
name|variables_hash_max_size
decl_stmt|;
DECL|member|variables_hash_bucket_size
name|ngx_uint_t
name|variables_hash_bucket_size
decl_stmt|;
DECL|member|variables_keys
name|ngx_hash_keys_arrays_t
modifier|*
name|variables_keys
decl_stmt|;
DECL|member|phases
name|ngx_http_phase_t
name|phases
index|[
name|NGX_HTTP_LOG_PHASE
operator|+
literal|1
index|]
decl_stmt|;
DECL|typedef|ngx_http_core_main_conf_t
block|}
name|ngx_http_core_main_conf_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon2b2065660708
typedef|typedef
struct|struct
block|{
comment|/*      * array of the ngx_http_core_loc_conf_t *,      * used in the ngx_http_core_find_location() and in the merge phase      */
DECL|member|locations
name|ngx_array_t
name|locations
decl_stmt|;
DECL|member|regex_start
name|unsigned
name|regex_start
range|:
literal|15
decl_stmt|;
DECL|member|named_start
name|unsigned
name|named_start
range|:
literal|15
decl_stmt|;
comment|/* array of the ngx_http_listen_t, "listen" directive */
DECL|member|listen
name|ngx_array_t
name|listen
decl_stmt|;
comment|/* array of the ngx_http_server_name_t, "server_name" directive */
DECL|member|server_names
name|ngx_array_t
name|server_names
decl_stmt|;
comment|/* server ctx */
DECL|member|ctx
name|ngx_http_conf_ctx_t
modifier|*
name|ctx
decl_stmt|;
DECL|member|server_name
name|ngx_str_t
name|server_name
decl_stmt|;
DECL|member|connection_pool_size
name|size_t
name|connection_pool_size
decl_stmt|;
DECL|member|request_pool_size
name|size_t
name|request_pool_size
decl_stmt|;
DECL|member|client_header_buffer_size
name|size_t
name|client_header_buffer_size
decl_stmt|;
DECL|member|large_client_header_buffers
name|ngx_bufs_t
name|large_client_header_buffers
decl_stmt|;
DECL|member|client_header_timeout
name|ngx_msec_t
name|client_header_timeout
decl_stmt|;
DECL|member|optimize_server_names
name|ngx_flag_t
name|optimize_server_names
decl_stmt|;
DECL|member|ignore_invalid_headers
name|ngx_flag_t
name|ignore_invalid_headers
decl_stmt|;
DECL|member|merge_slashes
name|ngx_flag_t
name|merge_slashes
decl_stmt|;
DECL|typedef|ngx_http_core_srv_conf_t
block|}
name|ngx_http_core_srv_conf_t
typedef|;
end_typedef

begin_comment
comment|/* list of structures to find core_srv_conf quickly at run time */
end_comment

begin_typedef
DECL|struct|__anon2b2065660808
typedef|typedef
struct|struct
block|{
DECL|member|addr
name|in_addr_t
name|addr
decl_stmt|;
comment|/* the default server configuration for this address:port */
DECL|member|core_srv_conf
name|ngx_http_core_srv_conf_t
modifier|*
name|core_srv_conf
decl_stmt|;
DECL|member|virtual_names
name|ngx_http_virtual_names_t
modifier|*
name|virtual_names
decl_stmt|;
DECL|typedef|ngx_http_in_addr_t
block|}
name|ngx_http_in_addr_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon2b2065660908
typedef|typedef
struct|struct
block|{
DECL|member|port
name|in_port_t
name|port
decl_stmt|;
DECL|member|port_text
name|ngx_str_t
name|port_text
decl_stmt|;
DECL|member|addrs
name|ngx_http_in_addr_t
modifier|*
name|addrs
decl_stmt|;
DECL|member|naddrs
name|ngx_uint_t
name|naddrs
decl_stmt|;
DECL|typedef|ngx_http_in_port_t
block|}
name|ngx_http_in_port_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon2b2065660a08
typedef|typedef
struct|struct
block|{
DECL|member|port
name|in_port_t
name|port
decl_stmt|;
DECL|member|addrs
name|ngx_array_t
name|addrs
decl_stmt|;
comment|/* array of ngx_http_conf_in_addr_t */
DECL|typedef|ngx_http_conf_in_port_t
block|}
name|ngx_http_conf_in_port_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon2b2065660b08
typedef|typedef
struct|struct
block|{
DECL|member|addr
name|in_addr_t
name|addr
decl_stmt|;
DECL|member|hash
name|ngx_hash_t
name|hash
decl_stmt|;
DECL|member|wc_head
name|ngx_hash_wildcard_t
modifier|*
name|wc_head
decl_stmt|;
DECL|member|wc_tail
name|ngx_hash_wildcard_t
modifier|*
name|wc_tail
decl_stmt|;
DECL|member|names
name|ngx_array_t
name|names
decl_stmt|;
comment|/* array of ngx_http_server_name_t */
if|#
directive|if
operator|(
name|NGX_PCRE
operator|)
DECL|member|nregex
name|ngx_uint_t
name|nregex
decl_stmt|;
DECL|member|regex
name|ngx_http_server_name_t
modifier|*
name|regex
decl_stmt|;
endif|#
directive|endif
comment|/* the default server configuration for this address:port */
DECL|member|core_srv_conf
name|ngx_http_core_srv_conf_t
modifier|*
name|core_srv_conf
decl_stmt|;
DECL|member|default_server
name|unsigned
name|default_server
range|:
literal|1
decl_stmt|;
DECL|member|bind
name|unsigned
name|bind
range|:
literal|1
decl_stmt|;
DECL|member|listen_conf
name|ngx_http_listen_conf_t
modifier|*
name|listen_conf
decl_stmt|;
DECL|typedef|ngx_http_conf_in_addr_t
block|}
name|ngx_http_conf_in_addr_t
typedef|;
end_typedef

begin_struct
DECL|struct|ngx_http_server_name_s
struct|struct
name|ngx_http_server_name_s
block|{
if|#
directive|if
operator|(
name|NGX_PCRE
operator|)
DECL|member|regex
name|ngx_regex_t
modifier|*
name|regex
decl_stmt|;
endif|#
directive|endif
DECL|member|core_srv_conf
name|ngx_http_core_srv_conf_t
modifier|*
name|core_srv_conf
decl_stmt|;
comment|/* virtual name server conf */
DECL|member|name
name|ngx_str_t
name|name
decl_stmt|;
block|}
struct|;
end_struct

begin_typedef
DECL|struct|__anon2b2065660c08
typedef|typedef
struct|struct
block|{
DECL|member|status
name|ngx_int_t
name|status
decl_stmt|;
DECL|member|overwrite
name|ngx_int_t
name|overwrite
decl_stmt|;
DECL|member|uri
name|ngx_str_t
name|uri
decl_stmt|;
DECL|member|args
name|ngx_str_t
name|args
decl_stmt|;
DECL|member|uri_lengths
name|ngx_array_t
modifier|*
name|uri_lengths
decl_stmt|;
DECL|member|uri_values
name|ngx_array_t
modifier|*
name|uri_values
decl_stmt|;
DECL|typedef|ngx_http_err_page_t
block|}
name|ngx_http_err_page_t
typedef|;
end_typedef

begin_typedef
DECL|typedef|ngx_http_core_loc_conf_t
typedef|typedef
name|struct
name|ngx_http_core_loc_conf_s
name|ngx_http_core_loc_conf_t
typedef|;
end_typedef

begin_struct
DECL|struct|ngx_http_core_loc_conf_s
struct|struct
name|ngx_http_core_loc_conf_s
block|{
DECL|member|name
name|ngx_str_t
name|name
decl_stmt|;
comment|/* location name */
if|#
directive|if
operator|(
name|NGX_PCRE
operator|)
DECL|member|regex
name|ngx_regex_t
modifier|*
name|regex
decl_stmt|;
endif|#
directive|endif
DECL|member|regex_start
name|unsigned
name|regex_start
range|:
literal|15
decl_stmt|;
DECL|member|noname
name|unsigned
name|noname
range|:
literal|1
decl_stmt|;
comment|/* "if () {}" block or limit_except */
DECL|member|named
name|unsigned
name|named
range|:
literal|1
decl_stmt|;
DECL|member|exact_match
name|unsigned
name|exact_match
range|:
literal|1
decl_stmt|;
DECL|member|noregex
name|unsigned
name|noregex
range|:
literal|1
decl_stmt|;
DECL|member|auto_redirect
name|unsigned
name|auto_redirect
range|:
literal|1
decl_stmt|;
DECL|member|alias
name|unsigned
name|alias
range|:
literal|1
decl_stmt|;
comment|/* array of inclusive ngx_http_core_loc_conf_t */
DECL|member|locations
name|ngx_array_t
modifier|*
name|locations
decl_stmt|;
comment|/* pointer to the modules' loc_conf */
DECL|member|loc_conf
name|void
modifier|*
modifier|*
name|loc_conf
decl_stmt|;
DECL|member|limit_except
name|uint32_t
name|limit_except
decl_stmt|;
DECL|member|limit_except_loc_conf
name|void
modifier|*
modifier|*
name|limit_except_loc_conf
decl_stmt|;
DECL|member|handler
name|ngx_http_handler_pt
name|handler
decl_stmt|;
DECL|member|root
name|ngx_str_t
name|root
decl_stmt|;
comment|/* root, alias */
DECL|member|post_action
name|ngx_str_t
name|post_action
decl_stmt|;
DECL|member|root_lengths
name|ngx_array_t
modifier|*
name|root_lengths
decl_stmt|;
DECL|member|root_values
name|ngx_array_t
modifier|*
name|root_values
decl_stmt|;
DECL|member|types
name|ngx_array_t
modifier|*
name|types
decl_stmt|;
DECL|member|types_hash
name|ngx_hash_t
name|types_hash
decl_stmt|;
DECL|member|default_type
name|ngx_str_t
name|default_type
decl_stmt|;
DECL|member|client_max_body_size
name|off_t
name|client_max_body_size
decl_stmt|;
comment|/* client_max_body_size */
DECL|member|client_body_buffer_size
name|size_t
name|client_body_buffer_size
decl_stmt|;
comment|/* client_body_buffer_size */
DECL|member|send_lowat
name|size_t
name|send_lowat
decl_stmt|;
comment|/* send_lowat */
DECL|member|postpone_output
name|size_t
name|postpone_output
decl_stmt|;
comment|/* postpone_output */
DECL|member|limit_rate
name|size_t
name|limit_rate
decl_stmt|;
comment|/* limit_rate */
DECL|member|sendfile_max_chunk
name|size_t
name|sendfile_max_chunk
decl_stmt|;
comment|/* sendfile_max_chunk */
DECL|member|client_body_timeout
name|ngx_msec_t
name|client_body_timeout
decl_stmt|;
comment|/* client_body_timeout */
DECL|member|send_timeout
name|ngx_msec_t
name|send_timeout
decl_stmt|;
comment|/* send_timeout */
DECL|member|keepalive_timeout
name|ngx_msec_t
name|keepalive_timeout
decl_stmt|;
comment|/* keepalive_timeout */
DECL|member|lingering_time
name|ngx_msec_t
name|lingering_time
decl_stmt|;
comment|/* lingering_time */
DECL|member|lingering_timeout
name|ngx_msec_t
name|lingering_timeout
decl_stmt|;
comment|/* lingering_timeout */
DECL|member|resolver_timeout
name|ngx_msec_t
name|resolver_timeout
decl_stmt|;
comment|/* resolver_timeout */
DECL|member|resolver
name|ngx_resolver_t
modifier|*
name|resolver
decl_stmt|;
comment|/* resolver */
DECL|member|keepalive_header
name|time_t
name|keepalive_header
decl_stmt|;
comment|/* keepalive_timeout */
DECL|member|satisfy
name|ngx_uint_t
name|satisfy
decl_stmt|;
comment|/* satisfy */
DECL|member|internal
name|ngx_flag_t
name|internal
decl_stmt|;
comment|/* internal */
DECL|member|client_body_in_file_only
name|ngx_flag_t
name|client_body_in_file_only
decl_stmt|;
comment|/* client_body_in_file_only */
DECL|member|sendfile
name|ngx_flag_t
name|sendfile
decl_stmt|;
comment|/* sendfile */
DECL|member|tcp_nopush
name|ngx_flag_t
name|tcp_nopush
decl_stmt|;
comment|/* tcp_nopush */
DECL|member|tcp_nodelay
name|ngx_flag_t
name|tcp_nodelay
decl_stmt|;
comment|/* tcp_nodelay */
DECL|member|reset_timedout_connection
name|ngx_flag_t
name|reset_timedout_connection
decl_stmt|;
comment|/* reset_timedout_connection */
DECL|member|server_name_in_redirect
name|ngx_flag_t
name|server_name_in_redirect
decl_stmt|;
comment|/* server_name_in_redirect */
DECL|member|port_in_redirect
name|ngx_flag_t
name|port_in_redirect
decl_stmt|;
comment|/* port_in_redirect */
DECL|member|msie_padding
name|ngx_flag_t
name|msie_padding
decl_stmt|;
comment|/* msie_padding */
DECL|member|msie_refresh
name|ngx_flag_t
name|msie_refresh
decl_stmt|;
comment|/* msie_refresh */
DECL|member|log_not_found
name|ngx_flag_t
name|log_not_found
decl_stmt|;
comment|/* log_not_found */
DECL|member|recursive_error_pages
name|ngx_flag_t
name|recursive_error_pages
decl_stmt|;
comment|/* recursive_error_pages */
DECL|member|server_tokens
name|ngx_flag_t
name|server_tokens
decl_stmt|;
comment|/* server_tokens */
if|#
directive|if
operator|(
name|NGX_HTTP_GZIP
operator|)
DECL|member|gzip_vary
name|ngx_flag_t
name|gzip_vary
decl_stmt|;
comment|/* gzip_vary */
DECL|member|gzip_http_version
name|ngx_uint_t
name|gzip_http_version
decl_stmt|;
comment|/* gzip_http_version */
DECL|member|gzip_proxied
name|ngx_uint_t
name|gzip_proxied
decl_stmt|;
comment|/* gzip_proxied */
if|#
directive|if
operator|(
name|NGX_PCRE
operator|)
DECL|member|gzip_disable
name|ngx_array_t
modifier|*
name|gzip_disable
decl_stmt|;
comment|/* gzip_disable */
endif|#
directive|endif
endif|#
directive|endif
DECL|member|error_pages
name|ngx_array_t
modifier|*
name|error_pages
decl_stmt|;
comment|/* error_page */
DECL|member|client_body_temp_path
name|ngx_path_t
modifier|*
name|client_body_temp_path
decl_stmt|;
comment|/* client_body_temp_path */
DECL|member|open_file_cache
name|ngx_open_file_cache_t
modifier|*
name|open_file_cache
decl_stmt|;
DECL|member|open_file_cache_valid
name|time_t
name|open_file_cache_valid
decl_stmt|;
DECL|member|open_file_cache_min_uses
name|ngx_uint_t
name|open_file_cache_min_uses
decl_stmt|;
DECL|member|open_file_cache_errors
name|ngx_flag_t
name|open_file_cache_errors
decl_stmt|;
DECL|member|open_file_cache_events
name|ngx_flag_t
name|open_file_cache_events
decl_stmt|;
DECL|member|err_log
name|ngx_log_t
modifier|*
name|err_log
decl_stmt|;
DECL|member|types_hash_max_size
name|ngx_uint_t
name|types_hash_max_size
decl_stmt|;
DECL|member|types_hash_bucket_size
name|ngx_uint_t
name|types_hash_bucket_size
decl_stmt|;
if|#
directive|if
literal|0
block|ngx_http_core_loc_conf_t  *prev_location;
endif|#
directive|endif
block|}
struct|;
end_struct

begin_function_decl
name|void
name|ngx_http_core_run_phases
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_int_t
name|ngx_http_core_generic_phase
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_phase_handler_t
modifier|*
name|ph
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_int_t
name|ngx_http_core_find_config_phase
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_phase_handler_t
modifier|*
name|ph
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_int_t
name|ngx_http_core_post_rewrite_phase
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_phase_handler_t
modifier|*
name|ph
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_int_t
name|ngx_http_core_access_phase
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_phase_handler_t
modifier|*
name|ph
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_int_t
name|ngx_http_core_post_access_phase
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_phase_handler_t
modifier|*
name|ph
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_int_t
name|ngx_http_core_content_phase
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_phase_handler_t
modifier|*
name|ph
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_int_t
name|ngx_http_set_content_type
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_int_t
name|ngx_http_set_exten
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|u_char
modifier|*
name|ngx_http_map_uri_to_path
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_str_t
modifier|*
name|name
parameter_list|,
name|size_t
modifier|*
name|root_length
parameter_list|,
name|size_t
name|reserved
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_int_t
name|ngx_http_auth_basic_user
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_int_t
name|ngx_http_server_addr
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_str_t
modifier|*
name|s
parameter_list|)
function_decl|;
end_function_decl

begin_if
if|#
directive|if
operator|(
name|NGX_HTTP_GZIP
operator|)
end_if

begin_function_decl
name|ngx_int_t
name|ngx_http_gzip_ok
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_function_decl
name|ngx_int_t
name|ngx_http_subrequest
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_str_t
modifier|*
name|uri
parameter_list|,
name|ngx_str_t
modifier|*
name|args
parameter_list|,
name|ngx_http_request_t
modifier|*
modifier|*
name|sr
parameter_list|,
name|ngx_http_post_subrequest_t
modifier|*
name|psr
parameter_list|,
name|ngx_uint_t
name|flags
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_int_t
name|ngx_http_internal_redirect
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_str_t
modifier|*
name|uri
parameter_list|,
name|ngx_str_t
modifier|*
name|args
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_int_t
name|ngx_http_named_location
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_str_t
modifier|*
name|name
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_http_cleanup_t
modifier|*
name|ngx_http_cleanup_add
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|size_t
name|size
parameter_list|)
function_decl|;
end_function_decl

begin_typedef
DECL|typedef|ngx_http_output_header_filter_pt
typedef|typedef
name|ngx_int_t
function_decl|(
modifier|*
name|ngx_http_output_header_filter_pt
function_decl|)
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
function_decl|;
end_typedef

begin_typedef
DECL|typedef|ngx_http_output_body_filter_pt
typedef|typedef
name|ngx_int_t
function_decl|(
modifier|*
name|ngx_http_output_body_filter_pt
function_decl|)
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_chain_t
modifier|*
name|chain
parameter_list|)
function_decl|;
end_typedef

begin_function_decl
name|ngx_int_t
name|ngx_http_output_filter
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_chain_t
modifier|*
name|chain
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_int_t
name|ngx_http_write_filter
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_chain_t
modifier|*
name|chain
parameter_list|)
function_decl|;
end_function_decl

begin_decl_stmt
specifier|extern
name|ngx_module_t
name|ngx_http_core_module
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|ngx_uint_t
name|ngx_http_max_module
decl_stmt|;
end_decl_stmt

begin_define
DECL|macro|ngx_http_clear_content_length (r)
define|#
directive|define
name|ngx_http_clear_content_length
parameter_list|(
name|r
parameter_list|)
define|\                                                                               \
value|r->headers_out.content_length_n = -1;                                     \     if (r->headers_out.content_length) {                                      \         r->headers_out.content_length->hash = 0;                              \         r->headers_out.content_length = NULL;                                 \     }
end_define

begin_expr_stmt
unit|\
DECL|macro|ngx_http_clear_accept_ranges (r)
operator|#
name|define
name|ngx_http_clear_accept_ranges
argument_list|(
argument|r
argument_list|)
expr|\                                                                               \
name|r
operator|->
name|allow_ranges
operator|=
literal|0
expr_stmt|;
end_expr_stmt

begin_if_stmt
unit|\
if|if
condition|(
name|r
operator|->
name|headers_out
operator|.
name|accept_ranges
condition|)
block|{
block_content|\
name|r
operator|->
name|headers_out
operator|.
name|accept_ranges
operator|->
name|hash
operator|=
literal|0
expr_stmt|;
block_content|\
name|r
operator|->
name|headers_out
operator|.
name|accept_ranges
operator|=
name|NULL
expr_stmt|;
block_content|\
block|}
end_if_stmt

begin_define
DECL|macro|ngx_http_clear_last_modified (r)
define|#
directive|define
name|ngx_http_clear_last_modified
parameter_list|(
name|r
parameter_list|)
define|\                                                                               \
value|r->headers_out.last_modified_time = -1;                                   \     if (r->headers_out.last_modified) {                                       \         r->headers_out.last_modified->hash = 0;                               \         r->headers_out.last_modified = NULL;                                  \     }
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* _NGX_HTTP_CORE_H_INCLUDED_ */
end_comment

end_unit

