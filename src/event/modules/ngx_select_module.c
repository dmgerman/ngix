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
name|ngx_select_init
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
name|ngx_select_done
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
name|ngx_select_add_event
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
name|ngx_select_del_event
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
name|ngx_select_process_events
parameter_list|(
name|ngx_log_t
modifier|*
name|log
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|char
modifier|*
name|ngx_select_init_conf
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
DECL|variable|master_read_fd_set
specifier|static
name|fd_set
name|master_read_fd_set
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|master_write_fd_set
specifier|static
name|fd_set
name|master_write_fd_set
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|work_read_fd_set
specifier|static
name|fd_set
name|work_read_fd_set
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|work_write_fd_set
specifier|static
name|fd_set
name|work_write_fd_set
decl_stmt|;
end_decl_stmt

begin_if
if|#
directive|if
operator|(
name|WIN32
operator|)
end_if

begin_decl_stmt
DECL|variable|max_read
specifier|static
name|int
name|max_read
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|max_write
specifier|static
name|int
name|max_write
decl_stmt|;
end_decl_stmt

begin_else
else|#
directive|else
end_else

begin_decl_stmt
DECL|variable|max_fd
specifier|static
name|int
name|max_fd
decl_stmt|;
end_decl_stmt

begin_endif
endif|#
directive|endif
end_endif

begin_decl_stmt
DECL|variable|nevents
specifier|static
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
DECL|variable|select_name
specifier|static
name|ngx_str_t
name|select_name
init|=
name|ngx_string
argument_list|(
literal|"select"
argument_list|)
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_select_module_ctx
name|ngx_event_module_t
name|ngx_select_module_ctx
init|=
block|{
operator|&
name|select_name
block|,
name|NULL
block|,
comment|/* create configuration */
name|ngx_select_init_conf
block|,
comment|/* init configuration */
block|{
name|ngx_select_add_event
block|,
comment|/* add an event */
name|ngx_select_del_event
block|,
comment|/* delete an event */
name|ngx_select_add_event
block|,
comment|/* enable an event */
name|ngx_select_del_event
block|,
comment|/* disable an event */
name|NULL
block|,
comment|/* add an connection */
name|NULL
block|,
comment|/* delete an connection */
name|ngx_select_process_events
block|,
comment|/* process the events */
name|ngx_select_init
block|,
comment|/* init the events */
name|ngx_select_done
comment|/* done the events */
block|}
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_select_module
name|ngx_module_t
name|ngx_select_module
init|=
block|{
name|NGX_MODULE
block|,
operator|&
name|ngx_select_module_ctx
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
DECL|function|ngx_select_init (ngx_cycle_t * cycle)
specifier|static
name|int
name|ngx_select_init
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
block|{
name|ngx_event_t
modifier|*
modifier|*
name|index
decl_stmt|;
if|if
condition|(
name|event_index
operator|==
name|NULL
condition|)
block|{
name|FD_ZERO
argument_list|(
operator|&
name|master_read_fd_set
argument_list|)
expr_stmt|;
name|FD_ZERO
argument_list|(
operator|&
name|master_write_fd_set
argument_list|)
expr_stmt|;
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
name|ngx_io
operator|=
name|ngx_os_io
expr_stmt|;
name|ngx_event_actions
operator|=
name|ngx_select_module_ctx
operator|.
name|actions
expr_stmt|;
name|ngx_event_flags
operator|=
name|NGX_USE_LEVEL_EVENT
operator||
name|NGX_USE_ONESHOT_EVENT
expr_stmt|;
if|#
directive|if
operator|(
name|WIN32
operator|)
name|max_read
operator|=
name|max_write
operator|=
literal|0
expr_stmt|;
else|#
directive|else
name|max_fd
operator|=
operator|-
literal|1
expr_stmt|;
endif|#
directive|endif
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_select_done (ngx_cycle_t * cycle)
specifier|static
name|void
name|ngx_select_done
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
block|{
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
name|event_index
operator|=
name|NULL
expr_stmt|;
block|}
end_function

begin_function
DECL|function|ngx_select_add_event (ngx_event_t * ev,int event,u_int flags)
specifier|static
name|int
name|ngx_select_add_event
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
name|c
operator|=
name|ev
operator|->
name|data
expr_stmt|;
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
literal|"select add event fd:%d ev:%d"
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
literal|"select event fd:%d ev:%d is already set"
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
if|#
directive|if
operator|(
name|WIN32
operator|)
if|if
condition|(
operator|(
name|event
operator|==
name|NGX_READ_EVENT
operator|)
operator|&&
operator|(
name|max_read
operator|>=
name|FD_SETSIZE
operator|)
operator|||
operator|(
name|event
operator|==
name|NGX_WRITE_EVENT
operator|)
operator|&&
operator|(
name|max_write
operator|>=
name|FD_SETSIZE
operator|)
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ERR
argument_list|,
name|ev
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"maximum number of descriptors "
literal|"supported by select() is %d"
argument_list|,
name|FD_SETSIZE
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|event
operator|==
name|NGX_READ_EVENT
condition|)
block|{
name|FD_SET
argument_list|(
name|c
operator|->
name|fd
argument_list|,
operator|&
name|master_read_fd_set
argument_list|)
expr_stmt|;
name|max_read
operator|++
expr_stmt|;
block|}
if|else if
condition|(
name|event
operator|==
name|NGX_WRITE_EVENT
condition|)
block|{
name|FD_SET
argument_list|(
name|c
operator|->
name|fd
argument_list|,
operator|&
name|master_write_fd_set
argument_list|)
expr_stmt|;
name|max_write
operator|++
expr_stmt|;
block|}
else|#
directive|else
if|if
condition|(
name|event
operator|==
name|NGX_READ_EVENT
condition|)
block|{
name|FD_SET
argument_list|(
name|c
operator|->
name|fd
argument_list|,
operator|&
name|master_read_fd_set
argument_list|)
expr_stmt|;
block|}
if|else if
condition|(
name|event
operator|==
name|NGX_WRITE_EVENT
condition|)
block|{
name|FD_SET
argument_list|(
name|c
operator|->
name|fd
argument_list|,
operator|&
name|master_write_fd_set
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|max_fd
operator|!=
operator|-
literal|1
operator|&&
name|max_fd
operator|<
name|c
operator|->
name|fd
condition|)
block|{
name|max_fd
operator|=
name|c
operator|->
name|fd
expr_stmt|;
block|}
endif|#
directive|endif
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
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_select_del_event (ngx_event_t * ev,int event,u_int flags)
specifier|static
name|int
name|ngx_select_del_event
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
name|c
operator|=
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
literal|"select del event fd:%d ev:%d"
argument_list|,
name|c
operator|->
name|fd
argument_list|,
name|event
argument_list|)
expr_stmt|;
if|#
directive|if
operator|(
name|WIN32
operator|)
if|if
condition|(
name|event
operator|==
name|NGX_READ_EVENT
condition|)
block|{
name|FD_CLR
argument_list|(
name|c
operator|->
name|fd
argument_list|,
operator|&
name|master_read_fd_set
argument_list|)
expr_stmt|;
name|max_read
operator|--
expr_stmt|;
block|}
if|else if
condition|(
name|event
operator|==
name|NGX_WRITE_EVENT
condition|)
block|{
name|FD_CLR
argument_list|(
name|c
operator|->
name|fd
argument_list|,
operator|&
name|master_write_fd_set
argument_list|)
expr_stmt|;
name|max_write
operator|--
expr_stmt|;
block|}
else|#
directive|else
if|if
condition|(
name|event
operator|==
name|NGX_READ_EVENT
condition|)
block|{
name|FD_CLR
argument_list|(
name|c
operator|->
name|fd
argument_list|,
operator|&
name|master_read_fd_set
argument_list|)
expr_stmt|;
block|}
if|else if
condition|(
name|event
operator|==
name|NGX_WRITE_EVENT
condition|)
block|{
name|FD_CLR
argument_list|(
name|c
operator|->
name|fd
argument_list|,
operator|&
name|master_write_fd_set
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|max_fd
operator|==
name|c
operator|->
name|fd
condition|)
block|{
name|max_fd
operator|=
operator|-
literal|1
expr_stmt|;
block|}
endif|#
directive|endif
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
name|nevents
condition|)
block|{
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
DECL|function|ngx_select_process_events (ngx_log_t * log)
specifier|static
name|int
name|ngx_select_process_events
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
name|ngx_connection_t
modifier|*
name|c
decl_stmt|;
name|ngx_epoch_msec_t
name|delta
decl_stmt|;
name|struct
name|timeval
name|tv
decl_stmt|,
modifier|*
name|tp
decl_stmt|;
if|#
directive|if
operator|(
name|HAVE_SELECT_CHANGE_TIMEOUT
operator|)
specifier|static
name|ngx_epoch_msec_t
name|deltas
init|=
literal|0
decl_stmt|;
endif|#
directive|endif
name|work_read_fd_set
operator|=
name|master_read_fd_set
expr_stmt|;
name|work_write_fd_set
operator|=
name|master_write_fd_set
expr_stmt|;
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
condition|)
block|{
name|tv
operator|.
name|tv_sec
operator|=
name|timer
operator|/
literal|1000
expr_stmt|;
name|tv
operator|.
name|tv_usec
operator|=
operator|(
name|timer
operator|%
literal|1000
operator|)
operator|*
literal|1000
expr_stmt|;
name|tp
operator|=
operator|&
name|tv
expr_stmt|;
block|}
else|else
block|{
name|tp
operator|=
name|NULL
expr_stmt|;
block|}
if|#
directive|if
operator|!
operator|(
name|WIN32
operator|)
if|if
condition|(
name|max_fd
operator|==
operator|-
literal|1
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
name|c
operator|=
name|event_index
index|[
name|i
index|]
operator|->
name|data
expr_stmt|;
if|if
condition|(
name|max_fd
operator|<
name|c
operator|->
name|fd
condition|)
block|{
name|max_fd
operator|=
name|c
operator|->
name|fd
expr_stmt|;
block|}
block|}
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"change max_fd: %d"
argument_list|,
name|max_fd
argument_list|)
expr_stmt|;
block|}
endif|#
directive|endif
if|#
directive|if
operator|(
name|NGX_DEBUG
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
name|ev
operator|=
name|event_index
index|[
name|i
index|]
expr_stmt|;
name|c
operator|=
name|ev
operator|->
name|data
expr_stmt|;
name|ngx_log_debug2
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"select event: fd:%d wr:%d"
argument_list|,
name|c
operator|->
name|fd
argument_list|,
name|ev
operator|->
name|write
argument_list|)
expr_stmt|;
block|}
endif|#
directive|endif
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"select timer: %d"
argument_list|,
name|timer
argument_list|)
expr_stmt|;
if|#
directive|if
operator|(
name|WIN32
operator|)
name|ready
operator|=
name|select
argument_list|(
literal|0
argument_list|,
operator|&
name|work_read_fd_set
argument_list|,
operator|&
name|work_write_fd_set
argument_list|,
name|NULL
argument_list|,
name|tp
argument_list|)
expr_stmt|;
else|#
directive|else
name|ready
operator|=
name|select
argument_list|(
name|max_fd
operator|+
literal|1
argument_list|,
operator|&
name|work_read_fd_set
argument_list|,
operator|&
name|work_write_fd_set
argument_list|,
name|NULL
argument_list|,
name|tp
argument_list|)
expr_stmt|;
endif|#
directive|endif
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
name|ngx_socket_errno
expr_stmt|;
block|}
else|else
block|{
name|err
operator|=
literal|0
expr_stmt|;
block|}
if|#
directive|if
operator|(
name|HAVE_SELECT_CHANGE_TIMEOUT
operator|)
if|if
condition|(
name|timer
condition|)
block|{
name|delta
operator|=
name|timer
operator|-
operator|(
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
operator|)
expr_stmt|;
comment|/*          * learn the real time and update the cached time          * if the sum of the last deltas overcomes 1 second          */
name|deltas
operator|+=
name|delta
expr_stmt|;
if|if
condition|(
name|deltas
operator|>
literal|1000
condition|)
block|{
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
name|deltas
operator|=
name|tv
operator|.
name|tv_usec
operator|/
literal|1000
expr_stmt|;
block|}
name|ngx_log_debug2
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"select timer: %d, delta: %d"
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
name|delta
operator|=
literal|0
expr_stmt|;
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
literal|"select() returned no events without timeout"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
block|}
else|#
directive|else
comment|/* !(HAVE_SELECT_CHANGE_TIMEOUT) */
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
literal|"select timer: %d, delta: %d"
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
name|log
argument_list|,
literal|0
argument_list|,
literal|"select() returned no events without timeout"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
block|}
endif|#
directive|endif
comment|/* HAVE_SELECT_CHANGE_TIMEOUT */
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"select ready %d"
argument_list|,
name|ready
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
name|log
argument_list|,
name|err
argument_list|,
literal|"select() failed"
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
condition|;
name|i
operator|++
control|)
block|{
name|ev
operator|=
name|event_index
index|[
name|i
index|]
expr_stmt|;
name|c
operator|=
name|ev
operator|->
name|data
expr_stmt|;
name|found
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|ev
operator|->
name|write
condition|)
block|{
if|if
condition|(
name|FD_ISSET
argument_list|(
name|c
operator|->
name|fd
argument_list|,
operator|&
name|work_write_fd_set
argument_list|)
condition|)
block|{
name|found
operator|=
literal|1
expr_stmt|;
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"select write %d"
argument_list|,
name|c
operator|->
name|fd
argument_list|)
expr_stmt|;
block|}
block|}
else|else
block|{
if|if
condition|(
name|FD_ISSET
argument_list|(
name|c
operator|->
name|fd
argument_list|,
operator|&
name|work_read_fd_set
argument_list|)
condition|)
block|{
name|found
operator|=
literal|1
expr_stmt|;
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"select read %d"
argument_list|,
name|c
operator|->
name|fd
argument_list|)
expr_stmt|;
block|}
block|}
if|if
condition|(
name|found
condition|)
block|{
name|ready_index
index|[
name|nready
operator|++
index|]
operator|=
name|ev
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
name|ready
operator|--
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
block|}
if|if
condition|(
name|ev
operator|->
name|write
condition|)
block|{
name|ngx_select_del_event
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
name|ngx_select_del_event
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
literal|"select ready != events"
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|timer
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
DECL|function|ngx_select_init_conf (ngx_cycle_t * cycle,void * conf)
specifier|static
name|char
modifier|*
name|ngx_select_init_conf
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
comment|/* disable warning: the default FD_SETSIZE is 1024U in FreeBSD 5.x */
if|if
condition|(
operator|(
name|unsigned
operator|)
name|ecf
operator|->
name|connections
operator|>
name|FD_SETSIZE
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
literal|0
argument_list|,
literal|"the maximum number of files "
literal|"supported by select() is "
name|ngx_value
argument_list|(
name|FD_SETSIZE
argument_list|)
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

end_unit

