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
DECL|typedef|ngx_ssl_variable_handler_pt
typedef|typedef
name|ngx_int_t
function_decl|(
modifier|*
name|ngx_ssl_variable_handler_pt
function_decl|)
parameter_list|(
name|ngx_connection_t
modifier|*
name|c
parameter_list|,
name|ngx_pool_t
modifier|*
name|pool
parameter_list|,
name|ngx_str_t
modifier|*
name|s
parameter_list|)
function_decl|;
end_typedef

begin_define
DECL|macro|NGX_DEFLAUT_CERTIFICATE
define|#
directive|define
name|NGX_DEFLAUT_CERTIFICATE
value|"cert.pem"
end_define

begin_define
DECL|macro|NGX_DEFLAUT_CERTIFICATE_KEY
define|#
directive|define
name|NGX_DEFLAUT_CERTIFICATE_KEY
value|"cert.pem"
end_define

begin_define
DECL|macro|NGX_DEFLAUT_CIPHERS
define|#
directive|define
name|NGX_DEFLAUT_CIPHERS
value|"ALL:!ADH:RC4+RSA:+HIGH:+MEDIUM:+LOW:+SSLv2:+EXP"
end_define

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_ssl_static_variable
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
name|ngx_int_t
name|ngx_http_ssl_variable
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
name|ngx_int_t
name|ngx_http_ssl_add_variables
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|void
modifier|*
name|ngx_http_ssl_create_srv_conf
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
name|ngx_http_ssl_merge_srv_conf
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

begin_if
if|#
directive|if
operator|!
name|defined
argument_list|(
name|SSL_OP_CIPHER_SERVER_PREFERENCE
argument_list|)
end_if

begin_function_decl
specifier|static
name|char
modifier|*
name|ngx_http_ssl_nosupported
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
DECL|variable|ngx_http_ssl_openssl097
specifier|static
name|char
name|ngx_http_ssl_openssl097
index|[]
init|=
literal|"OpenSSL 0.9.7 and higher"
decl_stmt|;
end_decl_stmt

begin_endif
endif|#
directive|endif
end_endif

begin_decl_stmt
DECL|variable|ngx_http_ssl_protocols
specifier|static
name|ngx_conf_bitmask_t
name|ngx_http_ssl_protocols
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"SSLv2"
argument_list|)
block|,
name|NGX_SSL_SSLv2
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"SSLv3"
argument_list|)
block|,
name|NGX_SSL_SSLv3
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"TLSv1"
argument_list|)
block|,
name|NGX_SSL_TLSv1
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
DECL|variable|ngx_http_ssl_commands
specifier|static
name|ngx_command_t
name|ngx_http_ssl_commands
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"ssl"
argument_list|)
block|,
name|NGX_HTTP_MAIN_CONF
operator||
name|NGX_HTTP_SRV_CONF
operator||
name|NGX_CONF_FLAG
block|,
name|ngx_conf_set_flag_slot
block|,
name|NGX_HTTP_SRV_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_http_ssl_srv_conf_t
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
literal|"ssl_certificate"
argument_list|)
block|,
name|NGX_HTTP_MAIN_CONF
operator||
name|NGX_HTTP_SRV_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_conf_set_str_slot
block|,
name|NGX_HTTP_SRV_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_http_ssl_srv_conf_t
argument_list|,
name|certificate
argument_list|)
block|,
name|NULL
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"ssl_certificate_key"
argument_list|)
block|,
name|NGX_HTTP_MAIN_CONF
operator||
name|NGX_HTTP_SRV_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_conf_set_str_slot
block|,
name|NGX_HTTP_SRV_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_http_ssl_srv_conf_t
argument_list|,
name|certificate_key
argument_list|)
block|,
name|NULL
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"ssl_protocols"
argument_list|)
block|,
name|NGX_HTTP_MAIN_CONF
operator||
name|NGX_HTTP_SRV_CONF
operator||
name|NGX_CONF_1MORE
block|,
name|ngx_conf_set_bitmask_slot
block|,
name|NGX_HTTP_SRV_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_http_ssl_srv_conf_t
argument_list|,
name|protocols
argument_list|)
block|,
operator|&
name|ngx_http_ssl_protocols
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"ssl_ciphers"
argument_list|)
block|,
name|NGX_HTTP_MAIN_CONF
operator||
name|NGX_HTTP_SRV_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_conf_set_str_slot
block|,
name|NGX_HTTP_SRV_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_http_ssl_srv_conf_t
argument_list|,
name|ciphers
argument_list|)
block|,
name|NULL
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"ssl_verify_client"
argument_list|)
block|,
name|NGX_HTTP_MAIN_CONF
operator||
name|NGX_HTTP_SRV_CONF
operator||
name|NGX_CONF_FLAG
block|,
name|ngx_conf_set_flag_slot
block|,
name|NGX_HTTP_SRV_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_http_ssl_srv_conf_t
argument_list|,
name|verify
argument_list|)
block|,
name|NULL
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"ssl_verify_depth"
argument_list|)
block|,
name|NGX_HTTP_MAIN_CONF
operator||
name|NGX_HTTP_SRV_CONF
operator||
name|NGX_CONF_1MORE
block|,
name|ngx_conf_set_num_slot
block|,
name|NGX_HTTP_SRV_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_http_ssl_srv_conf_t
argument_list|,
name|verify_depth
argument_list|)
block|,
name|NULL
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"ssl_client_certificate"
argument_list|)
block|,
name|NGX_HTTP_MAIN_CONF
operator||
name|NGX_HTTP_SRV_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_conf_set_str_slot
block|,
name|NGX_HTTP_SRV_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_http_ssl_srv_conf_t
argument_list|,
name|client_certificate
argument_list|)
block|,
name|NULL
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"ssl_prefer_server_ciphers"
argument_list|)
block|,
name|NGX_HTTP_MAIN_CONF
operator||
name|NGX_HTTP_SRV_CONF
operator||
name|NGX_CONF_FLAG
block|,
ifdef|#
directive|ifdef
name|SSL_OP_CIPHER_SERVER_PREFERENCE
name|ngx_conf_set_flag_slot
block|,
name|NGX_HTTP_SRV_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_http_ssl_srv_conf_t
argument_list|,
name|prefer_server_ciphers
argument_list|)
block|,
name|NULL
block|}
block|,
else|#
directive|else
name|ngx_http_ssl_nosupported
block|,
literal|0
block|,
literal|0
block|,
name|ngx_http_ssl_openssl097
block|}
decl_stmt|,
endif|#
directive|endif
block|{
name|ngx_string
argument_list|(
literal|"ssl_session_timeout"
argument_list|)
operator|,
name|NGX_HTTP_MAIN_CONF
operator||
name|NGX_HTTP_SRV_CONF
operator||
name|NGX_CONF_TAKE1
operator|,
name|ngx_conf_set_sec_slot
operator|,
name|NGX_HTTP_SRV_CONF_OFFSET
operator|,
name|offsetof
argument_list|(
name|ngx_http_ssl_srv_conf_t
argument_list|,
name|session_timeout
argument_list|)
operator|,
name|NULL
block|}
end_decl_stmt

begin_operator
operator|,
end_operator

begin_expr_stmt
name|ngx_null_command
end_expr_stmt

begin_decl_stmt
unit|};
DECL|variable|ngx_http_ssl_module_ctx
specifier|static
name|ngx_http_module_t
name|ngx_http_ssl_module_ctx
init|=
block|{
name|ngx_http_ssl_add_variables
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
name|ngx_http_ssl_create_srv_conf
block|,
comment|/* create server configuration */
name|ngx_http_ssl_merge_srv_conf
block|,
comment|/* merge server configuration */
name|NULL
block|,
comment|/* create location configuration */
name|NULL
comment|/* merge location configuration */
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_ssl_module
name|ngx_module_t
name|ngx_http_ssl_module
init|=
block|{
name|NGX_MODULE_V1
block|,
operator|&
name|ngx_http_ssl_module_ctx
block|,
comment|/* module context */
name|ngx_http_ssl_commands
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
DECL|variable|ngx_http_ssl_vars
specifier|static
name|ngx_http_variable_t
name|ngx_http_ssl_vars
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"ssl_protocol"
argument_list|)
block|,
name|NULL
block|,
name|ngx_http_ssl_static_variable
block|,
operator|(
name|uintptr_t
operator|)
name|ngx_ssl_get_protocol
block|,
name|NGX_HTTP_VAR_CHANGABLE
block|,
literal|0
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"ssl_cipher"
argument_list|)
block|,
name|NULL
block|,
name|ngx_http_ssl_static_variable
block|,
operator|(
name|uintptr_t
operator|)
name|ngx_ssl_get_cipher_name
block|,
name|NGX_HTTP_VAR_CHANGABLE
block|,
literal|0
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"ssl_client_s_dn"
argument_list|)
block|,
name|NULL
block|,
name|ngx_http_ssl_variable
block|,
operator|(
name|uintptr_t
operator|)
name|ngx_ssl_get_subject_dn
block|,
name|NGX_HTTP_VAR_CHANGABLE
block|,
literal|0
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"ssl_client_i_dn"
argument_list|)
block|,
name|NULL
block|,
name|ngx_http_ssl_variable
block|,
operator|(
name|uintptr_t
operator|)
name|ngx_ssl_get_issuer_dn
block|,
name|NGX_HTTP_VAR_CHANGABLE
block|,
literal|0
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"ssl_client_serial"
argument_list|)
block|,
name|NULL
block|,
name|ngx_http_ssl_variable
block|,
operator|(
name|uintptr_t
operator|)
name|ngx_ssl_get_serial_number
block|,
name|NGX_HTTP_VAR_CHANGABLE
block|,
literal|0
block|}
block|,
block|{
name|ngx_null_string
block|,
name|NULL
block|,
name|NULL
block|,
literal|0
block|,
literal|0
block|,
literal|0
block|}
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_session_id_ctx
specifier|static
name|u_char
name|ngx_http_session_id_ctx
index|[]
init|=
literal|"HTTP"
decl_stmt|;
end_decl_stmt

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_ssl_static_variable (ngx_http_request_t * r,ngx_http_variable_value_t * v,uintptr_t data)
name|ngx_http_ssl_static_variable
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
name|ngx_ssl_variable_handler_pt
name|handler
init|=
operator|(
name|ngx_ssl_variable_handler_pt
operator|)
name|data
decl_stmt|;
name|size_t
name|len
decl_stmt|;
if|if
condition|(
name|r
operator|->
name|connection
operator|->
name|ssl
condition|)
block|{
operator|(
name|void
operator|)
name|handler
argument_list|(
name|r
operator|->
name|connection
argument_list|,
name|NULL
argument_list|,
operator|(
name|ngx_str_t
operator|*
operator|)
name|v
argument_list|)
expr_stmt|;
for|for
control|(
name|len
operator|=
literal|0
init|;
name|v
operator|->
name|data
index|[
name|len
index|]
condition|;
name|len
operator|++
control|)
block|{
comment|/* void */
block|}
name|v
operator|->
name|len
operator|=
name|len
expr_stmt|;
name|v
operator|->
name|valid
operator|=
literal|1
expr_stmt|;
name|v
operator|->
name|no_cachable
operator|=
literal|0
expr_stmt|;
name|v
operator|->
name|not_found
operator|=
literal|0
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
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
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_ssl_variable (ngx_http_request_t * r,ngx_http_variable_value_t * v,uintptr_t data)
name|ngx_http_ssl_variable
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
name|ngx_ssl_variable_handler_pt
name|handler
init|=
operator|(
name|ngx_ssl_variable_handler_pt
operator|)
name|data
decl_stmt|;
if|if
condition|(
name|r
operator|->
name|connection
operator|->
name|ssl
condition|)
block|{
if|if
condition|(
name|handler
argument_list|(
name|r
operator|->
name|connection
argument_list|,
name|r
operator|->
name|pool
argument_list|,
operator|(
name|ngx_str_t
operator|*
operator|)
name|v
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|v
operator|->
name|len
condition|)
block|{
name|v
operator|->
name|valid
operator|=
literal|1
expr_stmt|;
name|v
operator|->
name|no_cachable
operator|=
literal|0
expr_stmt|;
name|v
operator|->
name|not_found
operator|=
literal|0
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
block|}
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
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_ssl_add_variables (ngx_conf_t * cf)
name|ngx_http_ssl_add_variables
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
block|{
name|ngx_http_variable_t
modifier|*
name|var
decl_stmt|,
modifier|*
name|v
decl_stmt|;
for|for
control|(
name|v
operator|=
name|ngx_http_ssl_vars
init|;
name|v
operator|->
name|name
operator|.
name|len
condition|;
name|v
operator|++
control|)
block|{
name|var
operator|=
name|ngx_http_add_variable
argument_list|(
name|cf
argument_list|,
operator|&
name|v
operator|->
name|name
argument_list|,
name|v
operator|->
name|flags
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
name|v
operator|->
name|get_handler
expr_stmt|;
name|var
operator|->
name|data
operator|=
name|v
operator|->
name|data
expr_stmt|;
block|}
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
specifier|static
name|void
modifier|*
DECL|function|ngx_http_ssl_create_srv_conf (ngx_conf_t * cf)
name|ngx_http_ssl_create_srv_conf
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
block|{
name|ngx_http_ssl_srv_conf_t
modifier|*
name|sscf
decl_stmt|;
name|sscf
operator|=
name|ngx_pcalloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_ssl_srv_conf_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|sscf
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
comment|/*      * set by ngx_pcalloc():      *      *     sscf->protocols = 0;      *     sscf->certificate.len = 0;      *     sscf->certificate.data = NULL;      *     sscf->certificate_key.len = 0;      *     sscf->certificate_key.data = NULL;      *     sscf->client_certificate.len = 0;      *     sscf->client_certificate.data = NULL;      *     sscf->ciphers.len = 0;      *     sscf->ciphers.data = NULL;      */
name|sscf
operator|->
name|enable
operator|=
name|NGX_CONF_UNSET
expr_stmt|;
name|sscf
operator|->
name|session_timeout
operator|=
name|NGX_CONF_UNSET
expr_stmt|;
name|sscf
operator|->
name|verify
operator|=
name|NGX_CONF_UNSET
expr_stmt|;
name|sscf
operator|->
name|verify_depth
operator|=
name|NGX_CONF_UNSET
expr_stmt|;
name|sscf
operator|->
name|prefer_server_ciphers
operator|=
name|NGX_CONF_UNSET
expr_stmt|;
return|return
name|sscf
return|;
block|}
end_function

begin_function
specifier|static
name|char
modifier|*
DECL|function|ngx_http_ssl_merge_srv_conf (ngx_conf_t * cf,void * parent,void * child)
name|ngx_http_ssl_merge_srv_conf
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
name|ngx_http_ssl_srv_conf_t
modifier|*
name|prev
init|=
name|parent
decl_stmt|;
name|ngx_http_ssl_srv_conf_t
modifier|*
name|conf
init|=
name|child
decl_stmt|;
name|ngx_pool_cleanup_t
modifier|*
name|cln
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
if|if
condition|(
name|conf
operator|->
name|enable
operator|==
literal|0
condition|)
block|{
return|return
name|NGX_CONF_OK
return|;
block|}
name|ngx_conf_merge_value
argument_list|(
name|conf
operator|->
name|session_timeout
argument_list|,
name|prev
operator|->
name|session_timeout
argument_list|,
literal|300
argument_list|)
expr_stmt|;
name|ngx_conf_merge_value
argument_list|(
name|conf
operator|->
name|prefer_server_ciphers
argument_list|,
name|prev
operator|->
name|prefer_server_ciphers
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|ngx_conf_merge_bitmask_value
argument_list|(
name|conf
operator|->
name|protocols
argument_list|,
name|prev
operator|->
name|protocols
argument_list|,
operator|(
name|NGX_CONF_BITMASK_SET
operator||
name|NGX_SSL_SSLv2
operator||
name|NGX_SSL_SSLv3
operator||
name|NGX_SSL_TLSv1
operator|)
argument_list|)
expr_stmt|;
name|ngx_conf_merge_value
argument_list|(
name|conf
operator|->
name|verify
argument_list|,
name|prev
operator|->
name|verify
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|ngx_conf_merge_value
argument_list|(
name|conf
operator|->
name|verify_depth
argument_list|,
name|prev
operator|->
name|verify_depth
argument_list|,
literal|1
argument_list|)
expr_stmt|;
name|ngx_conf_merge_str_value
argument_list|(
name|conf
operator|->
name|certificate
argument_list|,
name|prev
operator|->
name|certificate
argument_list|,
name|NGX_DEFLAUT_CERTIFICATE
argument_list|)
expr_stmt|;
name|ngx_conf_merge_str_value
argument_list|(
name|conf
operator|->
name|certificate_key
argument_list|,
name|prev
operator|->
name|certificate_key
argument_list|,
name|NGX_DEFLAUT_CERTIFICATE_KEY
argument_list|)
expr_stmt|;
name|ngx_conf_merge_str_value
argument_list|(
name|conf
operator|->
name|client_certificate
argument_list|,
name|prev
operator|->
name|client_certificate
argument_list|,
literal|""
argument_list|)
expr_stmt|;
name|ngx_conf_merge_str_value
argument_list|(
name|conf
operator|->
name|ciphers
argument_list|,
name|prev
operator|->
name|ciphers
argument_list|,
name|NGX_DEFLAUT_CIPHERS
argument_list|)
expr_stmt|;
name|conf
operator|->
name|ssl
operator|.
name|log
operator|=
name|cf
operator|->
name|log
expr_stmt|;
if|if
condition|(
name|ngx_ssl_create
argument_list|(
operator|&
name|conf
operator|->
name|ssl
argument_list|,
name|conf
operator|->
name|protocols
argument_list|,
name|conf
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|cln
operator|=
name|ngx_pool_cleanup_add
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
literal|0
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
name|NGX_CONF_ERROR
return|;
block|}
name|cln
operator|->
name|handler
operator|=
name|ngx_ssl_cleanup_ctx
expr_stmt|;
name|cln
operator|->
name|data
operator|=
operator|&
name|conf
operator|->
name|ssl
expr_stmt|;
if|if
condition|(
name|ngx_ssl_certificate
argument_list|(
name|cf
argument_list|,
operator|&
name|conf
operator|->
name|ssl
argument_list|,
operator|&
name|conf
operator|->
name|certificate
argument_list|,
operator|&
name|conf
operator|->
name|certificate_key
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
if|if
condition|(
name|SSL_CTX_set_cipher_list
argument_list|(
name|conf
operator|->
name|ssl
operator|.
name|ctx
argument_list|,
operator|(
specifier|const
name|char
operator|*
operator|)
name|conf
operator|->
name|ciphers
operator|.
name|data
argument_list|)
operator|==
literal|0
condition|)
block|{
name|ngx_ssl_error
argument_list|(
name|NGX_LOG_EMERG
argument_list|,
name|cf
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"SSL_CTX_set_cipher_list(\"%V\") failed"
argument_list|,
operator|&
name|conf
operator|->
name|ciphers
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|conf
operator|->
name|verify
condition|)
block|{
if|if
condition|(
name|ngx_ssl_client_certificate
argument_list|(
name|cf
argument_list|,
operator|&
name|conf
operator|->
name|ssl
argument_list|,
operator|&
name|conf
operator|->
name|client_certificate
argument_list|,
name|conf
operator|->
name|verify_depth
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
block|}
ifdef|#
directive|ifdef
name|SSL_OP_CIPHER_SERVER_PREFERENCE
if|if
condition|(
name|conf
operator|->
name|prefer_server_ciphers
condition|)
block|{
name|SSL_CTX_set_options
argument_list|(
name|conf
operator|->
name|ssl
operator|.
name|ctx
argument_list|,
name|SSL_OP_CIPHER_SERVER_PREFERENCE
argument_list|)
expr_stmt|;
block|}
endif|#
directive|endif
comment|/* a temporary 512-bit RSA key is required for export versions of MSIE */
if|if
condition|(
name|ngx_ssl_generate_rsa512_key
argument_list|(
operator|&
name|conf
operator|->
name|ssl
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|SSL_CTX_set_session_cache_mode
argument_list|(
name|conf
operator|->
name|ssl
operator|.
name|ctx
argument_list|,
name|SSL_SESS_CACHE_SERVER
argument_list|)
expr_stmt|;
name|SSL_CTX_set_session_id_context
argument_list|(
name|conf
operator|->
name|ssl
operator|.
name|ctx
argument_list|,
name|ngx_http_session_id_ctx
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_session_id_ctx
argument_list|)
operator|-
literal|1
argument_list|)
expr_stmt|;
name|SSL_CTX_set_timeout
argument_list|(
name|conf
operator|->
name|ssl
operator|.
name|ctx
argument_list|,
name|conf
operator|->
name|session_timeout
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

begin_if
if|#
directive|if
operator|!
name|defined
argument_list|(
name|SSL_OP_CIPHER_SERVER_PREFERENCE
argument_list|)
end_if

begin_function
specifier|static
name|char
modifier|*
DECL|function|ngx_http_ssl_nosupported (ngx_conf_t * cf,ngx_command_t * cmd,void * conf)
name|ngx_http_ssl_nosupported
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
literal|"\"%V\" directive is available only in %s,"
argument_list|,
operator|&
name|cmd
operator|->
name|name
argument_list|,
name|cmd
operator|->
name|post
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
end_function

begin_endif
endif|#
directive|endif
end_endif

end_unit

