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
DECL|struct|__anon2c2669840108
typedef|typedef
struct|struct
block|{
DECL|member|expires
name|time_t
name|expires
decl_stmt|;
DECL|typedef|ngx_http_headers_conf_t
block|}
name|ngx_http_headers_conf_t
typedef|;
end_typedef

begin_define
DECL|macro|NGX_HTTP_EXPIRES_UNSET
define|#
directive|define
name|NGX_HTTP_EXPIRES_UNSET
value|-2147483647
end_define

begin_define
DECL|macro|NGX_HTTP_EXPIRES_OFF
define|#
directive|define
name|NGX_HTTP_EXPIRES_OFF
value|-2147483646
end_define

begin_define
DECL|macro|NGX_HTTP_EXPIRES_EPOCH
define|#
directive|define
name|NGX_HTTP_EXPIRES_EPOCH
value|-2147483645
end_define

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_headers_filter_init
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
name|ngx_http_headers_create_conf
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
name|ngx_http_headers_merge_conf
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
name|char
modifier|*
name|ngx_http_headers_expires
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
DECL|variable|ngx_http_headers_filter_commands
specifier|static
name|ngx_command_t
name|ngx_http_headers_filter_commands
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"expires"
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
name|ngx_http_headers_expires
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
DECL|variable|ngx_http_headers_filter_module_ctx
specifier|static
name|ngx_http_module_t
name|ngx_http_headers_filter_module_ctx
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
name|ngx_http_headers_create_conf
block|,
comment|/* create location configuration */
name|ngx_http_headers_merge_conf
comment|/* merge location configuration */
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_headers_filter_module
name|ngx_module_t
name|ngx_http_headers_filter_module
init|=
block|{
name|NGX_MODULE
block|,
operator|&
name|ngx_http_headers_filter_module_ctx
block|,
comment|/* module context */
name|ngx_http_headers_filter_commands
block|,
comment|/* module directives */
name|NGX_HTTP_MODULE
block|,
comment|/* module type */
name|ngx_http_headers_filter_init
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

begin_function
DECL|function|ngx_http_headers_filter (ngx_http_request_t * r)
specifier|static
name|ngx_int_t
name|ngx_http_headers_filter
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
block|{
name|size_t
name|len
decl_stmt|;
name|ngx_table_elt_t
modifier|*
name|expires
decl_stmt|,
modifier|*
name|cc
decl_stmt|;
name|ngx_http_headers_conf_t
modifier|*
name|conf
decl_stmt|;
if|if
condition|(
name|r
operator|->
name|headers_out
operator|.
name|status
operator|!=
name|NGX_HTTP_OK
condition|)
block|{
return|return
name|ngx_http_next_header_filter
argument_list|(
name|r
argument_list|)
return|;
block|}
name|conf
operator|=
name|ngx_http_get_module_loc_conf
argument_list|(
name|r
argument_list|,
name|ngx_http_headers_filter_module
argument_list|)
expr_stmt|;
if|if
condition|(
name|conf
operator|->
name|expires
operator|!=
name|NGX_HTTP_EXPIRES_OFF
condition|)
block|{
name|expires
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
expr_stmt|;
if|if
condition|(
name|expires
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|r
operator|->
name|headers_out
operator|.
name|expires
operator|=
name|expires
expr_stmt|;
name|cc
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
expr_stmt|;
if|if
condition|(
name|cc
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|r
operator|->
name|headers_out
operator|.
name|cache_control
operator|=
name|cc
expr_stmt|;
name|len
operator|=
sizeof|sizeof
argument_list|(
literal|"Mon, 28 Sep 1970 06:00:00 GMT"
argument_list|)
expr_stmt|;
name|expires
operator|->
name|key
operator|.
name|len
operator|=
sizeof|sizeof
argument_list|(
literal|"Expires"
argument_list|)
operator|-
literal|1
expr_stmt|;
name|expires
operator|->
name|key
operator|.
name|data
operator|=
operator|(
name|u_char
operator|*
operator|)
literal|"Expires"
expr_stmt|;
name|expires
operator|->
name|value
operator|.
name|len
operator|=
name|len
operator|-
literal|1
expr_stmt|;
name|cc
operator|->
name|key
operator|.
name|len
operator|=
sizeof|sizeof
argument_list|(
literal|"Cache-Control"
argument_list|)
operator|-
literal|1
expr_stmt|;
name|cc
operator|->
name|key
operator|.
name|data
operator|=
operator|(
name|u_char
operator|*
operator|)
literal|"Cache-Control"
expr_stmt|;
if|if
condition|(
name|conf
operator|->
name|expires
operator|==
name|NGX_HTTP_EXPIRES_EPOCH
condition|)
block|{
name|expires
operator|->
name|value
operator|.
name|data
operator|=
operator|(
name|u_char
operator|*
operator|)
literal|"Thu, 01 Jan 1970 00:00:01 GMT"
expr_stmt|;
name|cc
operator|->
name|value
operator|.
name|len
operator|=
sizeof|sizeof
argument_list|(
literal|"no-cache"
argument_list|)
operator|-
literal|1
expr_stmt|;
name|cc
operator|->
name|value
operator|.
name|data
operator|=
operator|(
name|u_char
operator|*
operator|)
literal|"no-cache"
expr_stmt|;
block|}
else|else
block|{
name|expires
operator|->
name|value
operator|.
name|data
operator|=
name|ngx_palloc
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
name|expires
operator|->
name|value
operator|.
name|data
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
name|conf
operator|->
name|expires
operator|==
literal|0
condition|)
block|{
name|ngx_memcpy
argument_list|(
name|expires
operator|->
name|value
operator|.
name|data
argument_list|,
name|ngx_cached_http_time
operator|.
name|data
argument_list|,
name|ngx_cached_http_time
operator|.
name|len
operator|+
literal|1
argument_list|)
expr_stmt|;
name|cc
operator|->
name|value
operator|.
name|len
operator|=
sizeof|sizeof
argument_list|(
literal|"max-age=0"
argument_list|)
operator|-
literal|1
expr_stmt|;
name|cc
operator|->
name|value
operator|.
name|data
operator|=
operator|(
name|u_char
operator|*
operator|)
literal|"max-age=0"
expr_stmt|;
block|}
else|else
block|{
name|ngx_http_time
argument_list|(
name|expires
operator|->
name|value
operator|.
name|data
argument_list|,
name|ngx_cached_time
operator|+
name|conf
operator|->
name|expires
argument_list|)
expr_stmt|;
if|if
condition|(
name|conf
operator|->
name|expires
operator|<
literal|0
condition|)
block|{
name|cc
operator|->
name|value
operator|.
name|len
operator|=
sizeof|sizeof
argument_list|(
literal|"no-cache"
argument_list|)
operator|-
literal|1
expr_stmt|;
name|cc
operator|->
name|value
operator|.
name|data
operator|=
operator|(
name|u_char
operator|*
operator|)
literal|"no-cache"
expr_stmt|;
block|}
else|else
block|{
name|cc
operator|->
name|value
operator|.
name|data
operator|=
name|ngx_palloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
name|NGX_TIME_T_LEN
operator|+
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|cc
operator|->
name|value
operator|.
name|data
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|cc
operator|->
name|value
operator|.
name|len
operator|=
name|ngx_snprintf
argument_list|(
operator|(
name|char
operator|*
operator|)
name|cc
operator|->
name|value
operator|.
name|data
argument_list|,
sizeof|sizeof
argument_list|(
literal|"max-age="
argument_list|)
operator|+
name|NGX_TIME_T_LEN
argument_list|,
literal|"max-age="
name|TIME_T_FMT
argument_list|,
name|conf
operator|->
name|expires
argument_list|)
expr_stmt|;
block|}
block|}
block|}
block|}
return|return
name|ngx_http_next_header_filter
argument_list|(
name|r
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_headers_filter_init (ngx_cycle_t * cycle)
specifier|static
name|ngx_int_t
name|ngx_http_headers_filter_init
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
name|ngx_http_headers_filter
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_headers_create_conf (ngx_conf_t * cf)
specifier|static
name|void
modifier|*
name|ngx_http_headers_create_conf
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
block|{
name|ngx_http_headers_conf_t
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
name|ngx_http_headers_conf_t
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
name|expires
operator|=
name|NGX_HTTP_EXPIRES_UNSET
expr_stmt|;
return|return
name|conf
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_headers_merge_conf (ngx_conf_t * cf,void * parent,void * child)
specifier|static
name|char
modifier|*
name|ngx_http_headers_merge_conf
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
name|ngx_http_headers_conf_t
modifier|*
name|prev
init|=
name|parent
decl_stmt|;
name|ngx_http_headers_conf_t
modifier|*
name|conf
init|=
name|child
decl_stmt|;
if|if
condition|(
name|conf
operator|->
name|expires
operator|==
name|NGX_HTTP_EXPIRES_UNSET
condition|)
block|{
name|conf
operator|->
name|expires
operator|=
operator|(
name|prev
operator|->
name|expires
operator|==
name|NGX_HTTP_EXPIRES_UNSET
operator|)
condition|?
name|NGX_HTTP_EXPIRES_OFF
else|:
name|prev
operator|->
name|expires
expr_stmt|;
block|}
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_headers_expires (ngx_conf_t * cf,ngx_command_t * cmd,void * conf)
name|char
modifier|*
name|ngx_http_headers_expires
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
name|ngx_http_headers_conf_t
modifier|*
name|hcf
init|=
name|conf
decl_stmt|;
name|ngx_uint_t
name|minus
decl_stmt|;
name|ngx_str_t
modifier|*
name|value
decl_stmt|;
if|if
condition|(
name|hcf
operator|->
name|expires
operator|!=
name|NGX_HTTP_EXPIRES_UNSET
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
name|ngx_strcmp
argument_list|(
name|value
index|[
literal|1
index|]
operator|.
name|data
argument_list|,
literal|"epoch"
argument_list|)
operator|==
literal|0
condition|)
block|{
name|hcf
operator|->
name|expires
operator|=
name|NGX_HTTP_EXPIRES_EPOCH
expr_stmt|;
return|return
name|NGX_CONF_OK
return|;
block|}
if|if
condition|(
name|ngx_strcmp
argument_list|(
name|value
index|[
literal|1
index|]
operator|.
name|data
argument_list|,
literal|"off"
argument_list|)
operator|==
literal|0
condition|)
block|{
name|hcf
operator|->
name|expires
operator|=
name|NGX_HTTP_EXPIRES_OFF
expr_stmt|;
return|return
name|NGX_CONF_OK
return|;
block|}
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
literal|'+'
condition|)
block|{
name|value
index|[
literal|1
index|]
operator|.
name|data
operator|++
expr_stmt|;
name|value
index|[
literal|1
index|]
operator|.
name|len
operator|--
expr_stmt|;
name|minus
operator|=
literal|0
expr_stmt|;
block|}
if|else if
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
literal|'-'
condition|)
block|{
name|value
index|[
literal|1
index|]
operator|.
name|data
operator|++
expr_stmt|;
name|value
index|[
literal|1
index|]
operator|.
name|len
operator|--
expr_stmt|;
name|minus
operator|=
literal|1
expr_stmt|;
block|}
else|else
block|{
name|minus
operator|=
literal|0
expr_stmt|;
block|}
name|hcf
operator|->
name|expires
operator|=
name|ngx_parse_time
argument_list|(
operator|&
name|value
index|[
literal|1
index|]
argument_list|,
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|hcf
operator|->
name|expires
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
literal|"invalid value"
return|;
block|}
if|if
condition|(
name|hcf
operator|->
name|expires
operator|==
name|NGX_PARSE_LARGE_TIME
condition|)
block|{
return|return
literal|"value must be less than 68 years"
return|;
block|}
if|if
condition|(
name|minus
condition|)
block|{
name|hcf
operator|->
name|expires
operator|=
operator|-
name|hcf
operator|->
name|expires
expr_stmt|;
block|}
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

end_unit

