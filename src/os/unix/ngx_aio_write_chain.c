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
name|ngx_chain_t
modifier|*
DECL|function|ngx_aio_write_chain (ngx_connection_t * c,ngx_chain_t * in,off_t limit)
name|ngx_aio_write_chain
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
name|u_char
modifier|*
name|buf
decl_stmt|,
modifier|*
name|prev
decl_stmt|;
name|off_t
name|send
decl_stmt|,
name|sent
decl_stmt|;
name|size_t
name|len
decl_stmt|;
name|ssize_t
name|n
decl_stmt|,
name|size
decl_stmt|;
name|ngx_chain_t
modifier|*
name|cl
decl_stmt|;
comment|/* the maximum limit size is the maximum size_t value - the page size */
if|if
condition|(
name|limit
operator|==
literal|0
operator|||
name|limit
operator|>
operator|(
name|off_t
operator|)
operator|(
name|NGX_MAX_SIZE_T_VALUE
operator|-
name|ngx_pagesize
operator|)
condition|)
block|{
name|limit
operator|=
name|NGX_MAX_SIZE_T_VALUE
operator|-
name|ngx_pagesize
expr_stmt|;
block|}
name|send
operator|=
literal|0
expr_stmt|;
name|sent
operator|=
literal|0
expr_stmt|;
name|cl
operator|=
name|in
expr_stmt|;
while|while
condition|(
name|cl
condition|)
block|{
if|if
condition|(
name|cl
operator|->
name|buf
operator|->
name|pos
operator|==
name|cl
operator|->
name|buf
operator|->
name|last
condition|)
block|{
name|cl
operator|=
name|cl
operator|->
name|next
expr_stmt|;
continue|continue;
block|}
comment|/* we can post the single aio operation only */
if|if
condition|(
operator|!
name|c
operator|->
name|write
operator|->
name|ready
condition|)
block|{
return|return
name|cl
return|;
block|}
name|buf
operator|=
name|cl
operator|->
name|buf
operator|->
name|pos
expr_stmt|;
name|prev
operator|=
name|buf
expr_stmt|;
name|len
operator|=
literal|0
expr_stmt|;
comment|/* coalesce the neighbouring bufs */
while|while
condition|(
name|cl
operator|&&
name|prev
operator|==
name|cl
operator|->
name|buf
operator|->
name|pos
operator|&&
name|send
operator|<
name|limit
condition|)
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
name|len
operator|+=
name|size
expr_stmt|;
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
name|cl
operator|=
name|cl
operator|->
name|next
expr_stmt|;
block|}
name|n
operator|=
name|ngx_aio_write
argument_list|(
name|c
argument_list|,
name|buf
argument_list|,
name|len
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
literal|"aio_write: %z"
argument_list|,
name|n
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
operator|>
literal|0
condition|)
block|{
name|sent
operator|+=
name|n
expr_stmt|;
name|c
operator|->
name|sent
operator|+=
name|n
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
literal|"aio_write sent: %O"
argument_list|,
name|c
operator|->
name|sent
argument_list|)
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
name|sent
operator|>=
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
condition|)
block|{
name|sent
operator|-=
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
continue|continue;
block|}
name|cl
operator|->
name|buf
operator|->
name|pos
operator|+=
name|sent
expr_stmt|;
break|break;
block|}
block|}
return|return
name|cl
return|;
block|}
end_function

end_unit

