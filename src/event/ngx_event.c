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

begin_define
DECL|macro|DEFAULT_CONNECTIONS
define|#
directive|define
name|DEFAULT_CONNECTIONS
value|512
end_define

begin_decl_stmt
specifier|extern
name|ngx_module_t
name|ngx_select_module
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|ngx_event_module_t
name|ngx_select_module_ctx
decl_stmt|;
end_decl_stmt

begin_if
if|#
directive|if
operator|(
name|HAVE_KQUEUE
operator|)
end_if

begin_include
include|#
directive|include
file|<ngx_kqueue_module.h>
end_include

begin_endif
endif|#
directive|endif
end_endif

begin_if
if|#
directive|if
operator|(
name|HAVE_DEVPOLL
operator|)
end_if

begin_decl_stmt
specifier|extern
name|ngx_module_t
name|ngx_devpoll_module
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|ngx_event_module_t
name|ngx_devpoll_module_ctx
decl_stmt|;
end_decl_stmt

begin_endif
endif|#
directive|endif
end_endif

begin_if
if|#
directive|if
operator|(
name|HAVE_EPOLL
operator|)
end_if

begin_decl_stmt
specifier|extern
name|ngx_module_t
name|ngx_epoll_module
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|ngx_event_module_t
name|ngx_epoll_module_ctx
decl_stmt|;
end_decl_stmt

begin_endif
endif|#
directive|endif
end_endif

begin_if
if|#
directive|if
operator|(
name|HAVE_AIO
operator|)
end_if

begin_include
include|#
directive|include
file|<ngx_aio_module.h>
end_include

begin_endif
endif|#
directive|endif
end_endif

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_event_module_init
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_event_process_init
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|char
modifier|*
name|ngx_events_block
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
name|char
modifier|*
name|ngx_event_connections
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
name|char
modifier|*
name|ngx_event_use
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
name|ngx_event_create_conf
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|char
modifier|*
name|ngx_event_init_conf
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|,
name|void
modifier|*
name|conf
parameter_list|)
function_decl|;
end_function_decl

begin_decl_stmt
DECL|variable|ngx_event_max_module
specifier|static
name|ngx_uint_t
name|ngx_event_max_module
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_event_flags
name|ngx_uint_t
name|ngx_event_flags
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_event_actions
name|ngx_event_actions_t
name|ngx_event_actions
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_accept_mutex_ptr
name|ngx_atomic_t
modifier|*
name|ngx_accept_mutex_ptr
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_accept_mutex
name|ngx_atomic_t
modifier|*
name|ngx_accept_mutex
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_accept_mutex_held
name|ngx_uint_t
name|ngx_accept_mutex_held
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_posted_events
name|ngx_thread_volatile
name|ngx_event_t
modifier|*
name|ngx_posted_events
decl_stmt|;
end_decl_stmt

begin_if
if|#
directive|if
operator|(
name|NGX_THREADS
operator|)
end_if

begin_decl_stmt
DECL|variable|ngx_posted_events_mutex
name|ngx_mutex_t
modifier|*
name|ngx_posted_events_mutex
decl_stmt|;
end_decl_stmt

begin_endif
endif|#
directive|endif
end_endif

begin_decl_stmt
DECL|variable|events_name
specifier|static
name|ngx_str_t
name|events_name
init|=
name|ngx_string
argument_list|(
literal|"events"
argument_list|)
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_events_commands
specifier|static
name|ngx_command_t
name|ngx_events_commands
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"events"
argument_list|)
block|,
name|NGX_MAIN_CONF
operator||
name|NGX_CONF_BLOCK
operator||
name|NGX_CONF_NOARGS
block|,
name|ngx_events_block
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
DECL|variable|ngx_events_module
name|ngx_module_t
name|ngx_events_module
init|=
block|{
name|NGX_MODULE
block|,
operator|&
name|events_name
block|,
comment|/* module context */
name|ngx_events_commands
block|,
comment|/* module directives */
name|NGX_CORE_MODULE
block|,
comment|/* module type */
name|NULL
block|,
comment|/* init module */
name|NULL
comment|/* init child */
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|event_core_name
specifier|static
name|ngx_str_t
name|event_core_name
init|=
name|ngx_string
argument_list|(
literal|"event_core"
argument_list|)
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_event_core_commands
specifier|static
name|ngx_command_t
name|ngx_event_core_commands
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"connections"
argument_list|)
block|,
name|NGX_EVENT_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_event_connections
block|,
literal|0
block|,
literal|0
block|,
name|NULL
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"use"
argument_list|)
block|,
name|NGX_EVENT_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_event_use
block|,
literal|0
block|,
literal|0
block|,
name|NULL
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"multi_accept"
argument_list|)
block|,
name|NGX_EVENT_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_conf_set_flag_slot
block|,
literal|0
block|,
name|offsetof
argument_list|(
name|ngx_event_conf_t
argument_list|,
name|multi_accept
argument_list|)
block|,
name|NULL
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"accept_mutex"
argument_list|)
block|,
name|NGX_EVENT_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_conf_set_flag_slot
block|,
literal|0
block|,
name|offsetof
argument_list|(
name|ngx_event_conf_t
argument_list|,
name|accept_mutex
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
DECL|variable|ngx_event_core_module_ctx
name|ngx_event_module_t
name|ngx_event_core_module_ctx
init|=
block|{
operator|&
name|event_core_name
block|,
name|ngx_event_create_conf
block|,
comment|/* create configuration */
name|ngx_event_init_conf
block|,
comment|/* init configuration */
block|{
name|NULL
block|,
name|NULL
block|,
name|NULL
block|,
name|NULL
block|,
name|NULL
block|,
name|NULL
block|,
name|NULL
block|,
name|NULL
block|,
name|NULL
block|}
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_event_core_module
name|ngx_module_t
name|ngx_event_core_module
init|=
block|{
name|NGX_MODULE
block|,
operator|&
name|ngx_event_core_module_ctx
block|,
comment|/* module context */
name|ngx_event_core_commands
block|,
comment|/* module directives */
name|NGX_EVENT_MODULE
block|,
comment|/* module type */
name|ngx_event_module_init
block|,
comment|/* init module */
name|ngx_event_process_init
comment|/* init process */
block|}
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|ngx_event_module_init (ngx_cycle_t * cycle)
specifier|static
name|ngx_int_t
name|ngx_event_module_init
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
block|{
if|#
directive|if
operator|!
operator|(
name|WIN32
operator|)
name|ngx_core_conf_t
modifier|*
name|ccf
decl_stmt|;
name|ngx_event_conf_t
modifier|*
name|ecf
decl_stmt|;
name|ccf
operator|=
operator|(
name|ngx_core_conf_t
operator|*
operator|)
name|ngx_get_conf
argument_list|(
name|cycle
operator|->
name|conf_ctx
argument_list|,
name|ngx_core_module
argument_list|)
expr_stmt|;
if|if
condition|(
name|ccf
operator|->
name|master
operator|==
literal|0
condition|)
block|{
return|return
name|NGX_OK
return|;
block|}
name|ecf
operator|=
name|ngx_event_get_conf
argument_list|(
name|cycle
operator|->
name|conf_ctx
argument_list|,
name|ngx_event_core_module
argument_list|)
expr_stmt|;
if|if
condition|(
name|ecf
operator|->
name|accept_mutex
operator|==
literal|0
condition|)
block|{
return|return
name|NGX_OK
return|;
block|}
name|ngx_accept_mutex_ptr
operator|=
operator|(
name|ngx_atomic_t
operator|*
operator|)
name|mmap
argument_list|(
name|NULL
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_atomic_t
argument_list|)
argument_list|,
name|PROT_READ
operator||
name|PROT_WRITE
argument_list|,
name|MAP_ANON
operator||
name|MAP_SHARED
argument_list|,
operator|-
literal|1
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|ngx_accept_mutex_ptr
operator|==
name|NULL
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_EMERG
argument_list|,
name|cycle
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"mmap(MAP_ANON|MAP_SHARED) failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
endif|#
directive|endif
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_event_process_init (ngx_cycle_t * cycle)
specifier|static
name|ngx_int_t
name|ngx_event_process_init
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
block|{
name|ngx_uint_t
name|m
decl_stmt|,
name|i
decl_stmt|;
name|ngx_socket_t
name|fd
decl_stmt|;
name|ngx_event_t
modifier|*
name|rev
decl_stmt|,
modifier|*
name|wev
decl_stmt|;
name|ngx_listening_t
modifier|*
name|s
decl_stmt|;
name|ngx_connection_t
modifier|*
name|c
decl_stmt|;
name|ngx_core_conf_t
modifier|*
name|ccf
decl_stmt|;
name|ngx_event_conf_t
modifier|*
name|ecf
decl_stmt|;
name|ngx_event_module_t
modifier|*
name|module
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
name|ccf
operator|=
operator|(
name|ngx_core_conf_t
operator|*
operator|)
name|ngx_get_conf
argument_list|(
name|cycle
operator|->
name|conf_ctx
argument_list|,
name|ngx_core_module
argument_list|)
expr_stmt|;
if|if
condition|(
name|ccf
operator|->
name|worker_processes
operator|>
literal|1
operator|&&
name|ngx_accept_mutex_ptr
condition|)
block|{
name|ngx_accept_mutex
operator|=
name|ngx_accept_mutex_ptr
expr_stmt|;
name|ngx_accept_mutex_held
operator|=
literal|1
expr_stmt|;
block|}
if|#
directive|if
operator|(
name|NGX_THREADS
operator|)
if|if
condition|(
operator|!
operator|(
name|ngx_posted_events_mutex
operator|=
name|ngx_mutex_init
argument_list|(
name|cycle
operator|->
name|log
argument_list|,
literal|0
argument_list|)
operator|)
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
endif|#
directive|endif
if|if
condition|(
name|ngx_event_timer_init
argument_list|(
name|cycle
operator|->
name|log
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|ecf
operator|=
name|ngx_event_get_conf
argument_list|(
name|cycle
operator|->
name|conf_ctx
argument_list|,
name|ngx_event_core_module
argument_list|)
expr_stmt|;
name|cycle
operator|->
name|connection_n
operator|=
name|ecf
operator|->
name|connections
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
name|NGX_EVENT_MODULE
condition|)
block|{
continue|continue;
block|}
if|if
condition|(
name|ngx_modules
index|[
name|m
index|]
operator|->
name|ctx_index
operator|==
name|ecf
operator|->
name|use
condition|)
block|{
name|module
operator|=
name|ngx_modules
index|[
name|m
index|]
operator|->
name|ctx
expr_stmt|;
if|if
condition|(
name|module
operator|->
name|actions
operator|.
name|init
argument_list|(
name|cycle
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
comment|/* fatal */
name|exit
argument_list|(
literal|2
argument_list|)
expr_stmt|;
block|}
break|break;
block|}
block|}
name|cycle
operator|->
name|connections
operator|=
name|ngx_alloc
argument_list|(
sizeof|sizeof
argument_list|(
name|ngx_connection_t
argument_list|)
operator|*
name|ecf
operator|->
name|connections
argument_list|,
name|cycle
operator|->
name|log
argument_list|)
expr_stmt|;
if|if
condition|(
name|cycle
operator|->
name|connections
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|c
operator|=
name|cycle
operator|->
name|connections
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|cycle
operator|->
name|connection_n
condition|;
name|i
operator|++
control|)
block|{
name|c
index|[
name|i
index|]
operator|.
name|fd
operator|=
operator|(
name|ngx_socket_t
operator|)
operator|-
literal|1
expr_stmt|;
block|}
name|cycle
operator|->
name|read_events
operator|=
name|ngx_alloc
argument_list|(
sizeof|sizeof
argument_list|(
name|ngx_event_t
argument_list|)
operator|*
name|ecf
operator|->
name|connections
argument_list|,
name|cycle
operator|->
name|log
argument_list|)
expr_stmt|;
if|if
condition|(
name|cycle
operator|->
name|read_events
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|cycle
operator|->
name|write_events
operator|=
name|ngx_alloc
argument_list|(
sizeof|sizeof
argument_list|(
name|ngx_event_t
argument_list|)
operator|*
name|ecf
operator|->
name|connections
argument_list|,
name|cycle
operator|->
name|log
argument_list|)
expr_stmt|;
if|if
condition|(
name|cycle
operator|->
name|write_events
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
comment|/* for each listening socket */
name|s
operator|=
name|cycle
operator|->
name|listening
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
name|cycle
operator|->
name|listening
operator|.
name|nelts
condition|;
name|i
operator|++
control|)
block|{
name|fd
operator|=
name|s
index|[
name|i
index|]
operator|.
name|fd
expr_stmt|;
if|#
directive|if
operator|(
name|WIN32
operator|)
comment|/*          * Winsock assignes a socket number divisible by 4          * so to find a connection we divide a socket number by 4.          */
name|fd
operator|/=
literal|4
expr_stmt|;
endif|#
directive|endif
name|c
operator|=
operator|&
name|cycle
operator|->
name|connections
index|[
name|fd
index|]
expr_stmt|;
name|rev
operator|=
operator|&
name|cycle
operator|->
name|read_events
index|[
name|fd
index|]
expr_stmt|;
name|wev
operator|=
operator|&
name|cycle
operator|->
name|write_events
index|[
name|fd
index|]
expr_stmt|;
name|ngx_memzero
argument_list|(
name|c
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_connection_t
argument_list|)
argument_list|)
expr_stmt|;
name|ngx_memzero
argument_list|(
name|rev
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_event_t
argument_list|)
argument_list|)
expr_stmt|;
name|c
operator|->
name|fd
operator|=
name|s
index|[
name|i
index|]
operator|.
name|fd
expr_stmt|;
name|c
operator|->
name|listening
operator|=
operator|&
name|s
index|[
name|i
index|]
expr_stmt|;
name|c
operator|->
name|ctx
operator|=
name|s
index|[
name|i
index|]
operator|.
name|ctx
expr_stmt|;
name|c
operator|->
name|servers
operator|=
name|s
index|[
name|i
index|]
operator|.
name|servers
expr_stmt|;
name|c
operator|->
name|log
operator|=
name|s
index|[
name|i
index|]
operator|.
name|log
expr_stmt|;
name|c
operator|->
name|read
operator|=
name|rev
expr_stmt|;
comment|/* required by iocp in "c->write->active = 1" */
name|c
operator|->
name|write
operator|=
name|wev
expr_stmt|;
comment|/* required by poll */
name|wev
operator|->
name|index
operator|=
name|NGX_INVALID_INDEX
expr_stmt|;
name|rev
operator|->
name|log
operator|=
name|c
operator|->
name|log
expr_stmt|;
name|rev
operator|->
name|data
operator|=
name|c
expr_stmt|;
name|rev
operator|->
name|index
operator|=
name|NGX_INVALID_INDEX
expr_stmt|;
name|rev
operator|->
name|available
operator|=
literal|0
expr_stmt|;
name|rev
operator|->
name|accept
operator|=
literal|1
expr_stmt|;
if|#
directive|if
operator|(
name|HAVE_DEFERRED_ACCEPT
operator|)
name|rev
operator|->
name|deferred_accept
operator|=
name|s
index|[
name|i
index|]
operator|.
name|deferred_accept
expr_stmt|;
endif|#
directive|endif
if|if
condition|(
operator|!
operator|(
name|ngx_event_flags
operator|&
name|NGX_USE_IOCP_EVENT
operator|)
condition|)
block|{
if|if
condition|(
name|s
index|[
name|i
index|]
operator|.
name|remain
condition|)
block|{
comment|/*                  * delete the old accept events that were bound to                  * the old cycle read events array                  */
if|if
condition|(
name|ngx_del_event
argument_list|(
operator|&
name|cycle
operator|->
name|old_cycle
operator|->
name|read_events
index|[
name|fd
index|]
argument_list|,
name|NGX_READ_EVENT
argument_list|,
name|NGX_CLOSE_EVENT
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|cycle
operator|->
name|old_cycle
operator|->
name|connections
index|[
name|fd
index|]
operator|.
name|fd
operator|=
operator|(
name|ngx_socket_t
operator|)
operator|-
literal|1
expr_stmt|;
block|}
block|}
if|#
directive|if
operator|(
name|WIN32
operator|)
if|if
condition|(
name|ngx_event_flags
operator|&
name|NGX_USE_IOCP_EVENT
condition|)
block|{
name|rev
operator|->
name|event_handler
operator|=
operator|&
name|ngx_event_acceptex
expr_stmt|;
if|if
condition|(
name|ngx_add_event
argument_list|(
name|rev
argument_list|,
literal|0
argument_list|,
name|NGX_IOCP_ACCEPT
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|iocpcf
operator|=
name|ngx_event_get_conf
argument_list|(
name|cycle
operator|->
name|conf_ctx
argument_list|,
name|ngx_iocp_module
argument_list|)
expr_stmt|;
if|if
condition|(
name|ngx_event_post_acceptex
argument_list|(
operator|&
name|s
index|[
name|i
index|]
argument_list|,
name|iocpcf
operator|->
name|post_acceptex
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
block|}
else|else
block|{
name|rev
operator|->
name|event_handler
operator|=
operator|&
name|ngx_event_accept
expr_stmt|;
if|if
condition|(
name|ngx_add_event
argument_list|(
name|rev
argument_list|,
name|NGX_READ_EVENT
argument_list|,
literal|0
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
block|}
else|#
directive|else
name|rev
operator|->
name|event_handler
operator|=
operator|&
name|ngx_event_accept
expr_stmt|;
if|if
condition|(
name|ngx_event_flags
operator|&
name|NGX_USE_SIGIO_EVENT
condition|)
block|{
if|if
condition|(
name|ngx_add_conn
argument_list|(
name|c
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
block|}
else|else
block|{
if|if
condition|(
name|ngx_add_event
argument_list|(
name|rev
argument_list|,
name|NGX_READ_EVENT
argument_list|,
literal|0
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
block|}
endif|#
directive|endif
block|}
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_events_block (ngx_conf_t * cf,ngx_command_t * cmd,void * conf)
specifier|static
name|char
modifier|*
name|ngx_events_block
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
name|m
decl_stmt|;
name|char
modifier|*
name|rv
decl_stmt|;
name|void
modifier|*
modifier|*
modifier|*
name|ctx
decl_stmt|;
name|ngx_conf_t
name|pcf
decl_stmt|;
name|ngx_event_module_t
modifier|*
name|module
decl_stmt|;
comment|/* count the number of the event modules and set up their indices */
name|ngx_event_max_module
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
name|NGX_EVENT_MODULE
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
name|ngx_event_max_module
operator|++
expr_stmt|;
block|}
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
name|void
operator|*
argument_list|)
argument_list|)
argument_list|,
name|NGX_CONF_ERROR
argument_list|)
expr_stmt|;
name|ngx_test_null
argument_list|(
operator|*
name|ctx
argument_list|,
name|ngx_pcalloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
name|ngx_event_max_module
operator|*
sizeof|sizeof
argument_list|(
name|void
operator|*
argument_list|)
argument_list|)
argument_list|,
name|NGX_CONF_ERROR
argument_list|)
expr_stmt|;
operator|*
operator|(
name|void
operator|*
operator|*
operator|)
name|conf
operator|=
name|ctx
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
name|NGX_EVENT_MODULE
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
if|if
condition|(
name|module
operator|->
name|create_conf
condition|)
block|{
name|ngx_test_null
argument_list|(
operator|(
operator|*
name|ctx
operator|)
index|[
name|ngx_modules
index|[
name|m
index|]
operator|->
name|ctx_index
index|]
argument_list|,
name|module
operator|->
name|create_conf
argument_list|(
name|cf
operator|->
name|cycle
argument_list|)
argument_list|,
name|NGX_CONF_ERROR
argument_list|)
expr_stmt|;
block|}
block|}
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
name|NGX_EVENT_MODULE
expr_stmt|;
name|cf
operator|->
name|cmd_type
operator|=
name|NGX_EVENT_CONF
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
name|NGX_EVENT_MODULE
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
if|if
condition|(
name|module
operator|->
name|init_conf
condition|)
block|{
name|rv
operator|=
name|module
operator|->
name|init_conf
argument_list|(
name|cf
operator|->
name|cycle
argument_list|,
operator|(
operator|*
name|ctx
operator|)
index|[
name|ngx_modules
index|[
name|m
index|]
operator|->
name|ctx_index
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
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_event_connections (ngx_conf_t * cf,ngx_command_t * cmd,void * conf)
specifier|static
name|char
modifier|*
name|ngx_event_connections
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
name|ngx_event_conf_t
modifier|*
name|ecf
init|=
name|conf
decl_stmt|;
name|ngx_str_t
modifier|*
name|value
decl_stmt|;
if|if
condition|(
name|ecf
operator|->
name|connections
operator|!=
name|NGX_CONF_UNSET
condition|)
block|{
return|return
literal|"is duplicate"
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
name|ecf
operator|->
name|connections
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
name|ecf
operator|->
name|connections
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
literal|"invalid number \"%s\""
argument_list|,
name|value
index|[
literal|1
index|]
operator|.
name|data
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|cf
operator|->
name|cycle
operator|->
name|connection_n
operator|=
name|ecf
operator|->
name|connections
expr_stmt|;
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_event_use (ngx_conf_t * cf,ngx_command_t * cmd,void * conf)
specifier|static
name|char
modifier|*
name|ngx_event_use
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
name|ngx_event_conf_t
modifier|*
name|ecf
init|=
name|conf
decl_stmt|;
name|ngx_int_t
name|m
decl_stmt|;
name|ngx_str_t
modifier|*
name|value
decl_stmt|;
name|ngx_event_conf_t
modifier|*
name|old_ecf
decl_stmt|;
name|ngx_event_module_t
modifier|*
name|module
decl_stmt|;
if|if
condition|(
name|ecf
operator|->
name|use
operator|!=
name|NGX_CONF_UNSET
condition|)
block|{
return|return
literal|"is duplicate"
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
if|if
condition|(
name|cf
operator|->
name|cycle
operator|->
name|old_cycle
operator|->
name|conf_ctx
condition|)
block|{
name|old_ecf
operator|=
name|ngx_event_get_conf
argument_list|(
name|cf
operator|->
name|cycle
operator|->
name|old_cycle
operator|->
name|conf_ctx
argument_list|,
name|ngx_event_core_module
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|old_ecf
operator|=
name|NULL
expr_stmt|;
block|}
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
name|NGX_EVENT_MODULE
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
if|if
condition|(
name|module
operator|->
name|name
operator|->
name|len
operator|==
name|value
index|[
literal|1
index|]
operator|.
name|len
condition|)
block|{
if|if
condition|(
name|ngx_strcmp
argument_list|(
name|module
operator|->
name|name
operator|->
name|data
argument_list|,
name|value
index|[
literal|1
index|]
operator|.
name|data
argument_list|)
operator|==
literal|0
condition|)
block|{
name|ecf
operator|->
name|use
operator|=
name|ngx_modules
index|[
name|m
index|]
operator|->
name|ctx_index
expr_stmt|;
name|ecf
operator|->
name|name
operator|=
name|module
operator|->
name|name
operator|->
name|data
expr_stmt|;
if|if
condition|(
name|ngx_process
operator|==
name|NGX_PROCESS_SINGLE
operator|&&
name|old_ecf
operator|&&
name|old_ecf
operator|->
name|use
operator|!=
name|ecf
operator|->
name|use
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
literal|"when the server runs without a master process "
literal|"the \"%s\" event type must be the same as "
literal|"in previous configuration - \"%s\" "
literal|"and it can not be changed on the fly, "
literal|"to change it you need to stop server "
literal|"and start it again"
argument_list|,
name|value
index|[
literal|1
index|]
operator|.
name|data
argument_list|,
name|old_ecf
operator|->
name|name
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
block|}
block|}
name|ngx_conf_log_error
argument_list|(
name|NGX_LOG_EMERG
argument_list|,
name|cf
argument_list|,
literal|0
argument_list|,
literal|"invalid event type \"%s\""
argument_list|,
name|value
index|[
literal|1
index|]
operator|.
name|data
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
end_function

begin_function
DECL|function|ngx_event_create_conf (ngx_cycle_t * cycle)
specifier|static
name|void
modifier|*
name|ngx_event_create_conf
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
block|{
name|ngx_event_conf_t
modifier|*
name|ecf
decl_stmt|;
name|ngx_test_null
argument_list|(
name|ecf
argument_list|,
name|ngx_palloc
argument_list|(
name|cycle
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_event_conf_t
argument_list|)
argument_list|)
argument_list|,
name|NGX_CONF_ERROR
argument_list|)
expr_stmt|;
name|ecf
operator|->
name|connections
operator|=
name|NGX_CONF_UNSET
expr_stmt|;
name|ecf
operator|->
name|use
operator|=
name|NGX_CONF_UNSET
expr_stmt|;
name|ecf
operator|->
name|multi_accept
operator|=
name|NGX_CONF_UNSET
expr_stmt|;
name|ecf
operator|->
name|accept_mutex
operator|=
name|NGX_CONF_UNSET
expr_stmt|;
name|ecf
operator|->
name|name
operator|=
operator|(
name|void
operator|*
operator|)
name|NGX_CONF_UNSET
expr_stmt|;
return|return
name|ecf
return|;
block|}
end_function

begin_function
DECL|function|ngx_event_init_conf (ngx_cycle_t * cycle,void * conf)
specifier|static
name|char
modifier|*
name|ngx_event_init_conf
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|,
name|void
modifier|*
name|conf
parameter_list|)
block|{
name|ngx_event_conf_t
modifier|*
name|ecf
init|=
name|conf
decl_stmt|;
if|#
directive|if
operator|(
name|HAVE_KQUEUE
operator|)
name|ngx_conf_init_value
argument_list|(
name|ecf
operator|->
name|connections
argument_list|,
name|DEFAULT_CONNECTIONS
argument_list|)
expr_stmt|;
name|ngx_conf_init_value
argument_list|(
name|ecf
operator|->
name|use
argument_list|,
name|ngx_kqueue_module
operator|.
name|ctx_index
argument_list|)
expr_stmt|;
name|ngx_conf_init_ptr_value
argument_list|(
name|ecf
operator|->
name|name
argument_list|,
name|ngx_kqueue_module_ctx
operator|.
name|name
operator|->
name|data
argument_list|)
expr_stmt|;
elif|#
directive|elif
operator|(
name|HAVE_DEVPOLL
operator|)
name|ngx_conf_init_value
argument_list|(
name|ecf
operator|->
name|connections
argument_list|,
name|DEFAULT_CONNECTIONS
argument_list|)
expr_stmt|;
name|ngx_conf_init_value
argument_list|(
name|ecf
operator|->
name|use
argument_list|,
name|ngx_devpoll_module
operator|.
name|ctx_index
argument_list|)
expr_stmt|;
name|ngx_conf_init_ptr_value
argument_list|(
name|ecf
operator|->
name|name
argument_list|,
name|ngx_devpoll_module_ctx
operator|.
name|name
operator|->
name|data
argument_list|)
expr_stmt|;
elif|#
directive|elif
operator|(
name|HAVE_EPOLL
operator|)
name|ngx_conf_init_value
argument_list|(
name|ecf
operator|->
name|connections
argument_list|,
name|DEFAULT_CONNECTIONS
argument_list|)
expr_stmt|;
name|ngx_conf_init_value
argument_list|(
name|ecf
operator|->
name|use
argument_list|,
name|ngx_epoll_module
operator|.
name|ctx_index
argument_list|)
expr_stmt|;
name|ngx_conf_init_ptr_value
argument_list|(
name|ecf
operator|->
name|name
argument_list|,
name|ngx_epoll_module_ctx
operator|.
name|name
operator|->
name|data
argument_list|)
expr_stmt|;
elif|#
directive|elif
operator|(
name|HAVE_SELECT
operator|)
name|ngx_conf_init_value
argument_list|(
name|ecf
operator|->
name|connections
argument_list|,
name|FD_SETSIZE
operator|<
name|DEFAULT_CONNECTIONS
condition|?
name|FD_SETSIZE
else|:
name|DEFAULT_CONNECTIONS
argument_list|)
expr_stmt|;
name|ngx_conf_init_value
argument_list|(
name|ecf
operator|->
name|use
argument_list|,
name|ngx_select_module
operator|.
name|ctx_index
argument_list|)
expr_stmt|;
name|ngx_conf_init_ptr_value
argument_list|(
name|ecf
operator|->
name|name
argument_list|,
name|ngx_select_module_ctx
operator|.
name|name
operator|->
name|data
argument_list|)
expr_stmt|;
else|#
directive|else
name|ngx_int_t
name|i
decl_stmt|,
name|m
decl_stmt|;
name|ngx_event_module_t
modifier|*
name|module
decl_stmt|;
name|m
operator|=
operator|-
literal|1
expr_stmt|;
name|module
operator|=
name|NULL
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|ngx_modules
index|[
name|i
index|]
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|ngx_modules
index|[
name|i
index|]
operator|->
name|type
operator|==
name|NGX_EVENT_MODULE
condition|)
block|{
name|module
operator|=
name|ngx_modules
index|[
name|i
index|]
operator|->
name|ctx
expr_stmt|;
if|if
condition|(
name|ngx_strcmp
argument_list|(
name|module
operator|->
name|name
operator|->
name|data
argument_list|,
name|event_core_name
operator|.
name|data
argument_list|)
operator|==
literal|0
condition|)
block|{
continue|continue;
block|}
name|m
operator|=
name|ngx_modules
index|[
name|i
index|]
operator|->
name|ctx_index
expr_stmt|;
break|break;
block|}
block|}
if|if
condition|(
name|m
operator|==
operator|-
literal|1
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_EMERG
argument_list|,
name|cycle
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"no events module found"
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|ngx_conf_init_value
argument_list|(
name|ecf
operator|->
name|connections
argument_list|,
name|DEFAULT_CONNECTIONS
argument_list|)
expr_stmt|;
name|ngx_conf_init_value
argument_list|(
name|ecf
operator|->
name|use
argument_list|,
name|m
argument_list|)
expr_stmt|;
name|ngx_conf_init_ptr_value
argument_list|(
name|ecf
operator|->
name|name
argument_list|,
name|module
operator|->
name|name
operator|->
name|data
argument_list|)
expr_stmt|;
endif|#
directive|endif
name|cycle
operator|->
name|connection_n
operator|=
name|ecf
operator|->
name|connections
expr_stmt|;
name|ngx_conf_init_value
argument_list|(
name|ecf
operator|->
name|multi_accept
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|ngx_conf_init_value
argument_list|(
name|ecf
operator|->
name|accept_mutex
argument_list|,
literal|1
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

end_unit

