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
DECL|variable|ngx_temp_number
specifier|static
name|int
name|ngx_temp_number
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_random
specifier|static
name|int
name|ngx_random
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|ngx_write_chain_to_temp_file (ngx_temp_file_t * tf,ngx_chain_t * chain)
name|int
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
name|int
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
operator|!
name|tf
operator|->
name|persistent
operator|&&
name|tf
operator|->
name|warn
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_WARN
argument_list|,
name|tf
operator|->
name|file
operator|.
name|log
argument_list|,
literal|0
argument_list|,
name|tf
operator|->
name|warn
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
DECL|function|ngx_create_temp_file (ngx_file_t * file,ngx_path_t * path,ngx_pool_t * pool,int persistent)
name|int
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
name|int
name|persistent
parameter_list|)
block|{
name|int
name|num
decl_stmt|;
name|ngx_err_t
name|err
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
literal|10
expr_stmt|;
name|ngx_test_null
argument_list|(
name|file
operator|->
name|name
operator|.
name|data
argument_list|,
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
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
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
name|num
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
name|ngx_snprintf
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
literal|11
argument_list|,
literal|"%010u"
argument_list|,
name|num
argument_list|)
expr_stmt|;
name|ngx_create_hashed_filename
argument_list|(
name|file
argument_list|,
name|path
argument_list|)
expr_stmt|;
if|#
directive|if
literal|0
if|#
directive|if
operator|(
name|WIN32
operator|)
block_content|file->fd = CreateFile(file->name.data,                         GENERIC_READ|GENERIC_WRITE,                         FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,                         NULL,                         CREATE_NEW,                         persistent ? 0:                             FILE_ATTRIBUTE_TEMPORARY|FILE_FLAG_DELETE_ON_CLOSE,                         NULL);
else|#
directive|else
block_content|file->fd = open(file->name.data, O_CREAT|O_EXCL|O_WRONLY, 0600);
endif|#
directive|endif
endif|#
directive|endif
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
argument_list|)
expr_stmt|;
name|ngx_log_debug
argument_list|(
argument|file->log
argument_list|,
literal|"temp fd: %d"
argument|_ file->fd
argument_list|)
empty_stmt|;
if|if
condition|(
name|file
operator|->
name|fd
operator|!=
name|NGX_INVALID_FILE
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
if|if
condition|(
name|err
operator|==
name|NGX_EEXIST
condition|)
block|{
name|num
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
name|err
operator|!=
name|NGX_ENOENT
if|#
directive|if
operator|(
name|WIN32_NEED_TEST
operator|)
operator|&&
name|err
operator|!=
name|NGX_ENOTDIR
endif|#
directive|endif
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
DECL|function|ngx_create_hashed_filename (ngx_file_t * file,ngx_path_t * path)
name|void
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
name|int
name|i
decl_stmt|,
name|name
decl_stmt|,
name|pos
decl_stmt|,
name|level
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
name|ngx_log_debug
argument_list|(
argument|file->log
argument_list|,
literal|"hashed path: %s"
argument|_ file->name.data
argument_list|)
empty_stmt|;
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
name|ngx_log_debug
argument_list|(
argument|file->log
argument_list|,
literal|"hashed path: %s"
argument|_ file->name.data
argument_list|)
empty_stmt|;
block|}
end_function

begin_function
DECL|function|ngx_create_path (ngx_file_t * file,ngx_path_t * path)
name|int
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
name|int
name|i
decl_stmt|,
name|pos
decl_stmt|;
name|ngx_err_t
name|err
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
name|ngx_log_debug
argument_list|(
argument|file->log
argument_list|,
literal|"temp: %s"
argument|_ file->name.data
argument_list|)
empty_stmt|;
if|if
condition|(
name|ngx_mkdir
argument_list|(
name|file
operator|->
name|name
operator|.
name|data
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
name|ngx_mkdir_n
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
DECL|function|ngx_init_temp_number ()
name|void
name|ngx_init_temp_number
parameter_list|()
block|{
name|ngx_random
operator|=
literal|0
expr_stmt|;
name|ngx_temp_number
operator|=
name|ngx_random
expr_stmt|;
while|while
condition|(
name|ngx_random
operator|<
literal|10000
condition|)
block|{
name|ngx_random
operator|=
literal|123456
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|ngx_next_temp_number (int collision)
name|int
name|ngx_next_temp_number
parameter_list|(
name|int
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
name|ngx_random
expr_stmt|;
block|}
return|return
name|ngx_temp_number
operator|++
return|;
block|}
end_function

begin_function
DECL|function|ngx_conf_set_path_slot (ngx_conf_t * cf,ngx_command_t * cmd,void * conf)
name|char
modifier|*
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
name|int
name|i
decl_stmt|,
name|n
decl_stmt|;
name|ngx_str_t
modifier|*
name|value
decl_stmt|;
name|ngx_path_t
modifier|*
name|path
decl_stmt|,
modifier|*
modifier|*
name|pp
decl_stmt|;
name|pp
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
name|pp
condition|)
block|{
return|return
literal|"is duplicate"
return|;
block|}
name|ngx_test_null
argument_list|(
name|path
argument_list|,
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
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
operator|*
name|pp
operator|=
name|path
expr_stmt|;
name|value
operator|=
operator|(
name|ngx_str_t
operator|*
operator|)
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
name|path
operator|->
name|len
operator|=
literal|0
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
name|path
operator|->
name|level
index|[
name|i
index|]
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
name|path
operator|->
name|level
index|[
name|i
index|]
operator|==
name|NGX_ERROR
operator|||
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
return|return
literal|"invalid value"
return|;
block|}
name|path
operator|->
name|len
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
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

end_unit

