begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|_NGX_ATOMIC_H_INCLUDED_
end_ifndef

begin_define
DECL|macro|_NGX_ATOMIC_H_INCLUDED_
define|#
directive|define
name|_NGX_ATOMIC_H_INCLUDED_
end_define

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
name|__i386__
operator|||
name|__amd64__
operator|)
end_if

begin_typedef
DECL|typedef|ngx_atomic_t
typedef|typedef
specifier|volatile
name|uint32_t
name|ngx_atomic_t
typedef|;
end_typedef

begin_if
if|#
directive|if
operator|(
name|NGX_SMP
operator|)
end_if

begin_define
DECL|macro|NGX_SMP_LOCK
define|#
directive|define
name|NGX_SMP_LOCK
value|"lock;"
end_define

begin_else
else|#
directive|else
end_else

begin_define
DECL|macro|NGX_SMP_LOCK
define|#
directive|define
name|NGX_SMP_LOCK
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_function
DECL|function|ngx_atomic_inc (ngx_atomic_t * value)
specifier|static
name|ngx_inline
name|uint32_t
name|ngx_atomic_inc
parameter_list|(
name|ngx_atomic_t
modifier|*
name|value
parameter_list|)
block|{
name|uint32_t
name|old
decl_stmt|;
asm|__asm__
specifier|volatile
asm|(          NGX_SMP_LOCK     "   xaddl  %0, %2;   "     "   incl   %0;       "      : "=q" (old) : "0" (1), "m" (*value));
return|return
name|old
return|;
block|}
end_function

begin_function
DECL|function|ngx_atomic_dec (ngx_atomic_t * value)
specifier|static
name|ngx_inline
name|uint32_t
name|ngx_atomic_dec
parameter_list|(
name|ngx_atomic_t
modifier|*
name|value
parameter_list|)
block|{
name|uint32_t
name|old
decl_stmt|;
asm|__asm__
specifier|volatile
asm|(          NGX_SMP_LOCK     "   xaddl  %0, %1;   "     "   decl   %0;       "      : "=q" (old) : "0" (-1), "m" (*value));
return|return
name|old
return|;
block|}
end_function

begin_function
DECL|function|ngx_atomic_cmp_set (ngx_atomic_t * lock,ngx_atomic_t old,ngx_atomic_t set)
specifier|static
name|ngx_inline
name|uint32_t
name|ngx_atomic_cmp_set
parameter_list|(
name|ngx_atomic_t
modifier|*
name|lock
parameter_list|,
name|ngx_atomic_t
name|old
parameter_list|,
name|ngx_atomic_t
name|set
parameter_list|)
block|{
name|uint32_t
name|res
decl_stmt|;
asm|__asm__
specifier|volatile
asm|(          NGX_SMP_LOCK     "   cmpxchgl  %3, %1;   "     "   setz      %%al;     "     "   movzbl    %%al, %0; "      : "=a" (res) : "m" (*lock), "a" (old), "q" (set));
return|return
name|res
return|;
block|}
end_function

begin_elif
elif|#
directive|elif
operator|(
name|__sparc__
operator|)
end_elif

begin_typedef
DECL|typedef|ngx_atomic_t
typedef|typedef
specifier|volatile
name|uint32_t
name|ngx_atomic_t
typedef|;
end_typedef

begin_function
DECL|function|ngx_atomic_inc (ngx_atomic_t * value)
specifier|static
name|ngx_inline
name|uint32_t
name|ngx_atomic_inc
parameter_list|(
name|ngx_atomic_t
modifier|*
name|value
parameter_list|)
block|{
name|uint32_t
name|old
decl_stmt|,
name|new
decl_stmt|,
name|res
decl_stmt|;
name|old
operator|=
operator|*
name|value
expr_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
name|new
operator|=
name|old
operator|+
literal|1
expr_stmt|;
name|res
operator|=
name|new
expr_stmt|;
asm|__asm__
specifier|volatile
asm|(          "casa [%1]ASI_P, %2, %0"          : "+r" (res) : "r" (value), "r" (old));
if|if
condition|(
name|res
operator|==
name|old
condition|)
block|{
return|return
name|new
return|;
block|}
name|old
operator|=
name|res
expr_stmt|;
block|}
block|}
end_function

begin_comment
comment|/* STUB */
end_comment

begin_define
DECL|macro|ngx_atomic_dec (x)
define|#
directive|define
name|ngx_atomic_dec
parameter_list|(
name|x
parameter_list|)
value|(*(x))--;
end_define

begin_comment
comment|/**/
end_comment

begin_function
DECL|function|ngx_atomic_cmp_set (ngx_atomic_t * lock,ngx_atomic_t old,ngx_atomic_t set)
specifier|static
name|ngx_inline
name|uint32_t
name|ngx_atomic_cmp_set
parameter_list|(
name|ngx_atomic_t
modifier|*
name|lock
parameter_list|,
name|ngx_atomic_t
name|old
parameter_list|,
name|ngx_atomic_t
name|set
parameter_list|)
block|{
name|uint32_t
name|res
init|=
operator|(
name|uint32_t
operator|)
name|set
decl_stmt|;
asm|__asm__
specifier|volatile
asm|(      "casa [%1]ASI_P, %2, %0"      : "+r" (res) : "r" (lock), "r" (old));
return|return
operator|(
name|res
operator|==
name|old
operator|)
return|;
block|}
end_function

begin_else
else|#
directive|else
end_else

begin_typedef
DECL|typedef|ngx_atomic_t
typedef|typedef
specifier|volatile
name|uint32_t
name|ngx_atomic_t
typedef|;
end_typedef

begin_comment
comment|/* STUB */
end_comment

begin_define
DECL|macro|ngx_atomic_inc (x)
define|#
directive|define
name|ngx_atomic_inc
parameter_list|(
name|x
parameter_list|)
value|(*(x))++;
end_define

begin_define
DECL|macro|ngx_atomic_dec (x)
define|#
directive|define
name|ngx_atomic_dec
parameter_list|(
name|x
parameter_list|)
value|(*(x))--;
end_define

begin_define
DECL|macro|ngx_atomic_cmp_set (lock,old,set)
define|#
directive|define
name|ngx_atomic_cmp_set
parameter_list|(
name|lock
parameter_list|,
name|old
parameter_list|,
name|set
parameter_list|)
value|1
end_define

begin_comment
comment|/**/
end_comment

begin_endif
endif|#
directive|endif
end_endif

begin_function_decl
name|void
name|ngx_spinlock
parameter_list|(
name|ngx_atomic_t
modifier|*
name|lock
parameter_list|,
name|ngx_uint_t
name|spin
parameter_list|)
function_decl|;
end_function_decl

begin_define
DECL|macro|ngx_trylock (lock)
define|#
directive|define
name|ngx_trylock
parameter_list|(
name|lock
parameter_list|)
value|(*(lock) == 0&& ngx_atomic_cmp_set(lock, 0, 1))
end_define

begin_define
DECL|macro|ngx_unlock (lock)
define|#
directive|define
name|ngx_unlock
parameter_list|(
name|lock
parameter_list|)
value|*(lock) = 0
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* _NGX_ATOMIC_H_INCLUDED_ */
end_comment

end_unit

