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
file|<ngx_aio.h>
end_include

begin_if
if|#
directive|if
operator|(
name|NGX_HAVE_KQUEUE
operator|)
end_if

begin_include
include|#
directive|include
file|<ngx_kqueue_module.h>
end_include

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/*  * the ready data requires 3 syscalls:  *     aio_write(), aio_error(), aio_return()  * the non-ready data requires 4 (kqueue) or 5 syscalls:  *     aio_write(), aio_error(), notifiction, aio_error(), aio_return()  *                               timeout, aio_cancel(), aio_error()  */
end_comment

begin_function
DECL|function|ngx_aio_read (ngx_connection_t * c,u_char * buf,size_t size)
name|ssize_t
name|ngx_aio_read
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
decl_stmt|;
name|ngx_event_t
modifier|*
name|rev
decl_stmt|;
name|rev
operator|=
name|c
operator|->
name|read
expr_stmt|;
if|if
condition|(
operator|!
name|rev
operator|->
name|ready
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|c
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"second aio post"
argument_list|)
expr_stmt|;
return|return
name|NGX_AGAIN
return|;
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
literal|"rev->complete: %d"
argument_list|,
name|rev
operator|->
name|complete
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
literal|"aio size: %d"
argument_list|,
name|size
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|rev
operator|->
name|complete
condition|)
block|{
name|ngx_memzero
argument_list|(
operator|&
name|rev
operator|->
name|aiocb
argument_list|,
sizeof|sizeof
argument_list|(
expr|struct
name|aiocb
argument_list|)
argument_list|)
expr_stmt|;
name|rev
operator|->
name|aiocb
operator|.
name|aio_fildes
operator|=
name|c
operator|->
name|fd
expr_stmt|;
name|rev
operator|->
name|aiocb
operator|.
name|aio_buf
operator|=
name|buf
expr_stmt|;
name|rev
operator|->
name|aiocb
operator|.
name|aio_nbytes
operator|=
name|size
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_HAVE_KQUEUE
operator|)
name|rev
operator|->
name|aiocb
operator|.
name|aio_sigevent
operator|.
name|sigev_notify_kqueue
operator|=
name|ngx_kqueue
expr_stmt|;
name|rev
operator|->
name|aiocb
operator|.
name|aio_sigevent
operator|.
name|sigev_notify
operator|=
name|SIGEV_KEVENT
expr_stmt|;
name|rev
operator|->
name|aiocb
operator|.
name|aio_sigevent
operator|.
name|sigev_value
operator|.
name|sigval_ptr
operator|=
name|rev
expr_stmt|;
endif|#
directive|endif
if|if
condition|(
name|aio_read
argument_list|(
operator|&
name|rev
operator|->
name|aiocb
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
name|rev
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"aio_read() failed"
argument_list|)
expr_stmt|;
name|rev
operator|->
name|error
operator|=
literal|1
expr_stmt|;
return|return
name|NGX_ERROR
return|;
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
literal|"aio_read: #%d OK"
argument_list|,
name|c
operator|->
name|fd
argument_list|)
expr_stmt|;
name|rev
operator|->
name|active
operator|=
literal|1
expr_stmt|;
name|rev
operator|->
name|ready
operator|=
literal|0
expr_stmt|;
block|}
name|rev
operator|->
name|complete
operator|=
literal|0
expr_stmt|;
name|n
operator|=
name|aio_error
argument_list|(
operator|&
name|rev
operator|->
name|aiocb
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
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|c
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"aio_error() failed"
argument_list|)
expr_stmt|;
name|rev
operator|->
name|error
operator|=
literal|1
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|n
operator|!=
literal|0
condition|)
block|{
if|if
condition|(
name|n
operator|==
name|NGX_EINPROGRESS
condition|)
block|{
if|if
condition|(
name|rev
operator|->
name|ready
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|c
operator|->
name|log
argument_list|,
name|n
argument_list|,
literal|"aio_read() still in progress"
argument_list|)
expr_stmt|;
name|rev
operator|->
name|ready
operator|=
literal|0
expr_stmt|;
block|}
return|return
name|NGX_AGAIN
return|;
block|}
name|ngx_log_error
argument_list|(
name|NGX_LOG_CRIT
argument_list|,
name|c
operator|->
name|log
argument_list|,
name|n
argument_list|,
literal|"aio_read() failed"
argument_list|)
expr_stmt|;
name|rev
operator|->
name|error
operator|=
literal|1
expr_stmt|;
name|rev
operator|->
name|ready
operator|=
literal|0
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
name|n
operator|=
name|aio_return
argument_list|(
operator|&
name|rev
operator|->
name|aiocb
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
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|c
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"aio_return() failed"
argument_list|)
expr_stmt|;
name|rev
operator|->
name|error
operator|=
literal|1
expr_stmt|;
name|rev
operator|->
name|ready
operator|=
literal|0
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
name|ngx_log_debug2
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|rev
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"aio_read: #%d %d"
argument_list|,
name|c
operator|->
name|fd
argument_list|,
name|n
argument_list|)
expr_stmt|;
if|if
condition|(
name|n
operator|==
literal|0
condition|)
block|{
name|rev
operator|->
name|eof
operator|=
literal|1
expr_stmt|;
name|rev
operator|->
name|ready
operator|=
literal|0
expr_stmt|;
block|}
else|else
block|{
name|rev
operator|->
name|ready
operator|=
literal|1
expr_stmt|;
block|}
name|rev
operator|->
name|active
operator|=
literal|0
expr_stmt|;
return|return
name|n
return|;
block|}
end_function

end_unit

