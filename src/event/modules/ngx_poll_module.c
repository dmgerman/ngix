begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (C) 2002-2003 Igor Sysoev, http://sysoev.ru  */
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
specifier|static
name|int
name|ngx_poll_process_events
parameter_list|(
name|ngx_log_t
modifier|*
name|log
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
name|u_int
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
comment|/* init child */
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
name|ngx_event_t
modifier|*
modifier|*
name|index
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
name|ngx_test_null
argument_list|(
name|index
argument_list|,
name|ngx_alloc
argument_list|(
sizeof|sizeof
argument_list|(
name|ngx_event_t
operator|*
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
name|event_index
condition|)
block|{
name|ngx_memcpy
argument_list|(
name|index
argument_list|,
name|event_index
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
name|event_index
argument_list|)
expr_stmt|;
block|}
name|event_index
operator|=
name|index
expr_stmt|;
if|if
condition|(
name|ready_index
condition|)
block|{
name|ngx_free
argument_list|(
name|ready_index
argument_list|)
expr_stmt|;
block|}
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
block|}
if|if
condition|(
name|ngx_event_timer_init
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
name|NGX_HAVE_LEVEL_EVENT
operator||
name|NGX_HAVE_ONESHOT_EVENT
operator||
name|NGX_USE_LEVEL_EVENT
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
name|ngx_event_timer_done
argument_list|(
name|cycle
argument_list|)
expr_stmt|;
name|ngx_free
argument_list|(
name|event_list
argument_list|)
expr_stmt|;
name|ngx_free
argument_list|(
name|event_index
argument_list|)
expr_stmt|;
name|ngx_free
argument_list|(
name|ready_index
argument_list|)
expr_stmt|;
name|event_list
operator|=
name|NULL
expr_stmt|;
name|event_index
operator|=
name|NULL
expr_stmt|;
name|ready_index
operator|=
name|NULL
expr_stmt|;
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
specifier|static
name|int
name|ngx_poll_process_events
parameter_list|(
name|ngx_log_t
modifier|*
name|log
parameter_list|)
block|{
name|int
name|ready
decl_stmt|,
name|found
decl_stmt|;
name|u_int
name|i
decl_stmt|,
name|nready
decl_stmt|;
name|ngx_msec_t
name|timer
decl_stmt|,
name|delta
decl_stmt|;
name|ngx_err_t
name|err
decl_stmt|;
name|ngx_cycle_t
modifier|*
modifier|*
name|cycle
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
operator|(
name|int
operator|)
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
name|ngx_event_expire_timers
argument_list|(
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
block|}
if|if
condition|(
name|err
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
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
name|log
argument_list|,
literal|0
argument_list|,
literal|"unkonwn cycle"
argument_list|)
expr_stmt|;
name|exit
argument_list|(
literal|1
argument_list|)
expr_stmt|;
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
name|POLLNVAL
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|log
argument_list|,
name|EBADF
argument_list|,
literal|"poll() error on %d"
argument_list|,
name|event_list
index|[
name|i
index|]
operator|.
name|fd
argument_list|)
expr_stmt|;
continue|continue;
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
name|POLLIN
operator|||
operator|(
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
operator|)
operator|&&
name|c
operator|->
name|read
operator|->
name|active
operator|)
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
operator|||
operator|(
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
operator|)
operator|&&
name|c
operator|->
name|write
operator|->
name|active
operator|)
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
name|found
condition|)
block|{
name|ready
operator|--
expr_stmt|;
continue|continue;
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
operator|)
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
literal|"strange poll() error on %d:%d:%d"
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
name|ev
operator|->
name|timer_set
operator|=
literal|0
expr_stmt|;
block|}
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
name|ev
operator|->
name|event_handler
argument_list|(
name|ev
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
name|log
argument_list|,
literal|0
argument_list|,
literal|"poll ready != events"
argument_list|)
expr_stmt|;
block|}
return|return
name|NGX_OK
return|;
block|}
end_function

end_unit

