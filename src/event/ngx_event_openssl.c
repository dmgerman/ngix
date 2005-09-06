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
file|<openssl/engine.h>
end_include

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_ssl_handle_recv
parameter_list|(
name|ngx_connection_t
modifier|*
name|c
parameter_list|,
name|int
name|n
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|void
name|ngx_ssl_write_handler
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
name|ngx_ssl_read_handler
parameter_list|(
name|ngx_event_t
modifier|*
name|rev
parameter_list|)
function_decl|;
end_function_decl

begin_function
name|ngx_int_t
DECL|function|ngx_ssl_init (ngx_log_t * log)
name|ngx_ssl_init
parameter_list|(
name|ngx_log_t
modifier|*
name|log
parameter_list|)
block|{
name|SSL_library_init
argument_list|()
expr_stmt|;
name|SSL_load_error_strings
argument_list|()
expr_stmt|;
name|ENGINE_load_builtin_engines
argument_list|()
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
name|ngx_int_t
DECL|function|ngx_ssl_create_session (ngx_ssl_ctx_t * ssl_ctx,ngx_connection_t * c,ngx_uint_t flags)
name|ngx_ssl_create_session
parameter_list|(
name|ngx_ssl_ctx_t
modifier|*
name|ssl_ctx
parameter_list|,
name|ngx_connection_t
modifier|*
name|c
parameter_list|,
name|ngx_uint_t
name|flags
parameter_list|)
block|{
name|ngx_ssl_t
modifier|*
name|ssl
decl_stmt|;
name|ssl
operator|=
name|ngx_pcalloc
argument_list|(
name|c
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_ssl_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|ssl
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|ssl
operator|->
name|buf
operator|=
name|ngx_create_temp_buf
argument_list|(
name|c
operator|->
name|pool
argument_list|,
name|NGX_SSL_BUFSIZE
argument_list|)
expr_stmt|;
if|if
condition|(
name|ssl
operator|->
name|buf
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|flags
operator|&
name|NGX_SSL_BUFFER
condition|)
block|{
name|ssl
operator|->
name|buffer
operator|=
literal|1
expr_stmt|;
block|}
name|ssl
operator|->
name|ssl
operator|=
name|SSL_new
argument_list|(
name|ssl_ctx
argument_list|)
expr_stmt|;
if|if
condition|(
name|ssl
operator|->
name|ssl
operator|==
name|NULL
condition|)
block|{
name|ngx_ssl_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|c
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"SSL_new() failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|SSL_set_fd
argument_list|(
name|ssl
operator|->
name|ssl
argument_list|,
name|c
operator|->
name|fd
argument_list|)
operator|==
literal|0
condition|)
block|{
name|ngx_ssl_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|c
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"SSL_set_fd() failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
name|SSL_set_accept_state
argument_list|(
name|ssl
operator|->
name|ssl
argument_list|)
expr_stmt|;
name|c
operator|->
name|ssl
operator|=
name|ssl
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
name|ssize_t
DECL|function|ngx_ssl_recv (ngx_connection_t * c,u_char * buf,size_t size)
name|ngx_ssl_recv
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
name|int
name|n
decl_stmt|,
name|bytes
decl_stmt|;
if|if
condition|(
name|c
operator|->
name|ssl
operator|->
name|last
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|bytes
operator|=
literal|0
expr_stmt|;
comment|/*      * SSL_read() may return data in parts, so try to read      * until SSL_read() would return no data      */
for|for
control|(
init|;
condition|;
control|)
block|{
name|n
operator|=
name|SSL_read
argument_list|(
name|c
operator|->
name|ssl
operator|->
name|ssl
argument_list|,
name|buf
argument_list|,
name|size
argument_list|)
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
literal|"SSL_read: %d"
argument_list|,
name|n
argument_list|)
expr_stmt|;
if|if
condition|(
name|n
operator|>
literal|0
condition|)
block|{
name|bytes
operator|+=
name|n
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_DEBUG
operator|)
if|if
condition|(
operator|!
name|c
operator|->
name|ssl
operator|->
name|handshaked
operator|&&
name|SSL_is_init_finished
argument_list|(
name|c
operator|->
name|ssl
operator|->
name|ssl
argument_list|)
condition|)
block|{
name|char
name|buf
index|[
literal|129
index|]
decl_stmt|,
modifier|*
name|s
decl_stmt|,
modifier|*
name|d
decl_stmt|;
name|SSL_CIPHER
modifier|*
name|cipher
decl_stmt|;
name|c
operator|->
name|ssl
operator|->
name|handshaked
operator|=
literal|1
expr_stmt|;
name|cipher
operator|=
name|SSL_get_current_cipher
argument_list|(
name|c
operator|->
name|ssl
operator|->
name|ssl
argument_list|)
expr_stmt|;
if|if
condition|(
name|cipher
condition|)
block|{
name|SSL_CIPHER_description
argument_list|(
name|cipher
argument_list|,
operator|&
name|buf
index|[
literal|1
index|]
argument_list|,
literal|128
argument_list|)
expr_stmt|;
for|for
control|(
name|s
operator|=
operator|&
name|buf
index|[
literal|1
index|]
operator|,
name|d
operator|=
name|buf
init|;
operator|*
name|s
condition|;
name|s
operator|++
control|)
block|{
if|if
condition|(
operator|*
name|s
operator|==
literal|' '
operator|&&
operator|*
name|d
operator|==
literal|' '
condition|)
block|{
continue|continue;
block|}
if|if
condition|(
operator|*
name|s
operator|==
name|LF
operator|||
operator|*
name|s
operator|==
name|CR
condition|)
block|{
continue|continue;
block|}
operator|*
operator|++
name|d
operator|=
operator|*
name|s
expr_stmt|;
block|}
if|if
condition|(
operator|*
name|d
operator|!=
literal|' '
condition|)
block|{
name|d
operator|++
expr_stmt|;
block|}
operator|*
name|d
operator|=
literal|'\0'
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
literal|"SSL cipher: \"%s\""
argument_list|,
operator|&
name|buf
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|ngx_log_debug0
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|c
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"SSL no shared ciphers"
argument_list|)
expr_stmt|;
block|}
block|}
endif|#
directive|endif
block|}
name|c
operator|->
name|ssl
operator|->
name|last
operator|=
name|ngx_ssl_handle_recv
argument_list|(
name|c
argument_list|,
name|n
argument_list|)
expr_stmt|;
if|if
condition|(
name|c
operator|->
name|ssl
operator|->
name|last
operator|!=
name|NGX_OK
condition|)
block|{
if|if
condition|(
name|bytes
condition|)
block|{
return|return
name|bytes
return|;
block|}
else|else
block|{
return|return
name|c
operator|->
name|ssl
operator|->
name|last
return|;
block|}
block|}
name|size
operator|-=
name|n
expr_stmt|;
if|if
condition|(
name|size
operator|==
literal|0
condition|)
block|{
return|return
name|bytes
return|;
block|}
name|buf
operator|+=
name|n
expr_stmt|;
block|}
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_ssl_handle_recv (ngx_connection_t * c,int n)
name|ngx_ssl_handle_recv
parameter_list|(
name|ngx_connection_t
modifier|*
name|c
parameter_list|,
name|int
name|n
parameter_list|)
block|{
name|int
name|sslerr
decl_stmt|;
name|ngx_err_t
name|err
decl_stmt|;
name|char
modifier|*
name|handshake
decl_stmt|;
if|if
condition|(
name|n
operator|>
literal|0
condition|)
block|{
if|if
condition|(
name|c
operator|->
name|ssl
operator|->
name|saved_write_handler
condition|)
block|{
name|c
operator|->
name|write
operator|->
name|handler
operator|=
name|c
operator|->
name|ssl
operator|->
name|saved_write_handler
expr_stmt|;
name|c
operator|->
name|ssl
operator|->
name|saved_write_handler
operator|=
name|NULL
expr_stmt|;
name|c
operator|->
name|write
operator|->
name|ready
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|ngx_handle_write_event
argument_list|(
name|c
operator|->
name|write
argument_list|,
literal|0
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
name|ngx_mutex_lock
argument_list|(
name|ngx_posted_events_mutex
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|ngx_post_event
argument_list|(
name|c
operator|->
name|write
argument_list|)
expr_stmt|;
name|ngx_mutex_unlock
argument_list|(
name|ngx_posted_events_mutex
argument_list|)
expr_stmt|;
block|}
return|return
name|NGX_OK
return|;
block|}
if|if
condition|(
operator|!
name|SSL_is_init_finished
argument_list|(
name|c
operator|->
name|ssl
operator|->
name|ssl
argument_list|)
condition|)
block|{
name|handshake
operator|=
literal|" in SSL handshake"
expr_stmt|;
block|}
else|else
block|{
name|handshake
operator|=
literal|""
expr_stmt|;
block|}
name|sslerr
operator|=
name|SSL_get_error
argument_list|(
name|c
operator|->
name|ssl
operator|->
name|ssl
argument_list|,
name|n
argument_list|)
expr_stmt|;
name|err
operator|=
operator|(
name|sslerr
operator|==
name|SSL_ERROR_SYSCALL
operator|)
condition|?
name|ngx_errno
else|:
literal|0
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
literal|"SSL_get_error: %d"
argument_list|,
name|sslerr
argument_list|)
expr_stmt|;
if|if
condition|(
name|sslerr
operator|==
name|SSL_ERROR_WANT_READ
condition|)
block|{
name|c
operator|->
name|read
operator|->
name|ready
operator|=
literal|0
expr_stmt|;
return|return
name|NGX_AGAIN
return|;
block|}
if|if
condition|(
name|sslerr
operator|==
name|SSL_ERROR_WANT_WRITE
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
name|err
argument_list|,
literal|"client does SSL %shandshake"
argument_list|,
name|SSL_is_init_finished
argument_list|(
name|c
operator|->
name|ssl
operator|->
name|ssl
argument_list|)
condition|?
literal|"re"
else|:
literal|""
argument_list|)
expr_stmt|;
name|c
operator|->
name|write
operator|->
name|ready
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|ngx_handle_write_event
argument_list|(
name|c
operator|->
name|write
argument_list|,
literal|0
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
comment|/*          * we do not set the timer because there is already the read event timer          */
if|if
condition|(
name|c
operator|->
name|ssl
operator|->
name|saved_write_handler
operator|==
name|NULL
condition|)
block|{
name|c
operator|->
name|ssl
operator|->
name|saved_write_handler
operator|=
name|c
operator|->
name|write
operator|->
name|handler
expr_stmt|;
name|c
operator|->
name|write
operator|->
name|handler
operator|=
name|ngx_ssl_write_handler
expr_stmt|;
block|}
return|return
name|NGX_AGAIN
return|;
block|}
name|c
operator|->
name|ssl
operator|->
name|no_rcv_shut
operator|=
literal|1
expr_stmt|;
name|c
operator|->
name|ssl
operator|->
name|no_send_shut
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|sslerr
operator|==
name|SSL_ERROR_ZERO_RETURN
operator|||
name|ERR_peek_error
argument_list|()
operator|==
literal|0
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
name|err
argument_list|,
literal|"client closed connection%s"
argument_list|,
name|handshake
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
name|ngx_ssl_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|c
operator|->
name|log
argument_list|,
name|err
argument_list|,
literal|"SSL_read() failed%s"
argument_list|,
name|handshake
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
end_function

begin_function
specifier|static
name|void
DECL|function|ngx_ssl_write_handler (ngx_event_t * wev)
name|ngx_ssl_write_handler
parameter_list|(
name|ngx_event_t
modifier|*
name|wev
parameter_list|)
block|{
name|ngx_connection_t
modifier|*
name|c
decl_stmt|;
name|c
operator|=
name|wev
operator|->
name|data
expr_stmt|;
name|c
operator|->
name|read
operator|->
name|handler
argument_list|(
name|c
operator|->
name|read
argument_list|)
expr_stmt|;
block|}
end_function

begin_comment
comment|/*  * OpenSSL has no SSL_writev() so we copy several bufs into our 16K buffer  * before the SSL_write() call to decrease a SSL overhead.  *  * Besides for protocols such as HTTP it is possible to always buffer  * the output to decrease a SSL overhead some more.  */
end_comment

begin_function
name|ngx_chain_t
modifier|*
DECL|function|ngx_ssl_send_chain (ngx_connection_t * c,ngx_chain_t * in,off_t limit)
name|ngx_ssl_send_chain
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
block|{
name|int
name|n
decl_stmt|;
name|ngx_uint_t
name|flush
decl_stmt|;
name|ssize_t
name|send
decl_stmt|,
name|size
decl_stmt|;
name|ngx_buf_t
modifier|*
name|buf
decl_stmt|;
name|buf
operator|=
name|c
operator|->
name|ssl
operator|->
name|buf
expr_stmt|;
if|if
condition|(
name|in
operator|&&
name|in
operator|->
name|next
operator|==
name|NULL
operator|&&
operator|!
name|c
operator|->
name|buffered
operator|&&
operator|!
name|c
operator|->
name|ssl
operator|->
name|buffer
condition|)
block|{
comment|/*          * we avoid a buffer copy if the incoming buf is a single,          * our buffer is empty, and we do not need to buffer the output          */
name|n
operator|=
name|ngx_ssl_write
argument_list|(
name|c
argument_list|,
name|in
operator|->
name|buf
operator|->
name|pos
argument_list|,
name|in
operator|->
name|buf
operator|->
name|last
operator|-
name|in
operator|->
name|buf
operator|->
name|pos
argument_list|)
expr_stmt|;
if|if
condition|(
name|n
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|NGX_CHAIN_ERROR
return|;
block|}
if|if
condition|(
name|n
operator|==
name|NGX_AGAIN
condition|)
block|{
name|c
operator|->
name|buffered
operator|=
literal|1
expr_stmt|;
return|return
name|in
return|;
block|}
name|in
operator|->
name|buf
operator|->
name|pos
operator|+=
name|n
expr_stmt|;
return|return
name|in
return|;
block|}
comment|/* the maximum limit size is the maximum uint32_t value - the page size */
if|if
condition|(
name|limit
operator|==
literal|0
operator|||
name|limit
operator|>
name|NGX_MAX_UINT32_VALUE
operator|-
name|ngx_pagesize
condition|)
block|{
name|limit
operator|=
name|NGX_MAX_UINT32_VALUE
operator|-
name|ngx_pagesize
expr_stmt|;
block|}
name|send
operator|=
literal|0
expr_stmt|;
name|flush
operator|=
operator|(
name|in
operator|==
name|NULL
operator|)
condition|?
literal|1
else|:
literal|0
expr_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
while|while
condition|(
name|in
operator|&&
name|buf
operator|->
name|last
operator|<
name|buf
operator|->
name|end
condition|)
block|{
if|if
condition|(
name|in
operator|->
name|buf
operator|->
name|last_buf
condition|)
block|{
name|flush
operator|=
literal|1
expr_stmt|;
block|}
if|if
condition|(
name|ngx_buf_special
argument_list|(
name|in
operator|->
name|buf
argument_list|)
condition|)
block|{
name|in
operator|=
name|in
operator|->
name|next
expr_stmt|;
continue|continue;
block|}
name|size
operator|=
name|in
operator|->
name|buf
operator|->
name|last
operator|-
name|in
operator|->
name|buf
operator|->
name|pos
expr_stmt|;
if|if
condition|(
name|size
operator|>
name|buf
operator|->
name|end
operator|-
name|buf
operator|->
name|last
condition|)
block|{
name|size
operator|=
name|buf
operator|->
name|end
operator|-
name|buf
operator|->
name|last
expr_stmt|;
block|}
comment|/*              * TODO: the taking in->buf->flush into account can be              *       implemented using the limit on the higher level              */
if|if
condition|(
name|send
operator|+
name|size
operator|>
name|limit
condition|)
block|{
name|size
operator|=
name|limit
operator|-
name|send
expr_stmt|;
name|flush
operator|=
literal|1
expr_stmt|;
block|}
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
literal|"SSL buf copy: %d"
argument_list|,
name|size
argument_list|)
expr_stmt|;
name|ngx_memcpy
argument_list|(
name|buf
operator|->
name|last
argument_list|,
name|in
operator|->
name|buf
operator|->
name|pos
argument_list|,
name|size
argument_list|)
expr_stmt|;
name|buf
operator|->
name|last
operator|+=
name|size
expr_stmt|;
name|in
operator|->
name|buf
operator|->
name|pos
operator|+=
name|size
expr_stmt|;
if|if
condition|(
name|in
operator|->
name|buf
operator|->
name|pos
operator|==
name|in
operator|->
name|buf
operator|->
name|last
condition|)
block|{
name|in
operator|=
name|in
operator|->
name|next
expr_stmt|;
block|}
block|}
name|size
operator|=
name|buf
operator|->
name|last
operator|-
name|buf
operator|->
name|pos
expr_stmt|;
if|if
condition|(
operator|!
name|flush
operator|&&
name|buf
operator|->
name|last
operator|<
name|buf
operator|->
name|end
operator|&&
name|c
operator|->
name|ssl
operator|->
name|buffer
condition|)
block|{
break|break;
block|}
name|n
operator|=
name|ngx_ssl_write
argument_list|(
name|c
argument_list|,
name|buf
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
return|return
name|NGX_CHAIN_ERROR
return|;
block|}
if|if
condition|(
name|n
operator|==
name|NGX_AGAIN
condition|)
block|{
name|c
operator|->
name|buffered
operator|=
literal|1
expr_stmt|;
return|return
name|in
return|;
block|}
name|buf
operator|->
name|pos
operator|+=
name|n
expr_stmt|;
name|send
operator|+=
name|n
expr_stmt|;
name|c
operator|->
name|sent
operator|+=
name|n
expr_stmt|;
if|if
condition|(
name|n
operator|<
name|size
condition|)
block|{
break|break;
block|}
if|if
condition|(
name|buf
operator|->
name|pos
operator|==
name|buf
operator|->
name|last
condition|)
block|{
name|buf
operator|->
name|pos
operator|=
name|buf
operator|->
name|start
expr_stmt|;
name|buf
operator|->
name|last
operator|=
name|buf
operator|->
name|start
expr_stmt|;
block|}
if|if
condition|(
name|in
operator|==
name|NULL
operator|||
name|send
operator|==
name|limit
condition|)
block|{
break|break;
block|}
block|}
name|c
operator|->
name|buffered
operator|=
operator|(
name|buf
operator|->
name|pos
operator|<
name|buf
operator|->
name|last
operator|)
condition|?
literal|1
else|:
literal|0
expr_stmt|;
return|return
name|in
return|;
block|}
end_function

begin_function
name|ssize_t
DECL|function|ngx_ssl_write (ngx_connection_t * c,u_char * data,size_t size)
name|ngx_ssl_write
parameter_list|(
name|ngx_connection_t
modifier|*
name|c
parameter_list|,
name|u_char
modifier|*
name|data
parameter_list|,
name|size_t
name|size
parameter_list|)
block|{
name|int
name|n
decl_stmt|,
name|sslerr
decl_stmt|;
name|ngx_err_t
name|err
decl_stmt|;
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
literal|"SSL to write: %d"
argument_list|,
name|size
argument_list|)
expr_stmt|;
name|n
operator|=
name|SSL_write
argument_list|(
name|c
operator|->
name|ssl
operator|->
name|ssl
argument_list|,
name|data
argument_list|,
name|size
argument_list|)
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
literal|"SSL_write: %d"
argument_list|,
name|n
argument_list|)
expr_stmt|;
if|if
condition|(
name|n
operator|>
literal|0
condition|)
block|{
if|#
directive|if
operator|(
name|NGX_DEBUG
operator|)
if|if
condition|(
operator|!
name|c
operator|->
name|ssl
operator|->
name|handshaked
operator|&&
name|SSL_is_init_finished
argument_list|(
name|c
operator|->
name|ssl
operator|->
name|ssl
argument_list|)
condition|)
block|{
name|char
name|buf
index|[
literal|129
index|]
decl_stmt|,
modifier|*
name|s
decl_stmt|,
modifier|*
name|d
decl_stmt|;
name|SSL_CIPHER
modifier|*
name|cipher
decl_stmt|;
name|c
operator|->
name|ssl
operator|->
name|handshaked
operator|=
literal|1
expr_stmt|;
name|cipher
operator|=
name|SSL_get_current_cipher
argument_list|(
name|c
operator|->
name|ssl
operator|->
name|ssl
argument_list|)
expr_stmt|;
if|if
condition|(
name|cipher
condition|)
block|{
name|SSL_CIPHER_description
argument_list|(
name|cipher
argument_list|,
operator|&
name|buf
index|[
literal|1
index|]
argument_list|,
literal|128
argument_list|)
expr_stmt|;
for|for
control|(
name|s
operator|=
operator|&
name|buf
index|[
literal|1
index|]
operator|,
name|d
operator|=
name|buf
init|;
operator|*
name|s
condition|;
name|s
operator|++
control|)
block|{
if|if
condition|(
operator|*
name|s
operator|==
literal|' '
operator|&&
operator|*
name|d
operator|==
literal|' '
condition|)
block|{
continue|continue;
block|}
if|if
condition|(
operator|*
name|s
operator|==
name|LF
operator|||
operator|*
name|s
operator|==
name|CR
condition|)
block|{
continue|continue;
block|}
operator|*
operator|++
name|d
operator|=
operator|*
name|s
expr_stmt|;
block|}
if|if
condition|(
operator|*
name|d
operator|!=
literal|' '
condition|)
block|{
name|d
operator|++
expr_stmt|;
block|}
operator|*
name|d
operator|=
literal|'\0'
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
literal|"SSL cipher: \"%s\""
argument_list|,
operator|&
name|buf
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|ngx_log_debug0
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|c
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"SSL no shared ciphers"
argument_list|)
expr_stmt|;
block|}
block|}
endif|#
directive|endif
if|if
condition|(
name|c
operator|->
name|ssl
operator|->
name|saved_read_handler
condition|)
block|{
name|c
operator|->
name|read
operator|->
name|handler
operator|=
name|c
operator|->
name|ssl
operator|->
name|saved_read_handler
expr_stmt|;
name|c
operator|->
name|ssl
operator|->
name|saved_read_handler
operator|=
name|NULL
expr_stmt|;
name|c
operator|->
name|read
operator|->
name|ready
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|ngx_handle_read_event
argument_list|(
name|c
operator|->
name|read
argument_list|,
literal|0
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
name|ngx_mutex_lock
argument_list|(
name|ngx_posted_events_mutex
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|ngx_post_event
argument_list|(
name|c
operator|->
name|read
argument_list|)
expr_stmt|;
name|ngx_mutex_unlock
argument_list|(
name|ngx_posted_events_mutex
argument_list|)
expr_stmt|;
block|}
return|return
name|n
return|;
block|}
name|sslerr
operator|=
name|SSL_get_error
argument_list|(
name|c
operator|->
name|ssl
operator|->
name|ssl
argument_list|,
name|n
argument_list|)
expr_stmt|;
name|err
operator|=
operator|(
name|sslerr
operator|==
name|SSL_ERROR_SYSCALL
operator|)
condition|?
name|ngx_errno
else|:
literal|0
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
literal|"SSL_get_error: %d"
argument_list|,
name|sslerr
argument_list|)
expr_stmt|;
if|if
condition|(
name|sslerr
operator|==
name|SSL_ERROR_WANT_WRITE
condition|)
block|{
name|c
operator|->
name|write
operator|->
name|ready
operator|=
literal|0
expr_stmt|;
return|return
name|NGX_AGAIN
return|;
block|}
if|if
condition|(
name|sslerr
operator|==
name|SSL_ERROR_WANT_READ
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
name|err
argument_list|,
literal|"client does SSL %shandshake"
argument_list|,
name|SSL_is_init_finished
argument_list|(
name|c
operator|->
name|ssl
operator|->
name|ssl
argument_list|)
condition|?
literal|"re"
else|:
literal|""
argument_list|)
expr_stmt|;
name|c
operator|->
name|read
operator|->
name|ready
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|ngx_handle_read_event
argument_list|(
name|c
operator|->
name|read
argument_list|,
literal|0
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
comment|/*          * we do not set the timer because there is already          * the write event timer          */
if|if
condition|(
name|c
operator|->
name|ssl
operator|->
name|saved_read_handler
operator|==
name|NULL
condition|)
block|{
name|c
operator|->
name|ssl
operator|->
name|saved_read_handler
operator|=
name|c
operator|->
name|read
operator|->
name|handler
expr_stmt|;
name|c
operator|->
name|read
operator|->
name|handler
operator|=
name|ngx_ssl_read_handler
expr_stmt|;
block|}
return|return
name|NGX_AGAIN
return|;
block|}
name|c
operator|->
name|ssl
operator|->
name|no_rcv_shut
operator|=
literal|1
expr_stmt|;
name|c
operator|->
name|ssl
operator|->
name|no_send_shut
operator|=
literal|1
expr_stmt|;
name|ngx_ssl_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|c
operator|->
name|log
argument_list|,
name|err
argument_list|,
literal|"SSL_write() failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
end_function

begin_function
specifier|static
name|void
DECL|function|ngx_ssl_read_handler (ngx_event_t * rev)
name|ngx_ssl_read_handler
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
name|c
operator|->
name|write
operator|->
name|handler
argument_list|(
name|c
operator|->
name|write
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
name|ngx_int_t
DECL|function|ngx_ssl_shutdown (ngx_connection_t * c)
name|ngx_ssl_shutdown
parameter_list|(
name|ngx_connection_t
modifier|*
name|c
parameter_list|)
block|{
name|int
name|n
decl_stmt|,
name|sslerr
decl_stmt|,
name|mode
decl_stmt|;
name|ngx_uint_t
name|again
decl_stmt|;
if|if
condition|(
operator|!
name|c
operator|->
name|ssl
operator|->
name|shutdown_set
condition|)
block|{
comment|/* it seems that SSL_set_shutdown() could be called once only */
if|if
condition|(
name|c
operator|->
name|read
operator|->
name|timedout
condition|)
block|{
name|mode
operator|=
name|SSL_RECEIVED_SHUTDOWN
operator||
name|SSL_SENT_SHUTDOWN
expr_stmt|;
block|}
else|else
block|{
name|mode
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|c
operator|->
name|ssl
operator|->
name|no_rcv_shut
condition|)
block|{
name|mode
operator|=
name|SSL_RECEIVED_SHUTDOWN
expr_stmt|;
block|}
if|if
condition|(
name|c
operator|->
name|ssl
operator|->
name|no_send_shut
condition|)
block|{
name|mode
operator||=
name|SSL_SENT_SHUTDOWN
expr_stmt|;
block|}
block|}
if|if
condition|(
name|mode
condition|)
block|{
name|SSL_set_shutdown
argument_list|(
name|c
operator|->
name|ssl
operator|->
name|ssl
argument_list|,
name|mode
argument_list|)
expr_stmt|;
name|c
operator|->
name|ssl
operator|->
name|shutdown_set
operator|=
literal|1
expr_stmt|;
block|}
block|}
name|again
operator|=
literal|0
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_SUPPRESS_WARN
operator|)
name|sslerr
operator|=
literal|0
expr_stmt|;
endif|#
directive|endif
for|for
control|(
init|;
condition|;
control|)
block|{
name|n
operator|=
name|SSL_shutdown
argument_list|(
name|c
operator|->
name|ssl
operator|->
name|ssl
argument_list|)
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
literal|"SSL_shutdown: %d"
argument_list|,
name|n
argument_list|)
expr_stmt|;
if|if
condition|(
name|n
operator|==
literal|1
operator|||
operator|(
name|n
operator|==
literal|0
operator|&&
name|c
operator|->
name|read
operator|->
name|timedout
operator|)
condition|)
block|{
name|SSL_free
argument_list|(
name|c
operator|->
name|ssl
operator|->
name|ssl
argument_list|)
expr_stmt|;
name|c
operator|->
name|ssl
operator|=
name|NULL
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
if|if
condition|(
name|n
operator|==
literal|0
condition|)
block|{
name|again
operator|=
literal|1
expr_stmt|;
break|break;
block|}
break|break;
block|}
if|if
condition|(
operator|!
name|again
condition|)
block|{
name|sslerr
operator|=
name|SSL_get_error
argument_list|(
name|c
operator|->
name|ssl
operator|->
name|ssl
argument_list|,
name|n
argument_list|)
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
literal|"SSL_get_error: %d"
argument_list|,
name|sslerr
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|again
operator|||
name|sslerr
operator|==
name|SSL_ERROR_WANT_READ
condition|)
block|{
name|ngx_add_timer
argument_list|(
name|c
operator|->
name|read
argument_list|,
literal|30000
argument_list|)
expr_stmt|;
if|if
condition|(
name|ngx_handle_read_event
argument_list|(
name|c
operator|->
name|read
argument_list|,
literal|0
argument_list|)
operator|==
name|NGX_ERROR
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
if|if
condition|(
name|sslerr
operator|==
name|SSL_ERROR_WANT_WRITE
condition|)
block|{
if|if
condition|(
name|ngx_handle_write_event
argument_list|(
name|c
operator|->
name|write
argument_list|,
literal|0
argument_list|)
operator|==
name|NGX_ERROR
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
name|ngx_ssl_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|c
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"SSL_shutdown() failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
end_function

begin_function
name|void
DECL|function|ngx_ssl_error (ngx_uint_t level,ngx_log_t * log,ngx_err_t err,char * fmt,...)
name|ngx_ssl_error
parameter_list|(
name|ngx_uint_t
name|level
parameter_list|,
name|ngx_log_t
modifier|*
name|log
parameter_list|,
name|ngx_err_t
name|err
parameter_list|,
name|char
modifier|*
name|fmt
parameter_list|,
modifier|...
parameter_list|)
block|{
name|u_char
name|errstr
index|[
name|NGX_MAX_CONF_ERRSTR
index|]
decl_stmt|,
modifier|*
name|p
decl_stmt|,
modifier|*
name|last
decl_stmt|;
name|va_list
name|args
decl_stmt|;
name|last
operator|=
name|errstr
operator|+
name|NGX_MAX_CONF_ERRSTR
expr_stmt|;
name|va_start
argument_list|(
name|args
argument_list|,
name|fmt
argument_list|)
expr_stmt|;
name|p
operator|=
name|ngx_vsnprintf
argument_list|(
name|errstr
argument_list|,
sizeof|sizeof
argument_list|(
name|errstr
argument_list|)
operator|-
literal|1
argument_list|,
name|fmt
argument_list|,
name|args
argument_list|)
expr_stmt|;
name|va_end
argument_list|(
name|args
argument_list|)
expr_stmt|;
name|p
operator|=
name|ngx_cpystrn
argument_list|(
name|p
argument_list|,
operator|(
name|u_char
operator|*
operator|)
literal|" (SSL: "
argument_list|,
name|last
operator|-
name|p
argument_list|)
expr_stmt|;
name|ERR_error_string_n
argument_list|(
name|ERR_get_error
argument_list|()
argument_list|,
operator|(
name|char
operator|*
operator|)
name|p
argument_list|,
name|last
operator|-
name|p
argument_list|)
expr_stmt|;
name|ngx_log_error
argument_list|(
name|level
argument_list|,
name|log
argument_list|,
name|err
argument_list|,
literal|"%s)"
argument_list|,
name|errstr
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
name|void
DECL|function|ngx_ssl_cleanup_ctx (void * data)
name|ngx_ssl_cleanup_ctx
parameter_list|(
name|void
modifier|*
name|data
parameter_list|)
block|{
name|SSL_CTX
modifier|*
name|ctx
init|=
name|data
decl_stmt|;
name|SSL_CTX_free
argument_list|(
name|ctx
argument_list|)
expr_stmt|;
block|}
end_function

end_unit

