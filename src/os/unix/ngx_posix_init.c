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
file|<nginx.h>
end_include

begin_decl_stmt
DECL|variable|ngx_ncpu
name|ngx_int_t
name|ngx_ncpu
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_max_sockets
name|ngx_int_t
name|ngx_max_sockets
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_inherited_nonblocking
name|ngx_uint_t
name|ngx_inherited_nonblocking
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_tcp_nodelay_and_tcp_nopush
name|ngx_uint_t
name|ngx_tcp_nodelay_and_tcp_nopush
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|rlmt
name|struct
name|rlimit
name|rlmt
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
name|NULL
block|,
name|ngx_writev_chain
block|,
literal|0
block|}
decl_stmt|;
end_decl_stmt

begin_function
name|ngx_int_t
DECL|function|ngx_os_init (ngx_log_t * log)
name|ngx_os_init
parameter_list|(
name|ngx_log_t
modifier|*
name|log
parameter_list|)
block|{
if|#
directive|if
operator|(
name|NGX_HAVE_OS_SPECIFIC_INIT
operator|)
if|if
condition|(
name|ngx_os_specific_init
argument_list|(
name|log
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
endif|#
directive|endif
name|ngx_init_setproctitle
argument_list|(
name|log
argument_list|)
expr_stmt|;
name|ngx_pagesize
operator|=
name|getpagesize
argument_list|()
expr_stmt|;
name|ngx_cacheline_size
operator|=
name|NGX_CPU_CACHE_LINE
expr_stmt|;
if|if
condition|(
name|ngx_ncpu
operator|==
literal|0
condition|)
block|{
name|ngx_ncpu
operator|=
literal|1
expr_stmt|;
block|}
name|ngx_cpuinfo
argument_list|()
expr_stmt|;
if|if
condition|(
name|getrlimit
argument_list|(
name|RLIMIT_NOFILE
argument_list|,
operator|&
name|rlmt
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
literal|"getrlimit(RLIMIT_NOFILE) failed)"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
name|ngx_max_sockets
operator|=
operator|(
name|ngx_int_t
operator|)
name|rlmt
operator|.
name|rlim_cur
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_HAVE_INHERITED_NONBLOCK
operator|)
name|ngx_inherited_nonblocking
operator|=
literal|1
expr_stmt|;
else|#
directive|else
name|ngx_inherited_nonblocking
operator|=
literal|0
expr_stmt|;
endif|#
directive|endif
name|srandom
argument_list|(
name|ngx_time
argument_list|()
argument_list|)
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
name|void
DECL|function|ngx_os_status (ngx_log_t * log)
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
name|NGINX_VER
argument_list|)
expr_stmt|;
ifdef|#
directive|ifdef
name|NGX_COMPILER
name|ngx_log_error
argument_list|(
name|NGX_LOG_NOTICE
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"built by "
name|NGX_COMPILER
argument_list|)
expr_stmt|;
endif|#
directive|endif
if|#
directive|if
operator|(
name|NGX_HAVE_OS_SPECIFIC_INIT
operator|)
name|ngx_os_specific_status
argument_list|(
name|log
argument_list|)
expr_stmt|;
endif|#
directive|endif
name|ngx_log_error
argument_list|(
name|NGX_LOG_NOTICE
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"getrlimit(RLIMIT_NOFILE): %r:%r"
argument_list|,
name|rlmt
operator|.
name|rlim_cur
argument_list|,
name|rlmt
operator|.
name|rlim_max
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
name|ngx_int_t
DECL|function|ngx_posix_post_conf_init (ngx_log_t * log)
name|ngx_posix_post_conf_init
parameter_list|(
name|ngx_log_t
modifier|*
name|log
parameter_list|)
block|{
name|ngx_fd_t
name|pp
index|[
literal|2
index|]
decl_stmt|;
if|if
condition|(
name|pipe
argument_list|(
name|pp
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
literal|"pipe() failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|dup2
argument_list|(
name|pp
index|[
literal|1
index|]
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
name|errno
argument_list|,
literal|"dup2(STDERR) failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|pp
index|[
literal|1
index|]
operator|>
name|STDERR_FILENO
condition|)
block|{
if|if
condition|(
name|close
argument_list|(
name|pp
index|[
literal|1
index|]
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
name|errno
argument_list|,
literal|"close() failed"
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
end_function

end_unit

