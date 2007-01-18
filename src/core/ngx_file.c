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
DECL|variable|ngx_temp_number
specifier|static
name|ngx_atomic_uint_t
name|ngx_temp_number
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_random_number
specifier|static
name|ngx_atomic_uint_t
name|ngx_random_number
decl_stmt|;
end_decl_stmt

begin_function
name|ssize_t
DECL|function|ngx_write_chain_to_temp_file (ngx_temp_file_t * tf,ngx_chain_t * chain)
name|ngx_write_chain_to_temp_file
parameter_list|(
name|ngx_temp_file_t
modifier|*
name|tf
parameter_list|,
name|ngx_chain_t
modifier|*
name|chain
parameter_list|)
block|{
name|ngx_int_t
name|rc
decl_stmt|;
if|if
condition|(
name|tf
operator|->
name|file
operator|.
name|fd
operator|==
name|NGX_INVALID_FILE
condition|)
block|{
name|rc
operator|=
name|ngx_create_temp_file
argument_list|(
operator|&
name|tf
operator|->
name|file
argument_list|,
name|tf
operator|->
name|path
argument_list|,
name|tf
operator|->
name|pool
argument_list|,
name|tf
operator|->
name|persistent
argument_list|,
name|tf
operator|->
name|access
argument_list|)
expr_stmt|;
if|if
condition|(
name|rc
operator|==
name|NGX_ERROR
operator|||
name|rc
operator|==
name|NGX_AGAIN
condition|)
block|{
return|return
name|rc
return|;
block|}
if|if
condition|(
name|tf
operator|->
name|log_level
condition|)
block|{
name|ngx_log_error
argument_list|(
name|tf
operator|->
name|log_level
argument_list|,
name|tf
operator|->
name|file
operator|.
name|log
argument_list|,
literal|0
argument_list|,
literal|"%s %V"
argument_list|,
name|tf
operator|->
name|warn
argument_list|,
operator|&
name|tf
operator|->
name|file
operator|.
name|name
argument_list|)
expr_stmt|;
block|}
block|}
return|return
name|ngx_write_chain_to_file
argument_list|(
operator|&
name|tf
operator|->
name|file
argument_list|,
name|chain
argument_list|,
name|tf
operator|->
name|offset
argument_list|,
name|tf
operator|->
name|pool
argument_list|)
return|;
block|}
end_function

begin_function
name|ngx_int_t
DECL|function|ngx_create_temp_file (ngx_file_t * file,ngx_path_t * path,ngx_pool_t * pool,ngx_uint_t persistent,ngx_uint_t access)
name|ngx_create_temp_file
parameter_list|(
name|ngx_file_t
modifier|*
name|file
parameter_list|,
name|ngx_path_t
modifier|*
name|path
parameter_list|,
name|ngx_pool_t
modifier|*
name|pool
parameter_list|,
name|ngx_uint_t
name|persistent
parameter_list|,
name|ngx_uint_t
name|access
parameter_list|)
block|{
name|ngx_err_t
name|err
decl_stmt|;
name|ngx_atomic_uint_t
name|n
decl_stmt|;
name|ngx_pool_cleanup_t
modifier|*
name|cln
decl_stmt|;
name|ngx_pool_cleanup_file_t
modifier|*
name|clnf
decl_stmt|;
name|file
operator|->
name|name
operator|.
name|len
operator|=
name|path
operator|->
name|name
operator|.
name|len
operator|+
literal|1
operator|+
name|path
operator|->
name|len
operator|+
name|NGX_ATOMIC_T_LEN
expr_stmt|;
name|file
operator|->
name|name
operator|.
name|data
operator|=
name|ngx_palloc
argument_list|(
name|pool
argument_list|,
name|file
operator|->
name|name
operator|.
name|len
operator|+
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|file
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
if|#
directive|if
literal|0
block_content|for (i = 0; i< file->name.len; i++) {          file->name.data[i] = 'X';     }
endif|#
directive|endif
name|ngx_memcpy
argument_list|(
name|file
operator|->
name|name
operator|.
name|data
argument_list|,
name|path
operator|->
name|name
operator|.
name|data
argument_list|,
name|path
operator|->
name|name
operator|.
name|len
argument_list|)
expr_stmt|;
name|n
operator|=
name|ngx_next_temp_number
argument_list|(
literal|0
argument_list|)
expr_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
operator|(
name|void
operator|)
name|ngx_sprintf
argument_list|(
name|file
operator|->
name|name
operator|.
name|data
operator|+
name|path
operator|->
name|name
operator|.
name|len
operator|+
literal|1
operator|+
name|path
operator|->
name|len
argument_list|,
literal|"%0muA%Z"
argument_list|,
name|n
argument_list|)
expr_stmt|;
name|ngx_create_hashed_filename
argument_list|(
name|file
argument_list|,
name|path
argument_list|)
expr_stmt|;
name|cln
operator|=
name|ngx_pool_cleanup_add
argument_list|(
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_pool_cleanup_file_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|cln
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|file
operator|->
name|fd
operator|=
name|ngx_open_tempfile
argument_list|(
name|file
operator|->
name|name
operator|.
name|data
argument_list|,
name|persistent
argument_list|,
name|access
argument_list|)
expr_stmt|;
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_CORE
argument_list|,
name|file
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"temp fd:%d"
argument_list|,
name|file
operator|->
name|fd
argument_list|)
expr_stmt|;
if|if
condition|(
name|file
operator|->
name|fd
operator|!=
name|NGX_INVALID_FILE
condition|)
block|{
name|cln
operator|->
name|handler
operator|=
name|ngx_pool_cleanup_file
expr_stmt|;
name|clnf
operator|=
name|cln
operator|->
name|data
expr_stmt|;
name|clnf
operator|->
name|fd
operator|=
name|file
operator|->
name|fd
expr_stmt|;
name|clnf
operator|->
name|name
operator|=
name|file
operator|->
name|name
operator|.
name|data
expr_stmt|;
name|clnf
operator|->
name|log
operator|=
name|pool
operator|->
name|log
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
name|NGX_EEXIST
condition|)
block|{
name|n
operator|=
name|ngx_next_temp_number
argument_list|(
literal|1
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|(
name|path
operator|->
name|level
index|[
literal|0
index|]
operator|==
literal|0
operator|)
operator|||
operator|(
name|err
operator|!=
name|NGX_ENOENT
if|#
directive|if
operator|(
name|NGX_WIN32
operator|)
operator|&&
name|err
operator|!=
name|NGX_ENOTDIR
endif|#
directive|endif
operator|)
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
name|err
argument_list|,
name|ngx_open_tempfile_n
literal|" \"%s\" failed"
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
name|ngx_create_path
argument_list|(
name|file
argument_list|,
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
block|}
block|}
end_function

begin_function
name|void
DECL|function|ngx_create_hashed_filename (ngx_file_t * file,ngx_path_t * path)
name|ngx_create_hashed_filename
parameter_list|(
name|ngx_file_t
modifier|*
name|file
parameter_list|,
name|ngx_path_t
modifier|*
name|path
parameter_list|)
block|{
name|size_t
name|name
decl_stmt|,
name|pos
decl_stmt|,
name|level
decl_stmt|;
name|ngx_uint_t
name|i
decl_stmt|;
name|name
operator|=
name|file
operator|->
name|name
operator|.
name|len
expr_stmt|;
name|pos
operator|=
name|path
operator|->
name|name
operator|.
name|len
operator|+
literal|1
expr_stmt|;
name|file
operator|->
name|name
operator|.
name|data
index|[
name|path
operator|->
name|name
operator|.
name|len
operator|+
name|path
operator|->
name|len
index|]
operator|=
literal|'/'
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
literal|3
condition|;
name|i
operator|++
control|)
block|{
name|level
operator|=
name|path
operator|->
name|level
index|[
name|i
index|]
expr_stmt|;
if|if
condition|(
name|level
operator|==
literal|0
condition|)
block|{
break|break;
block|}
name|name
operator|-=
name|level
expr_stmt|;
name|file
operator|->
name|name
operator|.
name|data
index|[
name|pos
operator|-
literal|1
index|]
operator|=
literal|'/'
expr_stmt|;
name|ngx_memcpy
argument_list|(
operator|&
name|file
operator|->
name|name
operator|.
name|data
index|[
name|pos
index|]
argument_list|,
operator|&
name|file
operator|->
name|name
operator|.
name|data
index|[
name|name
index|]
argument_list|,
name|level
argument_list|)
expr_stmt|;
name|pos
operator|+=
name|level
operator|+
literal|1
expr_stmt|;
block|}
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_CORE
argument_list|,
name|file
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"hashed path: %s"
argument_list|,
name|file
operator|->
name|name
operator|.
name|data
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
name|ngx_int_t
DECL|function|ngx_create_path (ngx_file_t * file,ngx_path_t * path)
name|ngx_create_path
parameter_list|(
name|ngx_file_t
modifier|*
name|file
parameter_list|,
name|ngx_path_t
modifier|*
name|path
parameter_list|)
block|{
name|size_t
name|pos
decl_stmt|;
name|ngx_err_t
name|err
decl_stmt|;
name|ngx_uint_t
name|i
decl_stmt|;
name|pos
operator|=
name|path
operator|->
name|name
operator|.
name|len
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
literal|3
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|path
operator|->
name|level
index|[
name|i
index|]
operator|==
literal|0
condition|)
block|{
break|break;
block|}
name|pos
operator|+=
name|path
operator|->
name|level
index|[
name|i
index|]
operator|+
literal|1
expr_stmt|;
name|file
operator|->
name|name
operator|.
name|data
index|[
name|pos
index|]
operator|=
literal|'\0'
expr_stmt|;
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_CORE
argument_list|,
name|file
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"temp file: \"%s\""
argument_list|,
name|file
operator|->
name|name
operator|.
name|data
argument_list|)
expr_stmt|;
if|if
condition|(
name|ngx_create_dir
argument_list|(
name|file
operator|->
name|name
operator|.
name|data
argument_list|,
literal|0700
argument_list|)
operator|==
name|NGX_FILE_ERROR
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
name|NGX_EEXIST
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
name|err
argument_list|,
name|ngx_create_dir_n
literal|" \"%s\" failed"
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
block|}
name|file
operator|->
name|name
operator|.
name|data
index|[
name|pos
index|]
operator|=
literal|'/'
expr_stmt|;
block|}
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
name|ngx_err_t
DECL|function|ngx_create_full_path (u_char * dir,ngx_uint_t access)
name|ngx_create_full_path
parameter_list|(
name|u_char
modifier|*
name|dir
parameter_list|,
name|ngx_uint_t
name|access
parameter_list|)
block|{
name|u_char
modifier|*
name|p
decl_stmt|,
name|ch
decl_stmt|;
name|ngx_err_t
name|err
decl_stmt|;
for|for
control|(
name|p
operator|=
name|dir
operator|+
literal|1
init|;
operator|*
name|p
condition|;
name|p
operator|++
control|)
block|{
name|ch
operator|=
operator|*
name|p
expr_stmt|;
if|if
condition|(
name|ch
operator|!=
literal|'/'
condition|)
block|{
continue|continue;
block|}
operator|*
name|p
operator|=
literal|'\0'
expr_stmt|;
if|if
condition|(
name|ngx_create_dir
argument_list|(
name|dir
argument_list|,
name|access
argument_list|)
operator|==
name|NGX_FILE_ERROR
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
name|NGX_EEXIST
condition|)
block|{
return|return
name|err
return|;
block|}
block|}
operator|*
name|p
operator|=
literal|'/'
expr_stmt|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
name|void
DECL|function|ngx_init_temp_number (void)
name|ngx_init_temp_number
parameter_list|(
name|void
parameter_list|)
block|{
name|ngx_temp_number
operator|=
literal|0
expr_stmt|;
name|ngx_random_number
operator|=
literal|123456
expr_stmt|;
block|}
end_function

begin_function
name|ngx_atomic_uint_t
DECL|function|ngx_next_temp_number (ngx_uint_t collision)
name|ngx_next_temp_number
parameter_list|(
name|ngx_uint_t
name|collision
parameter_list|)
block|{
if|if
condition|(
name|collision
condition|)
block|{
name|ngx_temp_number
operator|+=
name|ngx_random_number
expr_stmt|;
block|}
return|return
name|ngx_temp_number
operator|++
return|;
block|}
end_function

begin_function
name|char
modifier|*
DECL|function|ngx_conf_set_path_slot (ngx_conf_t * cf,ngx_command_t * cmd,void * conf)
name|ngx_conf_set_path_slot
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|ngx_command_t
modifier|*
name|cmd
parameter_list|,
name|void
modifier|*
name|conf
parameter_list|)
block|{
name|char
modifier|*
name|p
init|=
name|conf
decl_stmt|;
name|ssize_t
name|level
decl_stmt|;
name|ngx_str_t
modifier|*
name|value
decl_stmt|;
name|ngx_uint_t
name|i
decl_stmt|,
name|n
decl_stmt|;
name|ngx_path_t
modifier|*
name|path
decl_stmt|,
modifier|*
modifier|*
name|slot
decl_stmt|;
name|slot
operator|=
operator|(
name|ngx_path_t
operator|*
operator|*
operator|)
operator|(
name|p
operator|+
name|cmd
operator|->
name|offset
operator|)
expr_stmt|;
if|if
condition|(
operator|*
name|slot
condition|)
block|{
return|return
literal|"is duplicate"
return|;
block|}
name|path
operator|=
name|ngx_pcalloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_path_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|path
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|value
operator|=
name|cf
operator|->
name|args
operator|->
name|elts
expr_stmt|;
name|path
operator|->
name|name
operator|=
name|value
index|[
literal|1
index|]
expr_stmt|;
if|if
condition|(
name|path
operator|->
name|name
operator|.
name|data
index|[
name|path
operator|->
name|name
operator|.
name|len
operator|-
literal|1
index|]
operator|==
literal|'/'
condition|)
block|{
name|path
operator|->
name|name
operator|.
name|len
operator|--
expr_stmt|;
block|}
if|if
condition|(
name|ngx_conf_full_name
argument_list|(
name|cf
operator|->
name|cycle
argument_list|,
operator|&
name|path
operator|->
name|name
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|NULL
return|;
block|}
name|path
operator|->
name|len
operator|=
literal|0
expr_stmt|;
name|path
operator|->
name|cleaner
operator|=
operator|(
name|ngx_gc_handler_pt
operator|)
name|cmd
operator|->
name|post
expr_stmt|;
name|path
operator|->
name|conf_file
operator|=
name|cf
operator|->
name|conf_file
operator|->
name|file
operator|.
name|name
operator|.
name|data
expr_stmt|;
name|path
operator|->
name|line
operator|=
name|cf
operator|->
name|conf_file
operator|->
name|line
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
operator|,
name|n
operator|=
literal|2
init|;
name|n
operator|<
name|cf
operator|->
name|args
operator|->
name|nelts
condition|;
name|i
operator|++
operator|,
name|n
operator|++
control|)
block|{
name|level
operator|=
name|ngx_atoi
argument_list|(
name|value
index|[
name|n
index|]
operator|.
name|data
argument_list|,
name|value
index|[
name|n
index|]
operator|.
name|len
argument_list|)
expr_stmt|;
if|if
condition|(
name|level
operator|==
name|NGX_ERROR
operator|||
name|level
operator|==
literal|0
condition|)
block|{
return|return
literal|"invalid value"
return|;
block|}
name|path
operator|->
name|level
index|[
name|i
index|]
operator|=
name|level
expr_stmt|;
name|path
operator|->
name|len
operator|+=
name|level
operator|+
literal|1
expr_stmt|;
block|}
while|while
condition|(
name|i
operator|<
literal|3
condition|)
block|{
name|path
operator|->
name|level
index|[
name|i
operator|++
index|]
operator|=
literal|0
expr_stmt|;
block|}
operator|*
name|slot
operator|=
name|path
expr_stmt|;
if|if
condition|(
name|ngx_add_path
argument_list|(
name|cf
argument_list|,
name|slot
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

begin_function
name|ngx_int_t
DECL|function|ngx_add_path (ngx_conf_t * cf,ngx_path_t ** slot)
name|ngx_add_path
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|ngx_path_t
modifier|*
modifier|*
name|slot
parameter_list|)
block|{
name|ngx_uint_t
name|i
decl_stmt|,
name|n
decl_stmt|;
name|ngx_path_t
modifier|*
name|path
decl_stmt|,
modifier|*
modifier|*
name|p
decl_stmt|;
name|path
operator|=
operator|*
name|slot
expr_stmt|;
name|p
operator|=
name|cf
operator|->
name|cycle
operator|->
name|pathes
operator|.
name|elts
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|cf
operator|->
name|cycle
operator|->
name|pathes
operator|.
name|nelts
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|p
index|[
name|i
index|]
operator|->
name|name
operator|.
name|len
operator|==
name|path
operator|->
name|name
operator|.
name|len
operator|&&
name|ngx_strcmp
argument_list|(
name|p
index|[
name|i
index|]
operator|->
name|name
operator|.
name|data
argument_list|,
name|path
operator|->
name|name
operator|.
name|data
argument_list|)
operator|==
literal|0
condition|)
block|{
for|for
control|(
name|n
operator|=
literal|0
init|;
name|n
operator|<
literal|3
condition|;
name|n
operator|++
control|)
block|{
if|if
condition|(
name|p
index|[
name|i
index|]
operator|->
name|level
index|[
name|n
index|]
operator|!=
name|path
operator|->
name|level
index|[
name|n
index|]
condition|)
block|{
if|if
condition|(
name|path
operator|->
name|conf_file
operator|==
name|NULL
condition|)
block|{
if|if
condition|(
name|p
index|[
name|i
index|]
operator|->
name|conf_file
operator|==
name|NULL
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_EMERG
argument_list|,
name|cf
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"the default path name \"%V\" has "
literal|"the same name as another default path, "
literal|"but the different levels, you need to "
literal|"redefine one of them in http section"
argument_list|,
operator|&
name|p
index|[
name|i
index|]
operator|->
name|name
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
name|ngx_log_error
argument_list|(
name|NGX_LOG_EMERG
argument_list|,
name|cf
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"the path name \"%V\" in %s:%ui has "
literal|"the same name as default path, but "
literal|"the different levels, you need to "
literal|"define default path in http section"
argument_list|,
operator|&
name|p
index|[
name|i
index|]
operator|->
name|name
argument_list|,
name|p
index|[
name|i
index|]
operator|->
name|conf_file
argument_list|,
name|p
index|[
name|i
index|]
operator|->
name|line
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
name|ngx_conf_log_error
argument_list|(
name|NGX_LOG_EMERG
argument_list|,
name|cf
argument_list|,
literal|0
argument_list|,
literal|"the same path name \"%V\" in %s:%ui "
literal|"has the different levels than"
argument_list|,
operator|&
name|p
index|[
name|i
index|]
operator|->
name|name
argument_list|,
name|p
index|[
name|i
index|]
operator|->
name|conf_file
argument_list|,
name|p
index|[
name|i
index|]
operator|->
name|line
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|p
index|[
name|i
index|]
operator|->
name|level
index|[
name|n
index|]
operator|==
literal|0
condition|)
block|{
break|break;
block|}
block|}
operator|*
name|slot
operator|=
name|p
index|[
name|i
index|]
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
block|}
name|p
operator|=
name|ngx_array_push
argument_list|(
operator|&
name|cf
operator|->
name|cycle
operator|->
name|pathes
argument_list|)
expr_stmt|;
if|if
condition|(
name|p
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
operator|*
name|p
operator|=
name|path
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
name|ngx_int_t
DECL|function|ngx_create_pathes (ngx_cycle_t * cycle,ngx_uid_t user)
name|ngx_create_pathes
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|,
name|ngx_uid_t
name|user
parameter_list|)
block|{
name|ngx_err_t
name|err
decl_stmt|;
name|ngx_uint_t
name|i
decl_stmt|;
name|ngx_path_t
modifier|*
modifier|*
name|path
decl_stmt|;
if|#
directive|if
operator|!
operator|(
name|NGX_WIN32
operator|)
name|ngx_file_info_t
name|fi
decl_stmt|;
endif|#
directive|endif
name|path
operator|=
name|cycle
operator|->
name|pathes
operator|.
name|elts
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|cycle
operator|->
name|pathes
operator|.
name|nelts
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|ngx_create_dir
argument_list|(
name|path
index|[
name|i
index|]
operator|->
name|name
operator|.
name|data
argument_list|,
literal|0700
argument_list|)
operator|==
name|NGX_FILE_ERROR
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
name|NGX_EEXIST
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_EMERG
argument_list|,
name|cycle
operator|->
name|log
argument_list|,
name|err
argument_list|,
name|ngx_create_dir_n
literal|" \"%s\" failed"
argument_list|,
name|path
index|[
name|i
index|]
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
block|}
if|if
condition|(
name|user
operator|==
operator|(
name|ngx_uid_t
operator|)
name|NGX_CONF_UNSET_UINT
condition|)
block|{
continue|continue;
block|}
if|#
directive|if
operator|!
operator|(
name|NGX_WIN32
operator|)
if|if
condition|(
name|ngx_file_info
argument_list|(
operator|(
specifier|const
name|char
operator|*
operator|)
name|path
index|[
name|i
index|]
operator|->
name|name
operator|.
name|data
argument_list|,
operator|&
name|fi
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
name|cycle
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
name|ngx_file_info_n
literal|" \"%s\" failed"
argument_list|,
name|path
index|[
name|i
index|]
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
name|fi
operator|.
name|st_uid
operator|!=
name|user
condition|)
block|{
if|if
condition|(
name|chown
argument_list|(
operator|(
specifier|const
name|char
operator|*
operator|)
name|path
index|[
name|i
index|]
operator|->
name|name
operator|.
name|data
argument_list|,
name|user
argument_list|,
operator|-
literal|1
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
name|cycle
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"chown(\"%s\", %d) failed"
argument_list|,
name|path
index|[
name|i
index|]
operator|->
name|name
operator|.
name|data
argument_list|,
name|user
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
block|}
if|if
condition|(
operator|(
name|fi
operator|.
name|st_mode
operator|&
operator|(
name|S_IRUSR
operator||
name|S_IWUSR
operator||
name|S_IXUSR
operator|)
operator|)
operator|!=
operator|(
name|S_IRUSR
operator||
name|S_IWUSR
operator||
name|S_IXUSR
operator|)
condition|)
block|{
name|fi
operator|.
name|st_mode
operator||=
operator|(
name|S_IRUSR
operator||
name|S_IWUSR
operator||
name|S_IXUSR
operator|)
expr_stmt|;
if|if
condition|(
name|chmod
argument_list|(
operator|(
specifier|const
name|char
operator|*
operator|)
name|path
index|[
name|i
index|]
operator|->
name|name
operator|.
name|data
argument_list|,
name|fi
operator|.
name|st_mode
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
name|cycle
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"chmod() \"%s\" failed"
argument_list|,
name|path
index|[
name|i
index|]
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
block|}
endif|#
directive|endif
block|}
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
name|ngx_int_t
DECL|function|ngx_walk_tree (ngx_tree_ctx_t * ctx,ngx_str_t * tree)
name|ngx_walk_tree
parameter_list|(
name|ngx_tree_ctx_t
modifier|*
name|ctx
parameter_list|,
name|ngx_str_t
modifier|*
name|tree
parameter_list|)
block|{
name|void
modifier|*
name|data
decl_stmt|,
modifier|*
name|prev
decl_stmt|;
name|u_char
modifier|*
name|p
decl_stmt|,
modifier|*
name|name
decl_stmt|;
name|size_t
name|len
decl_stmt|;
name|ngx_int_t
name|rc
decl_stmt|;
name|ngx_err_t
name|err
decl_stmt|;
name|ngx_str_t
name|file
decl_stmt|,
name|buf
decl_stmt|;
name|ngx_dir_t
name|dir
decl_stmt|;
name|buf
operator|.
name|len
operator|=
literal|0
expr_stmt|;
name|buf
operator|.
name|data
operator|=
name|NULL
expr_stmt|;
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_CORE
argument_list|,
name|ctx
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"walk tree \"%V\""
argument_list|,
name|tree
argument_list|)
expr_stmt|;
if|if
condition|(
name|ngx_open_dir
argument_list|(
name|tree
argument_list|,
operator|&
name|dir
argument_list|)
operator|==
name|NGX_ERROR
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
name|ngx_open_dir_n
literal|" \"%s\" failed"
argument_list|,
name|tree
operator|->
name|data
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
name|prev
operator|=
name|ctx
operator|->
name|data
expr_stmt|;
if|if
condition|(
name|ctx
operator|->
name|alloc
condition|)
block|{
name|data
operator|=
name|ngx_alloc
argument_list|(
name|ctx
operator|->
name|alloc
argument_list|,
name|ctx
operator|->
name|log
argument_list|)
expr_stmt|;
if|if
condition|(
name|data
operator|==
name|NULL
condition|)
block|{
goto|goto
name|failed
goto|;
block|}
if|if
condition|(
name|ctx
operator|->
name|init_handler
argument_list|(
name|data
argument_list|,
name|prev
argument_list|)
operator|==
name|NGX_ABORT
condition|)
block|{
goto|goto
name|failed
goto|;
block|}
name|ctx
operator|->
name|data
operator|=
name|data
expr_stmt|;
block|}
for|for
control|(
init|;
condition|;
control|)
block|{
name|ngx_set_errno
argument_list|(
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|ngx_read_dir
argument_list|(
operator|&
name|dir
argument_list|)
operator|==
name|NGX_ERROR
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
name|NGX_ENOMOREFILES
condition|)
block|{
name|rc
operator|=
name|NGX_OK
expr_stmt|;
block|}
else|else
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_CRIT
argument_list|,
name|ctx
operator|->
name|log
argument_list|,
name|err
argument_list|,
name|ngx_read_dir_n
literal|" \"%s\" failed"
argument_list|,
name|tree
operator|->
name|data
argument_list|)
expr_stmt|;
name|rc
operator|=
name|NGX_ERROR
expr_stmt|;
block|}
goto|goto
name|done
goto|;
block|}
name|len
operator|=
name|ngx_de_namelen
argument_list|(
operator|&
name|dir
argument_list|)
expr_stmt|;
name|name
operator|=
name|ngx_de_name
argument_list|(
operator|&
name|dir
argument_list|)
expr_stmt|;
name|ngx_log_debug2
argument_list|(
name|NGX_LOG_DEBUG_CORE
argument_list|,
name|ctx
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"tree name %uz:\"%s\""
argument_list|,
name|len
argument_list|,
name|name
argument_list|)
expr_stmt|;
if|if
condition|(
name|len
operator|==
literal|1
operator|&&
name|name
index|[
literal|0
index|]
operator|==
literal|'.'
condition|)
block|{
continue|continue;
block|}
if|if
condition|(
name|len
operator|==
literal|2
operator|&&
name|name
index|[
literal|0
index|]
operator|==
literal|'.'
operator|&&
name|name
index|[
literal|1
index|]
operator|==
literal|'.'
condition|)
block|{
continue|continue;
block|}
name|file
operator|.
name|len
operator|=
name|tree
operator|->
name|len
operator|+
literal|1
operator|+
name|len
expr_stmt|;
if|if
condition|(
name|file
operator|.
name|len
operator|+
name|NGX_DIR_MASK_LEN
operator|>
name|buf
operator|.
name|len
condition|)
block|{
if|if
condition|(
name|buf
operator|.
name|len
condition|)
block|{
name|ngx_free
argument_list|(
name|buf
operator|.
name|data
argument_list|)
expr_stmt|;
block|}
name|buf
operator|.
name|len
operator|=
name|tree
operator|->
name|len
operator|+
literal|1
operator|+
name|len
operator|+
name|NGX_DIR_MASK_LEN
expr_stmt|;
name|buf
operator|.
name|data
operator|=
name|ngx_alloc
argument_list|(
name|buf
operator|.
name|len
operator|+
literal|1
argument_list|,
name|ctx
operator|->
name|log
argument_list|)
expr_stmt|;
if|if
condition|(
name|buf
operator|.
name|data
operator|==
name|NULL
condition|)
block|{
goto|goto
name|failed
goto|;
block|}
block|}
name|p
operator|=
name|ngx_cpymem
argument_list|(
name|buf
operator|.
name|data
argument_list|,
name|tree
operator|->
name|data
argument_list|,
name|tree
operator|->
name|len
argument_list|)
expr_stmt|;
operator|*
name|p
operator|++
operator|=
literal|'/'
expr_stmt|;
name|ngx_memcpy
argument_list|(
name|p
argument_list|,
name|name
argument_list|,
name|len
operator|+
literal|1
argument_list|)
expr_stmt|;
name|file
operator|.
name|data
operator|=
name|buf
operator|.
name|data
expr_stmt|;
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_CORE
argument_list|,
name|ctx
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"tree path \"%s\""
argument_list|,
name|file
operator|.
name|data
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|dir
operator|.
name|valid_info
condition|)
block|{
if|if
condition|(
name|ngx_de_info
argument_list|(
name|file
operator|.
name|data
argument_list|,
operator|&
name|dir
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
name|ngx_de_info_n
literal|" \"%s\" failed"
argument_list|,
name|file
operator|.
name|data
argument_list|)
expr_stmt|;
continue|continue;
block|}
block|}
if|if
condition|(
name|ngx_de_is_file
argument_list|(
operator|&
name|dir
argument_list|)
condition|)
block|{
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_CORE
argument_list|,
name|ctx
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"tree file \"%s\""
argument_list|,
name|file
operator|.
name|data
argument_list|)
expr_stmt|;
name|ctx
operator|->
name|size
operator|=
name|ngx_de_size
argument_list|(
operator|&
name|dir
argument_list|)
expr_stmt|;
name|ctx
operator|->
name|access
operator|=
name|ngx_de_access
argument_list|(
operator|&
name|dir
argument_list|)
expr_stmt|;
name|ctx
operator|->
name|mtime
operator|=
name|ngx_de_mtime
argument_list|(
operator|&
name|dir
argument_list|)
expr_stmt|;
if|if
condition|(
name|ctx
operator|->
name|file_handler
argument_list|(
name|ctx
argument_list|,
operator|&
name|file
argument_list|)
operator|==
name|NGX_ABORT
condition|)
block|{
goto|goto
name|failed
goto|;
block|}
block|}
if|else if
condition|(
name|ngx_de_is_dir
argument_list|(
operator|&
name|dir
argument_list|)
condition|)
block|{
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_CORE
argument_list|,
name|ctx
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"tree enter dir \"%s\""
argument_list|,
name|file
operator|.
name|data
argument_list|)
expr_stmt|;
name|ctx
operator|->
name|access
operator|=
name|ngx_de_access
argument_list|(
operator|&
name|dir
argument_list|)
expr_stmt|;
name|ctx
operator|->
name|mtime
operator|=
name|ngx_de_mtime
argument_list|(
operator|&
name|dir
argument_list|)
expr_stmt|;
if|if
condition|(
name|ctx
operator|->
name|pre_tree_handler
argument_list|(
name|ctx
argument_list|,
operator|&
name|file
argument_list|)
operator|==
name|NGX_ABORT
condition|)
block|{
goto|goto
name|failed
goto|;
block|}
if|if
condition|(
name|ngx_walk_tree
argument_list|(
name|ctx
argument_list|,
operator|&
name|file
argument_list|)
operator|==
name|NGX_ABORT
condition|)
block|{
goto|goto
name|failed
goto|;
block|}
name|ctx
operator|->
name|access
operator|=
name|ngx_de_access
argument_list|(
operator|&
name|dir
argument_list|)
expr_stmt|;
name|ctx
operator|->
name|mtime
operator|=
name|ngx_de_mtime
argument_list|(
operator|&
name|dir
argument_list|)
expr_stmt|;
if|if
condition|(
name|ctx
operator|->
name|post_tree_handler
argument_list|(
name|ctx
argument_list|,
operator|&
name|file
argument_list|)
operator|==
name|NGX_ABORT
condition|)
block|{
goto|goto
name|failed
goto|;
block|}
block|}
else|else
block|{
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_CORE
argument_list|,
name|ctx
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"tree special \"%s\""
argument_list|,
name|file
operator|.
name|data
argument_list|)
expr_stmt|;
if|if
condition|(
name|ctx
operator|->
name|spec_handler
argument_list|(
name|ctx
argument_list|,
operator|&
name|file
argument_list|)
operator|==
name|NGX_ABORT
condition|)
block|{
goto|goto
name|failed
goto|;
block|}
block|}
block|}
name|failed
label|:
name|rc
operator|=
name|NGX_ABORT
expr_stmt|;
name|done
label|:
if|if
condition|(
name|buf
operator|.
name|len
condition|)
block|{
name|ngx_free
argument_list|(
name|buf
operator|.
name|data
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|ctx
operator|->
name|alloc
condition|)
block|{
name|ngx_free
argument_list|(
name|ctx
operator|->
name|data
argument_list|)
expr_stmt|;
name|ctx
operator|->
name|data
operator|=
name|prev
expr_stmt|;
block|}
if|if
condition|(
name|ngx_close_dir
argument_list|(
operator|&
name|dir
argument_list|)
operator|==
name|NGX_ERROR
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
name|ngx_close_dir_n
literal|" \"%s\" failed"
argument_list|,
name|tree
operator|->
name|data
argument_list|)
expr_stmt|;
block|}
return|return
name|rc
return|;
block|}
end_function

end_unit

