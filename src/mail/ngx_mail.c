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
file|<ngx_event.h>
end_include

begin_include
include|#
directive|include
file|<ngx_mail.h>
end_include

begin_function_decl
specifier|static
name|char
modifier|*
name|ngx_mail_block
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
name|ngx_int_t
name|ngx_mail_add_ports
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|ngx_array_t
modifier|*
name|ports
parameter_list|,
name|ngx_mail_listen_t
modifier|*
name|listen
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|char
modifier|*
name|ngx_mail_optimize_servers
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|ngx_array_t
modifier|*
name|ports
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_mail_add_addrs
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|ngx_mail_port_t
modifier|*
name|mport
parameter_list|,
name|ngx_mail_conf_addr_t
modifier|*
name|addr
parameter_list|)
function_decl|;
end_function_decl

begin_if
if|#
directive|if
operator|(
name|NGX_HAVE_INET6
operator|)
end_if

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_mail_add_addrs6
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|ngx_mail_port_t
modifier|*
name|mport
parameter_list|,
name|ngx_mail_conf_addr_t
modifier|*
name|addr
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_mail_cmp_conf_addrs
parameter_list|(
specifier|const
name|void
modifier|*
name|one
parameter_list|,
specifier|const
name|void
modifier|*
name|two
parameter_list|)
function_decl|;
end_function_decl

begin_decl_stmt
DECL|variable|ngx_mail_max_module
name|ngx_uint_t
name|ngx_mail_max_module
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_mail_commands
specifier|static
name|ngx_command_t
name|ngx_mail_commands
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"mail"
argument_list|)
block|,
name|NGX_MAIN_CONF
operator||
name|NGX_CONF_BLOCK
operator||
name|NGX_CONF_NOARGS
block|,
name|ngx_mail_block
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
DECL|variable|ngx_mail_module_ctx
specifier|static
name|ngx_core_module_t
name|ngx_mail_module_ctx
init|=
block|{
name|ngx_string
argument_list|(
literal|"mail"
argument_list|)
block|,
name|NULL
block|,
name|NULL
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_mail_module
name|ngx_module_t
name|ngx_mail_module
init|=
block|{
name|NGX_MODULE_V1
block|,
operator|&
name|ngx_mail_module_ctx
block|,
comment|/* module context */
name|ngx_mail_commands
block|,
comment|/* module directives */
name|NGX_CORE_MODULE
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
name|char
modifier|*
DECL|function|ngx_mail_block (ngx_conf_t * cf,ngx_command_t * cmd,void * conf)
name|ngx_mail_block
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
name|char
modifier|*
name|rv
decl_stmt|;
name|ngx_uint_t
name|i
decl_stmt|,
name|m
decl_stmt|,
name|mi
decl_stmt|,
name|s
decl_stmt|;
name|ngx_conf_t
name|pcf
decl_stmt|;
name|ngx_array_t
name|ports
decl_stmt|;
name|ngx_mail_listen_t
modifier|*
name|listen
decl_stmt|;
name|ngx_mail_module_t
modifier|*
name|module
decl_stmt|;
name|ngx_mail_conf_ctx_t
modifier|*
name|ctx
decl_stmt|;
name|ngx_mail_core_srv_conf_t
modifier|*
modifier|*
name|cscfp
decl_stmt|;
name|ngx_mail_core_main_conf_t
modifier|*
name|cmcf
decl_stmt|;
if|if
condition|(
operator|*
operator|(
name|ngx_mail_conf_ctx_t
operator|*
operator|*
operator|)
name|conf
condition|)
block|{
return|return
literal|"is duplicate"
return|;
block|}
comment|/* the main mail context */
name|ctx
operator|=
name|ngx_pcalloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_mail_conf_ctx_t
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
name|NGX_CONF_ERROR
return|;
block|}
operator|*
operator|(
name|ngx_mail_conf_ctx_t
operator|*
operator|*
operator|)
name|conf
operator|=
name|ctx
expr_stmt|;
comment|/* count the number of the mail modules and set up their indices */
name|ngx_mail_max_module
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
name|NGX_MAIL_MODULE
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
name|ngx_mail_max_module
operator|++
expr_stmt|;
block|}
comment|/* the mail main_conf context, it is the same in the all mail contexts */
name|ctx
operator|->
name|main_conf
operator|=
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
name|ngx_mail_max_module
argument_list|)
expr_stmt|;
if|if
condition|(
name|ctx
operator|->
name|main_conf
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
comment|/*      * the mail null srv_conf context, it is used to merge      * the server{}s' srv_conf's      */
name|ctx
operator|->
name|srv_conf
operator|=
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
name|ngx_mail_max_module
argument_list|)
expr_stmt|;
if|if
condition|(
name|ctx
operator|->
name|srv_conf
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
comment|/*      * create the main_conf's and the null srv_conf's of the all mail modules      */
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
name|NGX_MAIL_MODULE
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
name|ctx
operator|->
name|main_conf
index|[
name|mi
index|]
operator|=
name|module
operator|->
name|create_main_conf
argument_list|(
name|cf
argument_list|)
expr_stmt|;
if|if
condition|(
name|ctx
operator|->
name|main_conf
index|[
name|mi
index|]
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
name|module
operator|->
name|create_srv_conf
condition|)
block|{
name|ctx
operator|->
name|srv_conf
index|[
name|mi
index|]
operator|=
name|module
operator|->
name|create_srv_conf
argument_list|(
name|cf
argument_list|)
expr_stmt|;
if|if
condition|(
name|ctx
operator|->
name|srv_conf
index|[
name|mi
index|]
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
block|}
block|}
comment|/* parse inside the mail{} block */
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
name|NGX_MAIL_MODULE
expr_stmt|;
name|cf
operator|->
name|cmd_type
operator|=
name|NGX_MAIL_MAIN_CONF
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
if|if
condition|(
name|rv
operator|!=
name|NGX_CONF_OK
condition|)
block|{
operator|*
name|cf
operator|=
name|pcf
expr_stmt|;
return|return
name|rv
return|;
block|}
comment|/* init mail{} main_conf's, merge the server{}s' srv_conf's */
name|cmcf
operator|=
name|ctx
operator|->
name|main_conf
index|[
name|ngx_mail_core_module
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
name|NGX_MAIL_MODULE
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
comment|/* init mail{} main_conf's */
name|cf
operator|->
name|ctx
operator|=
name|ctx
expr_stmt|;
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
operator|*
name|cf
operator|=
name|pcf
expr_stmt|;
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
name|cf
operator|->
name|ctx
operator|=
name|cscfp
index|[
name|s
index|]
operator|->
name|ctx
expr_stmt|;
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
operator|*
name|cf
operator|=
name|pcf
expr_stmt|;
return|return
name|rv
return|;
block|}
block|}
block|}
block|}
operator|*
name|cf
operator|=
name|pcf
expr_stmt|;
if|if
condition|(
name|ngx_array_init
argument_list|(
operator|&
name|ports
argument_list|,
name|cf
operator|->
name|temp_pool
argument_list|,
literal|4
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_mail_conf_port_t
argument_list|)
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|listen
operator|=
name|cmcf
operator|->
name|listen
operator|.
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
name|cmcf
operator|->
name|listen
operator|.
name|nelts
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|ngx_mail_add_ports
argument_list|(
name|cf
argument_list|,
operator|&
name|ports
argument_list|,
operator|&
name|listen
index|[
name|i
index|]
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
block|}
return|return
name|ngx_mail_optimize_servers
argument_list|(
name|cf
argument_list|,
operator|&
name|ports
argument_list|)
return|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_mail_add_ports (ngx_conf_t * cf,ngx_array_t * ports,ngx_mail_listen_t * listen)
name|ngx_mail_add_ports
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|ngx_array_t
modifier|*
name|ports
parameter_list|,
name|ngx_mail_listen_t
modifier|*
name|listen
parameter_list|)
block|{
name|in_port_t
name|p
decl_stmt|;
name|ngx_uint_t
name|i
decl_stmt|;
name|struct
name|sockaddr
modifier|*
name|sa
decl_stmt|;
name|struct
name|sockaddr_in
modifier|*
name|sin
decl_stmt|;
name|ngx_mail_conf_port_t
modifier|*
name|port
decl_stmt|;
name|ngx_mail_conf_addr_t
modifier|*
name|addr
decl_stmt|;
if|#
directive|if
operator|(
name|NGX_HAVE_INET6
operator|)
name|struct
name|sockaddr_in6
modifier|*
name|sin6
decl_stmt|;
endif|#
directive|endif
name|sa
operator|=
operator|&
name|listen
operator|->
name|u
operator|.
name|sockaddr
expr_stmt|;
switch|switch
condition|(
name|sa
operator|->
name|sa_family
condition|)
block|{
if|#
directive|if
operator|(
name|NGX_HAVE_INET6
operator|)
case|case
name|AF_INET6
case|:
name|sin6
operator|=
operator|&
name|listen
operator|->
name|u
operator|.
name|sockaddr_in6
expr_stmt|;
name|p
operator|=
name|sin6
operator|->
name|sin6_port
expr_stmt|;
break|break;
endif|#
directive|endif
if|#
directive|if
operator|(
name|NGX_HAVE_UNIX_DOMAIN
operator|)
case|case
name|AF_UNIX
case|:
name|p
operator|=
literal|0
expr_stmt|;
break|break;
endif|#
directive|endif
default|default:
comment|/* AF_INET */
name|sin
operator|=
operator|&
name|listen
operator|->
name|u
operator|.
name|sockaddr_in
expr_stmt|;
name|p
operator|=
name|sin
operator|->
name|sin_port
expr_stmt|;
break|break;
block|}
name|port
operator|=
name|ports
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
name|ports
operator|->
name|nelts
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|p
operator|==
name|port
index|[
name|i
index|]
operator|.
name|port
operator|&&
name|sa
operator|->
name|sa_family
operator|==
name|port
index|[
name|i
index|]
operator|.
name|family
condition|)
block|{
comment|/* a port is already in the port list */
name|port
operator|=
operator|&
name|port
index|[
name|i
index|]
expr_stmt|;
goto|goto
name|found
goto|;
block|}
block|}
comment|/* add a port to the port list */
name|port
operator|=
name|ngx_array_push
argument_list|(
name|ports
argument_list|)
expr_stmt|;
if|if
condition|(
name|port
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|port
operator|->
name|family
operator|=
name|sa
operator|->
name|sa_family
expr_stmt|;
name|port
operator|->
name|port
operator|=
name|p
expr_stmt|;
if|if
condition|(
name|ngx_array_init
argument_list|(
operator|&
name|port
operator|->
name|addrs
argument_list|,
name|cf
operator|->
name|temp_pool
argument_list|,
literal|2
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_mail_conf_addr_t
argument_list|)
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|found
label|:
name|addr
operator|=
name|ngx_array_push
argument_list|(
operator|&
name|port
operator|->
name|addrs
argument_list|)
expr_stmt|;
if|if
condition|(
name|addr
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|addr
operator|->
name|opt
operator|=
operator|*
name|listen
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
specifier|static
name|char
modifier|*
DECL|function|ngx_mail_optimize_servers (ngx_conf_t * cf,ngx_array_t * ports)
name|ngx_mail_optimize_servers
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|ngx_array_t
modifier|*
name|ports
parameter_list|)
block|{
name|ngx_uint_t
name|i
decl_stmt|,
name|p
decl_stmt|,
name|last
decl_stmt|,
name|bind_wildcard
decl_stmt|;
name|ngx_listening_t
modifier|*
name|ls
decl_stmt|;
name|ngx_mail_port_t
modifier|*
name|mport
decl_stmt|;
name|ngx_mail_conf_port_t
modifier|*
name|port
decl_stmt|;
name|ngx_mail_conf_addr_t
modifier|*
name|addr
decl_stmt|;
name|ngx_mail_core_srv_conf_t
modifier|*
name|cscf
decl_stmt|;
name|port
operator|=
name|ports
operator|->
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
name|ports
operator|->
name|nelts
condition|;
name|p
operator|++
control|)
block|{
name|ngx_sort
argument_list|(
name|port
index|[
name|p
index|]
operator|.
name|addrs
operator|.
name|elts
argument_list|,
operator|(
name|size_t
operator|)
name|port
index|[
name|p
index|]
operator|.
name|addrs
operator|.
name|nelts
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_mail_conf_addr_t
argument_list|)
argument_list|,
name|ngx_mail_cmp_conf_addrs
argument_list|)
expr_stmt|;
name|addr
operator|=
name|port
index|[
name|p
index|]
operator|.
name|addrs
operator|.
name|elts
expr_stmt|;
name|last
operator|=
name|port
index|[
name|p
index|]
operator|.
name|addrs
operator|.
name|nelts
expr_stmt|;
comment|/*          * if there is the binding to the "*:port" then we need to bind()          * to the "*:port" only and ignore the other bindings          */
if|if
condition|(
name|addr
index|[
name|last
operator|-
literal|1
index|]
operator|.
name|opt
operator|.
name|wildcard
condition|)
block|{
name|addr
index|[
name|last
operator|-
literal|1
index|]
operator|.
name|opt
operator|.
name|bind
operator|=
literal|1
expr_stmt|;
name|bind_wildcard
operator|=
literal|1
expr_stmt|;
block|}
else|else
block|{
name|bind_wildcard
operator|=
literal|0
expr_stmt|;
block|}
name|i
operator|=
literal|0
expr_stmt|;
while|while
condition|(
name|i
operator|<
name|last
condition|)
block|{
if|if
condition|(
name|bind_wildcard
operator|&&
operator|!
name|addr
index|[
name|i
index|]
operator|.
name|opt
operator|.
name|bind
condition|)
block|{
name|i
operator|++
expr_stmt|;
continue|continue;
block|}
name|ls
operator|=
name|ngx_create_listening
argument_list|(
name|cf
argument_list|,
operator|&
name|addr
index|[
name|i
index|]
operator|.
name|opt
operator|.
name|u
operator|.
name|sockaddr
argument_list|,
name|addr
index|[
name|i
index|]
operator|.
name|opt
operator|.
name|socklen
argument_list|)
expr_stmt|;
if|if
condition|(
name|ls
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|ls
operator|->
name|addr_ntop
operator|=
literal|1
expr_stmt|;
name|ls
operator|->
name|handler
operator|=
name|ngx_mail_init_connection
expr_stmt|;
name|ls
operator|->
name|pool_size
operator|=
literal|256
expr_stmt|;
name|cscf
operator|=
name|addr
operator|->
name|opt
operator|.
name|ctx
operator|->
name|srv_conf
index|[
name|ngx_mail_core_module
operator|.
name|ctx_index
index|]
expr_stmt|;
name|ls
operator|->
name|logp
operator|=
name|cscf
operator|->
name|error_log
expr_stmt|;
name|ls
operator|->
name|log
operator|.
name|data
operator|=
operator|&
name|ls
operator|->
name|addr_text
expr_stmt|;
name|ls
operator|->
name|log
operator|.
name|handler
operator|=
name|ngx_accept_log_error
expr_stmt|;
name|ls
operator|->
name|backlog
operator|=
name|addr
index|[
name|i
index|]
operator|.
name|opt
operator|.
name|backlog
expr_stmt|;
name|ls
operator|->
name|keepalive
operator|=
name|addr
index|[
name|i
index|]
operator|.
name|opt
operator|.
name|so_keepalive
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_HAVE_KEEPALIVE_TUNABLE
operator|)
name|ls
operator|->
name|keepidle
operator|=
name|addr
index|[
name|i
index|]
operator|.
name|opt
operator|.
name|tcp_keepidle
expr_stmt|;
name|ls
operator|->
name|keepintvl
operator|=
name|addr
index|[
name|i
index|]
operator|.
name|opt
operator|.
name|tcp_keepintvl
expr_stmt|;
name|ls
operator|->
name|keepcnt
operator|=
name|addr
index|[
name|i
index|]
operator|.
name|opt
operator|.
name|tcp_keepcnt
expr_stmt|;
endif|#
directive|endif
if|#
directive|if
operator|(
name|NGX_HAVE_INET6
operator|&&
name|defined
name|IPV6_V6ONLY
operator|)
name|ls
operator|->
name|ipv6only
operator|=
name|addr
index|[
name|i
index|]
operator|.
name|opt
operator|.
name|ipv6only
expr_stmt|;
endif|#
directive|endif
name|mport
operator|=
name|ngx_palloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_mail_port_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|mport
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|ls
operator|->
name|servers
operator|=
name|mport
expr_stmt|;
name|mport
operator|->
name|naddrs
operator|=
name|i
operator|+
literal|1
expr_stmt|;
switch|switch
condition|(
name|ls
operator|->
name|sockaddr
operator|->
name|sa_family
condition|)
block|{
if|#
directive|if
operator|(
name|NGX_HAVE_INET6
operator|)
case|case
name|AF_INET6
case|:
if|if
condition|(
name|ngx_mail_add_addrs6
argument_list|(
name|cf
argument_list|,
name|mport
argument_list|,
name|addr
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
break|break;
endif|#
directive|endif
default|default:
comment|/* AF_INET */
if|if
condition|(
name|ngx_mail_add_addrs
argument_list|(
name|cf
argument_list|,
name|mport
argument_list|,
name|addr
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
break|break;
block|}
name|addr
operator|++
expr_stmt|;
name|last
operator|--
expr_stmt|;
block|}
block|}
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_mail_add_addrs (ngx_conf_t * cf,ngx_mail_port_t * mport,ngx_mail_conf_addr_t * addr)
name|ngx_mail_add_addrs
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|ngx_mail_port_t
modifier|*
name|mport
parameter_list|,
name|ngx_mail_conf_addr_t
modifier|*
name|addr
parameter_list|)
block|{
name|u_char
modifier|*
name|p
decl_stmt|;
name|size_t
name|len
decl_stmt|;
name|ngx_uint_t
name|i
decl_stmt|;
name|ngx_mail_in_addr_t
modifier|*
name|addrs
decl_stmt|;
name|struct
name|sockaddr_in
modifier|*
name|sin
decl_stmt|;
name|u_char
name|buf
index|[
name|NGX_SOCKADDR_STRLEN
index|]
decl_stmt|;
name|mport
operator|->
name|addrs
operator|=
name|ngx_pcalloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
name|mport
operator|->
name|naddrs
operator|*
sizeof|sizeof
argument_list|(
name|ngx_mail_in_addr_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|mport
operator|->
name|addrs
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|addrs
operator|=
name|mport
operator|->
name|addrs
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|mport
operator|->
name|naddrs
condition|;
name|i
operator|++
control|)
block|{
name|sin
operator|=
operator|&
name|addr
index|[
name|i
index|]
operator|.
name|opt
operator|.
name|u
operator|.
name|sockaddr_in
expr_stmt|;
name|addrs
index|[
name|i
index|]
operator|.
name|addr
operator|=
name|sin
operator|->
name|sin_addr
operator|.
name|s_addr
expr_stmt|;
name|addrs
index|[
name|i
index|]
operator|.
name|conf
operator|.
name|ctx
operator|=
name|addr
index|[
name|i
index|]
operator|.
name|opt
operator|.
name|ctx
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_MAIL_SSL
operator|)
name|addrs
index|[
name|i
index|]
operator|.
name|conf
operator|.
name|ssl
operator|=
name|addr
index|[
name|i
index|]
operator|.
name|opt
operator|.
name|ssl
expr_stmt|;
endif|#
directive|endif
name|len
operator|=
name|ngx_sock_ntop
argument_list|(
operator|&
name|addr
index|[
name|i
index|]
operator|.
name|opt
operator|.
name|u
operator|.
name|sockaddr
argument_list|,
name|addr
index|[
name|i
index|]
operator|.
name|opt
operator|.
name|socklen
argument_list|,
name|buf
argument_list|,
name|NGX_SOCKADDR_STRLEN
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|p
operator|=
name|ngx_pnalloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
name|len
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
name|ngx_memcpy
argument_list|(
name|p
argument_list|,
name|buf
argument_list|,
name|len
argument_list|)
expr_stmt|;
name|addrs
index|[
name|i
index|]
operator|.
name|conf
operator|.
name|addr_text
operator|.
name|len
operator|=
name|len
expr_stmt|;
name|addrs
index|[
name|i
index|]
operator|.
name|conf
operator|.
name|addr_text
operator|.
name|data
operator|=
name|p
expr_stmt|;
block|}
return|return
name|NGX_OK
return|;
block|}
end_function

begin_if
if|#
directive|if
operator|(
name|NGX_HAVE_INET6
operator|)
end_if

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_mail_add_addrs6 (ngx_conf_t * cf,ngx_mail_port_t * mport,ngx_mail_conf_addr_t * addr)
name|ngx_mail_add_addrs6
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|ngx_mail_port_t
modifier|*
name|mport
parameter_list|,
name|ngx_mail_conf_addr_t
modifier|*
name|addr
parameter_list|)
block|{
name|u_char
modifier|*
name|p
decl_stmt|;
name|size_t
name|len
decl_stmt|;
name|ngx_uint_t
name|i
decl_stmt|;
name|ngx_mail_in6_addr_t
modifier|*
name|addrs6
decl_stmt|;
name|struct
name|sockaddr_in6
modifier|*
name|sin6
decl_stmt|;
name|u_char
name|buf
index|[
name|NGX_SOCKADDR_STRLEN
index|]
decl_stmt|;
name|mport
operator|->
name|addrs
operator|=
name|ngx_pcalloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
name|mport
operator|->
name|naddrs
operator|*
sizeof|sizeof
argument_list|(
name|ngx_mail_in6_addr_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|mport
operator|->
name|addrs
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|addrs6
operator|=
name|mport
operator|->
name|addrs
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|mport
operator|->
name|naddrs
condition|;
name|i
operator|++
control|)
block|{
name|sin6
operator|=
operator|&
name|addr
index|[
name|i
index|]
operator|.
name|opt
operator|.
name|u
operator|.
name|sockaddr_in6
expr_stmt|;
name|addrs6
index|[
name|i
index|]
operator|.
name|addr6
operator|=
name|sin6
operator|->
name|sin6_addr
expr_stmt|;
name|addrs6
index|[
name|i
index|]
operator|.
name|conf
operator|.
name|ctx
operator|=
name|addr
index|[
name|i
index|]
operator|.
name|opt
operator|.
name|ctx
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_MAIL_SSL
operator|)
name|addrs6
index|[
name|i
index|]
operator|.
name|conf
operator|.
name|ssl
operator|=
name|addr
index|[
name|i
index|]
operator|.
name|opt
operator|.
name|ssl
expr_stmt|;
endif|#
directive|endif
name|len
operator|=
name|ngx_sock_ntop
argument_list|(
operator|&
name|addr
index|[
name|i
index|]
operator|.
name|opt
operator|.
name|u
operator|.
name|sockaddr
argument_list|,
name|addr
index|[
name|i
index|]
operator|.
name|opt
operator|.
name|socklen
argument_list|,
name|buf
argument_list|,
name|NGX_SOCKADDR_STRLEN
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|p
operator|=
name|ngx_pnalloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
name|len
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
name|ngx_memcpy
argument_list|(
name|p
argument_list|,
name|buf
argument_list|,
name|len
argument_list|)
expr_stmt|;
name|addrs6
index|[
name|i
index|]
operator|.
name|conf
operator|.
name|addr_text
operator|.
name|len
operator|=
name|len
expr_stmt|;
name|addrs6
index|[
name|i
index|]
operator|.
name|conf
operator|.
name|addr_text
operator|.
name|data
operator|=
name|p
expr_stmt|;
block|}
return|return
name|NGX_OK
return|;
block|}
end_function

begin_endif
endif|#
directive|endif
end_endif

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_mail_cmp_conf_addrs (const void * one,const void * two)
name|ngx_mail_cmp_conf_addrs
parameter_list|(
specifier|const
name|void
modifier|*
name|one
parameter_list|,
specifier|const
name|void
modifier|*
name|two
parameter_list|)
block|{
name|ngx_mail_conf_addr_t
modifier|*
name|first
decl_stmt|,
modifier|*
name|second
decl_stmt|;
name|first
operator|=
operator|(
name|ngx_mail_conf_addr_t
operator|*
operator|)
name|one
expr_stmt|;
name|second
operator|=
operator|(
name|ngx_mail_conf_addr_t
operator|*
operator|)
name|two
expr_stmt|;
if|if
condition|(
name|first
operator|->
name|opt
operator|.
name|wildcard
condition|)
block|{
comment|/* a wildcard must be the last resort, shift it to the end */
return|return
literal|1
return|;
block|}
if|if
condition|(
name|second
operator|->
name|opt
operator|.
name|wildcard
condition|)
block|{
comment|/* a wildcard must be the last resort, shift it to the end */
return|return
operator|-
literal|1
return|;
block|}
if|if
condition|(
name|first
operator|->
name|opt
operator|.
name|bind
operator|&&
operator|!
name|second
operator|->
name|opt
operator|.
name|bind
condition|)
block|{
comment|/* shift explicit bind()ed addresses to the start */
return|return
operator|-
literal|1
return|;
block|}
if|if
condition|(
operator|!
name|first
operator|->
name|opt
operator|.
name|bind
operator|&&
name|second
operator|->
name|opt
operator|.
name|bind
condition|)
block|{
comment|/* shift explicit bind()ed addresses to the start */
return|return
literal|1
return|;
block|}
comment|/* do not sort by default */
return|return
literal|0
return|;
block|}
end_function

end_unit

