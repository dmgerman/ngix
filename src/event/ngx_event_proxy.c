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
file|<ngx_event_proxy.h>
end_include

begin_function_decl
specifier|static
name|int
name|ngx_event_proxy_write_chain_to_temp_file
parameter_list|(
name|ngx_event_proxy_t
modifier|*
name|p
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_inline
specifier|static
name|void
name|ngx_remove_shadow_links
parameter_list|(
name|ngx_hunk_t
modifier|*
name|hunk
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_inline
specifier|static
name|void
name|ngx_remove_shadow_free_raw_hunk
parameter_list|(
name|ngx_chain_t
modifier|*
modifier|*
name|free
parameter_list|,
name|ngx_hunk_t
modifier|*
name|h
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_inline
specifier|static
name|void
name|ngx_add_after_partially_filled_hunk
parameter_list|(
name|ngx_chain_t
modifier|*
modifier|*
name|chain
parameter_list|,
name|ngx_chain_t
modifier|*
name|ce
parameter_list|)
function_decl|;
end_function_decl

begin_function
DECL|function|ngx_event_proxy (ngx_event_proxy_t * p,int do_write)
name|int
name|ngx_event_proxy
parameter_list|(
name|ngx_event_proxy_t
modifier|*
name|p
parameter_list|,
name|int
name|do_write
parameter_list|)
block|{
for|for
control|(
init|;
condition|;
control|)
block|{
if|if
condition|(
name|do_write
condition|)
block|{
if|if
condition|(
name|ngx_event_proxy_write_to_downstream
argument_list|(
name|p
argument_list|)
operator|==
name|NGX_ABORT
condition|)
block|{
return|return
name|NGX_ABORT
return|;
block|}
block|}
name|p
operator|->
name|read
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|ngx_event_proxy_read_upstream
argument_list|(
name|p
argument_list|)
operator|==
name|NGX_ABORT
condition|)
block|{
return|return
name|NGX_ABORT
return|;
block|}
if|if
condition|(
operator|!
name|p
operator|->
name|read
condition|)
block|{
break|break;
block|}
name|do_write
operator|=
literal|1
expr_stmt|;
block|}
if|if
condition|(
name|ngx_handle_read_event
argument_list|(
name|p
operator|->
name|upstream
operator|->
name|read
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|NGX_ABORT
return|;
block|}
if|if
condition|(
name|ngx_handle_write_event
argument_list|(
name|p
operator|->
name|downstream
operator|->
name|write
argument_list|,
comment|/* TODO: lowat */
literal|0
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|NGX_ABORT
return|;
block|}
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_event_proxy_read_upstream (ngx_event_proxy_t * p)
name|int
name|ngx_event_proxy_read_upstream
parameter_list|(
name|ngx_event_proxy_t
modifier|*
name|p
parameter_list|)
block|{
name|int
name|n
decl_stmt|,
name|rc
decl_stmt|,
name|size
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
name|ce
decl_stmt|,
modifier|*
name|te
decl_stmt|;
name|ngx_log_debug
argument_list|(
argument|p->log
argument_list|,
literal|"read upstream: %d"
argument|_ p->upstream->read->ready
argument_list|)
empty_stmt|;
while|while
condition|(
name|p
operator|->
name|preread_hunks
operator|||
operator|(
name|p
operator|->
name|upstream
operator|->
name|read
operator|->
name|ready
operator|&&
operator|!
name|p
operator|->
name|upstream_done
operator|)
condition|)
block|{
if|if
condition|(
name|p
operator|->
name|preread_hunks
condition|)
block|{
comment|/* use the pre-read hunks if they exist */
name|p
operator|->
name|read
operator|=
literal|1
expr_stmt|;
name|chain
operator|=
name|p
operator|->
name|preread_hunks
expr_stmt|;
name|p
operator|->
name|preread_hunks
operator|=
name|NULL
expr_stmt|;
name|n
operator|=
name|p
operator|->
name|preread_size
expr_stmt|;
name|ngx_log_debug
argument_list|(
argument|p->log
argument_list|,
literal|"preread: %d"
argument|_ n
argument_list|)
empty_stmt|;
block|}
else|else
block|{
if|#
directive|if
operator|(
name|HAVE_KQUEUE
operator|)
comment|/*              * kqueue notifies about the end of file or a pending error.              * This test allows not to allocate a hunk on these conditions              * and not to call ngx_recv_chain().              */
if|if
condition|(
name|ngx_event_flags
operator|==
name|NGX_HAVE_KQUEUE_EVENT
condition|)
block|{
if|if
condition|(
name|p
operator|->
name|upstream
operator|->
name|read
operator|->
name|error
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ERR
argument_list|,
name|p
operator|->
name|log
argument_list|,
name|p
operator|->
name|upstream
operator|->
name|read
operator|->
name|error
argument_list|,
literal|"readv() failed"
argument_list|)
expr_stmt|;
name|p
operator|->
name|upstream_error
operator|=
literal|1
expr_stmt|;
break|break;
block|}
if|else if
condition|(
name|p
operator|->
name|upstream
operator|->
name|read
operator|->
name|eof
operator|&&
name|p
operator|->
name|upstream
operator|->
name|read
operator|->
name|available
operator|==
literal|0
condition|)
block|{
name|p
operator|->
name|upstream_eof
operator|=
literal|1
expr_stmt|;
name|p
operator|->
name|read
operator|=
literal|1
expr_stmt|;
break|break;
block|}
block|}
endif|#
directive|endif
if|if
condition|(
name|p
operator|->
name|free_raw_hunks
condition|)
block|{
comment|/* use the free hunks if they exist */
name|chain
operator|=
name|p
operator|->
name|free_raw_hunks
expr_stmt|;
name|p
operator|->
name|free_raw_hunks
operator|=
name|NULL
expr_stmt|;
block|}
if|else if
condition|(
name|p
operator|->
name|hunks
operator|<
name|p
operator|->
name|bufs
operator|.
name|num
condition|)
block|{
comment|/* allocate a new hunk if it's still allowed */
name|ngx_test_null
argument_list|(
name|h
argument_list|,
name|ngx_create_temp_hunk
argument_list|(
name|p
operator|->
name|pool
argument_list|,
name|p
operator|->
name|bufs
operator|.
name|size
argument_list|,
literal|0
argument_list|,
literal|0
argument_list|)
argument_list|,
name|NGX_ABORT
argument_list|)
expr_stmt|;
name|p
operator|->
name|hunks
operator|++
expr_stmt|;
name|ngx_alloc_ce_and_set_hunk
argument_list|(
name|te
argument_list|,
name|h
argument_list|,
name|p
operator|->
name|pool
argument_list|,
name|NGX_ABORT
argument_list|)
expr_stmt|;
name|chain
operator|=
name|te
expr_stmt|;
block|}
if|else if
condition|(
operator|!
name|p
operator|->
name|cachable
operator|&&
name|p
operator|->
name|downstream
operator|->
name|write
operator|->
name|ready
condition|)
block|{
comment|/*                  * If the hunks are not needed to be saved in a cache and                  * a downstream is ready then write the hunks to a downstream.                  */
name|ngx_log_debug
argument_list|(
name|p
operator|->
name|log
argument_list|,
literal|"downstream ready"
argument_list|)
expr_stmt|;
break|break;
block|}
if|else if
condition|(
name|p
operator|->
name|temp_offset
operator|<
name|p
operator|->
name|max_temp_file_size
condition|)
block|{
comment|/*                  * If it's allowed then save some hunks from r->in                  * to a temporary file, and add them to a r->out chain.                  */
name|rc
operator|=
name|ngx_event_proxy_write_chain_to_temp_file
argument_list|(
name|p
argument_list|)
expr_stmt|;
name|ngx_log_debug
argument_list|(
argument|p->log
argument_list|,
literal|"temp offset: %d"
argument|_ p->temp_offset
argument_list|)
empty_stmt|;
if|if
condition|(
name|rc
operator|==
name|NGX_AGAIN
condition|)
block|{
if|if
condition|(
name|ngx_event_flags
operator|&
name|NGX_USE_LEVEL_EVENT
operator|&&
name|p
operator|->
name|upstream
operator|->
name|read
operator|->
name|active
operator|&&
name|p
operator|->
name|upstream
operator|->
name|read
operator|->
name|ready
condition|)
block|{
if|if
condition|(
name|ngx_del_event
argument_list|(
name|p
operator|->
name|upstream
operator|->
name|read
argument_list|,
name|NGX_READ_EVENT
argument_list|,
literal|0
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|NGX_ABORT
return|;
block|}
block|}
block|}
if|if
condition|(
name|rc
operator|!=
name|NGX_OK
condition|)
block|{
return|return
name|rc
return|;
block|}
name|chain
operator|=
name|p
operator|->
name|free_raw_hunks
expr_stmt|;
name|p
operator|->
name|free_raw_hunks
operator|=
name|NULL
expr_stmt|;
block|}
else|else
block|{
comment|/* if there're no hunks to read in then disable a level event */
name|ngx_log_debug
argument_list|(
name|p
operator|->
name|log
argument_list|,
literal|"no hunks to read in"
argument_list|)
expr_stmt|;
break|break;
block|}
name|n
operator|=
name|ngx_recv_chain
argument_list|(
name|p
operator|->
name|upstream
argument_list|,
name|chain
argument_list|)
expr_stmt|;
name|ngx_log_debug
argument_list|(
argument|p->log
argument_list|,
literal|"recv_chain: %d"
argument|_ n
argument_list|)
empty_stmt|;
if|if
condition|(
name|n
operator|==
name|NGX_ERROR
condition|)
block|{
name|p
operator|->
name|upstream_error
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
operator|==
name|NGX_AGAIN
condition|)
block|{
break|break;
block|}
comment|/* TODO THINK about eof */
name|p
operator|->
name|read
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|n
operator|==
literal|0
condition|)
block|{
name|p
operator|->
name|free_raw_hunks
operator|=
name|chain
expr_stmt|;
name|p
operator|->
name|upstream_eof
operator|=
literal|1
expr_stmt|;
break|break;
block|}
block|}
for|for
control|(
name|ce
operator|=
name|chain
init|;
name|ce
operator|&&
name|n
operator|>
literal|0
condition|;
name|ce
operator|=
name|ce
operator|->
name|next
control|)
block|{
name|ngx_remove_shadow_links
argument_list|(
name|ce
operator|->
name|hunk
argument_list|)
expr_stmt|;
name|size
operator|=
name|ce
operator|->
name|hunk
operator|->
name|end
operator|-
name|ce
operator|->
name|hunk
operator|->
name|last
expr_stmt|;
if|if
condition|(
name|n
operator|>=
name|size
condition|)
block|{
name|ce
operator|->
name|hunk
operator|->
name|last
operator|=
name|ce
operator|->
name|hunk
operator|->
name|end
expr_stmt|;
if|if
condition|(
name|p
operator|->
name|input_filter
argument_list|(
name|p
argument_list|,
name|ce
operator|->
name|hunk
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|NGX_ABORT
return|;
block|}
name|n
operator|-=
name|size
expr_stmt|;
name|chain
operator|=
name|ce
operator|->
name|next
expr_stmt|;
block|}
else|else
block|{
name|ce
operator|->
name|hunk
operator|->
name|last
operator|+=
name|n
expr_stmt|;
name|n
operator|=
literal|0
expr_stmt|;
block|}
block|}
name|p
operator|->
name|free_raw_hunks
operator|=
name|chain
expr_stmt|;
block|}
if|if
condition|(
operator|(
name|p
operator|->
name|upstream_eof
operator|||
name|p
operator|->
name|upstream_error
operator|)
operator|&&
name|p
operator|->
name|free_raw_hunks
condition|)
block|{
if|if
condition|(
name|p
operator|->
name|input_filter
argument_list|(
name|p
argument_list|,
name|p
operator|->
name|free_raw_hunks
operator|->
name|hunk
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|NGX_ABORT
return|;
block|}
comment|/* TODO: p->free_raw_hunk->next can be free()ed */
name|p
operator|->
name|free_raw_hunks
operator|=
name|p
operator|->
name|free_raw_hunks
operator|->
name|next
expr_stmt|;
block|}
if|if
condition|(
name|p
operator|->
name|cachable
operator|&&
name|p
operator|->
name|in
condition|)
block|{
if|if
condition|(
name|ngx_event_proxy_write_chain_to_temp_file
argument_list|(
name|p
argument_list|)
operator|==
name|NGX_ABORT
condition|)
block|{
return|return
name|NGX_ABORT
return|;
block|}
block|}
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_event_proxy_write_to_downstream (ngx_event_proxy_t * p)
name|int
name|ngx_event_proxy_write_to_downstream
parameter_list|(
name|ngx_event_proxy_t
modifier|*
name|p
parameter_list|)
block|{
name|int
name|rc
decl_stmt|;
name|ngx_hunk_t
modifier|*
name|h
decl_stmt|;
name|ngx_chain_t
modifier|*
name|out
decl_stmt|,
modifier|*
name|ce
decl_stmt|,
modifier|*
name|te
decl_stmt|;
name|ngx_log_debug
argument_list|(
argument|p->log
argument_list|,
literal|"write downstream: %d"
argument|_ p->downstream->write->ready
argument_list|)
empty_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
if|if
condition|(
operator|(
name|p
operator|->
name|upstream_eof
operator|||
name|p
operator|->
name|upstream_error
operator|||
name|p
operator|->
name|upstream_done
operator|)
operator|&&
name|p
operator|->
name|out
operator|==
name|NULL
operator|&&
name|p
operator|->
name|in
operator|==
name|NULL
condition|)
block|{
name|p
operator|->
name|downstream_done
operator|=
literal|1
expr_stmt|;
break|break;
block|}
if|if
condition|(
operator|!
name|p
operator|->
name|downstream
operator|->
name|write
operator|->
name|ready
condition|)
block|{
break|break;
block|}
if|if
condition|(
name|p
operator|->
name|out
condition|)
block|{
name|out
operator|=
name|p
operator|->
name|out
expr_stmt|;
if|if
condition|(
name|p
operator|->
name|busy_len
operator|+
name|ngx_hunk_size
argument_list|(
name|out
operator|->
name|hunk
argument_list|)
operator|>
name|p
operator|->
name|max_busy_len
condition|)
block|{
break|break;
block|}
name|p
operator|->
name|out
operator|=
name|p
operator|->
name|out
operator|->
name|next
expr_stmt|;
name|ngx_remove_shadow_free_raw_hunk
argument_list|(
operator|&
name|p
operator|->
name|free_raw_hunks
argument_list|,
name|out
operator|->
name|hunk
argument_list|)
expr_stmt|;
block|}
if|else if
condition|(
operator|!
name|p
operator|->
name|cachable
operator|&&
name|p
operator|->
name|in
condition|)
block|{
name|out
operator|=
name|p
operator|->
name|in
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|p
operator|->
name|upstream_eof
operator|||
name|p
operator|->
name|upstream_error
operator|||
name|p
operator|->
name|upstream_done
operator|)
operator|&&
operator|(
name|p
operator|->
name|busy_len
operator|+
name|ngx_hunk_size
argument_list|(
name|out
operator|->
name|hunk
argument_list|)
operator|>
name|p
operator|->
name|max_busy_len
operator|)
condition|)
block|{
break|break;
block|}
name|p
operator|->
name|in
operator|=
name|p
operator|->
name|in
operator|->
name|next
expr_stmt|;
block|}
else|else
block|{
break|break;
block|}
name|out
operator|->
name|next
operator|=
name|NULL
expr_stmt|;
name|rc
operator|=
name|p
operator|->
name|output_filter
argument_list|(
name|p
operator|->
name|output_ctx
argument_list|,
name|out
operator|->
name|hunk
argument_list|)
expr_stmt|;
if|if
condition|(
name|rc
operator|==
name|NGX_ERROR
condition|)
block|{
comment|/* TODO */
block|}
name|ngx_chain_update_chains
argument_list|(
operator|&
name|p
operator|->
name|free
argument_list|,
operator|&
name|p
operator|->
name|busy
argument_list|,
operator|&
name|out
argument_list|)
expr_stmt|;
comment|/* calculate p->busy_len */
name|p
operator|->
name|busy_len
operator|=
literal|0
expr_stmt|;
for|for
control|(
name|ce
operator|=
name|p
operator|->
name|busy
init|;
name|ce
condition|;
name|ce
operator|=
name|ce
operator|->
name|next
control|)
block|{
name|p
operator|->
name|busy_len
operator|+=
name|ngx_hunk_size
argument_list|(
name|ce
operator|->
name|hunk
argument_list|)
expr_stmt|;
block|}
comment|/* add the free shadow raw hunks to p->free_raw_hunks */
for|for
control|(
name|ce
operator|=
name|p
operator|->
name|free
init|;
name|ce
condition|;
name|ce
operator|=
name|ce
operator|->
name|next
control|)
block|{
if|if
condition|(
name|ce
operator|->
name|hunk
operator|->
name|type
operator|&
name|NGX_HUNK_LAST_SHADOW
condition|)
block|{
name|h
operator|=
name|ce
operator|->
name|hunk
operator|->
name|shadow
expr_stmt|;
comment|/* THINK NEEDED ??? */
name|h
operator|->
name|pos
operator|=
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
name|shadow
operator|=
name|NULL
expr_stmt|;
name|ngx_alloc_ce_and_set_hunk
argument_list|(
name|te
argument_list|,
name|ce
operator|->
name|hunk
operator|->
name|shadow
argument_list|,
name|p
operator|->
name|pool
argument_list|,
name|NGX_ABORT
argument_list|)
expr_stmt|;
name|ngx_add_after_partially_filled_hunk
argument_list|(
operator|&
name|p
operator|->
name|free_raw_hunks
argument_list|,
name|te
argument_list|)
expr_stmt|;
name|ce
operator|->
name|hunk
operator|->
name|type
operator|&=
operator|~
name|NGX_HUNK_LAST_SHADOW
expr_stmt|;
block|}
name|ce
operator|->
name|hunk
operator|->
name|shadow
operator|=
name|NULL
expr_stmt|;
block|}
if|#
directive|if
literal|0
comment|/* TODO THINK p->read_priority ??? */
block_content|if (p->upstream->read->ready) {             return;         }
endif|#
directive|endif
block|}
if|if
condition|(
name|p
operator|->
name|upstream_done
operator|&&
name|p
operator|->
name|in
operator|==
name|NULL
operator|&&
name|p
operator|->
name|out
operator|==
name|NULL
condition|)
block|{
name|p
operator|->
name|downstream_done
operator|=
literal|1
expr_stmt|;
block|}
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_event_proxy_write_chain_to_temp_file (ngx_event_proxy_t * p)
specifier|static
name|int
name|ngx_event_proxy_write_chain_to_temp_file
parameter_list|(
name|ngx_event_proxy_t
modifier|*
name|p
parameter_list|)
block|{
name|int
name|rc
decl_stmt|,
name|size
decl_stmt|;
name|ngx_hunk_t
modifier|*
name|h
decl_stmt|;
name|ngx_chain_t
modifier|*
name|ce
decl_stmt|,
modifier|*
name|te
decl_stmt|,
modifier|*
name|next
decl_stmt|,
modifier|*
name|in
decl_stmt|,
modifier|*
modifier|*
name|last
decl_stmt|,
modifier|*
modifier|*
name|last_free
decl_stmt|;
name|ngx_log_debug
argument_list|(
name|p
operator|->
name|log
argument_list|,
literal|"write to file"
argument_list|)
expr_stmt|;
if|if
condition|(
name|p
operator|->
name|temp_file
operator|->
name|fd
operator|==
name|NGX_INVALID_FILE
condition|)
block|{
name|rc
operator|=
name|ngx_create_temp_file
argument_list|(
name|p
operator|->
name|temp_file
argument_list|,
name|p
operator|->
name|temp_path
argument_list|,
name|p
operator|->
name|pool
argument_list|,
name|p
operator|->
name|cachable
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
name|NGX_ABORT
return|;
block|}
if|if
condition|(
name|rc
operator|==
name|NGX_AGAIN
condition|)
block|{
return|return
name|NGX_AGAIN
return|;
block|}
if|if
condition|(
operator|!
name|p
operator|->
name|cachable
operator|&&
name|p
operator|->
name|temp_file_warn
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_WARN
argument_list|,
name|p
operator|->
name|log
argument_list|,
literal|0
argument_list|,
name|p
operator|->
name|temp_file_warn
argument_list|)
expr_stmt|;
block|}
block|}
if|if
condition|(
operator|!
name|p
operator|->
name|cachable
condition|)
block|{
name|size
operator|=
literal|0
expr_stmt|;
name|ce
operator|=
name|p
operator|->
name|in
expr_stmt|;
do|do
block|{
name|size
operator|+=
name|ce
operator|->
name|hunk
operator|->
name|last
operator|-
name|ce
operator|->
name|hunk
operator|->
name|pos
expr_stmt|;
if|if
condition|(
name|size
operator|>=
name|p
operator|->
name|temp_file_write_size
condition|)
block|{
break|break;
block|}
name|ce
operator|=
name|ce
operator|->
name|next
expr_stmt|;
block|}
do|while
condition|(
name|ce
condition|)
do|;
name|in
operator|=
name|ce
operator|->
name|next
expr_stmt|;
name|last
operator|=
operator|&
name|ce
operator|->
name|next
expr_stmt|;
name|ce
operator|->
name|next
operator|=
name|NULL
expr_stmt|;
block|}
else|else
block|{
name|in
operator|=
name|NULL
expr_stmt|;
name|last
operator|=
operator|&
name|p
operator|->
name|in
expr_stmt|;
block|}
if|if
condition|(
name|ngx_write_chain_to_file
argument_list|(
name|p
operator|->
name|temp_file
argument_list|,
name|p
operator|->
name|in
argument_list|,
name|p
operator|->
name|temp_offset
argument_list|,
name|p
operator|->
name|pool
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|NGX_ABORT
return|;
block|}
for|for
control|(
name|last_free
operator|=
operator|&
name|p
operator|->
name|free_raw_hunks
init|;
operator|*
name|last_free
operator|!=
name|NULL
condition|;
name|last_free
operator|=
operator|&
operator|(
operator|*
name|last
operator|)
operator|->
name|next
control|)
block|{
comment|/* void */
block|}
for|for
control|(
name|ce
operator|=
name|p
operator|->
name|in
init|;
name|ce
condition|;
name|ce
operator|=
name|next
control|)
block|{
name|next
operator|=
name|ce
operator|->
name|next
expr_stmt|;
name|ce
operator|->
name|next
operator|=
name|NULL
expr_stmt|;
name|h
operator|=
name|ce
operator|->
name|hunk
expr_stmt|;
name|h
operator|->
name|type
operator||=
name|NGX_HUNK_FILE
expr_stmt|;
name|h
operator|->
name|file
operator|=
name|p
operator|->
name|temp_file
expr_stmt|;
name|h
operator|->
name|file_pos
operator|=
name|p
operator|->
name|temp_offset
expr_stmt|;
name|p
operator|->
name|temp_offset
operator|+=
name|h
operator|->
name|last
operator|-
name|h
operator|->
name|pos
expr_stmt|;
name|h
operator|->
name|file_last
operator|=
name|p
operator|->
name|temp_offset
expr_stmt|;
name|ngx_chain_add_ce
argument_list|(
name|p
operator|->
name|out
argument_list|,
name|p
operator|->
name|last_out
argument_list|,
name|ce
argument_list|)
expr_stmt|;
if|if
condition|(
name|h
operator|->
name|type
operator|&
name|NGX_HUNK_LAST_SHADOW
condition|)
block|{
name|h
operator|->
name|shadow
operator|->
name|last
operator|=
name|h
operator|->
name|shadow
operator|->
name|pos
operator|=
name|h
operator|->
name|shadow
operator|->
name|start
expr_stmt|;
name|ngx_alloc_ce_and_set_hunk
argument_list|(
name|te
argument_list|,
name|h
operator|->
name|shadow
argument_list|,
name|p
operator|->
name|pool
argument_list|,
name|NGX_ABORT
argument_list|)
expr_stmt|;
operator|*
name|last_free
operator|=
name|te
expr_stmt|;
name|last_free
operator|=
operator|&
name|te
operator|->
name|next
expr_stmt|;
block|}
block|}
name|p
operator|->
name|in
operator|=
name|in
expr_stmt|;
name|p
operator|->
name|last_in
operator|=
name|last
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_comment
comment|/* the copy input filter */
end_comment

begin_function
DECL|function|ngx_event_proxy_copy_input_filter (ngx_event_proxy_t * p,ngx_hunk_t * hunk)
name|int
name|ngx_event_proxy_copy_input_filter
parameter_list|(
name|ngx_event_proxy_t
modifier|*
name|p
parameter_list|,
name|ngx_hunk_t
modifier|*
name|hunk
parameter_list|)
block|{
name|ngx_hunk_t
modifier|*
name|h
decl_stmt|;
name|ngx_chain_t
modifier|*
name|ce
decl_stmt|;
if|if
condition|(
name|hunk
operator|->
name|pos
operator|==
name|hunk
operator|->
name|last
condition|)
block|{
return|return
name|NGX_OK
return|;
block|}
if|if
condition|(
name|p
operator|->
name|free
condition|)
block|{
name|h
operator|=
name|p
operator|->
name|free
operator|->
name|hunk
expr_stmt|;
name|p
operator|->
name|free
operator|=
name|p
operator|->
name|free
operator|->
name|next
expr_stmt|;
block|}
else|else
block|{
name|ngx_test_null
argument_list|(
name|h
argument_list|,
name|ngx_alloc_hunk
argument_list|(
name|p
operator|->
name|pool
argument_list|)
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
block|}
name|ngx_memcpy
argument_list|(
name|h
argument_list|,
name|hunk
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_hunk_t
argument_list|)
argument_list|)
expr_stmt|;
name|h
operator|->
name|shadow
operator|=
name|hunk
expr_stmt|;
name|h
operator|->
name|type
operator||=
name|NGX_HUNK_LAST_SHADOW
operator||
name|NGX_HUNK_RECYCLED
expr_stmt|;
name|hunk
operator|->
name|shadow
operator|=
name|h
expr_stmt|;
name|ngx_alloc_ce_and_set_hunk
argument_list|(
name|ce
argument_list|,
name|h
argument_list|,
name|p
operator|->
name|pool
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
name|ngx_chain_add_ce
argument_list|(
name|p
operator|->
name|in
argument_list|,
name|p
operator|->
name|last_in
argument_list|,
name|ce
argument_list|)
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_remove_shadow_links (ngx_hunk_t * hunk)
name|ngx_inline
specifier|static
name|void
name|ngx_remove_shadow_links
parameter_list|(
name|ngx_hunk_t
modifier|*
name|hunk
parameter_list|)
block|{
name|ngx_hunk_t
modifier|*
name|h
decl_stmt|,
modifier|*
name|next
decl_stmt|;
if|if
condition|(
name|hunk
operator|->
name|shadow
operator|==
name|NULL
condition|)
block|{
return|return;
block|}
name|h
operator|=
name|hunk
operator|->
name|shadow
expr_stmt|;
while|while
condition|(
operator|!
operator|(
name|h
operator|->
name|type
operator|&
name|NGX_HUNK_LAST_SHADOW
operator|)
condition|)
block|{
name|next
operator|=
name|h
operator|->
name|shadow
expr_stmt|;
name|h
operator|->
name|type
operator|&=
operator|~
operator|(
name|NGX_HUNK_TEMP
operator||
name|NGX_HUNK_IN_MEMORY
operator||
name|NGX_HUNK_RECYCLED
operator|)
expr_stmt|;
name|h
operator|->
name|shadow
operator|=
name|NULL
expr_stmt|;
name|h
operator|=
name|next
expr_stmt|;
block|}
name|h
operator|->
name|type
operator|&=
operator|~
operator|(
name|NGX_HUNK_TEMP
operator||
name|NGX_HUNK_IN_MEMORY
operator||
name|NGX_HUNK_RECYCLED
operator||
name|NGX_HUNK_LAST_SHADOW
operator|)
expr_stmt|;
name|h
operator|->
name|shadow
operator|=
name|NULL
expr_stmt|;
name|hunk
operator|->
name|shadow
operator|=
name|NULL
expr_stmt|;
block|}
end_function

begin_function
DECL|function|ngx_remove_shadow_free_raw_hunk (ngx_chain_t ** free,ngx_hunk_t * h)
name|ngx_inline
specifier|static
name|void
name|ngx_remove_shadow_free_raw_hunk
parameter_list|(
name|ngx_chain_t
modifier|*
modifier|*
name|free
parameter_list|,
name|ngx_hunk_t
modifier|*
name|h
parameter_list|)
block|{
name|ngx_hunk_t
modifier|*
name|s
decl_stmt|;
name|ngx_chain_t
modifier|*
name|ce
decl_stmt|,
modifier|*
modifier|*
name|le
decl_stmt|;
if|if
condition|(
name|h
operator|->
name|shadow
operator|==
name|NULL
condition|)
block|{
return|return;
block|}
for|for
control|(
name|s
operator|=
name|h
operator|->
name|shadow
init|;
operator|!
operator|(
name|s
operator|->
name|type
operator|&
name|NGX_HUNK_LAST_SHADOW
operator|)
condition|;
name|s
operator|=
name|s
operator|->
name|shadow
control|)
block|{
comment|/* void */
block|}
name|le
operator|=
name|free
expr_stmt|;
for|for
control|(
name|ce
operator|=
operator|*
name|free
init|;
name|ce
condition|;
name|ce
operator|=
name|ce
operator|->
name|next
control|)
block|{
if|if
condition|(
name|ce
operator|->
name|hunk
operator|==
name|s
condition|)
block|{
operator|*
name|le
operator|=
name|ce
operator|->
name|next
expr_stmt|;
break|break;
block|}
if|if
condition|(
name|ce
operator|->
name|hunk
operator|->
name|shadow
condition|)
block|{
break|break;
block|}
name|le
operator|=
operator|&
name|ce
operator|->
name|next
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|ngx_add_after_partially_filled_hunk (ngx_chain_t ** chain,ngx_chain_t * ce)
name|ngx_inline
specifier|static
name|void
name|ngx_add_after_partially_filled_hunk
parameter_list|(
name|ngx_chain_t
modifier|*
modifier|*
name|chain
parameter_list|,
name|ngx_chain_t
modifier|*
name|ce
parameter_list|)
block|{
if|if
condition|(
operator|*
name|chain
operator|==
name|NULL
condition|)
block|{
operator|*
name|chain
operator|=
name|ce
expr_stmt|;
return|return;
block|}
if|if
condition|(
operator|(
operator|*
name|chain
operator|)
operator|->
name|hunk
operator|->
name|pos
operator|!=
operator|(
operator|*
name|chain
operator|)
operator|->
name|hunk
operator|->
name|last
condition|)
block|{
name|ce
operator|->
name|next
operator|=
operator|(
operator|*
name|chain
operator|)
operator|->
name|next
expr_stmt|;
operator|(
operator|*
name|chain
operator|)
operator|->
name|next
operator|=
name|ce
expr_stmt|;
block|}
else|else
block|{
name|ce
operator|->
name|next
operator|=
operator|(
operator|*
name|chain
operator|)
expr_stmt|;
operator|(
operator|*
name|chain
operator|)
operator|=
name|ce
expr_stmt|;
block|}
block|}
end_function

end_unit

