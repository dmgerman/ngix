begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (C) 2002-2004 Igor Sysoev  */
end_comment

begin_ifndef
ifndef|#
directive|ifndef
name|_NGX_ARRAY_H_INCLUDED_
end_ifndef

begin_define
DECL|macro|_NGX_ARRAY_H_INCLUDED_
define|#
directive|define
name|_NGX_ARRAY_H_INCLUDED_
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

begin_struct
DECL|struct|ngx_array_s
struct|struct
name|ngx_array_s
block|{
DECL|member|elts
name|void
modifier|*
name|elts
decl_stmt|;
DECL|member|nelts
name|ngx_uint_t
name|nelts
decl_stmt|;
DECL|member|size
name|size_t
name|size
decl_stmt|;
DECL|member|nalloc
name|ngx_uint_t
name|nalloc
decl_stmt|;
DECL|member|pool
name|ngx_pool_t
modifier|*
name|pool
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
name|ngx_array_t
modifier|*
name|ngx_create_array
parameter_list|(
name|ngx_pool_t
modifier|*
name|p
parameter_list|,
name|ngx_uint_t
name|n
parameter_list|,
name|size_t
name|size
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|ngx_destroy_array
parameter_list|(
name|ngx_array_t
modifier|*
name|a
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
modifier|*
name|ngx_push_array
parameter_list|(
name|ngx_array_t
modifier|*
name|a
parameter_list|)
function_decl|;
end_function_decl

begin_function
DECL|function|ngx_array_init (ngx_array_t * array,ngx_pool_t * pool,ngx_uint_t n,size_t size)
name|ngx_inline
specifier|static
name|ngx_int_t
name|ngx_array_init
parameter_list|(
name|ngx_array_t
modifier|*
name|array
parameter_list|,
name|ngx_pool_t
modifier|*
name|pool
parameter_list|,
name|ngx_uint_t
name|n
parameter_list|,
name|size_t
name|size
parameter_list|)
block|{
if|if
condition|(
operator|!
operator|(
name|array
operator|->
name|elts
operator|=
name|ngx_palloc
argument_list|(
name|pool
argument_list|,
name|n
operator|*
name|size
argument_list|)
operator|)
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|array
operator|->
name|nelts
operator|=
literal|0
expr_stmt|;
name|array
operator|->
name|size
operator|=
name|size
expr_stmt|;
name|array
operator|->
name|nalloc
operator|=
name|n
expr_stmt|;
name|array
operator|->
name|pool
operator|=
name|pool
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_define
DECL|macro|ngx_init_array (a,p,n,s,rc)
define|#
directive|define
name|ngx_init_array
parameter_list|(
name|a
parameter_list|,
name|p
parameter_list|,
name|n
parameter_list|,
name|s
parameter_list|,
name|rc
parameter_list|)
define|\
value|ngx_test_null(a.elts, ngx_palloc(p, n * s), rc);                         \     a.nelts = 0; a.size = s; a.nalloc = n; a.pool = p;
end_define

begin_define
DECL|macro|ngx_array_create
define|#
directive|define
name|ngx_array_create
value|ngx_create_array
end_define

begin_define
DECL|macro|ngx_array_push
define|#
directive|define
name|ngx_array_push
value|ngx_push_array
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* _NGX_ARRAY_H_INCLUDED_ */
end_comment

end_unit

