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

begin_define
DECL|macro|NGX_SENDFILEVECS
define|#
directive|define
name|NGX_SENDFILEVECS
value|16
end_define

begin_function
DECL|function|ngx_solaris_sendfilev_chain (ngx_connection_t * c,ngx_chain_t * in,off_t limit)
name|ngx_chain_t
modifier|*
name|ngx_solaris_sendfilev_chain
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
name|fd
decl_stmt|;
name|u_char
modifier|*
name|prev
decl_stmt|;
name|off_t
name|fprev
decl_stmt|,
name|sprev
decl_stmt|,
name|send
decl_stmt|,
name|aligned
decl_stmt|;
name|ssize_t
name|size
decl_stmt|,
name|sent
decl_stmt|,
name|n
decl_stmt|;
name|ngx_int_t
name|eintr
decl_stmt|,
name|complete
decl_stmt|;
name|ngx_err_t
name|err
decl_stmt|;
name|sendfilevec_t
modifier|*
name|sfv
decl_stmt|,
name|sfvs
index|[
name|NGX_SENDFILEVECS
index|]
decl_stmt|;
name|ngx_array_t
name|vec
decl_stmt|;
name|ngx_event_t
modifier|*
name|wev
decl_stmt|;
name|ngx_chain_t
modifier|*
name|cl
decl_stmt|,
modifier|*
name|tail
decl_stmt|;
name|wev
operator|=
name|c
operator|->
name|write
expr_stmt|;
if|if
condition|(
operator|!
name|wev
operator|->
name|ready
condition|)
block|{
return|return
name|in
return|;
block|}
name|send
operator|=
literal|0
expr_stmt|;
name|complete
operator|=
literal|0
expr_stmt|;
name|vec
operator|.
name|elts
operator|=
name|sfvs
expr_stmt|;
name|vec
operator|.
name|size
operator|=
sizeof|sizeof
argument_list|(
name|sendfilevec_t
argument_list|)
expr_stmt|;
name|vec
operator|.
name|nalloc
operator|=
name|NGX_SENDFILEVECS
expr_stmt|;
name|vec
operator|.
name|pool
operator|=
name|c
operator|->
name|pool
expr_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
name|fd
operator|=
name|SFV_FD_SELF
expr_stmt|;
name|prev
operator|=
name|NULL
expr_stmt|;
name|fprev
operator|=
literal|0
expr_stmt|;
name|sfv
operator|=
name|NULL
expr_stmt|;
name|eintr
operator|=
literal|0
expr_stmt|;
name|sent
operator|=
literal|0
expr_stmt|;
name|sprev
operator|=
name|send
expr_stmt|;
name|vec
operator|.
name|nelts
operator|=
literal|0
expr_stmt|;
comment|/* create the sendfilevec and coalesce the neighbouring bufs */
for|for
control|(
name|cl
operator|=
name|in
init|;
name|cl
operator|&&
name|vec
operator|.
name|nelts
operator|<
name|IOV_MAX
operator|&&
name|send
operator|<
name|limit
condition|;
name|cl
operator|=
name|cl
operator|->
name|next
control|)
block|{
if|if
condition|(
name|ngx_buf_special
argument_list|(
name|cl
operator|->
name|buf
argument_list|)
condition|)
block|{
continue|continue;
block|}
if|if
condition|(
name|ngx_buf_in_memory_only
argument_list|(
name|cl
operator|->
name|buf
argument_list|)
condition|)
block|{
name|fd
operator|=
name|SFV_FD_SELF
expr_stmt|;
name|size
operator|=
name|cl
operator|->
name|buf
operator|->
name|last
operator|-
name|cl
operator|->
name|buf
operator|->
name|pos
expr_stmt|;
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
block|}
if|if
condition|(
name|prev
operator|==
name|cl
operator|->
name|buf
operator|->
name|pos
condition|)
block|{
name|sfv
operator|->
name|sfv_len
operator|+=
name|size
expr_stmt|;
block|}
else|else
block|{
if|if
condition|(
operator|!
operator|(
name|sfv
operator|=
name|ngx_array_push
argument_list|(
operator|&
name|vec
argument_list|)
operator|)
condition|)
block|{
return|return
name|NGX_CHAIN_ERROR
return|;
block|}
name|sfv
operator|->
name|sfv_fd
operator|=
name|SFV_FD_SELF
expr_stmt|;
name|sfv
operator|->
name|sfv_flag
operator|=
literal|0
expr_stmt|;
name|sfv
operator|->
name|sfv_off
operator|=
operator|(
name|off_t
operator|)
operator|(
name|uintptr_t
operator|)
name|cl
operator|->
name|buf
operator|->
name|pos
expr_stmt|;
name|sfv
operator|->
name|sfv_len
operator|=
name|size
expr_stmt|;
block|}
name|prev
operator|=
name|cl
operator|->
name|buf
operator|->
name|pos
operator|+
name|size
expr_stmt|;
name|send
operator|+=
name|size
expr_stmt|;
block|}
else|else
block|{
name|prev
operator|=
name|NULL
expr_stmt|;
name|size
operator|=
operator|(
name|size_t
operator|)
operator|(
name|cl
operator|->
name|buf
operator|->
name|file_last
operator|-
name|cl
operator|->
name|buf
operator|->
name|file_pos
operator|)
expr_stmt|;
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
name|aligned
operator|=
operator|(
name|cl
operator|->
name|buf
operator|->
name|file_pos
operator|+
name|size
operator|+
name|ngx_pagesize
operator|-
literal|1
operator|)
operator|&
operator|~
operator|(
name|ngx_pagesize
operator|-
literal|1
operator|)
expr_stmt|;
if|if
condition|(
name|aligned
operator|<=
name|cl
operator|->
name|buf
operator|->
name|file_last
condition|)
block|{
name|size
operator|=
name|aligned
operator|-
name|cl
operator|->
name|buf
operator|->
name|file_pos
expr_stmt|;
block|}
block|}
if|if
condition|(
name|fd
operator|==
name|cl
operator|->
name|buf
operator|->
name|file
operator|->
name|fd
operator|&&
name|fprev
operator|==
name|cl
operator|->
name|buf
operator|->
name|file_pos
condition|)
block|{
name|sfv
operator|->
name|sfv_len
operator|+=
name|size
expr_stmt|;
block|}
else|else
block|{
if|if
condition|(
operator|!
operator|(
name|sfv
operator|=
name|ngx_array_push
argument_list|(
operator|&
name|vec
argument_list|)
operator|)
condition|)
block|{
return|return
name|NGX_CHAIN_ERROR
return|;
block|}
name|fd
operator|=
name|cl
operator|->
name|buf
operator|->
name|file
operator|->
name|fd
expr_stmt|;
name|sfv
operator|->
name|sfv_fd
operator|=
name|fd
expr_stmt|;
name|sfv
operator|->
name|sfv_flag
operator|=
literal|0
expr_stmt|;
name|sfv
operator|->
name|sfv_off
operator|=
name|cl
operator|->
name|buf
operator|->
name|file_pos
expr_stmt|;
name|sfv
operator|->
name|sfv_len
operator|=
name|size
expr_stmt|;
block|}
name|fprev
operator|=
name|cl
operator|->
name|buf
operator|->
name|file_pos
operator|+
name|size
expr_stmt|;
name|send
operator|+=
name|size
expr_stmt|;
block|}
block|}
name|n
operator|=
name|sendfilev
argument_list|(
name|c
operator|->
name|fd
argument_list|,
name|vec
operator|.
name|elts
argument_list|,
name|vec
operator|.
name|nelts
argument_list|,
operator|&
name|sent
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
name|ngx_errno
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
if|if
condition|(
name|err
operator|==
name|NGX_EINTR
condition|)
block|{
name|eintr
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
name|err
argument_list|,
literal|"sendfilev() sent only "
name|SIZE_T_FMT
literal|" bytes"
argument_list|,
name|sent
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|wev
operator|->
name|error
operator|=
literal|1
expr_stmt|;
name|ngx_connection_error
argument_list|(
name|c
argument_list|,
name|err
argument_list|,
literal|"sendfilev() failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_CHAIN_ERROR
return|;
block|}
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
literal|"sendfilev: %d "
name|SIZE_T_FMT
argument_list|,
name|n
argument_list|,
name|sent
argument_list|)
expr_stmt|;
if|if
condition|(
name|send
operator|-
name|sprev
operator|==
name|sent
condition|)
block|{
name|complete
operator|=
literal|1
expr_stmt|;
block|}
name|c
operator|->
name|sent
operator|+=
name|sent
expr_stmt|;
for|for
control|(
name|cl
operator|=
name|in
init|;
name|cl
condition|;
name|cl
operator|=
name|cl
operator|->
name|next
control|)
block|{
if|if
condition|(
name|ngx_buf_special
argument_list|(
name|cl
operator|->
name|buf
argument_list|)
condition|)
block|{
continue|continue;
block|}
if|if
condition|(
name|sent
operator|==
literal|0
condition|)
block|{
break|break;
block|}
name|size
operator|=
name|ngx_buf_size
argument_list|(
name|cl
operator|->
name|buf
argument_list|)
expr_stmt|;
if|if
condition|(
name|sent
operator|>=
name|size
condition|)
block|{
name|sent
operator|-=
name|size
expr_stmt|;
if|if
condition|(
name|ngx_buf_in_memory
argument_list|(
name|cl
operator|->
name|buf
argument_list|)
condition|)
block|{
name|cl
operator|->
name|buf
operator|->
name|pos
operator|=
name|cl
operator|->
name|buf
operator|->
name|last
expr_stmt|;
block|}
if|if
condition|(
name|cl
operator|->
name|buf
operator|->
name|in_file
condition|)
block|{
name|cl
operator|->
name|buf
operator|->
name|file_pos
operator|=
name|cl
operator|->
name|buf
operator|->
name|file_last
expr_stmt|;
block|}
continue|continue;
block|}
if|if
condition|(
name|ngx_buf_in_memory
argument_list|(
name|cl
operator|->
name|buf
argument_list|)
condition|)
block|{
name|cl
operator|->
name|buf
operator|->
name|pos
operator|+=
name|sent
expr_stmt|;
block|}
if|if
condition|(
name|cl
operator|->
name|buf
operator|->
name|in_file
condition|)
block|{
name|cl
operator|->
name|buf
operator|->
name|file_pos
operator|+=
name|sent
expr_stmt|;
block|}
break|break;
block|}
if|if
condition|(
name|eintr
condition|)
block|{
continue|continue;
block|}
if|if
condition|(
operator|!
name|complete
condition|)
block|{
name|wev
operator|->
name|ready
operator|=
literal|0
expr_stmt|;
return|return
name|cl
return|;
block|}
if|if
condition|(
name|send
operator|>=
name|limit
operator|||
name|cl
operator|==
name|NULL
condition|)
block|{
return|return
name|cl
return|;
block|}
name|in
operator|=
name|cl
expr_stmt|;
block|}
block|}
end_function

end_unit

