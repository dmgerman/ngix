begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (C) 2002-2004 Igor Sysoev, http://sysoev.ru/en/  */
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
name|ngx_poll_init
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|void
name|ngx_poll_done
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
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
function_decl|;
end_function_decl

begin_function_decl
specifier|static
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
function_decl|;
end_function_decl

begin_function_decl
name|int
name|ngx_poll_process_events
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
function_decl|;
end_function_decl

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
name|int
name|nevents
decl_stmt|;
end_decl_stmt

begin_if
if|#
directive|if
literal|0
end_if

begin_endif
unit|static ngx_event_t   **ready_index;
endif|#
directive|endif
end_endif

begin_decl_stmt
DECL|variable|accept_events
specifier|static
name|ngx_event_t
modifier|*
name|accept_events
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|poll_name
specifier|static
name|ngx_str_t
name|poll_name
init|=
name|ngx_string
argument_list|(
literal|"poll"
argument_list|)
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_poll_module_ctx
name|ngx_event_module_t
name|ngx_poll_module_ctx
init|=
block|{
operator|&
name|poll_name
block|,
name|NULL
block|,
comment|/* create configuration */
name|NULL
block|,
comment|/* init configuration */
block|{
name|ngx_poll_add_event
block|,
comment|/* add an event */
name|ngx_poll_del_event
block|,
comment|/* delete an event */
name|ngx_poll_add_event
block|,
comment|/* enable an event */
name|ngx_poll_del_event
block|,
comment|/* disable an event */
name|NULL
block|,
comment|/* add an connection */
name|NULL
block|,
comment|/* delete an connection */
name|ngx_poll_process_events
block|,
comment|/* process the events */
name|ngx_poll_init
block|,
comment|/* init the events */
name|ngx_poll_done
comment|/* done the events */
block|}
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_poll_module
name|ngx_module_t
name|ngx_poll_module
init|=
block|{
name|NGX_MODULE
block|,
operator|&
name|ngx_poll_module_ctx
block|,
comment|/* module context */
name|NULL
block|,
comment|/* module directives */
name|NGX_EVENT_MODULE
block|,
comment|/* module type */
name|NULL
block|,
comment|/* init module */
name|NULL
comment|/* init process */
block|}
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|ngx_poll_init (ngx_cycle_t * cycle)
specifier|static
name|int
name|ngx_poll_init
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
block|{
name|struct
name|pollfd
modifier|*
name|list
decl_stmt|;
if|if
condition|(
name|event_list
operator|==
name|NULL
condition|)
block|{
name|nevents
operator|=
literal|0
expr_stmt|;
block|}
if|if
condition|(
name|ngx_process
operator|==
name|NGX_PROCESS_WORKER
operator|||
name|cycle
operator|->
name|old_cycle
operator|==
name|NULL
operator|||
name|cycle
operator|->
name|old_cycle
operator|->
name|connection_n
operator|<
name|cycle
operator|->
name|connection_n
condition|)
block|{
name|ngx_test_null
argument_list|(
name|list
argument_list|,
name|ngx_alloc
argument_list|(
sizeof|sizeof
argument_list|(
expr|struct
name|pollfd
argument_list|)
operator|*
name|cycle
operator|->
name|connection_n
argument_list|,
name|cycle
operator|->
name|log
argument_list|)
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
if|if
condition|(
name|event_list
condition|)
block|{
name|ngx_memcpy
argument_list|(
name|list
argument_list|,
name|event_list
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_event_t
operator|*
argument_list|)
operator|*
name|nevents
argument_list|)
expr_stmt|;
name|ngx_free
argument_list|(
name|event_list
argument_list|)
expr_stmt|;
block|}
name|event_list
operator|=
name|list
expr_stmt|;
if|#
directive|if
literal|0
block_content|if (ready_index) {             ngx_free(ready_index);         }          ngx_test_null(ready_index,                       ngx_alloc(sizeof(ngx_event_t *) * 2 * cycle->connection_n,                                 cycle->log),                       NGX_ERROR);
endif|#
directive|endif
block|}
name|ngx_io
operator|=
name|ngx_os_io
expr_stmt|;
name|ngx_event_actions
operator|=
name|ngx_poll_module_ctx
operator|.
name|actions
expr_stmt|;
name|ngx_event_flags
operator|=
name|NGX_USE_LEVEL_EVENT
operator||
name|NGX_USE_ONESHOT_EVENT
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_poll_done (ngx_cycle_t * cycle)
specifier|static
name|void
name|ngx_poll_done
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
block|{
name|ngx_free
argument_list|(
name|event_list
argument_list|)
expr_stmt|;
if|#
directive|if
literal|0
block_content|ngx_free(ready_index);
endif|#
directive|endif
name|event_list
operator|=
name|NULL
expr_stmt|;
if|#
directive|if
literal|0
block_content|ready_index = NULL;
endif|#
directive|endif
block|}
end_function

begin_function
DECL|function|ngx_poll_add_event (ngx_event_t * ev,int event,u_int flags)
specifier|static
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
if|if
condition|(
name|ev
operator|->
name|index
operator|!=
name|NGX_INVALID_INDEX
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
literal|"poll event fd:%d ev:%d is already set"
argument_list|,
name|c
operator|->
name|fd
argument_list|,
name|event
argument_list|)
expr_stmt|;
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
name|ngx_log_debug2
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|ev
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"poll add event: fd:%d ev:%d"
argument_list|,
name|c
operator|->
name|fd
argument_list|,
name|event
argument_list|)
expr_stmt|;
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
literal|"poll add index: %d"
argument_list|,
name|e
operator|->
name|index
argument_list|)
expr_stmt|;
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
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_poll_del_event (ngx_event_t * ev,int event,u_int flags)
specifier|static
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
name|ngx_uint_t
name|i
decl_stmt|;
name|ngx_cycle_t
modifier|*
modifier|*
name|cycle
decl_stmt|;
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
name|ev
operator|->
name|data
expr_stmt|;
name|ev
operator|->
name|active
operator|=
literal|0
expr_stmt|;
name|ev
operator|->
name|posted
operator|=
literal|0
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
literal|"poll event fd:%d ev:%d is already deleted"
argument_list|,
name|c
operator|->
name|fd
argument_list|,
name|event
argument_list|)
expr_stmt|;
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
name|ngx_log_debug2
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|ev
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"poll del event: fd:%d ev:%d"
argument_list|,
name|c
operator|->
name|fd
argument_list|,
name|event
argument_list|)
expr_stmt|;
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
name|nevents
operator|--
expr_stmt|;
if|if
condition|(
name|ev
operator|->
name|index
operator|<
operator|(
name|u_int
operator|)
name|nevents
condition|)
block|{
name|ngx_log_debug2
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|ev
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"index: copy event %d to %d"
argument_list|,
name|nevents
argument_list|,
name|ev
operator|->
name|index
argument_list|)
expr_stmt|;
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
name|c
operator|=
operator|&
name|ngx_cycle
operator|->
name|connections
index|[
name|event_list
index|[
name|nevents
index|]
operator|.
name|fd
index|]
expr_stmt|;
if|if
condition|(
name|c
operator|->
name|fd
operator|==
operator|-
literal|1
condition|)
block|{
name|cycle
operator|=
name|ngx_old_cycles
operator|.
name|elts
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|ngx_old_cycles
operator|.
name|nelts
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|cycle
index|[
name|i
index|]
operator|==
name|NULL
condition|)
block|{
continue|continue;
block|}
name|c
operator|=
operator|&
name|cycle
index|[
name|i
index|]
operator|->
name|connections
index|[
name|event_list
index|[
name|nevents
index|]
operator|.
name|fd
index|]
expr_stmt|;
if|if
condition|(
name|c
operator|->
name|fd
operator|!=
operator|-
literal|1
condition|)
block|{
break|break;
block|}
block|}
block|}
if|if
condition|(
name|c
operator|->
name|fd
operator|==
operator|-
literal|1
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
literal|"unexpected last event"
argument_list|)
expr_stmt|;
block|}
else|else
block|{
if|if
condition|(
name|c
operator|->
name|read
operator|->
name|index
operator|==
operator|(
name|u_int
operator|)
name|nevents
condition|)
block|{
name|c
operator|->
name|read
operator|->
name|index
operator|=
name|ev
operator|->
name|index
expr_stmt|;
block|}
if|if
condition|(
name|c
operator|->
name|write
operator|->
name|index
operator|==
operator|(
name|u_int
operator|)
name|nevents
condition|)
block|{
name|c
operator|->
name|write
operator|->
name|index
operator|=
name|ev
operator|->
name|index
expr_stmt|;
block|}
block|}
block|}
block|}
else|else
block|{
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
literal|"poll del index: %d"
argument_list|,
name|e
operator|->
name|index
argument_list|)
expr_stmt|;
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
DECL|function|ngx_poll_process_events (ngx_cycle_t * cycle)
name|int
name|ngx_poll_process_events
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
block|{
name|int
name|ready
decl_stmt|;
name|ngx_int_t
name|i
decl_stmt|,
name|nready
decl_stmt|;
name|ngx_uint_t
name|n
decl_stmt|,
name|found
decl_stmt|,
name|lock
decl_stmt|,
name|expire
decl_stmt|;
name|ngx_msec_t
name|timer
decl_stmt|;
name|ngx_err_t
name|err
decl_stmt|;
name|ngx_cycle_t
modifier|*
modifier|*
name|old_cycle
decl_stmt|;
name|ngx_event_t
modifier|*
name|ev
decl_stmt|;
name|ngx_epoch_msec_t
name|delta
decl_stmt|;
name|ngx_connection_t
modifier|*
name|c
decl_stmt|;
name|struct
name|timeval
name|tv
decl_stmt|;
if|if
condition|(
name|ngx_event_flags
operator|&
name|NGX_OVERFLOW_EVENT
condition|)
block|{
name|timer
operator|=
literal|0
expr_stmt|;
name|expire
operator|=
literal|0
expr_stmt|;
block|}
else|else
block|{
for|for
control|(
init|;
condition|;
control|)
block|{
name|timer
operator|=
name|ngx_event_find_timer
argument_list|()
expr_stmt|;
if|if
condition|(
name|timer
operator|!=
literal|0
condition|)
block|{
break|break;
block|}
name|ngx_log_debug0
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|cycle
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"poll expired timer"
argument_list|)
expr_stmt|;
name|ngx_event_expire_timers
argument_list|(
operator|(
name|ngx_msec_t
operator|)
operator|(
name|ngx_elapsed_msec
operator|-
name|ngx_old_elapsed_msec
operator|)
argument_list|)
expr_stmt|;
block|}
comment|/* NGX_TIMER_INFINITE == INFTIM */
if|if
condition|(
name|timer
operator|==
name|NGX_TIMER_INFINITE
condition|)
block|{
name|expire
operator|=
literal|0
expr_stmt|;
block|}
else|else
block|{
name|expire
operator|=
literal|1
expr_stmt|;
block|}
block|}
name|ngx_old_elapsed_msec
operator|=
name|ngx_elapsed_msec
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_DEBUG0
operator|)
if|if
condition|(
name|cycle
operator|->
name|log
operator|->
name|log_level
operator|&
name|NGX_LOG_DEBUG_ALL
condition|)
block|{
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
literal|"poll: %d: fd:%d ev:%04X"
argument_list|,
name|i
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
name|events
argument_list|)
expr_stmt|;
block|}
block|}
endif|#
directive|endif
if|if
condition|(
name|ngx_accept_mutex
condition|)
block|{
if|if
condition|(
name|ngx_accept_disabled
operator|>
literal|0
condition|)
block|{
name|ngx_accept_disabled
operator|--
expr_stmt|;
block|}
else|else
block|{
if|if
condition|(
name|ngx_trylock_accept_mutex
argument_list|(
name|cycle
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
name|ngx_accept_mutex_held
operator|==
literal|0
operator|&&
operator|(
name|timer
operator|==
name|NGX_TIMER_INFINITE
operator|||
name|timer
operator|>
name|ngx_accept_mutex_delay
operator|)
condition|)
block|{
name|timer
operator|=
name|ngx_accept_mutex_delay
expr_stmt|;
name|expire
operator|=
literal|0
expr_stmt|;
block|}
block|}
block|}
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
literal|"poll timer: %d"
argument_list|,
name|timer
argument_list|)
expr_stmt|;
name|ready
operator|=
name|poll
argument_list|(
name|event_list
argument_list|,
operator|(
name|u_int
operator|)
name|nevents
argument_list|,
operator|(
name|int
operator|)
name|timer
argument_list|)
expr_stmt|;
if|if
condition|(
name|ready
operator|==
operator|-
literal|1
condition|)
block|{
name|err
operator|=
name|ngx_errno
expr_stmt|;
block|}
else|else
block|{
name|err
operator|=
literal|0
expr_stmt|;
block|}
name|ngx_gettimeofday
argument_list|(
operator|&
name|tv
argument_list|)
expr_stmt|;
name|ngx_time_update
argument_list|(
name|tv
operator|.
name|tv_sec
argument_list|)
expr_stmt|;
name|delta
operator|=
name|ngx_elapsed_msec
expr_stmt|;
name|ngx_elapsed_msec
operator|=
name|tv
operator|.
name|tv_sec
operator|*
literal|1000
operator|+
name|tv
operator|.
name|tv_usec
operator|/
literal|1000
operator|-
name|ngx_start_msec
expr_stmt|;
name|ngx_log_debug2
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|cycle
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"poll ready %d of %d"
argument_list|,
name|ready
argument_list|,
name|nevents
argument_list|)
expr_stmt|;
if|if
condition|(
name|err
condition|)
block|{
name|ngx_log_error
argument_list|(
operator|(
name|err
operator|==
name|NGX_EINTR
operator|)
condition|?
name|NGX_LOG_INFO
else|:
name|NGX_LOG_ALERT
argument_list|,
name|cycle
operator|->
name|log
argument_list|,
name|err
argument_list|,
literal|"poll() failed"
argument_list|)
expr_stmt|;
name|ngx_accept_mutex_unlock
argument_list|()
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|timer
operator|!=
name|NGX_TIMER_INFINITE
condition|)
block|{
name|delta
operator|=
name|ngx_elapsed_msec
operator|-
name|delta
expr_stmt|;
name|ngx_log_debug2
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|cycle
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"poll timer: %d, delta: %d"
argument_list|,
name|timer
argument_list|,
operator|(
name|int
operator|)
name|delta
argument_list|)
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
name|cycle
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"poll() returned no events without timeout"
argument_list|)
expr_stmt|;
name|ngx_accept_mutex_unlock
argument_list|()
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
block|}
if|if
condition|(
operator|(
name|ngx_event_flags
operator|&
name|NGX_OVERFLOW_EVENT
operator|)
operator|&&
name|timer
operator|==
literal|0
operator|&&
name|ready
operator|==
literal|0
condition|)
block|{
comment|/* the overflowed rt signals queue has been drained */
name|ngx_accept_mutex_unlock
argument_list|()
expr_stmt|;
return|return
name|NGX_OK
return|;
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
name|ngx_accept_mutex_unlock
argument_list|()
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
name|lock
operator|=
literal|1
expr_stmt|;
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
if|#
directive|if
literal|0
block_content|ngx_log_debug4(NGX_LOG_DEBUG_EVENT, cycle->log, 0,                        "poll: %d: fd:%d ev:%04X rev:%04X",                        i, event_list[i].fd,                        event_list[i].events, event_list[i].revents);
else|#
directive|else
if|if
condition|(
name|event_list
index|[
name|i
index|]
operator|.
name|revents
condition|)
block|{
name|ngx_log_debug4
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|cycle
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"poll: %d: fd:%d ev:%04X rev:%04X"
argument_list|,
name|i
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
name|events
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
endif|#
directive|endif
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
name|POLLNVAL
operator|)
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|cycle
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"poll() error fd:%d ev:%04X rev:%04X"
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
name|events
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
name|event_list
index|[
name|i
index|]
operator|.
name|revents
operator|&
operator|~
operator|(
name|POLLIN
operator||
name|POLLOUT
operator||
name|POLLERR
operator||
name|POLLHUP
operator||
name|POLLNVAL
operator|)
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|cycle
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"strange poll() events fd:%d ev:%04X rev:%04X"
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
name|events
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
name|event_list
index|[
name|i
index|]
operator|.
name|fd
operator|==
operator|-
literal|1
condition|)
block|{
comment|/*              * the disabled event, a workaround for our possible bug,              * see the comment below              */
continue|continue;
block|}
name|c
operator|=
operator|&
name|ngx_cycle
operator|->
name|connections
index|[
name|event_list
index|[
name|i
index|]
operator|.
name|fd
index|]
expr_stmt|;
if|if
condition|(
name|c
operator|->
name|fd
operator|==
operator|-
literal|1
condition|)
block|{
name|old_cycle
operator|=
name|ngx_old_cycles
operator|.
name|elts
expr_stmt|;
for|for
control|(
name|n
operator|=
literal|0
init|;
name|n
operator|<
name|ngx_old_cycles
operator|.
name|nelts
condition|;
name|n
operator|++
control|)
block|{
if|if
condition|(
name|old_cycle
index|[
name|n
index|]
operator|==
name|NULL
condition|)
block|{
continue|continue;
block|}
name|c
operator|=
operator|&
name|old_cycle
index|[
name|n
index|]
operator|->
name|connections
index|[
name|event_list
index|[
name|i
index|]
operator|.
name|fd
index|]
expr_stmt|;
if|if
condition|(
name|c
operator|->
name|fd
operator|!=
operator|-
literal|1
condition|)
block|{
break|break;
block|}
block|}
block|}
if|if
condition|(
name|c
operator|->
name|fd
operator|==
operator|-
literal|1
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|cycle
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"unexpected event"
argument_list|)
expr_stmt|;
comment|/*              * it is certainly our fault and it should be investigated,              * in the meantime we disable this event to avoid a CPU spinning              */
if|if
condition|(
name|i
operator|==
name|nevents
operator|-
literal|1
condition|)
block|{
name|nevents
operator|--
expr_stmt|;
block|}
else|else
block|{
name|event_list
index|[
name|i
index|]
operator|.
name|fd
operator|=
operator|-
literal|1
expr_stmt|;
block|}
continue|continue;
block|}
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
operator|(
name|POLLIN
operator||
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
name|ev
operator|=
name|c
operator|->
name|read
expr_stmt|;
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
if|if
condition|(
name|ev
operator|->
name|accept
condition|)
block|{
name|ev
operator|->
name|next
operator|=
name|accept_events
expr_stmt|;
name|accept_events
operator|=
name|ev
expr_stmt|;
block|}
else|else
block|{
name|ngx_post_event
argument_list|(
name|ev
argument_list|)
expr_stmt|;
block|}
if|#
directive|if
literal|0
block_content|ready_index[nready++] = c->read;
endif|#
directive|endif
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
name|POLLOUT
operator||
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
name|ev
operator|=
name|c
operator|->
name|write
expr_stmt|;
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
name|ngx_post_event
argument_list|(
name|ev
argument_list|)
expr_stmt|;
if|#
directive|if
literal|0
block_content|ready_index[nready++] = c->write;
endif|#
directive|endif
block|}
if|if
condition|(
name|found
condition|)
block|{
name|ready
operator|--
expr_stmt|;
continue|continue;
block|}
block|}
if|#
directive|if
literal|0
block_content|for (i = 0; i< nready; i++) {         ev = ready_index[i];          if (!ev->active) {             continue;         }          ev->ready = 1;          if (ev->oneshot) {             if (ev->timer_set) {                 ngx_del_timer(ev);             }              if (ev->write) {                 ngx_poll_del_event(ev, NGX_WRITE_EVENT, 0);             } else {                 ngx_poll_del_event(ev, NGX_READ_EVENT, 0);             }         }          ev->event_handler(ev);     }
endif|#
directive|endif
name|ev
operator|=
name|accept_events
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
literal|"accept event "
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
break|break;
block|}
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
name|ngx_accept_disabled
operator|>
literal|0
condition|)
block|{
name|lock
operator|=
literal|0
expr_stmt|;
break|break;
block|}
name|ev
operator|=
name|ev
operator|->
name|next
expr_stmt|;
if|if
condition|(
name|ev
operator|==
name|NULL
condition|)
block|{
name|lock
operator|=
literal|0
expr_stmt|;
break|break;
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
name|ngx_accept_mutex_unlock
argument_list|()
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
block|}
name|ngx_accept_mutex_unlock
argument_list|()
expr_stmt|;
name|accept_events
operator|=
name|NULL
expr_stmt|;
if|if
condition|(
name|lock
condition|)
block|{
name|ngx_mutex_unlock
argument_list|(
name|ngx_posted_events_mutex
argument_list|)
expr_stmt|;
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
name|cycle
operator|->
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
name|expire
operator|&&
name|delta
condition|)
block|{
name|ngx_event_expire_timers
argument_list|(
operator|(
name|ngx_msec_t
operator|)
name|delta
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|ngx_threaded
condition|)
block|{
name|ngx_event_process_posted
argument_list|(
name|cycle
argument_list|)
expr_stmt|;
block|}
return|return
name|nready
return|;
block|}
end_function

end_unit

