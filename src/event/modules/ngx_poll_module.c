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
name|ngx_int_t
name|ngx_poll_init
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|,
name|ngx_msec_t
name|timer
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
name|ngx_int_t
name|ngx_poll_add_event
parameter_list|(
name|ngx_event_t
modifier|*
name|ev
parameter_list|,
name|ngx_int_t
name|event
parameter_list|,
name|ngx_uint_t
name|flags
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_poll_del_event
parameter_list|(
name|ngx_event_t
modifier|*
name|ev
parameter_list|,
name|ngx_int_t
name|event
parameter_list|,
name|ngx_uint_t
name|flags
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_poll_process_events
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|,
name|ngx_msec_t
name|timer
parameter_list|,
name|ngx_uint_t
name|flags
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|char
modifier|*
name|ngx_poll_init_conf
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|,
name|void
modifier|*
name|conf
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
name|ngx_int_t
name|nevents
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
name|ngx_poll_init_conf
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
name|NULL
block|,
comment|/* process the changes */
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
name|NGX_MODULE_V1
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
comment|/* init master */
name|NULL
block|,
comment|/* init module */
name|NULL
block|,
comment|/* init process */
name|NULL
block|,
comment|/* init thread */
name|NULL
block|,
comment|/* exit thread */
name|NULL
block|,
comment|/* exit process */
name|NULL
block|,
comment|/* exit master */
name|NGX_MODULE_V1_PADDING
block|}
decl_stmt|;
end_decl_stmt

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_poll_init (ngx_cycle_t * cycle,ngx_msec_t timer)
name|ngx_poll_init
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|,
name|ngx_msec_t
name|timer
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
operator|>=
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
name|list
operator|=
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
expr_stmt|;
if|if
condition|(
name|list
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
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
name|NGX_USE_FD_EVENT
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
specifier|static
name|void
DECL|function|ngx_poll_done (ngx_cycle_t * cycle)
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
name|event_list
operator|=
name|NULL
expr_stmt|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_poll_add_event (ngx_event_t * ev,ngx_int_t event,ngx_uint_t flags)
name|ngx_poll_add_event
parameter_list|(
name|ngx_event_t
modifier|*
name|ev
parameter_list|,
name|ngx_int_t
name|event
parameter_list|,
name|ngx_uint_t
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
literal|"poll event fd:%d ev:%i is already set"
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
literal|"poll add event: fd:%d ev:%i"
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
operator|(
name|short
operator|)
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
literal|"poll add index: %i"
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
operator|(
name|short
operator|)
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
specifier|static
name|ngx_int_t
DECL|function|ngx_poll_del_event (ngx_event_t * ev,ngx_int_t event,ngx_uint_t flags)
name|ngx_poll_del_event
parameter_list|(
name|ngx_event_t
modifier|*
name|ev
parameter_list|,
name|ngx_int_t
name|event
parameter_list|,
name|ngx_uint_t
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
literal|"poll event fd:%d ev:%i is already deleted"
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
literal|"poll del event: fd:%d ev:%i"
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
name|ngx_uint_t
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
literal|"index: copy event %ui to %i"
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
name|ngx_cycle
operator|->
name|files
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
name|ngx_uint_t
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
name|ngx_uint_t
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
literal|"poll del index: %i"
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
operator|(
name|short
operator|)
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
specifier|static
name|ngx_int_t
DECL|function|ngx_poll_process_events (ngx_cycle_t * cycle,ngx_msec_t timer,ngx_uint_t flags)
name|ngx_poll_process_events
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|,
name|ngx_msec_t
name|timer
parameter_list|,
name|ngx_uint_t
name|flags
parameter_list|)
block|{
name|int
name|ready
decl_stmt|,
name|revents
decl_stmt|;
name|ngx_err_t
name|err
decl_stmt|;
name|ngx_int_t
name|i
decl_stmt|,
name|nready
decl_stmt|;
name|ngx_uint_t
name|found
decl_stmt|,
name|level
decl_stmt|;
name|ngx_event_t
modifier|*
name|ev
decl_stmt|,
modifier|*
modifier|*
name|queue
decl_stmt|;
name|ngx_connection_t
modifier|*
name|c
decl_stmt|;
comment|/* NGX_TIMER_INFINITE == INFTIM */
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
literal|"poll: %d: fd:%d ev:%04Xd"
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
literal|"poll timer: %M"
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
name|err
operator|=
operator|(
name|ready
operator|==
operator|-
literal|1
operator|)
condition|?
name|ngx_errno
else|:
literal|0
expr_stmt|;
if|if
condition|(
name|flags
operator|&
name|NGX_UPDATE_TIME
operator|||
name|ngx_event_timer_alarm
condition|)
block|{
name|ngx_time_update
argument_list|(
literal|0
argument_list|)
expr_stmt|;
block|}
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
if|if
condition|(
name|err
operator|==
name|NGX_EINTR
condition|)
block|{
if|if
condition|(
name|ngx_event_timer_alarm
condition|)
block|{
name|ngx_event_timer_alarm
operator|=
literal|0
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
name|level
operator|=
name|NGX_LOG_INFO
expr_stmt|;
block|}
else|else
block|{
name|level
operator|=
name|NGX_LOG_ALERT
expr_stmt|;
block|}
name|ngx_log_error
argument_list|(
name|level
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
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|ready
operator|==
literal|0
condition|)
block|{
if|if
condition|(
name|timer
operator|!=
name|NGX_TIMER_INFINITE
condition|)
block|{
return|return
name|NGX_OK
return|;
block|}
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
return|return
name|NGX_ERROR
return|;
block|}
name|ngx_mutex_lock
argument_list|(
name|ngx_posted_events_mutex
argument_list|)
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
name|revents
operator|=
name|event_list
index|[
name|i
index|]
operator|.
name|revents
expr_stmt|;
if|#
directive|if
literal|1
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
literal|"poll: %d: fd:%d ev:%04Xd rev:%04Xd"
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
name|revents
argument_list|)
expr_stmt|;
else|#
directive|else
if|if
condition|(
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
literal|"poll: %d: fd:%d ev:%04Xd rev:%04Xd"
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
name|revents
argument_list|)
expr_stmt|;
block|}
endif|#
directive|endif
if|if
condition|(
name|revents
operator|&
name|POLLNVAL
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
literal|"poll() error fd:%d ev:%04Xd rev:%04Xd"
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
name|revents
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
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
literal|"strange poll() events fd:%d ev:%04Xd rev:%04Xd"
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
name|ngx_cycle
operator|->
name|files
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
if|if
condition|(
operator|(
name|revents
operator|&
operator|(
name|POLLERR
operator||
name|POLLHUP
operator||
name|POLLNVAL
operator|)
operator|)
operator|&&
operator|(
name|revents
operator|&
operator|(
name|POLLIN
operator||
name|POLLOUT
operator|)
operator|)
operator|==
literal|0
condition|)
block|{
comment|/*              * if the error events were returned without POLLIN or POLLOUT,              * then add these flags to handle the events at least in one              * active handler              */
name|revents
operator||=
name|POLLIN
operator||
name|POLLOUT
expr_stmt|;
block|}
name|found
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|revents
operator|&
name|POLLIN
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
if|if
condition|(
operator|(
name|flags
operator|&
name|NGX_POST_THREAD_EVENTS
operator|)
operator|&&
operator|!
name|ev
operator|->
name|accept
condition|)
block|{
name|ev
operator|->
name|posted_ready
operator|=
literal|1
expr_stmt|;
block|}
else|else
block|{
name|ev
operator|->
name|ready
operator|=
literal|1
expr_stmt|;
block|}
name|queue
operator|=
operator|(
name|ngx_event_t
operator|*
operator|*
operator|)
operator|(
name|ev
operator|->
name|accept
condition|?
operator|&
name|ngx_posted_accept_events
else|:
operator|&
name|ngx_posted_events
operator|)
expr_stmt|;
name|ngx_locked_post_event
argument_list|(
name|ev
argument_list|,
name|queue
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|revents
operator|&
name|POLLOUT
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
if|if
condition|(
name|flags
operator|&
name|NGX_POST_THREAD_EVENTS
condition|)
block|{
name|ev
operator|->
name|posted_ready
operator|=
literal|1
expr_stmt|;
block|}
else|else
block|{
name|ev
operator|->
name|ready
operator|=
literal|1
expr_stmt|;
block|}
name|ngx_locked_post_event
argument_list|(
name|ev
argument_list|,
operator|&
name|ngx_posted_events
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
continue|continue;
block|}
block|}
name|ngx_mutex_unlock
argument_list|(
name|ngx_posted_events_mutex
argument_list|)
expr_stmt|;
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
return|return
name|nready
return|;
block|}
end_function

begin_function
specifier|static
name|char
modifier|*
DECL|function|ngx_poll_init_conf (ngx_cycle_t * cycle,void * conf)
name|ngx_poll_init_conf
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|,
name|void
modifier|*
name|conf
parameter_list|)
block|{
name|ngx_event_conf_t
modifier|*
name|ecf
decl_stmt|;
name|ecf
operator|=
name|ngx_event_get_conf
argument_list|(
name|cycle
operator|->
name|conf_ctx
argument_list|,
name|ngx_event_core_module
argument_list|)
expr_stmt|;
if|if
condition|(
name|ecf
operator|->
name|use
operator|!=
name|ngx_poll_module
operator|.
name|ctx_index
condition|)
block|{
return|return
name|NGX_CONF_OK
return|;
block|}
if|#
directive|if
operator|(
name|NGX_THREADS
operator|)
name|ngx_log_error
argument_list|(
name|NGX_LOG_EMERG
argument_list|,
name|cycle
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"poll() is not supported in the threaded mode"
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
else|#
directive|else
return|return
name|NGX_CONF_OK
return|;
endif|#
directive|endif
block|}
end_function

end_unit

