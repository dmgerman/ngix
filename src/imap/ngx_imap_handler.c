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
file|<ngx_imap.h>
end_include

begin_include
include|#
directive|include
file|<nginx.h>
end_include

begin_function_decl
specifier|static
name|void
name|ngx_imap_init_session
parameter_list|(
name|ngx_event_t
modifier|*
name|rev
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|void
name|ngx_pop3_auth_state
parameter_list|(
name|ngx_event_t
modifier|*
name|rev
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_pop3_read_command
parameter_list|(
name|ngx_imap_session_t
modifier|*
name|s
parameter_list|)
function_decl|;
end_function_decl

begin_decl_stmt
DECL|variable|pop3_greeting
specifier|static
name|u_char
name|pop3_greeting
index|[]
init|=
literal|"+OK "
name|NGINX_VER
literal|" ready"
name|CRLF
decl_stmt|;
end_decl_stmt

begin_if
if|#
directive|if
literal|0
end_if

begin_endif
unit|static u_char imap_greeting[] = "* OK " NGINX_VER " ready" CRLF;
endif|#
directive|endif
end_endif

begin_decl_stmt
DECL|variable|pop3_ok
specifier|static
name|u_char
name|pop3_ok
index|[]
init|=
literal|"+OK"
name|CRLF
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|pop3_invalid_command
specifier|static
name|u_char
name|pop3_invalid_command
index|[]
init|=
literal|"-ERR invalid command"
name|CRLF
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|ngx_imap_init_connection (ngx_connection_t * c)
name|void
name|ngx_imap_init_connection
parameter_list|(
name|ngx_connection_t
modifier|*
name|c
parameter_list|)
block|{
name|u_char
modifier|*
name|greeting
decl_stmt|;
name|ssize_t
name|size
decl_stmt|;
name|ngx_log_debug0
argument_list|(
name|NGX_LOG_DEBUG_IMAP
argument_list|,
name|c
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"imap init connection"
argument_list|)
expr_stmt|;
name|c
operator|->
name|log_error
operator|=
name|NGX_ERROR_INFO
expr_stmt|;
name|greeting
operator|=
name|pop3_greeting
expr_stmt|;
name|size
operator|=
sizeof|sizeof
argument_list|(
name|pop3_greeting
argument_list|)
operator|-
literal|1
expr_stmt|;
if|if
condition|(
name|ngx_send
argument_list|(
name|c
argument_list|,
name|greeting
argument_list|,
name|size
argument_list|)
operator|<
name|size
condition|)
block|{
comment|/*          * we treat the incomplete sending as NGX_ERROR          * because it is very strange here          */
name|ngx_imap_close_connection
argument_list|(
name|c
argument_list|)
expr_stmt|;
return|return;
block|}
name|c
operator|->
name|read
operator|->
name|event_handler
operator|=
name|ngx_imap_init_session
expr_stmt|;
name|ngx_add_timer
argument_list|(
name|c
operator|->
name|read
argument_list|,
comment|/* STUB */
literal|60000
argument_list|)
expr_stmt|;
if|if
condition|(
name|ngx_handle_read_event
argument_list|(
name|c
operator|->
name|read
argument_list|,
literal|0
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
name|ngx_imap_close_connection
argument_list|(
name|c
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|ngx_imap_init_session (ngx_event_t * rev)
specifier|static
name|void
name|ngx_imap_init_session
parameter_list|(
name|ngx_event_t
modifier|*
name|rev
parameter_list|)
block|{
name|size_t
name|size
decl_stmt|;
name|ngx_connection_t
modifier|*
name|c
decl_stmt|;
name|ngx_imap_session_t
modifier|*
name|s
decl_stmt|;
name|c
operator|=
name|rev
operator|->
name|data
expr_stmt|;
if|if
condition|(
name|rev
operator|->
name|timedout
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_INFO
argument_list|,
name|c
operator|->
name|log
argument_list|,
name|NGX_ETIMEDOUT
argument_list|,
literal|"client timed out"
argument_list|)
expr_stmt|;
name|ngx_imap_close_connection
argument_list|(
name|c
argument_list|)
expr_stmt|;
return|return;
block|}
name|s
operator|=
name|ngx_pcalloc
argument_list|(
name|c
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_imap_session_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|s
operator|==
name|NULL
condition|)
block|{
name|ngx_imap_close_connection
argument_list|(
name|c
argument_list|)
expr_stmt|;
return|return;
block|}
name|c
operator|->
name|data
operator|=
name|s
expr_stmt|;
name|s
operator|->
name|connection
operator|=
name|c
expr_stmt|;
if|if
condition|(
name|ngx_array_init
argument_list|(
operator|&
name|s
operator|->
name|args
argument_list|,
name|c
operator|->
name|pool
argument_list|,
literal|2
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_str_t
argument_list|)
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
name|ngx_imap_close_connection
argument_list|(
name|c
argument_list|)
expr_stmt|;
return|return;
block|}
name|size
operator|=
comment|/* STUB: pop3: 128, imap: configurable 4K default */
literal|128
expr_stmt|;
name|s
operator|->
name|buffer
operator|=
name|ngx_create_temp_buf
argument_list|(
name|c
operator|->
name|pool
argument_list|,
name|size
argument_list|)
expr_stmt|;
if|if
condition|(
name|s
operator|->
name|buffer
operator|==
name|NULL
condition|)
block|{
name|ngx_imap_close_connection
argument_list|(
name|c
argument_list|)
expr_stmt|;
return|return;
block|}
name|c
operator|->
name|read
operator|->
name|event_handler
operator|=
name|ngx_pop3_auth_state
expr_stmt|;
name|ngx_pop3_auth_state
argument_list|(
name|rev
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|ngx_pop3_auth_state (ngx_event_t * rev)
specifier|static
name|void
name|ngx_pop3_auth_state
parameter_list|(
name|ngx_event_t
modifier|*
name|rev
parameter_list|)
block|{
name|u_char
modifier|*
name|text
decl_stmt|;
name|ssize_t
name|size
decl_stmt|;
name|ngx_int_t
name|rc
decl_stmt|;
name|ngx_uint_t
name|quit
decl_stmt|;
name|ngx_str_t
modifier|*
name|arg
decl_stmt|;
name|ngx_connection_t
modifier|*
name|c
decl_stmt|;
name|ngx_imap_session_t
modifier|*
name|s
decl_stmt|;
name|c
operator|=
name|rev
operator|->
name|data
expr_stmt|;
name|s
operator|=
name|c
operator|->
name|data
expr_stmt|;
name|ngx_log_debug0
argument_list|(
name|NGX_LOG_DEBUG_IMAP
argument_list|,
name|c
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"pop3 auth state"
argument_list|)
expr_stmt|;
if|if
condition|(
name|rev
operator|->
name|timedout
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_INFO
argument_list|,
name|c
operator|->
name|log
argument_list|,
name|NGX_ETIMEDOUT
argument_list|,
literal|"client timed out"
argument_list|)
expr_stmt|;
name|ngx_imap_close_connection
argument_list|(
name|c
argument_list|)
expr_stmt|;
return|return;
block|}
name|rc
operator|=
name|ngx_pop3_read_command
argument_list|(
name|s
argument_list|)
expr_stmt|;
if|if
condition|(
name|rc
operator|==
name|NGX_AGAIN
operator|||
name|rc
operator|==
name|NGX_ERROR
condition|)
block|{
return|return;
block|}
name|quit
operator|=
literal|0
expr_stmt|;
name|text
operator|=
name|pop3_ok
expr_stmt|;
name|size
operator|=
sizeof|sizeof
argument_list|(
name|pop3_ok
argument_list|)
operator|-
literal|1
expr_stmt|;
if|if
condition|(
name|rc
operator|==
name|NGX_OK
condition|)
block|{
switch|switch
condition|(
name|s
operator|->
name|imap_state
condition|)
block|{
case|case
name|ngx_pop3_start
case|:
switch|switch
condition|(
name|s
operator|->
name|command
condition|)
block|{
case|case
name|NGX_POP3_USER
case|:
if|if
condition|(
name|s
operator|->
name|args
operator|.
name|nelts
operator|==
literal|1
condition|)
block|{
name|s
operator|->
name|imap_state
operator|=
name|ngx_pop3_user
expr_stmt|;
name|arg
operator|=
name|s
operator|->
name|args
operator|.
name|elts
expr_stmt|;
name|s
operator|->
name|login
operator|.
name|len
operator|=
name|arg
index|[
literal|0
index|]
operator|.
name|len
expr_stmt|;
name|s
operator|->
name|login
operator|.
name|data
operator|=
name|ngx_palloc
argument_list|(
name|c
operator|->
name|pool
argument_list|,
name|s
operator|->
name|login
operator|.
name|len
operator|+
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|s
operator|->
name|login
operator|.
name|data
operator|==
name|NULL
condition|)
block|{
name|ngx_imap_close_connection
argument_list|(
name|c
argument_list|)
expr_stmt|;
return|return;
block|}
name|ngx_cpystrn
argument_list|(
name|s
operator|->
name|login
operator|.
name|data
argument_list|,
name|arg
index|[
literal|0
index|]
operator|.
name|data
argument_list|,
name|s
operator|->
name|login
operator|.
name|len
operator|+
literal|1
argument_list|)
expr_stmt|;
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_IMAP
argument_list|,
name|c
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"pop3 login: \"%s\""
argument_list|,
name|s
operator|->
name|login
operator|.
name|data
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|rc
operator|=
name|NGX_IMAP_PARSE_INVALID_COMMAND
expr_stmt|;
block|}
break|break;
case|case
name|NGX_POP3_QUIT
case|:
name|quit
operator|=
literal|1
expr_stmt|;
break|break;
default|default:
name|s
operator|->
name|imap_state
operator|=
name|ngx_pop3_start
expr_stmt|;
name|rc
operator|=
name|NGX_IMAP_PARSE_INVALID_COMMAND
expr_stmt|;
break|break;
block|}
break|break;
case|case
name|ngx_pop3_user
case|:
switch|switch
condition|(
name|s
operator|->
name|command
condition|)
block|{
case|case
name|NGX_POP3_PASS
case|:
if|if
condition|(
name|s
operator|->
name|args
operator|.
name|nelts
operator|==
literal|1
condition|)
block|{
comment|/* STUB */
name|s
operator|->
name|imap_state
operator|=
name|ngx_pop3_start
expr_stmt|;
name|arg
operator|=
name|s
operator|->
name|args
operator|.
name|elts
expr_stmt|;
name|s
operator|->
name|passwd
operator|.
name|len
operator|=
name|arg
index|[
literal|0
index|]
operator|.
name|len
expr_stmt|;
name|s
operator|->
name|passwd
operator|.
name|data
operator|=
name|ngx_palloc
argument_list|(
name|c
operator|->
name|pool
argument_list|,
name|s
operator|->
name|passwd
operator|.
name|len
operator|+
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|s
operator|->
name|passwd
operator|.
name|data
operator|==
name|NULL
condition|)
block|{
name|ngx_imap_close_connection
argument_list|(
name|c
argument_list|)
expr_stmt|;
return|return;
block|}
name|ngx_cpystrn
argument_list|(
name|s
operator|->
name|passwd
operator|.
name|data
argument_list|,
name|arg
index|[
literal|0
index|]
operator|.
name|data
argument_list|,
name|s
operator|->
name|passwd
operator|.
name|len
operator|+
literal|1
argument_list|)
expr_stmt|;
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_IMAP
argument_list|,
name|c
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"pop3 passwd: \"%s\""
argument_list|,
name|s
operator|->
name|passwd
operator|.
name|data
argument_list|)
expr_stmt|;
name|s
operator|->
name|buffer
operator|->
name|pos
operator|=
name|s
operator|->
name|buffer
operator|->
name|start
expr_stmt|;
name|s
operator|->
name|buffer
operator|->
name|last
operator|=
name|s
operator|->
name|buffer
operator|->
name|start
expr_stmt|;
name|ngx_imap_proxy_init
argument_list|(
name|s
argument_list|)
expr_stmt|;
return|return;
block|}
else|else
block|{
name|rc
operator|=
name|NGX_IMAP_PARSE_INVALID_COMMAND
expr_stmt|;
block|}
break|break;
case|case
name|NGX_POP3_QUIT
case|:
name|quit
operator|=
literal|1
expr_stmt|;
break|break;
default|default:
name|s
operator|->
name|imap_state
operator|=
name|ngx_pop3_start
expr_stmt|;
name|rc
operator|=
name|NGX_IMAP_PARSE_INVALID_COMMAND
expr_stmt|;
break|break;
block|}
break|break;
block|}
block|}
if|if
condition|(
name|rc
operator|==
name|NGX_IMAP_PARSE_INVALID_COMMAND
condition|)
block|{
name|text
operator|=
name|pop3_invalid_command
expr_stmt|;
name|size
operator|=
sizeof|sizeof
argument_list|(
name|pop3_invalid_command
argument_list|)
operator|-
literal|1
expr_stmt|;
block|}
if|if
condition|(
name|ngx_send
argument_list|(
name|c
argument_list|,
name|text
argument_list|,
name|size
argument_list|)
operator|<
name|size
condition|)
block|{
comment|/*          * we treat the incomplete sending as NGX_ERROR          * because it is very strange here          */
name|ngx_imap_close_connection
argument_list|(
name|c
argument_list|)
expr_stmt|;
return|return;
block|}
if|if
condition|(
name|quit
condition|)
block|{
name|ngx_imap_close_connection
argument_list|(
name|c
argument_list|)
expr_stmt|;
return|return;
block|}
name|s
operator|->
name|args
operator|.
name|nelts
operator|=
literal|0
expr_stmt|;
name|s
operator|->
name|buffer
operator|->
name|pos
operator|=
name|s
operator|->
name|buffer
operator|->
name|start
expr_stmt|;
name|s
operator|->
name|buffer
operator|->
name|last
operator|=
name|s
operator|->
name|buffer
operator|->
name|start
expr_stmt|;
block|}
end_function

begin_function
DECL|function|ngx_pop3_read_command (ngx_imap_session_t * s)
specifier|static
name|ngx_int_t
name|ngx_pop3_read_command
parameter_list|(
name|ngx_imap_session_t
modifier|*
name|s
parameter_list|)
block|{
name|ssize_t
name|n
decl_stmt|;
name|ngx_int_t
name|rc
decl_stmt|;
name|n
operator|=
name|ngx_recv
argument_list|(
name|s
operator|->
name|connection
argument_list|,
name|s
operator|->
name|buffer
operator|->
name|last
argument_list|,
name|s
operator|->
name|buffer
operator|->
name|end
operator|-
name|s
operator|->
name|buffer
operator|->
name|last
argument_list|)
expr_stmt|;
if|if
condition|(
name|n
operator|==
name|NGX_ERROR
operator|||
name|n
operator|==
literal|0
condition|)
block|{
name|ngx_imap_close_connection
argument_list|(
name|s
operator|->
name|connection
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|n
operator|>
literal|0
condition|)
block|{
name|s
operator|->
name|buffer
operator|->
name|last
operator|+=
name|n
expr_stmt|;
block|}
if|if
condition|(
name|n
operator|==
name|NGX_AGAIN
condition|)
block|{
if|if
condition|(
name|ngx_handle_read_event
argument_list|(
name|s
operator|->
name|connection
operator|->
name|read
argument_list|,
literal|0
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
name|ngx_imap_close_connection
argument_list|(
name|s
operator|->
name|connection
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
return|return
name|NGX_AGAIN
return|;
block|}
name|rc
operator|=
name|ngx_pop3_parse_command
argument_list|(
name|s
argument_list|)
expr_stmt|;
if|if
condition|(
name|rc
operator|==
name|NGX_AGAIN
operator|||
name|rc
operator|==
name|NGX_IMAP_PARSE_INVALID_COMMAND
condition|)
block|{
return|return
name|rc
return|;
block|}
if|if
condition|(
name|rc
operator|==
name|NGX_ERROR
condition|)
block|{
name|ngx_imap_close_connection
argument_list|(
name|s
operator|->
name|connection
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

begin_if
if|#
directive|if
literal|0
end_if

begin_endif
unit|void ngx_imap_close_session(ngx_imap_session_t *s) {     ngx_log_debug0(NGX_LOG_DEBUG_IMAP, s->connection->log, 0,                    "close imap session");      ngx_imap_close_connection(s->connection); }
endif|#
directive|endif
end_endif

begin_function
DECL|function|ngx_imap_close_connection (ngx_connection_t * c)
name|void
name|ngx_imap_close_connection
parameter_list|(
name|ngx_connection_t
modifier|*
name|c
parameter_list|)
block|{
name|ngx_pool_t
modifier|*
name|pool
decl_stmt|;
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_IMAP
argument_list|,
name|c
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"close imap connection: %d"
argument_list|,
name|c
operator|->
name|fd
argument_list|)
expr_stmt|;
name|pool
operator|=
name|c
operator|->
name|pool
expr_stmt|;
name|ngx_close_connection
argument_list|(
name|c
argument_list|)
expr_stmt|;
name|ngx_destroy_pool
argument_list|(
name|pool
argument_list|)
expr_stmt|;
block|}
end_function

end_unit

