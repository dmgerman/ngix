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
file|<ngx_event.h>
end_include

begin_define
DECL|macro|NGX_WSABUFS
define|#
directive|define
name|NGX_WSABUFS
value|8
end_define

begin_function
name|ssize_t
DECL|function|ngx_wsarecv_chain (ngx_connection_t * c,ngx_chain_t * chain)
name|ngx_wsarecv_chain
parameter_list|(
name|ngx_connection_t
modifier|*
name|c
parameter_list|,
name|ngx_chain_t
modifier|*
name|chain
parameter_list|)
block|{
name|int
name|rc
decl_stmt|;
name|u_char
modifier|*
name|prev
decl_stmt|;
name|u_long
name|bytes
decl_stmt|,
name|flags
decl_stmt|;
name|size_t
name|size
decl_stmt|;
name|ngx_err_t
name|err
decl_stmt|;
name|ngx_array_t
name|vec
decl_stmt|;
name|ngx_event_t
modifier|*
name|rev
decl_stmt|;
name|LPWSABUF
name|wsabuf
decl_stmt|;
name|WSABUF
name|wsabufs
index|[
name|NGX_WSABUFS
index|]
decl_stmt|;
name|prev
operator|=
name|NULL
expr_stmt|;
name|wsabuf
operator|=
name|NULL
expr_stmt|;
name|flags
operator|=
literal|0
expr_stmt|;
name|size
operator|=
literal|0
expr_stmt|;
name|bytes
operator|=
literal|0
expr_stmt|;
name|vec
operator|.
name|elts
operator|=
name|wsabufs
expr_stmt|;
name|vec
operator|.
name|nelts
operator|=
literal|0
expr_stmt|;
name|vec
operator|.
name|size
operator|=
sizeof|sizeof
argument_list|(
name|WSABUF
argument_list|)
expr_stmt|;
name|vec
operator|.
name|nalloc
operator|=
name|NGX_WSABUFS
expr_stmt|;
name|vec
operator|.
name|pool
operator|=
name|c
operator|->
name|pool
expr_stmt|;
comment|/* coalesce the neighbouring bufs */
while|while
condition|(
name|chain
condition|)
block|{
if|if
condition|(
name|prev
operator|==
name|chain
operator|->
name|buf
operator|->
name|last
condition|)
block|{
name|wsabuf
operator|->
name|len
operator|+=
name|chain
operator|->
name|buf
operator|->
name|end
operator|-
name|chain
operator|->
name|buf
operator|->
name|last
expr_stmt|;
block|}
else|else
block|{
name|wsabuf
operator|=
name|ngx_array_push
argument_list|(
operator|&
name|vec
argument_list|)
expr_stmt|;
if|if
condition|(
name|wsabuf
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|wsabuf
operator|->
name|buf
operator|=
operator|(
name|char
operator|*
operator|)
name|chain
operator|->
name|buf
operator|->
name|last
expr_stmt|;
name|wsabuf
operator|->
name|len
operator|=
name|chain
operator|->
name|buf
operator|->
name|end
operator|-
name|chain
operator|->
name|buf
operator|->
name|last
expr_stmt|;
block|}
name|size
operator|+=
name|chain
operator|->
name|buf
operator|->
name|end
operator|-
name|chain
operator|->
name|buf
operator|->
name|last
expr_stmt|;
name|prev
operator|=
name|chain
operator|->
name|buf
operator|->
name|end
expr_stmt|;
name|chain
operator|=
name|chain
operator|->
name|next
expr_stmt|;
block|}
name|ngx_log_debug2
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|c
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"WSARecv: %d:%d"
argument_list|,
name|vec
operator|.
name|nelts
argument_list|,
name|wsabuf
operator|->
name|len
argument_list|)
expr_stmt|;
name|rc
operator|=
name|WSARecv
argument_list|(
name|c
operator|->
name|fd
argument_list|,
name|vec
operator|.
name|elts
argument_list|,
name|vec
operator|.
name|nelts
argument_list|,
operator|&
name|bytes
argument_list|,
operator|&
name|flags
argument_list|,
name|NULL
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|rev
operator|=
name|c
operator|->
name|read
expr_stmt|;
if|if
condition|(
name|rc
operator|==
operator|-
literal|1
condition|)
block|{
name|rev
operator|->
name|ready
operator|=
literal|0
expr_stmt|;
name|err
operator|=
name|ngx_socket_errno
expr_stmt|;
if|if
condition|(
name|err
operator|==
name|WSAEWOULDBLOCK
condition|)
block|{
name|ngx_log_debug0
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|c
operator|->
name|log
argument_list|,
name|err
argument_list|,
literal|"WSARecv() not ready"
argument_list|)
expr_stmt|;
return|return
name|NGX_AGAIN
return|;
block|}
name|rev
operator|->
name|error
operator|=
literal|1
expr_stmt|;
name|ngx_connection_error
argument_list|(
name|c
argument_list|,
name|err
argument_list|,
literal|"WSARecv() failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|bytes
operator|<
name|size
condition|)
block|{
name|rev
operator|->
name|ready
operator|=
literal|0
expr_stmt|;
block|}
if|if
condition|(
name|bytes
operator|==
literal|0
condition|)
block|{
name|rev
operator|->
name|eof
operator|=
literal|1
expr_stmt|;
block|}
return|return
name|bytes
return|;
block|}
end_function

end_unit

