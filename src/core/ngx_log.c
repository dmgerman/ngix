begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_comment
comment|/*    TODO: log pid and tid */
end_comment

begin_comment
comment|/*    "[time as ctime()] [alert] 412#3 (32)Broken pipe: anything"     "[time as ctime()] [alert] (32)Broken pipe: anything"    "[time as ctime()] [alert] anything" */
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

begin_function_decl
specifier|static
name|char
modifier|*
name|ngx_set_error_log
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
DECL|variable|errlog_name
specifier|static
name|ngx_str_t
name|errlog_name
init|=
name|ngx_string
argument_list|(
literal|"errlog"
argument_list|)
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_errlog_commands
specifier|static
name|ngx_command_t
name|ngx_errlog_commands
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"error_log"
argument_list|)
block|,
name|NGX_MAIN_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_set_error_log
block|,
literal|0
block|,
literal|0
block|,
name|NULL
block|}
block|,
name|ngx_null_command
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_errlog_module
name|ngx_module_t
name|ngx_errlog_module
init|=
block|{
name|NGX_MODULE
block|,
operator|&
name|errlog_name
block|,
comment|/* module context */
name|ngx_errlog_commands
block|,
comment|/* module directives */
name|NGX_CORE_MODULE
block|,
comment|/* module type */
name|NULL
comment|/* init module */
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_stderr
specifier|static
name|ngx_open_file_t
name|ngx_stderr
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_log
specifier|static
name|ngx_log_t
name|ngx_log
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|err_levels
specifier|static
specifier|const
name|char
modifier|*
name|err_levels
index|[]
init|=
block|{
literal|"stderr"
block|,
literal|"emerg"
block|,
literal|"alert"
block|,
literal|"crit"
block|,
literal|"error"
block|,
literal|"warn"
block|,
literal|"notice"
block|,
literal|"info"
block|,
literal|"debug"
block|}
decl_stmt|;
end_decl_stmt

begin_if
if|#
directive|if
operator|(
name|HAVE_VARIADIC_MACROS
operator|)
end_if

begin_decl_stmt
DECL|function|ngx_log_error_core (int level,ngx_log_t * log,ngx_err_t err,const char * fmt,...)
name|void
name|ngx_log_error_core
argument_list|(
name|int
name|level
argument_list|,
name|ngx_log_t
operator|*
name|log
argument_list|,
name|ngx_err_t
name|err
argument_list|,
specifier|const
name|char
operator|*
name|fmt
argument_list|,
operator|...
argument_list|)
else|#
directive|else
name|void
name|ngx_log_error_core
argument_list|(
name|int
name|level
argument_list|,
name|ngx_log_t
operator|*
name|log
argument_list|,
name|ngx_err_t
name|err
argument_list|,
specifier|const
name|char
operator|*
name|fmt
argument_list|,
name|va_list
name|args
argument_list|)
endif|#
directive|endif
block|{
name|char
name|errstr
index|[
name|MAX_ERROR_STR
index|]
decl_stmt|;
name|ngx_tm_t
name|tm
decl_stmt|;
name|size_t
name|len
decl_stmt|;
if|#
directive|if
operator|(
name|HAVE_VARIADIC_MACROS
operator|)
name|va_list
name|args
decl_stmt|;
endif|#
directive|endif
if|#
directive|if
operator|(
name|WIN32
operator|)
name|int
name|written
decl_stmt|;
endif|#
directive|endif
name|ngx_localtime
argument_list|(
operator|&
name|tm
argument_list|)
expr_stmt|;
name|len
operator|=
name|ngx_snprintf
argument_list|(
name|errstr
argument_list|,
sizeof|sizeof
argument_list|(
name|errstr
argument_list|)
argument_list|,
literal|"%4d/%02d/%02d %02d:%02d:%02d"
argument_list|,
name|tm
operator|.
name|ngx_tm_year
argument_list|,
name|tm
operator|.
name|ngx_tm_mon
argument_list|,
name|tm
operator|.
name|ngx_tm_mday
argument_list|,
name|tm
operator|.
name|ngx_tm_hour
argument_list|,
name|tm
operator|.
name|ngx_tm_min
argument_list|,
name|tm
operator|.
name|ngx_tm_sec
argument_list|)
expr_stmt|;
name|len
operator|+=
name|ngx_snprintf
argument_list|(
name|errstr
operator|+
name|len
argument_list|,
sizeof|sizeof
argument_list|(
name|errstr
argument_list|)
operator|-
name|len
operator|-
literal|1
argument_list|,
literal|" [%s] "
argument_list|,
name|err_levels
index|[
name|level
index|]
argument_list|)
expr_stmt|;
comment|/* pid#tid */
name|len
operator|+=
name|ngx_snprintf
argument_list|(
name|errstr
operator|+
name|len
argument_list|,
sizeof|sizeof
argument_list|(
name|errstr
argument_list|)
operator|-
name|len
operator|-
literal|1
argument_list|,
name|PID_FMT
literal|"#%d: "
argument_list|,
name|ngx_getpid
argument_list|()
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|#
directive|if
operator|(
name|HAVE_VARIADIC_MACROS
operator|)
name|va_start
argument_list|(
name|args
argument_list|,
name|fmt
argument_list|)
expr_stmt|;
name|len
operator|+=
name|ngx_vsnprintf
argument_list|(
name|errstr
operator|+
name|len
argument_list|,
sizeof|sizeof
argument_list|(
name|errstr
argument_list|)
operator|-
name|len
operator|-
literal|1
argument_list|,
name|fmt
argument_list|,
name|args
argument_list|)
expr_stmt|;
name|va_end
argument_list|(
name|args
argument_list|)
expr_stmt|;
else|#
directive|else
name|len
operator|+=
name|ngx_vsnprintf
argument_list|(
name|errstr
operator|+
name|len
argument_list|,
sizeof|sizeof
argument_list|(
name|errstr
argument_list|)
operator|-
name|len
operator|-
literal|1
argument_list|,
name|fmt
argument_list|,
name|args
argument_list|)
expr_stmt|;
endif|#
directive|endif
if|if
condition|(
name|err
condition|)
block|{
if|#
directive|if
operator|(
name|WIN32
operator|)
if|if
condition|(
operator|(
name|unsigned
operator|)
name|err
operator|>=
literal|0x80000000
condition|)
block|{
name|len
operator|+=
name|ngx_snprintf
argument_list|(
name|errstr
operator|+
name|len
argument_list|,
sizeof|sizeof
argument_list|(
name|errstr
argument_list|)
operator|-
name|len
operator|-
literal|1
argument_list|,
literal|" (%X: "
argument_list|,
name|err
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|len
operator|+=
name|ngx_snprintf
argument_list|(
name|errstr
operator|+
name|len
argument_list|,
sizeof|sizeof
argument_list|(
name|errstr
argument_list|)
operator|-
name|len
operator|-
literal|1
argument_list|,
literal|" (%d: "
argument_list|,
name|err
argument_list|)
expr_stmt|;
block|}
else|#
directive|else
name|len
operator|+=
name|ngx_snprintf
argument_list|(
name|errstr
operator|+
name|len
argument_list|,
sizeof|sizeof
argument_list|(
name|errstr
argument_list|)
operator|-
name|len
operator|-
literal|1
argument_list|,
literal|" (%d: "
argument_list|,
name|err
argument_list|)
expr_stmt|;
endif|#
directive|endif
name|len
operator|+=
name|ngx_strerror_r
argument_list|(
name|err
argument_list|,
name|errstr
operator|+
name|len
argument_list|,
sizeof|sizeof
argument_list|(
name|errstr
argument_list|)
operator|-
name|len
operator|-
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|len
operator|<
sizeof|sizeof
argument_list|(
name|errstr
argument_list|)
operator|-
literal|2
condition|)
block|{
name|errstr
index|[
name|len
operator|++
index|]
operator|=
literal|')'
expr_stmt|;
block|}
else|else
block|{
name|len
operator|=
sizeof|sizeof
argument_list|(
name|errstr
argument_list|)
operator|-
literal|2
expr_stmt|;
block|}
block|}
if|if
condition|(
name|level
operator|!=
name|NGX_LOG_DEBUG
operator|&&
name|log
operator|->
name|handler
condition|)
block|{
name|len
operator|+=
name|log
operator|->
name|handler
argument_list|(
name|log
operator|->
name|data
argument_list|,
name|errstr
operator|+
name|len
argument_list|,
sizeof|sizeof
argument_list|(
name|errstr
argument_list|)
operator|-
name|len
operator|-
literal|1
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|len
operator|>
sizeof|sizeof
argument_list|(
name|errstr
argument_list|)
operator|-
literal|2
condition|)
block|{
name|len
operator|=
sizeof|sizeof
argument_list|(
name|errstr
argument_list|)
operator|-
literal|2
expr_stmt|;
block|}
if|#
directive|if
operator|(
name|WIN32
operator|)
name|errstr
index|[
name|len
operator|++
index|]
operator|=
literal|'\r'
expr_stmt|;
name|errstr
index|[
name|len
operator|++
index|]
operator|=
literal|'\n'
expr_stmt|;
if|if
condition|(
name|log
operator|->
name|file
operator|->
name|fd
condition|)
block|{
name|WriteFile
argument_list|(
name|log
operator|->
name|file
operator|->
name|fd
argument_list|,
name|errstr
argument_list|,
name|len
argument_list|,
operator|&
name|written
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
block|}
else|#
directive|else
name|errstr
index|[
name|len
operator|++
index|]
operator|=
literal|'\n'
expr_stmt|;
name|write
argument_list|(
name|log
operator|->
name|file
operator|->
name|fd
argument_list|,
name|errstr
argument_list|,
name|len
argument_list|)
expr_stmt|;
endif|#
directive|endif
if|#
directive|if
literal|0
block_content|errstr[len] = '\0';     fputs(errstr, stderr);     fflush(stderr);
endif|#
directive|endif
block|}
end_decl_stmt

begin_if
if|#
directive|if
operator|!
operator|(
name|HAVE_VARIADIC_MACROS
operator|)
end_if

begin_function
DECL|function|ngx_log_error (int level,ngx_log_t * log,ngx_err_t err,const char * fmt,...)
name|void
name|ngx_log_error
parameter_list|(
name|int
name|level
parameter_list|,
name|ngx_log_t
modifier|*
name|log
parameter_list|,
name|ngx_err_t
name|err
parameter_list|,
specifier|const
name|char
modifier|*
name|fmt
parameter_list|,
modifier|...
parameter_list|)
block|{
name|va_list
name|args
decl_stmt|;
if|if
condition|(
name|log
operator|->
name|log_level
operator|>=
name|level
condition|)
block|{
name|va_start
argument_list|(
name|args
argument_list|,
name|fmt
argument_list|)
expr_stmt|;
name|ngx_log_error_core
argument_list|(
name|level
argument_list|,
name|log
argument_list|,
name|err
argument_list|,
name|fmt
argument_list|,
name|args
argument_list|)
expr_stmt|;
name|va_end
argument_list|(
name|args
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|ngx_log_debug_core (ngx_log_t * log,const char * fmt,...)
name|void
name|ngx_log_debug_core
parameter_list|(
name|ngx_log_t
modifier|*
name|log
parameter_list|,
specifier|const
name|char
modifier|*
name|fmt
parameter_list|,
modifier|...
parameter_list|)
block|{
name|va_list
name|args
decl_stmt|;
name|va_start
argument_list|(
name|args
argument_list|,
name|fmt
argument_list|)
expr_stmt|;
name|ngx_log_error_core
argument_list|(
name|NGX_LOG_DEBUG
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
name|fmt
argument_list|,
name|args
argument_list|)
expr_stmt|;
name|va_end
argument_list|(
name|args
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|ngx_assert_core (ngx_log_t * log,const char * fmt,...)
name|void
name|ngx_assert_core
parameter_list|(
name|ngx_log_t
modifier|*
name|log
parameter_list|,
specifier|const
name|char
modifier|*
name|fmt
parameter_list|,
modifier|...
parameter_list|)
block|{
name|va_list
name|args
decl_stmt|;
name|va_start
argument_list|(
name|args
argument_list|,
name|fmt
argument_list|)
expr_stmt|;
name|ngx_log_error_core
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
name|fmt
argument_list|,
name|args
argument_list|)
expr_stmt|;
name|va_end
argument_list|(
name|args
argument_list|)
expr_stmt|;
block|}
end_function

begin_endif
endif|#
directive|endif
end_endif

begin_if
if|#
directive|if
literal|0
end_if

begin_endif
unit|void ngx_log_stderr(ngx_event_t *ev) {     char       errstr[MAX_ERROR_STR];     ssize_t    n;     ngx_err_t  err;      for ( ;; ) {         n = read((ngx_fd_t) ev->data, errstr, sizeof(errstr - 1));          if (n == -1) {             err = ngx_errno;             if (err == NGX_EAGAIN) {                 return;             }              ngx_log_error(NGX_LOG_ALERT,&ngx_log, err, "read() failed");             return;         }          if (n == 0) {             ngx_log_error(NGX_LOG_ALERT,&ngx_log, 0, "stderr clolsed");             return;         }          errstr[n] = '\0';         ngx_log_error(NGX_LOG_STDERR,&ngx_log, 0, "%s", errstr);     } }
endif|#
directive|endif
end_endif

begin_function
DECL|function|ngx_log_init_errlog ()
name|ngx_log_t
modifier|*
name|ngx_log_init_errlog
parameter_list|()
block|{
if|#
directive|if
operator|(
name|WIN32
operator|)
name|ngx_stderr
operator|.
name|fd
operator|=
name|GetStdHandle
argument_list|(
name|STD_ERROR_HANDLE
argument_list|)
expr_stmt|;
if|if
condition|(
name|ngx_stderr
operator|.
name|fd
operator|==
name|NGX_INVALID_FILE
condition|)
block|{
comment|/* TODO: where we can log error ? */
return|return
name|NULL
return|;
block|}
if|else if
condition|(
name|ngx_stderr
operator|.
name|fd
operator|==
name|NULL
condition|)
block|{
comment|/* there are no associated standard handles */
comment|/* TODO: where we can log possible errors ? */
block|}
else|#
directive|else
name|ngx_stderr
operator|.
name|fd
operator|=
name|STDERR_FILENO
expr_stmt|;
endif|#
directive|endif
name|ngx_log
operator|.
name|file
operator|=
operator|&
name|ngx_stderr
expr_stmt|;
name|ngx_log
operator|.
name|log_level
operator|=
name|NGX_LOG_INFO
expr_stmt|;
comment|/* STUB */
name|ngx_log
operator|.
name|log_level
operator|=
name|NGX_LOG_DEBUG
expr_stmt|;
return|return
operator|&
name|ngx_log
return|;
block|}
end_function

begin_function
DECL|function|ngx_log_create_errlog (ngx_cycle_t * cycle,ngx_str_t * name)
name|ngx_log_t
modifier|*
name|ngx_log_create_errlog
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|,
name|ngx_str_t
modifier|*
name|name
parameter_list|)
block|{
name|ngx_log_t
modifier|*
name|log
decl_stmt|;
name|ngx_test_null
argument_list|(
name|log
argument_list|,
name|ngx_pcalloc
argument_list|(
name|cycle
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_log_t
argument_list|)
argument_list|)
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|ngx_test_null
argument_list|(
name|log
operator|->
name|file
argument_list|,
name|ngx_push_array
argument_list|(
operator|&
name|cycle
operator|->
name|open_files
argument_list|)
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|log
operator|->
name|file
operator|->
name|fd
operator|=
name|NGX_INVALID_FILE
expr_stmt|;
if|if
condition|(
name|name
condition|)
block|{
name|log
operator|->
name|file
operator|->
name|name
operator|=
operator|*
name|name
expr_stmt|;
block|}
return|return
name|log
return|;
block|}
end_function

begin_function
DECL|function|ngx_set_error_log (ngx_conf_t * cf,ngx_command_t * cmd,void * conf)
specifier|static
name|char
modifier|*
name|ngx_set_error_log
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
name|ngx_str_t
modifier|*
name|value
decl_stmt|;
name|value
operator|=
name|cf
operator|->
name|args
operator|->
name|elts
expr_stmt|;
if|if
condition|(
name|value
index|[
literal|1
index|]
operator|.
name|len
operator|==
literal|6
operator|&&
name|ngx_strcmp
argument_list|(
name|value
index|[
literal|1
index|]
operator|.
name|data
argument_list|,
literal|"stderr"
argument_list|)
operator|==
literal|0
condition|)
block|{
name|cf
operator|->
name|cycle
operator|->
name|log
operator|->
name|file
operator|=
operator|&
name|ngx_stderr
expr_stmt|;
block|}
else|else
block|{
name|cf
operator|->
name|cycle
operator|->
name|log
operator|->
name|file
operator|->
name|name
operator|=
name|value
index|[
literal|1
index|]
expr_stmt|;
block|}
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

begin_if
if|#
directive|if
literal|0
end_if

begin_if
unit|char *ngx_log_set_errlog(ngx_conf_t *cf, ngx_command_t *cmd, ngx_log_t *log) {     int         len;     ngx_err_t   err;     ngx_str_t  *value;      value = cf->args->elts;      log->file->fd = ngx_open_file(value[1].data,                             NGX_FILE_RDWR,                             NGX_FILE_CREATE_OR_OPEN|NGX_FILE_APPEND);      if (log->file->fd == NGX_INVALID_FILE) {         err = ngx_errno;         len = ngx_snprintf(ngx_conf_errstr, sizeof(ngx_conf_errstr) - 1,                           ngx_open_file_n " \"%s\" failed (%d: ",                           value[1].data, err);         len += ngx_strerror_r(err, ngx_conf_errstr + len,                               sizeof(ngx_conf_errstr) - len - 1);         ngx_conf_errstr[len++] = ')';         ngx_conf_errstr[len++] = '\0';         return ngx_conf_errstr;     }
if|#
directive|if
operator|(
name|WIN32
operator|)
end_if

begin_endif
unit|if (ngx_file_append_mode(log->file->fd) == NGX_ERROR) {         err = ngx_errno;         len = ngx_snprintf(ngx_conf_errstr, sizeof(ngx_conf_errstr) - 1,                           ngx_file_append_mode_n " \"%s\" failed (%d: ",                           value[1].data, err);         len += ngx_strerror_r(err, ngx_conf_errstr + len,                               sizeof(ngx_conf_errstr) - len - 1);         ngx_conf_errstr[len++] = ')';         ngx_conf_errstr[len++] = '\0';         return ngx_conf_errstr;     }
endif|#
directive|endif
end_endif

begin_endif
unit|return NGX_CONF_OK; }
endif|#
directive|endif
end_endif

end_unit

