begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|<ngx_freebsd_init.h>
end_include

begin_comment
comment|/* FreeBSD 3.4 at least */
end_comment

begin_decl_stmt
DECL|variable|ngx_freebsd_kern_ostype
name|char
name|ngx_freebsd_kern_ostype
index|[
literal|20
index|]
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_freebsd_kern_osrelease
name|char
name|ngx_freebsd_kern_osrelease
index|[
literal|20
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
DECL|variable|ngx_freebsd_sendfile_nbytes_bug
name|int
name|ngx_freebsd_sendfile_nbytes_bug
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_freebsd_use_tcp_nopush
name|int
name|ngx_freebsd_use_tcp_nopush
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
if|#
directive|if
operator|(
name|HAVE_FREEBSD_SENDFILE
operator|)
name|ngx_freebsd_sendfile_chain
block|,
name|NGX_HAVE_SENDFILE
operator||
name|NGX_HAVE_ZEROCOPY
else|#
directive|else
name|ngx_writev_chain
block|,
name|NULL
endif|#
directive|endif
block|}
decl_stmt|;
end_decl_stmt

begin_typedef
DECL|struct|__anon2b3937f70108
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
expr|sizeof
operator|(
name|int
operator|)
block|}
block|,
block|{
literal|"net.inet.tcp.sendspace"
block|,
operator|&
name|ngx_freebsd_net_inet_tcp_sendspace
block|,
expr|sizeof
operator|(
name|int
operator|)
block|}
block|,
comment|/* FreeBSD 5.0 */
block|{
literal|"kern.ipc.zero_copy.send"
block|,
operator|&
name|ngx_freebsd_kern_ipc_zero_copy_send
block|,
expr|sizeof
operator|(
name|int
operator|)
block|}
block|,
block|{
name|NULL
block|,
name|NULL
block|,
literal|0
block|}
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
name|i
decl_stmt|,
name|version
decl_stmt|;
name|size_t
name|size
decl_stmt|;
name|ngx_err_t
name|err
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
name|errno
argument_list|,
literal|"sysctlbyname(kern.ostype) failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
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
name|errno
argument_list|,
literal|"sysctlbyname(kern.osrelease) failed"
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
name|ngx_freebsd_kern_ostype
argument_list|,
name|ngx_freebsd_kern_osrelease
argument_list|)
expr_stmt|;
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
name|errno
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
name|ngx_log_error
argument_list|(
name|NGX_LOG_INFO
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"kern.osreldate: %d, built on %d"
argument_list|,
name|version
argument_list|,
name|__FreeBSD_version
argument_list|)
expr_stmt|;
if|#
directive|if
operator|(
name|HAVE_FREEBSD_SENDFILE
operator|)
comment|/*      * The determination of the sendfile() nbytes bug is complex enough.      * There are two sendfile() syscalls: a new 393 has no bug while      * an old 336 has the bug in some versions and has not in others.      * Besides libc_r wrapper also emulates the bug in some versions.      * There's no way to say exactly if a given FreeBSD version has the bug.      * Here is the algorithm that works at least for RELEASEs      * and for syscalls only (not libc_r wrapper).      *      * We detect the new sendfile() version available at the compile time      * to allow an old binary to run correctly on an updated FreeBSD system.      */
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
comment|/* an old syscall that can have the bug */
name|ngx_freebsd_sendfile_nbytes_bug
operator|=
literal|1
expr_stmt|;
endif|#
directive|endif
endif|#
directive|endif
comment|/* HAVE_FREEBSD_SENDFILE */
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
operator|-
literal|1
condition|)
block|{
name|err
operator|=
name|errno
expr_stmt|;
if|if
condition|(
name|err
operator|!=
name|NGX_ENOENT
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
block|}
else|else
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_INFO
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
return|return
name|ngx_posix_init
argument_list|(
name|log
argument_list|)
return|;
block|}
end_function

end_unit

