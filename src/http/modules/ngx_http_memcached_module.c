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
file|<ngx_event.h>
end_include

begin_include
include|#
directive|include
file|<ngx_http.h>
end_include

begin_typedef
DECL|struct|__anon28b6160a0108
typedef|typedef
struct|struct
block|{
DECL|member|upstream
name|ngx_http_upstream_conf_t
name|upstream
decl_stmt|;
DECL|typedef|ngx_http_memcached_loc_conf_t
block|}
name|ngx_http_memcached_loc_conf_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon28b6160a0208
typedef|typedef
struct|struct
block|{
DECL|member|rest
name|size_t
name|rest
decl_stmt|;
DECL|member|request
name|ngx_http_request_t
modifier|*
name|request
decl_stmt|;
DECL|member|key
name|ngx_str_t
name|key
decl_stmt|;
DECL|typedef|ngx_http_memcached_ctx_t
block|}
name|ngx_http_memcached_ctx_t
typedef|;
end_typedef

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_memcached_create_request
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_memcached_reinit_request
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_memcached_process_header
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_memcached_filter_init
parameter_list|(
name|void
modifier|*
name|data
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_memcached_filter
parameter_list|(
name|void
modifier|*
name|data
parameter_list|,
name|ssize_t
name|bytes
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|void
name|ngx_http_memcached_abort_request
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
name|ngx_http_memcached_finalize_request
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_int_t
name|rc
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|void
modifier|*
name|ngx_http_memcached_create_loc_conf
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
name|ngx_http_memcached_merge_loc_conf
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
name|ngx_http_memcached_pass
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
name|ngx_http_memcached_upstream_max_fails_unsupported
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
name|ngx_http_memcached_upstream_fail_timeout_unsupported
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
DECL|variable|ngx_http_memcached_next_upstream_masks
specifier|static
name|ngx_conf_bitmask_t
name|ngx_http_memcached_next_upstream_masks
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"error"
argument_list|)
block|,
name|NGX_HTTP_UPSTREAM_FT_ERROR
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"timeout"
argument_list|)
block|,
name|NGX_HTTP_UPSTREAM_FT_TIMEOUT
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"invalid_response"
argument_list|)
block|,
name|NGX_HTTP_UPSTREAM_FT_INVALID_HEADER
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"not_found"
argument_list|)
block|,
name|NGX_HTTP_UPSTREAM_FT_HTTP_404
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"off"
argument_list|)
block|,
name|NGX_HTTP_UPSTREAM_FT_OFF
block|}
block|,
block|{
name|ngx_null_string
block|,
literal|0
block|}
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_memcached_commands
specifier|static
name|ngx_command_t
name|ngx_http_memcached_commands
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"memcached_pass"
argument_list|)
block|,
name|NGX_HTTP_LOC_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_http_memcached_pass
block|,
name|NGX_HTTP_LOC_CONF_OFFSET
block|,
literal|0
block|,
name|NULL
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"memcached_connect_timeout"
argument_list|)
block|,
name|NGX_HTTP_MAIN_CONF
operator||
name|NGX_HTTP_SRV_CONF
operator||
name|NGX_HTTP_LOC_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_conf_set_msec_slot
block|,
name|NGX_HTTP_LOC_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_http_memcached_loc_conf_t
argument_list|,
name|upstream
operator|.
name|connect_timeout
argument_list|)
block|,
name|NULL
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"memcached_send_timeout"
argument_list|)
block|,
name|NGX_HTTP_MAIN_CONF
operator||
name|NGX_HTTP_SRV_CONF
operator||
name|NGX_HTTP_LOC_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_conf_set_msec_slot
block|,
name|NGX_HTTP_LOC_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_http_memcached_loc_conf_t
argument_list|,
name|upstream
operator|.
name|send_timeout
argument_list|)
block|,
name|NULL
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"memcached_buffer_size"
argument_list|)
block|,
name|NGX_HTTP_MAIN_CONF
operator||
name|NGX_HTTP_SRV_CONF
operator||
name|NGX_HTTP_LOC_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_conf_set_size_slot
block|,
name|NGX_HTTP_LOC_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_http_memcached_loc_conf_t
argument_list|,
name|upstream
operator|.
name|buffer_size
argument_list|)
block|,
name|NULL
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"memcached_read_timeout"
argument_list|)
block|,
name|NGX_HTTP_MAIN_CONF
operator||
name|NGX_HTTP_SRV_CONF
operator||
name|NGX_HTTP_LOC_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_conf_set_msec_slot
block|,
name|NGX_HTTP_LOC_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_http_memcached_loc_conf_t
argument_list|,
name|upstream
operator|.
name|read_timeout
argument_list|)
block|,
name|NULL
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"memcached_next_upstream"
argument_list|)
block|,
name|NGX_HTTP_MAIN_CONF
operator||
name|NGX_HTTP_SRV_CONF
operator||
name|NGX_HTTP_LOC_CONF
operator||
name|NGX_CONF_1MORE
block|,
name|ngx_conf_set_bitmask_slot
block|,
name|NGX_HTTP_LOC_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_http_memcached_loc_conf_t
argument_list|,
name|upstream
operator|.
name|next_upstream
argument_list|)
block|,
operator|&
name|ngx_http_memcached_next_upstream_masks
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"memcached_upstream_max_fails"
argument_list|)
block|,
name|NGX_HTTP_MAIN_CONF
operator||
name|NGX_HTTP_SRV_CONF
operator||
name|NGX_HTTP_LOC_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_http_memcached_upstream_max_fails_unsupported
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
literal|"memcached_upstream_fail_timeout"
argument_list|)
block|,
name|NGX_HTTP_MAIN_CONF
operator||
name|NGX_HTTP_SRV_CONF
operator||
name|NGX_HTTP_LOC_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_http_memcached_upstream_fail_timeout_unsupported
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
DECL|variable|ngx_http_memcached_module_ctx
specifier|static
name|ngx_http_module_t
name|ngx_http_memcached_module_ctx
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
name|ngx_http_memcached_create_loc_conf
block|,
comment|/* create location configration */
name|ngx_http_memcached_merge_loc_conf
comment|/* merge location configration */
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_memcached_module
name|ngx_module_t
name|ngx_http_memcached_module
init|=
block|{
name|NGX_MODULE_V1
block|,
operator|&
name|ngx_http_memcached_module_ctx
block|,
comment|/* module context */
name|ngx_http_memcached_commands
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

begin_define
DECL|macro|NGX_HTTP_MEMCACHED_END
define|#
directive|define
name|NGX_HTTP_MEMCACHED_END
value|(sizeof(ngx_http_memcached_end) - 1)
end_define

begin_decl_stmt
DECL|variable|ngx_http_memcached_end
specifier|static
name|u_char
name|ngx_http_memcached_end
index|[]
init|=
name|CRLF
literal|"END"
name|CRLF
decl_stmt|;
end_decl_stmt

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_memcached_handler (ngx_http_request_t * r)
name|ngx_http_memcached_handler
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
block|{
name|ngx_int_t
name|rc
decl_stmt|;
name|ngx_http_upstream_t
modifier|*
name|u
decl_stmt|;
name|ngx_http_memcached_ctx_t
modifier|*
name|ctx
decl_stmt|;
name|ngx_http_memcached_loc_conf_t
modifier|*
name|mlcf
decl_stmt|;
if|if
condition|(
operator|!
operator|(
name|r
operator|->
name|method
operator|&
operator|(
name|NGX_HTTP_GET
operator||
name|NGX_HTTP_HEAD
operator|)
operator|)
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
operator|&&
name|rc
operator|!=
name|NGX_AGAIN
condition|)
block|{
return|return
name|rc
return|;
block|}
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
name|mlcf
operator|=
name|ngx_http_get_module_loc_conf
argument_list|(
name|r
argument_list|,
name|ngx_http_memcached_module
argument_list|)
expr_stmt|;
name|u
operator|=
name|ngx_pcalloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_upstream_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|u
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
name|u
operator|->
name|peer
operator|.
name|log
operator|=
name|r
operator|->
name|connection
operator|->
name|log
expr_stmt|;
name|u
operator|->
name|peer
operator|.
name|log_error
operator|=
name|NGX_ERROR_ERR
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_THREADS
operator|)
name|u
operator|->
name|peer
operator|.
name|lock
operator|=
operator|&
name|r
operator|->
name|connection
operator|->
name|lock
expr_stmt|;
endif|#
directive|endif
name|u
operator|->
name|output
operator|.
name|tag
operator|=
operator|(
name|ngx_buf_tag_t
operator|)
operator|&
name|ngx_http_memcached_module
expr_stmt|;
name|u
operator|->
name|conf
operator|=
operator|&
name|mlcf
operator|->
name|upstream
expr_stmt|;
name|u
operator|->
name|create_request
operator|=
name|ngx_http_memcached_create_request
expr_stmt|;
name|u
operator|->
name|reinit_request
operator|=
name|ngx_http_memcached_reinit_request
expr_stmt|;
name|u
operator|->
name|process_header
operator|=
name|ngx_http_memcached_process_header
expr_stmt|;
name|u
operator|->
name|abort_request
operator|=
name|ngx_http_memcached_abort_request
expr_stmt|;
name|u
operator|->
name|finalize_request
operator|=
name|ngx_http_memcached_finalize_request
expr_stmt|;
name|r
operator|->
name|upstream
operator|=
name|u
expr_stmt|;
name|ctx
operator|=
name|ngx_palloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_memcached_ctx_t
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
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
name|ctx
operator|->
name|rest
operator|=
name|NGX_HTTP_MEMCACHED_END
expr_stmt|;
name|ctx
operator|->
name|request
operator|=
name|r
expr_stmt|;
name|ngx_http_set_ctx
argument_list|(
name|r
argument_list|,
name|ctx
argument_list|,
name|ngx_http_memcached_module
argument_list|)
expr_stmt|;
name|u
operator|->
name|input_filter_init
operator|=
name|ngx_http_memcached_filter_init
expr_stmt|;
name|u
operator|->
name|input_filter
operator|=
name|ngx_http_memcached_filter
expr_stmt|;
name|u
operator|->
name|input_filter_ctx
operator|=
name|ctx
expr_stmt|;
name|ngx_http_upstream_init
argument_list|(
name|r
argument_list|)
expr_stmt|;
return|return
name|NGX_DONE
return|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_memcached_create_request (ngx_http_request_t * r)
name|ngx_http_memcached_create_request
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
block|{
name|size_t
name|len
decl_stmt|;
name|ngx_buf_t
modifier|*
name|b
decl_stmt|;
name|ngx_chain_t
modifier|*
name|cl
decl_stmt|;
name|ngx_http_memcached_ctx_t
modifier|*
name|ctx
decl_stmt|;
name|len
operator|=
sizeof|sizeof
argument_list|(
literal|"get "
argument_list|)
operator|-
literal|1
operator|+
name|r
operator|->
name|uri
operator|.
name|len
operator|+
sizeof|sizeof
argument_list|(
literal|" "
name|CRLF
argument_list|)
operator|-
literal|1
expr_stmt|;
if|if
condition|(
name|r
operator|->
name|args
operator|.
name|len
condition|)
block|{
name|len
operator|+=
literal|1
operator|+
name|r
operator|->
name|args
operator|.
name|len
expr_stmt|;
block|}
name|b
operator|=
name|ngx_create_temp_buf
argument_list|(
name|r
operator|->
name|pool
argument_list|,
name|len
argument_list|)
expr_stmt|;
if|if
condition|(
name|b
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|cl
operator|=
name|ngx_alloc_chain_link
argument_list|(
name|r
operator|->
name|pool
argument_list|)
expr_stmt|;
if|if
condition|(
name|cl
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|cl
operator|->
name|buf
operator|=
name|b
expr_stmt|;
name|cl
operator|->
name|next
operator|=
name|NULL
expr_stmt|;
name|r
operator|->
name|upstream
operator|->
name|request_bufs
operator|=
name|cl
expr_stmt|;
operator|*
name|b
operator|->
name|last
operator|++
operator|=
literal|'g'
expr_stmt|;
operator|*
name|b
operator|->
name|last
operator|++
operator|=
literal|'e'
expr_stmt|;
operator|*
name|b
operator|->
name|last
operator|++
operator|=
literal|'t'
expr_stmt|;
operator|*
name|b
operator|->
name|last
operator|++
operator|=
literal|' '
expr_stmt|;
name|ctx
operator|=
name|ngx_http_get_module_ctx
argument_list|(
name|r
argument_list|,
name|ngx_http_memcached_module
argument_list|)
expr_stmt|;
name|ctx
operator|->
name|key
operator|.
name|data
operator|=
name|b
operator|->
name|last
expr_stmt|;
name|b
operator|->
name|last
operator|=
name|ngx_copy
argument_list|(
name|b
operator|->
name|last
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
argument_list|)
expr_stmt|;
if|if
condition|(
name|r
operator|->
name|args
operator|.
name|len
condition|)
block|{
operator|*
name|b
operator|->
name|last
operator|++
operator|=
literal|'?'
expr_stmt|;
name|b
operator|->
name|last
operator|=
name|ngx_copy
argument_list|(
name|b
operator|->
name|last
argument_list|,
name|r
operator|->
name|args
operator|.
name|data
argument_list|,
name|r
operator|->
name|args
operator|.
name|len
argument_list|)
expr_stmt|;
block|}
name|ctx
operator|->
name|key
operator|.
name|len
operator|=
name|b
operator|->
name|last
operator|-
name|ctx
operator|->
name|key
operator|.
name|data
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
literal|"http memcached request: \"%V\""
argument_list|,
operator|&
name|ctx
operator|->
name|key
argument_list|)
expr_stmt|;
operator|*
name|b
operator|->
name|last
operator|++
operator|=
literal|' '
expr_stmt|;
operator|*
name|b
operator|->
name|last
operator|++
operator|=
name|CR
expr_stmt|;
operator|*
name|b
operator|->
name|last
operator|++
operator|=
name|LF
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_memcached_reinit_request (ngx_http_request_t * r)
name|ngx_http_memcached_reinit_request
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
block|{
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_memcached_process_header (ngx_http_request_t * r)
name|ngx_http_memcached_process_header
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
block|{
name|u_char
modifier|*
name|p
decl_stmt|,
modifier|*
name|len
decl_stmt|;
name|ngx_str_t
name|line
decl_stmt|;
name|ngx_http_upstream_t
modifier|*
name|u
decl_stmt|;
name|ngx_http_memcached_ctx_t
modifier|*
name|ctx
decl_stmt|;
name|u
operator|=
name|r
operator|->
name|upstream
expr_stmt|;
for|for
control|(
name|p
operator|=
name|u
operator|->
name|buffer
operator|.
name|pos
init|;
name|p
operator|<
name|u
operator|->
name|buffer
operator|.
name|last
condition|;
name|p
operator|++
control|)
block|{
if|if
condition|(
operator|*
name|p
operator|==
name|LF
condition|)
block|{
goto|goto
name|found
goto|;
block|}
block|}
return|return
name|NGX_AGAIN
return|;
name|found
label|:
operator|*
name|p
operator|=
literal|'\0'
expr_stmt|;
name|line
operator|.
name|len
operator|=
name|p
operator|-
name|u
operator|->
name|buffer
operator|.
name|pos
operator|-
literal|1
expr_stmt|;
name|line
operator|.
name|data
operator|=
name|u
operator|->
name|buffer
operator|.
name|pos
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
literal|"memcached: \"%V\""
argument_list|,
operator|&
name|line
argument_list|)
expr_stmt|;
name|p
operator|=
name|u
operator|->
name|buffer
operator|.
name|pos
expr_stmt|;
name|ctx
operator|=
name|ngx_http_get_module_ctx
argument_list|(
name|r
argument_list|,
name|ngx_http_memcached_module
argument_list|)
expr_stmt|;
if|if
condition|(
name|ngx_strncmp
argument_list|(
name|p
argument_list|,
literal|"VALUE "
argument_list|,
sizeof|sizeof
argument_list|(
literal|"VALUE "
argument_list|)
operator|-
literal|1
argument_list|)
operator|==
literal|0
condition|)
block|{
name|p
operator|+=
sizeof|sizeof
argument_list|(
literal|"VALUE "
argument_list|)
operator|-
literal|1
expr_stmt|;
if|if
condition|(
name|ngx_strncmp
argument_list|(
name|p
argument_list|,
name|ctx
operator|->
name|key
operator|.
name|data
argument_list|,
name|ctx
operator|->
name|key
operator|.
name|len
argument_list|)
operator|!=
literal|0
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
literal|0
argument_list|,
literal|"memcached sent invalid key in response \"%V\" "
literal|"for key \"%V\""
argument_list|,
operator|&
name|line
argument_list|,
operator|&
name|ctx
operator|->
name|key
argument_list|)
expr_stmt|;
return|return
name|NGX_HTTP_UPSTREAM_INVALID_HEADER
return|;
block|}
name|p
operator|+=
name|ctx
operator|->
name|key
operator|.
name|len
expr_stmt|;
if|if
condition|(
operator|*
name|p
operator|++
operator|!=
literal|' '
condition|)
block|{
goto|goto
name|no_valid
goto|;
block|}
comment|/* skip flags */
while|while
condition|(
operator|*
name|p
condition|)
block|{
if|if
condition|(
operator|*
name|p
operator|++
operator|==
literal|' '
condition|)
block|{
goto|goto
name|length
goto|;
block|}
block|}
goto|goto
name|no_valid
goto|;
name|length
label|:
name|len
operator|=
name|p
expr_stmt|;
while|while
condition|(
operator|*
name|p
operator|&&
operator|*
name|p
operator|++
operator|!=
name|CR
condition|)
block|{
comment|/* void */
block|}
name|r
operator|->
name|headers_out
operator|.
name|content_length_n
operator|=
name|ngx_atoof
argument_list|(
name|len
argument_list|,
name|p
operator|-
name|len
operator|-
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|r
operator|->
name|headers_out
operator|.
name|content_length_n
operator|==
operator|-
literal|1
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
literal|0
argument_list|,
literal|"memcached sent invalid length in response \"%V\" "
literal|"for key \"%V\""
argument_list|,
operator|&
name|line
argument_list|,
operator|&
name|ctx
operator|->
name|key
argument_list|)
expr_stmt|;
return|return
name|NGX_HTTP_UPSTREAM_INVALID_HEADER
return|;
block|}
name|u
operator|->
name|headers_in
operator|.
name|status_n
operator|=
literal|200
expr_stmt|;
name|u
operator|->
name|buffer
operator|.
name|pos
operator|=
name|p
operator|+
literal|1
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
if|if
condition|(
name|ngx_strcmp
argument_list|(
name|p
argument_list|,
literal|"END\x0d"
argument_list|)
operator|==
literal|0
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_INFO
argument_list|,
name|r
operator|->
name|connection
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"key: \"%V\" was not found by memcached"
argument_list|,
operator|&
name|ctx
operator|->
name|key
argument_list|)
expr_stmt|;
name|u
operator|->
name|headers_in
operator|.
name|status_n
operator|=
literal|404
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
name|no_valid
label|:
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
literal|0
argument_list|,
literal|"memcached sent invalid response: \"%V\""
argument_list|,
operator|&
name|line
argument_list|)
expr_stmt|;
return|return
name|NGX_HTTP_UPSTREAM_INVALID_HEADER
return|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_memcached_filter_init (void * data)
name|ngx_http_memcached_filter_init
parameter_list|(
name|void
modifier|*
name|data
parameter_list|)
block|{
name|ngx_http_memcached_ctx_t
modifier|*
name|ctx
init|=
name|data
decl_stmt|;
name|ngx_http_upstream_t
modifier|*
name|u
decl_stmt|;
name|u
operator|=
name|ctx
operator|->
name|request
operator|->
name|upstream
expr_stmt|;
name|u
operator|->
name|length
operator|+=
name|NGX_HTTP_MEMCACHED_END
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_memcached_filter (void * data,ssize_t bytes)
name|ngx_http_memcached_filter
parameter_list|(
name|void
modifier|*
name|data
parameter_list|,
name|ssize_t
name|bytes
parameter_list|)
block|{
name|ngx_http_memcached_ctx_t
modifier|*
name|ctx
init|=
name|data
decl_stmt|;
name|u_char
modifier|*
name|last
decl_stmt|;
name|ngx_buf_t
modifier|*
name|b
decl_stmt|;
name|ngx_chain_t
modifier|*
name|cl
decl_stmt|,
modifier|*
modifier|*
name|ll
decl_stmt|;
name|ngx_http_upstream_t
modifier|*
name|u
decl_stmt|;
name|u
operator|=
name|ctx
operator|->
name|request
operator|->
name|upstream
expr_stmt|;
name|b
operator|=
operator|&
name|u
operator|->
name|buffer
expr_stmt|;
if|if
condition|(
name|u
operator|->
name|length
operator|==
name|ctx
operator|->
name|rest
condition|)
block|{
if|if
condition|(
name|ngx_strncmp
argument_list|(
name|b
operator|->
name|last
argument_list|,
name|ngx_http_memcached_end
operator|+
name|NGX_HTTP_MEMCACHED_END
operator|-
name|ctx
operator|->
name|rest
argument_list|,
name|bytes
argument_list|)
operator|!=
literal|0
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ERR
argument_list|,
name|ctx
operator|->
name|request
operator|->
name|connection
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"memcached sent invalid trailer"
argument_list|)
expr_stmt|;
block|}
name|u
operator|->
name|length
operator|-=
name|bytes
expr_stmt|;
name|ctx
operator|->
name|rest
operator|-=
name|bytes
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
for|for
control|(
name|cl
operator|=
name|u
operator|->
name|out_bufs
operator|,
name|ll
operator|=
operator|&
name|u
operator|->
name|out_bufs
init|;
name|cl
condition|;
name|cl
operator|=
name|cl
operator|->
name|next
control|)
block|{
name|ll
operator|=
operator|&
name|cl
operator|->
name|next
expr_stmt|;
block|}
name|cl
operator|=
name|ngx_chain_get_free_buf
argument_list|(
name|ctx
operator|->
name|request
operator|->
name|pool
argument_list|,
operator|&
name|u
operator|->
name|free_bufs
argument_list|)
expr_stmt|;
if|if
condition|(
name|cl
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|cl
operator|->
name|buf
operator|->
name|flush
operator|=
literal|1
expr_stmt|;
name|cl
operator|->
name|buf
operator|->
name|memory
operator|=
literal|1
expr_stmt|;
operator|*
name|ll
operator|=
name|cl
expr_stmt|;
name|cl
operator|->
name|buf
operator|->
name|pos
operator|=
name|b
operator|->
name|last
expr_stmt|;
name|b
operator|->
name|last
operator|+=
name|bytes
expr_stmt|;
name|cl
operator|->
name|buf
operator|->
name|last
operator|=
name|b
operator|->
name|last
expr_stmt|;
name|ngx_log_debug4
argument_list|(
name|NGX_LOG_DEBUG_HTTP
argument_list|,
name|ctx
operator|->
name|request
operator|->
name|connection
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"memcached filter bytes:%z size:%z length:%z rest:%z"
argument_list|,
name|bytes
argument_list|,
name|b
operator|->
name|last
operator|-
name|b
operator|->
name|pos
argument_list|,
name|u
operator|->
name|length
argument_list|,
name|ctx
operator|->
name|rest
argument_list|)
expr_stmt|;
if|if
condition|(
name|b
operator|->
name|last
operator|-
name|b
operator|->
name|pos
operator|<=
operator|(
name|ssize_t
operator|)
operator|(
name|u
operator|->
name|length
operator|-
name|NGX_HTTP_MEMCACHED_END
operator|)
condition|)
block|{
name|u
operator|->
name|length
operator|-=
name|bytes
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
name|last
operator|=
name|b
operator|->
name|pos
operator|+
name|u
operator|->
name|length
operator|-
name|NGX_HTTP_MEMCACHED_END
expr_stmt|;
if|if
condition|(
name|ngx_strncmp
argument_list|(
name|last
argument_list|,
name|ngx_http_memcached_end
argument_list|,
name|b
operator|->
name|last
operator|-
name|last
argument_list|)
operator|!=
literal|0
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ERR
argument_list|,
name|ctx
operator|->
name|request
operator|->
name|connection
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"memcached sent invalid trailer"
argument_list|)
expr_stmt|;
block|}
name|ctx
operator|->
name|rest
operator|=
name|u
operator|->
name|length
operator|-
operator|(
name|b
operator|->
name|last
operator|-
name|b
operator|->
name|pos
operator|)
expr_stmt|;
name|b
operator|->
name|last
operator|=
name|last
expr_stmt|;
name|cl
operator|->
name|buf
operator|->
name|last
operator|=
name|last
expr_stmt|;
name|u
operator|->
name|length
operator|=
name|ctx
operator|->
name|rest
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
specifier|static
name|void
DECL|function|ngx_http_memcached_abort_request (ngx_http_request_t * r)
name|ngx_http_memcached_abort_request
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
block|{
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
literal|"abort http memcached request"
argument_list|)
expr_stmt|;
return|return;
block|}
end_function

begin_function
specifier|static
name|void
DECL|function|ngx_http_memcached_finalize_request (ngx_http_request_t * r,ngx_int_t rc)
name|ngx_http_memcached_finalize_request
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_int_t
name|rc
parameter_list|)
block|{
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
literal|"finalize http memcached request"
argument_list|)
expr_stmt|;
return|return;
block|}
end_function

begin_function
specifier|static
name|void
modifier|*
DECL|function|ngx_http_memcached_create_loc_conf (ngx_conf_t * cf)
name|ngx_http_memcached_create_loc_conf
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
block|{
name|ngx_http_memcached_loc_conf_t
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
name|ngx_http_memcached_loc_conf_t
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
comment|/*      * set by ngx_pcalloc():      *      *     conf->upstream.bufs.num = 0;      *     conf->upstream.next_upstream = 0;      *     conf->upstream.temp_path = NULL;      *     conf->upstream.schema = { 0, NULL };      *     conf->upstream.uri = { 0, NULL };      *     conf->upstream.location = NULL;      *      *     conf->peers = NULL;      */
name|conf
operator|->
name|upstream
operator|.
name|connect_timeout
operator|=
name|NGX_CONF_UNSET_MSEC
expr_stmt|;
name|conf
operator|->
name|upstream
operator|.
name|send_timeout
operator|=
name|NGX_CONF_UNSET_MSEC
expr_stmt|;
name|conf
operator|->
name|upstream
operator|.
name|read_timeout
operator|=
name|NGX_CONF_UNSET_MSEC
expr_stmt|;
name|conf
operator|->
name|upstream
operator|.
name|buffer_size
operator|=
name|NGX_CONF_UNSET_SIZE
expr_stmt|;
comment|/* the hardcoded values */
name|conf
operator|->
name|upstream
operator|.
name|cyclic_temp_file
operator|=
literal|0
expr_stmt|;
name|conf
operator|->
name|upstream
operator|.
name|buffering
operator|=
literal|0
expr_stmt|;
name|conf
operator|->
name|upstream
operator|.
name|ignore_client_abort
operator|=
literal|0
expr_stmt|;
name|conf
operator|->
name|upstream
operator|.
name|send_lowat
operator|=
literal|0
expr_stmt|;
name|conf
operator|->
name|upstream
operator|.
name|bufs
operator|.
name|num
operator|=
literal|0
expr_stmt|;
name|conf
operator|->
name|upstream
operator|.
name|busy_buffers_size
operator|=
literal|0
expr_stmt|;
name|conf
operator|->
name|upstream
operator|.
name|max_temp_file_size
operator|=
literal|0
expr_stmt|;
name|conf
operator|->
name|upstream
operator|.
name|temp_file_write_size
operator|=
literal|0
expr_stmt|;
name|conf
operator|->
name|upstream
operator|.
name|intercept_errors
operator|=
literal|1
expr_stmt|;
name|conf
operator|->
name|upstream
operator|.
name|intercept_404
operator|=
literal|1
expr_stmt|;
name|conf
operator|->
name|upstream
operator|.
name|pass_request_headers
operator|=
literal|0
expr_stmt|;
name|conf
operator|->
name|upstream
operator|.
name|pass_request_body
operator|=
literal|0
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
DECL|function|ngx_http_memcached_merge_loc_conf (ngx_conf_t * cf,void * parent,void * child)
name|ngx_http_memcached_merge_loc_conf
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
name|ngx_http_memcached_loc_conf_t
modifier|*
name|prev
init|=
name|parent
decl_stmt|;
name|ngx_http_memcached_loc_conf_t
modifier|*
name|conf
init|=
name|child
decl_stmt|;
name|ngx_conf_merge_msec_value
argument_list|(
name|conf
operator|->
name|upstream
operator|.
name|connect_timeout
argument_list|,
name|prev
operator|->
name|upstream
operator|.
name|connect_timeout
argument_list|,
literal|60000
argument_list|)
expr_stmt|;
name|ngx_conf_merge_msec_value
argument_list|(
name|conf
operator|->
name|upstream
operator|.
name|send_timeout
argument_list|,
name|prev
operator|->
name|upstream
operator|.
name|send_timeout
argument_list|,
literal|60000
argument_list|)
expr_stmt|;
name|ngx_conf_merge_msec_value
argument_list|(
name|conf
operator|->
name|upstream
operator|.
name|read_timeout
argument_list|,
name|prev
operator|->
name|upstream
operator|.
name|read_timeout
argument_list|,
literal|60000
argument_list|)
expr_stmt|;
name|ngx_conf_merge_size_value
argument_list|(
name|conf
operator|->
name|upstream
operator|.
name|buffer_size
argument_list|,
name|prev
operator|->
name|upstream
operator|.
name|buffer_size
argument_list|,
operator|(
name|size_t
operator|)
name|ngx_pagesize
argument_list|)
expr_stmt|;
name|ngx_conf_merge_bitmask_value
argument_list|(
name|conf
operator|->
name|upstream
operator|.
name|next_upstream
argument_list|,
name|prev
operator|->
name|upstream
operator|.
name|next_upstream
argument_list|,
operator|(
name|NGX_CONF_BITMASK_SET
operator||
name|NGX_HTTP_UPSTREAM_FT_ERROR
operator||
name|NGX_HTTP_UPSTREAM_FT_TIMEOUT
operator|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|conf
operator|->
name|upstream
operator|.
name|next_upstream
operator|&
name|NGX_HTTP_UPSTREAM_FT_OFF
condition|)
block|{
name|conf
operator|->
name|upstream
operator|.
name|next_upstream
operator|=
name|NGX_CONF_BITMASK_SET
operator||
name|NGX_HTTP_UPSTREAM_FT_OFF
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
DECL|function|ngx_http_memcached_pass (ngx_conf_t * cf,ngx_command_t * cmd,void * conf)
name|ngx_http_memcached_pass
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
name|ngx_http_memcached_loc_conf_t
modifier|*
name|lcf
init|=
name|conf
decl_stmt|;
name|ngx_str_t
modifier|*
name|value
decl_stmt|;
name|ngx_url_t
name|u
decl_stmt|;
name|ngx_http_core_loc_conf_t
modifier|*
name|clcf
decl_stmt|;
if|if
condition|(
name|lcf
operator|->
name|upstream
operator|.
name|schema
operator|.
name|len
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
name|ngx_memzero
argument_list|(
operator|&
name|u
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_url_t
argument_list|)
argument_list|)
expr_stmt|;
name|u
operator|.
name|url
operator|=
name|value
index|[
literal|1
index|]
expr_stmt|;
name|u
operator|.
name|upstream
operator|=
literal|1
expr_stmt|;
name|u
operator|.
name|no_resolve
operator|=
literal|1
expr_stmt|;
comment|/* u.uri_part = 1;  may be used as namespace */
name|lcf
operator|->
name|upstream
operator|.
name|upstream
operator|=
name|ngx_http_upstream_add
argument_list|(
name|cf
argument_list|,
operator|&
name|u
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|lcf
operator|->
name|upstream
operator|.
name|upstream
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|lcf
operator|->
name|upstream
operator|.
name|schema
operator|.
name|len
operator|=
sizeof|sizeof
argument_list|(
literal|"memcached://"
argument_list|)
operator|-
literal|1
expr_stmt|;
name|lcf
operator|->
name|upstream
operator|.
name|schema
operator|.
name|data
operator|=
operator|(
name|u_char
operator|*
operator|)
literal|"memcached://"
expr_stmt|;
name|clcf
operator|=
name|ngx_http_conf_get_module_loc_conf
argument_list|(
name|cf
argument_list|,
name|ngx_http_core_module
argument_list|)
expr_stmt|;
name|clcf
operator|->
name|handler
operator|=
name|ngx_http_memcached_handler
expr_stmt|;
name|lcf
operator|->
name|upstream
operator|.
name|location
operator|=
name|clcf
operator|->
name|name
expr_stmt|;
if|if
condition|(
name|clcf
operator|->
name|name
operator|.
name|data
index|[
name|clcf
operator|->
name|name
operator|.
name|len
operator|-
literal|1
index|]
operator|==
literal|'/'
condition|)
block|{
name|clcf
operator|->
name|auto_redirect
operator|=
literal|1
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
DECL|function|ngx_http_memcached_upstream_max_fails_unsupported (ngx_conf_t * cf,ngx_command_t * cmd,void * conf)
name|ngx_http_memcached_upstream_max_fails_unsupported
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
name|ngx_conf_log_error
argument_list|(
name|NGX_LOG_EMERG
argument_list|,
name|cf
argument_list|,
literal|0
argument_list|,
literal|"\"memcached_upstream_max_fails\" is not supported, "
literal|"use the \"max_fails\" parameter of the \"server\" directive "
argument_list|,
literal|"inside the \"upstream\" block"
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
end_function

begin_function
specifier|static
name|char
modifier|*
DECL|function|ngx_http_memcached_upstream_fail_timeout_unsupported (ngx_conf_t * cf,ngx_command_t * cmd,void * conf)
name|ngx_http_memcached_upstream_fail_timeout_unsupported
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
name|ngx_conf_log_error
argument_list|(
name|NGX_LOG_EMERG
argument_list|,
name|cf
argument_list|,
literal|0
argument_list|,
literal|"\"memcached_upstream_fail_timeout\" is not supported, "
literal|"use the \"fail_timeout\" parameter of the \"server\" directive "
argument_list|,
literal|"inside the \"upstream\" block"
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
end_function

end_unit

