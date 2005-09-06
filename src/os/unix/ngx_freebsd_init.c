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

begin_comment
comment|/* FreeBSD 3.0 at least */
end_comment

begin_decl_stmt
DECL|variable|ngx_freebsd_kern_ostype
name|char
name|ngx_freebsd_kern_ostype
index|[
literal|16
index|]
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_freebsd_kern_osrelease
name|char
name|ngx_freebsd_kern_osrelease
index|[
literal|128
index|]
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_freebsd_kern_osreldate
name|int
name|ngx_freebsd_kern_osreldate
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_freebsd_hw_ncpu
name|int
name|ngx_freebsd_hw_ncpu
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_freebsd_net_inet_tcp_sendspace
name|int
name|ngx_freebsd_net_inet_tcp_sendspace
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_freebsd_kern_ipc_somaxconn
name|int
name|ngx_freebsd_kern_ipc_somaxconn
decl_stmt|;
end_decl_stmt

begin_comment
comment|/* FreeBSD 4.9 */
end_comment

begin_decl_stmt
DECL|variable|ngx_freebsd_machdep_hlt_logical_cpus
name|int
name|ngx_freebsd_machdep_hlt_logical_cpus
decl_stmt|;
end_decl_stmt

begin_comment
comment|/* FreeBSD 5.0 */
end_comment

begin_decl_stmt
DECL|variable|ngx_freebsd_kern_ipc_zero_copy_send
name|int
name|ngx_freebsd_kern_ipc_zero_copy_send
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_freebsd_sendfile_nbytes_bug
name|ngx_uint_t
name|ngx_freebsd_sendfile_nbytes_bug
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_freebsd_use_tcp_nopush
name|ngx_uint_t
name|ngx_freebsd_use_tcp_nopush
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_freebsd_io
specifier|static
name|ngx_os_io_t
name|ngx_freebsd_io
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
name|ngx_freebsd_sendfile_chain
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

begin_typedef
DECL|struct|__anon29b5e4d00108
typedef|typedef
struct|struct
block|{
DECL|member|name
name|char
modifier|*
name|name
decl_stmt|;
DECL|member|value
name|int
modifier|*
name|value
decl_stmt|;
DECL|member|size
name|size_t
name|size
decl_stmt|;
DECL|member|exists
name|ngx_uint_t
name|exists
decl_stmt|;
DECL|typedef|sysctl_t
block|}
name|sysctl_t
typedef|;
end_typedef

begin_decl_stmt
DECL|variable|sysctls
name|sysctl_t
name|sysctls
index|[]
init|=
block|{
block|{
literal|"hw.ncpu"
block|,
operator|&
name|ngx_freebsd_hw_ncpu
block|,
sizeof|sizeof
argument_list|(
name|int
argument_list|)
block|,
literal|0
block|}
block|,
block|{
literal|"machdep.hlt_logical_cpus"
block|,
operator|&
name|ngx_freebsd_machdep_hlt_logical_cpus
block|,
sizeof|sizeof
argument_list|(
name|int
argument_list|)
block|,
literal|0
block|}
block|,
block|{
literal|"net.inet.tcp.sendspace"
block|,
operator|&
name|ngx_freebsd_net_inet_tcp_sendspace
block|,
sizeof|sizeof
argument_list|(
name|int
argument_list|)
block|,
literal|0
block|}
block|,
block|{
literal|"kern.ipc.somaxconn"
block|,
operator|&
name|ngx_freebsd_kern_ipc_somaxconn
block|,
sizeof|sizeof
argument_list|(
name|int
argument_list|)
block|,
literal|0
block|}
block|,
block|{
literal|"kern.ipc.zero_copy.send"
block|,
operator|&
name|ngx_freebsd_kern_ipc_zero_copy_send
block|,
sizeof|sizeof
argument_list|(
name|int
argument_list|)
block|,
literal|0
block|}
block|,
block|{
name|NULL
block|,
name|NULL
block|,
literal|0
block|,
literal|0
block|}
block|}
decl_stmt|;
end_decl_stmt

begin_function
name|void
DECL|function|ngx_debug_init ()
name|ngx_debug_init
parameter_list|()
block|{
if|#
directive|if
operator|(
name|NGX_DEBUG
operator|&&
operator|!
name|NGX_NO_DEBUG_MALLOC
operator|)
if|#
directive|if
name|__FreeBSD_version
operator|>=
literal|500014
name|_malloc_options
operator|=
literal|"J"
expr_stmt|;
else|#
directive|else
name|malloc_options
operator|=
literal|"J"
expr_stmt|;
endif|#
directive|endif
endif|#
directive|endif
block|}
end_function

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
name|int
name|version
decl_stmt|,
name|somaxconn
decl_stmt|;
name|size_t
name|size
decl_stmt|;
name|ngx_err_t
name|err
decl_stmt|;
name|ngx_uint_t
name|i
decl_stmt|;
name|size
operator|=
sizeof|sizeof
argument_list|(
name|ngx_freebsd_kern_ostype
argument_list|)
expr_stmt|;
if|if
condition|(
name|sysctlbyname
argument_list|(
literal|"kern.ostype"
argument_list|,
name|ngx_freebsd_kern_ostype
argument_list|,
operator|&
name|size
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
name|ngx_errno
argument_list|,
literal|"sysctlbyname(kern.ostype) failed"
argument_list|)
expr_stmt|;
if|if
condition|(
name|ngx_errno
operator|!=
name|NGX_ENOMEM
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|ngx_freebsd_kern_ostype
index|[
name|size
operator|-
literal|1
index|]
operator|=
literal|'\0'
expr_stmt|;
block|}
name|size
operator|=
sizeof|sizeof
argument_list|(
name|ngx_freebsd_kern_osrelease
argument_list|)
expr_stmt|;
if|if
condition|(
name|sysctlbyname
argument_list|(
literal|"kern.osrelease"
argument_list|,
name|ngx_freebsd_kern_osrelease
argument_list|,
operator|&
name|size
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
name|ngx_errno
argument_list|,
literal|"sysctlbyname(kern.osrelease) failed"
argument_list|)
expr_stmt|;
if|if
condition|(
name|ngx_errno
operator|!=
name|NGX_ENOMEM
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|ngx_freebsd_kern_osrelease
index|[
name|size
operator|-
literal|1
index|]
operator|=
literal|'\0'
expr_stmt|;
block|}
name|size
operator|=
sizeof|sizeof
argument_list|(
name|int
argument_list|)
expr_stmt|;
if|if
condition|(
name|sysctlbyname
argument_list|(
literal|"kern.osreldate"
argument_list|,
operator|&
name|ngx_freebsd_kern_osreldate
argument_list|,
operator|&
name|size
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
name|ngx_errno
argument_list|,
literal|"sysctlbyname(kern.osreldate) failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
name|version
operator|=
name|ngx_freebsd_kern_osreldate
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_HAVE_SENDFILE
operator|)
comment|/*      * The determination of the sendfile() "nbytes bug" is complex enough.      * There are two sendfile() syscalls: a new #393 has no bug while      * an old #336 has the bug in some versions and has not in others.      * Besides libc_r wrapper also emulates the bug in some versions.      * There is no way to say exactly if syscall #336 in FreeBSD circa 4.6      * has the bug.  We use the algorithm that is correct at least for      * RELEASEs and for syscalls only (not libc_r wrapper).      *      * 4.6.1-RELEASE and below have the bug      * 4.6.2-RELEASE and above have the new syscall      *      * We detect the new sendfile() syscall available at the compile time      * to allow an old binary to run correctly on an updated FreeBSD system.      */
if|#
directive|if
operator|(
name|__FreeBSD__
operator|==
literal|4
operator|&&
name|__FreeBSD_version
operator|>=
literal|460102
operator|)
expr|\
operator|||
name|__FreeBSD_version
operator|==
literal|460002
operator|||
name|__FreeBSD_version
operator|>=
literal|500039
comment|/* a new syscall without the bug */
name|ngx_freebsd_sendfile_nbytes_bug
operator|=
literal|0
expr_stmt|;
else|#
directive|else
comment|/* an old syscall that may have the bug */
name|ngx_freebsd_sendfile_nbytes_bug
operator|=
literal|1
expr_stmt|;
endif|#
directive|endif
endif|#
directive|endif
comment|/* NGX_HAVE_SENDFILE */
if|if
condition|(
operator|(
name|version
operator|<
literal|500000
operator|&&
name|version
operator|>=
literal|440003
operator|)
operator|||
name|version
operator|>=
literal|500017
condition|)
block|{
name|ngx_freebsd_use_tcp_nopush
operator|=
literal|1
expr_stmt|;
block|}
for|for
control|(
name|i
operator|=
literal|0
init|;
name|sysctls
index|[
name|i
index|]
operator|.
name|name
condition|;
name|i
operator|++
control|)
block|{
operator|*
name|sysctls
index|[
name|i
index|]
operator|.
name|value
operator|=
literal|0
expr_stmt|;
name|size
operator|=
name|sysctls
index|[
name|i
index|]
operator|.
name|size
expr_stmt|;
if|if
condition|(
name|sysctlbyname
argument_list|(
name|sysctls
index|[
name|i
index|]
operator|.
name|name
argument_list|,
name|sysctls
index|[
name|i
index|]
operator|.
name|value
argument_list|,
operator|&
name|size
argument_list|,
name|NULL
argument_list|,
literal|0
argument_list|)
operator|==
literal|0
condition|)
block|{
name|sysctls
index|[
name|i
index|]
operator|.
name|exists
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
name|err
operator|=
name|ngx_errno
expr_stmt|;
if|if
condition|(
name|err
operator|==
name|NGX_ENOENT
condition|)
block|{
continue|continue;
block|}
if|#
directive|if
literal|0
block_content|if (sysctls[i].value ==&ngx_freebsd_machdep_hlt_logical_cpus) {             continue;         }
endif|#
directive|endif
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|log
argument_list|,
name|err
argument_list|,
literal|"sysctlbyname(%s) failed"
argument_list|,
name|sysctls
index|[
name|i
index|]
operator|.
name|name
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|ngx_freebsd_machdep_hlt_logical_cpus
condition|)
block|{
name|ngx_ncpu
operator|=
name|ngx_freebsd_hw_ncpu
operator|/
literal|2
expr_stmt|;
block|}
else|else
block|{
name|ngx_ncpu
operator|=
name|ngx_freebsd_hw_ncpu
expr_stmt|;
block|}
if|if
condition|(
name|version
operator|<
literal|600008
condition|)
block|{
name|somaxconn
operator|=
literal|32767
expr_stmt|;
block|}
else|else
block|{
name|somaxconn
operator|=
literal|65535
expr_stmt|;
block|}
if|if
condition|(
name|ngx_freebsd_kern_ipc_somaxconn
operator|>
name|somaxconn
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"sysctl kern.ipc.somaxconn must be no more than %d"
argument_list|,
name|somaxconn
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
name|ngx_tcp_nodelay_and_tcp_nopush
operator|=
literal|1
expr_stmt|;
name|ngx_os_io
operator|=
name|ngx_freebsd_io
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
name|ngx_uint_t
name|i
decl_stmt|;
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
name|ngx_freebsd_kern_ostype
argument_list|,
name|ngx_freebsd_kern_osrelease
argument_list|)
expr_stmt|;
ifdef|#
directive|ifdef
name|__DragonFly_version
name|ngx_log_error
argument_list|(
name|NGX_LOG_NOTICE
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"kern.osreldate: %d, built on %d"
argument_list|,
name|ngx_freebsd_kern_osreldate
argument_list|,
name|__DragonFly_version
argument_list|)
expr_stmt|;
else|#
directive|else
name|ngx_log_error
argument_list|(
name|NGX_LOG_NOTICE
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"kern.osreldate: %d, built on %d"
argument_list|,
name|ngx_freebsd_kern_osreldate
argument_list|,
name|__FreeBSD_version
argument_list|)
expr_stmt|;
endif|#
directive|endif
for|for
control|(
name|i
operator|=
literal|0
init|;
name|sysctls
index|[
name|i
index|]
operator|.
name|name
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|sysctls
index|[
name|i
index|]
operator|.
name|exists
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_NOTICE
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"%s: %d"
argument_list|,
name|sysctls
index|[
name|i
index|]
operator|.
name|name
argument_list|,
operator|*
name|sysctls
index|[
name|i
index|]
operator|.
name|value
argument_list|)
expr_stmt|;
block|}
block|}
block|}
end_function

end_unit

