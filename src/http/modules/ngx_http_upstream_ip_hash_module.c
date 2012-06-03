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

begin_typedef
DECL|struct|__anon2c22db1e0108
typedef|typedef
struct|struct
block|{
comment|/* the round robin data must be first */
DECL|member|rrp
name|ngx_http_upstream_rr_peer_data_t
name|rrp
decl_stmt|;
DECL|member|hash
name|ngx_uint_t
name|hash
decl_stmt|;
DECL|member|addr
name|u_char
name|addr
index|[
literal|3
index|]
decl_stmt|;
DECL|member|tries
name|u_char
name|tries
decl_stmt|;
DECL|member|get_rr_peer
name|ngx_event_get_peer_pt
name|get_rr_peer
decl_stmt|;
DECL|typedef|ngx_http_upstream_ip_hash_peer_data_t
block|}
name|ngx_http_upstream_ip_hash_peer_data_t
typedef|;
end_typedef

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_upstream_init_ip_hash_peer
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_upstream_srv_conf_t
modifier|*
name|us
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_upstream_get_ip_hash_peer
parameter_list|(
name|ngx_peer_connection_t
modifier|*
name|pc
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
name|ngx_http_upstream_ip_hash
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
DECL|variable|ngx_http_upstream_ip_hash_commands
specifier|static
name|ngx_command_t
name|ngx_http_upstream_ip_hash_commands
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"ip_hash"
argument_list|)
block|,
name|NGX_HTTP_UPS_CONF
operator||
name|NGX_CONF_NOARGS
block|,
name|ngx_http_upstream_ip_hash
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
DECL|variable|ngx_http_upstream_ip_hash_module_ctx
specifier|static
name|ngx_http_module_t
name|ngx_http_upstream_ip_hash_module_ctx
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
name|NULL
block|,
comment|/* create location configuration */
name|NULL
comment|/* merge location configuration */
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_upstream_ip_hash_module
name|ngx_module_t
name|ngx_http_upstream_ip_hash_module
init|=
block|{
name|NGX_MODULE_V1
block|,
operator|&
name|ngx_http_upstream_ip_hash_module_ctx
block|,
comment|/* module context */
name|ngx_http_upstream_ip_hash_commands
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

begin_function
name|ngx_int_t
DECL|function|ngx_http_upstream_init_ip_hash (ngx_conf_t * cf,ngx_http_upstream_srv_conf_t * us)
name|ngx_http_upstream_init_ip_hash
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|ngx_http_upstream_srv_conf_t
modifier|*
name|us
parameter_list|)
block|{
if|if
condition|(
name|ngx_http_upstream_init_round_robin
argument_list|(
name|cf
argument_list|,
name|us
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|us
operator|->
name|peer
operator|.
name|init
operator|=
name|ngx_http_upstream_init_ip_hash_peer
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_upstream_init_ip_hash_peer (ngx_http_request_t * r,ngx_http_upstream_srv_conf_t * us)
name|ngx_http_upstream_init_ip_hash_peer
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_upstream_srv_conf_t
modifier|*
name|us
parameter_list|)
block|{
name|u_char
modifier|*
name|p
decl_stmt|;
name|struct
name|sockaddr_in
modifier|*
name|sin
decl_stmt|;
name|ngx_http_upstream_ip_hash_peer_data_t
modifier|*
name|iphp
decl_stmt|;
name|iphp
operator|=
name|ngx_palloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_upstream_ip_hash_peer_data_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|iphp
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|r
operator|->
name|upstream
operator|->
name|peer
operator|.
name|data
operator|=
operator|&
name|iphp
operator|->
name|rrp
expr_stmt|;
if|if
condition|(
name|ngx_http_upstream_init_round_robin_peer
argument_list|(
name|r
argument_list|,
name|us
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|r
operator|->
name|upstream
operator|->
name|peer
operator|.
name|get
operator|=
name|ngx_http_upstream_get_ip_hash_peer
expr_stmt|;
comment|/* AF_INET only */
if|if
condition|(
name|r
operator|->
name|connection
operator|->
name|sockaddr
operator|->
name|sa_family
operator|==
name|AF_INET
condition|)
block|{
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
name|p
operator|=
operator|(
name|u_char
operator|*
operator|)
operator|&
name|sin
operator|->
name|sin_addr
operator|.
name|s_addr
expr_stmt|;
name|iphp
operator|->
name|addr
index|[
literal|0
index|]
operator|=
name|p
index|[
literal|0
index|]
expr_stmt|;
name|iphp
operator|->
name|addr
index|[
literal|1
index|]
operator|=
name|p
index|[
literal|1
index|]
expr_stmt|;
name|iphp
operator|->
name|addr
index|[
literal|2
index|]
operator|=
name|p
index|[
literal|2
index|]
expr_stmt|;
block|}
else|else
block|{
name|iphp
operator|->
name|addr
index|[
literal|0
index|]
operator|=
literal|0
expr_stmt|;
name|iphp
operator|->
name|addr
index|[
literal|1
index|]
operator|=
literal|0
expr_stmt|;
name|iphp
operator|->
name|addr
index|[
literal|2
index|]
operator|=
literal|0
expr_stmt|;
block|}
name|iphp
operator|->
name|hash
operator|=
literal|89
expr_stmt|;
name|iphp
operator|->
name|tries
operator|=
literal|0
expr_stmt|;
name|iphp
operator|->
name|get_rr_peer
operator|=
name|ngx_http_upstream_get_round_robin_peer
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_upstream_get_ip_hash_peer (ngx_peer_connection_t * pc,void * data)
name|ngx_http_upstream_get_ip_hash_peer
parameter_list|(
name|ngx_peer_connection_t
modifier|*
name|pc
parameter_list|,
name|void
modifier|*
name|data
parameter_list|)
block|{
name|ngx_http_upstream_ip_hash_peer_data_t
modifier|*
name|iphp
init|=
name|data
decl_stmt|;
name|time_t
name|now
decl_stmt|;
name|ngx_int_t
name|w
decl_stmt|;
name|uintptr_t
name|m
decl_stmt|;
name|ngx_uint_t
name|i
decl_stmt|,
name|n
decl_stmt|,
name|p
decl_stmt|,
name|hash
decl_stmt|;
name|ngx_http_upstream_rr_peer_t
modifier|*
name|peer
decl_stmt|;
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_HTTP
argument_list|,
name|pc
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"get ip hash peer, try: %ui"
argument_list|,
name|pc
operator|->
name|tries
argument_list|)
expr_stmt|;
comment|/* TODO: cached */
if|if
condition|(
name|iphp
operator|->
name|tries
operator|>
literal|20
operator|||
name|iphp
operator|->
name|rrp
operator|.
name|peers
operator|->
name|single
condition|)
block|{
return|return
name|iphp
operator|->
name|get_rr_peer
argument_list|(
name|pc
argument_list|,
operator|&
name|iphp
operator|->
name|rrp
argument_list|)
return|;
block|}
name|now
operator|=
name|ngx_time
argument_list|()
expr_stmt|;
name|pc
operator|->
name|cached
operator|=
literal|0
expr_stmt|;
name|pc
operator|->
name|connection
operator|=
name|NULL
expr_stmt|;
name|hash
operator|=
name|iphp
operator|->
name|hash
expr_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
literal|3
condition|;
name|i
operator|++
control|)
block|{
name|hash
operator|=
operator|(
name|hash
operator|*
literal|113
operator|+
name|iphp
operator|->
name|addr
index|[
name|i
index|]
operator|)
operator|%
literal|6271
expr_stmt|;
block|}
if|if
condition|(
operator|!
name|iphp
operator|->
name|rrp
operator|.
name|peers
operator|->
name|weighted
condition|)
block|{
name|p
operator|=
name|hash
operator|%
name|iphp
operator|->
name|rrp
operator|.
name|peers
operator|->
name|number
expr_stmt|;
block|}
else|else
block|{
name|w
operator|=
name|hash
operator|%
name|iphp
operator|->
name|rrp
operator|.
name|peers
operator|->
name|total_weight
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|iphp
operator|->
name|rrp
operator|.
name|peers
operator|->
name|number
condition|;
name|i
operator|++
control|)
block|{
name|w
operator|-=
name|iphp
operator|->
name|rrp
operator|.
name|peers
operator|->
name|peer
index|[
name|i
index|]
operator|.
name|weight
expr_stmt|;
if|if
condition|(
name|w
operator|<
literal|0
condition|)
block|{
break|break;
block|}
block|}
name|p
operator|=
name|i
expr_stmt|;
block|}
name|n
operator|=
name|p
operator|/
operator|(
literal|8
operator|*
sizeof|sizeof
argument_list|(
name|uintptr_t
argument_list|)
operator|)
expr_stmt|;
name|m
operator|=
operator|(
name|uintptr_t
operator|)
literal|1
operator|<<
name|p
operator|%
operator|(
literal|8
operator|*
sizeof|sizeof
argument_list|(
name|uintptr_t
argument_list|)
operator|)
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|iphp
operator|->
name|rrp
operator|.
name|tried
index|[
name|n
index|]
operator|&
name|m
operator|)
condition|)
block|{
name|ngx_log_debug2
argument_list|(
name|NGX_LOG_DEBUG_HTTP
argument_list|,
name|pc
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"get ip hash peer, hash: %ui %04XA"
argument_list|,
name|p
argument_list|,
name|m
argument_list|)
expr_stmt|;
name|peer
operator|=
operator|&
name|iphp
operator|->
name|rrp
operator|.
name|peers
operator|->
name|peer
index|[
name|p
index|]
expr_stmt|;
comment|/* ngx_lock_mutex(iphp->rrp.peers->mutex); */
if|if
condition|(
operator|!
name|peer
operator|->
name|down
condition|)
block|{
if|if
condition|(
name|peer
operator|->
name|max_fails
operator|==
literal|0
operator|||
name|peer
operator|->
name|fails
operator|<
name|peer
operator|->
name|max_fails
condition|)
block|{
break|break;
block|}
if|if
condition|(
name|now
operator|-
name|peer
operator|->
name|checked
operator|>
name|peer
operator|->
name|fail_timeout
condition|)
block|{
name|peer
operator|->
name|checked
operator|=
name|now
expr_stmt|;
break|break;
block|}
block|}
name|iphp
operator|->
name|rrp
operator|.
name|tried
index|[
name|n
index|]
operator||=
name|m
expr_stmt|;
comment|/* ngx_unlock_mutex(iphp->rrp.peers->mutex); */
name|pc
operator|->
name|tries
operator|--
expr_stmt|;
block|}
if|if
condition|(
operator|++
name|iphp
operator|->
name|tries
operator|>=
literal|20
condition|)
block|{
return|return
name|iphp
operator|->
name|get_rr_peer
argument_list|(
name|pc
argument_list|,
operator|&
name|iphp
operator|->
name|rrp
argument_list|)
return|;
block|}
block|}
name|iphp
operator|->
name|rrp
operator|.
name|current
operator|=
name|p
expr_stmt|;
name|pc
operator|->
name|sockaddr
operator|=
name|peer
operator|->
name|sockaddr
expr_stmt|;
name|pc
operator|->
name|socklen
operator|=
name|peer
operator|->
name|socklen
expr_stmt|;
name|pc
operator|->
name|name
operator|=
operator|&
name|peer
operator|->
name|name
expr_stmt|;
comment|/* ngx_unlock_mutex(iphp->rrp.peers->mutex); */
name|iphp
operator|->
name|rrp
operator|.
name|tried
index|[
name|n
index|]
operator||=
name|m
expr_stmt|;
name|iphp
operator|->
name|hash
operator|=
name|hash
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
DECL|function|ngx_http_upstream_ip_hash (ngx_conf_t * cf,ngx_command_t * cmd,void * conf)
name|ngx_http_upstream_ip_hash
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
name|ngx_http_upstream_srv_conf_t
modifier|*
name|uscf
decl_stmt|;
name|uscf
operator|=
name|ngx_http_conf_get_module_srv_conf
argument_list|(
name|cf
argument_list|,
name|ngx_http_upstream_module
argument_list|)
expr_stmt|;
name|uscf
operator|->
name|peer
operator|.
name|init_upstream
operator|=
name|ngx_http_upstream_init_ip_hash
expr_stmt|;
name|uscf
operator|->
name|flags
operator|=
name|NGX_HTTP_UPSTREAM_CREATE
operator||
name|NGX_HTTP_UPSTREAM_WEIGHT
operator||
name|NGX_HTTP_UPSTREAM_MAX_FAILS
operator||
name|NGX_HTTP_UPSTREAM_FAIL_TIMEOUT
operator||
name|NGX_HTTP_UPSTREAM_DOWN
expr_stmt|;
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

end_unit

