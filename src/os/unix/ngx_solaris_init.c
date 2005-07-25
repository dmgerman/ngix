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

begin_decl_stmt
DECL|variable|ngx_solaris_sysname
name|char
name|ngx_solaris_sysname
index|[
literal|20
index|]
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_solaris_release
name|char
name|ngx_solaris_release
index|[
literal|10
index|]
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_solaris_version
name|char
name|ngx_solaris_version
index|[
literal|50
index|]
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_os_io
name|ngx_os_io_t
name|ngx_os_io
init|=
block|{
name|ngx_unix_recv
block|,
name|ngx_readv_chain
block|,
name|ngx_unix_send
block|,
if|#
directive|if
operator|(
name|NGX_HAVE_SENDFILE
operator|)
name|ngx_solaris_sendfilev_chain
block|,
name|NGX_IO_SENDFILE
else|#
directive|else
name|ngx_writev_chain
block|,
literal|0
endif|#
directive|endif
block|}
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|ngx_os_init (ngx_log_t * log)
name|ngx_int_t
name|ngx_os_init
parameter_list|(
name|ngx_log_t
modifier|*
name|log
parameter_list|)
block|{
if|if
condition|(
name|sysinfo
argument_list|(
name|SI_SYSNAME
argument_list|,
name|ngx_solaris_sysname
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_solaris_sysname
argument_list|)
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
name|ngx_errno
argument_list|,
literal|"sysinfo(SI_SYSNAME) failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|sysinfo
argument_list|(
name|SI_RELEASE
argument_list|,
name|ngx_solaris_release
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_solaris_release
argument_list|)
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
name|ngx_errno
argument_list|,
literal|"sysinfo(SI_RELEASE) failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|sysinfo
argument_list|(
name|SI_VERSION
argument_list|,
name|ngx_solaris_version
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_solaris_version
argument_list|)
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
name|ngx_errno
argument_list|,
literal|"sysinfo(SI_SYSNAME) failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
return|return
name|ngx_posix_init
argument_list|(
name|log
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|ngx_os_status (ngx_log_t * log)
name|void
name|ngx_os_status
parameter_list|(
name|ngx_log_t
modifier|*
name|log
parameter_list|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_NOTICE
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"OS: %s %s"
argument_list|,
name|ngx_solaris_sysname
argument_list|,
name|ngx_solaris_release
argument_list|)
expr_stmt|;
name|ngx_log_error
argument_list|(
name|NGX_LOG_NOTICE
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"version: %s"
argument_list|,
name|ngx_solaris_version
argument_list|)
expr_stmt|;
name|ngx_posix_status
argument_list|(
name|log
argument_list|)
expr_stmt|;
block|}
end_function

end_unit

