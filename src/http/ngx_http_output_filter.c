begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_include
include|#
directive|include
file|<ngx_core.h>
end_include

begin_include
include|#
directive|include
file|<ngx_files.h>
end_include

begin_include
include|#
directive|include
file|<ngx_string.h>
end_include

begin_include
include|#
directive|include
file|<ngx_hunk.h>
end_include

begin_include
include|#
directive|include
file|<ngx_config_command.h>
end_include

begin_include
include|#
directive|include
file|<ngx_http.h>
end_include

begin_include
include|#
directive|include
file|<ngx_http_config.h>
end_include

begin_include
include|#
directive|include
file|<ngx_http_output_filter.h>
end_include

begin_function_decl
specifier|static
name|int
name|ngx_http_output_filter_copy_hunk
parameter_list|(
name|ngx_hunk_t
modifier|*
name|dst
parameter_list|,
name|ngx_hunk_t
modifier|*
name|src
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|int
name|ngx_http_output_filter_init
parameter_list|(
name|int
function_decl|(
modifier|*
modifier|*
name|next_filter
function_decl|)
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_chain_t
modifier|*
name|ch
parameter_list|)
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|void
modifier|*
name|ngx_http_output_filter_create_conf
parameter_list|(
name|ngx_pool_t
modifier|*
name|pool
parameter_list|)
function_decl|;
end_function_decl

begin_decl_stmt
DECL|variable|ngx_http_output_filter_commands
specifier|static
name|ngx_command_t
name|ngx_http_output_filter_commands
index|[]
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_output_filter_module
name|ngx_http_module_t
name|ngx_http_output_filter_module
init|=
block|{
name|NGX_HTTP_MODULE
block|,
name|NULL
block|,
comment|/* create server config */
name|ngx_http_output_filter_create_conf
block|,
comment|/* create location config */
name|ngx_http_output_filter_commands
block|,
comment|/* module directives */
name|NULL
block|,
comment|/* init module */
name|ngx_http_output_filter_init
comment|/* init output body filter */
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_output_filter_commands
specifier|static
name|ngx_command_t
name|ngx_http_output_filter_commands
index|[]
init|=
block|{
block|{
literal|"output_buffer"
block|,
name|ngx_conf_set_size_slot
block|,
name|offsetof
argument_list|(
name|ngx_http_output_filter_conf_t
argument_list|,
name|hunk_size
argument_list|)
block|,
name|NGX_HTTP_LOC_CONF
block|,
name|NGX_CONF_TAKE1
block|,
literal|"set output filter buffer size"
block|}
block|,
block|{
name|NULL
block|}
block|}
decl_stmt|;
end_decl_stmt

begin_function_decl
DECL|variable|ngx_http_output_next_filter
specifier|static
name|int
function_decl|(
modifier|*
name|ngx_http_output_next_filter
function_decl|)
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_chain_t
modifier|*
name|ch
parameter_list|)
function_decl|;
end_function_decl

begin_function
DECL|function|ngx_http_output_filter (ngx_http_request_t * r,ngx_hunk_t * hunk)
name|int
name|ngx_http_output_filter
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_hunk_t
modifier|*
name|hunk
parameter_list|)
block|{
name|int
name|rc
decl_stmt|,
name|once
decl_stmt|;
name|size_t
name|size
decl_stmt|;
name|ssize_t
name|n
decl_stmt|;
name|ngx_chain_t
modifier|*
name|ce
decl_stmt|;
name|ngx_http_output_filter_ctx_t
modifier|*
name|ctx
decl_stmt|;
name|ngx_http_output_filter_conf_t
modifier|*
name|conf
decl_stmt|;
name|ctx
operator|=
operator|(
name|ngx_http_output_filter_ctx_t
operator|*
operator|)
name|ngx_get_module_ctx
argument_list|(
name|r
operator|->
expr|main
operator|?
name|r
operator|->
expr|main
operator|:
name|r
argument_list|,
name|ngx_http_output_filter_module
argument_list|)
expr_stmt|;
if|if
condition|(
name|ctx
operator|==
name|NULL
condition|)
block|{
name|ngx_http_create_ctx
argument_list|(
name|r
argument_list|,
name|ctx
argument_list|,
name|ngx_http_output_filter_module
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_output_filter_ctx_t
argument_list|)
argument_list|)
expr_stmt|;
name|ctx
operator|->
name|next_filter
operator|=
name|ngx_http_output_next_filter
expr_stmt|;
block|}
name|ngx_log_debug
argument_list|(
argument|r->connection->log
argument_list|,
literal|"HUNK: x%x CTX-IN: x%x CTX->HUNK: x%x"
argument|_                   hunk _ ctx->in _ ctx->hunk
argument_list|)
empty_stmt|;
if|if
condition|(
name|hunk
operator|&&
operator|(
name|hunk
operator|->
name|type
operator|&
name|NGX_HUNK_LAST
operator|)
condition|)
name|ctx
operator|->
name|last
operator|=
literal|1
expr_stmt|;
for|for
control|(
name|once
operator|=
literal|1
init|;
name|once
operator|||
name|ctx
operator|->
name|in
condition|;
name|once
operator|=
literal|0
control|)
block|{
comment|/* input chain is not empty */
if|if
condition|(
name|ctx
operator|->
name|in
condition|)
block|{
comment|/* add hunk to input chain */
if|if
condition|(
name|once
operator|&&
name|hunk
condition|)
block|{
for|for
control|(
name|ce
operator|=
name|ctx
operator|->
name|in
init|;
name|ce
operator|->
name|next
condition|;
name|ce
operator|=
name|ce
operator|->
name|next
control|)
comment|/* void */
empty_stmt|;
name|ngx_add_hunk_to_chain
argument_list|(
name|ce
operator|->
name|next
argument_list|,
name|hunk
argument_list|,
name|r
operator|->
name|pool
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
block|}
comment|/* our hunk is still busy */
if|if
condition|(
name|ctx
operator|->
name|hunk
operator|->
name|pos
operator|.
name|mem
operator|<
name|ctx
operator|->
name|hunk
operator|->
name|last
operator|.
name|mem
condition|)
block|{
name|rc
operator|=
name|ctx
operator|->
name|next_filter
argument_list|(
name|r
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
comment|/* our hunk is free */
block|}
else|else
block|{
name|ctx
operator|->
name|out
operator|.
name|hunk
operator|=
name|ctx
operator|->
name|hunk
expr_stmt|;
name|rc
operator|=
name|ngx_http_output_filter_copy_hunk
argument_list|(
name|ctx
operator|->
name|hunk
argument_list|,
name|ctx
operator|->
name|in
operator|->
name|hunk
argument_list|)
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_FILE_AIO
operator|)
if|if
condition|(
name|rc
operator|==
name|NGX_AGAIN
condition|)
return|return
name|rc
return|;
endif|#
directive|endif
if|if
condition|(
name|rc
operator|==
name|NGX_ERROR
condition|)
return|return
name|rc
return|;
comment|/* whole hunk is copied so we send to next filter chain part                    up to next hunk that need to be copied */
if|if
condition|(
name|ctx
operator|->
name|in
operator|->
name|hunk
operator|->
name|pos
operator|.
name|mem
operator|==
name|ctx
operator|->
name|in
operator|->
name|hunk
operator|->
name|last
operator|.
name|mem
condition|)
block|{
name|ctx
operator|->
name|out
operator|.
name|next
operator|=
name|ctx
operator|->
name|in
operator|->
name|next
expr_stmt|;
for|for
control|(
name|ce
operator|=
name|ctx
operator|->
name|in
operator|->
name|next
init|;
name|ce
condition|;
name|ce
operator|=
name|ce
operator|->
name|next
control|)
block|{
if|if
condition|(
name|ce
operator|->
name|hunk
operator|->
name|type
operator|&
name|NGX_HUNK_FILE
condition|)
break|break;
if|if
condition|(
operator|(
name|ce
operator|->
name|hunk
operator|->
name|type
operator|&
name|NGX_HUNK_MEMORY
operator||
name|NGX_HUNK_MMAP
operator|)
operator|&&
operator|(
name|r
operator|->
name|filter
operator|&
name|NGX_HTTP_FILTER_NEED_TEMP
operator|)
condition|)
break|break;
block|}
name|ctx
operator|->
name|out
operator|.
name|next
operator|=
name|ce
expr_stmt|;
block|}
else|else
block|{
name|ctx
operator|->
name|out
operator|.
name|next
operator|=
name|NULL
expr_stmt|;
block|}
name|rc
operator|=
name|ctx
operator|->
name|next_filter
argument_list|(
name|r
argument_list|,
operator|&
name|ctx
operator|->
name|out
argument_list|)
expr_stmt|;
block|}
comment|/* delete completed hunks from input chain */
for|for
control|(
name|ce
operator|=
name|ctx
operator|->
name|in
init|;
name|ce
condition|;
name|ce
operator|=
name|ce
operator|->
name|next
control|)
block|{
if|if
condition|(
name|ce
operator|->
name|hunk
operator|->
name|pos
operator|.
name|file
operator|==
name|ce
operator|->
name|hunk
operator|->
name|last
operator|.
name|file
condition|)
name|ctx
operator|->
name|in
operator|=
name|ce
operator|->
name|next
expr_stmt|;
block|}
if|if
condition|(
name|rc
operator|==
name|NGX_OK
condition|)
name|ctx
operator|->
name|hunk
operator|->
name|pos
operator|.
name|mem
operator|=
name|ctx
operator|->
name|hunk
operator|->
name|last
operator|.
name|mem
operator|=
name|ctx
operator|->
name|hunk
operator|->
name|start
expr_stmt|;
else|else
return|return
name|rc
return|;
comment|/* input chain is empty */
block|}
else|else
block|{
if|if
condition|(
name|hunk
operator|==
name|NULL
condition|)
block|{
name|rc
operator|=
name|ctx
operator|->
name|next_filter
argument_list|(
name|r
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
block|}
else|else
block|{
comment|/* we need to copy hunk to our hunk */
if|if
condition|(
operator|(
operator|(
name|r
operator|->
name|filter
operator|&
name|NGX_HTTP_FILTER_NEED_IN_MEMORY
operator|)
operator|&&
operator|(
name|hunk
operator|->
name|type
operator|&
name|NGX_HUNK_FILE
operator|)
operator|)
operator|||
operator|(
operator|(
name|r
operator|->
name|filter
operator|&
name|NGX_HTTP_FILTER_NEED_TEMP
operator|)
operator|&&
operator|(
name|hunk
operator|->
name|type
operator|&
name|NGX_HUNK_MEMORY
operator||
name|NGX_HUNK_MMAP
operator|)
operator|)
condition|)
block|{
comment|/* out hunk is still busy */
if|if
condition|(
name|ctx
operator|->
name|hunk
operator|&&
name|ctx
operator|->
name|hunk
operator|->
name|pos
operator|.
name|mem
operator|<
name|ctx
operator|->
name|hunk
operator|->
name|last
operator|.
name|mem
condition|)
block|{
name|ngx_add_hunk_to_chain
argument_list|(
name|ctx
operator|->
name|in
argument_list|,
name|hunk
argument_list|,
name|r
operator|->
name|pool
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
name|rc
operator|=
name|ctx
operator|->
name|next_filter
argument_list|(
name|r
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
block|}
else|else
block|{
if|if
condition|(
name|ctx
operator|->
name|hunk
operator|==
name|NULL
condition|)
block|{
if|if
condition|(
name|hunk
operator|->
name|type
operator|&
name|NGX_HUNK_LAST
condition|)
block|{
name|conf
operator|=
operator|(
name|ngx_http_output_filter_conf_t
operator|*
operator|)
name|ngx_get_module_loc_conf
argument_list|(
name|r
operator|->
expr|main
operator|?
name|r
operator|->
expr|main
operator|:
name|r
argument_list|,
name|ngx_http_output_filter_module
argument_list|)
expr_stmt|;
name|size
operator|=
name|hunk
operator|->
name|last
operator|.
name|mem
operator|-
name|hunk
operator|->
name|pos
operator|.
name|mem
expr_stmt|;
if|if
condition|(
name|size
operator|>
name|conf
operator|->
name|hunk_size
condition|)
name|size
operator|=
name|conf
operator|->
name|hunk_size
expr_stmt|;
block|}
else|else
block|{
name|size
operator|=
name|conf
operator|->
name|hunk_size
expr_stmt|;
block|}
name|ngx_test_null
argument_list|(
name|ctx
operator|->
name|hunk
argument_list|,
name|ngx_create_temp_hunk
argument_list|(
name|r
operator|->
name|pool
argument_list|,
name|size
argument_list|,
literal|50
argument_list|,
literal|50
argument_list|)
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
name|ctx
operator|->
name|hunk
operator|->
name|type
operator||=
name|NGX_HUNK_RECYCLED
expr_stmt|;
name|rc
operator|=
name|ngx_http_output_filter_copy_hunk
argument_list|(
name|ctx
operator|->
name|hunk
argument_list|,
name|hunk
argument_list|)
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_FILE_AIO
operator|)
if|if
condition|(
name|rc
operator|==
name|NGX_AGAIN
condition|)
block|{
comment|/* add hunk to input chain */
name|ngx_add_hunk_to_chain
argument_list|(
name|ctx
operator|->
name|in
argument_list|,
name|hunk
argument_list|,
name|r
operator|->
name|pool
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
return|return
name|rc
return|;
block|}
endif|#
directive|endif
if|if
condition|(
name|rc
operator|==
name|NGX_ERROR
condition|)
return|return
name|rc
return|;
if|if
condition|(
name|hunk
operator|->
name|pos
operator|.
name|mem
operator|<
name|hunk
operator|->
name|last
operator|.
name|mem
condition|)
name|ngx_add_hunk_to_chain
argument_list|(
name|ctx
operator|->
name|in
argument_list|,
name|hunk
argument_list|,
name|r
operator|->
name|pool
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
name|ctx
operator|->
name|out
operator|.
name|hunk
operator|=
name|ctx
operator|->
name|hunk
expr_stmt|;
name|ctx
operator|->
name|out
operator|.
name|next
operator|=
name|NULL
expr_stmt|;
name|rc
operator|=
name|ctx
operator|->
name|next_filter
argument_list|(
name|r
argument_list|,
operator|&
name|ctx
operator|->
name|out
argument_list|)
expr_stmt|;
block|}
block|}
block|}
else|else
block|{
name|ctx
operator|->
name|out
operator|.
name|hunk
operator|=
name|hunk
expr_stmt|;
name|ctx
operator|->
name|out
operator|.
name|next
operator|=
name|NULL
expr_stmt|;
name|rc
operator|=
name|ctx
operator|->
name|next_filter
argument_list|(
name|r
argument_list|,
operator|&
name|ctx
operator|->
name|out
argument_list|)
expr_stmt|;
block|}
block|}
block|}
if|if
condition|(
name|rc
operator|==
name|NGX_OK
operator|&&
name|ctx
operator|->
name|hunk
condition|)
name|ctx
operator|->
name|hunk
operator|->
name|pos
operator|.
name|mem
operator|=
name|ctx
operator|->
name|hunk
operator|->
name|last
operator|.
name|mem
operator|=
name|ctx
operator|->
name|hunk
operator|->
name|start
expr_stmt|;
block|}
if|if
condition|(
name|rc
operator|==
name|NGX_OK
operator|&&
name|ctx
operator|->
name|last
condition|)
return|return
name|NGX_OK
return|;
if|if
condition|(
name|rc
operator|==
name|NGX_OK
condition|)
block|{
name|ctx
operator|->
name|hunk
operator|->
name|pos
operator|.
name|mem
operator|=
name|ctx
operator|->
name|hunk
operator|->
name|last
operator|.
name|mem
operator|=
name|ctx
operator|->
name|hunk
operator|->
name|start
expr_stmt|;
if|#
directive|if
name|level_event
name|ngx_del_event
argument_list|(
name|r
operator|->
name|connection
operator|->
name|write
argument_list|,
name|NGX_WRITE_EVENT
argument_list|)
expr_stmt|;
endif|#
directive|endif
block|}
return|return
name|rc
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_output_filter_copy_hunk (ngx_hunk_t * dst,ngx_hunk_t * src)
specifier|static
name|int
name|ngx_http_output_filter_copy_hunk
parameter_list|(
name|ngx_hunk_t
modifier|*
name|dst
parameter_list|,
name|ngx_hunk_t
modifier|*
name|src
parameter_list|)
block|{
name|size_t
name|size
decl_stmt|;
name|ssize_t
name|n
decl_stmt|;
name|size
operator|=
name|src
operator|->
name|last
operator|.
name|mem
operator|-
name|src
operator|->
name|pos
operator|.
name|mem
expr_stmt|;
if|if
condition|(
name|size
operator|>
name|dst
operator|->
name|end
operator|-
name|dst
operator|->
name|pos
operator|.
name|mem
condition|)
name|size
operator|=
name|dst
operator|->
name|end
operator|-
name|dst
operator|->
name|pos
operator|.
name|mem
expr_stmt|;
if|if
condition|(
name|src
operator|->
name|type
operator|&
name|NGX_HUNK_FILE
condition|)
block|{
name|n
operator|=
name|ngx_read_file
argument_list|(
name|src
operator|->
name|file
argument_list|,
name|dst
operator|->
name|pos
operator|.
name|mem
argument_list|,
name|size
argument_list|,
name|src
operator|->
name|pos
operator|.
name|file
argument_list|)
expr_stmt|;
if|if
condition|(
name|n
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|n
return|;
if|#
directive|if
operator|(
name|NGX_FILE_AIO
operator|)
block|}
if|else if
condition|(
name|n
operator|==
name|NGX_AGAIN
condition|)
block|{
return|return
name|n
return|;
endif|#
directive|endif
block|}
else|else
block|{
name|ngx_assert
argument_list|(
argument|(n == size)
argument_list|,
comment|/* void */
argument|;
argument_list|,
argument|src->file->log
argument_list|,
argument|ngx_read_file_n
literal|" reads only %d of %d"
argument|_                        n _ size
argument_list|)
empty_stmt|;
block|}
name|src
operator|->
name|pos
operator|.
name|mem
operator|+=
name|n
expr_stmt|;
name|dst
operator|->
name|last
operator|.
name|mem
operator|+=
name|n
expr_stmt|;
block|}
else|else
block|{
name|ngx_memcpy
argument_list|(
name|src
operator|->
name|pos
operator|.
name|mem
argument_list|,
name|dst
operator|->
name|pos
operator|.
name|mem
argument_list|,
name|size
argument_list|)
expr_stmt|;
name|src
operator|->
name|pos
operator|.
name|mem
operator|+=
name|size
expr_stmt|;
name|dst
operator|->
name|last
operator|.
name|mem
operator|+=
name|size
expr_stmt|;
block|}
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_output_filter_create_conf (ngx_pool_t * pool)
specifier|static
name|void
modifier|*
name|ngx_http_output_filter_create_conf
parameter_list|(
name|ngx_pool_t
modifier|*
name|pool
parameter_list|)
block|{
name|ngx_http_output_filter_conf_t
modifier|*
name|conf
decl_stmt|;
name|ngx_test_null
argument_list|(
name|conf
argument_list|,
name|ngx_pcalloc
argument_list|(
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_output_filter_conf_t
argument_list|)
argument_list|)
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|conf
operator|->
name|hunk_size
operator|=
name|NGX_CONF_UNSET
expr_stmt|;
return|return
name|conf
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_output_filter_init (int (** next_filter)(ngx_http_request_t * r,ngx_chain_t * ch))
specifier|static
name|int
name|ngx_http_output_filter_init
parameter_list|(
name|int
function_decl|(
modifier|*
modifier|*
name|next_filter
function_decl|)
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_chain_t
modifier|*
name|ch
parameter_list|)
parameter_list|)
block|{
name|ngx_http_output_next_filter
operator|=
operator|*
name|next_filter
expr_stmt|;
operator|*
name|next_filter
operator|=
name|NULL
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

end_unit

