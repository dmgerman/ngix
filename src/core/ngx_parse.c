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

begin_function
name|ssize_t
DECL|function|ngx_parse_size (ngx_str_t * line)
name|ngx_parse_size
parameter_list|(
name|ngx_str_t
modifier|*
name|line
parameter_list|)
block|{
name|u_char
name|unit
decl_stmt|;
name|size_t
name|len
decl_stmt|;
name|ssize_t
name|size
decl_stmt|;
name|ngx_int_t
name|scale
decl_stmt|;
name|len
operator|=
name|line
operator|->
name|len
expr_stmt|;
name|unit
operator|=
name|line
operator|->
name|data
index|[
name|len
operator|-
literal|1
index|]
expr_stmt|;
switch|switch
condition|(
name|unit
condition|)
block|{
case|case
literal|'K'
case|:
case|case
literal|'k'
case|:
name|len
operator|--
expr_stmt|;
name|scale
operator|=
literal|1024
expr_stmt|;
break|break;
case|case
literal|'M'
case|:
case|case
literal|'m'
case|:
name|len
operator|--
expr_stmt|;
name|scale
operator|=
literal|1024
operator|*
literal|1024
expr_stmt|;
break|break;
default|default:
name|scale
operator|=
literal|1
expr_stmt|;
block|}
name|size
operator|=
name|ngx_atosz
argument_list|(
name|line
operator|->
name|data
argument_list|,
name|len
argument_list|)
expr_stmt|;
if|if
condition|(
name|size
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|size
operator|*=
name|scale
expr_stmt|;
return|return
name|size
return|;
block|}
end_function

begin_function
name|off_t
DECL|function|ngx_parse_offset (ngx_str_t * line)
name|ngx_parse_offset
parameter_list|(
name|ngx_str_t
modifier|*
name|line
parameter_list|)
block|{
name|u_char
name|unit
decl_stmt|;
name|off_t
name|offset
decl_stmt|;
name|size_t
name|len
decl_stmt|;
name|ngx_int_t
name|scale
decl_stmt|;
name|len
operator|=
name|line
operator|->
name|len
expr_stmt|;
name|unit
operator|=
name|line
operator|->
name|data
index|[
name|len
operator|-
literal|1
index|]
expr_stmt|;
switch|switch
condition|(
name|unit
condition|)
block|{
case|case
literal|'K'
case|:
case|case
literal|'k'
case|:
name|len
operator|--
expr_stmt|;
name|scale
operator|=
literal|1024
expr_stmt|;
break|break;
case|case
literal|'M'
case|:
case|case
literal|'m'
case|:
name|len
operator|--
expr_stmt|;
name|scale
operator|=
literal|1024
operator|*
literal|1024
expr_stmt|;
break|break;
case|case
literal|'G'
case|:
case|case
literal|'g'
case|:
name|len
operator|--
expr_stmt|;
name|scale
operator|=
literal|1024
operator|*
literal|1024
operator|*
literal|1024
expr_stmt|;
break|break;
default|default:
name|scale
operator|=
literal|1
expr_stmt|;
block|}
name|offset
operator|=
name|ngx_atoof
argument_list|(
name|line
operator|->
name|data
argument_list|,
name|len
argument_list|)
expr_stmt|;
if|if
condition|(
name|offset
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|offset
operator|*=
name|scale
expr_stmt|;
return|return
name|offset
return|;
block|}
end_function

begin_function
name|ngx_int_t
DECL|function|ngx_parse_time (ngx_str_t * line,ngx_uint_t is_sec)
name|ngx_parse_time
parameter_list|(
name|ngx_str_t
modifier|*
name|line
parameter_list|,
name|ngx_uint_t
name|is_sec
parameter_list|)
block|{
name|u_char
modifier|*
name|p
decl_stmt|,
modifier|*
name|last
decl_stmt|;
name|ngx_int_t
name|value
decl_stmt|,
name|total
decl_stmt|,
name|scale
decl_stmt|;
name|ngx_uint_t
name|max
decl_stmt|,
name|valid
decl_stmt|;
DECL|enum|__anon28830d590103
enum|enum
block|{
DECL|enumerator|st_start
name|st_start
init|=
literal|0
block|,
DECL|enumerator|st_year
name|st_year
block|,
DECL|enumerator|st_month
name|st_month
block|,
DECL|enumerator|st_week
name|st_week
block|,
DECL|enumerator|st_day
name|st_day
block|,
DECL|enumerator|st_hour
name|st_hour
block|,
DECL|enumerator|st_min
name|st_min
block|,
DECL|enumerator|st_sec
name|st_sec
block|,
DECL|enumerator|st_msec
name|st_msec
block|,
DECL|enumerator|st_last
name|st_last
block|}
name|step
enum|;
name|valid
operator|=
literal|0
expr_stmt|;
name|value
operator|=
literal|0
expr_stmt|;
name|total
operator|=
literal|0
expr_stmt|;
name|step
operator|=
name|is_sec
condition|?
name|st_start
else|:
name|st_month
expr_stmt|;
name|scale
operator|=
name|is_sec
condition|?
literal|1
else|:
literal|1000
expr_stmt|;
name|p
operator|=
name|line
operator|->
name|data
expr_stmt|;
name|last
operator|=
name|p
operator|+
name|line
operator|->
name|len
expr_stmt|;
while|while
condition|(
name|p
operator|<
name|last
condition|)
block|{
if|if
condition|(
operator|*
name|p
operator|>=
literal|'0'
operator|&&
operator|*
name|p
operator|<=
literal|'9'
condition|)
block|{
name|value
operator|=
name|value
operator|*
literal|10
operator|+
operator|(
operator|*
name|p
operator|++
operator|-
literal|'0'
operator|)
expr_stmt|;
name|valid
operator|=
literal|1
expr_stmt|;
continue|continue;
block|}
switch|switch
condition|(
operator|*
name|p
operator|++
condition|)
block|{
case|case
literal|'y'
case|:
if|if
condition|(
name|step
operator|>
name|st_start
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|step
operator|=
name|st_year
expr_stmt|;
name|max
operator|=
name|NGX_MAX_INT32_VALUE
operator|/
operator|(
literal|60
operator|*
literal|60
operator|*
literal|24
operator|*
literal|365
operator|)
expr_stmt|;
name|scale
operator|=
literal|60
operator|*
literal|60
operator|*
literal|24
operator|*
literal|365
expr_stmt|;
break|break;
case|case
literal|'M'
case|:
if|if
condition|(
name|step
operator|>=
name|st_month
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|step
operator|=
name|st_month
expr_stmt|;
name|max
operator|=
name|NGX_MAX_INT32_VALUE
operator|/
operator|(
literal|60
operator|*
literal|60
operator|*
literal|24
operator|*
literal|30
operator|)
expr_stmt|;
name|scale
operator|=
literal|60
operator|*
literal|60
operator|*
literal|24
operator|*
literal|30
expr_stmt|;
break|break;
case|case
literal|'w'
case|:
if|if
condition|(
name|step
operator|>=
name|st_week
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|step
operator|=
name|st_week
expr_stmt|;
name|max
operator|=
name|NGX_MAX_INT32_VALUE
operator|/
operator|(
literal|60
operator|*
literal|60
operator|*
literal|24
operator|*
literal|7
operator|)
expr_stmt|;
name|scale
operator|=
literal|60
operator|*
literal|60
operator|*
literal|24
operator|*
literal|7
expr_stmt|;
break|break;
case|case
literal|'d'
case|:
if|if
condition|(
name|step
operator|>=
name|st_day
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|step
operator|=
name|st_day
expr_stmt|;
name|max
operator|=
name|NGX_MAX_INT32_VALUE
operator|/
operator|(
literal|60
operator|*
literal|60
operator|*
literal|24
operator|)
expr_stmt|;
name|scale
operator|=
literal|60
operator|*
literal|60
operator|*
literal|24
expr_stmt|;
break|break;
case|case
literal|'h'
case|:
if|if
condition|(
name|step
operator|>=
name|st_hour
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|step
operator|=
name|st_hour
expr_stmt|;
name|max
operator|=
name|NGX_MAX_INT32_VALUE
operator|/
operator|(
literal|60
operator|*
literal|60
operator|)
expr_stmt|;
name|scale
operator|=
literal|60
operator|*
literal|60
expr_stmt|;
break|break;
case|case
literal|'m'
case|:
if|if
condition|(
operator|*
name|p
operator|==
literal|'s'
condition|)
block|{
if|if
condition|(
name|is_sec
operator|||
name|step
operator|>=
name|st_msec
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|p
operator|++
expr_stmt|;
name|step
operator|=
name|st_msec
expr_stmt|;
name|max
operator|=
name|NGX_MAX_INT32_VALUE
expr_stmt|;
name|scale
operator|=
literal|1
expr_stmt|;
break|break;
block|}
if|if
condition|(
name|step
operator|>=
name|st_min
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|step
operator|=
name|st_min
expr_stmt|;
name|max
operator|=
name|NGX_MAX_INT32_VALUE
operator|/
literal|60
expr_stmt|;
name|scale
operator|=
literal|60
expr_stmt|;
break|break;
case|case
literal|'s'
case|:
if|if
condition|(
name|step
operator|>=
name|st_sec
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|step
operator|=
name|st_sec
expr_stmt|;
name|max
operator|=
name|NGX_MAX_INT32_VALUE
expr_stmt|;
name|scale
operator|=
literal|1
expr_stmt|;
break|break;
case|case
literal|' '
case|:
if|if
condition|(
name|step
operator|>=
name|st_sec
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|step
operator|=
name|st_last
expr_stmt|;
name|max
operator|=
name|NGX_MAX_INT32_VALUE
expr_stmt|;
name|scale
operator|=
literal|1
expr_stmt|;
break|break;
default|default:
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|step
operator|!=
name|st_msec
operator|&&
operator|!
name|is_sec
condition|)
block|{
name|scale
operator|*=
literal|1000
expr_stmt|;
name|max
operator|/=
literal|1000
expr_stmt|;
block|}
if|if
condition|(
operator|(
name|ngx_uint_t
operator|)
name|value
operator|>
name|max
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|total
operator|+=
name|value
operator|*
name|scale
expr_stmt|;
if|if
condition|(
operator|(
name|ngx_uint_t
operator|)
name|total
operator|>
name|NGX_MAX_INT32_VALUE
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|value
operator|=
literal|0
expr_stmt|;
name|scale
operator|=
name|is_sec
condition|?
literal|1
else|:
literal|1000
expr_stmt|;
while|while
condition|(
name|p
operator|<
name|last
operator|&&
operator|*
name|p
operator|==
literal|' '
condition|)
block|{
name|p
operator|++
expr_stmt|;
block|}
block|}
if|if
condition|(
name|valid
condition|)
block|{
return|return
name|total
operator|+
name|value
operator|*
name|scale
return|;
block|}
return|return
name|NGX_ERROR
return|;
block|}
end_function

end_unit

