begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|_NGX_RBTREE_H_INCLUDED_
end_ifndef

begin_define
DECL|macro|_NGX_RBTREE_H_INCLUDED_
define|#
directive|define
name|_NGX_RBTREE_H_INCLUDED_
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
DECL|typedef|ngx_rbtree_t
typedef|typedef
name|struct
name|ngx_rbtree_s
name|ngx_rbtree_t
typedef|;
end_typedef

begin_struct
DECL|struct|ngx_rbtree_s
struct|struct
name|ngx_rbtree_s
block|{
DECL|member|key
name|ngx_int_t
name|key
decl_stmt|;
DECL|member|left
name|ngx_rbtree_t
modifier|*
name|left
decl_stmt|;
DECL|member|right
name|ngx_rbtree_t
modifier|*
name|right
decl_stmt|;
DECL|member|parent
name|ngx_rbtree_t
modifier|*
name|parent
decl_stmt|;
DECL|member|data
name|void
modifier|*
name|data
decl_stmt|;
block|}
struct|;
end_struct

begin_decl_stmt
specifier|extern
name|ngx_rbtree_t
name|sentinel
decl_stmt|;
end_decl_stmt

begin_function_decl
name|void
name|ngx_rbtree_insert
parameter_list|(
name|ngx_rbtree_t
modifier|*
modifier|*
name|root
parameter_list|,
name|ngx_rbtree_t
modifier|*
name|node
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|ngx_rbtree_delete
parameter_list|(
name|ngx_rbtree_t
modifier|*
modifier|*
name|root
parameter_list|,
name|ngx_rbtree_t
modifier|*
name|node
parameter_list|)
function_decl|;
end_function_decl

begin_function
DECL|function|ngx_rbtree_min (ngx_rbtree_t * root)
name|ngx_inline
specifier|static
name|ngx_rbtree_t
modifier|*
name|ngx_rbtree_min
parameter_list|(
name|ngx_rbtree_t
modifier|*
name|root
parameter_list|)
block|{
while|while
condition|(
name|root
operator|->
name|left
operator|!=
operator|&
name|sentinel
condition|)
block|{
name|root
operator|=
name|root
operator|->
name|left
expr_stmt|;
block|}
return|return
name|root
return|;
block|}
end_function

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* _NGX_RBTREE_H_INCLUDED_ */
end_comment

end_unit

