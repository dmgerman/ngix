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
file|<ngx_types.h>
end_include

begin_include
include|#
directive|include
file|<ngx_errno.h>
end_include

begin_include
include|#
directive|include
file|<ngx_log.h>
end_include

begin_include
include|#
directive|include
file|<ngx_time.h>
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

begin_include
include|#
directive|include
file|<ngx_poll_module.h>
end_include

begin_comment
comment|/* should be per-thread */
end_comment

begin_decl_stmt
DECL|variable|event_list
specifier|static
name|struct
name|pollfd
modifier|*
name|event_list
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|nevents
specifier|static
name|unsigned
name|int
name|nevents
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|event_index
specifier|static
name|ngx_event_t
modifier|*
modifier|*
name|event_index
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ready_index
specifier|static
name|ngx_event_t
modifier|*
modifier|*
name|ready_index
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|timer_queue
specifier|static
name|ngx_event_t
modifier|*
name|timer_queue
decl_stmt|;
end_decl_stmt

begin_comment
comment|/* */
end_comment

begin_function
DECL|function|ngx_poll_init (int max_connections,ngx_log_t * log)
name|int
name|ngx_poll_init
parameter_list|(
name|int
name|max_connections
parameter_list|,
name|ngx_log_t
modifier|*
name|log
parameter_list|)
block|{
name|ngx_test_null
argument_list|(
name|event_list
argument_list|,
name|ngx_alloc
argument_list|(
sizeof|sizeof
argument_list|(
expr|struct
name|pollfd
argument_list|)
operator|*
name|max_connections
argument_list|,
name|log
argument_list|)
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
name|ngx_test_null
argument_list|(
name|event_index
argument_list|,
name|ngx_alloc
argument_list|(
sizeof|sizeof
argument_list|(
name|ngx_event_t
operator|*
argument_list|)
operator|*
name|max_connections
argument_list|,
name|log
argument_list|)
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
name|ngx_test_null
argument_list|(
name|ready_index
argument_list|,
name|ngx_alloc
argument_list|(
sizeof|sizeof
argument_list|(
name|ngx_event_t
operator|*
argument_list|)
operator|*
literal|2
operator|*
name|max_connections
argument_list|,
name|log
argument_list|)
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
name|nevents
operator|=
literal|0
expr_stmt|;
name|timer_queue
operator|=
name|ngx_event_init_timer
argument_list|(
name|log
argument_list|)
expr_stmt|;
if|if
condition|(
name|timer_queue
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|ngx_event_actions
operator|.
name|add
operator|=
name|ngx_poll_add_event
expr_stmt|;
name|ngx_event_actions
operator|.
name|del
operator|=
name|ngx_poll_del_event
expr_stmt|;
name|ngx_event_actions
operator|.
name|timer
operator|=
name|ngx_event_add_timer
expr_stmt|;
name|ngx_event_actions
operator|.
name|process
operator|=
name|ngx_poll_process_events
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_poll_add_event (ngx_event_t * ev,int event,u_int flags)
name|int
name|ngx_poll_add_event
parameter_list|(
name|ngx_event_t
modifier|*
name|ev
parameter_list|,
name|int
name|event
parameter_list|,
name|u_int
name|flags
parameter_list|)
block|{
name|ngx_event_t
modifier|*
name|e
decl_stmt|;
name|ngx_connection_t
modifier|*
name|c
decl_stmt|;
name|c
operator|=
operator|(
name|ngx_connection_t
operator|*
operator|)
name|ev
operator|->
name|data
expr_stmt|;
name|ev
operator|->
name|active
operator|=
literal|1
expr_stmt|;
name|ev
operator|->
name|oneshot
operator|=
operator|(
name|flags
operator|&
name|NGX_ONESHOT_EVENT
operator|)
condition|?
literal|1
else|:
literal|0
expr_stmt|;
if|if
condition|(
name|event
operator|==
name|NGX_READ_EVENT
condition|)
block|{
name|e
operator|=
name|c
operator|->
name|write
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_READ_EVENT
operator|!=
name|POLLIN
operator|)
name|event
operator|=
name|POLLIN
expr_stmt|;
endif|#
directive|endif
block|}
else|else
block|{
name|e
operator|=
name|c
operator|->
name|read
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_WRITE_EVENT
operator|!=
name|POLLOUT
operator|)
name|event
operator|=
name|POLLOUT
expr_stmt|;
endif|#
directive|endif
block|}
if|#
directive|if
operator|(
name|NGX_DEBUG_EVENT
operator|)
name|ngx_log_debug
argument_list|(
argument|ev->log
argument_list|,
literal|"add event: %d:%d"
argument|_ c->fd _ event
argument_list|)
empty_stmt|;
endif|#
directive|endif
if|if
condition|(
name|e
operator|==
name|NULL
operator|||
name|e
operator|->
name|index
operator|==
name|NGX_INVALID_INDEX
condition|)
block|{
name|event_list
index|[
name|nevents
index|]
operator|.
name|fd
operator|=
name|c
operator|->
name|fd
expr_stmt|;
name|event_list
index|[
name|nevents
index|]
operator|.
name|events
operator|=
name|event
expr_stmt|;
name|event_list
index|[
name|nevents
index|]
operator|.
name|revents
operator|=
literal|0
expr_stmt|;
name|event_index
index|[
name|nevents
index|]
operator|=
name|ev
expr_stmt|;
name|ev
operator|->
name|index
operator|=
name|nevents
expr_stmt|;
name|nevents
operator|++
expr_stmt|;
block|}
else|else
block|{
name|event_list
index|[
name|e
operator|->
name|index
index|]
operator|.
name|events
operator||=
name|event
expr_stmt|;
name|ev
operator|->
name|index
operator|=
name|e
operator|->
name|index
expr_stmt|;
block|}
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_poll_del_event (ngx_event_t * ev,int event,u_int flags)
name|int
name|ngx_poll_del_event
parameter_list|(
name|ngx_event_t
modifier|*
name|ev
parameter_list|,
name|int
name|event
parameter_list|,
name|u_int
name|flags
parameter_list|)
block|{
name|ngx_event_t
modifier|*
name|e
decl_stmt|;
name|ngx_connection_t
modifier|*
name|c
decl_stmt|;
name|c
operator|=
operator|(
name|ngx_connection_t
operator|*
operator|)
name|ev
operator|->
name|data
expr_stmt|;
if|if
condition|(
name|ev
operator|->
name|index
operator|==
name|NGX_INVALID_INDEX
condition|)
block|{
return|return
name|NGX_OK
return|;
block|}
if|if
condition|(
name|event
operator|==
name|NGX_READ_EVENT
condition|)
block|{
name|e
operator|=
name|c
operator|->
name|write
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_READ_EVENT
operator|!=
name|POLLIN
operator|)
name|event
operator|=
name|POLLIN
expr_stmt|;
endif|#
directive|endif
block|}
else|else
block|{
name|e
operator|=
name|c
operator|->
name|read
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_WRITE_EVENT
operator|!=
name|POLLOUT
operator|)
name|event
operator|=
name|POLLOUT
expr_stmt|;
endif|#
directive|endif
block|}
if|#
directive|if
operator|(
name|NGX_DEBUG_EVENT
operator|)
name|ngx_log_debug
argument_list|(
argument|c->log
argument_list|,
literal|"del event: %d, %d"
argument|_ c->fd _ event
argument_list|)
empty_stmt|;
endif|#
directive|endif
if|if
condition|(
name|e
operator|==
name|NULL
operator|||
name|e
operator|->
name|index
operator|==
name|NGX_INVALID_INDEX
condition|)
block|{
if|if
condition|(
name|ev
operator|->
name|index
operator|<
operator|--
name|nevents
condition|)
block|{
name|event_list
index|[
name|ev
operator|->
name|index
index|]
operator|=
name|event_list
index|[
name|nevents
index|]
expr_stmt|;
name|event_index
index|[
name|ev
operator|->
name|index
index|]
operator|=
name|event_index
index|[
name|nevents
index|]
expr_stmt|;
name|event_index
index|[
name|ev
operator|->
name|index
index|]
operator|->
name|index
operator|=
name|ev
operator|->
name|index
expr_stmt|;
block|}
block|}
else|else
block|{
name|event_list
index|[
name|e
operator|->
name|index
index|]
operator|.
name|events
operator|&=
operator|~
name|event
expr_stmt|;
block|}
name|ev
operator|->
name|active
operator|=
literal|0
expr_stmt|;
name|ev
operator|->
name|index
operator|=
name|NGX_INVALID_INDEX
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_poll_process_events (ngx_log_t * log)
name|int
name|ngx_poll_process_events
parameter_list|(
name|ngx_log_t
modifier|*
name|log
parameter_list|)
block|{
name|int
name|i
decl_stmt|,
name|ready
decl_stmt|,
name|nready
decl_stmt|,
name|found
decl_stmt|;
name|ngx_msec_t
name|timer
decl_stmt|,
name|delta
decl_stmt|;
name|ngx_err_t
name|err
decl_stmt|;
name|ngx_event_t
modifier|*
name|ev
decl_stmt|;
name|ngx_connection_t
modifier|*
name|c
decl_stmt|;
name|timer
operator|=
name|ngx_event_find_timer
argument_list|()
expr_stmt|;
if|if
condition|(
name|timer
condition|)
block|{
name|delta
operator|=
name|ngx_msec
argument_list|()
expr_stmt|;
block|}
else|else
block|{
name|timer
operator|=
name|INFTIM
expr_stmt|;
name|delta
operator|=
literal|0
expr_stmt|;
block|}
if|#
directive|if
operator|(
name|NGX_DEBUG_EVENT
operator|)
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|nevents
condition|;
name|i
operator|++
control|)
block|{
name|ngx_log_debug
argument_list|(
argument|log
argument_list|,
literal|"poll: %d, %d"
argument|_                       event_list[i].fd _ event_list[i].events
argument_list|)
empty_stmt|;
block|}
name|ngx_log_debug
argument_list|(
argument|log
argument_list|,
literal|"poll timer: %d"
argument|_ timer
argument_list|)
empty_stmt|;
endif|#
directive|endif
if|if
condition|(
operator|(
name|ready
operator|=
name|poll
argument_list|(
name|event_list
argument_list|,
name|nevents
argument_list|,
name|timer
argument_list|)
operator|)
operator|==
operator|-
literal|1
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"poll() failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
name|ngx_log_debug
argument_list|(
argument|log
argument_list|,
literal|"poll ready %d"
argument|_ ready
argument_list|)
empty_stmt|;
if|if
condition|(
name|timer
operator|!=
name|INFTIM
condition|)
block|{
name|delta
operator|=
name|ngx_msec
argument_list|()
operator|-
name|delta
expr_stmt|;
block|}
else|else
block|{
if|if
condition|(
name|ready
operator|==
literal|0
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"poll() returns no events without timeout"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
block|}
if|#
directive|if
operator|(
name|NGX_DEBUG_EVENT
operator|)
name|ngx_log_debug
argument_list|(
argument|log
argument_list|,
literal|"poll timer: %d, delta: %d"
argument|_ timer _ delta
argument_list|)
empty_stmt|;
endif|#
directive|endif
name|nready
operator|=
literal|0
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|nevents
operator|&&
name|ready
condition|;
name|i
operator|++
control|)
block|{
name|c
operator|=
operator|&
name|ngx_connections
index|[
name|event_list
index|[
name|i
index|]
operator|.
name|fd
index|]
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_DEBUG_EVENT
operator|)
name|ngx_log_debug
argument_list|(
argument|log
argument_list|,
literal|"poll: fd:%d, ev:%d, rev:%d"
argument|_                       event_list[i].fd _                       event_list[i].events _ event_list[i].revents
argument_list|)
empty_stmt|;
endif|#
directive|endif
name|found
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|event_list
index|[
name|i
index|]
operator|.
name|revents
operator|&
name|POLLIN
condition|)
block|{
name|found
operator|=
literal|1
expr_stmt|;
name|ready_index
index|[
name|nready
operator|++
index|]
operator|=
name|c
operator|->
name|read
expr_stmt|;
block|}
if|if
condition|(
name|event_list
index|[
name|i
index|]
operator|.
name|revents
operator|&
name|POLLOUT
condition|)
block|{
name|found
operator|=
literal|1
expr_stmt|;
name|ready_index
index|[
name|nready
operator|++
index|]
operator|=
name|c
operator|->
name|write
expr_stmt|;
block|}
if|if
condition|(
name|event_list
index|[
name|i
index|]
operator|.
name|revents
operator|&
operator|(
name|POLLERR
operator||
name|POLLHUP
operator||
name|POLLNVAL
operator|)
condition|)
block|{
name|found
operator|=
literal|1
expr_stmt|;
name|err
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|event_list
index|[
name|i
index|]
operator|.
name|revents
operator|&
name|POLLNVAL
condition|)
block|{
name|err
operator|=
name|EBADF
expr_stmt|;
block|}
name|ngx_log_error
argument_list|(
name|NGX_LOG_ERR
argument_list|,
name|log
argument_list|,
name|err
argument_list|,
literal|"poll() error on %d:%d"
argument_list|,
name|event_list
index|[
name|i
index|]
operator|.
name|fd
argument_list|,
name|event_list
index|[
name|i
index|]
operator|.
name|revents
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|found
condition|)
block|{
name|ready
operator|--
expr_stmt|;
block|}
block|}
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|nready
condition|;
name|i
operator|++
control|)
block|{
name|ev
operator|=
name|ready_index
index|[
name|i
index|]
expr_stmt|;
if|if
condition|(
operator|!
name|ev
operator|->
name|active
condition|)
block|{
continue|continue;
block|}
name|ev
operator|->
name|ready
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|ev
operator|->
name|oneshot
condition|)
block|{
name|ngx_del_timer
argument_list|(
name|ev
argument_list|)
expr_stmt|;
if|if
condition|(
name|ev
operator|->
name|write
condition|)
block|{
name|ngx_poll_del_event
argument_list|(
name|ev
argument_list|,
name|NGX_WRITE_EVENT
argument_list|,
literal|0
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|ngx_poll_del_event
argument_list|(
name|ev
argument_list|,
name|NGX_READ_EVENT
argument_list|,
literal|0
argument_list|)
expr_stmt|;
block|}
block|}
if|if
condition|(
name|ev
operator|->
name|event_handler
argument_list|(
name|ev
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
name|ev
operator|->
name|close_handler
argument_list|(
name|ev
argument_list|)
expr_stmt|;
block|}
block|}
if|if
condition|(
name|ready
operator|!=
literal|0
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"poll ready != events"
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|timer
operator|!=
name|INFTIM
condition|)
block|{
name|ngx_event_expire_timers
argument_list|(
name|delta
argument_list|)
expr_stmt|;
block|}
return|return
name|NGX_OK
return|;
block|}
end_function

end_unit

