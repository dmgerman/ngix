begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (C) Igor Sysoev  * Copyright (C) Nginx, Inc.  */
end_comment

begin_ifndef
ifndef|#
directive|ifndef
name|_NGX_HTTP_UPSTREAM_ROUND_ROBIN_H_INCLUDED_
end_ifndef

begin_define
DECL|macro|_NGX_HTTP_UPSTREAM_ROUND_ROBIN_H_INCLUDED_
define|#
directive|define
name|_NGX_HTTP_UPSTREAM_ROUND_ROBIN_H_INCLUDED_
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
DECL|struct|__anon2c84455b0108
typedef|typedef
struct|struct
block|{
DECL|member|sockaddr
name|struct
name|sockaddr
modifier|*
name|sockaddr
decl_stmt|;
DECL|member|socklen
name|socklen_t
name|socklen
decl_stmt|;
DECL|member|name
name|ngx_str_t
name|name
decl_stmt|;
DECL|member|current_weight
name|ngx_int_t
name|current_weight
decl_stmt|;
DECL|member|effective_weight
name|ngx_int_t
name|effective_weight
decl_stmt|;
DECL|member|weight
name|ngx_int_t
name|weight
decl_stmt|;
DECL|member|fails
name|ngx_uint_t
name|fails
decl_stmt|;
DECL|member|accessed
name|time_t
name|accessed
decl_stmt|;
DECL|member|checked
name|time_t
name|checked
decl_stmt|;
DECL|member|max_fails
name|ngx_uint_t
name|max_fails
decl_stmt|;
DECL|member|fail_timeout
name|time_t
name|fail_timeout
decl_stmt|;
DECL|member|down
name|ngx_uint_t
name|down
decl_stmt|;
comment|/* unsigned  down:1; */
if|#
directive|if
operator|(
name|NGX_HTTP_SSL
operator|)
DECL|member|ssl_session
name|ngx_ssl_session_t
modifier|*
name|ssl_session
decl_stmt|;
comment|/* local to a process */
endif|#
directive|endif
DECL|typedef|ngx_http_upstream_rr_peer_t
block|}
name|ngx_http_upstream_rr_peer_t
typedef|;
end_typedef

begin_typedef
DECL|typedef|ngx_http_upstream_rr_peers_t
typedef|typedef
name|struct
name|ngx_http_upstream_rr_peers_s
name|ngx_http_upstream_rr_peers_t
typedef|;
end_typedef

begin_struct
DECL|struct|ngx_http_upstream_rr_peers_s
struct|struct
name|ngx_http_upstream_rr_peers_s
block|{
DECL|member|single
name|ngx_uint_t
name|single
decl_stmt|;
comment|/* unsigned  single:1; */
DECL|member|number
name|ngx_uint_t
name|number
decl_stmt|;
DECL|member|last_cached
name|ngx_uint_t
name|last_cached
decl_stmt|;
comment|/* ngx_mutex_t                    *mutex; */
DECL|member|cached
name|ngx_connection_t
modifier|*
modifier|*
name|cached
decl_stmt|;
DECL|member|name
name|ngx_str_t
modifier|*
name|name
decl_stmt|;
DECL|member|next
name|ngx_http_upstream_rr_peers_t
modifier|*
name|next
decl_stmt|;
DECL|member|peer
name|ngx_http_upstream_rr_peer_t
name|peer
index|[
literal|1
index|]
decl_stmt|;
block|}
struct|;
end_struct

begin_typedef
DECL|struct|__anon2c84455b0208
typedef|typedef
struct|struct
block|{
DECL|member|peers
name|ngx_http_upstream_rr_peers_t
modifier|*
name|peers
decl_stmt|;
DECL|member|current
name|ngx_uint_t
name|current
decl_stmt|;
DECL|member|tried
name|uintptr_t
modifier|*
name|tried
decl_stmt|;
DECL|member|data
name|uintptr_t
name|data
decl_stmt|;
DECL|typedef|ngx_http_upstream_rr_peer_data_t
block|}
name|ngx_http_upstream_rr_peer_data_t
typedef|;
end_typedef

begin_function_decl
name|ngx_int_t
name|ngx_http_upstream_init_round_robin
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|ngx_http_upstream_srv_conf_t
modifier|*
name|us
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_int_t
name|ngx_http_upstream_init_round_robin_peer
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_upstream_srv_conf_t
modifier|*
name|us
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_int_t
name|ngx_http_upstream_create_round_robin_peer
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_upstream_resolved_t
modifier|*
name|ur
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_int_t
name|ngx_http_upstream_get_round_robin_peer
parameter_list|(
name|ngx_peer_connection_t
modifier|*
name|pc
parameter_list|,
name|void
modifier|*
name|data
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|ngx_http_upstream_free_round_robin_peer
parameter_list|(
name|ngx_peer_connection_t
modifier|*
name|pc
parameter_list|,
name|void
modifier|*
name|data
parameter_list|,
name|ngx_uint_t
name|state
parameter_list|)
function_decl|;
end_function_decl

begin_if
if|#
directive|if
operator|(
name|NGX_HTTP_SSL
operator|)
end_if

begin_function_decl
name|ngx_int_t
name|ngx_http_upstream_set_round_robin_peer_session
parameter_list|(
name|ngx_peer_connection_t
modifier|*
name|pc
parameter_list|,
name|void
modifier|*
name|data
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|ngx_http_upstream_save_round_robin_peer_session
parameter_list|(
name|ngx_peer_connection_t
modifier|*
name|pc
parameter_list|,
name|void
modifier|*
name|data
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* _NGX_HTTP_UPSTREAM_ROUND_ROBIN_H_INCLUDED_ */
end_comment

end_unit

