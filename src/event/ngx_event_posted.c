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
name|ngx_delete_posted_event
argument_list|(
name|ev
argument_list|)
expr_stmt|;
if|#
directive|if
literal|0
comment|/* do not check instance ??? */
block_content|if (ev->accept) {             continue;         }
endif|#
directive|endif
if|if
condition|(
name|ev
operator|->
name|closed
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
DECL|function|ngx_wakeup_worker_thread (ngx_cycle_t * cycle)
name|void
name|ngx_wakeup_worker_thread
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
block|{
name|ngx_int_t
name|i
decl_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|ngx_threads_n
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|ngx_threads
index|[
name|i
index|]
operator|.
name|state
operator|==
name|NGX_THREAD_FREE
condition|)
block|{
name|ngx_cond_signal
argument_list|(
name|ngx_threads
index|[
name|i
index|]
operator|.
name|cv
argument_list|)
expr_stmt|;
return|return;
block|}
block|}
block|}
end_function

begin_function
DECL|function|ngx_event_thread_process_posted (ngx_cycle_t * cycle)
name|ngx_int_t
name|ngx_event_thread_process_posted
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
for|for
control|(
init|;
condition|;
control|)
block|{
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
return|return
name|NGX_OK
return|;
block|}
if|if
condition|(
name|ngx_trylock
argument_list|(
name|ev
operator|->
name|lock
argument_list|)
operator|==
literal|0
condition|)
block|{
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
literal|" is busy"
argument_list|,
name|ev
argument_list|)
expr_stmt|;
name|ev
operator|=
name|ev
operator|->
name|next
expr_stmt|;
continue|continue;
block|}
name|ngx_delete_posted_event
argument_list|(
name|ev
argument_list|)
expr_stmt|;
name|ngx_log_debug3
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|cycle
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"event instance: c:%d i:%d r:%d"
argument_list|,
name|ev
operator|->
name|closed
argument_list|,
name|ev
operator|->
name|instance
argument_list|,
name|ev
operator|->
name|returned_instance
argument_list|)
expr_stmt|;
if|if
condition|(
name|ev
operator|->
name|closed
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
comment|/*                  * The stale event from a file descriptor that was just                  * closed in this iteration.  We use ngx_cycle->log                  * because ev->log may be already destoyed.                  */
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|ngx_cycle
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
name|ngx_unlock
argument_list|(
name|ev
operator|->
name|lock
argument_list|)
expr_stmt|;
name|ev
operator|=
name|ev
operator|->
name|next
expr_stmt|;
continue|continue;
block|}
name|ev
operator|->
name|locked
operator|=
literal|1
expr_stmt|;
name|ev
operator|->
name|ready
operator||=
name|ev
operator|->
name|posted_ready
expr_stmt|;
name|ev
operator|->
name|timedout
operator||=
name|ev
operator|->
name|posted_timedout
expr_stmt|;
name|ev
operator|->
name|available
operator||=
name|ev
operator|->
name|posted_available
expr_stmt|;
name|ev
operator|->
name|pending_eof
operator||=
name|ev
operator|->
name|posted_eof
expr_stmt|;
if|#
directive|if
operator|(
name|HAVE_KQUEUE
operator|)
name|ev
operator|->
name|kq_errno
operator||=
name|ev
operator|->
name|posted_errno
expr_stmt|;
endif|#
directive|endif
name|ev
operator|->
name|posted_ready
operator|=
literal|0
expr_stmt|;
name|ev
operator|->
name|posted_timedout
operator|=
literal|0
expr_stmt|;
name|ev
operator|->
name|posted_available
operator|=
literal|0
expr_stmt|;
name|ev
operator|->
name|posted_eof
operator|=
literal|0
expr_stmt|;
if|#
directive|if
operator|(
name|HAVE_KQUEUE
operator|)
name|ev
operator|->
name|posted_errno
operator|=
literal|0
expr_stmt|;
endif|#
directive|endif
name|ngx_mutex_unlock
argument_list|(
name|ngx_posted_events_mutex
argument_list|)
expr_stmt|;
name|ev
operator|->
name|event_handler
argument_list|(
name|ev
argument_list|)
expr_stmt|;
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
if|if
condition|(
name|ev
operator|->
name|locked
condition|)
block|{
name|ngx_unlock
argument_list|(
name|ev
operator|->
name|lock
argument_list|)
expr_stmt|;
block|}
break|break;
block|}
block|}
block|}
end_function

begin_else
else|#
directive|else
end_else

begin_function
DECL|function|ngx_wakeup_worker_thread (ngx_cycle_t * cycle)
name|void
name|ngx_wakeup_worker_thread
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
block|{
block|}
end_function

begin_endif
endif|#
directive|endif
end_endif

end_unit

