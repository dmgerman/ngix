begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
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
file|<ngx_hunk.h>
end_include

begin_include
include|#
directive|include
file|<ngx_conf_file.h>
end_include

begin_include
include|#
directive|include
file|<ngx_connection.h>
end_include

begin_include
include|#
directive|include
file|<ngx_event_write.h>
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
file|<ngx_http_write_filter.h>
end_include

begin_function_decl
specifier|static
name|void
modifier|*
name|ngx_http_write_filter_create_conf
parameter_list|(
name|ngx_pool_t
modifier|*
name|pool
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|char
modifier|*
name|ngx_http_write_filter_merge_conf
parameter_list|(
name|ngx_pool_t
modifier|*
name|pool
parameter_list|,
name|void
modifier|*
name|parent
parameter_list|,
name|void
modifier|*
name|child
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|int
name|ngx_http_write_filter_init
parameter_list|(
name|ngx_pool_t
modifier|*
name|pool
parameter_list|)
function_decl|;
end_function_decl

begin_decl_stmt
DECL|variable|ngx_http_write_filter_commands
specifier|static
name|ngx_command_t
name|ngx_http_write_filter_commands
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"buffer_output"
argument_list|)
block|,
name|NGX_HTTP_LOC_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_conf_set_size_slot
block|,
name|NGX_HTTP_LOC_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_http_write_filter_conf_t
argument_list|,
name|buffer_output
argument_list|)
block|,
name|NULL
block|}
block|,
name|ngx_null_command
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_write_filter_module_ctx
name|ngx_http_module_t
name|ngx_http_write_filter_module_ctx
init|=
block|{
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
block|,
comment|/* merge server configuration */
name|ngx_http_write_filter_create_conf
block|,
comment|/* create location configuration */
name|ngx_http_write_filter_merge_conf
comment|/* merge location configuration */
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_write_filter_module
name|ngx_module_t
name|ngx_http_write_filter_module
init|=
block|{
name|NGX_MODULE
block|,
operator|&
name|ngx_http_write_filter_module_ctx
block|,
comment|/* module context */
name|ngx_http_write_filter_commands
block|,
comment|/* module directives */
name|NGX_HTTP_MODULE
block|,
comment|/* module type */
name|ngx_http_write_filter_init
comment|/* init module */
block|}
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|ngx_http_write_filter (ngx_http_request_t * r,ngx_chain_t * in)
name|int
name|ngx_http_write_filter
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_chain_t
modifier|*
name|in
parameter_list|)
block|{
name|int
name|last
decl_stmt|;
name|off_t
name|size
decl_stmt|,
name|flush
decl_stmt|;
name|ngx_chain_t
modifier|*
name|ce
decl_stmt|,
modifier|*
modifier|*
name|le
decl_stmt|,
modifier|*
name|chain
decl_stmt|;
name|ngx_http_write_filter_ctx_t
modifier|*
name|ctx
decl_stmt|;
name|ngx_http_write_filter_conf_t
modifier|*
name|conf
decl_stmt|;
name|ctx
operator|=
name|ngx_http_get_module_ctx
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
name|ngx_http_write_filter_module
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
name|ngx_http_write_filter_module
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_write_filter_ctx_t
argument_list|)
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
block|}
name|size
operator|=
name|flush
operator|=
literal|0
expr_stmt|;
name|last
operator|=
literal|0
expr_stmt|;
name|le
operator|=
operator|&
name|ctx
operator|->
name|out
expr_stmt|;
comment|/* find the size, the flush point and the last entry of the saved chain */
for|for
control|(
name|ce
operator|=
name|ctx
operator|->
name|out
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
name|le
operator|=
operator|&
name|ce
operator|->
name|next
expr_stmt|;
if|if
condition|(
name|ce
operator|->
name|hunk
operator|->
name|type
operator|&
name|NGX_HUNK_IN_MEMORY
condition|)
block|{
name|size
operator|+=
name|ce
operator|->
name|hunk
operator|->
name|last
operator|-
name|ce
operator|->
name|hunk
operator|->
name|pos
expr_stmt|;
block|}
else|else
block|{
name|size
operator|+=
name|ce
operator|->
name|hunk
operator|->
name|file_last
operator|-
name|ce
operator|->
name|hunk
operator|->
name|file_pos
expr_stmt|;
block|}
if|if
condition|(
name|ce
operator|->
name|hunk
operator|->
name|type
operator|&
operator|(
name|NGX_HUNK_FLUSH
operator||
name|NGX_HUNK_RECYCLED
operator|)
condition|)
block|{
name|flush
operator|=
name|size
expr_stmt|;
block|}
if|if
condition|(
name|ce
operator|->
name|hunk
operator|->
name|type
operator|&
name|NGX_HUNK_LAST
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
comment|/* void */
init|;
name|in
condition|;
name|in
operator|=
name|in
operator|->
name|next
control|)
block|{
name|ngx_test_null
argument_list|(
name|ce
argument_list|,
name|ngx_palloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_chain_t
argument_list|)
argument_list|)
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
name|ce
operator|->
name|hunk
operator|=
name|in
operator|->
name|hunk
expr_stmt|;
name|ce
operator|->
name|next
operator|=
name|NULL
expr_stmt|;
operator|*
name|le
operator|=
name|ce
expr_stmt|;
name|le
operator|=
operator|&
name|ce
operator|->
name|next
expr_stmt|;
if|if
condition|(
name|ce
operator|->
name|hunk
operator|->
name|type
operator|&
name|NGX_HUNK_IN_MEMORY
condition|)
block|{
name|size
operator|+=
name|ce
operator|->
name|hunk
operator|->
name|last
operator|-
name|ce
operator|->
name|hunk
operator|->
name|pos
expr_stmt|;
block|}
else|else
block|{
name|size
operator|+=
name|ce
operator|->
name|hunk
operator|->
name|file_last
operator|-
name|ce
operator|->
name|hunk
operator|->
name|file_pos
expr_stmt|;
block|}
if|if
condition|(
name|ce
operator|->
name|hunk
operator|->
name|type
operator|&
operator|(
name|NGX_HUNK_FLUSH
operator||
name|NGX_HUNK_RECYCLED
operator|)
condition|)
block|{
name|flush
operator|=
name|size
expr_stmt|;
block|}
if|if
condition|(
name|ce
operator|->
name|hunk
operator|->
name|type
operator|&
name|NGX_HUNK_LAST
condition|)
block|{
name|last
operator|=
literal|1
expr_stmt|;
block|}
block|}
name|conf
operator|=
name|ngx_http_get_module_loc_conf
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
name|ngx_http_write_filter_module
argument_list|)
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_DEBUG_WRITE_FILTER
operator|)
name|ngx_log_debug
argument_list|(
argument|r->connection->log
argument_list|,
literal|"write filter: last:%d flush:%qd size:%qd"
argument|_                   last _ flush _ size
argument_list|)
empty_stmt|;
endif|#
directive|endif
comment|/* avoid the output if there is no last hunk, no flush point and        size of the hunks is smaller then "buffer_output" */
if|if
condition|(
operator|!
name|last
operator|&&
name|flush
operator|==
literal|0
operator|&&
name|size
operator|<
name|conf
operator|->
name|buffer_output
condition|)
block|{
return|return
name|NGX_OK
return|;
block|}
if|if
condition|(
name|r
operator|->
name|connection
operator|->
name|write
operator|->
name|delayed
condition|)
block|{
return|return
name|NGX_AGAIN
return|;
block|}
if|#
directive|if
literal|1
name|chain
operator|=
name|ngx_write_chain
argument_list|(
name|r
operator|->
name|connection
argument_list|,
name|ctx
operator|->
name|out
argument_list|)
expr_stmt|;
else|#
directive|else
name|chain
operator|=
name|ngx_write_chain
argument_list|(
name|r
operator|->
name|connection
argument_list|,
name|ctx
operator|->
name|out
argument_list|,
name|flush
argument_list|)
expr_stmt|;
endif|#
directive|endif
if|#
directive|if
operator|(
name|NGX_DEBUG_WRITE_FILTER
operator|)
name|ngx_log_debug
argument_list|(
argument|r->connection->log
argument_list|,
literal|"write filter %x"
argument|_ chain
argument_list|)
empty_stmt|;
endif|#
directive|endif
if|if
condition|(
name|chain
operator|==
name|NGX_CHAIN_ERROR
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|ctx
operator|->
name|out
operator|=
name|chain
expr_stmt|;
if|if
condition|(
name|chain
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_OK
return|;
block|}
else|else
block|{
return|return
name|NGX_AGAIN
return|;
block|}
block|}
end_function

begin_function
DECL|function|ngx_http_write_filter_create_conf (ngx_pool_t * pool)
specifier|static
name|void
modifier|*
name|ngx_http_write_filter_create_conf
parameter_list|(
name|ngx_pool_t
modifier|*
name|pool
parameter_list|)
block|{
name|ngx_http_write_filter_conf_t
modifier|*
name|conf
decl_stmt|;
name|ngx_test_null
argument_list|(
name|conf
argument_list|,
name|ngx_palloc
argument_list|(
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_write_filter_conf_t
argument_list|)
argument_list|)
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|conf
operator|->
name|buffer_output
operator|=
name|NGX_CONF_UNSET
expr_stmt|;
return|return
name|conf
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_write_filter_merge_conf (ngx_pool_t * pool,void * parent,void * child)
specifier|static
name|char
modifier|*
name|ngx_http_write_filter_merge_conf
parameter_list|(
name|ngx_pool_t
modifier|*
name|pool
parameter_list|,
name|void
modifier|*
name|parent
parameter_list|,
name|void
modifier|*
name|child
parameter_list|)
block|{
name|ngx_http_write_filter_conf_t
modifier|*
name|prev
init|=
name|parent
decl_stmt|;
name|ngx_http_write_filter_conf_t
modifier|*
name|conf
init|=
name|child
decl_stmt|;
name|ngx_conf_merge_size_value
argument_list|(
name|conf
operator|->
name|buffer_output
argument_list|,
name|prev
operator|->
name|buffer_output
argument_list|,
literal|1460
argument_list|)
expr_stmt|;
return|return
name|NULL
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_write_filter_init (ngx_pool_t * pool)
specifier|static
name|int
name|ngx_http_write_filter_init
parameter_list|(
name|ngx_pool_t
modifier|*
name|pool
parameter_list|)
block|{
name|ngx_http_top_body_filter
operator|=
name|ngx_http_write_filter
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

end_unit

