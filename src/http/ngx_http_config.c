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
file|<ngx_config_file.h>
end_include

begin_include
include|#
directive|include
file|<ngx_http.h>
end_include

begin_include
include|#
directive|include
file|<ngx_http_write_filter.h>
end_include

begin_include
include|#
directive|include
file|<ngx_http_output_filter.h>
end_include

begin_include
include|#
directive|include
file|<ngx_http_index_handler.h>
end_include

begin_decl_stmt
DECL|variable|ngx_max_module
name|int
name|ngx_max_module
decl_stmt|;
end_decl_stmt

begin_function_decl
DECL|variable|ngx_http_top_header_filter
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

begin_comment
comment|/* STUB: gobal srv and loc conf */
end_comment

begin_decl_stmt
DECL|variable|ngx_srv_conf
name|void
modifier|*
modifier|*
name|ngx_srv_conf
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_loc_conf
name|void
modifier|*
modifier|*
name|ngx_loc_conf
decl_stmt|;
end_decl_stmt

begin_if
if|#
directive|if
literal|0
end_if

begin_if
unit|int ngx_http_block(ngx_conf_t *cf) {     ngx_http_conf_ctx_t  *ctx;      ngx_test_null(ctx,                   ngx_pcalloc(cf->pool, sizeof(ngx_http_conf_ctx_t)),                   NGX_ERROR);
if|#
directive|if
literal|0
end_if

begin_comment
comment|/* null server config */
end_comment

begin_endif
unit|ngx_test_null(ctx->srv_conf,                   ngx_pcalloc(cf->pool, sizeof(void *) * ngx_max_module),                   NGX_ERROR);
endif|#
directive|endif
end_endif

begin_comment
comment|/* null location config */
end_comment

begin_if
unit|ngx_test_null(ctx->loc_conf,                   ngx_pcalloc(cf->pool, sizeof(void *) * ngx_max_module),                   NGX_ERROR);      for (i = 0; modules[i]; i++) {
if|#
directive|if
literal|0
end_if

begin_endif
unit|if (modules[i]->create_srv_conf)             ngx_test_null(ctx->srv_conf[i],                           modules[i]->create_srv_conf(cf->pool),                           NGX_ERROR);
endif|#
directive|endif
end_endif

begin_comment
unit|if (modules[i]->create_loc_conf)             ngx_test_null(ctx->loc_conf[i],                           modules[i]->create_loc_conf(cf->pool),                           NGX_ERROR);     }      cf->ctx = ctx;     return ngx_conf_parse(cf); }  int ngx_server_block(ngx_conf_t *cf) {     ngx_http_conf_ctx_t  *ctx, *prev;      ngx_test_null(ctx,                   ngx_pcalloc(cf->pool, sizeof(ngx_http_conf_ctx_t)),                   NGX_ERROR);
comment|/* server config */
end_comment

begin_comment
unit|ngx_test_null(ctx->srv_conf,                   ngx_pcalloc(cf->pool, sizeof(void *) * ngx_max_module),                   NGX_ERROR);
comment|/* server location config */
end_comment

begin_if
unit|ngx_test_null(ctx->loc_conf,                   ngx_pcalloc(cf->pool, sizeof(void *) * ngx_max_module),                   NGX_ERROR);       for (i = 0; modules[i]; i++) {         if (modules[i]->create_srv_conf)             ngx_test_null(ctx->srv_conf[i],                           modules[i]->create_srv_conf(cf->pool),                           NGX_ERROR);          if (modules[i]->create_loc_conf)             ngx_test_null(ctx->loc_conf[i],                           modules[i]->create_loc_conf(cf->pool),                           NGX_ERROR);     }      prev = cf->ctx;     cf->ctx = ctx;     rc = ngx_conf_parse(cf);     cf->ctx = prev;      if (loc == NGX_ERROR)         return NGX_ERROR;      for (i = 0; modules[i]; i++) {
if|#
directive|if
literal|0
end_if

begin_endif
unit|if (modules[i]->merge_srv_conf)             if (modules[i]->merge_srv_conf(cf->pool,                                            prev->srv_conf, ctx->srv_conf)                                                                   == NGX_ERROR)                 return NGX_ERROR;
endif|#
directive|endif
end_endif

begin_endif
unit|if (modules[i]->init_srv_conf)             if (modules[i]->init_srv_conf(cf->pool, ctx->srv_conf) == NGX_ERROR)                 return NGX_ERROR;          if (modules[i]->merge_loc_conf)             if (modules[i]->merge_loc_conf(cf->pool,                                            prev->loc_conf, ctx->loc_conf)                                                                   == NGX_ERROR)                 return NGX_ERROR;              for (array) {                 if (modules[i]->merge_loc_conf(cf->pool,                                                ctx->loc_conf, loc->loc_conf)                                                                   == NGX_ERROR)                 return NGX_ERROR;             }         }     }      return NGX_OK; }  int ngx_location_block(ngx_conf_t *cf) {     ngx_http_conf_ctx_t  *ctx, *prev;      ngx_test_null(ctx, ngx_pcalloc(cf->pool, sizeof(ngx_http_conf_ctx_t)),                   NGX_ERROR);      ctx->srv_conf = cf->ctx->srv_conf;      ngx_test_null(ctx->loc_conf,                   ngx_pcalloc(cf->pool, sizeof(void *) * ngx_max_module),                   NGX_ERROR);      for (i = 0; modules[i]; i++) {         if (modules[i]->create_loc_conf)             ngx_test_null(ctx->loc_conf[i],                           modules[i]->create_loc_conf(cf->pool),                           NGX_ERROR);          if (ngx_http_core_module.index == i)             ctx->loc_conf[i].location = cf->args[0];     }      push      return ngx_conf_parse(cf); }
endif|#
directive|endif
end_endif

begin_function
DECL|function|ngx_http_config_modules (ngx_pool_t * pool,ngx_http_module_t ** modules)
name|int
name|ngx_http_config_modules
parameter_list|(
name|ngx_pool_t
modifier|*
name|pool
parameter_list|,
name|ngx_http_module_t
modifier|*
modifier|*
name|modules
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|modules
index|[
name|i
index|]
condition|;
name|i
operator|++
control|)
block|{
name|modules
index|[
name|i
index|]
operator|->
name|index
operator|=
name|i
expr_stmt|;
block|}
name|ngx_max_module
operator|=
name|i
expr_stmt|;
name|ngx_test_null
argument_list|(
name|ngx_srv_conf
argument_list|,
name|ngx_pcalloc
argument_list|(
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|void
operator|*
argument_list|)
operator|*
name|ngx_max_module
argument_list|)
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
name|ngx_test_null
argument_list|(
name|ngx_loc_conf
argument_list|,
name|ngx_pcalloc
argument_list|(
name|pool
argument_list|,
sizeof|sizeof
argument_list|(
name|void
operator|*
argument_list|)
operator|*
name|ngx_max_module
argument_list|)
argument_list|,
name|NGX_ERROR
argument_list|)
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|modules
index|[
name|i
index|]
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|modules
index|[
name|i
index|]
operator|->
name|create_srv_conf
condition|)
name|ngx_srv_conf
index|[
name|i
index|]
operator|=
name|modules
index|[
name|i
index|]
operator|->
name|create_srv_conf
argument_list|(
name|pool
argument_list|)
expr_stmt|;
if|if
condition|(
name|modules
index|[
name|i
index|]
operator|->
name|create_loc_conf
condition|)
name|ngx_loc_conf
index|[
name|i
index|]
operator|=
name|modules
index|[
name|i
index|]
operator|->
name|create_loc_conf
argument_list|(
name|pool
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|ngx_http_init_modules (ngx_pool_t * pool,ngx_http_module_t ** modules)
name|int
name|ngx_http_init_modules
parameter_list|(
name|ngx_pool_t
modifier|*
name|pool
parameter_list|,
name|ngx_http_module_t
modifier|*
modifier|*
name|modules
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|modules
index|[
name|i
index|]
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|modules
index|[
name|i
index|]
operator|->
name|init_module
condition|)
name|modules
index|[
name|i
index|]
operator|->
name|init_module
argument_list|(
name|pool
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_function
DECL|function|ngx_http_init_filters (ngx_pool_t * pool,ngx_http_module_t ** modules)
name|int
name|ngx_http_init_filters
parameter_list|(
name|ngx_pool_t
modifier|*
name|pool
parameter_list|,
name|ngx_http_module_t
modifier|*
modifier|*
name|modules
parameter_list|)
block|{
name|int
name|i
decl_stmt|;
name|int
function_decl|(
modifier|*
name|ohf
function_decl|)
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
function_decl|;
name|int
function_decl|(
modifier|*
name|obf
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
name|ohf
operator|=
name|NULL
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|modules
index|[
name|i
index|]
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|modules
index|[
name|i
index|]
operator|->
name|output_header_filter
condition|)
block|{
name|modules
index|[
name|i
index|]
operator|->
name|next_output_header_filter
operator|=
name|ohf
expr_stmt|;
name|ohf
operator|=
name|modules
index|[
name|i
index|]
operator|->
name|output_header_filter
expr_stmt|;
block|}
block|}
name|ngx_http_top_header_filter
operator|=
name|ohf
expr_stmt|;
name|obf
operator|=
name|NULL
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|modules
index|[
name|i
index|]
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|modules
index|[
name|i
index|]
operator|->
name|output_body_filter
condition|)
block|{
name|modules
index|[
name|i
index|]
operator|->
name|next_output_body_filter
operator|=
name|obf
expr_stmt|;
name|obf
operator|=
name|modules
index|[
name|i
index|]
operator|->
name|output_body_filter
expr_stmt|;
block|}
block|}
block|}
end_function

begin_comment
comment|/* STUB */
end_comment

begin_macro
DECL|function|ngx_http_output_filter_set_stub (ngx_pool_t * pool,ngx_http_module_t ** modules)
name|ngx_http_output_filter_set_stub
argument_list|(
argument|ngx_pool_t *pool
argument_list|,
argument|ngx_http_module_t **modules
argument_list|)
end_macro

begin_block
block|{
name|int
name|i
decl_stmt|;
name|ngx_command_t
modifier|*
name|cmd
decl_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|modules
index|[
name|i
index|]
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|modules
index|[
name|i
index|]
operator|==
operator|&
name|ngx_http_output_filter_module
condition|)
block|{
for|for
control|(
name|cmd
operator|=
name|modules
index|[
name|i
index|]
operator|->
name|commands
init|;
name|cmd
operator|->
name|name
condition|;
name|cmd
operator|++
control|)
block|{
if|if
condition|(
name|strcmp
argument_list|(
name|cmd
operator|->
name|name
argument_list|,
literal|"output_buffer"
argument_list|)
operator|==
literal|0
condition|)
block|{
name|cmd
operator|->
name|set
argument_list|(
name|ngx_loc_conf
index|[
name|i
index|]
argument_list|,
name|cmd
operator|->
name|offset
argument_list|,
literal|"32768"
argument_list|)
expr_stmt|;
block|}
block|}
block|}
block|}
block|}
end_block

begin_macro
DECL|function|ngx_http_write_filter_set_stub (ngx_pool_t * pool,ngx_http_module_t ** modules)
name|ngx_http_write_filter_set_stub
argument_list|(
argument|ngx_pool_t *pool
argument_list|,
argument|ngx_http_module_t **modules
argument_list|)
end_macro

begin_block
block|{
name|int
name|i
decl_stmt|;
name|ngx_command_t
modifier|*
name|cmd
decl_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|modules
index|[
name|i
index|]
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|modules
index|[
name|i
index|]
operator|==
operator|&
name|ngx_http_write_filter_module
condition|)
block|{
for|for
control|(
name|cmd
operator|=
name|modules
index|[
name|i
index|]
operator|->
name|commands
init|;
name|cmd
operator|->
name|name
condition|;
name|cmd
operator|++
control|)
block|{
if|if
condition|(
name|strcmp
argument_list|(
name|cmd
operator|->
name|name
argument_list|,
literal|"write_buffer"
argument_list|)
operator|==
literal|0
condition|)
block|{
name|cmd
operator|->
name|set
argument_list|(
name|ngx_loc_conf
index|[
name|i
index|]
argument_list|,
name|cmd
operator|->
name|offset
argument_list|,
literal|"1500"
argument_list|)
expr_stmt|;
block|}
block|}
block|}
block|}
block|}
end_block

begin_macro
DECL|function|ngx_http_index_set_stub (ngx_pool_t * pool,ngx_http_module_t ** modules)
name|ngx_http_index_set_stub
argument_list|(
argument|ngx_pool_t *pool
argument_list|,
argument|ngx_http_module_t **modules
argument_list|)
end_macro

begin_block
block|{
name|int
name|i
decl_stmt|;
name|ngx_str_t
name|index
decl_stmt|;
name|ngx_command_t
modifier|*
name|cmd
decl_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|modules
index|[
name|i
index|]
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|modules
index|[
name|i
index|]
operator|==
operator|&
name|ngx_http_index_module
condition|)
block|{
for|for
control|(
name|cmd
operator|=
name|modules
index|[
name|i
index|]
operator|->
name|commands
init|;
name|cmd
operator|->
name|name
condition|;
name|cmd
operator|++
control|)
block|{
if|if
condition|(
name|strcmp
argument_list|(
name|cmd
operator|->
name|name
argument_list|,
literal|"index"
argument_list|)
operator|==
literal|0
condition|)
block|{
name|index
operator|.
name|len
operator|=
sizeof|sizeof
argument_list|(
literal|"index.html"
argument_list|)
operator|-
literal|1
expr_stmt|;
name|index
operator|.
name|data
operator|=
literal|"index.html"
expr_stmt|;
name|cmd
operator|->
name|set
argument_list|(
name|pool
argument_list|,
name|ngx_loc_conf
index|[
name|i
index|]
argument_list|,
operator|&
name|index
argument_list|)
expr_stmt|;
block|}
block|}
block|}
block|}
block|}
end_block

end_unit

