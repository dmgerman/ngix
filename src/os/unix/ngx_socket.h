begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|_NGX_SOCKET_H_INCLUDED_
end_ifndef

begin_define
DECL|macro|_NGX_SOCKET_H_INCLUDED_
define|#
directive|define
name|_NGX_SOCKET_H_INCLUDED_
end_define

begin_include
include|#
directive|include
file|<ngx_config.h>
end_include

begin_typedef
DECL|typedef|ngx_socket_t
typedef|typedef
name|int
name|ngx_socket_t
typedef|;
end_typedef

begin_define
DECL|macro|ngx_init_sockets
define|#
directive|define
name|ngx_init_sockets
end_define

begin_define
DECL|macro|ngx_nonblocking (s)
define|#
directive|define
name|ngx_nonblocking
parameter_list|(
name|s
parameter_list|)
value|fcntl(s, F_SETFL, O_NONBLOCK)
end_define

begin_define
DECL|macro|ngx_nonblocking_n
define|#
directive|define
name|ngx_nonblocking_n
value|"fcntl (O_NONBLOCK)"
end_define

begin_define
DECL|macro|ngx_close_socket
define|#
directive|define
name|ngx_close_socket
value|close
end_define

begin_define
DECL|macro|ngx_close_socket_n
define|#
directive|define
name|ngx_close_socket_n
value|"close"
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* _NGX_SOCKET_H_INCLUDED_ */
end_comment

end_unit

