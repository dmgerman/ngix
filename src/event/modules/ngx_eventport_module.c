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

begin_if
if|#
directive|if
operator|(
name|NGX_TEST_BUILD_EVENTPORT
operator|)
end_if

begin_define
DECL|macro|ushort_t
define|#
directive|define
name|ushort_t
value|u_short
end_define

begin_define
DECL|macro|uint_t
define|#
directive|define
name|uint_t
value|u_int
end_define

begin_comment
comment|/* Solaris declarations */
end_comment

begin_define
DECL|macro|PORT_SOURCE_AIO
define|#
directive|define
name|PORT_SOURCE_AIO
value|1
end_define

begin_define
DECL|macro|PORT_SOURCE_TIMER
define|#
directive|define
name|PORT_SOURCE_TIMER
value|2
end_define

begin_define
DECL|macro|PORT_SOURCE_USER
define|#
directive|define
name|PORT_SOURCE_USER
value|3
end_define

begin_define
DECL|macro|PORT_SOURCE_FD
define|#
directive|define
name|PORT_SOURCE_FD
value|4
end_define

begin_define
DECL|macro|PORT_SOURCE_ALERT
define|#
directive|define
name|PORT_SOURCE_ALERT
value|5
end_define

begin_define
DECL|macro|PORT_SOURCE_MQ
define|#
directive|define
name|PORT_SOURCE_MQ
value|6
end_define

begin_define
DECL|macro|ETIME
define|#
directive|define
name|ETIME
value|64
end_define

begin_define
DECL|macro|SIGEV_PORT
define|#
directive|define
name|SIGEV_PORT
value|4
end_define

begin_typedef
DECL|struct|__anon2bddae800108
typedef|typedef
struct|struct
block|{
DECL|member|portev_events
name|int
name|portev_events
decl_stmt|;
comment|/* event data is source specific */
DECL|member|portev_source
name|ushort_t
name|portev_source
decl_stmt|;
comment|/* event source */
DECL|member|portev_pad
name|ushort_t
name|portev_pad
decl_stmt|;
comment|/* port internal use */
DECL|member|portev_object
name|uintptr_t
name|portev_object
decl_stmt|;
comment|/* source specific object */
DECL|member|portev_user
name|void
modifier|*
name|portev_user
decl_stmt|;
comment|/* user cookie */
DECL|typedef|port_event_t
block|}
name|port_event_t
typedef|;
end_typedef

begin_typedef
DECL|struct|port_notify
typedef|typedef
struct|struct
name|port_notify
block|{
DECL|member|portnfy_port
name|int
name|portnfy_port
decl_stmt|;
comment|/* bind request(s) to port */
DECL|member|portnfy_user
name|void
modifier|*
name|portnfy_user
decl_stmt|;
comment|/* user defined */
DECL|typedef|port_notify_t
block|}
name|port_notify_t
typedef|;
end_typedef

begin_if
if|#
directive|if
operator|(
name|__FreeBSD_version
operator|<
literal|700005
operator|)
end_if

begin_typedef
DECL|struct|itimerspec
typedef|typedef
struct|struct
name|itimerspec
block|{
comment|/* definition per POSIX.4 */
DECL|member|it_interval
name|struct
name|timespec
name|it_interval
decl_stmt|;
comment|/* timer period */
DECL|member|it_value
name|struct
name|timespec
name|it_value
decl_stmt|;
comment|/* timer expiration */
DECL|typedef|itimerspec_t
block|}
name|itimerspec_t
typedef|;
end_typedef

begin_endif
endif|#
directive|endif
end_endif

begin_function
DECL|function|port_create (void)
name|int
name|port_create
parameter_list|(
name|void
parameter_list|)
block|{
return|return
operator|-
literal|1
return|;
block|}
end_function

begin_function
DECL|function|port_associate (int port,int source,uintptr_t object,int events,void * user)
name|int
name|port_associate
parameter_list|(
name|int
name|port
parameter_list|,
name|int
name|source
parameter_list|,
name|uintptr_t
name|object
parameter_list|,
name|int
name|events
parameter_list|,
name|void
modifier|*
name|user
parameter_list|)
block|{
return|return
operator|-
literal|1
return|;
block|}
end_function

begin_function
DECL|function|port_dissociate (int port,int source,uintptr_t object)
name|int
name|port_dissociate
parameter_list|(
name|int
name|port
parameter_list|,
name|int
name|source
parameter_list|,
name|uintptr_t
name|object
parameter_list|)
block|{
return|return
operator|-
literal|1
return|;
block|}
end_function

begin_function
DECL|function|port_getn (int port,port_event_t list[],uint_t max,uint_t * nget,struct timespec * timeout)
name|int
name|port_getn
parameter_list|(
name|int
name|port
parameter_list|,
name|port_event_t
name|list
index|[]
parameter_list|,
name|uint_t
name|max
parameter_list|,
name|uint_t
modifier|*
name|nget
parameter_list|,
name|struct
name|timespec
modifier|*
name|timeout
parameter_list|)
block|{
return|return
operator|-
literal|1
return|;
block|}
end_function

begin_function
DECL|function|timer_create (clockid_t clock_id,struct sigevent * evp,timer_t * timerid)
name|int
name|timer_create
parameter_list|(
name|clockid_t
name|clock_id
parameter_list|,
name|struct
name|sigevent
modifier|*
name|evp
parameter_list|,
name|timer_t
modifier|*
name|timerid
parameter_list|)
block|{
return|return
operator|-
literal|1
return|;
block|}
end_function

begin_function
DECL|function|timer_settime (timer_t timerid,int flags,const struct itimerspec * value,struct itimerspec * ovalue)
name|int
name|timer_settime
parameter_list|(
name|timer_t
name|timerid
parameter_list|,
name|int
name|flags
parameter_list|,
specifier|const
name|struct
name|itimerspec
modifier|*
name|value
parameter_list|,
name|struct
name|itimerspec
modifier|*
name|ovalue
parameter_list|)
block|{
return|return
operator|-
literal|1
return|;
block|}
end_function

begin_function
DECL|function|timer_delete (timer_t timerid)
name|int
name|timer_delete
parameter_list|(
name|timer_t
name|timerid
parameter_list|)
block|{
return|return
operator|-
literal|1
return|;
block|}
end_function

begin_endif
endif|#
directive|endif
end_endif

begin_typedef
DECL|struct|__anon2bddae800208
typedef|typedef
struct|struct
block|{
DECL|member|events
name|ngx_uint_t
name|events
decl_stmt|;
DECL|typedef|ngx_eventport_conf_t
block|}
name|ngx_eventport_conf_t
typedef|;
end_typedef

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_eventport_init
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
name|ngx_eventport_done
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
name|ngx_eventport_add_event
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
name|ngx_eventport_del_event
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
name|ngx_eventport_process_events
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
name|void
modifier|*
name|ngx_eventport_create_conf
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
name|ngx_eventport_init_conf
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
DECL|variable|ep
specifier|static
name|int
name|ep
init|=
operator|-
literal|1
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|event_list
specifier|static
name|port_event_t
modifier|*
name|event_list
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|nevents
specifier|static
name|ngx_uint_t
name|nevents
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|event_timer
specifier|static
name|timer_t
name|event_timer
init|=
operator|(
name|timer_t
operator|)
operator|-
literal|1
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|eventport_name
specifier|static
name|ngx_str_t
name|eventport_name
init|=
name|ngx_string
argument_list|(
literal|"eventport"
argument_list|)
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_eventport_commands
specifier|static
name|ngx_command_t
name|ngx_eventport_commands
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"eventport_events"
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
name|ngx_eventport_conf_t
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
DECL|variable|ngx_eventport_module_ctx
name|ngx_event_module_t
name|ngx_eventport_module_ctx
init|=
block|{
operator|&
name|eventport_name
block|,
name|ngx_eventport_create_conf
block|,
comment|/* create configuration */
name|ngx_eventport_init_conf
block|,
comment|/* init configuration */
block|{
name|ngx_eventport_add_event
block|,
comment|/* add an event */
name|ngx_eventport_del_event
block|,
comment|/* delete an event */
name|ngx_eventport_add_event
block|,
comment|/* enable an event */
name|ngx_eventport_del_event
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
name|ngx_eventport_process_events
block|,
comment|/* process the events */
name|ngx_eventport_init
block|,
comment|/* init the events */
name|ngx_eventport_done
block|,
comment|/* done the events */
block|}
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_eventport_module
name|ngx_module_t
name|ngx_eventport_module
init|=
block|{
name|NGX_MODULE_V1
block|,
operator|&
name|ngx_eventport_module_ctx
block|,
comment|/* module context */
name|ngx_eventport_commands
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
DECL|function|ngx_eventport_init (ngx_cycle_t * cycle,ngx_msec_t timer)
name|ngx_eventport_init
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|,
name|ngx_msec_t
name|timer
parameter_list|)
block|{
name|port_notify_t
name|pn
decl_stmt|;
name|struct
name|itimerspec
name|its
decl_stmt|;
name|struct
name|sigevent
name|sev
decl_stmt|;
name|ngx_eventport_conf_t
modifier|*
name|epcf
decl_stmt|;
name|epcf
operator|=
name|ngx_event_get_conf
argument_list|(
name|cycle
operator|->
name|conf_ctx
argument_list|,
name|ngx_eventport_module
argument_list|)
expr_stmt|;
if|if
condition|(
name|ep
operator|==
operator|-
literal|1
condition|)
block|{
name|ep
operator|=
name|port_create
argument_list|()
expr_stmt|;
if|if
condition|(
name|ep
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
literal|"port_create() failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
block|}
if|if
condition|(
name|nevents
operator|<
name|epcf
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
name|event_list
operator|=
name|ngx_alloc
argument_list|(
sizeof|sizeof
argument_list|(
name|port_event_t
argument_list|)
operator|*
name|epcf
operator|->
name|events
argument_list|,
name|cycle
operator|->
name|log
argument_list|)
expr_stmt|;
if|if
condition|(
name|event_list
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
block|}
name|ngx_event_flags
operator|=
name|NGX_USE_EVENTPORT_EVENT
expr_stmt|;
if|if
condition|(
name|timer
condition|)
block|{
name|ngx_memzero
argument_list|(
operator|&
name|pn
argument_list|,
sizeof|sizeof
argument_list|(
name|port_notify_t
argument_list|)
argument_list|)
expr_stmt|;
name|pn
operator|.
name|portnfy_port
operator|=
name|ep
expr_stmt|;
name|ngx_memzero
argument_list|(
operator|&
name|sev
argument_list|,
sizeof|sizeof
argument_list|(
expr|struct
name|sigevent
argument_list|)
argument_list|)
expr_stmt|;
name|sev
operator|.
name|sigev_notify
operator|=
name|SIGEV_PORT
expr_stmt|;
if|#
directive|if
operator|!
operator|(
name|NGX_TEST_BUILD_EVENTPORT
operator|)
name|sev
operator|.
name|sigev_value
operator|.
name|sival_ptr
operator|=
operator|&
name|pn
expr_stmt|;
endif|#
directive|endif
if|if
condition|(
name|timer_create
argument_list|(
name|CLOCK_REALTIME
argument_list|,
operator|&
name|sev
argument_list|,
operator|&
name|event_timer
argument_list|)
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
literal|"timer_create() failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
name|its
operator|.
name|it_interval
operator|.
name|tv_sec
operator|=
name|timer
operator|/
literal|1000
expr_stmt|;
name|its
operator|.
name|it_interval
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
name|its
operator|.
name|it_value
operator|.
name|tv_sec
operator|=
name|timer
operator|/
literal|1000
expr_stmt|;
name|its
operator|.
name|it_value
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
if|if
condition|(
name|timer_settime
argument_list|(
name|event_timer
argument_list|,
literal|0
argument_list|,
operator|&
name|its
argument_list|,
name|NULL
argument_list|)
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
literal|"timer_settime() failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
name|ngx_event_flags
operator||=
name|NGX_USE_TIMER_EVENT
expr_stmt|;
block|}
name|nevents
operator|=
name|epcf
operator|->
name|events
expr_stmt|;
name|ngx_io
operator|=
name|ngx_os_io
expr_stmt|;
name|ngx_event_actions
operator|=
name|ngx_eventport_module_ctx
operator|.
name|actions
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
specifier|static
name|void
DECL|function|ngx_eventport_done (ngx_cycle_t * cycle)
name|ngx_eventport_done
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
block|{
if|if
condition|(
name|event_timer
operator|!=
operator|(
name|timer_t
operator|)
operator|-
literal|1
condition|)
block|{
if|if
condition|(
name|timer_delete
argument_list|(
name|event_timer
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
literal|"timer_delete() failed"
argument_list|)
expr_stmt|;
block|}
name|event_timer
operator|=
operator|(
name|timer_t
operator|)
operator|-
literal|1
expr_stmt|;
block|}
if|if
condition|(
name|close
argument_list|(
name|ep
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
literal|"close() event port failed"
argument_list|)
expr_stmt|;
block|}
name|ep
operator|=
operator|-
literal|1
expr_stmt|;
name|ngx_free
argument_list|(
name|event_list
argument_list|)
expr_stmt|;
name|event_list
operator|=
name|NULL
expr_stmt|;
name|nevents
operator|=
literal|0
expr_stmt|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_eventport_add_event (ngx_event_t * ev,ngx_int_t event,ngx_uint_t flags)
name|ngx_eventport_add_event
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
name|ngx_int_t
name|events
decl_stmt|,
name|prev
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
name|events
operator|=
name|event
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
name|prev
operator|=
name|POLLOUT
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_READ_EVENT
operator|!=
name|POLLIN
operator|)
name|events
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
name|prev
operator|=
name|POLLIN
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_WRITE_EVENT
operator|!=
name|POLLOUT
operator|)
name|events
operator|=
name|POLLOUT
expr_stmt|;
endif|#
directive|endif
block|}
if|if
condition|(
name|e
operator|->
name|oneshot
condition|)
block|{
name|events
operator||=
name|prev
expr_stmt|;
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
literal|"eventport add event: fd:%d ev:%04Xi"
argument_list|,
name|c
operator|->
name|fd
argument_list|,
name|events
argument_list|)
expr_stmt|;
if|if
condition|(
name|port_associate
argument_list|(
name|ep
argument_list|,
name|PORT_SOURCE_FD
argument_list|,
name|c
operator|->
name|fd
argument_list|,
name|events
argument_list|,
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
literal|"port_associate() failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
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
literal|1
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_eventport_del_event (ngx_event_t * ev,ngx_int_t event,ngx_uint_t flags)
name|ngx_eventport_del_event
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
comment|/*      * when the file descriptor is closed, the event port automatically      * dissociates it from the port, so we do not need to dissociate explicity      * the event before the closing the file descriptor      */
if|if
condition|(
name|flags
operator|&
name|NGX_CLOSE_EVENT
condition|)
block|{
name|ev
operator|->
name|active
operator|=
literal|0
expr_stmt|;
name|ev
operator|->
name|oneshot
operator|=
literal|0
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
name|c
operator|=
name|ev
operator|->
name|data
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
operator|->
name|oneshot
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
literal|"eventport change event: fd:%d ev:%04Xi"
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
name|port_associate
argument_list|(
name|ep
argument_list|,
name|PORT_SOURCE_FD
argument_list|,
name|c
operator|->
name|fd
argument_list|,
name|event
argument_list|,
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
literal|"port_associate() failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
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
literal|"eventport del event: fd:%d"
argument_list|,
name|c
operator|->
name|fd
argument_list|)
expr_stmt|;
if|if
condition|(
name|port_dissociate
argument_list|(
name|ep
argument_list|,
name|PORT_SOURCE_FD
argument_list|,
name|c
operator|->
name|fd
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
literal|"port_dissociate() failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
block|}
name|ev
operator|->
name|active
operator|=
literal|0
expr_stmt|;
name|ev
operator|->
name|oneshot
operator|=
literal|0
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
name|ngx_int_t
DECL|function|ngx_eventport_process_events (ngx_cycle_t * cycle,ngx_msec_t timer,ngx_uint_t flags)
name|ngx_eventport_process_events
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
name|n
decl_stmt|,
name|revents
decl_stmt|;
name|u_int
name|events
decl_stmt|;
name|ngx_err_t
name|err
decl_stmt|;
name|ngx_int_t
name|instance
decl_stmt|;
name|ngx_uint_t
name|i
decl_stmt|,
name|level
decl_stmt|;
name|ngx_event_t
modifier|*
name|ev
decl_stmt|,
modifier|*
name|rev
decl_stmt|,
modifier|*
name|wev
decl_stmt|,
modifier|*
modifier|*
name|queue
decl_stmt|;
name|ngx_connection_t
modifier|*
name|c
decl_stmt|;
name|struct
name|timespec
name|ts
decl_stmt|,
modifier|*
name|tp
decl_stmt|;
if|if
condition|(
name|timer
operator|==
name|NGX_TIMER_INFINITE
condition|)
block|{
name|tp
operator|=
name|NULL
expr_stmt|;
block|}
else|else
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
literal|"eventport timer: %M"
argument_list|,
name|timer
argument_list|)
expr_stmt|;
name|events
operator|=
literal|1
expr_stmt|;
name|n
operator|=
name|port_getn
argument_list|(
name|ep
argument_list|,
name|event_list
argument_list|,
operator|(
name|u_int
operator|)
name|nevents
argument_list|,
operator|&
name|events
argument_list|,
name|tp
argument_list|)
expr_stmt|;
name|err
operator|=
name|ngx_errno
expr_stmt|;
if|if
condition|(
name|flags
operator|&
name|NGX_UPDATE_TIME
condition|)
block|{
name|ngx_time_update
argument_list|()
expr_stmt|;
block|}
if|if
condition|(
name|n
operator|==
operator|-
literal|1
condition|)
block|{
if|if
condition|(
name|err
operator|==
name|ETIME
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
literal|"port_getn() returned no events without timeout"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
name|level
operator|=
operator|(
name|err
operator|==
name|NGX_EINTR
operator|)
condition|?
name|NGX_LOG_INFO
else|:
name|NGX_LOG_ALERT
expr_stmt|;
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
literal|"port_getn() failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|events
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
literal|"port_getn() returned no events without timeout"
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
if|if
condition|(
name|event_list
index|[
name|i
index|]
operator|.
name|portev_source
operator|==
name|PORT_SOURCE_TIMER
condition|)
block|{
name|ngx_time_update
argument_list|()
expr_stmt|;
continue|continue;
block|}
name|ev
operator|=
name|event_list
index|[
name|i
index|]
operator|.
name|portev_user
expr_stmt|;
switch|switch
condition|(
name|event_list
index|[
name|i
index|]
operator|.
name|portev_source
condition|)
block|{
case|case
name|PORT_SOURCE_FD
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
if|if
condition|(
name|ev
operator|->
name|closed
operator|||
name|ev
operator|->
name|instance
operator|!=
name|instance
condition|)
block|{
comment|/*                  * the stale event from a file descriptor                  * that was just closed in this iteration                  */
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
literal|"eventport: stale event %p"
argument_list|,
name|ev
argument_list|)
expr_stmt|;
continue|continue;
block|}
name|revents
operator|=
name|event_list
index|[
name|i
index|]
operator|.
name|portev_events
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
literal|"eventport: fd:%d, ev:%04Xd"
argument_list|,
name|event_list
index|[
name|i
index|]
operator|.
name|portev_object
argument_list|,
name|revents
argument_list|)
expr_stmt|;
if|if
condition|(
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
literal|"port_getn() error fd:%d ev:%04Xd"
argument_list|,
name|event_list
index|[
name|i
index|]
operator|.
name|portev_object
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
literal|"strange port_getn() events fd:%d ev:%04Xd"
argument_list|,
name|event_list
index|[
name|i
index|]
operator|.
name|portev_object
argument_list|,
name|revents
argument_list|)
expr_stmt|;
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
comment|/*                  * if the error events were returned without POLLIN or POLLOUT,                  * then add these flags to handle the events at least in one                  * active handler                  */
name|revents
operator||=
name|POLLIN
operator||
name|POLLOUT
expr_stmt|;
block|}
name|c
operator|=
name|ev
operator|->
name|data
expr_stmt|;
name|rev
operator|=
name|c
operator|->
name|read
expr_stmt|;
name|wev
operator|=
name|c
operator|->
name|write
expr_stmt|;
name|rev
operator|->
name|active
operator|=
literal|0
expr_stmt|;
name|wev
operator|->
name|active
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
if|if
condition|(
operator|(
name|flags
operator|&
name|NGX_POST_THREAD_EVENTS
operator|)
operator|&&
operator|!
name|rev
operator|->
name|accept
condition|)
block|{
name|rev
operator|->
name|posted_ready
operator|=
literal|1
expr_stmt|;
block|}
else|else
block|{
name|rev
operator|->
name|ready
operator|=
literal|1
expr_stmt|;
block|}
if|if
condition|(
name|flags
operator|&
name|NGX_POST_EVENTS
condition|)
block|{
name|queue
operator|=
operator|(
name|ngx_event_t
operator|*
operator|*
operator|)
operator|(
name|rev
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
name|rev
argument_list|,
name|queue
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|rev
operator|->
name|handler
argument_list|(
name|rev
argument_list|)
expr_stmt|;
if|if
condition|(
name|ev
operator|->
name|closed
condition|)
block|{
continue|continue;
block|}
block|}
if|if
condition|(
name|rev
operator|->
name|accept
condition|)
block|{
if|if
condition|(
name|ngx_use_accept_mutex
condition|)
block|{
name|ngx_accept_events
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|port_associate
argument_list|(
name|ep
argument_list|,
name|PORT_SOURCE_FD
argument_list|,
name|c
operator|->
name|fd
argument_list|,
name|POLLIN
argument_list|,
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
literal|"port_associate() failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
block|}
block|}
if|if
condition|(
name|revents
operator|&
name|POLLOUT
condition|)
block|{
if|if
condition|(
name|flags
operator|&
name|NGX_POST_THREAD_EVENTS
condition|)
block|{
name|wev
operator|->
name|posted_ready
operator|=
literal|1
expr_stmt|;
block|}
else|else
block|{
name|wev
operator|->
name|ready
operator|=
literal|1
expr_stmt|;
block|}
if|if
condition|(
name|flags
operator|&
name|NGX_POST_EVENTS
condition|)
block|{
name|ngx_locked_post_event
argument_list|(
name|wev
argument_list|,
operator|&
name|ngx_posted_events
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|wev
operator|->
name|handler
argument_list|(
name|wev
argument_list|)
expr_stmt|;
block|}
block|}
continue|continue;
default|default:
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
literal|"unexpected even_port object %d"
argument_list|,
name|event_list
index|[
name|i
index|]
operator|.
name|portev_object
argument_list|)
expr_stmt|;
continue|continue;
block|}
block|}
name|ngx_mutex_unlock
argument_list|(
name|ngx_posted_events_mutex
argument_list|)
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
specifier|static
name|void
modifier|*
DECL|function|ngx_eventport_create_conf (ngx_cycle_t * cycle)
name|ngx_eventport_create_conf
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
block|{
name|ngx_eventport_conf_t
modifier|*
name|epcf
decl_stmt|;
name|epcf
operator|=
name|ngx_palloc
argument_list|(
name|cycle
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_eventport_conf_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|epcf
operator|==
name|NULL
condition|)
block|{
return|return
name|NULL
return|;
block|}
name|epcf
operator|->
name|events
operator|=
name|NGX_CONF_UNSET
expr_stmt|;
return|return
name|epcf
return|;
block|}
end_function

begin_function
specifier|static
name|char
modifier|*
DECL|function|ngx_eventport_init_conf (ngx_cycle_t * cycle,void * conf)
name|ngx_eventport_init_conf
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
name|ngx_eventport_conf_t
modifier|*
name|epcf
init|=
name|conf
decl_stmt|;
name|ngx_conf_init_uint_value
argument_list|(
name|epcf
operator|->
name|events
argument_list|,
literal|32
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

end_unit

