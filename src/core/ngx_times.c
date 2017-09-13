begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (C) Igor Sysoev  * Copyright (C) Nginx, Inc.  */
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

begin_comment
comment|/*  * The time may be updated by signal handler or by several threads.  * The time update operations are rare and require to hold the ngx_time_lock.  * The time read operations are frequent, so they are lock-free and get time  * values and strings from the current slot.  Thus thread may get the corrupted  * values only if it is preempted while copying and then it is not scheduled  * to run more than NGX_TIME_SLOTS seconds.  */
end_comment

begin_define
DECL|macro|NGX_TIME_SLOTS
define|#
directive|define
name|NGX_TIME_SLOTS
value|64
end_define

begin_decl_stmt
DECL|variable|slot
specifier|static
name|ngx_uint_t
name|slot
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_time_lock
specifier|static
name|ngx_atomic_t
name|ngx_time_lock
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_current_msec
specifier|volatile
name|ngx_msec_t
name|ngx_current_msec
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_cached_time
specifier|volatile
name|ngx_time_t
modifier|*
name|ngx_cached_time
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_cached_err_log_time
specifier|volatile
name|ngx_str_t
name|ngx_cached_err_log_time
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_cached_http_time
specifier|volatile
name|ngx_str_t
name|ngx_cached_http_time
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_cached_http_log_time
specifier|volatile
name|ngx_str_t
name|ngx_cached_http_log_time
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_cached_http_log_iso8601
specifier|volatile
name|ngx_str_t
name|ngx_cached_http_log_iso8601
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_cached_syslog_time
specifier|volatile
name|ngx_str_t
name|ngx_cached_syslog_time
decl_stmt|;
end_decl_stmt

begin_if
if|#
directive|if
operator|!
operator|(
name|NGX_WIN32
operator|)
end_if

begin_comment
comment|/*  * localtime() and localtime_r() are not Async-Signal-Safe functions, therefore,  * they must not be called by a signal handler, so we use the cached  * GMT offset value. Fortunately the value is changed only two times a year.  */
end_comment

begin_decl_stmt
DECL|variable|cached_gmtoff
specifier|static
name|ngx_int_t
name|cached_gmtoff
decl_stmt|;
end_decl_stmt

begin_endif
endif|#
directive|endif
end_endif

begin_decl_stmt
DECL|variable|cached_time
specifier|static
name|ngx_time_t
name|cached_time
index|[
name|NGX_TIME_SLOTS
index|]
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|cached_err_log_time
specifier|static
name|u_char
name|cached_err_log_time
index|[
name|NGX_TIME_SLOTS
index|]
index|[
sizeof|sizeof
argument_list|(
literal|"1970/09/28 12:00:00"
argument_list|)
index|]
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|cached_http_time
specifier|static
name|u_char
name|cached_http_time
index|[
name|NGX_TIME_SLOTS
index|]
index|[
sizeof|sizeof
argument_list|(
literal|"Mon, 28 Sep 1970 06:00:00 GMT"
argument_list|)
index|]
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|cached_http_log_time
specifier|static
name|u_char
name|cached_http_log_time
index|[
name|NGX_TIME_SLOTS
index|]
index|[
sizeof|sizeof
argument_list|(
literal|"28/Sep/1970:12:00:00 +0600"
argument_list|)
index|]
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|cached_http_log_iso8601
specifier|static
name|u_char
name|cached_http_log_iso8601
index|[
name|NGX_TIME_SLOTS
index|]
index|[
sizeof|sizeof
argument_list|(
literal|"1970-09-28T12:00:00+06:00"
argument_list|)
index|]
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|cached_syslog_time
specifier|static
name|u_char
name|cached_syslog_time
index|[
name|NGX_TIME_SLOTS
index|]
index|[
sizeof|sizeof
argument_list|(
literal|"Sep 28 12:00:00"
argument_list|)
index|]
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|week
specifier|static
name|char
modifier|*
name|week
index|[]
init|=
block|{
literal|"Sun"
block|,
literal|"Mon"
block|,
literal|"Tue"
block|,
literal|"Wed"
block|,
literal|"Thu"
block|,
literal|"Fri"
block|,
literal|"Sat"
block|}
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|months
specifier|static
name|char
modifier|*
name|months
index|[]
init|=
block|{
literal|"Jan"
block|,
literal|"Feb"
block|,
literal|"Mar"
block|,
literal|"Apr"
block|,
literal|"May"
block|,
literal|"Jun"
block|,
literal|"Jul"
block|,
literal|"Aug"
block|,
literal|"Sep"
block|,
literal|"Oct"
block|,
literal|"Nov"
block|,
literal|"Dec"
block|}
decl_stmt|;
end_decl_stmt

begin_function
name|void
DECL|function|ngx_time_init (void)
name|ngx_time_init
parameter_list|(
name|void
parameter_list|)
block|{
name|ngx_cached_err_log_time
operator|.
name|len
operator|=
sizeof|sizeof
argument_list|(
literal|"1970/09/28 12:00:00"
argument_list|)
operator|-
literal|1
expr_stmt|;
name|ngx_cached_http_time
operator|.
name|len
operator|=
sizeof|sizeof
argument_list|(
literal|"Mon, 28 Sep 1970 06:00:00 GMT"
argument_list|)
operator|-
literal|1
expr_stmt|;
name|ngx_cached_http_log_time
operator|.
name|len
operator|=
sizeof|sizeof
argument_list|(
literal|"28/Sep/1970:12:00:00 +0600"
argument_list|)
operator|-
literal|1
expr_stmt|;
name|ngx_cached_http_log_iso8601
operator|.
name|len
operator|=
sizeof|sizeof
argument_list|(
literal|"1970-09-28T12:00:00+06:00"
argument_list|)
operator|-
literal|1
expr_stmt|;
name|ngx_cached_syslog_time
operator|.
name|len
operator|=
sizeof|sizeof
argument_list|(
literal|"Sep 28 12:00:00"
argument_list|)
operator|-
literal|1
expr_stmt|;
name|ngx_cached_time
operator|=
operator|&
name|cached_time
index|[
literal|0
index|]
expr_stmt|;
name|ngx_time_update
argument_list|()
expr_stmt|;
block|}
end_function

begin_function
name|void
DECL|function|ngx_time_update (void)
name|ngx_time_update
parameter_list|(
name|void
parameter_list|)
block|{
name|u_char
modifier|*
name|p0
decl_stmt|,
modifier|*
name|p1
decl_stmt|,
modifier|*
name|p2
decl_stmt|,
modifier|*
name|p3
decl_stmt|,
modifier|*
name|p4
decl_stmt|;
name|ngx_tm_t
name|tm
decl_stmt|,
name|gmt
decl_stmt|;
name|time_t
name|sec
decl_stmt|;
name|ngx_uint_t
name|msec
decl_stmt|;
name|ngx_time_t
modifier|*
name|tp
decl_stmt|;
name|struct
name|timeval
name|tv
decl_stmt|;
if|if
condition|(
operator|!
name|ngx_trylock
argument_list|(
operator|&
name|ngx_time_lock
argument_list|)
condition|)
block|{
return|return;
block|}
name|ngx_gettimeofday
argument_list|(
operator|&
name|tv
argument_list|)
expr_stmt|;
name|sec
operator|=
name|tv
operator|.
name|tv_sec
expr_stmt|;
name|msec
operator|=
name|tv
operator|.
name|tv_usec
operator|/
literal|1000
expr_stmt|;
name|ngx_current_msec
operator|=
operator|(
name|ngx_msec_t
operator|)
name|sec
operator|*
literal|1000
operator|+
name|msec
expr_stmt|;
name|tp
operator|=
operator|&
name|cached_time
index|[
name|slot
index|]
expr_stmt|;
if|if
condition|(
name|tp
operator|->
name|sec
operator|==
name|sec
condition|)
block|{
name|tp
operator|->
name|msec
operator|=
name|msec
expr_stmt|;
name|ngx_unlock
argument_list|(
operator|&
name|ngx_time_lock
argument_list|)
expr_stmt|;
return|return;
block|}
if|if
condition|(
name|slot
operator|==
name|NGX_TIME_SLOTS
operator|-
literal|1
condition|)
block|{
name|slot
operator|=
literal|0
expr_stmt|;
block|}
else|else
block|{
name|slot
operator|++
expr_stmt|;
block|}
name|tp
operator|=
operator|&
name|cached_time
index|[
name|slot
index|]
expr_stmt|;
name|tp
operator|->
name|sec
operator|=
name|sec
expr_stmt|;
name|tp
operator|->
name|msec
operator|=
name|msec
expr_stmt|;
name|ngx_gmtime
argument_list|(
name|sec
argument_list|,
operator|&
name|gmt
argument_list|)
expr_stmt|;
name|p0
operator|=
operator|&
name|cached_http_time
index|[
name|slot
index|]
index|[
literal|0
index|]
expr_stmt|;
operator|(
name|void
operator|)
name|ngx_sprintf
argument_list|(
name|p0
argument_list|,
literal|"%s, %02d %s %4d %02d:%02d:%02d GMT"
argument_list|,
name|week
index|[
name|gmt
operator|.
name|ngx_tm_wday
index|]
argument_list|,
name|gmt
operator|.
name|ngx_tm_mday
argument_list|,
name|months
index|[
name|gmt
operator|.
name|ngx_tm_mon
operator|-
literal|1
index|]
argument_list|,
name|gmt
operator|.
name|ngx_tm_year
argument_list|,
name|gmt
operator|.
name|ngx_tm_hour
argument_list|,
name|gmt
operator|.
name|ngx_tm_min
argument_list|,
name|gmt
operator|.
name|ngx_tm_sec
argument_list|)
expr_stmt|;
if|#
directive|if
operator|(
name|NGX_HAVE_GETTIMEZONE
operator|)
name|tp
operator|->
name|gmtoff
operator|=
name|ngx_gettimezone
argument_list|()
expr_stmt|;
name|ngx_gmtime
argument_list|(
name|sec
operator|+
name|tp
operator|->
name|gmtoff
operator|*
literal|60
argument_list|,
operator|&
name|tm
argument_list|)
expr_stmt|;
elif|#
directive|elif
operator|(
name|NGX_HAVE_GMTOFF
operator|)
name|ngx_localtime
argument_list|(
name|sec
argument_list|,
operator|&
name|tm
argument_list|)
expr_stmt|;
name|cached_gmtoff
operator|=
operator|(
name|ngx_int_t
operator|)
operator|(
name|tm
operator|.
name|ngx_tm_gmtoff
operator|/
literal|60
operator|)
expr_stmt|;
name|tp
operator|->
name|gmtoff
operator|=
name|cached_gmtoff
expr_stmt|;
else|#
directive|else
name|ngx_localtime
argument_list|(
name|sec
argument_list|,
operator|&
name|tm
argument_list|)
expr_stmt|;
name|cached_gmtoff
operator|=
name|ngx_timezone
argument_list|(
name|tm
operator|.
name|ngx_tm_isdst
argument_list|)
expr_stmt|;
name|tp
operator|->
name|gmtoff
operator|=
name|cached_gmtoff
expr_stmt|;
endif|#
directive|endif
name|p1
operator|=
operator|&
name|cached_err_log_time
index|[
name|slot
index|]
index|[
literal|0
index|]
expr_stmt|;
operator|(
name|void
operator|)
name|ngx_sprintf
argument_list|(
name|p1
argument_list|,
literal|"%4d/%02d/%02d %02d:%02d:%02d"
argument_list|,
name|tm
operator|.
name|ngx_tm_year
argument_list|,
name|tm
operator|.
name|ngx_tm_mon
argument_list|,
name|tm
operator|.
name|ngx_tm_mday
argument_list|,
name|tm
operator|.
name|ngx_tm_hour
argument_list|,
name|tm
operator|.
name|ngx_tm_min
argument_list|,
name|tm
operator|.
name|ngx_tm_sec
argument_list|)
expr_stmt|;
name|p2
operator|=
operator|&
name|cached_http_log_time
index|[
name|slot
index|]
index|[
literal|0
index|]
expr_stmt|;
operator|(
name|void
operator|)
name|ngx_sprintf
argument_list|(
name|p2
argument_list|,
literal|"%02d/%s/%d:%02d:%02d:%02d %c%02i%02i"
argument_list|,
name|tm
operator|.
name|ngx_tm_mday
argument_list|,
name|months
index|[
name|tm
operator|.
name|ngx_tm_mon
operator|-
literal|1
index|]
argument_list|,
name|tm
operator|.
name|ngx_tm_year
argument_list|,
name|tm
operator|.
name|ngx_tm_hour
argument_list|,
name|tm
operator|.
name|ngx_tm_min
argument_list|,
name|tm
operator|.
name|ngx_tm_sec
argument_list|,
name|tp
operator|->
name|gmtoff
operator|<
literal|0
condition|?
literal|'-'
else|:
literal|'+'
argument_list|,
name|ngx_abs
argument_list|(
name|tp
operator|->
name|gmtoff
operator|/
literal|60
argument_list|)
argument_list|,
name|ngx_abs
argument_list|(
name|tp
operator|->
name|gmtoff
operator|%
literal|60
argument_list|)
argument_list|)
expr_stmt|;
name|p3
operator|=
operator|&
name|cached_http_log_iso8601
index|[
name|slot
index|]
index|[
literal|0
index|]
expr_stmt|;
operator|(
name|void
operator|)
name|ngx_sprintf
argument_list|(
name|p3
argument_list|,
literal|"%4d-%02d-%02dT%02d:%02d:%02d%c%02i:%02i"
argument_list|,
name|tm
operator|.
name|ngx_tm_year
argument_list|,
name|tm
operator|.
name|ngx_tm_mon
argument_list|,
name|tm
operator|.
name|ngx_tm_mday
argument_list|,
name|tm
operator|.
name|ngx_tm_hour
argument_list|,
name|tm
operator|.
name|ngx_tm_min
argument_list|,
name|tm
operator|.
name|ngx_tm_sec
argument_list|,
name|tp
operator|->
name|gmtoff
operator|<
literal|0
condition|?
literal|'-'
else|:
literal|'+'
argument_list|,
name|ngx_abs
argument_list|(
name|tp
operator|->
name|gmtoff
operator|/
literal|60
argument_list|)
argument_list|,
name|ngx_abs
argument_list|(
name|tp
operator|->
name|gmtoff
operator|%
literal|60
argument_list|)
argument_list|)
expr_stmt|;
name|p4
operator|=
operator|&
name|cached_syslog_time
index|[
name|slot
index|]
index|[
literal|0
index|]
expr_stmt|;
operator|(
name|void
operator|)
name|ngx_sprintf
argument_list|(
name|p4
argument_list|,
literal|"%s %2d %02d:%02d:%02d"
argument_list|,
name|months
index|[
name|tm
operator|.
name|ngx_tm_mon
operator|-
literal|1
index|]
argument_list|,
name|tm
operator|.
name|ngx_tm_mday
argument_list|,
name|tm
operator|.
name|ngx_tm_hour
argument_list|,
name|tm
operator|.
name|ngx_tm_min
argument_list|,
name|tm
operator|.
name|ngx_tm_sec
argument_list|)
expr_stmt|;
name|ngx_memory_barrier
argument_list|()
expr_stmt|;
name|ngx_cached_time
operator|=
name|tp
expr_stmt|;
name|ngx_cached_http_time
operator|.
name|data
operator|=
name|p0
expr_stmt|;
name|ngx_cached_err_log_time
operator|.
name|data
operator|=
name|p1
expr_stmt|;
name|ngx_cached_http_log_time
operator|.
name|data
operator|=
name|p2
expr_stmt|;
name|ngx_cached_http_log_iso8601
operator|.
name|data
operator|=
name|p3
expr_stmt|;
name|ngx_cached_syslog_time
operator|.
name|data
operator|=
name|p4
expr_stmt|;
name|ngx_unlock
argument_list|(
operator|&
name|ngx_time_lock
argument_list|)
expr_stmt|;
block|}
end_function

begin_if
if|#
directive|if
operator|!
operator|(
name|NGX_WIN32
operator|)
end_if

begin_function
name|void
DECL|function|ngx_time_sigsafe_update (void)
name|ngx_time_sigsafe_update
parameter_list|(
name|void
parameter_list|)
block|{
name|u_char
modifier|*
name|p
decl_stmt|,
modifier|*
name|p2
decl_stmt|;
name|ngx_tm_t
name|tm
decl_stmt|;
name|time_t
name|sec
decl_stmt|;
name|ngx_time_t
modifier|*
name|tp
decl_stmt|;
name|struct
name|timeval
name|tv
decl_stmt|;
if|if
condition|(
operator|!
name|ngx_trylock
argument_list|(
operator|&
name|ngx_time_lock
argument_list|)
condition|)
block|{
return|return;
block|}
name|ngx_gettimeofday
argument_list|(
operator|&
name|tv
argument_list|)
expr_stmt|;
name|sec
operator|=
name|tv
operator|.
name|tv_sec
expr_stmt|;
name|tp
operator|=
operator|&
name|cached_time
index|[
name|slot
index|]
expr_stmt|;
if|if
condition|(
name|tp
operator|->
name|sec
operator|==
name|sec
condition|)
block|{
name|ngx_unlock
argument_list|(
operator|&
name|ngx_time_lock
argument_list|)
expr_stmt|;
return|return;
block|}
if|if
condition|(
name|slot
operator|==
name|NGX_TIME_SLOTS
operator|-
literal|1
condition|)
block|{
name|slot
operator|=
literal|0
expr_stmt|;
block|}
else|else
block|{
name|slot
operator|++
expr_stmt|;
block|}
name|tp
operator|=
operator|&
name|cached_time
index|[
name|slot
index|]
expr_stmt|;
name|tp
operator|->
name|sec
operator|=
literal|0
expr_stmt|;
name|ngx_gmtime
argument_list|(
name|sec
operator|+
name|cached_gmtoff
operator|*
literal|60
argument_list|,
operator|&
name|tm
argument_list|)
expr_stmt|;
name|p
operator|=
operator|&
name|cached_err_log_time
index|[
name|slot
index|]
index|[
literal|0
index|]
expr_stmt|;
operator|(
name|void
operator|)
name|ngx_sprintf
argument_list|(
name|p
argument_list|,
literal|"%4d/%02d/%02d %02d:%02d:%02d"
argument_list|,
name|tm
operator|.
name|ngx_tm_year
argument_list|,
name|tm
operator|.
name|ngx_tm_mon
argument_list|,
name|tm
operator|.
name|ngx_tm_mday
argument_list|,
name|tm
operator|.
name|ngx_tm_hour
argument_list|,
name|tm
operator|.
name|ngx_tm_min
argument_list|,
name|tm
operator|.
name|ngx_tm_sec
argument_list|)
expr_stmt|;
name|p2
operator|=
operator|&
name|cached_syslog_time
index|[
name|slot
index|]
index|[
literal|0
index|]
expr_stmt|;
operator|(
name|void
operator|)
name|ngx_sprintf
argument_list|(
name|p2
argument_list|,
literal|"%s %2d %02d:%02d:%02d"
argument_list|,
name|months
index|[
name|tm
operator|.
name|ngx_tm_mon
operator|-
literal|1
index|]
argument_list|,
name|tm
operator|.
name|ngx_tm_mday
argument_list|,
name|tm
operator|.
name|ngx_tm_hour
argument_list|,
name|tm
operator|.
name|ngx_tm_min
argument_list|,
name|tm
operator|.
name|ngx_tm_sec
argument_list|)
expr_stmt|;
name|ngx_memory_barrier
argument_list|()
expr_stmt|;
name|ngx_cached_err_log_time
operator|.
name|data
operator|=
name|p
expr_stmt|;
name|ngx_cached_syslog_time
operator|.
name|data
operator|=
name|p2
expr_stmt|;
name|ngx_unlock
argument_list|(
operator|&
name|ngx_time_lock
argument_list|)
expr_stmt|;
block|}
end_function

begin_endif
endif|#
directive|endif
end_endif

begin_function
name|u_char
modifier|*
DECL|function|ngx_http_time (u_char * buf,time_t t)
name|ngx_http_time
parameter_list|(
name|u_char
modifier|*
name|buf
parameter_list|,
name|time_t
name|t
parameter_list|)
block|{
name|ngx_tm_t
name|tm
decl_stmt|;
name|ngx_gmtime
argument_list|(
name|t
argument_list|,
operator|&
name|tm
argument_list|)
expr_stmt|;
return|return
name|ngx_sprintf
argument_list|(
name|buf
argument_list|,
literal|"%s, %02d %s %4d %02d:%02d:%02d GMT"
argument_list|,
name|week
index|[
name|tm
operator|.
name|ngx_tm_wday
index|]
argument_list|,
name|tm
operator|.
name|ngx_tm_mday
argument_list|,
name|months
index|[
name|tm
operator|.
name|ngx_tm_mon
operator|-
literal|1
index|]
argument_list|,
name|tm
operator|.
name|ngx_tm_year
argument_list|,
name|tm
operator|.
name|ngx_tm_hour
argument_list|,
name|tm
operator|.
name|ngx_tm_min
argument_list|,
name|tm
operator|.
name|ngx_tm_sec
argument_list|)
return|;
block|}
end_function

begin_function
name|u_char
modifier|*
DECL|function|ngx_http_cookie_time (u_char * buf,time_t t)
name|ngx_http_cookie_time
parameter_list|(
name|u_char
modifier|*
name|buf
parameter_list|,
name|time_t
name|t
parameter_list|)
block|{
name|ngx_tm_t
name|tm
decl_stmt|;
name|ngx_gmtime
argument_list|(
name|t
argument_list|,
operator|&
name|tm
argument_list|)
expr_stmt|;
comment|/*      * Netscape 3.x does not understand 4-digit years at all and      * 2-digit years more than "37"      */
return|return
name|ngx_sprintf
argument_list|(
name|buf
argument_list|,
operator|(
name|tm
operator|.
name|ngx_tm_year
operator|>
literal|2037
operator|)
condition|?
literal|"%s, %02d-%s-%d %02d:%02d:%02d GMT"
else|:
literal|"%s, %02d-%s-%02d %02d:%02d:%02d GMT"
argument_list|,
name|week
index|[
name|tm
operator|.
name|ngx_tm_wday
index|]
argument_list|,
name|tm
operator|.
name|ngx_tm_mday
argument_list|,
name|months
index|[
name|tm
operator|.
name|ngx_tm_mon
operator|-
literal|1
index|]
argument_list|,
operator|(
name|tm
operator|.
name|ngx_tm_year
operator|>
literal|2037
operator|)
condition|?
name|tm
operator|.
name|ngx_tm_year
else|:
name|tm
operator|.
name|ngx_tm_year
operator|%
literal|100
argument_list|,
name|tm
operator|.
name|ngx_tm_hour
argument_list|,
name|tm
operator|.
name|ngx_tm_min
argument_list|,
name|tm
operator|.
name|ngx_tm_sec
argument_list|)
return|;
block|}
end_function

begin_function
name|void
DECL|function|ngx_gmtime (time_t t,ngx_tm_t * tp)
name|ngx_gmtime
parameter_list|(
name|time_t
name|t
parameter_list|,
name|ngx_tm_t
modifier|*
name|tp
parameter_list|)
block|{
name|ngx_int_t
name|yday
decl_stmt|;
name|ngx_uint_t
name|sec
decl_stmt|,
name|min
decl_stmt|,
name|hour
decl_stmt|,
name|mday
decl_stmt|,
name|mon
decl_stmt|,
name|year
decl_stmt|,
name|wday
decl_stmt|,
name|days
decl_stmt|,
name|leap
decl_stmt|;
comment|/* the calculation is valid for positive time_t only */
if|if
condition|(
name|t
operator|<
literal|0
condition|)
block|{
name|t
operator|=
literal|0
expr_stmt|;
block|}
name|days
operator|=
name|t
operator|/
literal|86400
expr_stmt|;
name|sec
operator|=
name|t
operator|%
literal|86400
expr_stmt|;
comment|/*      * no more than 4 year digits supported,      * truncate to December 31, 9999, 23:59:59      */
if|if
condition|(
name|days
operator|>
literal|2932896
condition|)
block|{
name|days
operator|=
literal|2932896
expr_stmt|;
name|sec
operator|=
literal|86399
expr_stmt|;
block|}
comment|/* January 1, 1970 was Thursday */
name|wday
operator|=
operator|(
literal|4
operator|+
name|days
operator|)
operator|%
literal|7
expr_stmt|;
name|hour
operator|=
name|sec
operator|/
literal|3600
expr_stmt|;
name|sec
operator|%=
literal|3600
expr_stmt|;
name|min
operator|=
name|sec
operator|/
literal|60
expr_stmt|;
name|sec
operator|%=
literal|60
expr_stmt|;
comment|/*      * the algorithm based on Gauss' formula,      * see src/core/ngx_parse_time.c      */
comment|/* days since March 1, 1 BC */
name|days
operator|=
name|days
operator|-
operator|(
literal|31
operator|+
literal|28
operator|)
operator|+
literal|719527
expr_stmt|;
comment|/*      * The "days" should be adjusted to 1 only, however, some March 1st's go      * to previous year, so we adjust them to 2.  This causes also shift of the      * last February days to next year, but we catch the case when "yday"      * becomes negative.      */
name|year
operator|=
operator|(
name|days
operator|+
literal|2
operator|)
operator|*
literal|400
operator|/
operator|(
literal|365
operator|*
literal|400
operator|+
literal|100
operator|-
literal|4
operator|+
literal|1
operator|)
expr_stmt|;
name|yday
operator|=
name|days
operator|-
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
operator|)
expr_stmt|;
if|if
condition|(
name|yday
operator|<
literal|0
condition|)
block|{
name|leap
operator|=
operator|(
name|year
operator|%
literal|4
operator|==
literal|0
operator|)
operator|&&
operator|(
name|year
operator|%
literal|100
operator|||
operator|(
name|year
operator|%
literal|400
operator|==
literal|0
operator|)
operator|)
expr_stmt|;
name|yday
operator|=
literal|365
operator|+
name|leap
operator|+
name|yday
expr_stmt|;
name|year
operator|--
expr_stmt|;
block|}
comment|/*      * The empirical formula that maps "yday" to month.      * There are at least 10 variants, some of them are:      *     mon = (yday + 31) * 15 / 459      *     mon = (yday + 31) * 17 / 520      *     mon = (yday + 31) * 20 / 612      */
name|mon
operator|=
operator|(
name|yday
operator|+
literal|31
operator|)
operator|*
literal|10
operator|/
literal|306
expr_stmt|;
comment|/* the Gauss' formula that evaluates days before the month */
name|mday
operator|=
name|yday
operator|-
operator|(
literal|367
operator|*
name|mon
operator|/
literal|12
operator|-
literal|30
operator|)
operator|+
literal|1
expr_stmt|;
if|if
condition|(
name|yday
operator|>=
literal|306
condition|)
block|{
name|year
operator|++
expr_stmt|;
name|mon
operator|-=
literal|10
expr_stmt|;
comment|/*          * there is no "yday" in Win32 SYSTEMTIME          *          * yday -= 306;          */
block|}
else|else
block|{
name|mon
operator|+=
literal|2
expr_stmt|;
comment|/*          * there is no "yday" in Win32 SYSTEMTIME          *          * yday += 31 + 28 + leap;          */
block|}
name|tp
operator|->
name|ngx_tm_sec
operator|=
operator|(
name|ngx_tm_sec_t
operator|)
name|sec
expr_stmt|;
name|tp
operator|->
name|ngx_tm_min
operator|=
operator|(
name|ngx_tm_min_t
operator|)
name|min
expr_stmt|;
name|tp
operator|->
name|ngx_tm_hour
operator|=
operator|(
name|ngx_tm_hour_t
operator|)
name|hour
expr_stmt|;
name|tp
operator|->
name|ngx_tm_mday
operator|=
operator|(
name|ngx_tm_mday_t
operator|)
name|mday
expr_stmt|;
name|tp
operator|->
name|ngx_tm_mon
operator|=
operator|(
name|ngx_tm_mon_t
operator|)
name|mon
expr_stmt|;
name|tp
operator|->
name|ngx_tm_year
operator|=
operator|(
name|ngx_tm_year_t
operator|)
name|year
expr_stmt|;
name|tp
operator|->
name|ngx_tm_wday
operator|=
operator|(
name|ngx_tm_wday_t
operator|)
name|wday
expr_stmt|;
block|}
end_function

begin_function
name|time_t
DECL|function|ngx_next_time (time_t when)
name|ngx_next_time
parameter_list|(
name|time_t
name|when
parameter_list|)
block|{
name|time_t
name|now
decl_stmt|,
name|next
decl_stmt|;
name|struct
name|tm
name|tm
decl_stmt|;
name|now
operator|=
name|ngx_time
argument_list|()
expr_stmt|;
name|ngx_libc_localtime
argument_list|(
name|now
argument_list|,
operator|&
name|tm
argument_list|)
expr_stmt|;
name|tm
operator|.
name|tm_hour
operator|=
operator|(
name|int
operator|)
operator|(
name|when
operator|/
literal|3600
operator|)
expr_stmt|;
name|when
operator|%=
literal|3600
expr_stmt|;
name|tm
operator|.
name|tm_min
operator|=
operator|(
name|int
operator|)
operator|(
name|when
operator|/
literal|60
operator|)
expr_stmt|;
name|tm
operator|.
name|tm_sec
operator|=
operator|(
name|int
operator|)
operator|(
name|when
operator|%
literal|60
operator|)
expr_stmt|;
name|next
operator|=
name|mktime
argument_list|(
operator|&
name|tm
argument_list|)
expr_stmt|;
if|if
condition|(
name|next
operator|==
operator|-
literal|1
condition|)
block|{
return|return
operator|-
literal|1
return|;
block|}
if|if
condition|(
name|next
operator|-
name|now
operator|>
literal|0
condition|)
block|{
return|return
name|next
return|;
block|}
name|tm
operator|.
name|tm_mday
operator|++
expr_stmt|;
comment|/* mktime() should normalize a date (Jan 32, etc) */
name|next
operator|=
name|mktime
argument_list|(
operator|&
name|tm
argument_list|)
expr_stmt|;
if|if
condition|(
name|next
operator|!=
operator|-
literal|1
condition|)
block|{
return|return
name|next
return|;
block|}
return|return
operator|-
literal|1
return|;
block|}
end_function

end_unit

