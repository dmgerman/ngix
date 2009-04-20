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
DECL|variable|ngx_threads_n
name|ngx_int_t
name|ngx_threads_n
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|stack_size
specifier|static
name|size_t
name|stack_size
decl_stmt|;
end_decl_stmt

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
name|stack_size
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

begin_function
name|ngx_int_t
DECL|function|ngx_init_threads (int n,size_t size,ngx_cycle_t * cycle)
name|ngx_init_threads
parameter_list|(
name|int
name|n
parameter_list|,
name|size_t
name|size
parameter_list|,
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
block|{
name|stack_size
operator|=
name|size
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
name|ngx_err_t
DECL|function|ngx_thread_key_create (ngx_tls_key_t * key)
name|ngx_thread_key_create
parameter_list|(
name|ngx_tls_key_t
modifier|*
name|key
parameter_list|)
block|{
operator|*
name|key
operator|=
name|TlsAlloc
argument_list|()
expr_stmt|;
if|if
condition|(
operator|*
name|key
operator|==
name|TLS_OUT_OF_INDEXES
condition|)
block|{
return|return
name|ngx_errno
return|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
name|ngx_err_t
DECL|function|ngx_thread_set_tls (ngx_tls_key_t * key,void * data)
name|ngx_thread_set_tls
parameter_list|(
name|ngx_tls_key_t
modifier|*
name|key
parameter_list|,
name|void
modifier|*
name|data
parameter_list|)
block|{
if|if
condition|(
name|TlsSetValue
argument_list|(
operator|*
name|key
argument_list|,
name|data
argument_list|)
operator|==
literal|0
condition|)
block|{
return|return
name|ngx_errno
return|;
block|}
return|return
literal|0
return|;
block|}
end_function

begin_function
name|ngx_mutex_t
modifier|*
DECL|function|ngx_mutex_init (ngx_log_t * log,ngx_uint_t flags)
name|ngx_mutex_init
parameter_list|(
name|ngx_log_t
modifier|*
name|log
parameter_list|,
name|ngx_uint_t
name|flags
parameter_list|)
block|{
return|return
operator|(
name|ngx_mutex_t
operator|*
operator|)
literal|1
return|;
block|}
end_function

begin_comment
comment|/* STUB */
end_comment

begin_function
name|void
DECL|function|ngx_mutex_lock (ngx_mutex_t * m)
name|ngx_mutex_lock
parameter_list|(
name|ngx_mutex_t
modifier|*
name|m
parameter_list|)
block|{
return|return;
block|}
end_function

begin_function
name|ngx_int_t
DECL|function|ngx_mutex_trylock (ngx_mutex_t * m)
name|ngx_mutex_trylock
parameter_list|(
name|ngx_mutex_t
modifier|*
name|m
parameter_list|)
block|{
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
name|void
DECL|function|ngx_mutex_unlock (ngx_mutex_t * m)
name|ngx_mutex_unlock
parameter_list|(
name|ngx_mutex_t
modifier|*
name|m
parameter_list|)
block|{
return|return;
block|}
end_function

begin_comment
comment|/**/
end_comment

end_unit

