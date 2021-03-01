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

begin_comment
comment|/*  * The strerror() messages are copied because:  *  * 1) strerror() and strerror_r() functions are not Async-Signal-Safe,  *    therefore, they cannot be used in signal handlers;  *  * 2) a direct sys_errlist[] array may be used instead of these functions,  *    but Linux linker warns about its usage:  *  * warning: `sys_errlist' is deprecated; use `strerror' or `strerror_r' instead  * warning: `sys_nerr' is deprecated; use `strerror' or `strerror_r' instead  *  *    causing false bug reports.  */
end_comment

begin_decl_stmt
DECL|variable|ngx_sys_errlist
specifier|static
name|ngx_str_t
modifier|*
name|ngx_sys_errlist
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_unknown_error
specifier|static
name|ngx_str_t
name|ngx_unknown_error
init|=
name|ngx_string
argument_list|(
literal|"Unknown error"
argument_list|)
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_first_error
specifier|static
name|ngx_err_t
name|ngx_first_error
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_last_error
specifier|static
name|ngx_err_t
name|ngx_last_error
decl_stmt|;
end_decl_stmt

begin_function
name|u_char
modifier|*
DECL|function|ngx_strerror (ngx_err_t err,u_char * errstr,size_t size)
name|ngx_strerror
parameter_list|(
name|ngx_err_t
name|err
parameter_list|,
name|u_char
modifier|*
name|errstr
parameter_list|,
name|size_t
name|size
parameter_list|)
block|{
name|ngx_str_t
modifier|*
name|msg
decl_stmt|;
if|if
condition|(
name|err
operator|>=
name|ngx_first_error
operator|&&
name|err
operator|<
name|ngx_last_error
condition|)
block|{
name|msg
operator|=
operator|&
name|ngx_sys_errlist
index|[
name|err
operator|-
name|ngx_first_error
index|]
expr_stmt|;
block|}
else|else
block|{
name|msg
operator|=
operator|&
name|ngx_unknown_error
expr_stmt|;
block|}
name|size
operator|=
name|ngx_min
argument_list|(
name|size
argument_list|,
name|msg
operator|->
name|len
argument_list|)
expr_stmt|;
return|return
name|ngx_cpymem
argument_list|(
name|errstr
argument_list|,
name|msg
operator|->
name|data
argument_list|,
name|size
argument_list|)
return|;
block|}
end_function

begin_function
name|ngx_int_t
DECL|function|ngx_strerror_init (void)
name|ngx_strerror_init
parameter_list|(
name|void
parameter_list|)
block|{
name|char
modifier|*
name|msg
decl_stmt|;
name|u_char
modifier|*
name|p
decl_stmt|;
name|size_t
name|len
decl_stmt|;
name|ngx_err_t
name|err
decl_stmt|;
if|#
directive|if
operator|(
name|NGX_SYS_NERR
operator|)
name|ngx_first_error
operator|=
literal|0
expr_stmt|;
name|ngx_last_error
operator|=
name|NGX_SYS_NERR
expr_stmt|;
elif|#
directive|elif
operator|(
name|EPERM
operator|>
literal|1000
operator|&&
name|EPERM
operator|<
literal|0x7fffffff
operator|-
literal|1000
operator|)
comment|/*      * If number of errors is not known, and EPERM error code has large      * but reasonable value, guess possible error codes based on the error      * messages returned by strerror(), starting from EPERM.  Notably,      * this covers GNU/Hurd, where errors start at 0x40000001.      */
for|for
control|(
name|err
operator|=
name|EPERM
init|;
name|err
operator|>
name|EPERM
operator|-
literal|1000
condition|;
name|err
operator|--
control|)
block|{
name|ngx_set_errno
argument_list|(
literal|0
argument_list|)
expr_stmt|;
name|msg
operator|=
name|strerror
argument_list|(
name|err
argument_list|)
expr_stmt|;
if|if
condition|(
name|errno
operator|==
name|EINVAL
operator|||
name|msg
operator|==
name|NULL
operator|||
name|strncmp
argument_list|(
name|msg
argument_list|,
literal|"Unknown error"
argument_list|,
literal|13
argument_list|)
operator|==
literal|0
condition|)
block|{
continue|continue;
block|}
name|ngx_first_error
operator|=
name|err
expr_stmt|;
block|}
for|for
control|(
name|err
operator|=
name|EPERM
init|;
name|err
operator|<
name|EPERM
operator|+
literal|1000
condition|;
name|err
operator|++
control|)
block|{
name|ngx_set_errno
argument_list|(
literal|0
argument_list|)
expr_stmt|;
name|msg
operator|=
name|strerror
argument_list|(
name|err
argument_list|)
expr_stmt|;
if|if
condition|(
name|errno
operator|==
name|EINVAL
operator|||
name|msg
operator|==
name|NULL
operator|||
name|strncmp
argument_list|(
name|msg
argument_list|,
literal|"Unknown error"
argument_list|,
literal|13
argument_list|)
operator|==
literal|0
condition|)
block|{
continue|continue;
block|}
name|ngx_last_error
operator|=
name|err
operator|+
literal|1
expr_stmt|;
block|}
else|#
directive|else
comment|/*      * If number of errors is not known, guess it based on the error      * messages returned by strerror().      */
name|ngx_first_error
operator|=
literal|0
expr_stmt|;
for|for
control|(
name|err
operator|=
literal|0
init|;
name|err
operator|<
literal|1000
condition|;
name|err
operator|++
control|)
block|{
name|ngx_set_errno
argument_list|(
literal|0
argument_list|)
expr_stmt|;
name|msg
operator|=
name|strerror
argument_list|(
name|err
argument_list|)
expr_stmt|;
if|if
condition|(
name|errno
operator|==
name|EINVAL
operator|||
name|msg
operator|==
name|NULL
operator|||
name|strncmp
argument_list|(
name|msg
argument_list|,
literal|"Unknown error"
argument_list|,
literal|13
argument_list|)
operator|==
literal|0
condition|)
block|{
continue|continue;
block|}
name|ngx_last_error
operator|=
name|err
operator|+
literal|1
expr_stmt|;
block|}
endif|#
directive|endif
comment|/*      * ngx_strerror() is not ready to work at this stage, therefore,      * malloc() is used and possible errors are logged using strerror().      */
name|len
operator|=
operator|(
name|ngx_last_error
operator|-
name|ngx_first_error
operator|)
operator|*
sizeof|sizeof
argument_list|(
name|ngx_str_t
argument_list|)
expr_stmt|;
name|ngx_sys_errlist
operator|=
name|malloc
argument_list|(
name|len
argument_list|)
expr_stmt|;
if|if
condition|(
name|ngx_sys_errlist
operator|==
name|NULL
condition|)
block|{
goto|goto
name|failed
goto|;
block|}
for|for
control|(
name|err
operator|=
name|ngx_first_error
init|;
name|err
operator|<
name|ngx_last_error
condition|;
name|err
operator|++
control|)
block|{
name|msg
operator|=
name|strerror
argument_list|(
name|err
argument_list|)
expr_stmt|;
if|if
condition|(
name|msg
operator|==
name|NULL
condition|)
block|{
name|ngx_sys_errlist
index|[
name|err
operator|-
name|ngx_first_error
index|]
operator|=
name|ngx_unknown_error
expr_stmt|;
continue|continue;
block|}
name|len
operator|=
name|ngx_strlen
argument_list|(
name|msg
argument_list|)
expr_stmt|;
name|p
operator|=
name|malloc
argument_list|(
name|len
argument_list|)
expr_stmt|;
if|if
condition|(
name|p
operator|==
name|NULL
condition|)
block|{
goto|goto
name|failed
goto|;
block|}
name|ngx_memcpy
argument_list|(
name|p
argument_list|,
name|msg
argument_list|,
name|len
argument_list|)
expr_stmt|;
name|ngx_sys_errlist
index|[
name|err
operator|-
name|ngx_first_error
index|]
operator|.
name|len
operator|=
name|len
expr_stmt|;
name|ngx_sys_errlist
index|[
name|err
operator|-
name|ngx_first_error
index|]
operator|.
name|data
operator|=
name|p
expr_stmt|;
block|}
return|return
name|NGX_OK
return|;
name|failed
label|:
name|err
operator|=
name|errno
expr_stmt|;
name|ngx_log_stderr
argument_list|(
literal|0
argument_list|,
literal|"malloc(%uz) failed (%d: %s)"
argument_list|,
name|len
argument_list|,
name|err
argument_list|,
name|strerror
argument_list|(
name|err
argument_list|)
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
end_function

end_unit

