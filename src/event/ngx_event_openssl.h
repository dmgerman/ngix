begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|_NGX_EVENT_OPENSSL_H_INCLUDED_
end_ifndef

begin_define
DECL|macro|_NGX_EVENT_OPENSSL_H_INCLUDED_
define|#
directive|define
name|_NGX_EVENT_OPENSSL_H_INCLUDED_
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
file|<openssl/ssl.h>
end_include

begin_include
include|#
directive|include
file|<openssl/err.h>
end_include

begin_typedef
DECL|typedef|ngx_ssl_t
typedef|typedef
name|SSL
name|ngx_ssl_t
typedef|;
end_typedef

begin_typedef
DECL|typedef|ngx_ssl_ctx_t
typedef|typedef
name|SSL_CTX
name|ngx_ssl_ctx_t
typedef|;
end_typedef

begin_define
DECL|macro|NGX_SSL_HTTP_ERROR
define|#
directive|define
name|NGX_SSL_HTTP_ERROR
value|-10
end_define

begin_function_decl
name|ngx_int_t
name|ngx_ssl_init
parameter_list|(
name|ngx_log_t
modifier|*
name|log
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_int_t
name|ngx_ssl_create_session
parameter_list|(
name|ngx_ssl_ctx_t
modifier|*
name|ctx
parameter_list|,
name|ngx_connection_t
modifier|*
name|c
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_int_t
name|ngx_ssl_recv
parameter_list|(
name|ngx_connection_t
modifier|*
name|c
parameter_list|,
name|u_char
modifier|*
name|buf
parameter_list|,
name|size_t
name|size
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* _NGX_EVENT_OPENSSL_H_INCLUDED_ */
end_comment

end_unit

