begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (C) Nginx, Inc.  * Copyright (C) Valentin V. Bartenev  */
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

begin_include
include|#
directive|include
file|<ngx_http_spdy_module.h>
end_include

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_spdy_add_variables
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_spdy_variable
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_variable_value_t
modifier|*
name|v
parameter_list|,
name|uintptr_t
name|data
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_spdy_request_priority_variable
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_variable_value_t
modifier|*
name|v
parameter_list|,
name|uintptr_t
name|data
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_spdy_module_init
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|void
modifier|*
name|ngx_http_spdy_create_main_conf
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
name|ngx_http_spdy_init_main_conf
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|void
modifier|*
name|conf
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|void
modifier|*
name|ngx_http_spdy_create_srv_conf
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
name|ngx_http_spdy_merge_srv_conf
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
name|char
modifier|*
name|ngx_http_spdy_recv_buffer_size
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|void
modifier|*
name|post
parameter_list|,
name|void
modifier|*
name|data
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|char
modifier|*
name|ngx_http_spdy_pool_size
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|void
modifier|*
name|post
parameter_list|,
name|void
modifier|*
name|data
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|char
modifier|*
name|ngx_http_spdy_streams_index_mask
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|void
modifier|*
name|post
parameter_list|,
name|void
modifier|*
name|data
parameter_list|)
function_decl|;
end_function_decl

begin_decl_stmt
DECL|variable|ngx_http_spdy_headers_comp_bounds
specifier|static
name|ngx_conf_num_bounds_t
name|ngx_http_spdy_headers_comp_bounds
init|=
block|{
name|ngx_conf_check_num_bounds
block|,
literal|0
block|,
literal|9
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_spdy_recv_buffer_size_post
specifier|static
name|ngx_conf_post_t
name|ngx_http_spdy_recv_buffer_size_post
init|=
block|{
name|ngx_http_spdy_recv_buffer_size
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_spdy_pool_size_post
specifier|static
name|ngx_conf_post_t
name|ngx_http_spdy_pool_size_post
init|=
block|{
name|ngx_http_spdy_pool_size
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_spdy_streams_index_mask_post
specifier|static
name|ngx_conf_post_t
name|ngx_http_spdy_streams_index_mask_post
init|=
block|{
name|ngx_http_spdy_streams_index_mask
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_spdy_commands
specifier|static
name|ngx_command_t
name|ngx_http_spdy_commands
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"spdy_recv_buffer_size"
argument_list|)
block|,
name|NGX_HTTP_MAIN_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_conf_set_size_slot
block|,
name|NGX_HTTP_MAIN_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_http_spdy_main_conf_t
argument_list|,
name|recv_buffer_size
argument_list|)
block|,
operator|&
name|ngx_http_spdy_recv_buffer_size_post
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"spdy_pool_size"
argument_list|)
block|,
name|NGX_HTTP_MAIN_CONF
operator||
name|NGX_HTTP_SRV_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_conf_set_size_slot
block|,
name|NGX_HTTP_SRV_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_http_spdy_srv_conf_t
argument_list|,
name|pool_size
argument_list|)
block|,
operator|&
name|ngx_http_spdy_pool_size_post
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"spdy_max_concurrent_streams"
argument_list|)
block|,
name|NGX_HTTP_MAIN_CONF
operator||
name|NGX_HTTP_SRV_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_conf_set_num_slot
block|,
name|NGX_HTTP_SRV_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_http_spdy_srv_conf_t
argument_list|,
name|concurrent_streams
argument_list|)
block|,
name|NULL
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"spdy_streams_index_size"
argument_list|)
block|,
name|NGX_HTTP_MAIN_CONF
operator||
name|NGX_HTTP_SRV_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_conf_set_num_slot
block|,
name|NGX_HTTP_SRV_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_http_spdy_srv_conf_t
argument_list|,
name|streams_index_mask
argument_list|)
block|,
operator|&
name|ngx_http_spdy_streams_index_mask_post
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"spdy_recv_timeout"
argument_list|)
block|,
name|NGX_HTTP_MAIN_CONF
operator||
name|NGX_HTTP_SRV_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_conf_set_msec_slot
block|,
name|NGX_HTTP_SRV_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_http_spdy_srv_conf_t
argument_list|,
name|recv_timeout
argument_list|)
block|,
name|NULL
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"spdy_keepalive_timeout"
argument_list|)
block|,
name|NGX_HTTP_MAIN_CONF
operator||
name|NGX_HTTP_SRV_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_conf_set_msec_slot
block|,
name|NGX_HTTP_SRV_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_http_spdy_srv_conf_t
argument_list|,
name|keepalive_timeout
argument_list|)
block|,
name|NULL
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"spdy_headers_comp"
argument_list|)
block|,
name|NGX_HTTP_MAIN_CONF
operator||
name|NGX_HTTP_SRV_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_conf_set_num_slot
block|,
name|NGX_HTTP_SRV_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_http_spdy_srv_conf_t
argument_list|,
name|headers_comp
argument_list|)
block|,
operator|&
name|ngx_http_spdy_headers_comp_bounds
block|}
block|,
name|ngx_null_command
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_spdy_module_ctx
specifier|static
name|ngx_http_module_t
name|ngx_http_spdy_module_ctx
init|=
block|{
name|ngx_http_spdy_add_variables
block|,
comment|/* preconfiguration */
name|NULL
block|,
comment|/* postconfiguration */
name|ngx_http_spdy_create_main_conf
block|,
comment|/* create main configuration */
name|ngx_http_spdy_init_main_conf
block|,
comment|/* init main configuration */
name|ngx_http_spdy_create_srv_conf
block|,
comment|/* create server configuration */
name|ngx_http_spdy_merge_srv_conf
block|,
comment|/* merge server configuration */
name|NULL
block|,
comment|/* create location configuration */
name|NULL
comment|/* merge location configuration */
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_spdy_module
name|ngx_module_t
name|ngx_http_spdy_module
init|=
block|{
name|NGX_MODULE_V1
block|,
operator|&
name|ngx_http_spdy_module_ctx
block|,
comment|/* module context */
name|ngx_http_spdy_commands
block|,
comment|/* module directives */
name|NGX_HTTP_MODULE
block|,
comment|/* module type */
name|NULL
block|,
comment|/* init master */
name|ngx_http_spdy_module_init
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
DECL|variable|ngx_http_spdy_vars
specifier|static
name|ngx_http_variable_t
name|ngx_http_spdy_vars
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"spdy"
argument_list|)
block|,
name|NULL
block|,
name|ngx_http_spdy_variable
block|,
literal|0
block|,
literal|0
block|,
literal|0
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"spdy_request_priority"
argument_list|)
block|,
name|NULL
block|,
name|ngx_http_spdy_request_priority_variable
block|,
literal|0
block|,
literal|0
block|,
literal|0
block|}
block|,
block|{
name|ngx_null_string
block|,
name|NULL
block|,
name|NULL
block|,
literal|0
block|,
literal|0
block|,
literal|0
block|}
block|}
decl_stmt|;
end_decl_stmt

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_spdy_add_variables (ngx_conf_t * cf)
name|ngx_http_spdy_add_variables
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
block|{
name|ngx_http_variable_t
modifier|*
name|var
decl_stmt|,
modifier|*
name|v
decl_stmt|;
for|for
control|(
name|v
operator|=
name|ngx_http_spdy_vars
init|;
name|v
operator|->
name|name
operator|.
name|len
condition|;
name|v
operator|++
control|)
block|{
name|var
operator|=
name|ngx_http_add_variable
argument_list|(
name|cf
argument_list|,
operator|&
name|v
operator|->
name|name
argument_list|,
name|v
operator|->
name|flags
argument_list|)
expr_stmt|;
if|if
condition|(
name|var
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|var
operator|->
name|get_handler
operator|=
name|v
operator|->
name|get_handler
expr_stmt|;
name|var
operator|->
name|data
operator|=
name|v
operator|->
name|data
expr_stmt|;
block|}
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_spdy_variable (ngx_http_request_t * r,ngx_http_variable_value_t * v,uintptr_t data)
name|ngx_http_spdy_variable
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_variable_value_t
modifier|*
name|v
parameter_list|,
name|uintptr_t
name|data
parameter_list|)
block|{
if|if
condition|(
name|r
operator|->
name|spdy_stream
condition|)
block|{
name|v
operator|->
name|len
operator|=
literal|1
expr_stmt|;
name|v
operator|->
name|valid
operator|=
literal|1
expr_stmt|;
name|v
operator|->
name|no_cacheable
operator|=
literal|0
expr_stmt|;
name|v
operator|->
name|not_found
operator|=
literal|0
expr_stmt|;
name|v
operator|->
name|data
operator|=
operator|(
name|u_char
operator|*
operator|)
literal|"2"
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
operator|*
name|v
operator|=
name|ngx_http_variable_null_value
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_spdy_request_priority_variable (ngx_http_request_t * r,ngx_http_variable_value_t * v,uintptr_t data)
name|ngx_http_spdy_request_priority_variable
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_variable_value_t
modifier|*
name|v
parameter_list|,
name|uintptr_t
name|data
parameter_list|)
block|{
if|if
condition|(
name|r
operator|->
name|spdy_stream
condition|)
block|{
name|v
operator|->
name|len
operator|=
literal|1
expr_stmt|;
name|v
operator|->
name|valid
operator|=
literal|1
expr_stmt|;
name|v
operator|->
name|no_cacheable
operator|=
literal|0
expr_stmt|;
name|v
operator|->
name|not_found
operator|=
literal|0
expr_stmt|;
name|v
operator|->
name|data
operator|=
name|ngx_pnalloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|v
operator|->
name|data
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|v
operator|->
name|data
index|[
literal|0
index|]
operator|=
literal|'0'
operator|+
operator|(
name|u_char
operator|)
name|r
operator|->
name|spdy_stream
operator|->
name|priority
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
operator|*
name|v
operator|=
name|ngx_http_variable_null_value
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_spdy_module_init (ngx_cycle_t * cycle)
name|ngx_http_spdy_module_init
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
block|{
name|ngx_http_spdy_request_headers_init
argument_list|()
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
specifier|static
name|void
modifier|*
DECL|function|ngx_http_spdy_create_main_conf (ngx_conf_t * cf)
name|ngx_http_spdy_create_main_conf
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
block|{
name|ngx_http_spdy_main_conf_t
modifier|*
name|smcf
decl_stmt|;
name|smcf
operator|=
name|ngx_pcalloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_spdy_main_conf_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|smcf
operator|==
name|NULL
condition|)
block|{
return|return
name|NULL
return|;
block|}
name|smcf
operator|->
name|recv_buffer_size
operator|=
name|NGX_CONF_UNSET_SIZE
expr_stmt|;
return|return
name|smcf
return|;
block|}
end_function

begin_function
specifier|static
name|char
modifier|*
DECL|function|ngx_http_spdy_init_main_conf (ngx_conf_t * cf,void * conf)
name|ngx_http_spdy_init_main_conf
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|void
modifier|*
name|conf
parameter_list|)
block|{
name|ngx_http_spdy_main_conf_t
modifier|*
name|smcf
init|=
name|conf
decl_stmt|;
if|if
condition|(
name|smcf
operator|->
name|recv_buffer_size
operator|==
name|NGX_CONF_UNSET_SIZE
condition|)
block|{
name|smcf
operator|->
name|recv_buffer_size
operator|=
literal|256
operator|*
literal|1024
expr_stmt|;
block|}
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

begin_function
specifier|static
name|void
modifier|*
DECL|function|ngx_http_spdy_create_srv_conf (ngx_conf_t * cf)
name|ngx_http_spdy_create_srv_conf
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
block|{
name|ngx_http_spdy_srv_conf_t
modifier|*
name|sscf
decl_stmt|;
name|sscf
operator|=
name|ngx_pcalloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_spdy_srv_conf_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|sscf
operator|==
name|NULL
condition|)
block|{
return|return
name|NULL
return|;
block|}
name|sscf
operator|->
name|pool_size
operator|=
name|NGX_CONF_UNSET_SIZE
expr_stmt|;
name|sscf
operator|->
name|concurrent_streams
operator|=
name|NGX_CONF_UNSET_UINT
expr_stmt|;
name|sscf
operator|->
name|streams_index_mask
operator|=
name|NGX_CONF_UNSET_UINT
expr_stmt|;
name|sscf
operator|->
name|recv_timeout
operator|=
name|NGX_CONF_UNSET_MSEC
expr_stmt|;
name|sscf
operator|->
name|keepalive_timeout
operator|=
name|NGX_CONF_UNSET_MSEC
expr_stmt|;
name|sscf
operator|->
name|headers_comp
operator|=
name|NGX_CONF_UNSET
expr_stmt|;
return|return
name|sscf
return|;
block|}
end_function

begin_function
specifier|static
name|char
modifier|*
DECL|function|ngx_http_spdy_merge_srv_conf (ngx_conf_t * cf,void * parent,void * child)
name|ngx_http_spdy_merge_srv_conf
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
name|ngx_http_spdy_srv_conf_t
modifier|*
name|prev
init|=
name|parent
decl_stmt|;
name|ngx_http_spdy_srv_conf_t
modifier|*
name|conf
init|=
name|child
decl_stmt|;
name|ngx_conf_merge_size_value
argument_list|(
name|conf
operator|->
name|pool_size
argument_list|,
name|prev
operator|->
name|pool_size
argument_list|,
literal|4096
argument_list|)
expr_stmt|;
name|ngx_conf_merge_uint_value
argument_list|(
name|conf
operator|->
name|concurrent_streams
argument_list|,
name|prev
operator|->
name|concurrent_streams
argument_list|,
literal|100
argument_list|)
expr_stmt|;
name|ngx_conf_merge_uint_value
argument_list|(
name|conf
operator|->
name|streams_index_mask
argument_list|,
name|prev
operator|->
name|streams_index_mask
argument_list|,
literal|32
operator|-
literal|1
argument_list|)
expr_stmt|;
name|ngx_conf_merge_msec_value
argument_list|(
name|conf
operator|->
name|recv_timeout
argument_list|,
name|prev
operator|->
name|recv_timeout
argument_list|,
literal|30000
argument_list|)
expr_stmt|;
name|ngx_conf_merge_msec_value
argument_list|(
name|conf
operator|->
name|keepalive_timeout
argument_list|,
name|prev
operator|->
name|keepalive_timeout
argument_list|,
literal|180000
argument_list|)
expr_stmt|;
name|ngx_conf_merge_value
argument_list|(
name|conf
operator|->
name|headers_comp
argument_list|,
name|prev
operator|->
name|headers_comp
argument_list|,
literal|0
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

begin_function
specifier|static
name|char
modifier|*
DECL|function|ngx_http_spdy_recv_buffer_size (ngx_conf_t * cf,void * post,void * data)
name|ngx_http_spdy_recv_buffer_size
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|void
modifier|*
name|post
parameter_list|,
name|void
modifier|*
name|data
parameter_list|)
block|{
name|size_t
modifier|*
name|sp
init|=
name|data
decl_stmt|;
if|if
condition|(
operator|*
name|sp
operator|<=
literal|2
operator|*
name|NGX_SPDY_STATE_BUFFER_SIZE
condition|)
block|{
return|return
literal|"value is too small"
return|;
block|}
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

begin_function
specifier|static
name|char
modifier|*
DECL|function|ngx_http_spdy_pool_size (ngx_conf_t * cf,void * post,void * data)
name|ngx_http_spdy_pool_size
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|void
modifier|*
name|post
parameter_list|,
name|void
modifier|*
name|data
parameter_list|)
block|{
name|size_t
modifier|*
name|sp
init|=
name|data
decl_stmt|;
if|if
condition|(
operator|*
name|sp
operator|<
name|NGX_MIN_POOL_SIZE
condition|)
block|{
name|ngx_conf_log_error
argument_list|(
name|NGX_LOG_EMERG
argument_list|,
name|cf
argument_list|,
literal|0
argument_list|,
literal|"the pool size must be no less than %uz"
argument_list|,
name|NGX_MIN_POOL_SIZE
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
if|if
condition|(
operator|*
name|sp
operator|%
name|NGX_POOL_ALIGNMENT
condition|)
block|{
name|ngx_conf_log_error
argument_list|(
name|NGX_LOG_EMERG
argument_list|,
name|cf
argument_list|,
literal|0
argument_list|,
literal|"the pool size must be a multiple of %uz"
argument_list|,
name|NGX_POOL_ALIGNMENT
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

begin_function
specifier|static
name|char
modifier|*
DECL|function|ngx_http_spdy_streams_index_mask (ngx_conf_t * cf,void * post,void * data)
name|ngx_http_spdy_streams_index_mask
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|void
modifier|*
name|post
parameter_list|,
name|void
modifier|*
name|data
parameter_list|)
block|{
name|ngx_uint_t
modifier|*
name|np
init|=
name|data
decl_stmt|;
name|ngx_uint_t
name|mask
decl_stmt|;
name|mask
operator|=
operator|*
name|np
operator|-
literal|1
expr_stmt|;
if|if
condition|(
operator|*
name|np
operator|==
literal|0
operator|||
operator|(
operator|*
name|np
operator|&
name|mask
operator|)
condition|)
block|{
return|return
literal|"must be a power of two"
return|;
block|}
operator|*
name|np
operator|=
name|mask
expr_stmt|;
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

end_unit

