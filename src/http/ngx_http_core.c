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
file|<ngx_config_command.h>
end_include

begin_include
include|#
directive|include
file|<ngx_http.h>
end_include

begin_include
include|#
directive|include
file|<ngx_http_core.h>
end_include

begin_include
include|#
directive|include
file|<ngx_http_config.h>
end_include

begin_comment
comment|/* STUB */
end_comment

begin_include
include|#
directive|include
file|<ngx_http_output_filter.h>
end_include

begin_function_decl
name|int
name|ngx_http_static_handler
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|ngx_http_index_handler
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|ngx_http_proxy_handler
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/**/
end_comment

begin_function_decl
specifier|static
name|void
modifier|*
name|ngx_http_core_create_srv_conf
parameter_list|(
name|ngx_pool_t
modifier|*
name|pool
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|void
modifier|*
name|ngx_http_core_create_loc_conf
parameter_list|(
name|ngx_pool_t
modifier|*
name|pool
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|int
name|ngx_http_core_translate_handler
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
function_decl|;
end_function_decl

begin_decl_stmt
DECL|variable|ngx_http_core_commands
specifier|static
name|ngx_command_t
name|ngx_http_core_commands
index|[]
init|=
block|{
block|{
literal|"send_timeout"
block|,
name|ngx_conf_set_time_slot
block|,
name|offsetof
argument_list|(
name|ngx_http_core_loc_conf_t
argument_list|,
name|send_timeout
argument_list|)
block|,
name|NGX_HTTP_LOC_CONF
block|,
name|NGX_CONF_TAKE1
block|,
literal|"set timeout for sending response"
block|}
block|,
block|{
name|NULL
block|}
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_core_module
name|ngx_http_module_t
name|ngx_http_core_module
init|=
block|{
name|NGX_HTTP_MODULE
block|,
name|ngx_http_core_create_srv_conf
block|,
comment|/* create server config */
name|ngx_http_core_create_loc_conf
block|,
comment|/* create location config */
name|ngx_http_core_commands
block|,
comment|/* module directives */
comment|/* STUB */
name|NULL
block|,
comment|/* init module */
name|ngx_http_core_translate_handler
block|,
comment|/* translate handler */
name|NULL
comment|/* init output body filter */
block|}
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|ngx_http_handler (ngx_http_request_t * r)
name|int
name|ngx_http_handler
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
block|{
name|int
name|rc
decl_stmt|,
name|i
decl_stmt|;
name|r
operator|->
name|connection
operator|->
name|unexpected_eof
operator|=
literal|0
expr_stmt|;
name|r
operator|->
name|lingering_close
operator|=
literal|1
expr_stmt|;
name|r
operator|->
name|keepalive
operator|=
literal|1
expr_stmt|;
if|#
directive|if
literal|1
name|r
operator|->
name|filter
operator|=
name|NGX_HTTP_FILTER_NEED_IN_MEMORY
expr_stmt|;
endif|#
directive|endif
comment|/* run translation phase */
for|for
control|(
name|i
operator|=
literal|0
init|;
name|ngx_http_modules
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
name|ngx_http_modules
index|[
name|i
index|]
operator|->
name|translate_handler
condition|)
block|{
name|rc
operator|=
name|ngx_http_modules
index|[
name|i
index|]
operator|->
name|translate_handler
argument_list|(
name|r
argument_list|)
expr_stmt|;
if|if
condition|(
name|rc
operator|==
name|NGX_OK
condition|)
break|break;
if|if
condition|(
name|rc
operator|>=
name|NGX_HTTP_SPECIAL_RESPONSE
condition|)
return|return
name|ngx_http_special_response
argument_list|(
name|r
argument_list|,
name|rc
argument_list|)
return|;
block|}
block|}
name|rc
operator|=
name|r
operator|->
name|handler
argument_list|(
name|r
argument_list|)
expr_stmt|;
if|if
condition|(
name|rc
operator|>=
name|NGX_HTTP_SPECIAL_RESPONSE
condition|)
return|return
name|ngx_http_special_response
argument_list|(
name|r
argument_list|,
name|rc
argument_list|)
return|;
return|return
name|rc
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_core_translate_handler (ngx_http_request_t * r)
specifier|static
name|int
name|ngx_http_core_translate_handler
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
block|{
name|char
modifier|*
name|loc
decl_stmt|,
modifier|*
name|last
decl_stmt|;
name|ngx_err_t
name|err
decl_stmt|;
name|ngx_table_elt_t
modifier|*
name|h
decl_stmt|;
comment|/* TODO: find location conf */
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
comment|/* TODO: find index handler */
comment|/* STUB */
name|r
operator|->
name|handler
operator|=
name|ngx_http_index_handler
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
name|r
operator|->
name|file
operator|.
name|name
operator|.
name|len
operator|=
name|r
operator|->
name|server
operator|->
name|doc_root_len
operator|+
name|r
operator|->
name|uri
operator|.
name|len
operator|+
literal|2
expr_stmt|;
name|ngx_test_null
argument_list|(
name|r
operator|->
name|file
operator|.
name|name
operator|.
name|data
argument_list|,
name|ngx_palloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
name|r
operator|->
name|file
operator|.
name|name
operator|.
name|len
operator|+
literal|1
argument_list|)
argument_list|,
name|NGX_HTTP_INTERNAL_SERVER_ERROR
argument_list|)
expr_stmt|;
name|loc
operator|=
name|ngx_cpystrn
argument_list|(
name|r
operator|->
name|file
operator|.
name|name
operator|.
name|data
argument_list|,
name|r
operator|->
name|server
operator|->
name|doc_root
argument_list|,
name|r
operator|->
name|server
operator|->
name|doc_root_len
argument_list|)
expr_stmt|;
name|last
operator|=
name|ngx_cpystrn
argument_list|(
name|loc
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
name|ngx_log_debug
argument_list|(
argument|r->connection->log
argument_list|,
literal|"HTTP filename: '%s'"
argument|_                   r->file.name.data
argument_list|)
empty_stmt|;
if|#
directive|if
operator|(
name|WIN9X
operator|)
comment|/* There is no way to open file or directory in Win9X with        one syscall: Win9X has not FILE_FLAG_BACKUP_SEMANTICS flag.        so we need to check its type before opening */
if|#
directive|if
literal|0
comment|/* OLD: ngx_file_type() is to be removed */
block_content|if (ngx_file_type(r->file.name.data,&r->file.info) == -1) {
endif|#
directive|endif
name|r
operator|->
name|file
operator|.
name|info
operator|.
name|dwFileAttributes
operator|=
name|GetFileAttributes
argument_list|(
name|r
operator|->
name|file
operator|.
name|name
operator|.
name|data
argument_list|)
expr_stmt|;
if|if
condition|(
name|r
operator|->
name|file
operator|.
name|info
operator|.
name|dwFileAttributes
operator|==
name|INVALID_FILE_ATTRIBUTES
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
name|r
operator|->
name|connection
operator|->
name|log
argument_list|,
name|err
argument_list|,
literal|"ngx_http_core_translate_handler: "
name|ngx_file_type_n
literal|" %s failed"
argument_list|,
name|r
operator|->
name|file
operator|.
name|name
operator|.
name|data
argument_list|)
expr_stmt|;
if|if
condition|(
name|err
operator|==
name|ERROR_FILE_NOT_FOUND
condition|)
return|return
name|NGX_HTTP_NOT_FOUND
return|;
if|else if
condition|(
name|err
operator|==
name|ERROR_PATH_NOT_FOUND
condition|)
return|return
name|NGX_HTTP_NOT_FOUND
return|;
else|else
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
else|#
directive|else
if|if
condition|(
name|r
operator|->
name|file
operator|.
name|fd
operator|==
name|NGX_INVALID_FILE
condition|)
name|r
operator|->
name|file
operator|.
name|fd
operator|=
name|ngx_open_file
argument_list|(
name|r
operator|->
name|file
operator|.
name|name
operator|.
name|data
argument_list|,
name|NGX_FILE_RDONLY
argument_list|)
expr_stmt|;
if|if
condition|(
name|r
operator|->
name|file
operator|.
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
name|NGX_LOG_ERR
argument_list|,
name|r
operator|->
name|connection
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"ngx_http_core_handler: "
name|ngx_open_file_n
literal|" %s failed"
argument_list|,
name|r
operator|->
name|file
operator|.
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
condition|)
return|return
name|NGX_HTTP_NOT_FOUND
return|;
else|else
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
if|if
condition|(
operator|!
name|r
operator|->
name|file
operator|.
name|info_valid
condition|)
block|{
if|if
condition|(
name|ngx_stat_fd
argument_list|(
name|r
operator|->
name|file
operator|.
name|fd
argument_list|,
operator|&
name|r
operator|->
name|file
operator|.
name|info
argument_list|)
operator|==
name|NGX_FILE_ERROR
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
name|ngx_errno
argument_list|,
literal|"ngx_http_core_handler: "
name|ngx_stat_fd_n
literal|" %s failed"
argument_list|,
name|r
operator|->
name|file
operator|.
name|name
operator|.
name|data
argument_list|)
expr_stmt|;
if|if
condition|(
name|ngx_close_file
argument_list|(
name|r
operator|->
name|file
operator|.
name|fd
argument_list|)
operator|==
name|NGX_FILE_ERROR
condition|)
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
name|ngx_errno
argument_list|,
literal|"ngx_http_core_handler: "
name|ngx_close_file_n
literal|" %s failed"
argument_list|,
name|r
operator|->
name|file
operator|.
name|name
operator|.
name|data
argument_list|)
expr_stmt|;
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
name|r
operator|->
name|file
operator|.
name|info_valid
operator|=
literal|1
expr_stmt|;
block|}
endif|#
directive|endif
if|if
condition|(
name|ngx_is_dir
argument_list|(
name|r
operator|->
name|file
operator|.
name|info
argument_list|)
condition|)
block|{
name|ngx_log_debug
argument_list|(
argument|r->connection->log
argument_list|,
literal|"HTTP DIR: '%s'"
argument|_ r->file.name.data
argument_list|)
empty_stmt|;
if|#
directive|if
operator|!
operator|(
name|WIN9X
operator|)
if|if
condition|(
name|ngx_close_file
argument_list|(
name|r
operator|->
name|file
operator|.
name|fd
argument_list|)
operator|==
name|NGX_FILE_ERROR
condition|)
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
name|ngx_errno
argument_list|,
literal|"ngx_http_core_handler: "
name|ngx_close_file_n
literal|" %s failed"
argument_list|,
name|r
operator|->
name|file
operator|.
name|name
operator|.
name|data
argument_list|)
expr_stmt|;
endif|#
directive|endif
comment|/* BROKEN: need to include server name */
name|ngx_test_null
argument_list|(
name|h
argument_list|,
name|ngx_push_table
argument_list|(
name|r
operator|->
name|headers_out
operator|.
name|headers
argument_list|)
argument_list|,
name|NGX_HTTP_INTERNAL_SERVER_ERROR
argument_list|)
expr_stmt|;
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
name|h
operator|->
name|key
operator|.
name|len
operator|=
literal|8
expr_stmt|;
name|h
operator|->
name|key
operator|.
name|data
operator|=
literal|"Location"
expr_stmt|;
name|h
operator|->
name|value
operator|.
name|len
operator|=
name|last
operator|-
name|loc
expr_stmt|;
name|h
operator|->
name|value
operator|.
name|data
operator|=
name|loc
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|location
operator|=
name|h
expr_stmt|;
return|return
name|NGX_HTTP_MOVED_PERMANENTLY
return|;
block|}
comment|/* TODO: r->handler = loc_conf->default_handler; */
comment|/* STUB */
name|r
operator|->
name|handler
operator|=
name|ngx_http_static_handler
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_send_header (ngx_http_request_t * r)
name|int
name|ngx_http_send_header
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
block|{
return|return
call|(
modifier|*
name|ngx_http_top_header_filter
call|)
argument_list|(
name|r
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_redirect (ngx_http_request_t * r,int redirect)
name|int
name|ngx_http_redirect
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|int
name|redirect
parameter_list|)
block|{
comment|/* STUB */
comment|/* log request */
return|return
name|ngx_http_close_request
argument_list|(
name|r
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_error (ngx_http_request_t * r,int error)
name|int
name|ngx_http_error
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|int
name|error
parameter_list|)
block|{
comment|/* STUB */
name|ngx_log_debug
argument_list|(
argument|r->connection->log
argument_list|,
literal|"http error: %d"
argument|_ error
argument_list|)
empty_stmt|;
comment|/* log request */
name|ngx_http_special_response
argument_list|(
name|r
argument_list|,
name|error
argument_list|)
expr_stmt|;
return|return
name|ngx_http_close_request
argument_list|(
name|r
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_close_request (ngx_http_request_t * r)
name|int
name|ngx_http_close_request
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
block|{
name|ngx_log_debug
argument_list|(
argument|r->connection->log
argument_list|,
literal|"CLOSE#: %d"
argument|_ r->file.fd
argument_list|)
empty_stmt|;
name|ngx_http_log_handler
argument_list|(
name|r
argument_list|)
expr_stmt|;
name|ngx_assert
argument_list|(
argument|(r->file.fd != NGX_INVALID_FILE)
argument_list|,
comment|/* void */
argument|;
argument_list|,
argument|r->connection->log
argument_list|,
literal|"file already closed"
argument_list|)
empty_stmt|;
if|if
condition|(
name|r
operator|->
name|file
operator|.
name|fd
operator|!=
name|NGX_INVALID_FILE
condition|)
block|{
comment|/* STUB WIN32 */
if|#
directive|if
operator|(
name|WIN32
operator|)
if|if
condition|(
name|ngx_close_file
argument_list|(
name|r
operator|->
name|file
operator|.
name|fd
argument_list|)
operator|==
literal|0
condition|)
else|#
directive|else
if|if
condition|(
name|ngx_close_file
argument_list|(
name|r
operator|->
name|file
operator|.
name|fd
argument_list|)
operator|==
operator|-
literal|1
condition|)
endif|#
directive|endif
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
name|ngx_errno
argument_list|,
name|ngx_close_file_n
literal|" failed"
argument_list|)
expr_stmt|;
block|}
comment|/*     if (r->logging)         ngx_http_log_request(r); */
name|ngx_destroy_pool
argument_list|(
name|r
operator|->
name|pool
argument_list|)
expr_stmt|;
name|ngx_log_debug
argument_list|(
name|r
operator|->
name|connection
operator|->
name|log
argument_list|,
literal|"http close"
argument_list|)
expr_stmt|;
name|ngx_del_timer
argument_list|(
name|r
operator|->
name|connection
operator|->
name|read
argument_list|)
expr_stmt|;
name|ngx_del_timer
argument_list|(
name|r
operator|->
name|connection
operator|->
name|write
argument_list|)
expr_stmt|;
return|return
name|NGX_DONE
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_internal_redirect (ngx_http_request_t * r,ngx_str_t uri)
name|int
name|ngx_http_internal_redirect
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_str_t
name|uri
parameter_list|)
block|{
name|ngx_log_debug
argument_list|(
argument|r->connection->log
argument_list|,
literal|"internal redirect: '%s'"
argument|_ uri.data
argument_list|)
empty_stmt|;
name|r
operator|->
name|uri
operator|.
name|len
operator|=
name|uri
operator|.
name|len
expr_stmt|;
name|r
operator|->
name|uri
operator|.
name|data
operator|=
name|uri
operator|.
name|data
expr_stmt|;
comment|/* NEEDED ? */
name|r
operator|->
name|uri_start
operator|=
name|uri
operator|.
name|data
expr_stmt|;
name|r
operator|->
name|uri_end
operator|=
name|uri
operator|.
name|data
operator|+
name|uri
operator|.
name|len
expr_stmt|;
comment|/**/
return|return
name|ngx_http_handler
argument_list|(
name|r
argument_list|)
return|;
block|}
end_function

begin_if
if|#
directive|if
literal|0
end_if

begin_endif
unit|{"http", ngx_http_enter_container, 0,      NGX_GLOBAL_CONF, NGX_CONF_CONTAINER},      {"server", ngx_http_enter_server_container, 0,      NGX_HTTP_CONF, NGX_CONF_CONTAINER],      {"location", ngx_http_enter_location_container, 0,      NGX_HTTP_SRV_CONF, NGX_CONF_CONTAINER|NGX_CONF_TAKE1}   int ngx_http_enter_container() {      create_srv_conf(null_srv_conf)      create_loc_conf(null_loc_conf) }  int ngx_http_exit_container() {      nothing ? }   int ngx_http_enter_server_container() {      create_srv_conf()      create_loc_conf(NULL) }  int ngx_http_exit_server_container() {      merge_srv_conf(srv_conf, null_srv_conf)      merge_loc_conf(loc_conf, null_loc_conf)       iterate check_loc_conf_is_set and merge_loc_conf() }  int ngx_http_enter_location_container() {      create_loc_conf(loc)       push to array }  int ngx_http_exit_location_container() { }
endif|#
directive|endif
end_endif

begin_function
DECL|function|ngx_http_core_create_srv_conf (ngx_pool_t * pool)
specifier|static
name|void
modifier|*
name|ngx_http_core_create_srv_conf
parameter_list|(
name|ngx_pool_t
modifier|*
name|pool
parameter_list|)
block|{
name|ngx_http_core_srv_conf_t
modifier|*
name|conf
decl_stmt|;
name|ngx_test_null
argument_list|(
name|conf
argument_list|,
name|ngx_pcalloc
argument_list|(
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_core_srv_conf_t
argument_list|)
argument_list|)
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
return|return
name|conf
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_core_create_loc_conf (ngx_pool_t * pool)
specifier|static
name|void
modifier|*
name|ngx_http_core_create_loc_conf
parameter_list|(
name|ngx_pool_t
modifier|*
name|pool
parameter_list|)
block|{
name|ngx_http_core_loc_conf_t
modifier|*
name|conf
decl_stmt|;
name|ngx_test_null
argument_list|(
name|conf
argument_list|,
name|ngx_pcalloc
argument_list|(
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_core_loc_conf_t
argument_list|)
argument_list|)
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|conf
operator|->
name|send_timeout
operator|=
name|NGX_CONF_UNSET
expr_stmt|;
return|return
name|conf
return|;
block|}
end_function

begin_if
if|#
directive|if
literal|0
end_if

begin_endif
unit|static void *ngx_http_core_create_conf(ngx_pool_t *pool) {      ngx_test_null(conf, ngx_palloc(pool, sizeof(ngx_http_core_conf_t)), NULL);      ngx_test_null(conf->srv, ngx_http_core_create_srv_conf_t(pool), NULL);     ngx_test_null(conf->loc, ngx_http_core_create_loc_conf_t(pool), NULL);     conf->parent =      conf->next = NULL; }
endif|#
directive|endif
end_endif

end_unit

