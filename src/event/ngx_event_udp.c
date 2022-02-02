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

begin_if
if|#
directive|if
operator|!
operator|(
name|NGX_WIN32
operator|)
end_if

begin_struct
DECL|struct|ngx_udp_connection_s
struct|struct
name|ngx_udp_connection_s
block|{
DECL|member|node
name|ngx_rbtree_node_t
name|node
decl_stmt|;
DECL|member|connection
name|ngx_connection_t
modifier|*
name|connection
decl_stmt|;
DECL|member|buffer
name|ngx_buf_t
modifier|*
name|buffer
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
specifier|static
name|void
name|ngx_close_accepted_udp_connection
parameter_list|(
name|ngx_connection_t
modifier|*
name|c
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|ssize_t
name|ngx_udp_shared_recv
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
specifier|static
name|ngx_int_t
name|ngx_insert_udp_connection
parameter_list|(
name|ngx_connection_t
modifier|*
name|c
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|ngx_connection_t
modifier|*
name|ngx_lookup_udp_connection
parameter_list|(
name|ngx_listening_t
modifier|*
name|ls
parameter_list|,
name|struct
name|sockaddr
modifier|*
name|sockaddr
parameter_list|,
name|socklen_t
name|socklen
parameter_list|,
name|struct
name|sockaddr
modifier|*
name|local_sockaddr
parameter_list|,
name|socklen_t
name|local_socklen
parameter_list|)
function_decl|;
end_function_decl

begin_function
name|void
DECL|function|ngx_event_recvmsg (ngx_event_t * ev)
name|ngx_event_recvmsg
parameter_list|(
name|ngx_event_t
modifier|*
name|ev
parameter_list|)
block|{
name|ssize_t
name|n
decl_stmt|;
name|ngx_buf_t
name|buf
decl_stmt|;
name|ngx_log_t
modifier|*
name|log
decl_stmt|;
name|ngx_err_t
name|err
decl_stmt|;
name|socklen_t
name|socklen
decl_stmt|,
name|local_socklen
decl_stmt|;
name|ngx_event_t
modifier|*
name|rev
decl_stmt|,
modifier|*
name|wev
decl_stmt|;
name|struct
name|iovec
name|iov
index|[
literal|1
index|]
decl_stmt|;
name|struct
name|msghdr
name|msg
decl_stmt|;
name|ngx_sockaddr_t
name|sa
decl_stmt|,
name|lsa
decl_stmt|;
name|struct
name|sockaddr
modifier|*
name|sockaddr
decl_stmt|,
modifier|*
name|local_sockaddr
decl_stmt|;
name|ngx_listening_t
modifier|*
name|ls
decl_stmt|;
name|ngx_event_conf_t
modifier|*
name|ecf
decl_stmt|;
name|ngx_connection_t
modifier|*
name|c
decl_stmt|,
modifier|*
name|lc
decl_stmt|;
specifier|static
name|u_char
name|buffer
index|[
literal|65535
index|]
decl_stmt|;
if|#
directive|if
operator|(
name|NGX_HAVE_ADDRINFO_CMSG
operator|)
name|u_char
name|msg_control
index|[
name|CMSG_SPACE
argument_list|(
sizeof|sizeof
argument_list|(
name|ngx_addrinfo_t
argument_list|)
argument_list|)
index|]
decl_stmt|;
endif|#
directive|endif
if|if
condition|(
name|ev
operator|->
name|timedout
condition|)
block|{
if|if
condition|(
name|ngx_enable_accept_events
argument_list|(
operator|(
name|ngx_cycle_t
operator|*
operator|)
name|ngx_cycle
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
return|return;
block|}
name|ev
operator|->
name|timedout
operator|=
literal|0
expr_stmt|;
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
if|if
condition|(
operator|!
operator|(
name|ngx_event_flags
operator|&
name|NGX_USE_KQUEUE_EVENT
operator|)
condition|)
block|{
name|ev
operator|->
name|available
operator|=
name|ecf
operator|->
name|multi_accept
expr_stmt|;
block|}
name|lc
operator|=
name|ev
operator|->
name|data
expr_stmt|;
name|ls
operator|=
name|lc
operator|->
name|listening
expr_stmt|;
name|ev
operator|->
name|ready
operator|=
literal|0
expr_stmt|;
name|ngx_log_debug2
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|ev
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"recvmsg on %V, ready: %d"
argument_list|,
operator|&
name|ls
operator|->
name|addr_text
argument_list|,
name|ev
operator|->
name|available
argument_list|)
expr_stmt|;
do|do
block|{
name|ngx_memzero
argument_list|(
operator|&
name|msg
argument_list|,
sizeof|sizeof
argument_list|(
expr|struct
name|msghdr
argument_list|)
argument_list|)
expr_stmt|;
name|iov
index|[
literal|0
index|]
operator|.
name|iov_base
operator|=
operator|(
name|void
operator|*
operator|)
name|buffer
expr_stmt|;
name|iov
index|[
literal|0
index|]
operator|.
name|iov_len
operator|=
sizeof|sizeof
argument_list|(
name|buffer
argument_list|)
expr_stmt|;
name|msg
operator|.
name|msg_name
operator|=
operator|&
name|sa
expr_stmt|;
name|msg
operator|.
name|msg_namelen
operator|=
sizeof|sizeof
argument_list|(
name|ngx_sockaddr_t
argument_list|)
expr_stmt|;
name|msg
operator|.
name|msg_iov
operator|=
name|iov
expr_stmt|;
name|msg
operator|.
name|msg_iovlen
operator|=
literal|1
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_HAVE_ADDRINFO_CMSG
operator|)
if|if
condition|(
name|ls
operator|->
name|wildcard
condition|)
block|{
name|msg
operator|.
name|msg_control
operator|=
operator|&
name|msg_control
expr_stmt|;
name|msg
operator|.
name|msg_controllen
operator|=
sizeof|sizeof
argument_list|(
name|msg_control
argument_list|)
expr_stmt|;
name|ngx_memzero
argument_list|(
operator|&
name|msg_control
argument_list|,
sizeof|sizeof
argument_list|(
name|msg_control
argument_list|)
argument_list|)
expr_stmt|;
block|}
endif|#
directive|endif
name|n
operator|=
name|recvmsg
argument_list|(
name|lc
operator|->
name|fd
argument_list|,
operator|&
name|msg
argument_list|,
literal|0
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
name|ngx_log_debug0
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|ev
operator|->
name|log
argument_list|,
name|err
argument_list|,
literal|"recvmsg() not ready"
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
literal|"recvmsg() failed"
argument_list|)
expr_stmt|;
return|return;
block|}
if|#
directive|if
operator|(
name|NGX_HAVE_ADDRINFO_CMSG
operator|)
if|if
condition|(
name|msg
operator|.
name|msg_flags
operator|&
operator|(
name|MSG_TRUNC
operator||
name|MSG_CTRUNC
operator|)
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
literal|"recvmsg() truncated data"
argument_list|)
expr_stmt|;
continue|continue;
block|}
endif|#
directive|endif
name|sockaddr
operator|=
name|msg
operator|.
name|msg_name
expr_stmt|;
name|socklen
operator|=
name|msg
operator|.
name|msg_namelen
expr_stmt|;
if|if
condition|(
name|socklen
operator|>
operator|(
name|socklen_t
operator|)
sizeof|sizeof
argument_list|(
name|ngx_sockaddr_t
argument_list|)
condition|)
block|{
name|socklen
operator|=
sizeof|sizeof
argument_list|(
name|ngx_sockaddr_t
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|socklen
operator|==
literal|0
condition|)
block|{
comment|/*              * on Linux recvmsg() returns zero msg_namelen              * when receiving packets from unbound AF_UNIX sockets              */
name|socklen
operator|=
sizeof|sizeof
argument_list|(
expr|struct
name|sockaddr
argument_list|)
expr_stmt|;
name|ngx_memzero
argument_list|(
operator|&
name|sa
argument_list|,
sizeof|sizeof
argument_list|(
expr|struct
name|sockaddr
argument_list|)
argument_list|)
expr_stmt|;
name|sa
operator|.
name|sockaddr
operator|.
name|sa_family
operator|=
name|ls
operator|->
name|sockaddr
operator|->
name|sa_family
expr_stmt|;
block|}
name|local_sockaddr
operator|=
name|ls
operator|->
name|sockaddr
expr_stmt|;
name|local_socklen
operator|=
name|ls
operator|->
name|socklen
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_HAVE_ADDRINFO_CMSG
operator|)
if|if
condition|(
name|ls
operator|->
name|wildcard
condition|)
block|{
name|struct
name|cmsghdr
modifier|*
name|cmsg
decl_stmt|;
name|ngx_memcpy
argument_list|(
operator|&
name|lsa
argument_list|,
name|local_sockaddr
argument_list|,
name|local_socklen
argument_list|)
expr_stmt|;
name|local_sockaddr
operator|=
operator|&
name|lsa
operator|.
name|sockaddr
expr_stmt|;
for|for
control|(
name|cmsg
operator|=
name|CMSG_FIRSTHDR
argument_list|(
operator|&
name|msg
argument_list|)
init|;
name|cmsg
operator|!=
name|NULL
condition|;
name|cmsg
operator|=
name|CMSG_NXTHDR
argument_list|(
operator|&
name|msg
argument_list|,
name|cmsg
argument_list|)
control|)
block|{
if|if
condition|(
name|ngx_get_srcaddr_cmsg
argument_list|(
name|cmsg
argument_list|,
name|local_sockaddr
argument_list|)
operator|==
name|NGX_OK
condition|)
block|{
break|break;
block|}
block|}
block|}
endif|#
directive|endif
name|c
operator|=
name|ngx_lookup_udp_connection
argument_list|(
name|ls
argument_list|,
name|sockaddr
argument_list|,
name|socklen
argument_list|,
name|local_sockaddr
argument_list|,
name|local_socklen
argument_list|)
expr_stmt|;
if|if
condition|(
name|c
condition|)
block|{
if|#
directive|if
operator|(
name|NGX_DEBUG
operator|)
if|if
condition|(
name|c
operator|->
name|log
operator|->
name|log_level
operator|&
name|NGX_LOG_DEBUG_EVENT
condition|)
block|{
name|ngx_log_handler_pt
name|handler
decl_stmt|;
name|handler
operator|=
name|c
operator|->
name|log
operator|->
name|handler
expr_stmt|;
name|c
operator|->
name|log
operator|->
name|handler
operator|=
name|NULL
expr_stmt|;
name|ngx_log_debug2
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|c
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"recvmsg: fd:%d n:%z"
argument_list|,
name|c
operator|->
name|fd
argument_list|,
name|n
argument_list|)
expr_stmt|;
name|c
operator|->
name|log
operator|->
name|handler
operator|=
name|handler
expr_stmt|;
block|}
endif|#
directive|endif
name|ngx_memzero
argument_list|(
operator|&
name|buf
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_buf_t
argument_list|)
argument_list|)
expr_stmt|;
name|buf
operator|.
name|pos
operator|=
name|buffer
expr_stmt|;
name|buf
operator|.
name|last
operator|=
name|buffer
operator|+
name|n
expr_stmt|;
name|rev
operator|=
name|c
operator|->
name|read
expr_stmt|;
name|c
operator|->
name|udp
operator|->
name|buffer
operator|=
operator|&
name|buf
expr_stmt|;
name|rev
operator|->
name|ready
operator|=
literal|1
expr_stmt|;
name|rev
operator|->
name|active
operator|=
literal|0
expr_stmt|;
name|rev
operator|->
name|handler
argument_list|(
name|rev
argument_list|)
expr_stmt|;
if|if
condition|(
name|c
operator|->
name|udp
condition|)
block|{
name|c
operator|->
name|udp
operator|->
name|buffer
operator|=
name|NULL
expr_stmt|;
block|}
name|rev
operator|->
name|ready
operator|=
literal|0
expr_stmt|;
name|rev
operator|->
name|active
operator|=
literal|1
expr_stmt|;
goto|goto
name|next
goto|;
block|}
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
name|ngx_stat_accepted
argument_list|,
literal|1
argument_list|)
expr_stmt|;
endif|#
directive|endif
name|ngx_accept_disabled
operator|=
name|ngx_cycle
operator|->
name|connection_n
operator|/
literal|8
operator|-
name|ngx_cycle
operator|->
name|free_connection_n
expr_stmt|;
name|c
operator|=
name|ngx_get_connection
argument_list|(
name|lc
operator|->
name|fd
argument_list|,
name|ev
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
return|return;
block|}
name|c
operator|->
name|shared
operator|=
literal|1
expr_stmt|;
name|c
operator|->
name|type
operator|=
name|SOCK_DGRAM
expr_stmt|;
name|c
operator|->
name|socklen
operator|=
name|socklen
expr_stmt|;
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
literal|1
argument_list|)
expr_stmt|;
endif|#
directive|endif
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
name|ev
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
name|ngx_close_accepted_udp_connection
argument_list|(
name|c
argument_list|)
expr_stmt|;
return|return;
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
name|ngx_close_accepted_udp_connection
argument_list|(
name|c
argument_list|)
expr_stmt|;
return|return;
block|}
name|ngx_memcpy
argument_list|(
name|c
operator|->
name|sockaddr
argument_list|,
name|sockaddr
argument_list|,
name|socklen
argument_list|)
expr_stmt|;
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
name|log
operator|==
name|NULL
condition|)
block|{
name|ngx_close_accepted_udp_connection
argument_list|(
name|c
argument_list|)
expr_stmt|;
return|return;
block|}
operator|*
name|log
operator|=
name|ls
operator|->
name|log
expr_stmt|;
name|c
operator|->
name|recv
operator|=
name|ngx_udp_shared_recv
expr_stmt|;
name|c
operator|->
name|send
operator|=
name|ngx_udp_send
expr_stmt|;
name|c
operator|->
name|send_chain
operator|=
name|ngx_udp_send_chain
expr_stmt|;
name|c
operator|->
name|need_flush_buf
operator|=
literal|1
expr_stmt|;
name|c
operator|->
name|log
operator|=
name|log
expr_stmt|;
name|c
operator|->
name|pool
operator|->
name|log
operator|=
name|log
expr_stmt|;
name|c
operator|->
name|listening
operator|=
name|ls
expr_stmt|;
if|if
condition|(
name|local_sockaddr
operator|==
operator|&
name|lsa
operator|.
name|sockaddr
condition|)
block|{
name|local_sockaddr
operator|=
name|ngx_palloc
argument_list|(
name|c
operator|->
name|pool
argument_list|,
name|local_socklen
argument_list|)
expr_stmt|;
if|if
condition|(
name|local_sockaddr
operator|==
name|NULL
condition|)
block|{
name|ngx_close_accepted_udp_connection
argument_list|(
name|c
argument_list|)
expr_stmt|;
return|return;
block|}
name|ngx_memcpy
argument_list|(
name|local_sockaddr
argument_list|,
operator|&
name|lsa
argument_list|,
name|local_socklen
argument_list|)
expr_stmt|;
block|}
name|c
operator|->
name|local_sockaddr
operator|=
name|local_sockaddr
expr_stmt|;
name|c
operator|->
name|local_socklen
operator|=
name|local_socklen
expr_stmt|;
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
name|n
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
name|ngx_close_accepted_udp_connection
argument_list|(
name|c
argument_list|)
expr_stmt|;
return|return;
block|}
name|c
operator|->
name|buffer
operator|->
name|last
operator|=
name|ngx_cpymem
argument_list|(
name|c
operator|->
name|buffer
operator|->
name|last
argument_list|,
name|buffer
argument_list|,
name|n
argument_list|)
expr_stmt|;
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
name|active
operator|=
literal|1
expr_stmt|;
name|wev
operator|->
name|ready
operator|=
literal|1
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
comment|/*          * TODO: MT: - ngx_atomic_fetch_add()          *             or protection by critical section or light mutex          *          * TODO: MP: - allocated in a shared memory          *           - ngx_atomic_fetch_add()          *             or protection by critical section or light mutex          */
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
name|c
operator|->
name|start_time
operator|=
name|ngx_current_msec
expr_stmt|;
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
name|ngx_stat_handled
argument_list|,
literal|1
argument_list|)
expr_stmt|;
endif|#
directive|endif
if|if
condition|(
name|ls
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
name|ngx_pnalloc
argument_list|(
name|c
operator|->
name|pool
argument_list|,
name|ls
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
name|ngx_close_accepted_udp_connection
argument_list|(
name|c
argument_list|)
expr_stmt|;
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
name|sockaddr
argument_list|,
name|c
operator|->
name|socklen
argument_list|,
name|c
operator|->
name|addr_text
operator|.
name|data
argument_list|,
name|ls
operator|->
name|addr_text_max_len
argument_list|,
literal|0
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
name|ngx_close_accepted_udp_connection
argument_list|(
name|c
argument_list|)
expr_stmt|;
return|return;
block|}
block|}
if|#
directive|if
operator|(
name|NGX_DEBUG
operator|)
block|{
name|ngx_str_t
name|addr
decl_stmt|;
name|u_char
name|text
index|[
name|NGX_SOCKADDR_STRLEN
index|]
decl_stmt|;
name|ngx_debug_accepted_connection
argument_list|(
name|ecf
argument_list|,
name|c
argument_list|)
expr_stmt|;
if|if
condition|(
name|log
operator|->
name|log_level
operator|&
name|NGX_LOG_DEBUG_EVENT
condition|)
block|{
name|addr
operator|.
name|data
operator|=
name|text
expr_stmt|;
name|addr
operator|.
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
name|ngx_log_debug4
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"*%uA recvmsg: %V fd:%d n:%z"
argument_list|,
name|c
operator|->
name|number
argument_list|,
operator|&
name|addr
argument_list|,
name|c
operator|->
name|fd
argument_list|,
name|n
argument_list|)
expr_stmt|;
block|}
block|}
endif|#
directive|endif
if|if
condition|(
name|ngx_insert_udp_connection
argument_list|(
name|c
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
name|ngx_close_accepted_udp_connection
argument_list|(
name|c
argument_list|)
expr_stmt|;
return|return;
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
name|handler
argument_list|(
name|c
argument_list|)
expr_stmt|;
name|next
label|:
if|if
condition|(
name|ngx_event_flags
operator|&
name|NGX_USE_KQUEUE_EVENT
condition|)
block|{
name|ev
operator|->
name|available
operator|-=
name|n
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
block|}
end_function

begin_function
specifier|static
name|void
DECL|function|ngx_close_accepted_udp_connection (ngx_connection_t * c)
name|ngx_close_accepted_udp_connection
parameter_list|(
name|ngx_connection_t
modifier|*
name|c
parameter_list|)
block|{
name|ngx_free_connection
argument_list|(
name|c
argument_list|)
expr_stmt|;
name|c
operator|->
name|fd
operator|=
operator|(
name|ngx_socket_t
operator|)
operator|-
literal|1
expr_stmt|;
if|if
condition|(
name|c
operator|->
name|pool
condition|)
block|{
name|ngx_destroy_pool
argument_list|(
name|c
operator|->
name|pool
argument_list|)
expr_stmt|;
block|}
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
block|}
end_function

begin_function
specifier|static
name|ssize_t
DECL|function|ngx_udp_shared_recv (ngx_connection_t * c,u_char * buf,size_t size)
name|ngx_udp_shared_recv
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
block|{
name|ssize_t
name|n
decl_stmt|;
name|ngx_buf_t
modifier|*
name|b
decl_stmt|;
if|if
condition|(
name|c
operator|->
name|udp
operator|==
name|NULL
operator|||
name|c
operator|->
name|udp
operator|->
name|buffer
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_AGAIN
return|;
block|}
name|b
operator|=
name|c
operator|->
name|udp
operator|->
name|buffer
expr_stmt|;
name|n
operator|=
name|ngx_min
argument_list|(
name|b
operator|->
name|last
operator|-
name|b
operator|->
name|pos
argument_list|,
operator|(
name|ssize_t
operator|)
name|size
argument_list|)
expr_stmt|;
name|ngx_memcpy
argument_list|(
name|buf
argument_list|,
name|b
operator|->
name|pos
argument_list|,
name|n
argument_list|)
expr_stmt|;
name|c
operator|->
name|udp
operator|->
name|buffer
operator|=
name|NULL
expr_stmt|;
name|c
operator|->
name|read
operator|->
name|ready
operator|=
literal|0
expr_stmt|;
name|c
operator|->
name|read
operator|->
name|active
operator|=
literal|1
expr_stmt|;
return|return
name|n
return|;
block|}
end_function

begin_function
name|void
DECL|function|ngx_udp_rbtree_insert_value (ngx_rbtree_node_t * temp,ngx_rbtree_node_t * node,ngx_rbtree_node_t * sentinel)
name|ngx_udp_rbtree_insert_value
parameter_list|(
name|ngx_rbtree_node_t
modifier|*
name|temp
parameter_list|,
name|ngx_rbtree_node_t
modifier|*
name|node
parameter_list|,
name|ngx_rbtree_node_t
modifier|*
name|sentinel
parameter_list|)
block|{
name|ngx_int_t
name|rc
decl_stmt|;
name|ngx_connection_t
modifier|*
name|c
decl_stmt|,
modifier|*
name|ct
decl_stmt|;
name|ngx_rbtree_node_t
modifier|*
modifier|*
name|p
decl_stmt|;
name|ngx_udp_connection_t
modifier|*
name|udp
decl_stmt|,
modifier|*
name|udpt
decl_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
if|if
condition|(
name|node
operator|->
name|key
operator|<
name|temp
operator|->
name|key
condition|)
block|{
name|p
operator|=
operator|&
name|temp
operator|->
name|left
expr_stmt|;
block|}
if|else if
condition|(
name|node
operator|->
name|key
operator|>
name|temp
operator|->
name|key
condition|)
block|{
name|p
operator|=
operator|&
name|temp
operator|->
name|right
expr_stmt|;
block|}
else|else
block|{
comment|/* node->key == temp->key */
name|udp
operator|=
operator|(
name|ngx_udp_connection_t
operator|*
operator|)
name|node
expr_stmt|;
name|c
operator|=
name|udp
operator|->
name|connection
expr_stmt|;
name|udpt
operator|=
operator|(
name|ngx_udp_connection_t
operator|*
operator|)
name|temp
expr_stmt|;
name|ct
operator|=
name|udpt
operator|->
name|connection
expr_stmt|;
name|rc
operator|=
name|ngx_cmp_sockaddr
argument_list|(
name|c
operator|->
name|sockaddr
argument_list|,
name|c
operator|->
name|socklen
argument_list|,
name|ct
operator|->
name|sockaddr
argument_list|,
name|ct
operator|->
name|socklen
argument_list|,
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|rc
operator|==
literal|0
operator|&&
name|c
operator|->
name|listening
operator|->
name|wildcard
condition|)
block|{
name|rc
operator|=
name|ngx_cmp_sockaddr
argument_list|(
name|c
operator|->
name|local_sockaddr
argument_list|,
name|c
operator|->
name|local_socklen
argument_list|,
name|ct
operator|->
name|local_sockaddr
argument_list|,
name|ct
operator|->
name|local_socklen
argument_list|,
literal|1
argument_list|)
expr_stmt|;
block|}
name|p
operator|=
operator|(
name|rc
operator|<
literal|0
operator|)
condition|?
operator|&
name|temp
operator|->
name|left
else|:
operator|&
name|temp
operator|->
name|right
expr_stmt|;
block|}
if|if
condition|(
operator|*
name|p
operator|==
name|sentinel
condition|)
block|{
break|break;
block|}
name|temp
operator|=
operator|*
name|p
expr_stmt|;
block|}
operator|*
name|p
operator|=
name|node
expr_stmt|;
name|node
operator|->
name|parent
operator|=
name|temp
expr_stmt|;
name|node
operator|->
name|left
operator|=
name|sentinel
expr_stmt|;
name|node
operator|->
name|right
operator|=
name|sentinel
expr_stmt|;
name|ngx_rbt_red
argument_list|(
name|node
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_insert_udp_connection (ngx_connection_t * c)
name|ngx_insert_udp_connection
parameter_list|(
name|ngx_connection_t
modifier|*
name|c
parameter_list|)
block|{
name|uint32_t
name|hash
decl_stmt|;
name|ngx_pool_cleanup_t
modifier|*
name|cln
decl_stmt|;
name|ngx_udp_connection_t
modifier|*
name|udp
decl_stmt|;
if|if
condition|(
name|c
operator|->
name|udp
condition|)
block|{
return|return
name|NGX_OK
return|;
block|}
name|udp
operator|=
name|ngx_pcalloc
argument_list|(
name|c
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_udp_connection_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|udp
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|udp
operator|->
name|connection
operator|=
name|c
expr_stmt|;
name|ngx_crc32_init
argument_list|(
name|hash
argument_list|)
expr_stmt|;
name|ngx_crc32_update
argument_list|(
operator|&
name|hash
argument_list|,
operator|(
name|u_char
operator|*
operator|)
name|c
operator|->
name|sockaddr
argument_list|,
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
name|wildcard
condition|)
block|{
name|ngx_crc32_update
argument_list|(
operator|&
name|hash
argument_list|,
operator|(
name|u_char
operator|*
operator|)
name|c
operator|->
name|local_sockaddr
argument_list|,
name|c
operator|->
name|local_socklen
argument_list|)
expr_stmt|;
block|}
name|ngx_crc32_final
argument_list|(
name|hash
argument_list|)
expr_stmt|;
name|udp
operator|->
name|node
operator|.
name|key
operator|=
name|hash
expr_stmt|;
name|cln
operator|=
name|ngx_pool_cleanup_add
argument_list|(
name|c
operator|->
name|pool
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|cln
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|cln
operator|->
name|data
operator|=
name|c
expr_stmt|;
name|cln
operator|->
name|handler
operator|=
name|ngx_delete_udp_connection
expr_stmt|;
name|ngx_rbtree_insert
argument_list|(
operator|&
name|c
operator|->
name|listening
operator|->
name|rbtree
argument_list|,
operator|&
name|udp
operator|->
name|node
argument_list|)
expr_stmt|;
name|c
operator|->
name|udp
operator|=
name|udp
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
name|void
DECL|function|ngx_delete_udp_connection (void * data)
name|ngx_delete_udp_connection
parameter_list|(
name|void
modifier|*
name|data
parameter_list|)
block|{
name|ngx_connection_t
modifier|*
name|c
init|=
name|data
decl_stmt|;
if|if
condition|(
name|c
operator|->
name|udp
operator|==
name|NULL
condition|)
block|{
return|return;
block|}
name|ngx_rbtree_delete
argument_list|(
operator|&
name|c
operator|->
name|listening
operator|->
name|rbtree
argument_list|,
operator|&
name|c
operator|->
name|udp
operator|->
name|node
argument_list|)
expr_stmt|;
name|c
operator|->
name|udp
operator|=
name|NULL
expr_stmt|;
block|}
end_function

begin_function
specifier|static
name|ngx_connection_t
modifier|*
DECL|function|ngx_lookup_udp_connection (ngx_listening_t * ls,struct sockaddr * sockaddr,socklen_t socklen,struct sockaddr * local_sockaddr,socklen_t local_socklen)
name|ngx_lookup_udp_connection
parameter_list|(
name|ngx_listening_t
modifier|*
name|ls
parameter_list|,
name|struct
name|sockaddr
modifier|*
name|sockaddr
parameter_list|,
name|socklen_t
name|socklen
parameter_list|,
name|struct
name|sockaddr
modifier|*
name|local_sockaddr
parameter_list|,
name|socklen_t
name|local_socklen
parameter_list|)
block|{
name|uint32_t
name|hash
decl_stmt|;
name|ngx_int_t
name|rc
decl_stmt|;
name|ngx_connection_t
modifier|*
name|c
decl_stmt|;
name|ngx_rbtree_node_t
modifier|*
name|node
decl_stmt|,
modifier|*
name|sentinel
decl_stmt|;
name|ngx_udp_connection_t
modifier|*
name|udp
decl_stmt|;
if|#
directive|if
operator|(
name|NGX_HAVE_UNIX_DOMAIN
operator|)
if|if
condition|(
name|sockaddr
operator|->
name|sa_family
operator|==
name|AF_UNIX
condition|)
block|{
name|struct
name|sockaddr_un
modifier|*
name|saun
init|=
operator|(
expr|struct
name|sockaddr_un
operator|*
operator|)
name|sockaddr
decl_stmt|;
if|if
condition|(
name|socklen
operator|<=
operator|(
name|socklen_t
operator|)
name|offsetof
argument_list|(
expr|struct
name|sockaddr_un
argument_list|,
name|sun_path
argument_list|)
operator|||
name|saun
operator|->
name|sun_path
index|[
literal|0
index|]
operator|==
literal|'\0'
condition|)
block|{
name|ngx_log_debug0
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|ngx_cycle
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"unbound unix socket"
argument_list|)
expr_stmt|;
return|return
name|NULL
return|;
block|}
block|}
endif|#
directive|endif
name|node
operator|=
name|ls
operator|->
name|rbtree
operator|.
name|root
expr_stmt|;
name|sentinel
operator|=
name|ls
operator|->
name|rbtree
operator|.
name|sentinel
expr_stmt|;
name|ngx_crc32_init
argument_list|(
name|hash
argument_list|)
expr_stmt|;
name|ngx_crc32_update
argument_list|(
operator|&
name|hash
argument_list|,
operator|(
name|u_char
operator|*
operator|)
name|sockaddr
argument_list|,
name|socklen
argument_list|)
expr_stmt|;
if|if
condition|(
name|ls
operator|->
name|wildcard
condition|)
block|{
name|ngx_crc32_update
argument_list|(
operator|&
name|hash
argument_list|,
operator|(
name|u_char
operator|*
operator|)
name|local_sockaddr
argument_list|,
name|local_socklen
argument_list|)
expr_stmt|;
block|}
name|ngx_crc32_final
argument_list|(
name|hash
argument_list|)
expr_stmt|;
while|while
condition|(
name|node
operator|!=
name|sentinel
condition|)
block|{
if|if
condition|(
name|hash
operator|<
name|node
operator|->
name|key
condition|)
block|{
name|node
operator|=
name|node
operator|->
name|left
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|hash
operator|>
name|node
operator|->
name|key
condition|)
block|{
name|node
operator|=
name|node
operator|->
name|right
expr_stmt|;
continue|continue;
block|}
comment|/* hash == node->key */
name|udp
operator|=
operator|(
name|ngx_udp_connection_t
operator|*
operator|)
name|node
expr_stmt|;
name|c
operator|=
name|udp
operator|->
name|connection
expr_stmt|;
name|rc
operator|=
name|ngx_cmp_sockaddr
argument_list|(
name|sockaddr
argument_list|,
name|socklen
argument_list|,
name|c
operator|->
name|sockaddr
argument_list|,
name|c
operator|->
name|socklen
argument_list|,
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|rc
operator|==
literal|0
operator|&&
name|ls
operator|->
name|wildcard
condition|)
block|{
name|rc
operator|=
name|ngx_cmp_sockaddr
argument_list|(
name|local_sockaddr
argument_list|,
name|local_socklen
argument_list|,
name|c
operator|->
name|local_sockaddr
argument_list|,
name|c
operator|->
name|local_socklen
argument_list|,
literal|1
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|rc
operator|==
literal|0
condition|)
block|{
return|return
name|c
return|;
block|}
name|node
operator|=
operator|(
name|rc
operator|<
literal|0
operator|)
condition|?
name|node
operator|->
name|left
else|:
name|node
operator|->
name|right
expr_stmt|;
block|}
return|return
name|NULL
return|;
block|}
end_function

begin_else
else|#
directive|else
end_else

begin_function
name|void
DECL|function|ngx_delete_udp_connection (void * data)
name|ngx_delete_udp_connection
parameter_list|(
name|void
modifier|*
name|data
parameter_list|)
block|{
return|return;
block|}
end_function

begin_endif
endif|#
directive|endif
end_endif

end_unit

