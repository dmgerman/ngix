begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (C) Igor Sysoev  */
end_comment

begin_ifndef
ifndef|#
directive|ifndef
name|_NGX_SHMTX_H_INCLUDED_
end_ifndef

begin_define
DECL|macro|_NGX_SHMTX_H_INCLUDED_
define|#
directive|define
name|_NGX_SHMTX_H_INCLUDED_
end_define

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

begin_typedef
DECL|struct|__anon28b17b6d0108
typedef|typedef
struct|struct
block|{
if|#
directive|if
operator|(
name|NGX_HAVE_ATOMIC_OPS
operator|)
DECL|member|lock
name|ngx_atomic_t
modifier|*
name|lock
decl_stmt|;
else|#
directive|else
name|ngx_fd_t
name|fd
decl_stmt|;
name|u_char
modifier|*
name|name
decl_stmt|;
endif|#
directive|endif
DECL|typedef|ngx_shmtx_t
block|}
name|ngx_shmtx_t
typedef|;
end_typedef

begin_function_decl
name|ngx_int_t
name|ngx_shmtx_create
parameter_list|(
name|ngx_shmtx_t
modifier|*
name|mtx
parameter_list|,
name|void
modifier|*
name|addr
parameter_list|,
name|u_char
modifier|*
name|name
parameter_list|)
function_decl|;
end_function_decl

begin_if
if|#
directive|if
operator|(
name|NGX_HAVE_ATOMIC_OPS
operator|)
end_if

begin_function
specifier|static
name|ngx_inline
name|ngx_uint_t
DECL|function|ngx_shmtx_trylock (ngx_shmtx_t * mtx)
name|ngx_shmtx_trylock
parameter_list|(
name|ngx_shmtx_t
modifier|*
name|mtx
parameter_list|)
block|{
return|return
operator|(
operator|*
name|mtx
operator|->
name|lock
operator|==
literal|0
operator|&&
name|ngx_atomic_cmp_set
argument_list|(
name|mtx
operator|->
name|lock
argument_list|,
literal|0
argument_list|,
name|ngx_pid
argument_list|)
operator|)
return|;
block|}
end_function

begin_define
DECL|macro|ngx_shmtx_lock (mtx)
define|#
directive|define
name|ngx_shmtx_lock
parameter_list|(
name|mtx
parameter_list|)
value|ngx_spinlock((mtx)->lock, ngx_pid, 1024)
end_define

begin_define
DECL|macro|ngx_shmtx_unlock (mtx)
define|#
directive|define
name|ngx_shmtx_unlock
parameter_list|(
name|mtx
parameter_list|)
value|(void) ngx_atomic_cmp_set((mtx)->lock, ngx_pid, 0)
end_define

begin_define
DECL|macro|ngx_shmtx_destory (mtx)
define|#
directive|define
name|ngx_shmtx_destory
parameter_list|(
name|mtx
parameter_list|)
end_define

begin_else
else|#
directive|else
end_else

begin_function
specifier|static
name|ngx_inline
name|ngx_uint_t
DECL|function|ngx_shmtx_trylock (ngx_shmtx_t * mtx)
name|ngx_shmtx_trylock
parameter_list|(
name|ngx_shmtx_t
modifier|*
name|mtx
parameter_list|)
block|{
name|ngx_err_t
name|err
decl_stmt|;
name|err
operator|=
name|ngx_trylock_fd
argument_list|(
name|mtx
operator|->
name|fd
argument_list|)
expr_stmt|;
if|if
condition|(
name|err
operator|==
literal|0
condition|)
block|{
return|return
literal|1
return|;
block|}
if|if
condition|(
name|err
operator|==
name|NGX_EAGAIN
condition|)
block|{
return|return
literal|0
return|;
block|}
name|ngx_log_abort
argument_list|(
name|err
argument_list|,
name|ngx_trylock_fd_n
literal|" failed"
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
specifier|static
name|ngx_inline
name|void
DECL|function|ngx_shmtx_lock (ngx_shmtx_t * mtx)
name|ngx_shmtx_lock
parameter_list|(
name|ngx_shmtx_t
modifier|*
name|mtx
parameter_list|)
block|{
name|ngx_err_t
name|err
decl_stmt|;
name|err
operator|=
name|ngx_lock_fd
argument_list|(
name|mtx
operator|->
name|fd
argument_list|)
expr_stmt|;
if|if
condition|(
name|err
operator|==
literal|0
condition|)
block|{
return|return;
block|}
name|ngx_log_abort
argument_list|(
name|err
argument_list|,
name|ngx_lock_fd_n
literal|" failed"
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
specifier|static
name|ngx_inline
name|void
DECL|function|ngx_shmtx_unlock (ngx_shmtx_t * mtx)
name|ngx_shmtx_unlock
parameter_list|(
name|ngx_shmtx_t
modifier|*
name|mtx
parameter_list|)
block|{
name|ngx_err_t
name|err
decl_stmt|;
name|err
operator|=
name|ngx_unlock_fd
argument_list|(
name|mtx
operator|->
name|fd
argument_list|)
expr_stmt|;
if|if
condition|(
name|err
operator|==
literal|0
condition|)
block|{
return|return;
block|}
name|ngx_log_abort
argument_list|(
name|err
argument_list|,
name|ngx_unlock_fd_n
literal|" failed"
argument_list|)
expr_stmt|;
block|}
end_function

begin_function_decl
name|void
name|ngx_shmtx_destory
parameter_list|(
name|ngx_shmtx_t
modifier|*
name|mtx
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* _NGX_SHMTX_H_INCLUDED_ */
end_comment

end_unit

