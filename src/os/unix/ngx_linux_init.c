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
DECL|variable|ngx_linux_kern_ostype
name|u_char
name|ngx_linux_kern_ostype
index|[
literal|50
index|]
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_linux_kern_osrelease
name|u_char
name|ngx_linux_kern_osrelease
index|[
literal|50
index|]
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_linux_rtsig_max
name|int
name|ngx_linux_rtsig_max
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_linux_io
specifier|static
name|ngx_os_io_t
name|ngx_linux_io
init|=
block|{
name|ngx_unix_recv
block|,
name|ngx_readv_chain
block|,
name|ngx_udp_unix_recv
block|,
name|ngx_unix_send
block|,
if|#
directive|if
operator|(
name|NGX_HAVE_SENDFILE
operator|)
name|ngx_linux_sendfile_chain
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
name|ngx_int_t
DECL|function|ngx_os_specific_init (ngx_log_t * log)
name|ngx_os_specific_init
parameter_list|(
name|ngx_log_t
modifier|*
name|log
parameter_list|)
block|{
name|struct
name|utsname
name|u
decl_stmt|;
if|if
condition|(
name|uname
argument_list|(
operator|&
name|u
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
literal|"uname() failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
operator|(
name|void
operator|)
name|ngx_cpystrn
argument_list|(
name|ngx_linux_kern_ostype
argument_list|,
operator|(
name|u_char
operator|*
operator|)
name|u
operator|.
name|sysname
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_linux_kern_ostype
argument_list|)
argument_list|)
expr_stmt|;
operator|(
name|void
operator|)
name|ngx_cpystrn
argument_list|(
name|ngx_linux_kern_osrelease
argument_list|,
operator|(
name|u_char
operator|*
operator|)
name|u
operator|.
name|release
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_linux_kern_osrelease
argument_list|)
argument_list|)
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_HAVE_RTSIG
operator|)
block|{
name|int
name|name
index|[
literal|2
index|]
decl_stmt|;
name|size_t
name|len
decl_stmt|;
name|ngx_err_t
name|err
decl_stmt|;
name|name
index|[
literal|0
index|]
operator|=
name|CTL_KERN
expr_stmt|;
name|name
index|[
literal|1
index|]
operator|=
name|KERN_RTSIGMAX
expr_stmt|;
name|len
operator|=
sizeof|sizeof
argument_list|(
name|ngx_linux_rtsig_max
argument_list|)
expr_stmt|;
if|if
condition|(
name|sysctl
argument_list|(
name|name
argument_list|,
literal|2
argument_list|,
operator|&
name|ngx_linux_rtsig_max
argument_list|,
operator|&
name|len
argument_list|,
name|NULL
argument_list|,
literal|0
argument_list|)
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
operator|!=
name|NGX_ENOTDIR
operator|&&
name|err
operator|!=
name|NGX_ENOSYS
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|log
argument_list|,
name|err
argument_list|,
literal|"sysctl(KERN_RTSIGMAX) failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
name|ngx_linux_rtsig_max
operator|=
literal|0
expr_stmt|;
block|}
block|}
endif|#
directive|endif
name|ngx_os_io
operator|=
name|ngx_linux_io
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
name|void
DECL|function|ngx_os_specific_status (ngx_log_t * log)
name|ngx_os_specific_status
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
name|ngx_linux_kern_ostype
argument_list|,
name|ngx_linux_kern_osrelease
argument_list|)
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_HAVE_RTSIG
operator|)
name|ngx_log_error
argument_list|(
name|NGX_LOG_NOTICE
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"sysctl(KERN_RTSIGMAX): %d"
argument_list|,
name|ngx_linux_rtsig_max
argument_list|)
expr_stmt|;
endif|#
directive|endif
block|}
end_function

end_unit

