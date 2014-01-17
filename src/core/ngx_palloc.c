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

begin_function_decl
specifier|static
name|void
modifier|*
name|ngx_palloc_block
parameter_list|(
name|ngx_pool_t
modifier|*
name|pool
parameter_list|,
name|size_t
name|size
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|void
modifier|*
name|ngx_palloc_large
parameter_list|(
name|ngx_pool_t
modifier|*
name|pool
parameter_list|,
name|size_t
name|size
parameter_list|)
function_decl|;
end_function_decl

begin_function
name|ngx_pool_t
modifier|*
DECL|function|ngx_create_pool (size_t size,ngx_log_t * log)
name|ngx_create_pool
parameter_list|(
name|size_t
name|size
parameter_list|,
name|ngx_log_t
modifier|*
name|log
parameter_list|)
block|{
name|ngx_pool_t
modifier|*
name|p
decl_stmt|;
name|p
operator|=
name|ngx_memalign
argument_list|(
name|NGX_POOL_ALIGNMENT
argument_list|,
name|size
argument_list|,
name|log
argument_list|)
expr_stmt|;
if|if
condition|(
name|p
operator|==
name|NULL
condition|)
block|{
return|return
name|NULL
return|;
block|}
name|p
operator|->
name|d
operator|.
name|last
operator|=
operator|(
name|u_char
operator|*
operator|)
name|p
operator|+
sizeof|sizeof
argument_list|(
name|ngx_pool_t
argument_list|)
expr_stmt|;
name|p
operator|->
name|d
operator|.
name|end
operator|=
operator|(
name|u_char
operator|*
operator|)
name|p
operator|+
name|size
expr_stmt|;
name|p
operator|->
name|d
operator|.
name|next
operator|=
name|NULL
expr_stmt|;
name|p
operator|->
name|d
operator|.
name|failed
operator|=
literal|0
expr_stmt|;
name|size
operator|=
name|size
operator|-
sizeof|sizeof
argument_list|(
name|ngx_pool_t
argument_list|)
expr_stmt|;
name|p
operator|->
name|max
operator|=
operator|(
name|size
operator|<
name|NGX_MAX_ALLOC_FROM_POOL
operator|)
condition|?
name|size
else|:
name|NGX_MAX_ALLOC_FROM_POOL
expr_stmt|;
name|p
operator|->
name|current
operator|=
name|p
expr_stmt|;
name|p
operator|->
name|chain
operator|=
name|NULL
expr_stmt|;
name|p
operator|->
name|large
operator|=
name|NULL
expr_stmt|;
name|p
operator|->
name|cleanup
operator|=
name|NULL
expr_stmt|;
name|p
operator|->
name|log
operator|=
name|log
expr_stmt|;
return|return
name|p
return|;
block|}
end_function

begin_function
name|void
DECL|function|ngx_destroy_pool (ngx_pool_t * pool)
name|ngx_destroy_pool
parameter_list|(
name|ngx_pool_t
modifier|*
name|pool
parameter_list|)
block|{
name|ngx_pool_t
modifier|*
name|p
decl_stmt|,
modifier|*
name|n
decl_stmt|;
name|ngx_pool_large_t
modifier|*
name|l
decl_stmt|;
name|ngx_pool_cleanup_t
modifier|*
name|c
decl_stmt|;
for|for
control|(
name|c
operator|=
name|pool
operator|->
name|cleanup
init|;
name|c
condition|;
name|c
operator|=
name|c
operator|->
name|next
control|)
block|{
if|if
condition|(
name|c
operator|->
name|handler
condition|)
block|{
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_ALLOC
argument_list|,
name|pool
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"run cleanup: %p"
argument_list|,
name|c
argument_list|)
expr_stmt|;
name|c
operator|->
name|handler
argument_list|(
name|c
operator|->
name|data
argument_list|)
expr_stmt|;
block|}
block|}
for|for
control|(
name|l
operator|=
name|pool
operator|->
name|large
init|;
name|l
condition|;
name|l
operator|=
name|l
operator|->
name|next
control|)
block|{
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_ALLOC
argument_list|,
name|pool
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"free: %p"
argument_list|,
name|l
operator|->
name|alloc
argument_list|)
expr_stmt|;
if|if
condition|(
name|l
operator|->
name|alloc
condition|)
block|{
name|ngx_free
argument_list|(
name|l
operator|->
name|alloc
argument_list|)
expr_stmt|;
block|}
block|}
if|#
directive|if
operator|(
name|NGX_DEBUG
operator|)
comment|/*      * we could allocate the pool->log from this pool      * so we cannot use this log while free()ing the pool      */
for|for
control|(
name|p
operator|=
name|pool
operator|,
name|n
operator|=
name|pool
operator|->
name|d
operator|.
name|next
init|;
comment|/* void */
condition|;
name|p
operator|=
name|n
operator|,
name|n
operator|=
name|n
operator|->
name|d
operator|.
name|next
control|)
block|{
name|ngx_log_debug2
argument_list|(
name|NGX_LOG_DEBUG_ALLOC
argument_list|,
name|pool
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"free: %p, unused: %uz"
argument_list|,
name|p
argument_list|,
name|p
operator|->
name|d
operator|.
name|end
operator|-
name|p
operator|->
name|d
operator|.
name|last
argument_list|)
expr_stmt|;
if|if
condition|(
name|n
operator|==
name|NULL
condition|)
block|{
break|break;
block|}
block|}
endif|#
directive|endif
for|for
control|(
name|p
operator|=
name|pool
operator|,
name|n
operator|=
name|pool
operator|->
name|d
operator|.
name|next
init|;
comment|/* void */
condition|;
name|p
operator|=
name|n
operator|,
name|n
operator|=
name|n
operator|->
name|d
operator|.
name|next
control|)
block|{
name|ngx_free
argument_list|(
name|p
argument_list|)
expr_stmt|;
if|if
condition|(
name|n
operator|==
name|NULL
condition|)
block|{
break|break;
block|}
block|}
block|}
end_function

begin_function
name|void
DECL|function|ngx_reset_pool (ngx_pool_t * pool)
name|ngx_reset_pool
parameter_list|(
name|ngx_pool_t
modifier|*
name|pool
parameter_list|)
block|{
name|ngx_pool_t
modifier|*
name|p
decl_stmt|;
name|ngx_pool_large_t
modifier|*
name|l
decl_stmt|;
for|for
control|(
name|l
operator|=
name|pool
operator|->
name|large
init|;
name|l
condition|;
name|l
operator|=
name|l
operator|->
name|next
control|)
block|{
if|if
condition|(
name|l
operator|->
name|alloc
condition|)
block|{
name|ngx_free
argument_list|(
name|l
operator|->
name|alloc
argument_list|)
expr_stmt|;
block|}
block|}
for|for
control|(
name|p
operator|=
name|pool
init|;
name|p
condition|;
name|p
operator|=
name|p
operator|->
name|d
operator|.
name|next
control|)
block|{
name|p
operator|->
name|d
operator|.
name|last
operator|=
operator|(
name|u_char
operator|*
operator|)
name|p
operator|+
sizeof|sizeof
argument_list|(
name|ngx_pool_t
argument_list|)
expr_stmt|;
name|p
operator|->
name|d
operator|.
name|failed
operator|=
literal|0
expr_stmt|;
block|}
name|pool
operator|->
name|current
operator|=
name|pool
expr_stmt|;
name|pool
operator|->
name|chain
operator|=
name|NULL
expr_stmt|;
name|pool
operator|->
name|large
operator|=
name|NULL
expr_stmt|;
block|}
end_function

begin_function
name|void
modifier|*
DECL|function|ngx_palloc (ngx_pool_t * pool,size_t size)
name|ngx_palloc
parameter_list|(
name|ngx_pool_t
modifier|*
name|pool
parameter_list|,
name|size_t
name|size
parameter_list|)
block|{
name|u_char
modifier|*
name|m
decl_stmt|;
name|ngx_pool_t
modifier|*
name|p
decl_stmt|;
if|if
condition|(
name|size
operator|<=
name|pool
operator|->
name|max
condition|)
block|{
name|p
operator|=
name|pool
operator|->
name|current
expr_stmt|;
do|do
block|{
name|m
operator|=
name|ngx_align_ptr
argument_list|(
name|p
operator|->
name|d
operator|.
name|last
argument_list|,
name|NGX_ALIGNMENT
argument_list|)
expr_stmt|;
if|if
condition|(
operator|(
name|size_t
operator|)
operator|(
name|p
operator|->
name|d
operator|.
name|end
operator|-
name|m
operator|)
operator|>=
name|size
condition|)
block|{
name|p
operator|->
name|d
operator|.
name|last
operator|=
name|m
operator|+
name|size
expr_stmt|;
return|return
name|m
return|;
block|}
name|p
operator|=
name|p
operator|->
name|d
operator|.
name|next
expr_stmt|;
block|}
do|while
condition|(
name|p
condition|)
do|;
return|return
name|ngx_palloc_block
argument_list|(
name|pool
argument_list|,
name|size
argument_list|)
return|;
block|}
return|return
name|ngx_palloc_large
argument_list|(
name|pool
argument_list|,
name|size
argument_list|)
return|;
block|}
end_function

begin_function
name|void
modifier|*
DECL|function|ngx_pnalloc (ngx_pool_t * pool,size_t size)
name|ngx_pnalloc
parameter_list|(
name|ngx_pool_t
modifier|*
name|pool
parameter_list|,
name|size_t
name|size
parameter_list|)
block|{
name|u_char
modifier|*
name|m
decl_stmt|;
name|ngx_pool_t
modifier|*
name|p
decl_stmt|;
if|if
condition|(
name|size
operator|<=
name|pool
operator|->
name|max
condition|)
block|{
name|p
operator|=
name|pool
operator|->
name|current
expr_stmt|;
do|do
block|{
name|m
operator|=
name|p
operator|->
name|d
operator|.
name|last
expr_stmt|;
if|if
condition|(
operator|(
name|size_t
operator|)
operator|(
name|p
operator|->
name|d
operator|.
name|end
operator|-
name|m
operator|)
operator|>=
name|size
condition|)
block|{
name|p
operator|->
name|d
operator|.
name|last
operator|=
name|m
operator|+
name|size
expr_stmt|;
return|return
name|m
return|;
block|}
name|p
operator|=
name|p
operator|->
name|d
operator|.
name|next
expr_stmt|;
block|}
do|while
condition|(
name|p
condition|)
do|;
return|return
name|ngx_palloc_block
argument_list|(
name|pool
argument_list|,
name|size
argument_list|)
return|;
block|}
return|return
name|ngx_palloc_large
argument_list|(
name|pool
argument_list|,
name|size
argument_list|)
return|;
block|}
end_function

begin_function
specifier|static
name|void
modifier|*
DECL|function|ngx_palloc_block (ngx_pool_t * pool,size_t size)
name|ngx_palloc_block
parameter_list|(
name|ngx_pool_t
modifier|*
name|pool
parameter_list|,
name|size_t
name|size
parameter_list|)
block|{
name|u_char
modifier|*
name|m
decl_stmt|;
name|size_t
name|psize
decl_stmt|;
name|ngx_pool_t
modifier|*
name|p
decl_stmt|,
modifier|*
name|new
decl_stmt|,
modifier|*
name|current
decl_stmt|;
name|psize
operator|=
operator|(
name|size_t
operator|)
operator|(
name|pool
operator|->
name|d
operator|.
name|end
operator|-
operator|(
name|u_char
operator|*
operator|)
name|pool
operator|)
expr_stmt|;
name|m
operator|=
name|ngx_memalign
argument_list|(
name|NGX_POOL_ALIGNMENT
argument_list|,
name|psize
argument_list|,
name|pool
operator|->
name|log
argument_list|)
expr_stmt|;
if|if
condition|(
name|m
operator|==
name|NULL
condition|)
block|{
return|return
name|NULL
return|;
block|}
name|new
operator|=
operator|(
name|ngx_pool_t
operator|*
operator|)
name|m
expr_stmt|;
name|new
operator|->
name|d
operator|.
name|end
operator|=
name|m
operator|+
name|psize
expr_stmt|;
name|new
operator|->
name|d
operator|.
name|next
operator|=
name|NULL
expr_stmt|;
name|new
operator|->
name|d
operator|.
name|failed
operator|=
literal|0
expr_stmt|;
name|m
operator|+=
sizeof|sizeof
argument_list|(
name|ngx_pool_data_t
argument_list|)
expr_stmt|;
name|m
operator|=
name|ngx_align_ptr
argument_list|(
name|m
argument_list|,
name|NGX_ALIGNMENT
argument_list|)
expr_stmt|;
name|new
operator|->
name|d
operator|.
name|last
operator|=
name|m
operator|+
name|size
expr_stmt|;
name|current
operator|=
name|pool
operator|->
name|current
expr_stmt|;
for|for
control|(
name|p
operator|=
name|current
init|;
name|p
operator|->
name|d
operator|.
name|next
condition|;
name|p
operator|=
name|p
operator|->
name|d
operator|.
name|next
control|)
block|{
if|if
condition|(
name|p
operator|->
name|d
operator|.
name|failed
operator|++
operator|>
literal|4
condition|)
block|{
name|current
operator|=
name|p
operator|->
name|d
operator|.
name|next
expr_stmt|;
block|}
block|}
name|p
operator|->
name|d
operator|.
name|next
operator|=
name|new
expr_stmt|;
name|pool
operator|->
name|current
operator|=
name|current
condition|?
name|current
else|:
name|new
expr_stmt|;
return|return
name|m
return|;
block|}
end_function

begin_function
specifier|static
name|void
modifier|*
DECL|function|ngx_palloc_large (ngx_pool_t * pool,size_t size)
name|ngx_palloc_large
parameter_list|(
name|ngx_pool_t
modifier|*
name|pool
parameter_list|,
name|size_t
name|size
parameter_list|)
block|{
name|void
modifier|*
name|p
decl_stmt|;
name|ngx_uint_t
name|n
decl_stmt|;
name|ngx_pool_large_t
modifier|*
name|large
decl_stmt|;
name|p
operator|=
name|ngx_alloc
argument_list|(
name|size
argument_list|,
name|pool
operator|->
name|log
argument_list|)
expr_stmt|;
if|if
condition|(
name|p
operator|==
name|NULL
condition|)
block|{
return|return
name|NULL
return|;
block|}
name|n
operator|=
literal|0
expr_stmt|;
for|for
control|(
name|large
operator|=
name|pool
operator|->
name|large
init|;
name|large
condition|;
name|large
operator|=
name|large
operator|->
name|next
control|)
block|{
if|if
condition|(
name|large
operator|->
name|alloc
operator|==
name|NULL
condition|)
block|{
name|large
operator|->
name|alloc
operator|=
name|p
expr_stmt|;
return|return
name|p
return|;
block|}
if|if
condition|(
name|n
operator|++
operator|>
literal|3
condition|)
block|{
break|break;
block|}
block|}
name|large
operator|=
name|ngx_palloc
argument_list|(
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_pool_large_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|large
operator|==
name|NULL
condition|)
block|{
name|ngx_free
argument_list|(
name|p
argument_list|)
expr_stmt|;
return|return
name|NULL
return|;
block|}
name|large
operator|->
name|alloc
operator|=
name|p
expr_stmt|;
name|large
operator|->
name|next
operator|=
name|pool
operator|->
name|large
expr_stmt|;
name|pool
operator|->
name|large
operator|=
name|large
expr_stmt|;
return|return
name|p
return|;
block|}
end_function

begin_function
name|void
modifier|*
DECL|function|ngx_pmemalign (ngx_pool_t * pool,size_t size,size_t alignment)
name|ngx_pmemalign
parameter_list|(
name|ngx_pool_t
modifier|*
name|pool
parameter_list|,
name|size_t
name|size
parameter_list|,
name|size_t
name|alignment
parameter_list|)
block|{
name|void
modifier|*
name|p
decl_stmt|;
name|ngx_pool_large_t
modifier|*
name|large
decl_stmt|;
name|p
operator|=
name|ngx_memalign
argument_list|(
name|alignment
argument_list|,
name|size
argument_list|,
name|pool
operator|->
name|log
argument_list|)
expr_stmt|;
if|if
condition|(
name|p
operator|==
name|NULL
condition|)
block|{
return|return
name|NULL
return|;
block|}
name|large
operator|=
name|ngx_palloc
argument_list|(
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_pool_large_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|large
operator|==
name|NULL
condition|)
block|{
name|ngx_free
argument_list|(
name|p
argument_list|)
expr_stmt|;
return|return
name|NULL
return|;
block|}
name|large
operator|->
name|alloc
operator|=
name|p
expr_stmt|;
name|large
operator|->
name|next
operator|=
name|pool
operator|->
name|large
expr_stmt|;
name|pool
operator|->
name|large
operator|=
name|large
expr_stmt|;
return|return
name|p
return|;
block|}
end_function

begin_function
name|ngx_int_t
DECL|function|ngx_pfree (ngx_pool_t * pool,void * p)
name|ngx_pfree
parameter_list|(
name|ngx_pool_t
modifier|*
name|pool
parameter_list|,
name|void
modifier|*
name|p
parameter_list|)
block|{
name|ngx_pool_large_t
modifier|*
name|l
decl_stmt|;
for|for
control|(
name|l
operator|=
name|pool
operator|->
name|large
init|;
name|l
condition|;
name|l
operator|=
name|l
operator|->
name|next
control|)
block|{
if|if
condition|(
name|p
operator|==
name|l
operator|->
name|alloc
condition|)
block|{
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_ALLOC
argument_list|,
name|pool
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"free: %p"
argument_list|,
name|l
operator|->
name|alloc
argument_list|)
expr_stmt|;
name|ngx_free
argument_list|(
name|l
operator|->
name|alloc
argument_list|)
expr_stmt|;
name|l
operator|->
name|alloc
operator|=
name|NULL
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
block|}
return|return
name|NGX_DECLINED
return|;
block|}
end_function

begin_function
name|void
modifier|*
DECL|function|ngx_pcalloc (ngx_pool_t * pool,size_t size)
name|ngx_pcalloc
parameter_list|(
name|ngx_pool_t
modifier|*
name|pool
parameter_list|,
name|size_t
name|size
parameter_list|)
block|{
name|void
modifier|*
name|p
decl_stmt|;
name|p
operator|=
name|ngx_palloc
argument_list|(
name|pool
argument_list|,
name|size
argument_list|)
expr_stmt|;
if|if
condition|(
name|p
condition|)
block|{
name|ngx_memzero
argument_list|(
name|p
argument_list|,
name|size
argument_list|)
expr_stmt|;
block|}
return|return
name|p
return|;
block|}
end_function

begin_function
name|ngx_pool_cleanup_t
modifier|*
DECL|function|ngx_pool_cleanup_add (ngx_pool_t * p,size_t size)
name|ngx_pool_cleanup_add
parameter_list|(
name|ngx_pool_t
modifier|*
name|p
parameter_list|,
name|size_t
name|size
parameter_list|)
block|{
name|ngx_pool_cleanup_t
modifier|*
name|c
decl_stmt|;
name|c
operator|=
name|ngx_palloc
argument_list|(
name|p
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_pool_cleanup_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|c
operator|==
name|NULL
condition|)
block|{
return|return
name|NULL
return|;
block|}
if|if
condition|(
name|size
condition|)
block|{
name|c
operator|->
name|data
operator|=
name|ngx_palloc
argument_list|(
name|p
argument_list|,
name|size
argument_list|)
expr_stmt|;
if|if
condition|(
name|c
operator|->
name|data
operator|==
name|NULL
condition|)
block|{
return|return
name|NULL
return|;
block|}
block|}
else|else
block|{
name|c
operator|->
name|data
operator|=
name|NULL
expr_stmt|;
block|}
name|c
operator|->
name|handler
operator|=
name|NULL
expr_stmt|;
name|c
operator|->
name|next
operator|=
name|p
operator|->
name|cleanup
expr_stmt|;
name|p
operator|->
name|cleanup
operator|=
name|c
expr_stmt|;
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_ALLOC
argument_list|,
name|p
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"add cleanup: %p"
argument_list|,
name|c
argument_list|)
expr_stmt|;
return|return
name|c
return|;
block|}
end_function

begin_function
name|void
DECL|function|ngx_pool_run_cleanup_file (ngx_pool_t * p,ngx_fd_t fd)
name|ngx_pool_run_cleanup_file
parameter_list|(
name|ngx_pool_t
modifier|*
name|p
parameter_list|,
name|ngx_fd_t
name|fd
parameter_list|)
block|{
name|ngx_pool_cleanup_t
modifier|*
name|c
decl_stmt|;
name|ngx_pool_cleanup_file_t
modifier|*
name|cf
decl_stmt|;
for|for
control|(
name|c
operator|=
name|p
operator|->
name|cleanup
init|;
name|c
condition|;
name|c
operator|=
name|c
operator|->
name|next
control|)
block|{
if|if
condition|(
name|c
operator|->
name|handler
operator|==
name|ngx_pool_cleanup_file
condition|)
block|{
name|cf
operator|=
name|c
operator|->
name|data
expr_stmt|;
if|if
condition|(
name|cf
operator|->
name|fd
operator|==
name|fd
condition|)
block|{
name|c
operator|->
name|handler
argument_list|(
name|cf
argument_list|)
expr_stmt|;
name|c
operator|->
name|handler
operator|=
name|NULL
expr_stmt|;
return|return;
block|}
block|}
block|}
block|}
end_function

begin_function
name|void
DECL|function|ngx_pool_cleanup_file (void * data)
name|ngx_pool_cleanup_file
parameter_list|(
name|void
modifier|*
name|data
parameter_list|)
block|{
name|ngx_pool_cleanup_file_t
modifier|*
name|c
init|=
name|data
decl_stmt|;
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_ALLOC
argument_list|,
name|c
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"file cleanup: fd:%d"
argument_list|,
name|c
operator|->
name|fd
argument_list|)
expr_stmt|;
if|if
condition|(
name|ngx_close_file
argument_list|(
name|c
operator|->
name|fd
argument_list|)
operator|==
name|NGX_FILE_ERROR
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|c
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
name|ngx_close_file_n
literal|" \"%s\" failed"
argument_list|,
name|c
operator|->
name|name
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
name|void
DECL|function|ngx_pool_delete_file (void * data)
name|ngx_pool_delete_file
parameter_list|(
name|void
modifier|*
name|data
parameter_list|)
block|{
name|ngx_pool_cleanup_file_t
modifier|*
name|c
init|=
name|data
decl_stmt|;
name|ngx_err_t
name|err
decl_stmt|;
name|ngx_log_debug2
argument_list|(
name|NGX_LOG_DEBUG_ALLOC
argument_list|,
name|c
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"file cleanup: fd:%d %s"
argument_list|,
name|c
operator|->
name|fd
argument_list|,
name|c
operator|->
name|name
argument_list|)
expr_stmt|;
if|if
condition|(
name|ngx_delete_file
argument_list|(
name|c
operator|->
name|name
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
name|NGX_ENOENT
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_CRIT
argument_list|,
name|c
operator|->
name|log
argument_list|,
name|err
argument_list|,
name|ngx_delete_file_n
literal|" \"%s\" failed"
argument_list|,
name|c
operator|->
name|name
argument_list|)
expr_stmt|;
block|}
block|}
if|if
condition|(
name|ngx_close_file
argument_list|(
name|c
operator|->
name|fd
argument_list|)
operator|==
name|NGX_FILE_ERROR
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|c
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
name|ngx_close_file_n
literal|" \"%s\" failed"
argument_list|,
name|c
operator|->
name|name
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_if
if|#
directive|if
literal|0
end_if

begin_endif
unit|static void * ngx_get_cached_block(size_t size) {     void                     *p;     ngx_cached_block_slot_t  *slot;      if (ngx_cycle->cache == NULL) {         return NULL;     }      slot =&ngx_cycle->cache[(size + ngx_pagesize - 1) / ngx_pagesize];      slot->tries++;      if (slot->number) {         p = slot->block;         slot->block = slot->block->next;         slot->number--;         return p;     }      return NULL; }
endif|#
directive|endif
end_endif

end_unit

