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

begin_define
DECL|macro|NGX_HTTP_REWRITE_COPY_MATCH
define|#
directive|define
name|NGX_HTTP_REWRITE_COPY_MATCH
value|0
end_define

begin_define
DECL|macro|NGX_HTTP_REWRITE_COPY_SHORT
define|#
directive|define
name|NGX_HTTP_REWRITE_COPY_SHORT
value|1
end_define

begin_define
DECL|macro|NGX_HTTP_REWRITE_COPY_LONG
define|#
directive|define
name|NGX_HTTP_REWRITE_COPY_LONG
value|2
end_define

begin_typedef
DECL|struct|__anon29fd67380108
typedef|typedef
struct|struct
block|{
DECL|member|op
name|ngx_int_t
name|op
decl_stmt|;
DECL|member|len
name|size_t
name|len
decl_stmt|;
DECL|member|data
name|uintptr_t
name|data
decl_stmt|;
DECL|typedef|ngx_http_rewrite_op_t
block|}
name|ngx_http_rewrite_op_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon29fd67380208
typedef|typedef
struct|struct
block|{
DECL|member|regex
name|ngx_regex_t
modifier|*
name|regex
decl_stmt|;
DECL|member|msize
name|ngx_uint_t
name|msize
decl_stmt|;
DECL|member|ops
name|ngx_array_t
name|ops
decl_stmt|;
DECL|member|size
name|ngx_uint_t
name|size
decl_stmt|;
DECL|member|re_name
name|ngx_str_t
name|re_name
decl_stmt|;
DECL|member|s_name
name|ngx_str_t
name|s_name
decl_stmt|;
DECL|member|status
name|ngx_uint_t
name|status
decl_stmt|;
DECL|member|last
name|unsigned
name|last
range|:
literal|1
decl_stmt|;
DECL|typedef|ngx_http_rewrite_rule_t
block|}
name|ngx_http_rewrite_rule_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon29fd67380308
typedef|typedef
struct|struct
block|{
DECL|member|rules
name|ngx_array_t
name|rules
decl_stmt|;
DECL|member|log
name|ngx_flag_t
name|log
decl_stmt|;
DECL|typedef|ngx_http_rewrite_srv_conf_t
block|}
name|ngx_http_rewrite_srv_conf_t
typedef|;
end_typedef

begin_function_decl
specifier|static
name|void
modifier|*
name|ngx_http_rewrite_create_srv_conf
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
name|ngx_http_rewrite_merge_srv_conf
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
name|ngx_http_rewrite_rule
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
name|ngx_http_rewrite_init
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
function_decl|;
end_function_decl

begin_decl_stmt
DECL|variable|ngx_http_rewrite_commands
specifier|static
name|ngx_command_t
name|ngx_http_rewrite_commands
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"rewrite"
argument_list|)
block|,
name|NGX_HTTP_SRV_CONF
operator||
name|NGX_CONF_TAKE23
block|,
name|ngx_http_rewrite_rule
block|,
name|NGX_HTTP_SRV_CONF_OFFSET
block|,
literal|0
block|,
name|NULL
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"rewrite_log"
argument_list|)
block|,
name|NGX_HTTP_SRV_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_conf_set_flag_slot
block|,
name|NGX_HTTP_SRV_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_http_rewrite_srv_conf_t
argument_list|,
name|log
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
DECL|variable|ngx_http_rewrite_module_ctx
name|ngx_http_module_t
name|ngx_http_rewrite_module_ctx
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
name|ngx_http_rewrite_create_srv_conf
block|,
comment|/* create server configuration */
name|ngx_http_rewrite_merge_srv_conf
block|,
comment|/* merge server configuration */
name|NULL
block|,
comment|/* create location configration */
name|NULL
block|,
comment|/* merge location configration */
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_rewrite_module
name|ngx_module_t
name|ngx_http_rewrite_module
init|=
block|{
name|NGX_MODULE
block|,
operator|&
name|ngx_http_rewrite_module_ctx
block|,
comment|/* module context */
name|ngx_http_rewrite_commands
block|,
comment|/* module directives */
name|NGX_HTTP_MODULE
block|,
comment|/* module type */
name|ngx_http_rewrite_init
block|,
comment|/* init module */
name|NULL
comment|/* init child */
block|}
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|ngx_http_rewrite_handler (ngx_http_request_t * r)
specifier|static
name|ngx_int_t
name|ngx_http_rewrite_handler
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
block|{
name|int
modifier|*
name|matches
decl_stmt|;
name|u_char
modifier|*
name|p
decl_stmt|;
name|size_t
name|len
decl_stmt|;
name|uintptr_t
name|data
decl_stmt|;
name|ngx_int_t
name|rc
decl_stmt|;
name|ngx_uint_t
name|i
decl_stmt|,
name|m
decl_stmt|,
name|n
decl_stmt|;
name|ngx_str_t
name|uri
decl_stmt|;
name|ngx_http_rewrite_op_t
modifier|*
name|op
decl_stmt|;
name|ngx_http_rewrite_rule_t
modifier|*
name|rule
decl_stmt|;
name|ngx_http_rewrite_srv_conf_t
modifier|*
name|scf
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
literal|"http rewrite handler"
argument_list|)
expr_stmt|;
name|scf
operator|=
name|ngx_http_get_module_srv_conf
argument_list|(
name|r
argument_list|,
name|ngx_http_rewrite_module
argument_list|)
expr_stmt|;
name|rule
operator|=
name|scf
operator|->
name|rules
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
name|scf
operator|->
name|rules
operator|.
name|nelts
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|rule
index|[
name|i
index|]
operator|.
name|msize
condition|)
block|{
if|if
condition|(
operator|!
operator|(
name|matches
operator|=
name|ngx_palloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
name|rule
index|[
name|i
index|]
operator|.
name|msize
operator|*
sizeof|sizeof
argument_list|(
name|int
argument_list|)
argument_list|)
operator|)
condition|)
block|{
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
block|}
else|else
block|{
name|matches
operator|=
name|NULL
expr_stmt|;
block|}
name|rc
operator|=
name|ngx_regex_exec
argument_list|(
name|rule
index|[
name|i
index|]
operator|.
name|regex
argument_list|,
operator|&
name|r
operator|->
name|uri
argument_list|,
name|matches
argument_list|,
name|rule
index|[
name|i
index|]
operator|.
name|msize
argument_list|)
expr_stmt|;
if|if
condition|(
name|rc
operator|==
name|NGX_DECLINED
condition|)
block|{
if|if
condition|(
name|scf
operator|->
name|log
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_NOTICE
argument_list|,
name|r
operator|->
name|connection
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"\"%s\" does not match \"%s\""
argument_list|,
name|rule
index|[
name|i
index|]
operator|.
name|re_name
operator|.
name|data
argument_list|,
name|r
operator|->
name|uri
operator|.
name|data
argument_list|)
expr_stmt|;
block|}
continue|continue;
block|}
if|if
condition|(
name|rc
operator|<
literal|0
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|r
operator|->
name|connection
operator|->
name|log
argument_list|,
literal|0
argument_list|,
name|ngx_regex_exec_n
literal|" failed: %d on \"%s\" using \"%s\""
argument_list|,
name|rc
argument_list|,
name|r
operator|->
name|uri
operator|.
name|data
argument_list|,
name|rule
index|[
name|i
index|]
operator|.
name|re_name
operator|.
name|data
argument_list|)
expr_stmt|;
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
if|if
condition|(
name|scf
operator|->
name|log
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_NOTICE
argument_list|,
name|r
operator|->
name|connection
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"\"%s\" matches \"%s\""
argument_list|,
name|rule
index|[
name|i
index|]
operator|.
name|re_name
operator|.
name|data
argument_list|,
name|r
operator|->
name|uri
operator|.
name|data
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|rule
index|[
name|i
index|]
operator|.
name|status
condition|)
block|{
return|return
name|rule
index|[
name|i
index|]
operator|.
name|status
return|;
block|}
name|uri
operator|.
name|len
operator|=
name|rule
index|[
name|i
index|]
operator|.
name|size
expr_stmt|;
for|for
control|(
name|n
operator|=
literal|1
init|;
name|n
operator|<
operator|(
name|ngx_uint_t
operator|)
name|rc
condition|;
name|n
operator|++
control|)
block|{
name|uri
operator|.
name|len
operator|+=
name|matches
index|[
literal|2
operator|*
name|n
operator|+
literal|1
index|]
operator|-
name|matches
index|[
literal|2
operator|*
name|n
index|]
expr_stmt|;
block|}
if|if
condition|(
operator|!
operator|(
name|uri
operator|.
name|data
operator|=
name|ngx_palloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
name|uri
operator|.
name|len
operator|+
literal|1
argument_list|)
operator|)
condition|)
block|{
return|return
name|NGX_HTTP_INTERNAL_SERVER_ERROR
return|;
block|}
name|p
operator|=
name|uri
operator|.
name|data
expr_stmt|;
name|op
operator|=
name|rule
index|[
name|i
index|]
operator|.
name|ops
operator|.
name|elts
expr_stmt|;
for|for
control|(
name|n
operator|=
literal|0
init|;
name|n
operator|<
name|rule
index|[
name|i
index|]
operator|.
name|ops
operator|.
name|nelts
condition|;
name|n
operator|++
control|)
block|{
if|if
condition|(
name|op
index|[
name|n
index|]
operator|.
name|op
operator|==
name|NGX_HTTP_REWRITE_COPY_SHORT
condition|)
block|{
name|len
operator|=
name|op
index|[
name|n
index|]
operator|.
name|len
expr_stmt|;
name|data
operator|=
name|op
index|[
name|n
index|]
operator|.
name|data
expr_stmt|;
while|while
condition|(
name|len
operator|--
condition|)
block|{
operator|*
name|p
operator|++
operator|=
operator|(
name|char
operator|)
operator|(
name|data
operator|&
literal|0xff
operator|)
expr_stmt|;
name|data
operator|>>=
literal|8
expr_stmt|;
block|}
block|}
if|else if
condition|(
name|op
index|[
name|n
index|]
operator|.
name|op
operator|==
name|NGX_HTTP_REWRITE_COPY_LONG
condition|)
block|{
name|p
operator|=
name|ngx_cpymem
argument_list|(
name|p
argument_list|,
operator|(
name|void
operator|*
operator|)
name|op
index|[
name|n
index|]
operator|.
name|data
argument_list|,
name|op
index|[
name|n
index|]
operator|.
name|len
argument_list|)
expr_stmt|;
block|}
else|else
block|{
comment|/* NGX_HTTP_REWRITE_COPY_MATCH */
name|m
operator|=
literal|2
operator|*
name|op
index|[
name|n
index|]
operator|.
name|data
expr_stmt|;
name|p
operator|=
name|ngx_cpymem
argument_list|(
name|p
argument_list|,
operator|&
name|r
operator|->
name|uri
operator|.
name|data
index|[
name|matches
index|[
name|m
index|]
index|]
argument_list|,
name|matches
index|[
name|m
operator|+
literal|1
index|]
operator|-
name|matches
index|[
name|m
index|]
argument_list|)
expr_stmt|;
block|}
block|}
operator|*
name|p
operator|=
literal|'\0'
expr_stmt|;
if|if
condition|(
name|scf
operator|->
name|log
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_NOTICE
argument_list|,
name|r
operator|->
name|connection
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"rewritten uri: \"%s\""
argument_list|,
name|uri
operator|.
name|data
argument_list|)
expr_stmt|;
block|}
name|r
operator|->
name|uri
operator|=
name|uri
expr_stmt|;
if|if
condition|(
name|ngx_http_set_exten
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
if|if
condition|(
name|rule
index|[
name|i
index|]
operator|.
name|last
condition|)
block|{
return|return
name|NGX_DECLINED
return|;
block|}
block|}
return|return
name|NGX_DECLINED
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_rewrite_create_srv_conf (ngx_conf_t * cf)
specifier|static
name|void
modifier|*
name|ngx_http_rewrite_create_srv_conf
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
block|{
name|ngx_http_rewrite_srv_conf_t
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
name|ngx_http_rewrite_srv_conf_t
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
name|conf
operator|->
name|rules
argument_list|,
name|cf
operator|->
name|pool
argument_list|,
literal|5
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_rewrite_rule_t
argument_list|)
argument_list|,
name|NGX_CONF_ERROR
argument_list|)
expr_stmt|;
name|conf
operator|->
name|log
operator|=
name|NGX_CONF_UNSET
expr_stmt|;
return|return
name|conf
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_rewrite_merge_srv_conf (ngx_conf_t * cf,void * parent,void * child)
specifier|static
name|char
modifier|*
name|ngx_http_rewrite_merge_srv_conf
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
name|ngx_http_rewrite_srv_conf_t
modifier|*
name|prev
init|=
name|parent
decl_stmt|;
name|ngx_http_rewrite_srv_conf_t
modifier|*
name|conf
init|=
name|child
decl_stmt|;
name|ngx_conf_merge_value
argument_list|(
name|conf
operator|->
name|log
argument_list|,
name|prev
operator|->
name|log
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
DECL|function|ngx_http_rewrite_rule (ngx_conf_t * cf,ngx_command_t * cmd,void * conf)
specifier|static
name|char
modifier|*
name|ngx_http_rewrite_rule
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
name|ngx_http_rewrite_srv_conf_t
modifier|*
name|scf
init|=
name|conf
decl_stmt|;
name|u_char
modifier|*
name|data
decl_stmt|,
modifier|*
name|p
decl_stmt|;
name|size_t
name|len
decl_stmt|;
name|ngx_str_t
modifier|*
name|value
decl_stmt|,
name|err
decl_stmt|;
name|ngx_uint_t
name|i
decl_stmt|;
name|ngx_http_rewrite_op_t
modifier|*
name|op
decl_stmt|;
name|ngx_http_rewrite_rule_t
modifier|*
name|rule
decl_stmt|;
name|u_char
name|errstr
index|[
name|NGX_MAX_CONF_ERRSTR
index|]
decl_stmt|;
if|if
condition|(
operator|!
operator|(
name|rule
operator|=
name|ngx_push_array
argument_list|(
operator|&
name|scf
operator|->
name|rules
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
name|rule
operator|->
name|ops
argument_list|,
name|cf
operator|->
name|pool
argument_list|,
literal|5
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_rewrite_op_t
argument_list|)
argument_list|,
name|NGX_CONF_ERROR
argument_list|)
expr_stmt|;
name|rule
operator|->
name|msize
operator|=
literal|0
expr_stmt|;
name|rule
operator|->
name|size
operator|=
literal|0
expr_stmt|;
name|rule
operator|->
name|status
operator|=
literal|0
expr_stmt|;
name|rule
operator|->
name|last
operator|=
literal|0
expr_stmt|;
name|value
operator|=
name|cf
operator|->
name|args
operator|->
name|elts
expr_stmt|;
comment|/* STUB */
block|{
name|err
operator|.
name|len
operator|=
name|NGX_MAX_CONF_ERRSTR
expr_stmt|;
name|err
operator|.
name|data
operator|=
name|errstr
expr_stmt|;
name|rule
operator|->
name|regex
operator|=
name|ngx_regex_compile
argument_list|(
operator|&
name|value
index|[
literal|1
index|]
argument_list|,
literal|0
argument_list|,
name|cf
operator|->
name|pool
argument_list|,
operator|&
name|err
argument_list|)
expr_stmt|;
if|if
condition|(
name|rule
operator|->
name|regex
operator|==
name|NULL
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
literal|"%s"
argument_list|,
name|err
operator|.
name|data
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|rule
operator|->
name|re_name
operator|=
name|value
index|[
literal|1
index|]
expr_stmt|;
name|rule
operator|->
name|s_name
operator|=
name|value
index|[
literal|2
index|]
expr_stmt|;
if|if
condition|(
name|ngx_strcasecmp
argument_list|(
name|value
index|[
literal|2
index|]
operator|.
name|data
argument_list|,
literal|"forbidden:"
argument_list|)
operator|==
literal|0
condition|)
block|{
if|if
condition|(
name|cf
operator|->
name|args
operator|->
name|nelts
operator|==
literal|3
condition|)
block|{
name|rule
operator|->
name|status
operator|=
name|NGX_HTTP_FORBIDDEN
expr_stmt|;
name|rule
operator|->
name|last
operator|=
literal|1
expr_stmt|;
return|return
name|NGX_CONF_OK
return|;
block|}
name|ngx_conf_log_error
argument_list|(
name|NGX_LOG_EMERG
argument_list|,
name|cf
argument_list|,
literal|0
argument_list|,
literal|"invalid parameter \"%s\""
argument_list|,
name|value
index|[
literal|3
index|]
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
literal|0
init|;
name|i
operator|<
name|value
index|[
literal|2
index|]
operator|.
name|len
condition|;
comment|/* void */
control|)
block|{
if|if
condition|(
operator|!
operator|(
name|op
operator|=
name|ngx_push_array
argument_list|(
operator|&
name|rule
operator|->
name|ops
argument_list|)
operator|)
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|data
operator|=
operator|&
name|value
index|[
literal|2
index|]
operator|.
name|data
index|[
name|i
index|]
expr_stmt|;
if|if
condition|(
name|value
index|[
literal|2
index|]
operator|.
name|data
index|[
name|i
index|]
operator|==
literal|'$'
operator|&&
name|i
operator|<
name|value
index|[
literal|2
index|]
operator|.
name|len
operator|&&
name|value
index|[
literal|2
index|]
operator|.
name|data
index|[
name|i
operator|+
literal|1
index|]
operator|>=
literal|'1'
operator|&&
name|value
index|[
literal|2
index|]
operator|.
name|data
index|[
name|i
operator|+
literal|1
index|]
operator|<=
literal|'9'
condition|)
block|{
name|op
operator|->
name|op
operator|=
name|NGX_HTTP_REWRITE_COPY_MATCH
expr_stmt|;
name|op
operator|->
name|data
operator|=
name|value
index|[
literal|2
index|]
operator|.
name|data
index|[
operator|++
name|i
index|]
operator|-
literal|'0'
expr_stmt|;
if|if
condition|(
name|rule
operator|->
name|msize
operator|<
name|op
operator|->
name|data
condition|)
block|{
name|rule
operator|->
name|msize
operator|=
name|op
operator|->
name|data
expr_stmt|;
block|}
name|i
operator|++
expr_stmt|;
block|}
else|else
block|{
name|i
operator|++
expr_stmt|;
while|while
condition|(
name|i
operator|<
name|value
index|[
literal|2
index|]
operator|.
name|len
operator|&&
name|value
index|[
literal|2
index|]
operator|.
name|data
index|[
name|i
index|]
operator|!=
literal|'$'
condition|)
block|{
name|i
operator|++
expr_stmt|;
block|}
name|len
operator|=
operator|&
name|value
index|[
literal|2
index|]
operator|.
name|data
index|[
name|i
index|]
operator|-
name|data
expr_stmt|;
name|rule
operator|->
name|size
operator|+=
name|len
expr_stmt|;
if|if
condition|(
name|len
condition|)
block|{
name|op
operator|->
name|len
operator|=
name|len
expr_stmt|;
if|if
condition|(
name|len
operator|<=
sizeof|sizeof
argument_list|(
name|uintptr_t
argument_list|)
condition|)
block|{
name|op
operator|->
name|op
operator|=
name|NGX_HTTP_REWRITE_COPY_SHORT
expr_stmt|;
name|op
operator|->
name|data
operator|=
literal|0
expr_stmt|;
while|while
condition|(
name|len
operator|--
condition|)
block|{
name|op
operator|->
name|data
operator|<<=
literal|8
expr_stmt|;
name|op
operator|->
name|data
operator||=
name|data
index|[
name|len
index|]
expr_stmt|;
block|}
block|}
else|else
block|{
name|op
operator|->
name|op
operator|=
name|NGX_HTTP_REWRITE_COPY_LONG
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|p
operator|=
name|ngx_palloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
name|len
argument_list|)
operator|)
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|ngx_memcpy
argument_list|(
name|p
argument_list|,
name|data
argument_list|,
name|len
argument_list|)
expr_stmt|;
name|op
operator|->
name|data
operator|=
operator|(
name|uintptr_t
operator|)
name|p
expr_stmt|;
block|}
block|}
block|}
block|}
if|if
condition|(
name|rule
operator|->
name|msize
condition|)
block|{
name|rule
operator|->
name|msize
operator|++
expr_stmt|;
name|rule
operator|->
name|msize
operator|*=
literal|3
expr_stmt|;
block|}
if|if
condition|(
name|cf
operator|->
name|args
operator|->
name|nelts
operator|>
literal|3
condition|)
block|{
if|if
condition|(
name|ngx_strcmp
argument_list|(
name|value
index|[
literal|3
index|]
operator|.
name|data
argument_list|,
literal|"last"
argument_list|)
operator|==
literal|0
condition|)
block|{
name|rule
operator|->
name|last
operator|=
literal|1
expr_stmt|;
block|}
else|else
block|{
name|ngx_conf_log_error
argument_list|(
name|NGX_LOG_EMERG
argument_list|,
name|cf
argument_list|,
literal|0
argument_list|,
literal|"invalid parameter \"%s\""
argument_list|,
name|value
index|[
literal|3
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
block|}
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_rewrite_init (ngx_cycle_t * cycle)
specifier|static
name|ngx_int_t
name|ngx_http_rewrite_init
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
name|NGX_HTTP_REWRITE_PHASE
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
name|ngx_http_rewrite_handler
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

end_unit

