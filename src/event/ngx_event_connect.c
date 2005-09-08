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
file|<ngx_event.h>
end_include

begin_include
include|#
directive|include
file|<ngx_event_connect.h>
end_include

begin_define
DECL|macro|NGX_RESOLVER_BUFSIZE
define|#
directive|define
name|NGX_RESOLVER_BUFSIZE
value|8192
end_define

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
name|ngx_uint_t
name|instance
decl_stmt|,
name|level
decl_stmt|;
name|u_int
name|event
decl_stmt|;
name|time_t
name|now
decl_stmt|;
name|ngx_err_t
name|err
decl_stmt|;
name|ngx_peer_t
modifier|*
name|peer
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
name|ngx_event_conf_t
modifier|*
name|ecf
decl_stmt|;
name|now
operator|=
name|ngx_time
argument_list|()
expr_stmt|;
comment|/* ngx_lock_mutex(pc->peers->mutex); */
if|if
condition|(
name|pc
operator|->
name|peers
operator|->
name|last_cached
condition|)
block|{
comment|/* cached connection */
name|c
operator|=
name|pc
operator|->
name|peers
operator|->
name|cached
index|[
name|pc
operator|->
name|peers
operator|->
name|last_cached
index|]
expr_stmt|;
name|pc
operator|->
name|peers
operator|->
name|last_cached
operator|--
expr_stmt|;
comment|/* ngx_unlock_mutex(pc->peers->mutex); */
if|#
directive|if
operator|(
name|NGX_THREADS
operator|)
name|c
operator|->
name|read
operator|->
name|lock
operator|=
name|c
operator|->
name|read
operator|->
name|own_lock
expr_stmt|;
name|c
operator|->
name|write
operator|->
name|lock
operator|=
name|c
operator|->
name|write
operator|->
name|own_lock
expr_stmt|;
endif|#
directive|endif
name|pc
operator|->
name|connection
operator|=
name|c
expr_stmt|;
name|pc
operator|->
name|cached
operator|=
literal|1
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
name|pc
operator|->
name|cached
operator|=
literal|0
expr_stmt|;
name|pc
operator|->
name|connection
operator|=
name|NULL
expr_stmt|;
if|if
condition|(
name|pc
operator|->
name|peers
operator|->
name|number
operator|==
literal|1
condition|)
block|{
name|peer
operator|=
operator|&
name|pc
operator|->
name|peers
operator|->
name|peer
index|[
literal|0
index|]
expr_stmt|;
block|}
else|else
block|{
comment|/* there are several peers */
if|if
condition|(
name|pc
operator|->
name|tries
operator|==
name|pc
operator|->
name|peers
operator|->
name|number
condition|)
block|{
comment|/* it's a first try - get a current peer */
name|pc
operator|->
name|cur_peer
operator|=
name|pc
operator|->
name|peers
operator|->
name|current
expr_stmt|;
name|pc
operator|->
name|peers
operator|->
name|weight
operator|--
expr_stmt|;
if|if
condition|(
name|pc
operator|->
name|peers
operator|->
name|weight
operator|==
literal|0
condition|)
block|{
name|pc
operator|->
name|peers
operator|->
name|current
operator|++
expr_stmt|;
block|}
if|if
condition|(
name|pc
operator|->
name|peers
operator|->
name|current
operator|>=
name|pc
operator|->
name|peers
operator|->
name|number
condition|)
block|{
name|pc
operator|->
name|peers
operator|->
name|current
operator|=
literal|0
expr_stmt|;
block|}
if|if
condition|(
name|pc
operator|->
name|peers
operator|->
name|weight
operator|==
literal|0
condition|)
block|{
name|pc
operator|->
name|peers
operator|->
name|weight
operator|=
name|pc
operator|->
name|peers
operator|->
name|peer
index|[
name|pc
operator|->
name|peers
operator|->
name|current
index|]
operator|.
name|weight
expr_stmt|;
block|}
block|}
for|for
control|(
init|;
condition|;
control|)
block|{
name|peer
operator|=
operator|&
name|pc
operator|->
name|peers
operator|->
name|peer
index|[
name|pc
operator|->
name|cur_peer
index|]
expr_stmt|;
if|if
condition|(
name|peer
operator|->
name|fails
operator|<=
name|peer
operator|->
name|max_fails
condition|)
block|{
break|break;
block|}
if|if
condition|(
name|now
operator|-
name|peer
operator|->
name|accessed
operator|>
name|peer
operator|->
name|fail_timeout
condition|)
block|{
name|peer
operator|->
name|fails
operator|=
literal|0
expr_stmt|;
break|break;
block|}
name|pc
operator|->
name|cur_peer
operator|++
expr_stmt|;
if|if
condition|(
name|pc
operator|->
name|cur_peer
operator|>=
name|pc
operator|->
name|peers
operator|->
name|number
condition|)
block|{
name|pc
operator|->
name|cur_peer
operator|=
literal|0
expr_stmt|;
block|}
name|pc
operator|->
name|tries
operator|--
expr_stmt|;
if|if
condition|(
name|pc
operator|->
name|tries
operator|==
literal|0
condition|)
block|{
comment|/* ngx_unlock_mutex(pc->peers->mutex); */
return|return
name|NGX_ERROR
return|;
block|}
block|}
block|}
comment|/* ngx_unlock_mutex(pc->peers->mutex); */
name|s
operator|=
name|ngx_socket
argument_list|(
name|peer
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
name|ecf
operator|=
name|ngx_event_get_conf
argument_list|(
name|ngx_cycle
operator|->
name|conf_ctx
argument_list|,
name|ngx_event_core_module
argument_list|)
expr_stmt|;
comment|/* disable warning: Win32 SOCKET is u_int while UNIX socket is int */
if|if
condition|(
operator|(
name|ngx_uint_t
operator|)
name|s
operator|>=
name|ecf
operator|->
name|connections
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
literal|0
argument_list|,
literal|"socket() returned socket #%d while only %d "
literal|"connections was configured, closing the socket"
argument_list|,
name|s
argument_list|,
name|ecf
operator|->
name|connections
argument_list|)
expr_stmt|;
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
comment|/* TODO: sleep for some time */
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
literal|" failed"
argument_list|)
expr_stmt|;
block|}
return|return
name|NGX_ERROR
return|;
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
literal|" failed"
argument_list|)
expr_stmt|;
block|}
return|return
name|NGX_ERROR
return|;
block|}
if|#
directive|if
operator|(
name|NGX_WIN32
operator|)
comment|/*      * Winsock assignes a socket number divisible by 4      * so to find a connection we divide a socket number by 4.      */
if|if
condition|(
name|s
operator|%
literal|4
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_EMERG
argument_list|,
name|pc
operator|->
name|log
argument_list|,
literal|0
argument_list|,
name|ngx_socket_n
literal|" created socket %d, not divisible by 4"
argument_list|,
name|s
argument_list|)
expr_stmt|;
name|exit
argument_list|(
literal|1
argument_list|)
expr_stmt|;
block|}
name|c
operator|=
operator|&
name|ngx_cycle
operator|->
name|connections
index|[
name|s
operator|/
literal|4
index|]
expr_stmt|;
name|rev
operator|=
operator|&
name|ngx_cycle
operator|->
name|read_events
index|[
name|s
operator|/
literal|4
index|]
expr_stmt|;
name|wev
operator|=
operator|&
name|ngx_cycle
operator|->
name|write_events
index|[
name|s
operator|/
literal|4
index|]
expr_stmt|;
else|#
directive|else
name|c
operator|=
operator|&
name|ngx_cycle
operator|->
name|connections
index|[
name|s
index|]
expr_stmt|;
name|rev
operator|=
operator|&
name|ngx_cycle
operator|->
name|read_events
index|[
name|s
index|]
expr_stmt|;
name|wev
operator|=
operator|&
name|ngx_cycle
operator|->
name|write_events
index|[
name|s
index|]
expr_stmt|;
endif|#
directive|endif
name|instance
operator|=
name|rev
operator|->
name|instance
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_THREADS
operator|)
if|if
condition|(
operator|*
operator|(
operator|&
name|c
operator|->
name|lock
operator|)
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
literal|0
argument_list|,
literal|"spinlock in connect, fd:%d"
argument_list|,
name|s
argument_list|)
expr_stmt|;
name|ngx_spinlock
argument_list|(
operator|&
name|c
operator|->
name|lock
argument_list|,
literal|1000
argument_list|)
expr_stmt|;
name|ngx_unlock
argument_list|(
operator|&
name|c
operator|->
name|lock
argument_list|)
expr_stmt|;
block|}
endif|#
directive|endif
name|ngx_memzero
argument_list|(
name|c
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_connection_t
argument_list|)
argument_list|)
expr_stmt|;
name|ngx_memzero
argument_list|(
name|rev
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_event_t
argument_list|)
argument_list|)
expr_stmt|;
name|ngx_memzero
argument_list|(
name|wev
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_event_t
argument_list|)
argument_list|)
expr_stmt|;
name|rev
operator|->
name|instance
operator|=
operator|!
name|instance
expr_stmt|;
name|wev
operator|->
name|instance
operator|=
operator|!
name|instance
expr_stmt|;
name|rev
operator|->
name|index
operator|=
name|NGX_INVALID_INDEX
expr_stmt|;
name|wev
operator|->
name|index
operator|=
name|NGX_INVALID_INDEX
expr_stmt|;
name|rev
operator|->
name|data
operator|=
name|c
expr_stmt|;
name|wev
operator|->
name|data
operator|=
name|c
expr_stmt|;
name|c
operator|->
name|read
operator|=
name|rev
expr_stmt|;
name|c
operator|->
name|write
operator|=
name|wev
expr_stmt|;
name|wev
operator|->
name|write
operator|=
literal|1
expr_stmt|;
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
name|send_chain
operator|=
name|ngx_send_chain
expr_stmt|;
name|c
operator|->
name|log
operator|=
name|pc
operator|->
name|log
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
name|c
operator|->
name|fd
operator|=
name|s
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
name|peer
operator|->
name|sockaddr
operator|->
name|sa_family
operator|!=
name|AF_INET
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
block|}
name|pc
operator|->
name|connection
operator|=
name|c
expr_stmt|;
comment|/*      * TODO: MT: - atomic increment (x86: lock xadd)      *             or protection by critical section or mutex      *      * TODO: MP: - allocated in a shared memory      *           - atomic increment (x86: lock xadd)      *             or protection by critical section or mutex      */
name|c
operator|->
name|number
operator|=
name|ngx_atomic_inc
argument_list|(
name|ngx_connection_counter
argument_list|)
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_THREADS
operator|)
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
return|return
name|NGX_ERROR
return|;
block|}
block|}
name|ngx_log_debug2
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|pc
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"connect to %V, #%d"
argument_list|,
operator|&
name|peer
operator|->
name|name
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
name|peer
operator|->
name|sockaddr
argument_list|,
name|peer
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
comment|/* Winsock returns WSAEWOULDBLOCK (NGX_EAGAIN) */
if|if
condition|(
name|err
operator|!=
name|NGX_EINPROGRESS
operator|&&
name|err
operator|!=
name|NGX_EAGAIN
condition|)
block|{
if|if
condition|(
name|err
operator|==
name|NGX_ECONNREFUSED
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
operator|&
name|peer
operator|->
name|name
argument_list|)
expr_stmt|;
return|return
name|NGX_CONNECT_ERROR
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
return|return
name|NGX_ERROR
return|;
block|}
comment|/*          * FreeBSD's aio allows to post an operation on non-connected socket.          * NT does not support it.          *           * TODO: check in Win32, etc. As workaround we can use NGX_ONESHOT_EVENT          */
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
return|return
name|NGX_ERROR
return|;
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
return|return
name|NGX_ERROR
return|;
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
block|}
end_function

begin_function
name|void
DECL|function|ngx_event_connect_peer_failed (ngx_peer_connection_t * pc,ngx_uint_t down)
name|ngx_event_connect_peer_failed
parameter_list|(
name|ngx_peer_connection_t
modifier|*
name|pc
parameter_list|,
name|ngx_uint_t
name|down
parameter_list|)
block|{
name|time_t
name|now
decl_stmt|;
if|if
condition|(
name|down
condition|)
block|{
name|now
operator|=
name|ngx_time
argument_list|()
expr_stmt|;
comment|/* ngx_lock_mutex(pc->peers->mutex); */
name|pc
operator|->
name|peers
operator|->
name|peer
index|[
name|pc
operator|->
name|cur_peer
index|]
operator|.
name|fails
operator|++
expr_stmt|;
name|pc
operator|->
name|peers
operator|->
name|peer
index|[
name|pc
operator|->
name|cur_peer
index|]
operator|.
name|accessed
operator|=
name|now
expr_stmt|;
comment|/* ngx_unlock_mutex(pc->peers->mutex); */
block|}
name|pc
operator|->
name|cur_peer
operator|++
expr_stmt|;
if|if
condition|(
name|pc
operator|->
name|cur_peer
operator|>=
name|pc
operator|->
name|peers
operator|->
name|number
condition|)
block|{
name|pc
operator|->
name|cur_peer
operator|=
literal|0
expr_stmt|;
block|}
name|pc
operator|->
name|tries
operator|--
expr_stmt|;
block|}
end_function

end_unit

