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
file|<ngx_imap.h>
end_include

begin_function_decl
specifier|static
name|void
modifier|*
name|ngx_imap_core_create_main_conf
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
name|ngx_imap_core_create_srv_conf
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
name|ngx_imap_core_merge_srv_conf
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
name|ngx_imap_core_server
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
name|ngx_imap_core_listen
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
name|ngx_imap_core_capability
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
DECL|variable|ngx_imap_core_procotol
specifier|static
name|ngx_conf_enum_t
name|ngx_imap_core_procotol
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"pop3"
argument_list|)
block|,
name|NGX_IMAP_POP3_PROTOCOL
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"imap"
argument_list|)
block|,
name|NGX_IMAP_IMAP_PROTOCOL
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
DECL|variable|ngx_pop3_default_capabilities
specifier|static
name|ngx_str_t
name|ngx_pop3_default_capabilities
index|[]
init|=
block|{
name|ngx_string
argument_list|(
literal|"TOP"
argument_list|)
block|,
name|ngx_string
argument_list|(
literal|"USER"
argument_list|)
block|,
name|ngx_string
argument_list|(
literal|"UIDL"
argument_list|)
block|,
name|ngx_null_string
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_imap_default_capabilities
specifier|static
name|ngx_str_t
name|ngx_imap_default_capabilities
index|[]
init|=
block|{
name|ngx_string
argument_list|(
literal|"IMAP4"
argument_list|)
block|,
name|ngx_string
argument_list|(
literal|"IMAP4rev1"
argument_list|)
block|,
name|ngx_string
argument_list|(
literal|"UIDPLUS"
argument_list|)
block|,
name|ngx_null_string
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_imap_auth_methods
specifier|static
name|ngx_conf_bitmask_t
name|ngx_imap_auth_methods
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"plain"
argument_list|)
block|,
name|NGX_IMAP_AUTH_PLAIN_ENABLED
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"apop"
argument_list|)
block|,
name|NGX_IMAP_AUTH_APOP_ENABLED
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"cram-md5"
argument_list|)
block|,
name|NGX_IMAP_AUTH_CRAM_MD5_ENABLED
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
DECL|variable|ngx_pop3_auth_plain_capability
specifier|static
name|ngx_str_t
name|ngx_pop3_auth_plain_capability
init|=
name|ngx_string
argument_list|(
literal|"+OK methods supported:"
name|CRLF
literal|"LOGIN"
name|CRLF
literal|"PLAIN"
name|CRLF
literal|"."
name|CRLF
argument_list|)
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_pop3_auth_cram_md5_capability
specifier|static
name|ngx_str_t
name|ngx_pop3_auth_cram_md5_capability
init|=
name|ngx_string
argument_list|(
literal|"+OK methods supported:"
name|CRLF
literal|"LOGIN"
name|CRLF
literal|"PLAIN"
name|CRLF
literal|"CRAM-MD5"
name|CRLF
literal|"."
name|CRLF
argument_list|)
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_imap_core_commands
specifier|static
name|ngx_command_t
name|ngx_imap_core_commands
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"server"
argument_list|)
block|,
name|NGX_IMAP_MAIN_CONF
operator||
name|NGX_CONF_BLOCK
operator||
name|NGX_CONF_NOARGS
block|,
name|ngx_imap_core_server
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
literal|"listen"
argument_list|)
block|,
name|NGX_IMAP_SRV_CONF
operator||
name|NGX_CONF_TAKE12
block|,
name|ngx_imap_core_listen
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
literal|"protocol"
argument_list|)
block|,
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
name|ngx_imap_core_srv_conf_t
argument_list|,
name|protocol
argument_list|)
block|,
operator|&
name|ngx_imap_core_procotol
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"imap_client_buffer"
argument_list|)
block|,
name|NGX_IMAP_MAIN_CONF
operator||
name|NGX_IMAP_SRV_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_conf_set_size_slot
block|,
name|NGX_IMAP_SRV_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_imap_core_srv_conf_t
argument_list|,
name|imap_client_buffer_size
argument_list|)
block|,
name|NULL
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"so_keepalive"
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
name|ngx_imap_core_srv_conf_t
argument_list|,
name|so_keepalive
argument_list|)
block|,
name|NULL
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"timeout"
argument_list|)
block|,
name|NGX_IMAP_MAIN_CONF
operator||
name|NGX_IMAP_SRV_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_conf_set_msec_slot
block|,
name|NGX_IMAP_SRV_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_imap_core_srv_conf_t
argument_list|,
name|timeout
argument_list|)
block|,
name|NULL
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"pop3_capabilities"
argument_list|)
block|,
name|NGX_IMAP_MAIN_CONF
operator||
name|NGX_IMAP_SRV_CONF
operator||
name|NGX_CONF_1MORE
block|,
name|ngx_imap_core_capability
block|,
name|NGX_IMAP_SRV_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_imap_core_srv_conf_t
argument_list|,
name|pop3_capabilities
argument_list|)
block|,
name|NULL
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"imap_capabilities"
argument_list|)
block|,
name|NGX_IMAP_MAIN_CONF
operator||
name|NGX_IMAP_SRV_CONF
operator||
name|NGX_CONF_1MORE
block|,
name|ngx_imap_core_capability
block|,
name|NGX_IMAP_SRV_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_imap_core_srv_conf_t
argument_list|,
name|imap_capabilities
argument_list|)
block|,
name|NULL
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"server_name"
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
name|ngx_imap_core_srv_conf_t
argument_list|,
name|server_name
argument_list|)
block|,
name|NULL
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"auth"
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
name|ngx_imap_core_srv_conf_t
argument_list|,
name|auth_methods
argument_list|)
block|,
operator|&
name|ngx_imap_auth_methods
block|}
block|,
name|ngx_null_command
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_imap_core_module_ctx
specifier|static
name|ngx_imap_module_t
name|ngx_imap_core_module_ctx
init|=
block|{
name|ngx_imap_core_create_main_conf
block|,
comment|/* create main configuration */
name|NULL
block|,
comment|/* init main configuration */
name|ngx_imap_core_create_srv_conf
block|,
comment|/* create server configuration */
name|ngx_imap_core_merge_srv_conf
comment|/* merge server configuration */
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_imap_core_module
name|ngx_module_t
name|ngx_imap_core_module
init|=
block|{
name|NGX_MODULE_V1
block|,
operator|&
name|ngx_imap_core_module_ctx
block|,
comment|/* module context */
name|ngx_imap_core_commands
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

begin_function
specifier|static
name|void
modifier|*
DECL|function|ngx_imap_core_create_main_conf (ngx_conf_t * cf)
name|ngx_imap_core_create_main_conf
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
block|{
name|ngx_imap_core_main_conf_t
modifier|*
name|cmcf
decl_stmt|;
name|cmcf
operator|=
name|ngx_pcalloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_imap_core_main_conf_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|cmcf
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
if|if
condition|(
name|ngx_array_init
argument_list|(
operator|&
name|cmcf
operator|->
name|servers
argument_list|,
name|cf
operator|->
name|pool
argument_list|,
literal|4
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_imap_core_srv_conf_t
operator|*
argument_list|)
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
name|ngx_array_init
argument_list|(
operator|&
name|cmcf
operator|->
name|listen
argument_list|,
name|cf
operator|->
name|pool
argument_list|,
literal|4
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_imap_listen_t
argument_list|)
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
name|cmcf
return|;
block|}
end_function

begin_function
specifier|static
name|void
modifier|*
DECL|function|ngx_imap_core_create_srv_conf (ngx_conf_t * cf)
name|ngx_imap_core_create_srv_conf
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
block|{
name|ngx_imap_core_srv_conf_t
modifier|*
name|cscf
decl_stmt|;
name|cscf
operator|=
name|ngx_pcalloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_imap_core_srv_conf_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|cscf
operator|==
name|NULL
condition|)
block|{
return|return
name|NULL
return|;
block|}
name|cscf
operator|->
name|imap_client_buffer_size
operator|=
name|NGX_CONF_UNSET_SIZE
expr_stmt|;
name|cscf
operator|->
name|protocol
operator|=
name|NGX_CONF_UNSET_UINT
expr_stmt|;
name|cscf
operator|->
name|timeout
operator|=
name|NGX_CONF_UNSET_MSEC
expr_stmt|;
name|cscf
operator|->
name|so_keepalive
operator|=
name|NGX_CONF_UNSET
expr_stmt|;
if|if
condition|(
name|ngx_array_init
argument_list|(
operator|&
name|cscf
operator|->
name|pop3_capabilities
argument_list|,
name|cf
operator|->
name|pool
argument_list|,
literal|4
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_str_t
argument_list|)
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
return|return
name|NULL
return|;
block|}
if|if
condition|(
name|ngx_array_init
argument_list|(
operator|&
name|cscf
operator|->
name|imap_capabilities
argument_list|,
name|cf
operator|->
name|pool
argument_list|,
literal|4
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_str_t
argument_list|)
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
return|return
name|NULL
return|;
block|}
return|return
name|cscf
return|;
block|}
end_function

begin_function
specifier|static
name|char
modifier|*
DECL|function|ngx_imap_core_merge_srv_conf (ngx_conf_t * cf,void * parent,void * child)
name|ngx_imap_core_merge_srv_conf
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
name|ngx_imap_core_srv_conf_t
modifier|*
name|prev
init|=
name|parent
decl_stmt|;
name|ngx_imap_core_srv_conf_t
modifier|*
name|conf
init|=
name|child
decl_stmt|;
name|u_char
modifier|*
name|p
decl_stmt|;
name|size_t
name|size
decl_stmt|;
name|ngx_str_t
modifier|*
name|c
decl_stmt|,
modifier|*
name|d
decl_stmt|;
name|ngx_uint_t
name|i
decl_stmt|;
name|ngx_conf_merge_size_value
argument_list|(
name|conf
operator|->
name|imap_client_buffer_size
argument_list|,
name|prev
operator|->
name|imap_client_buffer_size
argument_list|,
operator|(
name|size_t
operator|)
name|ngx_pagesize
argument_list|)
expr_stmt|;
name|ngx_conf_merge_msec_value
argument_list|(
name|conf
operator|->
name|timeout
argument_list|,
name|prev
operator|->
name|timeout
argument_list|,
literal|60000
argument_list|)
expr_stmt|;
name|ngx_conf_merge_uint_value
argument_list|(
name|conf
operator|->
name|protocol
argument_list|,
name|prev
operator|->
name|protocol
argument_list|,
name|NGX_IMAP_IMAP_PROTOCOL
argument_list|)
expr_stmt|;
name|ngx_conf_merge_value
argument_list|(
name|conf
operator|->
name|so_keepalive
argument_list|,
name|prev
operator|->
name|so_keepalive
argument_list|,
literal|0
argument_list|)
expr_stmt|;
name|ngx_conf_merge_bitmask_value
argument_list|(
name|conf
operator|->
name|auth_methods
argument_list|,
name|prev
operator|->
name|auth_methods
argument_list|,
operator|(
name|NGX_CONF_BITMASK_SET
operator||
name|NGX_IMAP_AUTH_PLAIN_ENABLED
operator|)
argument_list|)
expr_stmt|;
name|ngx_conf_merge_str_value
argument_list|(
name|conf
operator|->
name|server_name
argument_list|,
name|prev
operator|->
name|server_name
argument_list|,
literal|""
argument_list|)
expr_stmt|;
if|if
condition|(
name|conf
operator|->
name|server_name
operator|.
name|len
operator|==
literal|0
condition|)
block|{
name|conf
operator|->
name|server_name
operator|.
name|data
operator|=
name|ngx_palloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
name|NGX_MAXHOSTNAMELEN
argument_list|)
expr_stmt|;
if|if
condition|(
name|conf
operator|->
name|server_name
operator|.
name|data
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
if|if
condition|(
name|gethostname
argument_list|(
operator|(
name|char
operator|*
operator|)
name|conf
operator|->
name|server_name
operator|.
name|data
argument_list|,
name|NGX_MAXHOSTNAMELEN
argument_list|)
operator|==
operator|-
literal|1
condition|)
block|{
name|ngx_conf_log_error
argument_list|(
name|NGX_LOG_EMERG
argument_list|,
name|cf
argument_list|,
name|ngx_errno
argument_list|,
literal|"gethostname() failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|conf
operator|->
name|server_name
operator|.
name|len
operator|=
name|ngx_strlen
argument_list|(
name|conf
operator|->
name|server_name
operator|.
name|data
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|conf
operator|->
name|pop3_capabilities
operator|.
name|nelts
operator|==
literal|0
condition|)
block|{
name|conf
operator|->
name|pop3_capabilities
operator|=
name|prev
operator|->
name|pop3_capabilities
expr_stmt|;
block|}
if|if
condition|(
name|conf
operator|->
name|pop3_capabilities
operator|.
name|nelts
operator|==
literal|0
condition|)
block|{
for|for
control|(
name|d
operator|=
name|ngx_pop3_default_capabilities
init|;
name|d
operator|->
name|len
condition|;
name|d
operator|++
control|)
block|{
name|c
operator|=
name|ngx_array_push
argument_list|(
operator|&
name|conf
operator|->
name|pop3_capabilities
argument_list|)
expr_stmt|;
if|if
condition|(
name|c
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
operator|*
name|c
operator|=
operator|*
name|d
expr_stmt|;
block|}
block|}
name|size
operator|=
sizeof|sizeof
argument_list|(
literal|"+OK Capability list follows"
name|CRLF
argument_list|)
operator|-
literal|1
operator|+
sizeof|sizeof
argument_list|(
literal|"."
name|CRLF
argument_list|)
operator|-
literal|1
expr_stmt|;
name|c
operator|=
name|conf
operator|->
name|pop3_capabilities
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
name|conf
operator|->
name|pop3_capabilities
operator|.
name|nelts
condition|;
name|i
operator|++
control|)
block|{
name|size
operator|+=
name|c
index|[
name|i
index|]
operator|.
name|len
operator|+
sizeof|sizeof
argument_list|(
name|CRLF
argument_list|)
operator|-
literal|1
expr_stmt|;
block|}
if|if
condition|(
name|conf
operator|->
name|auth_methods
operator|&
name|NGX_IMAP_AUTH_CRAM_MD5_ENABLED
condition|)
block|{
name|size
operator|+=
sizeof|sizeof
argument_list|(
literal|"SASL LOGIN PLAIN CRAM-MD5"
name|CRLF
argument_list|)
operator|-
literal|1
expr_stmt|;
block|}
else|else
block|{
name|size
operator|+=
sizeof|sizeof
argument_list|(
literal|"SASL LOGIN PLAIN"
name|CRLF
argument_list|)
operator|-
literal|1
expr_stmt|;
block|}
name|p
operator|=
name|ngx_palloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
name|size
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
name|NGX_CONF_ERROR
return|;
block|}
name|conf
operator|->
name|pop3_capability
operator|.
name|len
operator|=
name|size
expr_stmt|;
name|conf
operator|->
name|pop3_capability
operator|.
name|data
operator|=
name|p
expr_stmt|;
name|p
operator|=
name|ngx_cpymem
argument_list|(
name|p
argument_list|,
literal|"+OK Capability list follows"
name|CRLF
argument_list|,
sizeof|sizeof
argument_list|(
literal|"+OK Capability list follows"
name|CRLF
argument_list|)
operator|-
literal|1
argument_list|)
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|conf
operator|->
name|pop3_capabilities
operator|.
name|nelts
condition|;
name|i
operator|++
control|)
block|{
name|p
operator|=
name|ngx_cpymem
argument_list|(
name|p
argument_list|,
name|c
index|[
name|i
index|]
operator|.
name|data
argument_list|,
name|c
index|[
name|i
index|]
operator|.
name|len
argument_list|)
expr_stmt|;
operator|*
name|p
operator|++
operator|=
name|CR
expr_stmt|;
operator|*
name|p
operator|++
operator|=
name|LF
expr_stmt|;
block|}
if|if
condition|(
name|conf
operator|->
name|auth_methods
operator|&
name|NGX_IMAP_AUTH_CRAM_MD5_ENABLED
condition|)
block|{
name|p
operator|=
name|ngx_cpymem
argument_list|(
name|p
argument_list|,
literal|"SASL LOGIN PLAIN CRAM-MD5"
name|CRLF
argument_list|,
sizeof|sizeof
argument_list|(
literal|"SASL LOGIN PLAIN CRAM-MD5"
name|CRLF
argument_list|)
operator|-
literal|1
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|p
operator|=
name|ngx_cpymem
argument_list|(
name|p
argument_list|,
literal|"SASL LOGIN PLAIN"
name|CRLF
argument_list|,
sizeof|sizeof
argument_list|(
literal|"SASL LOGIN PLAIN"
name|CRLF
argument_list|)
operator|-
literal|1
argument_list|)
expr_stmt|;
block|}
operator|*
name|p
operator|++
operator|=
literal|'.'
expr_stmt|;
operator|*
name|p
operator|++
operator|=
name|CR
expr_stmt|;
operator|*
name|p
operator|=
name|LF
expr_stmt|;
name|size
operator|+=
sizeof|sizeof
argument_list|(
literal|"STLS"
name|CRLF
argument_list|)
operator|-
literal|1
expr_stmt|;
name|p
operator|=
name|ngx_palloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
name|size
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
name|NGX_CONF_ERROR
return|;
block|}
name|conf
operator|->
name|pop3_starttls_capability
operator|.
name|len
operator|=
name|size
expr_stmt|;
name|conf
operator|->
name|pop3_starttls_capability
operator|.
name|data
operator|=
name|p
expr_stmt|;
name|p
operator|=
name|ngx_cpymem
argument_list|(
name|p
argument_list|,
name|conf
operator|->
name|pop3_capability
operator|.
name|data
argument_list|,
name|conf
operator|->
name|pop3_capability
operator|.
name|len
operator|-
operator|(
sizeof|sizeof
argument_list|(
literal|"."
name|CRLF
argument_list|)
operator|-
literal|1
operator|)
argument_list|)
expr_stmt|;
name|p
operator|=
name|ngx_cpymem
argument_list|(
name|p
argument_list|,
literal|"STLS"
name|CRLF
argument_list|,
sizeof|sizeof
argument_list|(
literal|"STLS"
name|CRLF
argument_list|)
operator|-
literal|1
argument_list|)
expr_stmt|;
operator|*
name|p
operator|++
operator|=
literal|'.'
expr_stmt|;
operator|*
name|p
operator|++
operator|=
name|CR
expr_stmt|;
operator|*
name|p
operator|=
name|LF
expr_stmt|;
if|if
condition|(
name|conf
operator|->
name|auth_methods
operator|&
name|NGX_IMAP_AUTH_CRAM_MD5_ENABLED
condition|)
block|{
name|conf
operator|->
name|pop3_auth_capability
operator|=
name|ngx_pop3_auth_cram_md5_capability
expr_stmt|;
block|}
else|else
block|{
name|conf
operator|->
name|pop3_auth_capability
operator|=
name|ngx_pop3_auth_plain_capability
expr_stmt|;
block|}
if|if
condition|(
name|conf
operator|->
name|imap_capabilities
operator|.
name|nelts
operator|==
literal|0
condition|)
block|{
name|conf
operator|->
name|imap_capabilities
operator|=
name|prev
operator|->
name|imap_capabilities
expr_stmt|;
block|}
if|if
condition|(
name|conf
operator|->
name|imap_capabilities
operator|.
name|nelts
operator|==
literal|0
condition|)
block|{
for|for
control|(
name|d
operator|=
name|ngx_imap_default_capabilities
init|;
name|d
operator|->
name|len
condition|;
name|d
operator|++
control|)
block|{
name|c
operator|=
name|ngx_array_push
argument_list|(
operator|&
name|conf
operator|->
name|imap_capabilities
argument_list|)
expr_stmt|;
if|if
condition|(
name|c
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
operator|*
name|c
operator|=
operator|*
name|d
expr_stmt|;
block|}
block|}
name|size
operator|=
sizeof|sizeof
argument_list|(
literal|"* CAPABILITY"
argument_list|)
operator|-
literal|1
operator|+
sizeof|sizeof
argument_list|(
name|CRLF
argument_list|)
operator|-
literal|1
expr_stmt|;
name|c
operator|=
name|conf
operator|->
name|imap_capabilities
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
name|conf
operator|->
name|imap_capabilities
operator|.
name|nelts
condition|;
name|i
operator|++
control|)
block|{
name|size
operator|+=
literal|1
operator|+
name|c
index|[
name|i
index|]
operator|.
name|len
expr_stmt|;
block|}
name|p
operator|=
name|ngx_palloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
name|size
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
name|NGX_CONF_ERROR
return|;
block|}
name|conf
operator|->
name|imap_capability
operator|.
name|len
operator|=
name|size
expr_stmt|;
name|conf
operator|->
name|imap_capability
operator|.
name|data
operator|=
name|p
expr_stmt|;
name|p
operator|=
name|ngx_cpymem
argument_list|(
name|p
argument_list|,
literal|"* CAPABILITY"
argument_list|,
sizeof|sizeof
argument_list|(
literal|"* CAPABILITY"
argument_list|)
operator|-
literal|1
argument_list|)
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|conf
operator|->
name|imap_capabilities
operator|.
name|nelts
condition|;
name|i
operator|++
control|)
block|{
operator|*
name|p
operator|++
operator|=
literal|' '
expr_stmt|;
name|p
operator|=
name|ngx_cpymem
argument_list|(
name|p
argument_list|,
name|c
index|[
name|i
index|]
operator|.
name|data
argument_list|,
name|c
index|[
name|i
index|]
operator|.
name|len
argument_list|)
expr_stmt|;
block|}
operator|*
name|p
operator|++
operator|=
name|CR
expr_stmt|;
operator|*
name|p
operator|=
name|LF
expr_stmt|;
name|size
operator|+=
sizeof|sizeof
argument_list|(
literal|" STARTTLS"
argument_list|)
operator|-
literal|1
expr_stmt|;
name|p
operator|=
name|ngx_palloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
name|size
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
name|NGX_CONF_ERROR
return|;
block|}
name|conf
operator|->
name|imap_starttls_capability
operator|.
name|len
operator|=
name|size
expr_stmt|;
name|conf
operator|->
name|imap_starttls_capability
operator|.
name|data
operator|=
name|p
expr_stmt|;
name|p
operator|=
name|ngx_cpymem
argument_list|(
name|p
argument_list|,
name|conf
operator|->
name|imap_capability
operator|.
name|data
argument_list|,
name|conf
operator|->
name|imap_capability
operator|.
name|len
operator|-
operator|(
sizeof|sizeof
argument_list|(
name|CRLF
argument_list|)
operator|-
literal|1
operator|)
argument_list|)
expr_stmt|;
name|p
operator|=
name|ngx_cpymem
argument_list|(
name|p
argument_list|,
literal|" STARTTLS"
argument_list|,
sizeof|sizeof
argument_list|(
literal|" STARTTLS"
argument_list|)
operator|-
literal|1
argument_list|)
expr_stmt|;
operator|*
name|p
operator|++
operator|=
name|CR
expr_stmt|;
operator|*
name|p
operator|=
name|LF
expr_stmt|;
name|size
operator|+=
sizeof|sizeof
argument_list|(
literal|" LOGINDISABLED"
argument_list|)
operator|-
literal|1
expr_stmt|;
name|p
operator|=
name|ngx_palloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
name|size
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
name|NGX_CONF_ERROR
return|;
block|}
name|conf
operator|->
name|imap_starttls_only_capability
operator|.
name|len
operator|=
name|size
expr_stmt|;
name|conf
operator|->
name|imap_starttls_only_capability
operator|.
name|data
operator|=
name|p
expr_stmt|;
name|p
operator|=
name|ngx_cpymem
argument_list|(
name|p
argument_list|,
name|conf
operator|->
name|imap_starttls_capability
operator|.
name|data
argument_list|,
name|conf
operator|->
name|imap_starttls_capability
operator|.
name|len
operator|-
operator|(
sizeof|sizeof
argument_list|(
name|CRLF
argument_list|)
operator|-
literal|1
operator|)
argument_list|)
expr_stmt|;
name|p
operator|=
name|ngx_cpymem
argument_list|(
name|p
argument_list|,
literal|" LOGINDISABLED"
argument_list|,
sizeof|sizeof
argument_list|(
literal|" LOGINDISABLED"
argument_list|)
operator|-
literal|1
argument_list|)
expr_stmt|;
operator|*
name|p
operator|++
operator|=
name|CR
expr_stmt|;
operator|*
name|p
operator|=
name|LF
expr_stmt|;
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

begin_function
specifier|static
name|char
modifier|*
DECL|function|ngx_imap_core_server (ngx_conf_t * cf,ngx_command_t * cmd,void * conf)
name|ngx_imap_core_server
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
name|rv
decl_stmt|;
name|void
modifier|*
name|mconf
decl_stmt|;
name|ngx_uint_t
name|m
decl_stmt|;
name|ngx_conf_t
name|pcf
decl_stmt|;
name|ngx_imap_module_t
modifier|*
name|module
decl_stmt|;
name|ngx_imap_conf_ctx_t
modifier|*
name|ctx
decl_stmt|,
modifier|*
name|imap_ctx
decl_stmt|;
name|ngx_imap_core_srv_conf_t
modifier|*
name|cscf
decl_stmt|,
modifier|*
modifier|*
name|cscfp
decl_stmt|;
name|ngx_imap_core_main_conf_t
modifier|*
name|cmcf
decl_stmt|;
name|ctx
operator|=
name|ngx_pcalloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_imap_conf_ctx_t
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
name|NGX_CONF_ERROR
return|;
block|}
name|imap_ctx
operator|=
name|cf
operator|->
name|ctx
expr_stmt|;
name|ctx
operator|->
name|main_conf
operator|=
name|imap_ctx
operator|->
name|main_conf
expr_stmt|;
comment|/* the server{}'s srv_conf */
name|ctx
operator|->
name|srv_conf
operator|=
name|ngx_pcalloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|void
operator|*
argument_list|)
operator|*
name|ngx_imap_max_module
argument_list|)
expr_stmt|;
if|if
condition|(
name|ctx
operator|->
name|srv_conf
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
name|m
operator|=
literal|0
init|;
name|ngx_modules
index|[
name|m
index|]
condition|;
name|m
operator|++
control|)
block|{
if|if
condition|(
name|ngx_modules
index|[
name|m
index|]
operator|->
name|type
operator|!=
name|NGX_IMAP_MODULE
condition|)
block|{
continue|continue;
block|}
name|module
operator|=
name|ngx_modules
index|[
name|m
index|]
operator|->
name|ctx
expr_stmt|;
if|if
condition|(
name|module
operator|->
name|create_srv_conf
condition|)
block|{
name|mconf
operator|=
name|module
operator|->
name|create_srv_conf
argument_list|(
name|cf
argument_list|)
expr_stmt|;
if|if
condition|(
name|mconf
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|ctx
operator|->
name|srv_conf
index|[
name|ngx_modules
index|[
name|m
index|]
operator|->
name|ctx_index
index|]
operator|=
name|mconf
expr_stmt|;
block|}
block|}
comment|/* the server configuration context */
name|cscf
operator|=
name|ctx
operator|->
name|srv_conf
index|[
name|ngx_imap_core_module
operator|.
name|ctx_index
index|]
expr_stmt|;
name|cscf
operator|->
name|ctx
operator|=
name|ctx
expr_stmt|;
name|cmcf
operator|=
name|ctx
operator|->
name|main_conf
index|[
name|ngx_imap_core_module
operator|.
name|ctx_index
index|]
expr_stmt|;
name|cscfp
operator|=
name|ngx_array_push
argument_list|(
operator|&
name|cmcf
operator|->
name|servers
argument_list|)
expr_stmt|;
if|if
condition|(
name|cscfp
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
operator|*
name|cscfp
operator|=
name|cscf
expr_stmt|;
comment|/* parse inside server{} */
name|pcf
operator|=
operator|*
name|cf
expr_stmt|;
name|cf
operator|->
name|ctx
operator|=
name|ctx
expr_stmt|;
name|cf
operator|->
name|cmd_type
operator|=
name|NGX_IMAP_SRV_CONF
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
name|pcf
expr_stmt|;
return|return
name|rv
return|;
block|}
end_function

begin_comment
comment|/* AF_INET only */
end_comment

begin_function
specifier|static
name|char
modifier|*
DECL|function|ngx_imap_core_listen (ngx_conf_t * cf,ngx_command_t * cmd,void * conf)
name|ngx_imap_core_listen
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
name|ngx_str_t
modifier|*
name|value
decl_stmt|;
name|ngx_url_t
name|u
decl_stmt|;
name|ngx_uint_t
name|i
decl_stmt|;
name|ngx_imap_listen_t
modifier|*
name|imls
decl_stmt|;
name|ngx_imap_core_main_conf_t
modifier|*
name|cmcf
decl_stmt|;
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
name|listen
operator|=
literal|1
expr_stmt|;
if|if
condition|(
name|ngx_parse_url
argument_list|(
name|cf
argument_list|,
operator|&
name|u
argument_list|)
operator|!=
name|NGX_OK
condition|)
block|{
if|if
condition|(
name|u
operator|.
name|err
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
literal|"%s in \"%V\" of the \"listen\" directive"
argument_list|,
name|u
operator|.
name|err
argument_list|,
operator|&
name|u
operator|.
name|url
argument_list|)
expr_stmt|;
block|}
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|cmcf
operator|=
name|ngx_imap_conf_get_module_main_conf
argument_list|(
name|cf
argument_list|,
name|ngx_imap_core_module
argument_list|)
expr_stmt|;
name|imls
operator|=
name|cmcf
operator|->
name|listen
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
name|cmcf
operator|->
name|listen
operator|.
name|nelts
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|imls
index|[
name|i
index|]
operator|.
name|addr
operator|!=
name|u
operator|.
name|addr
operator|.
name|in_addr
operator|||
name|imls
index|[
name|i
index|]
operator|.
name|port
operator|!=
name|u
operator|.
name|port
condition|)
block|{
continue|continue;
block|}
name|ngx_conf_log_error
argument_list|(
name|NGX_LOG_EMERG
argument_list|,
name|cf
argument_list|,
literal|0
argument_list|,
literal|"duplicate \"%V\" address and port pair"
argument_list|,
operator|&
name|u
operator|.
name|url
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|imls
operator|=
name|ngx_array_push
argument_list|(
operator|&
name|cmcf
operator|->
name|listen
argument_list|)
expr_stmt|;
if|if
condition|(
name|imls
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|ngx_memzero
argument_list|(
name|imls
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_imap_listen_t
argument_list|)
argument_list|)
expr_stmt|;
name|imls
operator|->
name|addr
operator|=
name|u
operator|.
name|addr
operator|.
name|in_addr
expr_stmt|;
name|imls
operator|->
name|port
operator|=
name|u
operator|.
name|port
expr_stmt|;
name|imls
operator|->
name|family
operator|=
name|AF_INET
expr_stmt|;
name|imls
operator|->
name|ctx
operator|=
name|cf
operator|->
name|ctx
expr_stmt|;
if|if
condition|(
name|cf
operator|->
name|args
operator|->
name|nelts
operator|==
literal|2
condition|)
block|{
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
literal|2
index|]
operator|.
name|data
argument_list|,
literal|"bind"
argument_list|)
operator|==
literal|0
condition|)
block|{
name|imls
operator|->
name|bind
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
literal|"the invalid \"%V\" parameter"
argument_list|,
operator|&
name|value
index|[
literal|2
index|]
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
DECL|function|ngx_imap_core_capability (ngx_conf_t * cf,ngx_command_t * cmd,void * conf)
name|ngx_imap_core_capability
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
name|ngx_str_t
modifier|*
name|c
decl_stmt|,
modifier|*
name|value
decl_stmt|;
name|ngx_uint_t
name|i
decl_stmt|;
name|ngx_array_t
modifier|*
name|a
decl_stmt|;
name|a
operator|=
operator|(
name|ngx_array_t
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
name|c
operator|=
name|ngx_array_push
argument_list|(
name|a
argument_list|)
expr_stmt|;
if|if
condition|(
name|c
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
operator|*
name|c
operator|=
name|value
index|[
name|i
index|]
expr_stmt|;
block|}
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

end_unit

