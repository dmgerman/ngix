begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (C) 2002 Igor Sysoev, http://sysoev.ru  */
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
file|<ngx_kqueue_module.h>
end_include

begin_if
if|#
directive|if
operator|(
name|USE_KQUEUE
operator|)
operator|&&
operator|!
operator|(
name|HAVE_KQUEUE
operator|)
end_if

begin_error
error|#
directive|error
literal|"kqueue is not supported on this platform"
end_error

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* should be per-thread */
end_comment

begin_decl_stmt
DECL|variable|kq
specifier|static
name|int
name|kq
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
DECL|variable|nchanges
DECL|variable|nevents
specifier|static
name|int
name|nchanges
decl_stmt|,
name|nevents
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|timer_queue
specifier|static
name|ngx_event_t
name|timer_queue
decl_stmt|;
end_decl_stmt

begin_comment
comment|/* */
end_comment

begin_function
DECL|function|ngx_kqueue_init (int max_connections,ngx_log_t * log)
name|int
name|ngx_kqueue_init
parameter_list|(
name|int
name|max_connections
parameter_list|,
name|ngx_log_t
modifier|*
name|log
parameter_list|)
block|{
name|int
name|size
decl_stmt|;
name|size
operator|=
sizeof|sizeof
argument_list|(
expr|struct
name|kevent
argument_list|)
operator|*
literal|512
expr_stmt|;
name|nchanges
operator|=
literal|0
expr_stmt|;
name|nevents
operator|=
literal|512
expr_stmt|;
name|kq
operator|=
name|kqueue
argument_list|()
expr_stmt|;
if|if
condition|(
name|kq
operator|==
operator|-
literal|1
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_EMERG
argument_list|,
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
name|ngx_test_null
argument_list|(
name|change_list
argument_list|,
name|ngx_alloc
argument_list|(
name|size
argument_list|,
name|log
argument_list|)
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
name|ngx_test_null
argument_list|(
name|event_list
argument_list|,
name|ngx_alloc
argument_list|(
name|size
argument_list|,
name|log
argument_list|)
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
name|timer_queue
operator|.
name|timer_prev
operator|=
operator|&
name|timer_queue
expr_stmt|;
name|timer_queue
operator|.
name|timer_next
operator|=
operator|&
name|timer_queue
expr_stmt|;
if|#
directive|if
operator|!
operator|(
name|USE_KQUEUE
operator|)
name|ngx_event_actions
operator|.
name|add
operator|=
name|ngx_kqueue_add_event
expr_stmt|;
name|ngx_event_actions
operator|.
name|del
operator|=
name|ngx_kqueue_del_event
expr_stmt|;
name|ngx_event_actions
operator|.
name|timer
operator|=
name|ngx_kqueue_add_timer
expr_stmt|;
name|ngx_event_actions
operator|.
name|process
operator|=
name|ngx_kqueue_process_events
expr_stmt|;
endif|#
directive|endif
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_kqueue_add_event (ngx_event_t * ev,int event,u_int flags)
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
if|#
directive|if
literal|1
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
name|nchanges
operator|&&
name|change_list
index|[
name|ev
operator|->
name|index
index|]
operator|.
name|udata
operator|==
name|ev
condition|)
block|{
comment|/* DEBUG */
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
literal|"kqueue add event: %d: ft:%d"
argument|_ c->fd _ event
argument_list|)
empty_stmt|;
name|change_list
index|[
name|ev
operator|->
name|index
index|]
operator|.
name|filter
operator|=
name|event
expr_stmt|;
name|change_list
index|[
name|ev
operator|->
name|index
index|]
operator|.
name|flags
operator|=
name|flags
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
endif|#
directive|endif
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
name|nchanges
operator|&&
name|change_list
index|[
name|ev
operator|->
name|index
index|]
operator|.
name|udata
operator|==
name|ev
condition|)
block|{
comment|/* DEBUG */
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
literal|"kqueue del event: %d: ft:%d"
argument|_ c->fd _ event
argument_list|)
empty_stmt|;
if|if
condition|(
name|ev
operator|->
name|index
operator|<
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
if|if
condition|(
name|flags
operator|&
name|NGX_CLOSE_EVENT
condition|)
return|return
name|NGX_OK
return|;
return|return
name|ngx_kqueue_set_event
argument_list|(
name|ev
argument_list|,
name|event
argument_list|,
name|EV_DELETE
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|ngx_kqueue_set_event (ngx_event_t * ev,int filter,u_int flags)
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
literal|"kqueue set event: %d: ft:%d f:%08x"
argument|_                   c->fd _ filter _ flags
argument_list|)
empty_stmt|;
if|if
condition|(
name|nchanges
operator|>=
name|nevents
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
name|kq
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
literal|"kevent failed"
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
name|change_list
index|[
name|nchanges
index|]
operator|.
name|udata
operator|=
name|ev
expr_stmt|;
if|#
directive|if
literal|0
block_content|if (flags == EV_ADD)
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
name|i
decl_stmt|;
name|u_int
name|timer
decl_stmt|,
name|delta
decl_stmt|;
name|ngx_event_t
modifier|*
name|ev
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
literal|0
expr_stmt|;
name|delta
operator|=
literal|0
expr_stmt|;
name|tp
operator|=
name|NULL
expr_stmt|;
block|}
name|ngx_log_debug
argument_list|(
argument|log
argument_list|,
literal|"kevent timer: %d"
argument|_ timer
argument_list|)
empty_stmt|;
name|events
operator|=
name|kevent
argument_list|(
name|kq
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
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"kevent failed"
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
operator|-
name|delta
expr_stmt|;
block|}
else|else
block|{
name|ngx_assert
argument_list|(
argument|(events !=
literal|0
argument|)
argument_list|,
argument|return NGX_ERROR
argument_list|,
argument|log
argument_list|,
literal|"kevent returns no events without timeout"
argument_list|)
empty_stmt|;
block|}
name|ngx_log_debug
argument_list|(
argument|log
argument_list|,
literal|"kevent timer: %d, delta: %d"
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
init|;
condition|;
control|)
block|{
name|ev
operator|=
name|timer_queue
operator|.
name|timer_next
expr_stmt|;
if|if
condition|(
name|ev
operator|==
operator|&
name|timer_queue
operator|||
name|delta
operator|<
name|ev
operator|->
name|timer_delta
condition|)
break|break;
name|delta
operator|-=
name|ev
operator|->
name|timer_delta
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
name|NGX_ERROR
condition|)
name|ev
operator|->
name|close_handler
argument_list|(
name|ev
argument_list|)
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
name|ngx_log_debug
argument_list|(
argument|log
argument_list|,
literal|"kevent: %d: ft:%d f:%08x ff:%08x d:%d ud:%08x"
argument|_                       event_list[i].ident _ event_list[i].filter _                       event_list[i].flags _ event_list[i].fflags _                       event_list[i].data _ event_list[i].udata
argument_list|)
empty_stmt|;
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
literal|"kevent error on %d"
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
name|ev
operator|->
name|ready
operator|=
literal|1
expr_stmt|;
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
name|eof
operator|=
literal|1
expr_stmt|;
name|ev
operator|->
name|error
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
condition|)
name|ngx_del_timer
argument_list|(
name|ev
argument_list|)
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
name|NGX_ERROR
condition|)
name|ev
operator|->
name|close_handler
argument_list|(
name|ev
argument_list|)
expr_stmt|;
break|break;
default|default:
name|ngx_assert
argument_list|(
literal|0
argument_list|,
comment|/* void */
argument_list|,
argument|log
argument_list|,
literal|"unknown kevent filter %d"
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
DECL|function|ngx_kqueue_add_timer (ngx_event_t * ev,ngx_msec_t timer)
name|void
name|ngx_kqueue_add_timer
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
name|ngx_log_debug
argument_list|(
argument|ev->log
argument_list|,
literal|"set timer: %d"
argument|_ timer
argument_list|)
empty_stmt|;
name|ngx_assert
argument_list|(
argument|(!ev->timer_next&& !ev->timer_prev)
argument_list|,
argument|return
argument_list|,
argument|ev->log
argument_list|,
literal|"timer already set"
argument_list|)
empty_stmt|;
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

