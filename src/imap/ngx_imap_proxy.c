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

begin_include
include|#
directive|include
file|<ngx_imap.h>
end_include

begin_function_decl
specifier|static
name|void
name|ngx_imap_proxy_block_read
parameter_list|(
name|ngx_event_t
modifier|*
name|rev
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|void
name|ngx_imap_proxy_auth_handler
parameter_list|(
name|ngx_event_t
modifier|*
name|rev
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|void
name|ngx_imap_proxy_init_handler
parameter_list|(
name|ngx_event_t
modifier|*
name|wev
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|void
name|ngx_imap_proxy_dummy_handler
parameter_list|(
name|ngx_event_t
modifier|*
name|ev
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_imap_proxy_read_response
parameter_list|(
name|ngx_imap_session_t
modifier|*
name|s
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|void
name|ngx_imap_proxy_handler
parameter_list|(
name|ngx_event_t
modifier|*
name|ev
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|void
name|ngx_imap_proxy_close_session
parameter_list|(
name|ngx_imap_session_t
modifier|*
name|s
parameter_list|)
function_decl|;
end_function_decl

begin_function
DECL|function|ngx_imap_proxy_init (ngx_imap_session_t * s)
name|void
name|ngx_imap_proxy_init
parameter_list|(
name|ngx_imap_session_t
modifier|*
name|s
parameter_list|)
block|{
name|ngx_int_t
name|rc
decl_stmt|;
name|ngx_peers_t
modifier|*
name|peers
decl_stmt|;
name|ngx_imap_proxy_ctx_t
modifier|*
name|p
decl_stmt|;
if|if
condition|(
operator|!
operator|(
name|p
operator|=
name|ngx_pcalloc
argument_list|(
name|s
operator|->
name|connection
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_imap_proxy_ctx_t
argument_list|)
argument_list|)
operator|)
condition|)
block|{
name|ngx_imap_close_connection
argument_list|(
name|s
operator|->
name|connection
argument_list|)
expr_stmt|;
return|return;
block|}
name|s
operator|->
name|proxy
operator|=
name|p
expr_stmt|;
comment|/**/
if|if
condition|(
operator|!
operator|(
name|peers
operator|=
name|ngx_pcalloc
argument_list|(
name|s
operator|->
name|connection
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_peers_t
argument_list|)
argument_list|)
operator|)
condition|)
block|{
name|ngx_imap_close_connection
argument_list|(
name|s
operator|->
name|connection
argument_list|)
expr_stmt|;
return|return;
block|}
name|p
operator|->
name|upstream
operator|.
name|peers
operator|=
name|peers
expr_stmt|;
name|p
operator|->
name|upstream
operator|.
name|log
operator|=
name|s
operator|->
name|connection
operator|->
name|log
expr_stmt|;
name|p
operator|->
name|upstream
operator|.
name|log_error
operator|=
name|NGX_ERROR_ERR
expr_stmt|;
name|peers
operator|->
name|number
operator|=
literal|1
expr_stmt|;
name|peers
operator|->
name|max_fails
operator|=
literal|1
expr_stmt|;
if|#
directive|if
literal|0
block_content|peers->peers[0].addr = inet_addr("81.19.69.70");     peers->peers[0].addr_port_text.len = sizeof("81.19.69.70:110") - 1;     peers->peers[0].addr_port_text.data = (u_char *) "81.19.69.70:110";     peers->peers[0].port = htons(110);
else|#
directive|else
name|peers
operator|->
name|peers
index|[
literal|0
index|]
operator|.
name|addr
operator|=
name|inet_addr
argument_list|(
literal|"81.19.64.101"
argument_list|)
expr_stmt|;
name|peers
operator|->
name|peers
index|[
literal|0
index|]
operator|.
name|addr_port_text
operator|.
name|len
operator|=
sizeof|sizeof
argument_list|(
literal|"81.19.64.101:110"
argument_list|)
operator|-
literal|1
expr_stmt|;
name|peers
operator|->
name|peers
index|[
literal|0
index|]
operator|.
name|addr_port_text
operator|.
name|data
operator|=
operator|(
name|u_char
operator|*
operator|)
literal|"81.19.64.101:110"
expr_stmt|;
name|peers
operator|->
name|peers
index|[
literal|0
index|]
operator|.
name|port
operator|=
name|htons
argument_list|(
literal|110
argument_list|)
expr_stmt|;
endif|#
directive|endif
name|rc
operator|=
name|ngx_event_connect_peer
argument_list|(
operator|&
name|p
operator|->
name|upstream
argument_list|)
expr_stmt|;
if|if
condition|(
name|rc
operator|==
name|NGX_ERROR
condition|)
block|{
name|ngx_imap_proxy_close_session
argument_list|(
name|s
argument_list|)
expr_stmt|;
return|return;
block|}
name|p
operator|->
name|upstream
operator|.
name|connection
operator|->
name|data
operator|=
name|s
expr_stmt|;
name|p
operator|->
name|upstream
operator|.
name|connection
operator|->
name|pool
operator|=
name|s
operator|->
name|connection
operator|->
name|pool
expr_stmt|;
name|s
operator|->
name|connection
operator|->
name|read
operator|->
name|event_handler
operator|=
name|ngx_imap_proxy_block_read
expr_stmt|;
name|p
operator|->
name|upstream
operator|.
name|connection
operator|->
name|read
operator|->
name|event_handler
operator|=
name|ngx_imap_proxy_auth_handler
expr_stmt|;
name|p
operator|->
name|upstream
operator|.
name|connection
operator|->
name|write
operator|->
name|event_handler
operator|=
name|ngx_imap_proxy_dummy_handler
expr_stmt|;
block|}
end_function

begin_function
DECL|function|ngx_imap_proxy_block_read (ngx_event_t * rev)
specifier|static
name|void
name|ngx_imap_proxy_block_read
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
name|ngx_imap_session_t
modifier|*
name|s
decl_stmt|;
name|ngx_log_debug0
argument_list|(
name|NGX_LOG_DEBUG_IMAP
argument_list|,
name|rev
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"imap proxy block read"
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
operator|==
name|NGX_ERROR
condition|)
block|{
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
name|ngx_imap_proxy_close_session
argument_list|(
name|s
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|ngx_imap_proxy_auth_handler (ngx_event_t * rev)
specifier|static
name|void
name|ngx_imap_proxy_auth_handler
parameter_list|(
name|ngx_event_t
modifier|*
name|rev
parameter_list|)
block|{
name|u_char
modifier|*
name|p
decl_stmt|;
name|ngx_int_t
name|rc
decl_stmt|;
name|ngx_str_t
name|line
decl_stmt|;
name|ngx_connection_t
modifier|*
name|c
decl_stmt|;
name|ngx_imap_session_t
modifier|*
name|s
decl_stmt|;
name|ngx_log_debug0
argument_list|(
name|NGX_LOG_DEBUG_IMAP
argument_list|,
name|rev
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"imap proxy auth handler"
argument_list|)
expr_stmt|;
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
name|ngx_imap_proxy_close_session
argument_list|(
name|s
argument_list|)
expr_stmt|;
return|return;
block|}
if|if
condition|(
name|s
operator|->
name|proxy
operator|->
name|buffer
operator|==
name|NULL
condition|)
block|{
name|s
operator|->
name|proxy
operator|->
name|buffer
operator|=
name|ngx_create_temp_buf
argument_list|(
name|c
operator|->
name|pool
argument_list|,
comment|/* STUB */
literal|4096
argument_list|)
expr_stmt|;
if|if
condition|(
name|s
operator|->
name|proxy
operator|->
name|buffer
operator|==
name|NULL
condition|)
block|{
name|ngx_imap_proxy_close_session
argument_list|(
name|s
argument_list|)
expr_stmt|;
return|return;
block|}
block|}
name|rc
operator|=
name|ngx_imap_proxy_read_response
argument_list|(
name|s
argument_list|)
expr_stmt|;
if|if
condition|(
name|rc
operator|==
name|NGX_AGAIN
condition|)
block|{
return|return;
block|}
if|if
condition|(
name|rc
operator|==
name|NGX_ERROR
condition|)
block|{
comment|/* TODO: ngx_imap_proxy_finalize_session(s, NGX_IMAP_INTERNAL_ERROR) */
name|ngx_imap_proxy_close_session
argument_list|(
name|s
argument_list|)
expr_stmt|;
return|return;
block|}
if|if
condition|(
name|s
operator|->
name|imap_state
operator|==
name|ngx_pop3_start
condition|)
block|{
name|ngx_log_debug0
argument_list|(
name|NGX_LOG_DEBUG_IMAP
argument_list|,
name|rev
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"imap proxy send user"
argument_list|)
expr_stmt|;
name|line
operator|.
name|len
operator|=
sizeof|sizeof
argument_list|(
literal|"USER "
argument_list|)
operator|+
name|s
operator|->
name|login
operator|.
name|len
operator|-
literal|1
operator|+
literal|2
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|line
operator|.
name|data
operator|=
name|ngx_palloc
argument_list|(
name|c
operator|->
name|pool
argument_list|,
name|line
operator|.
name|len
argument_list|)
operator|)
condition|)
block|{
name|ngx_imap_proxy_close_session
argument_list|(
name|s
argument_list|)
expr_stmt|;
return|return;
block|}
name|p
operator|=
name|ngx_cpymem
argument_list|(
name|line
operator|.
name|data
argument_list|,
literal|"USER "
argument_list|,
sizeof|sizeof
argument_list|(
literal|"USER "
argument_list|)
operator|-
literal|1
argument_list|)
expr_stmt|;
name|p
operator|=
name|ngx_cpymem
argument_list|(
name|p
argument_list|,
name|s
operator|->
name|login
operator|.
name|data
argument_list|,
name|s
operator|->
name|login
operator|.
name|len
argument_list|)
expr_stmt|;
operator|*
name|p
operator|++
operator|=
name|CR
expr_stmt|;
operator|*
name|p
operator|++
operator|=
name|LF
expr_stmt|;
if|if
condition|(
name|ngx_send
argument_list|(
name|c
argument_list|,
name|line
operator|.
name|data
argument_list|,
name|line
operator|.
name|len
argument_list|)
operator|<
operator|(
name|ssize_t
operator|)
name|line
operator|.
name|len
condition|)
block|{
comment|/*              * we treat the incomplete sending as NGX_ERROR              * because it is very strange here              */
name|ngx_imap_close_connection
argument_list|(
name|c
argument_list|)
expr_stmt|;
return|return;
block|}
name|s
operator|->
name|imap_state
operator|=
name|ngx_pop3_user
expr_stmt|;
name|s
operator|->
name|proxy
operator|->
name|buffer
operator|->
name|pos
operator|=
name|s
operator|->
name|proxy
operator|->
name|buffer
operator|->
name|start
expr_stmt|;
name|s
operator|->
name|proxy
operator|->
name|buffer
operator|->
name|last
operator|=
name|s
operator|->
name|proxy
operator|->
name|buffer
operator|->
name|start
expr_stmt|;
return|return;
block|}
name|ngx_log_debug0
argument_list|(
name|NGX_LOG_DEBUG_IMAP
argument_list|,
name|rev
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"imap proxy send pass"
argument_list|)
expr_stmt|;
name|line
operator|.
name|len
operator|=
sizeof|sizeof
argument_list|(
literal|"PASS "
argument_list|)
operator|+
name|s
operator|->
name|passwd
operator|.
name|len
operator|-
literal|1
operator|+
literal|2
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|line
operator|.
name|data
operator|=
name|ngx_palloc
argument_list|(
name|c
operator|->
name|pool
argument_list|,
name|line
operator|.
name|len
argument_list|)
operator|)
condition|)
block|{
name|ngx_imap_proxy_close_session
argument_list|(
name|s
argument_list|)
expr_stmt|;
return|return;
block|}
name|p
operator|=
name|ngx_cpymem
argument_list|(
name|line
operator|.
name|data
argument_list|,
literal|"PASS "
argument_list|,
sizeof|sizeof
argument_list|(
literal|"PASS "
argument_list|)
operator|-
literal|1
argument_list|)
expr_stmt|;
name|p
operator|=
name|ngx_cpymem
argument_list|(
name|p
argument_list|,
name|s
operator|->
name|passwd
operator|.
name|data
argument_list|,
name|s
operator|->
name|passwd
operator|.
name|len
argument_list|)
expr_stmt|;
operator|*
name|p
operator|++
operator|=
name|CR
expr_stmt|;
operator|*
name|p
operator|++
operator|=
name|LF
expr_stmt|;
if|if
condition|(
name|ngx_send
argument_list|(
name|c
argument_list|,
name|line
operator|.
name|data
argument_list|,
name|line
operator|.
name|len
argument_list|)
operator|<
operator|(
name|ssize_t
operator|)
name|line
operator|.
name|len
condition|)
block|{
comment|/*          * we treat the incomplete sending as NGX_ERROR          * because it is very strange here          */
name|ngx_imap_close_connection
argument_list|(
name|c
argument_list|)
expr_stmt|;
return|return;
block|}
name|s
operator|->
name|proxy
operator|->
name|buffer
operator|->
name|pos
operator|=
name|s
operator|->
name|proxy
operator|->
name|buffer
operator|->
name|start
expr_stmt|;
name|s
operator|->
name|proxy
operator|->
name|buffer
operator|->
name|last
operator|=
name|s
operator|->
name|proxy
operator|->
name|buffer
operator|->
name|start
expr_stmt|;
name|s
operator|->
name|connection
operator|->
name|read
operator|->
name|event_handler
operator|=
name|ngx_imap_proxy_handler
expr_stmt|;
name|s
operator|->
name|connection
operator|->
name|write
operator|->
name|event_handler
operator|=
name|ngx_imap_proxy_handler
expr_stmt|;
name|rev
operator|->
name|event_handler
operator|=
name|ngx_imap_proxy_handler
expr_stmt|;
name|c
operator|->
name|write
operator|->
name|event_handler
operator|=
name|ngx_imap_proxy_handler
expr_stmt|;
block|}
end_function

begin_function
DECL|function|ngx_imap_proxy_dummy_handler (ngx_event_t * ev)
specifier|static
name|void
name|ngx_imap_proxy_dummy_handler
parameter_list|(
name|ngx_event_t
modifier|*
name|ev
parameter_list|)
block|{
name|ngx_log_debug0
argument_list|(
name|NGX_LOG_DEBUG_IMAP
argument_list|,
name|ev
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"imap proxy dummy handler"
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|ngx_imap_proxy_read_response (ngx_imap_session_t * s)
specifier|static
name|ngx_int_t
name|ngx_imap_proxy_read_response
parameter_list|(
name|ngx_imap_session_t
modifier|*
name|s
parameter_list|)
block|{
name|u_char
modifier|*
name|p
decl_stmt|;
name|ssize_t
name|n
decl_stmt|;
name|ngx_buf_t
modifier|*
name|b
decl_stmt|;
name|b
operator|=
name|s
operator|->
name|proxy
operator|->
name|buffer
expr_stmt|;
name|n
operator|=
name|ngx_recv
argument_list|(
name|s
operator|->
name|proxy
operator|->
name|upstream
operator|.
name|connection
argument_list|,
name|b
operator|->
name|last
argument_list|,
name|b
operator|->
name|end
operator|-
name|b
operator|->
name|last
argument_list|)
expr_stmt|;
if|if
condition|(
name|n
operator|==
name|NGX_ERROR
operator|||
name|n
operator|==
literal|0
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|n
operator|==
name|NGX_AGAIN
condition|)
block|{
return|return
name|NGX_AGAIN
return|;
block|}
name|b
operator|->
name|last
operator|+=
name|n
expr_stmt|;
if|if
condition|(
name|b
operator|->
name|last
operator|-
name|b
operator|->
name|pos
operator|<
literal|5
condition|)
block|{
return|return
name|NGX_AGAIN
return|;
block|}
if|if
condition|(
operator|*
operator|(
name|b
operator|->
name|last
operator|-
literal|2
operator|)
operator|!=
name|CR
operator|||
operator|*
operator|(
name|b
operator|->
name|last
operator|-
literal|1
operator|)
operator|!=
name|LF
condition|)
block|{
if|if
condition|(
name|b
operator|->
name|last
operator|==
name|b
operator|->
name|end
condition|)
block|{
operator|*
operator|(
name|b
operator|->
name|last
operator|-
literal|1
operator|)
operator|=
literal|'\0'
expr_stmt|;
name|ngx_log_error
argument_list|(
name|NGX_LOG_ERR
argument_list|,
name|s
operator|->
name|connection
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"upstream sent too long response line: \"%s\""
argument_list|,
name|b
operator|->
name|pos
argument_list|)
expr_stmt|;
return|return
name|NGX_IMAP_PROXY_INVALID
return|;
block|}
return|return
name|NGX_AGAIN
return|;
block|}
name|p
operator|=
name|b
operator|->
name|pos
expr_stmt|;
if|if
condition|(
name|p
index|[
literal|0
index|]
operator|==
literal|'+'
operator|&&
name|p
index|[
literal|1
index|]
operator|==
literal|'O'
operator|&&
name|p
index|[
literal|2
index|]
operator|==
literal|'K'
condition|)
block|{
return|return
name|NGX_OK
return|;
block|}
if|if
condition|(
name|p
index|[
literal|0
index|]
operator|==
literal|'-'
operator|&&
name|p
index|[
literal|1
index|]
operator|==
literal|'E'
operator|&&
name|p
index|[
literal|2
index|]
operator|==
literal|'R'
operator|&&
name|p
index|[
literal|3
index|]
operator|==
literal|'R'
condition|)
block|{
return|return
name|NGX_IMAP_PROXY_ERROR
return|;
block|}
operator|*
operator|(
name|b
operator|->
name|last
operator|-
literal|2
operator|)
operator|=
literal|'\0'
expr_stmt|;
name|ngx_log_error
argument_list|(
name|NGX_LOG_ERR
argument_list|,
name|s
operator|->
name|connection
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"upstream sent invalid greeting line: \"%s\""
argument_list|,
name|p
argument_list|)
expr_stmt|;
return|return
name|NGX_IMAP_PROXY_INVALID
return|;
block|}
end_function

begin_function
DECL|function|ngx_imap_proxy_handler (ngx_event_t * ev)
specifier|static
name|void
name|ngx_imap_proxy_handler
parameter_list|(
name|ngx_event_t
modifier|*
name|ev
parameter_list|)
block|{
name|size_t
name|size
decl_stmt|;
name|ssize_t
name|n
decl_stmt|;
name|ngx_buf_t
modifier|*
name|b
decl_stmt|;
name|ngx_uint_t
name|again
decl_stmt|,
name|do_write
decl_stmt|;
name|ngx_connection_t
modifier|*
name|c
decl_stmt|,
modifier|*
name|src
decl_stmt|,
modifier|*
name|dst
decl_stmt|;
name|ngx_imap_session_t
modifier|*
name|s
decl_stmt|;
name|c
operator|=
name|ev
operator|->
name|data
expr_stmt|;
name|s
operator|=
name|c
operator|->
name|data
expr_stmt|;
if|if
condition|(
name|ev
operator|->
name|timedout
condition|)
block|{
if|if
condition|(
name|c
operator|==
name|s
operator|->
name|connection
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
block|}
else|else
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
literal|"upstream timed out"
argument_list|)
expr_stmt|;
block|}
name|ngx_imap_proxy_close_session
argument_list|(
name|s
argument_list|)
expr_stmt|;
return|return;
block|}
if|if
condition|(
name|c
operator|==
name|s
operator|->
name|connection
condition|)
block|{
if|if
condition|(
name|ev
operator|->
name|write
condition|)
block|{
name|src
operator|=
name|s
operator|->
name|proxy
operator|->
name|upstream
operator|.
name|connection
expr_stmt|;
name|dst
operator|=
name|c
expr_stmt|;
name|b
operator|=
name|s
operator|->
name|proxy
operator|->
name|buffer
expr_stmt|;
block|}
else|else
block|{
name|src
operator|=
name|c
expr_stmt|;
name|dst
operator|=
name|s
operator|->
name|proxy
operator|->
name|upstream
operator|.
name|connection
expr_stmt|;
name|b
operator|=
name|s
operator|->
name|buffer
expr_stmt|;
block|}
block|}
else|else
block|{
if|if
condition|(
name|ev
operator|->
name|write
condition|)
block|{
name|src
operator|=
name|s
operator|->
name|connection
expr_stmt|;
name|dst
operator|=
name|c
expr_stmt|;
name|b
operator|=
name|s
operator|->
name|buffer
expr_stmt|;
block|}
else|else
block|{
name|src
operator|=
name|c
expr_stmt|;
name|dst
operator|=
name|s
operator|->
name|connection
expr_stmt|;
name|b
operator|=
name|s
operator|->
name|proxy
operator|->
name|buffer
expr_stmt|;
block|}
block|}
name|do_write
operator|=
name|ev
operator|->
name|write
condition|?
literal|1
else|:
literal|0
expr_stmt|;
name|ngx_log_debug3
argument_list|(
name|NGX_LOG_DEBUG_IMAP
argument_list|,
name|ev
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"imap proxy handler: %d, #%d> #%d"
argument_list|,
name|do_write
argument_list|,
name|src
operator|->
name|fd
argument_list|,
name|dst
operator|->
name|fd
argument_list|)
expr_stmt|;
do|do
block|{
name|again
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|do_write
operator|==
literal|1
condition|)
block|{
name|size
operator|=
name|b
operator|->
name|last
operator|-
name|b
operator|->
name|pos
expr_stmt|;
if|if
condition|(
name|size
operator|&&
name|dst
operator|->
name|write
operator|->
name|ready
condition|)
block|{
name|n
operator|=
name|ngx_send
argument_list|(
name|dst
argument_list|,
name|b
operator|->
name|pos
argument_list|,
name|size
argument_list|)
expr_stmt|;
if|if
condition|(
name|n
operator|==
name|NGX_ERROR
condition|)
block|{
name|ngx_imap_proxy_close_session
argument_list|(
name|s
argument_list|)
expr_stmt|;
return|return;
block|}
if|if
condition|(
name|n
operator|>
literal|0
condition|)
block|{
name|again
operator|=
literal|1
expr_stmt|;
name|b
operator|->
name|pos
operator|+=
name|n
expr_stmt|;
if|if
condition|(
name|b
operator|->
name|pos
operator|==
name|b
operator|->
name|last
condition|)
block|{
name|b
operator|->
name|pos
operator|=
name|b
operator|->
name|start
expr_stmt|;
name|b
operator|->
name|last
operator|=
name|b
operator|->
name|start
expr_stmt|;
block|}
block|}
if|if
condition|(
name|n
operator|==
name|NGX_AGAIN
operator|||
name|n
operator|<
operator|(
name|ssize_t
operator|)
name|size
condition|)
block|{
if|if
condition|(
name|ngx_handle_write_event
argument_list|(
name|dst
operator|->
name|write
argument_list|,
comment|/* TODO: LOWAT */
literal|0
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
name|ngx_imap_proxy_close_session
argument_list|(
name|s
argument_list|)
expr_stmt|;
return|return;
block|}
block|}
block|}
block|}
name|size
operator|=
name|b
operator|->
name|end
operator|-
name|b
operator|->
name|last
expr_stmt|;
if|if
condition|(
name|size
operator|&&
name|src
operator|->
name|read
operator|->
name|ready
condition|)
block|{
name|n
operator|=
name|ngx_recv
argument_list|(
name|src
argument_list|,
name|b
operator|->
name|last
argument_list|,
name|size
argument_list|)
expr_stmt|;
if|if
condition|(
name|n
operator|==
name|NGX_ERROR
operator|||
name|n
operator|==
literal|0
condition|)
block|{
name|ngx_imap_proxy_close_session
argument_list|(
name|s
argument_list|)
expr_stmt|;
return|return;
block|}
if|if
condition|(
name|n
operator|>
literal|0
condition|)
block|{
name|again
operator|=
literal|1
expr_stmt|;
name|do_write
operator|=
literal|1
expr_stmt|;
name|b
operator|->
name|last
operator|+=
name|n
expr_stmt|;
block|}
if|if
condition|(
name|n
operator|==
name|NGX_AGAIN
operator|||
name|n
operator|<
operator|(
name|ssize_t
operator|)
name|size
condition|)
block|{
if|if
condition|(
name|ngx_handle_read_event
argument_list|(
name|src
operator|->
name|read
argument_list|,
literal|0
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
name|ngx_imap_proxy_close_session
argument_list|(
name|s
argument_list|)
expr_stmt|;
return|return;
block|}
block|}
block|}
block|}
do|while
condition|(
name|again
condition|)
do|;
block|}
end_function

begin_function
DECL|function|ngx_imap_proxy_close_session (ngx_imap_session_t * s)
specifier|static
name|void
name|ngx_imap_proxy_close_session
parameter_list|(
name|ngx_imap_session_t
modifier|*
name|s
parameter_list|)
block|{
if|if
condition|(
name|ngx_close_socket
argument_list|(
name|s
operator|->
name|proxy
operator|->
name|upstream
operator|.
name|connection
operator|->
name|fd
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
name|s
operator|->
name|connection
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
name|ngx_imap_close_connection
argument_list|(
name|s
operator|->
name|connection
argument_list|)
expr_stmt|;
block|}
end_function

end_unit

