begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (C) 2002-2004 Igor Sysoev, http://sysoev.ru/en/  */
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

begin_comment
comment|/*  * Although FreeBSD sendfile() allows to pass a header and a trailer  * it can not send a header with a part of the file in one packet until  * FreeBSD 5.3.  Besides over the fast ethernet connection sendfile()  * may send the partially filled packets, i.e. the 8 file pages may be sent  * as the 11 full 1460-bytes packets, then one incomplete 324-bytes packet,  * and then again the 11 full 1460-bytes packets.  *  * So we use the TCP_NOPUSH option (similar to Linux's TCP_CORK)  * to postpone the sending - it not only sends a header and the first part  * of the file in one packet but also sends the file pages in the full packets.  *  * But until FreeBSD 4.5 the turning TCP_NOPUSH off does not flush a pending  * data that less than MSS so that data may be sent with 5 second delay.  * So we do not use TCP_NOPUSH on FreeBSD prior to 4.5 although it can be used  * for non-keepalive HTTP connections.  */
end_comment

begin_define
DECL|macro|NGX_HEADERS
define|#
directive|define
name|NGX_HEADERS
value|8
end_define

begin_define
DECL|macro|NGX_TRAILERS
define|#
directive|define
name|NGX_TRAILERS
value|4
end_define

begin_function
DECL|function|ngx_freebsd_sendfile_chain (ngx_connection_t * c,ngx_chain_t * in,off_t limit)
name|ngx_chain_t
modifier|*
name|ngx_freebsd_sendfile_chain
parameter_list|(
name|ngx_connection_t
modifier|*
name|c
parameter_list|,
name|ngx_chain_t
modifier|*
name|in
parameter_list|,
name|off_t
name|limit
parameter_list|)
block|{
name|int
name|rc
decl_stmt|;
name|u_char
modifier|*
name|prev
decl_stmt|;
name|off_t
name|fprev
decl_stmt|,
name|sent
decl_stmt|,
name|send
decl_stmt|,
name|sprev
decl_stmt|,
name|aligned
decl_stmt|;
name|size_t
name|hsize
decl_stmt|,
name|fsize
decl_stmt|;
name|ssize_t
name|size
decl_stmt|;
name|ngx_uint_t
name|eintr
decl_stmt|,
name|eagain
decl_stmt|,
name|complete
decl_stmt|;
name|ngx_err_t
name|err
decl_stmt|;
name|ngx_buf_t
modifier|*
name|file
decl_stmt|;
name|ngx_array_t
name|header
decl_stmt|,
name|trailer
decl_stmt|;
name|ngx_event_t
modifier|*
name|wev
decl_stmt|;
name|ngx_chain_t
modifier|*
name|cl
decl_stmt|;
name|struct
name|sf_hdtr
name|hdtr
decl_stmt|;
name|struct
name|iovec
modifier|*
name|iov
decl_stmt|,
name|headers
index|[
name|NGX_HEADERS
index|]
decl_stmt|,
name|trailers
index|[
name|NGX_TRAILERS
index|]
decl_stmt|;
name|wev
operator|=
name|c
operator|->
name|write
expr_stmt|;
if|if
condition|(
operator|!
name|wev
operator|->
name|ready
condition|)
block|{
return|return
name|in
return|;
block|}
if|#
directive|if
operator|(
name|HAVE_KQUEUE
operator|)
if|if
condition|(
operator|(
name|ngx_event_flags
operator|&
name|NGX_HAVE_KQUEUE_EVENT
operator|)
operator|&&
name|wev
operator|->
name|pending_eof
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_INFO
argument_list|,
name|c
operator|->
name|log
argument_list|,
name|wev
operator|->
name|kq_errno
argument_list|,
literal|"kevent() reported about an closed connection"
argument_list|)
expr_stmt|;
name|wev
operator|->
name|error
operator|=
literal|1
expr_stmt|;
return|return
name|NGX_CHAIN_ERROR
return|;
block|}
endif|#
directive|endif
name|send
operator|=
literal|0
expr_stmt|;
name|eagain
operator|=
literal|0
expr_stmt|;
name|header
operator|.
name|elts
operator|=
name|headers
expr_stmt|;
name|header
operator|.
name|size
operator|=
sizeof|sizeof
argument_list|(
expr|struct
name|iovec
argument_list|)
expr_stmt|;
name|header
operator|.
name|nalloc
operator|=
name|NGX_HEADERS
expr_stmt|;
name|header
operator|.
name|pool
operator|=
name|c
operator|->
name|pool
expr_stmt|;
name|trailer
operator|.
name|elts
operator|=
name|trailers
expr_stmt|;
name|trailer
operator|.
name|size
operator|=
sizeof|sizeof
argument_list|(
expr|struct
name|iovec
argument_list|)
expr_stmt|;
name|trailer
operator|.
name|nalloc
operator|=
name|NGX_TRAILERS
expr_stmt|;
name|trailer
operator|.
name|pool
operator|=
name|c
operator|->
name|pool
expr_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
name|file
operator|=
name|NULL
expr_stmt|;
name|fsize
operator|=
literal|0
expr_stmt|;
name|hsize
operator|=
literal|0
expr_stmt|;
name|eintr
operator|=
literal|0
expr_stmt|;
name|complete
operator|=
literal|0
expr_stmt|;
name|sprev
operator|=
name|send
expr_stmt|;
name|header
operator|.
name|nelts
operator|=
literal|0
expr_stmt|;
name|trailer
operator|.
name|nelts
operator|=
literal|0
expr_stmt|;
comment|/* create the header iovec and coalesce the neighbouring bufs */
name|prev
operator|=
name|NULL
expr_stmt|;
name|iov
operator|=
name|NULL
expr_stmt|;
for|for
control|(
name|cl
operator|=
name|in
init|;
name|cl
operator|&&
name|header
operator|.
name|nelts
operator|<
name|IOV_MAX
operator|&&
name|send
operator|<
name|limit
condition|;
name|cl
operator|=
name|cl
operator|->
name|next
control|)
block|{
if|if
condition|(
name|ngx_buf_special
argument_list|(
name|cl
operator|->
name|buf
argument_list|)
condition|)
block|{
continue|continue;
block|}
if|if
condition|(
operator|!
name|ngx_buf_in_memory_only
argument_list|(
name|cl
operator|->
name|buf
argument_list|)
condition|)
block|{
break|break;
block|}
name|size
operator|=
name|cl
operator|->
name|buf
operator|->
name|last
operator|-
name|cl
operator|->
name|buf
operator|->
name|pos
expr_stmt|;
if|if
condition|(
name|send
operator|+
name|size
operator|>
name|limit
condition|)
block|{
name|size
operator|=
name|limit
operator|-
name|send
expr_stmt|;
block|}
if|if
condition|(
name|prev
operator|==
name|cl
operator|->
name|buf
operator|->
name|pos
condition|)
block|{
name|iov
operator|->
name|iov_len
operator|+=
name|size
expr_stmt|;
block|}
else|else
block|{
if|if
condition|(
operator|!
operator|(
name|iov
operator|=
name|ngx_array_push
argument_list|(
operator|&
name|header
argument_list|)
operator|)
condition|)
block|{
return|return
name|NGX_CHAIN_ERROR
return|;
block|}
name|iov
operator|->
name|iov_base
operator|=
operator|(
name|void
operator|*
operator|)
name|cl
operator|->
name|buf
operator|->
name|pos
expr_stmt|;
name|iov
operator|->
name|iov_len
operator|=
name|size
expr_stmt|;
block|}
name|prev
operator|=
name|cl
operator|->
name|buf
operator|->
name|pos
operator|+
name|size
expr_stmt|;
name|hsize
operator|+=
name|size
expr_stmt|;
name|send
operator|+=
name|size
expr_stmt|;
block|}
comment|/* get the file buf */
if|if
condition|(
name|cl
operator|&&
name|cl
operator|->
name|buf
operator|->
name|in_file
operator|&&
name|send
operator|<
name|limit
condition|)
block|{
name|file
operator|=
name|cl
operator|->
name|buf
expr_stmt|;
comment|/* coalesce the neighbouring file bufs */
do|do
block|{
name|size
operator|=
operator|(
name|size_t
operator|)
operator|(
name|cl
operator|->
name|buf
operator|->
name|file_last
operator|-
name|cl
operator|->
name|buf
operator|->
name|file_pos
operator|)
expr_stmt|;
if|if
condition|(
name|send
operator|+
name|size
operator|>
name|limit
condition|)
block|{
name|size
operator|=
name|limit
operator|-
name|send
expr_stmt|;
name|aligned
operator|=
operator|(
name|cl
operator|->
name|buf
operator|->
name|file_pos
operator|+
name|size
operator|+
name|ngx_pagesize
operator|-
literal|1
operator|)
operator|&
operator|~
operator|(
name|ngx_pagesize
operator|-
literal|1
operator|)
expr_stmt|;
if|if
condition|(
name|aligned
operator|<=
name|cl
operator|->
name|buf
operator|->
name|file_last
condition|)
block|{
name|size
operator|=
name|aligned
operator|-
name|cl
operator|->
name|buf
operator|->
name|file_pos
expr_stmt|;
block|}
block|}
name|fsize
operator|+=
name|size
expr_stmt|;
name|send
operator|+=
name|size
expr_stmt|;
name|fprev
operator|=
name|cl
operator|->
name|buf
operator|->
name|file_pos
operator|+
name|size
expr_stmt|;
name|cl
operator|=
name|cl
operator|->
name|next
expr_stmt|;
block|}
do|while
condition|(
name|cl
operator|&&
name|cl
operator|->
name|buf
operator|->
name|in_file
operator|&&
name|send
operator|<
name|limit
operator|&&
name|file
operator|->
name|file
operator|->
name|fd
operator|==
name|cl
operator|->
name|buf
operator|->
name|file
operator|->
name|fd
operator|&&
name|fprev
operator|==
name|cl
operator|->
name|buf
operator|->
name|file_pos
condition|)
do|;
block|}
if|if
condition|(
name|file
condition|)
block|{
comment|/* create the tailer iovec and coalesce the neighbouring bufs */
name|prev
operator|=
name|NULL
expr_stmt|;
name|iov
operator|=
name|NULL
expr_stmt|;
for|for
control|(
comment|/* void */
init|;
name|cl
operator|&&
name|header
operator|.
name|nelts
operator|<
name|IOV_MAX
operator|&&
name|send
operator|<
name|limit
condition|;
name|cl
operator|=
name|cl
operator|->
name|next
control|)
block|{
if|if
condition|(
name|ngx_buf_special
argument_list|(
name|cl
operator|->
name|buf
argument_list|)
condition|)
block|{
continue|continue;
block|}
if|if
condition|(
operator|!
name|ngx_buf_in_memory_only
argument_list|(
name|cl
operator|->
name|buf
argument_list|)
condition|)
block|{
break|break;
block|}
name|size
operator|=
name|cl
operator|->
name|buf
operator|->
name|last
operator|-
name|cl
operator|->
name|buf
operator|->
name|pos
expr_stmt|;
if|if
condition|(
name|send
operator|+
name|size
operator|>
name|limit
condition|)
block|{
name|size
operator|=
name|limit
operator|-
name|send
expr_stmt|;
block|}
if|if
condition|(
name|prev
operator|==
name|cl
operator|->
name|buf
operator|->
name|pos
condition|)
block|{
name|iov
operator|->
name|iov_len
operator|+=
name|size
expr_stmt|;
block|}
else|else
block|{
if|if
condition|(
operator|!
operator|(
name|iov
operator|=
name|ngx_array_push
argument_list|(
operator|&
name|trailer
argument_list|)
operator|)
condition|)
block|{
return|return
name|NGX_CHAIN_ERROR
return|;
block|}
name|iov
operator|->
name|iov_base
operator|=
operator|(
name|void
operator|*
operator|)
name|cl
operator|->
name|buf
operator|->
name|pos
expr_stmt|;
name|iov
operator|->
name|iov_len
operator|=
name|size
expr_stmt|;
block|}
name|prev
operator|=
name|cl
operator|->
name|buf
operator|->
name|pos
operator|+
name|size
expr_stmt|;
name|send
operator|+=
name|size
expr_stmt|;
block|}
block|}
if|if
condition|(
name|file
condition|)
block|{
if|if
condition|(
name|ngx_freebsd_use_tcp_nopush
operator|&&
name|c
operator|->
name|tcp_nopush
operator|==
name|NGX_TCP_NOPUSH_UNSET
condition|)
block|{
if|if
condition|(
name|ngx_tcp_nopush
argument_list|(
name|c
operator|->
name|fd
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
name|err
operator|=
name|ngx_errno
expr_stmt|;
comment|/*                      * there is a tiny chance to be interrupted, however                      * we continue a processing without the TCP_NOPUSH                      */
if|if
condition|(
name|err
operator|!=
name|NGX_EINTR
condition|)
block|{
name|wev
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
name|ngx_tcp_nopush_n
literal|" failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_CHAIN_ERROR
return|;
block|}
block|}
else|else
block|{
name|c
operator|->
name|tcp_nopush
operator|=
name|NGX_TCP_NOPUSH_SET
expr_stmt|;
name|ngx_log_debug0
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|c
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"tcp_nopush"
argument_list|)
expr_stmt|;
block|}
block|}
name|hdtr
operator|.
name|headers
operator|=
operator|(
expr|struct
name|iovec
operator|*
operator|)
name|header
operator|.
name|elts
expr_stmt|;
name|hdtr
operator|.
name|hdr_cnt
operator|=
name|header
operator|.
name|nelts
expr_stmt|;
name|hdtr
operator|.
name|trailers
operator|=
operator|(
expr|struct
name|iovec
operator|*
operator|)
name|trailer
operator|.
name|elts
expr_stmt|;
name|hdtr
operator|.
name|trl_cnt
operator|=
name|trailer
operator|.
name|nelts
expr_stmt|;
comment|/*              * the "nbytes bug" of the old sendfile() syscall:              * http://www.freebsd.org/cgi/query-pr.cgi?pr=33771              */
if|if
condition|(
name|ngx_freebsd_sendfile_nbytes_bug
operator|==
literal|0
condition|)
block|{
name|hsize
operator|=
literal|0
expr_stmt|;
block|}
name|sent
operator|=
literal|0
expr_stmt|;
name|rc
operator|=
name|sendfile
argument_list|(
name|file
operator|->
name|file
operator|->
name|fd
argument_list|,
name|c
operator|->
name|fd
argument_list|,
name|file
operator|->
name|file_pos
argument_list|,
name|fsize
operator|+
name|hsize
argument_list|,
operator|&
name|hdtr
argument_list|,
operator|&
name|sent
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|rc
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
operator|==
name|NGX_EAGAIN
operator|||
name|err
operator|==
name|NGX_EINTR
condition|)
block|{
if|if
condition|(
name|err
operator|==
name|NGX_EINTR
condition|)
block|{
name|eintr
operator|=
literal|1
expr_stmt|;
block|}
else|else
block|{
name|eagain
operator|=
literal|1
expr_stmt|;
block|}
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|c
operator|->
name|log
argument_list|,
name|err
argument_list|,
literal|"sendfile() sent only "
name|OFF_T_FMT
literal|" bytes"
argument_list|,
name|sent
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|wev
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
literal|"sendfile() failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_CHAIN_ERROR
return|;
block|}
block|}
name|ngx_log_debug4
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|c
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"sendfile: %d, @"
name|OFF_T_FMT
literal|" "
name|OFF_T_FMT
literal|":%d"
argument_list|,
name|rc
argument_list|,
name|file
operator|->
name|file_pos
argument_list|,
name|sent
argument_list|,
name|fsize
operator|+
name|hsize
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|rc
operator|=
name|writev
argument_list|(
name|c
operator|->
name|fd
argument_list|,
name|header
operator|.
name|elts
argument_list|,
name|header
operator|.
name|nelts
argument_list|)
expr_stmt|;
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
literal|"writev: %d of "
name|SIZE_T_FMT
argument_list|,
name|rc
argument_list|,
name|hsize
argument_list|)
expr_stmt|;
if|if
condition|(
name|rc
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
operator|==
name|NGX_EAGAIN
operator|||
name|err
operator|==
name|NGX_EINTR
condition|)
block|{
if|if
condition|(
name|err
operator|==
name|NGX_EINTR
condition|)
block|{
name|eintr
operator|=
literal|1
expr_stmt|;
block|}
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
literal|"writev() not ready"
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|wev
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
literal|"writev() failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_CHAIN_ERROR
return|;
block|}
block|}
name|sent
operator|=
name|rc
operator|>
literal|0
condition|?
name|rc
else|:
literal|0
expr_stmt|;
block|}
if|if
condition|(
name|send
operator|-
name|sprev
operator|==
name|sent
condition|)
block|{
name|complete
operator|=
literal|1
expr_stmt|;
block|}
name|c
operator|->
name|sent
operator|+=
name|sent
expr_stmt|;
for|for
control|(
name|cl
operator|=
name|in
init|;
name|cl
condition|;
name|cl
operator|=
name|cl
operator|->
name|next
control|)
block|{
if|if
condition|(
name|ngx_buf_special
argument_list|(
name|cl
operator|->
name|buf
argument_list|)
condition|)
block|{
continue|continue;
block|}
if|if
condition|(
name|sent
operator|==
literal|0
condition|)
block|{
break|break;
block|}
name|size
operator|=
name|ngx_buf_size
argument_list|(
name|cl
operator|->
name|buf
argument_list|)
expr_stmt|;
if|if
condition|(
name|sent
operator|>=
name|size
condition|)
block|{
name|sent
operator|-=
name|size
expr_stmt|;
if|if
condition|(
name|ngx_buf_in_memory
argument_list|(
name|cl
operator|->
name|buf
argument_list|)
condition|)
block|{
name|cl
operator|->
name|buf
operator|->
name|pos
operator|=
name|cl
operator|->
name|buf
operator|->
name|last
expr_stmt|;
block|}
if|if
condition|(
name|cl
operator|->
name|buf
operator|->
name|in_file
condition|)
block|{
name|cl
operator|->
name|buf
operator|->
name|file_pos
operator|=
name|cl
operator|->
name|buf
operator|->
name|file_last
expr_stmt|;
block|}
continue|continue;
block|}
if|if
condition|(
name|ngx_buf_in_memory
argument_list|(
name|cl
operator|->
name|buf
argument_list|)
condition|)
block|{
name|cl
operator|->
name|buf
operator|->
name|pos
operator|+=
name|sent
expr_stmt|;
block|}
if|if
condition|(
name|cl
operator|->
name|buf
operator|->
name|in_file
condition|)
block|{
name|cl
operator|->
name|buf
operator|->
name|file_pos
operator|+=
name|sent
expr_stmt|;
block|}
break|break;
block|}
if|if
condition|(
name|eagain
condition|)
block|{
comment|/*              * sendfile() can return EAGAIN even if it has sent              * a whole file part but the successive sendfile() call would              * return EAGAIN right away and would not send anything.              * We use it as a hint.              */
name|wev
operator|->
name|ready
operator|=
literal|0
expr_stmt|;
return|return
name|cl
return|;
block|}
if|if
condition|(
name|eintr
condition|)
block|{
continue|continue;
block|}
if|if
condition|(
operator|!
name|complete
condition|)
block|{
name|wev
operator|->
name|ready
operator|=
literal|0
expr_stmt|;
return|return
name|cl
return|;
block|}
if|if
condition|(
name|send
operator|>=
name|limit
operator|||
name|cl
operator|==
name|NULL
condition|)
block|{
return|return
name|cl
return|;
block|}
name|in
operator|=
name|cl
expr_stmt|;
block|}
block|}
end_function

end_unit

