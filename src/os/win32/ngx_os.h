begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (C) Igor Sysoev  */
end_comment

begin_ifndef
ifndef|#
directive|ifndef
name|_NGX_OS_H_INCLUDED_
end_ifndef

begin_define
DECL|macro|_NGX_OS_H_INCLUDED_
define|#
directive|define
name|_NGX_OS_H_INCLUDED_
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

begin_define
DECL|macro|NGX_IO_SENDFILE
define|#
directive|define
name|NGX_IO_SENDFILE
value|1
end_define

begin_define
DECL|macro|NGX_IO_ZEROCOPY
define|#
directive|define
name|NGX_IO_ZEROCOPY
value|2
end_define

begin_if
if|#
directive|if
operator|(
name|HAVE_SENDFILE
operator|)
end_if

begin_define
DECL|macro|NGX_HAVE_SENDFILE
define|#
directive|define
name|NGX_HAVE_SENDFILE
value|NGX_IO_SENDFILE
end_define

begin_else
else|#
directive|else
end_else

begin_define
DECL|macro|NGX_HAVE_SENDFILE
define|#
directive|define
name|NGX_HAVE_SENDFILE
value|0
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_if
if|#
directive|if
operator|(
name|HAVE_ZEROCOPY
operator|)
end_if

begin_define
DECL|macro|NGX_HAVE_ZEROCOPY
define|#
directive|define
name|NGX_HAVE_ZEROCOPY
value|NGX_IO_ZEROCOPY
end_define

begin_else
else|#
directive|else
end_else

begin_define
DECL|macro|NGX_HAVE_ZEROCOPY
define|#
directive|define
name|NGX_HAVE_ZEROCOPY
value|0
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_typedef
DECL|typedef|ngx_recv_pt
typedef|typedef
name|ssize_t
function_decl|(
modifier|*
name|ngx_recv_pt
function_decl|)
parameter_list|(
name|ngx_connection_t
modifier|*
name|c
parameter_list|,
name|u_char
modifier|*
name|buf
parameter_list|,
name|size_t
name|size
parameter_list|)
function_decl|;
end_typedef

begin_typedef
DECL|typedef|ngx_recv_chain_pt
typedef|typedef
name|ssize_t
function_decl|(
modifier|*
name|ngx_recv_chain_pt
function_decl|)
parameter_list|(
name|ngx_connection_t
modifier|*
name|c
parameter_list|,
name|ngx_chain_t
modifier|*
name|in
parameter_list|)
function_decl|;
end_typedef

begin_typedef
DECL|typedef|ngx_send_pt
typedef|typedef
name|ssize_t
function_decl|(
modifier|*
name|ngx_send_pt
function_decl|)
parameter_list|(
name|ngx_connection_t
modifier|*
name|c
parameter_list|,
name|u_char
modifier|*
name|buf
parameter_list|,
name|size_t
name|size
parameter_list|)
function_decl|;
end_typedef

begin_typedef
DECL|typedef|ngx_send_chain_pt
typedef|typedef
name|ngx_chain_t
modifier|*
function_decl|(
modifier|*
name|ngx_send_chain_pt
function_decl|)
parameter_list|(
name|ngx_connection_t
modifier|*
name|c
parameter_list|,
name|ngx_chain_t
modifier|*
name|in
parameter_list|,
name|off_t
name|limit
parameter_list|)
function_decl|;
end_typedef

begin_typedef
DECL|struct|__anon28a7dd5d0108
typedef|typedef
struct|struct
block|{
DECL|member|recv
name|ngx_recv_pt
name|recv
decl_stmt|;
DECL|member|recv_chain
name|ngx_recv_chain_pt
name|recv_chain
decl_stmt|;
DECL|member|send
name|ngx_send_pt
name|send
decl_stmt|;
DECL|member|send_chain
name|ngx_send_chain_pt
name|send_chain
decl_stmt|;
DECL|member|flags
name|ngx_uint_t
name|flags
decl_stmt|;
DECL|typedef|ngx_os_io_t
block|}
name|ngx_os_io_t
typedef|;
end_typedef

begin_function_decl
name|ngx_int_t
name|ngx_os_init
parameter_list|(
name|ngx_log_t
modifier|*
name|log
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|ngx_os_status
parameter_list|(
name|ngx_log_t
modifier|*
name|log
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ssize_t
name|ngx_wsarecv
parameter_list|(
name|ngx_connection_t
modifier|*
name|c
parameter_list|,
name|u_char
modifier|*
name|buf
parameter_list|,
name|size_t
name|size
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ssize_t
name|ngx_overlapped_wsarecv
parameter_list|(
name|ngx_connection_t
modifier|*
name|c
parameter_list|,
name|u_char
modifier|*
name|buf
parameter_list|,
name|size_t
name|size
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ssize_t
name|ngx_wsarecv_chain
parameter_list|(
name|ngx_connection_t
modifier|*
name|c
parameter_list|,
name|ngx_chain_t
modifier|*
name|chain
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_chain_t
modifier|*
name|ngx_wsasend_chain
parameter_list|(
name|ngx_connection_t
modifier|*
name|c
parameter_list|,
name|ngx_chain_t
modifier|*
name|in
parameter_list|,
name|off_t
name|limit
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_chain_t
modifier|*
name|ngx_overlapped_wsasend_chain
parameter_list|(
name|ngx_connection_t
modifier|*
name|c
parameter_list|,
name|ngx_chain_t
modifier|*
name|in
parameter_list|,
name|off_t
name|limit
parameter_list|)
function_decl|;
end_function_decl

begin_decl_stmt
specifier|extern
name|ngx_os_io_t
name|ngx_os_io
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|ngx_uint_t
name|ngx_ncpu
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|ngx_int_t
name|ngx_max_sockets
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|ngx_uint_t
name|ngx_inherited_nonblocking
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|ngx_uint_t
name|ngx_win32_version
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|ngx_fd_t
name|ngx_stderr_fileno
decl_stmt|;
end_decl_stmt

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* _NGX_OS_H_INCLUDED_ */
end_comment

end_unit

