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
file|<ngx_log.h>
end_include

begin_include
include|#
directive|include
file|<ngx_alloc.h>
end_include

begin_include
include|#
directive|include
file|<ngx_connection.h>
end_include

begin_include
include|#
directive|include
file|<ngx_event.h>
end_include

begin_include
include|#
directive|include
file|<ngx_event_timer.h>
end_include

begin_comment
comment|/* STUB */
end_comment

begin_define
DECL|macro|NGX_TIMER_QUEUE_NUM
define|#
directive|define
name|NGX_TIMER_QUEUE_NUM
value|5
end_define

begin_comment
comment|/* should be per-thread */
end_comment

begin_decl_stmt
DECL|variable|ngx_timer_queue
specifier|static
name|ngx_event_t
modifier|*
name|ngx_timer_queue
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_timer_cur_queue
specifier|static
name|int
name|ngx_timer_cur_queue
decl_stmt|;
end_decl_stmt

begin_comment
comment|/* */
end_comment

begin_decl_stmt
DECL|variable|ngx_timer_queue_num
specifier|static
name|int
name|ngx_timer_queue_num
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|ngx_event_init_timer (ngx_log_t * log)
name|ngx_event_t
modifier|*
name|ngx_event_init_timer
parameter_list|(
name|ngx_log_t
modifier|*
name|log
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
name|ngx_timer_queue_num
operator|=
name|NGX_TIMER_QUEUE_NUM
expr_stmt|;
name|ngx_timer_cur_queue
operator|=
literal|0
expr_stmt|;
name|ngx_test_null
argument_list|(
name|ngx_timer_queue
argument_list|,
name|ngx_alloc
argument_list|(
name|ngx_timer_queue_num
operator|*
sizeof|sizeof
argument_list|(
name|ngx_event_t
argument_list|)
argument_list|,
name|log
argument_list|)
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|ngx_timer_queue_num
condition|;
name|i
operator|++
control|)
block|{
name|ngx_timer_queue
index|[
name|i
index|]
operator|.
name|timer_prev
operator|=
operator|&
name|ngx_timer_queue
index|[
name|i
index|]
expr_stmt|;
name|ngx_timer_queue
index|[
name|i
index|]
operator|.
name|timer_next
operator|=
operator|&
name|ngx_timer_queue
index|[
name|i
index|]
expr_stmt|;
block|}
return|return
name|ngx_timer_queue
return|;
block|}
end_function

begin_function
DECL|function|ngx_event_add_timer (ngx_event_t * ev,ngx_msec_t timer)
name|void
name|ngx_event_add_timer
parameter_list|(
name|ngx_event_t
modifier|*
name|ev
parameter_list|,
name|ngx_msec_t
name|timer
parameter_list|)
block|{
name|ngx_event_t
modifier|*
name|e
decl_stmt|;
if|#
directive|if
operator|(
name|NGX_DEBUG_EVENT
operator|)
name|ngx_connection_t
modifier|*
name|c
init|=
operator|(
name|ngx_connection_t
operator|*
operator|)
name|ev
operator|->
name|data
decl_stmt|;
name|ngx_log_debug
argument_list|(
argument|ev->log
argument_list|,
literal|"set timer: %d:%d, slot: %d"
argument|_                   c->fd _ timer _ ngx_timer_cur_queue
argument_list|)
empty_stmt|;
endif|#
directive|endif
if|if
condition|(
name|ev
operator|->
name|timer_next
operator|||
name|ev
operator|->
name|timer_prev
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|ev
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"timer already set"
argument_list|)
expr_stmt|;
return|return;
block|}
for|for
control|(
name|e
operator|=
name|ngx_timer_queue
index|[
name|ngx_timer_cur_queue
index|]
operator|.
name|timer_next
init|;
name|e
operator|!=
operator|&
name|ngx_timer_queue
index|[
name|ngx_timer_cur_queue
index|]
operator|&&
name|timer
operator|>
name|e
operator|->
name|timer_delta
condition|;
name|e
operator|=
name|e
operator|->
name|timer_next
control|)
block|{
name|timer
operator|-=
name|e
operator|->
name|timer_delta
expr_stmt|;
block|}
name|ngx_timer_cur_queue
operator|++
expr_stmt|;
if|if
condition|(
name|ngx_timer_cur_queue
operator|>=
name|ngx_timer_queue_num
condition|)
block|{
name|ngx_timer_cur_queue
operator|=
literal|0
expr_stmt|;
block|}
name|ev
operator|->
name|timer_delta
operator|=
name|timer
expr_stmt|;
name|ev
operator|->
name|timer_next
operator|=
name|e
expr_stmt|;
name|ev
operator|->
name|timer_prev
operator|=
name|e
operator|->
name|timer_prev
expr_stmt|;
name|e
operator|->
name|timer_prev
operator|->
name|timer_next
operator|=
name|ev
expr_stmt|;
name|e
operator|->
name|timer_prev
operator|=
name|ev
expr_stmt|;
block|}
end_function

begin_function
DECL|function|ngx_event_find_timer (void)
name|int
name|ngx_event_find_timer
parameter_list|(
name|void
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
name|ngx_msec_t
name|timer
decl_stmt|;
name|timer
operator|=
name|NGX_MAX_MSEC
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|ngx_timer_queue_num
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|ngx_timer_queue
index|[
name|i
index|]
operator|.
name|timer_next
operator|==
operator|&
name|ngx_timer_queue
index|[
name|i
index|]
condition|)
block|{
continue|continue;
block|}
if|if
condition|(
name|timer
operator|>
name|ngx_timer_queue
index|[
name|i
index|]
operator|.
name|timer_next
operator|->
name|timer_delta
condition|)
block|{
name|timer
operator|=
name|ngx_timer_queue
index|[
name|i
index|]
operator|.
name|timer_next
operator|->
name|timer_delta
expr_stmt|;
block|}
block|}
if|if
condition|(
name|timer
operator|==
name|NGX_MAX_MSEC
condition|)
block|{
return|return
literal|0
return|;
block|}
else|else
block|{
return|return
name|timer
return|;
block|}
block|}
end_function

begin_function
DECL|function|ngx_event_expire_timers (ngx_msec_t timer)
name|void
name|ngx_event_expire_timers
parameter_list|(
name|ngx_msec_t
name|timer
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
name|ngx_msec_t
name|delta
decl_stmt|;
name|ngx_event_t
modifier|*
name|ev
decl_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|ngx_timer_queue_num
condition|;
name|i
operator|++
control|)
block|{
name|delta
operator|=
name|timer
expr_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
name|ev
operator|=
name|ngx_timer_queue
index|[
name|i
index|]
operator|.
name|timer_next
expr_stmt|;
if|if
condition|(
name|ev
operator|==
operator|&
name|ngx_timer_queue
index|[
name|i
index|]
condition|)
block|{
break|break;
block|}
if|if
condition|(
name|ev
operator|->
name|timer_delta
operator|>
name|delta
condition|)
block|{
name|ev
operator|->
name|timer_delta
operator|-=
name|delta
expr_stmt|;
break|break;
block|}
name|delta
operator|-=
name|ev
operator|->
name|timer_delta
expr_stmt|;
name|ngx_del_timer
argument_list|(
name|ev
argument_list|)
expr_stmt|;
if|if
condition|(
name|ev
operator|->
name|delayed
condition|)
block|{
name|ev
operator|->
name|delayed
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|ev
operator|->
name|ready
operator|==
literal|0
condition|)
block|{
continue|continue;
block|}
block|}
else|else
block|{
name|ev
operator|->
name|timedout
operator|=
literal|1
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
block|}
end_function

end_unit

