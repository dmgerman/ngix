begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (C) 2002-2003 Igor Sysoev, http://sysoev.ru/en/  */
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

begin_include
include|#
directive|include
file|<ngx_kqueue_module.h>
end_include

begin_typedef
DECL|struct|__anon2b3193c20108
typedef|typedef
struct|struct
block|{
DECL|member|changes
name|int
name|changes
decl_stmt|;
DECL|member|events
name|int
name|events
decl_stmt|;
DECL|typedef|ngx_kqueue_conf_t
block|}
name|ngx_kqueue_conf_t
typedef|;
end_typedef

begin_function_decl
specifier|static
name|int
name|ngx_kqueue_init
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
name|ngx_kqueue_done
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
name|ngx_kqueue_add_event
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
name|ngx_kqueue_del_event
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
name|ngx_kqueue_set_event
parameter_list|(
name|ngx_event_t
modifier|*
name|ev
parameter_list|,
name|int
name|filter
parameter_list|,
name|u_int
name|flags
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|int
name|ngx_kqueue_process_events
parameter_list|(
name|ngx_log_t
modifier|*
name|log
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|void
modifier|*
name|ngx_kqueue_create_conf
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|char
modifier|*
name|ngx_kqueue_init_conf
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
DECL|variable|ngx_kqueue
name|int
name|ngx_kqueue
init|=
operator|-
literal|1
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|change_list
DECL|variable|event_list
specifier|static
name|struct
name|kevent
modifier|*
name|change_list
decl_stmt|,
modifier|*
name|event_list
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|max_changes
DECL|variable|nchanges
DECL|variable|nevents
specifier|static
name|int
name|max_changes
decl_stmt|,
name|nchanges
decl_stmt|,
name|nevents
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|kqueue_name
specifier|static
name|ngx_str_t
name|kqueue_name
init|=
name|ngx_string
argument_list|(
literal|"kqueue"
argument_list|)
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_kqueue_commands
specifier|static
name|ngx_command_t
name|ngx_kqueue_commands
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"kqueue_changes"
argument_list|)
block|,
name|NGX_EVENT_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_conf_set_num_slot
block|,
literal|0
block|,
name|offsetof
argument_list|(
name|ngx_kqueue_conf_t
argument_list|,
name|changes
argument_list|)
block|,
name|NULL
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"kqueue_events"
argument_list|)
block|,
name|NGX_EVENT_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_conf_set_num_slot
block|,
literal|0
block|,
name|offsetof
argument_list|(
name|ngx_kqueue_conf_t
argument_list|,
name|events
argument_list|)
block|,
name|NULL
block|}
block|,
name|ngx_null_command
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_kqueue_module_ctx
name|ngx_event_module_t
name|ngx_kqueue_module_ctx
init|=
block|{
operator|&
name|kqueue_name
block|,
name|ngx_kqueue_create_conf
block|,
comment|/* create configuration */
name|ngx_kqueue_init_conf
block|,
comment|/* init configuration */
block|{
name|ngx_kqueue_add_event
block|,
comment|/* add an event */
name|ngx_kqueue_del_event
block|,
comment|/* delete an event */
name|ngx_kqueue_add_event
block|,
comment|/* enable an event */
name|ngx_kqueue_del_event
block|,
comment|/* disable an event */
name|NULL
block|,
comment|/* add an connection */
name|NULL
block|,
comment|/* delete an connection */
name|ngx_kqueue_process_events
block|,
comment|/* process the events */
name|ngx_kqueue_init
block|,
comment|/* init the events */
name|ngx_kqueue_done
comment|/* done the events */
block|}
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_kqueue_module
name|ngx_module_t
name|ngx_kqueue_module
init|=
block|{
name|NGX_MODULE
block|,
operator|&
name|ngx_kqueue_module_ctx
block|,
comment|/* module context */
name|ngx_kqueue_commands
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
DECL|function|ngx_kqueue_init (ngx_cycle_t * cycle)
specifier|static
name|int
name|ngx_kqueue_init
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
block|{
name|struct
name|timespec
name|ts
decl_stmt|;
name|ngx_kqueue_conf_t
modifier|*
name|kcf
decl_stmt|;
name|kcf
operator|=
name|ngx_event_get_conf
argument_list|(
name|cycle
operator|->
name|conf_ctx
argument_list|,
name|ngx_kqueue_module
argument_list|)
expr_stmt|;
name|ngx_log_debug
argument_list|(
argument|cycle->log
argument_list|,
literal|"CH: %d"
argument|_ kcf->changes
argument_list|)
empty_stmt|;
name|ngx_log_debug
argument_list|(
argument|cycle->log
argument_list|,
literal|"EV: %d"
argument|_ kcf->events
argument_list|)
empty_stmt|;
if|if
condition|(
name|ngx_kqueue
operator|==
operator|-
literal|1
condition|)
block|{
name|ngx_kqueue
operator|=
name|kqueue
argument_list|()
expr_stmt|;
if|if
condition|(
name|ngx_kqueue
operator|==
operator|-
literal|1
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_EMERG
argument_list|,
name|cycle
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"kqueue() failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
block|}
if|if
condition|(
name|max_changes
operator|<
name|kcf
operator|->
name|changes
condition|)
block|{
if|if
condition|(
name|nchanges
condition|)
block|{
name|ts
operator|.
name|tv_sec
operator|=
literal|0
expr_stmt|;
name|ts
operator|.
name|tv_nsec
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|kevent
argument_list|(
name|ngx_kqueue
argument_list|,
name|change_list
argument_list|,
name|nchanges
argument_list|,
name|NULL
argument_list|,
literal|0
argument_list|,
operator|&
name|ts
argument_list|)
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
name|ngx_errno
argument_list|,
literal|"kevent() failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
name|nchanges
operator|=
literal|0
expr_stmt|;
block|}
if|if
condition|(
name|change_list
condition|)
block|{
name|ngx_free
argument_list|(
name|change_list
argument_list|)
expr_stmt|;
block|}
name|ngx_test_null
argument_list|(
name|change_list
argument_list|,
name|ngx_alloc
argument_list|(
name|kcf
operator|->
name|changes
operator|*
sizeof|sizeof
argument_list|(
expr|struct
name|kevent
argument_list|)
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
name|max_changes
operator|=
name|kcf
operator|->
name|changes
expr_stmt|;
if|if
condition|(
name|nevents
operator|<
name|kcf
operator|->
name|events
condition|)
block|{
if|if
condition|(
name|event_list
condition|)
block|{
name|ngx_free
argument_list|(
name|event_list
argument_list|)
expr_stmt|;
block|}
name|ngx_test_null
argument_list|(
name|event_list
argument_list|,
name|ngx_alloc
argument_list|(
name|kcf
operator|->
name|events
operator|*
sizeof|sizeof
argument_list|(
expr|struct
name|kevent
argument_list|)
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
name|nevents
operator|=
name|kcf
operator|->
name|events
expr_stmt|;
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
name|ngx_kqueue_module_ctx
operator|.
name|actions
expr_stmt|;
name|ngx_event_flags
operator|=
name|NGX_USE_ONESHOT_EVENT
if|#
directive|if
operator|(
name|HAVE_CLEAR_EVENT
operator|)
operator||
name|NGX_USE_CLEAR_EVENT
else|#
directive|else
operator||
name|NGX_USE_LEVEL_EVENT
endif|#
directive|endif
if|#
directive|if
operator|(
name|HAVE_LOWAT_EVENT
operator|)
operator||
name|NGX_HAVE_LOWAT_EVENT
endif|#
directive|endif
operator||
name|NGX_HAVE_KQUEUE_EVENT
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_kqueue_done (ngx_cycle_t * cycle)
specifier|static
name|void
name|ngx_kqueue_done
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
block|{
if|if
condition|(
name|close
argument_list|(
name|ngx_kqueue
argument_list|)
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
name|ngx_errno
argument_list|,
literal|"kqueue close() failed"
argument_list|)
expr_stmt|;
block|}
name|ngx_kqueue
operator|=
operator|-
literal|1
expr_stmt|;
name|ngx_event_timer_done
argument_list|(
name|cycle
argument_list|)
expr_stmt|;
name|ngx_free
argument_list|(
name|change_list
argument_list|)
expr_stmt|;
name|ngx_free
argument_list|(
name|event_list
argument_list|)
expr_stmt|;
name|change_list
operator|=
name|NULL
expr_stmt|;
name|event_list
operator|=
name|NULL
expr_stmt|;
name|max_changes
operator|=
literal|0
expr_stmt|;
name|nchanges
operator|=
literal|0
expr_stmt|;
name|nevents
operator|=
literal|0
expr_stmt|;
block|}
end_function

begin_function
DECL|function|ngx_kqueue_add_event (ngx_event_t * ev,int event,u_int flags)
specifier|static
name|int
name|ngx_kqueue_add_event
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
name|ngx_connection_t
modifier|*
name|c
decl_stmt|;
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
name|nchanges
operator|>
literal|0
operator|&&
name|ev
operator|->
name|index
operator|<
operator|(
name|u_int
operator|)
name|nchanges
operator|&&
operator|(
operator|(
name|uintptr_t
operator|)
name|change_list
index|[
name|ev
operator|->
name|index
index|]
operator|.
name|udata
operator|&
operator|(
name|uintptr_t
operator|)
operator|~
literal|1
operator|)
operator|==
operator|(
name|uintptr_t
operator|)
name|ev
condition|)
block|{
name|c
operator|=
name|ev
operator|->
name|data
expr_stmt|;
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
literal|"previous event on #%d were not passed in kernel"
argument_list|,
name|c
operator|->
name|fd
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
return|return
name|ngx_kqueue_set_event
argument_list|(
name|ev
argument_list|,
name|event
argument_list|,
name|EV_ADD
operator||
name|flags
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|ngx_kqueue_del_event (ngx_event_t * ev,int event,u_int flags)
specifier|static
name|int
name|ngx_kqueue_del_event
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
name|ev
operator|->
name|active
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|nchanges
operator|>
literal|0
operator|&&
name|ev
operator|->
name|index
operator|<
operator|(
name|u_int
operator|)
name|nchanges
operator|&&
operator|(
operator|(
name|uintptr_t
operator|)
name|change_list
index|[
name|ev
operator|->
name|index
index|]
operator|.
name|udata
operator|&
operator|(
name|uintptr_t
operator|)
operator|~
literal|1
operator|)
operator|==
operator|(
name|uintptr_t
operator|)
name|ev
condition|)
block|{
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
literal|"kqueue event deleted: %d: ft:%d"
argument|_                       c->fd _ event
argument_list|)
empty_stmt|;
endif|#
directive|endif
comment|/* if the event is still not passed to a kernel we will not pass it */
if|if
condition|(
name|ev
operator|->
name|index
operator|<
operator|(
name|u_int
operator|)
operator|--
name|nchanges
condition|)
block|{
name|e
operator|=
operator|(
name|ngx_event_t
operator|*
operator|)
name|change_list
index|[
name|nchanges
index|]
operator|.
name|udata
expr_stmt|;
name|change_list
index|[
name|ev
operator|->
name|index
index|]
operator|=
name|change_list
index|[
name|nchanges
index|]
expr_stmt|;
name|e
operator|->
name|index
operator|=
name|ev
operator|->
name|index
expr_stmt|;
block|}
return|return
name|NGX_OK
return|;
block|}
comment|/*      * when the file descriptor is closed a kqueue automatically deletes      * its filters so we do not need to delete explicity the event      * before the closing the file descriptor.      */
if|if
condition|(
name|flags
operator|&
name|NGX_CLOSE_EVENT
condition|)
block|{
return|return
name|NGX_OK
return|;
block|}
return|return
name|ngx_kqueue_set_event
argument_list|(
name|ev
argument_list|,
name|event
argument_list|,
name|flags
operator|&
name|NGX_DISABLE_EVENT
condition|?
name|EV_DISABLE
else|:
name|EV_DELETE
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|ngx_kqueue_set_event (ngx_event_t * ev,int filter,u_int flags)
specifier|static
name|int
name|ngx_kqueue_set_event
parameter_list|(
name|ngx_event_t
modifier|*
name|ev
parameter_list|,
name|int
name|filter
parameter_list|,
name|u_int
name|flags
parameter_list|)
block|{
name|struct
name|timespec
name|ts
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
if|#
directive|if
operator|(
name|NGX_DEBUG_EVENT
operator|)
name|ngx_log_debug
argument_list|(
argument|ev->log
argument_list|,
literal|"kqueue set event: %d: ft:%d fl:%08x"
argument|_                   c->fd _ filter _ flags
argument_list|)
empty_stmt|;
endif|#
directive|endif
if|if
condition|(
name|nchanges
operator|>=
name|max_changes
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_WARN
argument_list|,
name|ev
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"kqueue change list is filled up"
argument_list|)
expr_stmt|;
name|ts
operator|.
name|tv_sec
operator|=
literal|0
expr_stmt|;
name|ts
operator|.
name|tv_nsec
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|kevent
argument_list|(
name|ngx_kqueue
argument_list|,
name|change_list
argument_list|,
name|nchanges
argument_list|,
name|NULL
argument_list|,
literal|0
argument_list|,
operator|&
name|ts
argument_list|)
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
name|ngx_errno
argument_list|,
literal|"kevent() failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
name|nchanges
operator|=
literal|0
expr_stmt|;
block|}
name|change_list
index|[
name|nchanges
index|]
operator|.
name|ident
operator|=
name|c
operator|->
name|fd
expr_stmt|;
name|change_list
index|[
name|nchanges
index|]
operator|.
name|filter
operator|=
name|filter
expr_stmt|;
name|change_list
index|[
name|nchanges
index|]
operator|.
name|flags
operator|=
name|flags
expr_stmt|;
name|change_list
index|[
name|nchanges
index|]
operator|.
name|udata
operator|=
operator|(
name|void
operator|*
operator|)
operator|(
operator|(
name|uintptr_t
operator|)
name|ev
operator||
name|ev
operator|->
name|instance
operator|)
expr_stmt|;
if|#
directive|if
operator|(
name|HAVE_LOWAT_EVENT
operator|)
if|if
condition|(
name|flags
operator|&
name|NGX_LOWAT_EVENT
condition|)
block|{
name|change_list
index|[
name|nchanges
index|]
operator|.
name|fflags
operator|=
name|NOTE_LOWAT
expr_stmt|;
name|change_list
index|[
name|nchanges
index|]
operator|.
name|data
operator|=
name|ev
operator|->
name|available
expr_stmt|;
block|}
else|else
block|{
name|change_list
index|[
name|nchanges
index|]
operator|.
name|fflags
operator|=
literal|0
expr_stmt|;
name|change_list
index|[
name|nchanges
index|]
operator|.
name|data
operator|=
literal|0
expr_stmt|;
block|}
else|#
directive|else
name|change_list
index|[
name|nchanges
index|]
operator|.
name|fflags
operator|=
literal|0
expr_stmt|;
name|change_list
index|[
name|nchanges
index|]
operator|.
name|data
operator|=
literal|0
expr_stmt|;
endif|#
directive|endif
name|ev
operator|->
name|index
operator|=
name|nchanges
expr_stmt|;
name|nchanges
operator|++
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_kqueue_process_events (ngx_log_t * log)
specifier|static
name|int
name|ngx_kqueue_process_events
parameter_list|(
name|ngx_log_t
modifier|*
name|log
parameter_list|)
block|{
name|int
name|events
decl_stmt|,
name|instance
decl_stmt|,
name|i
decl_stmt|;
name|ngx_err_t
name|err
decl_stmt|;
name|ngx_msec_t
name|timer
decl_stmt|;
name|ngx_event_t
modifier|*
name|ev
decl_stmt|;
name|ngx_epoch_msec_t
name|delta
decl_stmt|;
name|struct
name|timeval
name|tv
decl_stmt|;
name|struct
name|timespec
name|ts
decl_stmt|,
modifier|*
name|tp
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
name|ts
operator|.
name|tv_sec
operator|=
name|timer
operator|/
literal|1000
expr_stmt|;
name|ts
operator|.
name|tv_nsec
operator|=
operator|(
name|timer
operator|%
literal|1000
operator|)
operator|*
literal|1000000
expr_stmt|;
name|tp
operator|=
operator|&
name|ts
expr_stmt|;
name|ngx_gettimeofday
argument_list|(
operator|&
name|tv
argument_list|)
expr_stmt|;
name|delta
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
expr_stmt|;
block|}
else|else
block|{
name|delta
operator|=
literal|0
expr_stmt|;
name|tp
operator|=
name|NULL
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
literal|"kevent timer: %d"
argument|_ timer
argument_list|)
empty_stmt|;
endif|#
directive|endif
name|events
operator|=
name|kevent
argument_list|(
name|ngx_kqueue
argument_list|,
name|change_list
argument_list|,
name|nchanges
argument_list|,
name|event_list
argument_list|,
name|nevents
argument_list|,
name|tp
argument_list|)
expr_stmt|;
if|if
condition|(
name|events
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
name|nchanges
operator|=
literal|0
expr_stmt|;
name|ngx_gettimeofday
argument_list|(
operator|&
name|tv
argument_list|)
expr_stmt|;
if|if
condition|(
name|ngx_cached_time
operator|!=
name|tv
operator|.
name|tv_sec
condition|)
block|{
name|ngx_cached_time
operator|=
name|tv
operator|.
name|tv_sec
expr_stmt|;
name|ngx_time_update
argument_list|()
expr_stmt|;
block|}
if|if
condition|(
name|timer
condition|)
block|{
name|delta
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
literal|"kevent timer: %d, delta: %d"
argument|_ timer _ (int) delta
argument_list|)
empty_stmt|;
endif|#
directive|endif
comment|/*          * The expired timers must be handled before a processing of the events          * because the new timers can be added during a processing          */
name|ngx_event_expire_timers
argument_list|(
operator|(
name|ngx_msec_t
operator|)
name|delta
argument_list|)
expr_stmt|;
block|}
else|else
block|{
if|if
condition|(
name|events
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
literal|"kevent() returned no events without timeout"
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
literal|"kevent timer: %d, delta: %d"
argument|_ timer _ (int) delta
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
literal|"kevent() failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|events
condition|;
name|i
operator|++
control|)
block|{
if|#
directive|if
operator|(
name|NGX_DEBUG_EVENT
operator|)
if|if
condition|(
name|event_list
index|[
name|i
index|]
operator|.
name|ident
operator|>
literal|0x8000000
operator|&&
name|event_list
index|[
name|i
index|]
operator|.
name|ident
operator|!=
operator|(
name|unsigned
operator|)
operator|-
literal|1
condition|)
block|{
name|ngx_log_debug
argument_list|(
argument|log
argument_list|,
literal|"kevent: %08x: ft:%d fl:%08x ff:%08x d:%d ud:%08x"
argument|_                           event_list[i].ident _ event_list[i].filter _                           event_list[i].flags _ event_list[i].fflags _                           event_list[i].data _ event_list[i].udata
argument_list|)
empty_stmt|;
block|}
else|else
block|{
name|ngx_log_debug
argument_list|(
argument|log
argument_list|,
literal|"kevent: %d: ft:%d fl:%08x ff:%08x d:%d ud:%08x"
argument|_                           event_list[i].ident _ event_list[i].filter _                           event_list[i].flags _ event_list[i].fflags _                           event_list[i].data _ event_list[i].udata
argument_list|)
empty_stmt|;
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
name|flags
operator|&
name|EV_ERROR
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|log
argument_list|,
name|event_list
index|[
name|i
index|]
operator|.
name|data
argument_list|,
literal|"kevent() error on %d"
argument_list|,
name|event_list
index|[
name|i
index|]
operator|.
name|ident
argument_list|)
expr_stmt|;
continue|continue;
block|}
name|ev
operator|=
operator|(
name|ngx_event_t
operator|*
operator|)
name|event_list
index|[
name|i
index|]
operator|.
name|udata
expr_stmt|;
switch|switch
condition|(
name|event_list
index|[
name|i
index|]
operator|.
name|filter
condition|)
block|{
case|case
name|EVFILT_READ
case|:
case|case
name|EVFILT_WRITE
case|:
name|instance
operator|=
operator|(
name|uintptr_t
operator|)
name|ev
operator|&
literal|1
expr_stmt|;
name|ev
operator|=
operator|(
name|ngx_event_t
operator|*
operator|)
operator|(
operator|(
name|uintptr_t
operator|)
name|ev
operator|&
operator|(
name|uintptr_t
operator|)
operator|~
literal|1
operator|)
expr_stmt|;
comment|/*              * it's a stale event from a file descriptor              * that was just closed in this iteration              */
if|if
condition|(
name|ev
operator|->
name|active
operator|==
literal|0
operator|||
name|ev
operator|->
name|instance
operator|!=
name|instance
condition|)
block|{
name|ngx_log_debug
argument_list|(
name|log
argument_list|,
literal|"stale kevent"
argument_list|)
expr_stmt|;
continue|continue;
block|}
name|ev
operator|->
name|available
operator|=
name|event_list
index|[
name|i
index|]
operator|.
name|data
expr_stmt|;
if|if
condition|(
name|event_list
index|[
name|i
index|]
operator|.
name|flags
operator|&
name|EV_EOF
condition|)
block|{
name|ev
operator|->
name|kq_eof
operator|=
literal|1
expr_stmt|;
name|ev
operator|->
name|kq_errno
operator|=
name|event_list
index|[
name|i
index|]
operator|.
name|fflags
expr_stmt|;
block|}
if|if
condition|(
name|ev
operator|->
name|oneshot
operator|&&
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
name|ev
operator|->
name|ready
operator|=
literal|1
expr_stmt|;
name|ev
operator|->
name|event_handler
argument_list|(
name|ev
argument_list|)
expr_stmt|;
break|break;
case|case
name|EVFILT_AIO
case|:
name|ev
operator|->
name|complete
operator|=
literal|1
expr_stmt|;
name|ev
operator|->
name|ready
operator|=
literal|1
expr_stmt|;
name|ev
operator|->
name|event_handler
argument_list|(
name|ev
argument_list|)
expr_stmt|;
break|break;
default|default:
name|ngx_log_error
argument_list|(
argument|NGX_LOG_ALERT
argument_list|,
argument|log
argument_list|,
literal|0
argument_list|,
literal|"unexpected kevent filter %d"
argument|_ event_list[i].filter
argument_list|)
empty_stmt|;
block|}
block|}
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_kqueue_create_conf (ngx_cycle_t * cycle)
specifier|static
name|void
modifier|*
name|ngx_kqueue_create_conf
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
block|{
name|ngx_kqueue_conf_t
modifier|*
name|kcf
decl_stmt|;
name|ngx_test_null
argument_list|(
name|kcf
argument_list|,
name|ngx_palloc
argument_list|(
name|cycle
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_kqueue_conf_t
argument_list|)
argument_list|)
argument_list|,
name|NGX_CONF_ERROR
argument_list|)
expr_stmt|;
name|kcf
operator|->
name|changes
operator|=
name|NGX_CONF_UNSET
expr_stmt|;
name|kcf
operator|->
name|events
operator|=
name|NGX_CONF_UNSET
expr_stmt|;
return|return
name|kcf
return|;
block|}
end_function

begin_function
DECL|function|ngx_kqueue_init_conf (ngx_cycle_t * cycle,void * conf)
specifier|static
name|char
modifier|*
name|ngx_kqueue_init_conf
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
name|ngx_kqueue_conf_t
modifier|*
name|kcf
init|=
name|conf
decl_stmt|;
name|ngx_conf_init_value
argument_list|(
name|kcf
operator|->
name|changes
argument_list|,
literal|512
argument_list|)
expr_stmt|;
name|ngx_conf_init_value
argument_list|(
name|kcf
operator|->
name|events
argument_list|,
literal|512
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

end_unit

