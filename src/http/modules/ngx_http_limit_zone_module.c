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

begin_typedef
DECL|struct|__anon2772aae00108
typedef|typedef
struct|struct
block|{
DECL|member|len
name|u_short
name|len
decl_stmt|;
DECL|member|conn
name|u_short
name|conn
decl_stmt|;
DECL|member|data
name|u_char
name|data
index|[
literal|1
index|]
decl_stmt|;
DECL|typedef|ngx_http_limit_zone_node_t
block|}
name|ngx_http_limit_zone_node_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon2772aae00208
typedef|typedef
struct|struct
block|{
DECL|member|shm_zone
name|ngx_shm_zone_t
modifier|*
name|shm_zone
decl_stmt|;
DECL|member|node
name|ngx_rbtree_node_t
modifier|*
name|node
decl_stmt|;
DECL|typedef|ngx_http_limit_zone_cleanup_t
block|}
name|ngx_http_limit_zone_cleanup_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon2772aae00308
typedef|typedef
struct|struct
block|{
DECL|member|shm_zone
name|ngx_shm_zone_t
modifier|*
name|shm_zone
decl_stmt|;
DECL|member|index
name|ngx_int_t
name|index
decl_stmt|;
DECL|member|conn
name|ngx_uint_t
name|conn
decl_stmt|;
DECL|typedef|ngx_http_limit_zone_conf_t
block|}
name|ngx_http_limit_zone_conf_t
typedef|;
end_typedef

begin_function_decl
specifier|static
name|void
name|ngx_http_limit_zone_cleanup
parameter_list|(
name|void
modifier|*
name|data
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|void
modifier|*
name|ngx_http_limit_zone_create_conf
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
name|ngx_http_limit_zone_merge_conf
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
name|char
modifier|*
name|ngx_http_limit_zone
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
name|ngx_http_limit_conn
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
name|ngx_http_limit_zone_init
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
function_decl|;
end_function_decl

begin_decl_stmt
DECL|variable|ngx_http_limit_zone_commands
specifier|static
name|ngx_command_t
name|ngx_http_limit_zone_commands
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"limit_zone"
argument_list|)
block|,
name|NGX_HTTP_MAIN_CONF
operator||
name|NGX_CONF_TAKE2
block|,
name|ngx_http_limit_zone
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
literal|"limit_conn"
argument_list|)
block|,
name|NGX_HTTP_MAIN_CONF
operator||
name|NGX_HTTP_SRV_CONF
operator||
name|NGX_HTTP_LOC_CONF
operator||
name|NGX_CONF_TAKE3
block|,
name|ngx_http_limit_conn
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
DECL|variable|ngx_http_limit_zone_module_ctx
specifier|static
name|ngx_http_module_t
name|ngx_http_limit_zone_module_ctx
init|=
block|{
name|NULL
block|,
comment|/* preconfiguration */
name|ngx_http_limit_zone_init
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
name|ngx_http_limit_zone_create_conf
block|,
comment|/* create location configration */
name|ngx_http_limit_zone_merge_conf
comment|/* merge location configration */
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_limit_zone_module
name|ngx_module_t
name|ngx_http_limit_zone_module
init|=
block|{
name|NGX_MODULE_V1
block|,
operator|&
name|ngx_http_limit_zone_module_ctx
block|,
comment|/* module context */
name|ngx_http_limit_zone_commands
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
DECL|function|ngx_http_limit_zone_handler (ngx_http_request_t * r)
name|ngx_http_limit_zone_handler
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
block|{
name|size_t
name|len
decl_stmt|,
name|n
decl_stmt|;
name|uint32_t
name|hash
decl_stmt|;
name|ngx_rbtree_t
modifier|*
name|rbtree
decl_stmt|;
name|ngx_slab_pool_t
modifier|*
name|shpool
decl_stmt|;
name|ngx_rbtree_node_t
modifier|*
name|node
decl_stmt|,
modifier|*
name|sentinel
decl_stmt|;
name|ngx_pool_cleanup_t
modifier|*
name|cln
decl_stmt|;
name|ngx_http_variable_value_t
modifier|*
name|vv
decl_stmt|;
name|ngx_http_limit_zone_node_t
modifier|*
name|lz
decl_stmt|;
name|ngx_http_limit_zone_conf_t
modifier|*
name|lzcf
decl_stmt|;
name|ngx_http_limit_zone_cleanup_t
modifier|*
name|lzcln
decl_stmt|;
name|lzcf
operator|=
name|ngx_http_get_module_loc_conf
argument_list|(
name|r
argument_list|,
name|ngx_http_limit_zone_module
argument_list|)
expr_stmt|;
if|if
condition|(
name|lzcf
operator|->
name|shm_zone
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_DECLINED
return|;
block|}
name|vv
operator|=
name|ngx_http_get_indexed_variable
argument_list|(
name|r
argument_list|,
name|lzcf
operator|->
name|index
argument_list|)
expr_stmt|;
if|if
condition|(
name|vv
operator|==
name|NULL
operator|||
name|vv
operator|->
name|not_found
condition|)
block|{
return|return
name|NGX_DECLINED
return|;
block|}
name|len
operator|=
name|vv
operator|->
name|len
expr_stmt|;
name|hash
operator|=
name|ngx_crc32_short
argument_list|(
name|vv
operator|->
name|data
argument_list|,
name|len
argument_list|)
expr_stmt|;
name|cln
operator|=
name|ngx_pool_cleanup_add
argument_list|(
name|r
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_limit_zone_cleanup_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|cln
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
name|rbtree
operator|=
name|lzcf
operator|->
name|shm_zone
operator|->
name|data
expr_stmt|;
name|shpool
operator|=
operator|(
name|ngx_slab_pool_t
operator|*
operator|)
name|lzcf
operator|->
name|shm_zone
operator|->
name|shm
operator|.
name|addr
expr_stmt|;
name|ngx_shmtx_lock
argument_list|(
operator|&
name|shpool
operator|->
name|mutex
argument_list|)
expr_stmt|;
name|node
operator|=
name|rbtree
operator|->
name|root
expr_stmt|;
name|sentinel
operator|=
name|rbtree
operator|->
name|sentinel
expr_stmt|;
while|while
condition|(
name|node
operator|!=
name|sentinel
condition|)
block|{
if|if
condition|(
name|hash
operator|<
name|node
operator|->
name|key
condition|)
block|{
name|node
operator|=
name|node
operator|->
name|left
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|hash
operator|>
name|node
operator|->
name|key
condition|)
block|{
name|node
operator|=
name|node
operator|->
name|right
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|hash
operator|==
name|node
operator|->
name|key
condition|)
block|{
name|lz
operator|=
operator|(
name|ngx_http_limit_zone_node_t
operator|*
operator|)
operator|&
name|node
operator|->
name|data
expr_stmt|;
if|if
condition|(
name|len
operator|==
operator|(
name|size_t
operator|)
name|lz
operator|->
name|len
operator|&&
name|ngx_strncmp
argument_list|(
name|lz
operator|->
name|data
argument_list|,
name|vv
operator|->
name|data
argument_list|,
name|len
argument_list|)
operator|==
literal|0
condition|)
block|{
if|if
condition|(
name|lz
operator|->
name|conn
operator|<
operator|(
name|u_short
operator|)
name|lzcf
operator|->
name|conn
condition|)
block|{
name|lz
operator|->
name|conn
operator|++
expr_stmt|;
goto|goto
name|done
goto|;
block|}
name|ngx_shmtx_unlock
argument_list|(
operator|&
name|shpool
operator|->
name|mutex
argument_list|)
expr_stmt|;
return|return
name|NGX_HTTP_SERVICE_UNAVAILABLE
return|;
block|}
block|}
block|}
name|n
operator|=
name|offsetof
argument_list|(
name|ngx_rbtree_node_t
argument_list|,
name|data
argument_list|)
operator|+
name|offsetof
argument_list|(
name|ngx_http_limit_zone_node_t
argument_list|,
name|data
argument_list|)
operator|+
name|len
expr_stmt|;
name|node
operator|=
name|ngx_slab_alloc_locked
argument_list|(
name|shpool
argument_list|,
name|n
argument_list|)
expr_stmt|;
if|if
condition|(
name|node
operator|==
name|NULL
condition|)
block|{
name|ngx_shmtx_unlock
argument_list|(
operator|&
name|shpool
operator|->
name|mutex
argument_list|)
expr_stmt|;
return|return
name|NGX_HTTP_SERVICE_UNAVAILABLE
return|;
block|}
name|lz
operator|=
operator|(
name|ngx_http_limit_zone_node_t
operator|*
operator|)
operator|&
name|node
operator|->
name|data
expr_stmt|;
name|node
operator|->
name|key
operator|=
name|hash
expr_stmt|;
name|lz
operator|->
name|len
operator|=
operator|(
name|u_short
operator|)
name|len
expr_stmt|;
name|lz
operator|->
name|conn
operator|=
literal|1
expr_stmt|;
name|ngx_memcpy
argument_list|(
name|lz
operator|->
name|data
argument_list|,
name|vv
operator|->
name|data
argument_list|,
name|len
argument_list|)
expr_stmt|;
name|ngx_rbtree_insert
argument_list|(
name|rbtree
argument_list|,
name|node
argument_list|)
expr_stmt|;
name|done
label|:
name|ngx_log_debug2
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
literal|"limit zone: %08XD %d"
argument_list|,
name|node
operator|->
name|key
argument_list|,
name|lz
operator|->
name|conn
argument_list|)
expr_stmt|;
name|ngx_shmtx_unlock
argument_list|(
operator|&
name|shpool
operator|->
name|mutex
argument_list|)
expr_stmt|;
name|cln
operator|->
name|handler
operator|=
name|ngx_http_limit_zone_cleanup
expr_stmt|;
name|lzcln
operator|=
name|cln
operator|->
name|data
expr_stmt|;
name|lzcln
operator|->
name|shm_zone
operator|=
name|lzcf
operator|->
name|shm_zone
expr_stmt|;
name|lzcln
operator|->
name|node
operator|=
name|node
expr_stmt|;
return|return
name|NGX_DECLINED
return|;
block|}
end_function

begin_function
specifier|static
name|void
DECL|function|ngx_http_limit_zone_cleanup (void * data)
name|ngx_http_limit_zone_cleanup
parameter_list|(
name|void
modifier|*
name|data
parameter_list|)
block|{
name|ngx_http_limit_zone_cleanup_t
modifier|*
name|lzcln
init|=
name|data
decl_stmt|;
name|ngx_rbtree_t
modifier|*
name|rbtree
decl_stmt|;
name|ngx_slab_pool_t
modifier|*
name|shpool
decl_stmt|;
name|ngx_rbtree_node_t
modifier|*
name|node
decl_stmt|;
name|ngx_http_limit_zone_node_t
modifier|*
name|lz
decl_stmt|;
name|rbtree
operator|=
name|lzcln
operator|->
name|shm_zone
operator|->
name|data
expr_stmt|;
name|shpool
operator|=
operator|(
name|ngx_slab_pool_t
operator|*
operator|)
name|lzcln
operator|->
name|shm_zone
operator|->
name|shm
operator|.
name|addr
expr_stmt|;
name|node
operator|=
name|lzcln
operator|->
name|node
expr_stmt|;
name|lz
operator|=
operator|(
name|ngx_http_limit_zone_node_t
operator|*
operator|)
operator|&
name|node
operator|->
name|data
expr_stmt|;
name|ngx_shmtx_lock
argument_list|(
operator|&
name|shpool
operator|->
name|mutex
argument_list|)
expr_stmt|;
name|ngx_log_debug2
argument_list|(
name|NGX_LOG_DEBUG_HTTP
argument_list|,
name|lzcln
operator|->
name|shm_zone
operator|->
name|shm
operator|.
name|log
argument_list|,
literal|0
argument_list|,
literal|"limit zone cleanup: %08XD %d"
argument_list|,
name|node
operator|->
name|key
argument_list|,
name|lz
operator|->
name|conn
argument_list|)
expr_stmt|;
name|lz
operator|->
name|conn
operator|--
expr_stmt|;
if|if
condition|(
name|lz
operator|->
name|conn
operator|==
literal|0
condition|)
block|{
name|ngx_rbtree_delete
argument_list|(
name|rbtree
argument_list|,
name|node
argument_list|)
expr_stmt|;
name|ngx_slab_free_locked
argument_list|(
name|shpool
argument_list|,
name|node
argument_list|)
expr_stmt|;
block|}
name|ngx_shmtx_unlock
argument_list|(
operator|&
name|shpool
operator|->
name|mutex
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_limit_zone_init_zone (ngx_shm_zone_t * shm_zone)
name|ngx_http_limit_zone_init_zone
parameter_list|(
name|ngx_shm_zone_t
modifier|*
name|shm_zone
parameter_list|)
block|{
name|ngx_rbtree_t
modifier|*
name|rbtree
decl_stmt|;
name|ngx_slab_pool_t
modifier|*
name|shpool
decl_stmt|;
name|ngx_rbtree_node_t
modifier|*
name|sentinel
decl_stmt|;
name|shpool
operator|=
operator|(
name|ngx_slab_pool_t
operator|*
operator|)
name|shm_zone
operator|->
name|shm
operator|.
name|addr
expr_stmt|;
name|rbtree
operator|=
name|ngx_slab_alloc
argument_list|(
name|shpool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_rbtree_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|rbtree
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|sentinel
operator|=
name|ngx_slab_alloc
argument_list|(
name|shpool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_rbtree_node_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|sentinel
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|ngx_rbtree_sentinel_init
argument_list|(
name|sentinel
argument_list|)
expr_stmt|;
name|rbtree
operator|->
name|root
operator|=
name|sentinel
expr_stmt|;
name|rbtree
operator|->
name|sentinel
operator|=
name|sentinel
expr_stmt|;
name|rbtree
operator|->
name|insert
operator|=
name|ngx_rbtree_insert_value
expr_stmt|;
name|shm_zone
operator|->
name|data
operator|=
name|rbtree
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
DECL|function|ngx_http_limit_zone_create_conf (ngx_conf_t * cf)
name|ngx_http_limit_zone_create_conf
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
block|{
name|ngx_http_limit_zone_conf_t
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
name|ngx_http_limit_zone_conf_t
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
comment|/*      * set by ngx_pcalloc():      *      *     conf->shm_zone = NULL;      *     conf->index = 0;      *     conf->conn = 0;      */
return|return
name|conf
return|;
block|}
end_function

begin_function
specifier|static
name|char
modifier|*
DECL|function|ngx_http_limit_zone_merge_conf (ngx_conf_t * cf,void * parent,void * child)
name|ngx_http_limit_zone_merge_conf
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
name|ngx_http_limit_zone_conf_t
modifier|*
name|prev
init|=
name|parent
decl_stmt|;
name|ngx_http_limit_zone_conf_t
modifier|*
name|conf
init|=
name|child
decl_stmt|;
if|if
condition|(
name|conf
operator|->
name|shm_zone
operator|==
name|NULL
condition|)
block|{
operator|*
name|conf
operator|=
operator|*
name|prev
expr_stmt|;
block|}
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

begin_function
specifier|static
name|char
modifier|*
DECL|function|ngx_http_limit_zone (ngx_conf_t * cf,ngx_command_t * cmd,void * conf)
name|ngx_http_limit_zone
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
name|ssize_t
name|n
decl_stmt|;
name|ngx_str_t
modifier|*
name|value
decl_stmt|;
name|ngx_shm_zone_t
modifier|*
name|shm_zone
decl_stmt|;
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
name|ngx_parse_size
argument_list|(
operator|&
name|value
index|[
literal|2
index|]
argument_list|)
expr_stmt|;
if|if
condition|(
name|n
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
literal|"invalid size of limit_zone \"%V\""
argument_list|,
operator|&
name|value
index|[
literal|2
index|]
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
if|if
condition|(
name|n
operator|<
operator|(
name|ngx_int_t
operator|)
operator|(
literal|8
operator|*
name|ngx_pagesize
operator|)
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
literal|"limit_zone \"%V\" is too small"
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
name|shm_zone
operator|=
name|ngx_shared_memory_add
argument_list|(
name|cf
argument_list|,
operator|&
name|value
index|[
literal|1
index|]
argument_list|,
name|n
argument_list|,
operator|&
name|ngx_http_limit_zone_module
argument_list|)
expr_stmt|;
if|if
condition|(
name|shm_zone
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|shm_zone
operator|->
name|init
operator|=
name|ngx_http_limit_zone_init_zone
expr_stmt|;
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

begin_function
specifier|static
name|char
modifier|*
DECL|function|ngx_http_limit_conn (ngx_conf_t * cf,ngx_command_t * cmd,void * conf)
name|ngx_http_limit_conn
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
name|ngx_http_limit_zone_conf_t
modifier|*
name|lzcf
init|=
name|conf
decl_stmt|;
name|ngx_int_t
name|n
decl_stmt|;
name|ngx_str_t
modifier|*
name|value
decl_stmt|;
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
operator|<=
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
literal|"invalid number of connections"
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
if|if
condition|(
name|value
index|[
literal|2
index|]
operator|.
name|data
index|[
literal|0
index|]
operator|!=
literal|'$'
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
literal|"invalid variable name \"%V\""
argument_list|,
operator|&
name|value
index|[
literal|2
index|]
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|value
index|[
literal|2
index|]
operator|.
name|len
operator|--
expr_stmt|;
name|value
index|[
literal|2
index|]
operator|.
name|data
operator|++
expr_stmt|;
name|lzcf
operator|->
name|index
operator|=
name|ngx_http_get_variable_index
argument_list|(
name|cf
argument_list|,
operator|&
name|value
index|[
literal|2
index|]
argument_list|)
expr_stmt|;
if|if
condition|(
name|lzcf
operator|->
name|index
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|lzcf
operator|->
name|shm_zone
operator|=
name|ngx_shared_memory_add
argument_list|(
name|cf
argument_list|,
operator|&
name|value
index|[
literal|3
index|]
argument_list|,
literal|0
argument_list|,
operator|&
name|ngx_http_limit_zone_module
argument_list|)
expr_stmt|;
if|if
condition|(
name|lzcf
operator|->
name|shm_zone
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|lzcf
operator|->
name|conn
operator|=
name|n
expr_stmt|;
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_limit_zone_init (ngx_conf_t * cf)
name|ngx_http_limit_zone_init
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
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
name|ngx_http_conf_get_module_main_conf
argument_list|(
name|cf
argument_list|,
name|ngx_http_core_module
argument_list|)
expr_stmt|;
name|h
operator|=
name|ngx_array_push
argument_list|(
operator|&
name|cmcf
operator|->
name|phases
index|[
name|NGX_HTTP_PREACCESS_PHASE
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
name|ngx_http_limit_zone_handler
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

end_unit

