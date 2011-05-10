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
DECL|struct|__anon29c887b00108
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
if|#
directive|if
operator|(
name|NGX_HAVE_POSIX_SEM
operator|)
DECL|member|semaphore
name|ngx_uint_t
name|semaphore
decl_stmt|;
DECL|member|sem
name|sem_t
name|sem
decl_stmt|;
endif|#
directive|endif
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
DECL|member|spin
name|ngx_uint_t
name|spin
decl_stmt|;
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

begin_function_decl
name|ngx_uint_t
name|ngx_shmtx_trylock
parameter_list|(
name|ngx_shmtx_t
modifier|*
name|mtx
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|ngx_shmtx_lock
parameter_list|(
name|ngx_shmtx_t
modifier|*
name|mtx
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|ngx_shmtx_unlock
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

begin_comment
comment|/* _NGX_SHMTX_H_INCLUDED_ */
end_comment

end_unit

