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

begin_include
include|#
directive|include
file|<ngx_iocp_module.h>
end_include

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_iocp_init
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
name|ngx_iocp_done
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
name|ngx_iocp_add_event
parameter_list|(
name|ngx_event_t
modifier|*
name|ev
parameter_list|,
name|int
name|event
parameter_list|,
name|u_int
name|key
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_iocp_del_connection
parameter_list|(
name|ngx_connection_t
modifier|*
name|c
parameter_list|,
name|u_int
name|flags
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_iocp_process_events
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
modifier|*
name|ngx_iocp_create_conf
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
name|ngx_iocp_init_conf
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
DECL|variable|iocp_name
specifier|static
name|ngx_str_t
name|iocp_name
init|=
name|ngx_string
argument_list|(
literal|"iocp"
argument_list|)
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_iocp_commands
specifier|static
name|ngx_command_t
name|ngx_iocp_commands
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"iocp_threads"
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
name|ngx_iocp_conf_t
argument_list|,
name|threads
argument_list|)
block|,
name|NULL
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"post_acceptex"
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
name|ngx_iocp_conf_t
argument_list|,
name|post_acceptex
argument_list|)
block|,
name|NULL
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"acceptex_read"
argument_list|)
block|,
name|NGX_EVENT_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_conf_set_flag_slot
block|,
literal|0
block|,
name|offsetof
argument_list|(
name|ngx_iocp_conf_t
argument_list|,
name|acceptex_read
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
DECL|variable|ngx_iocp_module_ctx
name|ngx_event_module_t
name|ngx_iocp_module_ctx
init|=
block|{
operator|&
name|iocp_name
block|,
name|ngx_iocp_create_conf
block|,
comment|/* create configuration */
name|ngx_iocp_init_conf
block|,
comment|/* init configuration */
block|{
name|ngx_iocp_add_event
block|,
comment|/* add an event */
name|NULL
block|,
comment|/* delete an event */
name|NULL
block|,
comment|/* enable an event */
name|NULL
block|,
comment|/* disable an event */
name|NULL
block|,
comment|/* add an connection */
name|ngx_iocp_del_connection
block|,
comment|/* delete an connection */
name|NULL
block|,
comment|/* process the changes */
name|ngx_iocp_process_events
block|,
comment|/* process the events */
name|ngx_iocp_init
block|,
comment|/* init the events */
name|ngx_iocp_done
comment|/* done the events */
block|}
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_iocp_module
name|ngx_module_t
name|ngx_iocp_module
init|=
block|{
name|NGX_MODULE
block|,
operator|&
name|ngx_iocp_module_ctx
block|,
comment|/* module context */
name|ngx_iocp_commands
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

begin_decl_stmt
DECL|variable|ngx_iocp_io
name|ngx_os_io_t
name|ngx_iocp_io
init|=
block|{
name|ngx_overlapped_wsarecv
block|,
name|NULL
block|,
name|NULL
block|,
name|ngx_overlapped_wsasend_chain
block|,
literal|0
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|iocp
specifier|static
name|HANDLE
name|iocp
decl_stmt|;
end_decl_stmt

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_iocp_init (ngx_cycle_t * cycle)
name|ngx_iocp_init
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
block|{
name|ngx_iocp_conf_t
modifier|*
name|cf
decl_stmt|;
name|cf
operator|=
name|ngx_event_get_conf
argument_list|(
name|cycle
operator|->
name|conf_ctx
argument_list|,
name|ngx_iocp_module
argument_list|)
expr_stmt|;
if|if
condition|(
name|iocp
operator|==
name|NULL
condition|)
block|{
name|iocp
operator|=
name|CreateIoCompletionPort
argument_list|(
name|INVALID_HANDLE_VALUE
argument_list|,
name|NULL
argument_list|,
literal|0
argument_list|,
name|cf
operator|->
name|threads
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|iocp
operator|==
name|NULL
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
literal|"CreateIoCompletionPort() failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
name|ngx_io
operator|=
name|ngx_iocp_io
expr_stmt|;
name|ngx_event_actions
operator|=
name|ngx_iocp_module_ctx
operator|.
name|actions
expr_stmt|;
name|ngx_event_flags
operator|=
name|NGX_USE_AIO_EVENT
operator||
name|NGX_USE_IOCP_EVENT
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
specifier|static
name|void
DECL|function|ngx_iocp_done (ngx_cycle_t * cycle)
name|ngx_iocp_done
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
block|{
if|if
condition|(
name|CloseHandle
argument_list|(
name|iocp
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
literal|"iocp CloseHandle() failed"
argument_list|)
expr_stmt|;
block|}
name|iocp
operator|=
name|NULL
expr_stmt|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_iocp_add_event (ngx_event_t * ev,int event,u_int key)
name|ngx_iocp_add_event
parameter_list|(
name|ngx_event_t
modifier|*
name|ev
parameter_list|,
name|int
name|event
parameter_list|,
name|u_int
name|key
parameter_list|)
block|{
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
name|c
operator|->
name|read
operator|->
name|active
operator|=
literal|1
expr_stmt|;
name|c
operator|->
name|write
operator|->
name|active
operator|=
literal|1
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
literal|"iocp add: fd:%d k:%d ov:%p"
argument_list|,
name|c
operator|->
name|fd
argument_list|,
name|key
argument_list|,
operator|&
name|ev
operator|->
name|ovlp
argument_list|)
expr_stmt|;
if|if
condition|(
name|CreateIoCompletionPort
argument_list|(
operator|(
name|HANDLE
operator|)
name|c
operator|->
name|fd
argument_list|,
name|iocp
argument_list|,
name|key
argument_list|,
literal|0
argument_list|)
operator|==
name|NULL
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|c
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"CreateIoCompletionPort() failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_iocp_del_connection (ngx_connection_t * c,u_int flags)
name|ngx_iocp_del_connection
parameter_list|(
name|ngx_connection_t
modifier|*
name|c
parameter_list|,
name|u_int
name|flags
parameter_list|)
block|{
if|#
directive|if
literal|0
block_content|if (flags& NGX_CLOSE_EVENT) {         return NGX_OK;     }      if (CancelIo((HANDLE) c->fd) == 0) {         ngx_log_error(NGX_LOG_ALERT, c->log, ngx_errno, "CancelIo() failed");         return NGX_ERROR;     }
endif|#
directive|endif
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
specifier|static
DECL|function|ngx_iocp_process_events (ngx_cycle_t * cycle)
name|ngx_int_t
name|ngx_iocp_process_events
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
block|{
name|int
name|rc
decl_stmt|;
name|u_int
name|key
decl_stmt|;
name|u_long
name|bytes
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
name|struct
name|timeval
name|tv
decl_stmt|;
name|ngx_epoch_msec_t
name|delta
decl_stmt|;
name|ngx_event_ovlp_t
modifier|*
name|ovlp
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
name|INFINITE
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
literal|"iocp timer: %d"
argument_list|,
name|timer
argument_list|)
expr_stmt|;
name|rc
operator|=
name|GetQueuedCompletionStatus
argument_list|(
name|iocp
argument_list|,
operator|&
name|bytes
argument_list|,
operator|(
name|LPDWORD
operator|)
operator|&
name|key
argument_list|,
operator|(
name|LPOVERLAPPED
operator|*
operator|)
operator|&
name|ovlp
argument_list|,
name|timer
argument_list|)
expr_stmt|;
if|if
condition|(
name|rc
operator|==
literal|0
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
literal|"iocp: %d b:%d k:%d ov:%p"
argument_list|,
name|rc
argument_list|,
name|bytes
argument_list|,
name|key
argument_list|,
name|ovlp
argument_list|)
expr_stmt|;
name|delta
operator|=
name|ngx_elapsed_msec
expr_stmt|;
name|ngx_elapsed_msec
operator|=
operator|(
name|ngx_epoch_msec_t
operator|)
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
name|INFINITE
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
literal|"iocp timer: %d, delta: %d"
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
if|if
condition|(
name|err
condition|)
block|{
if|if
condition|(
name|ovlp
operator|==
name|NULL
condition|)
block|{
if|if
condition|(
name|err
operator|!=
name|WAIT_TIMEOUT
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
name|err
argument_list|,
literal|"GetQueuedCompletionStatus() failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|timer
operator|!=
name|INFINITE
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
name|ovlp
operator|->
name|error
operator|=
name|err
expr_stmt|;
block|}
if|if
condition|(
name|ovlp
operator|==
name|NULL
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
literal|"GetQueuedCompletionStatus() returned no operation"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
name|ev
operator|=
name|ovlp
operator|->
name|event
expr_stmt|;
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|cycle
operator|->
name|log
argument_list|,
name|err
argument_list|,
literal|"iocp event:%p"
argument_list|,
name|ev
argument_list|)
expr_stmt|;
if|if
condition|(
name|err
operator|==
name|ERROR_NETNAME_DELETED
comment|/* the socket was closed */
operator|||
name|err
operator|==
name|ERROR_OPERATION_ABORTED
comment|/* the operation was canceled */
condition|)
block|{
comment|/*          * the WSA_OPERATION_ABORTED completion notification          * for a file descriptor that was closed          */
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|cycle
operator|->
name|log
argument_list|,
name|err
argument_list|,
literal|"iocp: aborted event %p"
argument_list|,
name|ev
argument_list|)
expr_stmt|;
if|if
condition|(
name|timer
operator|!=
name|INFINITE
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
if|if
condition|(
name|err
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
name|err
argument_list|,
literal|"GetQueuedCompletionStatus() returned operation error"
argument_list|)
expr_stmt|;
block|}
switch|switch
condition|(
name|key
condition|)
block|{
case|case
name|NGX_IOCP_ACCEPT
case|:
if|if
condition|(
name|bytes
condition|)
block|{
name|ev
operator|->
name|ready
operator|=
literal|1
expr_stmt|;
block|}
break|break;
case|case
name|NGX_IOCP_IO
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
break|break;
case|case
name|NGX_IOCP_CONNECT
case|:
name|ev
operator|->
name|ready
operator|=
literal|1
expr_stmt|;
block|}
name|ev
operator|->
name|available
operator|=
name|bytes
expr_stmt|;
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
literal|"iocp event handler: %p"
argument_list|,
name|ev
operator|->
name|event_handler
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
name|timer
operator|!=
name|INFINITE
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
specifier|static
name|void
modifier|*
DECL|function|ngx_iocp_create_conf (ngx_cycle_t * cycle)
name|ngx_iocp_create_conf
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
block|{
name|ngx_iocp_conf_t
modifier|*
name|cf
decl_stmt|;
name|cf
operator|=
name|ngx_palloc
argument_list|(
name|cycle
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_iocp_conf_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|cf
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|cf
operator|->
name|threads
operator|=
name|NGX_CONF_UNSET
expr_stmt|;
name|cf
operator|->
name|post_acceptex
operator|=
name|NGX_CONF_UNSET
expr_stmt|;
name|cf
operator|->
name|acceptex_read
operator|=
name|NGX_CONF_UNSET
expr_stmt|;
return|return
name|cf
return|;
block|}
end_function

begin_function
specifier|static
name|char
modifier|*
DECL|function|ngx_iocp_init_conf (ngx_cycle_t * cycle,void * conf)
name|ngx_iocp_init_conf
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
name|ngx_iocp_conf_t
modifier|*
name|cf
init|=
name|conf
decl_stmt|;
name|ngx_conf_init_value
argument_list|(
name|cf
operator|->
name|threads
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|ngx_conf_init_value
argument_list|(
name|cf
operator|->
name|post_acceptex
argument_list|,
literal|10
argument_list|)
expr_stmt|;
name|ngx_conf_init_value
argument_list|(
name|cf
operator|->
name|acceptex_read
argument_list|,
literal|1
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

end_unit

