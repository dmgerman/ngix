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
file|<ngx_select_module.h>
end_include

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
DECL|variable|timer_queue
specifier|static
name|ngx_event_t
name|timer_queue
decl_stmt|;
end_decl_stmt

begin_function_decl
specifier|static
name|fd_set
modifier|*
name|ngx_select_get_fd_set
parameter_list|(
name|ngx_socket_t
name|fd
parameter_list|,
name|int
name|event
parameter_list|,
name|ngx_log_t
modifier|*
name|log
parameter_list|)
function_decl|;
end_function_decl

begin_function
DECL|function|ngx_select_init (int max_connections,ngx_log_t * log)
name|int
name|ngx_select_init
parameter_list|(
name|int
name|max_connections
parameter_list|,
name|ngx_log_t
modifier|*
name|log
parameter_list|)
block|{
if|if
condition|(
name|max_connections
operator|>
name|FD_SETSIZE
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_EMERG
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
if|#
directive|if
operator|(
name|WIN32
operator|)
literal|"maximum number of descriptors "
literal|"supported by select() is %d"
argument_list|,
name|FD_SETSIZE
argument_list|)
expr_stmt|;
else|#
directive|else
literal|"maximum descriptor number"
literal|"supported by select() is %d"
operator|,
name|FD_SETSIZE
operator|-
literal|1
block_content|)
empty_stmt|;
endif|#
directive|endif
name|exit
argument_list|(
literal|1
argument_list|)
expr_stmt|;
block|}
end_function

begin_expr_stmt
name|FD_ZERO
argument_list|(
operator|&
name|master_read_fd_set
argument_list|)
expr_stmt|;
end_expr_stmt

begin_expr_stmt
name|FD_ZERO
argument_list|(
operator|&
name|master_write_fd_set
argument_list|)
expr_stmt|;
end_expr_stmt

begin_expr_stmt
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
end_expr_stmt

begin_expr_stmt
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
end_expr_stmt

begin_expr_stmt
name|nevents
operator|=
literal|0
expr_stmt|;
end_expr_stmt

begin_expr_stmt
name|timer_queue
operator|.
name|timer_prev
operator|=
operator|&
name|timer_queue
expr_stmt|;
end_expr_stmt

begin_expr_stmt
name|timer_queue
operator|.
name|timer_next
operator|=
operator|&
name|timer_queue
expr_stmt|;
end_expr_stmt

begin_expr_stmt
name|ngx_event_actions
operator|.
name|add
operator|=
name|ngx_select_add_event
expr_stmt|;
end_expr_stmt

begin_expr_stmt
name|ngx_event_actions
operator|.
name|del
operator|=
name|ngx_select_del_event
expr_stmt|;
end_expr_stmt

begin_expr_stmt
name|ngx_event_actions
operator|.
name|timer
operator|=
name|ngx_select_add_timer
expr_stmt|;
end_expr_stmt

begin_expr_stmt
name|ngx_event_actions
operator|.
name|process
operator|=
name|ngx_select_process_events
expr_stmt|;
end_expr_stmt

begin_if
if|#
directive|if
operator|(
name|WIN32
operator|)
end_if

begin_expr_stmt
name|max_read
operator|=
name|max_write
operator|=
literal|0
expr_stmt|;
end_expr_stmt

begin_else
else|#
directive|else
end_else

begin_expr_stmt
name|max_fd
operator|=
operator|-
literal|1
expr_stmt|;
end_expr_stmt

begin_endif
endif|#
directive|endif
end_endif

begin_return
return|return
name|NGX_OK
return|;
end_return

begin_macro
unit|}  int
DECL|function|ngx_select_add_event (ngx_event_t * ev,int event,u_int flags)
name|ngx_select_add_event
argument_list|(
argument|ngx_event_t *ev
argument_list|,
argument|int event
argument_list|,
argument|u_int flags
argument_list|)
end_macro

begin_block
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
name|ngx_log_debug
argument_list|(
argument|ev->log
argument_list|,
literal|"select fd:%d event:%d"
argument|_ c->fd _ event
argument_list|)
empty_stmt|;
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
if|else if
condition|(
name|event
operator|==
name|NGX_WRITE_EVENT
condition|)
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
name|max_fd
operator|=
name|c
operator|->
name|fd
expr_stmt|;
endif|#
directive|endif
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
end_block

begin_function
DECL|function|ngx_select_del_event (ngx_event_t * ev,int event)
name|int
name|ngx_select_del_event
parameter_list|(
name|ngx_event_t
modifier|*
name|ev
parameter_list|,
name|int
name|event
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
if|else if
condition|(
name|event
operator|==
name|NGX_WRITE_EVENT
condition|)
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
if|if
condition|(
name|max_fd
operator|==
name|c
operator|->
name|fd
condition|)
name|max_fd
operator|=
operator|-
literal|1
expr_stmt|;
endif|#
directive|endif
if|if
condition|(
name|ev
operator|->
name|index
operator|<
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
name|nevents
operator|--
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_select_process_events (ngx_log_t * log)
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
name|found
decl_stmt|,
name|nready
decl_stmt|;
name|u_int
name|timer
decl_stmt|,
name|delta
decl_stmt|;
name|ngx_event_t
modifier|*
name|ev
decl_stmt|,
modifier|*
name|nx
decl_stmt|;
name|ngx_connection_t
modifier|*
name|c
decl_stmt|;
name|struct
name|timeval
name|tv
decl_stmt|,
modifier|*
name|tp
decl_stmt|;
name|work_read_fd_set
operator|=
name|master_read_fd_set
expr_stmt|;
name|work_write_fd_set
operator|=
name|master_write_fd_set
expr_stmt|;
if|if
condition|(
name|timer_queue
operator|.
name|timer_next
operator|!=
operator|&
name|timer_queue
condition|)
block|{
name|timer
operator|=
name|timer_queue
operator|.
name|timer_next
operator|->
name|timer_delta
expr_stmt|;
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
literal|0
expr_stmt|;
name|tp
operator|=
name|NULL
expr_stmt|;
name|delta
operator|=
literal|0
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
operator|(
name|ngx_connection_t
operator|*
operator|)
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
name|max_fd
operator|=
name|c
operator|->
name|fd
expr_stmt|;
block|}
name|ngx_log_debug
argument_list|(
argument|log
argument_list|,
literal|"change max_fd: %d"
argument|_ max_fd
argument_list|)
empty_stmt|;
block|}
endif|#
directive|endif
name|ngx_log_debug
argument_list|(
argument|log
argument_list|,
literal|"select timer: %d"
argument|_ timer
argument_list|)
empty_stmt|;
if|#
directive|if
operator|(
name|WIN32
operator|)
if|if
condition|(
operator|(
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
operator|)
else|#
directive|else
if|if
condition|(
operator|(
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
operator|)
endif|#
directive|endif
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
name|ngx_socket_errno
argument_list|,
literal|"select() failed"
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
literal|"select ready %d"
argument|_ ready
argument_list|)
empty_stmt|;
if|if
condition|(
name|timer
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
name|ngx_assert
argument_list|(
argument|(ready !=
literal|0
argument|)
argument_list|,
argument|return NGX_ERROR
argument_list|,
argument|log
argument_list|,
literal|"select() returns no events without timeout"
argument_list|)
empty_stmt|;
block|}
name|ngx_log_debug
argument_list|(
argument|log
argument_list|,
literal|"select timer: %d, delta: %d"
argument|_ timer _ delta
argument_list|)
empty_stmt|;
if|if
condition|(
name|timer
condition|)
block|{
if|if
condition|(
name|delta
operator|>=
name|timer
condition|)
block|{
for|for
control|(
name|ev
operator|=
name|timer_queue
operator|.
name|timer_next
init|;
name|ev
operator|!=
operator|&
name|timer_queue
operator|&&
name|delta
operator|>=
name|ev
operator|->
name|timer_delta
condition|;
comment|/* void */
control|)
block|{
name|delta
operator|-=
name|ev
operator|->
name|timer_delta
expr_stmt|;
name|nx
operator|=
name|ev
operator|->
name|timer_next
expr_stmt|;
name|ngx_del_timer
argument_list|(
name|ev
argument_list|)
expr_stmt|;
name|ev
operator|->
name|timedout
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|ev
operator|->
name|event_handler
argument_list|(
name|ev
argument_list|)
operator|==
operator|-
literal|1
condition|)
name|ev
operator|->
name|close_handler
argument_list|(
name|ev
argument_list|)
expr_stmt|;
name|ev
operator|=
name|nx
expr_stmt|;
block|}
block|}
else|else
block|{
name|timer_queue
operator|.
name|timer_next
operator|->
name|timer_delta
operator|-=
name|delta
expr_stmt|;
block|}
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
operator|(
name|ngx_connection_t
operator|*
operator|)
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
name|ngx_log_debug
argument_list|(
argument|log
argument_list|,
literal|"select write %d"
argument|_                               c->fd
argument_list|)
empty_stmt|;
name|found
operator|=
literal|1
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
name|ngx_log_debug
argument_list|(
argument|log
argument_list|,
literal|"select read %d"
argument|_                               c->fd
argument_list|)
empty_stmt|;
name|found
operator|=
literal|1
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
name|ngx_select_del_event
argument_list|(
name|ev
argument_list|,
name|NGX_WRITE_EVENT
argument_list|)
expr_stmt|;
else|else
name|ngx_select_del_event
argument_list|(
name|ev
argument_list|,
name|NGX_READ_EVENT
argument_list|)
expr_stmt|;
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
operator|-
literal|1
condition|)
name|ev
operator|->
name|close_handler
argument_list|(
name|ev
argument_list|)
expr_stmt|;
name|ready
operator|--
expr_stmt|;
block|}
name|ngx_assert
argument_list|(
argument|(ready ==
literal|0
argument|)
argument_list|,
comment|/* void */
argument|;
argument_list|,
argument|log
argument_list|,
literal|"select ready != events"
argument_list|)
empty_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_select_add_timer (ngx_event_t * ev,ngx_msec_t timer)
name|void
name|ngx_select_add_timer
parameter_list|(
name|ngx_event_t
modifier|*
name|ev
parameter_list|,
name|ngx_msec_t
name|timer
parameter_list|)
block|{
name|ngx_event_t
modifier|*
name|e
decl_stmt|;
for|for
control|(
name|e
operator|=
name|timer_queue
operator|.
name|timer_next
init|;
name|e
operator|!=
operator|&
name|timer_queue
operator|&&
name|timer
operator|>
name|e
operator|->
name|timer_delta
condition|;
name|e
operator|=
name|e
operator|->
name|timer_next
control|)
name|timer
operator|-=
name|e
operator|->
name|timer_delta
expr_stmt|;
name|ev
operator|->
name|timer_delta
operator|=
name|timer
expr_stmt|;
name|ev
operator|->
name|timer_next
operator|=
name|e
expr_stmt|;
name|ev
operator|->
name|timer_prev
operator|=
name|e
operator|->
name|timer_prev
expr_stmt|;
name|e
operator|->
name|timer_prev
operator|->
name|timer_next
operator|=
name|ev
expr_stmt|;
name|e
operator|->
name|timer_prev
operator|=
name|ev
expr_stmt|;
block|}
end_function

end_unit

