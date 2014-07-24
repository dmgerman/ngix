begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (C) Igor Sysoev  * Copyright (C) Nginx, Inc.  */
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
name|ngx_error_log
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

begin_function_decl
specifier|static
name|char
modifier|*
name|ngx_log_set_levels
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|ngx_log_t
modifier|*
name|log
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|void
name|ngx_log_insert
parameter_list|(
name|ngx_log_t
modifier|*
name|log
parameter_list|,
name|ngx_log_t
modifier|*
name|new_log
parameter_list|)
function_decl|;
end_function_decl

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
name|NGX_CONF_1MORE
block|,
name|ngx_error_log
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
DECL|variable|ngx_errlog_module_ctx
specifier|static
name|ngx_core_module_t
name|ngx_errlog_module_ctx
init|=
block|{
name|ngx_string
argument_list|(
literal|"errlog"
argument_list|)
block|,
name|NULL
block|,
name|NULL
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_errlog_module
name|ngx_module_t
name|ngx_errlog_module
init|=
block|{
name|NGX_MODULE_V1
block|,
operator|&
name|ngx_errlog_module_ctx
block|,
comment|/* module context */
name|ngx_errlog_commands
block|,
comment|/* module directives */
name|NGX_CORE_MODULE
block|,
comment|/* module type */
name|NULL
block|,
comment|/* init master */
name|NULL
block|,
comment|/* init module */
name|NULL
block|,
comment|/* init process */
name|NULL
block|,
comment|/* init thread */
name|NULL
block|,
comment|/* exit thread */
name|NULL
block|,
comment|/* exit process */
name|NULL
block|,
comment|/* exit master */
name|NGX_MODULE_V1_PADDING
block|}
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
DECL|variable|ngx_log_file
specifier|static
name|ngx_open_file_t
name|ngx_log_file
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_use_stderr
name|ngx_uint_t
name|ngx_use_stderr
init|=
literal|1
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|err_levels
specifier|static
name|ngx_str_t
name|err_levels
index|[]
init|=
block|{
name|ngx_null_string
block|,
name|ngx_string
argument_list|(
literal|"emerg"
argument_list|)
block|,
name|ngx_string
argument_list|(
literal|"alert"
argument_list|)
block|,
name|ngx_string
argument_list|(
literal|"crit"
argument_list|)
block|,
name|ngx_string
argument_list|(
literal|"error"
argument_list|)
block|,
name|ngx_string
argument_list|(
literal|"warn"
argument_list|)
block|,
name|ngx_string
argument_list|(
literal|"notice"
argument_list|)
block|,
name|ngx_string
argument_list|(
literal|"info"
argument_list|)
block|,
name|ngx_string
argument_list|(
literal|"debug"
argument_list|)
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|debug_levels
specifier|static
specifier|const
name|char
modifier|*
name|debug_levels
index|[]
init|=
block|{
literal|"debug_core"
block|,
literal|"debug_alloc"
block|,
literal|"debug_mutex"
block|,
literal|"debug_event"
block|,
literal|"debug_http"
block|,
literal|"debug_mail"
block|,
literal|"debug_mysql"
block|}
decl_stmt|;
end_decl_stmt

begin_if
if|#
directive|if
operator|(
name|NGX_HAVE_VARIADIC_MACROS
operator|)
end_if

begin_decl_stmt
name|void
DECL|function|ngx_log_error_core (ngx_uint_t level,ngx_log_t * log,ngx_err_t err,const char * fmt,...)
name|ngx_log_error_core
argument_list|(
name|ngx_uint_t
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
name|ngx_uint_t
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
if|#
directive|if
operator|(
name|NGX_HAVE_VARIADIC_MACROS
operator|)
name|va_list
name|args
decl_stmt|;
endif|#
directive|endif
name|u_char
modifier|*
name|p
decl_stmt|,
modifier|*
name|last
decl_stmt|,
modifier|*
name|msg
decl_stmt|;
name|u_char
name|errstr
index|[
name|NGX_MAX_ERROR_STR
index|]
decl_stmt|;
name|ngx_uint_t
name|wrote_stderr
decl_stmt|,
name|debug_connection
decl_stmt|;
name|last
operator|=
name|errstr
operator|+
name|NGX_MAX_ERROR_STR
expr_stmt|;
name|ngx_memcpy
argument_list|(
name|errstr
argument_list|,
name|ngx_cached_err_log_time
operator|.
name|data
argument_list|,
name|ngx_cached_err_log_time
operator|.
name|len
argument_list|)
expr_stmt|;
name|p
operator|=
name|errstr
operator|+
name|ngx_cached_err_log_time
operator|.
name|len
expr_stmt|;
name|p
operator|=
name|ngx_slprintf
argument_list|(
name|p
argument_list|,
name|last
argument_list|,
literal|" [%V] "
argument_list|,
operator|&
name|err_levels
index|[
name|level
index|]
argument_list|)
expr_stmt|;
comment|/* pid#tid */
name|p
operator|=
name|ngx_slprintf
argument_list|(
name|p
argument_list|,
name|last
argument_list|,
literal|"%P#"
name|NGX_TID_T_FMT
literal|": "
argument_list|,
name|ngx_log_pid
argument_list|,
name|ngx_log_tid
argument_list|)
expr_stmt|;
if|if
condition|(
name|log
operator|->
name|connection
condition|)
block|{
name|p
operator|=
name|ngx_slprintf
argument_list|(
name|p
argument_list|,
name|last
argument_list|,
literal|"*%uA "
argument_list|,
name|log
operator|->
name|connection
argument_list|)
expr_stmt|;
block|}
name|msg
operator|=
name|p
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_HAVE_VARIADIC_MACROS
operator|)
name|va_start
argument_list|(
name|args
argument_list|,
name|fmt
argument_list|)
expr_stmt|;
name|p
operator|=
name|ngx_vslprintf
argument_list|(
name|p
argument_list|,
name|last
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
name|p
operator|=
name|ngx_vslprintf
argument_list|(
name|p
argument_list|,
name|last
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
name|p
operator|=
name|ngx_log_errno
argument_list|(
name|p
argument_list|,
name|last
argument_list|,
name|err
argument_list|)
expr_stmt|;
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
name|p
operator|=
name|log
operator|->
name|handler
argument_list|(
name|log
argument_list|,
name|p
argument_list|,
name|last
operator|-
name|p
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|p
operator|>
name|last
operator|-
name|NGX_LINEFEED_SIZE
condition|)
block|{
name|p
operator|=
name|last
operator|-
name|NGX_LINEFEED_SIZE
expr_stmt|;
block|}
name|ngx_linefeed
argument_list|(
name|p
argument_list|)
expr_stmt|;
name|wrote_stderr
operator|=
literal|0
expr_stmt|;
name|debug_connection
operator|=
operator|(
name|log
operator|->
name|log_level
operator|&
name|NGX_LOG_DEBUG_CONNECTION
operator|)
operator|!=
literal|0
expr_stmt|;
while|while
condition|(
name|log
condition|)
block|{
if|if
condition|(
name|log
operator|->
name|log_level
operator|<
name|level
operator|&&
operator|!
name|debug_connection
condition|)
block|{
break|break;
block|}
if|if
condition|(
name|log
operator|->
name|writer
condition|)
block|{
name|log
operator|->
name|writer
argument_list|(
name|log
argument_list|,
name|level
argument_list|,
name|errstr
argument_list|,
name|p
operator|-
name|errstr
argument_list|)
expr_stmt|;
name|log
operator|=
name|log
operator|->
name|next
expr_stmt|;
continue|continue;
block|}
operator|(
name|void
operator|)
name|ngx_write_fd
argument_list|(
name|log
operator|->
name|file
operator|->
name|fd
argument_list|,
name|errstr
argument_list|,
name|p
operator|-
name|errstr
argument_list|)
expr_stmt|;
if|if
condition|(
name|log
operator|->
name|file
operator|->
name|fd
operator|==
name|ngx_stderr
condition|)
block|{
name|wrote_stderr
operator|=
literal|1
expr_stmt|;
block|}
name|log
operator|=
name|log
operator|->
name|next
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|ngx_use_stderr
operator|||
name|level
operator|>
name|NGX_LOG_WARN
operator|||
name|wrote_stderr
condition|)
block|{
return|return;
block|}
name|msg
operator|-=
operator|(
literal|7
operator|+
name|err_levels
index|[
name|level
index|]
operator|.
name|len
operator|+
literal|3
operator|)
expr_stmt|;
operator|(
name|void
operator|)
name|ngx_sprintf
argument_list|(
name|msg
argument_list|,
literal|"nginx: [%V] "
argument_list|,
operator|&
name|err_levels
index|[
name|level
index|]
argument_list|)
expr_stmt|;
operator|(
name|void
operator|)
name|ngx_write_console
argument_list|(
name|ngx_stderr
argument_list|,
name|msg
argument_list|,
name|p
operator|-
name|msg
argument_list|)
expr_stmt|;
block|}
end_decl_stmt

begin_if
if|#
directive|if
operator|!
operator|(
name|NGX_HAVE_VARIADIC_MACROS
operator|)
end_if

begin_function
name|void
name|ngx_cdecl
DECL|function|ngx_log_error (ngx_uint_t level,ngx_log_t * log,ngx_err_t err,const char * fmt,...)
name|ngx_log_error
parameter_list|(
name|ngx_uint_t
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
name|void
name|ngx_cdecl
DECL|function|ngx_log_debug_core (ngx_log_t * log,ngx_err_t err,const char * fmt,...)
name|ngx_log_debug_core
parameter_list|(
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
end_function

begin_endif
endif|#
directive|endif
end_endif

begin_function
name|void
name|ngx_cdecl
DECL|function|ngx_log_abort (ngx_err_t err,const char * fmt,...)
name|ngx_log_abort
parameter_list|(
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
name|u_char
modifier|*
name|p
decl_stmt|;
name|va_list
name|args
decl_stmt|;
name|u_char
name|errstr
index|[
name|NGX_MAX_CONF_ERRSTR
index|]
decl_stmt|;
name|va_start
argument_list|(
name|args
argument_list|,
name|fmt
argument_list|)
expr_stmt|;
name|p
operator|=
name|ngx_vsnprintf
argument_list|(
name|errstr
argument_list|,
sizeof|sizeof
argument_list|(
name|errstr
argument_list|)
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
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|ngx_cycle
operator|->
name|log
argument_list|,
name|err
argument_list|,
literal|"%*s"
argument_list|,
name|p
operator|-
name|errstr
argument_list|,
name|errstr
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
name|void
name|ngx_cdecl
DECL|function|ngx_log_stderr (ngx_err_t err,const char * fmt,...)
name|ngx_log_stderr
parameter_list|(
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
name|u_char
modifier|*
name|p
decl_stmt|,
modifier|*
name|last
decl_stmt|;
name|va_list
name|args
decl_stmt|;
name|u_char
name|errstr
index|[
name|NGX_MAX_ERROR_STR
index|]
decl_stmt|;
name|last
operator|=
name|errstr
operator|+
name|NGX_MAX_ERROR_STR
expr_stmt|;
name|p
operator|=
name|errstr
operator|+
literal|7
expr_stmt|;
name|ngx_memcpy
argument_list|(
name|errstr
argument_list|,
literal|"nginx: "
argument_list|,
literal|7
argument_list|)
expr_stmt|;
name|va_start
argument_list|(
name|args
argument_list|,
name|fmt
argument_list|)
expr_stmt|;
name|p
operator|=
name|ngx_vslprintf
argument_list|(
name|p
argument_list|,
name|last
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
if|if
condition|(
name|err
condition|)
block|{
name|p
operator|=
name|ngx_log_errno
argument_list|(
name|p
argument_list|,
name|last
argument_list|,
name|err
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|p
operator|>
name|last
operator|-
name|NGX_LINEFEED_SIZE
condition|)
block|{
name|p
operator|=
name|last
operator|-
name|NGX_LINEFEED_SIZE
expr_stmt|;
block|}
name|ngx_linefeed
argument_list|(
name|p
argument_list|)
expr_stmt|;
operator|(
name|void
operator|)
name|ngx_write_console
argument_list|(
name|ngx_stderr
argument_list|,
name|errstr
argument_list|,
name|p
operator|-
name|errstr
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
name|u_char
modifier|*
DECL|function|ngx_log_errno (u_char * buf,u_char * last,ngx_err_t err)
name|ngx_log_errno
parameter_list|(
name|u_char
modifier|*
name|buf
parameter_list|,
name|u_char
modifier|*
name|last
parameter_list|,
name|ngx_err_t
name|err
parameter_list|)
block|{
if|if
condition|(
name|buf
operator|>
name|last
operator|-
literal|50
condition|)
block|{
comment|/* leave a space for an error code */
name|buf
operator|=
name|last
operator|-
literal|50
expr_stmt|;
operator|*
name|buf
operator|++
operator|=
literal|'.'
expr_stmt|;
operator|*
name|buf
operator|++
operator|=
literal|'.'
expr_stmt|;
operator|*
name|buf
operator|++
operator|=
literal|'.'
expr_stmt|;
block|}
if|#
directive|if
operator|(
name|NGX_WIN32
operator|)
name|buf
operator|=
name|ngx_slprintf
argument_list|(
name|buf
argument_list|,
name|last
argument_list|,
operator|(
operator|(
name|unsigned
operator|)
name|err
operator|<
literal|0x80000000
operator|)
condition|?
literal|" (%d: "
else|:
literal|" (%Xd: "
argument_list|,
name|err
argument_list|)
expr_stmt|;
else|#
directive|else
name|buf
operator|=
name|ngx_slprintf
argument_list|(
name|buf
argument_list|,
name|last
argument_list|,
literal|" (%d: "
argument_list|,
name|err
argument_list|)
expr_stmt|;
endif|#
directive|endif
name|buf
operator|=
name|ngx_strerror
argument_list|(
name|err
argument_list|,
name|buf
argument_list|,
name|last
operator|-
name|buf
argument_list|)
expr_stmt|;
if|if
condition|(
name|buf
operator|<
name|last
condition|)
block|{
operator|*
name|buf
operator|++
operator|=
literal|')'
expr_stmt|;
block|}
return|return
name|buf
return|;
block|}
end_function

begin_function
name|ngx_log_t
modifier|*
DECL|function|ngx_log_init (u_char * prefix)
name|ngx_log_init
parameter_list|(
name|u_char
modifier|*
name|prefix
parameter_list|)
block|{
name|u_char
modifier|*
name|p
decl_stmt|,
modifier|*
name|name
decl_stmt|;
name|size_t
name|nlen
decl_stmt|,
name|plen
decl_stmt|;
name|ngx_log
operator|.
name|file
operator|=
operator|&
name|ngx_log_file
expr_stmt|;
name|ngx_log
operator|.
name|log_level
operator|=
name|NGX_LOG_NOTICE
expr_stmt|;
name|name
operator|=
operator|(
name|u_char
operator|*
operator|)
name|NGX_ERROR_LOG_PATH
expr_stmt|;
comment|/*      * we use ngx_strlen() here since BCC warns about      * condition is always false and unreachable code      */
name|nlen
operator|=
name|ngx_strlen
argument_list|(
name|name
argument_list|)
expr_stmt|;
if|if
condition|(
name|nlen
operator|==
literal|0
condition|)
block|{
name|ngx_log_file
operator|.
name|fd
operator|=
name|ngx_stderr
expr_stmt|;
return|return
operator|&
name|ngx_log
return|;
block|}
name|p
operator|=
name|NULL
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_WIN32
operator|)
if|if
condition|(
name|name
index|[
literal|1
index|]
operator|!=
literal|':'
condition|)
block|{
else|#
directive|else
if|if
condition|(
name|name
index|[
literal|0
index|]
operator|!=
literal|'/'
condition|)
block|{
endif|#
directive|endif
if|if
condition|(
name|prefix
condition|)
block|{
name|plen
operator|=
name|ngx_strlen
argument_list|(
name|prefix
argument_list|)
expr_stmt|;
block|}
else|else
block|{
ifdef|#
directive|ifdef
name|NGX_PREFIX
name|prefix
operator|=
operator|(
name|u_char
operator|*
operator|)
name|NGX_PREFIX
expr_stmt|;
name|plen
operator|=
name|ngx_strlen
argument_list|(
name|prefix
argument_list|)
expr_stmt|;
else|#
directive|else
name|plen
operator|=
literal|0
expr_stmt|;
endif|#
directive|endif
block|}
if|if
condition|(
name|plen
condition|)
block|{
name|name
operator|=
name|malloc
argument_list|(
name|plen
operator|+
name|nlen
operator|+
literal|2
argument_list|)
expr_stmt|;
if|if
condition|(
name|name
operator|==
name|NULL
condition|)
block|{
return|return
name|NULL
return|;
block|}
name|p
operator|=
name|ngx_cpymem
argument_list|(
name|name
argument_list|,
name|prefix
argument_list|,
name|plen
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|ngx_path_separator
argument_list|(
operator|*
operator|(
name|p
operator|-
literal|1
operator|)
argument_list|)
condition|)
block|{
operator|*
name|p
operator|++
operator|=
literal|'/'
expr_stmt|;
block|}
name|ngx_cpystrn
argument_list|(
name|p
argument_list|,
operator|(
name|u_char
operator|*
operator|)
name|NGX_ERROR_LOG_PATH
argument_list|,
name|nlen
operator|+
literal|1
argument_list|)
expr_stmt|;
name|p
operator|=
name|name
expr_stmt|;
block|}
block|}
name|ngx_log_file
operator|.
name|fd
operator|=
name|ngx_open_file
argument_list|(
name|name
argument_list|,
name|NGX_FILE_APPEND
argument_list|,
name|NGX_FILE_CREATE_OR_OPEN
argument_list|,
name|NGX_FILE_DEFAULT_ACCESS
argument_list|)
expr_stmt|;
if|if
condition|(
name|ngx_log_file
operator|.
name|fd
operator|==
name|NGX_INVALID_FILE
condition|)
block|{
name|ngx_log_stderr
argument_list|(
name|ngx_errno
argument_list|,
literal|"[alert] could not open error log file: "
name|ngx_open_file_n
literal|" \"%s\" failed"
argument_list|,
name|name
argument_list|)
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_WIN32
operator|)
name|ngx_event_log
argument_list|(
name|ngx_errno
argument_list|,
literal|"could not open error log file: "
name|ngx_open_file_n
literal|" \"%s\" failed"
argument_list|,
name|name
argument_list|)
expr_stmt|;
endif|#
directive|endif
name|ngx_log_file
operator|.
name|fd
operator|=
name|ngx_stderr
expr_stmt|;
block|}
if|if
condition|(
name|p
condition|)
block|{
name|ngx_free
argument_list|(
name|p
argument_list|)
expr_stmt|;
block|}
return|return
operator|&
name|ngx_log
return|;
block|}
name|ngx_int_t
DECL|function|ngx_log_open_default (ngx_cycle_t * cycle)
name|ngx_log_open_default
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
block|{
name|ngx_log_t
modifier|*
name|log
decl_stmt|;
specifier|static
name|ngx_str_t
name|error_log
init|=
name|ngx_string
argument_list|(
name|NGX_ERROR_LOG_PATH
argument_list|)
decl_stmt|;
if|if
condition|(
name|ngx_log_get_file_log
argument_list|(
operator|&
name|cycle
operator|->
name|new_log
argument_list|)
operator|!=
name|NULL
condition|)
block|{
return|return
name|NGX_OK
return|;
block|}
if|if
condition|(
name|cycle
operator|->
name|new_log
operator|.
name|log_level
operator|!=
literal|0
condition|)
block|{
comment|/* there are some error logs, but no files */
name|log
operator|=
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
expr_stmt|;
if|if
condition|(
name|log
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
block|}
else|else
block|{
comment|/* no error logs at all */
name|log
operator|=
operator|&
name|cycle
operator|->
name|new_log
expr_stmt|;
block|}
name|log
operator|->
name|log_level
operator|=
name|NGX_LOG_ERR
expr_stmt|;
name|log
operator|->
name|file
operator|=
name|ngx_conf_open_file
argument_list|(
name|cycle
argument_list|,
operator|&
name|error_log
argument_list|)
expr_stmt|;
if|if
condition|(
name|log
operator|->
name|file
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|log
operator|!=
operator|&
name|cycle
operator|->
name|new_log
condition|)
block|{
name|ngx_log_insert
argument_list|(
operator|&
name|cycle
operator|->
name|new_log
argument_list|,
name|log
argument_list|)
expr_stmt|;
block|}
return|return
name|NGX_OK
return|;
block|}
name|ngx_int_t
DECL|function|ngx_log_redirect_stderr (ngx_cycle_t * cycle)
name|ngx_log_redirect_stderr
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
block|{
name|ngx_fd_t
name|fd
decl_stmt|;
if|if
condition|(
name|cycle
operator|->
name|log_use_stderr
condition|)
block|{
return|return
name|NGX_OK
return|;
block|}
comment|/* file log always exists when we are called */
name|fd
operator|=
name|ngx_log_get_file_log
argument_list|(
name|cycle
operator|->
name|log
argument_list|)
operator|->
name|file
operator|->
name|fd
expr_stmt|;
if|if
condition|(
name|fd
operator|!=
name|ngx_stderr
condition|)
block|{
if|if
condition|(
name|ngx_set_stderr
argument_list|(
name|fd
argument_list|)
operator|==
name|NGX_FILE_ERROR
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
name|ngx_set_stderr_n
literal|" failed"
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
name|ngx_log_t
modifier|*
DECL|function|ngx_log_get_file_log (ngx_log_t * head)
name|ngx_log_get_file_log
parameter_list|(
name|ngx_log_t
modifier|*
name|head
parameter_list|)
block|{
name|ngx_log_t
modifier|*
name|log
decl_stmt|;
for|for
control|(
name|log
operator|=
name|head
init|;
name|log
condition|;
name|log
operator|=
name|log
operator|->
name|next
control|)
block|{
if|if
condition|(
name|log
operator|->
name|file
operator|!=
name|NULL
condition|)
block|{
return|return
name|log
return|;
block|}
block|}
return|return
name|NULL
return|;
block|}
specifier|static
name|char
modifier|*
DECL|function|ngx_log_set_levels (ngx_conf_t * cf,ngx_log_t * log)
name|ngx_log_set_levels
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|ngx_log_t
modifier|*
name|log
parameter_list|)
block|{
name|ngx_uint_t
name|i
decl_stmt|,
name|n
decl_stmt|,
name|d
decl_stmt|,
name|found
decl_stmt|;
name|ngx_str_t
modifier|*
name|value
decl_stmt|;
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
name|log
operator|->
name|log_level
operator|=
name|NGX_LOG_ERR
expr_stmt|;
return|return
name|NGX_CONF_OK
return|;
block|}
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
literal|2
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
name|found
operator|=
literal|0
expr_stmt|;
for|for
control|(
name|n
operator|=
literal|1
init|;
name|n
operator|<=
name|NGX_LOG_DEBUG
condition|;
name|n
operator|++
control|)
block|{
if|if
condition|(
name|ngx_strcmp
argument_list|(
name|value
index|[
name|i
index|]
operator|.
name|data
argument_list|,
name|err_levels
index|[
name|n
index|]
operator|.
name|data
argument_list|)
operator|==
literal|0
condition|)
block|{
if|if
condition|(
name|log
operator|->
name|log_level
operator|!=
literal|0
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
literal|"duplicate log level \"%V\""
argument_list|,
operator|&
name|value
index|[
name|i
index|]
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|log
operator|->
name|log_level
operator|=
name|n
expr_stmt|;
name|found
operator|=
literal|1
expr_stmt|;
break|break;
block|}
block|}
for|for
control|(
name|n
operator|=
literal|0
operator|,
name|d
operator|=
name|NGX_LOG_DEBUG_FIRST
init|;
name|d
operator|<=
name|NGX_LOG_DEBUG_LAST
condition|;
name|d
operator|<<=
literal|1
control|)
block|{
if|if
condition|(
name|ngx_strcmp
argument_list|(
name|value
index|[
name|i
index|]
operator|.
name|data
argument_list|,
name|debug_levels
index|[
name|n
operator|++
index|]
argument_list|)
operator|==
literal|0
condition|)
block|{
if|if
condition|(
name|log
operator|->
name|log_level
operator|&
operator|~
name|NGX_LOG_DEBUG_ALL
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
literal|"invalid log level \"%V\""
argument_list|,
operator|&
name|value
index|[
name|i
index|]
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|log
operator|->
name|log_level
operator||=
name|d
expr_stmt|;
name|found
operator|=
literal|1
expr_stmt|;
break|break;
block|}
block|}
if|if
condition|(
operator|!
name|found
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
literal|"invalid log level \"%V\""
argument_list|,
operator|&
name|value
index|[
name|i
index|]
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
block|}
if|if
condition|(
name|log
operator|->
name|log_level
operator|==
name|NGX_LOG_DEBUG
condition|)
block|{
name|log
operator|->
name|log_level
operator|=
name|NGX_LOG_DEBUG_ALL
expr_stmt|;
block|}
return|return
name|NGX_CONF_OK
return|;
block|}
specifier|static
name|char
modifier|*
DECL|function|ngx_error_log (ngx_conf_t * cf,ngx_command_t * cmd,void * conf)
name|ngx_error_log
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
name|ngx_log_t
modifier|*
name|dummy
decl_stmt|;
name|dummy
operator|=
operator|&
name|cf
operator|->
name|cycle
operator|->
name|new_log
expr_stmt|;
return|return
name|ngx_log_set_log
argument_list|(
name|cf
argument_list|,
operator|&
name|dummy
argument_list|)
return|;
block|}
name|char
modifier|*
DECL|function|ngx_log_set_log (ngx_conf_t * cf,ngx_log_t ** head)
name|ngx_log_set_log
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|ngx_log_t
modifier|*
modifier|*
name|head
parameter_list|)
block|{
name|ngx_log_t
modifier|*
name|new_log
decl_stmt|;
name|ngx_str_t
modifier|*
name|value
decl_stmt|,
name|name
decl_stmt|;
name|ngx_syslog_peer_t
modifier|*
name|peer
decl_stmt|;
if|if
condition|(
operator|*
name|head
operator|!=
name|NULL
operator|&&
operator|(
operator|*
name|head
operator|)
operator|->
name|log_level
operator|==
literal|0
condition|)
block|{
name|new_log
operator|=
operator|*
name|head
expr_stmt|;
block|}
else|else
block|{
name|new_log
operator|=
name|ngx_pcalloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_log_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|new_log
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
if|if
condition|(
operator|*
name|head
operator|==
name|NULL
condition|)
block|{
operator|*
name|head
operator|=
name|new_log
expr_stmt|;
block|}
block|}
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
name|ngx_str_null
argument_list|(
operator|&
name|name
argument_list|)
expr_stmt|;
name|cf
operator|->
name|cycle
operator|->
name|log_use_stderr
operator|=
literal|1
expr_stmt|;
name|new_log
operator|->
name|file
operator|=
name|ngx_conf_open_file
argument_list|(
name|cf
operator|->
name|cycle
argument_list|,
operator|&
name|name
argument_list|)
expr_stmt|;
if|if
condition|(
name|new_log
operator|->
name|file
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
block|}
if|else if
condition|(
name|ngx_strncmp
argument_list|(
name|value
index|[
literal|1
index|]
operator|.
name|data
argument_list|,
literal|"syslog:"
argument_list|,
literal|7
argument_list|)
operator|==
literal|0
condition|)
block|{
name|peer
operator|=
name|ngx_pcalloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_syslog_peer_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|peer
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
if|if
condition|(
name|ngx_syslog_process_conf
argument_list|(
name|cf
argument_list|,
name|peer
argument_list|)
operator|!=
name|NGX_CONF_OK
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|new_log
operator|->
name|writer
operator|=
name|ngx_syslog_writer
expr_stmt|;
name|new_log
operator|->
name|wdata
operator|=
name|peer
expr_stmt|;
block|}
else|else
block|{
name|new_log
operator|->
name|file
operator|=
name|ngx_conf_open_file
argument_list|(
name|cf
operator|->
name|cycle
argument_list|,
operator|&
name|value
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
if|if
condition|(
name|new_log
operator|->
name|file
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
block|}
if|if
condition|(
name|ngx_log_set_levels
argument_list|(
name|cf
argument_list|,
name|new_log
argument_list|)
operator|!=
name|NGX_CONF_OK
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
if|if
condition|(
operator|*
name|head
operator|!=
name|new_log
condition|)
block|{
name|ngx_log_insert
argument_list|(
operator|*
name|head
argument_list|,
name|new_log
argument_list|)
expr_stmt|;
block|}
return|return
name|NGX_CONF_OK
return|;
block|}
specifier|static
name|void
DECL|function|ngx_log_insert (ngx_log_t * log,ngx_log_t * new_log)
name|ngx_log_insert
parameter_list|(
name|ngx_log_t
modifier|*
name|log
parameter_list|,
name|ngx_log_t
modifier|*
name|new_log
parameter_list|)
block|{
name|ngx_log_t
name|tmp
decl_stmt|;
if|if
condition|(
name|new_log
operator|->
name|log_level
operator|>
name|log
operator|->
name|log_level
condition|)
block|{
comment|/*          * list head address is permanent, insert new log after          * head and swap its contents with head          */
name|tmp
operator|=
operator|*
name|log
expr_stmt|;
operator|*
name|log
operator|=
operator|*
name|new_log
expr_stmt|;
operator|*
name|new_log
operator|=
name|tmp
expr_stmt|;
name|log
operator|->
name|next
operator|=
name|new_log
expr_stmt|;
return|return;
block|}
while|while
condition|(
name|log
operator|->
name|next
condition|)
block|{
if|if
condition|(
name|new_log
operator|->
name|log_level
operator|>
name|log
operator|->
name|next
operator|->
name|log_level
condition|)
block|{
name|new_log
operator|->
name|next
operator|=
name|log
operator|->
name|next
expr_stmt|;
name|log
operator|->
name|next
operator|=
name|new_log
expr_stmt|;
return|return;
block|}
name|log
operator|=
name|log
operator|->
name|next
expr_stmt|;
block|}
name|log
operator|->
name|next
operator|=
name|new_log
expr_stmt|;
block|}
end_function

end_unit

