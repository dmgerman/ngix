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
DECL|struct|__anon2b8b0efa0108
typedef|typedef
struct|struct
block|{
DECL|member|tables
name|char
modifier|*
modifier|*
name|tables
decl_stmt|;
DECL|member|name
name|ngx_str_t
name|name
decl_stmt|;
DECL|member|server
name|unsigned
name|server
decl_stmt|;
DECL|typedef|ngx_http_charset_t
block|}
name|ngx_http_charset_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon2b8b0efa0208
typedef|typedef
struct|struct
block|{
DECL|member|src
name|ngx_int_t
name|src
decl_stmt|;
DECL|member|dst
name|ngx_int_t
name|dst
decl_stmt|;
DECL|member|src2dst
name|char
modifier|*
name|src2dst
decl_stmt|;
DECL|member|dst2src
name|char
modifier|*
name|dst2src
decl_stmt|;
DECL|typedef|ngx_http_charset_tables_t
block|}
name|ngx_http_charset_tables_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon2b8b0efa0308
typedef|typedef
struct|struct
block|{
DECL|member|charsets
name|ngx_array_t
name|charsets
decl_stmt|;
comment|/* ngx_http_charset_t */
DECL|member|tables
name|ngx_array_t
name|tables
decl_stmt|;
comment|/* ngx_http_charset_tables_t */
DECL|typedef|ngx_http_charset_main_conf_t
block|}
name|ngx_http_charset_main_conf_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon2b8b0efa0408
typedef|typedef
struct|struct
block|{
DECL|member|enable
name|ngx_flag_t
name|enable
decl_stmt|;
DECL|member|autodetect
name|ngx_flag_t
name|autodetect
decl_stmt|;
DECL|member|default_charset
name|ngx_int_t
name|default_charset
decl_stmt|;
DECL|member|source_charset
name|ngx_int_t
name|source_charset
decl_stmt|;
DECL|typedef|ngx_http_charset_loc_conf_t
block|}
name|ngx_http_charset_loc_conf_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon2b8b0efa0508
typedef|typedef
struct|struct
block|{
DECL|member|server
name|ngx_int_t
name|server
decl_stmt|;
DECL|member|client
name|ngx_int_t
name|client
decl_stmt|;
DECL|typedef|ngx_http_charset_ctx_t
block|}
name|ngx_http_charset_ctx_t
typedef|;
end_typedef

begin_function_decl
specifier|static
name|void
name|ngx_charset_recode
parameter_list|(
name|ngx_buf_t
modifier|*
name|b
parameter_list|,
name|char
modifier|*
name|table
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|char
modifier|*
name|ngx_charset_map_block
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
name|ngx_charset_map
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|ngx_command_t
modifier|*
name|dummy
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
name|ngx_http_set_charset_slot
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
name|ngx_http_add_charset
parameter_list|(
name|ngx_array_t
modifier|*
name|charsets
parameter_list|,
name|ngx_str_t
modifier|*
name|name
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_charset_filter_init
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
name|ngx_http_charset_create_main_conf
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
name|ngx_http_charset_init_main_conf
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
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
name|ngx_http_charset_create_loc_conf
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
name|ngx_http_charset_merge_loc_conf
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

begin_decl_stmt
DECL|variable|ngx_http_charset_filter_commands
specifier|static
name|ngx_command_t
name|ngx_http_charset_filter_commands
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"charset_map"
argument_list|)
block|,
name|NGX_HTTP_MAIN_CONF
operator||
name|NGX_CONF_BLOCK
operator||
name|NGX_CONF_TAKE2
block|,
name|ngx_charset_map_block
block|,
name|NGX_HTTP_MAIN_CONF_OFFSET
block|,
literal|0
block|,
name|NULL
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"default_charset"
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
name|ngx_http_set_charset_slot
block|,
name|NGX_HTTP_LOC_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_http_charset_loc_conf_t
argument_list|,
name|default_charset
argument_list|)
block|,
name|NULL
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"source_charset"
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
name|ngx_http_set_charset_slot
block|,
name|NGX_HTTP_LOC_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_http_charset_loc_conf_t
argument_list|,
name|source_charset
argument_list|)
block|,
name|NULL
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"charset"
argument_list|)
block|,
name|NGX_HTTP_MAIN_CONF
operator||
name|NGX_HTTP_SRV_CONF
operator||
name|NGX_HTTP_LOC_CONF
operator||
name|NGX_CONF_FLAG
block|,
name|ngx_conf_set_flag_slot
block|,
name|NGX_HTTP_LOC_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_http_charset_loc_conf_t
argument_list|,
name|enable
argument_list|)
block|,
name|NULL
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"autodetect_charset"
argument_list|)
block|,
name|NGX_HTTP_MAIN_CONF
operator||
name|NGX_HTTP_SRV_CONF
operator||
name|NGX_HTTP_LOC_CONF
operator||
name|NGX_CONF_FLAG
block|,
name|ngx_conf_set_flag_slot
block|,
name|NGX_HTTP_LOC_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_http_charset_loc_conf_t
argument_list|,
name|autodetect
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
DECL|variable|ngx_http_charset_filter_module_ctx
specifier|static
name|ngx_http_module_t
name|ngx_http_charset_filter_module_ctx
init|=
block|{
name|NULL
block|,
comment|/* pre conf */
name|ngx_http_charset_create_main_conf
block|,
comment|/* create main configuration */
name|ngx_http_charset_init_main_conf
block|,
comment|/* init main configuration */
name|NULL
block|,
comment|/* create server configuration */
name|NULL
block|,
comment|/* merge server configuration */
name|ngx_http_charset_create_loc_conf
block|,
comment|/* create location configuration */
name|ngx_http_charset_merge_loc_conf
comment|/* merge location configuration */
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_charset_filter_module
name|ngx_module_t
name|ngx_http_charset_filter_module
init|=
block|{
name|NGX_MODULE
block|,
operator|&
name|ngx_http_charset_filter_module_ctx
block|,
comment|/* module context */
name|ngx_http_charset_filter_commands
block|,
comment|/* module directives */
name|NGX_HTTP_MODULE
block|,
comment|/* module type */
name|ngx_http_charset_filter_init
block|,
comment|/* init module */
name|NULL
comment|/* init child */
block|}
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
DECL|function|ngx_http_charset_header_filter (ngx_http_request_t * r)
specifier|static
name|ngx_int_t
name|ngx_http_charset_header_filter
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
block|{
name|ngx_http_charset_t
modifier|*
name|charsets
decl_stmt|;
name|ngx_http_charset_ctx_t
modifier|*
name|ctx
decl_stmt|;
name|ngx_http_charset_loc_conf_t
modifier|*
name|lcf
decl_stmt|;
name|ngx_http_charset_main_conf_t
modifier|*
name|mcf
decl_stmt|;
name|mcf
operator|=
name|ngx_http_get_module_main_conf
argument_list|(
name|r
argument_list|,
name|ngx_http_charset_filter_module
argument_list|)
expr_stmt|;
name|lcf
operator|=
name|ngx_http_get_module_loc_conf
argument_list|(
name|r
argument_list|,
name|ngx_http_charset_filter_module
argument_list|)
expr_stmt|;
if|if
condition|(
name|lcf
operator|->
name|enable
operator|==
literal|0
condition|)
block|{
return|return
name|ngx_http_next_header_filter
argument_list|(
name|r
argument_list|)
return|;
block|}
if|#
directive|if
literal|0
block_content|if (lcf->default_charset.len == 0) {         return ngx_http_next_header_filter(r);     }
endif|#
directive|endif
if|if
condition|(
name|r
operator|->
name|headers_out
operator|.
name|content_type
operator|==
name|NULL
operator|||
name|ngx_strncasecmp
argument_list|(
name|r
operator|->
name|headers_out
operator|.
name|content_type
operator|->
name|value
operator|.
name|data
argument_list|,
literal|"text/"
argument_list|,
literal|5
argument_list|)
operator|!=
literal|0
operator|||
name|ngx_strstr
argument_list|(
name|r
operator|->
name|headers_out
operator|.
name|content_type
operator|->
name|value
operator|.
name|data
argument_list|,
literal|"charset"
argument_list|)
operator|!=
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
operator|==
name|NGX_HTTP_MOVED_PERMANENTLY
operator|&&
name|r
operator|->
name|headers_out
operator|.
name|status
operator|==
name|NGX_HTTP_MOVED_TEMPORARILY
condition|)
block|{
comment|/*          * do not set charset for the redirect because NN 4.x uses this          * charset instead of the next page charset          */
name|r
operator|->
name|headers_out
operator|.
name|charset
operator|.
name|len
operator|=
literal|0
expr_stmt|;
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
name|charset
operator|.
name|len
condition|)
block|{
return|return
name|ngx_http_next_header_filter
argument_list|(
name|r
argument_list|)
return|;
block|}
name|charsets
operator|=
name|mcf
operator|->
name|charsets
operator|.
name|elts
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|charset
operator|=
name|charsets
index|[
name|lcf
operator|->
name|default_charset
index|]
operator|.
name|name
expr_stmt|;
if|if
condition|(
name|lcf
operator|->
name|default_charset
operator|==
name|lcf
operator|->
name|source_charset
condition|)
block|{
return|return
name|ngx_http_next_header_filter
argument_list|(
name|r
argument_list|)
return|;
block|}
name|ngx_http_create_ctx
argument_list|(
name|r
argument_list|,
name|ctx
argument_list|,
name|ngx_http_charset_filter_module
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_charset_ctx_t
argument_list|)
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
name|r
operator|->
name|filter_need_in_memory
operator|=
literal|1
expr_stmt|;
return|return
name|ngx_http_next_header_filter
argument_list|(
name|r
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_charset_body_filter (ngx_http_request_t * r,ngx_chain_t * in)
specifier|static
name|ngx_int_t
name|ngx_http_charset_body_filter
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
name|char
modifier|*
name|table
decl_stmt|;
name|ngx_chain_t
modifier|*
name|cl
decl_stmt|;
name|ngx_http_charset_t
modifier|*
name|charsets
decl_stmt|;
name|ngx_http_charset_ctx_t
modifier|*
name|ctx
decl_stmt|;
name|ngx_http_charset_loc_conf_t
modifier|*
name|lcf
decl_stmt|;
name|ngx_http_charset_main_conf_t
modifier|*
name|mcf
decl_stmt|;
name|ctx
operator|=
name|ngx_http_get_module_ctx
argument_list|(
name|r
argument_list|,
name|ngx_http_charset_filter_module
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
name|ngx_http_next_body_filter
argument_list|(
name|r
argument_list|,
name|in
argument_list|)
return|;
block|}
name|mcf
operator|=
name|ngx_http_get_module_main_conf
argument_list|(
name|r
argument_list|,
name|ngx_http_charset_filter_module
argument_list|)
expr_stmt|;
name|lcf
operator|=
name|ngx_http_get_module_loc_conf
argument_list|(
name|r
argument_list|,
name|ngx_http_charset_filter_module
argument_list|)
expr_stmt|;
name|charsets
operator|=
name|mcf
operator|->
name|charsets
operator|.
name|elts
expr_stmt|;
name|table
operator|=
name|charsets
index|[
name|lcf
operator|->
name|source_charset
index|]
operator|.
name|tables
index|[
name|lcf
operator|->
name|default_charset
index|]
expr_stmt|;
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
name|ngx_charset_recode
argument_list|(
name|cl
operator|->
name|buf
argument_list|,
name|table
argument_list|)
expr_stmt|;
block|}
return|return
name|ngx_http_next_body_filter
argument_list|(
name|r
argument_list|,
name|in
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|ngx_charset_recode (ngx_buf_t * b,char * table)
specifier|static
name|void
name|ngx_charset_recode
parameter_list|(
name|ngx_buf_t
modifier|*
name|b
parameter_list|,
name|char
modifier|*
name|table
parameter_list|)
block|{
name|u_char
modifier|*
name|p
decl_stmt|,
name|c
decl_stmt|;
for|for
control|(
name|p
operator|=
name|b
operator|->
name|pos
init|;
name|p
operator|<
name|b
operator|->
name|last
condition|;
name|p
operator|++
control|)
block|{
name|c
operator|=
operator|*
name|p
expr_stmt|;
operator|*
name|p
operator|=
name|table
index|[
name|c
index|]
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|ngx_charset_map_block (ngx_conf_t * cf,ngx_command_t * cmd,void * conf)
specifier|static
name|char
modifier|*
name|ngx_charset_map_block
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
name|ngx_http_charset_main_conf_t
modifier|*
name|mcf
init|=
name|conf
decl_stmt|;
name|char
modifier|*
name|rv
decl_stmt|;
name|ngx_int_t
name|src
decl_stmt|,
name|dst
decl_stmt|;
name|ngx_uint_t
name|i
decl_stmt|;
name|ngx_str_t
modifier|*
name|value
decl_stmt|;
name|ngx_conf_t
name|pvcf
decl_stmt|;
name|ngx_http_charset_tables_t
modifier|*
name|table
decl_stmt|;
name|value
operator|=
name|cf
operator|->
name|args
operator|->
name|elts
expr_stmt|;
name|src
operator|=
name|ngx_http_add_charset
argument_list|(
operator|&
name|mcf
operator|->
name|charsets
argument_list|,
operator|&
name|value
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
if|if
condition|(
name|src
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|dst
operator|=
name|ngx_http_add_charset
argument_list|(
operator|&
name|mcf
operator|->
name|charsets
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
name|dst
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
if|if
condition|(
name|src
operator|==
name|dst
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
literal|"\"charset_map\" between the same charsets "
literal|"\"%s\" and \"%s\""
argument_list|,
name|value
index|[
literal|1
index|]
operator|.
name|data
argument_list|,
name|value
index|[
literal|2
index|]
operator|.
name|data
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|table
operator|=
name|mcf
operator|->
name|tables
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
name|mcf
operator|->
name|tables
operator|.
name|nelts
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
operator|(
name|src
operator|==
name|table
operator|->
name|src
operator|&&
name|dst
operator|==
name|table
operator|->
name|dst
operator|)
operator|||
operator|(
name|src
operator|==
name|table
operator|->
name|dst
operator|&&
name|dst
operator|==
name|table
operator|->
name|src
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
literal|"duplicate \"charset_map\" between "
literal|"\"%s\" and \"%s\""
argument_list|,
name|value
index|[
literal|1
index|]
operator|.
name|data
argument_list|,
name|value
index|[
literal|2
index|]
operator|.
name|data
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
block|}
if|if
condition|(
operator|!
operator|(
name|table
operator|=
name|ngx_push_array
argument_list|(
operator|&
name|mcf
operator|->
name|tables
argument_list|)
operator|)
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|table
operator|->
name|src
operator|=
name|src
expr_stmt|;
name|table
operator|->
name|dst
operator|=
name|dst
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|table
operator|->
name|src2dst
operator|=
name|ngx_palloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
literal|256
argument_list|)
operator|)
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
if|if
condition|(
operator|!
operator|(
name|table
operator|->
name|dst2src
operator|=
name|ngx_palloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
literal|256
argument_list|)
operator|)
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
literal|128
condition|;
name|i
operator|++
control|)
block|{
name|table
operator|->
name|src2dst
index|[
name|i
index|]
operator|=
operator|(
name|char
operator|)
name|i
expr_stmt|;
name|table
operator|->
name|dst2src
index|[
name|i
index|]
operator|=
operator|(
name|char
operator|)
name|i
expr_stmt|;
block|}
for|for
control|(
comment|/* void */
init|;
name|i
operator|<
literal|256
condition|;
name|i
operator|++
control|)
block|{
name|table
operator|->
name|src2dst
index|[
name|i
index|]
operator|=
literal|'?'
expr_stmt|;
name|table
operator|->
name|dst2src
index|[
name|i
index|]
operator|=
literal|'?'
expr_stmt|;
block|}
name|pvcf
operator|=
operator|*
name|cf
expr_stmt|;
name|cf
operator|->
name|ctx
operator|=
name|table
expr_stmt|;
name|cf
operator|->
name|handler
operator|=
name|ngx_charset_map
expr_stmt|;
name|cf
operator|->
name|handler_conf
operator|=
name|conf
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
name|pvcf
expr_stmt|;
return|return
name|rv
return|;
block|}
end_function

begin_function
DECL|function|ngx_charset_map (ngx_conf_t * cf,ngx_command_t * dummy,void * conf)
specifier|static
name|char
modifier|*
name|ngx_charset_map
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|ngx_command_t
modifier|*
name|dummy
parameter_list|,
name|void
modifier|*
name|conf
parameter_list|)
block|{
name|ngx_int_t
name|src
decl_stmt|,
name|dst
decl_stmt|;
name|ngx_str_t
modifier|*
name|value
decl_stmt|;
name|ngx_http_charset_tables_t
modifier|*
name|table
decl_stmt|;
if|if
condition|(
name|cf
operator|->
name|args
operator|->
name|nelts
operator|!=
literal|2
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
literal|"invalid parameters number"
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
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
name|src
operator|=
name|ngx_hextoi
argument_list|(
name|value
index|[
literal|0
index|]
operator|.
name|data
argument_list|,
name|value
index|[
literal|0
index|]
operator|.
name|len
argument_list|)
expr_stmt|;
if|if
condition|(
name|src
operator|==
name|NGX_ERROR
operator|||
name|src
operator|>
literal|255
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
literal|"invalid value \"%s\""
argument_list|,
name|value
index|[
literal|0
index|]
operator|.
name|data
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|dst
operator|=
name|ngx_hextoi
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
name|dst
operator|==
name|NGX_ERROR
operator|||
name|dst
operator|>
literal|255
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
literal|"invalid value \"%s\""
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
name|table
operator|=
name|cf
operator|->
name|ctx
expr_stmt|;
name|table
operator|->
name|src2dst
index|[
name|src
index|]
operator|=
operator|(
name|char
operator|)
name|dst
expr_stmt|;
name|table
operator|->
name|dst2src
index|[
name|dst
index|]
operator|=
operator|(
name|char
operator|)
name|src
expr_stmt|;
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_set_charset_slot (ngx_conf_t * cf,ngx_command_t * cmd,void * conf)
specifier|static
name|char
modifier|*
name|ngx_http_set_charset_slot
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
name|char
modifier|*
name|p
init|=
name|conf
decl_stmt|;
name|ngx_int_t
modifier|*
name|cp
decl_stmt|;
name|ngx_str_t
modifier|*
name|value
decl_stmt|;
name|ngx_http_charset_t
modifier|*
name|charset
decl_stmt|;
name|ngx_http_conf_ctx_t
modifier|*
name|ctx
decl_stmt|;
name|ngx_http_charset_main_conf_t
modifier|*
name|mcf
decl_stmt|;
name|cp
operator|=
operator|(
name|ngx_int_t
operator|*
operator|)
operator|(
name|p
operator|+
name|cmd
operator|->
name|offset
operator|)
expr_stmt|;
if|if
condition|(
operator|*
name|cp
operator|!=
name|NGX_CONF_UNSET
condition|)
block|{
return|return
literal|"is duplicate"
return|;
block|}
name|ctx
operator|=
name|cf
operator|->
name|ctx
expr_stmt|;
name|mcf
operator|=
name|ctx
operator|->
name|main_conf
index|[
name|ngx_http_charset_filter_module
operator|.
name|ctx_index
index|]
expr_stmt|;
name|value
operator|=
name|cf
operator|->
name|args
operator|->
name|elts
expr_stmt|;
operator|*
name|cp
operator|=
name|ngx_http_add_charset
argument_list|(
operator|&
name|mcf
operator|->
name|charsets
argument_list|,
operator|&
name|value
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
if|if
condition|(
operator|*
name|cp
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
if|if
condition|(
name|cmd
operator|->
name|offset
operator|==
name|offsetof
argument_list|(
name|ngx_http_charset_loc_conf_t
argument_list|,
name|source_charset
argument_list|)
condition|)
block|{
name|charset
operator|=
name|mcf
operator|->
name|charsets
operator|.
name|elts
expr_stmt|;
name|charset
index|[
operator|*
name|cp
index|]
operator|.
name|server
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
DECL|function|ngx_http_add_charset (ngx_array_t * charsets,ngx_str_t * name)
specifier|static
name|ngx_int_t
name|ngx_http_add_charset
parameter_list|(
name|ngx_array_t
modifier|*
name|charsets
parameter_list|,
name|ngx_str_t
modifier|*
name|name
parameter_list|)
block|{
name|ngx_uint_t
name|i
decl_stmt|;
name|ngx_http_charset_t
modifier|*
name|c
decl_stmt|;
name|c
operator|=
name|charsets
operator|->
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
name|charsets
operator|->
name|nelts
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|name
operator|->
name|len
operator|!=
name|c
index|[
name|i
index|]
operator|.
name|name
operator|.
name|len
condition|)
block|{
continue|continue;
block|}
if|if
condition|(
name|ngx_strcasecmp
argument_list|(
name|name
operator|->
name|data
argument_list|,
name|c
index|[
name|i
index|]
operator|.
name|name
operator|.
name|data
argument_list|)
operator|==
literal|0
condition|)
block|{
break|break;
block|}
block|}
if|if
condition|(
name|i
operator|<
name|charsets
operator|->
name|nelts
condition|)
block|{
return|return
name|i
return|;
block|}
if|if
condition|(
operator|!
operator|(
name|c
operator|=
name|ngx_push_array
argument_list|(
name|charsets
argument_list|)
operator|)
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|c
operator|->
name|name
operator|=
operator|*
name|name
expr_stmt|;
return|return
name|i
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_charset_filter_init (ngx_cycle_t * cycle)
specifier|static
name|ngx_int_t
name|ngx_http_charset_filter_init
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
block|{
name|ngx_http_next_header_filter
operator|=
name|ngx_http_top_header_filter
expr_stmt|;
name|ngx_http_top_header_filter
operator|=
name|ngx_http_charset_header_filter
expr_stmt|;
name|ngx_http_next_body_filter
operator|=
name|ngx_http_top_body_filter
expr_stmt|;
name|ngx_http_top_body_filter
operator|=
name|ngx_http_charset_body_filter
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_charset_create_main_conf (ngx_conf_t * cf)
specifier|static
name|void
modifier|*
name|ngx_http_charset_create_main_conf
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
block|{
name|ngx_http_charset_main_conf_t
modifier|*
name|mcf
decl_stmt|;
if|if
condition|(
operator|!
operator|(
name|mcf
operator|=
name|ngx_pcalloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_charset_main_conf_t
argument_list|)
argument_list|)
operator|)
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|ngx_init_array
argument_list|(
name|mcf
operator|->
name|charsets
argument_list|,
name|cf
operator|->
name|pool
argument_list|,
literal|5
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_charset_t
argument_list|)
argument_list|,
name|NGX_CONF_ERROR
argument_list|)
expr_stmt|;
name|ngx_init_array
argument_list|(
name|mcf
operator|->
name|tables
argument_list|,
name|cf
operator|->
name|pool
argument_list|,
literal|10
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_charset_tables_t
argument_list|)
argument_list|,
name|NGX_CONF_ERROR
argument_list|)
expr_stmt|;
return|return
name|mcf
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_charset_init_main_conf (ngx_conf_t * cf,void * conf)
specifier|static
name|char
modifier|*
name|ngx_http_charset_init_main_conf
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|void
modifier|*
name|conf
parameter_list|)
block|{
name|ngx_http_charset_main_conf_t
modifier|*
name|mcf
init|=
name|conf
decl_stmt|;
name|ngx_uint_t
name|i
decl_stmt|,
name|n
decl_stmt|;
name|ngx_http_charset_t
modifier|*
name|charset
decl_stmt|;
name|ngx_http_charset_tables_t
modifier|*
name|tables
decl_stmt|;
name|tables
operator|=
name|mcf
operator|->
name|tables
operator|.
name|elts
expr_stmt|;
name|charset
operator|=
name|mcf
operator|->
name|charsets
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
name|mcf
operator|->
name|charsets
operator|.
name|nelts
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
operator|!
name|charset
index|[
name|i
index|]
operator|.
name|server
condition|)
block|{
continue|continue;
block|}
name|charset
index|[
name|i
index|]
operator|.
name|tables
operator|=
name|ngx_pcalloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|char
operator|*
argument_list|)
operator|*
name|mcf
operator|->
name|charsets
operator|.
name|nelts
argument_list|)
expr_stmt|;
if|if
condition|(
name|charset
index|[
name|i
index|]
operator|.
name|tables
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
for|for
control|(
name|n
operator|=
literal|0
init|;
name|n
operator|<
name|mcf
operator|->
name|tables
operator|.
name|nelts
condition|;
name|n
operator|++
control|)
block|{
if|if
condition|(
operator|(
name|ngx_int_t
operator|)
name|i
operator|==
name|tables
index|[
name|n
index|]
operator|.
name|src
condition|)
block|{
name|charset
index|[
name|i
index|]
operator|.
name|tables
index|[
name|tables
index|[
name|n
index|]
operator|.
name|dst
index|]
operator|=
name|tables
index|[
name|n
index|]
operator|.
name|src2dst
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
operator|(
name|ngx_int_t
operator|)
name|i
operator|==
name|tables
index|[
name|n
index|]
operator|.
name|dst
condition|)
block|{
name|charset
index|[
name|i
index|]
operator|.
name|tables
index|[
name|tables
index|[
name|n
index|]
operator|.
name|src
index|]
operator|=
name|tables
index|[
name|n
index|]
operator|.
name|dst2src
expr_stmt|;
block|}
block|}
block|}
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|mcf
operator|->
name|charsets
operator|.
name|nelts
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
operator|!
name|charset
index|[
name|i
index|]
operator|.
name|server
condition|)
block|{
continue|continue;
block|}
for|for
control|(
name|n
operator|=
literal|0
init|;
name|n
operator|<
name|mcf
operator|->
name|charsets
operator|.
name|nelts
condition|;
name|n
operator|++
control|)
block|{
if|if
condition|(
name|i
operator|==
name|n
condition|)
block|{
continue|continue;
block|}
if|if
condition|(
name|charset
index|[
name|i
index|]
operator|.
name|tables
index|[
name|n
index|]
condition|)
block|{
continue|continue;
block|}
name|ngx_log_error
argument_list|(
name|NGX_LOG_EMERG
argument_list|,
name|cf
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|" no \"charset_map\" between the charsets "
literal|"\"%s\" and \"%s\""
argument_list|,
name|charset
index|[
name|i
index|]
operator|.
name|name
operator|.
name|data
argument_list|,
name|charset
index|[
name|n
index|]
operator|.
name|name
operator|.
name|data
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
block|}
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_charset_create_loc_conf (ngx_conf_t * cf)
specifier|static
name|void
modifier|*
name|ngx_http_charset_create_loc_conf
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
block|{
name|ngx_http_charset_loc_conf_t
modifier|*
name|lcf
decl_stmt|;
if|if
condition|(
operator|!
operator|(
name|lcf
operator|=
name|ngx_pcalloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_charset_loc_conf_t
argument_list|)
argument_list|)
operator|)
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|lcf
operator|->
name|enable
operator|=
name|NGX_CONF_UNSET
expr_stmt|;
name|lcf
operator|->
name|autodetect
operator|=
name|NGX_CONF_UNSET
expr_stmt|;
name|lcf
operator|->
name|default_charset
operator|=
name|NGX_CONF_UNSET
expr_stmt|;
name|lcf
operator|->
name|source_charset
operator|=
name|NGX_CONF_UNSET
expr_stmt|;
return|return
name|lcf
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_charset_merge_loc_conf (ngx_conf_t * cf,void * parent,void * child)
specifier|static
name|char
modifier|*
name|ngx_http_charset_merge_loc_conf
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
name|ngx_http_charset_loc_conf_t
modifier|*
name|prev
init|=
name|parent
decl_stmt|;
name|ngx_http_charset_loc_conf_t
modifier|*
name|conf
init|=
name|child
decl_stmt|;
name|ngx_conf_merge_value
argument_list|(
name|conf
operator|->
name|enable
argument_list|,
name|prev
operator|->
name|enable
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|ngx_conf_merge_value
argument_list|(
name|conf
operator|->
name|autodetect
argument_list|,
name|prev
operator|->
name|autodetect
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|conf
operator|->
name|source_charset
operator|==
name|NGX_CONF_UNSET
condition|)
block|{
name|conf
operator|->
name|source_charset
operator|=
name|prev
operator|->
name|source_charset
expr_stmt|;
block|}
name|ngx_conf_merge_value
argument_list|(
name|conf
operator|->
name|default_charset
argument_list|,
name|prev
operator|->
name|default_charset
argument_list|,
name|conf
operator|->
name|source_charset
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

end_unit

