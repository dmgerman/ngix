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

begin_comment
comment|/* AF_INET only */
end_comment

begin_typedef
DECL|struct|__anon2a53a7350108
typedef|typedef
struct|struct
block|{
DECL|member|mask
name|in_addr_t
name|mask
decl_stmt|;
DECL|member|addr
name|in_addr_t
name|addr
decl_stmt|;
DECL|member|deny
name|unsigned
name|deny
decl_stmt|;
DECL|typedef|ngx_http_access_rule_t
block|}
name|ngx_http_access_rule_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon2a53a7350208
typedef|typedef
struct|struct
block|{
DECL|member|rules
name|ngx_array_t
modifier|*
name|rules
decl_stmt|;
comment|/* array of ngx_http_access_rule_t */
DECL|typedef|ngx_http_access_loc_conf_t
block|}
name|ngx_http_access_loc_conf_t
typedef|;
end_typedef

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_access_handler
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|char
modifier|*
name|ngx_http_access_rule
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

begin_function_decl
specifier|static
name|void
modifier|*
name|ngx_http_access_create_loc_conf
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
name|ngx_http_access_merge_loc_conf
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
name|ngx_http_access_init
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
function_decl|;
end_function_decl

begin_decl_stmt
DECL|variable|ngx_http_access_commands
specifier|static
name|ngx_command_t
name|ngx_http_access_commands
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"allow"
argument_list|)
block|,
name|NGX_HTTP_MAIN_CONF
operator||
name|NGX_HTTP_SRV_CONF
operator||
name|NGX_HTTP_LOC_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_http_access_rule
block|,
name|NGX_HTTP_LOC_CONF_OFFSET
block|,
literal|0
block|,
name|NULL
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"deny"
argument_list|)
block|,
name|NGX_HTTP_MAIN_CONF
operator||
name|NGX_HTTP_SRV_CONF
operator||
name|NGX_HTTP_LOC_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_http_access_rule
block|,
name|NGX_HTTP_LOC_CONF_OFFSET
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
DECL|variable|ngx_http_access_module_ctx
name|ngx_http_module_t
name|ngx_http_access_module_ctx
init|=
block|{
name|NULL
block|,
comment|/* pre conf */
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
name|ngx_http_access_create_loc_conf
block|,
comment|/* create location configuration */
name|ngx_http_access_merge_loc_conf
comment|/* merge location configuration */
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_access_module
name|ngx_module_t
name|ngx_http_access_module
init|=
block|{
name|NGX_MODULE
block|,
operator|&
name|ngx_http_access_module_ctx
block|,
comment|/* module context */
name|ngx_http_access_commands
block|,
comment|/* module directives */
name|NGX_HTTP_MODULE
block|,
comment|/* module type */
name|ngx_http_access_init
block|,
comment|/* init module */
name|NULL
comment|/* init process */
block|}
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|ngx_http_access_handler (ngx_http_request_t * r)
specifier|static
name|ngx_int_t
name|ngx_http_access_handler
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
block|{
name|ngx_uint_t
name|i
decl_stmt|;
name|struct
name|sockaddr_in
modifier|*
name|sin
decl_stmt|;
name|ngx_http_access_rule_t
modifier|*
name|rule
decl_stmt|;
name|ngx_http_access_loc_conf_t
modifier|*
name|alcf
decl_stmt|;
name|alcf
operator|=
name|ngx_http_get_module_loc_conf
argument_list|(
name|r
argument_list|,
name|ngx_http_access_module
argument_list|)
expr_stmt|;
if|if
condition|(
name|alcf
operator|->
name|rules
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_OK
return|;
block|}
comment|/* AF_INET only */
name|sin
operator|=
operator|(
expr|struct
name|sockaddr_in
operator|*
operator|)
name|r
operator|->
name|connection
operator|->
name|sockaddr
expr_stmt|;
name|rule
operator|=
name|alcf
operator|->
name|rules
operator|->
name|elts
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|alcf
operator|->
name|rules
operator|->
name|nelts
condition|;
name|i
operator|++
control|)
block|{
name|ngx_log_debug3
argument_list|(
name|NGX_LOG_DEBUG_HTTP
argument_list|,
name|r
operator|->
name|connection
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"%08XD %08XD %08XD"
argument_list|,
name|sin
operator|->
name|sin_addr
operator|.
name|s_addr
argument_list|,
name|rule
index|[
name|i
index|]
operator|.
name|mask
argument_list|,
name|rule
index|[
name|i
index|]
operator|.
name|addr
argument_list|)
expr_stmt|;
if|if
condition|(
operator|(
name|sin
operator|->
name|sin_addr
operator|.
name|s_addr
operator|&
name|rule
index|[
name|i
index|]
operator|.
name|mask
operator|)
operator|==
name|rule
index|[
name|i
index|]
operator|.
name|addr
condition|)
block|{
if|if
condition|(
name|rule
index|[
name|i
index|]
operator|.
name|deny
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ERR
argument_list|,
name|r
operator|->
name|connection
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"access forbidden by rule"
argument_list|)
expr_stmt|;
return|return
name|NGX_HTTP_FORBIDDEN
return|;
block|}
return|return
name|NGX_OK
return|;
block|}
block|}
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_access_rule (ngx_conf_t * cf,ngx_command_t * cmd,void * conf)
specifier|static
name|char
modifier|*
name|ngx_http_access_rule
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
name|ngx_http_access_loc_conf_t
modifier|*
name|alcf
init|=
name|conf
decl_stmt|;
name|ngx_str_t
modifier|*
name|value
decl_stmt|;
name|ngx_inet_cidr_t
name|in_cidr
decl_stmt|;
name|ngx_http_access_rule_t
modifier|*
name|rule
decl_stmt|;
if|if
condition|(
name|alcf
operator|->
name|rules
operator|==
name|NULL
condition|)
block|{
name|alcf
operator|->
name|rules
operator|=
name|ngx_create_array
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
literal|4
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_access_rule_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|alcf
operator|->
name|rules
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
block|}
if|if
condition|(
operator|!
operator|(
name|rule
operator|=
name|ngx_push_array
argument_list|(
name|alcf
operator|->
name|rules
argument_list|)
operator|)
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|value
operator|=
name|cf
operator|->
name|args
operator|->
name|elts
expr_stmt|;
name|rule
operator|->
name|deny
operator|=
operator|(
name|value
index|[
literal|0
index|]
operator|.
name|data
index|[
literal|0
index|]
operator|==
literal|'d'
operator|)
condition|?
literal|1
else|:
literal|0
expr_stmt|;
if|if
condition|(
name|value
index|[
literal|1
index|]
operator|.
name|len
operator|==
literal|3
operator|&&
name|ngx_strcmp
argument_list|(
name|value
index|[
literal|1
index|]
operator|.
name|data
argument_list|,
literal|"all"
argument_list|)
operator|==
literal|0
condition|)
block|{
name|rule
operator|->
name|mask
operator|=
literal|0
expr_stmt|;
name|rule
operator|->
name|addr
operator|=
literal|0
expr_stmt|;
return|return
name|NGX_CONF_OK
return|;
block|}
name|rule
operator|->
name|addr
operator|=
name|inet_addr
argument_list|(
operator|(
name|char
operator|*
operator|)
name|value
index|[
literal|1
index|]
operator|.
name|data
argument_list|)
expr_stmt|;
if|if
condition|(
name|rule
operator|->
name|addr
operator|!=
name|INADDR_NONE
condition|)
block|{
name|rule
operator|->
name|mask
operator|=
literal|0xffffffff
expr_stmt|;
return|return
name|NGX_CONF_OK
return|;
block|}
if|if
condition|(
name|ngx_ptocidr
argument_list|(
operator|&
name|value
index|[
literal|1
index|]
argument_list|,
operator|&
name|in_cidr
argument_list|)
operator|==
name|NGX_ERROR
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
literal|"invalid parameter \"%V\""
argument_list|,
operator|&
name|value
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|rule
operator|->
name|mask
operator|=
name|in_cidr
operator|.
name|mask
expr_stmt|;
name|rule
operator|->
name|addr
operator|=
name|in_cidr
operator|.
name|addr
expr_stmt|;
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_access_create_loc_conf (ngx_conf_t * cf)
specifier|static
name|void
modifier|*
name|ngx_http_access_create_loc_conf
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
block|{
name|ngx_http_access_loc_conf_t
modifier|*
name|conf
decl_stmt|;
if|if
condition|(
operator|!
operator|(
name|conf
operator|=
name|ngx_pcalloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_access_loc_conf_t
argument_list|)
argument_list|)
operator|)
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
return|return
name|conf
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_access_merge_loc_conf (ngx_conf_t * cf,void * parent,void * child)
specifier|static
name|char
modifier|*
name|ngx_http_access_merge_loc_conf
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
name|ngx_http_access_loc_conf_t
modifier|*
name|prev
init|=
name|parent
decl_stmt|;
name|ngx_http_access_loc_conf_t
modifier|*
name|conf
init|=
name|child
decl_stmt|;
if|if
condition|(
name|conf
operator|->
name|rules
operator|==
name|NULL
condition|)
block|{
name|conf
operator|->
name|rules
operator|=
name|prev
operator|->
name|rules
expr_stmt|;
block|}
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_access_init (ngx_cycle_t * cycle)
specifier|static
name|ngx_int_t
name|ngx_http_access_init
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
block|{
name|ngx_http_handler_pt
modifier|*
name|h
decl_stmt|;
name|ngx_http_core_main_conf_t
modifier|*
name|cmcf
decl_stmt|;
name|cmcf
operator|=
name|ngx_http_cycle_get_module_main_conf
argument_list|(
name|cycle
argument_list|,
name|ngx_http_core_module
argument_list|)
expr_stmt|;
name|h
operator|=
name|ngx_push_array
argument_list|(
operator|&
name|cmcf
operator|->
name|phases
index|[
name|NGX_HTTP_ACCESS_PHASE
index|]
operator|.
name|handlers
argument_list|)
expr_stmt|;
if|if
condition|(
name|h
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
operator|*
name|h
operator|=
name|ngx_http_access_handler
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

end_unit

