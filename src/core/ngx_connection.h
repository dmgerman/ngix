begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
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
file|<ngx_socket.h>
end_include

begin_include
include|#
directive|include
file|<ngx_log.h>
end_include

begin_include
include|#
directive|include
file|<ngx_hunk.h>
end_include

begin_include
include|#
directive|include
file|<ngx_alloc.h>
end_include

begin_include
include|#
directive|include
file|<ngx_string.h>
end_include

begin_include
include|#
directive|include
file|<ngx_server.h>
end_include

begin_typedef
DECL|typedef|ngx_connection_t
typedef|typedef
name|struct
name|ngx_connection_s
name|ngx_connection_t
typedef|;
end_typedef

begin_ifdef
ifdef|#
directive|ifdef
name|NGX_EVENT
end_ifdef

begin_include
include|#
directive|include
file|<ngx_event.h>
end_include

begin_endif
endif|#
directive|endif
end_endif

begin_struct
DECL|struct|ngx_connection_s
struct|struct
name|ngx_connection_s
block|{
DECL|member|fd
name|ngx_socket_t
name|fd
decl_stmt|;
DECL|member|data
name|void
modifier|*
name|data
decl_stmt|;
comment|/* STUB */
DECL|member|requests
name|ngx_array_t
modifier|*
name|requests
decl_stmt|;
DECL|member|requests_len
name|int
name|requests_len
decl_stmt|;
ifdef|#
directive|ifdef
name|NGX_EVENT
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
endif|#
directive|endif
DECL|member|sent
name|off_t
name|sent
decl_stmt|;
DECL|member|log
name|ngx_log_t
modifier|*
name|log
decl_stmt|;
DECL|member|handler
name|int
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
DECL|member|server
name|ngx_server_t
modifier|*
name|server
decl_stmt|;
DECL|member|servers
name|ngx_server_t
modifier|*
name|servers
decl_stmt|;
DECL|member|pool
name|ngx_pool_t
modifier|*
name|pool
decl_stmt|;
DECL|member|family
name|int
name|family
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
DECL|member|addr
name|size_t
name|addr
decl_stmt|;
DECL|member|addr_text
name|ngx_str_t
name|addr_text
decl_stmt|;
DECL|member|buffer
name|ngx_hunk_t
modifier|*
name|buffer
decl_stmt|;
DECL|member|post_accept_timeout
name|unsigned
name|int
name|post_accept_timeout
decl_stmt|;
DECL|member|unexpected_eof
name|unsigned
name|unexpected_eof
range|:
literal|1
decl_stmt|;
block|}
struct|;
end_struct

begin_if
if|#
directive|if
literal|0
end_if

begin_endif
unit|cached file     int      fd;       -2 unused, -1 closed (but read or mmaped),>=0 open     char    *name;      void    *buf;      addr if read or mmaped                        aiocb* if aio_read                        OVERLAPPED if TransmitFile or TransmitPackets                        NULL if sendfile      size_t   buf_size; for plain read     off_t    offset;   for plain read      size_t   size;     time_t   mod;     char    *last_mod; 'Sun, 17 Mar 2002 19:39:50 GMT'     char    *etag;     '"a6d08-1302-3c94f106"'     char    *len;      '4866'  EV_VNODE        should notify by some signal if diretory tree is changed                 or stat if aged>= N seconds (big enough)
endif|#
directive|endif
end_endif

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* _NGX_CONNECTION_H_INCLUDED_ */
end_comment

end_unit

