begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (C) 2002-2004 Igor Sysoev  */
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
file|<ngx_types.h>
end_include

begin_decl_stmt
DECL|variable|mday
specifier|static
name|int
name|mday
index|[]
init|=
block|{
literal|31
block|,
literal|28
block|,
literal|31
block|,
literal|30
block|,
literal|31
block|,
literal|30
block|,
literal|31
block|,
literal|31
block|,
literal|30
block|,
literal|31
block|,
literal|30
block|,
literal|31
block|}
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|ngx_http_parse_time (u_char * value,size_t len)
name|time_t
name|ngx_http_parse_time
parameter_list|(
name|u_char
modifier|*
name|value
parameter_list|,
name|size_t
name|len
parameter_list|)
block|{
name|u_char
modifier|*
name|p
decl_stmt|,
modifier|*
name|end
decl_stmt|;
name|int
name|day
decl_stmt|,
name|month
decl_stmt|,
name|year
decl_stmt|,
name|hour
decl_stmt|,
name|min
decl_stmt|,
name|sec
decl_stmt|;
DECL|enum|__anon2aafa13f0103
enum|enum
block|{
DECL|enumerator|no
name|no
init|=
literal|0
block|,
DECL|enumerator|rfc822
name|rfc822
block|,
comment|/* Tue 10 Nov 2002 23:50:13    */
DECL|enumerator|rfc850
name|rfc850
block|,
comment|/* Tuesday, 10-Dec-02 23:50:13 */
DECL|enumerator|isoc
name|isoc
comment|/* Tue Dec 10 23:50:13 2002    */
block|}
name|fmt
enum|;
name|fmt
operator|=
literal|0
expr_stmt|;
name|end
operator|=
name|value
operator|+
name|len
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_SUPPRESS_WARN
operator|)
name|day
operator|=
literal|32
expr_stmt|;
name|year
operator|=
literal|2038
expr_stmt|;
endif|#
directive|endif
for|for
control|(
name|p
operator|=
name|value
init|;
name|p
operator|<
name|end
condition|;
name|p
operator|++
control|)
block|{
if|if
condition|(
operator|*
name|p
operator|==
literal|','
condition|)
block|{
break|break;
block|}
if|if
condition|(
operator|*
name|p
operator|==
literal|' '
condition|)
block|{
name|fmt
operator|=
name|isoc
expr_stmt|;
break|break;
block|}
block|}
for|for
control|(
name|p
operator|++
init|;
name|p
operator|<
name|end
condition|;
name|p
operator|++
control|)
if|if
condition|(
operator|*
name|p
operator|!=
literal|' '
condition|)
block|{
break|break;
block|}
if|if
condition|(
name|end
operator|-
name|p
operator|<
literal|18
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|fmt
operator|!=
name|isoc
condition|)
block|{
if|if
condition|(
operator|*
name|p
argument_list|<
literal|'0'
operator|||
operator|*
name|p
argument_list|>
literal|'9'
operator|||
operator|*
operator|(
name|p
operator|+
literal|1
operator|)
operator|<
literal|'0'
operator|||
operator|*
operator|(
name|p
operator|+
literal|1
operator|)
operator|>
literal|'9'
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|day
operator|=
operator|(
operator|*
name|p
operator|-
literal|'0'
operator|)
operator|*
literal|10
operator|+
operator|*
operator|(
name|p
operator|+
literal|1
operator|)
operator|-
literal|'0'
expr_stmt|;
name|p
operator|+=
literal|2
expr_stmt|;
if|if
condition|(
operator|*
name|p
operator|==
literal|' '
condition|)
block|{
if|if
condition|(
name|end
operator|-
name|p
operator|<
literal|18
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|fmt
operator|=
name|rfc822
expr_stmt|;
block|}
if|else if
condition|(
operator|*
name|p
operator|==
literal|'-'
condition|)
block|{
name|fmt
operator|=
name|rfc850
expr_stmt|;
block|}
else|else
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|p
operator|++
expr_stmt|;
block|}
switch|switch
condition|(
operator|*
name|p
condition|)
block|{
case|case
literal|'J'
case|:
name|month
operator|=
operator|*
operator|(
name|p
operator|+
literal|1
operator|)
operator|==
literal|'a'
condition|?
literal|0
else|:
operator|*
operator|(
name|p
operator|+
literal|2
operator|)
operator|==
literal|'n'
condition|?
literal|5
else|:
literal|6
expr_stmt|;
break|break;
case|case
literal|'F'
case|:
name|month
operator|=
literal|1
expr_stmt|;
break|break;
case|case
literal|'M'
case|:
name|month
operator|=
operator|*
operator|(
name|p
operator|+
literal|2
operator|)
operator|==
literal|'r'
condition|?
literal|2
else|:
literal|4
expr_stmt|;
break|break;
case|case
literal|'A'
case|:
name|month
operator|=
operator|*
operator|(
name|p
operator|+
literal|1
operator|)
operator|==
literal|'p'
condition|?
literal|3
else|:
literal|7
expr_stmt|;
break|break;
case|case
literal|'S'
case|:
name|month
operator|=
literal|8
expr_stmt|;
break|break;
case|case
literal|'O'
case|:
name|month
operator|=
literal|9
expr_stmt|;
break|break;
case|case
literal|'N'
case|:
name|month
operator|=
literal|10
expr_stmt|;
break|break;
case|case
literal|'D'
case|:
name|month
operator|=
literal|11
expr_stmt|;
break|break;
default|default:
return|return
name|NGX_ERROR
return|;
block|}
name|p
operator|+=
literal|3
expr_stmt|;
if|if
condition|(
operator|(
name|fmt
operator|==
name|rfc822
operator|&&
operator|*
name|p
operator|!=
literal|' '
operator|)
operator|||
operator|(
name|fmt
operator|==
name|rfc850
operator|&&
operator|*
name|p
operator|!=
literal|'-'
operator|)
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|p
operator|++
expr_stmt|;
if|if
condition|(
name|fmt
operator|==
name|rfc822
condition|)
block|{
if|if
condition|(
operator|*
name|p
argument_list|<
literal|'0'
operator|||
operator|*
name|p
argument_list|>
literal|'9'
operator|||
operator|*
operator|(
name|p
operator|+
literal|1
operator|)
operator|<
literal|'0'
operator|||
operator|*
operator|(
name|p
operator|+
literal|1
operator|)
operator|>
literal|'9'
operator|||
operator|*
operator|(
name|p
operator|+
literal|2
operator|)
operator|<
literal|'0'
operator|||
operator|*
operator|(
name|p
operator|+
literal|2
operator|)
operator|>
literal|'9'
operator|||
operator|*
operator|(
name|p
operator|+
literal|3
operator|)
operator|<
literal|'0'
operator|||
operator|*
operator|(
name|p
operator|+
literal|3
operator|)
operator|>
literal|'9'
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|year
operator|=
operator|(
operator|*
name|p
operator|-
literal|'0'
operator|)
operator|*
literal|1000
operator|+
operator|(
operator|*
operator|(
name|p
operator|+
literal|1
operator|)
operator|-
literal|'0'
operator|)
operator|*
literal|100
operator|+
operator|(
operator|*
operator|(
name|p
operator|+
literal|2
operator|)
operator|-
literal|'0'
operator|)
operator|*
literal|10
operator|+
operator|*
operator|(
name|p
operator|+
literal|3
operator|)
operator|-
literal|'0'
expr_stmt|;
name|p
operator|+=
literal|4
expr_stmt|;
block|}
if|else if
condition|(
name|fmt
operator|==
name|rfc850
condition|)
block|{
if|if
condition|(
operator|*
name|p
argument_list|<
literal|'0'
operator|||
operator|*
name|p
argument_list|>
literal|'9'
operator|||
operator|*
operator|(
name|p
operator|+
literal|1
operator|)
operator|<
literal|'0'
operator|||
operator|*
operator|(
name|p
operator|+
literal|1
operator|)
operator|>
literal|'9'
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|year
operator|=
operator|(
operator|*
name|p
operator|-
literal|'0'
operator|)
operator|*
literal|10
operator|+
operator|*
operator|(
name|p
operator|+
literal|1
operator|)
operator|-
literal|'0'
expr_stmt|;
name|year
operator|+=
operator|(
name|year
operator|<
literal|70
operator|)
condition|?
literal|2000
else|:
literal|1900
expr_stmt|;
name|p
operator|+=
literal|2
expr_stmt|;
block|}
if|if
condition|(
name|fmt
operator|==
name|isoc
condition|)
block|{
if|if
condition|(
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
if|if
condition|(
operator|*
name|p
argument_list|<
literal|'0'
operator|||
operator|*
name|p
argument_list|>
literal|'9'
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|day
operator|=
operator|*
name|p
operator|++
operator|-
literal|'0'
expr_stmt|;
if|if
condition|(
operator|*
name|p
operator|!=
literal|' '
condition|)
block|{
if|if
condition|(
operator|*
name|p
argument_list|<
literal|'0'
operator|||
operator|*
name|p
argument_list|>
literal|'9'
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|day
operator|=
name|day
operator|*
literal|10
operator|+
operator|*
name|p
operator|++
operator|-
literal|'0'
expr_stmt|;
block|}
if|if
condition|(
name|end
operator|-
name|p
operator|<
literal|14
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
block|}
if|if
condition|(
operator|*
name|p
operator|++
operator|!=
literal|' '
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
operator|*
name|p
argument_list|<
literal|'0'
operator|||
operator|*
name|p
argument_list|>
literal|'9'
operator|||
operator|*
operator|(
name|p
operator|+
literal|1
operator|)
operator|<
literal|'0'
operator|||
operator|*
operator|(
name|p
operator|+
literal|1
operator|)
operator|>
literal|'9'
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|hour
operator|=
operator|(
operator|*
name|p
operator|-
literal|'0'
operator|)
operator|*
literal|10
operator|+
operator|*
operator|(
name|p
operator|+
literal|1
operator|)
operator|-
literal|'0'
expr_stmt|;
name|p
operator|+=
literal|2
expr_stmt|;
if|if
condition|(
operator|*
name|p
operator|++
operator|!=
literal|':'
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
operator|*
name|p
argument_list|<
literal|'0'
operator|||
operator|*
name|p
argument_list|>
literal|'9'
operator|||
operator|*
operator|(
name|p
operator|+
literal|1
operator|)
operator|<
literal|'0'
operator|||
operator|*
operator|(
name|p
operator|+
literal|1
operator|)
operator|>
literal|'9'
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|min
operator|=
operator|(
operator|*
name|p
operator|-
literal|'0'
operator|)
operator|*
literal|10
operator|+
operator|*
operator|(
name|p
operator|+
literal|1
operator|)
operator|-
literal|'0'
expr_stmt|;
name|p
operator|+=
literal|2
expr_stmt|;
if|if
condition|(
operator|*
name|p
operator|++
operator|!=
literal|':'
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
operator|*
name|p
argument_list|<
literal|'0'
operator|||
operator|*
name|p
argument_list|>
literal|'9'
operator|||
operator|*
operator|(
name|p
operator|+
literal|1
operator|)
operator|<
literal|'0'
operator|||
operator|*
operator|(
name|p
operator|+
literal|1
operator|)
operator|>
literal|'9'
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|sec
operator|=
operator|(
operator|*
name|p
operator|-
literal|'0'
operator|)
operator|*
literal|10
operator|+
operator|*
operator|(
name|p
operator|+
literal|1
operator|)
operator|-
literal|'0'
expr_stmt|;
if|if
condition|(
name|fmt
operator|==
name|isoc
condition|)
block|{
name|p
operator|+=
literal|2
expr_stmt|;
if|if
condition|(
operator|*
name|p
operator|++
operator|!=
literal|' '
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
operator|*
name|p
argument_list|<
literal|'0'
operator|||
operator|*
name|p
argument_list|>
literal|'9'
operator|||
operator|*
operator|(
name|p
operator|+
literal|1
operator|)
operator|<
literal|'0'
operator|||
operator|*
operator|(
name|p
operator|+
literal|1
operator|)
operator|>
literal|'9'
operator|||
operator|*
operator|(
name|p
operator|+
literal|2
operator|)
operator|<
literal|'0'
operator|||
operator|*
operator|(
name|p
operator|+
literal|2
operator|)
operator|>
literal|'9'
operator|||
operator|*
operator|(
name|p
operator|+
literal|3
operator|)
operator|<
literal|'0'
operator|||
operator|*
operator|(
name|p
operator|+
literal|3
operator|)
operator|>
literal|'9'
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|year
operator|=
operator|(
operator|*
name|p
operator|-
literal|'0'
operator|)
operator|*
literal|1000
operator|+
operator|(
operator|*
operator|(
name|p
operator|+
literal|1
operator|)
operator|-
literal|'0'
operator|)
operator|*
literal|100
operator|+
operator|(
operator|*
operator|(
name|p
operator|+
literal|2
operator|)
operator|-
literal|'0'
operator|)
operator|*
literal|10
operator|+
operator|*
operator|(
name|p
operator|+
literal|3
operator|)
operator|-
literal|'0'
expr_stmt|;
block|}
if|#
directive|if
literal|0
block_content|printf("%d.%d.%d %d:%d:%d\n", day, month + 1, year, hour, min, sec);
endif|#
directive|endif
if|if
condition|(
name|hour
operator|>
literal|23
operator|||
name|min
operator|>
literal|59
operator|||
name|sec
operator|>
literal|59
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|day
operator|==
literal|29
operator|&&
name|month
operator|==
literal|1
condition|)
block|{
if|if
condition|(
operator|(
name|year
operator|&
literal|3
operator|)
operator|||
operator|(
operator|(
name|year
operator|%
literal|100
operator|==
literal|0
operator|)
operator|&&
operator|(
name|year
operator|%
literal|400
operator|)
operator|!=
literal|0
operator|)
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
block|}
if|else if
condition|(
name|day
operator|>
name|mday
index|[
name|month
index|]
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
sizeof|sizeof
argument_list|(
name|time_t
argument_list|)
operator|<=
literal|4
operator|&&
name|year
operator|>=
literal|2038
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
comment|/*      * shift new year to March 1 and start months from 1 (not 0),      * it's needed for Gauss's formula      */
if|if
condition|(
operator|--
name|month
operator|<=
literal|0
condition|)
block|{
name|month
operator|+=
literal|12
expr_stmt|;
name|year
operator|-=
literal|1
expr_stmt|;
block|}
comment|/* Gauss's formula for Grigorian days from 1 March 1 BC */
return|return
operator|(
literal|365
operator|*
name|year
operator|+
name|year
operator|/
literal|4
operator|-
name|year
operator|/
literal|100
operator|+
name|year
operator|/
literal|400
operator|+
literal|367
operator|*
name|month
operator|/
literal|12
operator|-
literal|31
operator|+
name|day
comment|/*             * 719527 days were between March 1, 1 BC and March 1, 1970,             * 31 and 28 days in January and February 1970             */
operator|-
literal|719527
operator|+
literal|31
operator|+
literal|28
operator|)
operator|*
literal|86400
operator|+
name|hour
operator|*
literal|3600
operator|+
name|min
operator|*
literal|60
operator|+
name|sec
return|;
block|}
end_function

begin_if
if|#
directive|if
literal|0
end_if

begin_endif
unit|char zero[] = "Sun, 01 Jan 1970 08:49:30"; char one[]  = "Sunday, 11-Dec-02 08:49:30"; char two[]  = "Sun Mar 1 08:49:37 2000"; char thr[]  = "Sun Dec 11 08:49:37 2002";  main() {     int rc;      rc = ngx_http_parse_time(zero, sizeof(zero) - 1);     printf("rc: %d\n", rc);      rc = ngx_http_parse_time(one, sizeof(one) - 1);     printf("rc: %d\n", rc);      rc = ngx_http_parse_time(two, sizeof(two) - 1);     printf("rc: %d\n", rc);      rc = ngx_http_parse_time(thr, sizeof(thr) - 1);     printf("rc: %d\n", rc); }
endif|#
directive|endif
end_endif

end_unit

