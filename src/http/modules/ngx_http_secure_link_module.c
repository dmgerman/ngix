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

begin_include
include|#
directive|include
file|<ngx_md5.h>
end_include

begin_typedef
DECL|struct|__anon2774e5870108
typedef|typedef
struct|struct
block|{
DECL|member|secret
name|ngx_str_t
name|secret
decl_stmt|;
DECL|typedef|ngx_http_secure_link_conf_t
block|}
name|ngx_http_secure_link_conf_t
typedef|;
end_typedef

begin_function_decl
specifier|static
name|void
modifier|*
name|ngx_http_secure_link_create_conf
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
name|ngx_http_secure_link_merge_conf
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
name|ngx_http_secure_link_add_variables
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
function_decl|;
end_function_decl

begin_decl_stmt
DECL|variable|ngx_http_secure_link_commands
specifier|static
name|ngx_command_t
name|ngx_http_secure_link_commands
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"secure_link_secret"
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
name|ngx_conf_set_str_slot
block|,
name|NGX_HTTP_LOC_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_http_secure_link_conf_t
argument_list|,
name|secret
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
DECL|variable|ngx_http_secure_link_module_ctx
specifier|static
name|ngx_http_module_t
name|ngx_http_secure_link_module_ctx
init|=
block|{
name|ngx_http_secure_link_add_variables
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
name|ngx_http_secure_link_create_conf
block|,
comment|/* create location configuration */
name|ngx_http_secure_link_merge_conf
comment|/* merge location configuration */
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_secure_link_module
name|ngx_module_t
name|ngx_http_secure_link_module
init|=
block|{
name|NGX_MODULE_V1
block|,
operator|&
name|ngx_http_secure_link_module_ctx
block|,
comment|/* module context */
name|ngx_http_secure_link_commands
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
DECL|variable|ngx_http_secure_link
specifier|static
name|ngx_str_t
name|ngx_http_secure_link
init|=
name|ngx_string
argument_list|(
literal|"secure_link"
argument_list|)
decl_stmt|;
end_decl_stmt

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_secure_link_variable (ngx_http_request_t * r,ngx_http_variable_value_t * v,uintptr_t data)
name|ngx_http_secure_link_variable
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
decl_stmt|,
modifier|*
name|start
decl_stmt|,
modifier|*
name|end
decl_stmt|,
modifier|*
name|last
decl_stmt|;
name|size_t
name|len
decl_stmt|;
name|ngx_int_t
name|n
decl_stmt|;
name|ngx_uint_t
name|i
decl_stmt|;
name|ngx_md5_t
name|md5
decl_stmt|;
name|ngx_http_secure_link_conf_t
modifier|*
name|conf
decl_stmt|;
name|u_char
name|hash
index|[
literal|16
index|]
decl_stmt|;
name|conf
operator|=
name|ngx_http_get_module_loc_conf
argument_list|(
name|r
argument_list|,
name|ngx_http_secure_link_module
argument_list|)
expr_stmt|;
if|if
condition|(
name|conf
operator|->
name|secret
operator|.
name|len
operator|==
literal|0
condition|)
block|{
goto|goto
name|not_found
goto|;
block|}
name|p
operator|=
operator|&
name|r
operator|->
name|unparsed_uri
operator|.
name|data
index|[
literal|1
index|]
expr_stmt|;
name|last
operator|=
name|r
operator|->
name|unparsed_uri
operator|.
name|data
operator|+
name|r
operator|->
name|unparsed_uri
operator|.
name|len
expr_stmt|;
while|while
condition|(
name|p
operator|<
name|last
condition|)
block|{
if|if
condition|(
operator|*
name|p
operator|++
operator|==
literal|'/'
condition|)
block|{
name|start
operator|=
name|p
expr_stmt|;
goto|goto
name|md5_start
goto|;
block|}
block|}
goto|goto
name|not_found
goto|;
name|md5_start
label|:
while|while
condition|(
name|p
operator|<
name|last
condition|)
block|{
if|if
condition|(
operator|*
name|p
operator|++
operator|==
literal|'/'
condition|)
block|{
name|end
operator|=
name|p
operator|-
literal|1
expr_stmt|;
goto|goto
name|url_start
goto|;
block|}
block|}
goto|goto
name|not_found
goto|;
name|url_start
label|:
name|len
operator|=
name|last
operator|-
name|p
expr_stmt|;
if|if
condition|(
name|end
operator|-
name|start
operator|!=
literal|32
operator|||
name|len
operator|<
literal|3
condition|)
block|{
goto|goto
name|not_found
goto|;
block|}
name|ngx_md5_init
argument_list|(
operator|&
name|md5
argument_list|)
expr_stmt|;
name|ngx_md5_update
argument_list|(
operator|&
name|md5
argument_list|,
name|p
argument_list|,
name|len
argument_list|)
expr_stmt|;
name|ngx_md5_update
argument_list|(
operator|&
name|md5
argument_list|,
name|conf
operator|->
name|secret
operator|.
name|data
argument_list|,
name|conf
operator|->
name|secret
operator|.
name|len
argument_list|)
expr_stmt|;
name|ngx_md5_final
argument_list|(
name|hash
argument_list|,
operator|&
name|md5
argument_list|)
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
literal|16
condition|;
name|i
operator|++
control|)
block|{
name|n
operator|=
name|ngx_hextoi
argument_list|(
operator|&
name|start
index|[
literal|2
operator|*
name|i
index|]
argument_list|,
literal|2
argument_list|)
expr_stmt|;
if|if
condition|(
name|n
operator|==
name|NGX_ERROR
operator|||
name|n
operator|!=
name|hash
index|[
name|i
index|]
condition|)
block|{
goto|goto
name|not_found
goto|;
block|}
block|}
name|v
operator|->
name|len
operator|=
name|len
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
name|not_found
label|:
name|v
operator|->
name|not_found
operator|=
literal|1
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
DECL|function|ngx_http_secure_link_create_conf (ngx_conf_t * cf)
name|ngx_http_secure_link_create_conf
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
block|{
name|ngx_http_secure_link_conf_t
modifier|*
name|conf
decl_stmt|;
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
name|ngx_http_secure_link_conf_t
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
name|NGX_CONF_ERROR
return|;
block|}
comment|/*      * set by ngx_pcalloc():      *      *     conf->secret = { 0, NULL }      */
return|return
name|conf
return|;
block|}
end_function

begin_function
specifier|static
name|char
modifier|*
DECL|function|ngx_http_secure_link_merge_conf (ngx_conf_t * cf,void * parent,void * child)
name|ngx_http_secure_link_merge_conf
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
name|ngx_http_secure_link_conf_t
modifier|*
name|prev
init|=
name|parent
decl_stmt|;
name|ngx_http_secure_link_conf_t
modifier|*
name|conf
init|=
name|child
decl_stmt|;
name|ngx_conf_merge_str_value
argument_list|(
name|conf
operator|->
name|secret
argument_list|,
name|prev
operator|->
name|secret
argument_list|,
literal|""
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_secure_link_add_variables (ngx_conf_t * cf)
name|ngx_http_secure_link_add_variables
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
block|{
name|ngx_http_variable_t
modifier|*
name|var
decl_stmt|;
name|var
operator|=
name|ngx_http_add_variable
argument_list|(
name|cf
argument_list|,
operator|&
name|ngx_http_secure_link
argument_list|,
name|NGX_HTTP_VAR_NOHASH
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
name|ngx_http_secure_link_variable
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

end_unit

