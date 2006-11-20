begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (C) Igor Sysoev  */
end_comment

begin_ifndef
ifndef|#
directive|ifndef
name|_NGX_SLAB_H_INCLUDED_
end_ifndef

begin_define
DECL|macro|_NGX_SLAB_H_INCLUDED_
define|#
directive|define
name|_NGX_SLAB_H_INCLUDED_
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
DECL|typedef|ngx_slab_page_t
typedef|typedef
name|struct
name|ngx_slab_page_s
name|ngx_slab_page_t
typedef|;
end_typedef

begin_struct
DECL|struct|ngx_slab_page_s
struct|struct
name|ngx_slab_page_s
block|{
DECL|member|slab
name|uintptr_t
name|slab
decl_stmt|;
DECL|member|next
name|ngx_slab_page_t
modifier|*
name|next
decl_stmt|;
DECL|member|prev
name|uintptr_t
name|prev
decl_stmt|;
block|}
struct|;
end_struct

begin_typedef
DECL|struct|__anon2abda62f0108
typedef|typedef
struct|struct
block|{
DECL|member|lock
name|ngx_atomic_t
name|lock
decl_stmt|;
DECL|member|min_size
name|size_t
name|min_size
decl_stmt|;
DECL|member|min_shift
name|size_t
name|min_shift
decl_stmt|;
DECL|member|pages
name|ngx_slab_page_t
modifier|*
name|pages
decl_stmt|;
DECL|member|free
name|ngx_slab_page_t
name|free
decl_stmt|;
DECL|member|start
name|u_char
modifier|*
name|start
decl_stmt|;
DECL|member|end
name|u_char
modifier|*
name|end
decl_stmt|;
DECL|member|mutex
name|ngx_shmtx_t
name|mutex
decl_stmt|;
DECL|typedef|ngx_slab_pool_t
block|}
name|ngx_slab_pool_t
typedef|;
end_typedef

begin_function_decl
name|void
name|ngx_slab_init
parameter_list|(
name|ngx_slab_pool_t
modifier|*
name|pool
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
modifier|*
name|ngx_slab_alloc
parameter_list|(
name|ngx_slab_pool_t
modifier|*
name|pool
parameter_list|,
name|size_t
name|size
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|ngx_slab_free
parameter_list|(
name|ngx_slab_pool_t
modifier|*
name|pool
parameter_list|,
name|void
modifier|*
name|p
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* _NGX_SLAB_H_INCLUDED_ */
end_comment

end_unit

