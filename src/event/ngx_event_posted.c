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

begin_decl_stmt
DECL|variable|ngx_posted_events
name|ngx_thread_volatile
name|ngx_event_t
modifier|*
name|ngx_posted_events
decl_stmt|;
end_decl_stmt

begin_if
if|#
directive|if
operator|(
name|NGX_THREADS
operator|)
end_if

begin_decl_stmt
DECL|variable|ngx_posted_events_mutex
name|ngx_mutex_t
modifier|*
name|ngx_posted_events_mutex
decl_stmt|;
end_decl_stmt

begin_endif
endif|#
directive|endif
end_endif

begin_function
DECL|function|ngx_event_process_posted (ngx_cycle_t * cycle)
name|void
name|ngx_event_process_posted
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
block|{
name|ngx_event_t
modifier|*
name|ev
decl_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
name|ev
operator|=
operator|(
name|ngx_event_t
operator|*
operator|)
name|ngx_posted_events
expr_stmt|;
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|cycle
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"posted event "
name|PTR_FMT
argument_list|,
name|ev
argument_list|)
expr_stmt|;
if|if
condition|(
name|ev
operator|==
name|NULL
condition|)
block|{
return|return;
block|}
name|ngx_posted_events
operator|=
name|ev
operator|->
name|next
expr_stmt|;
if|if
condition|(
operator|(
operator|!
name|ev
operator|->
name|posted
operator|&&
operator|!
name|ev
operator|->
name|active
operator|)
operator|||
operator|(
name|ev
operator|->
name|use_instance
operator|&&
name|ev
operator|->
name|instance
operator|!=
name|ev
operator|->
name|returned_instance
operator|)
condition|)
block|{
comment|/*              * the stale event from a file descriptor              * that was just closed in this iteration              */
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|cycle
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"stale posted event "
name|PTR_FMT
argument_list|,
name|ev
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|ev
operator|->
name|posted
condition|)
block|{
name|ev
operator|->
name|posted
operator|=
literal|0
expr_stmt|;
block|}
name|ev
operator|->
name|event_handler
argument_list|(
name|ev
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_if
if|#
directive|if
operator|(
name|NGX_THREADS
operator|)
end_if

begin_function
DECL|function|ngx_event_thread_handler (ngx_event_t * ev)
name|void
name|ngx_event_thread_handler
parameter_list|(
name|ngx_event_t
modifier|*
name|ev
parameter_list|)
block|{
if|if
condition|(
operator|(
operator|!
name|ev
operator|->
name|posted
operator|&&
operator|!
name|ev
operator|->
name|active
operator|)
operator|||
operator|(
name|ev
operator|->
name|use_instance
operator|&&
name|ev
operator|->
name|instance
operator|!=
name|ev
operator|->
name|returned_instance
operator|)
condition|)
block|{
comment|/*          * the stale event from a file descriptor          * that was just closed in this iteration          */
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|ev
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"kevent: stale event "
name|PTR_FMT
argument_list|,
name|ev
argument_list|)
expr_stmt|;
return|return;
block|}
if|if
condition|(
name|ev
operator|->
name|posted
condition|)
block|{
name|ev
operator|->
name|posted
operator|=
literal|0
expr_stmt|;
block|}
name|ev
operator|->
name|event_handler
argument_list|(
name|ev
argument_list|)
expr_stmt|;
block|}
end_function

begin_endif
endif|#
directive|endif
end_endif

end_unit

