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
file|<ngx_http.h>
end_include

begin_function_decl
specifier|static
name|int
name|ngx_http_busy_lock_look_cachable
parameter_list|(
name|ngx_http_busy_lock_t
modifier|*
name|bl
parameter_list|,
name|ngx_http_busy_lock_ctx_t
modifier|*
name|bc
parameter_list|,
name|int
name|lock
parameter_list|)
function_decl|;
end_function_decl

begin_function
DECL|function|ngx_http_busy_lock (ngx_http_busy_lock_t * bl,ngx_http_busy_lock_ctx_t * bc)
name|int
name|ngx_http_busy_lock
parameter_list|(
name|ngx_http_busy_lock_t
modifier|*
name|bl
parameter_list|,
name|ngx_http_busy_lock_ctx_t
modifier|*
name|bc
parameter_list|)
block|{
if|if
condition|(
name|bl
operator|->
name|busy
operator|<
name|bl
operator|->
name|max_busy
condition|)
block|{
name|bl
operator|->
name|busy
operator|++
expr_stmt|;
if|if
condition|(
name|bc
operator|->
name|time
condition|)
block|{
name|bc
operator|->
name|time
operator|=
literal|0
expr_stmt|;
name|bl
operator|->
name|waiting
operator|--
expr_stmt|;
block|}
return|return
name|NGX_OK
return|;
block|}
if|if
condition|(
name|bc
operator|->
name|time
condition|)
block|{
if|if
condition|(
name|bc
operator|->
name|time
operator|<
name|bl
operator|->
name|timeout
condition|)
block|{
name|ngx_add_timer
argument_list|(
name|bc
operator|->
name|event
argument_list|,
literal|1000
argument_list|)
expr_stmt|;
return|return
name|NGX_AGAIN
return|;
block|}
name|bl
operator|->
name|waiting
operator|--
expr_stmt|;
return|return
name|NGX_DONE
return|;
block|}
if|if
condition|(
name|bl
operator|->
name|timeout
operator|==
literal|0
condition|)
block|{
return|return
name|NGX_DONE
return|;
block|}
if|if
condition|(
name|bl
operator|->
name|waiting
operator|<
name|bl
operator|->
name|max_waiting
condition|)
block|{
name|bl
operator|->
name|waiting
operator|++
expr_stmt|;
name|ngx_add_timer
argument_list|(
name|bc
operator|->
name|event
argument_list|,
literal|1000
argument_list|)
expr_stmt|;
name|bc
operator|->
name|event
operator|->
name|event_handler
operator|=
name|bc
operator|->
name|event_handler
expr_stmt|;
comment|/* TODO: ngx_handle_level_read_event() */
return|return
name|NGX_AGAIN
return|;
block|}
return|return
name|NGX_ERROR
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_busy_lock_cachable (ngx_http_busy_lock_t * bl,ngx_http_busy_lock_ctx_t * bc,int lock)
name|int
name|ngx_http_busy_lock_cachable
parameter_list|(
name|ngx_http_busy_lock_t
modifier|*
name|bl
parameter_list|,
name|ngx_http_busy_lock_ctx_t
modifier|*
name|bc
parameter_list|,
name|int
name|lock
parameter_list|)
block|{
name|int
name|rc
decl_stmt|;
name|rc
operator|=
name|ngx_http_busy_lock_look_cachable
argument_list|(
name|bl
argument_list|,
name|bc
argument_list|,
name|lock
argument_list|)
expr_stmt|;
name|ngx_log_debug3
argument_list|(
name|NGX_LOG_DEBUG_HTTP
argument_list|,
name|bc
operator|->
name|event
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"http busylock: %d w:%d mw::%d"
argument_list|,
name|rc
argument_list|,
name|bl
operator|->
name|waiting
argument_list|,
name|bl
operator|->
name|max_waiting
argument_list|)
expr_stmt|;
if|if
condition|(
name|rc
operator|==
name|NGX_OK
condition|)
block|{
comment|/* no the same request, there's free slot */
return|return
name|NGX_OK
return|;
block|}
if|if
condition|(
name|rc
operator|==
name|NGX_ERROR
operator|&&
operator|!
name|lock
condition|)
block|{
comment|/* no the same request, no free slot */
return|return
name|NGX_OK
return|;
block|}
comment|/* rc == NGX_AGAIN:  the same request */
if|if
condition|(
name|bc
operator|->
name|time
condition|)
block|{
if|if
condition|(
name|bc
operator|->
name|time
operator|<
name|bl
operator|->
name|timeout
condition|)
block|{
name|ngx_add_timer
argument_list|(
name|bc
operator|->
name|event
argument_list|,
literal|1000
argument_list|)
expr_stmt|;
return|return
name|NGX_AGAIN
return|;
block|}
name|bl
operator|->
name|waiting
operator|--
expr_stmt|;
return|return
name|NGX_DONE
return|;
block|}
if|if
condition|(
name|bl
operator|->
name|timeout
operator|==
literal|0
condition|)
block|{
return|return
name|NGX_DONE
return|;
block|}
if|if
condition|(
name|bl
operator|->
name|waiting
operator|<
name|bl
operator|->
name|max_waiting
condition|)
block|{
name|bl
operator|->
name|waiting
operator|++
expr_stmt|;
name|ngx_add_timer
argument_list|(
name|bc
operator|->
name|event
argument_list|,
literal|1000
argument_list|)
expr_stmt|;
name|bc
operator|->
name|event
operator|->
name|event_handler
operator|=
name|bc
operator|->
name|event_handler
expr_stmt|;
comment|/* TODO: ngx_handle_level_read_event() */
return|return
name|NGX_AGAIN
return|;
block|}
return|return
name|NGX_ERROR
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_busy_unlock (ngx_http_busy_lock_t * bl,ngx_http_busy_lock_ctx_t * bc)
name|void
name|ngx_http_busy_unlock
parameter_list|(
name|ngx_http_busy_lock_t
modifier|*
name|bl
parameter_list|,
name|ngx_http_busy_lock_ctx_t
modifier|*
name|bc
parameter_list|)
block|{
if|if
condition|(
name|bl
operator|==
name|NULL
condition|)
block|{
return|return;
block|}
if|if
condition|(
name|bl
operator|->
name|md5
condition|)
block|{
name|bl
operator|->
name|md5_mask
index|[
name|bc
operator|->
name|slot
operator|/
literal|8
index|]
operator|&=
operator|~
operator|(
literal|1
operator|<<
operator|(
name|bc
operator|->
name|slot
operator|&
literal|7
operator|)
operator|)
expr_stmt|;
name|bl
operator|->
name|cachable
operator|--
expr_stmt|;
block|}
name|bl
operator|->
name|busy
operator|--
expr_stmt|;
block|}
end_function

begin_function
DECL|function|ngx_http_busy_lock_look_cachable (ngx_http_busy_lock_t * bl,ngx_http_busy_lock_ctx_t * bc,int lock)
specifier|static
name|int
name|ngx_http_busy_lock_look_cachable
parameter_list|(
name|ngx_http_busy_lock_t
modifier|*
name|bl
parameter_list|,
name|ngx_http_busy_lock_ctx_t
modifier|*
name|bc
parameter_list|,
name|int
name|lock
parameter_list|)
block|{
name|int
name|i
decl_stmt|,
name|b
decl_stmt|,
name|cachable
decl_stmt|,
name|free
decl_stmt|;
name|u_int
name|mask
decl_stmt|;
name|b
operator|=
literal|0
expr_stmt|;
name|cachable
operator|=
literal|0
expr_stmt|;
name|free
operator|=
operator|-
literal|1
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_SUPPRESS_WARN
operator|)
name|mask
operator|=
literal|0
expr_stmt|;
endif|#
directive|endif
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|bl
operator|->
name|max_busy
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
operator|(
name|b
operator|&
literal|7
operator|)
operator|==
literal|0
condition|)
block|{
name|mask
operator|=
name|bl
operator|->
name|md5_mask
index|[
name|i
operator|/
literal|8
index|]
expr_stmt|;
block|}
if|if
condition|(
name|mask
operator|&
literal|1
condition|)
block|{
if|if
condition|(
name|ngx_memcmp
argument_list|(
operator|&
name|bl
operator|->
name|md5
index|[
name|i
operator|*
literal|16
index|]
argument_list|,
name|bc
operator|->
name|md5
argument_list|,
literal|16
argument_list|)
operator|==
literal|0
condition|)
block|{
return|return
name|NGX_AGAIN
return|;
block|}
name|cachable
operator|++
expr_stmt|;
block|}
if|else if
condition|(
name|free
operator|==
operator|-
literal|1
condition|)
block|{
name|free
operator|=
name|i
expr_stmt|;
block|}
if|#
directive|if
literal|1
if|if
condition|(
name|cachable
operator|==
name|bl
operator|->
name|cachable
condition|)
block|{
if|if
condition|(
name|free
operator|==
operator|-
literal|1
operator|&&
name|cachable
operator|<
name|bl
operator|->
name|max_busy
condition|)
block|{
name|free
operator|=
name|i
operator|+
literal|1
expr_stmt|;
block|}
break|break;
block|}
endif|#
directive|endif
name|mask
operator|>>=
literal|1
expr_stmt|;
name|b
operator|++
expr_stmt|;
block|}
if|if
condition|(
name|free
operator|==
operator|-
literal|1
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|lock
condition|)
block|{
if|if
condition|(
name|bl
operator|->
name|busy
operator|==
name|bl
operator|->
name|max_busy
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|ngx_memcpy
argument_list|(
operator|&
name|bl
operator|->
name|md5
index|[
name|free
operator|*
literal|16
index|]
argument_list|,
name|bc
operator|->
name|md5
argument_list|,
literal|16
argument_list|)
expr_stmt|;
name|bl
operator|->
name|md5_mask
index|[
name|free
operator|/
literal|8
index|]
operator||=
literal|1
operator|<<
operator|(
name|free
operator|&
literal|7
operator|)
expr_stmt|;
name|bc
operator|->
name|slot
operator|=
name|free
expr_stmt|;
name|bl
operator|->
name|cachable
operator|++
expr_stmt|;
name|bl
operator|->
name|busy
operator|++
expr_stmt|;
block|}
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_set_busy_lock_slot (ngx_conf_t * cf,ngx_command_t * cmd,void * conf)
name|char
modifier|*
name|ngx_http_set_busy_lock_slot
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|ngx_command_t
modifier|*
name|cmd
parameter_list|,
name|void
modifier|*
name|conf
parameter_list|)
block|{
name|char
modifier|*
name|p
init|=
name|conf
decl_stmt|;
name|int
name|i
decl_stmt|,
name|dup
decl_stmt|,
name|invalid
decl_stmt|;
name|ngx_str_t
modifier|*
name|value
decl_stmt|,
name|line
decl_stmt|;
name|ngx_http_busy_lock_t
modifier|*
name|bl
decl_stmt|,
modifier|*
modifier|*
name|blp
decl_stmt|;
name|blp
operator|=
operator|(
name|ngx_http_busy_lock_t
operator|*
operator|*
operator|)
operator|(
name|p
operator|+
name|cmd
operator|->
name|offset
operator|)
expr_stmt|;
if|if
condition|(
operator|*
name|blp
condition|)
block|{
return|return
literal|"is duplicate"
return|;
block|}
comment|/* ngx_calloc_shared() */
if|if
condition|(
operator|!
operator|(
name|bl
operator|=
name|ngx_pcalloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_busy_lock_t
argument_list|)
argument_list|)
operator|)
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
operator|*
name|blp
operator|=
name|bl
expr_stmt|;
comment|/* ngx_calloc_shared() */
if|if
condition|(
operator|!
operator|(
name|bl
operator|->
name|mutex
operator|=
name|ngx_pcalloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_event_mutex_t
argument_list|)
argument_list|)
operator|)
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|dup
operator|=
literal|0
expr_stmt|;
name|invalid
operator|=
literal|0
expr_stmt|;
name|value
operator|=
name|cf
operator|->
name|args
operator|->
name|elts
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|1
init|;
name|i
operator|<
name|cf
operator|->
name|args
operator|->
name|nelts
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|value
index|[
name|i
index|]
operator|.
name|data
index|[
literal|1
index|]
operator|!=
literal|'='
condition|)
block|{
name|ngx_conf_log_error
argument_list|(
name|NGX_LOG_EMERG
argument_list|,
name|cf
argument_list|,
literal|0
argument_list|,
literal|"invalid value \"%s\""
argument_list|,
name|value
index|[
name|i
index|]
operator|.
name|data
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
switch|switch
condition|(
name|value
index|[
name|i
index|]
operator|.
name|data
index|[
literal|0
index|]
condition|)
block|{
case|case
literal|'b'
case|:
if|if
condition|(
name|bl
operator|->
name|max_busy
condition|)
block|{
name|dup
operator|=
literal|1
expr_stmt|;
break|break;
block|}
name|bl
operator|->
name|max_busy
operator|=
name|ngx_atoi
argument_list|(
name|value
index|[
name|i
index|]
operator|.
name|data
operator|+
literal|2
argument_list|,
name|value
index|[
name|i
index|]
operator|.
name|len
operator|-
literal|2
argument_list|)
expr_stmt|;
if|if
condition|(
name|bl
operator|->
name|max_busy
operator|==
name|NGX_ERROR
condition|)
block|{
name|invalid
operator|=
literal|1
expr_stmt|;
break|break;
block|}
continue|continue;
case|case
literal|'w'
case|:
if|if
condition|(
name|bl
operator|->
name|max_waiting
condition|)
block|{
name|dup
operator|=
literal|1
expr_stmt|;
break|break;
block|}
name|bl
operator|->
name|max_waiting
operator|=
name|ngx_atoi
argument_list|(
name|value
index|[
name|i
index|]
operator|.
name|data
operator|+
literal|2
argument_list|,
name|value
index|[
name|i
index|]
operator|.
name|len
operator|-
literal|2
argument_list|)
expr_stmt|;
if|if
condition|(
name|bl
operator|->
name|max_waiting
operator|==
name|NGX_ERROR
condition|)
block|{
name|invalid
operator|=
literal|1
expr_stmt|;
break|break;
block|}
continue|continue;
case|case
literal|'t'
case|:
if|if
condition|(
name|bl
operator|->
name|timeout
condition|)
block|{
name|dup
operator|=
literal|1
expr_stmt|;
break|break;
block|}
name|line
operator|.
name|len
operator|=
name|value
index|[
name|i
index|]
operator|.
name|len
operator|-
literal|2
expr_stmt|;
name|line
operator|.
name|data
operator|=
name|value
index|[
name|i
index|]
operator|.
name|data
operator|+
literal|2
expr_stmt|;
name|bl
operator|->
name|timeout
operator|=
name|ngx_parse_time
argument_list|(
operator|&
name|line
argument_list|,
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|bl
operator|->
name|timeout
operator|==
name|NGX_ERROR
condition|)
block|{
name|invalid
operator|=
literal|1
expr_stmt|;
break|break;
block|}
continue|continue;
default|default:
name|invalid
operator|=
literal|1
expr_stmt|;
block|}
if|if
condition|(
name|dup
condition|)
block|{
name|ngx_conf_log_error
argument_list|(
name|NGX_LOG_EMERG
argument_list|,
name|cf
argument_list|,
literal|0
argument_list|,
literal|"duplicate value \"%s\""
argument_list|,
name|value
index|[
name|i
index|]
operator|.
name|data
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
if|if
condition|(
name|invalid
condition|)
block|{
name|ngx_conf_log_error
argument_list|(
name|NGX_LOG_EMERG
argument_list|,
name|cf
argument_list|,
literal|0
argument_list|,
literal|"invalid value \"%s\""
argument_list|,
name|value
index|[
name|i
index|]
operator|.
name|data
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
block|}
if|if
condition|(
name|bl
operator|->
name|timeout
operator|==
literal|0
operator|&&
name|bl
operator|->
name|max_waiting
condition|)
block|{
name|ngx_conf_log_error
argument_list|(
name|NGX_LOG_WARN
argument_list|,
name|cf
argument_list|,
literal|0
argument_list|,
literal|"busy lock waiting is useless with zero timeout, ignoring"
argument_list|)
expr_stmt|;
block|}
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

end_unit

