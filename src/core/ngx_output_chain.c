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

begin_define
DECL|macro|NGX_NONE
define|#
directive|define
name|NGX_NONE
value|1
end_define

begin_function_decl
name|ngx_inline
specifier|static
name|int
name|ngx_output_chain_need_to_copy
parameter_list|(
name|ngx_output_chain_ctx_t
modifier|*
name|ctx
parameter_list|,
name|ngx_hunk_t
modifier|*
name|hunk
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|int
name|ngx_output_chain_copy_hunk
parameter_list|(
name|ngx_hunk_t
modifier|*
name|dst
parameter_list|,
name|ngx_hunk_t
modifier|*
name|src
parameter_list|,
name|u_int
name|sendfile
parameter_list|)
function_decl|;
end_function_decl

begin_function
DECL|function|ngx_output_chain (ngx_output_chain_ctx_t * ctx,ngx_chain_t * in)
name|int
name|ngx_output_chain
parameter_list|(
name|ngx_output_chain_ctx_t
modifier|*
name|ctx
parameter_list|,
name|ngx_chain_t
modifier|*
name|in
parameter_list|)
block|{
name|int
name|rc
decl_stmt|,
name|last
decl_stmt|;
name|size_t
name|size
decl_stmt|,
name|hsize
decl_stmt|;
name|ngx_chain_t
modifier|*
name|cl
decl_stmt|,
modifier|*
name|out
decl_stmt|,
modifier|*
modifier|*
name|last_out
decl_stmt|;
comment|/*      * the short path for the case when the chain ctx->in is empty      * and the incoming chain is empty too or it has the single hunk      * that does not require the copy      */
if|if
condition|(
name|ctx
operator|->
name|in
operator|==
name|NULL
condition|)
block|{
if|if
condition|(
name|in
operator|==
name|NULL
condition|)
block|{
return|return
name|ctx
operator|->
name|output_filter
argument_list|(
name|ctx
operator|->
name|output_ctx
argument_list|,
name|in
argument_list|)
return|;
block|}
if|if
condition|(
name|in
operator|->
name|next
operator|==
name|NULL
operator|&&
operator|(
operator|!
name|ngx_output_chain_need_to_copy
argument_list|(
name|ctx
argument_list|,
name|in
operator|->
name|hunk
argument_list|)
operator|)
condition|)
block|{
return|return
name|ctx
operator|->
name|output_filter
argument_list|(
name|ctx
operator|->
name|output_ctx
argument_list|,
name|in
argument_list|)
return|;
block|}
block|}
comment|/* add the incoming hunk to the chain ctx->in */
if|if
condition|(
name|in
condition|)
block|{
if|if
condition|(
name|ngx_chain_add_copy
argument_list|(
name|ctx
operator|->
name|pool
argument_list|,
operator|&
name|ctx
operator|->
name|in
argument_list|,
name|in
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
block|}
name|last
operator|=
name|NGX_NONE
expr_stmt|;
name|out
operator|=
name|NULL
expr_stmt|;
name|last_out
operator|=
operator|&
name|out
expr_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
while|while
condition|(
name|ctx
operator|->
name|in
condition|)
block|{
if|if
condition|(
operator|!
name|ngx_output_chain_need_to_copy
argument_list|(
name|ctx
argument_list|,
name|ctx
operator|->
name|in
operator|->
name|hunk
argument_list|)
condition|)
block|{
comment|/* move the chain link to the chain out */
name|cl
operator|=
name|ctx
operator|->
name|in
expr_stmt|;
name|ctx
operator|->
name|in
operator|=
name|cl
operator|->
name|next
expr_stmt|;
operator|*
name|last_out
operator|=
name|cl
expr_stmt|;
name|last_out
operator|=
operator|&
name|cl
operator|->
name|next
expr_stmt|;
name|cl
operator|->
name|next
operator|=
name|NULL
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|ctx
operator|->
name|hunk
operator|==
name|NULL
condition|)
block|{
comment|/* get the free hunk */
if|if
condition|(
name|ctx
operator|->
name|free
condition|)
block|{
name|ctx
operator|->
name|hunk
operator|=
name|ctx
operator|->
name|free
operator|->
name|hunk
expr_stmt|;
name|ctx
operator|->
name|free
operator|=
name|ctx
operator|->
name|free
operator|->
name|next
expr_stmt|;
block|}
if|else if
condition|(
name|ctx
operator|->
name|hunks
operator|<
name|ctx
operator|->
name|bufs
operator|.
name|num
condition|)
block|{
name|size
operator|=
name|ctx
operator|->
name|bufs
operator|.
name|size
expr_stmt|;
if|if
condition|(
name|ctx
operator|->
name|in
operator|->
name|hunk
operator|->
name|type
operator|&
name|NGX_HUNK_LAST
condition|)
block|{
name|hsize
operator|=
name|ngx_hunk_size
argument_list|(
name|ctx
operator|->
name|in
operator|->
name|hunk
argument_list|)
expr_stmt|;
if|if
condition|(
name|hsize
operator|<
name|ctx
operator|->
name|bufs
operator|.
name|size
condition|)
block|{
comment|/*                             * allocate small temp hunk for the small last hunk                             * or its small last part                             */
name|size
operator|=
name|hsize
expr_stmt|;
block|}
if|else if
condition|(
name|ctx
operator|->
name|bufs
operator|.
name|num
operator|==
literal|1
operator|&&
operator|(
name|hsize
operator|<
name|ctx
operator|->
name|bufs
operator|.
name|size
operator|+
operator|(
name|ctx
operator|->
name|bufs
operator|.
name|size
operator|>>
literal|2
operator|)
operator|)
condition|)
block|{
comment|/*                              * allocate a temp hunk that equals                              * to the last hunk if the last hunk size is lesser                              * than 1.25 of bufs.size and a temp hunk is single                              */
name|size
operator|=
name|hsize
expr_stmt|;
block|}
block|}
name|ngx_test_null
argument_list|(
name|ctx
operator|->
name|hunk
argument_list|,
name|ngx_create_temp_hunk
argument_list|(
name|ctx
operator|->
name|pool
argument_list|,
name|size
argument_list|)
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
name|ctx
operator|->
name|hunk
operator|->
name|tag
operator|=
name|ctx
operator|->
name|tag
expr_stmt|;
name|ctx
operator|->
name|hunk
operator|->
name|type
operator||=
name|NGX_HUNK_RECYCLED
expr_stmt|;
name|ctx
operator|->
name|hunks
operator|++
expr_stmt|;
block|}
else|else
block|{
break|break;
block|}
block|}
name|rc
operator|=
name|ngx_output_chain_copy_hunk
argument_list|(
name|ctx
operator|->
name|hunk
argument_list|,
name|ctx
operator|->
name|in
operator|->
name|hunk
argument_list|,
name|ctx
operator|->
name|sendfile
argument_list|)
expr_stmt|;
if|if
condition|(
name|rc
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|rc
return|;
block|}
if|if
condition|(
name|rc
operator|==
name|NGX_AGAIN
condition|)
block|{
if|if
condition|(
name|out
condition|)
block|{
break|break;
block|}
return|return
name|rc
return|;
block|}
comment|/* delete the completed hunk from the chain ctx->in */
if|if
condition|(
name|ngx_hunk_size
argument_list|(
name|ctx
operator|->
name|in
operator|->
name|hunk
argument_list|)
operator|==
literal|0
condition|)
block|{
name|ctx
operator|->
name|in
operator|=
name|ctx
operator|->
name|in
operator|->
name|next
expr_stmt|;
block|}
name|ngx_alloc_link_and_set_hunk
argument_list|(
name|cl
argument_list|,
name|ctx
operator|->
name|hunk
argument_list|,
name|ctx
operator|->
name|pool
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
operator|*
name|last_out
operator|=
name|cl
expr_stmt|;
name|last_out
operator|=
operator|&
name|cl
operator|->
name|next
expr_stmt|;
name|ctx
operator|->
name|hunk
operator|=
name|NULL
expr_stmt|;
if|if
condition|(
name|ctx
operator|->
name|free
operator|==
name|NULL
condition|)
block|{
break|break;
block|}
block|}
if|if
condition|(
name|out
operator|==
name|NULL
operator|&&
name|last
operator|!=
name|NGX_NONE
condition|)
block|{
return|return
name|last
return|;
block|}
name|last
operator|=
name|ctx
operator|->
name|output_filter
argument_list|(
name|ctx
operator|->
name|output_ctx
argument_list|,
name|out
argument_list|)
expr_stmt|;
name|ngx_chain_update_chains
argument_list|(
operator|&
name|ctx
operator|->
name|free
argument_list|,
operator|&
name|ctx
operator|->
name|busy
argument_list|,
operator|&
name|out
argument_list|,
name|ctx
operator|->
name|tag
argument_list|)
expr_stmt|;
name|last_out
operator|=
operator|&
name|out
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|ngx_output_chain_need_to_copy (ngx_output_chain_ctx_t * ctx,ngx_hunk_t * hunk)
name|ngx_inline
specifier|static
name|int
name|ngx_output_chain_need_to_copy
parameter_list|(
name|ngx_output_chain_ctx_t
modifier|*
name|ctx
parameter_list|,
name|ngx_hunk_t
modifier|*
name|hunk
parameter_list|)
block|{
if|if
condition|(
name|ngx_hunk_special
argument_list|(
name|hunk
argument_list|)
condition|)
block|{
return|return
literal|0
return|;
block|}
if|if
condition|(
operator|!
name|ctx
operator|->
name|sendfile
condition|)
block|{
if|if
condition|(
operator|!
operator|(
name|hunk
operator|->
name|type
operator|&
name|NGX_HUNK_IN_MEMORY
operator|)
condition|)
block|{
return|return
literal|1
return|;
block|}
name|hunk
operator|->
name|type
operator|&=
operator|~
name|NGX_HUNK_FILE
expr_stmt|;
block|}
if|if
condition|(
name|ctx
operator|->
name|need_in_memory
operator|&&
operator|(
operator|!
operator|(
name|hunk
operator|->
name|type
operator|&
name|NGX_HUNK_IN_MEMORY
operator|)
operator|)
condition|)
block|{
return|return
literal|1
return|;
block|}
if|if
condition|(
name|ctx
operator|->
name|need_in_temp
operator|&&
operator|(
name|hunk
operator|->
name|type
operator|&
operator|(
name|NGX_HUNK_MEMORY
operator||
name|NGX_HUNK_MMAP
operator|)
operator|)
condition|)
block|{
return|return
literal|1
return|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|ngx_output_chain_copy_hunk (ngx_hunk_t * dst,ngx_hunk_t * src,u_int sendfile)
specifier|static
name|int
name|ngx_output_chain_copy_hunk
parameter_list|(
name|ngx_hunk_t
modifier|*
name|dst
parameter_list|,
name|ngx_hunk_t
modifier|*
name|src
parameter_list|,
name|u_int
name|sendfile
parameter_list|)
block|{
name|size_t
name|size
decl_stmt|;
name|ssize_t
name|n
decl_stmt|;
name|size
operator|=
name|ngx_hunk_size
argument_list|(
name|src
argument_list|)
expr_stmt|;
if|if
condition|(
name|size
operator|>
operator|(
name|size_t
operator|)
operator|(
name|dst
operator|->
name|end
operator|-
name|dst
operator|->
name|pos
operator|)
condition|)
block|{
name|size
operator|=
name|dst
operator|->
name|end
operator|-
name|dst
operator|->
name|pos
expr_stmt|;
block|}
if|if
condition|(
name|src
operator|->
name|type
operator|&
name|NGX_HUNK_IN_MEMORY
condition|)
block|{
name|ngx_memcpy
argument_list|(
name|dst
operator|->
name|pos
argument_list|,
name|src
operator|->
name|pos
argument_list|,
name|size
argument_list|)
expr_stmt|;
name|src
operator|->
name|pos
operator|+=
name|size
expr_stmt|;
name|dst
operator|->
name|last
operator|+=
name|size
expr_stmt|;
if|if
condition|(
name|src
operator|->
name|type
operator|&
name|NGX_HUNK_FILE
condition|)
block|{
name|src
operator|->
name|file_pos
operator|+=
name|size
expr_stmt|;
block|}
if|if
condition|(
operator|(
name|src
operator|->
name|type
operator|&
name|NGX_HUNK_LAST
operator|)
operator|&&
name|src
operator|->
name|pos
operator|==
name|src
operator|->
name|last
condition|)
block|{
name|dst
operator|->
name|type
operator||=
name|NGX_HUNK_LAST
expr_stmt|;
block|}
block|}
else|else
block|{
name|n
operator|=
name|ngx_read_file
argument_list|(
name|src
operator|->
name|file
argument_list|,
name|dst
operator|->
name|pos
argument_list|,
name|size
argument_list|,
name|src
operator|->
name|file_pos
argument_list|)
expr_stmt|;
if|if
condition|(
name|n
operator|==
literal|0
condition|)
block|{
name|ngx_log_debug
argument_list|(
argument|src->file->log
argument_list|,
literal|"READ: %qd:%qd %X:%X %X:%X"
argument|_               src->file_pos _ src->file_last _               dst->pos _ dst->last _ dst->start _ dst->end
argument_list|)
empty_stmt|;
block|}
if|if
condition|(
name|n
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|n
return|;
block|}
if|#
directive|if
operator|(
name|NGX_FILE_AIO_READ
operator|)
if|if
condition|(
name|n
operator|==
name|NGX_AGAIN
condition|)
block|{
return|return
name|n
return|;
block|}
endif|#
directive|endif
if|if
condition|(
operator|(
name|size_t
operator|)
name|n
operator|!=
name|size
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|src
operator|->
name|file
operator|->
name|log
argument_list|,
literal|0
argument_list|,
name|ngx_read_file_n
literal|" reads only %d of %d from file"
argument_list|,
name|n
argument_list|,
name|size
argument_list|)
expr_stmt|;
if|if
condition|(
name|n
operator|==
literal|0
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
block|}
name|src
operator|->
name|file_pos
operator|+=
name|n
expr_stmt|;
name|dst
operator|->
name|last
operator|+=
name|n
expr_stmt|;
if|if
condition|(
operator|!
name|sendfile
condition|)
block|{
name|dst
operator|->
name|type
operator|&=
operator|~
name|NGX_HUNK_FILE
expr_stmt|;
block|}
if|if
condition|(
operator|(
name|src
operator|->
name|type
operator|&
name|NGX_HUNK_LAST
operator|)
operator|&&
name|src
operator|->
name|file_pos
operator|==
name|src
operator|->
name|file_last
condition|)
block|{
name|dst
operator|->
name|type
operator||=
name|NGX_HUNK_LAST
expr_stmt|;
block|}
block|}
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_chain_writer (void * data,ngx_chain_t * in)
name|int
name|ngx_chain_writer
parameter_list|(
name|void
modifier|*
name|data
parameter_list|,
name|ngx_chain_t
modifier|*
name|in
parameter_list|)
block|{
name|ngx_chain_writer_ctx_t
modifier|*
name|ctx
init|=
name|data
decl_stmt|;
name|ngx_chain_t
modifier|*
name|cl
decl_stmt|;
for|for
control|(
comment|/* void */
init|;
name|in
condition|;
name|in
operator|=
name|in
operator|->
name|next
control|)
block|{
name|ngx_alloc_link_and_set_hunk
argument_list|(
name|cl
argument_list|,
name|in
operator|->
name|hunk
argument_list|,
name|ctx
operator|->
name|pool
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
operator|*
name|ctx
operator|->
name|last
operator|=
name|cl
expr_stmt|;
name|ctx
operator|->
name|last
operator|=
operator|&
name|cl
operator|->
name|next
expr_stmt|;
block|}
name|ctx
operator|->
name|out
operator|=
name|ngx_write_chain
argument_list|(
name|ctx
operator|->
name|connection
argument_list|,
name|ctx
operator|->
name|out
argument_list|)
expr_stmt|;
if|if
condition|(
name|ctx
operator|->
name|out
operator|==
name|NGX_CHAIN_ERROR
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|ctx
operator|->
name|out
operator|==
name|NULL
condition|)
block|{
name|ctx
operator|->
name|last
operator|=
operator|&
name|ctx
operator|->
name|out
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
return|return
name|NGX_AGAIN
return|;
block|}
end_function

end_unit

