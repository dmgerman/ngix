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
file|<ngx_http.h>
end_include

begin_if
if|#
directive|if
operator|(
name|HAVE_OPENSSL_MD5_H
operator|)
end_if

begin_include
include|#
directive|include
file|<openssl/md5.h>
end_include

begin_else
else|#
directive|else
end_else

begin_include
include|#
directive|include
file|<md5.h>
end_include

begin_endif
endif|#
directive|endif
end_endif

begin_if
if|#
directive|if
operator|(
name|HAVE_OPENSSL_MD5
operator|)
end_if

begin_define
DECL|macro|MD5Init
define|#
directive|define
name|MD5Init
value|MD5_Init
end_define

begin_define
DECL|macro|MD5Update
define|#
directive|define
name|MD5Update
value|MD5_Update
end_define

begin_define
DECL|macro|MD5Final
define|#
directive|define
name|MD5Final
value|MD5_Final
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_function
DECL|function|ngx_http_cache_get_file (ngx_http_request_t * r,ngx_http_cache_ctx_t * ctx)
name|int
name|ngx_http_cache_get_file
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_cache_ctx_t
modifier|*
name|ctx
parameter_list|)
block|{
name|MD5_CTX
name|md5
decl_stmt|;
comment|/* we use offsetof() because sizeof() pads struct size to int size */
name|ctx
operator|->
name|header_size
operator|=
name|offsetof
argument_list|(
name|ngx_http_cache_header_t
argument_list|,
name|key
argument_list|)
operator|+
name|ctx
operator|->
name|key
operator|.
name|len
operator|+
literal|1
expr_stmt|;
name|ctx
operator|->
name|file
operator|.
name|name
operator|.
name|len
operator|=
name|ctx
operator|->
name|path
operator|->
name|name
operator|.
name|len
operator|+
literal|1
operator|+
name|ctx
operator|->
name|path
operator|->
name|len
operator|+
literal|32
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|ctx
operator|->
name|file
operator|.
name|name
operator|.
name|data
operator|=
name|ngx_palloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
name|ctx
operator|->
name|file
operator|.
name|name
operator|.
name|len
operator|+
literal|1
argument_list|)
operator|)
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|ngx_memcpy
argument_list|(
name|ctx
operator|->
name|file
operator|.
name|name
operator|.
name|data
argument_list|,
name|ctx
operator|->
name|path
operator|->
name|name
operator|.
name|data
argument_list|,
name|ctx
operator|->
name|path
operator|->
name|name
operator|.
name|len
argument_list|)
expr_stmt|;
name|MD5Init
argument_list|(
operator|&
name|md5
argument_list|)
expr_stmt|;
name|MD5Update
argument_list|(
operator|&
name|md5
argument_list|,
operator|(
name|u_char
operator|*
operator|)
name|ctx
operator|->
name|key
operator|.
name|data
argument_list|,
name|ctx
operator|->
name|key
operator|.
name|len
argument_list|)
expr_stmt|;
name|MD5Final
argument_list|(
name|ctx
operator|->
name|md5
argument_list|,
operator|&
name|md5
argument_list|)
expr_stmt|;
name|ngx_md5_text
argument_list|(
name|ctx
operator|->
name|file
operator|.
name|name
operator|.
name|data
operator|+
name|ctx
operator|->
name|path
operator|->
name|name
operator|.
name|len
operator|+
literal|1
operator|+
name|ctx
operator|->
name|path
operator|->
name|len
argument_list|,
name|ctx
operator|->
name|md5
argument_list|)
expr_stmt|;
name|ngx_log_debug2
argument_list|(
name|NGX_LOG_DEBUG_HTTP
argument_list|,
name|r
operator|->
name|connection
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"file cache uri: %s, md5: %s"
argument_list|,
name|ctx
operator|->
name|key
operator|.
name|data
argument_list|,
name|ctx
operator|->
name|file
operator|.
name|name
operator|.
name|data
operator|+
name|ctx
operator|->
name|path
operator|->
name|name
operator|.
name|len
operator|+
literal|1
operator|+
name|ctx
operator|->
name|path
operator|->
name|len
argument_list|)
expr_stmt|;
name|ngx_create_hashed_filename
argument_list|(
operator|&
name|ctx
operator|->
name|file
argument_list|,
name|ctx
operator|->
name|path
argument_list|)
expr_stmt|;
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_HTTP
argument_list|,
name|r
operator|->
name|connection
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"file cache name: %s"
argument_list|,
name|ctx
operator|->
name|file
operator|.
name|name
operator|.
name|data
argument_list|)
expr_stmt|;
comment|/* TODO: look open files cache */
return|return
name|ngx_http_cache_open_file
argument_list|(
name|ctx
argument_list|,
literal|0
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_cache_open_file (ngx_http_cache_ctx_t * ctx,ngx_file_uniq_t uniq)
name|int
name|ngx_http_cache_open_file
parameter_list|(
name|ngx_http_cache_ctx_t
modifier|*
name|ctx
parameter_list|,
name|ngx_file_uniq_t
name|uniq
parameter_list|)
block|{
name|ssize_t
name|n
decl_stmt|;
name|ngx_err_t
name|err
decl_stmt|;
name|ngx_http_cache_header_t
modifier|*
name|h
decl_stmt|;
name|ctx
operator|->
name|file
operator|.
name|fd
operator|=
name|ngx_open_file
argument_list|(
name|ctx
operator|->
name|file
operator|.
name|name
operator|.
name|data
argument_list|,
name|NGX_FILE_RDONLY
argument_list|,
name|NGX_FILE_OPEN
argument_list|)
expr_stmt|;
if|if
condition|(
name|ctx
operator|->
name|file
operator|.
name|fd
operator|==
name|NGX_INVALID_FILE
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
name|NGX_ENOENT
operator|||
name|err
operator|==
name|NGX_ENOTDIR
condition|)
block|{
return|return
name|NGX_DECLINED
return|;
block|}
name|ngx_log_error
argument_list|(
name|NGX_LOG_CRIT
argument_list|,
name|ctx
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
name|ngx_open_file_n
literal|" \"%s\" failed"
argument_list|,
name|ctx
operator|->
name|file
operator|.
name|name
operator|.
name|data
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|uniq
condition|)
block|{
if|if
condition|(
name|ngx_fd_info
argument_list|(
name|ctx
operator|->
name|file
operator|.
name|fd
argument_list|,
operator|&
name|ctx
operator|->
name|file
operator|.
name|info
argument_list|)
operator|==
name|NGX_FILE_ERROR
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_CRIT
argument_list|,
name|ctx
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
name|ngx_fd_info_n
literal|" \"%s\" failed"
argument_list|,
name|ctx
operator|->
name|file
operator|.
name|name
operator|.
name|data
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|ngx_file_uniq
argument_list|(
operator|&
name|ctx
operator|->
name|file
operator|.
name|info
argument_list|)
operator|==
name|uniq
condition|)
block|{
if|if
condition|(
name|ngx_close_file
argument_list|(
name|ctx
operator|->
name|file
operator|.
name|fd
argument_list|)
operator|==
name|NGX_FILE_ERROR
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|ctx
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
name|ngx_close_file_n
literal|" \"%s\" failed"
argument_list|,
name|ctx
operator|->
name|file
operator|.
name|name
operator|.
name|data
argument_list|)
expr_stmt|;
block|}
return|return
name|NGX_HTTP_CACHE_THE_SAME
return|;
block|}
block|}
name|n
operator|=
name|ngx_read_file
argument_list|(
operator|&
name|ctx
operator|->
name|file
argument_list|,
name|ctx
operator|->
name|buf
operator|->
name|pos
argument_list|,
name|ctx
operator|->
name|buf
operator|->
name|end
operator|-
name|ctx
operator|->
name|buf
operator|->
name|last
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|n
operator|==
name|NGX_ERROR
operator|||
name|n
operator|==
name|NGX_AGAIN
condition|)
block|{
return|return
name|n
return|;
block|}
if|if
condition|(
name|n
operator|<=
name|ctx
operator|->
name|header_size
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_CRIT
argument_list|,
name|ctx
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"cache file \"%s\" is too small"
argument_list|,
name|ctx
operator|->
name|file
operator|.
name|name
operator|.
name|data
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
name|h
operator|=
operator|(
name|ngx_http_cache_header_t
operator|*
operator|)
name|ctx
operator|->
name|buf
operator|->
name|pos
expr_stmt|;
name|ctx
operator|->
name|expires
operator|=
name|h
operator|->
name|expires
expr_stmt|;
name|ctx
operator|->
name|last_modified
operator|=
name|h
operator|->
name|last_modified
expr_stmt|;
name|ctx
operator|->
name|date
operator|=
name|h
operator|->
name|date
expr_stmt|;
name|ctx
operator|->
name|length
operator|=
name|h
operator|->
name|length
expr_stmt|;
if|if
condition|(
name|h
operator|->
name|key_len
operator|>
operator|(
name|size_t
operator|)
operator|(
name|ctx
operator|->
name|buf
operator|->
name|end
operator|-
name|ctx
operator|->
name|buf
operator|->
name|pos
operator|)
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|ctx
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"cache file \"%s\" is probably invalid"
argument_list|,
name|ctx
operator|->
name|file
operator|.
name|name
operator|.
name|data
argument_list|)
expr_stmt|;
return|return
name|NGX_DECLINED
return|;
block|}
if|if
condition|(
name|ctx
operator|->
name|key
operator|.
name|len
operator|&&
operator|(
name|h
operator|->
name|key_len
operator|!=
name|ctx
operator|->
name|key
operator|.
name|len
operator|||
name|ngx_strncmp
argument_list|(
name|h
operator|->
name|key
argument_list|,
name|ctx
operator|->
name|key
operator|.
name|data
argument_list|,
name|h
operator|->
name|key_len
argument_list|)
operator|!=
literal|0
operator|)
condition|)
block|{
name|h
operator|->
name|key
index|[
name|h
operator|->
name|key_len
index|]
operator|=
literal|'\0'
expr_stmt|;
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|ctx
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"md5 collision: \"%s\" and \"%s\""
argument_list|,
name|h
operator|->
name|key
argument_list|,
name|ctx
operator|->
name|key
operator|.
name|data
argument_list|)
expr_stmt|;
return|return
name|NGX_DECLINED
return|;
block|}
name|ctx
operator|->
name|buf
operator|->
name|last
operator|+=
name|n
expr_stmt|;
if|if
condition|(
name|ctx
operator|->
name|expires
operator|<
name|ngx_time
argument_list|()
condition|)
block|{
name|ngx_log_debug0
argument_list|(
name|NGX_LOG_DEBUG_HTTP
argument_list|,
name|ctx
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"http file cache expired"
argument_list|)
expr_stmt|;
return|return
name|NGX_HTTP_CACHE_STALE
return|;
block|}
comment|/* TODO: NGX_HTTP_CACHE_AGED */
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_cache_update_file (ngx_http_request_t * r,ngx_http_cache_ctx_t * ctx,ngx_str_t * temp_file)
name|int
name|ngx_http_cache_update_file
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_cache_ctx_t
modifier|*
name|ctx
parameter_list|,
name|ngx_str_t
modifier|*
name|temp_file
parameter_list|)
block|{
name|int
name|retry
decl_stmt|;
name|ngx_err_t
name|err
decl_stmt|;
name|retry
operator|=
literal|0
expr_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
if|if
condition|(
name|ngx_rename_file
argument_list|(
name|temp_file
operator|->
name|data
argument_list|,
name|ctx
operator|->
name|file
operator|.
name|name
operator|.
name|data
argument_list|)
operator|==
name|NGX_OK
condition|)
block|{
return|return
name|NGX_OK
return|;
block|}
name|err
operator|=
name|ngx_errno
expr_stmt|;
if|#
directive|if
operator|(
name|WIN32
operator|)
if|if
condition|(
name|err
operator|==
name|NGX_EEXIST
condition|)
block|{
if|if
condition|(
name|ngx_win32_rename_file
argument_list|(
name|temp_file
argument_list|,
operator|&
name|ctx
operator|->
name|file
operator|.
name|name
argument_list|,
name|r
operator|->
name|pool
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
block|}
endif|#
directive|endif
if|if
condition|(
name|retry
operator|||
operator|(
name|err
operator|!=
name|NGX_ENOENT
operator|&&
name|err
operator|!=
name|NGX_ENOTDIR
operator|)
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_CRIT
argument_list|,
name|r
operator|->
name|connection
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
name|ngx_rename_file_n
literal|"(\"%s\", \"%s\") failed"
argument_list|,
name|temp_file
operator|->
name|data
argument_list|,
name|ctx
operator|->
name|file
operator|.
name|name
operator|.
name|data
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|ngx_create_path
argument_list|(
operator|&
name|ctx
operator|->
name|file
argument_list|,
name|ctx
operator|->
name|path
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|retry
operator|=
literal|1
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|ngx_garbage_collector_http_cache_handler (ngx_gc_t * gc,ngx_str_t * name,ngx_dir_t * dir)
name|int
name|ngx_garbage_collector_http_cache_handler
parameter_list|(
name|ngx_gc_t
modifier|*
name|gc
parameter_list|,
name|ngx_str_t
modifier|*
name|name
parameter_list|,
name|ngx_dir_t
modifier|*
name|dir
parameter_list|)
block|{
name|int
name|rc
decl_stmt|;
name|char
name|data
index|[
sizeof|sizeof
argument_list|(
name|ngx_http_cache_header_t
argument_list|)
index|]
decl_stmt|;
name|ngx_hunk_t
name|buf
decl_stmt|;
name|ngx_http_cache_ctx_t
name|ctx
decl_stmt|;
name|ctx
operator|.
name|file
operator|.
name|fd
operator|=
name|NGX_INVALID_FILE
expr_stmt|;
name|ctx
operator|.
name|file
operator|.
name|name
operator|=
operator|*
name|name
expr_stmt|;
name|ctx
operator|.
name|file
operator|.
name|log
operator|=
name|gc
operator|->
name|log
expr_stmt|;
name|ctx
operator|.
name|header_size
operator|=
sizeof|sizeof
argument_list|(
name|ngx_http_cache_header_t
argument_list|)
expr_stmt|;
name|ctx
operator|.
name|buf
operator|=
operator|&
name|buf
expr_stmt|;
name|ctx
operator|.
name|log
operator|=
name|gc
operator|->
name|log
expr_stmt|;
name|ctx
operator|.
name|key
operator|.
name|len
operator|=
literal|0
expr_stmt|;
name|buf
operator|.
name|type
operator|=
name|NGX_HUNK_IN_MEMORY
operator||
name|NGX_HUNK_TEMP
expr_stmt|;
name|buf
operator|.
name|pos
operator|=
name|data
expr_stmt|;
name|buf
operator|.
name|last
operator|=
name|data
expr_stmt|;
name|buf
operator|.
name|start
operator|=
name|data
expr_stmt|;
name|buf
operator|.
name|end
operator|=
name|data
operator|+
sizeof|sizeof
argument_list|(
name|ngx_http_cache_header_t
argument_list|)
expr_stmt|;
name|rc
operator|=
name|ngx_http_cache_open_file
argument_list|(
operator|&
name|ctx
argument_list|,
literal|0
argument_list|)
expr_stmt|;
comment|/* TODO: NGX_AGAIN */
if|if
condition|(
name|rc
operator|!=
name|NGX_ERROR
operator|&&
name|rc
operator|!=
name|NGX_DECLINED
operator|&&
name|rc
operator|!=
name|NGX_HTTP_CACHE_STALE
condition|)
block|{
return|return
name|NGX_OK
return|;
block|}
if|if
condition|(
name|ngx_delete_file
argument_list|(
name|name
operator|->
name|data
argument_list|)
operator|==
name|NGX_FILE_ERROR
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_CRIT
argument_list|,
name|gc
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
name|ngx_delete_file_n
literal|" \"%s\" failed"
argument_list|,
name|name
operator|->
name|data
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
name|gc
operator|->
name|deleted
operator|++
expr_stmt|;
name|gc
operator|->
name|freed
operator|+=
name|ngx_de_size
argument_list|(
name|dir
argument_list|)
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

end_unit

