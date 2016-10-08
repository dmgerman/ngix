begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (C) Igor Sysoev  * Copyright (C) Nginx, Inc.  */
end_comment

begin_ifndef
ifndef|#
directive|ifndef
name|_NGX_MAIL_H_INCLUDED_
end_ifndef

begin_define
DECL|macro|_NGX_MAIL_H_INCLUDED_
define|#
directive|define
name|_NGX_MAIL_H_INCLUDED_
end_define

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
file|<ngx_event_connect.h>
end_include

begin_if
if|#
directive|if
operator|(
name|NGX_MAIL_SSL
operator|)
end_if

begin_include
include|#
directive|include
file|<ngx_mail_ssl_module.h>
end_include

begin_endif
endif|#
directive|endif
end_endif

begin_typedef
DECL|struct|__anon28eadea90108
typedef|typedef
struct|struct
block|{
DECL|member|main_conf
name|void
modifier|*
modifier|*
name|main_conf
decl_stmt|;
DECL|member|srv_conf
name|void
modifier|*
modifier|*
name|srv_conf
decl_stmt|;
DECL|typedef|ngx_mail_conf_ctx_t
block|}
name|ngx_mail_conf_ctx_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon28eadea90208
typedef|typedef
struct|struct
block|{
DECL|member|sockaddr
name|ngx_sockaddr_t
name|sockaddr
decl_stmt|;
DECL|member|socklen
name|socklen_t
name|socklen
decl_stmt|;
comment|/* server ctx */
DECL|member|ctx
name|ngx_mail_conf_ctx_t
modifier|*
name|ctx
decl_stmt|;
DECL|member|bind
name|unsigned
name|bind
range|:
literal|1
decl_stmt|;
DECL|member|wildcard
name|unsigned
name|wildcard
range|:
literal|1
decl_stmt|;
DECL|member|ssl
name|unsigned
name|ssl
range|:
literal|1
decl_stmt|;
if|#
directive|if
operator|(
name|NGX_HAVE_INET6
operator|)
DECL|member|ipv6only
name|unsigned
name|ipv6only
range|:
literal|1
decl_stmt|;
endif|#
directive|endif
DECL|member|so_keepalive
name|unsigned
name|so_keepalive
range|:
literal|2
decl_stmt|;
if|#
directive|if
operator|(
name|NGX_HAVE_KEEPALIVE_TUNABLE
operator|)
DECL|member|tcp_keepidle
name|int
name|tcp_keepidle
decl_stmt|;
DECL|member|tcp_keepintvl
name|int
name|tcp_keepintvl
decl_stmt|;
DECL|member|tcp_keepcnt
name|int
name|tcp_keepcnt
decl_stmt|;
endif|#
directive|endif
DECL|member|backlog
name|int
name|backlog
decl_stmt|;
DECL|typedef|ngx_mail_listen_t
block|}
name|ngx_mail_listen_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon28eadea90308
typedef|typedef
struct|struct
block|{
DECL|member|ctx
name|ngx_mail_conf_ctx_t
modifier|*
name|ctx
decl_stmt|;
DECL|member|addr_text
name|ngx_str_t
name|addr_text
decl_stmt|;
DECL|member|ssl
name|ngx_uint_t
name|ssl
decl_stmt|;
comment|/* unsigned   ssl:1; */
DECL|typedef|ngx_mail_addr_conf_t
block|}
name|ngx_mail_addr_conf_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon28eadea90408
typedef|typedef
struct|struct
block|{
DECL|member|addr
name|in_addr_t
name|addr
decl_stmt|;
DECL|member|conf
name|ngx_mail_addr_conf_t
name|conf
decl_stmt|;
DECL|typedef|ngx_mail_in_addr_t
block|}
name|ngx_mail_in_addr_t
typedef|;
end_typedef

begin_if
if|#
directive|if
operator|(
name|NGX_HAVE_INET6
operator|)
end_if

begin_typedef
DECL|struct|__anon28eadea90508
typedef|typedef
struct|struct
block|{
DECL|member|addr6
name|struct
name|in6_addr
name|addr6
decl_stmt|;
DECL|member|conf
name|ngx_mail_addr_conf_t
name|conf
decl_stmt|;
DECL|typedef|ngx_mail_in6_addr_t
block|}
name|ngx_mail_in6_addr_t
typedef|;
end_typedef

begin_endif
endif|#
directive|endif
end_endif

begin_typedef
DECL|struct|__anon28eadea90608
typedef|typedef
struct|struct
block|{
comment|/* ngx_mail_in_addr_t or ngx_mail_in6_addr_t */
DECL|member|addrs
name|void
modifier|*
name|addrs
decl_stmt|;
DECL|member|naddrs
name|ngx_uint_t
name|naddrs
decl_stmt|;
DECL|typedef|ngx_mail_port_t
block|}
name|ngx_mail_port_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon28eadea90708
typedef|typedef
struct|struct
block|{
DECL|member|family
name|int
name|family
decl_stmt|;
DECL|member|port
name|in_port_t
name|port
decl_stmt|;
DECL|member|addrs
name|ngx_array_t
name|addrs
decl_stmt|;
comment|/* array of ngx_mail_conf_addr_t */
DECL|typedef|ngx_mail_conf_port_t
block|}
name|ngx_mail_conf_port_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon28eadea90808
typedef|typedef
struct|struct
block|{
DECL|member|opt
name|ngx_mail_listen_t
name|opt
decl_stmt|;
DECL|typedef|ngx_mail_conf_addr_t
block|}
name|ngx_mail_conf_addr_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon28eadea90908
typedef|typedef
struct|struct
block|{
DECL|member|servers
name|ngx_array_t
name|servers
decl_stmt|;
comment|/* ngx_mail_core_srv_conf_t */
DECL|member|listen
name|ngx_array_t
name|listen
decl_stmt|;
comment|/* ngx_mail_listen_t */
DECL|typedef|ngx_mail_core_main_conf_t
block|}
name|ngx_mail_core_main_conf_t
typedef|;
end_typedef

begin_define
DECL|macro|NGX_MAIL_POP3_PROTOCOL
define|#
directive|define
name|NGX_MAIL_POP3_PROTOCOL
value|0
end_define

begin_define
DECL|macro|NGX_MAIL_IMAP_PROTOCOL
define|#
directive|define
name|NGX_MAIL_IMAP_PROTOCOL
value|1
end_define

begin_define
DECL|macro|NGX_MAIL_SMTP_PROTOCOL
define|#
directive|define
name|NGX_MAIL_SMTP_PROTOCOL
value|2
end_define

begin_typedef
DECL|typedef|ngx_mail_protocol_t
typedef|typedef
name|struct
name|ngx_mail_protocol_s
name|ngx_mail_protocol_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon28eadea90a08
typedef|typedef
struct|struct
block|{
DECL|member|protocol
name|ngx_mail_protocol_t
modifier|*
name|protocol
decl_stmt|;
DECL|member|timeout
name|ngx_msec_t
name|timeout
decl_stmt|;
DECL|member|resolver_timeout
name|ngx_msec_t
name|resolver_timeout
decl_stmt|;
DECL|member|server_name
name|ngx_str_t
name|server_name
decl_stmt|;
DECL|member|file_name
name|u_char
modifier|*
name|file_name
decl_stmt|;
DECL|member|line
name|ngx_uint_t
name|line
decl_stmt|;
DECL|member|resolver
name|ngx_resolver_t
modifier|*
name|resolver
decl_stmt|;
DECL|member|error_log
name|ngx_log_t
modifier|*
name|error_log
decl_stmt|;
comment|/* server ctx */
DECL|member|ctx
name|ngx_mail_conf_ctx_t
modifier|*
name|ctx
decl_stmt|;
DECL|member|listen
name|ngx_uint_t
name|listen
decl_stmt|;
comment|/* unsigned  listen:1; */
DECL|typedef|ngx_mail_core_srv_conf_t
block|}
name|ngx_mail_core_srv_conf_t
typedef|;
end_typedef

begin_typedef
DECL|enum|__anon28eadea90b03
typedef|typedef
enum|enum
block|{
DECL|enumerator|ngx_pop3_start
name|ngx_pop3_start
init|=
literal|0
block|,
DECL|enumerator|ngx_pop3_user
name|ngx_pop3_user
block|,
DECL|enumerator|ngx_pop3_passwd
name|ngx_pop3_passwd
block|,
DECL|enumerator|ngx_pop3_auth_login_username
name|ngx_pop3_auth_login_username
block|,
DECL|enumerator|ngx_pop3_auth_login_password
name|ngx_pop3_auth_login_password
block|,
DECL|enumerator|ngx_pop3_auth_plain
name|ngx_pop3_auth_plain
block|,
DECL|enumerator|ngx_pop3_auth_cram_md5
name|ngx_pop3_auth_cram_md5
block|,
DECL|enumerator|ngx_pop3_auth_external
name|ngx_pop3_auth_external
DECL|typedef|ngx_pop3_state_e
block|}
name|ngx_pop3_state_e
typedef|;
end_typedef

begin_typedef
DECL|enum|__anon28eadea90c03
typedef|typedef
enum|enum
block|{
DECL|enumerator|ngx_imap_start
name|ngx_imap_start
init|=
literal|0
block|,
DECL|enumerator|ngx_imap_auth_login_username
name|ngx_imap_auth_login_username
block|,
DECL|enumerator|ngx_imap_auth_login_password
name|ngx_imap_auth_login_password
block|,
DECL|enumerator|ngx_imap_auth_plain
name|ngx_imap_auth_plain
block|,
DECL|enumerator|ngx_imap_auth_cram_md5
name|ngx_imap_auth_cram_md5
block|,
DECL|enumerator|ngx_imap_auth_external
name|ngx_imap_auth_external
block|,
DECL|enumerator|ngx_imap_login
name|ngx_imap_login
block|,
DECL|enumerator|ngx_imap_user
name|ngx_imap_user
block|,
DECL|enumerator|ngx_imap_passwd
name|ngx_imap_passwd
DECL|typedef|ngx_imap_state_e
block|}
name|ngx_imap_state_e
typedef|;
end_typedef

begin_typedef
DECL|enum|__anon28eadea90d03
typedef|typedef
enum|enum
block|{
DECL|enumerator|ngx_smtp_start
name|ngx_smtp_start
init|=
literal|0
block|,
DECL|enumerator|ngx_smtp_auth_login_username
name|ngx_smtp_auth_login_username
block|,
DECL|enumerator|ngx_smtp_auth_login_password
name|ngx_smtp_auth_login_password
block|,
DECL|enumerator|ngx_smtp_auth_plain
name|ngx_smtp_auth_plain
block|,
DECL|enumerator|ngx_smtp_auth_cram_md5
name|ngx_smtp_auth_cram_md5
block|,
DECL|enumerator|ngx_smtp_auth_external
name|ngx_smtp_auth_external
block|,
DECL|enumerator|ngx_smtp_helo
name|ngx_smtp_helo
block|,
DECL|enumerator|ngx_smtp_helo_xclient
name|ngx_smtp_helo_xclient
block|,
DECL|enumerator|ngx_smtp_helo_from
name|ngx_smtp_helo_from
block|,
DECL|enumerator|ngx_smtp_xclient
name|ngx_smtp_xclient
block|,
DECL|enumerator|ngx_smtp_xclient_from
name|ngx_smtp_xclient_from
block|,
DECL|enumerator|ngx_smtp_xclient_helo
name|ngx_smtp_xclient_helo
block|,
DECL|enumerator|ngx_smtp_from
name|ngx_smtp_from
block|,
DECL|enumerator|ngx_smtp_to
name|ngx_smtp_to
DECL|typedef|ngx_smtp_state_e
block|}
name|ngx_smtp_state_e
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon28eadea90e08
typedef|typedef
struct|struct
block|{
DECL|member|upstream
name|ngx_peer_connection_t
name|upstream
decl_stmt|;
DECL|member|buffer
name|ngx_buf_t
modifier|*
name|buffer
decl_stmt|;
DECL|typedef|ngx_mail_proxy_ctx_t
block|}
name|ngx_mail_proxy_ctx_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon28eadea90f08
typedef|typedef
struct|struct
block|{
DECL|member|signature
name|uint32_t
name|signature
decl_stmt|;
comment|/* "MAIL" */
DECL|member|connection
name|ngx_connection_t
modifier|*
name|connection
decl_stmt|;
DECL|member|out
name|ngx_str_t
name|out
decl_stmt|;
DECL|member|buffer
name|ngx_buf_t
modifier|*
name|buffer
decl_stmt|;
DECL|member|ctx
name|void
modifier|*
modifier|*
name|ctx
decl_stmt|;
DECL|member|main_conf
name|void
modifier|*
modifier|*
name|main_conf
decl_stmt|;
DECL|member|srv_conf
name|void
modifier|*
modifier|*
name|srv_conf
decl_stmt|;
DECL|member|resolver_ctx
name|ngx_resolver_ctx_t
modifier|*
name|resolver_ctx
decl_stmt|;
DECL|member|proxy
name|ngx_mail_proxy_ctx_t
modifier|*
name|proxy
decl_stmt|;
DECL|member|mail_state
name|ngx_uint_t
name|mail_state
decl_stmt|;
DECL|member|protocol
name|unsigned
name|protocol
range|:
literal|3
decl_stmt|;
DECL|member|blocked
name|unsigned
name|blocked
range|:
literal|1
decl_stmt|;
DECL|member|quit
name|unsigned
name|quit
range|:
literal|1
decl_stmt|;
DECL|member|quoted
name|unsigned
name|quoted
range|:
literal|1
decl_stmt|;
DECL|member|backslash
name|unsigned
name|backslash
range|:
literal|1
decl_stmt|;
DECL|member|no_sync_literal
name|unsigned
name|no_sync_literal
range|:
literal|1
decl_stmt|;
DECL|member|starttls
name|unsigned
name|starttls
range|:
literal|1
decl_stmt|;
DECL|member|esmtp
name|unsigned
name|esmtp
range|:
literal|1
decl_stmt|;
DECL|member|auth_method
name|unsigned
name|auth_method
range|:
literal|3
decl_stmt|;
DECL|member|auth_wait
name|unsigned
name|auth_wait
range|:
literal|1
decl_stmt|;
DECL|member|login
name|ngx_str_t
name|login
decl_stmt|;
DECL|member|passwd
name|ngx_str_t
name|passwd
decl_stmt|;
DECL|member|salt
name|ngx_str_t
name|salt
decl_stmt|;
DECL|member|tag
name|ngx_str_t
name|tag
decl_stmt|;
DECL|member|tagged_line
name|ngx_str_t
name|tagged_line
decl_stmt|;
DECL|member|text
name|ngx_str_t
name|text
decl_stmt|;
DECL|member|addr_text
name|ngx_str_t
modifier|*
name|addr_text
decl_stmt|;
DECL|member|host
name|ngx_str_t
name|host
decl_stmt|;
DECL|member|smtp_helo
name|ngx_str_t
name|smtp_helo
decl_stmt|;
DECL|member|smtp_from
name|ngx_str_t
name|smtp_from
decl_stmt|;
DECL|member|smtp_to
name|ngx_str_t
name|smtp_to
decl_stmt|;
DECL|member|cmd
name|ngx_str_t
name|cmd
decl_stmt|;
DECL|member|command
name|ngx_uint_t
name|command
decl_stmt|;
DECL|member|args
name|ngx_array_t
name|args
decl_stmt|;
DECL|member|login_attempt
name|ngx_uint_t
name|login_attempt
decl_stmt|;
comment|/* used to parse POP3/IMAP/SMTP command */
DECL|member|state
name|ngx_uint_t
name|state
decl_stmt|;
DECL|member|cmd_start
name|u_char
modifier|*
name|cmd_start
decl_stmt|;
DECL|member|arg_start
name|u_char
modifier|*
name|arg_start
decl_stmt|;
DECL|member|arg_end
name|u_char
modifier|*
name|arg_end
decl_stmt|;
DECL|member|literal_len
name|ngx_uint_t
name|literal_len
decl_stmt|;
DECL|typedef|ngx_mail_session_t
block|}
name|ngx_mail_session_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon28eadea91008
typedef|typedef
struct|struct
block|{
DECL|member|client
name|ngx_str_t
modifier|*
name|client
decl_stmt|;
DECL|member|session
name|ngx_mail_session_t
modifier|*
name|session
decl_stmt|;
DECL|typedef|ngx_mail_log_ctx_t
block|}
name|ngx_mail_log_ctx_t
typedef|;
end_typedef

begin_define
DECL|macro|NGX_POP3_USER
define|#
directive|define
name|NGX_POP3_USER
value|1
end_define

begin_define
DECL|macro|NGX_POP3_PASS
define|#
directive|define
name|NGX_POP3_PASS
value|2
end_define

begin_define
DECL|macro|NGX_POP3_CAPA
define|#
directive|define
name|NGX_POP3_CAPA
value|3
end_define

begin_define
DECL|macro|NGX_POP3_QUIT
define|#
directive|define
name|NGX_POP3_QUIT
value|4
end_define

begin_define
DECL|macro|NGX_POP3_NOOP
define|#
directive|define
name|NGX_POP3_NOOP
value|5
end_define

begin_define
DECL|macro|NGX_POP3_STLS
define|#
directive|define
name|NGX_POP3_STLS
value|6
end_define

begin_define
DECL|macro|NGX_POP3_APOP
define|#
directive|define
name|NGX_POP3_APOP
value|7
end_define

begin_define
DECL|macro|NGX_POP3_AUTH
define|#
directive|define
name|NGX_POP3_AUTH
value|8
end_define

begin_define
DECL|macro|NGX_POP3_STAT
define|#
directive|define
name|NGX_POP3_STAT
value|9
end_define

begin_define
DECL|macro|NGX_POP3_LIST
define|#
directive|define
name|NGX_POP3_LIST
value|10
end_define

begin_define
DECL|macro|NGX_POP3_RETR
define|#
directive|define
name|NGX_POP3_RETR
value|11
end_define

begin_define
DECL|macro|NGX_POP3_DELE
define|#
directive|define
name|NGX_POP3_DELE
value|12
end_define

begin_define
DECL|macro|NGX_POP3_RSET
define|#
directive|define
name|NGX_POP3_RSET
value|13
end_define

begin_define
DECL|macro|NGX_POP3_TOP
define|#
directive|define
name|NGX_POP3_TOP
value|14
end_define

begin_define
DECL|macro|NGX_POP3_UIDL
define|#
directive|define
name|NGX_POP3_UIDL
value|15
end_define

begin_define
DECL|macro|NGX_IMAP_LOGIN
define|#
directive|define
name|NGX_IMAP_LOGIN
value|1
end_define

begin_define
DECL|macro|NGX_IMAP_LOGOUT
define|#
directive|define
name|NGX_IMAP_LOGOUT
value|2
end_define

begin_define
DECL|macro|NGX_IMAP_CAPABILITY
define|#
directive|define
name|NGX_IMAP_CAPABILITY
value|3
end_define

begin_define
DECL|macro|NGX_IMAP_NOOP
define|#
directive|define
name|NGX_IMAP_NOOP
value|4
end_define

begin_define
DECL|macro|NGX_IMAP_STARTTLS
define|#
directive|define
name|NGX_IMAP_STARTTLS
value|5
end_define

begin_define
DECL|macro|NGX_IMAP_NEXT
define|#
directive|define
name|NGX_IMAP_NEXT
value|6
end_define

begin_define
DECL|macro|NGX_IMAP_AUTHENTICATE
define|#
directive|define
name|NGX_IMAP_AUTHENTICATE
value|7
end_define

begin_define
DECL|macro|NGX_SMTP_HELO
define|#
directive|define
name|NGX_SMTP_HELO
value|1
end_define

begin_define
DECL|macro|NGX_SMTP_EHLO
define|#
directive|define
name|NGX_SMTP_EHLO
value|2
end_define

begin_define
DECL|macro|NGX_SMTP_AUTH
define|#
directive|define
name|NGX_SMTP_AUTH
value|3
end_define

begin_define
DECL|macro|NGX_SMTP_QUIT
define|#
directive|define
name|NGX_SMTP_QUIT
value|4
end_define

begin_define
DECL|macro|NGX_SMTP_NOOP
define|#
directive|define
name|NGX_SMTP_NOOP
value|5
end_define

begin_define
DECL|macro|NGX_SMTP_MAIL
define|#
directive|define
name|NGX_SMTP_MAIL
value|6
end_define

begin_define
DECL|macro|NGX_SMTP_RSET
define|#
directive|define
name|NGX_SMTP_RSET
value|7
end_define

begin_define
DECL|macro|NGX_SMTP_RCPT
define|#
directive|define
name|NGX_SMTP_RCPT
value|8
end_define

begin_define
DECL|macro|NGX_SMTP_DATA
define|#
directive|define
name|NGX_SMTP_DATA
value|9
end_define

begin_define
DECL|macro|NGX_SMTP_VRFY
define|#
directive|define
name|NGX_SMTP_VRFY
value|10
end_define

begin_define
DECL|macro|NGX_SMTP_EXPN
define|#
directive|define
name|NGX_SMTP_EXPN
value|11
end_define

begin_define
DECL|macro|NGX_SMTP_HELP
define|#
directive|define
name|NGX_SMTP_HELP
value|12
end_define

begin_define
DECL|macro|NGX_SMTP_STARTTLS
define|#
directive|define
name|NGX_SMTP_STARTTLS
value|13
end_define

begin_define
DECL|macro|NGX_MAIL_AUTH_PLAIN
define|#
directive|define
name|NGX_MAIL_AUTH_PLAIN
value|0
end_define

begin_define
DECL|macro|NGX_MAIL_AUTH_LOGIN
define|#
directive|define
name|NGX_MAIL_AUTH_LOGIN
value|1
end_define

begin_define
DECL|macro|NGX_MAIL_AUTH_LOGIN_USERNAME
define|#
directive|define
name|NGX_MAIL_AUTH_LOGIN_USERNAME
value|2
end_define

begin_define
DECL|macro|NGX_MAIL_AUTH_APOP
define|#
directive|define
name|NGX_MAIL_AUTH_APOP
value|3
end_define

begin_define
DECL|macro|NGX_MAIL_AUTH_CRAM_MD5
define|#
directive|define
name|NGX_MAIL_AUTH_CRAM_MD5
value|4
end_define

begin_define
DECL|macro|NGX_MAIL_AUTH_EXTERNAL
define|#
directive|define
name|NGX_MAIL_AUTH_EXTERNAL
value|5
end_define

begin_define
DECL|macro|NGX_MAIL_AUTH_NONE
define|#
directive|define
name|NGX_MAIL_AUTH_NONE
value|6
end_define

begin_define
DECL|macro|NGX_MAIL_AUTH_PLAIN_ENABLED
define|#
directive|define
name|NGX_MAIL_AUTH_PLAIN_ENABLED
value|0x0002
end_define

begin_define
DECL|macro|NGX_MAIL_AUTH_LOGIN_ENABLED
define|#
directive|define
name|NGX_MAIL_AUTH_LOGIN_ENABLED
value|0x0004
end_define

begin_define
DECL|macro|NGX_MAIL_AUTH_APOP_ENABLED
define|#
directive|define
name|NGX_MAIL_AUTH_APOP_ENABLED
value|0x0008
end_define

begin_define
DECL|macro|NGX_MAIL_AUTH_CRAM_MD5_ENABLED
define|#
directive|define
name|NGX_MAIL_AUTH_CRAM_MD5_ENABLED
value|0x0010
end_define

begin_define
DECL|macro|NGX_MAIL_AUTH_EXTERNAL_ENABLED
define|#
directive|define
name|NGX_MAIL_AUTH_EXTERNAL_ENABLED
value|0x0020
end_define

begin_define
DECL|macro|NGX_MAIL_AUTH_NONE_ENABLED
define|#
directive|define
name|NGX_MAIL_AUTH_NONE_ENABLED
value|0x0040
end_define

begin_define
DECL|macro|NGX_MAIL_PARSE_INVALID_COMMAND
define|#
directive|define
name|NGX_MAIL_PARSE_INVALID_COMMAND
value|20
end_define

begin_typedef
DECL|typedef|ngx_mail_init_session_pt
typedef|typedef
name|void
function_decl|(
modifier|*
name|ngx_mail_init_session_pt
function_decl|)
parameter_list|(
name|ngx_mail_session_t
modifier|*
name|s
parameter_list|,
name|ngx_connection_t
modifier|*
name|c
parameter_list|)
function_decl|;
end_typedef

begin_typedef
DECL|typedef|ngx_mail_init_protocol_pt
typedef|typedef
name|void
function_decl|(
modifier|*
name|ngx_mail_init_protocol_pt
function_decl|)
parameter_list|(
name|ngx_event_t
modifier|*
name|rev
parameter_list|)
function_decl|;
end_typedef

begin_typedef
DECL|typedef|ngx_mail_auth_state_pt
typedef|typedef
name|void
function_decl|(
modifier|*
name|ngx_mail_auth_state_pt
function_decl|)
parameter_list|(
name|ngx_event_t
modifier|*
name|rev
parameter_list|)
function_decl|;
end_typedef

begin_typedef
DECL|typedef|ngx_mail_parse_command_pt
typedef|typedef
name|ngx_int_t
function_decl|(
modifier|*
name|ngx_mail_parse_command_pt
function_decl|)
parameter_list|(
name|ngx_mail_session_t
modifier|*
name|s
parameter_list|)
function_decl|;
end_typedef

begin_struct
DECL|struct|ngx_mail_protocol_s
struct|struct
name|ngx_mail_protocol_s
block|{
DECL|member|name
name|ngx_str_t
name|name
decl_stmt|;
DECL|member|port
name|in_port_t
name|port
index|[
literal|4
index|]
decl_stmt|;
DECL|member|type
name|ngx_uint_t
name|type
decl_stmt|;
DECL|member|init_session
name|ngx_mail_init_session_pt
name|init_session
decl_stmt|;
DECL|member|init_protocol
name|ngx_mail_init_protocol_pt
name|init_protocol
decl_stmt|;
DECL|member|parse_command
name|ngx_mail_parse_command_pt
name|parse_command
decl_stmt|;
DECL|member|auth_state
name|ngx_mail_auth_state_pt
name|auth_state
decl_stmt|;
DECL|member|internal_server_error
name|ngx_str_t
name|internal_server_error
decl_stmt|;
DECL|member|cert_error
name|ngx_str_t
name|cert_error
decl_stmt|;
DECL|member|no_cert
name|ngx_str_t
name|no_cert
decl_stmt|;
block|}
struct|;
end_struct

begin_typedef
DECL|struct|__anon28eadea91108
typedef|typedef
struct|struct
block|{
DECL|member|protocol
name|ngx_mail_protocol_t
modifier|*
name|protocol
decl_stmt|;
DECL|member|create_main_conf
name|void
modifier|*
function_decl|(
modifier|*
name|create_main_conf
function_decl|)
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
function_decl|;
DECL|member|init_main_conf
name|char
modifier|*
function_decl|(
modifier|*
name|init_main_conf
function_decl|)
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
DECL|member|create_srv_conf
name|void
modifier|*
function_decl|(
modifier|*
name|create_srv_conf
function_decl|)
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
function_decl|;
DECL|member|merge_srv_conf
name|char
modifier|*
function_decl|(
modifier|*
name|merge_srv_conf
function_decl|)
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|void
modifier|*
name|prev
parameter_list|,
name|void
modifier|*
name|conf
parameter_list|)
function_decl|;
DECL|typedef|ngx_mail_module_t
block|}
name|ngx_mail_module_t
typedef|;
end_typedef

begin_define
DECL|macro|NGX_MAIL_MODULE
define|#
directive|define
name|NGX_MAIL_MODULE
value|0x4C49414D
end_define

begin_comment
DECL|macro|NGX_MAIL_MODULE
comment|/* "MAIL" */
end_comment

begin_define
DECL|macro|NGX_MAIL_MAIN_CONF
define|#
directive|define
name|NGX_MAIL_MAIN_CONF
value|0x02000000
end_define

begin_define
DECL|macro|NGX_MAIL_SRV_CONF
define|#
directive|define
name|NGX_MAIL_SRV_CONF
value|0x04000000
end_define

begin_define
DECL|macro|NGX_MAIL_MAIN_CONF_OFFSET
define|#
directive|define
name|NGX_MAIL_MAIN_CONF_OFFSET
value|offsetof(ngx_mail_conf_ctx_t, main_conf)
end_define

begin_define
DECL|macro|NGX_MAIL_SRV_CONF_OFFSET
define|#
directive|define
name|NGX_MAIL_SRV_CONF_OFFSET
value|offsetof(ngx_mail_conf_ctx_t, srv_conf)
end_define

begin_define
DECL|macro|ngx_mail_get_module_ctx (s,module)
define|#
directive|define
name|ngx_mail_get_module_ctx
parameter_list|(
name|s
parameter_list|,
name|module
parameter_list|)
value|(s)->ctx[module.ctx_index]
end_define

begin_define
DECL|macro|ngx_mail_set_ctx (s,c,module)
define|#
directive|define
name|ngx_mail_set_ctx
parameter_list|(
name|s
parameter_list|,
name|c
parameter_list|,
name|module
parameter_list|)
value|s->ctx[module.ctx_index] = c;
end_define

begin_define
DECL|macro|ngx_mail_delete_ctx (s,module)
define|#
directive|define
name|ngx_mail_delete_ctx
parameter_list|(
name|s
parameter_list|,
name|module
parameter_list|)
value|s->ctx[module.ctx_index] = NULL;
end_define

begin_define
DECL|macro|ngx_mail_get_module_main_conf (s,module)
define|#
directive|define
name|ngx_mail_get_module_main_conf
parameter_list|(
name|s
parameter_list|,
name|module
parameter_list|)
define|\
value|(s)->main_conf[module.ctx_index]
end_define

begin_define
DECL|macro|ngx_mail_get_module_srv_conf (s,module)
define|#
directive|define
name|ngx_mail_get_module_srv_conf
parameter_list|(
name|s
parameter_list|,
name|module
parameter_list|)
value|(s)->srv_conf[module.ctx_index]
end_define

begin_define
DECL|macro|ngx_mail_conf_get_module_main_conf (cf,module)
define|#
directive|define
name|ngx_mail_conf_get_module_main_conf
parameter_list|(
name|cf
parameter_list|,
name|module
parameter_list|)
define|\
value|((ngx_mail_conf_ctx_t *) cf->ctx)->main_conf[module.ctx_index]
end_define

begin_define
DECL|macro|ngx_mail_conf_get_module_srv_conf (cf,module)
define|#
directive|define
name|ngx_mail_conf_get_module_srv_conf
parameter_list|(
name|cf
parameter_list|,
name|module
parameter_list|)
define|\
value|((ngx_mail_conf_ctx_t *) cf->ctx)->srv_conf[module.ctx_index]
end_define

begin_if
if|#
directive|if
operator|(
name|NGX_MAIL_SSL
operator|)
end_if

begin_function_decl
name|void
name|ngx_mail_starttls_handler
parameter_list|(
name|ngx_event_t
modifier|*
name|rev
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_int_t
name|ngx_mail_starttls_only
parameter_list|(
name|ngx_mail_session_t
modifier|*
name|s
parameter_list|,
name|ngx_connection_t
modifier|*
name|c
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_function_decl
name|void
name|ngx_mail_init_connection
parameter_list|(
name|ngx_connection_t
modifier|*
name|c
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_int_t
name|ngx_mail_salt
parameter_list|(
name|ngx_mail_session_t
modifier|*
name|s
parameter_list|,
name|ngx_connection_t
modifier|*
name|c
parameter_list|,
name|ngx_mail_core_srv_conf_t
modifier|*
name|cscf
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_int_t
name|ngx_mail_auth_plain
parameter_list|(
name|ngx_mail_session_t
modifier|*
name|s
parameter_list|,
name|ngx_connection_t
modifier|*
name|c
parameter_list|,
name|ngx_uint_t
name|n
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_int_t
name|ngx_mail_auth_login_username
parameter_list|(
name|ngx_mail_session_t
modifier|*
name|s
parameter_list|,
name|ngx_connection_t
modifier|*
name|c
parameter_list|,
name|ngx_uint_t
name|n
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_int_t
name|ngx_mail_auth_login_password
parameter_list|(
name|ngx_mail_session_t
modifier|*
name|s
parameter_list|,
name|ngx_connection_t
modifier|*
name|c
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_int_t
name|ngx_mail_auth_cram_md5_salt
parameter_list|(
name|ngx_mail_session_t
modifier|*
name|s
parameter_list|,
name|ngx_connection_t
modifier|*
name|c
parameter_list|,
name|char
modifier|*
name|prefix
parameter_list|,
name|size_t
name|len
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_int_t
name|ngx_mail_auth_cram_md5
parameter_list|(
name|ngx_mail_session_t
modifier|*
name|s
parameter_list|,
name|ngx_connection_t
modifier|*
name|c
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_int_t
name|ngx_mail_auth_external
parameter_list|(
name|ngx_mail_session_t
modifier|*
name|s
parameter_list|,
name|ngx_connection_t
modifier|*
name|c
parameter_list|,
name|ngx_uint_t
name|n
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_int_t
name|ngx_mail_auth_parse
parameter_list|(
name|ngx_mail_session_t
modifier|*
name|s
parameter_list|,
name|ngx_connection_t
modifier|*
name|c
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|ngx_mail_send
parameter_list|(
name|ngx_event_t
modifier|*
name|wev
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_int_t
name|ngx_mail_read_command
parameter_list|(
name|ngx_mail_session_t
modifier|*
name|s
parameter_list|,
name|ngx_connection_t
modifier|*
name|c
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|ngx_mail_auth
parameter_list|(
name|ngx_mail_session_t
modifier|*
name|s
parameter_list|,
name|ngx_connection_t
modifier|*
name|c
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|ngx_mail_close_connection
parameter_list|(
name|ngx_connection_t
modifier|*
name|c
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|ngx_mail_session_internal_server_error
parameter_list|(
name|ngx_mail_session_t
modifier|*
name|s
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|u_char
modifier|*
name|ngx_mail_log_error
parameter_list|(
name|ngx_log_t
modifier|*
name|log
parameter_list|,
name|u_char
modifier|*
name|buf
parameter_list|,
name|size_t
name|len
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|char
modifier|*
name|ngx_mail_capabilities
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

begin_comment
comment|/* STUB */
end_comment

begin_function_decl
name|void
name|ngx_mail_proxy_init
parameter_list|(
name|ngx_mail_session_t
modifier|*
name|s
parameter_list|,
name|ngx_addr_t
modifier|*
name|peer
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|ngx_mail_auth_http_init
parameter_list|(
name|ngx_mail_session_t
modifier|*
name|s
parameter_list|)
function_decl|;
end_function_decl

begin_comment
comment|/**/
end_comment

begin_decl_stmt
specifier|extern
name|ngx_uint_t
name|ngx_mail_max_module
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|ngx_module_t
name|ngx_mail_core_module
decl_stmt|;
end_decl_stmt

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* _NGX_MAIL_H_INCLUDED_ */
end_comment

end_unit

