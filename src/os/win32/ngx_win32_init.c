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
DECL|variable|ngx_win32_version
name|int
name|ngx_win32_version
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_max_sockets
name|int
name|ngx_max_sockets
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_inherited_nonblocking
name|int
name|ngx_inherited_nonblocking
init|=
literal|1
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_os_io
name|ngx_os_io_t
name|ngx_os_io
init|=
block|{
name|ngx_wsarecv
block|,
name|ngx_wsarecv_chain
block|,
name|NULL
block|,
name|ngx_wsasend_chain
block|,
literal|0
block|}
decl_stmt|;
end_decl_stmt

begin_typedef
DECL|struct|__anon2bb5362e0108
typedef|typedef
struct|struct
block|{
DECL|member|wServicePackMinor
name|WORD
name|wServicePackMinor
decl_stmt|;
DECL|member|wSuiteMask
name|WORD
name|wSuiteMask
decl_stmt|;
DECL|member|wProductType
name|BYTE
name|wProductType
decl_stmt|;
DECL|typedef|ngx_osviex_stub_t
block|}
name|ngx_osviex_stub_t
typedef|;
end_typedef

begin_comment
comment|/* Should these pointers be per protocol ? */
end_comment

begin_decl_stmt
DECL|variable|acceptex
name|LPFN_ACCEPTEX
name|acceptex
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|getacceptexsockaddrs
name|LPFN_GETACCEPTEXSOCKADDRS
name|getacceptexsockaddrs
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|transmitfile
name|LPFN_TRANSMITFILE
name|transmitfile
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ae_guid
specifier|static
name|GUID
name|ae_guid
init|=
name|WSAID_ACCEPTEX
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|as_guid
specifier|static
name|GUID
name|as_guid
init|=
name|WSAID_GETACCEPTEXSOCKADDRS
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|tf_guid
specifier|static
name|GUID
name|tf_guid
init|=
name|WSAID_TRANSMITFILE
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
name|u_int
name|osviex
decl_stmt|;
name|DWORD
name|bytes
decl_stmt|;
name|SOCKET
name|s
decl_stmt|;
name|WSADATA
name|wsd
decl_stmt|;
name|OSVERSIONINFOEX
name|osvi
decl_stmt|;
name|ngx_osviex_stub_t
modifier|*
name|osviex_stub
decl_stmt|;
comment|/* get Windows version */
name|ngx_memzero
argument_list|(
operator|&
name|osvi
argument_list|,
sizeof|sizeof
argument_list|(
name|OSVERSIONINFOEX
argument_list|)
argument_list|)
expr_stmt|;
name|osvi
operator|.
name|dwOSVersionInfoSize
operator|=
sizeof|sizeof
argument_list|(
name|OSVERSIONINFOEX
argument_list|)
expr_stmt|;
name|osviex
operator|=
name|GetVersionEx
argument_list|(
operator|(
name|OSVERSIONINFO
operator|*
operator|)
operator|&
name|osvi
argument_list|)
expr_stmt|;
if|if
condition|(
name|osviex
operator|==
literal|0
condition|)
block|{
name|osvi
operator|.
name|dwOSVersionInfoSize
operator|=
sizeof|sizeof
argument_list|(
name|OSVERSIONINFOEX
argument_list|)
expr_stmt|;
if|if
condition|(
name|GetVersionEx
argument_list|(
operator|(
name|OSVERSIONINFO
operator|*
operator|)
operator|&
name|osvi
argument_list|)
operator|==
literal|0
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
literal|"GetVersionEx() failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
block|}
comment|/*      *  Windows 3.1 Win32s   0xxxxx      *      *  Windows 95           140000      *  Windows 98           141000      *  Windows ME           149000      *  Windows NT 3.51      235100      *  Windows NT 4.0       240000      *  Windows NT 4.0 SP5   240050      *  Windows 2000         250000      *  Windows XP           250100      *  Windows 2003         250200      *      *  Windows CE x.x       3xxxxx      */
name|ngx_win32_version
operator|=
name|osvi
operator|.
name|dwPlatformId
operator|*
literal|100000
operator|+
name|osvi
operator|.
name|dwMajorVersion
operator|*
literal|10000
operator|+
name|osvi
operator|.
name|dwMinorVersion
operator|*
literal|100
expr_stmt|;
if|if
condition|(
name|osviex
condition|)
block|{
name|ngx_win32_version
operator|+=
name|osvi
operator|.
name|wServicePackMajor
operator|*
literal|10
operator|+
name|osvi
operator|.
name|wServicePackMinor
expr_stmt|;
comment|/*          * the MSVC 6.0 SP2 defines wSuiteMask and wProductType          * as WORD wReserved[2]          */
name|osviex_stub
operator|=
operator|(
name|ngx_osviex_stub_t
operator|*
operator|)
operator|&
name|osvi
operator|.
name|wServicePackMinor
expr_stmt|;
name|ngx_log_error
argument_list|(
name|NGX_LOG_INFO
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"OS: %u build:%u, \"%s\", suite:%x, type:%u"
argument_list|,
name|ngx_win32_version
argument_list|,
name|osvi
operator|.
name|dwBuildNumber
argument_list|,
name|osvi
operator|.
name|szCSDVersion
argument_list|,
name|osviex_stub
operator|->
name|wSuiteMask
argument_list|,
name|osviex_stub
operator|->
name|wProductType
argument_list|)
expr_stmt|;
block|}
else|else
block|{
if|if
condition|(
name|osvi
operator|.
name|dwPlatformId
operator|==
name|VER_PLATFORM_WIN32_WINDOWS
condition|)
block|{
comment|/* Win9x build */
name|ngx_log_error
argument_list|(
name|NGX_LOG_INFO
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"OS: %u build:%u.%u.%u, \"%s\""
argument_list|,
name|ngx_win32_version
argument_list|,
name|osvi
operator|.
name|dwBuildNumber
operator|>>
literal|24
argument_list|,
operator|(
name|osvi
operator|.
name|dwBuildNumber
operator|>>
literal|16
operator|)
operator|&
literal|0xff
argument_list|,
name|osvi
operator|.
name|dwBuildNumber
operator|&
literal|0xffff
argument_list|,
name|osvi
operator|.
name|szCSDVersion
argument_list|)
expr_stmt|;
block|}
else|else
block|{
comment|/*              * VER_PLATFORM_WIN32_NT              *              * we do not currently support VER_PLATFORM_WIN32_CE              * and we do not support VER_PLATFORM_WIN32s at all              */
name|ngx_log_error
argument_list|(
name|NGX_LOG_INFO
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"OS: %u build:%u, \"%s\""
argument_list|,
name|ngx_win32_version
argument_list|,
name|osvi
operator|.
name|dwBuildNumber
argument_list|,
name|osvi
operator|.
name|szCSDVersion
argument_list|)
expr_stmt|;
block|}
block|}
comment|/* init Winsock */
if|if
condition|(
name|WSAStartup
argument_list|(
name|MAKEWORD
argument_list|(
literal|2
argument_list|,
literal|2
argument_list|)
argument_list|,
operator|&
name|wsd
argument_list|)
operator|!=
literal|0
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_EMERG
argument_list|,
name|log
argument_list|,
name|ngx_socket_errno
argument_list|,
literal|"WSAStartup() failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|ngx_win32_version
operator|<
name|NGX_WIN_NT
condition|)
block|{
return|return
name|NGX_OK
return|;
block|}
comment|/* get AcceptEx(), GetAcceptExSockAddrs() and TransmitFile() addresses */
name|s
operator|=
name|ngx_socket
argument_list|(
name|AF_INET
argument_list|,
name|SOCK_STREAM
argument_list|,
name|IPPROTO_IP
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|s
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
name|ngx_socket_errno
argument_list|,
name|ngx_socket_n
literal|" %s falied"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|WSAIoctl
argument_list|(
name|s
argument_list|,
name|SIO_GET_EXTENSION_FUNCTION_POINTER
argument_list|,
operator|&
name|ae_guid
argument_list|,
sizeof|sizeof
argument_list|(
name|GUID
argument_list|)
argument_list|,
operator|&
name|acceptex
argument_list|,
sizeof|sizeof
argument_list|(
name|LPFN_ACCEPTEX
argument_list|)
argument_list|,
operator|&
name|bytes
argument_list|,
name|NULL
argument_list|,
name|NULL
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
name|ngx_socket_errno
argument_list|,
literal|"WSAIoctl(SIO_GET_EXTENSION_FUNCTION_POINTER, "
literal|"WSAID_ACCEPTEX) failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|WSAIoctl
argument_list|(
name|s
argument_list|,
name|SIO_GET_EXTENSION_FUNCTION_POINTER
argument_list|,
operator|&
name|as_guid
argument_list|,
sizeof|sizeof
argument_list|(
name|GUID
argument_list|)
argument_list|,
operator|&
name|getacceptexsockaddrs
argument_list|,
sizeof|sizeof
argument_list|(
name|LPFN_GETACCEPTEXSOCKADDRS
argument_list|)
argument_list|,
operator|&
name|bytes
argument_list|,
name|NULL
argument_list|,
name|NULL
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
name|ngx_socket_errno
argument_list|,
literal|"WSAIoctl(SIO_GET_EXTENSION_FUNCTION_POINTER, "
literal|"WSAID_ACCEPTEX) failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|WSAIoctl
argument_list|(
name|s
argument_list|,
name|SIO_GET_EXTENSION_FUNCTION_POINTER
argument_list|,
operator|&
name|tf_guid
argument_list|,
sizeof|sizeof
argument_list|(
name|GUID
argument_list|)
argument_list|,
operator|&
name|transmitfile
argument_list|,
sizeof|sizeof
argument_list|(
name|LPFN_TRANSMITFILE
argument_list|)
argument_list|,
operator|&
name|bytes
argument_list|,
name|NULL
argument_list|,
name|NULL
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
name|ngx_socket_errno
argument_list|,
literal|"WSAIoctl(SIO_GET_EXTENSION_FUNCTION_POINTER, "
literal|"WSAID_TRANSMITFILE) failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|ngx_close_socket
argument_list|(
name|s
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
name|ngx_socket_errno
argument_list|,
name|ngx_close_socket_n
literal|" failed"
argument_list|)
expr_stmt|;
block|}
return|return
name|NGX_OK
return|;
block|}
end_function

end_unit

