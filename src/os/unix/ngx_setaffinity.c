begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (C) Nginx, Inc.  */
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

begin_if
if|#
directive|if
operator|(
name|NGX_HAVE_CPUSET_SETAFFINITY
operator|)
end_if

begin_include
include|#
directive|include
file|<sys/cpuset.h>
end_include

begin_function
name|void
DECL|function|ngx_setaffinity (uint64_t cpu_affinity,ngx_log_t * log)
name|ngx_setaffinity
parameter_list|(
name|uint64_t
name|cpu_affinity
parameter_list|,
name|ngx_log_t
modifier|*
name|log
parameter_list|)
block|{
name|cpuset_t
name|mask
decl_stmt|;
name|ngx_uint_t
name|i
decl_stmt|;
name|ngx_log_error
argument_list|(
name|NGX_LOG_NOTICE
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"cpuset_setaffinity(0x%08Xl)"
argument_list|,
name|cpu_affinity
argument_list|)
expr_stmt|;
name|CPU_ZERO
argument_list|(
operator|&
name|mask
argument_list|)
expr_stmt|;
name|i
operator|=
literal|0
expr_stmt|;
do|do
block|{
if|if
condition|(
name|cpu_affinity
operator|&
literal|1
condition|)
block|{
name|CPU_SET
argument_list|(
name|i
argument_list|,
operator|&
name|mask
argument_list|)
expr_stmt|;
block|}
name|i
operator|++
expr_stmt|;
name|cpu_affinity
operator|>>=
literal|1
expr_stmt|;
block|}
do|while
condition|(
name|cpu_affinity
condition|)
do|;
if|if
condition|(
name|cpuset_setaffinity
argument_list|(
name|CPU_LEVEL_WHICH
argument_list|,
name|CPU_WHICH_PID
argument_list|,
operator|-
literal|1
argument_list|,
sizeof|sizeof
argument_list|(
name|cpuset_t
argument_list|)
argument_list|,
operator|&
name|mask
argument_list|)
operator|==
operator|-
literal|1
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"cpuset_setaffinity() failed"
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_elif
elif|#
directive|elif
operator|(
name|NGX_HAVE_SCHED_SETAFFINITY
operator|)
end_elif

begin_function
name|void
DECL|function|ngx_setaffinity (uint64_t cpu_affinity,ngx_log_t * log)
name|ngx_setaffinity
parameter_list|(
name|uint64_t
name|cpu_affinity
parameter_list|,
name|ngx_log_t
modifier|*
name|log
parameter_list|)
block|{
name|cpu_set_t
name|mask
decl_stmt|;
name|ngx_uint_t
name|i
decl_stmt|;
name|ngx_log_error
argument_list|(
name|NGX_LOG_NOTICE
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"sched_setaffinity(0x%08Xl)"
argument_list|,
name|cpu_affinity
argument_list|)
expr_stmt|;
name|CPU_ZERO
argument_list|(
operator|&
name|mask
argument_list|)
expr_stmt|;
name|i
operator|=
literal|0
expr_stmt|;
do|do
block|{
if|if
condition|(
name|cpu_affinity
operator|&
literal|1
condition|)
block|{
name|CPU_SET
argument_list|(
name|i
argument_list|,
operator|&
name|mask
argument_list|)
expr_stmt|;
block|}
name|i
operator|++
expr_stmt|;
name|cpu_affinity
operator|>>=
literal|1
expr_stmt|;
block|}
do|while
condition|(
name|cpu_affinity
condition|)
do|;
if|if
condition|(
name|sched_setaffinity
argument_list|(
literal|0
argument_list|,
sizeof|sizeof
argument_list|(
name|cpu_set_t
argument_list|)
argument_list|,
operator|&
name|mask
argument_list|)
operator|==
operator|-
literal|1
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"sched_setaffinity() failed"
argument_list|)
expr_stmt|;
block|}
block|}
end_function

begin_endif
endif|#
directive|endif
end_endif

end_unit

