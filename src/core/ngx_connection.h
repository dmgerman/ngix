begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (C) Igor Sysoev  */
end_comment

begin_ifndef
ifndef|#
directive|ifndef
name|_NGX_CONNECTION_H_INCLUDED_
end_ifndef

begin_define
DECL|macro|_NGX_CONNECTION_H_INCLUDED_
define|#
directive|define
name|_NGX_CONNECTION_H_INCLUDED_
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

begin_typedef
DECL|struct|__anon27e766a20108
typedef|typedef
struct|struct
block|{
DECL|member|fd
name|ngx_socket_t
name|fd
decl_stmt|;
DECL|member|sockaddr
name|struct
name|sockaddr
modifier|*
name|sockaddr
decl_stmt|;
DECL|member|socklen
name|socklen_t
name|socklen
decl_stmt|;
comment|/* size of sockaddr */
DECL|member|addr
name|int
name|addr
decl_stmt|;
comment|/* offset to address in sockaddr */
DECL|member|addr_text_max_len
name|int
name|addr_text_max_len
decl_stmt|;
DECL|member|addr_text
name|ngx_str_t
name|addr_text
decl_stmt|;
DECL|member|family
name|int
name|family
decl_stmt|;
DECL|member|type
name|int
name|type
decl_stmt|;
DECL|member|protocol
name|int
name|protocol
decl_stmt|;
DECL|member|flags
name|int
name|flags
decl_stmt|;
comment|/* Winsock2 flags */
DECL|member|handler
name|void
function_decl|(
modifier|*
name|handler
function_decl|)
parameter_list|(
name|ngx_connection_t
modifier|*
name|c
parameter_list|)
function_decl|;
comment|/* handler of accepted                                                         connection */
DECL|member|ctx
name|void
modifier|*
name|ctx
decl_stmt|;
comment|/* ngx_http_conf_ctx_t, for example */
DECL|member|servers
name|void
modifier|*
name|servers
decl_stmt|;
comment|/* array of ngx_http_in_addr_t, for example */
DECL|member|log
name|ngx_log_t
modifier|*
name|log
decl_stmt|;
DECL|member|backlog
name|int
name|backlog
decl_stmt|;
DECL|member|pool_size
name|size_t
name|pool_size
decl_stmt|;
DECL|member|post_accept_buffer_size
name|size_t
name|post_accept_buffer_size
decl_stmt|;
comment|/* should be here because                                                   of the AcceptEx() preread */
DECL|member|post_accept_timeout
name|time_t
name|post_accept_timeout
decl_stmt|;
comment|/* should be here because                                                   of the deferred accept */
DECL|member|new
name|unsigned
name|new
range|:
literal|1
decl_stmt|;
DECL|member|remain
name|unsigned
name|remain
range|:
literal|1
decl_stmt|;
DECL|member|ignore
name|unsigned
name|ignore
range|:
literal|1
decl_stmt|;
DECL|member|bound
name|unsigned
name|bound
range|:
literal|1
decl_stmt|;
comment|/* already bound */
DECL|member|inherited
name|unsigned
name|inherited
range|:
literal|1
decl_stmt|;
comment|/* inherited from previous process */
DECL|member|nonblocking_accept
name|unsigned
name|nonblocking_accept
range|:
literal|1
decl_stmt|;
DECL|member|nonblocking
name|unsigned
name|nonblocking
range|:
literal|1
decl_stmt|;
if|#
directive|if
literal|0
block|unsigned          overlapped:1;
comment|/* Winsock2 overlapped */
endif|#
directive|endif
DECL|member|shared
name|unsigned
name|shared
range|:
literal|1
decl_stmt|;
comment|/* shared between threads or processes */
if|#
directive|if
operator|(
name|HAVE_DEFERRED_ACCEPT
operator|)
DECL|member|deferred_accept
name|unsigned
name|deferred_accept
range|:
literal|1
decl_stmt|;
endif|#
directive|endif
DECL|member|addr_ntop
name|unsigned
name|addr_ntop
range|:
literal|1
decl_stmt|;
DECL|typedef|ngx_listening_t
block|}
name|ngx_listening_t
typedef|;
end_typedef

begin_typedef
DECL|enum|__anon27e766a20203
typedef|typedef
enum|enum
block|{
DECL|enumerator|NGX_ERROR_CRIT
name|NGX_ERROR_CRIT
init|=
literal|0
block|,
DECL|enumerator|NGX_ERROR_ERR
name|NGX_ERROR_ERR
block|,
DECL|enumerator|NGX_ERROR_INFO
name|NGX_ERROR_INFO
block|,
DECL|enumerator|NGX_ERROR_IGNORE_ECONNRESET
name|NGX_ERROR_IGNORE_ECONNRESET
DECL|typedef|ngx_connection_log_error_e
block|}
name|ngx_connection_log_error_e
typedef|;
end_typedef

begin_typedef
DECL|enum|__anon27e766a20303
typedef|typedef
enum|enum
block|{
DECL|enumerator|NGX_TCP_NOPUSH_DISABLED
name|NGX_TCP_NOPUSH_DISABLED
init|=
operator|-
literal|1
block|,
DECL|enumerator|NGX_TCP_NOPUSH_UNSET
name|NGX_TCP_NOPUSH_UNSET
init|=
literal|0
block|,
DECL|enumerator|NGX_TCP_NOPUSH_SET
name|NGX_TCP_NOPUSH_SET
DECL|typedef|ngx_connection_tcp_nopush_e
block|}
name|ngx_connection_tcp_nopush_e
typedef|;
end_typedef

begin_struct
DECL|struct|ngx_connection_s
struct|struct
name|ngx_connection_s
block|{
DECL|member|data
name|void
modifier|*
name|data
decl_stmt|;
DECL|member|read
name|ngx_event_t
modifier|*
name|read
decl_stmt|;
DECL|member|write
name|ngx_event_t
modifier|*
name|write
decl_stmt|;
DECL|member|fd
name|ngx_socket_t
name|fd
decl_stmt|;
DECL|member|recv
name|ngx_recv_pt
name|recv
decl_stmt|;
DECL|member|send_chain
name|ngx_send_chain_pt
name|send_chain
decl_stmt|;
DECL|member|listening
name|ngx_listening_t
modifier|*
name|listening
decl_stmt|;
DECL|member|sent
name|off_t
name|sent
decl_stmt|;
DECL|member|ctx
name|void
modifier|*
name|ctx
decl_stmt|;
DECL|member|servers
name|void
modifier|*
name|servers
decl_stmt|;
DECL|member|log
name|ngx_log_t
modifier|*
name|log
decl_stmt|;
DECL|member|pool
name|ngx_pool_t
modifier|*
name|pool
decl_stmt|;
DECL|member|sockaddr
name|struct
name|sockaddr
modifier|*
name|sockaddr
decl_stmt|;
DECL|member|socklen
name|socklen_t
name|socklen
decl_stmt|;
DECL|member|addr_text
name|ngx_str_t
name|addr_text
decl_stmt|;
if|#
directive|if
operator|(
name|NGX_OPENSSL
operator|)
DECL|member|ssl
name|ngx_ssl_t
modifier|*
name|ssl
decl_stmt|;
endif|#
directive|endif
if|#
directive|if
operator|(
name|HAVE_IOCP
operator|)
DECL|member|local_sockaddr
name|struct
name|sockaddr
modifier|*
name|local_sockaddr
decl_stmt|;
DECL|member|local_socklen
name|socklen_t
name|local_socklen
decl_stmt|;
endif|#
directive|endif
DECL|member|buffer
name|ngx_buf_t
modifier|*
name|buffer
decl_stmt|;
DECL|member|number
name|ngx_uint_t
name|number
decl_stmt|;
DECL|member|log_error
name|unsigned
name|log_error
range|:
literal|2
decl_stmt|;
comment|/* ngx_connection_log_error_e */
DECL|member|buffered
name|unsigned
name|buffered
range|:
literal|1
decl_stmt|;
DECL|member|single_connection
name|unsigned
name|single_connection
range|:
literal|1
decl_stmt|;
DECL|member|unexpected_eof
name|unsigned
name|unexpected_eof
range|:
literal|1
decl_stmt|;
DECL|member|timedout
name|unsigned
name|timedout
range|:
literal|1
decl_stmt|;
DECL|member|sendfile
name|unsigned
name|sendfile
range|:
literal|1
decl_stmt|;
DECL|member|sndlowat
name|unsigned
name|sndlowat
range|:
literal|1
decl_stmt|;
DECL|member|tcp_nodelay
name|unsigned
name|tcp_nodelay
range|:
literal|1
decl_stmt|;
DECL|member|tcp_nopush
name|signed
name|tcp_nopush
range|:
literal|2
decl_stmt|;
if|#
directive|if
operator|(
name|HAVE_IOCP
operator|)
DECL|member|accept_context_updated
name|unsigned
name|accept_context_updated
range|:
literal|1
decl_stmt|;
endif|#
directive|endif
if|#
directive|if
operator|(
name|NGX_THREADS
operator|)
DECL|member|lock
name|ngx_atomic_t
name|lock
decl_stmt|;
endif|#
directive|endif
block|}
struct|;
end_struct

begin_ifndef
ifndef|#
directive|ifndef
name|ngx_ssl_set_nosendshut
end_ifndef

begin_define
DECL|macro|ngx_ssl_set_nosendshut (ssl)
define|#
directive|define
name|ngx_ssl_set_nosendshut
parameter_list|(
name|ssl
parameter_list|)
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_function_decl
name|ngx_listening_t
modifier|*
name|ngx_listening_inet_stream_socket
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|in_addr_t
name|addr
parameter_list|,
name|in_port_t
name|port
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_int_t
name|ngx_set_inherited_sockets
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_int_t
name|ngx_open_listening_sockets
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|ngx_close_listening_sockets
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|ngx_close_connection
parameter_list|(
name|ngx_connection_t
modifier|*
name|c
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_int_t
name|ngx_connection_error
parameter_list|(
name|ngx_connection_t
modifier|*
name|c
parameter_list|,
name|ngx_err_t
name|err
parameter_list|,
name|char
modifier|*
name|text
parameter_list|)
function_decl|;
end_function_decl

begin_decl_stmt
specifier|extern
name|ngx_os_io_t
name|ngx_io
decl_stmt|;
end_decl_stmt

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* _NGX_CONNECTION_H_INCLUDED_ */
end_comment

end_unit

