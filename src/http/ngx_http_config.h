begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|_NGX_HTTP_CONFIG_H_INCLUDED_
end_ifndef

begin_define
DECL|macro|_NGX_HTTP_CONFIG_H_INCLUDED_
define|#
directive|define
name|_NGX_HTTP_CONFIG_H_INCLUDED_
end_define

begin_include
include|#
directive|include
file|<ngx_alloc.h>
end_include

begin_include
include|#
directive|include
file|<ngx_http.h>
end_include

begin_typedef
DECL|struct|__anon29f453f60108
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
DECL|member|loc_conf
name|void
modifier|*
modifier|*
name|loc_conf
decl_stmt|;
DECL|typedef|ngx_http_conf_ctx_t
block|}
name|ngx_http_conf_ctx_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon29f453f60208
typedef|typedef
struct|struct
block|{
DECL|member|output_header_filter
name|int
function_decl|(
modifier|*
name|output_header_filter
function_decl|)
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
function_decl|;
DECL|member|output_body_filter
name|int
function_decl|(
modifier|*
name|output_body_filter
function_decl|)
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_chain_t
modifier|*
name|ch
parameter_list|)
function_decl|;
DECL|typedef|ngx_http_conf_filter_t
block|}
name|ngx_http_conf_filter_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon29f453f60308
typedef|typedef
struct|struct
block|{
DECL|member|index
name|int
name|index
decl_stmt|;
DECL|member|create_srv_conf
name|void
modifier|*
function_decl|(
modifier|*
name|create_srv_conf
function_decl|)
parameter_list|(
name|ngx_pool_t
modifier|*
name|p
parameter_list|)
function_decl|;
DECL|member|init_srv_conf
name|char
modifier|*
function_decl|(
modifier|*
name|init_srv_conf
function_decl|)
parameter_list|(
name|ngx_pool_t
modifier|*
name|p
parameter_list|,
name|void
modifier|*
name|conf
parameter_list|)
function_decl|;
DECL|member|create_loc_conf
name|void
modifier|*
function_decl|(
modifier|*
name|create_loc_conf
function_decl|)
parameter_list|(
name|ngx_pool_t
modifier|*
name|p
parameter_list|)
function_decl|;
DECL|member|merge_loc_conf
name|char
modifier|*
function_decl|(
modifier|*
name|merge_loc_conf
function_decl|)
parameter_list|(
name|ngx_pool_t
modifier|*
name|p
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
DECL|typedef|ngx_http_module_t
block|}
name|ngx_http_module_t
typedef|;
end_typedef

begin_define
DECL|macro|NGX_HTTP_MODULE_TYPE
define|#
directive|define
name|NGX_HTTP_MODULE_TYPE
value|0x50545448
end_define

begin_comment
DECL|macro|NGX_HTTP_MODULE_TYPE
comment|/* "HTTP" */
end_comment

begin_define
DECL|macro|NGX_HTTP_MODULE
define|#
directive|define
name|NGX_HTTP_MODULE
value|0
end_define

begin_define
DECL|macro|NGX_HTTP_MAIN_CONF
define|#
directive|define
name|NGX_HTTP_MAIN_CONF
value|0x2000000
end_define

begin_define
DECL|macro|NGX_HTTP_SRV_CONF
define|#
directive|define
name|NGX_HTTP_SRV_CONF
value|0x4000000
end_define

begin_define
DECL|macro|NGX_HTTP_LOC_CONF
define|#
directive|define
name|NGX_HTTP_LOC_CONF
value|0x8000000
end_define

begin_define
DECL|macro|NGX_HTTP_MAIN_CONF_OFFSET
define|#
directive|define
name|NGX_HTTP_MAIN_CONF_OFFSET
value|offsetof(ngx_http_conf_ctx_t, main_conf)
end_define

begin_define
DECL|macro|NGX_HTTP_SRV_CONF_OFFSET
define|#
directive|define
name|NGX_HTTP_SRV_CONF_OFFSET
value|offsetof(ngx_http_conf_ctx_t, srv_conf)
end_define

begin_define
DECL|macro|NGX_HTTP_LOC_CONF_OFFSET
define|#
directive|define
name|NGX_HTTP_LOC_CONF_OFFSET
value|offsetof(ngx_http_conf_ctx_t, loc_conf)
end_define

begin_define
DECL|macro|ngx_http_get_module_main_conf (r,ctx)
define|#
directive|define
name|ngx_http_get_module_main_conf
parameter_list|(
name|r
parameter_list|,
name|ctx
parameter_list|)
value|r->main_conf[ctx.index]
end_define

begin_define
DECL|macro|ngx_http_get_module_srv_conf (r,ctx)
define|#
directive|define
name|ngx_http_get_module_srv_conf
parameter_list|(
name|r
parameter_list|,
name|ctx
parameter_list|)
value|r->srv_conf[ctx.index]
end_define

begin_define
DECL|macro|ngx_http_get_module_loc_conf (r,ctx)
define|#
directive|define
name|ngx_http_get_module_loc_conf
parameter_list|(
name|r
parameter_list|,
name|ctx
parameter_list|)
value|r->loc_conf[ctx.index]
end_define

begin_function_decl
name|int
name|ngx_http_config_modules
parameter_list|(
name|ngx_pool_t
modifier|*
name|pool
parameter_list|,
name|ngx_module_t
modifier|*
modifier|*
name|modules
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|extern
name|int
function_decl|(
modifier|*
name|ngx_http_top_header_filter
function_decl|)
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* _NGX_HTTP_CONFIG_H_INCLUDED_ */
end_comment

end_unit

