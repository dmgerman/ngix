begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (C) Roman Arutyunyan  * Copyright (C) Nginx, Inc.  */
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
file|<ngx_stream.h>
end_include

begin_function_decl
specifier|static
name|void
name|ngx_stream_log_session
parameter_list|(
name|ngx_stream_session_t
modifier|*
name|s
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|void
name|ngx_stream_close_connection
parameter_list|(
name|ngx_connection_t
modifier|*
name|c
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|u_char
modifier|*
name|ngx_stream_log_error
parameter_list|(
name|ngx_log_t
modifier|*
name|log
parameter_list|,
name|u_char
modifier|*
name|buf
parameter_list|,
name|size_t
name|len
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|void
name|ngx_stream_proxy_protocol_handler
parameter_list|(
name|ngx_event_t
modifier|*
name|rev
parameter_list|)
function_decl|;
end_function_decl

begin_function
name|void
DECL|function|ngx_stream_init_connection (ngx_connection_t * c)
name|ngx_stream_init_connection
parameter_list|(
name|ngx_connection_t
modifier|*
name|c
parameter_list|)
block|{
name|u_char
name|text
index|[
name|NGX_SOCKADDR_STRLEN
index|]
decl_stmt|;
name|size_t
name|len
decl_stmt|;
name|ngx_uint_t
name|i
decl_stmt|;
name|ngx_time_t
modifier|*
name|tp
decl_stmt|;
name|ngx_event_t
modifier|*
name|rev
decl_stmt|;
name|struct
name|sockaddr
modifier|*
name|sa
decl_stmt|;
name|ngx_stream_port_t
modifier|*
name|port
decl_stmt|;
name|struct
name|sockaddr_in
modifier|*
name|sin
decl_stmt|;
name|ngx_stream_in_addr_t
modifier|*
name|addr
decl_stmt|;
name|ngx_stream_session_t
modifier|*
name|s
decl_stmt|;
name|ngx_stream_addr_conf_t
modifier|*
name|addr_conf
decl_stmt|;
if|#
directive|if
operator|(
name|NGX_HAVE_INET6
operator|)
name|struct
name|sockaddr_in6
modifier|*
name|sin6
decl_stmt|;
name|ngx_stream_in6_addr_t
modifier|*
name|addr6
decl_stmt|;
endif|#
directive|endif
name|ngx_stream_core_srv_conf_t
modifier|*
name|cscf
decl_stmt|;
name|ngx_stream_core_main_conf_t
modifier|*
name|cmcf
decl_stmt|;
comment|/* find the server configuration for the address:port */
name|port
operator|=
name|c
operator|->
name|listening
operator|->
name|servers
expr_stmt|;
if|if
condition|(
name|port
operator|->
name|naddrs
operator|>
literal|1
condition|)
block|{
comment|/*          * There are several addresses on this port and one of them          * is the "*:port" wildcard so getsockname() is needed to determine          * the server address.          *          * AcceptEx() and recvmsg() already gave this address.          */
if|if
condition|(
name|ngx_connection_local_sockaddr
argument_list|(
name|c
argument_list|,
name|NULL
argument_list|,
literal|0
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
name|ngx_stream_close_connection
argument_list|(
name|c
argument_list|)
expr_stmt|;
return|return;
block|}
name|sa
operator|=
name|c
operator|->
name|local_sockaddr
expr_stmt|;
switch|switch
condition|(
name|sa
operator|->
name|sa_family
condition|)
block|{
if|#
directive|if
operator|(
name|NGX_HAVE_INET6
operator|)
case|case
name|AF_INET6
case|:
name|sin6
operator|=
operator|(
expr|struct
name|sockaddr_in6
operator|*
operator|)
name|sa
expr_stmt|;
name|addr6
operator|=
name|port
operator|->
name|addrs
expr_stmt|;
comment|/* the last address is "*" */
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|port
operator|->
name|naddrs
operator|-
literal|1
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|ngx_memcmp
argument_list|(
operator|&
name|addr6
index|[
name|i
index|]
operator|.
name|addr6
argument_list|,
operator|&
name|sin6
operator|->
name|sin6_addr
argument_list|,
literal|16
argument_list|)
operator|==
literal|0
condition|)
block|{
break|break;
block|}
block|}
name|addr_conf
operator|=
operator|&
name|addr6
index|[
name|i
index|]
operator|.
name|conf
expr_stmt|;
break|break;
endif|#
directive|endif
default|default:
comment|/* AF_INET */
name|sin
operator|=
operator|(
expr|struct
name|sockaddr_in
operator|*
operator|)
name|sa
expr_stmt|;
name|addr
operator|=
name|port
operator|->
name|addrs
expr_stmt|;
comment|/* the last address is "*" */
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|port
operator|->
name|naddrs
operator|-
literal|1
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|addr
index|[
name|i
index|]
operator|.
name|addr
operator|==
name|sin
operator|->
name|sin_addr
operator|.
name|s_addr
condition|)
block|{
break|break;
block|}
block|}
name|addr_conf
operator|=
operator|&
name|addr
index|[
name|i
index|]
operator|.
name|conf
expr_stmt|;
break|break;
block|}
block|}
else|else
block|{
switch|switch
condition|(
name|c
operator|->
name|local_sockaddr
operator|->
name|sa_family
condition|)
block|{
if|#
directive|if
operator|(
name|NGX_HAVE_INET6
operator|)
case|case
name|AF_INET6
case|:
name|addr6
operator|=
name|port
operator|->
name|addrs
expr_stmt|;
name|addr_conf
operator|=
operator|&
name|addr6
index|[
literal|0
index|]
operator|.
name|conf
expr_stmt|;
break|break;
endif|#
directive|endif
default|default:
comment|/* AF_INET */
name|addr
operator|=
name|port
operator|->
name|addrs
expr_stmt|;
name|addr_conf
operator|=
operator|&
name|addr
index|[
literal|0
index|]
operator|.
name|conf
expr_stmt|;
break|break;
block|}
block|}
name|s
operator|=
name|ngx_pcalloc
argument_list|(
name|c
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_stream_session_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|s
operator|==
name|NULL
condition|)
block|{
name|ngx_stream_close_connection
argument_list|(
name|c
argument_list|)
expr_stmt|;
return|return;
block|}
name|s
operator|->
name|signature
operator|=
name|NGX_STREAM_MODULE
expr_stmt|;
name|s
operator|->
name|main_conf
operator|=
name|addr_conf
operator|->
name|ctx
operator|->
name|main_conf
expr_stmt|;
name|s
operator|->
name|srv_conf
operator|=
name|addr_conf
operator|->
name|ctx
operator|->
name|srv_conf
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_STREAM_SSL
operator|)
name|s
operator|->
name|ssl
operator|=
name|addr_conf
operator|->
name|ssl
expr_stmt|;
endif|#
directive|endif
if|if
condition|(
name|c
operator|->
name|buffer
condition|)
block|{
name|s
operator|->
name|received
operator|+=
name|c
operator|->
name|buffer
operator|->
name|last
operator|-
name|c
operator|->
name|buffer
operator|->
name|pos
expr_stmt|;
block|}
name|s
operator|->
name|connection
operator|=
name|c
expr_stmt|;
name|c
operator|->
name|data
operator|=
name|s
expr_stmt|;
name|cscf
operator|=
name|ngx_stream_get_module_srv_conf
argument_list|(
name|s
argument_list|,
name|ngx_stream_core_module
argument_list|)
expr_stmt|;
name|ngx_set_connection_log
argument_list|(
name|c
argument_list|,
name|cscf
operator|->
name|error_log
argument_list|)
expr_stmt|;
name|len
operator|=
name|ngx_sock_ntop
argument_list|(
name|c
operator|->
name|sockaddr
argument_list|,
name|c
operator|->
name|socklen
argument_list|,
name|text
argument_list|,
name|NGX_SOCKADDR_STRLEN
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|ngx_log_error
argument_list|(
name|NGX_LOG_INFO
argument_list|,
name|c
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"*%uA %sclient %*s connected to %V"
argument_list|,
name|c
operator|->
name|number
argument_list|,
name|c
operator|->
name|type
operator|==
name|SOCK_DGRAM
condition|?
literal|"udp "
else|:
literal|""
argument_list|,
name|len
argument_list|,
name|text
argument_list|,
operator|&
name|addr_conf
operator|->
name|addr_text
argument_list|)
expr_stmt|;
name|c
operator|->
name|log
operator|->
name|connection
operator|=
name|c
operator|->
name|number
expr_stmt|;
name|c
operator|->
name|log
operator|->
name|handler
operator|=
name|ngx_stream_log_error
expr_stmt|;
name|c
operator|->
name|log
operator|->
name|data
operator|=
name|s
expr_stmt|;
name|c
operator|->
name|log
operator|->
name|action
operator|=
literal|"initializing session"
expr_stmt|;
name|c
operator|->
name|log_error
operator|=
name|NGX_ERROR_INFO
expr_stmt|;
name|s
operator|->
name|ctx
operator|=
name|ngx_pcalloc
argument_list|(
name|c
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|void
operator|*
argument_list|)
operator|*
name|ngx_stream_max_module
argument_list|)
expr_stmt|;
if|if
condition|(
name|s
operator|->
name|ctx
operator|==
name|NULL
condition|)
block|{
name|ngx_stream_close_connection
argument_list|(
name|c
argument_list|)
expr_stmt|;
return|return;
block|}
name|cmcf
operator|=
name|ngx_stream_get_module_main_conf
argument_list|(
name|s
argument_list|,
name|ngx_stream_core_module
argument_list|)
expr_stmt|;
name|s
operator|->
name|variables
operator|=
name|ngx_pcalloc
argument_list|(
name|s
operator|->
name|connection
operator|->
name|pool
argument_list|,
name|cmcf
operator|->
name|variables
operator|.
name|nelts
operator|*
sizeof|sizeof
argument_list|(
name|ngx_stream_variable_value_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|s
operator|->
name|variables
operator|==
name|NULL
condition|)
block|{
name|ngx_stream_close_connection
argument_list|(
name|c
argument_list|)
expr_stmt|;
return|return;
block|}
name|tp
operator|=
name|ngx_timeofday
argument_list|()
expr_stmt|;
name|s
operator|->
name|start_sec
operator|=
name|tp
operator|->
name|sec
expr_stmt|;
name|s
operator|->
name|start_msec
operator|=
name|tp
operator|->
name|msec
expr_stmt|;
name|rev
operator|=
name|c
operator|->
name|read
expr_stmt|;
name|rev
operator|->
name|handler
operator|=
name|ngx_stream_session_handler
expr_stmt|;
if|if
condition|(
name|addr_conf
operator|->
name|proxy_protocol
condition|)
block|{
name|c
operator|->
name|log
operator|->
name|action
operator|=
literal|"reading PROXY protocol"
expr_stmt|;
name|rev
operator|->
name|handler
operator|=
name|ngx_stream_proxy_protocol_handler
expr_stmt|;
if|if
condition|(
operator|!
name|rev
operator|->
name|ready
condition|)
block|{
name|ngx_add_timer
argument_list|(
name|rev
argument_list|,
name|cscf
operator|->
name|proxy_protocol_timeout
argument_list|)
expr_stmt|;
if|if
condition|(
name|ngx_handle_read_event
argument_list|(
name|rev
argument_list|,
literal|0
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
name|ngx_stream_finalize_session
argument_list|(
name|s
argument_list|,
name|NGX_STREAM_INTERNAL_SERVER_ERROR
argument_list|)
expr_stmt|;
block|}
return|return;
block|}
block|}
if|if
condition|(
name|ngx_use_accept_mutex
condition|)
block|{
name|ngx_post_event
argument_list|(
name|rev
argument_list|,
operator|&
name|ngx_posted_events
argument_list|)
expr_stmt|;
return|return;
block|}
name|rev
operator|->
name|handler
argument_list|(
name|rev
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
specifier|static
name|void
DECL|function|ngx_stream_proxy_protocol_handler (ngx_event_t * rev)
name|ngx_stream_proxy_protocol_handler
parameter_list|(
name|ngx_event_t
modifier|*
name|rev
parameter_list|)
block|{
name|u_char
modifier|*
name|p
decl_stmt|,
name|buf
index|[
name|NGX_PROXY_PROTOCOL_MAX_HEADER
index|]
decl_stmt|;
name|size_t
name|size
decl_stmt|;
name|ssize_t
name|n
decl_stmt|;
name|ngx_err_t
name|err
decl_stmt|;
name|ngx_connection_t
modifier|*
name|c
decl_stmt|;
name|ngx_stream_session_t
modifier|*
name|s
decl_stmt|;
name|ngx_stream_core_srv_conf_t
modifier|*
name|cscf
decl_stmt|;
name|c
operator|=
name|rev
operator|->
name|data
expr_stmt|;
name|s
operator|=
name|c
operator|->
name|data
expr_stmt|;
name|ngx_log_debug0
argument_list|(
name|NGX_LOG_DEBUG_STREAM
argument_list|,
name|c
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"stream PROXY protocol handler"
argument_list|)
expr_stmt|;
if|if
condition|(
name|rev
operator|->
name|timedout
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_INFO
argument_list|,
name|c
operator|->
name|log
argument_list|,
name|NGX_ETIMEDOUT
argument_list|,
literal|"client timed out"
argument_list|)
expr_stmt|;
name|ngx_stream_finalize_session
argument_list|(
name|s
argument_list|,
name|NGX_STREAM_OK
argument_list|)
expr_stmt|;
return|return;
block|}
name|n
operator|=
name|recv
argument_list|(
name|c
operator|->
name|fd
argument_list|,
operator|(
name|char
operator|*
operator|)
name|buf
argument_list|,
sizeof|sizeof
argument_list|(
name|buf
argument_list|)
argument_list|,
name|MSG_PEEK
argument_list|)
expr_stmt|;
name|err
operator|=
name|ngx_socket_errno
expr_stmt|;
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_STREAM
argument_list|,
name|c
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"recv(): %z"
argument_list|,
name|n
argument_list|)
expr_stmt|;
if|if
condition|(
name|n
operator|==
operator|-
literal|1
condition|)
block|{
if|if
condition|(
name|err
operator|==
name|NGX_EAGAIN
condition|)
block|{
name|rev
operator|->
name|ready
operator|=
literal|0
expr_stmt|;
if|if
condition|(
operator|!
name|rev
operator|->
name|timer_set
condition|)
block|{
name|cscf
operator|=
name|ngx_stream_get_module_srv_conf
argument_list|(
name|s
argument_list|,
name|ngx_stream_core_module
argument_list|)
expr_stmt|;
name|ngx_add_timer
argument_list|(
name|rev
argument_list|,
name|cscf
operator|->
name|proxy_protocol_timeout
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|ngx_handle_read_event
argument_list|(
name|rev
argument_list|,
literal|0
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
name|ngx_stream_finalize_session
argument_list|(
name|s
argument_list|,
name|NGX_STREAM_INTERNAL_SERVER_ERROR
argument_list|)
expr_stmt|;
block|}
return|return;
block|}
name|ngx_connection_error
argument_list|(
name|c
argument_list|,
name|err
argument_list|,
literal|"recv() failed"
argument_list|)
expr_stmt|;
name|ngx_stream_finalize_session
argument_list|(
name|s
argument_list|,
name|NGX_STREAM_OK
argument_list|)
expr_stmt|;
return|return;
block|}
if|if
condition|(
name|rev
operator|->
name|timer_set
condition|)
block|{
name|ngx_del_timer
argument_list|(
name|rev
argument_list|)
expr_stmt|;
block|}
name|p
operator|=
name|ngx_proxy_protocol_read
argument_list|(
name|c
argument_list|,
name|buf
argument_list|,
name|buf
operator|+
name|n
argument_list|)
expr_stmt|;
if|if
condition|(
name|p
operator|==
name|NULL
condition|)
block|{
name|ngx_stream_finalize_session
argument_list|(
name|s
argument_list|,
name|NGX_STREAM_BAD_REQUEST
argument_list|)
expr_stmt|;
return|return;
block|}
name|size
operator|=
name|p
operator|-
name|buf
expr_stmt|;
if|if
condition|(
name|c
operator|->
name|recv
argument_list|(
name|c
argument_list|,
name|buf
argument_list|,
name|size
argument_list|)
operator|!=
operator|(
name|ssize_t
operator|)
name|size
condition|)
block|{
name|ngx_stream_finalize_session
argument_list|(
name|s
argument_list|,
name|NGX_STREAM_INTERNAL_SERVER_ERROR
argument_list|)
expr_stmt|;
return|return;
block|}
name|c
operator|->
name|log
operator|->
name|action
operator|=
literal|"initializing session"
expr_stmt|;
name|ngx_stream_session_handler
argument_list|(
name|rev
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
name|void
DECL|function|ngx_stream_session_handler (ngx_event_t * rev)
name|ngx_stream_session_handler
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
name|ngx_stream_session_t
modifier|*
name|s
decl_stmt|;
name|c
operator|=
name|rev
operator|->
name|data
expr_stmt|;
name|s
operator|=
name|c
operator|->
name|data
expr_stmt|;
name|ngx_stream_core_run_phases
argument_list|(
name|s
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
name|void
DECL|function|ngx_stream_finalize_session (ngx_stream_session_t * s,ngx_uint_t rc)
name|ngx_stream_finalize_session
parameter_list|(
name|ngx_stream_session_t
modifier|*
name|s
parameter_list|,
name|ngx_uint_t
name|rc
parameter_list|)
block|{
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_STREAM
argument_list|,
name|s
operator|->
name|connection
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"finalize stream session: %i"
argument_list|,
name|rc
argument_list|)
expr_stmt|;
name|s
operator|->
name|status
operator|=
name|rc
expr_stmt|;
name|ngx_stream_log_session
argument_list|(
name|s
argument_list|)
expr_stmt|;
name|ngx_stream_close_connection
argument_list|(
name|s
operator|->
name|connection
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
specifier|static
name|void
DECL|function|ngx_stream_log_session (ngx_stream_session_t * s)
name|ngx_stream_log_session
parameter_list|(
name|ngx_stream_session_t
modifier|*
name|s
parameter_list|)
block|{
name|ngx_uint_t
name|i
decl_stmt|,
name|n
decl_stmt|;
name|ngx_stream_handler_pt
modifier|*
name|log_handler
decl_stmt|;
name|ngx_stream_core_main_conf_t
modifier|*
name|cmcf
decl_stmt|;
name|cmcf
operator|=
name|ngx_stream_get_module_main_conf
argument_list|(
name|s
argument_list|,
name|ngx_stream_core_module
argument_list|)
expr_stmt|;
name|log_handler
operator|=
name|cmcf
operator|->
name|phases
index|[
name|NGX_STREAM_LOG_PHASE
index|]
operator|.
name|handlers
operator|.
name|elts
expr_stmt|;
name|n
operator|=
name|cmcf
operator|->
name|phases
index|[
name|NGX_STREAM_LOG_PHASE
index|]
operator|.
name|handlers
operator|.
name|nelts
expr_stmt|;
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
name|log_handler
index|[
name|i
index|]
operator|(
name|s
operator|)
expr_stmt|;
block|}
block|}
end_function

begin_function
specifier|static
name|void
DECL|function|ngx_stream_close_connection (ngx_connection_t * c)
name|ngx_stream_close_connection
parameter_list|(
name|ngx_connection_t
modifier|*
name|c
parameter_list|)
block|{
name|ngx_pool_t
modifier|*
name|pool
decl_stmt|;
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_STREAM
argument_list|,
name|c
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"close stream connection: %d"
argument_list|,
name|c
operator|->
name|fd
argument_list|)
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_STREAM_SSL
operator|)
if|if
condition|(
name|c
operator|->
name|ssl
condition|)
block|{
if|if
condition|(
name|ngx_ssl_shutdown
argument_list|(
name|c
argument_list|)
operator|==
name|NGX_AGAIN
condition|)
block|{
name|c
operator|->
name|ssl
operator|->
name|handler
operator|=
name|ngx_stream_close_connection
expr_stmt|;
return|return;
block|}
block|}
endif|#
directive|endif
if|#
directive|if
operator|(
name|NGX_STAT_STUB
operator|)
operator|(
name|void
operator|)
name|ngx_atomic_fetch_add
argument_list|(
name|ngx_stat_active
argument_list|,
operator|-
literal|1
argument_list|)
expr_stmt|;
endif|#
directive|endif
name|pool
operator|=
name|c
operator|->
name|pool
expr_stmt|;
name|ngx_close_connection
argument_list|(
name|c
argument_list|)
expr_stmt|;
name|ngx_destroy_pool
argument_list|(
name|pool
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
specifier|static
name|u_char
modifier|*
DECL|function|ngx_stream_log_error (ngx_log_t * log,u_char * buf,size_t len)
name|ngx_stream_log_error
parameter_list|(
name|ngx_log_t
modifier|*
name|log
parameter_list|,
name|u_char
modifier|*
name|buf
parameter_list|,
name|size_t
name|len
parameter_list|)
block|{
name|u_char
modifier|*
name|p
decl_stmt|;
name|ngx_stream_session_t
modifier|*
name|s
decl_stmt|;
if|if
condition|(
name|log
operator|->
name|action
condition|)
block|{
name|p
operator|=
name|ngx_snprintf
argument_list|(
name|buf
argument_list|,
name|len
argument_list|,
literal|" while %s"
argument_list|,
name|log
operator|->
name|action
argument_list|)
expr_stmt|;
name|len
operator|-=
name|p
operator|-
name|buf
expr_stmt|;
name|buf
operator|=
name|p
expr_stmt|;
block|}
name|s
operator|=
name|log
operator|->
name|data
expr_stmt|;
name|p
operator|=
name|ngx_snprintf
argument_list|(
name|buf
argument_list|,
name|len
argument_list|,
literal|", %sclient: %V, server: %V"
argument_list|,
name|s
operator|->
name|connection
operator|->
name|type
operator|==
name|SOCK_DGRAM
condition|?
literal|"udp "
else|:
literal|""
argument_list|,
operator|&
name|s
operator|->
name|connection
operator|->
name|addr_text
argument_list|,
operator|&
name|s
operator|->
name|connection
operator|->
name|listening
operator|->
name|addr_text
argument_list|)
expr_stmt|;
name|len
operator|-=
name|p
operator|-
name|buf
expr_stmt|;
name|buf
operator|=
name|p
expr_stmt|;
if|if
condition|(
name|s
operator|->
name|log_handler
condition|)
block|{
name|p
operator|=
name|s
operator|->
name|log_handler
argument_list|(
name|log
argument_list|,
name|buf
argument_list|,
name|len
argument_list|)
expr_stmt|;
block|}
return|return
name|p
return|;
block|}
end_function

end_unit

