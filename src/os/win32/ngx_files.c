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

begin_define
DECL|macro|NGX_UTF16_BUFLEN
define|#
directive|define
name|NGX_UTF16_BUFLEN
value|256
end_define

begin_function_decl
specifier|static
name|u_short
modifier|*
name|ngx_utf8_to_utf16
parameter_list|(
name|u_short
modifier|*
name|utf16
parameter_list|,
name|u_char
modifier|*
name|utf8
parameter_list|,
name|size_t
name|len
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/* FILE_FLAG_BACKUP_SEMANTICS allows to obtain a handle to a directory */
end_comment

begin_function
name|ngx_fd_t
DECL|function|ngx_open_file (u_char * name,u_long mode,u_long create,u_long access)
name|ngx_open_file
parameter_list|(
name|u_char
modifier|*
name|name
parameter_list|,
name|u_long
name|mode
parameter_list|,
name|u_long
name|create
parameter_list|,
name|u_long
name|access
parameter_list|)
block|{
name|u_short
modifier|*
name|u
decl_stmt|;
name|ngx_fd_t
name|fd
decl_stmt|;
name|ngx_err_t
name|err
decl_stmt|;
name|u_short
name|utf16
index|[
name|NGX_UTF16_BUFLEN
index|]
decl_stmt|;
name|u
operator|=
name|ngx_utf8_to_utf16
argument_list|(
name|utf16
argument_list|,
name|name
argument_list|,
name|NGX_UTF16_BUFLEN
argument_list|)
expr_stmt|;
if|if
condition|(
name|u
operator|==
name|NULL
condition|)
block|{
return|return
name|INVALID_HANDLE_VALUE
return|;
block|}
name|fd
operator|=
name|CreateFileW
argument_list|(
name|u
argument_list|,
name|mode
argument_list|,
name|FILE_SHARE_READ
operator||
name|FILE_SHARE_WRITE
operator||
name|FILE_SHARE_DELETE
argument_list|,
name|NULL
argument_list|,
name|create
argument_list|,
name|FILE_FLAG_BACKUP_SEMANTICS
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
name|u
operator|!=
name|utf16
condition|)
block|{
name|err
operator|=
name|ngx_errno
expr_stmt|;
name|ngx_free
argument_list|(
name|u
argument_list|)
expr_stmt|;
name|ngx_set_errno
argument_list|(
name|err
argument_list|)
expr_stmt|;
block|}
return|return
name|fd
return|;
block|}
end_function

begin_function
name|ssize_t
DECL|function|ngx_read_file (ngx_file_t * file,u_char * buf,size_t size,off_t offset)
name|ngx_read_file
parameter_list|(
name|ngx_file_t
modifier|*
name|file
parameter_list|,
name|u_char
modifier|*
name|buf
parameter_list|,
name|size_t
name|size
parameter_list|,
name|off_t
name|offset
parameter_list|)
block|{
name|u_long
name|n
decl_stmt|;
name|ngx_err_t
name|err
decl_stmt|;
name|OVERLAPPED
name|ovlp
decl_stmt|,
modifier|*
name|povlp
decl_stmt|;
name|ovlp
operator|.
name|Internal
operator|=
literal|0
expr_stmt|;
name|ovlp
operator|.
name|InternalHigh
operator|=
literal|0
expr_stmt|;
name|ovlp
operator|.
name|Offset
operator|=
operator|(
name|u_long
operator|)
name|offset
expr_stmt|;
name|ovlp
operator|.
name|OffsetHigh
operator|=
operator|(
name|u_long
operator|)
operator|(
name|offset
operator|>>
literal|32
operator|)
expr_stmt|;
name|ovlp
operator|.
name|hEvent
operator|=
name|NULL
expr_stmt|;
name|povlp
operator|=
operator|&
name|ovlp
expr_stmt|;
if|if
condition|(
name|ReadFile
argument_list|(
name|file
operator|->
name|fd
argument_list|,
name|buf
argument_list|,
name|size
argument_list|,
operator|&
name|n
argument_list|,
name|povlp
argument_list|)
operator|==
literal|0
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
name|ERROR_HANDLE_EOF
condition|)
block|{
return|return
literal|0
return|;
block|}
name|ngx_log_error
argument_list|(
name|NGX_LOG_ERR
argument_list|,
name|file
operator|->
name|log
argument_list|,
name|err
argument_list|,
literal|"ReadFile() \"%s\" failed"
argument_list|,
name|file
operator|->
name|name
operator|.
name|data
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
name|file
operator|->
name|offset
operator|+=
name|n
expr_stmt|;
return|return
name|n
return|;
block|}
end_function

begin_function
name|ssize_t
DECL|function|ngx_write_file (ngx_file_t * file,u_char * buf,size_t size,off_t offset)
name|ngx_write_file
parameter_list|(
name|ngx_file_t
modifier|*
name|file
parameter_list|,
name|u_char
modifier|*
name|buf
parameter_list|,
name|size_t
name|size
parameter_list|,
name|off_t
name|offset
parameter_list|)
block|{
name|u_long
name|n
decl_stmt|;
name|OVERLAPPED
name|ovlp
decl_stmt|,
modifier|*
name|povlp
decl_stmt|;
name|ovlp
operator|.
name|Internal
operator|=
literal|0
expr_stmt|;
name|ovlp
operator|.
name|InternalHigh
operator|=
literal|0
expr_stmt|;
name|ovlp
operator|.
name|Offset
operator|=
operator|(
name|u_long
operator|)
name|offset
expr_stmt|;
name|ovlp
operator|.
name|OffsetHigh
operator|=
operator|(
name|u_long
operator|)
operator|(
name|offset
operator|>>
literal|32
operator|)
expr_stmt|;
name|ovlp
operator|.
name|hEvent
operator|=
name|NULL
expr_stmt|;
name|povlp
operator|=
operator|&
name|ovlp
expr_stmt|;
if|if
condition|(
name|WriteFile
argument_list|(
name|file
operator|->
name|fd
argument_list|,
name|buf
argument_list|,
name|size
argument_list|,
operator|&
name|n
argument_list|,
name|povlp
argument_list|)
operator|==
literal|0
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ERR
argument_list|,
name|file
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"WriteFile() \"%s\" failed"
argument_list|,
name|file
operator|->
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
name|n
operator|!=
name|size
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_CRIT
argument_list|,
name|file
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"WriteFile() \"%s\" has written only %ul of %uz"
argument_list|,
name|file
operator|->
name|name
operator|.
name|data
argument_list|,
name|n
argument_list|,
name|size
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
name|file
operator|->
name|offset
operator|+=
name|n
expr_stmt|;
return|return
name|n
return|;
block|}
end_function

begin_function
name|ssize_t
DECL|function|ngx_write_chain_to_file (ngx_file_t * file,ngx_chain_t * cl,off_t offset,ngx_pool_t * pool)
name|ngx_write_chain_to_file
parameter_list|(
name|ngx_file_t
modifier|*
name|file
parameter_list|,
name|ngx_chain_t
modifier|*
name|cl
parameter_list|,
name|off_t
name|offset
parameter_list|,
name|ngx_pool_t
modifier|*
name|pool
parameter_list|)
block|{
name|u_char
modifier|*
name|buf
decl_stmt|,
modifier|*
name|prev
decl_stmt|;
name|size_t
name|size
decl_stmt|;
name|ssize_t
name|total
decl_stmt|,
name|n
decl_stmt|;
name|total
operator|=
literal|0
expr_stmt|;
while|while
condition|(
name|cl
condition|)
block|{
name|buf
operator|=
name|cl
operator|->
name|buf
operator|->
name|pos
expr_stmt|;
name|prev
operator|=
name|buf
expr_stmt|;
name|size
operator|=
literal|0
expr_stmt|;
comment|/* coalesce the neighbouring bufs */
while|while
condition|(
name|cl
operator|&&
name|prev
operator|==
name|cl
operator|->
name|buf
operator|->
name|pos
condition|)
block|{
name|size
operator|+=
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
name|prev
operator|=
name|cl
operator|->
name|buf
operator|->
name|last
expr_stmt|;
name|cl
operator|=
name|cl
operator|->
name|next
expr_stmt|;
block|}
name|n
operator|=
name|ngx_write_file
argument_list|(
name|file
argument_list|,
name|buf
argument_list|,
name|size
argument_list|,
name|offset
argument_list|)
expr_stmt|;
if|if
condition|(
name|n
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|total
operator|+=
name|n
expr_stmt|;
name|offset
operator|+=
name|n
expr_stmt|;
block|}
return|return
name|total
return|;
block|}
end_function

begin_function
name|ssize_t
DECL|function|ngx_read_fd (ngx_fd_t fd,void * buf,size_t size)
name|ngx_read_fd
parameter_list|(
name|ngx_fd_t
name|fd
parameter_list|,
name|void
modifier|*
name|buf
parameter_list|,
name|size_t
name|size
parameter_list|)
block|{
name|u_long
name|n
decl_stmt|;
if|if
condition|(
name|ReadFile
argument_list|(
name|fd
argument_list|,
name|buf
argument_list|,
name|size
argument_list|,
operator|&
name|n
argument_list|,
name|NULL
argument_list|)
operator|!=
literal|0
condition|)
block|{
return|return
operator|(
name|size_t
operator|)
name|n
return|;
block|}
return|return
operator|-
literal|1
return|;
block|}
end_function

begin_function
name|ssize_t
DECL|function|ngx_write_fd (ngx_fd_t fd,void * buf,size_t size)
name|ngx_write_fd
parameter_list|(
name|ngx_fd_t
name|fd
parameter_list|,
name|void
modifier|*
name|buf
parameter_list|,
name|size_t
name|size
parameter_list|)
block|{
name|u_long
name|n
decl_stmt|;
if|if
condition|(
name|WriteFile
argument_list|(
name|fd
argument_list|,
name|buf
argument_list|,
name|size
argument_list|,
operator|&
name|n
argument_list|,
name|NULL
argument_list|)
operator|!=
literal|0
condition|)
block|{
return|return
operator|(
name|size_t
operator|)
name|n
return|;
block|}
return|return
operator|-
literal|1
return|;
block|}
end_function

begin_function
name|ssize_t
DECL|function|ngx_write_console (ngx_fd_t fd,void * buf,size_t size)
name|ngx_write_console
parameter_list|(
name|ngx_fd_t
name|fd
parameter_list|,
name|void
modifier|*
name|buf
parameter_list|,
name|size_t
name|size
parameter_list|)
block|{
name|u_long
name|n
decl_stmt|;
operator|(
name|void
operator|)
name|CharToOemBuff
argument_list|(
name|buf
argument_list|,
name|buf
argument_list|,
name|size
argument_list|)
expr_stmt|;
if|if
condition|(
name|WriteFile
argument_list|(
name|fd
argument_list|,
name|buf
argument_list|,
name|size
argument_list|,
operator|&
name|n
argument_list|,
name|NULL
argument_list|)
operator|!=
literal|0
condition|)
block|{
return|return
operator|(
name|size_t
operator|)
name|n
return|;
block|}
return|return
operator|-
literal|1
return|;
block|}
end_function

begin_function
name|ngx_int_t
DECL|function|ngx_win32_rename_file (ngx_str_t * from,ngx_str_t * to,ngx_log_t * log)
name|ngx_win32_rename_file
parameter_list|(
name|ngx_str_t
modifier|*
name|from
parameter_list|,
name|ngx_str_t
modifier|*
name|to
parameter_list|,
name|ngx_log_t
modifier|*
name|log
parameter_list|)
block|{
name|u_char
modifier|*
name|name
decl_stmt|;
name|ngx_int_t
name|rc
decl_stmt|;
name|ngx_uint_t
name|collision
decl_stmt|;
name|ngx_atomic_uint_t
name|num
decl_stmt|;
name|name
operator|=
name|ngx_alloc
argument_list|(
name|to
operator|->
name|len
operator|+
literal|1
operator|+
literal|10
operator|+
literal|1
operator|+
sizeof|sizeof
argument_list|(
literal|"DELETE"
argument_list|)
argument_list|,
name|log
argument_list|)
expr_stmt|;
if|if
condition|(
name|name
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|ngx_memcpy
argument_list|(
name|name
argument_list|,
name|to
operator|->
name|data
argument_list|,
name|to
operator|->
name|len
argument_list|)
expr_stmt|;
name|collision
operator|=
literal|0
expr_stmt|;
comment|/* mutex_lock() (per cache or single ?) */
for|for
control|(
init|;
condition|;
control|)
block|{
name|num
operator|=
name|ngx_next_temp_number
argument_list|(
name|collision
argument_list|)
expr_stmt|;
name|ngx_sprintf
argument_list|(
name|name
operator|+
name|to
operator|->
name|len
argument_list|,
literal|".%0muA.DELETE%Z"
argument_list|,
name|num
argument_list|)
expr_stmt|;
if|if
condition|(
name|MoveFile
argument_list|(
operator|(
specifier|const
name|char
operator|*
operator|)
name|to
operator|->
name|data
argument_list|,
operator|(
specifier|const
name|char
operator|*
operator|)
name|name
argument_list|)
operator|!=
literal|0
condition|)
block|{
break|break;
block|}
name|collision
operator|=
literal|1
expr_stmt|;
name|ngx_log_error
argument_list|(
name|NGX_LOG_CRIT
argument_list|,
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"MoveFile() \"%s\" to \"%s\" failed"
argument_list|,
name|to
operator|->
name|data
argument_list|,
name|name
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|MoveFile
argument_list|(
operator|(
specifier|const
name|char
operator|*
operator|)
name|from
operator|->
name|data
argument_list|,
operator|(
specifier|const
name|char
operator|*
operator|)
name|to
operator|->
name|data
argument_list|)
operator|==
literal|0
condition|)
block|{
name|rc
operator|=
name|NGX_ERROR
expr_stmt|;
block|}
else|else
block|{
name|rc
operator|=
name|NGX_OK
expr_stmt|;
block|}
if|if
condition|(
name|DeleteFile
argument_list|(
operator|(
specifier|const
name|char
operator|*
operator|)
name|name
argument_list|)
operator|==
literal|0
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_CRIT
argument_list|,
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"DeleteFile() \"%s\" failed"
argument_list|,
name|name
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|rc
operator|==
name|NGX_ERROR
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_CRIT
argument_list|,
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"MoveFile() \"%s\" to \"%s\" failed"
argument_list|,
name|from
operator|->
name|data
argument_list|,
name|to
operator|->
name|data
argument_list|)
expr_stmt|;
block|}
comment|/* mutex_unlock() */
name|ngx_free
argument_list|(
name|name
argument_list|)
expr_stmt|;
return|return
name|rc
return|;
block|}
end_function

begin_function
name|ngx_int_t
DECL|function|ngx_file_info (u_char * file,ngx_file_info_t * sb)
name|ngx_file_info
parameter_list|(
name|u_char
modifier|*
name|file
parameter_list|,
name|ngx_file_info_t
modifier|*
name|sb
parameter_list|)
block|{
name|long
name|rc
decl_stmt|;
name|u_short
modifier|*
name|u
decl_stmt|;
name|ngx_err_t
name|err
decl_stmt|;
name|WIN32_FILE_ATTRIBUTE_DATA
name|fa
decl_stmt|;
name|u_short
name|utf16
index|[
name|NGX_UTF16_BUFLEN
index|]
decl_stmt|;
name|u
operator|=
name|ngx_utf8_to_utf16
argument_list|(
name|utf16
argument_list|,
name|file
argument_list|,
name|NGX_UTF16_BUFLEN
argument_list|)
expr_stmt|;
if|if
condition|(
name|u
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_FILE_ERROR
return|;
block|}
name|rc
operator|=
name|GetFileAttributesExW
argument_list|(
name|u
argument_list|,
name|GetFileExInfoStandard
argument_list|,
operator|&
name|fa
argument_list|)
expr_stmt|;
if|if
condition|(
name|u
operator|!=
name|utf16
condition|)
block|{
name|err
operator|=
name|ngx_errno
expr_stmt|;
name|ngx_free
argument_list|(
name|u
argument_list|)
expr_stmt|;
name|ngx_set_errno
argument_list|(
name|err
argument_list|)
expr_stmt|;
block|}
name|sb
operator|->
name|dwFileAttributes
operator|=
name|fa
operator|.
name|dwFileAttributes
expr_stmt|;
name|sb
operator|->
name|ftCreationTime
operator|=
name|fa
operator|.
name|ftCreationTime
expr_stmt|;
name|sb
operator|->
name|ftLastAccessTime
operator|=
name|fa
operator|.
name|ftLastAccessTime
expr_stmt|;
name|sb
operator|->
name|ftLastWriteTime
operator|=
name|fa
operator|.
name|ftLastWriteTime
expr_stmt|;
name|sb
operator|->
name|nFileSizeHigh
operator|=
name|fa
operator|.
name|nFileSizeHigh
expr_stmt|;
name|sb
operator|->
name|nFileSizeLow
operator|=
name|fa
operator|.
name|nFileSizeLow
expr_stmt|;
return|return
name|rc
return|;
block|}
end_function

begin_function
name|ngx_int_t
DECL|function|ngx_set_file_time (u_char * name,ngx_fd_t fd,time_t s)
name|ngx_set_file_time
parameter_list|(
name|u_char
modifier|*
name|name
parameter_list|,
name|ngx_fd_t
name|fd
parameter_list|,
name|time_t
name|s
parameter_list|)
block|{
name|uint64_t
name|intervals
decl_stmt|;
name|FILETIME
name|ft
decl_stmt|;
comment|/* 116444736000000000 is commented in src/os/win32/ngx_time.c */
name|intervals
operator|=
name|s
operator|*
literal|10000000
operator|+
literal|116444736000000000
expr_stmt|;
name|ft
operator|.
name|dwLowDateTime
operator|=
operator|(
name|DWORD
operator|)
name|intervals
expr_stmt|;
name|ft
operator|.
name|dwHighDateTime
operator|=
operator|(
name|DWORD
operator|)
operator|(
name|intervals
operator|>>
literal|32
operator|)
expr_stmt|;
if|if
condition|(
name|SetFileTime
argument_list|(
name|fd
argument_list|,
name|NULL
argument_list|,
name|NULL
argument_list|,
operator|&
name|ft
argument_list|)
operator|!=
literal|0
condition|)
block|{
return|return
name|NGX_OK
return|;
block|}
return|return
name|NGX_ERROR
return|;
block|}
end_function

begin_function
name|char
modifier|*
DECL|function|ngx_realpath (u_char * path,u_char * resolved)
name|ngx_realpath
parameter_list|(
name|u_char
modifier|*
name|path
parameter_list|,
name|u_char
modifier|*
name|resolved
parameter_list|)
block|{
comment|/* STUB */
return|return
operator|(
name|char
operator|*
operator|)
name|path
return|;
block|}
end_function

begin_function
name|ngx_int_t
DECL|function|ngx_open_dir (ngx_str_t * name,ngx_dir_t * dir)
name|ngx_open_dir
parameter_list|(
name|ngx_str_t
modifier|*
name|name
parameter_list|,
name|ngx_dir_t
modifier|*
name|dir
parameter_list|)
block|{
name|ngx_cpystrn
argument_list|(
name|name
operator|->
name|data
operator|+
name|name
operator|->
name|len
argument_list|,
name|NGX_DIR_MASK
argument_list|,
name|NGX_DIR_MASK_LEN
operator|+
literal|1
argument_list|)
expr_stmt|;
name|dir
operator|->
name|dir
operator|=
name|FindFirstFile
argument_list|(
operator|(
specifier|const
name|char
operator|*
operator|)
name|name
operator|->
name|data
argument_list|,
operator|&
name|dir
operator|->
name|finddata
argument_list|)
expr_stmt|;
if|if
condition|(
name|dir
operator|->
name|dir
operator|==
name|INVALID_HANDLE_VALUE
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|dir
operator|->
name|valid_info
operator|=
literal|1
expr_stmt|;
name|dir
operator|->
name|valid_type
operator|=
literal|1
expr_stmt|;
name|dir
operator|->
name|ready
operator|=
literal|1
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
name|ngx_int_t
DECL|function|ngx_read_dir (ngx_dir_t * dir)
name|ngx_read_dir
parameter_list|(
name|ngx_dir_t
modifier|*
name|dir
parameter_list|)
block|{
if|if
condition|(
name|dir
operator|->
name|ready
condition|)
block|{
name|dir
operator|->
name|ready
operator|=
literal|0
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
if|if
condition|(
name|FindNextFile
argument_list|(
name|dir
operator|->
name|dir
argument_list|,
operator|&
name|dir
operator|->
name|finddata
argument_list|)
operator|!=
literal|0
condition|)
block|{
return|return
name|NGX_OK
return|;
block|}
return|return
name|NGX_ERROR
return|;
block|}
end_function

begin_function
name|ngx_int_t
DECL|function|ngx_open_glob (ngx_glob_t * gl)
name|ngx_open_glob
parameter_list|(
name|ngx_glob_t
modifier|*
name|gl
parameter_list|)
block|{
name|u_char
modifier|*
name|p
decl_stmt|;
name|size_t
name|len
decl_stmt|;
name|ngx_err_t
name|err
decl_stmt|;
name|gl
operator|->
name|dir
operator|=
name|FindFirstFile
argument_list|(
operator|(
specifier|const
name|char
operator|*
operator|)
name|gl
operator|->
name|pattern
argument_list|,
operator|&
name|gl
operator|->
name|finddata
argument_list|)
expr_stmt|;
if|if
condition|(
name|gl
operator|->
name|dir
operator|==
name|INVALID_HANDLE_VALUE
condition|)
block|{
name|err
operator|=
name|ngx_errno
expr_stmt|;
if|if
condition|(
operator|(
name|err
operator|==
name|ERROR_FILE_NOT_FOUND
operator|||
name|err
operator|==
name|ERROR_PATH_NOT_FOUND
operator|)
operator|&&
name|gl
operator|->
name|test
condition|)
block|{
name|gl
operator|->
name|no_match
operator|=
literal|1
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
return|return
name|NGX_ERROR
return|;
block|}
for|for
control|(
name|p
operator|=
name|gl
operator|->
name|pattern
init|;
operator|*
name|p
condition|;
name|p
operator|++
control|)
block|{
if|if
condition|(
operator|*
name|p
operator|==
literal|'/'
condition|)
block|{
name|gl
operator|->
name|last
operator|=
name|p
operator|+
literal|1
operator|-
name|gl
operator|->
name|pattern
expr_stmt|;
block|}
block|}
name|len
operator|=
name|ngx_strlen
argument_list|(
name|gl
operator|->
name|finddata
operator|.
name|cFileName
argument_list|)
expr_stmt|;
name|gl
operator|->
name|name
operator|.
name|len
operator|=
name|gl
operator|->
name|last
operator|+
name|len
expr_stmt|;
name|gl
operator|->
name|name
operator|.
name|data
operator|=
name|ngx_alloc
argument_list|(
name|gl
operator|->
name|name
operator|.
name|len
operator|+
literal|1
argument_list|,
name|gl
operator|->
name|log
argument_list|)
expr_stmt|;
if|if
condition|(
name|gl
operator|->
name|name
operator|.
name|data
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|ngx_memcpy
argument_list|(
name|gl
operator|->
name|name
operator|.
name|data
argument_list|,
name|gl
operator|->
name|pattern
argument_list|,
name|gl
operator|->
name|last
argument_list|)
expr_stmt|;
name|ngx_cpystrn
argument_list|(
name|gl
operator|->
name|name
operator|.
name|data
operator|+
name|gl
operator|->
name|last
argument_list|,
operator|(
name|u_char
operator|*
operator|)
name|gl
operator|->
name|finddata
operator|.
name|cFileName
argument_list|,
name|len
operator|+
literal|1
argument_list|)
expr_stmt|;
name|gl
operator|->
name|ready
operator|=
literal|1
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
name|ngx_int_t
DECL|function|ngx_read_glob (ngx_glob_t * gl,ngx_str_t * name)
name|ngx_read_glob
parameter_list|(
name|ngx_glob_t
modifier|*
name|gl
parameter_list|,
name|ngx_str_t
modifier|*
name|name
parameter_list|)
block|{
name|size_t
name|len
decl_stmt|;
name|ngx_err_t
name|err
decl_stmt|;
if|if
condition|(
name|gl
operator|->
name|no_match
condition|)
block|{
return|return
name|NGX_DONE
return|;
block|}
if|if
condition|(
name|gl
operator|->
name|ready
condition|)
block|{
operator|*
name|name
operator|=
name|gl
operator|->
name|name
expr_stmt|;
name|gl
operator|->
name|ready
operator|=
literal|0
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
name|ngx_free
argument_list|(
name|gl
operator|->
name|name
operator|.
name|data
argument_list|)
expr_stmt|;
name|gl
operator|->
name|name
operator|.
name|data
operator|=
name|NULL
expr_stmt|;
if|if
condition|(
name|FindNextFile
argument_list|(
name|gl
operator|->
name|dir
argument_list|,
operator|&
name|gl
operator|->
name|finddata
argument_list|)
operator|!=
literal|0
condition|)
block|{
name|len
operator|=
name|ngx_strlen
argument_list|(
name|gl
operator|->
name|finddata
operator|.
name|cFileName
argument_list|)
expr_stmt|;
name|gl
operator|->
name|name
operator|.
name|len
operator|=
name|gl
operator|->
name|last
operator|+
name|len
expr_stmt|;
name|gl
operator|->
name|name
operator|.
name|data
operator|=
name|ngx_alloc
argument_list|(
name|gl
operator|->
name|name
operator|.
name|len
operator|+
literal|1
argument_list|,
name|gl
operator|->
name|log
argument_list|)
expr_stmt|;
if|if
condition|(
name|gl
operator|->
name|name
operator|.
name|data
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|ngx_memcpy
argument_list|(
name|gl
operator|->
name|name
operator|.
name|data
argument_list|,
name|gl
operator|->
name|pattern
argument_list|,
name|gl
operator|->
name|last
argument_list|)
expr_stmt|;
name|ngx_cpystrn
argument_list|(
name|gl
operator|->
name|name
operator|.
name|data
operator|+
name|gl
operator|->
name|last
argument_list|,
operator|(
name|u_char
operator|*
operator|)
name|gl
operator|->
name|finddata
operator|.
name|cFileName
argument_list|,
name|len
operator|+
literal|1
argument_list|)
expr_stmt|;
operator|*
name|name
operator|=
name|gl
operator|->
name|name
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
name|err
operator|=
name|ngx_errno
expr_stmt|;
if|if
condition|(
name|err
operator|==
name|NGX_ENOMOREFILES
condition|)
block|{
return|return
name|NGX_DONE
return|;
block|}
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|gl
operator|->
name|log
argument_list|,
name|err
argument_list|,
literal|"FindNextFile(%s) failed"
argument_list|,
name|gl
operator|->
name|pattern
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
end_function

begin_function
name|void
DECL|function|ngx_close_glob (ngx_glob_t * gl)
name|ngx_close_glob
parameter_list|(
name|ngx_glob_t
modifier|*
name|gl
parameter_list|)
block|{
if|if
condition|(
name|gl
operator|->
name|name
operator|.
name|data
condition|)
block|{
name|ngx_free
argument_list|(
name|gl
operator|->
name|name
operator|.
name|data
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|gl
operator|->
name|dir
operator|==
name|INVALID_HANDLE_VALUE
condition|)
block|{
return|return;
block|}
if|if
condition|(
name|FindClose
argument_list|(
name|gl
operator|->
name|dir
argument_list|)
operator|==
literal|0
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|gl
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"FindClose(%s) failed"
argument_list|,
name|gl
operator|->
name|pattern
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
name|ngx_int_t
DECL|function|ngx_de_info (u_char * name,ngx_dir_t * dir)
name|ngx_de_info
parameter_list|(
name|u_char
modifier|*
name|name
parameter_list|,
name|ngx_dir_t
modifier|*
name|dir
parameter_list|)
block|{
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
name|ngx_int_t
DECL|function|ngx_de_link_info (u_char * name,ngx_dir_t * dir)
name|ngx_de_link_info
parameter_list|(
name|u_char
modifier|*
name|name
parameter_list|,
name|ngx_dir_t
modifier|*
name|dir
parameter_list|)
block|{
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
name|ngx_int_t
DECL|function|ngx_directio_on (ngx_fd_t fd)
name|ngx_directio_on
parameter_list|(
name|ngx_fd_t
name|fd
parameter_list|)
block|{
return|return
operator|~
name|NGX_FILE_ERROR
return|;
block|}
end_function

begin_function
name|ngx_int_t
DECL|function|ngx_directio_off (ngx_fd_t fd)
name|ngx_directio_off
parameter_list|(
name|ngx_fd_t
name|fd
parameter_list|)
block|{
return|return
operator|~
name|NGX_FILE_ERROR
return|;
block|}
end_function

begin_function
name|size_t
DECL|function|ngx_fs_bsize (u_char * name)
name|ngx_fs_bsize
parameter_list|(
name|u_char
modifier|*
name|name
parameter_list|)
block|{
name|u_char
name|root
index|[
literal|4
index|]
decl_stmt|;
name|u_long
name|sc
decl_stmt|,
name|bs
decl_stmt|,
name|nfree
decl_stmt|,
name|ncl
decl_stmt|;
if|if
condition|(
name|name
index|[
literal|2
index|]
operator|==
literal|':'
condition|)
block|{
name|ngx_cpystrn
argument_list|(
name|root
argument_list|,
name|name
argument_list|,
literal|4
argument_list|)
expr_stmt|;
name|name
operator|=
name|root
expr_stmt|;
block|}
if|if
condition|(
name|GetDiskFreeSpace
argument_list|(
operator|(
specifier|const
name|char
operator|*
operator|)
name|name
argument_list|,
operator|&
name|sc
argument_list|,
operator|&
name|bs
argument_list|,
operator|&
name|nfree
argument_list|,
operator|&
name|ncl
argument_list|)
operator|==
literal|0
condition|)
block|{
return|return
literal|512
return|;
block|}
return|return
name|sc
operator|*
name|bs
return|;
block|}
end_function

begin_function
specifier|static
name|u_short
modifier|*
DECL|function|ngx_utf8_to_utf16 (u_short * utf16,u_char * utf8,size_t len)
name|ngx_utf8_to_utf16
parameter_list|(
name|u_short
modifier|*
name|utf16
parameter_list|,
name|u_char
modifier|*
name|utf8
parameter_list|,
name|size_t
name|len
parameter_list|)
block|{
name|u_char
modifier|*
name|p
decl_stmt|;
name|u_short
modifier|*
name|u
decl_stmt|,
modifier|*
name|last
decl_stmt|;
name|uint32_t
name|n
decl_stmt|;
name|p
operator|=
name|utf8
expr_stmt|;
name|u
operator|=
name|utf16
expr_stmt|;
name|last
operator|=
name|utf16
operator|+
name|len
expr_stmt|;
while|while
condition|(
name|u
operator|<
name|last
condition|)
block|{
if|if
condition|(
operator|*
name|p
operator|<
literal|0x80
condition|)
block|{
operator|*
name|u
operator|=
operator|(
name|u_short
operator|)
operator|*
name|p
expr_stmt|;
if|if
condition|(
operator|*
name|p
operator|==
literal|0
condition|)
block|{
return|return
name|utf16
return|;
block|}
name|u
operator|++
expr_stmt|;
name|p
operator|++
expr_stmt|;
continue|continue;
block|}
name|n
operator|=
name|ngx_utf8_decode
argument_list|(
operator|&
name|p
argument_list|,
literal|4
argument_list|)
expr_stmt|;
if|if
condition|(
name|n
operator|>
literal|0xffff
condition|)
block|{
name|free
argument_list|(
name|utf16
argument_list|)
expr_stmt|;
name|ngx_set_errno
argument_list|(
name|NGX_EILSEQ
argument_list|)
expr_stmt|;
return|return
name|NULL
return|;
block|}
operator|*
name|u
operator|++
operator|=
operator|(
name|u_short
operator|)
name|n
expr_stmt|;
block|}
comment|/* the given buffer is not enough, allocate a new one */
name|u
operator|=
name|malloc
argument_list|(
operator|(
operator|(
name|p
operator|-
name|utf8
operator|)
operator|+
name|ngx_strlen
argument_list|(
name|p
argument_list|)
operator|+
literal|1
operator|)
operator|*
sizeof|sizeof
argument_list|(
name|u_short
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|u
operator|==
name|NULL
condition|)
block|{
return|return
name|NULL
return|;
block|}
name|ngx_memcpy
argument_list|(
name|u
argument_list|,
name|utf16
argument_list|,
name|len
operator|*
literal|2
argument_list|)
expr_stmt|;
name|utf16
operator|=
name|u
expr_stmt|;
name|u
operator|+=
name|len
expr_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
if|if
condition|(
operator|*
name|p
operator|<
literal|0x80
condition|)
block|{
operator|*
name|u
operator|=
operator|(
name|u_short
operator|)
operator|*
name|p
expr_stmt|;
if|if
condition|(
operator|*
name|p
operator|==
literal|0
condition|)
block|{
return|return
name|utf16
return|;
block|}
name|u
operator|++
expr_stmt|;
name|p
operator|++
expr_stmt|;
continue|continue;
block|}
name|n
operator|=
name|ngx_utf8_decode
argument_list|(
operator|&
name|p
argument_list|,
literal|4
argument_list|)
expr_stmt|;
if|if
condition|(
name|n
operator|>
literal|0xffff
condition|)
block|{
name|free
argument_list|(
name|utf16
argument_list|)
expr_stmt|;
name|ngx_set_errno
argument_list|(
name|NGX_EILSEQ
argument_list|)
expr_stmt|;
return|return
name|NULL
return|;
block|}
operator|*
name|u
operator|++
operator|=
operator|(
name|u_short
operator|)
name|n
expr_stmt|;
block|}
comment|/* unreachable */
block|}
end_function

end_unit

