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
DECL|struct|__anon2c35069c0108
typedef|typedef
struct|struct
block|{
DECL|member|redirect_cache
name|ngx_http_cache_hash_t
modifier|*
name|redirect_cache
decl_stmt|;
DECL|typedef|ngx_http_static_loc_conf_t
block|}
name|ngx_http_static_loc_conf_t
typedef|;
end_typedef

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_static_handler
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|void
modifier|*
name|ngx_http_static_create_loc_conf
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
name|ngx_http_static_merge_loc_conf
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
name|ngx_http_static_init
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
function_decl|;
end_function_decl

begin_decl_stmt
DECL|variable|ngx_http_static_commands
specifier|static
name|ngx_command_t
name|ngx_http_static_commands
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"redirect_cache"
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
name|ngx_http_static_loc_conf_t
argument_list|,
name|redirect_cache
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
DECL|variable|ngx_http_static_module_ctx
name|ngx_http_module_t
name|ngx_http_static_module_ctx
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
name|ngx_http_static_create_loc_conf
block|,
comment|/* create location configuration */
name|ngx_http_static_merge_loc_conf
comment|/* merge location configuration */
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_static_module
name|ngx_module_t
name|ngx_http_static_module
init|=
block|{
name|NGX_MODULE
block|,
operator|&
name|ngx_http_static_module_ctx
block|,
comment|/* module context */
name|ngx_http_static_commands
block|,
comment|/* module directives */
name|NGX_HTTP_MODULE
block|,
comment|/* module type */
name|ngx_http_static_init
block|,
comment|/* init module */
name|NULL
comment|/* init child */
block|}
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|ngx_http_static_handler (ngx_http_request_t * r)
specifier|static
name|ngx_int_t
name|ngx_http_static_handler
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
block|{
name|char
modifier|*
name|last
decl_stmt|;
name|uint32_t
name|file_crc
decl_stmt|,
name|redirect_crc
decl_stmt|;
name|ngx_fd_t
name|fd
decl_stmt|;
name|ngx_int_t
name|rc
decl_stmt|;
name|ngx_uint_t
name|level
decl_stmt|;
name|ngx_str_t
name|name
decl_stmt|,
name|location
decl_stmt|;
name|ngx_err_t
name|err
decl_stmt|;
name|ngx_log_t
modifier|*
name|log
decl_stmt|;
name|ngx_hunk_t
modifier|*
name|h
decl_stmt|;
name|ngx_chain_t
name|out
decl_stmt|;
name|ngx_file_info_t
name|fi
decl_stmt|;
name|ngx_http_cache_t
modifier|*
name|file
decl_stmt|,
modifier|*
name|redirect
decl_stmt|;
name|ngx_http_cleanup_t
modifier|*
name|file_cleanup
decl_stmt|,
modifier|*
name|redirect_cleanup
decl_stmt|;
name|ngx_http_log_ctx_t
modifier|*
name|ctx
decl_stmt|;
name|ngx_http_core_loc_conf_t
modifier|*
name|clcf
decl_stmt|;
name|ngx_http_static_loc_conf_t
modifier|*
name|slcf
decl_stmt|;
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
operator|==
literal|'/'
condition|)
block|{
return|return
name|NGX_DECLINED
return|;
block|}
if|if
condition|(
name|r
operator|->
name|method
operator|!=
name|NGX_HTTP_GET
operator|&&
name|r
operator|->
name|method
operator|!=
name|NGX_HTTP_HEAD
condition|)
block|{
return|return
name|NGX_HTTP_NOT_ALLOWED
return|;
block|}
name|rc
operator|=
name|ngx_http_discard_body
argument_list|(
name|r
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
comment|/*      * there is a valid cached open file, i.e by the index handler,      * and it should be already registered in r->cleanup      */
if|if
condition|(
name|r
operator|->
name|cache
operator|&&
operator|!
name|r
operator|->
name|cache
operator|->
name|expired
condition|)
block|{
return|return
name|ngx_http_send_cached
argument_list|(
name|r
argument_list|)
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
name|clcf
operator|=
name|ngx_http_get_module_loc_conf
argument_list|(
name|r
argument_list|,
name|ngx_http_core_module
argument_list|)
expr_stmt|;
comment|/*      * make a file name, reserve 2 bytes for a trailing '/'      * in a possible redirect and for the last '\0'      */
name|name
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
name|doc_root
operator|.
name|len
operator|+
name|r
operator|->
name|uri
operator|.
name|len
operator|+
literal|2
argument_list|)
expr_stmt|;
if|if
condition|(
name|name
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
name|location
operator|.
name|data
operator|=
name|ngx_cpymem
argument_list|(
name|name
operator|.
name|data
argument_list|,
name|clcf
operator|->
name|doc_root
operator|.
name|data
argument_list|,
name|clcf
operator|->
name|doc_root
operator|.
name|len
argument_list|)
expr_stmt|;
name|last
operator|=
name|ngx_cpystrn
argument_list|(
name|location
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
name|name
operator|.
name|len
operator|=
name|last
operator|-
name|name
operator|.
name|data
expr_stmt|;
name|location
operator|.
name|len
operator|=
name|last
operator|-
name|location
operator|.
name|data
operator|+
literal|1
expr_stmt|;
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_HTTP
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"http filename: \"%s\""
argument_list|,
name|name
operator|.
name|data
argument_list|)
expr_stmt|;
comment|/* allocate cleanups */
if|if
condition|(
operator|!
operator|(
name|file_cleanup
operator|=
name|ngx_push_array
argument_list|(
operator|&
name|r
operator|->
name|cleanup
argument_list|)
operator|)
condition|)
block|{
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
name|file_cleanup
operator|->
name|valid
operator|=
literal|0
expr_stmt|;
name|slcf
operator|=
name|ngx_http_get_module_loc_conf
argument_list|(
name|r
argument_list|,
name|ngx_http_static_module
argument_list|)
expr_stmt|;
if|if
condition|(
name|slcf
operator|->
name|redirect_cache
condition|)
block|{
if|if
condition|(
operator|!
operator|(
name|redirect_cleanup
operator|=
name|ngx_push_array
argument_list|(
operator|&
name|r
operator|->
name|cleanup
argument_list|)
operator|)
condition|)
block|{
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
name|redirect_cleanup
operator|->
name|valid
operator|=
literal|0
expr_stmt|;
block|}
else|else
block|{
name|redirect_cleanup
operator|=
name|NULL
expr_stmt|;
block|}
comment|/* look up an open files cache */
if|if
condition|(
name|clcf
operator|->
name|open_files
condition|)
block|{
name|file
operator|=
name|ngx_http_cache_get
argument_list|(
name|clcf
operator|->
name|open_files
argument_list|,
name|file_cleanup
argument_list|,
operator|&
name|name
argument_list|,
operator|&
name|file_crc
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
literal|"http open file cache get: "
name|PTR_FMT
argument_list|,
name|file
argument_list|)
expr_stmt|;
if|if
condition|(
name|file
operator|&&
operator|!
name|file
operator|->
name|expired
condition|)
block|{
name|r
operator|->
name|cache
operator|=
name|file
expr_stmt|;
return|return
name|ngx_http_send_cached
argument_list|(
name|r
argument_list|)
return|;
block|}
block|}
else|else
block|{
name|file
operator|=
name|NULL
expr_stmt|;
block|}
comment|/* look up an redirect cache */
if|if
condition|(
name|slcf
operator|->
name|redirect_cache
condition|)
block|{
name|redirect
operator|=
name|ngx_http_cache_get
argument_list|(
name|slcf
operator|->
name|redirect_cache
argument_list|,
name|redirect_cleanup
argument_list|,
operator|&
name|name
argument_list|,
operator|&
name|redirect_crc
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
literal|"http redirect cache get: "
name|PTR_FMT
argument_list|,
name|redirect
argument_list|)
expr_stmt|;
if|if
condition|(
name|redirect
operator|&&
operator|!
name|redirect
operator|->
name|expired
condition|)
block|{
comment|/*              * We do not copy a cached value so the cache entry is locked              * until the end of the request.  In a single threaded model              * the redirected request should complete before other event              * will be processed.  In a multithreaded model this locking              * should keep more popular redirects in cache.              */
if|if
condition|(
operator|!
operator|(
name|r
operator|->
name|headers_out
operator|.
name|location
operator|=
name|ngx_http_add_header
argument_list|(
operator|&
name|r
operator|->
name|headers_out
argument_list|,
name|ngx_http_headers_out
argument_list|)
operator|)
condition|)
block|{
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
name|r
operator|->
name|headers_out
operator|.
name|location
operator|->
name|value
operator|=
name|redirect
operator|->
name|data
operator|.
name|value
expr_stmt|;
return|return
name|NGX_HTTP_MOVED_PERMANENTLY
return|;
block|}
block|}
else|else
block|{
name|redirect
operator|=
name|NULL
expr_stmt|;
block|}
comment|/* open file */
if|#
directive|if
operator|(
name|WIN9X
operator|)
comment|/* TODO: redirect cache */
if|if
condition|(
name|ngx_win32_version
operator|<
name|NGX_WIN_NT
condition|)
block|{
comment|/*          * there is no way to open a file or a directory in Win9X with          * one syscall because Win9X has no FILE_FLAG_BACKUP_SEMANTICS flag          * so we need to check its type before the opening          */
if|if
condition|(
name|ngx_file_info
argument_list|(
name|name
operator|.
name|data
argument_list|,
operator|&
name|fi
argument_list|)
operator|==
name|NGX_FILE_ERROR
condition|)
block|{
name|err
operator|=
name|ngx_errno
expr_stmt|;
name|ngx_log_error
argument_list|(
name|NGX_LOG_ERR
argument_list|,
name|log
argument_list|,
name|err
argument_list|,
name|ngx_file_info_n
literal|" \"%s\" failed"
argument_list|,
name|name
operator|.
name|data
argument_list|)
expr_stmt|;
if|if
condition|(
name|err
operator|==
name|NGX_ENOENT
operator|||
name|err
operator|==
name|NGX_ENOTDIR
condition|)
block|{
return|return
name|NGX_HTTP_NOT_FOUND
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
name|NGX_HTTP_FORBIDDEN
return|;
block|}
else|else
block|{
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
block|}
if|if
condition|(
name|ngx_is_dir
argument_list|(
operator|&
name|fi
argument_list|)
condition|)
block|{
name|ngx_log_debug
argument_list|(
argument|log
argument_list|,
literal|"HTTP DIR: '%s'"
argument|_ name.data
argument_list|)
empty_stmt|;
if|if
condition|(
operator|!
operator|(
name|r
operator|->
name|headers_out
operator|.
name|location
operator|=
name|ngx_http_add_header
argument_list|(
operator|&
name|r
operator|->
name|headers_out
argument_list|,
name|ngx_http_headers_out
argument_list|)
operator|)
condition|)
block|{
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
operator|*
name|last
operator|++
operator|=
literal|'/'
expr_stmt|;
operator|*
name|last
operator|=
literal|'\0'
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|location
operator|->
name|value
operator|.
name|len
operator|=
name|last
operator|-
name|location
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|location
operator|->
name|value
operator|.
name|data
operator|=
name|location
expr_stmt|;
return|return
name|NGX_HTTP_MOVED_PERMANENTLY
return|;
block|}
block|}
endif|#
directive|endif
name|fd
operator|=
name|ngx_open_file
argument_list|(
name|name
operator|.
name|data
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
name|NGX_INVALID_FILE
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
operator|||
name|err
operator|==
name|NGX_ENOTDIR
condition|)
block|{
name|level
operator|=
name|NGX_LOG_ERR
expr_stmt|;
name|rc
operator|=
name|NGX_HTTP_NOT_FOUND
expr_stmt|;
block|}
if|else if
condition|(
name|err
operator|==
name|NGX_EACCES
condition|)
block|{
name|level
operator|=
name|NGX_LOG_ERR
expr_stmt|;
name|rc
operator|=
name|NGX_HTTP_FORBIDDEN
expr_stmt|;
block|}
else|else
block|{
name|level
operator|=
name|NGX_LOG_CRIT
expr_stmt|;
name|rc
operator|=
name|NGX_HTTP_INTERNAL_SERVER_ERROR
expr_stmt|;
block|}
name|ngx_log_error
argument_list|(
name|level
argument_list|,
name|log
argument_list|,
name|err
argument_list|,
name|ngx_open_file_n
literal|" \"%s\" failed"
argument_list|,
name|name
operator|.
name|data
argument_list|)
expr_stmt|;
return|return
name|rc
return|;
block|}
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_HTTP
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"http static fd: %d"
argument_list|,
name|fd
argument_list|)
expr_stmt|;
if|if
condition|(
name|ngx_fd_info
argument_list|(
name|fd
argument_list|,
operator|&
name|fi
argument_list|)
operator|==
name|NGX_FILE_ERROR
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_CRIT
argument_list|,
name|log
argument_list|,
name|ngx_errno
argument_list|,
name|ngx_fd_info_n
literal|" \"%s\" failed"
argument_list|,
name|name
operator|.
name|data
argument_list|)
expr_stmt|;
if|if
condition|(
name|ngx_close_file
argument_list|(
name|fd
argument_list|)
operator|==
name|NGX_FILE_ERROR
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|log
argument_list|,
name|ngx_errno
argument_list|,
name|ngx_close_file_n
literal|" \"%s\" failed"
argument_list|,
name|name
operator|.
name|data
argument_list|)
expr_stmt|;
block|}
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
if|if
condition|(
name|ngx_is_dir
argument_list|(
operator|&
name|fi
argument_list|)
condition|)
block|{
name|ngx_log_debug0
argument_list|(
name|NGX_LOG_DEBUG_HTTP
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"http dir"
argument_list|)
expr_stmt|;
if|if
condition|(
name|ngx_close_file
argument_list|(
name|fd
argument_list|)
operator|==
name|NGX_FILE_ERROR
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|log
argument_list|,
name|ngx_errno
argument_list|,
name|ngx_close_file_n
literal|" \"%s\" failed"
argument_list|,
name|name
operator|.
name|data
argument_list|)
expr_stmt|;
block|}
operator|*
name|last
operator|++
operator|=
literal|'/'
expr_stmt|;
operator|*
name|last
operator|=
literal|'\0'
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|r
operator|->
name|headers_out
operator|.
name|location
operator|=
name|ngx_http_add_header
argument_list|(
operator|&
name|r
operator|->
name|headers_out
argument_list|,
name|ngx_http_headers_out
argument_list|)
operator|)
condition|)
block|{
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
name|r
operator|->
name|headers_out
operator|.
name|location
operator|->
name|value
operator|=
name|location
expr_stmt|;
if|if
condition|(
name|slcf
operator|->
name|redirect_cache
condition|)
block|{
if|if
condition|(
name|redirect
condition|)
block|{
if|if
condition|(
name|location
operator|.
name|len
operator|==
name|redirect
operator|->
name|data
operator|.
name|value
operator|.
name|len
operator|&&
name|ngx_memcmp
argument_list|(
name|redirect
operator|->
name|data
operator|.
name|value
operator|.
name|data
argument_list|,
name|location
operator|.
name|data
argument_list|,
name|location
operator|.
name|len
argument_list|)
operator|==
literal|0
condition|)
block|{
name|redirect
operator|->
name|accessed
operator|=
name|ngx_cached_time
expr_stmt|;
name|redirect
operator|->
name|updated
operator|=
name|ngx_cached_time
expr_stmt|;
comment|/*                      * we can unlock the cache entry because                      * we have the local copy anyway                      */
name|ngx_http_cache_unlock
argument_list|(
name|slcf
operator|->
name|redirect_cache
argument_list|,
name|redirect
argument_list|,
name|log
argument_list|)
expr_stmt|;
name|redirect_cleanup
operator|->
name|valid
operator|=
literal|0
expr_stmt|;
return|return
name|NGX_HTTP_MOVED_PERMANENTLY
return|;
block|}
block|}
name|location
operator|.
name|len
operator|++
expr_stmt|;
name|redirect
operator|=
name|ngx_http_cache_alloc
argument_list|(
name|slcf
operator|->
name|redirect_cache
argument_list|,
name|redirect
argument_list|,
name|redirect_cleanup
argument_list|,
operator|&
name|name
argument_list|,
name|redirect_crc
argument_list|,
operator|&
name|location
argument_list|,
name|log
argument_list|)
expr_stmt|;
name|location
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
literal|"http redirect cache alloc: "
name|PTR_FMT
argument_list|,
name|redirect
argument_list|)
expr_stmt|;
if|if
condition|(
name|redirect
condition|)
block|{
name|redirect
operator|->
name|fd
operator|=
name|NGX_INVALID_FILE
expr_stmt|;
name|redirect
operator|->
name|accessed
operator|=
name|ngx_cached_time
expr_stmt|;
name|redirect
operator|->
name|last_modified
operator|=
literal|0
expr_stmt|;
name|redirect
operator|->
name|updated
operator|=
name|ngx_cached_time
expr_stmt|;
name|redirect
operator|->
name|memory
operator|=
literal|1
expr_stmt|;
name|ngx_http_cache_unlock
argument_list|(
name|slcf
operator|->
name|redirect_cache
argument_list|,
name|redirect
argument_list|,
name|log
argument_list|)
expr_stmt|;
name|redirect_cleanup
operator|->
name|valid
operator|=
literal|0
expr_stmt|;
block|}
block|}
return|return
name|NGX_HTTP_MOVED_PERMANENTLY
return|;
block|}
if|#
directive|if
operator|!
operator|(
name|WIN32
operator|)
comment|/* the not regular files are probably Unix specific */
if|if
condition|(
operator|!
name|ngx_is_file
argument_list|(
operator|&
name|fi
argument_list|)
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_CRIT
argument_list|,
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"%s is not a regular file"
argument_list|,
name|name
operator|.
name|data
argument_list|)
expr_stmt|;
if|if
condition|(
name|ngx_close_file
argument_list|(
name|fd
argument_list|)
operator|==
name|NGX_FILE_ERROR
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|log
argument_list|,
name|ngx_errno
argument_list|,
name|ngx_close_file_n
literal|" \"%s\" failed"
argument_list|,
name|name
operator|.
name|data
argument_list|)
expr_stmt|;
block|}
return|return
name|NGX_HTTP_NOT_FOUND
return|;
block|}
endif|#
directive|endif
if|if
condition|(
name|clcf
operator|->
name|open_files
condition|)
block|{
if|#
directive|if
operator|(
name|NGX_USE_HTTP_FILE_CACHE_UNIQ
operator|)
if|if
condition|(
name|file
operator|&&
name|file
operator|->
name|uniq
operator|==
name|ngx_file_uniq
argument_list|(
operator|&
name|fi
argument_list|)
condition|)
block|{
if|if
condition|(
name|ngx_close_file
argument_list|(
name|fd
argument_list|)
operator|==
name|NGX_FILE_ERROR
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|log
argument_list|,
name|ngx_errno
argument_list|,
name|ngx_close_file_n
literal|" \"%s\" failed"
argument_list|,
name|name
operator|.
name|data
argument_list|)
expr_stmt|;
block|}
name|file
operator|->
name|accessed
operator|=
name|ngx_cached_time
expr_stmt|;
name|file
operator|->
name|updated
operator|=
name|ngx_cached_time
expr_stmt|;
name|file
operator|->
name|expired
operator|=
literal|0
expr_stmt|;
name|r
operator|->
name|cache
operator|=
name|file
expr_stmt|;
return|return
name|ngx_http_send_cached
argument_list|(
name|r
argument_list|)
return|;
block|}
else|else
block|{
if|if
condition|(
name|file
condition|)
block|{
name|ngx_http_cache_unlock
argument_list|(
name|clcf
operator|->
name|open_files
argument_list|,
name|file
argument_list|,
name|log
argument_list|)
expr_stmt|;
name|file
operator|=
name|NULL
expr_stmt|;
block|}
name|file
operator|=
name|ngx_http_cache_alloc
argument_list|(
name|clcf
operator|->
name|open_files
argument_list|,
name|file
argument_list|,
name|file_cleanup
argument_list|,
operator|&
name|name
argument_list|,
name|file_crc
argument_list|,
name|NULL
argument_list|,
name|log
argument_list|)
expr_stmt|;
if|if
condition|(
name|file
condition|)
block|{
name|file
operator|->
name|uniq
operator|=
name|ngx_file_uniq
argument_list|(
operator|&
name|fi
argument_list|)
expr_stmt|;
block|}
block|}
else|#
directive|else
name|file
operator|=
name|ngx_http_cache_alloc
argument_list|(
name|clcf
operator|->
name|open_files
argument_list|,
name|file
argument_list|,
name|file_cleanup
argument_list|,
operator|&
name|name
argument_list|,
name|file_crc
argument_list|,
name|NULL
argument_list|,
name|log
argument_list|)
expr_stmt|;
endif|#
directive|endif
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_HTTP
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"http open file cache alloc: "
name|PTR_FMT
argument_list|,
name|file
argument_list|)
expr_stmt|;
if|if
condition|(
name|file
condition|)
block|{
name|file
operator|->
name|fd
operator|=
name|fd
expr_stmt|;
name|file
operator|->
name|data
operator|.
name|size
operator|=
name|ngx_file_size
argument_list|(
operator|&
name|fi
argument_list|)
expr_stmt|;
name|file
operator|->
name|accessed
operator|=
name|ngx_cached_time
expr_stmt|;
name|file
operator|->
name|last_modified
operator|=
name|ngx_file_mtime
argument_list|(
operator|&
name|fi
argument_list|)
expr_stmt|;
name|file
operator|->
name|updated
operator|=
name|ngx_cached_time
expr_stmt|;
name|r
operator|->
name|cache
operator|=
name|file
expr_stmt|;
block|}
return|return
name|ngx_http_send_cached
argument_list|(
name|r
argument_list|)
return|;
block|}
name|ctx
operator|=
name|log
operator|->
name|data
expr_stmt|;
name|ctx
operator|->
name|action
operator|=
literal|"sending response to client"
expr_stmt|;
name|file_cleanup
operator|->
name|data
operator|.
name|file
operator|.
name|fd
operator|=
name|fd
expr_stmt|;
name|file_cleanup
operator|->
name|data
operator|.
name|file
operator|.
name|name
operator|=
name|name
operator|.
name|data
expr_stmt|;
name|file_cleanup
operator|->
name|valid
operator|=
literal|1
expr_stmt|;
name|file_cleanup
operator|->
name|cache
operator|=
literal|0
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|status
operator|=
name|NGX_HTTP_OK
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|content_length_n
operator|=
name|ngx_file_size
argument_list|(
operator|&
name|fi
argument_list|)
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|last_modified_time
operator|=
name|ngx_file_mtime
argument_list|(
operator|&
name|fi
argument_list|)
expr_stmt|;
if|if
condition|(
name|ngx_http_set_content_type
argument_list|(
name|r
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
comment|/* we need to allocate all before the header would be sent */
if|if
condition|(
operator|!
operator|(
name|h
operator|=
name|ngx_pcalloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_hunk_t
argument_list|)
argument_list|)
operator|)
condition|)
block|{
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
if|if
condition|(
operator|!
operator|(
name|h
operator|->
name|file
operator|=
name|ngx_pcalloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_file_t
argument_list|)
argument_list|)
operator|)
condition|)
block|{
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
name|rc
operator|=
name|ngx_http_send_header
argument_list|(
name|r
argument_list|)
expr_stmt|;
if|if
condition|(
name|rc
operator|==
name|NGX_ERROR
operator|||
name|rc
operator|>
name|NGX_OK
operator|||
name|r
operator|->
name|header_only
condition|)
block|{
return|return
name|rc
return|;
block|}
name|h
operator|->
name|type
operator|=
name|r
operator|->
expr|main
operator|?
name|NGX_HUNK_FILE
operator|:
name|NGX_HUNK_FILE
operator||
name|NGX_HUNK_LAST
expr_stmt|;
name|h
operator|->
name|file_pos
operator|=
literal|0
expr_stmt|;
name|h
operator|->
name|file_last
operator|=
name|ngx_file_size
argument_list|(
operator|&
name|fi
argument_list|)
expr_stmt|;
name|h
operator|->
name|file
operator|->
name|fd
operator|=
name|fd
expr_stmt|;
name|h
operator|->
name|file
operator|->
name|log
operator|=
name|log
expr_stmt|;
name|out
operator|.
name|hunk
operator|=
name|h
expr_stmt|;
name|out
operator|.
name|next
operator|=
name|NULL
expr_stmt|;
return|return
name|ngx_http_output_filter
argument_list|(
name|r
argument_list|,
operator|&
name|out
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_static_create_loc_conf (ngx_conf_t * cf)
specifier|static
name|void
modifier|*
name|ngx_http_static_create_loc_conf
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
block|{
name|ngx_http_static_loc_conf_t
modifier|*
name|conf
decl_stmt|;
if|if
condition|(
operator|!
operator|(
name|conf
operator|=
name|ngx_palloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_static_loc_conf_t
argument_list|)
argument_list|)
operator|)
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|conf
operator|->
name|redirect_cache
operator|=
name|NULL
expr_stmt|;
return|return
name|conf
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_static_merge_loc_conf (ngx_conf_t * cf,void * parent,void * child)
specifier|static
name|char
modifier|*
name|ngx_http_static_merge_loc_conf
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
name|ngx_http_static_loc_conf_t
modifier|*
name|prev
init|=
name|parent
decl_stmt|;
name|ngx_http_static_loc_conf_t
modifier|*
name|conf
init|=
name|child
decl_stmt|;
if|if
condition|(
name|conf
operator|->
name|redirect_cache
operator|==
name|NULL
condition|)
block|{
name|conf
operator|->
name|redirect_cache
operator|=
name|prev
operator|->
name|redirect_cache
expr_stmt|;
block|}
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_static_init (ngx_cycle_t * cycle)
specifier|static
name|ngx_int_t
name|ngx_http_static_init
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
name|ngx_http_static_handler
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

end_unit

