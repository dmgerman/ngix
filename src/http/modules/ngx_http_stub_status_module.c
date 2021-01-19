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
file|<ngx_http.h>
end_include

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_stub_status_handler
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_stub_status_variable
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
name|ngx_http_stub_status_add_variables
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
name|ngx_http_set_stub_status
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|ngx_command_t
modifier|*
name|cmd
parameter_list|,
name|void
modifier|*
name|conf
parameter_list|)
function_decl|;
end_function_decl

begin_decl_stmt
DECL|variable|ngx_http_status_commands
specifier|static
name|ngx_command_t
name|ngx_http_status_commands
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"stub_status"
argument_list|)
block|,
name|NGX_HTTP_SRV_CONF
operator||
name|NGX_HTTP_LOC_CONF
operator||
name|NGX_CONF_NOARGS
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_http_set_stub_status
block|,
literal|0
block|,
literal|0
block|,
name|NULL
block|}
block|,
name|ngx_null_command
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_stub_status_module_ctx
specifier|static
name|ngx_http_module_t
name|ngx_http_stub_status_module_ctx
init|=
block|{
name|ngx_http_stub_status_add_variables
block|,
comment|/* preconfiguration */
name|NULL
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
name|NULL
block|,
comment|/* create location configuration */
name|NULL
comment|/* merge location configuration */
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_stub_status_module
name|ngx_module_t
name|ngx_http_stub_status_module
init|=
block|{
name|NGX_MODULE_V1
block|,
operator|&
name|ngx_http_stub_status_module_ctx
block|,
comment|/* module context */
name|ngx_http_status_commands
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
DECL|variable|ngx_http_stub_status_vars
specifier|static
name|ngx_http_variable_t
name|ngx_http_stub_status_vars
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"connections_active"
argument_list|)
block|,
name|NULL
block|,
name|ngx_http_stub_status_variable
block|,
literal|0
block|,
name|NGX_HTTP_VAR_NOCACHEABLE
block|,
literal|0
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"connections_reading"
argument_list|)
block|,
name|NULL
block|,
name|ngx_http_stub_status_variable
block|,
literal|1
block|,
name|NGX_HTTP_VAR_NOCACHEABLE
block|,
literal|0
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"connections_writing"
argument_list|)
block|,
name|NULL
block|,
name|ngx_http_stub_status_variable
block|,
literal|2
block|,
name|NGX_HTTP_VAR_NOCACHEABLE
block|,
literal|0
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"connections_waiting"
argument_list|)
block|,
name|NULL
block|,
name|ngx_http_stub_status_variable
block|,
literal|3
block|,
name|NGX_HTTP_VAR_NOCACHEABLE
block|,
literal|0
block|}
block|,
name|ngx_http_null_variable
block|}
decl_stmt|;
end_decl_stmt

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_stub_status_handler (ngx_http_request_t * r)
name|ngx_http_stub_status_handler
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
block|{
name|size_t
name|size
decl_stmt|;
name|ngx_int_t
name|rc
decl_stmt|;
name|ngx_buf_t
modifier|*
name|b
decl_stmt|;
name|ngx_chain_t
name|out
decl_stmt|;
name|ngx_atomic_int_t
name|ap
decl_stmt|,
name|hn
decl_stmt|,
name|ac
decl_stmt|,
name|rq
decl_stmt|,
name|rd
decl_stmt|,
name|wr
decl_stmt|,
name|wa
decl_stmt|;
if|if
condition|(
operator|!
operator|(
name|r
operator|->
name|method
operator|&
operator|(
name|NGX_HTTP_GET
operator||
name|NGX_HTTP_HEAD
operator|)
operator|)
condition|)
block|{
return|return
name|NGX_HTTP_NOT_ALLOWED
return|;
block|}
name|rc
operator|=
name|ngx_http_discard_request_body
argument_list|(
name|r
argument_list|)
expr_stmt|;
if|if
condition|(
name|rc
operator|!=
name|NGX_OK
condition|)
block|{
return|return
name|rc
return|;
block|}
name|r
operator|->
name|headers_out
operator|.
name|content_type_len
operator|=
sizeof|sizeof
argument_list|(
literal|"text/plain"
argument_list|)
operator|-
literal|1
expr_stmt|;
name|ngx_str_set
argument_list|(
operator|&
name|r
operator|->
name|headers_out
operator|.
name|content_type
argument_list|,
literal|"text/plain"
argument_list|)
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|content_type_lowcase
operator|=
name|NULL
expr_stmt|;
name|size
operator|=
sizeof|sizeof
argument_list|(
literal|"Active connections:  \n"
argument_list|)
operator|+
name|NGX_ATOMIC_T_LEN
operator|+
sizeof|sizeof
argument_list|(
literal|"server accepts handled requests\n"
argument_list|)
operator|-
literal|1
operator|+
literal|6
operator|+
literal|3
operator|*
name|NGX_ATOMIC_T_LEN
operator|+
sizeof|sizeof
argument_list|(
literal|"Reading:  Writing:  Waiting:  \n"
argument_list|)
operator|+
literal|3
operator|*
name|NGX_ATOMIC_T_LEN
expr_stmt|;
name|b
operator|=
name|ngx_create_temp_buf
argument_list|(
name|r
operator|->
name|pool
argument_list|,
name|size
argument_list|)
expr_stmt|;
if|if
condition|(
name|b
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
name|out
operator|.
name|buf
operator|=
name|b
expr_stmt|;
name|out
operator|.
name|next
operator|=
name|NULL
expr_stmt|;
name|ap
operator|=
operator|*
name|ngx_stat_accepted
expr_stmt|;
name|hn
operator|=
operator|*
name|ngx_stat_handled
expr_stmt|;
name|ac
operator|=
operator|*
name|ngx_stat_active
expr_stmt|;
name|rq
operator|=
operator|*
name|ngx_stat_requests
expr_stmt|;
name|rd
operator|=
operator|*
name|ngx_stat_reading
expr_stmt|;
name|wr
operator|=
operator|*
name|ngx_stat_writing
expr_stmt|;
name|wa
operator|=
operator|*
name|ngx_stat_waiting
expr_stmt|;
name|b
operator|->
name|last
operator|=
name|ngx_sprintf
argument_list|(
name|b
operator|->
name|last
argument_list|,
literal|"Active connections: %uA \n"
argument_list|,
name|ac
argument_list|)
expr_stmt|;
name|b
operator|->
name|last
operator|=
name|ngx_cpymem
argument_list|(
name|b
operator|->
name|last
argument_list|,
literal|"server accepts handled requests\n"
argument_list|,
sizeof|sizeof
argument_list|(
literal|"server accepts handled requests\n"
argument_list|)
operator|-
literal|1
argument_list|)
expr_stmt|;
name|b
operator|->
name|last
operator|=
name|ngx_sprintf
argument_list|(
name|b
operator|->
name|last
argument_list|,
literal|" %uA %uA %uA \n"
argument_list|,
name|ap
argument_list|,
name|hn
argument_list|,
name|rq
argument_list|)
expr_stmt|;
name|b
operator|->
name|last
operator|=
name|ngx_sprintf
argument_list|(
name|b
operator|->
name|last
argument_list|,
literal|"Reading: %uA Writing: %uA Waiting: %uA \n"
argument_list|,
name|rd
argument_list|,
name|wr
argument_list|,
name|wa
argument_list|)
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|status
operator|=
name|NGX_HTTP_OK
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|content_length_n
operator|=
name|b
operator|->
name|last
operator|-
name|b
operator|->
name|pos
expr_stmt|;
name|b
operator|->
name|last_buf
operator|=
operator|(
name|r
operator|==
name|r
operator|->
expr|main
operator|)
condition|?
literal|1
else|:
literal|0
expr_stmt|;
name|b
operator|->
name|last_in_chain
operator|=
literal|1
expr_stmt|;
name|rc
operator|=
name|ngx_http_send_header
argument_list|(
name|r
argument_list|)
expr_stmt|;
if|if
condition|(
name|rc
operator|==
name|NGX_ERROR
operator|||
name|rc
operator|>
name|NGX_OK
operator|||
name|r
operator|->
name|header_only
condition|)
block|{
return|return
name|rc
return|;
block|}
return|return
name|ngx_http_output_filter
argument_list|(
name|r
argument_list|,
operator|&
name|out
argument_list|)
return|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_stub_status_variable (ngx_http_request_t * r,ngx_http_variable_value_t * v,uintptr_t data)
name|ngx_http_stub_status_variable
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
name|u_char
modifier|*
name|p
decl_stmt|;
name|ngx_atomic_int_t
name|value
decl_stmt|;
name|p
operator|=
name|ngx_pnalloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
name|NGX_ATOMIC_T_LEN
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
name|NGX_ERROR
return|;
block|}
switch|switch
condition|(
name|data
condition|)
block|{
case|case
literal|0
case|:
name|value
operator|=
operator|*
name|ngx_stat_active
expr_stmt|;
break|break;
case|case
literal|1
case|:
name|value
operator|=
operator|*
name|ngx_stat_reading
expr_stmt|;
break|break;
case|case
literal|2
case|:
name|value
operator|=
operator|*
name|ngx_stat_writing
expr_stmt|;
break|break;
case|case
literal|3
case|:
name|value
operator|=
operator|*
name|ngx_stat_waiting
expr_stmt|;
break|break;
comment|/* suppress warning */
default|default:
name|value
operator|=
literal|0
expr_stmt|;
break|break;
block|}
name|v
operator|->
name|len
operator|=
name|ngx_sprintf
argument_list|(
name|p
argument_list|,
literal|"%uA"
argument_list|,
name|value
argument_list|)
operator|-
name|p
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
name|p
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_stub_status_add_variables (ngx_conf_t * cf)
name|ngx_http_stub_status_add_variables
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
name|ngx_http_stub_status_vars
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
name|char
modifier|*
DECL|function|ngx_http_set_stub_status (ngx_conf_t * cf,ngx_command_t * cmd,void * conf)
name|ngx_http_set_stub_status
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|ngx_command_t
modifier|*
name|cmd
parameter_list|,
name|void
modifier|*
name|conf
parameter_list|)
block|{
name|ngx_http_core_loc_conf_t
modifier|*
name|clcf
decl_stmt|;
name|clcf
operator|=
name|ngx_http_conf_get_module_loc_conf
argument_list|(
name|cf
argument_list|,
name|ngx_http_core_module
argument_list|)
expr_stmt|;
name|clcf
operator|->
name|handler
operator|=
name|ngx_http_stub_status_handler
expr_stmt|;
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

end_unit

