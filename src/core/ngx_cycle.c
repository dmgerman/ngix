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

begin_comment
comment|/* STUB */
end_comment

begin_include
include|#
directive|include
file|<nginx.h>
end_include

begin_function_decl
specifier|static
name|void
name|ngx_clean_old_cycles
parameter_list|(
name|ngx_event_t
modifier|*
name|ev
parameter_list|)
function_decl|;
end_function_decl

begin_decl_stmt
DECL|variable|ngx_cycle
specifier|volatile
name|ngx_cycle_t
modifier|*
name|ngx_cycle
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_old_cycles
name|ngx_array_t
name|ngx_old_cycles
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_temp_pool
specifier|static
name|ngx_pool_t
modifier|*
name|ngx_temp_pool
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_cleaner_event
specifier|static
name|ngx_event_t
name|ngx_cleaner_event
decl_stmt|;
end_decl_stmt

begin_comment
comment|/* STUB NAME */
end_comment

begin_decl_stmt
DECL|variable|dumb
specifier|static
name|ngx_connection_t
name|dumb
decl_stmt|;
end_decl_stmt

begin_comment
comment|/* STUB */
end_comment

begin_function
DECL|function|ngx_init_cycle (ngx_cycle_t * old_cycle)
name|ngx_cycle_t
modifier|*
name|ngx_init_cycle
parameter_list|(
name|ngx_cycle_t
modifier|*
name|old_cycle
parameter_list|)
block|{
name|ngx_int_t
name|i
decl_stmt|,
name|n
decl_stmt|,
name|failed
decl_stmt|;
name|ngx_str_t
name|conf_file
decl_stmt|;
name|ngx_log_t
modifier|*
name|log
decl_stmt|;
name|ngx_conf_t
name|conf
decl_stmt|;
name|ngx_pool_t
modifier|*
name|pool
decl_stmt|;
name|ngx_cycle_t
modifier|*
name|cycle
decl_stmt|,
modifier|*
modifier|*
name|old
decl_stmt|;
name|ngx_socket_t
name|fd
decl_stmt|;
name|ngx_open_file_t
modifier|*
name|file
decl_stmt|;
name|ngx_listening_t
modifier|*
name|ls
decl_stmt|,
modifier|*
name|nls
decl_stmt|;
name|log
operator|=
name|old_cycle
operator|->
name|log
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|pool
operator|=
name|ngx_create_pool
argument_list|(
literal|16
operator|*
literal|1024
argument_list|,
name|log
argument_list|)
operator|)
condition|)
block|{
return|return
name|NULL
return|;
block|}
if|if
condition|(
operator|!
operator|(
name|cycle
operator|=
name|ngx_pcalloc
argument_list|(
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_cycle_t
argument_list|)
argument_list|)
operator|)
condition|)
block|{
name|ngx_destroy_pool
argument_list|(
name|pool
argument_list|)
expr_stmt|;
return|return
name|NULL
return|;
block|}
name|cycle
operator|->
name|pool
operator|=
name|pool
expr_stmt|;
name|cycle
operator|->
name|old_cycle
operator|=
name|old_cycle
expr_stmt|;
name|cycle
operator|->
name|conf_file
operator|=
name|old_cycle
operator|->
name|conf_file
expr_stmt|;
name|n
operator|=
name|old_cycle
operator|->
name|pathes
operator|.
name|nelts
condition|?
name|old_cycle
operator|->
name|pathes
operator|.
name|nelts
else|:
literal|10
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|cycle
operator|->
name|pathes
operator|.
name|elts
operator|=
name|ngx_pcalloc
argument_list|(
name|pool
argument_list|,
name|n
operator|*
sizeof|sizeof
argument_list|(
name|ngx_path_t
operator|*
argument_list|)
argument_list|)
operator|)
condition|)
block|{
name|ngx_destroy_pool
argument_list|(
name|pool
argument_list|)
expr_stmt|;
return|return
name|NULL
return|;
block|}
name|cycle
operator|->
name|pathes
operator|.
name|nelts
operator|=
literal|0
expr_stmt|;
name|cycle
operator|->
name|pathes
operator|.
name|size
operator|=
sizeof|sizeof
argument_list|(
name|ngx_path_t
operator|*
argument_list|)
expr_stmt|;
name|cycle
operator|->
name|pathes
operator|.
name|nalloc
operator|=
name|n
expr_stmt|;
name|cycle
operator|->
name|pathes
operator|.
name|pool
operator|=
name|pool
expr_stmt|;
name|n
operator|=
name|old_cycle
operator|->
name|open_files
operator|.
name|nelts
condition|?
name|old_cycle
operator|->
name|open_files
operator|.
name|nelts
else|:
literal|20
expr_stmt|;
name|cycle
operator|->
name|open_files
operator|.
name|elts
operator|=
name|ngx_pcalloc
argument_list|(
name|pool
argument_list|,
name|n
operator|*
sizeof|sizeof
argument_list|(
name|ngx_open_file_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|cycle
operator|->
name|open_files
operator|.
name|elts
operator|==
name|NULL
condition|)
block|{
name|ngx_destroy_pool
argument_list|(
name|pool
argument_list|)
expr_stmt|;
return|return
name|NULL
return|;
block|}
name|cycle
operator|->
name|open_files
operator|.
name|nelts
operator|=
literal|0
expr_stmt|;
name|cycle
operator|->
name|open_files
operator|.
name|size
operator|=
sizeof|sizeof
argument_list|(
name|ngx_open_file_t
argument_list|)
expr_stmt|;
name|cycle
operator|->
name|open_files
operator|.
name|nalloc
operator|=
name|n
expr_stmt|;
name|cycle
operator|->
name|open_files
operator|.
name|pool
operator|=
name|pool
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|cycle
operator|->
name|log
operator|=
name|ngx_log_create_errlog
argument_list|(
name|cycle
argument_list|,
name|NULL
argument_list|)
operator|)
condition|)
block|{
name|ngx_destroy_pool
argument_list|(
name|pool
argument_list|)
expr_stmt|;
return|return
name|NULL
return|;
block|}
name|n
operator|=
name|old_cycle
operator|->
name|listening
operator|.
name|nelts
condition|?
name|old_cycle
operator|->
name|listening
operator|.
name|nelts
else|:
literal|10
expr_stmt|;
name|cycle
operator|->
name|listening
operator|.
name|elts
operator|=
name|ngx_pcalloc
argument_list|(
name|pool
argument_list|,
name|n
operator|*
sizeof|sizeof
argument_list|(
name|ngx_listening_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|cycle
operator|->
name|listening
operator|.
name|elts
operator|==
name|NULL
condition|)
block|{
name|ngx_destroy_pool
argument_list|(
name|pool
argument_list|)
expr_stmt|;
return|return
name|NULL
return|;
block|}
name|cycle
operator|->
name|listening
operator|.
name|nelts
operator|=
literal|0
expr_stmt|;
name|cycle
operator|->
name|listening
operator|.
name|size
operator|=
sizeof|sizeof
argument_list|(
name|ngx_listening_t
argument_list|)
expr_stmt|;
name|cycle
operator|->
name|listening
operator|.
name|nalloc
operator|=
name|n
expr_stmt|;
name|cycle
operator|->
name|listening
operator|.
name|pool
operator|=
name|pool
expr_stmt|;
name|cycle
operator|->
name|conf_ctx
operator|=
name|ngx_pcalloc
argument_list|(
name|pool
argument_list|,
name|ngx_max_module
operator|*
sizeof|sizeof
argument_list|(
name|void
operator|*
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|cycle
operator|->
name|conf_ctx
operator|==
name|NULL
condition|)
block|{
name|ngx_destroy_pool
argument_list|(
name|pool
argument_list|)
expr_stmt|;
return|return
name|NULL
return|;
block|}
if|if
condition|(
name|ngx_core_module
operator|.
name|init_module
argument_list|(
name|cycle
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
name|ngx_destroy_pool
argument_list|(
name|pool
argument_list|)
expr_stmt|;
return|return
name|NULL
return|;
block|}
name|ngx_memzero
argument_list|(
operator|&
name|conf
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_conf_t
argument_list|)
argument_list|)
expr_stmt|;
comment|/* STUB: init array ? */
name|conf
operator|.
name|args
operator|=
name|ngx_create_array
argument_list|(
name|pool
argument_list|,
literal|10
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_str_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|conf
operator|.
name|args
operator|==
name|NULL
condition|)
block|{
name|ngx_destroy_pool
argument_list|(
name|pool
argument_list|)
expr_stmt|;
return|return
name|NULL
return|;
block|}
name|conf
operator|.
name|ctx
operator|=
name|cycle
operator|->
name|conf_ctx
expr_stmt|;
name|conf
operator|.
name|cycle
operator|=
name|cycle
expr_stmt|;
comment|/* STUB */
name|conf
operator|.
name|pool
operator|=
name|cycle
operator|->
name|pool
expr_stmt|;
name|conf
operator|.
name|log
operator|=
name|log
expr_stmt|;
name|conf
operator|.
name|module_type
operator|=
name|NGX_CORE_MODULE
expr_stmt|;
name|conf
operator|.
name|cmd_type
operator|=
name|NGX_MAIN_CONF
expr_stmt|;
if|if
condition|(
name|ngx_conf_parse
argument_list|(
operator|&
name|conf
argument_list|,
operator|&
name|cycle
operator|->
name|conf_file
argument_list|)
operator|!=
name|NGX_CONF_OK
condition|)
block|{
name|ngx_destroy_pool
argument_list|(
name|pool
argument_list|)
expr_stmt|;
return|return
name|NULL
return|;
block|}
name|failed
operator|=
literal|0
expr_stmt|;
name|file
operator|=
name|cycle
operator|->
name|open_files
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
name|open_files
operator|.
name|nelts
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|file
index|[
name|i
index|]
operator|.
name|name
operator|.
name|data
operator|==
name|NULL
condition|)
block|{
continue|continue;
block|}
name|file
index|[
name|i
index|]
operator|.
name|fd
operator|=
name|ngx_open_file
argument_list|(
name|file
index|[
name|i
index|]
operator|.
name|name
operator|.
name|data
argument_list|,
name|NGX_FILE_RDWR
argument_list|,
name|NGX_FILE_CREATE_OR_OPEN
operator||
name|NGX_FILE_APPEND
argument_list|)
expr_stmt|;
if|if
condition|(
name|file
index|[
name|i
index|]
operator|.
name|fd
operator|==
name|NGX_INVALID_FILE
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
name|ngx_open_file_n
literal|" \"%s\" failed"
argument_list|,
name|file
index|[
name|i
index|]
operator|.
name|name
operator|.
name|data
argument_list|)
expr_stmt|;
name|failed
operator|=
literal|1
expr_stmt|;
break|break;
block|}
if|#
directive|if
operator|(
name|WIN32
operator|)
if|if
condition|(
name|ngx_file_append_mode
argument_list|(
name|file
index|[
name|i
index|]
operator|.
name|fd
argument_list|)
operator|==
name|NGX_ERROR
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
name|ngx_file_append_mode_n
literal|" \"%s\" failed"
argument_list|,
name|file
index|[
name|i
index|]
operator|.
name|name
operator|.
name|data
argument_list|)
expr_stmt|;
name|failed
operator|=
literal|1
expr_stmt|;
break|break;
block|}
else|#
directive|else
if|if
condition|(
name|fcntl
argument_list|(
name|file
index|[
name|i
index|]
operator|.
name|fd
argument_list|,
name|F_SETFD
argument_list|,
name|FD_CLOEXEC
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
literal|"fcntl(FD_CLOEXEC) \"%s\" failed"
argument_list|,
name|file
index|[
name|i
index|]
operator|.
name|name
operator|.
name|data
argument_list|)
expr_stmt|;
name|failed
operator|=
literal|1
expr_stmt|;
break|break;
block|}
endif|#
directive|endif
block|}
if|if
condition|(
operator|!
name|failed
condition|)
block|{
if|if
condition|(
name|old_cycle
operator|->
name|listening
operator|.
name|nelts
condition|)
block|{
name|ls
operator|=
name|old_cycle
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
name|old_cycle
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
name|nls
operator|=
name|cycle
operator|->
name|listening
operator|.
name|elts
expr_stmt|;
for|for
control|(
name|n
operator|=
literal|0
init|;
name|n
operator|<
name|cycle
operator|->
name|listening
operator|.
name|nelts
condition|;
name|n
operator|++
control|)
block|{
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|old_cycle
operator|->
name|listening
operator|.
name|nelts
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|ls
index|[
name|i
index|]
operator|.
name|ignore
condition|)
block|{
continue|continue;
block|}
if|if
condition|(
name|ngx_memcmp
argument_list|(
name|nls
index|[
name|n
index|]
operator|.
name|sockaddr
argument_list|,
name|ls
index|[
name|i
index|]
operator|.
name|sockaddr
argument_list|,
name|ls
index|[
name|i
index|]
operator|.
name|socklen
argument_list|)
operator|==
literal|0
condition|)
block|{
name|fd
operator|=
name|ls
index|[
name|i
index|]
operator|.
name|fd
expr_stmt|;
if|#
directive|if
operator|(
name|WIN32
operator|)
comment|/*                          * Winsock assignes a socket number divisible by 4 so                          * to find a connection we divide a socket number by 4.                          */
name|fd
operator|/=
literal|4
expr_stmt|;
endif|#
directive|endif
if|if
condition|(
name|fd
operator|>=
operator|(
name|ngx_socket_t
operator|)
name|cycle
operator|->
name|connection_n
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
literal|"%d connections is not enough to hold "
literal|"an open listening socket on %s, "
literal|"required at least %d connections"
argument_list|,
name|cycle
operator|->
name|connection_n
argument_list|,
name|ls
index|[
name|i
index|]
operator|.
name|addr_text
operator|.
name|data
argument_list|,
name|fd
argument_list|)
expr_stmt|;
name|failed
operator|=
literal|1
expr_stmt|;
break|break;
block|}
name|nls
index|[
name|n
index|]
operator|.
name|fd
operator|=
name|ls
index|[
name|i
index|]
operator|.
name|fd
expr_stmt|;
name|nls
index|[
name|i
index|]
operator|.
name|remain
operator|=
literal|1
expr_stmt|;
name|ls
index|[
name|i
index|]
operator|.
name|remain
operator|=
literal|1
expr_stmt|;
break|break;
block|}
block|}
if|if
condition|(
name|nls
index|[
name|n
index|]
operator|.
name|fd
operator|==
operator|-
literal|1
condition|)
block|{
name|nls
index|[
name|n
index|]
operator|.
name|new
operator|=
literal|1
expr_stmt|;
block|}
block|}
block|}
else|else
block|{
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
name|new
operator|=
literal|1
expr_stmt|;
block|}
block|}
if|if
condition|(
operator|!
name|failed
condition|)
block|{
if|if
condition|(
name|ngx_open_listening_sockets
argument_list|(
name|cycle
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
name|failed
operator|=
literal|1
expr_stmt|;
block|}
block|}
block|}
comment|/* TODO: Win32 DuplicateHandle ? */
if|if
condition|(
name|dup2
argument_list|(
name|cycle
operator|->
name|log
operator|->
name|file
operator|->
name|fd
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
name|ngx_errno
argument_list|,
literal|"dup2(STDERR) failed"
argument_list|)
expr_stmt|;
name|failed
operator|=
literal|1
expr_stmt|;
block|}
if|if
condition|(
name|failed
condition|)
block|{
comment|/* rollback the new cycle configuration */
name|file
operator|=
name|cycle
operator|->
name|open_files
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
name|open_files
operator|.
name|nelts
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|file
index|[
name|i
index|]
operator|.
name|fd
operator|==
name|NGX_INVALID_FILE
condition|)
block|{
continue|continue;
block|}
if|if
condition|(
name|ngx_close_file
argument_list|(
name|file
index|[
name|i
index|]
operator|.
name|fd
argument_list|)
operator|==
name|NGX_FILE_ERROR
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
name|ngx_close_file_n
literal|" \"%s\" failed"
argument_list|,
name|file
index|[
name|i
index|]
operator|.
name|name
operator|.
name|data
argument_list|)
expr_stmt|;
block|}
block|}
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
if|if
condition|(
name|ls
index|[
name|i
index|]
operator|.
name|new
operator|&&
name|ls
index|[
name|i
index|]
operator|.
name|fd
operator|==
operator|-
literal|1
condition|)
block|{
continue|continue;
block|}
if|if
condition|(
name|ngx_close_socket
argument_list|(
name|ls
index|[
name|i
index|]
operator|.
name|fd
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
name|ngx_socket_errno
argument_list|,
name|ngx_close_socket_n
literal|" %s failed"
argument_list|,
name|ls
index|[
name|i
index|]
operator|.
name|addr_text
operator|.
name|data
argument_list|)
expr_stmt|;
block|}
block|}
name|ngx_destroy_pool
argument_list|(
name|pool
argument_list|)
expr_stmt|;
return|return
name|NULL
return|;
block|}
comment|/* commit the new cycle configuration */
name|pool
operator|->
name|log
operator|=
name|cycle
operator|->
name|log
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
name|init_module
condition|)
block|{
if|if
condition|(
name|ngx_modules
index|[
name|i
index|]
operator|->
name|init_module
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
literal|1
argument_list|)
expr_stmt|;
block|}
block|}
block|}
comment|/* close and delete stuff that lefts from an old cycle */
comment|/* close the unneeded listening sockets */
name|ls
operator|=
name|old_cycle
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
name|old_cycle
operator|->
name|listening
operator|.
name|nelts
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|ls
index|[
name|i
index|]
operator|.
name|remain
condition|)
block|{
continue|continue;
block|}
if|if
condition|(
name|ngx_close_socket
argument_list|(
name|ls
index|[
name|i
index|]
operator|.
name|fd
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
name|ngx_socket_errno
argument_list|,
name|ngx_close_socket_n
literal|" %s failed"
argument_list|,
name|ls
index|[
name|i
index|]
operator|.
name|addr_text
operator|.
name|data
argument_list|)
expr_stmt|;
block|}
block|}
comment|/* close the unneeded open files */
name|file
operator|=
name|old_cycle
operator|->
name|open_files
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
name|old_cycle
operator|->
name|open_files
operator|.
name|nelts
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|file
index|[
name|i
index|]
operator|.
name|fd
operator|==
name|NGX_INVALID_FILE
condition|)
block|{
continue|continue;
block|}
if|if
condition|(
name|ngx_close_file
argument_list|(
name|file
index|[
name|i
index|]
operator|.
name|fd
argument_list|)
operator|==
name|NGX_FILE_ERROR
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
name|ngx_close_file_n
literal|" \"%s\" failed"
argument_list|,
name|file
index|[
name|i
index|]
operator|.
name|name
operator|.
name|data
argument_list|)
expr_stmt|;
block|}
block|}
if|if
condition|(
name|old_cycle
operator|->
name|connections
operator|==
name|NULL
condition|)
block|{
comment|/* an old cycle is an init cycle */
name|ngx_destroy_pool
argument_list|(
name|old_cycle
operator|->
name|pool
argument_list|)
expr_stmt|;
return|return
name|cycle
return|;
block|}
if|if
condition|(
name|ngx_process
operator|==
name|NGX_PROCESS_MASTER
condition|)
block|{
name|ngx_destroy_pool
argument_list|(
name|old_cycle
operator|->
name|pool
argument_list|)
expr_stmt|;
return|return
name|cycle
return|;
block|}
if|if
condition|(
name|ngx_temp_pool
operator|==
name|NULL
condition|)
block|{
name|ngx_temp_pool
operator|=
name|ngx_create_pool
argument_list|(
literal|128
argument_list|,
name|cycle
operator|->
name|log
argument_list|)
expr_stmt|;
if|if
condition|(
name|ngx_temp_pool
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
literal|"can not create ngx_temp_pool"
argument_list|)
expr_stmt|;
name|exit
argument_list|(
literal|1
argument_list|)
expr_stmt|;
block|}
name|n
operator|=
literal|10
expr_stmt|;
name|ngx_old_cycles
operator|.
name|elts
operator|=
name|ngx_pcalloc
argument_list|(
name|ngx_temp_pool
argument_list|,
name|n
operator|*
sizeof|sizeof
argument_list|(
name|ngx_cycle_t
operator|*
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|ngx_old_cycles
operator|.
name|elts
operator|==
name|NULL
condition|)
block|{
name|exit
argument_list|(
literal|1
argument_list|)
expr_stmt|;
block|}
name|ngx_old_cycles
operator|.
name|nelts
operator|=
literal|0
expr_stmt|;
name|ngx_old_cycles
operator|.
name|size
operator|=
sizeof|sizeof
argument_list|(
name|ngx_cycle_t
operator|*
argument_list|)
expr_stmt|;
name|ngx_old_cycles
operator|.
name|nalloc
operator|=
name|n
expr_stmt|;
name|ngx_old_cycles
operator|.
name|pool
operator|=
name|ngx_temp_pool
expr_stmt|;
name|ngx_cleaner_event
operator|.
name|event_handler
operator|=
name|ngx_clean_old_cycles
expr_stmt|;
name|ngx_cleaner_event
operator|.
name|log
operator|=
name|cycle
operator|->
name|log
expr_stmt|;
name|ngx_cleaner_event
operator|.
name|data
operator|=
operator|&
name|dumb
expr_stmt|;
name|dumb
operator|.
name|fd
operator|=
operator|(
name|ngx_socket_t
operator|)
operator|-
literal|1
expr_stmt|;
block|}
name|ngx_temp_pool
operator|->
name|log
operator|=
name|cycle
operator|->
name|log
expr_stmt|;
name|old
operator|=
name|ngx_push_array
argument_list|(
operator|&
name|ngx_old_cycles
argument_list|)
expr_stmt|;
if|if
condition|(
name|old
operator|==
name|NULL
condition|)
block|{
name|exit
argument_list|(
literal|1
argument_list|)
expr_stmt|;
block|}
operator|*
name|old
operator|=
name|old_cycle
expr_stmt|;
if|if
condition|(
operator|!
name|ngx_cleaner_event
operator|.
name|timer_set
condition|)
block|{
name|ngx_add_timer
argument_list|(
operator|&
name|ngx_cleaner_event
argument_list|,
literal|30000
argument_list|)
expr_stmt|;
name|ngx_cleaner_event
operator|.
name|timer_set
operator|=
literal|1
expr_stmt|;
block|}
return|return
name|cycle
return|;
block|}
end_function

begin_function
DECL|function|ngx_reopen_files (ngx_cycle_t * cycle,uid_t user)
name|void
name|ngx_reopen_files
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|,
name|uid_t
name|user
parameter_list|)
block|{
name|ngx_fd_t
name|fd
decl_stmt|;
name|ngx_int_t
name|i
decl_stmt|;
name|ngx_open_file_t
modifier|*
name|file
decl_stmt|;
name|file
operator|=
name|cycle
operator|->
name|open_files
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
name|open_files
operator|.
name|nelts
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|file
index|[
name|i
index|]
operator|.
name|name
operator|.
name|data
operator|==
name|NULL
condition|)
block|{
continue|continue;
block|}
name|fd
operator|=
name|ngx_open_file
argument_list|(
name|file
index|[
name|i
index|]
operator|.
name|name
operator|.
name|data
argument_list|,
name|NGX_FILE_RDWR
argument_list|,
name|NGX_FILE_CREATE_OR_OPEN
operator||
name|NGX_FILE_APPEND
argument_list|)
expr_stmt|;
name|ngx_log_debug3
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|cycle
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"reopen file \"%s\", old:%d new:%d"
argument_list|,
name|file
index|[
name|i
index|]
operator|.
name|name
operator|.
name|data
argument_list|,
name|file
index|[
name|i
index|]
operator|.
name|fd
argument_list|,
name|fd
argument_list|)
expr_stmt|;
if|if
condition|(
name|fd
operator|==
name|NGX_INVALID_FILE
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
name|ngx_open_file_n
literal|" \"%s\" failed"
argument_list|,
name|file
index|[
name|i
index|]
operator|.
name|name
operator|.
name|data
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|user
operator|!=
operator|(
name|uid_t
operator|)
operator|-
literal|1
condition|)
block|{
if|if
condition|(
name|chown
argument_list|(
name|file
index|[
name|i
index|]
operator|.
name|name
operator|.
name|data
argument_list|,
name|user
argument_list|,
operator|-
literal|1
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
literal|"chown \"%s\" failed"
argument_list|,
name|file
index|[
name|i
index|]
operator|.
name|name
operator|.
name|data
argument_list|)
expr_stmt|;
if|if
condition|(
name|ngx_close_file
argument_list|(
name|fd
argument_list|)
operator|==
name|NGX_FILE_ERROR
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
name|ngx_close_file_n
literal|" \"%s\" failed"
argument_list|,
name|file
index|[
name|i
index|]
operator|.
name|name
operator|.
name|data
argument_list|)
expr_stmt|;
block|}
block|}
block|}
if|#
directive|if
operator|(
name|WIN32
operator|)
if|if
condition|(
name|ngx_file_append_mode
argument_list|(
name|fd
argument_list|)
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
name|ngx_errno
argument_list|,
name|ngx_file_append_mode_n
literal|" \"%s\" failed"
argument_list|,
name|file
index|[
name|i
index|]
operator|.
name|name
operator|.
name|data
argument_list|)
expr_stmt|;
if|if
condition|(
name|ngx_close_file
argument_list|(
name|fd
argument_list|)
operator|==
name|NGX_FILE_ERROR
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
name|ngx_close_file_n
literal|" \"%s\" failed"
argument_list|,
name|file
index|[
name|i
index|]
operator|.
name|name
operator|.
name|data
argument_list|)
expr_stmt|;
block|}
continue|continue;
block|}
else|#
directive|else
if|if
condition|(
name|fcntl
argument_list|(
name|fd
argument_list|,
name|F_SETFD
argument_list|,
name|FD_CLOEXEC
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
literal|"fcntl(FD_CLOEXEC) \"%s\" failed"
argument_list|,
name|file
index|[
name|i
index|]
operator|.
name|name
operator|.
name|data
argument_list|)
expr_stmt|;
if|if
condition|(
name|ngx_close_file
argument_list|(
name|fd
argument_list|)
operator|==
name|NGX_FILE_ERROR
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
name|ngx_close_file_n
literal|" \"%s\" failed"
argument_list|,
name|file
index|[
name|i
index|]
operator|.
name|name
operator|.
name|data
argument_list|)
expr_stmt|;
block|}
continue|continue;
block|}
endif|#
directive|endif
if|if
condition|(
name|ngx_close_file
argument_list|(
name|file
index|[
name|i
index|]
operator|.
name|fd
argument_list|)
operator|==
name|NGX_FILE_ERROR
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
name|ngx_close_file_n
literal|" \"%s\" failed"
argument_list|,
name|file
index|[
name|i
index|]
operator|.
name|name
operator|.
name|data
argument_list|)
expr_stmt|;
block|}
name|file
index|[
name|i
index|]
operator|.
name|fd
operator|=
name|fd
expr_stmt|;
block|}
comment|/* TODO: Win32 DuplicateHandle ? */
if|if
condition|(
name|dup2
argument_list|(
name|cycle
operator|->
name|log
operator|->
name|file
operator|->
name|fd
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
name|cycle
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"dup2(STDERR) failed"
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|ngx_clean_old_cycles (ngx_event_t * ev)
specifier|static
name|void
name|ngx_clean_old_cycles
parameter_list|(
name|ngx_event_t
modifier|*
name|ev
parameter_list|)
block|{
name|int
name|i
decl_stmt|,
name|n
decl_stmt|,
name|found
decl_stmt|,
name|live
decl_stmt|;
name|ngx_log_t
modifier|*
name|log
decl_stmt|;
name|ngx_cycle_t
modifier|*
modifier|*
name|cycle
decl_stmt|;
name|log
operator|=
name|ngx_cycle
operator|->
name|log
expr_stmt|;
name|ngx_temp_pool
operator|->
name|log
operator|=
name|log
expr_stmt|;
name|ngx_log_debug0
argument_list|(
name|NGX_LOG_DEBUG_CORE
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"clean old cycles"
argument_list|)
expr_stmt|;
name|live
operator|=
literal|0
expr_stmt|;
name|cycle
operator|=
name|ngx_old_cycles
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
name|ngx_old_cycles
operator|.
name|nelts
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|cycle
index|[
name|i
index|]
operator|==
name|NULL
condition|)
block|{
continue|continue;
block|}
name|found
operator|=
literal|0
expr_stmt|;
for|for
control|(
name|n
operator|=
literal|0
init|;
name|n
operator|<
name|cycle
index|[
name|i
index|]
operator|->
name|connection_n
condition|;
name|n
operator|++
control|)
block|{
if|if
condition|(
name|cycle
index|[
name|i
index|]
operator|->
name|connections
index|[
name|n
index|]
operator|.
name|fd
operator|!=
operator|-
literal|1
condition|)
block|{
name|found
operator|=
literal|1
expr_stmt|;
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_CORE
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"live fd:%d"
argument_list|,
name|n
argument_list|)
expr_stmt|;
break|break;
block|}
block|}
if|if
condition|(
name|found
condition|)
block|{
name|live
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_CORE
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"clean old cycle: %d"
argument_list|,
name|i
argument_list|)
expr_stmt|;
name|ngx_destroy_pool
argument_list|(
name|cycle
index|[
name|i
index|]
operator|->
name|pool
argument_list|)
expr_stmt|;
name|cycle
index|[
name|i
index|]
operator|=
name|NULL
expr_stmt|;
block|}
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_CORE
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"old cycles status: %d"
argument_list|,
name|live
argument_list|)
expr_stmt|;
if|if
condition|(
name|live
condition|)
block|{
name|ngx_add_timer
argument_list|(
name|ev
argument_list|,
literal|30000
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|ngx_destroy_pool
argument_list|(
name|ngx_temp_pool
argument_list|)
expr_stmt|;
name|ngx_temp_pool
operator|=
name|NULL
expr_stmt|;
name|ngx_old_cycles
operator|.
name|nelts
operator|=
literal|0
expr_stmt|;
block|}
block|}
end_function

end_unit

