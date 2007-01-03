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
file|<ngx_imap.h>
end_include

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
name|void
modifier|*
name|ngx_imap_ssl_create_conf
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
name|ngx_imap_ssl_merge_conf
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
name|ngx_imap_ssl_session_cache
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
name|ngx_imap_ssl_nosupported
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
DECL|variable|ngx_imap_ssl_openssl097
specifier|static
name|char
name|ngx_imap_ssl_openssl097
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
DECL|variable|ngx_http_starttls_state
specifier|static
name|ngx_conf_enum_t
name|ngx_http_starttls_state
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"off"
argument_list|)
block|,
name|NGX_IMAP_STARTTLS_OFF
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"on"
argument_list|)
block|,
name|NGX_IMAP_STARTTLS_ON
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"only"
argument_list|)
block|,
name|NGX_IMAP_STARTTLS_ONLY
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
DECL|variable|ngx_imap_ssl_protocols
specifier|static
name|ngx_conf_bitmask_t
name|ngx_imap_ssl_protocols
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
DECL|variable|ngx_imap_ssl_commands
specifier|static
name|ngx_command_t
name|ngx_imap_ssl_commands
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"ssl"
argument_list|)
block|,
name|NGX_IMAP_MAIN_CONF
operator||
name|NGX_IMAP_SRV_CONF
operator||
name|NGX_CONF_FLAG
block|,
name|ngx_conf_set_flag_slot
block|,
name|NGX_IMAP_SRV_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_imap_ssl_conf_t
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
literal|"starttls"
argument_list|)
block|,
name|NGX_IMAP_MAIN_CONF
operator||
name|NGX_IMAP_SRV_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_conf_set_enum_slot
block|,
name|NGX_IMAP_SRV_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_imap_ssl_conf_t
argument_list|,
name|starttls
argument_list|)
block|,
name|ngx_http_starttls_state
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"ssl_certificate"
argument_list|)
block|,
name|NGX_IMAP_MAIN_CONF
operator||
name|NGX_IMAP_SRV_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_conf_set_str_slot
block|,
name|NGX_IMAP_SRV_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_imap_ssl_conf_t
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
name|NGX_IMAP_MAIN_CONF
operator||
name|NGX_IMAP_SRV_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_conf_set_str_slot
block|,
name|NGX_IMAP_SRV_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_imap_ssl_conf_t
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
name|NGX_IMAP_MAIN_CONF
operator||
name|NGX_IMAP_SRV_CONF
operator||
name|NGX_CONF_1MORE
block|,
name|ngx_conf_set_bitmask_slot
block|,
name|NGX_IMAP_SRV_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_imap_ssl_conf_t
argument_list|,
name|protocols
argument_list|)
block|,
operator|&
name|ngx_imap_ssl_protocols
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"ssl_ciphers"
argument_list|)
block|,
name|NGX_IMAP_MAIN_CONF
operator||
name|NGX_IMAP_SRV_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_conf_set_str_slot
block|,
name|NGX_IMAP_SRV_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_imap_ssl_conf_t
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
literal|"ssl_prefer_server_ciphers"
argument_list|)
block|,
name|NGX_IMAP_MAIN_CONF
operator||
name|NGX_IMAP_SRV_CONF
operator||
name|NGX_CONF_FLAG
block|,
ifdef|#
directive|ifdef
name|SSL_OP_CIPHER_SERVER_PREFERENCE
name|ngx_conf_set_flag_slot
block|,
name|NGX_IMAP_SRV_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_imap_ssl_conf_t
argument_list|,
name|prefer_server_ciphers
argument_list|)
block|,
name|NULL
block|}
block|,
else|#
directive|else
name|ngx_imap_ssl_nosupported
block|,
literal|0
block|,
literal|0
block|,
name|ngx_imap_ssl_openssl097
block|}
decl_stmt|,
endif|#
directive|endif
block|{
name|ngx_string
argument_list|(
literal|"ssl_session_cache"
argument_list|)
operator|,
name|NGX_IMAP_MAIN_CONF
operator||
name|NGX_IMAP_SRV_CONF
operator||
name|NGX_CONF_TAKE12
operator|,
name|ngx_imap_ssl_session_cache
operator|,
name|NGX_IMAP_SRV_CONF_OFFSET
operator|,
literal|0
operator|,
name|NULL
block|}
end_decl_stmt

begin_operator
operator|,
end_operator

begin_block
block|{
name|ngx_string
argument_list|(
literal|"ssl_session_timeout"
argument_list|)
operator|,
name|NGX_IMAP_MAIN_CONF
operator||
name|NGX_IMAP_SRV_CONF
operator||
name|NGX_CONF_TAKE1
operator|,
name|ngx_conf_set_sec_slot
operator|,
name|NGX_IMAP_SRV_CONF_OFFSET
operator|,
name|offsetof
argument_list|(
name|ngx_imap_ssl_conf_t
argument_list|,
name|session_timeout
argument_list|)
operator|,
name|NULL
block|}
end_block

begin_operator
operator|,
end_operator

begin_expr_stmt
name|ngx_null_command
end_expr_stmt

begin_decl_stmt
unit|};
DECL|variable|ngx_imap_ssl_module_ctx
specifier|static
name|ngx_imap_module_t
name|ngx_imap_ssl_module_ctx
init|=
block|{
name|NULL
block|,
comment|/* create main configuration */
name|NULL
block|,
comment|/* init main configuration */
name|ngx_imap_ssl_create_conf
block|,
comment|/* create server configuration */
name|ngx_imap_ssl_merge_conf
comment|/* merge server configuration */
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_imap_ssl_module
name|ngx_module_t
name|ngx_imap_ssl_module
init|=
block|{
name|NGX_MODULE_V1
block|,
operator|&
name|ngx_imap_ssl_module_ctx
block|,
comment|/* module context */
name|ngx_imap_ssl_commands
block|,
comment|/* module directives */
name|NGX_IMAP_MODULE
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
DECL|variable|ngx_imap_ssl_sess_id_ctx
specifier|static
name|ngx_str_t
name|ngx_imap_ssl_sess_id_ctx
init|=
name|ngx_string
argument_list|(
literal|"IMAP"
argument_list|)
decl_stmt|;
end_decl_stmt

begin_function
specifier|static
name|void
modifier|*
DECL|function|ngx_imap_ssl_create_conf (ngx_conf_t * cf)
name|ngx_imap_ssl_create_conf
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
block|{
name|ngx_imap_ssl_conf_t
modifier|*
name|scf
decl_stmt|;
name|scf
operator|=
name|ngx_pcalloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_imap_ssl_conf_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|scf
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
comment|/*      * set by ngx_pcalloc():      *      *     scf->protocols = 0;      *     scf->certificate.len = 0;      *     scf->certificate.data = NULL;      *     scf->certificate_key.len = 0;      *     scf->certificate_key.data = NULL;      *     scf->ciphers.len = 0;      *     scf->ciphers.data = NULL;      *     scf->shm_zone = NULL;      */
name|scf
operator|->
name|enable
operator|=
name|NGX_CONF_UNSET
expr_stmt|;
name|scf
operator|->
name|starttls
operator|=
name|NGX_CONF_UNSET
expr_stmt|;
name|scf
operator|->
name|prefer_server_ciphers
operator|=
name|NGX_CONF_UNSET
expr_stmt|;
name|scf
operator|->
name|builtin_session_cache
operator|=
name|NGX_CONF_UNSET
expr_stmt|;
name|scf
operator|->
name|session_timeout
operator|=
name|NGX_CONF_UNSET
expr_stmt|;
return|return
name|scf
return|;
block|}
end_function

begin_function
specifier|static
name|char
modifier|*
DECL|function|ngx_imap_ssl_merge_conf (ngx_conf_t * cf,void * parent,void * child)
name|ngx_imap_ssl_merge_conf
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
name|ngx_imap_ssl_conf_t
modifier|*
name|prev
init|=
name|parent
decl_stmt|;
name|ngx_imap_ssl_conf_t
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
name|ngx_conf_merge_value
argument_list|(
name|conf
operator|->
name|starttls
argument_list|,
name|prev
operator|->
name|starttls
argument_list|,
name|NGX_IMAP_STARTTLS_OFF
argument_list|)
expr_stmt|;
if|if
condition|(
name|conf
operator|->
name|enable
operator|==
literal|0
operator|&&
name|conf
operator|->
name|starttls
operator|==
name|NGX_IMAP_STARTTLS_OFF
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
name|NULL
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
name|conf
operator|->
name|ciphers
operator|.
name|len
condition|)
block|{
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
name|ngx_conf_merge_value
argument_list|(
name|conf
operator|->
name|builtin_session_cache
argument_list|,
name|prev
operator|->
name|builtin_session_cache
argument_list|,
name|NGX_SSL_DFLT_BUILTIN_SCACHE
argument_list|)
expr_stmt|;
if|if
condition|(
name|conf
operator|->
name|shm_zone
operator|==
name|NULL
condition|)
block|{
name|conf
operator|->
name|shm_zone
operator|=
name|prev
operator|->
name|shm_zone
expr_stmt|;
block|}
if|if
condition|(
name|ngx_ssl_session_cache
argument_list|(
operator|&
name|conf
operator|->
name|ssl
argument_list|,
operator|&
name|ngx_imap_ssl_sess_id_ctx
argument_list|,
name|conf
operator|->
name|builtin_session_cache
argument_list|,
name|conf
operator|->
name|shm_zone
argument_list|,
name|conf
operator|->
name|session_timeout
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
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
DECL|function|ngx_imap_ssl_session_cache (ngx_conf_t * cf,ngx_command_t * cmd,void * conf)
name|ngx_imap_ssl_session_cache
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
name|ngx_imap_ssl_conf_t
modifier|*
name|scf
init|=
name|conf
decl_stmt|;
name|size_t
name|len
decl_stmt|;
name|ngx_str_t
modifier|*
name|value
decl_stmt|,
name|name
decl_stmt|,
name|size
decl_stmt|;
name|ngx_int_t
name|n
decl_stmt|;
name|ngx_uint_t
name|i
decl_stmt|,
name|j
decl_stmt|;
name|value
operator|=
name|cf
operator|->
name|args
operator|->
name|elts
expr_stmt|;
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
name|ngx_strcmp
argument_list|(
name|value
index|[
name|i
index|]
operator|.
name|data
argument_list|,
literal|"builtin"
argument_list|)
operator|==
literal|0
condition|)
block|{
name|scf
operator|->
name|builtin_session_cache
operator|=
name|NGX_SSL_DFLT_BUILTIN_SCACHE
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|value
index|[
name|i
index|]
operator|.
name|len
operator|>
sizeof|sizeof
argument_list|(
literal|"builtin:"
argument_list|)
operator|-
literal|1
operator|&&
name|ngx_strncmp
argument_list|(
name|value
index|[
name|i
index|]
operator|.
name|data
argument_list|,
literal|"builtin:"
argument_list|,
sizeof|sizeof
argument_list|(
literal|"builtin:"
argument_list|)
operator|-
literal|1
argument_list|)
operator|==
literal|0
condition|)
block|{
name|n
operator|=
name|ngx_atoi
argument_list|(
name|value
index|[
name|i
index|]
operator|.
name|data
operator|+
sizeof|sizeof
argument_list|(
literal|"builtin:"
argument_list|)
operator|-
literal|1
argument_list|,
name|value
index|[
name|i
index|]
operator|.
name|len
operator|-
operator|(
sizeof|sizeof
argument_list|(
literal|"builtin:"
argument_list|)
operator|-
literal|1
operator|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|n
operator|==
name|NGX_ERROR
condition|)
block|{
goto|goto
name|invalid
goto|;
block|}
name|scf
operator|->
name|builtin_session_cache
operator|=
name|n
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|value
index|[
name|i
index|]
operator|.
name|len
operator|>
sizeof|sizeof
argument_list|(
literal|"shared:"
argument_list|)
operator|-
literal|1
operator|&&
name|ngx_strncmp
argument_list|(
name|value
index|[
name|i
index|]
operator|.
name|data
argument_list|,
literal|"shared:"
argument_list|,
sizeof|sizeof
argument_list|(
literal|"shared:"
argument_list|)
operator|-
literal|1
argument_list|)
operator|==
literal|0
condition|)
block|{
name|len
operator|=
literal|0
expr_stmt|;
for|for
control|(
name|j
operator|=
sizeof|sizeof
argument_list|(
literal|"shared:"
argument_list|)
operator|-
literal|1
init|;
name|j
operator|<
name|value
index|[
name|i
index|]
operator|.
name|len
condition|;
name|j
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
name|data
index|[
name|j
index|]
operator|==
literal|':'
condition|)
block|{
break|break;
block|}
name|len
operator|++
expr_stmt|;
block|}
if|if
condition|(
name|len
operator|==
literal|0
condition|)
block|{
goto|goto
name|invalid
goto|;
block|}
name|name
operator|.
name|len
operator|=
name|len
expr_stmt|;
name|name
operator|.
name|data
operator|=
name|value
index|[
name|i
index|]
operator|.
name|data
operator|+
sizeof|sizeof
argument_list|(
literal|"shared:"
argument_list|)
operator|-
literal|1
expr_stmt|;
name|size
operator|.
name|len
operator|=
name|value
index|[
name|i
index|]
operator|.
name|len
operator|-
name|j
operator|-
literal|1
expr_stmt|;
name|size
operator|.
name|data
operator|=
name|name
operator|.
name|data
operator|+
name|len
operator|+
literal|1
expr_stmt|;
name|n
operator|=
name|ngx_parse_size
argument_list|(
operator|&
name|size
argument_list|)
expr_stmt|;
if|if
condition|(
name|n
operator|==
name|NGX_ERROR
condition|)
block|{
goto|goto
name|invalid
goto|;
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
literal|"session cache \"%V\" is too small"
argument_list|,
operator|&
name|value
index|[
name|i
index|]
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|scf
operator|->
name|shm_zone
operator|=
name|ngx_shared_memory_add
argument_list|(
name|cf
argument_list|,
operator|&
name|name
argument_list|,
name|n
argument_list|,
operator|&
name|ngx_imap_ssl_module
argument_list|)
expr_stmt|;
if|if
condition|(
name|scf
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
continue|continue;
block|}
goto|goto
name|invalid
goto|;
block|}
if|if
condition|(
name|scf
operator|->
name|shm_zone
operator|&&
name|scf
operator|->
name|builtin_session_cache
operator|==
name|NGX_CONF_UNSET
condition|)
block|{
name|scf
operator|->
name|builtin_session_cache
operator|=
name|NGX_SSL_NO_BUILTIN_SCACHE
expr_stmt|;
block|}
return|return
name|NGX_CONF_OK
return|;
name|invalid
label|:
name|ngx_conf_log_error
argument_list|(
name|NGX_LOG_EMERG
argument_list|,
name|cf
argument_list|,
literal|0
argument_list|,
literal|"invalid session cache \"%V\""
argument_list|,
operator|&
name|value
index|[
name|i
index|]
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
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
DECL|function|ngx_imap_ssl_nosupported (ngx_conf_t * cf,ngx_command_t * cmd,void * conf)
name|ngx_imap_ssl_nosupported
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

