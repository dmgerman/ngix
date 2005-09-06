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

begin_function
DECL|function|ngx_event_acceptex (ngx_event_t * rev)
name|void
name|ngx_event_acceptex
parameter_list|(
name|ngx_event_t
modifier|*
name|rev
parameter_list|)
block|{
name|ngx_connection_t
modifier|*
name|c
decl_stmt|;
name|c
operator|=
name|rev
operator|->
name|data
expr_stmt|;
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|c
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"AcceptEx: %d"
argument_list|,
name|c
operator|->
name|fd
argument_list|)
expr_stmt|;
if|if
condition|(
name|rev
operator|->
name|ovlp
operator|.
name|error
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_CRIT
argument_list|,
name|c
operator|->
name|log
argument_list|,
name|rev
operator|->
name|ovlp
operator|.
name|error
argument_list|,
literal|"AcceptEx() %V failed"
argument_list|,
operator|&
name|c
operator|->
name|listening
operator|->
name|addr_text
argument_list|)
expr_stmt|;
return|return;
block|}
comment|/* SO_UPDATE_ACCEPT_CONTEXT is required for shutdown() to work */
if|if
condition|(
name|setsockopt
argument_list|(
name|c
operator|->
name|fd
argument_list|,
name|SOL_SOCKET
argument_list|,
name|SO_UPDATE_ACCEPT_CONTEXT
argument_list|,
operator|(
name|char
operator|*
operator|)
operator|&
name|c
operator|->
name|listening
operator|->
name|fd
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_socket_t
argument_list|)
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
name|c
operator|->
name|log
argument_list|,
name|ngx_socket_errno
argument_list|,
literal|"setsockopt(SO_UPDATE_ACCEPT_CONTEXT) failed for %V"
argument_list|,
operator|&
name|c
operator|->
name|addr_text
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|c
operator|->
name|accept_context_updated
operator|=
literal|1
expr_stmt|;
block|}
name|getacceptexsockaddrs
argument_list|(
name|c
operator|->
name|buffer
operator|->
name|pos
argument_list|,
name|c
operator|->
name|listening
operator|->
name|post_accept_buffer_size
argument_list|,
name|c
operator|->
name|listening
operator|->
name|socklen
operator|+
literal|16
argument_list|,
name|c
operator|->
name|listening
operator|->
name|socklen
operator|+
literal|16
argument_list|,
operator|&
name|c
operator|->
name|local_sockaddr
argument_list|,
operator|&
name|c
operator|->
name|local_socklen
argument_list|,
operator|&
name|c
operator|->
name|sockaddr
argument_list|,
operator|&
name|c
operator|->
name|socklen
argument_list|)
expr_stmt|;
if|if
condition|(
name|c
operator|->
name|listening
operator|->
name|post_accept_buffer_size
condition|)
block|{
name|c
operator|->
name|buffer
operator|->
name|last
operator|+=
name|rev
operator|->
name|available
expr_stmt|;
name|c
operator|->
name|buffer
operator|->
name|end
operator|=
name|c
operator|->
name|buffer
operator|->
name|start
operator|+
name|c
operator|->
name|listening
operator|->
name|post_accept_buffer_size
expr_stmt|;
block|}
else|else
block|{
name|c
operator|->
name|buffer
operator|=
name|NULL
expr_stmt|;
block|}
if|if
condition|(
name|c
operator|->
name|listening
operator|->
name|addr_ntop
condition|)
block|{
name|c
operator|->
name|addr_text
operator|.
name|data
operator|=
name|ngx_palloc
argument_list|(
name|c
operator|->
name|pool
argument_list|,
name|c
operator|->
name|listening
operator|->
name|addr_text_max_len
argument_list|)
expr_stmt|;
if|if
condition|(
name|c
operator|->
name|addr_text
operator|.
name|data
operator|==
name|NULL
condition|)
block|{
comment|/* TODO: close socket */
return|return;
block|}
name|c
operator|->
name|addr_text
operator|.
name|len
operator|=
name|ngx_sock_ntop
argument_list|(
name|c
operator|->
name|listening
operator|->
name|family
argument_list|,
name|c
operator|->
name|sockaddr
argument_list|,
name|c
operator|->
name|addr_text
operator|.
name|data
argument_list|,
name|c
operator|->
name|listening
operator|->
name|addr_text_max_len
argument_list|)
expr_stmt|;
if|if
condition|(
name|c
operator|->
name|addr_text
operator|.
name|len
operator|==
literal|0
condition|)
block|{
comment|/* TODO: close socket */
return|return;
block|}
block|}
name|ngx_event_post_acceptex
argument_list|(
name|c
operator|->
name|listening
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|c
operator|->
name|number
operator|=
name|ngx_atomic_inc
argument_list|(
name|ngx_connection_counter
argument_list|)
expr_stmt|;
name|c
operator|->
name|listening
operator|->
name|handler
argument_list|(
name|c
argument_list|)
expr_stmt|;
return|return;
block|}
end_function

begin_function
DECL|function|ngx_event_post_acceptex (ngx_listening_t * ls,int n)
name|int
name|ngx_event_post_acceptex
parameter_list|(
name|ngx_listening_t
modifier|*
name|ls
parameter_list|,
name|int
name|n
parameter_list|)
block|{
name|u_long
name|rcvd
decl_stmt|;
name|ngx_int_t
name|i
decl_stmt|;
name|ngx_err_t
name|err
decl_stmt|;
name|ngx_event_t
modifier|*
name|rev
decl_stmt|,
modifier|*
name|wev
decl_stmt|;
name|ngx_socket_t
name|s
decl_stmt|;
name|ngx_connection_t
modifier|*
name|c
decl_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|n
condition|;
name|i
operator|++
control|)
block|{
comment|/* TODO: look up reused sockets */
name|s
operator|=
name|ngx_socket
argument_list|(
name|ls
operator|->
name|family
argument_list|,
name|ls
operator|->
name|type
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|ls
operator|->
name|log
argument_list|,
literal|0
argument_list|,
name|ngx_socket_n
literal|" s:%d"
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
name|ls
operator|->
name|log
argument_list|,
name|ngx_socket_errno
argument_list|,
name|ngx_socket_n
literal|" for AcceptEx() %s post failed"
argument_list|,
name|ls
operator|->
name|addr_text
operator|.
name|data
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
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
name|c
operator|->
name|listening
operator|=
name|ls
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
name|ovlp
operator|.
name|event
operator|=
name|rev
expr_stmt|;
name|wev
operator|->
name|ovlp
operator|.
name|event
operator|=
name|wev
expr_stmt|;
name|rev
operator|->
name|handler
operator|=
name|ngx_event_acceptex
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
name|rev
operator|->
name|ready
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
name|pool
operator|=
name|ngx_create_pool
argument_list|(
name|ls
operator|->
name|pool_size
argument_list|,
name|ls
operator|->
name|log
argument_list|)
expr_stmt|;
if|if
condition|(
name|c
operator|->
name|pool
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|c
operator|->
name|buffer
operator|=
name|ngx_create_temp_buf
argument_list|(
name|c
operator|->
name|pool
argument_list|,
name|ls
operator|->
name|post_accept_buffer_size
operator|+
literal|2
operator|*
operator|(
name|c
operator|->
name|listening
operator|->
name|socklen
operator|+
literal|16
operator|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|c
operator|->
name|buffer
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|c
operator|->
name|local_sockaddr
operator|=
name|ngx_palloc
argument_list|(
name|c
operator|->
name|pool
argument_list|,
name|ls
operator|->
name|socklen
argument_list|)
expr_stmt|;
if|if
condition|(
name|c
operator|->
name|local_sockaddr
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|c
operator|->
name|sockaddr
operator|=
name|ngx_palloc
argument_list|(
name|c
operator|->
name|pool
argument_list|,
name|ls
operator|->
name|socklen
argument_list|)
expr_stmt|;
if|if
condition|(
name|c
operator|->
name|sockaddr
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
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
return|return
name|NGX_ERROR
return|;
block|}
name|ngx_memcpy
argument_list|(
name|c
operator|->
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
name|c
operator|->
name|read
operator|->
name|log
operator|=
name|c
operator|->
name|log
expr_stmt|;
name|c
operator|->
name|write
operator|->
name|log
operator|=
name|c
operator|->
name|log
expr_stmt|;
if|if
condition|(
name|ngx_add_event
argument_list|(
name|rev
argument_list|,
literal|0
argument_list|,
name|NGX_IOCP_IO
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|acceptex
argument_list|(
name|ls
operator|->
name|fd
argument_list|,
name|s
argument_list|,
name|c
operator|->
name|buffer
operator|->
name|pos
argument_list|,
name|ls
operator|->
name|post_accept_buffer_size
argument_list|,
name|ls
operator|->
name|socklen
operator|+
literal|16
argument_list|,
name|ls
operator|->
name|socklen
operator|+
literal|16
argument_list|,
operator|&
name|rcvd
argument_list|,
operator|(
name|LPOVERLAPPED
operator|)
operator|&
name|rev
operator|->
name|ovlp
argument_list|)
operator|==
literal|0
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
name|WSA_IO_PENDING
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|ls
operator|->
name|log
argument_list|,
name|err
argument_list|,
literal|"AcceptEx() %s falied"
argument_list|,
name|ls
operator|->
name|addr_text
operator|.
name|data
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
block|}
block|}
return|return
name|NGX_OK
return|;
block|}
end_function

end_unit

