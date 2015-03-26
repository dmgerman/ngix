begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (C) Igor Sysoev  * Copyright (C) Nginx, Inc.  */
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

begin_function
name|ngx_err_t
DECL|function|ngx_create_thread (ngx_tid_t * tid,ngx_thread_value_t (__stdcall * func)(void * arg),void * arg,ngx_log_t * log)
name|ngx_create_thread
parameter_list|(
name|ngx_tid_t
modifier|*
name|tid
parameter_list|,
name|ngx_thread_value_t
function_decl|(
name|__stdcall
modifier|*
name|func
function_decl|)
parameter_list|(
name|void
modifier|*
name|arg
parameter_list|)
parameter_list|,
name|void
modifier|*
name|arg
parameter_list|,
name|ngx_log_t
modifier|*
name|log
parameter_list|)
block|{
name|u_long
name|id
decl_stmt|;
name|ngx_err_t
name|err
decl_stmt|;
operator|*
name|tid
operator|=
name|CreateThread
argument_list|(
name|NULL
argument_list|,
literal|0
argument_list|,
name|func
argument_list|,
name|arg
argument_list|,
literal|0
argument_list|,
operator|&
name|id
argument_list|)
expr_stmt|;
if|if
condition|(
operator|*
name|tid
operator|!=
name|NULL
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
literal|"create thread "
name|NGX_TID_T_FMT
argument_list|,
name|id
argument_list|)
expr_stmt|;
return|return
literal|0
return|;
block|}
name|err
operator|=
name|ngx_errno
expr_stmt|;
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|log
argument_list|,
name|err
argument_list|,
literal|"CreateThread() failed"
argument_list|)
expr_stmt|;
return|return
name|err
return|;
block|}
end_function

end_unit

