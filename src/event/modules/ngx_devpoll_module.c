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

begin_if
if|#
directive|if
operator|(
name|TEST_BUILD_DEVPOLL
operator|)
end_if

begin_comment
comment|/* Solaris declarations */
end_comment

begin_define
DECL|macro|POLLREMOVE
define|#
directive|define
name|POLLREMOVE
value|0x0800
end_define

begin_define
DECL|macro|DP_POLL
define|#
directive|define
name|DP_POLL
value|0xD001
end_define

begin_struct
DECL|struct|dvpoll
struct|struct
name|dvpoll
block|{
DECL|member|dp_fds
name|struct
name|pollfd
modifier|*
name|dp_fds
decl_stmt|;
DECL|member|dp_nfds
name|int
name|dp_nfds
decl_stmt|;
DECL|member|dp_timeout
name|int
name|dp_timeout
decl_stmt|;
block|}
struct|;
end_struct

begin_endif
endif|#
directive|endif
end_endif

begin_typedef
DECL|struct|__anon2b64c4ef0108
typedef|typedef
struct|struct
block|{
DECL|member|changes
name|u_int
name|changes
decl_stmt|;
DECL|member|events
name|u_int
name|events
decl_stmt|;
DECL|typedef|ngx_devpoll_conf_t
block|}
name|ngx_devpoll_conf_t
typedef|;
end_typedef

begin_function_decl
specifier|static
name|int
name|ngx_devpoll_init
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
name|ngx_devpoll_done
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
name|ngx_devpoll_add_event
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
name|ngx_devpoll_del_event
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
name|ngx_devpoll_set_event
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
name|ngx_devpoll_process_events
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
name|ngx_devpoll_create_conf
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
name|ngx_devpoll_init_conf
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
DECL|variable|dp
specifier|static
name|int
name|dp
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
name|pollfd
modifier|*
name|change_list
decl_stmt|,
modifier|*
name|event_list
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|nchanges
DECL|variable|max_changes
DECL|variable|nevents
specifier|static
name|u_int
name|nchanges
decl_stmt|,
name|max_changes
decl_stmt|,
name|nevents
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|change_index
specifier|static
name|ngx_event_t
modifier|*
modifier|*
name|change_index
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|devpoll_name
specifier|static
name|ngx_str_t
name|devpoll_name
init|=
name|ngx_string
argument_list|(
literal|"/dev/poll"
argument_list|)
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_devpoll_commands
specifier|static
name|ngx_command_t
name|ngx_devpoll_commands
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"devpoll_changes"
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
name|ngx_devpoll_conf_t
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
literal|"devpoll_events"
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
name|ngx_devpoll_conf_t
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
DECL|variable|ngx_devpoll_module_ctx
name|ngx_event_module_t
name|ngx_devpoll_module_ctx
init|=
block|{
operator|&
name|devpoll_name
block|,
name|ngx_devpoll_create_conf
block|,
comment|/* create configuration */
name|ngx_devpoll_init_conf
block|,
comment|/* init configuration */
block|{
name|ngx_devpoll_add_event
block|,
comment|/* add an event */
name|ngx_devpoll_del_event
block|,
comment|/* delete an event */
name|ngx_devpoll_add_event
block|,
comment|/* enable an event */
name|ngx_devpoll_del_event
block|,
comment|/* disable an event */
name|NULL
block|,
comment|/* add an connection */
name|NULL
block|,
comment|/* delete an connection */
name|ngx_devpoll_process_events
block|,
comment|/* process the events */
name|ngx_devpoll_init
block|,
comment|/* init the events */
name|ngx_devpoll_done
block|,
comment|/* done the events */
block|}
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_devpoll_module
name|ngx_module_t
name|ngx_devpoll_module
init|=
block|{
name|NGX_MODULE
block|,
operator|&
name|ngx_devpoll_module_ctx
block|,
comment|/* module context */
name|ngx_devpoll_commands
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
DECL|function|ngx_devpoll_init (ngx_cycle_t * cycle)
specifier|static
name|int
name|ngx_devpoll_init
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
block|{
name|int
name|n
decl_stmt|;
name|ngx_devpoll_conf_t
modifier|*
name|dpcf
decl_stmt|;
name|dpcf
operator|=
name|ngx_event_get_conf
argument_list|(
name|cycle
operator|->
name|conf_ctx
argument_list|,
name|ngx_devpoll_module
argument_list|)
expr_stmt|;
name|ngx_log_debug
argument_list|(
argument|cycle->log
argument_list|,
literal|"CH: %d"
argument|_ dpcf->changes
argument_list|)
empty_stmt|;
name|ngx_log_debug
argument_list|(
argument|cycle->log
argument_list|,
literal|"EV: %d"
argument|_ dpcf->events
argument_list|)
empty_stmt|;
if|if
condition|(
name|dp
operator|==
operator|-
literal|1
condition|)
block|{
name|dp
operator|=
name|open
argument_list|(
literal|"/dev/poll"
argument_list|,
name|O_RDWR
argument_list|)
expr_stmt|;
if|if
condition|(
name|dp
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
literal|"open(/dev/poll) failed"
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
name|dpcf
operator|->
name|changes
condition|)
block|{
if|if
condition|(
name|nchanges
condition|)
block|{
name|n
operator|=
name|nchanges
operator|*
sizeof|sizeof
argument_list|(
expr|struct
name|pollfd
argument_list|)
expr_stmt|;
if|if
condition|(
name|write
argument_list|(
name|dp
argument_list|,
name|change_list
argument_list|,
name|n
argument_list|)
operator|!=
name|n
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
literal|"write(/dev/poll) failed"
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
sizeof|sizeof
argument_list|(
expr|struct
name|pollfd
argument_list|)
operator|*
name|dpcf
operator|->
name|changes
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
name|change_index
condition|)
block|{
name|ngx_free
argument_list|(
name|change_index
argument_list|)
expr_stmt|;
block|}
name|ngx_test_null
argument_list|(
name|change_index
argument_list|,
name|ngx_alloc
argument_list|(
sizeof|sizeof
argument_list|(
name|ngx_event_t
operator|*
argument_list|)
operator|*
name|dpcf
operator|->
name|changes
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
name|dpcf
operator|->
name|changes
expr_stmt|;
if|if
condition|(
name|nevents
operator|<
name|dpcf
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
sizeof|sizeof
argument_list|(
expr|struct
name|pollfd
argument_list|)
operator|*
name|dpcf
operator|->
name|events
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
name|dpcf
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
name|ngx_devpoll_module_ctx
operator|.
name|actions
expr_stmt|;
name|ngx_event_flags
operator|=
name|NGX_USE_LEVEL_EVENT
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_devpoll_done (ngx_cycle_t * cycle)
specifier|static
name|void
name|ngx_devpoll_done
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
name|dp
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
literal|"close(/dev/poll) failed"
argument_list|)
expr_stmt|;
block|}
name|dp
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
name|ngx_free
argument_list|(
name|change_index
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
name|change_index
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
DECL|function|ngx_devpoll_add_event (ngx_event_t * ev,int event,u_int flags)
specifier|static
name|int
name|ngx_devpoll_add_event
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
endif|#
directive|endif
if|#
directive|if
operator|(
name|NGX_READ_EVENT
operator|!=
name|POLLIN
operator|)
if|if
condition|(
name|event
operator|==
name|NGX_READ_EVENT
condition|)
block|{
name|event
operator|=
name|POLLOUT
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_WRITE_EVENT
operator|!=
name|POLLOUT
operator|)
block|}
else|else
block|{
name|event
operator|=
name|POLLIN
expr_stmt|;
endif|#
directive|endif
block|}
endif|#
directive|endif
if|#
directive|if
operator|(
name|NGX_DEBUG_EVENT
operator|)
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
name|ev
operator|->
name|active
operator|=
literal|1
expr_stmt|;
return|return
name|ngx_devpoll_set_event
argument_list|(
name|ev
argument_list|,
name|event
argument_list|,
literal|0
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|ngx_devpoll_del_event (ngx_event_t * ev,int event,u_int flags)
specifier|static
name|int
name|ngx_devpoll_del_event
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
name|ngx_devpoll_set_event
argument_list|(
name|ev
argument_list|,
name|POLLREMOVE
argument_list|,
name|flags
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|ev
operator|->
name|active
operator|=
literal|0
expr_stmt|;
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
comment|/* we need to restore the second event if it exists */
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
name|event
operator|=
name|POLLOUT
expr_stmt|;
block|}
else|else
block|{
name|e
operator|=
name|c
operator|->
name|read
expr_stmt|;
name|event
operator|=
name|POLLIN
expr_stmt|;
block|}
if|if
condition|(
name|e
condition|)
block|{
return|return
name|ngx_devpoll_set_event
argument_list|(
name|e
argument_list|,
name|event
argument_list|,
literal|0
argument_list|)
return|;
block|}
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_devpoll_set_event (ngx_event_t * ev,int event,u_int flags)
specifier|static
name|int
name|ngx_devpoll_set_event
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
name|int
name|n
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
literal|"devpoll fd:%d event:%d flush:%d"
argument|_                            c->fd _ event _ flags
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
literal|"/dev/pool change list is filled up"
argument_list|)
expr_stmt|;
name|n
operator|=
name|nchanges
operator|*
sizeof|sizeof
argument_list|(
expr|struct
name|pollfd
argument_list|)
expr_stmt|;
if|if
condition|(
name|write
argument_list|(
name|dp
argument_list|,
name|change_list
argument_list|,
name|n
argument_list|)
operator|!=
name|n
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
literal|"write(/dev/poll) failed"
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
name|fd
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
name|events
operator|=
name|event
expr_stmt|;
name|change_list
index|[
name|nchanges
index|]
operator|.
name|revents
operator|=
literal|0
expr_stmt|;
name|change_index
index|[
name|nchanges
index|]
operator|=
name|ev
expr_stmt|;
name|ev
operator|->
name|index
operator|=
name|nchanges
expr_stmt|;
name|nchanges
operator|++
expr_stmt|;
if|if
condition|(
name|flags
operator|&
name|NGX_CLOSE_EVENT
condition|)
block|{
name|n
operator|=
name|nchanges
operator|*
sizeof|sizeof
argument_list|(
expr|struct
name|pollfd
argument_list|)
expr_stmt|;
if|if
condition|(
name|write
argument_list|(
name|dp
argument_list|,
name|change_list
argument_list|,
name|n
argument_list|)
operator|!=
name|n
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
literal|"write(/dev/poll) failed"
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
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_devpoll_process_events (ngx_log_t * log)
name|int
name|ngx_devpoll_process_events
parameter_list|(
name|ngx_log_t
modifier|*
name|log
parameter_list|)
block|{
name|int
name|events
decl_stmt|,
name|n
decl_stmt|,
name|i
decl_stmt|,
name|j
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
name|ngx_connection_t
modifier|*
name|c
decl_stmt|;
name|struct
name|dvpoll
name|dvp
decl_stmt|;
name|struct
name|timeval
name|tv
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
name|gettimeofday
argument_list|(
operator|&
name|tv
argument_list|,
name|NULL
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
name|ngx_log_debug
argument_list|(
argument|log
argument_list|,
literal|"devpoll timer: %d"
argument|_ timer
argument_list|)
empty_stmt|;
endif|#
directive|endif
if|if
condition|(
name|nchanges
condition|)
block|{
name|n
operator|=
name|nchanges
operator|*
sizeof|sizeof
argument_list|(
expr|struct
name|pollfd
argument_list|)
expr_stmt|;
if|if
condition|(
name|write
argument_list|(
name|dp
argument_list|,
name|change_list
argument_list|,
name|n
argument_list|)
operator|!=
name|n
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
literal|"write(/dev/poll) failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
block|}
name|dvp
operator|.
name|dp_fds
operator|=
name|event_list
expr_stmt|;
name|dvp
operator|.
name|dp_nfds
operator|=
name|nevents
expr_stmt|;
name|dvp
operator|.
name|dp_timeout
operator|=
name|timer
expr_stmt|;
name|events
operator|=
name|ioctl
argument_list|(
name|dp
argument_list|,
name|DP_POLL
argument_list|,
operator|&
name|dvp
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
name|gettimeofday
argument_list|(
operator|&
name|tv
argument_list|,
name|NULL
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
literal|"devpoll timer: %d, delta: %d"
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
literal|"ioctl(DP_POLL) returned no events without timeout"
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
literal|"devpoll timer: %d, delta: %d"
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
literal|"ioctl(DP_POLL) failed"
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
name|j
operator|=
literal|0
init|;
name|j
operator|<
name|ngx_old_cycles
operator|.
name|nelts
condition|;
name|j
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
name|j
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
literal|"devpoll: %d: ev:%d rev:%d"
argument|_                       event_list[i].fd _                       event_list[i].events _ event_list[i].revents
argument_list|)
empty_stmt|;
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
name|POLLIN
condition|)
block|{
if|if
condition|(
operator|!
name|c
operator|->
name|read
operator|->
name|active
condition|)
block|{
continue|continue;
block|}
name|c
operator|->
name|read
operator|->
name|ready
operator|=
literal|1
expr_stmt|;
name|c
operator|->
name|read
operator|->
name|event_handler
argument_list|(
name|c
operator|->
name|read
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
name|POLLOUT
condition|)
block|{
if|if
condition|(
operator|!
name|c
operator|->
name|write
operator|->
name|active
condition|)
block|{
continue|continue;
block|}
name|c
operator|->
name|write
operator|->
name|ready
operator|=
literal|1
expr_stmt|;
name|c
operator|->
name|write
operator|->
name|event_handler
argument_list|(
name|c
operator|->
name|write
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
operator|(
name|POLLERR
operator||
name|POLLHUP
operator||
name|POLLNVAL
operator|)
condition|)
block|{
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
literal|"ioctl(DP_POLL) error on %d:%d"
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
block|}
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_devpoll_create_conf (ngx_cycle_t * cycle)
specifier|static
name|void
modifier|*
name|ngx_devpoll_create_conf
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
block|{
name|ngx_devpoll_conf_t
modifier|*
name|dpcf
decl_stmt|;
name|ngx_test_null
argument_list|(
name|dpcf
argument_list|,
name|ngx_palloc
argument_list|(
name|cycle
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_devpoll_conf_t
argument_list|)
argument_list|)
argument_list|,
name|NGX_CONF_ERROR
argument_list|)
expr_stmt|;
name|dpcf
operator|->
name|changes
operator|=
name|NGX_CONF_UNSET
expr_stmt|;
name|dpcf
operator|->
name|events
operator|=
name|NGX_CONF_UNSET
expr_stmt|;
return|return
name|dpcf
return|;
block|}
end_function

begin_function
DECL|function|ngx_devpoll_init_conf (ngx_cycle_t * cycle,void * conf)
specifier|static
name|char
modifier|*
name|ngx_devpoll_init_conf
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
name|ngx_devpoll_conf_t
modifier|*
name|dpcf
init|=
name|conf
decl_stmt|;
name|ngx_conf_init_unsigned_value
argument_list|(
name|dpcf
operator|->
name|changes
argument_list|,
literal|512
argument_list|)
expr_stmt|;
name|ngx_conf_init_unsigned_value
argument_list|(
name|dpcf
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

