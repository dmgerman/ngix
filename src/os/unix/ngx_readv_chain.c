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

begin_if
if|#
directive|if
operator|(
name|HAVE_KQUEUE
operator|)
end_if

begin_function
DECL|function|ngx_readv_chain (ngx_connection_t * c,ngx_chain_t * chain)
name|ssize_t
name|ngx_readv_chain
parameter_list|(
name|ngx_connection_t
modifier|*
name|c
parameter_list|,
name|ngx_chain_t
modifier|*
name|chain
parameter_list|)
block|{
name|u_char
modifier|*
name|prev
decl_stmt|;
name|ssize_t
name|n
decl_stmt|,
name|size
decl_stmt|;
name|struct
name|iovec
modifier|*
name|iov
decl_stmt|;
name|ngx_err_t
name|err
decl_stmt|;
name|ngx_array_t
name|io
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
name|ngx_event_flags
operator|&
name|NGX_HAVE_KQUEUE_EVENT
condition|)
block|{
name|ngx_log_debug3
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|c
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"readv: eof:%d, avail:%d, err:%d"
argument_list|,
name|rev
operator|->
name|kq_eof
argument_list|,
name|rev
operator|->
name|available
argument_list|,
name|rev
operator|->
name|kq_errno
argument_list|)
expr_stmt|;
if|if
condition|(
name|rev
operator|->
name|available
operator|==
literal|0
condition|)
block|{
if|if
condition|(
name|rev
operator|->
name|kq_eof
condition|)
block|{
name|rev
operator|->
name|ready
operator|=
literal|0
expr_stmt|;
name|rev
operator|->
name|eof
operator|=
literal|1
expr_stmt|;
name|ngx_log_error
argument_list|(
name|NGX_LOG_INFO
argument_list|,
name|c
operator|->
name|log
argument_list|,
name|rev
operator|->
name|kq_errno
argument_list|,
literal|"kevent() reported about an closed connection"
argument_list|)
expr_stmt|;
if|if
condition|(
name|rev
operator|->
name|kq_errno
condition|)
block|{
name|rev
operator|->
name|error
operator|=
literal|1
expr_stmt|;
name|ngx_set_socket_errno
argument_list|(
name|rev
operator|->
name|kq_errno
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
return|return
literal|0
return|;
block|}
else|else
block|{
return|return
name|NGX_AGAIN
return|;
block|}
block|}
block|}
name|prev
operator|=
name|NULL
expr_stmt|;
name|iov
operator|=
name|NULL
expr_stmt|;
name|size
operator|=
literal|0
expr_stmt|;
name|ngx_init_array
argument_list|(
name|io
argument_list|,
name|c
operator|->
name|pool
argument_list|,
literal|10
argument_list|,
sizeof|sizeof
argument_list|(
expr|struct
name|iovec
argument_list|)
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
comment|/* coalesce the neighbouring hunks */
while|while
condition|(
name|chain
condition|)
block|{
if|if
condition|(
name|prev
operator|==
name|chain
operator|->
name|hunk
operator|->
name|last
condition|)
block|{
name|iov
operator|->
name|iov_len
operator|+=
name|chain
operator|->
name|hunk
operator|->
name|end
operator|-
name|chain
operator|->
name|hunk
operator|->
name|last
expr_stmt|;
block|}
else|else
block|{
name|ngx_test_null
argument_list|(
name|iov
argument_list|,
name|ngx_push_array
argument_list|(
operator|&
name|io
argument_list|)
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
name|iov
operator|->
name|iov_base
operator|=
operator|(
name|void
operator|*
operator|)
name|chain
operator|->
name|hunk
operator|->
name|last
expr_stmt|;
name|iov
operator|->
name|iov_len
operator|=
name|chain
operator|->
name|hunk
operator|->
name|end
operator|-
name|chain
operator|->
name|hunk
operator|->
name|last
expr_stmt|;
block|}
name|size
operator|+=
name|chain
operator|->
name|hunk
operator|->
name|end
operator|-
name|chain
operator|->
name|hunk
operator|->
name|last
expr_stmt|;
name|prev
operator|=
name|chain
operator|->
name|hunk
operator|->
name|end
expr_stmt|;
name|chain
operator|=
name|chain
operator|->
name|next
expr_stmt|;
block|}
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
literal|"readv: %d, last:%d"
argument_list|,
name|io
operator|.
name|nelts
argument_list|,
name|iov
operator|->
name|iov_len
argument_list|)
expr_stmt|;
name|rev
operator|=
name|c
operator|->
name|read
expr_stmt|;
do|do
block|{
name|n
operator|=
name|readv
argument_list|(
name|c
operator|->
name|fd
argument_list|,
operator|(
expr|struct
name|iovec
operator|*
operator|)
name|io
operator|.
name|elts
argument_list|,
name|io
operator|.
name|nelts
argument_list|)
expr_stmt|;
if|if
condition|(
name|n
operator|>=
literal|0
condition|)
block|{
if|if
condition|(
name|ngx_event_flags
operator|&
name|NGX_HAVE_KQUEUE_EVENT
condition|)
block|{
name|rev
operator|->
name|available
operator|-=
name|n
expr_stmt|;
comment|/*                  * rev->available can be negative here because some additional                  * bytes can be received between kevent() and recv()                  */
if|if
condition|(
name|rev
operator|->
name|available
operator|<=
literal|0
condition|)
block|{
if|if
condition|(
operator|!
name|rev
operator|->
name|kq_eof
condition|)
block|{
name|rev
operator|->
name|ready
operator|=
literal|0
expr_stmt|;
block|}
if|if
condition|(
name|rev
operator|->
name|available
operator|<
literal|0
condition|)
block|{
name|rev
operator|->
name|available
operator|=
literal|0
expr_stmt|;
block|}
block|}
return|return
name|n
return|;
block|}
if|if
condition|(
name|n
operator|<
name|size
condition|)
block|{
name|rev
operator|->
name|ready
operator|=
literal|0
expr_stmt|;
block|}
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
block|}
return|return
name|n
return|;
block|}
name|err
operator|=
name|ngx_socket_errno
expr_stmt|;
if|if
condition|(
name|err
operator|==
name|NGX_EAGAIN
operator|||
name|err
operator|==
name|NGX_EINTR
condition|)
block|{
name|ngx_log_debug0
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|c
operator|->
name|log
argument_list|,
name|err
argument_list|,
literal|"readv() not ready"
argument_list|)
expr_stmt|;
name|n
operator|=
name|NGX_AGAIN
expr_stmt|;
block|}
else|else
block|{
name|n
operator|=
name|ngx_connection_error
argument_list|(
name|c
argument_list|,
name|err
argument_list|,
literal|"readv() failed"
argument_list|)
expr_stmt|;
break|break;
block|}
block|}
do|while
condition|(
name|err
operator|==
name|NGX_EINTR
condition|)
do|;
name|rev
operator|->
name|ready
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|n
operator|==
name|NGX_ERROR
condition|)
block|{
name|c
operator|->
name|read
operator|->
name|error
operator|=
literal|1
expr_stmt|;
block|}
return|return
name|n
return|;
block|}
end_function

begin_else
else|#
directive|else
end_else

begin_comment
comment|/* ! NAVE_KQUEUE */
end_comment

begin_function
DECL|function|ngx_readv_chain (ngx_connection_t * c,ngx_chain_t * chain)
name|ssize_t
name|ngx_readv_chain
parameter_list|(
name|ngx_connection_t
modifier|*
name|c
parameter_list|,
name|ngx_chain_t
modifier|*
name|chain
parameter_list|)
block|{
name|u_char
modifier|*
name|prev
decl_stmt|;
name|ssize_t
name|n
decl_stmt|,
name|size
decl_stmt|;
name|struct
name|iovec
modifier|*
name|iov
decl_stmt|;
name|ngx_err_t
name|err
decl_stmt|;
name|ngx_array_t
name|io
decl_stmt|;
name|ngx_event_t
modifier|*
name|rev
decl_stmt|;
name|prev
operator|=
name|NULL
expr_stmt|;
name|iov
operator|=
name|NULL
expr_stmt|;
name|size
operator|=
literal|0
expr_stmt|;
name|ngx_init_array
argument_list|(
name|io
argument_list|,
name|c
operator|->
name|pool
argument_list|,
literal|10
argument_list|,
sizeof|sizeof
argument_list|(
expr|struct
name|iovec
argument_list|)
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
comment|/* coalesce the neighbouring hunks */
while|while
condition|(
name|chain
condition|)
block|{
if|if
condition|(
name|prev
operator|==
name|chain
operator|->
name|hunk
operator|->
name|last
condition|)
block|{
name|iov
operator|->
name|iov_len
operator|+=
name|chain
operator|->
name|hunk
operator|->
name|end
operator|-
name|chain
operator|->
name|hunk
operator|->
name|last
expr_stmt|;
block|}
else|else
block|{
name|ngx_test_null
argument_list|(
name|iov
argument_list|,
name|ngx_push_array
argument_list|(
operator|&
name|io
argument_list|)
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
name|iov
operator|->
name|iov_base
operator|=
name|chain
operator|->
name|hunk
operator|->
name|last
expr_stmt|;
name|iov
operator|->
name|iov_len
operator|=
name|chain
operator|->
name|hunk
operator|->
name|end
operator|-
name|chain
operator|->
name|hunk
operator|->
name|last
expr_stmt|;
block|}
name|size
operator|+=
name|chain
operator|->
name|hunk
operator|->
name|end
operator|-
name|chain
operator|->
name|hunk
operator|->
name|last
expr_stmt|;
name|prev
operator|=
name|chain
operator|->
name|hunk
operator|->
name|end
expr_stmt|;
name|chain
operator|=
name|chain
operator|->
name|next
expr_stmt|;
block|}
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
literal|"readv: %d:%d"
argument_list|,
name|io
operator|.
name|nelts
argument_list|,
name|iov
operator|->
name|iov_len
argument_list|)
expr_stmt|;
name|rev
operator|=
name|c
operator|->
name|read
expr_stmt|;
do|do
block|{
name|n
operator|=
name|readv
argument_list|(
name|c
operator|->
name|fd
argument_list|,
operator|(
expr|struct
name|iovec
operator|*
operator|)
name|io
operator|.
name|elts
argument_list|,
name|io
operator|.
name|nelts
argument_list|)
expr_stmt|;
if|if
condition|(
name|n
operator|>=
literal|0
condition|)
block|{
if|if
condition|(
name|n
operator|<
name|size
condition|)
block|{
name|rev
operator|->
name|ready
operator|=
literal|0
expr_stmt|;
block|}
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
block|}
return|return
name|n
return|;
block|}
name|err
operator|=
name|ngx_socket_errno
expr_stmt|;
if|if
condition|(
name|err
operator|==
name|NGX_EAGAIN
operator|||
name|err
operator|==
name|NGX_EINTR
condition|)
block|{
name|ngx_log_debug0
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|c
operator|->
name|log
argument_list|,
name|err
argument_list|,
literal|"readv() not ready"
argument_list|)
expr_stmt|;
name|n
operator|=
name|NGX_AGAIN
expr_stmt|;
block|}
else|else
block|{
name|n
operator|=
name|ngx_connection_error
argument_list|(
name|c
argument_list|,
name|err
argument_list|,
literal|"readv() failed"
argument_list|)
expr_stmt|;
break|break;
block|}
block|}
do|while
condition|(
name|err
operator|==
name|NGX_EINTR
condition|)
do|;
name|rev
operator|->
name|ready
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|n
operator|==
name|NGX_ERROR
condition|)
block|{
name|c
operator|->
name|read
operator|->
name|error
operator|=
literal|1
expr_stmt|;
block|}
return|return
name|n
return|;
block|}
end_function

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* NAVE_KQUEUE */
end_comment

end_unit

