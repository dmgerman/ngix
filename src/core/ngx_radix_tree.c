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

begin_function_decl
specifier|static
name|void
modifier|*
name|ngx_radix_alloc
parameter_list|(
name|ngx_radix_tree_t
modifier|*
name|tree
parameter_list|)
function_decl|;
end_function_decl

begin_function
name|ngx_radix_tree_t
modifier|*
DECL|function|ngx_radix_tree_create (ngx_pool_t * pool,ngx_int_t preallocate)
name|ngx_radix_tree_create
parameter_list|(
name|ngx_pool_t
modifier|*
name|pool
parameter_list|,
name|ngx_int_t
name|preallocate
parameter_list|)
block|{
name|uint32_t
name|key
decl_stmt|,
name|mask
decl_stmt|,
name|inc
decl_stmt|;
name|ngx_radix_tree_t
modifier|*
name|tree
decl_stmt|;
name|tree
operator|=
name|ngx_palloc
argument_list|(
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_radix_tree_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|tree
operator|==
name|NULL
condition|)
block|{
return|return
name|NULL
return|;
block|}
name|tree
operator|->
name|pool
operator|=
name|pool
expr_stmt|;
name|tree
operator|->
name|free
operator|=
name|NULL
expr_stmt|;
name|tree
operator|->
name|start
operator|=
name|NULL
expr_stmt|;
name|tree
operator|->
name|size
operator|=
literal|0
expr_stmt|;
name|tree
operator|->
name|root
operator|=
name|ngx_radix_alloc
argument_list|(
name|tree
argument_list|)
expr_stmt|;
if|if
condition|(
name|tree
operator|->
name|root
operator|==
name|NULL
condition|)
block|{
return|return
name|NULL
return|;
block|}
name|tree
operator|->
name|root
operator|->
name|right
operator|=
name|NULL
expr_stmt|;
name|tree
operator|->
name|root
operator|->
name|left
operator|=
name|NULL
expr_stmt|;
name|tree
operator|->
name|root
operator|->
name|parent
operator|=
name|NULL
expr_stmt|;
name|tree
operator|->
name|root
operator|->
name|value
operator|=
name|NGX_RADIX_NO_VALUE
expr_stmt|;
if|if
condition|(
name|preallocate
operator|==
literal|0
condition|)
block|{
return|return
name|tree
return|;
block|}
comment|/*      * The preallocation the first nodes: 0, 1, 00, 01, 10, 11, 000, 001, etc.      * increases the TLB hits even if for the first lookup iterations.      * On the 32-bit platforms the 7 preallocated bits takes continuous 4K,      * 8 - 8K, 9 - 16K, etc.  On the 64-bit platforms the 6 preallocated bits      * takes continuous 4K, 7 - 8K, 8 - 16K, etc.  There is no sense to      * to preallocate more than one page, because further preallocation      * distributes the only bit per page.  Instead, the random insertion      * may distribute several bits per page.      *      * Thus, by default we preallocate maximum      *     6 bits on amd64 (64-bit platform and 4K pages)      *     7 bits on i386 (32-bit platform and 4K pages)      *     7 bits on sparc64 in 64-bit mode (8K pages)      *     8 bits on sparc64 in 32-bit mode (8K pages)      */
if|if
condition|(
name|preallocate
operator|==
operator|-
literal|1
condition|)
block|{
switch|switch
condition|(
name|ngx_pagesize
operator|/
sizeof|sizeof
argument_list|(
name|ngx_radix_tree_t
argument_list|)
condition|)
block|{
comment|/* amd64 */
case|case
literal|128
case|:
name|preallocate
operator|=
literal|6
expr_stmt|;
break|break;
comment|/* i386, sparc64 */
case|case
literal|256
case|:
name|preallocate
operator|=
literal|7
expr_stmt|;
break|break;
comment|/* sparc64 in 32-bit mode */
default|default:
name|preallocate
operator|=
literal|8
expr_stmt|;
block|}
block|}
name|mask
operator|=
literal|0
expr_stmt|;
name|inc
operator|=
literal|0x80000000
expr_stmt|;
while|while
condition|(
name|preallocate
operator|--
condition|)
block|{
name|key
operator|=
literal|0
expr_stmt|;
name|mask
operator|>>=
literal|1
expr_stmt|;
name|mask
operator||=
literal|0x80000000
expr_stmt|;
do|do
block|{
if|if
condition|(
name|ngx_radix32tree_insert
argument_list|(
name|tree
argument_list|,
name|key
argument_list|,
name|mask
argument_list|,
name|NGX_RADIX_NO_VALUE
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
return|return
name|NULL
return|;
block|}
name|key
operator|+=
name|inc
expr_stmt|;
block|}
do|while
condition|(
name|key
condition|)
do|;
name|inc
operator|>>=
literal|1
expr_stmt|;
block|}
return|return
name|tree
return|;
block|}
end_function

begin_function
name|ngx_int_t
DECL|function|ngx_radix32tree_insert (ngx_radix_tree_t * tree,uint32_t key,uint32_t mask,uintptr_t value)
name|ngx_radix32tree_insert
parameter_list|(
name|ngx_radix_tree_t
modifier|*
name|tree
parameter_list|,
name|uint32_t
name|key
parameter_list|,
name|uint32_t
name|mask
parameter_list|,
name|uintptr_t
name|value
parameter_list|)
block|{
name|uint32_t
name|bit
decl_stmt|;
name|ngx_radix_node_t
modifier|*
name|node
decl_stmt|,
modifier|*
name|next
decl_stmt|;
name|bit
operator|=
literal|0x80000000
expr_stmt|;
name|node
operator|=
name|tree
operator|->
name|root
expr_stmt|;
name|next
operator|=
name|tree
operator|->
name|root
expr_stmt|;
while|while
condition|(
name|bit
operator|&
name|mask
condition|)
block|{
if|if
condition|(
name|key
operator|&
name|bit
condition|)
block|{
name|next
operator|=
name|node
operator|->
name|right
expr_stmt|;
block|}
else|else
block|{
name|next
operator|=
name|node
operator|->
name|left
expr_stmt|;
block|}
if|if
condition|(
name|next
operator|==
name|NULL
condition|)
block|{
break|break;
block|}
name|bit
operator|>>=
literal|1
expr_stmt|;
name|node
operator|=
name|next
expr_stmt|;
block|}
if|if
condition|(
name|next
condition|)
block|{
if|if
condition|(
name|node
operator|->
name|value
operator|!=
name|NGX_RADIX_NO_VALUE
condition|)
block|{
return|return
name|NGX_BUSY
return|;
block|}
name|node
operator|->
name|value
operator|=
name|value
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
while|while
condition|(
name|bit
operator|&
name|mask
condition|)
block|{
name|next
operator|=
name|ngx_radix_alloc
argument_list|(
name|tree
argument_list|)
expr_stmt|;
if|if
condition|(
name|next
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|next
operator|->
name|right
operator|=
name|NULL
expr_stmt|;
name|next
operator|->
name|left
operator|=
name|NULL
expr_stmt|;
name|next
operator|->
name|parent
operator|=
name|node
expr_stmt|;
name|next
operator|->
name|value
operator|=
name|NGX_RADIX_NO_VALUE
expr_stmt|;
if|if
condition|(
name|key
operator|&
name|bit
condition|)
block|{
name|node
operator|->
name|right
operator|=
name|next
expr_stmt|;
block|}
else|else
block|{
name|node
operator|->
name|left
operator|=
name|next
expr_stmt|;
block|}
name|bit
operator|>>=
literal|1
expr_stmt|;
name|node
operator|=
name|next
expr_stmt|;
block|}
name|node
operator|->
name|value
operator|=
name|value
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
name|ngx_int_t
DECL|function|ngx_radix32tree_delete (ngx_radix_tree_t * tree,uint32_t key,uint32_t mask)
name|ngx_radix32tree_delete
parameter_list|(
name|ngx_radix_tree_t
modifier|*
name|tree
parameter_list|,
name|uint32_t
name|key
parameter_list|,
name|uint32_t
name|mask
parameter_list|)
block|{
name|uint32_t
name|bit
decl_stmt|;
name|ngx_radix_node_t
modifier|*
name|node
decl_stmt|;
name|bit
operator|=
literal|0x80000000
expr_stmt|;
name|node
operator|=
name|tree
operator|->
name|root
expr_stmt|;
while|while
condition|(
name|node
operator|&&
operator|(
name|bit
operator|&
name|mask
operator|)
condition|)
block|{
if|if
condition|(
name|key
operator|&
name|bit
condition|)
block|{
name|node
operator|=
name|node
operator|->
name|right
expr_stmt|;
block|}
else|else
block|{
name|node
operator|=
name|node
operator|->
name|left
expr_stmt|;
block|}
name|bit
operator|>>=
literal|1
expr_stmt|;
block|}
if|if
condition|(
name|node
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|node
operator|->
name|right
operator|||
name|node
operator|->
name|left
condition|)
block|{
if|if
condition|(
name|node
operator|->
name|value
operator|!=
name|NGX_RADIX_NO_VALUE
condition|)
block|{
name|node
operator|->
name|value
operator|=
name|NGX_RADIX_NO_VALUE
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
return|return
name|NGX_ERROR
return|;
block|}
for|for
control|(
init|;
condition|;
control|)
block|{
if|if
condition|(
name|node
operator|->
name|parent
operator|->
name|right
operator|==
name|node
condition|)
block|{
name|node
operator|->
name|parent
operator|->
name|right
operator|=
name|NULL
expr_stmt|;
block|}
else|else
block|{
name|node
operator|->
name|parent
operator|->
name|left
operator|=
name|NULL
expr_stmt|;
block|}
name|node
operator|->
name|right
operator|=
name|tree
operator|->
name|free
expr_stmt|;
name|tree
operator|->
name|free
operator|=
name|node
expr_stmt|;
name|node
operator|=
name|node
operator|->
name|parent
expr_stmt|;
if|if
condition|(
name|node
operator|->
name|right
operator|||
name|node
operator|->
name|left
condition|)
block|{
break|break;
block|}
if|if
condition|(
name|node
operator|->
name|value
operator|!=
name|NGX_RADIX_NO_VALUE
condition|)
block|{
break|break;
block|}
if|if
condition|(
name|node
operator|->
name|parent
operator|==
name|NULL
condition|)
block|{
break|break;
block|}
block|}
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
name|uintptr_t
DECL|function|ngx_radix32tree_find (ngx_radix_tree_t * tree,uint32_t key)
name|ngx_radix32tree_find
parameter_list|(
name|ngx_radix_tree_t
modifier|*
name|tree
parameter_list|,
name|uint32_t
name|key
parameter_list|)
block|{
name|uint32_t
name|bit
decl_stmt|;
name|uintptr_t
name|value
decl_stmt|;
name|ngx_radix_node_t
modifier|*
name|node
decl_stmt|;
name|bit
operator|=
literal|0x80000000
expr_stmt|;
name|value
operator|=
name|NGX_RADIX_NO_VALUE
expr_stmt|;
name|node
operator|=
name|tree
operator|->
name|root
expr_stmt|;
while|while
condition|(
name|node
condition|)
block|{
if|if
condition|(
name|node
operator|->
name|value
operator|!=
name|NGX_RADIX_NO_VALUE
condition|)
block|{
name|value
operator|=
name|node
operator|->
name|value
expr_stmt|;
block|}
if|if
condition|(
name|key
operator|&
name|bit
condition|)
block|{
name|node
operator|=
name|node
operator|->
name|right
expr_stmt|;
block|}
else|else
block|{
name|node
operator|=
name|node
operator|->
name|left
expr_stmt|;
block|}
name|bit
operator|>>=
literal|1
expr_stmt|;
block|}
return|return
name|value
return|;
block|}
end_function

begin_function
specifier|static
name|void
modifier|*
DECL|function|ngx_radix_alloc (ngx_radix_tree_t * tree)
name|ngx_radix_alloc
parameter_list|(
name|ngx_radix_tree_t
modifier|*
name|tree
parameter_list|)
block|{
name|char
modifier|*
name|p
decl_stmt|;
if|if
condition|(
name|tree
operator|->
name|free
condition|)
block|{
name|p
operator|=
operator|(
name|char
operator|*
operator|)
name|tree
operator|->
name|free
expr_stmt|;
name|tree
operator|->
name|free
operator|=
name|tree
operator|->
name|free
operator|->
name|right
expr_stmt|;
return|return
name|p
return|;
block|}
if|if
condition|(
name|tree
operator|->
name|size
operator|<
sizeof|sizeof
argument_list|(
name|ngx_radix_node_t
argument_list|)
condition|)
block|{
name|tree
operator|->
name|start
operator|=
name|ngx_pmemalign
argument_list|(
name|tree
operator|->
name|pool
argument_list|,
name|ngx_pagesize
argument_list|,
name|ngx_pagesize
argument_list|)
expr_stmt|;
if|if
condition|(
name|tree
operator|->
name|start
operator|==
name|NULL
condition|)
block|{
return|return
name|NULL
return|;
block|}
name|tree
operator|->
name|size
operator|=
name|ngx_pagesize
expr_stmt|;
block|}
name|p
operator|=
name|tree
operator|->
name|start
expr_stmt|;
name|tree
operator|->
name|start
operator|+=
sizeof|sizeof
argument_list|(
name|ngx_radix_node_t
argument_list|)
expr_stmt|;
name|tree
operator|->
name|size
operator|-=
sizeof|sizeof
argument_list|(
name|ngx_radix_node_t
argument_list|)
expr_stmt|;
return|return
name|p
return|;
block|}
end_function

end_unit

