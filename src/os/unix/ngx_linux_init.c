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

begin_decl_stmt
DECL|variable|ngx_linux_kern_ostype
name|char
name|ngx_linux_kern_ostype
index|[
literal|50
index|]
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_linux_kern_osrelease
name|char
name|ngx_linux_kern_osrelease
index|[
literal|20
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
name|NULL
block|,
name|NULL
block|,
name|ngx_linux_sendfile_chain
block|,
name|NGX_IO_SENDFILE
block|}
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|ngx_os_init (ngx_log_t * log)
name|int
name|ngx_os_init
parameter_list|(
name|ngx_log_t
modifier|*
name|log
parameter_list|)
block|{
name|int
name|name
index|[
literal|2
index|]
decl_stmt|,
name|len
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
name|KERN_OSTYPE
expr_stmt|;
name|len
operator|=
sizeof|sizeof
argument_list|(
name|ngx_linux_kern_ostype
argument_list|)
expr_stmt|;
if|if
condition|(
name|sysctl
argument_list|(
name|name
argument_list|,
sizeof|sizeof
argument_list|(
name|name
argument_list|)
argument_list|,
name|ngx_linux_kern_ostype
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
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|log
argument_list|,
name|errno
argument_list|,
literal|"sysctl(KERN_OSTYPE) failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
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
name|KERN_OSRELEASE
expr_stmt|;
name|len
operator|=
sizeof|sizeof
argument_list|(
name|ngx_linux_kern_osrelease
argument_list|)
expr_stmt|;
if|if
condition|(
name|sysctl
argument_list|(
name|name
argument_list|,
sizeof|sizeof
argument_list|(
name|name
argument_list|)
argument_list|,
name|ngx_linux_kern_osrelease
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
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|log
argument_list|,
name|errno
argument_list|,
literal|"sysctl(KERN_OSRELEASE) failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
name|ngx_log_error
argument_list|(
name|NGX_LOG_INFO
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
return|return
name|ngx_posix_init
argument_list|(
name|log
argument_list|)
return|;
block|}
end_function

end_unit

