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
file|<ngx_aio.h>
end_include

begin_function
DECL|function|ngx_aio_write_chain (ngx_connection_t * c,ngx_chain_t * in)
name|ngx_chain_t
modifier|*
name|ngx_aio_write_chain
parameter_list|(
name|ngx_connection_t
modifier|*
name|c
parameter_list|,
name|ngx_chain_t
modifier|*
name|in
parameter_list|)
block|{
name|int
name|n
decl_stmt|;
name|char
modifier|*
name|buf
decl_stmt|,
modifier|*
name|prev
decl_stmt|;
name|off_t
name|sent
decl_stmt|;
name|size_t
name|size
decl_stmt|;
name|ngx_err_t
name|err
decl_stmt|;
name|ngx_chain_t
modifier|*
name|cl
decl_stmt|;
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
name|hunk
operator|->
name|last
operator|-
name|cl
operator|->
name|hunk
operator|->
name|pos
operator|==
literal|0
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
name|c
operator|->
name|write
operator|->
name|active
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
name|hunk
operator|->
name|pos
expr_stmt|;
name|prev
operator|=
name|buf
expr_stmt|;
name|size
operator|=
literal|0
expr_stmt|;
comment|/* coalesce the neighbouring hunks */
while|while
condition|(
name|cl
operator|&&
name|prev
operator|==
name|cl
operator|->
name|hunk
operator|->
name|pos
condition|)
block|{
name|size
operator|+=
name|cl
operator|->
name|hunk
operator|->
name|last
operator|-
name|cl
operator|->
name|hunk
operator|->
name|pos
expr_stmt|;
name|prev
operator|=
name|cl
operator|->
name|hunk
operator|->
name|last
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
name|size
argument_list|)
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_DEBUG_WRITE_CHAIN
operator|)
name|ngx_log_debug
argument_list|(
argument|c->log
argument_list|,
literal|"aio_write: %d"
argument|_ n
argument_list|)
empty_stmt|;
endif|#
directive|endif
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
if|#
directive|if
operator|(
name|NGX_DEBUG_WRITE_CHAIN
operator|)
name|ngx_log_debug
argument_list|(
argument|c->log
argument_list|,
literal|"aio_write sent: "
argument|OFF_FMT _ c->sent
argument_list|)
empty_stmt|;
endif|#
directive|endif
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
name|hunk
operator|->
name|last
operator|-
name|cl
operator|->
name|hunk
operator|->
name|pos
condition|)
block|{
name|sent
operator|-=
name|cl
operator|->
name|hunk
operator|->
name|last
operator|-
name|cl
operator|->
name|hunk
operator|->
name|pos
expr_stmt|;
name|cl
operator|->
name|hunk
operator|->
name|pos
operator|=
name|cl
operator|->
name|hunk
operator|->
name|last
expr_stmt|;
continue|continue;
block|}
name|cl
operator|->
name|hunk
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

