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
file|<nginx.h>
end_include

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_add_inherited_sockets
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
name|ngx_getopt
parameter_list|(
name|ngx_master_ctx_t
modifier|*
name|ctx
parameter_list|,
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
name|ngx_core_module_create_conf
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
name|ngx_core_module_init_conf
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

begin_function_decl
specifier|static
name|char
modifier|*
name|ngx_set_user
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
function_decl|;
end_function_decl

begin_decl_stmt
DECL|variable|ngx_core_commands
specifier|static
name|ngx_command_t
name|ngx_core_commands
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"daemon"
argument_list|)
block|,
name|NGX_MAIN_CONF
operator||
name|NGX_DIRECT_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_conf_set_flag_slot
block|,
literal|0
block|,
name|offsetof
argument_list|(
name|ngx_core_conf_t
argument_list|,
name|daemon
argument_list|)
block|,
name|NULL
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"master_process"
argument_list|)
block|,
name|NGX_MAIN_CONF
operator||
name|NGX_DIRECT_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_conf_set_flag_slot
block|,
literal|0
block|,
name|offsetof
argument_list|(
name|ngx_core_conf_t
argument_list|,
name|master
argument_list|)
block|,
name|NULL
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"worker_processes"
argument_list|)
block|,
name|NGX_MAIN_CONF
operator||
name|NGX_DIRECT_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_conf_set_num_slot
block|,
literal|0
block|,
name|offsetof
argument_list|(
name|ngx_core_conf_t
argument_list|,
name|worker_processes
argument_list|)
block|,
name|NULL
block|}
block|,
if|#
directive|if
operator|(
name|NGX_THREADS
operator|)
block|{
name|ngx_string
argument_list|(
literal|"worker_threads"
argument_list|)
block|,
name|NGX_MAIN_CONF
operator||
name|NGX_DIRECT_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_conf_set_num_slot
block|,
literal|0
block|,
name|offsetof
argument_list|(
name|ngx_core_conf_t
argument_list|,
name|worker_threads
argument_list|)
block|,
name|NULL
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"thread_stack_size"
argument_list|)
block|,
name|NGX_MAIN_CONF
operator||
name|NGX_DIRECT_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_conf_set_size_slot
block|,
literal|0
block|,
name|offsetof
argument_list|(
name|ngx_core_conf_t
argument_list|,
name|thread_stack_size
argument_list|)
block|,
name|NULL
block|}
block|,
endif|#
directive|endif
block|{
name|ngx_string
argument_list|(
literal|"user"
argument_list|)
block|,
name|NGX_MAIN_CONF
operator||
name|NGX_DIRECT_CONF
operator||
name|NGX_CONF_TAKE12
block|,
name|ngx_set_user
block|,
literal|0
block|,
literal|0
block|,
name|NULL
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"pid"
argument_list|)
block|,
name|NGX_MAIN_CONF
operator||
name|NGX_DIRECT_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_conf_set_str_slot
block|,
literal|0
block|,
name|offsetof
argument_list|(
name|ngx_core_conf_t
argument_list|,
name|pid
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
DECL|variable|ngx_core_module_ctx
specifier|static
name|ngx_core_module_t
name|ngx_core_module_ctx
init|=
block|{
name|ngx_string
argument_list|(
literal|"core"
argument_list|)
block|,
name|ngx_core_module_create_conf
block|,
name|ngx_core_module_init_conf
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_core_module
name|ngx_module_t
name|ngx_core_module
init|=
block|{
name|NGX_MODULE
block|,
operator|&
name|ngx_core_module_ctx
block|,
comment|/* module context */
name|ngx_core_commands
block|,
comment|/* module directives */
name|NGX_CORE_MODULE
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

begin_decl_stmt
DECL|variable|ngx_max_module
name|ngx_uint_t
name|ngx_max_module
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|main (int argc,char * const * argv)
name|int
name|main
parameter_list|(
name|int
name|argc
parameter_list|,
name|char
modifier|*
specifier|const
modifier|*
name|argv
parameter_list|)
block|{
name|ngx_int_t
name|i
decl_stmt|;
name|ngx_log_t
modifier|*
name|log
decl_stmt|;
name|ngx_cycle_t
modifier|*
name|cycle
decl_stmt|,
name|init_cycle
decl_stmt|;
name|ngx_core_conf_t
modifier|*
name|ccf
decl_stmt|;
name|ngx_master_ctx_t
name|ctx
decl_stmt|;
if|#
directive|if
name|defined
name|__FreeBSD__
name|ngx_debug_init
argument_list|()
expr_stmt|;
endif|#
directive|endif
comment|/* TODO */
name|ngx_max_sockets
operator|=
operator|-
literal|1
expr_stmt|;
name|ngx_time_init
argument_list|()
expr_stmt|;
if|#
directive|if
operator|(
name|HAVE_PCRE
operator|)
name|ngx_regex_init
argument_list|()
expr_stmt|;
endif|#
directive|endif
name|ngx_pid
operator|=
name|ngx_getpid
argument_list|()
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|log
operator|=
name|ngx_log_init_errlog
argument_list|()
operator|)
condition|)
block|{
return|return
literal|1
return|;
block|}
if|#
directive|if
operator|(
name|NGX_OPENSSL
operator|)
name|ngx_ssl_init
argument_list|(
name|log
argument_list|)
expr_stmt|;
endif|#
directive|endif
comment|/* init_cycle->log is required for signal handlers and ngx_getopt() */
name|ngx_memzero
argument_list|(
operator|&
name|init_cycle
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_cycle_t
argument_list|)
argument_list|)
expr_stmt|;
name|init_cycle
operator|.
name|log
operator|=
name|log
expr_stmt|;
name|ngx_cycle
operator|=
operator|&
name|init_cycle
expr_stmt|;
name|ngx_memzero
argument_list|(
operator|&
name|ctx
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_master_ctx_t
argument_list|)
argument_list|)
expr_stmt|;
name|ctx
operator|.
name|argc
operator|=
name|argc
expr_stmt|;
name|ctx
operator|.
name|argv
operator|=
name|argv
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|init_cycle
operator|.
name|pool
operator|=
name|ngx_create_pool
argument_list|(
literal|1024
argument_list|,
name|log
argument_list|)
operator|)
condition|)
block|{
return|return
literal|1
return|;
block|}
if|if
condition|(
name|ngx_getopt
argument_list|(
operator|&
name|ctx
argument_list|,
operator|&
name|init_cycle
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
literal|1
return|;
block|}
if|if
condition|(
name|ngx_os_init
argument_list|(
name|log
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
literal|1
return|;
block|}
if|if
condition|(
name|ngx_add_inherited_sockets
argument_list|(
operator|&
name|init_cycle
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
literal|1
return|;
block|}
name|ngx_max_module
operator|=
literal|0
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
name|ngx_modules
index|[
name|i
index|]
operator|->
name|index
operator|=
name|ngx_max_module
operator|++
expr_stmt|;
block|}
name|cycle
operator|=
name|ngx_init_cycle
argument_list|(
operator|&
name|init_cycle
argument_list|)
expr_stmt|;
if|if
condition|(
name|cycle
operator|==
name|NULL
condition|)
block|{
if|if
condition|(
name|ngx_test_config
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
literal|"the configuration file %s test failed"
argument_list|,
name|init_cycle
operator|.
name|conf_file
operator|.
name|data
argument_list|)
expr_stmt|;
block|}
return|return
literal|1
return|;
block|}
if|if
condition|(
name|ngx_test_config
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_INFO
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"the configuration file %s was tested successfully"
argument_list|,
name|init_cycle
operator|.
name|conf_file
operator|.
name|data
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
name|ngx_cycle
operator|=
name|cycle
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
name|ngx_process
operator|=
name|ccf
operator|->
name|master
condition|?
name|NGX_PROCESS_MASTER
else|:
name|NGX_PROCESS_SINGLE
expr_stmt|;
if|#
directive|if
operator|(
name|WIN32
operator|)
if|#
directive|if
literal|0
block_content|TODO:      if (ccf->run_as_service) {         if (ngx_service(cycle->log) == NGX_ERROR) {             return 1;         }          return 0;     }
endif|#
directive|endif
else|#
directive|else
if|if
condition|(
operator|!
name|ngx_inherited
operator|&&
name|ccf
operator|->
name|daemon
condition|)
block|{
if|if
condition|(
name|ngx_daemon
argument_list|(
name|cycle
operator|->
name|log
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
literal|1
return|;
block|}
name|ngx_daemonized
operator|=
literal|1
expr_stmt|;
block|}
if|if
condition|(
name|ngx_create_pidfile
argument_list|(
name|cycle
argument_list|,
name|NULL
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
literal|1
return|;
block|}
endif|#
directive|endif
if|if
condition|(
name|ngx_process
operator|==
name|NGX_PROCESS_MASTER
condition|)
block|{
name|ngx_master_process_cycle
argument_list|(
name|cycle
argument_list|,
operator|&
name|ctx
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|ngx_single_process_cycle
argument_list|(
name|cycle
argument_list|,
operator|&
name|ctx
argument_list|)
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
DECL|function|ngx_add_inherited_sockets (ngx_cycle_t * cycle)
specifier|static
name|ngx_int_t
name|ngx_add_inherited_sockets
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
block|{
name|u_char
modifier|*
name|p
decl_stmt|,
modifier|*
name|v
decl_stmt|,
modifier|*
name|inherited
decl_stmt|;
name|ngx_socket_t
name|s
decl_stmt|;
name|ngx_listening_t
modifier|*
name|ls
decl_stmt|;
name|inherited
operator|=
operator|(
name|u_char
operator|*
operator|)
name|getenv
argument_list|(
name|NGINX_VAR
argument_list|)
expr_stmt|;
if|if
condition|(
name|inherited
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_OK
return|;
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
literal|"using inherited sockets from \"%s\""
argument_list|,
name|inherited
argument_list|)
expr_stmt|;
name|ngx_init_array
argument_list|(
name|cycle
operator|->
name|listening
argument_list|,
name|cycle
operator|->
name|pool
argument_list|,
literal|10
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_listening_t
argument_list|)
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
for|for
control|(
name|p
operator|=
name|inherited
operator|,
name|v
operator|=
name|p
init|;
operator|*
name|p
condition|;
name|p
operator|++
control|)
block|{
if|if
condition|(
operator|*
name|p
operator|==
literal|':'
operator|||
operator|*
name|p
operator|==
literal|';'
condition|)
block|{
name|s
operator|=
name|ngx_atoi
argument_list|(
name|v
argument_list|,
name|p
operator|-
name|v
argument_list|)
expr_stmt|;
if|if
condition|(
name|s
operator|==
name|NGX_ERROR
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
literal|"invalid socket number \"%s\" in "
name|NGINX_VAR
literal|" enviroment variable, "
literal|"ignoring the rest of the variable"
argument_list|,
name|v
argument_list|)
expr_stmt|;
break|break;
block|}
name|v
operator|=
name|p
operator|+
literal|1
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|ls
operator|=
name|ngx_push_array
argument_list|(
operator|&
name|cycle
operator|->
name|listening
argument_list|)
operator|)
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|ls
operator|->
name|fd
operator|=
name|s
expr_stmt|;
block|}
block|}
name|ngx_inherited
operator|=
literal|1
expr_stmt|;
return|return
name|ngx_set_inherited_sockets
argument_list|(
name|cycle
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|ngx_exec_new_binary (ngx_cycle_t * cycle,char * const * argv)
name|ngx_pid_t
name|ngx_exec_new_binary
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|,
name|char
modifier|*
specifier|const
modifier|*
name|argv
parameter_list|)
block|{
name|char
modifier|*
name|env
index|[
literal|2
index|]
decl_stmt|,
modifier|*
name|var
decl_stmt|,
modifier|*
name|p
decl_stmt|;
name|ngx_uint_t
name|i
decl_stmt|;
name|ngx_pid_t
name|pid
decl_stmt|;
name|ngx_exec_ctx_t
name|ctx
decl_stmt|;
name|ngx_listening_t
modifier|*
name|ls
decl_stmt|;
name|ctx
operator|.
name|path
operator|=
name|argv
index|[
literal|0
index|]
expr_stmt|;
name|ctx
operator|.
name|name
operator|=
literal|"new binary process"
expr_stmt|;
name|ctx
operator|.
name|argv
operator|=
name|argv
expr_stmt|;
name|var
operator|=
name|ngx_alloc
argument_list|(
sizeof|sizeof
argument_list|(
name|NGINX_VAR
argument_list|)
operator|+
name|cycle
operator|->
name|listening
operator|.
name|nelts
operator|*
operator|(
name|NGX_INT32_LEN
operator|+
literal|1
operator|)
operator|+
literal|2
argument_list|,
name|cycle
operator|->
name|log
argument_list|)
expr_stmt|;
name|p
operator|=
operator|(
name|char
operator|*
operator|)
name|ngx_cpymem
argument_list|(
name|var
argument_list|,
name|NGINX_VAR
literal|"="
argument_list|,
sizeof|sizeof
argument_list|(
name|NGINX_VAR
argument_list|)
argument_list|)
expr_stmt|;
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
name|p
operator|+=
name|ngx_snprintf
argument_list|(
name|p
argument_list|,
name|NGX_INT32_LEN
operator|+
literal|2
argument_list|,
literal|"%u;"
argument_list|,
name|ls
index|[
name|i
index|]
operator|.
name|fd
argument_list|)
expr_stmt|;
block|}
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_CORE
argument_list|,
name|cycle
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"inherited: %s"
argument_list|,
name|var
argument_list|)
expr_stmt|;
name|env
index|[
literal|0
index|]
operator|=
name|var
expr_stmt|;
name|env
index|[
literal|1
index|]
operator|=
name|NULL
expr_stmt|;
name|ctx
operator|.
name|envp
operator|=
operator|(
name|char
operator|*
specifier|const
operator|*
operator|)
operator|&
name|env
expr_stmt|;
name|pid
operator|=
name|ngx_execute
argument_list|(
name|cycle
argument_list|,
operator|&
name|ctx
argument_list|)
expr_stmt|;
name|ngx_free
argument_list|(
name|var
argument_list|)
expr_stmt|;
return|return
name|pid
return|;
block|}
end_function

begin_function
DECL|function|ngx_getopt (ngx_master_ctx_t * ctx,ngx_cycle_t * cycle)
specifier|static
name|ngx_int_t
name|ngx_getopt
parameter_list|(
name|ngx_master_ctx_t
modifier|*
name|ctx
parameter_list|,
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
block|{
name|ngx_int_t
name|i
decl_stmt|;
for|for
control|(
name|i
operator|=
literal|1
init|;
name|i
operator|<
name|ctx
operator|->
name|argc
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|ctx
operator|->
name|argv
index|[
name|i
index|]
index|[
literal|0
index|]
operator|!=
literal|'-'
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
literal|"invalid option: \"%s\""
argument_list|,
name|ctx
operator|->
name|argv
index|[
name|i
index|]
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
switch|switch
condition|(
name|ctx
operator|->
name|argv
index|[
name|i
index|]
index|[
literal|1
index|]
condition|)
block|{
case|case
literal|'t'
case|:
name|ngx_test_config
operator|=
literal|1
expr_stmt|;
break|break;
case|case
literal|'c'
case|:
if|if
condition|(
name|ctx
operator|->
name|argv
index|[
name|i
operator|+
literal|1
index|]
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
literal|0
argument_list|,
literal|"the option: \"%s\" requires file name"
argument_list|,
name|ctx
operator|->
name|argv
index|[
name|i
index|]
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
name|cycle
operator|->
name|conf_file
operator|.
name|data
operator|=
operator|(
name|u_char
operator|*
operator|)
name|ctx
operator|->
name|argv
index|[
operator|++
name|i
index|]
expr_stmt|;
name|cycle
operator|->
name|conf_file
operator|.
name|len
operator|=
name|ngx_strlen
argument_list|(
name|cycle
operator|->
name|conf_file
operator|.
name|data
argument_list|)
expr_stmt|;
break|break;
default|default:
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
literal|"invalid option: \"%s\""
argument_list|,
name|ctx
operator|->
name|argv
index|[
name|i
index|]
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
block|}
if|if
condition|(
name|cycle
operator|->
name|conf_file
operator|.
name|data
operator|==
name|NULL
condition|)
block|{
name|cycle
operator|->
name|conf_file
operator|.
name|len
operator|=
sizeof|sizeof
argument_list|(
name|NGX_CONF_PATH
argument_list|)
operator|-
literal|1
expr_stmt|;
name|cycle
operator|->
name|conf_file
operator|.
name|data
operator|=
operator|(
name|u_char
operator|*
operator|)
name|NGX_CONF_PATH
expr_stmt|;
block|}
if|if
condition|(
name|ngx_conf_full_name
argument_list|(
name|cycle
argument_list|,
operator|&
name|cycle
operator|->
name|conf_file
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
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
DECL|function|ngx_core_module_create_conf (ngx_cycle_t * cycle)
specifier|static
name|void
modifier|*
name|ngx_core_module_create_conf
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
block|{
name|ngx_core_conf_t
modifier|*
name|ccf
decl_stmt|;
if|if
condition|(
operator|!
operator|(
name|ccf
operator|=
name|ngx_pcalloc
argument_list|(
name|cycle
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_core_conf_t
argument_list|)
argument_list|)
operator|)
condition|)
block|{
return|return
name|NULL
return|;
block|}
comment|/* set by pcalloc()      *      * ccf->pid = NULL;      * ccf->newpid = NULL;      */
name|ccf
operator|->
name|daemon
operator|=
name|NGX_CONF_UNSET
expr_stmt|;
name|ccf
operator|->
name|master
operator|=
name|NGX_CONF_UNSET
expr_stmt|;
name|ccf
operator|->
name|worker_processes
operator|=
name|NGX_CONF_UNSET
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_THREADS
operator|)
name|ccf
operator|->
name|worker_threads
operator|=
name|NGX_CONF_UNSET
expr_stmt|;
name|ccf
operator|->
name|thread_stack_size
operator|=
name|NGX_CONF_UNSET
expr_stmt|;
endif|#
directive|endif
name|ccf
operator|->
name|user
operator|=
operator|(
name|ngx_uid_t
operator|)
name|NGX_CONF_UNSET
expr_stmt|;
name|ccf
operator|->
name|group
operator|=
operator|(
name|ngx_gid_t
operator|)
name|NGX_CONF_UNSET
expr_stmt|;
return|return
name|ccf
return|;
block|}
end_function

begin_function
DECL|function|ngx_core_module_init_conf (ngx_cycle_t * cycle,void * conf)
specifier|static
name|char
modifier|*
name|ngx_core_module_init_conf
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
name|ngx_core_conf_t
modifier|*
name|ccf
init|=
name|conf
decl_stmt|;
if|#
directive|if
operator|!
operator|(
name|WIN32
operator|)
name|struct
name|passwd
modifier|*
name|pwd
decl_stmt|;
name|struct
name|group
modifier|*
name|grp
decl_stmt|;
endif|#
directive|endif
name|ngx_conf_init_value
argument_list|(
name|ccf
operator|->
name|daemon
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|ngx_conf_init_value
argument_list|(
name|ccf
operator|->
name|master
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|ngx_conf_init_value
argument_list|(
name|ccf
operator|->
name|worker_processes
argument_list|,
literal|1
argument_list|)
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_THREADS
operator|)
name|ngx_conf_init_value
argument_list|(
name|ccf
operator|->
name|worker_threads
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|ngx_threads_n
operator|=
name|ccf
operator|->
name|worker_threads
expr_stmt|;
name|ngx_conf_init_size_value
argument_list|(
name|ccf
operator|->
name|thread_stack_size
argument_list|,
literal|2
operator|*
literal|1024
operator|*
literal|1024
argument_list|)
expr_stmt|;
endif|#
directive|endif
if|#
directive|if
operator|!
operator|(
name|WIN32
operator|)
if|if
condition|(
name|ccf
operator|->
name|user
operator|==
operator|(
name|uid_t
operator|)
name|NGX_CONF_UNSET
condition|)
block|{
name|pwd
operator|=
name|getpwnam
argument_list|(
literal|"nobody"
argument_list|)
expr_stmt|;
if|if
condition|(
name|pwd
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
literal|"getpwnam(\"nobody\") failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|ccf
operator|->
name|user
operator|=
name|pwd
operator|->
name|pw_uid
expr_stmt|;
name|grp
operator|=
name|getgrnam
argument_list|(
literal|"nobody"
argument_list|)
expr_stmt|;
if|if
condition|(
name|grp
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
literal|"getgrnam(\"nobody\") failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|ccf
operator|->
name|group
operator|=
name|grp
operator|->
name|gr_gid
expr_stmt|;
block|}
if|if
condition|(
name|ccf
operator|->
name|pid
operator|.
name|len
operator|==
literal|0
condition|)
block|{
name|ccf
operator|->
name|pid
operator|.
name|len
operator|=
sizeof|sizeof
argument_list|(
name|NGX_PID_PATH
argument_list|)
operator|-
literal|1
expr_stmt|;
name|ccf
operator|->
name|pid
operator|.
name|data
operator|=
name|NGX_PID_PATH
expr_stmt|;
block|}
if|if
condition|(
name|ngx_conf_full_name
argument_list|(
name|cycle
argument_list|,
operator|&
name|ccf
operator|->
name|pid
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|ccf
operator|->
name|newpid
operator|.
name|len
operator|=
name|ccf
operator|->
name|pid
operator|.
name|len
operator|+
sizeof|sizeof
argument_list|(
name|NGX_NEWPID_EXT
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|ccf
operator|->
name|newpid
operator|.
name|data
operator|=
name|ngx_palloc
argument_list|(
name|cycle
operator|->
name|pool
argument_list|,
name|ccf
operator|->
name|newpid
operator|.
name|len
argument_list|)
operator|)
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|ngx_memcpy
argument_list|(
name|ngx_cpymem
argument_list|(
name|ccf
operator|->
name|newpid
operator|.
name|data
argument_list|,
name|ccf
operator|->
name|pid
operator|.
name|data
argument_list|,
name|ccf
operator|->
name|pid
operator|.
name|len
argument_list|)
argument_list|,
name|NGX_NEWPID_EXT
argument_list|,
sizeof|sizeof
argument_list|(
name|NGX_NEWPID_EXT
argument_list|)
argument_list|)
expr_stmt|;
endif|#
directive|endif
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_set_user (ngx_conf_t * cf,ngx_command_t * cmd,void * conf)
specifier|static
name|char
modifier|*
name|ngx_set_user
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
if|#
directive|if
operator|(
name|WIN32
operator|)
name|ngx_conf_log_error
argument_list|(
name|NGX_LOG_WARN
argument_list|,
name|cf
argument_list|,
literal|0
argument_list|,
literal|"\"user\" is not supported, ignored"
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_OK
return|;
else|#
directive|else
name|ngx_core_conf_t
modifier|*
name|ccf
init|=
name|conf
decl_stmt|;
name|struct
name|passwd
modifier|*
name|pwd
decl_stmt|;
name|struct
name|group
modifier|*
name|grp
decl_stmt|;
name|ngx_str_t
modifier|*
name|value
decl_stmt|;
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
condition|)
block|{
return|return
literal|"is duplicate"
return|;
block|}
name|value
operator|=
operator|(
name|ngx_str_t
operator|*
operator|)
name|cf
operator|->
name|args
operator|->
name|elts
expr_stmt|;
name|pwd
operator|=
name|getpwnam
argument_list|(
operator|(
specifier|const
name|char
operator|*
operator|)
name|value
index|[
literal|1
index|]
operator|.
name|data
argument_list|)
expr_stmt|;
if|if
condition|(
name|pwd
operator|==
name|NULL
condition|)
block|{
name|ngx_conf_log_error
argument_list|(
name|NGX_LOG_EMERG
argument_list|,
name|cf
argument_list|,
name|ngx_errno
argument_list|,
literal|"getpwnam(%s) failed"
argument_list|,
name|value
index|[
literal|1
index|]
operator|.
name|data
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|ccf
operator|->
name|user
operator|=
name|pwd
operator|->
name|pw_uid
expr_stmt|;
if|if
condition|(
name|cf
operator|->
name|args
operator|->
name|nelts
operator|==
literal|2
condition|)
block|{
return|return
name|NGX_CONF_OK
return|;
block|}
name|grp
operator|=
name|getgrnam
argument_list|(
operator|(
specifier|const
name|char
operator|*
operator|)
name|value
index|[
literal|2
index|]
operator|.
name|data
argument_list|)
expr_stmt|;
if|if
condition|(
name|grp
operator|==
name|NULL
condition|)
block|{
name|ngx_conf_log_error
argument_list|(
name|NGX_LOG_EMERG
argument_list|,
name|cf
argument_list|,
name|ngx_errno
argument_list|,
literal|"getgrnam(%s) failed"
argument_list|,
name|value
index|[
literal|1
index|]
operator|.
name|data
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|ccf
operator|->
name|group
operator|=
name|grp
operator|->
name|gr_gid
expr_stmt|;
return|return
name|NGX_CONF_OK
return|;
endif|#
directive|endif
block|}
end_function

end_unit

