begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (C) Igor Sysoev  * Copyright (C) Nginx, Inc.  */
end_comment

begin_ifndef
ifndef|#
directive|ifndef
name|_NGX_HTTP_SSL_H_INCLUDED_
end_ifndef

begin_define
DECL|macro|_NGX_HTTP_SSL_H_INCLUDED_
define|#
directive|define
name|_NGX_HTTP_SSL_H_INCLUDED_
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
file|<ngx_http.h>
end_include

begin_typedef
DECL|struct|__anon28d81e6f0108
typedef|typedef
struct|struct
block|{
DECL|member|enable
name|ngx_flag_t
name|enable
decl_stmt|;
DECL|member|ssl
name|ngx_ssl_t
name|ssl
decl_stmt|;
DECL|member|prefer_server_ciphers
name|ngx_flag_t
name|prefer_server_ciphers
decl_stmt|;
DECL|member|protocols
name|ngx_uint_t
name|protocols
decl_stmt|;
DECL|member|verify
name|ngx_uint_t
name|verify
decl_stmt|;
DECL|member|verify_depth
name|ngx_uint_t
name|verify_depth
decl_stmt|;
DECL|member|builtin_session_cache
name|ssize_t
name|builtin_session_cache
decl_stmt|;
DECL|member|session_timeout
name|time_t
name|session_timeout
decl_stmt|;
DECL|member|certificate
name|ngx_str_t
name|certificate
decl_stmt|;
DECL|member|certificate_key
name|ngx_str_t
name|certificate_key
decl_stmt|;
DECL|member|dhparam
name|ngx_str_t
name|dhparam
decl_stmt|;
DECL|member|ecdh_curve
name|ngx_str_t
name|ecdh_curve
decl_stmt|;
DECL|member|client_certificate
name|ngx_str_t
name|client_certificate
decl_stmt|;
DECL|member|trusted_certificate
name|ngx_str_t
name|trusted_certificate
decl_stmt|;
DECL|member|crl
name|ngx_str_t
name|crl
decl_stmt|;
DECL|member|ciphers
name|ngx_str_t
name|ciphers
decl_stmt|;
DECL|member|shm_zone
name|ngx_shm_zone_t
modifier|*
name|shm_zone
decl_stmt|;
DECL|member|stapling
name|ngx_flag_t
name|stapling
decl_stmt|;
DECL|member|stapling_verify
name|ngx_flag_t
name|stapling_verify
decl_stmt|;
DECL|member|stapling_file
name|ngx_str_t
name|stapling_file
decl_stmt|;
DECL|member|stapling_responder
name|ngx_str_t
name|stapling_responder
decl_stmt|;
DECL|member|file
name|u_char
modifier|*
name|file
decl_stmt|;
DECL|member|line
name|ngx_uint_t
name|line
decl_stmt|;
DECL|typedef|ngx_http_ssl_srv_conf_t
block|}
name|ngx_http_ssl_srv_conf_t
typedef|;
end_typedef

begin_decl_stmt
specifier|extern
name|ngx_module_t
name|ngx_http_ssl_module
decl_stmt|;
end_decl_stmt

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* _NGX_HTTP_SSL_H_INCLUDED_ */
end_comment

end_unit

