begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_comment
comment|/*    TODO: log pid and tid */
end_comment

begin_comment
comment|/*    "[time as ctime()] [alert] 412:3 (32)Broken pipe: anything"     "[time as ctime()] [alert] (32)Broken pipe: anything"    "[time as ctime()] [alert] anything" */
end_comment

begin_include
include|#
directive|include
file|<ngx_config.h>
end_include

begin_include
include|#
directive|include
file|<ngx_errno.h>
end_include

begin_include
include|#
directive|include
file|<ngx_time.h>
end_include

begin_include
include|#
directive|include
file|<ngx_process.h>
end_include

begin_include
include|#
directive|include
file|<ngx_string.h>
end_include

begin_include
include|#
directive|include
file|<ngx_log.h>
end_include

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
literal|"%d#%d: "
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
else|else
endif|#
directive|endif
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
endif|#
directive|endif
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
literal|2
argument_list|,
name|errstr
argument_list|,
name|len
argument_list|)
expr_stmt|;
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

end_unit

