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
literal|"ssl_ciphers"
argument_list|)
block|,
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
name|ngx_null_command
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_ssl_module_ctx
specifier|static
name|ngx_http_module_t
name|ngx_http_ssl_module_ctx
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
name|ngx_http_ssl_srv_conf_t
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
comment|/*      * set by ngx_pcalloc():      *      *     scf->certificate.len = 0;      *     scf->certificate.data = NULL;      *     scf->certificate_key.len = 0;      *     scf->certificate_key.data = NULL;      *     scf->ciphers.len = 0;      *     scf->ciphers.data = NULL;      */
name|scf
operator|->
name|enable
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
literal|""
argument_list|)
expr_stmt|;
comment|/* TODO: configure methods */
name|conf
operator|->
name|ssl_ctx
operator|=
name|SSL_CTX_new
argument_list|(
name|SSLv23_server_method
argument_list|()
argument_list|)
expr_stmt|;
if|if
condition|(
name|conf
operator|->
name|ssl_ctx
operator|==
name|NULL
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
literal|"SSL_CTX_new() failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
if|if
condition|(
name|ngx_pool_cleanup_add
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
name|ngx_ssl_cleanup_ctx
argument_list|,
name|conf
operator|->
name|ssl_ctx
argument_list|)
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
if|#
directive|if
literal|0
block_content|SSL_CTX_set_options(conf->ssl_ctx, SSL_OP_ALL);     SSL_CTX_set_options(conf->ssl_ctx, SSL_OP_NO_SSLv3);     SSL_CTX_set_options(conf->ssl_ctx, SSL_OP_SINGLE_DH_USE);
endif|#
directive|endif
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
name|ssl_ctx
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
if|if
condition|(
name|SSL_CTX_use_certificate_chain_file
argument_list|(
name|conf
operator|->
name|ssl_ctx
argument_list|,
operator|(
name|char
operator|*
operator|)
name|conf
operator|->
name|certificate
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
literal|"SSL_CTX_use_certificate_chain_file(\"%s\") failed"
argument_list|,
name|conf
operator|->
name|certificate
operator|.
name|data
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
if|if
condition|(
name|SSL_CTX_use_PrivateKey_file
argument_list|(
name|conf
operator|->
name|ssl_ctx
argument_list|,
operator|(
name|char
operator|*
operator|)
name|conf
operator|->
name|certificate_key
operator|.
name|data
argument_list|,
name|SSL_FILETYPE_PEM
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
literal|"SSL_CTX_use_PrivateKey_file(\"%s\") failed"
argument_list|,
name|conf
operator|->
name|certificate_key
operator|.
name|data
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|SSL_CTX_set_verify
argument_list|(
name|conf
operator|->
name|ssl_ctx
argument_list|,
name|SSL_VERIFY_NONE
argument_list|,
name|NULL
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
literal|0
end_if

begin_comment
comment|/* how to enumrate server' configs */
end_comment

begin_endif
unit|static ngx_int_t ngx_http_ssl_init_process(ngx_cycle_t *cycle) {     ngx_uint_t                   i;     ngx_http_ssl_srv_conf_t     *sscf;     ngx_http_core_srv_conf_t   **cscfp;     ngx_http_core_main_conf_t   *cmcf;      cmcf = ngx_http_cycle_get_module_main_conf(cycle, ngx_http_core_module);      cscfp = cmcf->servers.elts;      for (i = 0; i< cmcf->servers.nelts; i++) {         sscf = cscfp[i]->ctx->srv_conf[ngx_http_ssl_module.ctx_index];          if (sscf->enable) {             cscfp[i]->recv = ngx_ssl_recv;             cscfp[i]->send_chain = ngx_ssl_send_chain;         }     }      return NGX_OK; }
endif|#
directive|endif
end_endif

end_unit

