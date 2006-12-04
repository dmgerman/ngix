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
file|<ngx_mysql.h>
end_include

begin_include
include|#
directive|include
file|<ngx_http.h>
end_include

begin_typedef
DECL|struct|__anon2b7bd74f0108
typedef|typedef
struct|struct
block|{
DECL|member|peers
name|ngx_peer_addr_t
modifier|*
name|peers
decl_stmt|;
DECL|member|npeers
name|ngx_uint_t
name|npeers
decl_stmt|;
DECL|typedef|ngx_http_mysql_test_conf_t
block|}
name|ngx_http_mysql_test_conf_t
typedef|;
end_typedef

begin_function_decl
specifier|static
name|void
name|ngx_http_mysql_auth
parameter_list|(
name|ngx_mysql_t
modifier|*
name|m
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|void
name|ngx_http_mysql_done
parameter_list|(
name|ngx_mysql_t
modifier|*
name|m
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|void
modifier|*
name|ngx_http_mysql_test_create_loc_conf
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
name|ngx_http_mysql_test
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
DECL|variable|ngx_http_mysql_test_commands
specifier|static
name|ngx_command_t
name|ngx_http_mysql_test_commands
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"mysql_test"
argument_list|)
block|,
name|NGX_HTTP_LOC_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_http_mysql_test
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
DECL|variable|ngx_http_mysql_test_module_ctx
specifier|static
name|ngx_http_module_t
name|ngx_http_mysql_test_module_ctx
init|=
block|{
name|NULL
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
name|ngx_http_mysql_test_create_loc_conf
block|,
comment|/* create location configuration */
name|NULL
comment|/* merge location configuration */
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_mysql_test_module
name|ngx_module_t
name|ngx_http_mysql_test_module
init|=
block|{
name|NGX_MODULE_V1
block|,
operator|&
name|ngx_http_mysql_test_module_ctx
block|,
comment|/* module context */
name|ngx_http_mysql_test_commands
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
DECL|variable|ngx_mysql_login
specifier|static
name|ngx_str_t
name|ngx_mysql_login
init|=
name|ngx_string
argument_list|(
literal|"root"
argument_list|)
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_mysql_passwd
specifier|static
name|ngx_str_t
name|ngx_mysql_passwd
init|=
name|ngx_string
argument_list|(
literal|"tp"
argument_list|)
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_mysql_database
specifier|static
name|ngx_str_t
name|ngx_mysql_database
init|=
name|ngx_string
argument_list|(
literal|"mysql"
argument_list|)
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_mysql_command_query
specifier|static
name|ngx_str_t
name|ngx_mysql_command_query
init|=
name|ngx_string
argument_list|(
literal|"select * from user"
argument_list|)
decl_stmt|;
end_decl_stmt

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_mysql_test_handler (ngx_http_request_t * r)
name|ngx_http_mysql_test_handler
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
block|{
name|ngx_int_t
name|rc
decl_stmt|;
name|ngx_mysql_t
modifier|*
name|m
decl_stmt|;
name|ngx_http_mysql_test_conf_t
modifier|*
name|mtcf
decl_stmt|;
name|mtcf
operator|=
name|ngx_http_get_module_loc_conf
argument_list|(
name|r
argument_list|,
name|ngx_http_mysql_test_module
argument_list|)
expr_stmt|;
name|m
operator|=
name|ngx_pcalloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_mysql_t
argument_list|)
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
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
name|m
operator|->
name|pool
operator|=
name|r
operator|->
name|pool
expr_stmt|;
name|m
operator|->
name|handler
operator|=
name|ngx_http_mysql_auth
expr_stmt|;
name|m
operator|->
name|data
operator|=
name|r
expr_stmt|;
name|m
operator|->
name|login
operator|=
operator|&
name|ngx_mysql_login
expr_stmt|;
name|m
operator|->
name|passwd
operator|=
operator|&
name|ngx_mysql_passwd
expr_stmt|;
name|m
operator|->
name|database
operator|=
operator|&
name|ngx_mysql_database
expr_stmt|;
comment|/* STUB */
name|m
operator|->
name|peer
operator|.
name|sockaddr
operator|=
name|mtcf
operator|->
name|peers
index|[
literal|0
index|]
operator|.
name|sockaddr
expr_stmt|;
name|m
operator|->
name|peer
operator|.
name|socklen
operator|=
name|mtcf
operator|->
name|peers
index|[
literal|0
index|]
operator|.
name|socklen
expr_stmt|;
name|m
operator|->
name|peer
operator|.
name|name
operator|=
operator|&
name|mtcf
operator|->
name|peers
index|[
literal|0
index|]
operator|.
name|name
expr_stmt|;
name|m
operator|->
name|peer
operator|.
name|tries
operator|=
name|mtcf
operator|->
name|npeers
expr_stmt|;
name|m
operator|->
name|peer
operator|.
name|get
operator|=
name|ngx_event_get_peer
expr_stmt|;
comment|/**/
name|m
operator|->
name|peer
operator|.
name|log
operator|=
name|r
operator|->
name|connection
operator|->
name|log
expr_stmt|;
name|m
operator|->
name|peer
operator|.
name|log_error
operator|=
name|NGX_ERROR_ERR
expr_stmt|;
name|rc
operator|=
name|ngx_mysql_connect
argument_list|(
name|m
argument_list|)
expr_stmt|;
if|if
condition|(
name|rc
operator|==
name|NGX_OK
operator|||
name|rc
operator|==
name|NGX_AGAIN
condition|)
block|{
return|return
name|NGX_DONE
return|;
block|}
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
end_function

begin_function
specifier|static
name|void
DECL|function|ngx_http_mysql_auth (ngx_mysql_t * m)
name|ngx_http_mysql_auth
parameter_list|(
name|ngx_mysql_t
modifier|*
name|m
parameter_list|)
block|{
name|ngx_http_request_t
modifier|*
name|r
decl_stmt|;
name|r
operator|=
name|m
operator|->
name|data
expr_stmt|;
if|if
condition|(
name|m
operator|->
name|state
operator|!=
name|NGX_OK
condition|)
block|{
name|ngx_http_finalize_request
argument_list|(
name|r
argument_list|,
name|NGX_HTTP_NO_CONTENT
argument_list|)
expr_stmt|;
return|return;
block|}
name|m
operator|->
name|query
operator|.
name|len
operator|=
name|NGX_MYSQL_CMDPKT_LEN
operator|+
name|ngx_mysql_command_query
operator|.
name|len
expr_stmt|;
name|m
operator|->
name|query
operator|.
name|data
operator|=
name|ngx_palloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
name|m
operator|->
name|query
operator|.
name|len
argument_list|)
expr_stmt|;
if|if
condition|(
name|m
operator|->
name|query
operator|.
name|data
operator|==
name|NULL
condition|)
block|{
name|ngx_http_finalize_request
argument_list|(
name|r
argument_list|,
name|NGX_HTTP_INTERNAL_SERVER_ERROR
argument_list|)
expr_stmt|;
return|return;
block|}
name|ngx_memcpy
argument_list|(
name|m
operator|->
name|query
operator|.
name|data
operator|+
name|NGX_MYSQL_CMDPKT_LEN
argument_list|,
name|ngx_mysql_command_query
operator|.
name|data
argument_list|,
name|ngx_mysql_command_query
operator|.
name|len
argument_list|)
expr_stmt|;
name|m
operator|->
name|handler
operator|=
name|ngx_http_mysql_done
expr_stmt|;
name|ngx_mysql_query
argument_list|(
name|m
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
specifier|static
name|void
DECL|function|ngx_http_mysql_done (ngx_mysql_t * m)
name|ngx_http_mysql_done
parameter_list|(
name|ngx_mysql_t
modifier|*
name|m
parameter_list|)
block|{
name|ngx_http_request_t
modifier|*
name|r
decl_stmt|;
name|r
operator|=
name|m
operator|->
name|data
expr_stmt|;
name|ngx_http_finalize_request
argument_list|(
name|r
argument_list|,
name|NGX_HTTP_NO_CONTENT
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
specifier|static
name|void
modifier|*
DECL|function|ngx_http_mysql_test_create_loc_conf (ngx_conf_t * cf)
name|ngx_http_mysql_test_create_loc_conf
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
block|{
name|ngx_http_mysql_test_conf_t
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
name|ngx_http_mysql_test_conf_t
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
return|return
name|conf
return|;
block|}
end_function

begin_function
specifier|static
name|char
modifier|*
DECL|function|ngx_http_mysql_test (ngx_conf_t * cf,ngx_command_t * cmd,void * conf)
name|ngx_http_mysql_test
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
name|ngx_http_mysql_test_conf_t
modifier|*
name|mtcf
init|=
name|conf
decl_stmt|;
name|ngx_str_t
modifier|*
name|value
decl_stmt|;
name|ngx_url_t
name|u
decl_stmt|;
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
name|ngx_http_mysql_test_handler
expr_stmt|;
name|value
operator|=
name|cf
operator|->
name|args
operator|->
name|elts
expr_stmt|;
name|ngx_memzero
argument_list|(
operator|&
name|u
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_url_t
argument_list|)
argument_list|)
expr_stmt|;
name|u
operator|.
name|url
operator|=
name|value
index|[
literal|1
index|]
expr_stmt|;
name|u
operator|.
name|default_portn
operator|=
literal|3306
expr_stmt|;
if|if
condition|(
name|ngx_parse_url
argument_list|(
name|cf
argument_list|,
operator|&
name|u
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
if|if
condition|(
name|u
operator|.
name|err
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
literal|"%s in upstream \"%V\""
argument_list|,
name|u
operator|.
name|err
argument_list|,
operator|&
name|u
operator|.
name|url
argument_list|)
expr_stmt|;
block|}
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|mtcf
operator|->
name|peers
operator|=
name|u
operator|.
name|addrs
expr_stmt|;
name|mtcf
operator|->
name|npeers
operator|=
name|u
operator|.
name|naddrs
expr_stmt|;
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

end_unit

