begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|_NGX_HTTP_H_INCLUDED_
end_ifndef

begin_define
DECL|macro|_NGX_HTTP_H_INCLUDED_
define|#
directive|define
name|_NGX_HTTP_H_INCLUDED_
end_define

begin_include
include|#
directive|include
file|<ngx_config.h>
end_include

begin_include
include|#
directive|include
file|<ngx_types.h>
end_include

begin_include
include|#
directive|include
file|<ngx_hunk.h>
end_include

begin_include
include|#
directive|include
file|<ngx_files.h>
end_include

begin_include
include|#
directive|include
file|<ngx_connection.h>
end_include

begin_include
include|#
directive|include
file|<ngx_config_command.h>
end_include

begin_define
DECL|macro|NGX_HTTP_GET
define|#
directive|define
name|NGX_HTTP_GET
value|1
end_define

begin_define
DECL|macro|NGX_HTTP_HEAD
define|#
directive|define
name|NGX_HTTP_HEAD
value|2
end_define

begin_define
DECL|macro|NGX_HTTP_POST
define|#
directive|define
name|NGX_HTTP_POST
value|3
end_define

begin_define
DECL|macro|NGX_HTTP_CONN_CLOSE
define|#
directive|define
name|NGX_HTTP_CONN_CLOSE
value|0
end_define

begin_define
DECL|macro|NGX_HTTP_CONN_KEEP_ALIVE
define|#
directive|define
name|NGX_HTTP_CONN_KEEP_ALIVE
value|1
end_define

begin_define
DECL|macro|NGX_HTTP_HEADER_DONE
define|#
directive|define
name|NGX_HTTP_HEADER_DONE
value|1
end_define

begin_define
DECL|macro|NGX_HTTP_INVALID_METHOD
define|#
directive|define
name|NGX_HTTP_INVALID_METHOD
value|10
end_define

begin_define
DECL|macro|NGX_HTTP_INVALID_REQUEST
define|#
directive|define
name|NGX_HTTP_INVALID_REQUEST
value|11
end_define

begin_define
DECL|macro|NGX_HTTP_INVALID_HEAD
define|#
directive|define
name|NGX_HTTP_INVALID_HEAD
value|12
end_define

begin_define
DECL|macro|NGX_HTTP_INVALID_HEADER
define|#
directive|define
name|NGX_HTTP_INVALID_HEADER
value|13
end_define

begin_define
DECL|macro|NGX_HTTP_OK
define|#
directive|define
name|NGX_HTTP_OK
value|200
end_define

begin_define
DECL|macro|NGX_HTTP_SPECIAL_RESPONSE
define|#
directive|define
name|NGX_HTTP_SPECIAL_RESPONSE
value|300
end_define

begin_define
DECL|macro|NGX_HTTP_MOVED_PERMANENTLY
define|#
directive|define
name|NGX_HTTP_MOVED_PERMANENTLY
value|302
end_define

begin_define
DECL|macro|NGX_HTTP_BAD_REQUEST
define|#
directive|define
name|NGX_HTTP_BAD_REQUEST
value|400
end_define

begin_define
DECL|macro|NGX_HTTP_NOT_FOUND
define|#
directive|define
name|NGX_HTTP_NOT_FOUND
value|404
end_define

begin_define
DECL|macro|NGX_HTTP_INTERNAL_SERVER_ERROR
define|#
directive|define
name|NGX_HTTP_INTERNAL_SERVER_ERROR
value|503
end_define

begin_define
DECL|macro|NGX_HTTP_STATIC_HANDLER
define|#
directive|define
name|NGX_HTTP_STATIC_HANDLER
value|0
end_define

begin_define
DECL|macro|NGX_HTTP_DIRECTORY_HANDLER
define|#
directive|define
name|NGX_HTTP_DIRECTORY_HANDLER
value|1
end_define

begin_typedef
DECL|struct|__anon29f9ed430108
typedef|typedef
struct|struct
block|{
DECL|member|doc_root
name|char
modifier|*
name|doc_root
decl_stmt|;
DECL|member|doc_root_len
name|size_t
name|doc_root_len
decl_stmt|;
DECL|member|request_pool_size
name|size_t
name|request_pool_size
decl_stmt|;
DECL|member|header_buffer_size
name|size_t
name|header_buffer_size
decl_stmt|;
DECL|member|discarded_buffer_size
name|size_t
name|discarded_buffer_size
decl_stmt|;
DECL|member|header_timeout
name|unsigned
name|int
name|header_timeout
decl_stmt|;
DECL|typedef|ngx_http_server_t
block|}
name|ngx_http_server_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon29f9ed430208
typedef|typedef
struct|struct
block|{
DECL|member|buff
name|char
modifier|*
name|buff
decl_stmt|;
DECL|member|pos
name|char
modifier|*
name|pos
decl_stmt|;
DECL|member|last
name|char
modifier|*
name|last
decl_stmt|;
DECL|member|end
name|char
modifier|*
name|end
decl_stmt|;
DECL|typedef|ngx_buff_t
block|}
name|ngx_buff_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon29f9ed430308
typedef|typedef
struct|struct
block|{
DECL|member|status
name|int
name|status
decl_stmt|;
DECL|member|connection
name|int
name|connection
decl_stmt|;
DECL|member|content_length
name|off_t
name|content_length
decl_stmt|;
DECL|member|location
name|char
modifier|*
name|location
decl_stmt|;
DECL|member|content_type
name|char
modifier|*
name|content_type
decl_stmt|;
DECL|member|charset
name|char
modifier|*
name|charset
decl_stmt|;
DECL|member|etag
name|char
modifier|*
name|etag
decl_stmt|;
DECL|member|server
name|char
modifier|*
name|server
decl_stmt|;
DECL|member|date
name|time_t
name|date
decl_stmt|;
DECL|member|last_modified
name|time_t
name|last_modified
decl_stmt|;
DECL|typedef|ngx_http_headers_out_t
block|}
name|ngx_http_headers_out_t
typedef|;
end_typedef

begin_typedef
DECL|typedef|ngx_http_request_t
typedef|typedef
name|struct
name|ngx_http_request_s
name|ngx_http_request_t
typedef|;
end_typedef

begin_struct
DECL|struct|ngx_http_request_s
struct|struct
name|ngx_http_request_s
block|{
DECL|member|filename
name|char
modifier|*
name|filename
decl_stmt|;
DECL|member|location
name|char
modifier|*
name|location
decl_stmt|;
DECL|member|fd
name|ngx_fd_t
name|fd
decl_stmt|;
DECL|member|ctx
name|void
modifier|*
modifier|*
name|ctx
decl_stmt|;
DECL|member|loc_conf
name|void
modifier|*
modifier|*
name|loc_conf
decl_stmt|;
DECL|member|srv_conf
name|void
modifier|*
modifier|*
name|srv_conf
decl_stmt|;
DECL|member|pool
name|ngx_pool_t
modifier|*
name|pool
decl_stmt|;
DECL|member|header_in
name|ngx_hunk_t
modifier|*
name|header_in
decl_stmt|;
comment|/*     ngx_http_headers_in_t *headers_in; */
DECL|member|headers_out
name|ngx_http_headers_out_t
modifier|*
name|headers_out
decl_stmt|;
DECL|member|filename_len
name|int
name|filename_len
decl_stmt|;
DECL|member|handler
name|int
function_decl|(
modifier|*
name|handler
function_decl|)
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
function_decl|;
DECL|member|fileinfo
name|ngx_file_info_t
name|fileinfo
decl_stmt|;
DECL|member|method
name|int
name|method
decl_stmt|;
DECL|member|http_version
name|int
name|http_version
decl_stmt|;
DECL|member|http_major
name|int
name|http_major
decl_stmt|;
DECL|member|http_minor
name|int
name|http_minor
decl_stmt|;
DECL|member|uri
name|char
modifier|*
name|uri
decl_stmt|;
DECL|member|main
name|ngx_http_request_t
modifier|*
decl|main
decl_stmt|;
DECL|member|connection
name|ngx_connection_t
modifier|*
name|connection
decl_stmt|;
DECL|member|server
name|ngx_http_server_t
modifier|*
name|server
decl_stmt|;
DECL|member|filter
name|int
name|filter
decl_stmt|;
DECL|member|client_content_length
name|ssize_t
name|client_content_length
decl_stmt|;
DECL|member|discarded_buffer
name|char
modifier|*
name|discarded_buffer
decl_stmt|;
DECL|member|header_timeout
name|unsigned
name|header_timeout
range|:
literal|1
decl_stmt|;
DECL|member|process_header
name|unsigned
name|process_header
range|:
literal|1
decl_stmt|;
DECL|member|header_only
name|unsigned
name|header_only
range|:
literal|1
decl_stmt|;
DECL|member|unusual_uri
name|unsigned
name|unusual_uri
range|:
literal|1
decl_stmt|;
DECL|member|complex_uri
name|unsigned
name|complex_uri
range|:
literal|1
decl_stmt|;
DECL|member|state
name|int
name|state
decl_stmt|;
DECL|member|uri_start
name|char
modifier|*
name|uri_start
decl_stmt|;
DECL|member|uri_end
name|char
modifier|*
name|uri_end
decl_stmt|;
DECL|member|uri_ext
name|char
modifier|*
name|uri_ext
decl_stmt|;
DECL|member|args_start
name|char
modifier|*
name|args_start
decl_stmt|;
DECL|member|header_name_start
name|char
modifier|*
name|header_name_start
decl_stmt|;
DECL|member|header_name_end
name|char
modifier|*
name|header_name_end
decl_stmt|;
DECL|member|header_start
name|char
modifier|*
name|header_start
decl_stmt|;
DECL|member|header_end
name|char
modifier|*
name|header_end
decl_stmt|;
ifdef|#
directive|ifdef
name|NGX_EVENT
DECL|member|state_handler
name|int
function_decl|(
modifier|*
name|state_handler
function_decl|)
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
function_decl|;
endif|#
directive|endif
block|}
struct|;
end_struct

begin_typedef
DECL|struct|__anon29f9ed430408
typedef|typedef
struct|struct
block|{
DECL|member|action
name|char
modifier|*
name|action
decl_stmt|;
DECL|member|client
name|char
modifier|*
name|client
decl_stmt|;
DECL|member|url
name|char
modifier|*
name|url
decl_stmt|;
DECL|typedef|ngx_http_log_ctx_t
block|}
name|ngx_http_log_ctx_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon29f9ed430508
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
DECL|member|commands
name|ngx_command_t
modifier|*
name|commands
decl_stmt|;
DECL|member|init_module
name|int
function_decl|(
modifier|*
name|init_module
function_decl|)
parameter_list|(
name|ngx_pool_t
modifier|*
name|p
parameter_list|)
function_decl|;
DECL|member|init_output_body_filter
name|int
function_decl|(
modifier|*
name|init_output_body_filter
function_decl|)
parameter_list|(
name|int
function_decl|(
modifier|*
modifier|*
name|next_filter
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
parameter_list|)
function_decl|;
DECL|typedef|ngx_http_module_t
block|}
name|ngx_http_module_t
typedef|;
end_typedef

begin_define
DECL|macro|NGX_HTTP_MODULE
define|#
directive|define
name|NGX_HTTP_MODULE
value|0
end_define

begin_define
DECL|macro|ngx_get_module_loc_conf (r,module)
define|#
directive|define
name|ngx_get_module_loc_conf
parameter_list|(
name|r
parameter_list|,
name|module
parameter_list|)
value|r->loc_conf[module.index]
end_define

begin_define
DECL|macro|ngx_get_module_ctx (r,module)
define|#
directive|define
name|ngx_get_module_ctx
parameter_list|(
name|r
parameter_list|,
name|module
parameter_list|)
value|r->ctx[module.index]
end_define

begin_define
DECL|macro|ngx_http_create_ctx (r,ctx,module,size)
define|#
directive|define
name|ngx_http_create_ctx
parameter_list|(
name|r
parameter_list|,
name|ctx
parameter_list|,
name|module
parameter_list|,
name|size
parameter_list|)
define|\
value|do {                                                              \                ngx_test_null(ctx, ngx_pcalloc(r->pool, size), NGX_ERROR);     \                r->ctx[module.index] = ctx;                                    \             } while (0)
end_define

begin_comment
comment|/* STUB */
end_comment

begin_define
DECL|macro|NGX_INDEX
define|#
directive|define
name|NGX_INDEX
value|"index.html"
end_define

begin_comment
comment|/* STUB */
end_comment

begin_function_decl
name|int
name|ngx_http_init
parameter_list|(
name|ngx_pool_t
modifier|*
name|pool
parameter_list|,
name|ngx_log_t
modifier|*
name|log
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|int
name|ngx_http_init_connection
parameter_list|(
name|ngx_connection_t
modifier|*
name|c
parameter_list|)
function_decl|;
end_function_decl

begin_decl_stmt
specifier|extern
name|int
name|ngx_max_module
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|ngx_http_module_t
modifier|*
name|ngx_http_modules
index|[]
decl_stmt|;
end_decl_stmt

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* _NGX_HTTP_H_INCLUDED_ */
end_comment

end_unit

