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

begin_function
DECL|function|ngx_create_temp_hunk (ngx_pool_t * pool,size_t size)
name|ngx_hunk_t
modifier|*
name|ngx_create_temp_hunk
parameter_list|(
name|ngx_pool_t
modifier|*
name|pool
parameter_list|,
name|size_t
name|size
parameter_list|)
block|{
name|ngx_hunk_t
modifier|*
name|h
decl_stmt|;
name|ngx_test_null
argument_list|(
name|h
argument_list|,
name|ngx_alloc_hunk
argument_list|(
name|pool
argument_list|)
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|ngx_test_null
argument_list|(
name|h
operator|->
name|start
argument_list|,
name|ngx_palloc
argument_list|(
name|pool
argument_list|,
name|size
argument_list|)
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|h
operator|->
name|pos
operator|=
name|h
operator|->
name|start
expr_stmt|;
name|h
operator|->
name|last
operator|=
name|h
operator|->
name|start
expr_stmt|;
name|h
operator|->
name|file_pos
operator|=
literal|0
expr_stmt|;
name|h
operator|->
name|file_last
operator|=
literal|0
expr_stmt|;
name|h
operator|->
name|end
operator|=
name|h
operator|->
name|last
operator|+
name|size
expr_stmt|;
name|h
operator|->
name|type
operator|=
name|NGX_HUNK_TEMP
operator||
name|NGX_HUNK_IN_MEMORY
expr_stmt|;
name|h
operator|->
name|file
operator|=
name|NULL
expr_stmt|;
name|h
operator|->
name|shadow
operator|=
name|NULL
expr_stmt|;
name|h
operator|->
name|tag
operator|=
literal|0
expr_stmt|;
return|return
name|h
return|;
block|}
end_function

begin_function
DECL|function|ngx_create_chain_of_hunks (ngx_pool_t * pool,ngx_bufs_t * bufs)
name|ngx_chain_t
modifier|*
name|ngx_create_chain_of_hunks
parameter_list|(
name|ngx_pool_t
modifier|*
name|pool
parameter_list|,
name|ngx_bufs_t
modifier|*
name|bufs
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
name|char
modifier|*
name|p
decl_stmt|;
name|ngx_hunk_t
modifier|*
name|h
decl_stmt|;
name|ngx_chain_t
modifier|*
name|chain
decl_stmt|,
modifier|*
name|cl
decl_stmt|,
modifier|*
modifier|*
name|ll
decl_stmt|;
name|ngx_test_null
argument_list|(
name|p
argument_list|,
name|ngx_palloc
argument_list|(
name|pool
argument_list|,
name|bufs
operator|->
name|num
operator|*
name|bufs
operator|->
name|size
argument_list|)
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|ll
operator|=
operator|&
name|chain
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|bufs
operator|->
name|num
condition|;
name|i
operator|++
control|)
block|{
name|ngx_test_null
argument_list|(
name|h
argument_list|,
name|ngx_alloc_hunk
argument_list|(
name|pool
argument_list|)
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|h
operator|->
name|pos
operator|=
name|p
expr_stmt|;
name|h
operator|->
name|last
operator|=
name|p
expr_stmt|;
name|h
operator|->
name|file_pos
operator|=
literal|0
expr_stmt|;
name|h
operator|->
name|file_last
operator|=
literal|0
expr_stmt|;
name|h
operator|->
name|type
operator|=
name|NGX_HUNK_IN_MEMORY
operator||
name|NGX_HUNK_TEMP
expr_stmt|;
name|h
operator|->
name|start
operator|=
name|p
expr_stmt|;
name|p
operator|+=
name|bufs
operator|->
name|size
expr_stmt|;
name|h
operator|->
name|end
operator|=
name|p
expr_stmt|;
name|h
operator|->
name|file
operator|=
name|NULL
expr_stmt|;
name|h
operator|->
name|shadow
operator|=
name|NULL
expr_stmt|;
name|h
operator|->
name|tag
operator|=
literal|0
expr_stmt|;
name|ngx_test_null
argument_list|(
name|cl
argument_list|,
name|ngx_alloc_chain_link
argument_list|(
name|pool
argument_list|)
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|cl
operator|->
name|hunk
operator|=
name|h
expr_stmt|;
operator|*
name|ll
operator|=
name|cl
expr_stmt|;
name|ll
operator|=
operator|&
name|cl
operator|->
name|next
expr_stmt|;
block|}
operator|*
name|ll
operator|=
name|NULL
expr_stmt|;
return|return
name|chain
return|;
block|}
end_function

begin_function
DECL|function|ngx_chain_add_copy (ngx_pool_t * pool,ngx_chain_t ** chain,ngx_chain_t * in)
name|int
name|ngx_chain_add_copy
parameter_list|(
name|ngx_pool_t
modifier|*
name|pool
parameter_list|,
name|ngx_chain_t
modifier|*
modifier|*
name|chain
parameter_list|,
name|ngx_chain_t
modifier|*
name|in
parameter_list|)
block|{
name|ngx_chain_t
modifier|*
name|cl
decl_stmt|,
modifier|*
modifier|*
name|ll
decl_stmt|;
name|ll
operator|=
name|chain
expr_stmt|;
for|for
control|(
name|cl
operator|=
operator|*
name|chain
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
name|ll
operator|=
operator|&
name|cl
operator|->
name|next
expr_stmt|;
block|}
while|while
condition|(
name|in
condition|)
block|{
name|ngx_test_null
argument_list|(
name|cl
argument_list|,
name|ngx_alloc_chain_link
argument_list|(
name|pool
argument_list|)
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
name|cl
operator|->
name|hunk
operator|=
name|in
operator|->
name|hunk
expr_stmt|;
operator|*
name|ll
operator|=
name|cl
expr_stmt|;
name|ll
operator|=
operator|&
name|cl
operator|->
name|next
expr_stmt|;
name|in
operator|=
name|in
operator|->
name|next
expr_stmt|;
block|}
operator|*
name|ll
operator|=
name|NULL
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_chain_update_chains (ngx_chain_t ** free,ngx_chain_t ** busy,ngx_chain_t ** out,ngx_hunk_tag_t tag)
name|void
name|ngx_chain_update_chains
parameter_list|(
name|ngx_chain_t
modifier|*
modifier|*
name|free
parameter_list|,
name|ngx_chain_t
modifier|*
modifier|*
name|busy
parameter_list|,
name|ngx_chain_t
modifier|*
modifier|*
name|out
parameter_list|,
name|ngx_hunk_tag_t
name|tag
parameter_list|)
block|{
name|ngx_chain_t
modifier|*
name|te
decl_stmt|;
if|if
condition|(
operator|*
name|busy
operator|==
name|NULL
condition|)
block|{
operator|*
name|busy
operator|=
operator|*
name|out
expr_stmt|;
block|}
else|else
block|{
for|for
control|(
name|te
operator|=
operator|*
name|busy
init|;
comment|/* void */
condition|;
name|te
operator|=
name|te
operator|->
name|next
control|)
block|{
if|if
condition|(
name|te
operator|->
name|next
operator|==
name|NULL
condition|)
block|{
name|te
operator|->
name|next
operator|=
operator|*
name|out
expr_stmt|;
break|break;
block|}
block|}
block|}
operator|*
name|out
operator|=
name|NULL
expr_stmt|;
while|while
condition|(
operator|*
name|busy
condition|)
block|{
if|if
condition|(
name|ngx_hunk_size
argument_list|(
operator|(
operator|*
name|busy
operator|)
operator|->
name|hunk
argument_list|)
operator|!=
literal|0
condition|)
block|{
break|break;
block|}
if|#
directive|if
operator|(
name|HAVE_WRITE_ZEROCOPY
operator|)
if|if
condition|(
operator|(
operator|*
name|busy
operator|)
operator|->
name|hunk
operator|->
name|type
operator|&
name|NGX_HUNK_ZEROCOPY_BUSY
condition|)
block|{
break|break;
block|}
endif|#
directive|endif
if|if
condition|(
operator|(
operator|*
name|busy
operator|)
operator|->
name|hunk
operator|->
name|tag
operator|!=
name|tag
condition|)
block|{
operator|*
name|busy
operator|=
operator|(
operator|*
name|busy
operator|)
operator|->
name|next
expr_stmt|;
continue|continue;
block|}
operator|(
operator|*
name|busy
operator|)
operator|->
name|hunk
operator|->
name|pos
operator|=
operator|(
operator|*
name|busy
operator|)
operator|->
name|hunk
operator|->
name|last
operator|=
operator|(
operator|*
name|busy
operator|)
operator|->
name|hunk
operator|->
name|start
expr_stmt|;
name|te
operator|=
operator|*
name|busy
expr_stmt|;
operator|*
name|busy
operator|=
operator|(
operator|*
name|busy
operator|)
operator|->
name|next
expr_stmt|;
name|te
operator|->
name|next
operator|=
operator|*
name|free
expr_stmt|;
operator|*
name|free
operator|=
name|te
expr_stmt|;
block|}
block|}
end_function

end_unit

