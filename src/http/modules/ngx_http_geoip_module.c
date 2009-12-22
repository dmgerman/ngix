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

begin_include
include|#
directive|include
file|<GeoIP.h>
end_include

begin_include
include|#
directive|include
file|<GeoIPCity.h>
end_include

begin_typedef
DECL|struct|__anon2aab931f0108
typedef|typedef
struct|struct
block|{
DECL|member|country
name|GeoIP
modifier|*
name|country
decl_stmt|;
DECL|member|city
name|GeoIP
modifier|*
name|city
decl_stmt|;
DECL|typedef|ngx_http_geoip_conf_t
block|}
name|ngx_http_geoip_conf_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon2aab931f0208
typedef|typedef
struct|struct
block|{
DECL|member|name
name|ngx_str_t
modifier|*
name|name
decl_stmt|;
DECL|member|data
name|uintptr_t
name|data
decl_stmt|;
DECL|typedef|ngx_http_geoip_var_t
block|}
name|ngx_http_geoip_var_t
typedef|;
end_typedef

begin_typedef
DECL|typedef|ngx_http_geoip_variable_handler_pt
typedef|typedef
specifier|const
name|char
modifier|*
function_decl|(
modifier|*
name|ngx_http_geoip_variable_handler_pt
function_decl|)
parameter_list|(
name|GeoIP
modifier|*
parameter_list|,
name|u_long
name|addr
parameter_list|)
function_decl|;
end_typedef

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_geoip_country_variable
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_variable_value_t
modifier|*
name|v
parameter_list|,
name|uintptr_t
name|data
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_geoip_city_variable
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_variable_value_t
modifier|*
name|v
parameter_list|,
name|uintptr_t
name|data
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_geoip_city_float_variable
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_variable_value_t
modifier|*
name|v
parameter_list|,
name|uintptr_t
name|data
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|GeoIPRecord
modifier|*
name|ngx_http_geoip_get_city_record
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|ngx_int_t
name|ngx_http_geoip_add_variables
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
name|ngx_http_geoip_create_conf
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
name|ngx_http_geoip_country
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
name|ngx_http_geoip_city
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
name|void
name|ngx_http_geoip_cleanup
parameter_list|(
name|void
modifier|*
name|data
parameter_list|)
function_decl|;
end_function_decl

begin_decl_stmt
DECL|variable|ngx_http_geoip_commands
specifier|static
name|ngx_command_t
name|ngx_http_geoip_commands
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"geoip_country"
argument_list|)
block|,
name|NGX_HTTP_MAIN_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_http_geoip_country
block|,
name|NGX_HTTP_MAIN_CONF_OFFSET
block|,
literal|0
block|,
name|NULL
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"geoip_city"
argument_list|)
block|,
name|NGX_HTTP_MAIN_CONF
operator||
name|NGX_CONF_TAKE1
block|,
name|ngx_http_geoip_city
block|,
name|NGX_HTTP_MAIN_CONF_OFFSET
block|,
literal|0
block|,
name|NULL
block|}
block|,
name|ngx_null_command
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_geoip_module_ctx
specifier|static
name|ngx_http_module_t
name|ngx_http_geoip_module_ctx
init|=
block|{
name|ngx_http_geoip_add_variables
block|,
comment|/* preconfiguration */
name|NULL
block|,
comment|/* postconfiguration */
name|ngx_http_geoip_create_conf
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
name|NULL
block|,
comment|/* create location configuration */
name|NULL
comment|/* merge location configuration */
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_http_geoip_module
name|ngx_module_t
name|ngx_http_geoip_module
init|=
block|{
name|NGX_MODULE_V1
block|,
operator|&
name|ngx_http_geoip_module_ctx
block|,
comment|/* module context */
name|ngx_http_geoip_commands
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

begin_decl_stmt
DECL|variable|ngx_http_geoip_vars
specifier|static
name|ngx_http_variable_t
name|ngx_http_geoip_vars
index|[]
init|=
block|{
block|{
name|ngx_string
argument_list|(
literal|"geoip_country_code"
argument_list|)
block|,
name|NULL
block|,
name|ngx_http_geoip_country_variable
block|,
operator|(
name|uintptr_t
operator|)
name|GeoIP_country_code_by_ipnum
block|,
literal|0
block|,
literal|0
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"geoip_country_code3"
argument_list|)
block|,
name|NULL
block|,
name|ngx_http_geoip_country_variable
block|,
operator|(
name|uintptr_t
operator|)
name|GeoIP_country_code3_by_ipnum
block|,
literal|0
block|,
literal|0
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"geoip_country_name"
argument_list|)
block|,
name|NULL
block|,
name|ngx_http_geoip_country_variable
block|,
operator|(
name|uintptr_t
operator|)
name|GeoIP_country_name_by_ipnum
block|,
literal|0
block|,
literal|0
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"geoip_city_continent_code"
argument_list|)
block|,
name|NULL
block|,
name|ngx_http_geoip_city_variable
block|,
name|offsetof
argument_list|(
name|GeoIPRecord
argument_list|,
name|continent_code
argument_list|)
block|,
literal|0
block|,
literal|0
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"geoip_city_country_code"
argument_list|)
block|,
name|NULL
block|,
name|ngx_http_geoip_city_variable
block|,
name|offsetof
argument_list|(
name|GeoIPRecord
argument_list|,
name|country_code
argument_list|)
block|,
literal|0
block|,
literal|0
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"geoip_city_country_code3"
argument_list|)
block|,
name|NULL
block|,
name|ngx_http_geoip_city_variable
block|,
name|offsetof
argument_list|(
name|GeoIPRecord
argument_list|,
name|country_code3
argument_list|)
block|,
literal|0
block|,
literal|0
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"geoip_city_country_name"
argument_list|)
block|,
name|NULL
block|,
name|ngx_http_geoip_city_variable
block|,
name|offsetof
argument_list|(
name|GeoIPRecord
argument_list|,
name|country_name
argument_list|)
block|,
literal|0
block|,
literal|0
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"geoip_region"
argument_list|)
block|,
name|NULL
block|,
name|ngx_http_geoip_city_variable
block|,
name|offsetof
argument_list|(
name|GeoIPRecord
argument_list|,
name|region
argument_list|)
block|,
literal|0
block|,
literal|0
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"geoip_city"
argument_list|)
block|,
name|NULL
block|,
name|ngx_http_geoip_city_variable
block|,
name|offsetof
argument_list|(
name|GeoIPRecord
argument_list|,
name|city
argument_list|)
block|,
literal|0
block|,
literal|0
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"geoip_postal_code"
argument_list|)
block|,
name|NULL
block|,
name|ngx_http_geoip_city_variable
block|,
name|offsetof
argument_list|(
name|GeoIPRecord
argument_list|,
name|postal_code
argument_list|)
block|,
literal|0
block|,
literal|0
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"geoip_latitude"
argument_list|)
block|,
name|NULL
block|,
name|ngx_http_geoip_city_float_variable
block|,
name|offsetof
argument_list|(
name|GeoIPRecord
argument_list|,
name|latitude
argument_list|)
block|,
literal|0
block|,
literal|0
block|}
block|,
block|{
name|ngx_string
argument_list|(
literal|"geoip_longitude"
argument_list|)
block|,
name|NULL
block|,
name|ngx_http_geoip_city_float_variable
block|,
name|offsetof
argument_list|(
name|GeoIPRecord
argument_list|,
name|longitude
argument_list|)
block|,
literal|0
block|,
literal|0
block|}
block|,
block|{
name|ngx_null_string
block|,
name|NULL
block|,
name|NULL
block|,
literal|0
block|,
literal|0
block|,
literal|0
block|}
block|}
decl_stmt|;
end_decl_stmt

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_geoip_country_variable (ngx_http_request_t * r,ngx_http_variable_value_t * v,uintptr_t data)
name|ngx_http_geoip_country_variable
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_variable_value_t
modifier|*
name|v
parameter_list|,
name|uintptr_t
name|data
parameter_list|)
block|{
name|ngx_http_geoip_variable_handler_pt
name|handler
init|=
operator|(
name|ngx_http_geoip_variable_handler_pt
operator|)
name|data
decl_stmt|;
name|u_long
name|addr
decl_stmt|;
specifier|const
name|char
modifier|*
name|val
decl_stmt|;
name|struct
name|sockaddr_in
modifier|*
name|sin
decl_stmt|;
name|ngx_http_geoip_conf_t
modifier|*
name|gcf
decl_stmt|;
name|gcf
operator|=
name|ngx_http_get_module_main_conf
argument_list|(
name|r
argument_list|,
name|ngx_http_geoip_module
argument_list|)
expr_stmt|;
if|if
condition|(
name|gcf
operator|->
name|country
operator|==
name|NULL
condition|)
block|{
goto|goto
name|not_found
goto|;
block|}
if|if
condition|(
name|r
operator|->
name|connection
operator|->
name|sockaddr
operator|->
name|sa_family
operator|!=
name|AF_INET
condition|)
block|{
goto|goto
name|not_found
goto|;
block|}
name|sin
operator|=
operator|(
expr|struct
name|sockaddr_in
operator|*
operator|)
name|r
operator|->
name|connection
operator|->
name|sockaddr
expr_stmt|;
name|addr
operator|=
name|ntohl
argument_list|(
name|sin
operator|->
name|sin_addr
operator|.
name|s_addr
argument_list|)
expr_stmt|;
name|val
operator|=
name|handler
argument_list|(
name|gcf
operator|->
name|country
argument_list|,
name|addr
argument_list|)
expr_stmt|;
if|if
condition|(
name|val
operator|==
name|NULL
condition|)
block|{
goto|goto
name|not_found
goto|;
block|}
name|v
operator|->
name|len
operator|=
name|ngx_strlen
argument_list|(
name|val
argument_list|)
expr_stmt|;
name|v
operator|->
name|valid
operator|=
literal|1
expr_stmt|;
name|v
operator|->
name|no_cacheable
operator|=
literal|0
expr_stmt|;
name|v
operator|->
name|not_found
operator|=
literal|0
expr_stmt|;
name|v
operator|->
name|data
operator|=
operator|(
name|u_char
operator|*
operator|)
name|val
expr_stmt|;
return|return
name|NGX_OK
return|;
name|not_found
label|:
name|v
operator|->
name|not_found
operator|=
literal|1
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_geoip_city_variable (ngx_http_request_t * r,ngx_http_variable_value_t * v,uintptr_t data)
name|ngx_http_geoip_city_variable
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_variable_value_t
modifier|*
name|v
parameter_list|,
name|uintptr_t
name|data
parameter_list|)
block|{
name|char
modifier|*
name|val
decl_stmt|;
name|size_t
name|len
decl_stmt|;
name|GeoIPRecord
modifier|*
name|gr
decl_stmt|;
name|gr
operator|=
name|ngx_http_geoip_get_city_record
argument_list|(
name|r
argument_list|)
expr_stmt|;
if|if
condition|(
name|gr
operator|==
name|NULL
condition|)
block|{
goto|goto
name|not_found
goto|;
block|}
name|val
operator|=
operator|*
operator|(
name|char
operator|*
operator|*
operator|)
operator|(
operator|(
name|char
operator|*
operator|)
name|gr
operator|+
name|data
operator|)
expr_stmt|;
if|if
condition|(
name|val
operator|==
name|NULL
condition|)
block|{
goto|goto
name|no_value
goto|;
block|}
name|len
operator|=
name|ngx_strlen
argument_list|(
name|val
argument_list|)
expr_stmt|;
name|v
operator|->
name|data
operator|=
name|ngx_pnalloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
name|len
argument_list|)
expr_stmt|;
if|if
condition|(
name|v
operator|->
name|data
operator|==
name|NULL
condition|)
block|{
name|GeoIPRecord_delete
argument_list|(
name|gr
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
name|ngx_memcpy
argument_list|(
name|v
operator|->
name|data
argument_list|,
name|val
argument_list|,
name|len
argument_list|)
expr_stmt|;
name|v
operator|->
name|len
operator|=
name|len
expr_stmt|;
name|v
operator|->
name|valid
operator|=
literal|1
expr_stmt|;
name|v
operator|->
name|no_cacheable
operator|=
literal|0
expr_stmt|;
name|v
operator|->
name|not_found
operator|=
literal|0
expr_stmt|;
name|GeoIPRecord_delete
argument_list|(
name|gr
argument_list|)
expr_stmt|;
return|return
name|NGX_OK
return|;
name|no_value
label|:
name|GeoIPRecord_delete
argument_list|(
name|gr
argument_list|)
expr_stmt|;
name|not_found
label|:
name|v
operator|->
name|not_found
operator|=
literal|1
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_geoip_city_float_variable (ngx_http_request_t * r,ngx_http_variable_value_t * v,uintptr_t data)
name|ngx_http_geoip_city_float_variable
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|,
name|ngx_http_variable_value_t
modifier|*
name|v
parameter_list|,
name|uintptr_t
name|data
parameter_list|)
block|{
name|float
name|val
decl_stmt|;
name|GeoIPRecord
modifier|*
name|gr
decl_stmt|;
name|gr
operator|=
name|ngx_http_geoip_get_city_record
argument_list|(
name|r
argument_list|)
expr_stmt|;
if|if
condition|(
name|gr
operator|==
name|NULL
condition|)
block|{
name|v
operator|->
name|not_found
operator|=
literal|1
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
name|v
operator|->
name|data
operator|=
name|ngx_pnalloc
argument_list|(
name|r
operator|->
name|pool
argument_list|,
name|NGX_INT64_LEN
operator|+
literal|5
argument_list|)
expr_stmt|;
if|if
condition|(
name|v
operator|->
name|data
operator|==
name|NULL
condition|)
block|{
name|GeoIPRecord_delete
argument_list|(
name|gr
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
name|val
operator|=
operator|*
operator|(
name|float
operator|*
operator|)
operator|(
operator|(
name|char
operator|*
operator|)
name|gr
operator|+
name|data
operator|)
expr_stmt|;
name|v
operator|->
name|len
operator|=
name|ngx_sprintf
argument_list|(
name|v
operator|->
name|data
argument_list|,
literal|"%.4f"
argument_list|,
name|val
argument_list|)
operator|-
name|v
operator|->
name|data
expr_stmt|;
name|GeoIPRecord_delete
argument_list|(
name|gr
argument_list|)
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
specifier|static
name|GeoIPRecord
modifier|*
DECL|function|ngx_http_geoip_get_city_record (ngx_http_request_t * r)
name|ngx_http_geoip_get_city_record
parameter_list|(
name|ngx_http_request_t
modifier|*
name|r
parameter_list|)
block|{
name|u_long
name|addr
decl_stmt|;
name|struct
name|sockaddr_in
modifier|*
name|sin
decl_stmt|;
name|ngx_http_geoip_conf_t
modifier|*
name|gcf
decl_stmt|;
name|gcf
operator|=
name|ngx_http_get_module_main_conf
argument_list|(
name|r
argument_list|,
name|ngx_http_geoip_module
argument_list|)
expr_stmt|;
if|if
condition|(
name|gcf
operator|->
name|city
operator|&&
name|r
operator|->
name|connection
operator|->
name|sockaddr
operator|->
name|sa_family
operator|==
name|AF_INET
condition|)
block|{
name|sin
operator|=
operator|(
expr|struct
name|sockaddr_in
operator|*
operator|)
name|r
operator|->
name|connection
operator|->
name|sockaddr
expr_stmt|;
name|addr
operator|=
name|ntohl
argument_list|(
name|sin
operator|->
name|sin_addr
operator|.
name|s_addr
argument_list|)
expr_stmt|;
return|return
name|GeoIP_record_by_ipnum
argument_list|(
name|gcf
operator|->
name|city
argument_list|,
name|addr
argument_list|)
return|;
block|}
return|return
name|NULL
return|;
block|}
end_function

begin_function
specifier|static
name|ngx_int_t
DECL|function|ngx_http_geoip_add_variables (ngx_conf_t * cf)
name|ngx_http_geoip_add_variables
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
block|{
name|ngx_http_variable_t
modifier|*
name|var
decl_stmt|,
modifier|*
name|v
decl_stmt|;
for|for
control|(
name|v
operator|=
name|ngx_http_geoip_vars
init|;
name|v
operator|->
name|name
operator|.
name|len
condition|;
name|v
operator|++
control|)
block|{
name|var
operator|=
name|ngx_http_add_variable
argument_list|(
name|cf
argument_list|,
operator|&
name|v
operator|->
name|name
argument_list|,
name|v
operator|->
name|flags
argument_list|)
expr_stmt|;
if|if
condition|(
name|var
operator|==
name|NULL
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|var
operator|->
name|get_handler
operator|=
name|v
operator|->
name|get_handler
expr_stmt|;
name|var
operator|->
name|data
operator|=
name|v
operator|->
name|data
expr_stmt|;
block|}
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
specifier|static
name|void
modifier|*
DECL|function|ngx_http_geoip_create_conf (ngx_conf_t * cf)
name|ngx_http_geoip_create_conf
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|)
block|{
name|ngx_pool_cleanup_t
modifier|*
name|cln
decl_stmt|;
name|ngx_http_geoip_conf_t
modifier|*
name|conf
decl_stmt|;
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
name|ngx_http_geoip_conf_t
argument_list|)
argument_list|)
expr_stmt|;
if|if
condition|(
name|conf
operator|==
name|NULL
condition|)
block|{
return|return
name|NULL
return|;
block|}
name|cln
operator|=
name|ngx_pool_cleanup_add
argument_list|(
name|cf
operator|->
name|pool
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|cln
operator|==
name|NULL
condition|)
block|{
return|return
name|NULL
return|;
block|}
name|cln
operator|->
name|handler
operator|=
name|ngx_http_geoip_cleanup
expr_stmt|;
name|cln
operator|->
name|data
operator|=
name|conf
expr_stmt|;
return|return
name|conf
return|;
block|}
end_function

begin_function
specifier|static
name|char
modifier|*
DECL|function|ngx_http_geoip_country (ngx_conf_t * cf,ngx_command_t * cmd,void * conf)
name|ngx_http_geoip_country
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
name|ngx_http_geoip_conf_t
modifier|*
name|gcf
init|=
name|conf
decl_stmt|;
name|ngx_str_t
modifier|*
name|value
decl_stmt|;
if|if
condition|(
name|gcf
operator|->
name|country
condition|)
block|{
return|return
literal|"is duplicate"
return|;
block|}
name|value
operator|=
name|cf
operator|->
name|args
operator|->
name|elts
expr_stmt|;
name|gcf
operator|->
name|country
operator|=
name|GeoIP_open
argument_list|(
operator|(
name|char
operator|*
operator|)
name|value
index|[
literal|1
index|]
operator|.
name|data
argument_list|,
name|GEOIP_MEMORY_CACHE
argument_list|)
expr_stmt|;
if|if
condition|(
name|gcf
operator|->
name|country
operator|==
name|NULL
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
literal|"GeoIO_open(\"%V\") failed"
argument_list|,
operator|&
name|value
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
switch|switch
condition|(
name|gcf
operator|->
name|country
operator|->
name|databaseType
condition|)
block|{
case|case
name|GEOIP_COUNTRY_EDITION
case|:
case|case
name|GEOIP_PROXY_EDITION
case|:
case|case
name|GEOIP_NETSPEED_EDITION
case|:
return|return
name|NGX_CONF_OK
return|;
default|default:
name|ngx_conf_log_error
argument_list|(
name|NGX_LOG_EMERG
argument_list|,
name|cf
argument_list|,
literal|0
argument_list|,
literal|"invalid GeoIP database \"%V\" type:%d"
argument_list|,
operator|&
name|value
index|[
literal|1
index|]
argument_list|,
name|gcf
operator|->
name|country
operator|->
name|databaseType
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
block|}
end_function

begin_function
specifier|static
name|char
modifier|*
DECL|function|ngx_http_geoip_city (ngx_conf_t * cf,ngx_command_t * cmd,void * conf)
name|ngx_http_geoip_city
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
name|ngx_http_geoip_conf_t
modifier|*
name|gcf
init|=
name|conf
decl_stmt|;
name|ngx_str_t
modifier|*
name|value
decl_stmt|;
if|if
condition|(
name|gcf
operator|->
name|city
condition|)
block|{
return|return
literal|"is duplicate"
return|;
block|}
name|value
operator|=
name|cf
operator|->
name|args
operator|->
name|elts
expr_stmt|;
name|gcf
operator|->
name|city
operator|=
name|GeoIP_open
argument_list|(
operator|(
name|char
operator|*
operator|)
name|value
index|[
literal|1
index|]
operator|.
name|data
argument_list|,
name|GEOIP_MEMORY_CACHE
argument_list|)
expr_stmt|;
if|if
condition|(
name|gcf
operator|->
name|city
operator|==
name|NULL
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
literal|"GeoIO_open(\"%V\") failed"
argument_list|,
operator|&
name|value
index|[
literal|1
index|]
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
switch|switch
condition|(
name|gcf
operator|->
name|city
operator|->
name|databaseType
condition|)
block|{
case|case
name|GEOIP_CITY_EDITION_REV0
case|:
case|case
name|GEOIP_CITY_EDITION_REV1
case|:
return|return
name|NGX_CONF_OK
return|;
default|default:
name|ngx_conf_log_error
argument_list|(
name|NGX_LOG_EMERG
argument_list|,
name|cf
argument_list|,
literal|0
argument_list|,
literal|"invalid GeoIP City database \"%V\" type:%d"
argument_list|,
operator|&
name|value
index|[
literal|1
index|]
argument_list|,
name|gcf
operator|->
name|city
operator|->
name|databaseType
argument_list|)
expr_stmt|;
return|return
name|NGX_CONF_ERROR
return|;
block|}
block|}
end_function

begin_function
specifier|static
name|void
DECL|function|ngx_http_geoip_cleanup (void * data)
name|ngx_http_geoip_cleanup
parameter_list|(
name|void
modifier|*
name|data
parameter_list|)
block|{
name|ngx_http_geoip_conf_t
modifier|*
name|gcf
init|=
name|data
decl_stmt|;
if|if
condition|(
name|gcf
operator|->
name|country
condition|)
block|{
name|GeoIP_delete
argument_list|(
name|gcf
operator|->
name|country
argument_list|)
expr_stmt|;
block|}
if|if
condition|(
name|gcf
operator|->
name|city
condition|)
block|{
name|GeoIP_delete
argument_list|(
name|gcf
operator|->
name|city
argument_list|)
expr_stmt|;
block|}
block|}
end_function

end_unit

