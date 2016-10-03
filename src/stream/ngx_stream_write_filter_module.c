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

begin_include
include|#
directive|include
file|<ngx_stream.h>
end_include

begin_typedef
DECL|struct|__anon2b2e52640108
typedef|typedef
struct|struct
block|{
DECL|member|from_upstream
name|ngx_chain_t
modifier|*
name|from_upstream
decl_stmt|;
DECL|member|from_downstream
name|ngx_chain_t
modifier|*
name|from_downstream
decl_stmt|;
DECL|typedef|ngx_stream_write_filter_ctx_t
block|}
name|ngx_stream_write_filter_ctx_t
typedef|;
end_typedef

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_stream_write_filter
parameter_list|(
name|ngx_stream_session_t
modifier|*
name|s
parameter_list|,
name|ngx_chain_t
modifier|*
name|in
parameter_list|,
name|ngx_uint_t
name|from_upstream
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_stream_write_filter_init
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
function_decl|;
end_function_decl

begin_decl_stmt
DECL|variable|ngx_stream_write_filter_module_ctx
specifier|static
name|ngx_stream_module_t
name|ngx_stream_write_filter_module_ctx
init|=
block|{
name|NULL
block|,
comment|/* preconfiguration */
name|ngx_stream_write_filter_init
block|,
comment|/* postconfiguration */
name|NULL
block|,
comment|/* create main configuration */
name|NULL
block|,
comment|/* init main configuration */
name|NULL
block|,
comment|/* create server configuration */
name|NULL
comment|/* merge server configuration */
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_stream_write_filter_module
name|ngx_module_t
name|ngx_stream_write_filter_module
init|=
block|{
name|NGX_MODULE_V1
block|,
operator|&
name|ngx_stream_write_filter_module_ctx
block|,
comment|/* module context */
name|NULL
block|,
comment|/* module directives */
name|NGX_STREAM_MODULE
block|,
comment|/* module type */
name|NULL
block|,
comment|/* init master */
name|NULL
block|,
comment|/* init module */
name|NULL
block|,
comment|/* init process */
name|NULL
block|,
comment|/* init thread */
name|NULL
block|,
comment|/* exit thread */
name|NULL
block|,
comment|/* exit process */
name|NULL
block|,
comment|/* exit master */
name|NGX_MODULE_V1_PADDING
block|}
decl_stmt|;
end_decl_stmt

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_stream_write_filter (ngx_stream_session_t * s,ngx_chain_t * in,ngx_uint_t from_upstream)
name|ngx_stream_write_filter
parameter_list|(
name|ngx_stream_session_t
modifier|*
name|s
parameter_list|,
name|ngx_chain_t
modifier|*
name|in
parameter_list|,
name|ngx_uint_t
name|from_upstream
parameter_list|)
block|{
name|off_t
name|size
decl_stmt|;
name|ngx_uint_t
name|last
decl_stmt|,
name|flush
decl_stmt|,
name|sync
decl_stmt|;
name|ngx_chain_t
modifier|*
name|cl
decl_stmt|,
modifier|*
name|ln
decl_stmt|,
modifier|*
modifier|*
name|ll
decl_stmt|,
modifier|*
modifier|*
name|out
decl_stmt|,
modifier|*
name|chain
decl_stmt|;
name|ngx_connection_t
modifier|*
name|c
decl_stmt|;
name|ngx_stream_write_filter_ctx_t
modifier|*
name|ctx
decl_stmt|;
name|ctx
operator|=
name|ngx_stream_get_module_ctx
argument_list|(
name|s
argument_list|,
name|ngx_stream_write_filter_module
argument_list|)
expr_stmt|;
if|if
condition|(
name|ctx
operator|==
name|NULL
condition|)
block|{
name|ctx
operator|=
name|ngx_pcalloc
argument_list|(
name|s
operator|->
name|connection
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_stream_write_filter_ctx_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|ctx
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|ngx_stream_set_ctx
argument_list|(
name|s
argument_list|,
name|ctx
argument_list|,
name|ngx_stream_write_filter_module
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|from_upstream
condition|)
block|{
name|c
operator|=
name|s
operator|->
name|connection
expr_stmt|;
name|out
operator|=
operator|&
name|ctx
operator|->
name|from_upstream
expr_stmt|;
block|}
else|else
block|{
name|c
operator|=
name|s
operator|->
name|upstream
operator|->
name|peer
operator|.
name|connection
expr_stmt|;
name|out
operator|=
operator|&
name|ctx
operator|->
name|from_downstream
expr_stmt|;
block|}
if|if
condition|(
name|c
operator|->
name|error
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|size
operator|=
literal|0
expr_stmt|;
name|flush
operator|=
literal|0
expr_stmt|;
name|sync
operator|=
literal|0
expr_stmt|;
name|last
operator|=
literal|0
expr_stmt|;
name|ll
operator|=
name|out
expr_stmt|;
comment|/* find the size, the flush point and the last link of the saved chain */
for|for
control|(
name|cl
operator|=
operator|*
name|out
init|;
name|cl
condition|;
name|cl
operator|=
name|cl
operator|->
name|next
control|)
block|{
name|ll
operator|=
operator|&
name|cl
operator|->
name|next
expr_stmt|;
name|ngx_log_debug7
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|c
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"write old buf t:%d f:%d %p, pos %p, size: %z "
literal|"file: %O, size: %O"
argument_list|,
name|cl
operator|->
name|buf
operator|->
name|temporary
argument_list|,
name|cl
operator|->
name|buf
operator|->
name|in_file
argument_list|,
name|cl
operator|->
name|buf
operator|->
name|start
argument_list|,
name|cl
operator|->
name|buf
operator|->
name|pos
argument_list|,
name|cl
operator|->
name|buf
operator|->
name|last
operator|-
name|cl
operator|->
name|buf
operator|->
name|pos
argument_list|,
name|cl
operator|->
name|buf
operator|->
name|file_pos
argument_list|,
name|cl
operator|->
name|buf
operator|->
name|file_last
operator|-
name|cl
operator|->
name|buf
operator|->
name|file_pos
argument_list|)
expr_stmt|;
if|#
directive|if
literal|1
if|if
condition|(
name|ngx_buf_size
argument_list|(
name|cl
operator|->
name|buf
argument_list|)
operator|==
literal|0
operator|&&
operator|!
name|ngx_buf_special
argument_list|(
name|cl
operator|->
name|buf
argument_list|)
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
literal|0
argument_list|,
literal|"zero size buf in writer "
literal|"t:%d r:%d f:%d %p %p-%p %p %O-%O"
argument_list|,
name|cl
operator|->
name|buf
operator|->
name|temporary
argument_list|,
name|cl
operator|->
name|buf
operator|->
name|recycled
argument_list|,
name|cl
operator|->
name|buf
operator|->
name|in_file
argument_list|,
name|cl
operator|->
name|buf
operator|->
name|start
argument_list|,
name|cl
operator|->
name|buf
operator|->
name|pos
argument_list|,
name|cl
operator|->
name|buf
operator|->
name|last
argument_list|,
name|cl
operator|->
name|buf
operator|->
name|file
argument_list|,
name|cl
operator|->
name|buf
operator|->
name|file_pos
argument_list|,
name|cl
operator|->
name|buf
operator|->
name|file_last
argument_list|)
expr_stmt|;
name|ngx_debug_point
argument_list|()
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
endif|#
directive|endif
name|size
operator|+=
name|ngx_buf_size
argument_list|(
name|cl
operator|->
name|buf
argument_list|)
expr_stmt|;
if|if
condition|(
name|cl
operator|->
name|buf
operator|->
name|flush
operator|||
name|cl
operator|->
name|buf
operator|->
name|recycled
condition|)
block|{
name|flush
operator|=
literal|1
expr_stmt|;
block|}
if|if
condition|(
name|cl
operator|->
name|buf
operator|->
name|sync
condition|)
block|{
name|sync
operator|=
literal|1
expr_stmt|;
block|}
if|if
condition|(
name|cl
operator|->
name|buf
operator|->
name|last_buf
condition|)
block|{
name|last
operator|=
literal|1
expr_stmt|;
block|}
block|}
comment|/* add the new chain to the existent one */
for|for
control|(
name|ln
operator|=
name|in
init|;
name|ln
condition|;
name|ln
operator|=
name|ln
operator|->
name|next
control|)
block|{
name|cl
operator|=
name|ngx_alloc_chain_link
argument_list|(
name|c
operator|->
name|pool
argument_list|)
expr_stmt|;
if|if
condition|(
name|cl
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|cl
operator|->
name|buf
operator|=
name|ln
operator|->
name|buf
expr_stmt|;
operator|*
name|ll
operator|=
name|cl
expr_stmt|;
name|ll
operator|=
operator|&
name|cl
operator|->
name|next
expr_stmt|;
name|ngx_log_debug7
argument_list|(
name|NGX_LOG_DEBUG_EVENT
argument_list|,
name|c
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"write new buf t:%d f:%d %p, pos %p, size: %z "
literal|"file: %O, size: %O"
argument_list|,
name|cl
operator|->
name|buf
operator|->
name|temporary
argument_list|,
name|cl
operator|->
name|buf
operator|->
name|in_file
argument_list|,
name|cl
operator|->
name|buf
operator|->
name|start
argument_list|,
name|cl
operator|->
name|buf
operator|->
name|pos
argument_list|,
name|cl
operator|->
name|buf
operator|->
name|last
operator|-
name|cl
operator|->
name|buf
operator|->
name|pos
argument_list|,
name|cl
operator|->
name|buf
operator|->
name|file_pos
argument_list|,
name|cl
operator|->
name|buf
operator|->
name|file_last
operator|-
name|cl
operator|->
name|buf
operator|->
name|file_pos
argument_list|)
expr_stmt|;
if|#
directive|if
literal|1
if|if
condition|(
name|ngx_buf_size
argument_list|(
name|cl
operator|->
name|buf
argument_list|)
operator|==
literal|0
operator|&&
operator|!
name|ngx_buf_special
argument_list|(
name|cl
operator|->
name|buf
argument_list|)
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
literal|0
argument_list|,
literal|"zero size buf in writer "
literal|"t:%d r:%d f:%d %p %p-%p %p %O-%O"
argument_list|,
name|cl
operator|->
name|buf
operator|->
name|temporary
argument_list|,
name|cl
operator|->
name|buf
operator|->
name|recycled
argument_list|,
name|cl
operator|->
name|buf
operator|->
name|in_file
argument_list|,
name|cl
operator|->
name|buf
operator|->
name|start
argument_list|,
name|cl
operator|->
name|buf
operator|->
name|pos
argument_list|,
name|cl
operator|->
name|buf
operator|->
name|last
argument_list|,
name|cl
operator|->
name|buf
operator|->
name|file
argument_list|,
name|cl
operator|->
name|buf
operator|->
name|file_pos
argument_list|,
name|cl
operator|->
name|buf
operator|->
name|file_last
argument_list|)
expr_stmt|;
name|ngx_debug_point
argument_list|()
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
endif|#
directive|endif
name|size
operator|+=
name|ngx_buf_size
argument_list|(
name|cl
operator|->
name|buf
argument_list|)
expr_stmt|;
if|if
condition|(
name|cl
operator|->
name|buf
operator|->
name|flush
operator|||
name|cl
operator|->
name|buf
operator|->
name|recycled
condition|)
block|{
name|flush
operator|=
literal|1
expr_stmt|;
block|}
if|if
condition|(
name|cl
operator|->
name|buf
operator|->
name|sync
condition|)
block|{
name|sync
operator|=
literal|1
expr_stmt|;
block|}
if|if
condition|(
name|cl
operator|->
name|buf
operator|->
name|last_buf
condition|)
block|{
name|last
operator|=
literal|1
expr_stmt|;
block|}
block|}
operator|*
name|ll
operator|=
name|NULL
expr_stmt|;
name|ngx_log_debug3
argument_list|(
name|NGX_LOG_DEBUG_STREAM
argument_list|,
name|c
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"stream write filter: l:%ui f:%ui s:%O"
argument_list|,
name|last
argument_list|,
name|flush
argument_list|,
name|size
argument_list|)
expr_stmt|;
if|if
condition|(
name|size
operator|==
literal|0
operator|&&
operator|!
operator|(
name|c
operator|->
name|buffered
operator|&
name|NGX_LOWLEVEL_BUFFERED
operator|)
operator|&&
operator|!
operator|(
name|last
operator|&&
name|c
operator|->
name|need_last_buf
operator|)
condition|)
block|{
if|if
condition|(
name|last
operator|||
name|flush
operator|||
name|sync
condition|)
block|{
for|for
control|(
name|cl
operator|=
operator|*
name|out
init|;
name|cl
condition|;
comment|/* void */
control|)
block|{
name|ln
operator|=
name|cl
expr_stmt|;
name|cl
operator|=
name|cl
operator|->
name|next
expr_stmt|;
name|ngx_free_chain
argument_list|(
name|c
operator|->
name|pool
argument_list|,
name|ln
argument_list|)
expr_stmt|;
block|}
operator|*
name|out
operator|=
name|NULL
expr_stmt|;
name|c
operator|->
name|buffered
operator|&=
operator|~
name|NGX_STREAM_WRITE_BUFFERED
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|c
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"the stream output chain is empty"
argument_list|)
expr_stmt|;
name|ngx_debug_point
argument_list|()
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
name|chain
operator|=
name|c
operator|->
name|send_chain
argument_list|(
name|c
argument_list|,
operator|*
name|out
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_STREAM
argument_list|,
name|c
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"stream write filter %p"
argument_list|,
name|chain
argument_list|)
expr_stmt|;
if|if
condition|(
name|chain
operator|==
name|NGX_CHAIN_ERROR
condition|)
block|{
name|c
operator|->
name|error
operator|=
literal|1
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
for|for
control|(
name|cl
operator|=
operator|*
name|out
init|;
name|cl
operator|&&
name|cl
operator|!=
name|chain
condition|;
comment|/* void */
control|)
block|{
name|ln
operator|=
name|cl
expr_stmt|;
name|cl
operator|=
name|cl
operator|->
name|next
expr_stmt|;
name|ngx_free_chain
argument_list|(
name|c
operator|->
name|pool
argument_list|,
name|ln
argument_list|)
expr_stmt|;
block|}
operator|*
name|out
operator|=
name|chain
expr_stmt|;
if|if
condition|(
name|chain
condition|)
block|{
if|if
condition|(
name|c
operator|->
name|shared
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
literal|0
argument_list|,
literal|"shared connection is busy"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
name|c
operator|->
name|buffered
operator||=
name|NGX_STREAM_WRITE_BUFFERED
expr_stmt|;
return|return
name|NGX_AGAIN
return|;
block|}
name|c
operator|->
name|buffered
operator|&=
operator|~
name|NGX_STREAM_WRITE_BUFFERED
expr_stmt|;
if|if
condition|(
name|c
operator|->
name|buffered
operator|&
name|NGX_LOWLEVEL_BUFFERED
condition|)
block|{
return|return
name|NGX_AGAIN
return|;
block|}
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_stream_write_filter_init (ngx_conf_t * cf)
name|ngx_stream_write_filter_init
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
block|{
name|ngx_stream_top_filter
operator|=
name|ngx_stream_write_filter
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

end_unit
