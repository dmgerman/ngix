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

begin_decl_stmt
DECL|variable|ngx_max_sockets
name|int
name|ngx_max_sockets
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_inherited_nonblocking
name|int
name|ngx_inherited_nonblocking
decl_stmt|;
end_decl_stmt

begin_function_decl
name|void
name|ngx_signal_handler
parameter_list|(
name|int
name|signo
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|ngx_exit_signal_handler
parameter_list|(
name|int
name|signo
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|ngx_restart_signal_handler
parameter_list|(
name|int
name|signo
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|ngx_rotate_signal_handler
parameter_list|(
name|int
name|signo
parameter_list|)
function_decl|;
end_function_decl

begin_typedef
DECL|struct|__anon28941d220108
typedef|typedef
struct|struct
block|{
DECL|member|signo
name|int
name|signo
decl_stmt|;
DECL|member|signame
name|char
modifier|*
name|signame
decl_stmt|;
DECL|member|action
name|char
modifier|*
name|action
decl_stmt|;
DECL|member|handler
name|void
function_decl|(
modifier|*
name|handler
function_decl|)
parameter_list|(
name|int
name|signo
parameter_list|)
function_decl|;
DECL|typedef|ngx_signal_t
block|}
name|ngx_signal_t
typedef|;
end_typedef

begin_decl_stmt
DECL|variable|signals
name|ngx_signal_t
name|signals
index|[]
init|=
block|{
block|{
name|ngx_signal_value
argument_list|(
name|NGX_RESTART_SIGNAL
argument_list|)
block|,
literal|"SIG"
name|ngx_value
argument_list|(
name|NGX_RESTART_SIGNAL
argument_list|)
block|,
literal|"restarting"
block|,
name|ngx_signal_handler
block|}
block|,
block|{
name|ngx_signal_value
argument_list|(
name|NGX_ROTATE_SIGNAL
argument_list|)
block|,
literal|"SIG"
name|ngx_value
argument_list|(
name|NGX_ROTATE_SIGNAL
argument_list|)
block|,
literal|"reopen logs"
block|,
name|ngx_signal_handler
block|}
block|,
block|{
name|ngx_signal_value
argument_list|(
name|NGX_INTERRUPT_SIGNAL
argument_list|)
block|,
literal|"SIG"
name|ngx_value
argument_list|(
name|NGX_INTERRUPT_SIGNAL
argument_list|)
block|,
literal|"exiting"
block|,
name|ngx_signal_handler
block|}
block|,
block|{
name|ngx_signal_value
argument_list|(
name|NGX_SHUTDOWN_SIGNAL
argument_list|)
block|,
literal|"SIG"
name|ngx_value
argument_list|(
name|NGX_SHUTDOWN_SIGNAL
argument_list|)
block|,
literal|"shutdowning"
block|,
name|ngx_signal_handler
block|}
block|,
block|{
name|SIGCHLD
block|,
literal|"SIGCHLD"
block|,
name|NULL
block|,
name|ngx_sigchld_handler
block|}
block|,
block|{
name|SIGPIPE
block|,
literal|"SIGPIPE, SIG_IGN"
block|,
name|NULL
block|,
name|SIG_IGN
block|}
block|,
block|{
literal|0
block|,
name|NULL
block|,
name|NULL
block|,
name|NULL
block|}
block|}
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|ngx_posix_init (ngx_log_t * log)
name|int
name|ngx_posix_init
parameter_list|(
name|ngx_log_t
modifier|*
name|log
parameter_list|)
block|{
name|ngx_signal_t
modifier|*
name|sig
decl_stmt|;
name|struct
name|rlimit
name|rlmt
decl_stmt|;
name|struct
name|sigaction
name|sa
decl_stmt|;
for|for
control|(
name|sig
operator|=
name|signals
init|;
name|sig
operator|->
name|signo
operator|!=
literal|0
condition|;
name|sig
operator|++
control|)
block|{
name|ngx_memzero
argument_list|(
operator|&
name|sa
argument_list|,
sizeof|sizeof
argument_list|(
expr|struct
name|sigaction
argument_list|)
argument_list|)
expr_stmt|;
name|sa
operator|.
name|sa_handler
operator|=
name|sig
operator|->
name|handler
expr_stmt|;
name|sigemptyset
argument_list|(
operator|&
name|sa
operator|.
name|sa_mask
argument_list|)
expr_stmt|;
if|if
condition|(
name|sigaction
argument_list|(
name|sig
operator|->
name|signo
argument_list|,
operator|&
name|sa
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
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"sigaction(%s) failed"
argument_list|,
name|sig
operator|->
name|signame
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
block|}
if|if
condition|(
name|getrlimit
argument_list|(
name|RLIMIT_NOFILE
argument_list|,
operator|&
name|rlmt
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
name|log
argument_list|,
name|errno
argument_list|,
literal|"getrlimit(RLIMIT_NOFILE) failed)"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
name|ngx_log_error
argument_list|(
name|NGX_LOG_INFO
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"getrlimit(RLIMIT_NOFILE): "
name|RLIM_T_FMT
literal|":"
name|RLIM_T_FMT
argument_list|,
name|rlmt
operator|.
name|rlim_cur
argument_list|,
name|rlmt
operator|.
name|rlim_max
argument_list|)
expr_stmt|;
name|ngx_max_sockets
operator|=
name|rlmt
operator|.
name|rlim_cur
expr_stmt|;
if|#
directive|if
operator|(
name|HAVE_INHERITED_NONBLOCK
operator|)
name|ngx_inherited_nonblocking
operator|=
literal|1
expr_stmt|;
else|#
directive|else
name|ngx_inherited_nonblocking
operator|=
literal|0
expr_stmt|;
endif|#
directive|endif
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_signal_handler (int signo)
name|void
name|ngx_signal_handler
parameter_list|(
name|int
name|signo
parameter_list|)
block|{
name|char
modifier|*
name|name
decl_stmt|;
name|ngx_signal_t
modifier|*
name|sig
decl_stmt|;
for|for
control|(
name|sig
operator|=
name|signals
init|;
name|sig
operator|->
name|signo
operator|!=
literal|0
condition|;
name|sig
operator|++
control|)
block|{
if|if
condition|(
name|sig
operator|->
name|signo
operator|==
name|signo
condition|)
block|{
break|break;
block|}
block|}
comment|/* STUB */
name|name
operator|=
name|strsignal
argument_list|(
name|signo
argument_list|)
expr_stmt|;
name|ngx_log_error
argument_list|(
name|NGX_LOG_INFO
argument_list|,
name|ngx_cycle
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"signal #%d (%s: %s) received, %s"
argument_list|,
name|signo
argument_list|,
name|sig
operator|->
name|signame
argument_list|,
name|name
argument_list|,
name|sig
operator|->
name|action
argument_list|)
expr_stmt|;
switch|switch
condition|(
name|signo
condition|)
block|{
comment|/* STUB */
case|case
name|SIGQUIT
case|:
case|case
name|SIGABRT
case|:
case|case
name|ngx_signal_value
argument_list|(
name|NGX_SHUTDOWN_SIGNAL
argument_list|)
case|:
case|case
name|ngx_signal_value
argument_list|(
name|NGX_INTERRUPT_SIGNAL
argument_list|)
case|:
name|done
operator|=
literal|1
expr_stmt|;
break|break;
case|case
name|ngx_signal_value
argument_list|(
name|NGX_RESTART_SIGNAL
argument_list|)
case|:
name|restart
operator|=
literal|1
expr_stmt|;
break|break;
case|case
name|ngx_signal_value
argument_list|(
name|NGX_ROTATE_SIGNAL
argument_list|)
case|:
name|rotate
operator|=
literal|1
expr_stmt|;
break|break;
block|}
block|}
end_function

begin_function
DECL|function|ngx_exit_signal_handler (int signo)
name|void
name|ngx_exit_signal_handler
parameter_list|(
name|int
name|signo
parameter_list|)
block|{
name|char
modifier|*
name|s
decl_stmt|;
name|s
operator|=
name|strsignal
argument_list|(
name|signo
argument_list|)
expr_stmt|;
name|ngx_log_error
argument_list|(
name|NGX_LOG_INFO
argument_list|,
name|ngx_cycle
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"%s signal received, exiting"
argument_list|,
name|s
argument_list|)
expr_stmt|;
name|done
operator|=
literal|1
expr_stmt|;
block|}
end_function

begin_function
DECL|function|ngx_restart_signal_handler (int signo)
name|void
name|ngx_restart_signal_handler
parameter_list|(
name|int
name|signo
parameter_list|)
block|{
name|restart
operator|=
literal|1
expr_stmt|;
block|}
end_function

begin_function
DECL|function|ngx_rotate_signal_handler (int signo)
name|void
name|ngx_rotate_signal_handler
parameter_list|(
name|int
name|signo
parameter_list|)
block|{
name|rotate
operator|=
literal|1
expr_stmt|;
block|}
end_function

begin_function
DECL|function|ngx_posix_post_conf_init (ngx_log_t * log)
name|int
name|ngx_posix_post_conf_init
parameter_list|(
name|ngx_log_t
modifier|*
name|log
parameter_list|)
block|{
name|ngx_fd_t
name|pp
index|[
literal|2
index|]
decl_stmt|;
if|if
condition|(
name|pipe
argument_list|(
name|pp
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
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"pipe() failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|dup2
argument_list|(
name|pp
index|[
literal|1
index|]
argument_list|,
name|STDERR_FILENO
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
name|log
argument_list|,
name|errno
argument_list|,
literal|"dup2(STDERR) failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|pp
index|[
literal|1
index|]
operator|>
name|STDERR_FILENO
condition|)
block|{
if|if
condition|(
name|close
argument_list|(
name|pp
index|[
literal|1
index|]
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
name|log
argument_list|,
name|errno
argument_list|,
literal|"close() failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
block|}
return|return
name|NGX_OK
return|;
block|}
end_function

end_unit

