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

begin_include
include|#
directive|include
file|<ngx_http.h>
end_include

begin_typedef
DECL|struct|__anon29e7ea2c0108
typedef|typedef
struct|struct
block|{
DECL|member|bufs
name|ngx_bufs_t
name|bufs
decl_stmt|;
DECL|typedef|ngx_http_copy_filter_conf_t
block|}
name|ngx_http_copy_filter_conf_t
typedef|;
end_typedef

begin_if
if|#
directive|if
operator|(
name|NGX_HAVE_FILE_AIO
operator|)
end_if

begin_function_decl
specifier|static
name|void
name|ngx_http_copy_aio_handler
parameter_list|(
name|ngx_output_chain_ctx_t
modifier|*
name|ctx
parameter_list|,
name|ngx_file_t
modifier|*
name|file
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|void
name|ngx_http_copy_aio_event_handler
parameter_list|(
name|ngx_event_t
modifier|*
name|ev
parameter_list|)
function_decl|;
end_function_decl

begin_if
if|#
directive|if
operator|(
name|NGX_HAVE_AIO_SENDFILE
operator|)
end_if

begin_function_decl
specifier|static
name|void
name|ngx_http_copy_aio_sendfile_event_handler
parameter_list|(
name|ngx_event_t
modifier|*
name|ev
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

begin_function_decl
specifier|static
name|void
modifier|*
name|ngx_http_copy_filter_create_conf
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|char
modifier|*
name|ngx_http_copy_filter_merge_conf
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
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
name|ngx_int_t
name|ngx_http_copy_filter_init
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
function_decl|;
end_function_decl

begin_decl_stmt
DECL|variable|ngx_http_copy_filter_commands
specifier|static
name|ngx_command_t
name|ngx_http_copy_filter_commands
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"output_buffers"
argument_list|)
block|,
name|NGX_HTTP_MAIN_CONF
operator||
name|NGX_HTTP_SRV_CONF
operator||
name|NGX_HTTP_LOC_CONF
operator||
name|NGX_CONF_TAKE2
block|,
name|ngx_conf_set_bufs_slot
block|,
name|NGX_HTTP_LOC_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_http_copy_filter_conf_t
argument_list|,
name|bufs
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
DECL|variable|ngx_http_copy_filter_module_ctx
specifier|static
name|ngx_http_module_t
name|ngx_http_copy_filter_module_ctx
init|=
block|{
name|NULL
block|,
comment|/* preconfiguration */
name|ngx_http_copy_filter_init
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
block|,
comment|/* merge server configuration */
name|ngx_http_copy_filter_create_conf
block|,
comment|/* create location configuration */
name|ngx_http_copy_filter_merge_conf
comment|/* merge location configuration */
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_copy_filter_module
name|ngx_module_t
name|ngx_http_copy_filter_module
init|=
block|{
name|NGX_MODULE_V1
block|,
operator|&
name|ngx_http_copy_filter_module_ctx
block|,
comment|/* module context */
name|ngx_http_copy_filter_commands
block|,
comment|/* module directives */
name|NGX_HTTP_MODULE
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

begin_decl_stmt
DECL|variable|ngx_http_next_filter
specifier|static
name|ngx_http_output_body_filter_pt
name|ngx_http_next_filter
decl_stmt|;
end_decl_stmt

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_copy_filter (ngx_http_request_t * r,ngx_chain_t * in)
name|ngx_http_copy_filter
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
name|ngx_int_t
name|rc
decl_stmt|;
name|ngx_connection_t
modifier|*
name|c
decl_stmt|;
name|ngx_output_chain_ctx_t
modifier|*
name|ctx
decl_stmt|;
name|ngx_http_core_loc_conf_t
modifier|*
name|clcf
decl_stmt|;
name|ngx_http_copy_filter_conf_t
modifier|*
name|conf
decl_stmt|;
name|c
operator|=
name|r
operator|->
name|connection
expr_stmt|;
name|ngx_log_debug2
argument_list|(
name|NGX_LOG_DEBUG_HTTP
argument_list|,
name|c
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"http copy filter: \"%V?%V\""
argument_list|,
operator|&
name|r
operator|->
name|uri
argument_list|,
operator|&
name|r
operator|->
name|args
argument_list|)
expr_stmt|;
name|ctx
operator|=
name|ngx_http_get_module_ctx
argument_list|(
name|r
argument_list|,
name|ngx_http_copy_filter_module
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
name|r
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_output_chain_ctx_t
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
name|ngx_http_set_ctx
argument_list|(
name|r
argument_list|,
name|ctx
argument_list|,
name|ngx_http_copy_filter_module
argument_list|)
expr_stmt|;
name|conf
operator|=
name|ngx_http_get_module_loc_conf
argument_list|(
name|r
argument_list|,
name|ngx_http_copy_filter_module
argument_list|)
expr_stmt|;
name|clcf
operator|=
name|ngx_http_get_module_loc_conf
argument_list|(
name|r
argument_list|,
name|ngx_http_core_module
argument_list|)
expr_stmt|;
name|ctx
operator|->
name|sendfile
operator|=
name|c
operator|->
name|sendfile
expr_stmt|;
name|ctx
operator|->
name|need_in_memory
operator|=
name|r
operator|->
name|main_filter_need_in_memory
operator|||
name|r
operator|->
name|filter_need_in_memory
expr_stmt|;
name|ctx
operator|->
name|need_in_temp
operator|=
name|r
operator|->
name|filter_need_temporary
expr_stmt|;
name|ctx
operator|->
name|alignment
operator|=
name|clcf
operator|->
name|directio_alignment
expr_stmt|;
name|ctx
operator|->
name|pool
operator|=
name|r
operator|->
name|pool
expr_stmt|;
name|ctx
operator|->
name|bufs
operator|=
name|conf
operator|->
name|bufs
expr_stmt|;
name|ctx
operator|->
name|tag
operator|=
operator|(
name|ngx_buf_tag_t
operator|)
operator|&
name|ngx_http_copy_filter_module
expr_stmt|;
name|ctx
operator|->
name|output_filter
operator|=
operator|(
name|ngx_output_chain_filter_pt
operator|)
name|ngx_http_next_filter
expr_stmt|;
name|ctx
operator|->
name|filter_ctx
operator|=
name|r
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_HAVE_FILE_AIO
operator|)
if|if
condition|(
name|ngx_file_aio
operator|&&
name|clcf
operator|->
name|aio
condition|)
block|{
name|ctx
operator|->
name|aio_handler
operator|=
name|ngx_http_copy_aio_handler
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_HAVE_AIO_SENDFILE
operator|)
name|c
operator|->
name|aio_sendfile
operator|=
operator|(
name|clcf
operator|->
name|aio
operator|==
name|NGX_HTTP_AIO_SENDFILE
operator|)
expr_stmt|;
endif|#
directive|endif
block|}
endif|#
directive|endif
if|if
condition|(
name|in
operator|&&
name|in
operator|->
name|buf
operator|&&
name|ngx_buf_size
argument_list|(
name|in
operator|->
name|buf
argument_list|)
condition|)
block|{
name|r
operator|->
name|request_output
operator|=
literal|1
expr_stmt|;
block|}
block|}
if|#
directive|if
operator|(
name|NGX_HAVE_FILE_AIO
operator|)
name|ctx
operator|->
name|aio
operator|=
name|r
operator|->
name|aio
expr_stmt|;
endif|#
directive|endif
for|for
control|(
init|;
condition|;
control|)
block|{
name|rc
operator|=
name|ngx_output_chain
argument_list|(
name|ctx
argument_list|,
name|in
argument_list|)
expr_stmt|;
if|if
condition|(
name|ctx
operator|->
name|in
operator|==
name|NULL
condition|)
block|{
name|r
operator|->
name|buffered
operator|&=
operator|~
name|NGX_HTTP_COPY_BUFFERED
expr_stmt|;
block|}
else|else
block|{
name|r
operator|->
name|buffered
operator||=
name|NGX_HTTP_COPY_BUFFERED
expr_stmt|;
block|}
name|ngx_log_debug3
argument_list|(
name|NGX_LOG_DEBUG_HTTP
argument_list|,
name|c
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"http copy filter: %i \"%V?%V\""
argument_list|,
name|rc
argument_list|,
operator|&
name|r
operator|->
name|uri
argument_list|,
operator|&
name|r
operator|->
name|args
argument_list|)
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_HAVE_FILE_AIO
operator|&&
name|NGX_HAVE_AIO_SENDFILE
operator|)
if|if
condition|(
name|c
operator|->
name|busy_sendfile
condition|)
block|{
name|ssize_t
name|n
decl_stmt|;
name|off_t
name|offset
decl_stmt|;
name|ngx_file_t
modifier|*
name|file
decl_stmt|;
name|ngx_http_ephemeral_t
modifier|*
name|e
decl_stmt|;
name|file
operator|=
name|c
operator|->
name|busy_sendfile
operator|->
name|file
expr_stmt|;
name|offset
operator|=
name|c
operator|->
name|busy_sendfile
operator|->
name|file_pos
expr_stmt|;
if|if
condition|(
name|file
operator|->
name|aio
condition|)
block|{
name|c
operator|->
name|aio_sendfile
operator|=
operator|(
name|offset
operator|!=
name|file
operator|->
name|aio
operator|->
name|last_offset
operator|)
expr_stmt|;
name|file
operator|->
name|aio
operator|->
name|last_offset
operator|=
name|offset
expr_stmt|;
if|if
condition|(
name|c
operator|->
name|aio_sendfile
operator|==
literal|0
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
literal|"sendfile(%V) returned busy again"
argument_list|,
operator|&
name|file
operator|->
name|name
argument_list|)
expr_stmt|;
block|}
block|}
name|c
operator|->
name|busy_sendfile
operator|=
name|NULL
expr_stmt|;
name|e
operator|=
operator|(
name|ngx_http_ephemeral_t
operator|*
operator|)
operator|&
name|r
operator|->
name|uri_start
expr_stmt|;
name|n
operator|=
name|ngx_file_aio_read
argument_list|(
name|file
argument_list|,
operator|&
name|e
operator|->
name|aio_preload
argument_list|,
literal|1
argument_list|,
name|offset
argument_list|,
name|r
operator|->
name|pool
argument_list|)
expr_stmt|;
if|if
condition|(
name|n
operator|>
literal|0
condition|)
block|{
name|in
operator|=
name|NULL
expr_stmt|;
continue|continue;
block|}
name|rc
operator|=
name|n
expr_stmt|;
if|if
condition|(
name|file
operator|->
name|aio
condition|)
block|{
name|file
operator|->
name|aio
operator|->
name|data
operator|=
name|r
expr_stmt|;
name|file
operator|->
name|aio
operator|->
name|handler
operator|=
name|ngx_http_copy_aio_sendfile_event_handler
expr_stmt|;
name|r
operator|->
expr|main
operator|->
name|blocked
operator|++
expr_stmt|;
name|r
operator|->
name|aio
operator|=
literal|1
expr_stmt|;
block|}
block|}
endif|#
directive|endif
return|return
name|rc
return|;
block|}
block|}
end_function

begin_if
if|#
directive|if
operator|(
name|NGX_HAVE_FILE_AIO
operator|)
end_if

begin_function
specifier|static
name|void
DECL|function|ngx_http_copy_aio_handler (ngx_output_chain_ctx_t * ctx,ngx_file_t * file)
name|ngx_http_copy_aio_handler
parameter_list|(
name|ngx_output_chain_ctx_t
modifier|*
name|ctx
parameter_list|,
name|ngx_file_t
modifier|*
name|file
parameter_list|)
block|{
name|ngx_http_request_t
modifier|*
name|r
decl_stmt|;
name|r
operator|=
name|ctx
operator|->
name|filter_ctx
expr_stmt|;
name|file
operator|->
name|aio
operator|->
name|data
operator|=
name|r
expr_stmt|;
name|file
operator|->
name|aio
operator|->
name|handler
operator|=
name|ngx_http_copy_aio_event_handler
expr_stmt|;
name|r
operator|->
expr|main
operator|->
name|blocked
operator|++
expr_stmt|;
name|r
operator|->
name|aio
operator|=
literal|1
expr_stmt|;
block|}
end_function

begin_function
specifier|static
name|void
DECL|function|ngx_http_copy_aio_event_handler (ngx_event_t * ev)
name|ngx_http_copy_aio_event_handler
parameter_list|(
name|ngx_event_t
modifier|*
name|ev
parameter_list|)
block|{
name|ngx_event_aio_t
modifier|*
name|aio
decl_stmt|;
name|ngx_http_request_t
modifier|*
name|r
decl_stmt|;
name|aio
operator|=
name|ev
operator|->
name|data
expr_stmt|;
name|r
operator|=
name|aio
operator|->
name|data
expr_stmt|;
name|r
operator|->
expr|main
operator|->
name|blocked
operator|--
expr_stmt|;
name|r
operator|->
name|aio
operator|=
literal|0
expr_stmt|;
name|r
operator|->
name|connection
operator|->
name|write
operator|->
name|handler
argument_list|(
name|r
operator|->
name|connection
operator|->
name|write
argument_list|)
expr_stmt|;
block|}
end_function

begin_if
if|#
directive|if
operator|(
name|NGX_HAVE_AIO_SENDFILE
operator|)
end_if

begin_function
specifier|static
name|void
DECL|function|ngx_http_copy_aio_sendfile_event_handler (ngx_event_t * ev)
name|ngx_http_copy_aio_sendfile_event_handler
parameter_list|(
name|ngx_event_t
modifier|*
name|ev
parameter_list|)
block|{
name|ngx_event_aio_t
modifier|*
name|aio
decl_stmt|;
name|ngx_http_request_t
modifier|*
name|r
decl_stmt|;
name|aio
operator|=
name|ev
operator|->
name|data
expr_stmt|;
name|r
operator|=
name|aio
operator|->
name|data
expr_stmt|;
name|r
operator|->
expr|main
operator|->
name|blocked
operator|--
expr_stmt|;
name|r
operator|->
name|aio
operator|=
literal|0
expr_stmt|;
name|ev
operator|->
name|complete
operator|=
literal|0
expr_stmt|;
name|r
operator|->
name|connection
operator|->
name|write
operator|->
name|handler
argument_list|(
name|r
operator|->
name|connection
operator|->
name|write
argument_list|)
expr_stmt|;
block|}
end_function

begin_endif
endif|#
directive|endif
end_endif

begin_endif
endif|#
directive|endif
end_endif

begin_function
specifier|static
name|void
modifier|*
DECL|function|ngx_http_copy_filter_create_conf (ngx_conf_t * cf)
name|ngx_http_copy_filter_create_conf
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
block|{
name|ngx_http_copy_filter_conf_t
modifier|*
name|conf
decl_stmt|;
name|conf
operator|=
name|ngx_palloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_copy_filter_conf_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|conf
operator|==
name|NULL
condition|)
block|{
return|return
name|NULL
return|;
block|}
name|conf
operator|->
name|bufs
operator|.
name|num
operator|=
literal|0
expr_stmt|;
return|return
name|conf
return|;
block|}
end_function

begin_function
specifier|static
name|char
modifier|*
DECL|function|ngx_http_copy_filter_merge_conf (ngx_conf_t * cf,void * parent,void * child)
name|ngx_http_copy_filter_merge_conf
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
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
name|ngx_http_copy_filter_conf_t
modifier|*
name|prev
init|=
name|parent
decl_stmt|;
name|ngx_http_copy_filter_conf_t
modifier|*
name|conf
init|=
name|child
decl_stmt|;
name|ngx_conf_merge_bufs_value
argument_list|(
name|conf
operator|->
name|bufs
argument_list|,
name|prev
operator|->
name|bufs
argument_list|,
literal|1
argument_list|,
literal|32768
argument_list|)
expr_stmt|;
return|return
name|NULL
return|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_copy_filter_init (ngx_conf_t * cf)
name|ngx_http_copy_filter_init
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
block|{
name|ngx_http_next_filter
operator|=
name|ngx_http_top_body_filter
expr_stmt|;
name|ngx_http_top_body_filter
operator|=
name|ngx_http_copy_filter
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

end_unit

