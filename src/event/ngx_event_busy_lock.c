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

begin_function_decl
specifier|static
name|int
name|ngx_event_busy_lock_look_cachable
parameter_list|(
name|ngx_event_busy_lock_t
modifier|*
name|bl
parameter_list|,
name|ngx_event_busy_lock_ctx_t
modifier|*
name|ctx
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|void
name|ngx_event_busy_lock_handler
parameter_list|(
name|ngx_event_t
modifier|*
name|ev
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|void
name|ngx_event_busy_lock_posted_handler
parameter_list|(
name|ngx_event_t
modifier|*
name|ev
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/*  * NGX_OK:     the busy lock is held  * NGX_AGAIN:  the all busy locks are held but we will wait the specified time  * NGX_BUSY:   ctx->timer == 0: there are many the busy locks  *             ctx->timer != 0: there are many the waiting locks  * NGX_ERROR:  an error occured while the mutex locking  */
end_comment

begin_function
DECL|function|ngx_event_busy_lock (ngx_event_busy_lock_t * bl,ngx_event_busy_lock_ctx_t * ctx)
name|ngx_int_t
name|ngx_event_busy_lock
parameter_list|(
name|ngx_event_busy_lock_t
modifier|*
name|bl
parameter_list|,
name|ngx_event_busy_lock_ctx_t
modifier|*
name|ctx
parameter_list|)
block|{
name|ngx_int_t
name|rc
decl_stmt|;
if|if
condition|(
name|ngx_mutex_lock
argument_list|(
name|bl
operator|->
name|mutex
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|ngx_log_debug2
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|ctx
operator|->
name|event
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"event busy lock: b:%d mb:%d"
argument_list|,
name|bl
operator|->
name|busy
argument_list|,
name|bl
operator|->
name|max_busy
argument_list|)
expr_stmt|;
if|if
condition|(
name|bl
operator|->
name|busy
operator|<
name|bl
operator|->
name|max_busy
condition|)
block|{
name|bl
operator|->
name|busy
operator|++
expr_stmt|;
name|rc
operator|=
name|NGX_OK
expr_stmt|;
block|}
if|else if
condition|(
name|ctx
operator|->
name|timer
operator|&&
name|bl
operator|->
name|waiting
operator|<
name|bl
operator|->
name|max_waiting
condition|)
block|{
name|bl
operator|->
name|waiting
operator|++
expr_stmt|;
name|ngx_add_timer
argument_list|(
name|ctx
operator|->
name|event
argument_list|,
name|ctx
operator|->
name|timer
argument_list|)
expr_stmt|;
name|ctx
operator|->
name|event
operator|->
name|event_handler
operator|=
name|ngx_event_busy_lock_handler
expr_stmt|;
if|if
condition|(
name|bl
operator|->
name|events
condition|)
block|{
name|bl
operator|->
name|last
operator|->
name|next
operator|=
name|ctx
expr_stmt|;
block|}
else|else
block|{
name|bl
operator|->
name|events
operator|=
name|ctx
expr_stmt|;
block|}
name|bl
operator|->
name|last
operator|=
name|ctx
expr_stmt|;
name|rc
operator|=
name|NGX_AGAIN
expr_stmt|;
block|}
else|else
block|{
name|rc
operator|=
name|NGX_BUSY
expr_stmt|;
block|}
name|ngx_mutex_unlock
argument_list|(
name|bl
operator|->
name|mutex
argument_list|)
expr_stmt|;
return|return
name|rc
return|;
block|}
end_function

begin_function
DECL|function|ngx_event_busy_lock_cachable (ngx_event_busy_lock_t * bl,ngx_event_busy_lock_ctx_t * ctx)
name|ngx_int_t
name|ngx_event_busy_lock_cachable
parameter_list|(
name|ngx_event_busy_lock_t
modifier|*
name|bl
parameter_list|,
name|ngx_event_busy_lock_ctx_t
modifier|*
name|ctx
parameter_list|)
block|{
name|ngx_int_t
name|rc
decl_stmt|;
if|if
condition|(
name|ngx_mutex_lock
argument_list|(
name|bl
operator|->
name|mutex
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|rc
operator|=
name|ngx_event_busy_lock_look_cachable
argument_list|(
name|bl
argument_list|,
name|ctx
argument_list|)
expr_stmt|;
name|ngx_log_debug3
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|ctx
operator|->
name|event
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"event busy lock: %d w:%d mw:%d"
argument_list|,
name|rc
argument_list|,
name|bl
operator|->
name|waiting
argument_list|,
name|bl
operator|->
name|max_waiting
argument_list|)
expr_stmt|;
comment|/*      * NGX_OK:     no the same request, there is free slot and we locked it      * NGX_BUSY:   no the same request and there is no free slot      * NGX_AGAIN:  the same request is processing      */
if|if
condition|(
name|rc
operator|==
name|NGX_AGAIN
condition|)
block|{
if|if
condition|(
name|ctx
operator|->
name|timer
operator|&&
name|bl
operator|->
name|waiting
operator|<
name|bl
operator|->
name|max_waiting
condition|)
block|{
name|bl
operator|->
name|waiting
operator|++
expr_stmt|;
name|ngx_add_timer
argument_list|(
name|ctx
operator|->
name|event
argument_list|,
name|ctx
operator|->
name|timer
argument_list|)
expr_stmt|;
name|ctx
operator|->
name|event
operator|->
name|event_handler
operator|=
name|ngx_event_busy_lock_handler
expr_stmt|;
if|if
condition|(
name|bl
operator|->
name|events
operator|==
name|NULL
condition|)
block|{
name|bl
operator|->
name|events
operator|=
name|ctx
expr_stmt|;
block|}
else|else
block|{
name|bl
operator|->
name|last
operator|->
name|next
operator|=
name|ctx
expr_stmt|;
block|}
name|bl
operator|->
name|last
operator|=
name|ctx
expr_stmt|;
block|}
else|else
block|{
name|rc
operator|=
name|NGX_BUSY
expr_stmt|;
block|}
block|}
name|ngx_mutex_unlock
argument_list|(
name|bl
operator|->
name|mutex
argument_list|)
expr_stmt|;
return|return
name|rc
return|;
block|}
end_function

begin_function
DECL|function|ngx_event_busy_unlock (ngx_event_busy_lock_t * bl,ngx_event_busy_lock_ctx_t * ctx)
name|ngx_int_t
name|ngx_event_busy_unlock
parameter_list|(
name|ngx_event_busy_lock_t
modifier|*
name|bl
parameter_list|,
name|ngx_event_busy_lock_ctx_t
modifier|*
name|ctx
parameter_list|)
block|{
name|ngx_event_t
modifier|*
name|ev
decl_stmt|;
name|ngx_event_busy_lock_ctx_t
modifier|*
name|wakeup
decl_stmt|;
if|if
condition|(
name|ngx_mutex_lock
argument_list|(
name|bl
operator|->
name|mutex
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
name|bl
operator|->
name|events
condition|)
block|{
name|wakeup
operator|=
name|bl
operator|->
name|events
expr_stmt|;
name|bl
operator|->
name|events
operator|=
name|bl
operator|->
name|events
operator|->
name|next
expr_stmt|;
block|}
else|else
block|{
name|wakeup
operator|=
name|NULL
expr_stmt|;
name|bl
operator|->
name|busy
operator|--
expr_stmt|;
block|}
comment|/*      * MP: all ctx's and their queue must be in shared memory,      *     each ctx has pid to wake up      */
if|if
condition|(
name|wakeup
operator|==
name|NULL
condition|)
block|{
name|ngx_mutex_unlock
argument_list|(
name|bl
operator|->
name|mutex
argument_list|)
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
if|if
condition|(
name|ctx
operator|->
name|md5
condition|)
block|{
for|for
control|(
name|wakeup
operator|=
name|bl
operator|->
name|events
init|;
name|wakeup
condition|;
name|wakeup
operator|=
name|wakeup
operator|->
name|next
control|)
block|{
if|if
condition|(
name|wakeup
operator|->
name|md5
operator|==
name|NULL
operator|||
name|wakeup
operator|->
name|slot
operator|!=
name|ctx
operator|->
name|slot
condition|)
block|{
continue|continue;
block|}
name|wakeup
operator|->
name|handler
operator|=
name|ngx_event_busy_lock_posted_handler
expr_stmt|;
name|wakeup
operator|->
name|cache_updated
operator|=
literal|1
expr_stmt|;
name|ev
operator|=
name|wakeup
operator|->
name|event
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
name|ngx_post_event
argument_list|(
name|ev
argument_list|)
expr_stmt|;
name|ngx_mutex_unlock
argument_list|(
name|ngx_posted_events_mutex
argument_list|)
expr_stmt|;
block|}
name|ngx_mutex_unlock
argument_list|(
name|bl
operator|->
name|mutex
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|bl
operator|->
name|waiting
operator|--
expr_stmt|;
name|ngx_mutex_unlock
argument_list|(
name|bl
operator|->
name|mutex
argument_list|)
expr_stmt|;
name|wakeup
operator|->
name|handler
operator|=
name|ngx_event_busy_lock_posted_handler
expr_stmt|;
name|wakeup
operator|->
name|locked
operator|=
literal|1
expr_stmt|;
name|ev
operator|=
name|wakeup
operator|->
name|event
expr_stmt|;
if|if
condition|(
name|ev
operator|->
name|timer_set
condition|)
block|{
name|ngx_del_timer
argument_list|(
name|ev
argument_list|)
expr_stmt|;
block|}
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
name|ngx_post_event
argument_list|(
name|ev
argument_list|)
expr_stmt|;
name|ngx_mutex_unlock
argument_list|(
name|ngx_posted_events_mutex
argument_list|)
expr_stmt|;
block|}
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_event_busy_lock_cancel (ngx_event_busy_lock_t * bl,ngx_event_busy_lock_ctx_t * ctx)
name|ngx_int_t
name|ngx_event_busy_lock_cancel
parameter_list|(
name|ngx_event_busy_lock_t
modifier|*
name|bl
parameter_list|,
name|ngx_event_busy_lock_ctx_t
modifier|*
name|ctx
parameter_list|)
block|{
name|ngx_event_busy_lock_ctx_t
modifier|*
name|c
decl_stmt|,
modifier|*
name|p
decl_stmt|;
if|if
condition|(
name|ngx_mutex_lock
argument_list|(
name|bl
operator|->
name|mutex
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|bl
operator|->
name|waiting
operator|--
expr_stmt|;
if|if
condition|(
name|ctx
operator|==
name|bl
operator|->
name|events
condition|)
block|{
name|bl
operator|->
name|events
operator|=
name|ctx
operator|->
name|next
expr_stmt|;
block|}
else|else
block|{
name|p
operator|=
name|bl
operator|->
name|events
expr_stmt|;
for|for
control|(
name|c
operator|=
name|bl
operator|->
name|events
operator|->
name|next
init|;
name|c
condition|;
name|c
operator|=
name|c
operator|->
name|next
control|)
block|{
if|if
condition|(
name|c
operator|==
name|ctx
condition|)
block|{
name|p
operator|->
name|next
operator|=
name|ctx
operator|->
name|next
expr_stmt|;
break|break;
block|}
name|p
operator|=
name|c
expr_stmt|;
block|}
block|}
name|ngx_mutex_unlock
argument_list|(
name|bl
operator|->
name|mutex
argument_list|)
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_event_busy_lock_look_cachable (ngx_event_busy_lock_t * bl,ngx_event_busy_lock_ctx_t * ctx)
specifier|static
name|int
name|ngx_event_busy_lock_look_cachable
parameter_list|(
name|ngx_event_busy_lock_t
modifier|*
name|bl
parameter_list|,
name|ngx_event_busy_lock_ctx_t
modifier|*
name|ctx
parameter_list|)
block|{
name|ngx_int_t
name|free
decl_stmt|;
name|ngx_uint_t
name|i
decl_stmt|,
name|bit
decl_stmt|,
name|cachable
decl_stmt|,
name|mask
decl_stmt|;
name|bit
operator|=
literal|0
expr_stmt|;
name|cachable
operator|=
literal|0
expr_stmt|;
name|free
operator|=
operator|-
literal|1
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_SUPPRESS_WARN
operator|)
name|mask
operator|=
literal|0
expr_stmt|;
endif|#
directive|endif
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|bl
operator|->
name|max_busy
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
operator|(
name|bit
operator|&
literal|7
operator|)
operator|==
literal|0
condition|)
block|{
name|mask
operator|=
name|bl
operator|->
name|md5_mask
index|[
name|i
operator|/
literal|8
index|]
expr_stmt|;
block|}
if|if
condition|(
name|mask
operator|&
literal|1
condition|)
block|{
if|if
condition|(
name|ngx_memcmp
argument_list|(
operator|&
name|bl
operator|->
name|md5
index|[
name|i
operator|*
literal|16
index|]
argument_list|,
name|ctx
operator|->
name|md5
argument_list|,
literal|16
argument_list|)
operator|==
literal|0
condition|)
block|{
name|ctx
operator|->
name|waiting
operator|=
literal|1
expr_stmt|;
name|ctx
operator|->
name|slot
operator|=
name|i
expr_stmt|;
return|return
name|NGX_AGAIN
return|;
block|}
name|cachable
operator|++
expr_stmt|;
block|}
if|else if
condition|(
name|free
operator|==
operator|-
literal|1
condition|)
block|{
name|free
operator|=
name|i
expr_stmt|;
block|}
if|if
condition|(
name|cachable
operator|==
name|bl
operator|->
name|cachable
condition|)
block|{
if|if
condition|(
name|free
operator|==
operator|-
literal|1
operator|&&
name|cachable
operator|<
name|bl
operator|->
name|max_busy
condition|)
block|{
name|free
operator|=
name|i
operator|+
literal|1
expr_stmt|;
block|}
break|break;
block|}
name|mask
operator|>>=
literal|1
expr_stmt|;
name|bit
operator|++
expr_stmt|;
block|}
if|if
condition|(
name|free
operator|==
operator|-
literal|1
condition|)
block|{
return|return
name|NGX_BUSY
return|;
block|}
if|#
directive|if
literal|0
block_content|if (bl->busy == bl->max_busy) {         return NGX_BUSY;     }
endif|#
directive|endif
name|ngx_memcpy
argument_list|(
operator|&
name|bl
operator|->
name|md5
index|[
name|free
operator|*
literal|16
index|]
argument_list|,
name|ctx
operator|->
name|md5
argument_list|,
literal|16
argument_list|)
expr_stmt|;
name|bl
operator|->
name|md5_mask
index|[
name|free
operator|/
literal|8
index|]
operator||=
literal|1
operator|<<
operator|(
name|free
operator|&
literal|7
operator|)
expr_stmt|;
name|ctx
operator|->
name|slot
operator|=
name|free
expr_stmt|;
name|bl
operator|->
name|cachable
operator|++
expr_stmt|;
name|bl
operator|->
name|busy
operator|++
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_event_busy_lock_handler (ngx_event_t * ev)
specifier|static
name|void
name|ngx_event_busy_lock_handler
parameter_list|(
name|ngx_event_t
modifier|*
name|ev
parameter_list|)
block|{
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
return|return;
block|}
name|ngx_post_event
argument_list|(
name|ev
argument_list|)
expr_stmt|;
name|ngx_mutex_unlock
argument_list|(
name|ngx_posted_events_mutex
argument_list|)
expr_stmt|;
name|ev
operator|->
name|event_handler
operator|=
name|ngx_event_busy_lock_posted_handler
expr_stmt|;
block|}
end_function

begin_function
DECL|function|ngx_event_busy_lock_posted_handler (ngx_event_t * ev)
specifier|static
name|void
name|ngx_event_busy_lock_posted_handler
parameter_list|(
name|ngx_event_t
modifier|*
name|ev
parameter_list|)
block|{
name|ngx_event_busy_lock_ctx_t
modifier|*
name|ctx
decl_stmt|;
name|ctx
operator|=
name|ev
operator|->
name|data
expr_stmt|;
name|ctx
operator|->
name|handler
argument_list|(
name|ev
argument_list|)
expr_stmt|;
block|}
end_function

end_unit

