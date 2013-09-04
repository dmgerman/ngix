begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (C) Igor Sysoev  * Copyright (C) Nginx, Inc.  */
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
file|<ngx_event.h>
end_include

begin_include
include|#
directive|include
file|<ngx_event_connect.h>
end_include

begin_function
name|ngx_int_t
DECL|function|ngx_event_connect_peer (ngx_peer_connection_t * pc)
name|ngx_event_connect_peer
parameter_list|(
name|ngx_peer_connection_t
modifier|*
name|pc
parameter_list|)
block|{
name|int
name|rc
decl_stmt|;
name|ngx_int_t
name|event
decl_stmt|;
name|ngx_err_t
name|err
decl_stmt|;
name|ngx_uint_t
name|level
decl_stmt|;
name|ngx_socket_t
name|s
decl_stmt|;
name|ngx_event_t
modifier|*
name|rev
decl_stmt|,
modifier|*
name|wev
decl_stmt|;
name|ngx_connection_t
modifier|*
name|c
decl_stmt|;
name|rc
operator|=
name|pc
operator|->
name|get
argument_list|(
name|pc
argument_list|,
name|pc
operator|->
name|data
argument_list|)
expr_stmt|;
if|if
condition|(
name|rc
operator|!=
name|NGX_OK
condition|)
block|{
return|return
name|rc
return|;
block|}
name|s
operator|=
name|ngx_socket
argument_list|(
name|pc
operator|->
name|sockaddr
operator|->
name|sa_family
argument_list|,
name|SOCK_STREAM
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|pc
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"socket %d"
argument_list|,
name|s
argument_list|)
expr_stmt|;
if|if
condition|(
name|s
operator|==
operator|(
name|ngx_socket_t
operator|)
operator|-
literal|1
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|pc
operator|->
name|log
argument_list|,
name|ngx_socket_errno
argument_list|,
name|ngx_socket_n
literal|" failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
name|c
operator|=
name|ngx_get_connection
argument_list|(
name|s
argument_list|,
name|pc
operator|->
name|log
argument_list|)
expr_stmt|;
if|if
condition|(
name|c
operator|==
name|NULL
condition|)
block|{
if|if
condition|(
name|ngx_close_socket
argument_list|(
name|s
argument_list|)
operator|==
operator|-
literal|1
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|pc
operator|->
name|log
argument_list|,
name|ngx_socket_errno
argument_list|,
name|ngx_close_socket_n
literal|"failed"
argument_list|)
expr_stmt|;
block|}
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|pc
operator|->
name|rcvbuf
condition|)
block|{
if|if
condition|(
name|setsockopt
argument_list|(
name|s
argument_list|,
name|SOL_SOCKET
argument_list|,
name|SO_RCVBUF
argument_list|,
operator|(
specifier|const
name|void
operator|*
operator|)
operator|&
name|pc
operator|->
name|rcvbuf
argument_list|,
sizeof|sizeof
argument_list|(
name|int
argument_list|)
argument_list|)
operator|==
operator|-
literal|1
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|pc
operator|->
name|log
argument_list|,
name|ngx_socket_errno
argument_list|,
literal|"setsockopt(SO_RCVBUF) failed"
argument_list|)
expr_stmt|;
goto|goto
name|failed
goto|;
block|}
block|}
if|if
condition|(
name|ngx_nonblocking
argument_list|(
name|s
argument_list|)
operator|==
operator|-
literal|1
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|pc
operator|->
name|log
argument_list|,
name|ngx_socket_errno
argument_list|,
name|ngx_nonblocking_n
literal|" failed"
argument_list|)
expr_stmt|;
goto|goto
name|failed
goto|;
block|}
if|if
condition|(
name|pc
operator|->
name|local
condition|)
block|{
if|if
condition|(
name|bind
argument_list|(
name|s
argument_list|,
name|pc
operator|->
name|local
operator|->
name|sockaddr
argument_list|,
name|pc
operator|->
name|local
operator|->
name|socklen
argument_list|)
operator|==
operator|-
literal|1
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_CRIT
argument_list|,
name|pc
operator|->
name|log
argument_list|,
name|ngx_socket_errno
argument_list|,
literal|"bind(%V) failed"
argument_list|,
operator|&
name|pc
operator|->
name|local
operator|->
name|name
argument_list|)
expr_stmt|;
goto|goto
name|failed
goto|;
block|}
block|}
name|c
operator|->
name|recv
operator|=
name|ngx_recv
expr_stmt|;
name|c
operator|->
name|send
operator|=
name|ngx_send
expr_stmt|;
name|c
operator|->
name|recv_chain
operator|=
name|ngx_recv_chain
expr_stmt|;
name|c
operator|->
name|send_chain
operator|=
name|ngx_send_chain
expr_stmt|;
name|c
operator|->
name|sendfile
operator|=
literal|1
expr_stmt|;
name|c
operator|->
name|log_error
operator|=
name|pc
operator|->
name|log_error
expr_stmt|;
if|if
condition|(
name|pc
operator|->
name|sockaddr
operator|->
name|sa_family
operator|==
name|AF_UNIX
condition|)
block|{
name|c
operator|->
name|tcp_nopush
operator|=
name|NGX_TCP_NOPUSH_DISABLED
expr_stmt|;
name|c
operator|->
name|tcp_nodelay
operator|=
name|NGX_TCP_NODELAY_DISABLED
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_SOLARIS
operator|)
comment|/* Solaris's sendfilev() supports AF_NCA, AF_INET, and AF_INET6 */
name|c
operator|->
name|sendfile
operator|=
literal|0
expr_stmt|;
endif|#
directive|endif
block|}
name|rev
operator|=
name|c
operator|->
name|read
expr_stmt|;
name|wev
operator|=
name|c
operator|->
name|write
expr_stmt|;
name|rev
operator|->
name|log
operator|=
name|pc
operator|->
name|log
expr_stmt|;
name|wev
operator|->
name|log
operator|=
name|pc
operator|->
name|log
expr_stmt|;
name|pc
operator|->
name|connection
operator|=
name|c
expr_stmt|;
name|c
operator|->
name|number
operator|=
name|ngx_atomic_fetch_add
argument_list|(
name|ngx_connection_counter
argument_list|,
literal|1
argument_list|)
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_THREADS
operator|)
comment|/* TODO: lock event when call completion handler */
name|rev
operator|->
name|lock
operator|=
name|pc
operator|->
name|lock
expr_stmt|;
name|wev
operator|->
name|lock
operator|=
name|pc
operator|->
name|lock
expr_stmt|;
name|rev
operator|->
name|own_lock
operator|=
operator|&
name|c
operator|->
name|lock
expr_stmt|;
name|wev
operator|->
name|own_lock
operator|=
operator|&
name|c
operator|->
name|lock
expr_stmt|;
endif|#
directive|endif
if|if
condition|(
name|ngx_add_conn
condition|)
block|{
if|if
condition|(
name|ngx_add_conn
argument_list|(
name|c
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
goto|goto
name|failed
goto|;
block|}
block|}
name|ngx_log_debug3
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|pc
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"connect to %V, fd:%d #%d"
argument_list|,
name|pc
operator|->
name|name
argument_list|,
name|s
argument_list|,
name|c
operator|->
name|number
argument_list|)
expr_stmt|;
name|rc
operator|=
name|connect
argument_list|(
name|s
argument_list|,
name|pc
operator|->
name|sockaddr
argument_list|,
name|pc
operator|->
name|socklen
argument_list|)
expr_stmt|;
if|if
condition|(
name|rc
operator|==
operator|-
literal|1
condition|)
block|{
name|err
operator|=
name|ngx_socket_errno
expr_stmt|;
if|if
condition|(
name|err
operator|!=
name|NGX_EINPROGRESS
if|#
directive|if
operator|(
name|NGX_WIN32
operator|)
comment|/* Winsock returns WSAEWOULDBLOCK (NGX_EAGAIN) */
operator|&&
name|err
operator|!=
name|NGX_EAGAIN
endif|#
directive|endif
condition|)
block|{
if|if
condition|(
name|err
operator|==
name|NGX_ECONNREFUSED
if|#
directive|if
operator|(
name|NGX_LINUX
operator|)
comment|/*                  * Linux returns EAGAIN instead of ECONNREFUSED                  * for unix sockets if listen queue is full                  */
operator|||
name|err
operator|==
name|NGX_EAGAIN
endif|#
directive|endif
operator|||
name|err
operator|==
name|NGX_ECONNRESET
operator|||
name|err
operator|==
name|NGX_ENETDOWN
operator|||
name|err
operator|==
name|NGX_ENETUNREACH
operator|||
name|err
operator|==
name|NGX_EHOSTDOWN
operator|||
name|err
operator|==
name|NGX_EHOSTUNREACH
condition|)
block|{
name|level
operator|=
name|NGX_LOG_ERR
expr_stmt|;
block|}
else|else
block|{
name|level
operator|=
name|NGX_LOG_CRIT
expr_stmt|;
block|}
name|ngx_log_error
argument_list|(
name|level
argument_list|,
name|c
operator|->
name|log
argument_list|,
name|err
argument_list|,
literal|"connect() to %V failed"
argument_list|,
name|pc
operator|->
name|name
argument_list|)
expr_stmt|;
name|ngx_close_connection
argument_list|(
name|c
argument_list|)
expr_stmt|;
name|pc
operator|->
name|connection
operator|=
name|NULL
expr_stmt|;
return|return
name|NGX_DECLINED
return|;
block|}
block|}
if|if
condition|(
name|ngx_add_conn
condition|)
block|{
if|if
condition|(
name|rc
operator|==
operator|-
literal|1
condition|)
block|{
comment|/* NGX_EINPROGRESS */
return|return
name|NGX_AGAIN
return|;
block|}
name|ngx_log_debug0
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|pc
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"connected"
argument_list|)
expr_stmt|;
name|wev
operator|->
name|ready
operator|=
literal|1
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
if|if
condition|(
name|ngx_event_flags
operator|&
name|NGX_USE_AIO_EVENT
condition|)
block|{
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|pc
operator|->
name|log
argument_list|,
name|ngx_socket_errno
argument_list|,
literal|"connect(): %d"
argument_list|,
name|rc
argument_list|)
expr_stmt|;
comment|/* aio, iocp */
if|if
condition|(
name|ngx_blocking
argument_list|(
name|s
argument_list|)
operator|==
operator|-
literal|1
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|pc
operator|->
name|log
argument_list|,
name|ngx_socket_errno
argument_list|,
name|ngx_blocking_n
literal|" failed"
argument_list|)
expr_stmt|;
goto|goto
name|failed
goto|;
block|}
comment|/*          * FreeBSD's aio allows to post an operation on non-connected socket.          * NT does not support it.          *          * TODO: check in Win32, etc. As workaround we can use NGX_ONESHOT_EVENT          */
name|rev
operator|->
name|ready
operator|=
literal|1
expr_stmt|;
name|wev
operator|->
name|ready
operator|=
literal|1
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
if|if
condition|(
name|ngx_event_flags
operator|&
name|NGX_USE_CLEAR_EVENT
condition|)
block|{
comment|/* kqueue */
name|event
operator|=
name|NGX_CLEAR_EVENT
expr_stmt|;
block|}
else|else
block|{
comment|/* select, poll, /dev/poll */
name|event
operator|=
name|NGX_LEVEL_EVENT
expr_stmt|;
block|}
if|if
condition|(
name|ngx_add_event
argument_list|(
name|rev
argument_list|,
name|NGX_READ_EVENT
argument_list|,
name|event
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
goto|goto
name|failed
goto|;
block|}
if|if
condition|(
name|rc
operator|==
operator|-
literal|1
condition|)
block|{
comment|/* NGX_EINPROGRESS */
if|if
condition|(
name|ngx_add_event
argument_list|(
name|wev
argument_list|,
name|NGX_WRITE_EVENT
argument_list|,
name|event
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
goto|goto
name|failed
goto|;
block|}
return|return
name|NGX_AGAIN
return|;
block|}
name|ngx_log_debug0
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|pc
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"connected"
argument_list|)
expr_stmt|;
name|wev
operator|->
name|ready
operator|=
literal|1
expr_stmt|;
return|return
name|NGX_OK
return|;
name|failed
label|:
name|ngx_close_connection
argument_list|(
name|c
argument_list|)
expr_stmt|;
name|pc
operator|->
name|connection
operator|=
name|NULL
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
end_function

begin_function
name|ngx_int_t
DECL|function|ngx_event_get_peer (ngx_peer_connection_t * pc,void * data)
name|ngx_event_get_peer
parameter_list|(
name|ngx_peer_connection_t
modifier|*
name|pc
parameter_list|,
name|void
modifier|*
name|data
parameter_list|)
block|{
return|return
name|NGX_OK
return|;
block|}
end_function

end_unit

