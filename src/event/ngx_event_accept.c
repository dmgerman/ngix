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

begin_function_decl
specifier|static
name|size_t
name|ngx_accept_log_error
parameter_list|(
name|void
modifier|*
name|data
parameter_list|,
name|char
modifier|*
name|buf
parameter_list|,
name|size_t
name|len
parameter_list|)
function_decl|;
end_function_decl

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
decl_stmt|,
name|accepted
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
name|ngx_log_t
modifier|*
name|log
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
name|ngx_cycle
operator|->
name|conf_ctx
argument_list|,
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
literal|"accept on %s ready: %d"
argument|_                   ls->listening->addr_text.data _                   ev->available
argument_list|)
empty_stmt|;
name|ev
operator|->
name|ready
operator|=
literal|0
expr_stmt|;
name|accepted
operator|=
literal|0
expr_stmt|;
do|do
block|{
comment|/*          * Create the pool before accept() to avoid copy the sockaddr.          * Although accept() can fail it's an uncommon case          * and besides the pool can be got from the free pool list          */
if|if
condition|(
operator|!
operator|(
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
operator|)
condition|)
block|{
return|return;
block|}
if|if
condition|(
operator|!
operator|(
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
operator|)
condition|)
block|{
return|return;
block|}
if|if
condition|(
operator|!
operator|(
name|log
operator|=
name|ngx_palloc
argument_list|(
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_log_t
argument_list|)
argument_list|)
operator|)
condition|)
block|{
return|return;
block|}
name|ngx_memcpy
argument_list|(
name|log
argument_list|,
name|ls
operator|->
name|log
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_log_t
argument_list|)
argument_list|)
expr_stmt|;
name|pool
operator|->
name|log
operator|=
name|log
expr_stmt|;
name|log
operator|->
name|data
operator|=
name|ls
operator|->
name|listening
operator|->
name|addr_text
operator|.
name|data
expr_stmt|;
name|log
operator|->
name|handler
operator|=
name|ngx_accept_log_error
expr_stmt|;
name|len
operator|=
name|ls
operator|->
name|listening
operator|->
name|socklen
expr_stmt|;
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
name|log
argument_list|,
name|err
argument_list|,
literal|"EAGAIN after %d accepted connection(s)"
argument_list|,
name|accepted
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
literal|"accept() on %s failed"
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
comment|/* disable warning: Win32 SOCKET is u_int while UNIX socket is int */
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
literal|"accept() on %s returned socket #%d while "
literal|"only %d connections was configured, "
literal|"closing the connection"
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
name|log
argument_list|,
name|ngx_socket_errno
argument_list|,
name|ngx_close_socket_n
literal|"failed"
argument_list|)
expr_stmt|;
block|}
comment|/* TODO: disable temporary accept() event */
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
name|log
argument_list|,
name|ngx_socket_errno
argument_list|,
name|ngx_blocking_n
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
name|log
argument_list|,
name|ngx_socket_errno
argument_list|,
name|ngx_close_socket_n
literal|" failed"
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
name|log
argument_list|,
name|ngx_socket_errno
argument_list|,
name|ngx_close_socket_n
literal|" failed"
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
name|ev
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"accept() on %s returned socket #%d, "
literal|"not divisible by 4"
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
name|wev
operator|->
name|ready
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|ngx_event_flags
operator|&
operator|(
name|NGX_USE_AIO_EVENT
operator||
name|NGX_USE_EDGE_EVENT
operator|)
condition|)
block|{
comment|/* aio, iocp, epoll */
name|rev
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
name|log
expr_stmt|;
name|rev
operator|->
name|log
operator|=
name|log
expr_stmt|;
name|wev
operator|->
name|log
operator|=
name|log
expr_stmt|;
comment|/*          * In the multithreaded model the connection counter is updated by          * the main thread only that accept()s connections.          *          * TODO: MP: - allocated in a shared memory          *           - atomic increment (x86: lock xadd)          *             or protection by critical section or mutex          */
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
literal|" failed"
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
name|log
operator|->
name|data
operator|=
name|NULL
expr_stmt|;
name|log
operator|->
name|handler
operator|=
name|NULL
expr_stmt|;
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
name|accepted
operator|++
expr_stmt|;
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

begin_function
DECL|function|ngx_accept_log_error (void * data,char * buf,size_t len)
specifier|static
name|size_t
name|ngx_accept_log_error
parameter_list|(
name|void
modifier|*
name|data
parameter_list|,
name|char
modifier|*
name|buf
parameter_list|,
name|size_t
name|len
parameter_list|)
block|{
name|char
modifier|*
name|sock
init|=
name|data
decl_stmt|;
return|return
name|ngx_snprintf
argument_list|(
name|buf
argument_list|,
name|len
argument_list|,
literal|" while accept() on %s"
argument_list|,
name|sock
argument_list|)
return|;
block|}
end_function

end_unit

