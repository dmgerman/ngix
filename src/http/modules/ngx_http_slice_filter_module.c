begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (C) Roman Arutyunyan  * Copyright (C) Nginx, Inc.  */
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
DECL|struct|__anon2afc4dac0108
typedef|typedef
struct|struct
block|{
DECL|member|size
name|size_t
name|size
decl_stmt|;
DECL|typedef|ngx_http_slice_loc_conf_t
block|}
name|ngx_http_slice_loc_conf_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon2afc4dac0208
typedef|typedef
struct|struct
block|{
DECL|member|start
name|off_t
name|start
decl_stmt|;
DECL|member|end
name|off_t
name|end
decl_stmt|;
DECL|member|range
name|ngx_str_t
name|range
decl_stmt|;
DECL|member|etag
name|ngx_str_t
name|etag
decl_stmt|;
DECL|member|last
name|unsigned
name|last
range|:
literal|1
decl_stmt|;
DECL|member|active
name|unsigned
name|active
range|:
literal|1
decl_stmt|;
DECL|member|sr
name|ngx_http_request_t
modifier|*
name|sr
decl_stmt|;
DECL|typedef|ngx_http_slice_ctx_t
block|}
name|ngx_http_slice_ctx_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon2afc4dac0308
typedef|typedef
struct|struct
block|{
DECL|member|start
name|off_t
name|start
decl_stmt|;
DECL|member|end
name|off_t
name|end
decl_stmt|;
DECL|member|complete_length
name|off_t
name|complete_length
decl_stmt|;
DECL|typedef|ngx_http_slice_content_range_t
block|}
name|ngx_http_slice_content_range_t
typedef|;
end_typedef

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_slice_header_filter
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
name|ngx_http_slice_body_filter
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_chain_t
modifier|*
name|in
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_slice_parse_content_range
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_slice_content_range_t
modifier|*
name|cr
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_slice_range_variable
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_variable_value_t
modifier|*
name|v
parameter_list|,
name|uintptr_t
name|data
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|off_t
name|ngx_http_slice_get_start
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
name|ngx_http_slice_create_loc_conf
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
name|ngx_http_slice_merge_loc_conf
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
name|ngx_http_slice_add_variables
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_slice_init
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
function_decl|;
end_function_decl

begin_decl_stmt
DECL|variable|ngx_http_slice_filter_commands
specifier|static
name|ngx_command_t
name|ngx_http_slice_filter_commands
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"slice"
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
name|ngx_http_slice_loc_conf_t
argument_list|,
name|size
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
DECL|variable|ngx_http_slice_filter_module_ctx
specifier|static
name|ngx_http_module_t
name|ngx_http_slice_filter_module_ctx
init|=
block|{
name|ngx_http_slice_add_variables
block|,
comment|/* preconfiguration */
name|ngx_http_slice_init
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
name|ngx_http_slice_create_loc_conf
block|,
comment|/* create location configuration */
name|ngx_http_slice_merge_loc_conf
comment|/* merge location configuration */
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_slice_filter_module
name|ngx_module_t
name|ngx_http_slice_filter_module
init|=
block|{
name|NGX_MODULE_V1
block|,
operator|&
name|ngx_http_slice_filter_module_ctx
block|,
comment|/* module context */
name|ngx_http_slice_filter_commands
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

begin_decl_stmt
DECL|variable|ngx_http_slice_range_name
specifier|static
name|ngx_str_t
name|ngx_http_slice_range_name
init|=
name|ngx_string
argument_list|(
literal|"slice_range"
argument_list|)
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_next_header_filter
specifier|static
name|ngx_http_output_header_filter_pt
name|ngx_http_next_header_filter
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_next_body_filter
specifier|static
name|ngx_http_output_body_filter_pt
name|ngx_http_next_body_filter
decl_stmt|;
end_decl_stmt

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_slice_header_filter (ngx_http_request_t * r)
name|ngx_http_slice_header_filter
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
block|{
name|off_t
name|end
decl_stmt|;
name|ngx_int_t
name|rc
decl_stmt|;
name|ngx_table_elt_t
modifier|*
name|h
decl_stmt|;
name|ngx_http_slice_ctx_t
modifier|*
name|ctx
decl_stmt|;
name|ngx_http_slice_loc_conf_t
modifier|*
name|slcf
decl_stmt|;
name|ngx_http_slice_content_range_t
name|cr
decl_stmt|;
name|ctx
operator|=
name|ngx_http_get_module_ctx
argument_list|(
name|r
argument_list|,
name|ngx_http_slice_filter_module
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
name|ngx_http_next_header_filter
argument_list|(
name|r
argument_list|)
return|;
block|}
if|if
condition|(
name|r
operator|->
name|headers_out
operator|.
name|status
operator|!=
name|NGX_HTTP_PARTIAL_CONTENT
condition|)
block|{
if|if
condition|(
name|r
operator|==
name|r
operator|->
expr|main
condition|)
block|{
name|ngx_http_set_ctx
argument_list|(
name|r
argument_list|,
name|NULL
argument_list|,
name|ngx_http_slice_filter_module
argument_list|)
expr_stmt|;
return|return
name|ngx_http_next_header_filter
argument_list|(
name|r
argument_list|)
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
literal|0
argument_list|,
literal|"unexpected status code %ui in slice response"
argument_list|,
name|r
operator|->
name|headers_out
operator|.
name|status
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
name|h
operator|=
name|r
operator|->
name|headers_out
operator|.
name|etag
expr_stmt|;
if|if
condition|(
name|ctx
operator|->
name|etag
operator|.
name|len
condition|)
block|{
if|if
condition|(
name|h
operator|==
name|NULL
operator|||
name|h
operator|->
name|value
operator|.
name|len
operator|!=
name|ctx
operator|->
name|etag
operator|.
name|len
operator|||
name|ngx_strncmp
argument_list|(
name|h
operator|->
name|value
operator|.
name|data
argument_list|,
name|ctx
operator|->
name|etag
operator|.
name|data
argument_list|,
name|ctx
operator|->
name|etag
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
literal|"etag mismatch in slice response"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
block|}
if|if
condition|(
name|h
condition|)
block|{
name|ctx
operator|->
name|etag
operator|=
name|h
operator|->
name|value
expr_stmt|;
block|}
if|if
condition|(
name|ngx_http_slice_parse_content_range
argument_list|(
name|r
argument_list|,
operator|&
name|cr
argument_list|)
operator|!=
name|NGX_OK
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
literal|"invalid range in slice response"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|cr
operator|.
name|complete_length
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
literal|"no complete length in slice response"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
name|ngx_log_debug3
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
literal|"http slice response range: %O-%O/%O"
argument_list|,
name|cr
operator|.
name|start
argument_list|,
name|cr
operator|.
name|end
argument_list|,
name|cr
operator|.
name|complete_length
argument_list|)
expr_stmt|;
name|slcf
operator|=
name|ngx_http_get_module_loc_conf
argument_list|(
name|r
argument_list|,
name|ngx_http_slice_filter_module
argument_list|)
expr_stmt|;
name|end
operator|=
name|ngx_min
argument_list|(
name|cr
operator|.
name|start
operator|+
operator|(
name|off_t
operator|)
name|slcf
operator|->
name|size
argument_list|,
name|cr
operator|.
name|complete_length
argument_list|)
expr_stmt|;
if|if
condition|(
name|cr
operator|.
name|start
operator|!=
name|ctx
operator|->
name|start
operator|||
name|cr
operator|.
name|end
operator|!=
name|end
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
literal|"unexpected range in slice response: %O-%O"
argument_list|,
name|cr
operator|.
name|start
argument_list|,
name|cr
operator|.
name|end
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
name|ctx
operator|->
name|start
operator|=
name|end
expr_stmt|;
name|ctx
operator|->
name|active
operator|=
literal|1
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
name|status_line
operator|.
name|len
operator|=
literal|0
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|content_length_n
operator|=
name|cr
operator|.
name|complete_length
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|content_offset
operator|=
name|cr
operator|.
name|start
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|content_range
operator|->
name|hash
operator|=
literal|0
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|content_range
operator|=
name|NULL
expr_stmt|;
if|if
condition|(
name|r
operator|->
name|headers_out
operator|.
name|accept_ranges
condition|)
block|{
name|r
operator|->
name|headers_out
operator|.
name|accept_ranges
operator|->
name|hash
operator|=
literal|0
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|accept_ranges
operator|=
name|NULL
expr_stmt|;
block|}
name|r
operator|->
name|allow_ranges
operator|=
literal|1
expr_stmt|;
name|r
operator|->
name|subrequest_ranges
operator|=
literal|1
expr_stmt|;
name|r
operator|->
name|single_range
operator|=
literal|1
expr_stmt|;
name|rc
operator|=
name|ngx_http_next_header_filter
argument_list|(
name|r
argument_list|)
expr_stmt|;
if|if
condition|(
name|r
operator|!=
name|r
operator|->
expr|main
condition|)
block|{
return|return
name|rc
return|;
block|}
name|r
operator|->
name|preserve_body
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|r
operator|->
name|headers_out
operator|.
name|status
operator|==
name|NGX_HTTP_PARTIAL_CONTENT
condition|)
block|{
if|if
condition|(
name|ctx
operator|->
name|start
operator|+
operator|(
name|off_t
operator|)
name|slcf
operator|->
name|size
operator|<=
name|r
operator|->
name|headers_out
operator|.
name|content_offset
condition|)
block|{
name|ctx
operator|->
name|start
operator|=
name|slcf
operator|->
name|size
operator|*
operator|(
name|r
operator|->
name|headers_out
operator|.
name|content_offset
operator|/
name|slcf
operator|->
name|size
operator|)
expr_stmt|;
block|}
name|ctx
operator|->
name|end
operator|=
name|r
operator|->
name|headers_out
operator|.
name|content_offset
operator|+
name|r
operator|->
name|headers_out
operator|.
name|content_length_n
expr_stmt|;
block|}
else|else
block|{
name|ctx
operator|->
name|end
operator|=
name|cr
operator|.
name|complete_length
expr_stmt|;
block|}
return|return
name|rc
return|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_slice_body_filter (ngx_http_request_t * r,ngx_chain_t * in)
name|ngx_http_slice_body_filter
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_chain_t
modifier|*
name|in
parameter_list|)
block|{
name|ngx_int_t
name|rc
decl_stmt|;
name|ngx_chain_t
modifier|*
name|cl
decl_stmt|;
name|ngx_http_slice_ctx_t
modifier|*
name|ctx
decl_stmt|;
name|ngx_http_slice_loc_conf_t
modifier|*
name|slcf
decl_stmt|;
name|ctx
operator|=
name|ngx_http_get_module_ctx
argument_list|(
name|r
argument_list|,
name|ngx_http_slice_filter_module
argument_list|)
expr_stmt|;
if|if
condition|(
name|ctx
operator|==
name|NULL
operator|||
name|r
operator|!=
name|r
operator|->
expr|main
condition|)
block|{
return|return
name|ngx_http_next_body_filter
argument_list|(
name|r
argument_list|,
name|in
argument_list|)
return|;
block|}
for|for
control|(
name|cl
operator|=
name|in
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
if|if
condition|(
name|cl
operator|->
name|buf
operator|->
name|last_buf
condition|)
block|{
name|cl
operator|->
name|buf
operator|->
name|last_buf
operator|=
literal|0
expr_stmt|;
name|cl
operator|->
name|buf
operator|->
name|last_in_chain
operator|=
literal|1
expr_stmt|;
name|cl
operator|->
name|buf
operator|->
name|sync
operator|=
literal|1
expr_stmt|;
name|ctx
operator|->
name|last
operator|=
literal|1
expr_stmt|;
block|}
block|}
name|rc
operator|=
name|ngx_http_next_body_filter
argument_list|(
name|r
argument_list|,
name|in
argument_list|)
expr_stmt|;
if|if
condition|(
name|rc
operator|==
name|NGX_ERROR
operator|||
operator|!
name|ctx
operator|->
name|last
condition|)
block|{
return|return
name|rc
return|;
block|}
if|if
condition|(
name|ctx
operator|->
name|sr
operator|&&
operator|!
name|ctx
operator|->
name|sr
operator|->
name|done
condition|)
block|{
return|return
name|rc
return|;
block|}
if|if
condition|(
operator|!
name|ctx
operator|->
name|active
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
literal|"missing slice response"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|ctx
operator|->
name|start
operator|>=
name|ctx
operator|->
name|end
condition|)
block|{
name|ngx_http_set_ctx
argument_list|(
name|r
argument_list|,
name|NULL
argument_list|,
name|ngx_http_slice_filter_module
argument_list|)
expr_stmt|;
name|ngx_http_send_special
argument_list|(
name|r
argument_list|,
name|NGX_HTTP_LAST
argument_list|)
expr_stmt|;
return|return
name|rc
return|;
block|}
if|if
condition|(
name|r
operator|->
name|buffered
condition|)
block|{
return|return
name|rc
return|;
block|}
if|if
condition|(
name|ngx_http_subrequest
argument_list|(
name|r
argument_list|,
operator|&
name|r
operator|->
name|uri
argument_list|,
operator|&
name|r
operator|->
name|args
argument_list|,
operator|&
name|ctx
operator|->
name|sr
argument_list|,
name|NULL
argument_list|,
name|NGX_HTTP_SUBREQUEST_CLONE
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|ngx_http_set_ctx
argument_list|(
name|ctx
operator|->
name|sr
argument_list|,
name|ctx
argument_list|,
name|ngx_http_slice_filter_module
argument_list|)
expr_stmt|;
name|slcf
operator|=
name|ngx_http_get_module_loc_conf
argument_list|(
name|r
argument_list|,
name|ngx_http_slice_filter_module
argument_list|)
expr_stmt|;
name|ctx
operator|->
name|range
operator|.
name|len
operator|=
name|ngx_sprintf
argument_list|(
name|ctx
operator|->
name|range
operator|.
name|data
argument_list|,
literal|"bytes=%O-%O"
argument_list|,
name|ctx
operator|->
name|start
argument_list|,
name|ctx
operator|->
name|start
operator|+
operator|(
name|off_t
operator|)
name|slcf
operator|->
name|size
operator|-
literal|1
argument_list|)
operator|-
name|ctx
operator|->
name|range
operator|.
name|data
expr_stmt|;
name|ctx
operator|->
name|active
operator|=
literal|0
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
literal|"http slice subrequest: \"%V\""
argument_list|,
operator|&
name|ctx
operator|->
name|range
argument_list|)
expr_stmt|;
return|return
name|rc
return|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_slice_parse_content_range (ngx_http_request_t * r,ngx_http_slice_content_range_t * cr)
name|ngx_http_slice_parse_content_range
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_slice_content_range_t
modifier|*
name|cr
parameter_list|)
block|{
name|off_t
name|start
decl_stmt|,
name|end
decl_stmt|,
name|complete_length
decl_stmt|,
name|cutoff
decl_stmt|,
name|cutlim
decl_stmt|;
name|u_char
modifier|*
name|p
decl_stmt|;
name|ngx_table_elt_t
modifier|*
name|h
decl_stmt|;
name|h
operator|=
name|r
operator|->
name|headers_out
operator|.
name|content_range
expr_stmt|;
if|if
condition|(
name|h
operator|==
name|NULL
operator|||
name|h
operator|->
name|value
operator|.
name|len
operator|<
literal|7
operator|||
name|ngx_strncmp
argument_list|(
name|h
operator|->
name|value
operator|.
name|data
argument_list|,
literal|"bytes "
argument_list|,
literal|6
argument_list|)
operator|!=
literal|0
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|p
operator|=
name|h
operator|->
name|value
operator|.
name|data
operator|+
literal|6
expr_stmt|;
name|cutoff
operator|=
name|NGX_MAX_OFF_T_VALUE
operator|/
literal|10
expr_stmt|;
name|cutlim
operator|=
name|NGX_MAX_OFF_T_VALUE
operator|%
literal|10
expr_stmt|;
name|start
operator|=
literal|0
expr_stmt|;
name|end
operator|=
literal|0
expr_stmt|;
name|complete_length
operator|=
literal|0
expr_stmt|;
while|while
condition|(
operator|*
name|p
operator|==
literal|' '
condition|)
block|{
name|p
operator|++
expr_stmt|;
block|}
if|if
condition|(
operator|*
name|p
argument_list|<
literal|'0'
operator|||
operator|*
name|p
argument_list|>
literal|'9'
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
while|while
condition|(
operator|*
name|p
operator|>=
literal|'0'
operator|&&
operator|*
name|p
operator|<=
literal|'9'
condition|)
block|{
if|if
condition|(
name|start
operator|>=
name|cutoff
operator|&&
operator|(
name|start
operator|>
name|cutoff
operator|||
operator|*
name|p
operator|-
literal|'0'
operator|>
name|cutlim
operator|)
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|start
operator|=
name|start
operator|*
literal|10
operator|+
operator|(
operator|*
name|p
operator|++
operator|-
literal|'0'
operator|)
expr_stmt|;
block|}
while|while
condition|(
operator|*
name|p
operator|==
literal|' '
condition|)
block|{
name|p
operator|++
expr_stmt|;
block|}
if|if
condition|(
operator|*
name|p
operator|++
operator|!=
literal|'-'
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
while|while
condition|(
operator|*
name|p
operator|==
literal|' '
condition|)
block|{
name|p
operator|++
expr_stmt|;
block|}
if|if
condition|(
operator|*
name|p
argument_list|<
literal|'0'
operator|||
operator|*
name|p
argument_list|>
literal|'9'
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
while|while
condition|(
operator|*
name|p
operator|>=
literal|'0'
operator|&&
operator|*
name|p
operator|<=
literal|'9'
condition|)
block|{
if|if
condition|(
name|end
operator|>=
name|cutoff
operator|&&
operator|(
name|end
operator|>
name|cutoff
operator|||
operator|*
name|p
operator|-
literal|'0'
operator|>
name|cutlim
operator|)
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|end
operator|=
name|end
operator|*
literal|10
operator|+
operator|(
operator|*
name|p
operator|++
operator|-
literal|'0'
operator|)
expr_stmt|;
block|}
name|end
operator|++
expr_stmt|;
while|while
condition|(
operator|*
name|p
operator|==
literal|' '
condition|)
block|{
name|p
operator|++
expr_stmt|;
block|}
if|if
condition|(
operator|*
name|p
operator|++
operator|!=
literal|'/'
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
while|while
condition|(
operator|*
name|p
operator|==
literal|' '
condition|)
block|{
name|p
operator|++
expr_stmt|;
block|}
if|if
condition|(
operator|*
name|p
operator|!=
literal|'*'
condition|)
block|{
if|if
condition|(
operator|*
name|p
argument_list|<
literal|'0'
operator|||
operator|*
name|p
argument_list|>
literal|'9'
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
while|while
condition|(
operator|*
name|p
operator|>=
literal|'0'
operator|&&
operator|*
name|p
operator|<=
literal|'9'
condition|)
block|{
if|if
condition|(
name|complete_length
operator|>=
name|cutoff
operator|&&
operator|(
name|complete_length
operator|>
name|cutoff
operator|||
operator|*
name|p
operator|-
literal|'0'
operator|>
name|cutlim
operator|)
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|complete_length
operator|=
name|complete_length
operator|*
literal|10
operator|+
operator|(
operator|*
name|p
operator|++
operator|-
literal|'0'
operator|)
expr_stmt|;
block|}
block|}
else|else
block|{
name|complete_length
operator|=
operator|-
literal|1
expr_stmt|;
name|p
operator|++
expr_stmt|;
block|}
while|while
condition|(
operator|*
name|p
operator|==
literal|' '
condition|)
block|{
name|p
operator|++
expr_stmt|;
block|}
if|if
condition|(
operator|*
name|p
operator|!=
literal|'\0'
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|cr
operator|->
name|start
operator|=
name|start
expr_stmt|;
name|cr
operator|->
name|end
operator|=
name|end
expr_stmt|;
name|cr
operator|->
name|complete_length
operator|=
name|complete_length
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_slice_range_variable (ngx_http_request_t * r,ngx_http_variable_value_t * v,uintptr_t data)
name|ngx_http_slice_range_variable
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_variable_value_t
modifier|*
name|v
parameter_list|,
name|uintptr_t
name|data
parameter_list|)
block|{
name|u_char
modifier|*
name|p
decl_stmt|;
name|ngx_http_slice_ctx_t
modifier|*
name|ctx
decl_stmt|;
name|ngx_http_slice_loc_conf_t
modifier|*
name|slcf
decl_stmt|;
name|ctx
operator|=
name|ngx_http_get_module_ctx
argument_list|(
name|r
argument_list|,
name|ngx_http_slice_filter_module
argument_list|)
expr_stmt|;
if|if
condition|(
name|ctx
operator|==
name|NULL
condition|)
block|{
if|if
condition|(
name|r
operator|!=
name|r
operator|->
expr|main
operator|||
name|r
operator|->
name|headers_out
operator|.
name|status
condition|)
block|{
name|v
operator|->
name|not_found
operator|=
literal|1
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
name|slcf
operator|=
name|ngx_http_get_module_loc_conf
argument_list|(
name|r
argument_list|,
name|ngx_http_slice_filter_module
argument_list|)
expr_stmt|;
if|if
condition|(
name|slcf
operator|->
name|size
operator|==
literal|0
condition|)
block|{
name|v
operator|->
name|not_found
operator|=
literal|1
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
name|ctx
operator|=
name|ngx_pcalloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_slice_ctx_t
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
name|NGX_ERROR
return|;
block|}
name|ngx_http_set_ctx
argument_list|(
name|r
argument_list|,
name|ctx
argument_list|,
name|ngx_http_slice_filter_module
argument_list|)
expr_stmt|;
name|p
operator|=
name|ngx_pnalloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
literal|"bytes=-"
argument_list|)
operator|-
literal|1
operator|+
literal|2
operator|*
name|NGX_OFF_T_LEN
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
name|ctx
operator|->
name|start
operator|=
name|slcf
operator|->
name|size
operator|*
operator|(
name|ngx_http_slice_get_start
argument_list|(
name|r
argument_list|)
operator|/
name|slcf
operator|->
name|size
operator|)
expr_stmt|;
name|ctx
operator|->
name|range
operator|.
name|data
operator|=
name|p
expr_stmt|;
name|ctx
operator|->
name|range
operator|.
name|len
operator|=
name|ngx_sprintf
argument_list|(
name|p
argument_list|,
literal|"bytes=%O-%O"
argument_list|,
name|ctx
operator|->
name|start
argument_list|,
name|ctx
operator|->
name|start
operator|+
operator|(
name|off_t
operator|)
name|slcf
operator|->
name|size
operator|-
literal|1
argument_list|)
operator|-
name|p
expr_stmt|;
block|}
name|v
operator|->
name|data
operator|=
name|ctx
operator|->
name|range
operator|.
name|data
expr_stmt|;
name|v
operator|->
name|valid
operator|=
literal|1
expr_stmt|;
name|v
operator|->
name|not_found
operator|=
literal|0
expr_stmt|;
name|v
operator|->
name|no_cacheable
operator|=
literal|1
expr_stmt|;
name|v
operator|->
name|len
operator|=
name|ctx
operator|->
name|range
operator|.
name|len
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
specifier|static
name|off_t
DECL|function|ngx_http_slice_get_start (ngx_http_request_t * r)
name|ngx_http_slice_get_start
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
block|{
name|off_t
name|start
decl_stmt|,
name|cutoff
decl_stmt|,
name|cutlim
decl_stmt|;
name|u_char
modifier|*
name|p
decl_stmt|;
name|ngx_table_elt_t
modifier|*
name|h
decl_stmt|;
if|if
condition|(
name|r
operator|->
name|headers_in
operator|.
name|if_range
condition|)
block|{
return|return
literal|0
return|;
block|}
name|h
operator|=
name|r
operator|->
name|headers_in
operator|.
name|range
expr_stmt|;
if|if
condition|(
name|h
operator|==
name|NULL
operator|||
name|h
operator|->
name|value
operator|.
name|len
operator|<
literal|7
operator|||
name|ngx_strncasecmp
argument_list|(
name|h
operator|->
name|value
operator|.
name|data
argument_list|,
operator|(
name|u_char
operator|*
operator|)
literal|"bytes="
argument_list|,
literal|6
argument_list|)
operator|!=
literal|0
condition|)
block|{
return|return
literal|0
return|;
block|}
name|p
operator|=
name|h
operator|->
name|value
operator|.
name|data
operator|+
literal|6
expr_stmt|;
if|if
condition|(
name|ngx_strchr
argument_list|(
name|p
argument_list|,
literal|','
argument_list|)
condition|)
block|{
return|return
literal|0
return|;
block|}
while|while
condition|(
operator|*
name|p
operator|==
literal|' '
condition|)
block|{
name|p
operator|++
expr_stmt|;
block|}
if|if
condition|(
operator|*
name|p
operator|==
literal|'-'
condition|)
block|{
return|return
literal|0
return|;
block|}
name|cutoff
operator|=
name|NGX_MAX_OFF_T_VALUE
operator|/
literal|10
expr_stmt|;
name|cutlim
operator|=
name|NGX_MAX_OFF_T_VALUE
operator|%
literal|10
expr_stmt|;
name|start
operator|=
literal|0
expr_stmt|;
while|while
condition|(
operator|*
name|p
operator|>=
literal|'0'
operator|&&
operator|*
name|p
operator|<=
literal|'9'
condition|)
block|{
if|if
condition|(
name|start
operator|>=
name|cutoff
operator|&&
operator|(
name|start
operator|>
name|cutoff
operator|||
operator|*
name|p
operator|-
literal|'0'
operator|>
name|cutlim
operator|)
condition|)
block|{
return|return
literal|0
return|;
block|}
name|start
operator|=
name|start
operator|*
literal|10
operator|+
operator|(
operator|*
name|p
operator|++
operator|-
literal|'0'
operator|)
expr_stmt|;
block|}
return|return
name|start
return|;
block|}
end_function

begin_function
specifier|static
name|void
modifier|*
DECL|function|ngx_http_slice_create_loc_conf (ngx_conf_t * cf)
name|ngx_http_slice_create_loc_conf
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
block|{
name|ngx_http_slice_loc_conf_t
modifier|*
name|slcf
decl_stmt|;
name|slcf
operator|=
name|ngx_palloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_slice_loc_conf_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|slcf
operator|==
name|NULL
condition|)
block|{
return|return
name|NULL
return|;
block|}
name|slcf
operator|->
name|size
operator|=
name|NGX_CONF_UNSET_SIZE
expr_stmt|;
return|return
name|slcf
return|;
block|}
end_function

begin_function
specifier|static
name|char
modifier|*
DECL|function|ngx_http_slice_merge_loc_conf (ngx_conf_t * cf,void * parent,void * child)
name|ngx_http_slice_merge_loc_conf
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
name|ngx_http_slice_loc_conf_t
modifier|*
name|prev
init|=
name|parent
decl_stmt|;
name|ngx_http_slice_loc_conf_t
modifier|*
name|conf
init|=
name|child
decl_stmt|;
name|ngx_conf_merge_size_value
argument_list|(
name|conf
operator|->
name|size
argument_list|,
name|prev
operator|->
name|size
argument_list|,
literal|0
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_slice_add_variables (ngx_conf_t * cf)
name|ngx_http_slice_add_variables
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
block|{
name|ngx_http_variable_t
modifier|*
name|var
decl_stmt|;
name|var
operator|=
name|ngx_http_add_variable
argument_list|(
name|cf
argument_list|,
operator|&
name|ngx_http_slice_range_name
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|var
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|var
operator|->
name|get_handler
operator|=
name|ngx_http_slice_range_variable
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_slice_init (ngx_conf_t * cf)
name|ngx_http_slice_init
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
block|{
name|ngx_http_next_header_filter
operator|=
name|ngx_http_top_header_filter
expr_stmt|;
name|ngx_http_top_header_filter
operator|=
name|ngx_http_slice_header_filter
expr_stmt|;
name|ngx_http_next_body_filter
operator|=
name|ngx_http_top_body_filter
expr_stmt|;
name|ngx_http_top_body_filter
operator|=
name|ngx_http_slice_body_filter
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

end_unit

