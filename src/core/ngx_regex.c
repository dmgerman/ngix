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

begin_function_decl
specifier|static
name|void
modifier|*
name|ngx_regex_malloc
parameter_list|(
name|size_t
name|size
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
specifier|static
name|void
name|ngx_regex_free
parameter_list|(
name|void
modifier|*
name|p
parameter_list|)
function_decl|;
end_function_decl

begin_decl_stmt
DECL|variable|ngx_pcre_pool
specifier|static
name|ngx_pool_t
modifier|*
name|ngx_pcre_pool
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|ngx_regex_init ()
name|void
name|ngx_regex_init
parameter_list|()
block|{
name|pcre_malloc
operator|=
name|ngx_regex_malloc
expr_stmt|;
name|pcre_free
operator|=
name|ngx_regex_free
expr_stmt|;
block|}
end_function

begin_function
DECL|function|ngx_regex_compile (ngx_str_t * pattern,ngx_int_t options,ngx_pool_t * pool,ngx_str_t * err)
name|ngx_regex_t
modifier|*
name|ngx_regex_compile
parameter_list|(
name|ngx_str_t
modifier|*
name|pattern
parameter_list|,
name|ngx_int_t
name|options
parameter_list|,
name|ngx_pool_t
modifier|*
name|pool
parameter_list|,
name|ngx_str_t
modifier|*
name|err
parameter_list|)
block|{
name|int
name|erroff
decl_stmt|;
specifier|const
name|char
modifier|*
name|errstr
decl_stmt|;
name|ngx_regex_t
modifier|*
name|re
decl_stmt|;
if|#
directive|if
operator|(
name|NGX_THREADS
operator|)
name|ngx_core_tls_t
modifier|*
name|tls
decl_stmt|;
if|#
directive|if
operator|(
name|NGX_SUPPRESS_WARN
operator|)
name|tls
operator|=
name|NULL
expr_stmt|;
endif|#
directive|endif
if|if
condition|(
name|ngx_threaded
condition|)
block|{
name|tls
operator|=
name|ngx_thread_get_tls
argument_list|(
name|ngx_core_tls_key
argument_list|)
expr_stmt|;
name|tls
operator|->
name|pool
operator|=
name|pool
expr_stmt|;
block|}
else|else
block|{
name|ngx_pcre_pool
operator|=
name|pool
expr_stmt|;
block|}
else|#
directive|else
name|ngx_pcre_pool
operator|=
name|pool
expr_stmt|;
endif|#
directive|endif
name|re
operator|=
name|pcre_compile
argument_list|(
operator|(
specifier|const
name|char
operator|*
operator|)
name|pattern
operator|->
name|data
argument_list|,
operator|(
name|int
operator|)
name|options
argument_list|,
operator|&
name|errstr
argument_list|,
operator|&
name|erroff
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
if|if
condition|(
name|re
operator|==
name|NULL
condition|)
block|{
if|if
condition|(
operator|(
name|size_t
operator|)
name|erroff
operator|==
name|pattern
operator|->
name|len
condition|)
block|{
name|ngx_snprintf
argument_list|(
operator|(
name|char
operator|*
operator|)
name|err
operator|->
name|data
argument_list|,
name|err
operator|->
name|len
operator|-
literal|1
argument_list|,
literal|"pcre_compile() failed: %s in \"%s\""
argument_list|,
name|errstr
argument_list|,
name|pattern
operator|->
name|data
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|ngx_snprintf
argument_list|(
operator|(
name|char
operator|*
operator|)
name|err
operator|->
name|data
argument_list|,
name|err
operator|->
name|len
operator|-
literal|1
argument_list|,
literal|"pcre_compile() failed: %s in \"%s\" at \"%s\""
argument_list|,
name|errstr
argument_list|,
name|pattern
operator|->
name|data
argument_list|,
name|pattern
operator|->
name|data
operator|+
name|erroff
argument_list|)
expr_stmt|;
block|}
block|}
comment|/* ensure that there is no current pool */
if|#
directive|if
operator|(
name|NGX_THREADS
operator|)
if|if
condition|(
name|ngx_threaded
condition|)
block|{
name|tls
operator|->
name|pool
operator|=
name|NULL
expr_stmt|;
block|}
else|else
block|{
name|ngx_pcre_pool
operator|=
name|NULL
expr_stmt|;
block|}
else|#
directive|else
name|ngx_pcre_pool
operator|=
name|NULL
expr_stmt|;
endif|#
directive|endif
return|return
name|re
return|;
block|}
end_function

begin_function
DECL|function|ngx_regex_exec (ngx_regex_t * re,ngx_str_t * s,int * matches,ngx_int_t size)
name|ngx_int_t
name|ngx_regex_exec
parameter_list|(
name|ngx_regex_t
modifier|*
name|re
parameter_list|,
name|ngx_str_t
modifier|*
name|s
parameter_list|,
name|int
modifier|*
name|matches
parameter_list|,
name|ngx_int_t
name|size
parameter_list|)
block|{
name|int
name|rc
decl_stmt|;
name|rc
operator|=
name|pcre_exec
argument_list|(
name|re
argument_list|,
name|NULL
argument_list|,
operator|(
specifier|const
name|char
operator|*
operator|)
name|s
operator|->
name|data
argument_list|,
name|s
operator|->
name|len
argument_list|,
literal|0
argument_list|,
literal|0
argument_list|,
name|matches
argument_list|,
name|size
argument_list|)
expr_stmt|;
if|if
condition|(
name|rc
operator|==
operator|-
literal|1
condition|)
block|{
return|return
name|NGX_DECLINED
return|;
block|}
return|return
name|rc
return|;
block|}
end_function

begin_function
DECL|function|ngx_regex_malloc (size_t size)
specifier|static
name|void
modifier|*
name|ngx_regex_malloc
parameter_list|(
name|size_t
name|size
parameter_list|)
block|{
name|ngx_pool_t
modifier|*
name|pool
decl_stmt|;
if|#
directive|if
operator|(
name|NGX_THREADS
operator|)
name|ngx_core_tls_t
modifier|*
name|tls
decl_stmt|;
if|if
condition|(
name|ngx_threaded
condition|)
block|{
name|tls
operator|=
name|ngx_thread_get_tls
argument_list|(
name|ngx_core_tls_key
argument_list|)
expr_stmt|;
name|pool
operator|=
name|tls
operator|->
name|pool
expr_stmt|;
block|}
else|else
block|{
name|pool
operator|=
name|ngx_pcre_pool
expr_stmt|;
block|}
else|#
directive|else
name|pool
operator|=
name|ngx_pcre_pool
expr_stmt|;
endif|#
directive|endif
if|if
condition|(
name|pool
condition|)
block|{
return|return
name|ngx_palloc
argument_list|(
name|pool
argument_list|,
name|size
argument_list|)
return|;
block|}
return|return
name|NULL
return|;
block|}
end_function

begin_function
DECL|function|ngx_regex_free (void * p)
specifier|static
name|void
name|ngx_regex_free
parameter_list|(
name|void
modifier|*
name|p
parameter_list|)
block|{
return|return;
block|}
end_function

end_unit

