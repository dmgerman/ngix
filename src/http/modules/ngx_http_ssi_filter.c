begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
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
DECL|macro|NGX_HTTP_SSI_COMMAND_LEN
define|#
directive|define
name|NGX_HTTP_SSI_COMMAND_LEN
value|31
end_define

begin_define
DECL|macro|NGX_HTTP_SSI_PARAM_LEN
define|#
directive|define
name|NGX_HTTP_SSI_PARAM_LEN
value|31
end_define

begin_define
DECL|macro|NGX_HTTP_SSI_COPY
define|#
directive|define
name|NGX_HTTP_SSI_COPY
value|1
end_define

begin_define
DECL|macro|NGX_HTTP_SSI_INVALID_COMMAND
define|#
directive|define
name|NGX_HTTP_SSI_INVALID_COMMAND
value|2
end_define

begin_define
DECL|macro|NGX_HTTP_SSI_INVALID_PARAM
define|#
directive|define
name|NGX_HTTP_SSI_INVALID_PARAM
value|3
end_define

begin_define
DECL|macro|NGX_HTTP_SSI_INVALID_VALUE
define|#
directive|define
name|NGX_HTTP_SSI_INVALID_VALUE
value|4
end_define

begin_define
DECL|macro|NGX_HTTP_SSI_LONG_VALUE
define|#
directive|define
name|NGX_HTTP_SSI_LONG_VALUE
value|5
end_define

begin_typedef
DECL|struct|__anon2c2a382b0108
typedef|typedef
struct|struct
block|{
DECL|member|enable
name|int
name|enable
decl_stmt|;
DECL|typedef|ngx_http_ssi_conf_t
block|}
name|ngx_http_ssi_conf_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon2c2a382b0208
typedef|typedef
struct|struct
block|{
DECL|member|buf
name|ngx_hunk_t
modifier|*
name|buf
decl_stmt|;
DECL|member|start
name|char
modifier|*
name|start
decl_stmt|;
DECL|member|last
name|char
modifier|*
name|last
decl_stmt|;
DECL|member|pos
name|char
modifier|*
name|pos
decl_stmt|;
DECL|member|param
name|ngx_table_elt_t
modifier|*
name|param
decl_stmt|;
DECL|member|command
name|ngx_str_t
name|command
decl_stmt|;
DECL|member|params
name|ngx_array_t
name|params
decl_stmt|;
DECL|member|state
name|int
name|state
decl_stmt|;
DECL|member|in
name|ngx_chain_t
modifier|*
name|in
decl_stmt|;
DECL|member|current
name|ngx_chain_t
modifier|*
name|current
decl_stmt|;
DECL|member|out
name|ngx_chain_t
modifier|*
name|out
decl_stmt|;
DECL|member|last_out
name|ngx_chain_t
modifier|*
modifier|*
name|last_out
decl_stmt|;
DECL|member|busy
name|ngx_chain_t
modifier|*
name|busy
decl_stmt|;
DECL|member|prev
name|size_t
name|prev
decl_stmt|;
DECL|member|value_len
name|u_int
name|value_len
decl_stmt|;
DECL|typedef|ngx_http_ssi_ctx_t
block|}
name|ngx_http_ssi_ctx_t
typedef|;
end_typedef

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_ssi_parse
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_ssi_ctx_t
modifier|*
name|ctx
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|void
modifier|*
name|ngx_http_ssi_create_conf
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
name|ngx_http_ssi_merge_conf
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
name|int
name|ngx_http_ssi_filter_init
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
function_decl|;
end_function_decl

begin_decl_stmt
DECL|variable|ngx_http_ssi_filter_commands
specifier|static
name|ngx_command_t
name|ngx_http_ssi_filter_commands
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"ssi"
argument_list|)
block|,
name|NGX_HTTP_MAIN_CONF
operator||
name|NGX_HTTP_SRV_CONF
operator||
name|NGX_HTTP_LOC_CONF
operator||
name|NGX_CONF_FLAG
block|,
name|ngx_conf_set_flag_slot
block|,
name|NGX_HTTP_LOC_CONF_OFFSET
block|,
name|offsetof
argument_list|(
name|ngx_http_ssi_conf_t
argument_list|,
name|enable
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
DECL|variable|ngx_http_ssi_filter_module_ctx
specifier|static
name|ngx_http_module_t
name|ngx_http_ssi_filter_module_ctx
init|=
block|{
name|NULL
block|,
comment|/* pre conf */
name|NULL
block|,
comment|/* create main configuration */
name|NULL
block|,
comment|/* init main configuration */
name|NULL
block|,
comment|/* create server configuration */
name|NULL
block|,
comment|/* merge server configuration */
name|ngx_http_ssi_create_conf
block|,
comment|/* create location configuration */
name|ngx_http_ssi_merge_conf
comment|/* merge location configuration */
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_ssi_filter_module
name|ngx_module_t
name|ngx_http_ssi_filter_module
init|=
block|{
name|NGX_MODULE
block|,
operator|&
name|ngx_http_ssi_filter_module_ctx
block|,
comment|/* module context */
name|ngx_http_ssi_filter_commands
block|,
comment|/* module directives */
name|NGX_HTTP_MODULE
block|,
comment|/* module type */
name|ngx_http_ssi_filter_init
block|,
comment|/* init module */
name|NULL
comment|/* init child */
block|}
decl_stmt|;
end_decl_stmt

begin_function_decl
DECL|variable|ngx_http_next_header_filter
specifier|static
name|int
function_decl|(
modifier|*
name|ngx_http_next_header_filter
function_decl|)
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
DECL|variable|ngx_http_next_body_filter
specifier|static
name|int
function_decl|(
modifier|*
name|ngx_http_next_body_filter
function_decl|)
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_chain_t
modifier|*
name|in
parameter_list|)
function_decl|;
end_function_decl

begin_decl_stmt
DECL|variable|comment_string
specifier|static
name|char
name|comment_string
index|[]
init|=
literal|"<!--"
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|error_string
specifier|static
name|char
name|error_string
index|[]
init|=
literal|"[an error occurred while processing "
literal|"the directive]"
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|ngx_http_ssi_header_filter (ngx_http_request_t * r)
specifier|static
name|int
name|ngx_http_ssi_header_filter
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
block|{
name|ngx_http_ssi_ctx_t
modifier|*
name|ctx
decl_stmt|;
name|ngx_http_ssi_conf_t
modifier|*
name|conf
decl_stmt|;
name|conf
operator|=
name|ngx_http_get_module_loc_conf
argument_list|(
name|r
argument_list|,
name|ngx_http_ssi_filter_module
argument_list|)
expr_stmt|;
if|if
condition|(
operator|!
name|conf
operator|->
name|enable
condition|)
block|{
return|return
name|ngx_http_next_header_filter
argument_list|(
name|r
argument_list|)
return|;
block|}
name|ngx_http_create_ctx
argument_list|(
name|r
argument_list|,
name|ctx
argument_list|,
name|ngx_http_ssi_filter_module
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_ssi_ctx_t
argument_list|)
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
name|ctx
operator|->
name|last_out
operator|=
operator|&
name|ctx
operator|->
name|out
expr_stmt|;
comment|/* STUB: conf */
name|ctx
operator|->
name|value_len
operator|=
literal|200
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|content_length_n
operator|=
operator|-
literal|1
expr_stmt|;
if|if
condition|(
name|r
operator|->
name|headers_out
operator|.
name|content_length
condition|)
block|{
name|r
operator|->
name|headers_out
operator|.
name|content_length
operator|->
name|key
operator|.
name|len
operator|=
literal|0
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|content_length
operator|=
name|NULL
expr_stmt|;
block|}
name|r
operator|->
name|headers_out
operator|.
name|last_modified_time
operator|=
operator|-
literal|1
expr_stmt|;
if|if
condition|(
name|r
operator|->
name|headers_out
operator|.
name|last_modified
condition|)
block|{
name|r
operator|->
name|headers_out
operator|.
name|last_modified
operator|->
name|key
operator|.
name|len
operator|=
literal|0
expr_stmt|;
name|r
operator|->
name|headers_out
operator|.
name|last_modified
operator|=
name|NULL
expr_stmt|;
block|}
name|r
operator|->
name|filter
operator||=
name|NGX_HTTP_FILTER_NEED_IN_MEMORY
expr_stmt|;
return|return
name|ngx_http_next_header_filter
argument_list|(
name|r
argument_list|)
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_ssi_body_filter (ngx_http_request_t * r,ngx_chain_t * in)
specifier|static
name|int
name|ngx_http_ssi_body_filter
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_chain_t
modifier|*
name|in
parameter_list|)
block|{
name|ngx_int_t
name|rc
decl_stmt|;
name|ngx_hunk_t
modifier|*
name|hunk
decl_stmt|;
name|ngx_chain_t
modifier|*
name|cl
decl_stmt|,
modifier|*
name|tl
decl_stmt|;
name|ngx_http_ssi_ctx_t
modifier|*
name|ctx
decl_stmt|;
name|ctx
operator|=
name|ngx_http_get_module_ctx
argument_list|(
name|r
argument_list|,
name|ngx_http_ssi_filter_module
argument_list|)
expr_stmt|;
if|if
condition|(
name|ctx
operator|==
name|NULL
operator|||
operator|(
name|in
operator|==
name|NULL
operator|&&
name|ctx
operator|->
name|in
operator|==
name|NULL
operator|)
condition|)
block|{
return|return
name|ngx_http_next_body_filter
argument_list|(
name|r
argument_list|,
name|NULL
argument_list|)
return|;
block|}
comment|/* add the incoming hunk to the chain ctx->in */
if|if
condition|(
name|in
condition|)
block|{
if|if
condition|(
name|ngx_chain_add_copy
argument_list|(
name|r
operator|->
name|pool
argument_list|,
operator|&
name|ctx
operator|->
name|in
argument_list|,
name|in
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|ctx
operator|->
name|current
operator|==
name|NULL
condition|)
block|{
name|ctx
operator|->
name|current
operator|=
name|ctx
operator|->
name|in
expr_stmt|;
block|}
block|}
while|while
condition|(
name|ctx
operator|->
name|current
condition|)
block|{
if|if
condition|(
name|ctx
operator|->
name|buf
operator|==
name|NULL
condition|)
block|{
name|ctx
operator|->
name|buf
operator|=
name|ctx
operator|->
name|current
operator|->
name|hunk
expr_stmt|;
name|ctx
operator|->
name|current
operator|=
name|ctx
operator|->
name|current
operator|->
name|next
expr_stmt|;
name|ctx
operator|->
name|start
operator|=
name|ctx
operator|->
name|buf
operator|->
name|pos
expr_stmt|;
name|ctx
operator|->
name|pos
operator|=
name|ctx
operator|->
name|buf
operator|->
name|pos
expr_stmt|;
name|ctx
operator|->
name|last
operator|=
name|ctx
operator|->
name|buf
operator|->
name|pos
expr_stmt|;
block|}
while|while
condition|(
name|ctx
operator|->
name|pos
operator|<
name|ctx
operator|->
name|buf
operator|->
name|last
condition|)
block|{
name|rc
operator|=
name|ngx_http_ssi_parse
argument_list|(
name|r
argument_list|,
name|ctx
argument_list|)
expr_stmt|;
if|if
condition|(
name|rc
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|rc
return|;
block|}
if|else if
condition|(
name|rc
operator|==
name|NGX_HTTP_SSI_COPY
condition|)
block|{
if|if
condition|(
name|ctx
operator|->
name|prev
condition|)
block|{
if|if
condition|(
operator|!
operator|(
name|hunk
operator|=
name|ngx_calloc_hunk
argument_list|(
name|r
operator|->
name|pool
argument_list|)
operator|)
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|hunk
operator|->
name|type
operator|=
name|NGX_HUNK_IN_MEMORY
operator||
name|NGX_HUNK_TEMP
expr_stmt|;
name|hunk
operator|->
name|pos
operator|=
name|comment_string
expr_stmt|;
name|hunk
operator|->
name|last
operator|=
name|comment_string
operator|+
name|ctx
operator|->
name|prev
expr_stmt|;
name|ngx_alloc_link_and_set_hunk
argument_list|(
name|cl
argument_list|,
name|hunk
argument_list|,
name|r
operator|->
name|pool
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
operator|*
name|ctx
operator|->
name|last_out
operator|=
name|cl
expr_stmt|;
name|ctx
operator|->
name|last_out
operator|=
operator|&
name|cl
operator|->
name|next
expr_stmt|;
name|ctx
operator|->
name|prev
operator|=
literal|0
expr_stmt|;
block|}
if|if
condition|(
operator|!
operator|(
name|hunk
operator|=
name|ngx_calloc_hunk
argument_list|(
name|r
operator|->
name|pool
argument_list|)
operator|)
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|hunk
operator|->
name|type
operator|=
name|NGX_HUNK_IN_MEMORY
operator||
name|NGX_HUNK_TEMP
operator||
name|NGX_HUNK_RECYCLED
expr_stmt|;
name|hunk
operator|->
name|pos
operator|=
name|ctx
operator|->
name|start
expr_stmt|;
name|hunk
operator|->
name|last
operator|=
name|ctx
operator|->
name|last
expr_stmt|;
name|hunk
operator|->
name|shadow
operator|=
name|ctx
operator|->
name|buf
expr_stmt|;
name|ngx_alloc_link_and_set_hunk
argument_list|(
name|cl
argument_list|,
name|hunk
argument_list|,
name|r
operator|->
name|pool
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
operator|*
name|ctx
operator|->
name|last_out
operator|=
name|cl
expr_stmt|;
name|ctx
operator|->
name|last_out
operator|=
operator|&
name|cl
operator|->
name|next
expr_stmt|;
if|if
condition|(
name|ctx
operator|->
name|pos
operator|==
name|ctx
operator|->
name|buf
operator|->
name|last
condition|)
block|{
name|ctx
operator|->
name|prev
operator|=
name|ctx
operator|->
name|buf
operator|->
name|last
operator|-
name|ctx
operator|->
name|last
expr_stmt|;
block|}
continue|continue;
if|#
directive|if
literal|0
block_content|} else if (rc == NGX_HTTP_SSI_INVALID_COMMAND) {             } else if (rc == NGX_HTTP_SSI_INVALID_PARAM) {             } else if (rc == NGX_HTTP_SSI_INVALID_VALUE) {             } else if (rc == NGX_HTTP_SSI_LONG_VALUE) {
endif|#
directive|endif
block|}
else|else
block|{
if|if
condition|(
operator|!
operator|(
name|hunk
operator|=
name|ngx_calloc_hunk
argument_list|(
name|r
operator|->
name|pool
argument_list|)
operator|)
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|hunk
operator|->
name|type
operator|=
name|NGX_HUNK_IN_MEMORY
operator||
name|NGX_HUNK_TEMP
expr_stmt|;
name|hunk
operator|->
name|pos
operator|=
name|error_string
expr_stmt|;
name|hunk
operator|->
name|last
operator|=
name|error_string
operator|+
sizeof|sizeof
argument_list|(
name|error_string
argument_list|)
operator|-
literal|1
expr_stmt|;
name|ngx_alloc_link_and_set_hunk
argument_list|(
name|cl
argument_list|,
name|hunk
argument_list|,
name|r
operator|->
name|pool
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
operator|*
name|ctx
operator|->
name|last_out
operator|=
name|cl
expr_stmt|;
name|ctx
operator|->
name|last_out
operator|=
operator|&
name|cl
operator|->
name|next
expr_stmt|;
block|}
block|}
name|ctx
operator|->
name|buf
operator|=
name|NULL
expr_stmt|;
block|}
if|if
condition|(
name|ctx
operator|->
name|out
condition|)
block|{
if|if
condition|(
name|ngx_http_next_body_filter
argument_list|(
name|r
argument_list|,
name|ctx
operator|->
name|out
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|ctx
operator|->
name|busy
operator|==
name|NULL
condition|)
block|{
name|ctx
operator|->
name|busy
operator|=
name|ctx
operator|->
name|out
expr_stmt|;
block|}
else|else
block|{
for|for
control|(
name|tl
operator|=
name|ctx
operator|->
name|busy
init|;
comment|/* void */
condition|;
name|tl
operator|=
name|tl
operator|->
name|next
control|)
block|{
if|if
condition|(
name|tl
operator|->
name|next
operator|==
name|NULL
condition|)
block|{
name|tl
operator|->
name|next
operator|=
name|ctx
operator|->
name|out
expr_stmt|;
break|break;
block|}
block|}
block|}
name|ctx
operator|->
name|out
operator|=
name|NULL
expr_stmt|;
name|ctx
operator|->
name|last_out
operator|=
operator|&
name|ctx
operator|->
name|out
expr_stmt|;
while|while
condition|(
name|ctx
operator|->
name|busy
condition|)
block|{
if|if
condition|(
name|ngx_hunk_size
argument_list|(
name|ctx
operator|->
name|busy
operator|->
name|hunk
argument_list|)
operator|!=
literal|0
condition|)
block|{
break|break;
block|}
if|if
condition|(
name|ctx
operator|->
name|busy
operator|->
name|hunk
operator|->
name|shadow
condition|)
block|{
name|ctx
operator|->
name|busy
operator|->
name|hunk
operator|->
name|shadow
operator|->
name|pos
operator|=
name|ctx
operator|->
name|busy
operator|->
name|hunk
operator|->
name|pos
expr_stmt|;
block|}
name|ctx
operator|->
name|busy
operator|=
name|ctx
operator|->
name|busy
operator|->
name|next
expr_stmt|;
block|}
block|}
return|return
name|NGX_OK
return|;
block|}
end_function

begin_if
if|#
directive|if
literal|0
end_if

begin_comment
unit|static int ngx_http_ssi_copy_opcode(ngx_http_request_t *r,                                     ngx_http_ssi_ctx_t *ctx, void *data) {     ngx_http_ssi_copy_t *copy = data;      ngx_hunk_t   *h;     ngx_chain_t   chain;      h = ctx->incoming->hunk;      if (ctx->looked == 0&& ctx->pos == h->last) {         chain.hunk = h;         chain.next = NULL;          return next_body_filter(r,&chain);     }      if (ctx->hunk == NULL) {         ngx_test_null(ctx->hunk, ngx_calloc_hunk(r->pool), NGX_ERROR);         ctx->hunk->type = h->type& NGX_HUNK_STORAGE;     }       if (h->type& NGX_HUNK_FILE) {         if (copy->start<= h->file_pos) {             ctx->hunk->file_pos = h->file_pos;         } else if (copy->start< h->file_last) {             ctx->hunk->file_pos = copy->file_pos;         }          if (copy->end>= h->file_last) {             ctx->hunk->file_last = h->file_last;         } else if (copy->end> h->file_pos) {         }      }      if (h->type& NGX_HUNK_IN_MEMORY) {         if (copy->start<= ctx->offset + (h->pos - h->start)) {             ctx->hunk->pos = h->pos;         } else if (copy->start< ctx->offset + (h->last - h->start)) {             ctx->hunk->pos = h->start + (copy->start - ctx->offset);         }          if (copy->end>= ctx->offset + (h->last - h->start) {             ctx->hunk->last = h->last;         } else if (copy->end> ctx->offset + (h->pos - h->start)) {             ctx->hunk->last = h->start + (copy->end - ctx->offset);         }     }
comment|/* TODO: NGX_HUNK_FLUSH */
end_comment

begin_comment
unit|if ((h->type& NGX_HUNK_LAST)&& ctx->hunk->last == h->last)
comment|/* LAST */
end_comment

begin_endif
unit|}
endif|#
directive|endif
end_endif

begin_function
DECL|function|ngx_http_ssi_parse (ngx_http_request_t * r,ngx_http_ssi_ctx_t * ctx)
specifier|static
name|ngx_int_t
name|ngx_http_ssi_parse
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_ssi_ctx_t
modifier|*
name|ctx
parameter_list|)
block|{
name|char
modifier|*
name|p
decl_stmt|,
modifier|*
name|last
decl_stmt|,
modifier|*
name|end
decl_stmt|,
name|ch
decl_stmt|;
DECL|enum|__anon2c2a382b0303
enum|enum
block|{
DECL|enumerator|ssi_start_state
name|ssi_start_state
init|=
literal|0
block|,
DECL|enumerator|ssi_tag_state
name|ssi_tag_state
block|,
DECL|enumerator|ssi_comment0_state
name|ssi_comment0_state
block|,
DECL|enumerator|ssi_comment1_state
name|ssi_comment1_state
block|,
DECL|enumerator|ssi_sharp_state
name|ssi_sharp_state
block|,
DECL|enumerator|ssi_precommand_state
name|ssi_precommand_state
block|,
DECL|enumerator|ssi_command_state
name|ssi_command_state
block|,
DECL|enumerator|ssi_preparam_state
name|ssi_preparam_state
block|,
DECL|enumerator|ssi_param_state
name|ssi_param_state
block|,
DECL|enumerator|ssi_preequal_state
name|ssi_preequal_state
block|,
DECL|enumerator|ssi_prevalue_state
name|ssi_prevalue_state
block|,
DECL|enumerator|ssi_double_quoted_value_state
name|ssi_double_quoted_value_state
block|,
DECL|enumerator|ssi_double_quoted_value_quote_state
name|ssi_double_quoted_value_quote_state
block|,
DECL|enumerator|ssi_quoted_value_state
name|ssi_quoted_value_state
block|,
DECL|enumerator|ssi_quoted_value_quote_state
name|ssi_quoted_value_quote_state
block|,
DECL|enumerator|ssi_comment_end0_state
name|ssi_comment_end0_state
block|,
DECL|enumerator|ssi_comment_end1_state
name|ssi_comment_end1_state
block|}
name|state
enum|;
name|state
operator|=
name|ctx
operator|->
name|state
expr_stmt|;
name|last
operator|=
name|ctx
operator|->
name|last
expr_stmt|;
name|p
operator|=
name|ctx
operator|->
name|pos
expr_stmt|;
name|end
operator|=
name|ctx
operator|->
name|buf
operator|->
name|last
expr_stmt|;
while|while
condition|(
name|p
operator|<
name|end
condition|)
block|{
name|ch
operator|=
operator|*
name|p
operator|++
expr_stmt|;
switch|switch
condition|(
name|state
condition|)
block|{
case|case
name|ssi_start_state
case|:
name|last
operator|=
name|NULL
expr_stmt|;
comment|/* a tight loop */
for|for
control|(
init|;
condition|;
control|)
block|{
if|if
condition|(
name|ch
operator|==
literal|'<'
condition|)
block|{
name|state
operator|=
name|ssi_tag_state
expr_stmt|;
name|last
operator|=
name|p
operator|-
literal|1
expr_stmt|;
break|break;
block|}
if|if
condition|(
name|p
operator|==
name|end
condition|)
block|{
name|ctx
operator|->
name|state
operator|=
name|ssi_start_state
expr_stmt|;
name|ctx
operator|->
name|last
operator|=
name|p
expr_stmt|;
name|ctx
operator|->
name|pos
operator|=
name|p
expr_stmt|;
return|return
name|NGX_HTTP_SSI_COPY
return|;
block|}
name|ch
operator|=
operator|*
name|p
operator|++
expr_stmt|;
block|}
break|break;
case|case
name|ssi_tag_state
case|:
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
literal|'!'
case|:
name|state
operator|=
name|ssi_comment0_state
expr_stmt|;
break|break;
case|case
literal|'<'
case|:
name|last
operator|=
name|p
operator|-
literal|1
expr_stmt|;
break|break;
default|default:
name|state
operator|=
name|ssi_start_state
expr_stmt|;
break|break;
block|}
break|break;
case|case
name|ssi_comment0_state
case|:
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
literal|'-'
case|:
name|state
operator|=
name|ssi_comment1_state
expr_stmt|;
break|break;
case|case
literal|'<'
case|:
name|last
operator|=
name|p
operator|-
literal|1
expr_stmt|;
name|state
operator|=
name|ssi_tag_state
expr_stmt|;
break|break;
default|default:
name|state
operator|=
name|ssi_start_state
expr_stmt|;
break|break;
block|}
break|break;
case|case
name|ssi_comment1_state
case|:
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
literal|'-'
case|:
name|state
operator|=
name|ssi_sharp_state
expr_stmt|;
break|break;
case|case
literal|'<'
case|:
name|last
operator|=
name|p
operator|-
literal|1
expr_stmt|;
name|state
operator|=
name|ssi_tag_state
expr_stmt|;
break|break;
default|default:
name|state
operator|=
name|ssi_start_state
expr_stmt|;
break|break;
block|}
break|break;
case|case
name|ssi_sharp_state
case|:
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
literal|'#'
case|:
name|ctx
operator|->
name|state
operator|=
name|ssi_precommand_state
expr_stmt|;
name|ctx
operator|->
name|last
operator|=
name|last
expr_stmt|;
name|ctx
operator|->
name|pos
operator|=
name|p
expr_stmt|;
return|return
name|NGX_HTTP_SSI_COPY
return|;
case|case
literal|'<'
case|:
name|last
operator|=
name|p
operator|-
literal|1
expr_stmt|;
name|state
operator|=
name|ssi_tag_state
expr_stmt|;
break|break;
default|default:
name|state
operator|=
name|ssi_start_state
expr_stmt|;
break|break;
block|}
break|break;
case|case
name|ssi_precommand_state
case|:
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
literal|' '
case|:
case|case
name|CR
case|:
case|case
name|LF
case|:
case|case
literal|'\t'
case|:
break|break;
default|default:
name|ctx
operator|->
name|command
operator|.
name|data
operator|=
name|ngx_palloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
name|NGX_HTTP_SSI_COMMAND_LEN
operator|+
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|ctx
operator|->
name|command
operator|.
name|data
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|ctx
operator|->
name|command
operator|.
name|data
index|[
literal|0
index|]
operator|=
name|ch
expr_stmt|;
name|ctx
operator|->
name|command
operator|.
name|len
operator|=
literal|1
expr_stmt|;
name|state
operator|=
name|ssi_command_state
expr_stmt|;
break|break;
block|}
break|break;
case|case
name|ssi_command_state
case|:
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
literal|' '
case|:
case|case
name|CR
case|:
case|case
name|LF
case|:
case|case
literal|'\t'
case|:
name|ctx
operator|->
name|command
operator|.
name|data
index|[
name|ctx
operator|->
name|command
operator|.
name|len
index|]
operator|=
literal|0
expr_stmt|;
name|state
operator|=
name|ssi_preparam_state
expr_stmt|;
break|break;
case|case
literal|'-'
case|:
name|ctx
operator|->
name|command
operator|.
name|data
index|[
name|ctx
operator|->
name|command
operator|.
name|len
index|]
operator|=
literal|0
expr_stmt|;
name|state
operator|=
name|ssi_comment_end0_state
expr_stmt|;
break|break;
default|default:
if|if
condition|(
name|ctx
operator|->
name|command
operator|.
name|len
operator|>=
name|NGX_HTTP_SSI_COMMAND_LEN
condition|)
block|{
return|return
name|NGX_HTTP_SSI_INVALID_COMMAND
return|;
block|}
name|ctx
operator|->
name|command
operator|.
name|data
index|[
name|ctx
operator|->
name|command
operator|.
name|len
operator|++
index|]
operator|=
name|ch
expr_stmt|;
block|}
break|break;
case|case
name|ssi_preparam_state
case|:
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
literal|' '
case|:
case|case
name|CR
case|:
case|case
name|LF
case|:
case|case
literal|'\t'
case|:
break|break;
case|case
literal|'-'
case|:
name|state
operator|=
name|ssi_comment_end0_state
expr_stmt|;
break|break;
default|default:
if|if
condition|(
operator|!
operator|(
name|ctx
operator|->
name|param
operator|=
name|ngx_push_array
argument_list|(
operator|&
name|ctx
operator|->
name|params
argument_list|)
operator|)
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|ctx
operator|->
name|param
operator|->
name|key
operator|.
name|data
operator|=
name|ngx_palloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
name|NGX_HTTP_SSI_PARAM_LEN
operator|+
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|ctx
operator|->
name|param
operator|->
name|key
operator|.
name|data
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|ctx
operator|->
name|param
operator|->
name|key
operator|.
name|data
index|[
literal|0
index|]
operator|=
name|ch
expr_stmt|;
name|ctx
operator|->
name|param
operator|->
name|key
operator|.
name|len
operator|=
literal|1
expr_stmt|;
name|ctx
operator|->
name|param
operator|->
name|value
operator|.
name|data
operator|=
name|ngx_palloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
name|ctx
operator|->
name|value_len
operator|+
literal|1
argument_list|)
expr_stmt|;
if|if
condition|(
name|ctx
operator|->
name|param
operator|->
name|value
operator|.
name|data
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|ctx
operator|->
name|param
operator|->
name|value
operator|.
name|len
operator|=
literal|0
expr_stmt|;
name|state
operator|=
name|ssi_param_state
expr_stmt|;
break|break;
block|}
break|break;
case|case
name|ssi_param_state
case|:
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
literal|' '
case|:
case|case
name|CR
case|:
case|case
name|LF
case|:
case|case
literal|'\t'
case|:
name|ctx
operator|->
name|param
operator|->
name|key
operator|.
name|data
index|[
name|ctx
operator|->
name|param
operator|->
name|key
operator|.
name|len
index|]
operator|=
literal|0
expr_stmt|;
name|state
operator|=
name|ssi_preequal_state
expr_stmt|;
break|break;
case|case
literal|'='
case|:
name|ctx
operator|->
name|param
operator|->
name|key
operator|.
name|data
index|[
name|ctx
operator|->
name|param
operator|->
name|key
operator|.
name|len
index|]
operator|=
literal|0
expr_stmt|;
name|state
operator|=
name|ssi_prevalue_state
expr_stmt|;
break|break;
default|default:
if|if
condition|(
name|ctx
operator|->
name|param
operator|->
name|key
operator|.
name|len
operator|>=
name|NGX_HTTP_SSI_PARAM_LEN
condition|)
block|{
return|return
name|NGX_HTTP_SSI_INVALID_PARAM
return|;
block|}
name|ctx
operator|->
name|param
operator|->
name|key
operator|.
name|data
index|[
name|ctx
operator|->
name|param
operator|->
name|key
operator|.
name|len
operator|++
index|]
operator|=
name|ch
expr_stmt|;
block|}
break|break;
case|case
name|ssi_preequal_state
case|:
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
literal|' '
case|:
case|case
name|CR
case|:
case|case
name|LF
case|:
case|case
literal|'\t'
case|:
break|break;
case|case
literal|'='
case|:
name|state
operator|=
name|ssi_prevalue_state
expr_stmt|;
break|break;
default|default:
return|return
name|NGX_HTTP_SSI_INVALID_PARAM
return|;
block|}
break|break;
case|case
name|ssi_prevalue_state
case|:
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
literal|' '
case|:
case|case
name|CR
case|:
case|case
name|LF
case|:
case|case
literal|'\t'
case|:
break|break;
case|case
literal|'"'
case|:
name|state
operator|=
name|ssi_double_quoted_value_state
expr_stmt|;
break|break;
case|case
literal|'\''
case|:
name|state
operator|=
name|ssi_quoted_value_state
expr_stmt|;
break|break;
default|default:
return|return
name|NGX_HTTP_SSI_INVALID_VALUE
return|;
block|}
break|break;
case|case
name|ssi_double_quoted_value_state
case|:
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
literal|'\\'
case|:
name|state
operator|=
name|ssi_double_quoted_value_quote_state
expr_stmt|;
break|break;
case|case
literal|'"'
case|:
name|state
operator|=
name|ssi_preparam_state
expr_stmt|;
break|break;
default|default:
if|if
condition|(
name|ctx
operator|->
name|param
operator|->
name|value
operator|.
name|len
operator|>=
name|ctx
operator|->
name|value_len
condition|)
block|{
return|return
name|NGX_HTTP_SSI_LONG_VALUE
return|;
block|}
name|ctx
operator|->
name|param
operator|->
name|value
operator|.
name|data
index|[
name|ctx
operator|->
name|param
operator|->
name|value
operator|.
name|len
operator|++
index|]
operator|=
name|ch
expr_stmt|;
block|}
break|break;
case|case
name|ssi_double_quoted_value_quote_state
case|:
if|if
condition|(
name|ctx
operator|->
name|param
operator|->
name|value
operator|.
name|len
operator|>=
name|ctx
operator|->
name|value_len
condition|)
block|{
return|return
name|NGX_HTTP_SSI_LONG_VALUE
return|;
block|}
name|ctx
operator|->
name|param
operator|->
name|value
operator|.
name|data
index|[
name|ctx
operator|->
name|param
operator|->
name|value
operator|.
name|len
operator|++
index|]
operator|=
name|ch
expr_stmt|;
name|state
operator|=
name|ssi_double_quoted_value_state
expr_stmt|;
break|break;
case|case
name|ssi_quoted_value_state
case|:
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
literal|'\\'
case|:
name|state
operator|=
name|ssi_quoted_value_quote_state
expr_stmt|;
break|break;
case|case
literal|'\''
case|:
name|state
operator|=
name|ssi_preparam_state
expr_stmt|;
break|break;
default|default:
if|if
condition|(
name|ctx
operator|->
name|param
operator|->
name|value
operator|.
name|len
operator|>=
name|ctx
operator|->
name|value_len
condition|)
block|{
return|return
name|NGX_HTTP_SSI_LONG_VALUE
return|;
block|}
name|ctx
operator|->
name|param
operator|->
name|value
operator|.
name|data
index|[
name|ctx
operator|->
name|param
operator|->
name|value
operator|.
name|len
operator|++
index|]
operator|=
name|ch
expr_stmt|;
block|}
break|break;
case|case
name|ssi_quoted_value_quote_state
case|:
if|if
condition|(
name|ctx
operator|->
name|param
operator|->
name|value
operator|.
name|len
operator|>=
name|ctx
operator|->
name|value_len
condition|)
block|{
return|return
name|NGX_HTTP_SSI_LONG_VALUE
return|;
block|}
name|ctx
operator|->
name|param
operator|->
name|value
operator|.
name|data
index|[
name|ctx
operator|->
name|param
operator|->
name|value
operator|.
name|len
operator|++
index|]
operator|=
name|ch
expr_stmt|;
name|state
operator|=
name|ssi_quoted_value_state
expr_stmt|;
break|break;
case|case
name|ssi_comment_end0_state
case|:
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
literal|'-'
case|:
name|state
operator|=
name|ssi_comment_end1_state
expr_stmt|;
break|break;
default|default:
return|return
name|NGX_HTTP_SSI_INVALID_COMMAND
return|;
block|}
break|break;
case|case
name|ssi_comment_end1_state
case|:
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
literal|'>'
case|:
name|ctx
operator|->
name|state
operator|=
name|ssi_start_state
expr_stmt|;
name|ctx
operator|->
name|start
operator|=
name|p
expr_stmt|;
name|ctx
operator|->
name|pos
operator|=
name|p
expr_stmt|;
return|return
name|NGX_OK
return|;
default|default:
return|return
name|NGX_HTTP_SSI_INVALID_COMMAND
return|;
block|}
break|break;
block|}
block|}
name|ctx
operator|->
name|state
operator|=
name|state
expr_stmt|;
name|ctx
operator|->
name|last
operator|=
name|last
expr_stmt|;
name|ctx
operator|->
name|pos
operator|=
name|p
expr_stmt|;
return|return
name|NGX_HTTP_SSI_COPY
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_ssi_create_conf (ngx_conf_t * cf)
specifier|static
name|void
modifier|*
name|ngx_http_ssi_create_conf
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
block|{
name|ngx_http_ssi_conf_t
modifier|*
name|conf
decl_stmt|;
if|if
condition|(
operator|!
operator|(
name|conf
operator|=
name|ngx_pcalloc
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|ngx_http_ssi_conf_t
argument_list|)
argument_list|)
operator|)
condition|)
block|{
return|return
name|NGX_CONF_ERROR
return|;
block|}
name|conf
operator|->
name|enable
operator|=
name|NGX_CONF_UNSET
expr_stmt|;
return|return
name|conf
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_ssi_merge_conf (ngx_conf_t * cf,void * parent,void * child)
specifier|static
name|char
modifier|*
name|ngx_http_ssi_merge_conf
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
name|ngx_http_ssi_conf_t
modifier|*
name|prev
init|=
name|parent
decl_stmt|;
name|ngx_http_ssi_conf_t
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
return|return
name|NGX_CONF_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_http_ssi_filter_init (ngx_cycle_t * cycle)
specifier|static
name|int
name|ngx_http_ssi_filter_init
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
block|{
name|ngx_http_next_header_filter
operator|=
name|ngx_http_top_header_filter
expr_stmt|;
name|ngx_http_top_header_filter
operator|=
name|ngx_http_ssi_header_filter
expr_stmt|;
name|ngx_http_next_body_filter
operator|=
name|ngx_http_top_body_filter
expr_stmt|;
name|ngx_http_top_body_filter
operator|=
name|ngx_http_ssi_body_filter
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

end_unit

