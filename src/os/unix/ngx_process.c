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

begin_function_decl
specifier|static
name|void
name|ngx_exec_proc
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|,
name|void
modifier|*
name|data
parameter_list|)
function_decl|;
end_function_decl

begin_decl_stmt
DECL|variable|ngx_last_process
name|ngx_uint_t
name|ngx_last_process
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_processes
name|ngx_process_t
name|ngx_processes
index|[
name|NGX_MAX_PROCESSES
index|]
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|ngx_spawn_process (ngx_cycle_t * cycle,ngx_spawn_proc_pt proc,void * data,char * name,ngx_int_t respawn)
name|ngx_pid_t
name|ngx_spawn_process
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|,
name|ngx_spawn_proc_pt
name|proc
parameter_list|,
name|void
modifier|*
name|data
parameter_list|,
name|char
modifier|*
name|name
parameter_list|,
name|ngx_int_t
name|respawn
parameter_list|)
block|{
name|sigset_t
name|set
decl_stmt|,
name|oset
decl_stmt|;
name|ngx_pid_t
name|pid
decl_stmt|;
if|if
condition|(
name|respawn
operator|<
literal|0
condition|)
block|{
name|sigemptyset
argument_list|(
operator|&
name|set
argument_list|)
expr_stmt|;
name|sigaddset
argument_list|(
operator|&
name|set
argument_list|,
name|SIGCHLD
argument_list|)
expr_stmt|;
if|if
condition|(
name|sigprocmask
argument_list|(
name|SIG_BLOCK
argument_list|,
operator|&
name|set
argument_list|,
operator|&
name|oset
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
literal|"sigprocmask() failed while spawning %s"
argument_list|,
name|name
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
block|}
name|pid
operator|=
name|fork
argument_list|()
expr_stmt|;
if|if
condition|(
name|pid
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
literal|"fork() failed while spawning \"%s\""
argument_list|,
name|name
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|pid
operator|==
operator|-
literal|1
operator|||
name|pid
operator|==
literal|0
condition|)
block|{
if|if
condition|(
name|sigprocmask
argument_list|(
name|SIG_SETMASK
argument_list|,
operator|&
name|oset
argument_list|,
operator|&
name|set
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
literal|"sigprocmask() failed while spawning %s"
argument_list|,
name|name
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
block|}
switch|switch
condition|(
name|pid
condition|)
block|{
case|case
operator|-
literal|1
case|:
return|return
name|NGX_ERROR
return|;
case|case
literal|0
case|:
name|proc
argument_list|(
name|cycle
argument_list|,
name|data
argument_list|)
expr_stmt|;
break|break;
default|default:
break|break;
block|}
name|ngx_log_debug2
argument_list|(
name|NGX_LOG_DEBUG_CORE
argument_list|,
name|cycle
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"spawn %s: "
name|PID_T_FMT
argument_list|,
name|name
argument_list|,
name|pid
argument_list|)
expr_stmt|;
if|if
condition|(
name|respawn
operator|>=
literal|0
condition|)
block|{
name|ngx_processes
index|[
name|respawn
index|]
operator|.
name|pid
operator|=
name|pid
expr_stmt|;
name|ngx_processes
index|[
name|respawn
index|]
operator|.
name|exited
operator|=
literal|0
expr_stmt|;
return|return
name|pid
return|;
block|}
name|ngx_processes
index|[
name|ngx_last_process
index|]
operator|.
name|pid
operator|=
name|pid
expr_stmt|;
name|ngx_processes
index|[
name|ngx_last_process
index|]
operator|.
name|proc
operator|=
name|proc
expr_stmt|;
name|ngx_processes
index|[
name|ngx_last_process
index|]
operator|.
name|data
operator|=
name|data
expr_stmt|;
name|ngx_processes
index|[
name|ngx_last_process
index|]
operator|.
name|name
operator|=
name|name
expr_stmt|;
name|ngx_processes
index|[
name|ngx_last_process
index|]
operator|.
name|respawn
operator|=
operator|(
name|respawn
operator|==
name|NGX_PROCESS_RESPAWN
operator|)
condition|?
literal|1
else|:
literal|0
expr_stmt|;
name|ngx_processes
index|[
name|ngx_last_process
index|]
operator|.
name|detached
operator|=
operator|(
name|respawn
operator|==
name|NGX_PROCESS_DETACHED
operator|)
condition|?
literal|1
else|:
literal|0
expr_stmt|;
name|ngx_processes
index|[
name|ngx_last_process
index|]
operator|.
name|signal
operator|=
literal|0
expr_stmt|;
name|ngx_processes
index|[
name|ngx_last_process
index|]
operator|.
name|exited
operator|=
literal|0
expr_stmt|;
name|ngx_processes
index|[
name|ngx_last_process
index|]
operator|.
name|exiting
operator|=
literal|0
expr_stmt|;
name|ngx_last_process
operator|++
expr_stmt|;
if|if
condition|(
name|sigprocmask
argument_list|(
name|SIG_SETMASK
argument_list|,
operator|&
name|oset
argument_list|,
operator|&
name|set
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
literal|"sigprocmask() failed while spawning %s"
argument_list|,
name|name
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
return|return
name|pid
return|;
block|}
end_function

begin_function
DECL|function|ngx_exec (ngx_cycle_t * cycle,ngx_exec_ctx_t * ctx)
name|ngx_int_t
name|ngx_exec
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|,
name|ngx_exec_ctx_t
modifier|*
name|ctx
parameter_list|)
block|{
if|if
condition|(
name|ngx_spawn_process
argument_list|(
name|cycle
argument_list|,
name|ngx_exec_proc
argument_list|,
name|ctx
argument_list|,
name|ctx
operator|->
name|name
argument_list|,
name|NGX_PROCESS_DETACHED
argument_list|)
operator|==
name|NGX_ERROR
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
literal|"can not spawn %s"
argument_list|,
name|ctx
operator|->
name|name
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
DECL|function|ngx_exec_proc (ngx_cycle_t * cycle,void * data)
specifier|static
name|void
name|ngx_exec_proc
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|,
name|void
modifier|*
name|data
parameter_list|)
block|{
name|ngx_exec_ctx_t
modifier|*
name|ctx
init|=
name|data
decl_stmt|;
if|if
condition|(
name|execve
argument_list|(
name|ctx
operator|->
name|path
argument_list|,
name|ctx
operator|->
name|argv
argument_list|,
name|ctx
operator|->
name|envp
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
literal|"execve() failed while executing %s \"%s\""
argument_list|,
name|ctx
operator|->
name|name
argument_list|,
name|ctx
operator|->
name|path
argument_list|)
expr_stmt|;
block|}
name|exit
argument_list|(
literal|1
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|ngx_signal_processes (ngx_cycle_t * cycle,ngx_int_t signo)
name|void
name|ngx_signal_processes
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|,
name|ngx_int_t
name|signo
parameter_list|)
block|{
name|ngx_uint_t
name|i
decl_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|ngx_last_process
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
operator|!
name|ngx_processes
index|[
name|i
index|]
operator|.
name|signal
condition|)
block|{
continue|continue;
block|}
if|#
directive|if
literal|0
block_content|if (ngx_processes[i].exited) {             if (i != --ngx_last_process) {                 ngx_processes[i--] = ngx_processes[ngx_last_process];             }             continue;         }
endif|#
directive|endif
name|ngx_log_debug2
argument_list|(
name|NGX_LOG_DEBUG_CORE
argument_list|,
name|cycle
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"kill ("
name|PID_T_FMT
literal|", %d)"
argument_list|,
name|ngx_processes
index|[
name|i
index|]
operator|.
name|pid
argument_list|,
name|signo
argument_list|)
expr_stmt|;
if|if
condition|(
name|kill
argument_list|(
name|ngx_processes
index|[
name|i
index|]
operator|.
name|pid
argument_list|,
name|signo
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
literal|"kill(%d, %d) failed"
argument_list|,
name|ngx_processes
index|[
name|i
index|]
operator|.
name|pid
argument_list|,
name|signo
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|signo
operator|!=
name|ngx_signal_value
argument_list|(
name|NGX_REOPEN_SIGNAL
argument_list|)
condition|)
block|{
name|ngx_processes
index|[
name|i
index|]
operator|.
name|exiting
operator|=
literal|1
expr_stmt|;
block|}
block|}
block|}
end_function

begin_function
DECL|function|ngx_respawn_processes (ngx_cycle_t * cycle)
name|void
name|ngx_respawn_processes
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
block|{
name|ngx_uint_t
name|i
decl_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|ngx_last_process
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|ngx_processes
index|[
name|i
index|]
operator|.
name|exiting
operator|||
operator|!
name|ngx_processes
index|[
name|i
index|]
operator|.
name|exited
condition|)
block|{
continue|continue;
block|}
if|if
condition|(
operator|!
name|ngx_processes
index|[
name|i
index|]
operator|.
name|respawn
condition|)
block|{
if|if
condition|(
name|i
operator|!=
operator|--
name|ngx_last_process
condition|)
block|{
name|ngx_processes
index|[
name|i
operator|--
index|]
operator|=
name|ngx_processes
index|[
name|ngx_last_process
index|]
expr_stmt|;
block|}
continue|continue;
block|}
if|if
condition|(
name|ngx_spawn_process
argument_list|(
name|cycle
argument_list|,
name|ngx_processes
index|[
name|i
index|]
operator|.
name|proc
argument_list|,
name|ngx_processes
index|[
name|i
index|]
operator|.
name|data
argument_list|,
name|ngx_processes
index|[
name|i
index|]
operator|.
name|name
argument_list|,
name|i
argument_list|)
operator|==
name|NGX_ERROR
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
literal|"can not respawn %s"
argument_list|,
name|ngx_processes
index|[
name|i
index|]
operator|.
name|name
argument_list|)
expr_stmt|;
block|}
block|}
block|}
end_function

begin_function
DECL|function|ngx_process_get_status ()
name|void
name|ngx_process_get_status
parameter_list|()
block|{
name|int
name|status
decl_stmt|;
name|char
modifier|*
name|process
decl_stmt|;
name|ngx_pid_t
name|pid
decl_stmt|;
name|ngx_err_t
name|err
decl_stmt|;
name|ngx_uint_t
name|i
decl_stmt|,
name|one
decl_stmt|;
name|struct
name|timeval
name|tv
decl_stmt|;
name|one
operator|=
literal|0
expr_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
name|pid
operator|=
name|waitpid
argument_list|(
operator|-
literal|1
argument_list|,
operator|&
name|status
argument_list|,
name|WNOHANG
argument_list|)
expr_stmt|;
if|if
condition|(
name|pid
operator|==
literal|0
condition|)
block|{
return|return;
block|}
if|if
condition|(
name|pid
operator|==
operator|-
literal|1
condition|)
block|{
name|err
operator|=
name|ngx_errno
expr_stmt|;
if|if
condition|(
name|err
operator|==
name|NGX_EINTR
condition|)
block|{
continue|continue;
block|}
if|if
condition|(
name|err
operator|==
name|NGX_ECHILD
operator|&&
name|one
condition|)
block|{
return|return;
block|}
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|ngx_cycle
operator|->
name|log
argument_list|,
name|errno
argument_list|,
literal|"waitpid() failed"
argument_list|)
expr_stmt|;
return|return;
block|}
name|one
operator|=
literal|1
expr_stmt|;
name|process
operator|=
literal|""
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|ngx_last_process
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|ngx_processes
index|[
name|i
index|]
operator|.
name|pid
operator|==
name|pid
condition|)
block|{
name|ngx_processes
index|[
name|i
index|]
operator|.
name|status
operator|=
name|status
expr_stmt|;
name|ngx_processes
index|[
name|i
index|]
operator|.
name|exited
operator|=
literal|1
expr_stmt|;
name|process
operator|=
name|ngx_processes
index|[
name|i
index|]
operator|.
name|name
expr_stmt|;
break|break;
block|}
block|}
if|if
condition|(
name|i
operator|==
name|ngx_last_process
condition|)
block|{
name|process
operator|=
literal|"unknown process"
expr_stmt|;
block|}
if|if
condition|(
name|WTERMSIG
argument_list|(
name|status
argument_list|)
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|ngx_cycle
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"%s "
name|PID_T_FMT
literal|" exited on signal %d%s"
argument_list|,
name|process
argument_list|,
name|pid
argument_list|,
name|WTERMSIG
argument_list|(
name|status
argument_list|)
argument_list|,
name|WCOREDUMP
argument_list|(
name|status
argument_list|)
condition|?
literal|" (core dumped)"
else|:
literal|""
argument_list|)
expr_stmt|;
block|}
else|else
block|{
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
literal|"%s "
name|PID_T_FMT
literal|" exited with code %d"
argument_list|,
name|process
argument_list|,
name|pid
argument_list|,
name|WEXITSTATUS
argument_list|(
name|status
argument_list|)
argument_list|)
expr_stmt|;
block|}
block|}
block|}
end_function

end_unit

