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
file|<ngx_http.h>
end_include

begin_typedef
DECL|struct|__anon2950b08b0108
typedef|typedef
struct|struct
block|{
DECL|member|indices
name|ngx_array_t
name|indices
decl_stmt|;
DECL|member|max_index_len
name|size_t
name|max_index_len
decl_stmt|;
DECL|member|index_cache
name|ngx_http_cache_hash_t
modifier|*
name|index_cache
decl_stmt|;
DECL|typedef|ngx_http_index_loc_conf_t
block|}
name|ngx_http_index_loc_conf_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon2950b08b0208
typedef|typedef
struct|struct
block|{
DECL|member|index
name|ngx_uint_t
name|index
decl_stmt|;
DECL|member|last
name|u_char
modifier|*
name|last
decl_stmt|;
DECL|member|path
name|ngx_str_t
name|path
decl_stmt|;
DECL|member|redirect
name|ngx_str_t
name|redirect
decl_stmt|;
DECL|member|cache
name|ngx_http_cache_t
modifier|*
name|cache
decl_stmt|;
DECL|member|tested
name|unsigned
name|tested
range|:
literal|1
decl_stmt|;
DECL|typedef|ngx_http_index_ctx_t
block|}
name|ngx_http_index_ctx_t
typedef|;
end_typedef

begin_define
DECL|macro|NGX_HTTP_DEFAULT_INDEX
define|#
directive|define
name|NGX_HTTP_DEFAULT_INDEX
value|"index.html"
end_define

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_index_test_dir
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_index_ctx_t
modifier|*
name|ctx
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_index_error
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_index_ctx_t
modifier|*
name|ctx
parameter_list|,
name|ngx_err_t
name|err
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_index_init
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|void
modifier|*
name|ngx_http_index_create_loc_conf
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
name|ngx_http_index_merge_loc_conf
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
name|ngx_http_index_set_index
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
DECL|variable|ngx_http_index_commands
specifier|static
name|ngx_command_t
name|ngx_http_index_commands
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"index"
argument_list|)
block|,
name|NGX_HTTP_LOC_CONF
operator||
name|NGX_CONF_1MORE
block|,
name|ngx_http_index_set_index
block|,
name|NGX_HTTP_LOC_CONF_OFFSET
block|,
literal|0
block|,
name|NULL
block|}
block|,
if|#
directive|if
operator|(
name|NGX_HTTP_CACHE
operator|)
block|{
name|ngx_string
argument_list|(
literal|"index_cache"
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
name|ngx_http_set_cache_slot
block|,
name|NGX_HTTP_LOC_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_http_index_loc_conf_t
argument_list|,
name|index_cache
argument_list|)
block|,
name|NULL
block|}
block|,
endif|#
directive|endif
name|ngx_null_command
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_index_module_ctx
name|ngx_http_module_t
name|ngx_http_index_module_ctx
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
name|ngx_http_index_create_loc_conf
block|,
comment|/* create location configration */
name|ngx_http_index_merge_loc_conf
comment|/* merge location configration */
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_index_module
name|ngx_module_t
name|ngx_http_index_module
init|=
block|{
name|NGX_MODULE
block|,
operator|&
name|ngx_http_index_module_ctx
block|,
comment|/* module context */
name|ngx_http_index_commands
block|,
comment|/* module directives */
name|NGX_HTTP_MODULE
block|,
comment|/* module type */
name|ngx_http_index_init
block|,
comment|/* init module */
name|NULL
comment|/* init child */
block|}
decl_stmt|;
end_decl_stmt

begin_comment
comment|/*  * Try to open the first index file before the test of the directory existence  * because the valid requests should be many more than invalid ones.  * If open() failed then stat() should be more quickly because some data  * is already cached in the kernel.  * Besides Win32 has ERROR_PATH_NOT_FOUND (NGX_ENOTDIR).  * Unix has ENOTDIR error, although it less helpfull - it shows only  * that path contains the usual file in place of the directory.  */
end_comment

begin_function
DECL|function|ngx_http_index_handler (ngx_http_request_t * r)
name|ngx_int_t
name|ngx_http_index_handler
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
block|{
name|u_char
modifier|*
name|name
decl_stmt|;
name|ngx_fd_t
name|fd
decl_stmt|;
name|ngx_int_t
name|rc
decl_stmt|;
name|ngx_str_t
modifier|*
name|index
decl_stmt|;
name|ngx_err_t
name|err
decl_stmt|;
name|ngx_log_t
modifier|*
name|log
decl_stmt|;
name|ngx_http_index_ctx_t
modifier|*
name|ctx
decl_stmt|;
name|ngx_http_core_loc_conf_t
modifier|*
name|clcf
decl_stmt|;
name|ngx_http_index_loc_conf_t
modifier|*
name|ilcf
decl_stmt|;
if|#
directive|if
operator|(
name|NGX_HTTP_CACHE
operator|)
name|uint32_t
name|crc
decl_stmt|;
endif|#
directive|endif
if|if
condition|(
name|r
operator|->
name|uri
operator|.
name|data
index|[
name|r
operator|->
name|uri
operator|.
name|len
operator|-
literal|1
index|]
operator|!=
literal|'/'
condition|)
block|{
return|return
name|NGX_DECLINED
return|;
block|}
name|log
operator|=
name|r
operator|->
name|connection
operator|->
name|log
expr_stmt|;
comment|/*      * we use context because the handler supports an async file opening      * and thus can be called several times      */
name|clcf
operator|=
name|ngx_http_get_module_loc_conf
argument_list|(
name|r
argument_list|,
name|ngx_http_core_module
argument_list|)
expr_stmt|;
name|ilcf
operator|=
name|ngx_http_get_module_loc_conf
argument_list|(
name|r
argument_list|,
name|ngx_http_index_module
argument_list|)
expr_stmt|;
name|ctx
operator|=
name|ngx_http_get_module_ctx
argument_list|(
name|r
argument_list|,
name|ngx_http_index_module
argument_list|)
expr_stmt|;
if|if
condition|(
name|ctx
operator|==
name|NULL
condition|)
block|{
name|ngx_http_create_ctx
argument_list|(
name|r
argument_list|,
name|ctx
argument_list|,
name|ngx_http_index_module
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_index_ctx_t
argument_list|)
argument_list|,
name|NGX_HTTP_INTERNAL_SERVER_ERROR
argument_list|)
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_HTTP_CACHE
operator|)
if|if
condition|(
name|ilcf
operator|->
name|index_cache
condition|)
block|{
name|ctx
operator|->
name|cache
operator|=
name|ngx_http_cache_get
argument_list|(
name|ilcf
operator|->
name|index_cache
argument_list|,
name|NULL
argument_list|,
operator|&
name|r
operator|->
name|uri
argument_list|,
operator|&
name|crc
argument_list|)
expr_stmt|;
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_HTTP
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"http index cache get: "
name|PTR_FMT
argument_list|,
name|ctx
operator|->
name|cache
argument_list|)
expr_stmt|;
if|if
condition|(
name|ctx
operator|->
name|cache
operator|&&
operator|!
name|ctx
operator|->
name|cache
operator|->
name|expired
condition|)
block|{
name|ctx
operator|->
name|cache
operator|->
name|accessed
operator|=
name|ngx_cached_time
expr_stmt|;
name|ctx
operator|->
name|redirect
operator|.
name|len
operator|=
name|ctx
operator|->
name|cache
operator|->
name|data
operator|.
name|value
operator|.
name|len
expr_stmt|;
name|ctx
operator|->
name|redirect
operator|.
name|data
operator|=
name|ngx_palloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
name|ctx
operator|->
name|redirect
operator|.
name|len
operator|+
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|ctx
operator|->
name|redirect
operator|.
name|data
operator|==
name|NULL
condition|)
block|{
name|ngx_http_cache_unlock
argument_list|(
name|ilcf
operator|->
name|index_cache
argument_list|,
name|ctx
operator|->
name|cache
argument_list|,
name|log
argument_list|)
expr_stmt|;
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
name|ngx_memcpy
argument_list|(
name|ctx
operator|->
name|redirect
operator|.
name|data
argument_list|,
name|ctx
operator|->
name|cache
operator|->
name|data
operator|.
name|value
operator|.
name|data
argument_list|,
name|ctx
operator|->
name|redirect
operator|.
name|len
operator|+
literal|1
argument_list|)
expr_stmt|;
name|ngx_http_cache_unlock
argument_list|(
name|ilcf
operator|->
name|index_cache
argument_list|,
name|ctx
operator|->
name|cache
argument_list|,
name|log
argument_list|)
expr_stmt|;
return|return
name|ngx_http_internal_redirect
argument_list|(
name|r
argument_list|,
operator|&
name|ctx
operator|->
name|redirect
argument_list|,
name|NULL
argument_list|)
return|;
block|}
block|}
endif|#
directive|endif
name|ctx
operator|->
name|path
operator|.
name|data
operator|=
name|ngx_palloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
name|clcf
operator|->
name|root
operator|.
name|len
operator|+
name|r
operator|->
name|uri
operator|.
name|len
operator|+
name|ilcf
operator|->
name|max_index_len
operator|-
name|clcf
operator|->
name|alias
operator|*
name|clcf
operator|->
name|name
operator|.
name|len
argument_list|)
expr_stmt|;
if|if
condition|(
name|ctx
operator|->
name|path
operator|.
name|data
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
name|ctx
operator|->
name|redirect
operator|.
name|data
operator|=
name|ngx_cpymem
argument_list|(
name|ctx
operator|->
name|path
operator|.
name|data
argument_list|,
name|clcf
operator|->
name|root
operator|.
name|data
argument_list|,
name|clcf
operator|->
name|root
operator|.
name|len
argument_list|)
expr_stmt|;
if|if
condition|(
name|clcf
operator|->
name|alias
condition|)
block|{
name|ctx
operator|->
name|last
operator|=
name|ngx_cpystrn
argument_list|(
name|ctx
operator|->
name|redirect
operator|.
name|data
argument_list|,
name|r
operator|->
name|uri
operator|.
name|data
operator|+
name|clcf
operator|->
name|name
operator|.
name|len
argument_list|,
name|r
operator|->
name|uri
operator|.
name|len
operator|+
literal|1
operator|-
name|clcf
operator|->
name|name
operator|.
name|len
argument_list|)
expr_stmt|;
comment|/*              * aliases usually have trailling "/",              * set it in the start of the possible redirect              */
if|if
condition|(
operator|*
name|ctx
operator|->
name|redirect
operator|.
name|data
operator|!=
literal|'/'
condition|)
block|{
name|ctx
operator|->
name|redirect
operator|.
name|data
operator|--
expr_stmt|;
block|}
block|}
else|else
block|{
name|ctx
operator|->
name|last
operator|=
name|ngx_cpystrn
argument_list|(
name|ctx
operator|->
name|redirect
operator|.
name|data
argument_list|,
name|r
operator|->
name|uri
operator|.
name|data
argument_list|,
name|r
operator|->
name|uri
operator|.
name|len
operator|+
literal|1
argument_list|)
expr_stmt|;
block|}
block|}
name|ctx
operator|->
name|path
operator|.
name|len
operator|=
name|ctx
operator|->
name|last
operator|-
name|ctx
operator|->
name|path
operator|.
name|data
expr_stmt|;
name|index
operator|=
name|ilcf
operator|->
name|indices
operator|.
name|elts
expr_stmt|;
for|for
control|(
comment|/* void */
init|;
name|ctx
operator|->
name|index
operator|<
name|ilcf
operator|->
name|indices
operator|.
name|nelts
condition|;
name|ctx
operator|->
name|index
operator|++
control|)
block|{
if|if
condition|(
name|index
index|[
name|ctx
operator|->
name|index
index|]
operator|.
name|data
index|[
literal|0
index|]
operator|==
literal|'/'
condition|)
block|{
name|name
operator|=
name|index
index|[
name|ctx
operator|->
name|index
index|]
operator|.
name|data
expr_stmt|;
block|}
else|else
block|{
name|ngx_memcpy
argument_list|(
name|ctx
operator|->
name|last
argument_list|,
name|index
index|[
name|ctx
operator|->
name|index
index|]
operator|.
name|data
argument_list|,
name|index
index|[
name|ctx
operator|->
name|index
index|]
operator|.
name|len
operator|+
literal|1
argument_list|)
expr_stmt|;
name|name
operator|=
name|ctx
operator|->
name|path
operator|.
name|data
expr_stmt|;
block|}
name|fd
operator|=
name|ngx_open_file
argument_list|(
name|name
argument_list|,
name|NGX_FILE_RDONLY
argument_list|,
name|NGX_FILE_OPEN
argument_list|)
expr_stmt|;
if|if
condition|(
name|fd
operator|==
operator|(
name|ngx_fd_t
operator|)
name|NGX_AGAIN
condition|)
block|{
return|return
name|NGX_AGAIN
return|;
block|}
if|if
condition|(
name|fd
operator|==
name|NGX_INVALID_FILE
condition|)
block|{
name|err
operator|=
name|ngx_errno
expr_stmt|;
name|ngx_log_error
argument_list|(
name|NGX_LOG_DEBUG
argument_list|,
name|log
argument_list|,
name|err
argument_list|,
literal|"debug: "
name|ngx_open_file_n
literal|" %s failed"
argument_list|,
name|name
argument_list|)
expr_stmt|;
if|if
condition|(
name|err
operator|==
name|NGX_ENOTDIR
condition|)
block|{
return|return
name|ngx_http_index_error
argument_list|(
name|r
argument_list|,
name|ctx
argument_list|,
name|err
argument_list|)
return|;
block|}
if|else if
condition|(
name|err
operator|==
name|NGX_EACCES
condition|)
block|{
return|return
name|ngx_http_index_error
argument_list|(
name|r
argument_list|,
name|ctx
argument_list|,
name|err
argument_list|)
return|;
block|}
if|if
condition|(
operator|!
name|ctx
operator|->
name|tested
condition|)
block|{
name|rc
operator|=
name|ngx_http_index_test_dir
argument_list|(
name|r
argument_list|,
name|ctx
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
name|ctx
operator|->
name|tested
operator|=
literal|1
expr_stmt|;
block|}
if|if
condition|(
name|err
operator|==
name|NGX_ENOENT
condition|)
block|{
continue|continue;
block|}
name|ngx_log_error
argument_list|(
name|NGX_LOG_ERR
argument_list|,
name|log
argument_list|,
name|err
argument_list|,
name|ngx_open_file_n
literal|" %s failed"
argument_list|,
name|name
argument_list|)
expr_stmt|;
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
comment|/* STUB: open file cache */
name|r
operator|->
name|file
operator|.
name|name
operator|.
name|data
operator|=
name|name
expr_stmt|;
name|r
operator|->
name|file
operator|.
name|fd
operator|=
name|fd
expr_stmt|;
if|if
condition|(
name|index
index|[
name|ctx
operator|->
name|index
index|]
operator|.
name|data
index|[
literal|0
index|]
operator|==
literal|'/'
condition|)
block|{
name|r
operator|->
name|file
operator|.
name|name
operator|.
name|len
operator|=
name|index
index|[
name|ctx
operator|->
name|index
index|]
operator|.
name|len
expr_stmt|;
name|ctx
operator|->
name|redirect
operator|.
name|len
operator|=
name|index
index|[
name|ctx
operator|->
name|index
index|]
operator|.
name|len
expr_stmt|;
name|ctx
operator|->
name|redirect
operator|.
name|data
operator|=
name|index
index|[
name|ctx
operator|->
name|index
index|]
operator|.
name|data
expr_stmt|;
block|}
else|else
block|{
name|ctx
operator|->
name|redirect
operator|.
name|len
operator|=
name|r
operator|->
name|uri
operator|.
name|len
operator|+
name|index
index|[
name|ctx
operator|->
name|index
index|]
operator|.
name|len
expr_stmt|;
name|r
operator|->
name|file
operator|.
name|name
operator|.
name|len
operator|=
name|clcf
operator|->
name|root
operator|.
name|len
operator|+
name|r
operator|->
name|uri
operator|.
name|len
operator|+
name|index
index|[
name|ctx
operator|->
name|index
index|]
operator|.
name|len
expr_stmt|;
block|}
comment|/**/
if|#
directive|if
operator|(
name|NGX_HTTP_CACHE
operator|)
if|if
condition|(
name|ilcf
operator|->
name|index_cache
condition|)
block|{
if|if
condition|(
name|ctx
operator|->
name|cache
condition|)
block|{
if|if
condition|(
name|ctx
operator|->
name|redirect
operator|.
name|len
operator|==
name|ctx
operator|->
name|cache
operator|->
name|data
operator|.
name|value
operator|.
name|len
operator|&&
name|ngx_memcmp
argument_list|(
name|ctx
operator|->
name|cache
operator|->
name|data
operator|.
name|value
operator|.
name|data
argument_list|,
name|ctx
operator|->
name|redirect
operator|.
name|data
argument_list|,
name|ctx
operator|->
name|redirect
operator|.
name|len
argument_list|)
operator|==
literal|0
condition|)
block|{
name|ctx
operator|->
name|cache
operator|->
name|accessed
operator|=
name|ngx_cached_time
expr_stmt|;
name|ctx
operator|->
name|cache
operator|->
name|updated
operator|=
name|ngx_cached_time
expr_stmt|;
name|ngx_http_cache_unlock
argument_list|(
name|ilcf
operator|->
name|index_cache
argument_list|,
name|ctx
operator|->
name|cache
argument_list|,
name|log
argument_list|)
expr_stmt|;
return|return
name|ngx_http_internal_redirect
argument_list|(
name|r
argument_list|,
operator|&
name|ctx
operator|->
name|redirect
argument_list|,
name|NULL
argument_list|)
return|;
block|}
block|}
name|ctx
operator|->
name|redirect
operator|.
name|len
operator|++
expr_stmt|;
name|ctx
operator|->
name|cache
operator|=
name|ngx_http_cache_alloc
argument_list|(
name|ilcf
operator|->
name|index_cache
argument_list|,
name|ctx
operator|->
name|cache
argument_list|,
name|NULL
argument_list|,
operator|&
name|r
operator|->
name|uri
argument_list|,
name|crc
argument_list|,
operator|&
name|ctx
operator|->
name|redirect
argument_list|,
name|log
argument_list|)
expr_stmt|;
name|ctx
operator|->
name|redirect
operator|.
name|len
operator|--
expr_stmt|;
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_HTTP
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"http index cache alloc: "
name|PTR_FMT
argument_list|,
name|ctx
operator|->
name|cache
argument_list|)
expr_stmt|;
if|if
condition|(
name|ctx
operator|->
name|cache
condition|)
block|{
name|ctx
operator|->
name|cache
operator|->
name|fd
operator|=
name|NGX_INVALID_FILE
expr_stmt|;
name|ctx
operator|->
name|cache
operator|->
name|accessed
operator|=
name|ngx_cached_time
expr_stmt|;
name|ctx
operator|->
name|cache
operator|->
name|last_modified
operator|=
literal|0
expr_stmt|;
name|ctx
operator|->
name|cache
operator|->
name|updated
operator|=
name|ngx_cached_time
expr_stmt|;
name|ctx
operator|->
name|cache
operator|->
name|memory
operator|=
literal|1
expr_stmt|;
name|ngx_http_cache_unlock
argument_list|(
name|ilcf
operator|->
name|index_cache
argument_list|,
name|ctx
operator|->
name|cache
argument_list|,
name|log
argument_list|)
expr_stmt|;
block|}
block|}
endif|#
directive|endif
return|return
name|ngx_http_internal_redirect
argument_list|(
name|r
argument_list|,
operator|&
name|ctx
operator|->
name|redirect
argument_list|,
name|NULL
argument_list|)
return|;
block|}
return|return
name|NGX_DECLINED
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_index_test_dir (ngx_http_request_t * r,ngx_http_index_ctx_t * ctx)
specifier|static
name|ngx_int_t
name|ngx_http_index_test_dir
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_index_ctx_t
modifier|*
name|ctx
parameter_list|)
block|{
name|ngx_err_t
name|err
decl_stmt|;
name|ctx
operator|->
name|path
operator|.
name|data
index|[
name|ctx
operator|->
name|path
operator|.
name|len
operator|-
literal|1
index|]
operator|=
literal|'\0'
expr_stmt|;
name|ctx
operator|->
name|path
operator|.
name|data
index|[
name|ctx
operator|->
name|path
operator|.
name|len
index|]
operator|=
literal|'\0'
expr_stmt|;
name|ngx_log_debug1
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
literal|"http check dir: \"%s\""
argument_list|,
name|ctx
operator|->
name|path
operator|.
name|data
argument_list|)
expr_stmt|;
if|if
condition|(
name|ngx_file_info
argument_list|(
name|ctx
operator|->
name|path
operator|.
name|data
argument_list|,
operator|&
name|r
operator|->
name|file
operator|.
name|info
argument_list|)
operator|==
operator|-
literal|1
condition|)
block|{
name|err
operator|=
name|ngx_errno
expr_stmt|;
if|if
condition|(
name|err
operator|==
name|NGX_ENOENT
condition|)
block|{
name|ctx
operator|->
name|path
operator|.
name|data
index|[
name|ctx
operator|->
name|path
operator|.
name|len
operator|-
literal|1
index|]
operator|=
literal|'/'
expr_stmt|;
return|return
name|ngx_http_index_error
argument_list|(
name|r
argument_list|,
name|ctx
argument_list|,
name|err
argument_list|)
return|;
block|}
name|ngx_log_error
argument_list|(
name|NGX_LOG_CRIT
argument_list|,
name|r
operator|->
name|connection
operator|->
name|log
argument_list|,
name|err
argument_list|,
name|ngx_file_info_n
literal|" %s failed"
argument_list|,
name|ctx
operator|->
name|path
operator|.
name|data
argument_list|)
expr_stmt|;
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
name|ctx
operator|->
name|path
operator|.
name|data
index|[
name|ctx
operator|->
name|path
operator|.
name|len
operator|-
literal|1
index|]
operator|=
literal|'/'
expr_stmt|;
if|if
condition|(
name|ngx_is_dir
argument_list|(
operator|&
name|r
operator|->
name|file
operator|.
name|info
argument_list|)
condition|)
block|{
return|return
name|NGX_OK
return|;
block|}
comment|/* THINK: not reached ??? */
return|return
name|ngx_http_index_error
argument_list|(
name|r
argument_list|,
name|ctx
argument_list|,
literal|0
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_index_error (ngx_http_request_t * r,ngx_http_index_ctx_t * ctx,ngx_err_t err)
specifier|static
name|ngx_int_t
name|ngx_http_index_error
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_index_ctx_t
modifier|*
name|ctx
parameter_list|,
name|ngx_err_t
name|err
parameter_list|)
block|{
if|if
condition|(
name|err
operator|==
name|NGX_EACCES
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
name|err
argument_list|,
literal|"\"%s\" is forbidden"
argument_list|,
name|ctx
operator|->
name|path
operator|.
name|data
argument_list|)
expr_stmt|;
return|return
name|NGX_HTTP_FORBIDDEN
return|;
block|}
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
name|err
argument_list|,
literal|"\"%s\" is not found"
argument_list|,
name|ctx
operator|->
name|path
operator|.
name|data
argument_list|)
expr_stmt|;
return|return
name|NGX_HTTP_NOT_FOUND
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_index_init (ngx_cycle_t * cycle)
specifier|static
name|ngx_int_t
name|ngx_http_index_init
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
name|ngx_http_conf_ctx_t
modifier|*
name|ctx
decl_stmt|;
name|ngx_http_core_main_conf_t
modifier|*
name|cmcf
decl_stmt|;
name|ctx
operator|=
operator|(
name|ngx_http_conf_ctx_t
operator|*
operator|)
name|cycle
operator|->
name|conf_ctx
index|[
name|ngx_http_module
operator|.
name|index
index|]
expr_stmt|;
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
name|h
operator|=
name|ngx_push_array
argument_list|(
operator|&
name|cmcf
operator|->
name|phases
index|[
name|NGX_HTTP_CONTENT_PHASE
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
name|ngx_http_index_handler
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_index_create_loc_conf (ngx_conf_t * cf)
specifier|static
name|void
modifier|*
name|ngx_http_index_create_loc_conf
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
block|{
name|ngx_http_index_loc_conf_t
modifier|*
name|conf
decl_stmt|;
name|ngx_test_null
argument_list|(
name|conf
argument_list|,
name|ngx_palloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_index_loc_conf_t
argument_list|)
argument_list|)
argument_list|,
name|NGX_CONF_ERROR
argument_list|)
expr_stmt|;
name|ngx_init_array
argument_list|(
name|conf
operator|->
name|indices
argument_list|,
name|cf
operator|->
name|pool
argument_list|,
literal|3
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_str_t
argument_list|)
argument_list|,
name|NGX_CONF_ERROR
argument_list|)
expr_stmt|;
name|conf
operator|->
name|max_index_len
operator|=
literal|0
expr_stmt|;
name|conf
operator|->
name|index_cache
operator|=
name|NULL
expr_stmt|;
return|return
name|conf
return|;
block|}
end_function

begin_comment
comment|/* TODO: remove duplicate indices */
end_comment

begin_function
DECL|function|ngx_http_index_merge_loc_conf (ngx_conf_t * cf,void * parent,void * child)
specifier|static
name|char
modifier|*
name|ngx_http_index_merge_loc_conf
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
name|ngx_http_index_loc_conf_t
modifier|*
name|prev
init|=
name|parent
decl_stmt|;
name|ngx_http_index_loc_conf_t
modifier|*
name|conf
init|=
name|child
decl_stmt|;
name|ngx_uint_t
name|i
decl_stmt|;
name|ngx_str_t
modifier|*
name|index
decl_stmt|,
modifier|*
name|prev_index
decl_stmt|;
if|if
condition|(
name|conf
operator|->
name|max_index_len
operator|==
literal|0
condition|)
block|{
if|if
condition|(
name|prev
operator|->
name|max_index_len
operator|!=
literal|0
condition|)
block|{
name|ngx_memcpy
argument_list|(
name|conf
argument_list|,
name|prev
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_index_loc_conf_t
argument_list|)
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_OK
return|;
block|}
name|ngx_test_null
argument_list|(
name|index
argument_list|,
name|ngx_push_array
argument_list|(
operator|&
name|conf
operator|->
name|indices
argument_list|)
argument_list|,
name|NGX_CONF_ERROR
argument_list|)
expr_stmt|;
name|index
operator|->
name|len
operator|=
sizeof|sizeof
argument_list|(
name|NGX_HTTP_DEFAULT_INDEX
argument_list|)
operator|-
literal|1
expr_stmt|;
name|index
operator|->
name|data
operator|=
operator|(
name|u_char
operator|*
operator|)
name|NGX_HTTP_DEFAULT_INDEX
expr_stmt|;
name|conf
operator|->
name|max_index_len
operator|=
sizeof|sizeof
argument_list|(
name|NGX_HTTP_DEFAULT_INDEX
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_OK
return|;
block|}
if|if
condition|(
name|prev
operator|->
name|max_index_len
operator|!=
literal|0
condition|)
block|{
name|prev_index
operator|=
name|prev
operator|->
name|indices
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
name|prev
operator|->
name|indices
operator|.
name|nelts
condition|;
name|i
operator|++
control|)
block|{
name|ngx_test_null
argument_list|(
name|index
argument_list|,
name|ngx_push_array
argument_list|(
operator|&
name|conf
operator|->
name|indices
argument_list|)
argument_list|,
name|NGX_CONF_ERROR
argument_list|)
expr_stmt|;
name|index
operator|->
name|len
operator|=
name|prev_index
index|[
name|i
index|]
operator|.
name|len
expr_stmt|;
name|index
operator|->
name|data
operator|=
name|prev_index
index|[
name|i
index|]
operator|.
name|data
expr_stmt|;
block|}
block|}
if|if
condition|(
name|conf
operator|->
name|max_index_len
operator|<
name|prev
operator|->
name|max_index_len
condition|)
block|{
name|conf
operator|->
name|max_index_len
operator|=
name|prev
operator|->
name|max_index_len
expr_stmt|;
block|}
if|if
condition|(
name|conf
operator|->
name|index_cache
operator|==
name|NULL
condition|)
block|{
name|conf
operator|->
name|index_cache
operator|=
name|prev
operator|->
name|index_cache
expr_stmt|;
block|}
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

begin_comment
comment|/* TODO: warn about duplicate indices */
end_comment

begin_function
DECL|function|ngx_http_index_set_index (ngx_conf_t * cf,ngx_command_t * cmd,void * conf)
specifier|static
name|char
modifier|*
name|ngx_http_index_set_index
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
name|ngx_http_index_loc_conf_t
modifier|*
name|ilcf
init|=
name|conf
decl_stmt|;
name|ngx_uint_t
name|i
decl_stmt|;
name|ngx_str_t
modifier|*
name|index
decl_stmt|,
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
if|if
condition|(
name|value
index|[
literal|1
index|]
operator|.
name|data
index|[
literal|0
index|]
operator|==
literal|'/'
operator|&&
name|ilcf
operator|->
name|indices
operator|.
name|nelts
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
literal|"first index \"%s\" in \"%s\" directive "
literal|"must not be absolute"
argument_list|,
name|value
index|[
literal|1
index|]
operator|.
name|data
argument_list|,
name|cmd
operator|->
name|name
operator|.
name|data
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
for|for
control|(
name|i
operator|=
literal|1
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
name|value
index|[
name|i
index|]
operator|.
name|len
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
literal|"index \"%s\" in \"%s\" directive is invalid"
argument_list|,
name|value
index|[
literal|1
index|]
operator|.
name|data
argument_list|,
name|cmd
operator|->
name|name
operator|.
name|data
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|ngx_test_null
argument_list|(
name|index
argument_list|,
name|ngx_push_array
argument_list|(
operator|&
name|ilcf
operator|->
name|indices
argument_list|)
argument_list|,
name|NGX_CONF_ERROR
argument_list|)
expr_stmt|;
name|index
operator|->
name|len
operator|=
name|value
index|[
name|i
index|]
operator|.
name|len
expr_stmt|;
name|index
operator|->
name|data
operator|=
name|value
index|[
name|i
index|]
operator|.
name|data
expr_stmt|;
if|if
condition|(
name|ilcf
operator|->
name|max_index_len
operator|<
name|index
operator|->
name|len
operator|+
literal|1
condition|)
block|{
name|ilcf
operator|->
name|max_index_len
operator|=
name|index
operator|->
name|len
operator|+
literal|1
expr_stmt|;
block|}
block|}
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

end_unit

