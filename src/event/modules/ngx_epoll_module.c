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

begin_if
if|#
directive|if
operator|(
name|TEST_BUILD_EPOLL
operator|)
end_if

begin_comment
comment|/* epoll declarations */
end_comment

begin_define
DECL|macro|EPOLLIN
define|#
directive|define
name|EPOLLIN
value|0x001
end_define

begin_define
DECL|macro|EPOLLPRI
define|#
directive|define
name|EPOLLPRI
value|0x002
end_define

begin_define
DECL|macro|EPOLLOUT
define|#
directive|define
name|EPOLLOUT
value|0x004
end_define

begin_define
DECL|macro|EPOLLRDNORM
define|#
directive|define
name|EPOLLRDNORM
value|0x040
end_define

begin_define
DECL|macro|EPOLLRDBAND
define|#
directive|define
name|EPOLLRDBAND
value|0x080
end_define

begin_define
DECL|macro|EPOLLWRNORM
define|#
directive|define
name|EPOLLWRNORM
value|0x100
end_define

begin_define
DECL|macro|EPOLLWRBAND
define|#
directive|define
name|EPOLLWRBAND
value|0x200
end_define

begin_define
DECL|macro|EPOLLMSG
define|#
directive|define
name|EPOLLMSG
value|0x400
end_define

begin_define
DECL|macro|EPOLLERR
define|#
directive|define
name|EPOLLERR
value|0x008
end_define

begin_define
DECL|macro|EPOLLHUP
define|#
directive|define
name|EPOLLHUP
value|0x010
end_define

begin_define
DECL|macro|EPOLLET
define|#
directive|define
name|EPOLLET
value|0x80000000
end_define

begin_define
DECL|macro|EPOLLONESHOT
define|#
directive|define
name|EPOLLONESHOT
value|0x40000000
end_define

begin_define
DECL|macro|EPOLL_CTL_ADD
define|#
directive|define
name|EPOLL_CTL_ADD
value|1
end_define

begin_define
DECL|macro|EPOLL_CTL_DEL
define|#
directive|define
name|EPOLL_CTL_DEL
value|2
end_define

begin_define
DECL|macro|EPOLL_CTL_MOD
define|#
directive|define
name|EPOLL_CTL_MOD
value|3
end_define

begin_typedef
DECL|union|epoll_data
typedef|typedef
union|union
name|epoll_data
block|{
DECL|member|ptr
name|void
modifier|*
name|ptr
decl_stmt|;
DECL|member|fd
name|int
name|fd
decl_stmt|;
DECL|member|u32
name|uint32_t
name|u32
decl_stmt|;
DECL|member|u64
name|uint64_t
name|u64
decl_stmt|;
DECL|typedef|epoll_data_t
block|}
name|epoll_data_t
typedef|;
end_typedef

begin_struct
DECL|struct|epoll_event
struct|struct
name|epoll_event
block|{
DECL|member|events
name|uint32_t
name|events
decl_stmt|;
DECL|member|data
name|epoll_data_t
name|data
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
name|int
name|epoll_create
parameter_list|(
name|int
name|size
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|epoll_ctl
parameter_list|(
name|int
name|epfd
parameter_list|,
name|int
name|op
parameter_list|,
name|int
name|fd
parameter_list|,
name|struct
name|epoll_event
modifier|*
name|event
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|epoll_wait
parameter_list|(
name|int
name|epfd
parameter_list|,
name|struct
name|epoll_event
modifier|*
name|events
parameter_list|,
name|int
name|nevents
parameter_list|,
name|int
name|timeout
parameter_list|)
function_decl|;
end_function_decl

begin_function
DECL|function|epoll_create (int size)
name|int
name|epoll_create
parameter_list|(
name|int
name|size
parameter_list|)
block|{
return|return
operator|-
literal|1
return|;
block|}
end_function

begin_function
DECL|function|epoll_ctl (int epfd,int op,int fd,struct epoll_event * event)
name|int
name|epoll_ctl
parameter_list|(
name|int
name|epfd
parameter_list|,
name|int
name|op
parameter_list|,
name|int
name|fd
parameter_list|,
name|struct
name|epoll_event
modifier|*
name|event
parameter_list|)
block|{
return|return
operator|-
literal|1
return|;
block|}
end_function

begin_function
DECL|function|epoll_wait (int epfd,struct epoll_event * events,int nevents,int timeout)
name|int
name|epoll_wait
parameter_list|(
name|int
name|epfd
parameter_list|,
name|struct
name|epoll_event
modifier|*
name|events
parameter_list|,
name|int
name|nevents
parameter_list|,
name|int
name|timeout
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
DECL|struct|__anon2aa231d50108
typedef|typedef
struct|struct
block|{
DECL|member|events
name|u_int
name|events
decl_stmt|;
DECL|typedef|ngx_epoll_conf_t
block|}
name|ngx_epoll_conf_t
typedef|;
end_typedef

begin_function_decl
specifier|static
name|int
name|ngx_epoll_init
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
name|ngx_epoll_done
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
name|ngx_epoll_add_event
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
name|ngx_epoll_del_event
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
name|ngx_epoll_add_connection
parameter_list|(
name|ngx_connection_t
modifier|*
name|c
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|int
name|ngx_epoll_del_connection
parameter_list|(
name|ngx_connection_t
modifier|*
name|c
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|int
name|ngx_epoll_process_events
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
name|ngx_epoll_create_conf
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
name|ngx_epoll_init_conf
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
name|struct
name|epoll_event
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
DECL|variable|epoll_name
specifier|static
name|ngx_str_t
name|epoll_name
init|=
name|ngx_string
argument_list|(
literal|"epoll"
argument_list|)
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_epoll_commands
specifier|static
name|ngx_command_t
name|ngx_epoll_commands
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"epoll_events"
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
name|ngx_epoll_conf_t
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
DECL|variable|ngx_epoll_module_ctx
name|ngx_event_module_t
name|ngx_epoll_module_ctx
init|=
block|{
operator|&
name|epoll_name
block|,
name|ngx_epoll_create_conf
block|,
comment|/* create configuration */
name|ngx_epoll_init_conf
block|,
comment|/* init configuration */
block|{
name|ngx_epoll_add_event
block|,
comment|/* add an event */
name|ngx_epoll_del_event
block|,
comment|/* delete an event */
name|ngx_epoll_add_event
block|,
comment|/* enable an event */
name|ngx_epoll_del_event
block|,
comment|/* disable an event */
name|NULL
block|,
comment|/* add an connection */
name|NULL
block|,
comment|/* delete an connection */
name|ngx_epoll_process_events
block|,
comment|/* process the events */
name|ngx_epoll_init
block|,
comment|/* init the events */
name|ngx_epoll_done
block|,
comment|/* done the events */
block|}
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_epoll_module
name|ngx_module_t
name|ngx_epoll_module
init|=
block|{
name|NGX_MODULE
block|,
operator|&
name|ngx_epoll_module_ctx
block|,
comment|/* module context */
name|ngx_epoll_commands
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
DECL|function|ngx_epoll_init (ngx_cycle_t * cycle)
specifier|static
name|int
name|ngx_epoll_init
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
block|{
name|size_t
name|n
decl_stmt|;
name|ngx_event_conf_t
modifier|*
name|ecf
decl_stmt|;
name|ngx_epoll_conf_t
modifier|*
name|epcf
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
name|epcf
operator|=
name|ngx_event_get_conf
argument_list|(
name|cycle
operator|->
name|conf_ctx
argument_list|,
name|ngx_epoll_module
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
name|epoll_create
argument_list|(
name|ecf
operator|->
name|connections
operator|/
literal|2
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
literal|"epoll_create() failed"
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
expr|struct
name|epoll_event
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
name|ngx_epoll_module_ctx
operator|.
name|actions
expr_stmt|;
if|#
directive|if
operator|(
name|HAVE_CLEAR_EVENT
operator|)
name|ngx_event_flags
operator|=
name|NGX_USE_CLEAR_EVENT
expr_stmt|;
else|#
directive|else
name|ngx_event_flags
operator|=
name|NGX_USE_LEVEL_EVENT
expr_stmt|;
endif|#
directive|endif
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_epoll_done (ngx_cycle_t * cycle)
specifier|static
name|void
name|ngx_epoll_done
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
literal|"epoll close() failed"
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
DECL|function|ngx_epoll_add_event (ngx_event_t * ev,int event,u_int flags)
specifier|static
name|int
name|ngx_epoll_add_event
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
name|op
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
name|struct
name|epoll_event
name|ee
decl_stmt|;
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
name|prev
operator|=
name|EPOLLOUT
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_READ_EVENT
operator|!=
name|EPOLLIN
operator|)
name|event
operator|=
name|EPOLLIN
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
name|EPOLLIN
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_WRITE_EVENT
operator|!=
name|EPOLLOUT
operator|)
name|event
operator|=
name|EPOLLOUT
expr_stmt|;
endif|#
directive|endif
block|}
if|if
condition|(
name|e
operator|->
name|active
condition|)
block|{
name|op
operator|=
name|EPOLL_CTL_MOD
expr_stmt|;
name|event
operator||=
name|prev
expr_stmt|;
block|}
else|else
block|{
name|op
operator|=
name|EPOLL_CTL_ADD
expr_stmt|;
block|}
name|ee
operator|.
name|events
operator|=
name|event
operator||
name|flags
expr_stmt|;
name|ee
operator|.
name|data
operator|.
name|ptr
operator|=
operator|(
name|void
operator|*
operator|)
operator|(
operator|(
name|uintptr_t
operator|)
name|c
operator||
name|ev
operator|->
name|instance
operator|)
expr_stmt|;
name|ngx_log_debug3
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|ev
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"epoll add event: fd:%d op:%d ev:%08X"
argument_list|,
name|c
operator|->
name|fd
argument_list|,
name|op
argument_list|,
name|ee
operator|.
name|events
argument_list|)
expr_stmt|;
if|if
condition|(
name|epoll_ctl
argument_list|(
name|ep
argument_list|,
name|op
argument_list|,
name|c
operator|->
name|fd
argument_list|,
operator|&
name|ee
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
literal|"epoll_ctl(%d, %d) failed"
argument_list|,
name|op
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
name|ev
operator|->
name|active
operator|=
literal|1
expr_stmt|;
if|#
directive|if
literal|0
block_content|ev->oneshot = (flags& NGX_ONESHOT_EVENT) ? 1 : 0;
endif|#
directive|endif
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_epoll_del_event (ngx_event_t * ev,int event,u_int flags)
specifier|static
name|int
name|ngx_epoll_del_event
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
name|op
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
name|struct
name|epoll_event
name|ee
decl_stmt|;
comment|/*      * when the file descriptor is closed the epoll automatically deletes      * it from its queue so we do not need to delete explicity the event      * before the closing the file descriptor.      */
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
name|prev
operator|=
name|EPOLLOUT
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
name|prev
operator|=
name|EPOLLIN
expr_stmt|;
block|}
if|if
condition|(
name|e
operator|->
name|active
condition|)
block|{
name|op
operator|=
name|EPOLL_CTL_MOD
expr_stmt|;
name|ee
operator|.
name|events
operator|=
name|prev
operator||
name|flags
expr_stmt|;
name|ee
operator|.
name|data
operator|.
name|ptr
operator|=
operator|(
name|void
operator|*
operator|)
operator|(
operator|(
name|uintptr_t
operator|)
name|c
operator||
name|ev
operator|->
name|instance
operator|)
expr_stmt|;
block|}
else|else
block|{
name|op
operator|=
name|EPOLL_CTL_DEL
expr_stmt|;
name|ee
operator|.
name|events
operator|=
literal|0
expr_stmt|;
name|ee
operator|.
name|data
operator|.
name|ptr
operator|=
name|NULL
expr_stmt|;
block|}
name|ngx_log_debug3
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|ev
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"epoll del event: fd:%d op:%d ev:%08X"
argument_list|,
name|c
operator|->
name|fd
argument_list|,
name|op
argument_list|,
name|ee
operator|.
name|events
argument_list|)
expr_stmt|;
if|if
condition|(
name|epoll_ctl
argument_list|(
name|ep
argument_list|,
name|op
argument_list|,
name|c
operator|->
name|fd
argument_list|,
operator|&
name|ee
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
literal|"epoll_ctl(%d, %d) failed"
argument_list|,
name|op
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
name|ev
operator|->
name|active
operator|=
literal|0
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_if
if|#
directive|if
literal|0
end_if

begin_endif
unit|static int ngx_epoll_add_connection(ngx_connection_t *c) {     struct epoll_event  ee;      ee.events = EPOLLIN|EPOLLOUT|EPOLLET;     ee.data.ptr = (void *) ((uintptr_t) c | c->read->instance);      ngx_log_debug2(NGX_LOG_DEBUG_EVENT, c->log, 0,                    "epoll add connection: fd:%d ev:%08X", c->fd, ee.events);      if (epoll_ctl(ep, EPOLL_CTL_ADD, c->fd,&ee) == -1) {         ngx_log_error(NGX_LOG_ALERT, c->log, ngx_errno,                       "epoll_ctl(EPOLL_CTL_ADD, %d) failed", c->fd);         return NGX_ERROR;     }      c->read->active = 1;     c->write->active = 1;      return NGX_OK; }   static int ngx_epoll_del_connection(ngx_connection_t *c) {     c->read->active = 0;     c->write->active = 0;      return NGX_OK; }
endif|#
directive|endif
end_endif

begin_function
DECL|function|ngx_epoll_process_events (ngx_log_t * log)
name|int
name|ngx_epoll_process_events
parameter_list|(
name|ngx_log_t
modifier|*
name|log
parameter_list|)
block|{
name|int
name|events
decl_stmt|;
name|ngx_int_t
name|instance
decl_stmt|,
name|i
decl_stmt|;
name|size_t
name|n
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
name|cycle
decl_stmt|;
name|struct
name|timeval
name|tv
decl_stmt|;
name|ngx_connection_t
modifier|*
name|c
decl_stmt|;
name|ngx_epoch_msec_t
name|delta
decl_stmt|;
name|timer
operator|=
name|ngx_event_find_timer
argument_list|()
expr_stmt|;
name|ngx_old_elapsed_msec
operator|=
name|ngx_elapsed_msec
expr_stmt|;
if|if
condition|(
name|timer
operator|==
literal|0
condition|)
block|{
name|timer
operator|=
operator|(
name|ngx_msec_t
operator|)
operator|-
literal|1
expr_stmt|;
block|}
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"epoll timer: %d"
argument_list|,
name|timer
argument_list|)
expr_stmt|;
name|events
operator|=
name|epoll_wait
argument_list|(
name|ep
argument_list|,
name|event_list
argument_list|,
name|nevents
argument_list|,
name|timer
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
if|if
condition|(
name|timer
operator|!=
operator|(
name|ngx_msec_t
operator|)
operator|-
literal|1
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
name|log
argument_list|,
literal|0
argument_list|,
literal|"epoll timer: %d, delta: %d"
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
literal|"epoll_wait() returned no events without timeout"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
block|}
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
name|log
argument_list|,
name|err
argument_list|,
literal|"epoll_wait() failed"
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
name|event_list
index|[
name|i
index|]
operator|.
name|data
operator|.
name|ptr
expr_stmt|;
name|instance
operator|=
operator|(
name|uintptr_t
operator|)
name|c
operator|&
literal|1
expr_stmt|;
name|c
operator|=
operator|(
name|ngx_connection_t
operator|*
operator|)
operator|(
operator|(
name|uintptr_t
operator|)
name|c
operator|&
operator|(
name|uintptr_t
operator|)
operator|~
literal|1
operator|)
expr_stmt|;
name|ngx_log_debug3
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"epoll: fd:%d ev:%04X d:"
name|PTR_FMT
argument_list|,
name|c
operator|->
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
name|data
argument_list|)
expr_stmt|;
if|if
condition|(
name|c
operator|->
name|read
operator|->
name|instance
operator|!=
name|instance
condition|)
block|{
comment|/*              * the stale event from a file descriptor              * that was just closed in this iteration              */
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"epoll: stale event "
name|PTR_FMT
argument_list|,
name|c
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
name|events
operator|&
operator|(
name|EPOLLERR
operator||
name|EPOLLHUP
operator|)
condition|)
block|{
name|ngx_log_debug2
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"epoll_wait() error on fd:%d ev:%04X"
argument_list|,
name|c
operator|->
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
if|if
condition|(
name|event_list
index|[
name|i
index|]
operator|.
name|events
operator|&
operator|~
operator|(
name|EPOLLIN
operator||
name|EPOLLOUT
operator||
name|EPOLLERR
operator||
name|EPOLLHUP
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
literal|"strange epoll_wait() events fd:%d ev:%04X"
argument_list|,
name|c
operator|->
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
if|if
condition|(
operator|(
name|event_list
index|[
name|i
index|]
operator|.
name|events
operator|&
operator|(
name|EPOLLIN
operator||
name|EPOLLERR
operator||
name|EPOLLHUP
operator|)
operator|)
operator|&&
name|c
operator|->
name|read
operator|->
name|active
condition|)
block|{
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
operator|(
name|event_list
index|[
name|i
index|]
operator|.
name|events
operator|&
operator|(
name|EPOLLOUT
operator||
name|EPOLLERR
operator||
name|EPOLLHUP
operator|)
operator|)
operator|&&
name|c
operator|->
name|write
operator|->
name|active
condition|)
block|{
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
block|}
if|if
condition|(
name|timer
operator|!=
operator|(
name|ngx_msec_t
operator|)
operator|-
literal|1
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
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_epoll_create_conf (ngx_cycle_t * cycle)
specifier|static
name|void
modifier|*
name|ngx_epoll_create_conf
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
block|{
name|ngx_epoll_conf_t
modifier|*
name|epcf
decl_stmt|;
name|ngx_test_null
argument_list|(
name|epcf
argument_list|,
name|ngx_palloc
argument_list|(
name|cycle
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_epoll_conf_t
argument_list|)
argument_list|)
argument_list|,
name|NGX_CONF_ERROR
argument_list|)
expr_stmt|;
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
DECL|function|ngx_epoll_init_conf (ngx_cycle_t * cycle,void * conf)
specifier|static
name|char
modifier|*
name|ngx_epoll_init_conf
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
name|ngx_epoll_conf_t
modifier|*
name|epcf
init|=
name|conf
decl_stmt|;
name|ngx_conf_init_unsigned_value
argument_list|(
name|epcf
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

