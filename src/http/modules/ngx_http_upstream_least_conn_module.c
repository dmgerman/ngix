begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (C) Maxim Dounin  * Copyright (C) Nginx, Inc.  */
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
DECL|struct|__anon287e37360108
typedef|typedef
struct|struct
block|{
DECL|member|conns
name|ngx_uint_t
modifier|*
name|conns
decl_stmt|;
DECL|typedef|ngx_http_upstream_least_conn_conf_t
block|}
name|ngx_http_upstream_least_conn_conf_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon287e37360208
typedef|typedef
struct|struct
block|{
comment|/* the round robin data must be first */
DECL|member|rrp
name|ngx_http_upstream_rr_peer_data_t
name|rrp
decl_stmt|;
DECL|member|conns
name|ngx_uint_t
modifier|*
name|conns
decl_stmt|;
DECL|member|get_rr_peer
name|ngx_event_get_peer_pt
name|get_rr_peer
decl_stmt|;
DECL|member|free_rr_peer
name|ngx_event_free_peer_pt
name|free_rr_peer
decl_stmt|;
DECL|typedef|ngx_http_upstream_lc_peer_data_t
block|}
name|ngx_http_upstream_lc_peer_data_t
typedef|;
end_typedef

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_upstream_init_least_conn_peer
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
name|ngx_http_upstream_get_least_conn_peer
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
name|void
name|ngx_http_upstream_free_least_conn_peer
parameter_list|(
name|ngx_peer_connection_t
modifier|*
name|pc
parameter_list|,
name|void
modifier|*
name|data
parameter_list|,
name|ngx_uint_t
name|state
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|void
modifier|*
name|ngx_http_upstream_least_conn_create_conf
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
name|ngx_http_upstream_least_conn
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
DECL|variable|ngx_http_upstream_least_conn_commands
specifier|static
name|ngx_command_t
name|ngx_http_upstream_least_conn_commands
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"least_conn"
argument_list|)
block|,
name|NGX_HTTP_UPS_CONF
operator||
name|NGX_CONF_NOARGS
block|,
name|ngx_http_upstream_least_conn
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
DECL|variable|ngx_http_upstream_least_conn_module_ctx
specifier|static
name|ngx_http_module_t
name|ngx_http_upstream_least_conn_module_ctx
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
name|ngx_http_upstream_least_conn_create_conf
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
DECL|variable|ngx_http_upstream_least_conn_module
name|ngx_module_t
name|ngx_http_upstream_least_conn_module
init|=
block|{
name|NGX_MODULE_V1
block|,
operator|&
name|ngx_http_upstream_least_conn_module_ctx
block|,
comment|/* module context */
name|ngx_http_upstream_least_conn_commands
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
specifier|static
name|ngx_int_t
DECL|function|ngx_http_upstream_init_least_conn (ngx_conf_t * cf,ngx_http_upstream_srv_conf_t * us)
name|ngx_http_upstream_init_least_conn
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
name|ngx_uint_t
name|n
decl_stmt|;
name|ngx_http_upstream_rr_peers_t
modifier|*
name|peers
decl_stmt|;
name|ngx_http_upstream_least_conn_conf_t
modifier|*
name|lcf
decl_stmt|;
name|ngx_log_debug0
argument_list|(
name|NGX_LOG_DEBUG_HTTP
argument_list|,
name|cf
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"init least conn"
argument_list|)
expr_stmt|;
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
name|peers
operator|=
name|us
operator|->
name|peer
operator|.
name|data
expr_stmt|;
name|n
operator|=
name|peers
operator|->
name|number
expr_stmt|;
if|if
condition|(
name|peers
operator|->
name|next
condition|)
block|{
name|n
operator|+=
name|peers
operator|->
name|next
operator|->
name|number
expr_stmt|;
block|}
name|lcf
operator|=
name|ngx_http_conf_upstream_srv_conf
argument_list|(
name|us
argument_list|,
name|ngx_http_upstream_least_conn_module
argument_list|)
expr_stmt|;
name|lcf
operator|->
name|conns
operator|=
name|ngx_pcalloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_uint_t
argument_list|)
operator|*
name|n
argument_list|)
expr_stmt|;
if|if
condition|(
name|lcf
operator|->
name|conns
operator|==
name|NULL
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
name|ngx_http_upstream_init_least_conn_peer
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_upstream_init_least_conn_peer (ngx_http_request_t * r,ngx_http_upstream_srv_conf_t * us)
name|ngx_http_upstream_init_least_conn_peer
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
name|ngx_http_upstream_lc_peer_data_t
modifier|*
name|lcp
decl_stmt|;
name|ngx_http_upstream_least_conn_conf_t
modifier|*
name|lcf
decl_stmt|;
name|ngx_log_debug0
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
literal|"init least conn peer"
argument_list|)
expr_stmt|;
name|lcf
operator|=
name|ngx_http_conf_upstream_srv_conf
argument_list|(
name|us
argument_list|,
name|ngx_http_upstream_least_conn_module
argument_list|)
expr_stmt|;
name|lcp
operator|=
name|ngx_palloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_upstream_lc_peer_data_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|lcp
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|lcp
operator|->
name|conns
operator|=
name|lcf
operator|->
name|conns
expr_stmt|;
name|r
operator|->
name|upstream
operator|->
name|peer
operator|.
name|data
operator|=
operator|&
name|lcp
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
name|ngx_http_upstream_get_least_conn_peer
expr_stmt|;
name|r
operator|->
name|upstream
operator|->
name|peer
operator|.
name|free
operator|=
name|ngx_http_upstream_free_least_conn_peer
expr_stmt|;
name|lcp
operator|->
name|get_rr_peer
operator|=
name|ngx_http_upstream_get_round_robin_peer
expr_stmt|;
name|lcp
operator|->
name|free_rr_peer
operator|=
name|ngx_http_upstream_free_round_robin_peer
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_upstream_get_least_conn_peer (ngx_peer_connection_t * pc,void * data)
name|ngx_http_upstream_get_least_conn_peer
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
name|ngx_http_upstream_lc_peer_data_t
modifier|*
name|lcp
init|=
name|data
decl_stmt|;
name|time_t
name|now
decl_stmt|;
name|uintptr_t
name|m
decl_stmt|;
name|ngx_int_t
name|rc
decl_stmt|,
name|total
decl_stmt|;
name|ngx_uint_t
name|i
decl_stmt|,
name|n
decl_stmt|,
name|p
decl_stmt|,
name|many
decl_stmt|;
name|ngx_http_upstream_rr_peer_t
modifier|*
name|peer
decl_stmt|,
modifier|*
name|best
decl_stmt|;
name|ngx_http_upstream_rr_peers_t
modifier|*
name|peers
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
literal|"get least conn peer, try: %ui"
argument_list|,
name|pc
operator|->
name|tries
argument_list|)
expr_stmt|;
if|if
condition|(
name|lcp
operator|->
name|rrp
operator|.
name|peers
operator|->
name|single
condition|)
block|{
return|return
name|lcp
operator|->
name|get_rr_peer
argument_list|(
name|pc
argument_list|,
operator|&
name|lcp
operator|->
name|rrp
argument_list|)
return|;
block|}
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
name|now
operator|=
name|ngx_time
argument_list|()
expr_stmt|;
name|peers
operator|=
name|lcp
operator|->
name|rrp
operator|.
name|peers
expr_stmt|;
name|best
operator|=
name|NULL
expr_stmt|;
name|total
operator|=
literal|0
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_SUPPRESS_WARN
operator|)
name|many
operator|=
literal|0
expr_stmt|;
name|p
operator|=
literal|0
expr_stmt|;
endif|#
directive|endif
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|peers
operator|->
name|number
condition|;
name|i
operator|++
control|)
block|{
name|n
operator|=
name|i
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
name|i
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
name|lcp
operator|->
name|rrp
operator|.
name|tried
index|[
name|n
index|]
operator|&
name|m
condition|)
block|{
continue|continue;
block|}
name|peer
operator|=
operator|&
name|peers
operator|->
name|peer
index|[
name|i
index|]
expr_stmt|;
if|if
condition|(
name|peer
operator|->
name|down
condition|)
block|{
continue|continue;
block|}
if|if
condition|(
name|peer
operator|->
name|max_fails
operator|&&
name|peer
operator|->
name|fails
operator|>=
name|peer
operator|->
name|max_fails
operator|&&
name|now
operator|-
name|peer
operator|->
name|checked
operator|<=
name|peer
operator|->
name|fail_timeout
condition|)
block|{
continue|continue;
block|}
comment|/*          * select peer with least number of connections; if there are          * multiple peers with the same number of connections, select          * based on round-robin          */
if|if
condition|(
name|best
operator|==
name|NULL
operator|||
name|lcp
operator|->
name|conns
index|[
name|i
index|]
operator|*
name|best
operator|->
name|weight
operator|<
name|lcp
operator|->
name|conns
index|[
name|p
index|]
operator|*
name|peer
operator|->
name|weight
condition|)
block|{
name|best
operator|=
name|peer
expr_stmt|;
name|many
operator|=
literal|0
expr_stmt|;
name|p
operator|=
name|i
expr_stmt|;
block|}
if|else if
condition|(
name|lcp
operator|->
name|conns
index|[
name|i
index|]
operator|*
name|best
operator|->
name|weight
operator|==
name|lcp
operator|->
name|conns
index|[
name|p
index|]
operator|*
name|peer
operator|->
name|weight
condition|)
block|{
name|many
operator|=
literal|1
expr_stmt|;
block|}
block|}
if|if
condition|(
name|best
operator|==
name|NULL
condition|)
block|{
name|ngx_log_debug0
argument_list|(
name|NGX_LOG_DEBUG_HTTP
argument_list|,
name|pc
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"get least conn peer, no peer found"
argument_list|)
expr_stmt|;
goto|goto
name|failed
goto|;
block|}
if|if
condition|(
name|many
condition|)
block|{
name|ngx_log_debug0
argument_list|(
name|NGX_LOG_DEBUG_HTTP
argument_list|,
name|pc
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"get least conn peer, many"
argument_list|)
expr_stmt|;
for|for
control|(
name|i
operator|=
name|p
init|;
name|i
operator|<
name|peers
operator|->
name|number
condition|;
name|i
operator|++
control|)
block|{
name|n
operator|=
name|i
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
name|i
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
name|lcp
operator|->
name|rrp
operator|.
name|tried
index|[
name|n
index|]
operator|&
name|m
condition|)
block|{
continue|continue;
block|}
name|peer
operator|=
operator|&
name|peers
operator|->
name|peer
index|[
name|i
index|]
expr_stmt|;
if|if
condition|(
name|peer
operator|->
name|down
condition|)
block|{
continue|continue;
block|}
if|if
condition|(
name|lcp
operator|->
name|conns
index|[
name|i
index|]
operator|*
name|best
operator|->
name|weight
operator|!=
name|lcp
operator|->
name|conns
index|[
name|p
index|]
operator|*
name|peer
operator|->
name|weight
condition|)
block|{
continue|continue;
block|}
if|if
condition|(
name|peer
operator|->
name|max_fails
operator|&&
name|peer
operator|->
name|fails
operator|>=
name|peer
operator|->
name|max_fails
operator|&&
name|now
operator|-
name|peer
operator|->
name|checked
operator|<=
name|peer
operator|->
name|fail_timeout
condition|)
block|{
continue|continue;
block|}
name|peer
operator|->
name|current_weight
operator|+=
name|peer
operator|->
name|effective_weight
expr_stmt|;
name|total
operator|+=
name|peer
operator|->
name|effective_weight
expr_stmt|;
if|if
condition|(
name|peer
operator|->
name|effective_weight
operator|<
name|peer
operator|->
name|weight
condition|)
block|{
name|peer
operator|->
name|effective_weight
operator|++
expr_stmt|;
block|}
if|if
condition|(
name|peer
operator|->
name|current_weight
operator|>
name|best
operator|->
name|current_weight
condition|)
block|{
name|best
operator|=
name|peer
expr_stmt|;
name|p
operator|=
name|i
expr_stmt|;
block|}
block|}
block|}
name|best
operator|->
name|current_weight
operator|-=
name|total
expr_stmt|;
name|best
operator|->
name|checked
operator|=
name|now
expr_stmt|;
name|pc
operator|->
name|sockaddr
operator|=
name|best
operator|->
name|sockaddr
expr_stmt|;
name|pc
operator|->
name|socklen
operator|=
name|best
operator|->
name|socklen
expr_stmt|;
name|pc
operator|->
name|name
operator|=
operator|&
name|best
operator|->
name|name
expr_stmt|;
name|lcp
operator|->
name|rrp
operator|.
name|current
operator|=
name|p
expr_stmt|;
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
name|lcp
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
name|lcp
operator|->
name|conns
index|[
name|p
index|]
operator|++
expr_stmt|;
if|if
condition|(
name|pc
operator|->
name|tries
operator|==
literal|1
operator|&&
name|peers
operator|->
name|next
condition|)
block|{
name|pc
operator|->
name|tries
operator|+=
name|peers
operator|->
name|next
operator|->
name|number
expr_stmt|;
block|}
return|return
name|NGX_OK
return|;
name|failed
label|:
if|if
condition|(
name|peers
operator|->
name|next
condition|)
block|{
name|ngx_log_debug0
argument_list|(
name|NGX_LOG_DEBUG_HTTP
argument_list|,
name|pc
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"get least conn peer, backup servers"
argument_list|)
expr_stmt|;
name|lcp
operator|->
name|conns
operator|+=
name|peers
operator|->
name|number
expr_stmt|;
name|lcp
operator|->
name|rrp
operator|.
name|peers
operator|=
name|peers
operator|->
name|next
expr_stmt|;
name|pc
operator|->
name|tries
operator|=
name|lcp
operator|->
name|rrp
operator|.
name|peers
operator|->
name|number
expr_stmt|;
name|n
operator|=
name|lcp
operator|->
name|rrp
operator|.
name|peers
operator|->
name|number
operator|/
operator|(
literal|8
operator|*
sizeof|sizeof
argument_list|(
name|uintptr_t
argument_list|)
operator|)
operator|+
literal|1
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|n
condition|;
name|i
operator|++
control|)
block|{
name|lcp
operator|->
name|rrp
operator|.
name|tried
index|[
name|i
index|]
operator|=
literal|0
expr_stmt|;
block|}
name|rc
operator|=
name|ngx_http_upstream_get_least_conn_peer
argument_list|(
name|pc
argument_list|,
name|lcp
argument_list|)
expr_stmt|;
if|if
condition|(
name|rc
operator|!=
name|NGX_BUSY
condition|)
block|{
return|return
name|rc
return|;
block|}
block|}
comment|/* all peers failed, mark them as live for quick recovery */
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|peers
operator|->
name|number
condition|;
name|i
operator|++
control|)
block|{
name|peers
operator|->
name|peer
index|[
name|i
index|]
operator|.
name|fails
operator|=
literal|0
expr_stmt|;
block|}
name|pc
operator|->
name|name
operator|=
name|peers
operator|->
name|name
expr_stmt|;
return|return
name|NGX_BUSY
return|;
block|}
end_function

begin_function
specifier|static
name|void
DECL|function|ngx_http_upstream_free_least_conn_peer (ngx_peer_connection_t * pc,void * data,ngx_uint_t state)
name|ngx_http_upstream_free_least_conn_peer
parameter_list|(
name|ngx_peer_connection_t
modifier|*
name|pc
parameter_list|,
name|void
modifier|*
name|data
parameter_list|,
name|ngx_uint_t
name|state
parameter_list|)
block|{
name|ngx_http_upstream_lc_peer_data_t
modifier|*
name|lcp
init|=
name|data
decl_stmt|;
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
literal|"free least conn peer %ui %ui"
argument_list|,
name|pc
operator|->
name|tries
argument_list|,
name|state
argument_list|)
expr_stmt|;
if|if
condition|(
name|lcp
operator|->
name|rrp
operator|.
name|peers
operator|->
name|single
condition|)
block|{
name|lcp
operator|->
name|free_rr_peer
argument_list|(
name|pc
argument_list|,
operator|&
name|lcp
operator|->
name|rrp
argument_list|,
name|state
argument_list|)
expr_stmt|;
return|return;
block|}
if|if
condition|(
name|state
operator|==
literal|0
operator|&&
name|pc
operator|->
name|tries
operator|==
literal|0
condition|)
block|{
return|return;
block|}
name|lcp
operator|->
name|conns
index|[
name|lcp
operator|->
name|rrp
operator|.
name|current
index|]
operator|--
expr_stmt|;
name|lcp
operator|->
name|free_rr_peer
argument_list|(
name|pc
argument_list|,
operator|&
name|lcp
operator|->
name|rrp
argument_list|,
name|state
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
specifier|static
name|void
modifier|*
DECL|function|ngx_http_upstream_least_conn_create_conf (ngx_conf_t * cf)
name|ngx_http_upstream_least_conn_create_conf
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
block|{
name|ngx_http_upstream_least_conn_conf_t
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
name|ngx_http_upstream_least_conn_conf_t
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
comment|/*      * set by ngx_pcalloc():      *      *     conf->conns = NULL;      */
return|return
name|conf
return|;
block|}
end_function

begin_function
specifier|static
name|char
modifier|*
DECL|function|ngx_http_upstream_least_conn (ngx_conf_t * cf,ngx_command_t * cmd,void * conf)
name|ngx_http_upstream_least_conn
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
name|ngx_http_upstream_init_least_conn
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
operator||
name|NGX_HTTP_UPSTREAM_BACKUP
expr_stmt|;
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

end_unit
