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
file|<ngx_event.h>
end_include

begin_include
include|#
directive|include
file|<nginx.h>
end_include

begin_function_decl
specifier|static
name|void
name|ngx_master_exit
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|,
name|ngx_master_ctx_t
modifier|*
name|ctx
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|void
name|ngx_worker_process_cycle
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

begin_if
if|#
directive|if
operator|(
name|NGX_THREADS
operator|)
end_if

begin_function_decl
specifier|static
name|int
name|ngx_worker_thread_cycle
parameter_list|(
name|void
modifier|*
name|data
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_decl_stmt
DECL|variable|ngx_process
name|ngx_int_t
name|ngx_process
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_pid
name|ngx_pid_t
name|ngx_pid
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_new_binary
name|ngx_pid_t
name|ngx_new_binary
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_inherited
name|ngx_int_t
name|ngx_inherited
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_threaded
name|ngx_int_t
name|ngx_threaded
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_reap
name|sig_atomic_t
name|ngx_reap
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_timer
name|sig_atomic_t
name|ngx_timer
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_terminate
name|sig_atomic_t
name|ngx_terminate
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_quit
name|sig_atomic_t
name|ngx_quit
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_noaccept
name|sig_atomic_t
name|ngx_noaccept
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_reconfigure
name|sig_atomic_t
name|ngx_reconfigure
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_reopen
name|sig_atomic_t
name|ngx_reopen
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_change_binary
name|sig_atomic_t
name|ngx_change_binary
decl_stmt|;
end_decl_stmt

begin_comment
comment|/* TODO: broken NGX_PROCESS_SINGLE */
end_comment

begin_function
DECL|function|ngx_master_process_cycle (ngx_cycle_t * cycle,ngx_master_ctx_t * ctx)
name|void
name|ngx_master_process_cycle
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|,
name|ngx_master_ctx_t
modifier|*
name|ctx
parameter_list|)
block|{
name|int
name|signo
decl_stmt|;
name|sigset_t
name|set
decl_stmt|;
name|struct
name|timeval
name|tv
decl_stmt|;
name|struct
name|itimerval
name|itv
decl_stmt|;
name|ngx_uint_t
name|i
decl_stmt|,
name|live
decl_stmt|;
name|ngx_msec_t
name|delay
decl_stmt|;
name|ngx_core_conf_t
modifier|*
name|ccf
decl_stmt|;
if|if
condition|(
name|ngx_process
operator|==
name|NGX_PROCESS_MASTER
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
name|sigaddset
argument_list|(
operator|&
name|set
argument_list|,
name|SIGALRM
argument_list|)
expr_stmt|;
name|sigaddset
argument_list|(
operator|&
name|set
argument_list|,
name|SIGINT
argument_list|)
expr_stmt|;
name|sigaddset
argument_list|(
operator|&
name|set
argument_list|,
name|ngx_signal_value
argument_list|(
name|NGX_RECONFIGURE_SIGNAL
argument_list|)
argument_list|)
expr_stmt|;
name|sigaddset
argument_list|(
operator|&
name|set
argument_list|,
name|ngx_signal_value
argument_list|(
name|NGX_REOPEN_SIGNAL
argument_list|)
argument_list|)
expr_stmt|;
name|sigaddset
argument_list|(
operator|&
name|set
argument_list|,
name|ngx_signal_value
argument_list|(
name|NGX_NOACCEPT_SIGNAL
argument_list|)
argument_list|)
expr_stmt|;
name|sigaddset
argument_list|(
operator|&
name|set
argument_list|,
name|ngx_signal_value
argument_list|(
name|NGX_TERMINATE_SIGNAL
argument_list|)
argument_list|)
expr_stmt|;
name|sigaddset
argument_list|(
operator|&
name|set
argument_list|,
name|ngx_signal_value
argument_list|(
name|NGX_SHUTDOWN_SIGNAL
argument_list|)
argument_list|)
expr_stmt|;
name|sigaddset
argument_list|(
operator|&
name|set
argument_list|,
name|ngx_signal_value
argument_list|(
name|NGX_CHANGEBIN_SIGNAL
argument_list|)
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
name|NULL
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
literal|"sigprocmask() failed"
argument_list|)
expr_stmt|;
block|}
name|sigemptyset
argument_list|(
operator|&
name|set
argument_list|)
expr_stmt|;
block|}
name|ngx_setproctitle
argument_list|(
literal|"master process"
argument_list|)
expr_stmt|;
name|ngx_new_binary
operator|=
literal|0
expr_stmt|;
name|delay
operator|=
literal|0
expr_stmt|;
name|signo
operator|=
literal|0
expr_stmt|;
name|live
operator|=
literal|0
expr_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
name|ngx_log_debug0
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|cycle
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"new cycle"
argument_list|)
expr_stmt|;
name|ccf
operator|=
operator|(
name|ngx_core_conf_t
operator|*
operator|)
name|ngx_get_conf
argument_list|(
name|cycle
operator|->
name|conf_ctx
argument_list|,
name|ngx_core_module
argument_list|)
expr_stmt|;
if|if
condition|(
name|ngx_process
operator|==
name|NGX_PROCESS_MASTER
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
operator|(
name|ngx_uint_t
operator|)
name|ccf
operator|->
name|worker_processes
condition|;
name|i
operator|++
control|)
block|{
name|ngx_spawn_process
argument_list|(
name|cycle
argument_list|,
name|ngx_worker_process_cycle
argument_list|,
name|NULL
argument_list|,
literal|"worker process"
argument_list|,
name|NGX_PROCESS_RESPAWN
argument_list|)
expr_stmt|;
block|}
comment|/*              * we have to limit the maximum life time of the worker processes              * by 10 days because our millisecond event timer is limited              * by 24 days on 32-bit platforms              */
name|itv
operator|.
name|it_interval
operator|.
name|tv_sec
operator|=
literal|0
expr_stmt|;
name|itv
operator|.
name|it_interval
operator|.
name|tv_usec
operator|=
literal|0
expr_stmt|;
name|itv
operator|.
name|it_value
operator|.
name|tv_sec
operator|=
literal|10
operator|*
literal|24
operator|*
literal|60
operator|*
literal|60
expr_stmt|;
name|itv
operator|.
name|it_value
operator|.
name|tv_usec
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|setitimer
argument_list|(
name|ITIMER_REAL
argument_list|,
operator|&
name|itv
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
name|NGX_LOG_ALERT
argument_list|,
name|cycle
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"setitimer() failed"
argument_list|)
expr_stmt|;
block|}
name|live
operator|=
literal|1
expr_stmt|;
block|}
else|else
block|{
name|ngx_init_temp_number
argument_list|()
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|ngx_modules
index|[
name|i
index|]
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|ngx_modules
index|[
name|i
index|]
operator|->
name|init_process
condition|)
block|{
if|if
condition|(
name|ngx_modules
index|[
name|i
index|]
operator|->
name|init_process
argument_list|(
name|cycle
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
comment|/* fatal */
name|exit
argument_list|(
literal|2
argument_list|)
expr_stmt|;
block|}
block|}
block|}
block|}
comment|/* a cycle with the same configuration because a new one is invalid */
for|for
control|(
init|;
condition|;
control|)
block|{
comment|/* an event loop */
for|for
control|(
init|;
condition|;
control|)
block|{
if|if
condition|(
name|ngx_process
operator|==
name|NGX_PROCESS_MASTER
condition|)
block|{
if|if
condition|(
name|delay
condition|)
block|{
name|delay
operator|*=
literal|2
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
literal|"temination cycle: %d"
argument_list|,
name|delay
argument_list|)
expr_stmt|;
name|itv
operator|.
name|it_interval
operator|.
name|tv_sec
operator|=
literal|0
expr_stmt|;
name|itv
operator|.
name|it_interval
operator|.
name|tv_usec
operator|=
literal|0
expr_stmt|;
name|itv
operator|.
name|it_value
operator|.
name|tv_sec
operator|=
name|delay
operator|/
literal|1000
expr_stmt|;
name|itv
operator|.
name|it_value
operator|.
name|tv_usec
operator|=
operator|(
name|delay
operator|%
literal|1000
operator|)
operator|*
literal|1000
expr_stmt|;
if|if
condition|(
name|setitimer
argument_list|(
name|ITIMER_REAL
argument_list|,
operator|&
name|itv
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
name|NGX_LOG_ALERT
argument_list|,
name|cycle
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"setitimer() failed"
argument_list|)
expr_stmt|;
block|}
block|}
name|ngx_log_debug0
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|cycle
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"sigsuspend"
argument_list|)
expr_stmt|;
name|sigsuspend
argument_list|(
operator|&
name|set
argument_list|)
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
name|ngx_log_debug0
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|cycle
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"wake up"
argument_list|)
expr_stmt|;
block|}
else|else
block|{
comment|/* NGX_PROCESS_SINGLE */
name|ngx_log_debug0
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|cycle
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"worker cycle"
argument_list|)
expr_stmt|;
name|ngx_process_events
argument_list|(
name|cycle
argument_list|)
expr_stmt|;
name|live
operator|=
literal|0
expr_stmt|;
block|}
if|if
condition|(
name|ngx_reap
condition|)
block|{
name|ngx_reap
operator|=
literal|0
expr_stmt|;
name|ngx_log_debug0
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|cycle
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"reap childs"
argument_list|)
expr_stmt|;
name|live
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
name|ngx_last_process
condition|;
name|i
operator|++
control|)
block|{
name|ngx_log_debug5
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|cycle
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"child: "
name|PID_T_FMT
literal|" e:%d t:%d d:%d r:%d"
argument_list|,
name|ngx_processes
index|[
name|i
index|]
operator|.
name|pid
argument_list|,
name|ngx_processes
index|[
name|i
index|]
operator|.
name|exiting
argument_list|,
name|ngx_processes
index|[
name|i
index|]
operator|.
name|exited
argument_list|,
name|ngx_processes
index|[
name|i
index|]
operator|.
name|detached
argument_list|,
name|ngx_processes
index|[
name|i
index|]
operator|.
name|respawn
argument_list|)
expr_stmt|;
if|if
condition|(
name|ngx_processes
index|[
name|i
index|]
operator|.
name|exited
condition|)
block|{
if|if
condition|(
name|ngx_processes
index|[
name|i
index|]
operator|.
name|respawn
operator|&&
operator|!
name|ngx_processes
index|[
name|i
index|]
operator|.
name|exiting
operator|&&
operator|!
name|ngx_terminate
operator|&&
operator|!
name|ngx_quit
condition|)
block|{
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
continue|continue;
block|}
name|live
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|ngx_processes
index|[
name|i
index|]
operator|.
name|pid
operator|==
name|ngx_new_binary
condition|)
block|{
name|ngx_new_binary
operator|=
literal|0
expr_stmt|;
comment|/* TODO: if (ngx_noaccept) ngx_configure = 1 */
block|}
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
block|}
if|else if
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
name|detached
condition|)
block|{
name|live
operator|=
literal|1
expr_stmt|;
block|}
block|}
block|}
if|if
condition|(
operator|!
name|live
operator|&&
operator|(
name|ngx_terminate
operator|||
name|ngx_quit
operator|)
condition|)
block|{
name|ngx_master_exit
argument_list|(
name|cycle
argument_list|,
name|ctx
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|ngx_terminate
condition|)
block|{
if|if
condition|(
name|delay
operator|==
literal|0
condition|)
block|{
name|delay
operator|=
literal|50
expr_stmt|;
block|}
if|if
condition|(
name|delay
operator|>
literal|1000
condition|)
block|{
name|signo
operator|=
name|SIGKILL
expr_stmt|;
block|}
else|else
block|{
name|signo
operator|=
name|ngx_signal_value
argument_list|(
name|NGX_TERMINATE_SIGNAL
argument_list|)
expr_stmt|;
block|}
block|}
if|else if
condition|(
name|ngx_quit
condition|)
block|{
name|signo
operator|=
name|ngx_signal_value
argument_list|(
name|NGX_SHUTDOWN_SIGNAL
argument_list|)
expr_stmt|;
block|}
if|else if
condition|(
name|ngx_timer
condition|)
block|{
name|signo
operator|=
name|ngx_signal_value
argument_list|(
name|NGX_SHUTDOWN_SIGNAL
argument_list|)
expr_stmt|;
block|}
else|else
block|{
if|if
condition|(
name|ngx_noaccept
condition|)
block|{
name|signo
operator|=
name|ngx_signal_value
argument_list|(
name|NGX_SHUTDOWN_SIGNAL
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|ngx_change_binary
condition|)
block|{
name|ngx_change_binary
operator|=
literal|0
expr_stmt|;
name|ngx_log_error
argument_list|(
name|NGX_LOG_INFO
argument_list|,
name|cycle
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"changing binary"
argument_list|)
expr_stmt|;
name|ngx_new_binary
operator|=
name|ngx_exec_new_binary
argument_list|(
name|cycle
argument_list|,
name|ctx
operator|->
name|argv
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|ngx_reconfigure
condition|)
block|{
name|signo
operator|=
name|ngx_signal_value
argument_list|(
name|NGX_SHUTDOWN_SIGNAL
argument_list|)
expr_stmt|;
name|ngx_log_error
argument_list|(
name|NGX_LOG_INFO
argument_list|,
name|cycle
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"reconfiguring"
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|ngx_reopen
condition|)
block|{
if|if
condition|(
name|ngx_process
operator|==
name|NGX_PROCESS_MASTER
condition|)
block|{
name|signo
operator|=
name|ngx_signal_value
argument_list|(
name|NGX_REOPEN_SIGNAL
argument_list|)
expr_stmt|;
name|ngx_reopen
operator|=
literal|0
expr_stmt|;
block|}
else|else
block|{
comment|/* NGX_PROCESS_SINGLE */
name|ngx_reopen
operator|=
literal|0
expr_stmt|;
block|}
name|ngx_log_error
argument_list|(
name|NGX_LOG_INFO
argument_list|,
name|cycle
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"reopening logs"
argument_list|)
expr_stmt|;
name|ngx_reopen_files
argument_list|(
name|cycle
argument_list|,
name|ccf
operator|->
name|user
argument_list|)
expr_stmt|;
block|}
block|}
if|if
condition|(
name|signo
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
name|detached
condition|)
block|{
continue|continue;
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
name|signo
operator|=
literal|0
expr_stmt|;
block|}
if|if
condition|(
name|ngx_reopen
operator|||
name|ngx_reconfigure
operator|||
name|ngx_timer
condition|)
block|{
break|break;
block|}
block|}
if|if
condition|(
name|ngx_reopen
condition|)
block|{
name|ngx_reopen
operator|=
literal|0
expr_stmt|;
block|}
if|else if
condition|(
name|ngx_timer
condition|)
block|{
name|ngx_timer
operator|=
literal|0
expr_stmt|;
block|}
if|else if
condition|(
name|ngx_noaccept
condition|)
block|{
name|ngx_noaccept
operator|=
literal|0
expr_stmt|;
name|ngx_reconfigure
operator|=
literal|0
expr_stmt|;
block|}
else|else
block|{
name|cycle
operator|=
name|ngx_init_cycle
argument_list|(
name|cycle
argument_list|)
expr_stmt|;
if|if
condition|(
name|cycle
operator|==
name|NULL
condition|)
block|{
name|cycle
operator|=
operator|(
name|ngx_cycle_t
operator|*
operator|)
name|ngx_cycle
expr_stmt|;
continue|continue;
block|}
name|ngx_cycle
operator|=
name|cycle
expr_stmt|;
name|ngx_reconfigure
operator|=
literal|0
expr_stmt|;
block|}
break|break;
block|}
block|}
block|}
end_function

begin_function
DECL|function|ngx_master_exit (ngx_cycle_t * cycle,ngx_master_ctx_t * ctx)
specifier|static
name|void
name|ngx_master_exit
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|,
name|ngx_master_ctx_t
modifier|*
name|ctx
parameter_list|)
block|{
name|ngx_delete_pidfile
argument_list|(
name|cycle
argument_list|)
expr_stmt|;
name|ngx_log_error
argument_list|(
name|NGX_LOG_INFO
argument_list|,
name|cycle
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"exit"
argument_list|)
expr_stmt|;
name|exit
argument_list|(
literal|0
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|ngx_worker_process_cycle (ngx_cycle_t * cycle,void * data)
specifier|static
name|void
name|ngx_worker_process_cycle
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
name|sigset_t
name|set
decl_stmt|;
name|ngx_uint_t
name|i
decl_stmt|;
name|ngx_listening_t
modifier|*
name|ls
decl_stmt|;
name|ngx_core_conf_t
modifier|*
name|ccf
decl_stmt|;
if|#
directive|if
operator|(
name|NGX_THREADS
operator|)
name|ngx_tid_t
name|tid
decl_stmt|;
endif|#
directive|endif
name|ngx_process
operator|=
name|NGX_PROCESS_WORKER
expr_stmt|;
name|ngx_last_process
operator|=
literal|0
expr_stmt|;
name|ccf
operator|=
operator|(
name|ngx_core_conf_t
operator|*
operator|)
name|ngx_get_conf
argument_list|(
name|cycle
operator|->
name|conf_ctx
argument_list|,
name|ngx_core_module
argument_list|)
expr_stmt|;
if|if
condition|(
name|ccf
operator|->
name|group
operator|!=
operator|(
name|gid_t
operator|)
name|NGX_CONF_UNSET
condition|)
block|{
if|if
condition|(
name|setuid
argument_list|(
name|ccf
operator|->
name|group
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
literal|"setgid(%d) failed"
argument_list|,
name|ccf
operator|->
name|group
argument_list|)
expr_stmt|;
comment|/* fatal */
name|exit
argument_list|(
literal|2
argument_list|)
expr_stmt|;
block|}
block|}
if|if
condition|(
name|ccf
operator|->
name|user
operator|!=
operator|(
name|uid_t
operator|)
name|NGX_CONF_UNSET
operator|&&
name|geteuid
argument_list|()
operator|==
literal|0
condition|)
block|{
if|if
condition|(
name|setuid
argument_list|(
name|ccf
operator|->
name|user
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
literal|"setuid(%d) failed"
argument_list|,
name|ccf
operator|->
name|user
argument_list|)
expr_stmt|;
comment|/* fatal */
name|exit
argument_list|(
literal|2
argument_list|)
expr_stmt|;
block|}
block|}
if|#
directive|if
operator|(
name|HAVE_PR_SET_DUMPABLE
operator|)
comment|/* allow coredump after setuid() in Linux 2.4.x */
if|if
condition|(
name|prctl
argument_list|(
name|PR_SET_DUMPABLE
argument_list|,
literal|1
argument_list|,
literal|0
argument_list|,
literal|0
argument_list|,
literal|0
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
literal|"prctl(PR_SET_DUMPABLE) failed"
argument_list|)
expr_stmt|;
block|}
endif|#
directive|endif
name|sigemptyset
argument_list|(
operator|&
name|set
argument_list|)
expr_stmt|;
if|if
condition|(
name|sigprocmask
argument_list|(
name|SIG_SETMASK
argument_list|,
operator|&
name|set
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
name|NGX_LOG_ALERT
argument_list|,
name|cycle
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"sigprocmask() failed"
argument_list|)
expr_stmt|;
block|}
name|ngx_init_temp_number
argument_list|()
expr_stmt|;
comment|/*      * disable deleting previous events for the listening sockets because      * in the worker processes there are no events at all at this point      */
name|ls
operator|=
name|cycle
operator|->
name|listening
operator|.
name|elts
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|cycle
operator|->
name|listening
operator|.
name|nelts
condition|;
name|i
operator|++
control|)
block|{
name|ls
index|[
name|i
index|]
operator|.
name|remain
operator|=
literal|0
expr_stmt|;
block|}
for|for
control|(
name|i
operator|=
literal|0
init|;
name|ngx_modules
index|[
name|i
index|]
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|ngx_modules
index|[
name|i
index|]
operator|->
name|init_process
condition|)
block|{
if|if
condition|(
name|ngx_modules
index|[
name|i
index|]
operator|->
name|init_process
argument_list|(
name|cycle
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
comment|/* fatal */
name|exit
argument_list|(
literal|2
argument_list|)
expr_stmt|;
block|}
block|}
block|}
name|ngx_setproctitle
argument_list|(
literal|"worker process"
argument_list|)
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_THREADS
operator|)
if|if
condition|(
name|ngx_init_threads
argument_list|(
literal|5
argument_list|,
literal|128
operator|*
literal|1024
operator|*
literal|1024
argument_list|,
name|cycle
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
comment|/* fatal */
name|exit
argument_list|(
literal|2
argument_list|)
expr_stmt|;
block|}
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
literal|1
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|ngx_create_thread
argument_list|(
operator|&
name|tid
argument_list|,
name|ngx_worker_thread_cycle
argument_list|,
name|cycle
argument_list|,
name|cycle
operator|->
name|log
argument_list|)
operator|!=
literal|0
condition|)
block|{
comment|/* fatal */
name|exit
argument_list|(
literal|2
argument_list|)
expr_stmt|;
block|}
block|}
endif|#
directive|endif
for|for
control|(
init|;
condition|;
control|)
block|{
name|ngx_log_debug0
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|cycle
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"worker cycle"
argument_list|)
expr_stmt|;
name|ngx_process_events
argument_list|(
name|cycle
argument_list|)
expr_stmt|;
if|if
condition|(
name|ngx_terminate
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_INFO
argument_list|,
name|cycle
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"exiting"
argument_list|)
expr_stmt|;
name|exit
argument_list|(
literal|0
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|ngx_quit
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_INFO
argument_list|,
name|cycle
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"gracefully shutting down"
argument_list|)
expr_stmt|;
name|ngx_setproctitle
argument_list|(
literal|"worker process is shutting down"
argument_list|)
expr_stmt|;
break|break;
block|}
if|if
condition|(
name|ngx_reopen
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_INFO
argument_list|,
name|cycle
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"reopen logs"
argument_list|)
expr_stmt|;
name|ngx_reopen_files
argument_list|(
name|cycle
argument_list|,
operator|-
literal|1
argument_list|)
expr_stmt|;
name|ngx_reopen
operator|=
literal|0
expr_stmt|;
block|}
block|}
name|ngx_close_listening_sockets
argument_list|(
name|cycle
argument_list|)
expr_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
if|if
condition|(
name|ngx_event_timer_rbtree
operator|==
operator|&
name|ngx_event_timer_sentinel
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_INFO
argument_list|,
name|cycle
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"exiting"
argument_list|)
expr_stmt|;
name|exit
argument_list|(
literal|0
argument_list|)
expr_stmt|;
block|}
name|ngx_log_debug0
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|cycle
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"worker cycle"
argument_list|)
expr_stmt|;
name|ngx_process_events
argument_list|(
name|cycle
argument_list|)
expr_stmt|;
if|if
condition|(
name|ngx_reopen
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_INFO
argument_list|,
name|cycle
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"reopen logs"
argument_list|)
expr_stmt|;
name|ngx_reopen_files
argument_list|(
name|cycle
argument_list|,
operator|-
literal|1
argument_list|)
expr_stmt|;
name|ngx_reopen
operator|=
literal|0
expr_stmt|;
block|}
block|}
block|}
end_function

begin_if
if|#
directive|if
operator|(
name|NGX_THREADS
operator|)
end_if

begin_function
DECL|function|ngx_worker_thread_cycle (void * data)
name|int
name|ngx_worker_thread_cycle
parameter_list|(
name|void
modifier|*
name|data
parameter_list|)
block|{
name|ngx_cycle_t
modifier|*
name|cycle
init|=
name|data
decl_stmt|;
name|ngx_err_t
name|err
decl_stmt|;
name|sigset_t
name|set
decl_stmt|;
name|struct
name|timeval
name|tv
decl_stmt|;
name|sigfillset
argument_list|(
operator|&
name|set
argument_list|)
expr_stmt|;
name|sigdelset
argument_list|(
operator|&
name|set
argument_list|,
name|SIGALRM
argument_list|)
expr_stmt|;
name|sigdelset
argument_list|(
operator|&
name|set
argument_list|,
name|ngx_signal_value
argument_list|(
name|NGX_TERMINATE_SIGNAL
argument_list|)
argument_list|)
expr_stmt|;
name|sigdelset
argument_list|(
operator|&
name|set
argument_list|,
name|ngx_signal_value
argument_list|(
name|NGX_SHUTDOWN_SIGNAL
argument_list|)
argument_list|)
expr_stmt|;
name|err
operator|=
name|ngx_thread_sigmask
argument_list|(
name|SIG_BLOCK
argument_list|,
operator|&
name|set
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
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
name|ngx_thread_sigmask_n
literal|" failed"
argument_list|)
expr_stmt|;
return|return
literal|1
return|;
block|}
comment|/* STUB */
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_CORE
argument_list|,
name|ngx_cycle
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"thread %d started"
argument_list|,
name|ngx_thread_self
argument_list|()
argument_list|)
expr_stmt|;
name|ngx_setproctitle
argument_list|(
literal|"worker thread"
argument_list|)
expr_stmt|;
name|sleep
argument_list|(
literal|5
argument_list|)
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
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_CORE
argument_list|,
name|ngx_cycle
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"thread %d done"
argument_list|,
name|ngx_thread_self
argument_list|()
argument_list|)
expr_stmt|;
return|return
literal|1
return|;
block|}
end_function

begin_endif
endif|#
directive|endif
end_endif

end_unit

