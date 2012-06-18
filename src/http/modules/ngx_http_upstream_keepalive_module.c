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
DECL|struct|__anon2c3b440a0108
typedef|typedef
struct|struct
block|{
DECL|member|max_cached
name|ngx_uint_t
name|max_cached
decl_stmt|;
DECL|member|cache
name|ngx_queue_t
name|cache
decl_stmt|;
DECL|member|free
name|ngx_queue_t
name|free
decl_stmt|;
DECL|member|original_init_upstream
name|ngx_http_upstream_init_pt
name|original_init_upstream
decl_stmt|;
DECL|member|original_init_peer
name|ngx_http_upstream_init_peer_pt
name|original_init_peer
decl_stmt|;
DECL|typedef|ngx_http_upstream_keepalive_srv_conf_t
block|}
name|ngx_http_upstream_keepalive_srv_conf_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon2c3b440a0208
typedef|typedef
struct|struct
block|{
DECL|member|conf
name|ngx_http_upstream_keepalive_srv_conf_t
modifier|*
name|conf
decl_stmt|;
DECL|member|upstream
name|ngx_http_upstream_t
modifier|*
name|upstream
decl_stmt|;
DECL|member|data
name|void
modifier|*
name|data
decl_stmt|;
DECL|member|original_get_peer
name|ngx_event_get_peer_pt
name|original_get_peer
decl_stmt|;
DECL|member|original_free_peer
name|ngx_event_free_peer_pt
name|original_free_peer
decl_stmt|;
if|#
directive|if
operator|(
name|NGX_HTTP_SSL
operator|)
DECL|member|original_set_session
name|ngx_event_set_peer_session_pt
name|original_set_session
decl_stmt|;
DECL|member|original_save_session
name|ngx_event_save_peer_session_pt
name|original_save_session
decl_stmt|;
endif|#
directive|endif
DECL|member|failed
name|ngx_uint_t
name|failed
decl_stmt|;
comment|/* unsigned:1 */
DECL|typedef|ngx_http_upstream_keepalive_peer_data_t
block|}
name|ngx_http_upstream_keepalive_peer_data_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon2c3b440a0308
typedef|typedef
struct|struct
block|{
DECL|member|conf
name|ngx_http_upstream_keepalive_srv_conf_t
modifier|*
name|conf
decl_stmt|;
DECL|member|queue
name|ngx_queue_t
name|queue
decl_stmt|;
DECL|member|connection
name|ngx_connection_t
modifier|*
name|connection
decl_stmt|;
DECL|member|socklen
name|socklen_t
name|socklen
decl_stmt|;
DECL|member|sockaddr
name|u_char
name|sockaddr
index|[
name|NGX_SOCKADDRLEN
index|]
decl_stmt|;
DECL|typedef|ngx_http_upstream_keepalive_cache_t
block|}
name|ngx_http_upstream_keepalive_cache_t
typedef|;
end_typedef

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_upstream_init_keepalive_peer
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
name|ngx_http_upstream_get_keepalive_peer
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
name|ngx_http_upstream_free_keepalive_peer
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
name|ngx_http_upstream_keepalive_dummy_handler
parameter_list|(
name|ngx_event_t
modifier|*
name|ev
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|void
name|ngx_http_upstream_keepalive_close_handler
parameter_list|(
name|ngx_event_t
modifier|*
name|ev
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|void
name|ngx_http_upstream_keepalive_close
parameter_list|(
name|ngx_connection_t
modifier|*
name|c
parameter_list|)
function_decl|;
end_function_decl

begin_if
if|#
directive|if
operator|(
name|NGX_HTTP_SSL
operator|)
end_if

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_upstream_keepalive_set_session
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
name|ngx_http_upstream_keepalive_save_session
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

begin_endif
endif|#
directive|endif
end_endif

begin_function_decl
specifier|static
name|void
modifier|*
name|ngx_http_upstream_keepalive_create_conf
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
name|ngx_http_upstream_keepalive
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
DECL|variable|ngx_http_upstream_keepalive_commands
specifier|static
name|ngx_command_t
name|ngx_http_upstream_keepalive_commands
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"keepalive"
argument_list|)
block|,
name|NGX_HTTP_UPS_CONF
operator||
name|NGX_CONF_TAKE12
block|,
name|ngx_http_upstream_keepalive
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
DECL|variable|ngx_http_upstream_keepalive_module_ctx
specifier|static
name|ngx_http_module_t
name|ngx_http_upstream_keepalive_module_ctx
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
name|ngx_http_upstream_keepalive_create_conf
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
DECL|variable|ngx_http_upstream_keepalive_module
name|ngx_module_t
name|ngx_http_upstream_keepalive_module
init|=
block|{
name|NGX_MODULE_V1
block|,
operator|&
name|ngx_http_upstream_keepalive_module_ctx
block|,
comment|/* module context */
name|ngx_http_upstream_keepalive_commands
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
DECL|function|ngx_http_upstream_init_keepalive (ngx_conf_t * cf,ngx_http_upstream_srv_conf_t * us)
name|ngx_http_upstream_init_keepalive
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
name|i
decl_stmt|;
name|ngx_http_upstream_keepalive_srv_conf_t
modifier|*
name|kcf
decl_stmt|;
name|ngx_http_upstream_keepalive_cache_t
modifier|*
name|cached
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
literal|"init keepalive"
argument_list|)
expr_stmt|;
name|kcf
operator|=
name|ngx_http_conf_upstream_srv_conf
argument_list|(
name|us
argument_list|,
name|ngx_http_upstream_keepalive_module
argument_list|)
expr_stmt|;
if|if
condition|(
name|kcf
operator|->
name|original_init_upstream
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
name|kcf
operator|->
name|original_init_peer
operator|=
name|us
operator|->
name|peer
operator|.
name|init
expr_stmt|;
name|us
operator|->
name|peer
operator|.
name|init
operator|=
name|ngx_http_upstream_init_keepalive_peer
expr_stmt|;
comment|/* allocate cache items and add to free queue */
name|cached
operator|=
name|ngx_pcalloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_upstream_keepalive_cache_t
argument_list|)
operator|*
name|kcf
operator|->
name|max_cached
argument_list|)
expr_stmt|;
if|if
condition|(
name|cached
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|ngx_queue_init
argument_list|(
operator|&
name|kcf
operator|->
name|cache
argument_list|)
expr_stmt|;
name|ngx_queue_init
argument_list|(
operator|&
name|kcf
operator|->
name|free
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
name|kcf
operator|->
name|max_cached
condition|;
name|i
operator|++
control|)
block|{
name|ngx_queue_insert_head
argument_list|(
operator|&
name|kcf
operator|->
name|free
argument_list|,
operator|&
name|cached
index|[
name|i
index|]
operator|.
name|queue
argument_list|)
expr_stmt|;
name|cached
index|[
name|i
index|]
operator|.
name|conf
operator|=
name|kcf
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
DECL|function|ngx_http_upstream_init_keepalive_peer (ngx_http_request_t * r,ngx_http_upstream_srv_conf_t * us)
name|ngx_http_upstream_init_keepalive_peer
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
name|ngx_http_upstream_keepalive_peer_data_t
modifier|*
name|kp
decl_stmt|;
name|ngx_http_upstream_keepalive_srv_conf_t
modifier|*
name|kcf
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
literal|"init keepalive peer"
argument_list|)
expr_stmt|;
name|kcf
operator|=
name|ngx_http_conf_upstream_srv_conf
argument_list|(
name|us
argument_list|,
name|ngx_http_upstream_keepalive_module
argument_list|)
expr_stmt|;
name|kp
operator|=
name|ngx_palloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_upstream_keepalive_peer_data_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|kp
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|kcf
operator|->
name|original_init_peer
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
name|kp
operator|->
name|conf
operator|=
name|kcf
expr_stmt|;
name|kp
operator|->
name|upstream
operator|=
name|r
operator|->
name|upstream
expr_stmt|;
name|kp
operator|->
name|data
operator|=
name|r
operator|->
name|upstream
operator|->
name|peer
operator|.
name|data
expr_stmt|;
name|kp
operator|->
name|original_get_peer
operator|=
name|r
operator|->
name|upstream
operator|->
name|peer
operator|.
name|get
expr_stmt|;
name|kp
operator|->
name|original_free_peer
operator|=
name|r
operator|->
name|upstream
operator|->
name|peer
operator|.
name|free
expr_stmt|;
name|r
operator|->
name|upstream
operator|->
name|peer
operator|.
name|data
operator|=
name|kp
expr_stmt|;
name|r
operator|->
name|upstream
operator|->
name|peer
operator|.
name|get
operator|=
name|ngx_http_upstream_get_keepalive_peer
expr_stmt|;
name|r
operator|->
name|upstream
operator|->
name|peer
operator|.
name|free
operator|=
name|ngx_http_upstream_free_keepalive_peer
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_HTTP_SSL
operator|)
name|kp
operator|->
name|original_set_session
operator|=
name|r
operator|->
name|upstream
operator|->
name|peer
operator|.
name|set_session
expr_stmt|;
name|kp
operator|->
name|original_save_session
operator|=
name|r
operator|->
name|upstream
operator|->
name|peer
operator|.
name|save_session
expr_stmt|;
name|r
operator|->
name|upstream
operator|->
name|peer
operator|.
name|set_session
operator|=
name|ngx_http_upstream_keepalive_set_session
expr_stmt|;
name|r
operator|->
name|upstream
operator|->
name|peer
operator|.
name|save_session
operator|=
name|ngx_http_upstream_keepalive_save_session
expr_stmt|;
endif|#
directive|endif
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_upstream_get_keepalive_peer (ngx_peer_connection_t * pc,void * data)
name|ngx_http_upstream_get_keepalive_peer
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
name|ngx_http_upstream_keepalive_peer_data_t
modifier|*
name|kp
init|=
name|data
decl_stmt|;
name|ngx_http_upstream_keepalive_cache_t
modifier|*
name|item
decl_stmt|;
name|ngx_int_t
name|rc
decl_stmt|;
name|ngx_queue_t
modifier|*
name|q
decl_stmt|,
modifier|*
name|cache
decl_stmt|;
name|ngx_connection_t
modifier|*
name|c
decl_stmt|;
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
literal|"get keepalive peer"
argument_list|)
expr_stmt|;
name|kp
operator|->
name|failed
operator|=
literal|0
expr_stmt|;
comment|/* ask balancer */
name|rc
operator|=
name|kp
operator|->
name|original_get_peer
argument_list|(
name|pc
argument_list|,
name|kp
operator|->
name|data
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
comment|/* search cache for suitable connection */
name|cache
operator|=
operator|&
name|kp
operator|->
name|conf
operator|->
name|cache
expr_stmt|;
for|for
control|(
name|q
operator|=
name|ngx_queue_head
argument_list|(
name|cache
argument_list|)
init|;
name|q
operator|!=
name|ngx_queue_sentinel
argument_list|(
name|cache
argument_list|)
condition|;
name|q
operator|=
name|ngx_queue_next
argument_list|(
name|q
argument_list|)
control|)
block|{
name|item
operator|=
name|ngx_queue_data
argument_list|(
name|q
argument_list|,
name|ngx_http_upstream_keepalive_cache_t
argument_list|,
name|queue
argument_list|)
expr_stmt|;
name|c
operator|=
name|item
operator|->
name|connection
expr_stmt|;
if|if
condition|(
name|ngx_memn2cmp
argument_list|(
operator|(
name|u_char
operator|*
operator|)
operator|&
name|item
operator|->
name|sockaddr
argument_list|,
operator|(
name|u_char
operator|*
operator|)
name|pc
operator|->
name|sockaddr
argument_list|,
name|item
operator|->
name|socklen
argument_list|,
name|pc
operator|->
name|socklen
argument_list|)
operator|==
literal|0
condition|)
block|{
name|ngx_queue_remove
argument_list|(
name|q
argument_list|)
expr_stmt|;
name|ngx_queue_insert_head
argument_list|(
operator|&
name|kp
operator|->
name|conf
operator|->
name|free
argument_list|,
name|q
argument_list|)
expr_stmt|;
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
literal|"get keepalive peer: using connection %p"
argument_list|,
name|c
argument_list|)
expr_stmt|;
name|c
operator|->
name|idle
operator|=
literal|0
expr_stmt|;
name|c
operator|->
name|log
operator|=
name|pc
operator|->
name|log
expr_stmt|;
name|c
operator|->
name|read
operator|->
name|log
operator|=
name|pc
operator|->
name|log
expr_stmt|;
name|c
operator|->
name|write
operator|->
name|log
operator|=
name|pc
operator|->
name|log
expr_stmt|;
name|c
operator|->
name|pool
operator|->
name|log
operator|=
name|pc
operator|->
name|log
expr_stmt|;
name|pc
operator|->
name|connection
operator|=
name|c
expr_stmt|;
name|pc
operator|->
name|cached
operator|=
literal|1
expr_stmt|;
return|return
name|NGX_DONE
return|;
block|}
block|}
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
specifier|static
name|void
DECL|function|ngx_http_upstream_free_keepalive_peer (ngx_peer_connection_t * pc,void * data,ngx_uint_t state)
name|ngx_http_upstream_free_keepalive_peer
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
name|ngx_http_upstream_keepalive_peer_data_t
modifier|*
name|kp
init|=
name|data
decl_stmt|;
name|ngx_http_upstream_keepalive_cache_t
modifier|*
name|item
decl_stmt|;
name|ngx_queue_t
modifier|*
name|q
decl_stmt|;
name|ngx_connection_t
modifier|*
name|c
decl_stmt|;
name|ngx_http_upstream_t
modifier|*
name|u
decl_stmt|;
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
literal|"free keepalive peer"
argument_list|)
expr_stmt|;
comment|/* remember failed state - peer.free() may be called more than once */
if|if
condition|(
name|state
operator|&
name|NGX_PEER_FAILED
condition|)
block|{
name|kp
operator|->
name|failed
operator|=
literal|1
expr_stmt|;
block|}
comment|/* cache valid connections */
name|u
operator|=
name|kp
operator|->
name|upstream
expr_stmt|;
name|c
operator|=
name|pc
operator|->
name|connection
expr_stmt|;
if|if
condition|(
name|kp
operator|->
name|failed
operator|||
name|c
operator|==
name|NULL
operator|||
name|c
operator|->
name|read
operator|->
name|eof
operator|||
name|c
operator|->
name|read
operator|->
name|error
operator|||
name|c
operator|->
name|read
operator|->
name|timedout
operator|||
name|c
operator|->
name|write
operator|->
name|error
operator|||
name|c
operator|->
name|write
operator|->
name|timedout
condition|)
block|{
goto|goto
name|invalid
goto|;
block|}
if|if
condition|(
operator|!
name|u
operator|->
name|keepalive
condition|)
block|{
goto|goto
name|invalid
goto|;
block|}
if|if
condition|(
name|ngx_handle_read_event
argument_list|(
name|c
operator|->
name|read
argument_list|,
literal|0
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
goto|goto
name|invalid
goto|;
block|}
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
literal|"free keepalive peer: saving connection %p"
argument_list|,
name|c
argument_list|)
expr_stmt|;
if|if
condition|(
name|ngx_queue_empty
argument_list|(
operator|&
name|kp
operator|->
name|conf
operator|->
name|free
argument_list|)
condition|)
block|{
name|q
operator|=
name|ngx_queue_last
argument_list|(
operator|&
name|kp
operator|->
name|conf
operator|->
name|cache
argument_list|)
expr_stmt|;
name|ngx_queue_remove
argument_list|(
name|q
argument_list|)
expr_stmt|;
name|item
operator|=
name|ngx_queue_data
argument_list|(
name|q
argument_list|,
name|ngx_http_upstream_keepalive_cache_t
argument_list|,
name|queue
argument_list|)
expr_stmt|;
name|ngx_http_upstream_keepalive_close
argument_list|(
name|item
operator|->
name|connection
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|q
operator|=
name|ngx_queue_head
argument_list|(
operator|&
name|kp
operator|->
name|conf
operator|->
name|free
argument_list|)
expr_stmt|;
name|ngx_queue_remove
argument_list|(
name|q
argument_list|)
expr_stmt|;
name|item
operator|=
name|ngx_queue_data
argument_list|(
name|q
argument_list|,
name|ngx_http_upstream_keepalive_cache_t
argument_list|,
name|queue
argument_list|)
expr_stmt|;
block|}
name|item
operator|->
name|connection
operator|=
name|c
expr_stmt|;
name|ngx_queue_insert_head
argument_list|(
operator|&
name|kp
operator|->
name|conf
operator|->
name|cache
argument_list|,
name|q
argument_list|)
expr_stmt|;
name|pc
operator|->
name|connection
operator|=
name|NULL
expr_stmt|;
if|if
condition|(
name|c
operator|->
name|read
operator|->
name|timer_set
condition|)
block|{
name|ngx_del_timer
argument_list|(
name|c
operator|->
name|read
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|c
operator|->
name|write
operator|->
name|timer_set
condition|)
block|{
name|ngx_del_timer
argument_list|(
name|c
operator|->
name|write
argument_list|)
expr_stmt|;
block|}
name|c
operator|->
name|write
operator|->
name|handler
operator|=
name|ngx_http_upstream_keepalive_dummy_handler
expr_stmt|;
name|c
operator|->
name|read
operator|->
name|handler
operator|=
name|ngx_http_upstream_keepalive_close_handler
expr_stmt|;
name|c
operator|->
name|data
operator|=
name|item
expr_stmt|;
name|c
operator|->
name|idle
operator|=
literal|1
expr_stmt|;
name|c
operator|->
name|log
operator|=
name|ngx_cycle
operator|->
name|log
expr_stmt|;
name|c
operator|->
name|read
operator|->
name|log
operator|=
name|ngx_cycle
operator|->
name|log
expr_stmt|;
name|c
operator|->
name|write
operator|->
name|log
operator|=
name|ngx_cycle
operator|->
name|log
expr_stmt|;
name|c
operator|->
name|pool
operator|->
name|log
operator|=
name|ngx_cycle
operator|->
name|log
expr_stmt|;
name|item
operator|->
name|socklen
operator|=
name|pc
operator|->
name|socklen
expr_stmt|;
name|ngx_memcpy
argument_list|(
operator|&
name|item
operator|->
name|sockaddr
argument_list|,
name|pc
operator|->
name|sockaddr
argument_list|,
name|pc
operator|->
name|socklen
argument_list|)
expr_stmt|;
if|if
condition|(
name|c
operator|->
name|read
operator|->
name|ready
condition|)
block|{
name|ngx_http_upstream_keepalive_close_handler
argument_list|(
name|c
operator|->
name|read
argument_list|)
expr_stmt|;
block|}
name|invalid
label|:
name|kp
operator|->
name|original_free_peer
argument_list|(
name|pc
argument_list|,
name|kp
operator|->
name|data
argument_list|,
name|state
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
specifier|static
name|void
DECL|function|ngx_http_upstream_keepalive_dummy_handler (ngx_event_t * ev)
name|ngx_http_upstream_keepalive_dummy_handler
parameter_list|(
name|ngx_event_t
modifier|*
name|ev
parameter_list|)
block|{
name|ngx_log_debug0
argument_list|(
name|NGX_LOG_DEBUG_HTTP
argument_list|,
name|ev
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"keepalive dummy handler"
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
specifier|static
name|void
DECL|function|ngx_http_upstream_keepalive_close_handler (ngx_event_t * ev)
name|ngx_http_upstream_keepalive_close_handler
parameter_list|(
name|ngx_event_t
modifier|*
name|ev
parameter_list|)
block|{
name|ngx_http_upstream_keepalive_srv_conf_t
modifier|*
name|conf
decl_stmt|;
name|ngx_http_upstream_keepalive_cache_t
modifier|*
name|item
decl_stmt|;
name|int
name|n
decl_stmt|;
name|char
name|buf
index|[
literal|1
index|]
decl_stmt|;
name|ngx_connection_t
modifier|*
name|c
decl_stmt|;
name|ngx_log_debug0
argument_list|(
name|NGX_LOG_DEBUG_HTTP
argument_list|,
name|ev
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"keepalive close handler"
argument_list|)
expr_stmt|;
name|c
operator|=
name|ev
operator|->
name|data
expr_stmt|;
if|if
condition|(
name|c
operator|->
name|close
condition|)
block|{
goto|goto
name|close
goto|;
block|}
name|n
operator|=
name|recv
argument_list|(
name|c
operator|->
name|fd
argument_list|,
name|buf
argument_list|,
literal|1
argument_list|,
name|MSG_PEEK
argument_list|)
expr_stmt|;
if|if
condition|(
name|n
operator|==
operator|-
literal|1
operator|&&
name|ngx_socket_errno
operator|==
name|NGX_EAGAIN
condition|)
block|{
comment|/* stale event */
if|if
condition|(
name|ngx_handle_read_event
argument_list|(
name|c
operator|->
name|read
argument_list|,
literal|0
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
goto|goto
name|close
goto|;
block|}
return|return;
block|}
name|close
label|:
name|item
operator|=
name|c
operator|->
name|data
expr_stmt|;
name|conf
operator|=
name|item
operator|->
name|conf
expr_stmt|;
name|ngx_http_upstream_keepalive_close
argument_list|(
name|c
argument_list|)
expr_stmt|;
name|ngx_queue_remove
argument_list|(
operator|&
name|item
operator|->
name|queue
argument_list|)
expr_stmt|;
name|ngx_queue_insert_head
argument_list|(
operator|&
name|conf
operator|->
name|free
argument_list|,
operator|&
name|item
operator|->
name|queue
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
specifier|static
name|void
DECL|function|ngx_http_upstream_keepalive_close (ngx_connection_t * c)
name|ngx_http_upstream_keepalive_close
parameter_list|(
name|ngx_connection_t
modifier|*
name|c
parameter_list|)
block|{
if|#
directive|if
operator|(
name|NGX_HTTP_SSL
operator|)
if|if
condition|(
name|c
operator|->
name|ssl
condition|)
block|{
name|c
operator|->
name|ssl
operator|->
name|no_wait_shutdown
operator|=
literal|1
expr_stmt|;
name|c
operator|->
name|ssl
operator|->
name|no_send_shutdown
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|ngx_ssl_shutdown
argument_list|(
name|c
argument_list|)
operator|==
name|NGX_AGAIN
condition|)
block|{
name|c
operator|->
name|ssl
operator|->
name|handler
operator|=
name|ngx_http_upstream_keepalive_close
expr_stmt|;
return|return;
block|}
block|}
endif|#
directive|endif
name|ngx_destroy_pool
argument_list|(
name|c
operator|->
name|pool
argument_list|)
expr_stmt|;
name|ngx_close_connection
argument_list|(
name|c
argument_list|)
expr_stmt|;
block|}
end_function

begin_if
if|#
directive|if
operator|(
name|NGX_HTTP_SSL
operator|)
end_if

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_upstream_keepalive_set_session (ngx_peer_connection_t * pc,void * data)
name|ngx_http_upstream_keepalive_set_session
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
name|ngx_http_upstream_keepalive_peer_data_t
modifier|*
name|kp
init|=
name|data
decl_stmt|;
return|return
name|kp
operator|->
name|original_set_session
argument_list|(
name|pc
argument_list|,
name|kp
operator|->
name|data
argument_list|)
return|;
block|}
end_function

begin_function
specifier|static
name|void
DECL|function|ngx_http_upstream_keepalive_save_session (ngx_peer_connection_t * pc,void * data)
name|ngx_http_upstream_keepalive_save_session
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
name|ngx_http_upstream_keepalive_peer_data_t
modifier|*
name|kp
init|=
name|data
decl_stmt|;
name|kp
operator|->
name|original_save_session
argument_list|(
name|pc
argument_list|,
name|kp
operator|->
name|data
argument_list|)
expr_stmt|;
return|return;
block|}
end_function

begin_endif
endif|#
directive|endif
end_endif

begin_function
specifier|static
name|void
modifier|*
DECL|function|ngx_http_upstream_keepalive_create_conf (ngx_conf_t * cf)
name|ngx_http_upstream_keepalive_create_conf
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
block|{
name|ngx_http_upstream_keepalive_srv_conf_t
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
name|ngx_http_upstream_keepalive_srv_conf_t
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
comment|/*      * set by ngx_pcalloc():      *      *     conf->original_init_upstream = NULL;      *     conf->original_init_peer = NULL;      */
name|conf
operator|->
name|max_cached
operator|=
literal|1
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
DECL|function|ngx_http_upstream_keepalive (ngx_conf_t * cf,ngx_command_t * cmd,void * conf)
name|ngx_http_upstream_keepalive
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
name|ngx_http_upstream_keepalive_srv_conf_t
modifier|*
name|kcf
decl_stmt|;
name|ngx_int_t
name|n
decl_stmt|;
name|ngx_str_t
modifier|*
name|value
decl_stmt|;
name|ngx_uint_t
name|i
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
name|kcf
operator|=
name|ngx_http_conf_upstream_srv_conf
argument_list|(
name|uscf
argument_list|,
name|ngx_http_upstream_keepalive_module
argument_list|)
expr_stmt|;
name|kcf
operator|->
name|original_init_upstream
operator|=
name|uscf
operator|->
name|peer
operator|.
name|init_upstream
condition|?
name|uscf
operator|->
name|peer
operator|.
name|init_upstream
else|:
name|ngx_http_upstream_init_round_robin
expr_stmt|;
name|uscf
operator|->
name|peer
operator|.
name|init_upstream
operator|=
name|ngx_http_upstream_init_keepalive
expr_stmt|;
comment|/* read options */
name|value
operator|=
name|cf
operator|->
name|args
operator|->
name|elts
expr_stmt|;
name|n
operator|=
name|ngx_atoi
argument_list|(
name|value
index|[
literal|1
index|]
operator|.
name|data
argument_list|,
name|value
index|[
literal|1
index|]
operator|.
name|len
argument_list|)
expr_stmt|;
if|if
condition|(
name|n
operator|==
name|NGX_ERROR
operator|||
name|n
operator|==
literal|0
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
literal|"invalid value \"%V\" in \"%V\" directive"
argument_list|,
operator|&
name|value
index|[
literal|1
index|]
argument_list|,
operator|&
name|cmd
operator|->
name|name
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|kcf
operator|->
name|max_cached
operator|=
name|n
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|2
init|;
name|i
operator|<
name|cf
operator|->
name|args
operator|->
name|nelts
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|ngx_strcmp
argument_list|(
name|value
index|[
name|i
index|]
operator|.
name|data
argument_list|,
literal|"single"
argument_list|)
operator|==
literal|0
condition|)
block|{
name|ngx_conf_log_error
argument_list|(
name|NGX_LOG_WARN
argument_list|,
name|cf
argument_list|,
literal|0
argument_list|,
literal|"the \"single\" parameter is deprecated"
argument_list|)
expr_stmt|;
continue|continue;
block|}
goto|goto
name|invalid
goto|;
block|}
return|return
name|NGX_CONF_OK
return|;
name|invalid
label|:
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
name|i
index|]
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
end_function

end_unit

