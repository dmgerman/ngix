begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
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
file|<nginx.h>
end_include

begin_function
DECL|function|ngx_event_accept (ngx_event_t * ev)
name|void
name|ngx_event_accept
parameter_list|(
name|ngx_event_t
modifier|*
name|ev
parameter_list|)
block|{
name|int
name|instance
decl_stmt|;
name|socklen_t
name|len
decl_stmt|;
name|struct
name|sockaddr
modifier|*
name|sa
decl_stmt|;
name|ngx_err_t
name|err
decl_stmt|;
name|ngx_pool_t
modifier|*
name|pool
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
decl_stmt|,
modifier|*
name|ls
decl_stmt|;
name|ngx_event_conf_t
modifier|*
name|ecf
decl_stmt|;
name|ecf
operator|=
name|ngx_event_get_conf
argument_list|(
name|ngx_event_core_module
argument_list|)
expr_stmt|;
name|ls
operator|=
name|ev
operator|->
name|data
expr_stmt|;
name|ngx_log_debug
argument_list|(
argument|ev->log
argument_list|,
literal|"ngx_event_accept: accept ready: %d"
argument|_                   ev->available
argument_list|)
empty_stmt|;
name|ev
operator|->
name|ready
operator|=
literal|0
expr_stmt|;
do|do
block|{
comment|/*          * Create the pool before accept() to avoid copy the sockaddr.          * Although accept() can fail it's an uncommon case          * and the pool can be got from the free pool list          */
name|pool
operator|=
name|ngx_create_pool
argument_list|(
name|ls
operator|->
name|listening
operator|->
name|pool_size
argument_list|,
name|ev
operator|->
name|log
argument_list|)
expr_stmt|;
if|if
condition|(
name|pool
operator|==
name|NULL
condition|)
block|{
return|return;
block|}
name|sa
operator|=
name|ngx_palloc
argument_list|(
name|pool
argument_list|,
name|ls
operator|->
name|listening
operator|->
name|socklen
argument_list|)
expr_stmt|;
if|if
condition|(
name|sa
operator|==
name|NULL
condition|)
block|{
return|return;
block|}
name|len
operator|=
name|ls
operator|->
name|listening
operator|->
name|socklen
expr_stmt|;
name|ngx_log_debug
argument_list|(
argument|ev->log
argument_list|,
literal|"ADDR %s"
argument|_ ls->listening->addr_text.data
argument_list|)
empty_stmt|;
name|s
operator|=
name|accept
argument_list|(
name|ls
operator|->
name|fd
argument_list|,
name|sa
argument_list|,
operator|&
name|len
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
name|err
operator|=
name|ngx_socket_errno
expr_stmt|;
if|if
condition|(
name|err
operator|==
name|NGX_EAGAIN
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_NOTICE
argument_list|,
name|ev
operator|->
name|log
argument_list|,
name|err
argument_list|,
literal|"EAGAIN while accept() %s"
argument_list|,
name|ls
operator|->
name|listening
operator|->
name|addr_text
operator|.
name|data
argument_list|)
expr_stmt|;
return|return;
block|}
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|ev
operator|->
name|log
argument_list|,
name|err
argument_list|,
literal|"accept() %s failed"
argument_list|,
name|ls
operator|->
name|listening
operator|->
name|addr_text
operator|.
name|data
argument_list|)
expr_stmt|;
name|ngx_destroy_pool
argument_list|(
name|pool
argument_list|)
expr_stmt|;
return|return;
block|}
comment|/* disable warnings: Win32 SOCKET is u_int while UNIX socket is int */
if|if
condition|(
operator|(
name|unsigned
operator|)
name|s
operator|>=
operator|(
name|unsigned
operator|)
name|ecf
operator|->
name|connections
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|ev
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"accept() %s returned socket #%d while "
literal|"only %d connections was configured, "
literal|"sleeping for 1 second"
argument_list|,
name|ls
operator|->
name|listening
operator|->
name|addr_text
operator|.
name|data
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
name|ev
operator|->
name|log
argument_list|,
name|ngx_socket_errno
argument_list|,
name|ngx_close_socket_n
literal|" %s failed"
argument_list|,
name|ls
operator|->
name|listening
operator|->
name|addr_text
operator|.
name|data
argument_list|)
expr_stmt|;
block|}
name|ngx_msleep
argument_list|(
literal|1000
argument_list|)
expr_stmt|;
name|ngx_destroy_pool
argument_list|(
name|pool
argument_list|)
expr_stmt|;
return|return;
block|}
comment|/* set a blocking mode for aio and non-blocking mode for others */
if|if
condition|(
name|ngx_inherited_nonblocking
condition|)
block|{
if|if
condition|(
operator|(
name|ngx_event_flags
operator|&
name|NGX_USE_AIO_EVENT
operator|)
condition|)
block|{
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
name|ev
operator|->
name|log
argument_list|,
name|ngx_socket_errno
argument_list|,
name|ngx_blocking_n
literal|" %s failed"
argument_list|,
name|ls
operator|->
name|listening
operator|->
name|addr_text
operator|.
name|data
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
name|ev
operator|->
name|log
argument_list|,
name|ngx_socket_errno
argument_list|,
name|ngx_close_socket_n
literal|" %s failed"
argument_list|,
name|ls
operator|->
name|listening
operator|->
name|addr_text
operator|.
name|data
argument_list|)
expr_stmt|;
block|}
name|ngx_destroy_pool
argument_list|(
name|pool
argument_list|)
expr_stmt|;
return|return;
block|}
block|}
block|}
else|else
block|{
if|if
condition|(
operator|(
name|ngx_event_flags
operator|&
name|NGX_USE_AIO_EVENT
operator|)
operator|==
literal|0
condition|)
block|{
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
name|ev
operator|->
name|log
argument_list|,
name|ngx_socket_errno
argument_list|,
name|ngx_nonblocking_n
literal|" %s failed"
argument_list|,
name|ls
operator|->
name|listening
operator|->
name|addr_text
operator|.
name|data
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
name|ev
operator|->
name|log
argument_list|,
name|ngx_socket_errno
argument_list|,
name|ngx_close_socket_n
literal|" %s failed"
argument_list|,
name|ls
operator|->
name|listening
operator|->
name|addr_text
operator|.
name|data
argument_list|)
expr_stmt|;
block|}
name|ngx_destroy_pool
argument_list|(
name|pool
argument_list|)
expr_stmt|;
return|return;
block|}
block|}
block|}
if|#
directive|if
operator|(
name|WIN32
operator|)
comment|/*          * Winsock assignes a socket number divisible by 4          * so to find a connection we divide a socket number by 4.          */
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
name|ls
operator|->
name|log
argument_list|,
literal|0
argument_list|,
name|ngx_socket_n
literal|" created socket %d"
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
name|rev
operator|=
operator|&
name|ngx_read_events
index|[
name|s
operator|/
literal|4
index|]
expr_stmt|;
name|wev
operator|=
operator|&
name|ngx_write_events
index|[
name|s
operator|/
literal|4
index|]
expr_stmt|;
name|c
operator|=
operator|&
name|ngx_connections
index|[
name|s
operator|/
literal|4
index|]
expr_stmt|;
else|#
directive|else
name|rev
operator|=
operator|&
name|ngx_read_events
index|[
name|s
index|]
expr_stmt|;
name|wev
operator|=
operator|&
name|ngx_write_events
index|[
name|s
index|]
expr_stmt|;
name|c
operator|=
operator|&
name|ngx_connections
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
name|c
operator|->
name|pool
operator|=
name|pool
expr_stmt|;
name|c
operator|->
name|listening
operator|=
name|ls
operator|->
name|listening
expr_stmt|;
name|c
operator|->
name|sockaddr
operator|=
name|sa
expr_stmt|;
name|c
operator|->
name|socklen
operator|=
name|len
expr_stmt|;
name|rev
operator|->
name|instance
operator|=
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
name|c
operator|->
name|fd
operator|=
name|s
expr_stmt|;
name|c
operator|->
name|unexpected_eof
operator|=
literal|1
expr_stmt|;
name|wev
operator|->
name|write
operator|=
literal|1
expr_stmt|;
if|if
condition|(
operator|(
name|ngx_event_flags
operator|&
name|NGX_USE_AIO_EVENT
operator|)
operator|==
literal|0
condition|)
block|{
name|wev
operator|->
name|ready
operator|=
literal|1
expr_stmt|;
block|}
name|c
operator|->
name|ctx
operator|=
name|ls
operator|->
name|ctx
expr_stmt|;
name|c
operator|->
name|servers
operator|=
name|ls
operator|->
name|servers
expr_stmt|;
name|c
operator|->
name|log
operator|=
name|ngx_palloc
argument_list|(
name|c
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_log_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|c
operator|->
name|log
operator|==
name|NULL
condition|)
block|{
return|return;
block|}
name|ngx_memcpy
argument_list|(
name|c
operator|->
name|log
argument_list|,
name|ev
operator|->
name|log
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_log_t
argument_list|)
argument_list|)
expr_stmt|;
name|rev
operator|->
name|log
operator|=
name|wev
operator|->
name|log
operator|=
name|c
operator|->
name|log
expr_stmt|;
comment|/* TODO: x86: MT: lock xadd, MP: lock xadd, shared */
name|c
operator|->
name|number
operator|=
name|ngx_connection_counter
operator|++
expr_stmt|;
name|ngx_log_debug
argument_list|(
argument|ev->log
argument_list|,
literal|"accept: %d, %d"
argument|_ s _ c->number
argument_list|)
empty_stmt|;
if|if
condition|(
name|ev
operator|->
name|deferred_accept
condition|)
block|{
name|rev
operator|->
name|ready
operator|=
literal|1
expr_stmt|;
block|}
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
name|ev
operator|->
name|log
argument_list|,
name|ngx_socket_errno
argument_list|,
name|ngx_close_socket_n
literal|" %s failed"
argument_list|,
name|ls
operator|->
name|listening
operator|->
name|addr_text
operator|.
name|data
argument_list|)
expr_stmt|;
block|}
name|ngx_destroy_pool
argument_list|(
name|pool
argument_list|)
expr_stmt|;
return|return;
block|}
block|}
name|ls
operator|->
name|listening
operator|->
name|handler
argument_list|(
name|c
argument_list|)
expr_stmt|;
if|if
condition|(
name|ngx_event_flags
operator|&
name|NGX_HAVE_KQUEUE_EVENT
condition|)
block|{
name|ev
operator|->
name|available
operator|--
expr_stmt|;
block|}
block|}
do|while
condition|(
name|ev
operator|->
name|available
condition|)
do|;
return|return;
block|}
end_function

end_unit

