begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|<ngx_core.h>
end_include

begin_include
include|#
directive|include
file|<ngx_types.h>
end_include

begin_include
include|#
directive|include
file|<ngx_file.h>
end_include

begin_function
DECL|function|ngx_read_file (ngx_file_t * file,char * buf,size_t size,off_t offset)
name|ssize_t
name|ngx_read_file
parameter_list|(
name|ngx_file_t
modifier|*
name|file
parameter_list|,
name|char
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
name|size_t
name|n
decl_stmt|;
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
name|NULL
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
literal|"ReadFile() failed"
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

end_unit

