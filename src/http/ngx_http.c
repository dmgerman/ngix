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
file|<ngx_event.h>
end_include

begin_include
include|#
directive|include
file|<ngx_http.h>
end_include

begin_function_decl
specifier|static
name|char
modifier|*
name|ngx_http_block
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
DECL|variable|ngx_http_max_module
name|int
name|ngx_http_max_module
decl_stmt|;
end_decl_stmt

begin_function_decl
DECL|variable|ngx_http_top_header_filter
name|int
function_decl|(
modifier|*
name|ngx_http_top_header_filter
function_decl|)
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
DECL|variable|ngx_http_top_body_filter
name|int
function_decl|(
modifier|*
name|ngx_http_top_body_filter
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

begin_decl_stmt
DECL|variable|http_name
specifier|static
name|ngx_str_t
name|http_name
init|=
name|ngx_string
argument_list|(
literal|"http"
argument_list|)
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_commands
specifier|static
name|ngx_command_t
name|ngx_http_commands
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"http"
argument_list|)
block|,
name|NGX_MAIN_CONF
operator||
name|NGX_CONF_BLOCK
operator||
name|NGX_CONF_NOARGS
block|,
name|ngx_http_block
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
DECL|variable|ngx_http_module
name|ngx_module_t
name|ngx_http_module
init|=
block|{
name|NGX_MODULE
block|,
operator|&
name|http_name
block|,
comment|/* module context */
name|ngx_http_commands
block|,
comment|/* module directives */
name|NGX_CORE_MODULE
block|,
comment|/* module type */
name|NULL
comment|/* init module */
block|}
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|ngx_http_block (ngx_conf_t * cf,ngx_command_t * cmd,void * conf)
specifier|static
name|char
modifier|*
name|ngx_http_block
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
name|int
name|mi
decl_stmt|,
name|m
decl_stmt|,
name|s
decl_stmt|,
name|l
decl_stmt|,
name|p
decl_stmt|,
name|a
decl_stmt|,
name|n
decl_stmt|;
name|int
name|port_found
decl_stmt|,
name|addr_found
decl_stmt|,
name|virtual_names
decl_stmt|;
name|char
modifier|*
name|rv
decl_stmt|;
name|struct
name|sockaddr_in
modifier|*
name|addr_in
decl_stmt|;
name|ngx_array_t
name|in_ports
decl_stmt|;
name|ngx_listening_t
modifier|*
name|ls
decl_stmt|;
name|ngx_http_module_t
modifier|*
name|module
decl_stmt|;
name|ngx_conf_t
name|pcf
decl_stmt|;
name|ngx_http_conf_ctx_t
modifier|*
name|ctx
decl_stmt|;
name|ngx_http_in_port_t
modifier|*
name|in_port
decl_stmt|,
modifier|*
name|inport
decl_stmt|;
name|ngx_http_in_addr_t
modifier|*
name|in_addr
decl_stmt|,
modifier|*
name|inaddr
decl_stmt|;
name|ngx_http_core_main_conf_t
modifier|*
name|cmcf
decl_stmt|;
name|ngx_http_core_srv_conf_t
modifier|*
modifier|*
name|cscfp
decl_stmt|,
modifier|*
name|cscf
decl_stmt|;
name|ngx_http_core_loc_conf_t
modifier|*
modifier|*
name|clcfp
decl_stmt|;
name|ngx_http_listen_t
modifier|*
name|lscf
decl_stmt|;
name|ngx_http_server_name_t
modifier|*
name|s_name
decl_stmt|,
modifier|*
name|name
decl_stmt|;
if|#
directive|if
operator|(
name|WIN32
operator|)
name|ngx_iocp_conf_t
modifier|*
name|iocpcf
decl_stmt|;
endif|#
directive|endif
comment|/* the main http context */
name|ngx_test_null
argument_list|(
name|ctx
argument_list|,
name|ngx_pcalloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_conf_ctx_t
argument_list|)
argument_list|)
argument_list|,
name|NGX_CONF_ERROR
argument_list|)
expr_stmt|;
operator|*
operator|(
name|ngx_http_conf_ctx_t
operator|*
operator|*
operator|)
name|conf
operator|=
name|ctx
expr_stmt|;
comment|/* count the number of the http modules and set up their indices */
name|ngx_http_max_module
operator|=
literal|0
expr_stmt|;
for|for
control|(
name|m
operator|=
literal|0
init|;
name|ngx_modules
index|[
name|m
index|]
condition|;
name|m
operator|++
control|)
block|{
if|if
condition|(
name|ngx_modules
index|[
name|m
index|]
operator|->
name|type
operator|!=
name|NGX_HTTP_MODULE
condition|)
block|{
continue|continue;
block|}
name|ngx_modules
index|[
name|m
index|]
operator|->
name|ctx_index
operator|=
name|ngx_http_max_module
operator|++
expr_stmt|;
block|}
comment|/* the main http main_conf, it's the same in the all http contexts */
name|ngx_test_null
argument_list|(
name|ctx
operator|->
name|main_conf
argument_list|,
name|ngx_pcalloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|void
operator|*
argument_list|)
operator|*
name|ngx_http_max_module
argument_list|)
argument_list|,
name|NGX_CONF_ERROR
argument_list|)
expr_stmt|;
comment|/* the http null srv_conf, it's used to merge the server{}s' srv_conf's */
name|ngx_test_null
argument_list|(
name|ctx
operator|->
name|srv_conf
argument_list|,
name|ngx_pcalloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|void
operator|*
argument_list|)
operator|*
name|ngx_http_max_module
argument_list|)
argument_list|,
name|NGX_CONF_ERROR
argument_list|)
expr_stmt|;
comment|/* the http null loc_conf, it's used to merge the server{}s' loc_conf's */
name|ngx_test_null
argument_list|(
name|ctx
operator|->
name|loc_conf
argument_list|,
name|ngx_pcalloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|void
operator|*
argument_list|)
operator|*
name|ngx_http_max_module
argument_list|)
argument_list|,
name|NGX_CONF_ERROR
argument_list|)
expr_stmt|;
comment|/* create the main_conf, srv_conf and loc_conf in all http modules */
for|for
control|(
name|m
operator|=
literal|0
init|;
name|ngx_modules
index|[
name|m
index|]
condition|;
name|m
operator|++
control|)
block|{
if|if
condition|(
name|ngx_modules
index|[
name|m
index|]
operator|->
name|type
operator|!=
name|NGX_HTTP_MODULE
condition|)
block|{
continue|continue;
block|}
name|module
operator|=
name|ngx_modules
index|[
name|m
index|]
operator|->
name|ctx
expr_stmt|;
name|mi
operator|=
name|ngx_modules
index|[
name|m
index|]
operator|->
name|ctx_index
expr_stmt|;
if|if
condition|(
name|module
operator|->
name|create_main_conf
condition|)
block|{
name|ngx_test_null
argument_list|(
name|ctx
operator|->
name|main_conf
index|[
name|mi
index|]
argument_list|,
name|module
operator|->
name|create_main_conf
argument_list|(
name|cf
operator|->
name|pool
argument_list|)
argument_list|,
name|NGX_CONF_ERROR
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|module
operator|->
name|create_srv_conf
condition|)
block|{
name|ngx_test_null
argument_list|(
name|ctx
operator|->
name|srv_conf
index|[
name|mi
index|]
argument_list|,
name|module
operator|->
name|create_srv_conf
argument_list|(
name|cf
operator|->
name|pool
argument_list|)
argument_list|,
name|NGX_CONF_ERROR
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|module
operator|->
name|create_loc_conf
condition|)
block|{
name|ngx_test_null
argument_list|(
name|ctx
operator|->
name|loc_conf
index|[
name|mi
index|]
argument_list|,
name|module
operator|->
name|create_loc_conf
argument_list|(
name|cf
operator|->
name|pool
argument_list|)
argument_list|,
name|NGX_CONF_ERROR
argument_list|)
expr_stmt|;
block|}
block|}
comment|/* parse inside the http{} block */
name|pcf
operator|=
operator|*
name|cf
expr_stmt|;
name|cf
operator|->
name|ctx
operator|=
name|ctx
expr_stmt|;
name|cf
operator|->
name|module_type
operator|=
name|NGX_HTTP_MODULE
expr_stmt|;
name|cf
operator|->
name|cmd_type
operator|=
name|NGX_HTTP_MAIN_CONF
expr_stmt|;
name|rv
operator|=
name|ngx_conf_parse
argument_list|(
name|cf
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
operator|*
name|cf
operator|=
name|pcf
expr_stmt|;
if|if
condition|(
name|rv
operator|!=
name|NGX_CONF_OK
condition|)
return|return
name|rv
return|;
comment|/* init http{} main_conf's, merge the server{}s' srv_conf's        and its location{}s' loc_conf's */
name|cmcf
operator|=
name|ctx
operator|->
name|main_conf
index|[
name|ngx_http_core_module
operator|.
name|ctx_index
index|]
expr_stmt|;
name|cscfp
operator|=
name|cmcf
operator|->
name|servers
operator|.
name|elts
expr_stmt|;
for|for
control|(
name|m
operator|=
literal|0
init|;
name|ngx_modules
index|[
name|m
index|]
condition|;
name|m
operator|++
control|)
block|{
if|if
condition|(
name|ngx_modules
index|[
name|m
index|]
operator|->
name|type
operator|!=
name|NGX_HTTP_MODULE
condition|)
block|{
continue|continue;
block|}
name|module
operator|=
name|ngx_modules
index|[
name|m
index|]
operator|->
name|ctx
expr_stmt|;
name|mi
operator|=
name|ngx_modules
index|[
name|m
index|]
operator|->
name|ctx_index
expr_stmt|;
comment|/* init http{} main_conf's */
if|if
condition|(
name|module
operator|->
name|init_main_conf
condition|)
block|{
name|rv
operator|=
name|module
operator|->
name|init_main_conf
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
name|ctx
operator|->
name|main_conf
index|[
name|mi
index|]
argument_list|)
expr_stmt|;
if|if
condition|(
name|rv
operator|!=
name|NGX_CONF_OK
condition|)
block|{
return|return
name|rv
return|;
block|}
block|}
for|for
control|(
name|s
operator|=
literal|0
init|;
name|s
operator|<
name|cmcf
operator|->
name|servers
operator|.
name|nelts
condition|;
name|s
operator|++
control|)
block|{
comment|/* merge the server{}s' srv_conf's */
if|if
condition|(
name|module
operator|->
name|merge_srv_conf
condition|)
block|{
name|rv
operator|=
name|module
operator|->
name|merge_srv_conf
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
name|ctx
operator|->
name|srv_conf
index|[
name|mi
index|]
argument_list|,
name|cscfp
index|[
name|s
index|]
operator|->
name|ctx
operator|->
name|srv_conf
index|[
name|mi
index|]
argument_list|)
expr_stmt|;
if|if
condition|(
name|rv
operator|!=
name|NGX_CONF_OK
condition|)
block|{
return|return
name|rv
return|;
block|}
block|}
if|if
condition|(
name|module
operator|->
name|merge_loc_conf
condition|)
block|{
comment|/* merge the server{}'s loc_conf */
name|rv
operator|=
name|module
operator|->
name|merge_loc_conf
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
name|ctx
operator|->
name|loc_conf
index|[
name|mi
index|]
argument_list|,
name|cscfp
index|[
name|s
index|]
operator|->
name|ctx
operator|->
name|loc_conf
index|[
name|mi
index|]
argument_list|)
expr_stmt|;
if|if
condition|(
name|rv
operator|!=
name|NGX_CONF_OK
condition|)
block|{
return|return
name|rv
return|;
block|}
comment|/* merge the locations{}' loc_conf's */
name|clcfp
operator|=
operator|(
name|ngx_http_core_loc_conf_t
operator|*
operator|*
operator|)
name|cscfp
index|[
name|s
index|]
operator|->
name|locations
operator|.
name|elts
expr_stmt|;
for|for
control|(
name|l
operator|=
literal|0
init|;
name|l
operator|<
name|cscfp
index|[
name|s
index|]
operator|->
name|locations
operator|.
name|nelts
condition|;
name|l
operator|++
control|)
block|{
name|rv
operator|=
name|module
operator|->
name|merge_loc_conf
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
name|cscfp
index|[
name|s
index|]
operator|->
name|ctx
operator|->
name|loc_conf
index|[
name|mi
index|]
argument_list|,
name|clcfp
index|[
name|l
index|]
operator|->
name|loc_conf
index|[
name|mi
index|]
argument_list|)
expr_stmt|;
if|if
condition|(
name|rv
operator|!=
name|NGX_CONF_OK
condition|)
block|{
return|return
name|rv
return|;
block|}
block|}
block|}
block|}
block|}
comment|/* init list of the handlers */
name|ngx_init_array
argument_list|(
name|cmcf
operator|->
name|translate_handlers
argument_list|,
name|cf
operator|->
name|cycle
operator|->
name|pool
argument_list|,
literal|10
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_handler_pt
argument_list|)
argument_list|,
name|NGX_CONF_ERROR
argument_list|)
expr_stmt|;
name|ngx_init_array
argument_list|(
name|cmcf
operator|->
name|index_handlers
argument_list|,
name|cf
operator|->
name|cycle
operator|->
name|pool
argument_list|,
literal|3
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_handler_pt
argument_list|)
argument_list|,
name|NGX_CONF_ERROR
argument_list|)
expr_stmt|;
comment|/* create the lists of the ports, the addresses and the server names        to allow quickly find the server core module configuration at run-time */
name|ngx_init_array
argument_list|(
name|in_ports
argument_list|,
name|cf
operator|->
name|pool
argument_list|,
literal|10
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_in_port_t
argument_list|)
argument_list|,
name|NGX_CONF_ERROR
argument_list|)
expr_stmt|;
comment|/* "server" directives */
name|cscfp
operator|=
name|cmcf
operator|->
name|servers
operator|.
name|elts
expr_stmt|;
for|for
control|(
name|s
operator|=
literal|0
init|;
name|s
operator|<
name|cmcf
operator|->
name|servers
operator|.
name|nelts
condition|;
name|s
operator|++
control|)
block|{
comment|/* "listen" directives */
name|lscf
operator|=
name|cscfp
index|[
name|s
index|]
operator|->
name|listen
operator|.
name|elts
expr_stmt|;
for|for
control|(
name|l
operator|=
literal|0
init|;
name|l
operator|<
name|cscfp
index|[
name|s
index|]
operator|->
name|listen
operator|.
name|nelts
condition|;
name|l
operator|++
control|)
block|{
name|port_found
operator|=
literal|0
expr_stmt|;
comment|/* AF_INET only */
name|in_port
operator|=
name|in_ports
operator|.
name|elts
expr_stmt|;
for|for
control|(
name|p
operator|=
literal|0
init|;
name|p
operator|<
name|in_ports
operator|.
name|nelts
condition|;
name|p
operator|++
control|)
block|{
if|if
condition|(
name|lscf
index|[
name|l
index|]
operator|.
name|port
operator|==
name|in_port
index|[
name|p
index|]
operator|.
name|port
condition|)
block|{
comment|/* the port is already in the port list */
name|port_found
operator|=
literal|1
expr_stmt|;
name|addr_found
operator|=
literal|0
expr_stmt|;
name|in_addr
operator|=
name|in_port
index|[
name|p
index|]
operator|.
name|addrs
operator|.
name|elts
expr_stmt|;
for|for
control|(
name|a
operator|=
literal|0
init|;
name|a
operator|<
name|in_port
index|[
name|p
index|]
operator|.
name|addrs
operator|.
name|nelts
condition|;
name|a
operator|++
control|)
block|{
if|if
condition|(
name|lscf
index|[
name|l
index|]
operator|.
name|addr
operator|==
name|in_addr
index|[
name|a
index|]
operator|.
name|addr
condition|)
block|{
comment|/* the address is already bound to this port */
comment|/* "server_name" directives */
name|s_name
operator|=
name|cscfp
index|[
name|s
index|]
operator|->
name|server_names
operator|.
name|elts
expr_stmt|;
for|for
control|(
name|n
operator|=
literal|0
init|;
name|n
operator|<
name|cscfp
index|[
name|s
index|]
operator|->
name|server_names
operator|.
name|nelts
condition|;
name|n
operator|++
control|)
block|{
comment|/* add the server name and server core module                                    configuration to the address:port */
comment|/* TODO: duplicate names can be checked here */
name|ngx_test_null
argument_list|(
name|name
argument_list|,
name|ngx_push_array
argument_list|(
operator|&
name|in_addr
index|[
name|a
index|]
operator|.
name|names
argument_list|)
argument_list|,
name|NGX_CONF_ERROR
argument_list|)
expr_stmt|;
name|name
operator|->
name|name
operator|=
name|s_name
index|[
name|n
index|]
operator|.
name|name
expr_stmt|;
name|name
operator|->
name|core_srv_conf
operator|=
name|s_name
index|[
name|n
index|]
operator|.
name|core_srv_conf
expr_stmt|;
block|}
comment|/* check duplicate "default" server that                                serves this address:port */
if|if
condition|(
name|lscf
index|[
name|l
index|]
operator|.
name|flags
operator|&
name|NGX_HTTP_DEFAULT_SERVER
condition|)
block|{
if|if
condition|(
name|in_addr
index|[
name|a
index|]
operator|.
name|flags
operator|&
name|NGX_HTTP_DEFAULT_SERVER
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ERR
argument_list|,
name|cf
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"duplicate default server in %s:%d"
argument_list|,
name|lscf
index|[
name|l
index|]
operator|.
name|file_name
operator|.
name|data
argument_list|,
name|lscf
index|[
name|l
index|]
operator|.
name|line
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|in_addr
index|[
name|a
index|]
operator|.
name|flags
operator||=
name|NGX_HTTP_DEFAULT_SERVER
expr_stmt|;
name|in_addr
index|[
name|a
index|]
operator|.
name|core_srv_conf
operator|=
name|cscfp
index|[
name|s
index|]
expr_stmt|;
block|}
name|addr_found
operator|=
literal|1
expr_stmt|;
break|break;
block|}
if|else if
condition|(
name|in_addr
index|[
name|a
index|]
operator|.
name|addr
operator|==
name|INADDR_ANY
condition|)
block|{
comment|/* "*:port" must be the last resort so move it                                to the end of the address list and add                                the new address at its place */
name|ngx_test_null
argument_list|(
name|inaddr
argument_list|,
name|ngx_push_array
argument_list|(
operator|&
name|in_port
index|[
name|p
index|]
operator|.
name|addrs
argument_list|)
argument_list|,
name|NGX_CONF_ERROR
argument_list|)
expr_stmt|;
name|ngx_memcpy
argument_list|(
name|inaddr
argument_list|,
operator|&
name|in_addr
index|[
name|a
index|]
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_in_addr_t
argument_list|)
argument_list|)
expr_stmt|;
name|in_addr
index|[
name|a
index|]
operator|.
name|addr
operator|=
name|lscf
index|[
name|l
index|]
operator|.
name|addr
expr_stmt|;
name|in_addr
index|[
name|a
index|]
operator|.
name|flags
operator|=
name|lscf
index|[
name|l
index|]
operator|.
name|flags
expr_stmt|;
name|in_addr
index|[
name|a
index|]
operator|.
name|core_srv_conf
operator|=
name|cscfp
index|[
name|s
index|]
expr_stmt|;
comment|/* create the empty list of the server names that                                can be served on this address:port */
name|ngx_init_array
argument_list|(
name|inaddr
operator|->
name|names
argument_list|,
name|cf
operator|->
name|pool
argument_list|,
literal|10
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_server_name_t
argument_list|)
argument_list|,
name|NGX_CONF_ERROR
argument_list|)
expr_stmt|;
name|addr_found
operator|=
literal|1
expr_stmt|;
break|break;
block|}
block|}
if|if
condition|(
operator|!
name|addr_found
condition|)
block|{
comment|/* add the address to the addresses list that                            bound to this port */
name|ngx_test_null
argument_list|(
name|inaddr
argument_list|,
name|ngx_push_array
argument_list|(
operator|&
name|in_port
index|[
name|p
index|]
operator|.
name|addrs
argument_list|)
argument_list|,
name|NGX_CONF_ERROR
argument_list|)
expr_stmt|;
name|inaddr
operator|->
name|addr
operator|=
name|lscf
index|[
name|l
index|]
operator|.
name|addr
expr_stmt|;
name|inaddr
operator|->
name|flags
operator|=
name|lscf
index|[
name|l
index|]
operator|.
name|flags
expr_stmt|;
name|inaddr
operator|->
name|core_srv_conf
operator|=
name|cscfp
index|[
name|s
index|]
expr_stmt|;
comment|/* create the empty list of the server names that                            can be served on this address:port */
name|ngx_init_array
argument_list|(
name|inaddr
operator|->
name|names
argument_list|,
name|cf
operator|->
name|pool
argument_list|,
literal|10
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_server_name_t
argument_list|)
argument_list|,
name|NGX_CONF_ERROR
argument_list|)
expr_stmt|;
block|}
block|}
block|}
if|if
condition|(
operator|!
name|port_found
condition|)
block|{
comment|/* add the port to the in_port list */
name|ngx_test_null
argument_list|(
name|in_port
argument_list|,
name|ngx_push_array
argument_list|(
operator|&
name|in_ports
argument_list|)
argument_list|,
name|NGX_CONF_ERROR
argument_list|)
expr_stmt|;
name|in_port
operator|->
name|port
operator|=
name|lscf
index|[
name|l
index|]
operator|.
name|port
expr_stmt|;
name|ngx_test_null
argument_list|(
name|in_port
operator|->
name|port_name
operator|.
name|data
argument_list|,
name|ngx_palloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
literal|7
argument_list|)
argument_list|,
name|NGX_CONF_ERROR
argument_list|)
expr_stmt|;
name|in_port
operator|->
name|port_name
operator|.
name|len
operator|=
name|ngx_snprintf
argument_list|(
name|in_port
operator|->
name|port_name
operator|.
name|data
argument_list|,
literal|7
argument_list|,
literal|":%d"
argument_list|,
name|in_port
operator|->
name|port
argument_list|)
expr_stmt|;
comment|/* create list of the addresses that bound to this port ... */
name|ngx_init_array
argument_list|(
name|in_port
operator|->
name|addrs
argument_list|,
name|cf
operator|->
name|pool
argument_list|,
literal|10
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_in_addr_t
argument_list|)
argument_list|,
name|NGX_CONF_ERROR
argument_list|)
expr_stmt|;
name|ngx_test_null
argument_list|(
name|inaddr
argument_list|,
name|ngx_push_array
argument_list|(
operator|&
name|in_port
operator|->
name|addrs
argument_list|)
argument_list|,
name|NGX_CONF_ERROR
argument_list|)
expr_stmt|;
comment|/* ... and add the address to this list */
name|inaddr
operator|->
name|addr
operator|=
name|lscf
index|[
name|l
index|]
operator|.
name|addr
expr_stmt|;
name|inaddr
operator|->
name|flags
operator|=
name|lscf
index|[
name|l
index|]
operator|.
name|flags
expr_stmt|;
name|inaddr
operator|->
name|core_srv_conf
operator|=
name|cscfp
index|[
name|s
index|]
expr_stmt|;
comment|/* create the empty list of the server names that                    can be served on this address:port */
name|ngx_init_array
argument_list|(
name|inaddr
operator|->
name|names
argument_list|,
name|cf
operator|->
name|pool
argument_list|,
literal|10
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_server_name_t
argument_list|)
argument_list|,
name|NGX_CONF_ERROR
argument_list|)
expr_stmt|;
block|}
block|}
block|}
comment|/* optimize the lists of the ports, the addresses and the server names */
comment|/* AF_INET only */
name|in_port
operator|=
name|in_ports
operator|.
name|elts
expr_stmt|;
for|for
control|(
name|p
operator|=
literal|0
init|;
name|p
operator|<
name|in_ports
operator|.
name|nelts
condition|;
name|p
operator|++
control|)
block|{
comment|/* check whether the all server names point to the same server */
name|in_addr
operator|=
name|in_port
index|[
name|p
index|]
operator|.
name|addrs
operator|.
name|elts
expr_stmt|;
for|for
control|(
name|a
operator|=
literal|0
init|;
name|a
operator|<
name|in_port
index|[
name|p
index|]
operator|.
name|addrs
operator|.
name|nelts
condition|;
name|a
operator|++
control|)
block|{
name|virtual_names
operator|=
literal|0
expr_stmt|;
name|name
operator|=
name|in_addr
index|[
name|a
index|]
operator|.
name|names
operator|.
name|elts
expr_stmt|;
for|for
control|(
name|n
operator|=
literal|0
init|;
name|n
operator|<
name|in_addr
index|[
name|a
index|]
operator|.
name|names
operator|.
name|nelts
condition|;
name|n
operator|++
control|)
block|{
if|if
condition|(
name|in_addr
index|[
name|a
index|]
operator|.
name|core_srv_conf
operator|!=
name|name
index|[
name|n
index|]
operator|.
name|core_srv_conf
condition|)
block|{
name|virtual_names
operator|=
literal|1
expr_stmt|;
break|break;
block|}
block|}
comment|/* if the all server names point to the same server                then we do not need to check them at run-time */
if|if
condition|(
operator|!
name|virtual_names
condition|)
block|{
name|in_addr
index|[
name|a
index|]
operator|.
name|names
operator|.
name|nelts
operator|=
literal|0
expr_stmt|;
block|}
block|}
comment|/* if there's the binding to "*:port" then we need to bind()            to "*:port" only and ignore the other bindings */
if|if
condition|(
name|in_addr
index|[
name|a
operator|-
literal|1
index|]
operator|.
name|addr
operator|==
name|INADDR_ANY
condition|)
block|{
name|a
operator|--
expr_stmt|;
block|}
else|else
block|{
name|a
operator|=
literal|0
expr_stmt|;
block|}
name|in_addr
operator|=
name|in_port
index|[
name|p
index|]
operator|.
name|addrs
operator|.
name|elts
expr_stmt|;
while|while
condition|(
name|a
operator|<
name|in_port
index|[
name|p
index|]
operator|.
name|addrs
operator|.
name|nelts
condition|)
block|{
name|ngx_test_null
argument_list|(
name|ls
argument_list|,
name|ngx_push_array
argument_list|(
operator|&
name|cf
operator|->
name|cycle
operator|->
name|listening
argument_list|)
argument_list|,
name|NGX_CONF_ERROR
argument_list|)
expr_stmt|;
name|ngx_memzero
argument_list|(
name|ls
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_listening_t
argument_list|)
argument_list|)
expr_stmt|;
name|ngx_test_null
argument_list|(
name|addr_in
argument_list|,
name|ngx_pcalloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
expr|struct
name|sockaddr_in
argument_list|)
argument_list|)
argument_list|,
name|NGX_CONF_ERROR
argument_list|)
expr_stmt|;
name|addr_in
operator|->
name|sin_family
operator|=
name|AF_INET
expr_stmt|;
name|addr_in
operator|->
name|sin_addr
operator|.
name|s_addr
operator|=
name|in_addr
index|[
name|a
index|]
operator|.
name|addr
expr_stmt|;
name|addr_in
operator|->
name|sin_port
operator|=
name|htons
argument_list|(
operator|(
name|u_short
operator|)
name|in_port
index|[
name|p
index|]
operator|.
name|port
argument_list|)
expr_stmt|;
name|ngx_test_null
argument_list|(
name|ls
operator|->
name|addr_text
operator|.
name|data
argument_list|,
name|ngx_palloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
name|INET_ADDRSTRLEN
operator|+
literal|6
argument_list|)
argument_list|,
name|NGX_CONF_ERROR
argument_list|)
expr_stmt|;
name|ls
operator|->
name|addr_text
operator|.
name|len
operator|=
name|ngx_snprintf
argument_list|(
name|ls
operator|->
name|addr_text
operator|.
name|data
operator|+
name|ngx_inet_ntop
argument_list|(
name|AF_INET
argument_list|,
operator|(
name|char
operator|*
operator|)
operator|&
name|in_addr
index|[
name|a
index|]
operator|.
name|addr
argument_list|,
name|ls
operator|->
name|addr_text
operator|.
name|data
argument_list|,
name|INET_ADDRSTRLEN
argument_list|)
argument_list|,
literal|6
argument_list|,
literal|":%d"
argument_list|,
name|in_port
index|[
name|p
index|]
operator|.
name|port
argument_list|)
expr_stmt|;
name|ls
operator|->
name|fd
operator|=
operator|-
literal|1
expr_stmt|;
name|ls
operator|->
name|family
operator|=
name|AF_INET
expr_stmt|;
name|ls
operator|->
name|type
operator|=
name|SOCK_STREAM
expr_stmt|;
name|ls
operator|->
name|protocol
operator|=
name|IPPROTO_IP
expr_stmt|;
if|#
directive|if
operator|(
name|WIN32
operator|)
name|ls
operator|->
name|flags
operator|=
name|WSA_FLAG_OVERLAPPED
expr_stmt|;
endif|#
directive|endif
name|ls
operator|->
name|sockaddr
operator|=
operator|(
expr|struct
name|sockaddr
operator|*
operator|)
name|addr_in
expr_stmt|;
name|ls
operator|->
name|socklen
operator|=
sizeof|sizeof
argument_list|(
expr|struct
name|sockaddr_in
argument_list|)
expr_stmt|;
name|ls
operator|->
name|addr
operator|=
name|offsetof
argument_list|(
expr|struct
name|sockaddr_in
argument_list|,
name|sin_addr
argument_list|)
expr_stmt|;
name|ls
operator|->
name|addr_text_max_len
operator|=
name|INET_ADDRSTRLEN
expr_stmt|;
name|ls
operator|->
name|backlog
operator|=
operator|-
literal|1
expr_stmt|;
name|ls
operator|->
name|nonblocking
operator|=
literal|1
expr_stmt|;
name|ls
operator|->
name|handler
operator|=
name|ngx_http_init_connection
expr_stmt|;
name|ls
operator|->
name|log
operator|=
name|cf
operator|->
name|log
expr_stmt|;
name|cscf
operator|=
name|in_addr
index|[
name|a
index|]
operator|.
name|core_srv_conf
expr_stmt|;
name|ls
operator|->
name|pool_size
operator|=
name|cscf
operator|->
name|connection_pool_size
expr_stmt|;
name|ls
operator|->
name|post_accept_timeout
operator|=
name|cscf
operator|->
name|post_accept_timeout
expr_stmt|;
if|#
directive|if
operator|(
name|WIN32
operator|)
name|iocpcf
operator|=
name|ngx_event_get_conf
argument_list|(
name|ngx_iocp_module
argument_list|)
expr_stmt|;
if|if
condition|(
name|iocpcf
operator|->
name|acceptex_read
condition|)
block|{
name|ls
operator|->
name|post_accept_buffer_size
operator|=
name|cscf
operator|->
name|client_header_buffer_size
expr_stmt|;
block|}
endif|#
directive|endif
name|ls
operator|->
name|ctx
operator|=
name|ctx
expr_stmt|;
if|if
condition|(
name|in_port
index|[
name|p
index|]
operator|.
name|addrs
operator|.
name|nelts
operator|>
literal|1
condition|)
block|{
name|in_addr
operator|=
name|in_port
index|[
name|p
index|]
operator|.
name|addrs
operator|.
name|elts
expr_stmt|;
if|if
condition|(
name|in_addr
index|[
name|in_port
index|[
name|p
index|]
operator|.
name|addrs
operator|.
name|nelts
operator|-
literal|1
index|]
operator|.
name|addr
operator|!=
name|INADDR_ANY
condition|)
block|{
comment|/* if this port has not the "*:port" binding then create                        the separate ngx_http_in_port_t for the all bindings */
name|ngx_test_null
argument_list|(
name|inport
argument_list|,
name|ngx_palloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_in_port_t
argument_list|)
argument_list|)
argument_list|,
name|NGX_CONF_ERROR
argument_list|)
expr_stmt|;
name|inport
operator|->
name|port
operator|=
name|in_port
index|[
name|p
index|]
operator|.
name|port
expr_stmt|;
name|inport
operator|->
name|port_name
operator|=
name|in_port
index|[
name|p
index|]
operator|.
name|port_name
expr_stmt|;
comment|/* init list of the addresses ... */
name|ngx_init_array
argument_list|(
name|inport
operator|->
name|addrs
argument_list|,
name|cf
operator|->
name|pool
argument_list|,
literal|1
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_in_addr_t
argument_list|)
argument_list|,
name|NGX_CONF_ERROR
argument_list|)
expr_stmt|;
comment|/* ... and set up it with the first address */
name|inport
operator|->
name|addrs
operator|.
name|nelts
operator|=
literal|1
expr_stmt|;
name|inport
operator|->
name|addrs
operator|.
name|elts
operator|=
name|in_port
index|[
name|p
index|]
operator|.
name|addrs
operator|.
name|elts
expr_stmt|;
name|ls
operator|->
name|servers
operator|=
name|inport
expr_stmt|;
comment|/* prepare for the next cycle */
name|in_port
index|[
name|p
index|]
operator|.
name|addrs
operator|.
name|elts
operator|=
operator|(
name|char
operator|*
operator|)
name|in_port
index|[
name|p
index|]
operator|.
name|addrs
operator|.
name|elts
operator|+
name|in_port
index|[
name|p
index|]
operator|.
name|addrs
operator|.
name|size
expr_stmt|;
name|in_port
index|[
name|p
index|]
operator|.
name|addrs
operator|.
name|nelts
operator|--
expr_stmt|;
name|in_addr
operator|=
operator|(
name|ngx_http_in_addr_t
operator|*
operator|)
name|in_port
index|[
name|p
index|]
operator|.
name|addrs
operator|.
name|elts
expr_stmt|;
name|a
operator|=
literal|0
expr_stmt|;
continue|continue;
block|}
block|}
name|ls
operator|->
name|servers
operator|=
operator|&
name|in_port
index|[
name|p
index|]
expr_stmt|;
name|a
operator|++
expr_stmt|;
block|}
block|}
comment|/* DEBUG STUFF */
name|in_port
operator|=
name|in_ports
operator|.
name|elts
expr_stmt|;
for|for
control|(
name|p
operator|=
literal|0
init|;
name|p
operator|<
name|in_ports
operator|.
name|nelts
condition|;
name|p
operator|++
control|)
block|{
name|ngx_log_debug
argument_list|(
argument|cf->log
argument_list|,
literal|"port: %d"
argument|_ in_port[p].port
argument_list|)
empty_stmt|;
name|in_addr
operator|=
name|in_port
index|[
name|p
index|]
operator|.
name|addrs
operator|.
name|elts
expr_stmt|;
for|for
control|(
name|a
operator|=
literal|0
init|;
name|a
operator|<
name|in_port
index|[
name|p
index|]
operator|.
name|addrs
operator|.
name|nelts
condition|;
name|a
operator|++
control|)
block|{
name|char
name|ip
index|[
literal|20
index|]
decl_stmt|;
name|ngx_inet_ntop
argument_list|(
name|AF_INET
argument_list|,
operator|(
name|char
operator|*
operator|)
operator|&
name|in_addr
index|[
name|a
index|]
operator|.
name|addr
argument_list|,
name|ip
argument_list|,
literal|20
argument_list|)
expr_stmt|;
name|ngx_log_debug
argument_list|(
argument|cf->log
argument_list|,
literal|"%s %08x"
argument|_ ip _ in_addr[a].core_srv_conf
argument_list|)
empty_stmt|;
name|s_name
operator|=
name|in_addr
index|[
name|a
index|]
operator|.
name|names
operator|.
name|elts
expr_stmt|;
for|for
control|(
name|n
operator|=
literal|0
init|;
name|n
operator|<
name|in_addr
index|[
name|a
index|]
operator|.
name|names
operator|.
name|nelts
condition|;
name|n
operator|++
control|)
block|{
name|ngx_log_debug
argument_list|(
argument|cf->log
argument_list|,
literal|"%s %08x"
argument|_ s_name[n].name.data _               s_name[n].core_srv_conf
argument_list|)
empty_stmt|;
block|}
block|}
block|}
comment|/**/
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

end_unit

