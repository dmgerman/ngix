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

begin_comment
comment|/*  * The threads implementation uses the rfork(RFPROC|RFTHREAD|RFMEM)  * to create threads.  All threads use the stacks of the same size mmap()ed  * below the main stack.  Thus the stack pointer is used to determine  * the current thread id.  *  * The mutex implementation uses the ngx_atomic_cmp_set() operation  * to acquire mutex and the SysV semaphore to wait on a mutex or to wake up  * the waiting threads.  *  * The condition variable implementation uses the SysV semaphore set of two  * semaphores. The first is used by the CV mutex, and the second is used  * by CV itself.  *  * This threads implementation currently works on i486 and amd64  * platforms only.  */
end_comment

begin_decl_stmt
DECL|variable|ngx_threaded
name|ngx_int_t
name|ngx_threaded
decl_stmt|;
end_decl_stmt

begin_function_decl
specifier|static
specifier|inline
name|int
name|ngx_gettid
parameter_list|()
function_decl|;
end_function_decl

begin_decl_stmt
DECL|variable|usrstack
specifier|static
name|char
modifier|*
name|usrstack
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|rz_size
specifier|static
name|size_t
name|rz_size
init|=
comment|/* STUB: PAGE_SIZE */
literal|4096
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|stack_size
specifier|static
name|size_t
name|stack_size
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|usable_stack_size
specifier|static
name|size_t
name|usable_stack_size
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|last_stack
specifier|static
name|char
modifier|*
name|last_stack
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|nthreads
specifier|static
name|ngx_uint_t
name|nthreads
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|max_threads
specifier|static
name|ngx_uint_t
name|max_threads
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|tids
specifier|static
name|ngx_tid_t
modifier|*
name|tids
decl_stmt|;
end_decl_stmt

begin_comment
DECL|variable|tids
comment|/* the threads tids array */
end_comment

begin_comment
comment|/* the thread-safe libc errno */
end_comment

begin_decl_stmt
DECL|variable|errno0
specifier|static
name|int
name|errno0
decl_stmt|;
end_decl_stmt

begin_comment
DECL|variable|errno0
comment|/* the main thread's errno */
end_comment

begin_decl_stmt
DECL|variable|errnos
specifier|static
name|int
modifier|*
name|errnos
decl_stmt|;
end_decl_stmt

begin_comment
DECL|variable|errnos
comment|/* the threads errno's array */
end_comment

begin_function
DECL|function|__error ()
name|int
modifier|*
name|__error
parameter_list|()
block|{
name|int
name|tid
decl_stmt|;
name|tid
operator|=
name|ngx_gettid
argument_list|()
expr_stmt|;
return|return
name|tid
condition|?
operator|&
name|errnos
index|[
name|tid
operator|-
literal|1
index|]
else|:
operator|&
name|errno0
return|;
block|}
end_function

begin_comment
comment|/*  * __isthreaded enables spinlock() in some libc functions, i.e. in malloc()  * and some other places.  Nevertheless we protect our malloc()/free() calls  * by own mutex that is more efficient than the spinlock.  *  * We define own _spinlock() because a weak referenced _spinlock() stub in  * src/lib/libc/gen/_spinlock_stub.c does nothing.  */
end_comment

begin_decl_stmt
specifier|extern
name|int
name|__isthreaded
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|_spinlock (ngx_atomic_t * lock)
name|void
name|_spinlock
parameter_list|(
name|ngx_atomic_t
modifier|*
name|lock
parameter_list|)
block|{
name|ngx_int_t
name|tries
decl_stmt|;
name|tries
operator|=
literal|0
expr_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
if|if
condition|(
operator|*
name|lock
condition|)
block|{
if|if
condition|(
name|ngx_freebsd_hw_ncpu
operator|>
literal|1
operator|&&
name|tries
operator|++
operator|<
literal|1000
condition|)
block|{
continue|continue;
block|}
name|sched_yield
argument_list|()
expr_stmt|;
name|tries
operator|=
literal|0
expr_stmt|;
block|}
else|else
block|{
if|if
condition|(
name|ngx_atomic_cmp_set
argument_list|(
name|lock
argument_list|,
literal|0
argument_list|,
literal|1
argument_list|)
condition|)
block|{
return|return;
block|}
block|}
block|}
block|}
end_function

begin_function
DECL|function|ngx_create_thread (ngx_tid_t * tid,int (* func)(void * arg),void * arg,ngx_log_t * log)
name|int
name|ngx_create_thread
parameter_list|(
name|ngx_tid_t
modifier|*
name|tid
parameter_list|,
name|int
function_decl|(
modifier|*
name|func
function_decl|)
parameter_list|(
name|void
modifier|*
name|arg
parameter_list|)
parameter_list|,
name|void
modifier|*
name|arg
parameter_list|,
name|ngx_log_t
modifier|*
name|log
parameter_list|)
block|{
name|int
name|id
decl_stmt|,
name|err
decl_stmt|;
name|char
modifier|*
name|stack
decl_stmt|,
modifier|*
name|stack_top
decl_stmt|;
if|if
condition|(
name|nthreads
operator|>=
name|max_threads
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_CRIT
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"no more than %d threads can be created"
argument_list|,
name|max_threads
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
name|last_stack
operator|-=
name|stack_size
expr_stmt|;
name|stack
operator|=
name|mmap
argument_list|(
name|last_stack
argument_list|,
name|usable_stack_size
argument_list|,
name|PROT_READ
operator||
name|PROT_WRITE
argument_list|,
name|MAP_STACK
argument_list|,
operator|-
literal|1
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|stack
operator|==
name|MAP_FAILED
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
literal|"mmap("
name|PTR_FMT
literal|":"
name|SIZE_T_FMT
literal|", MAP_STACK) thread stack failed"
argument_list|,
name|last_stack
argument_list|,
name|usable_stack_size
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|stack
operator|!=
name|last_stack
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"stack address was changed"
argument_list|)
expr_stmt|;
block|}
name|stack_top
operator|=
name|stack
operator|+
name|usable_stack_size
expr_stmt|;
name|ngx_log_debug2
argument_list|(
name|NGX_LOG_DEBUG_CORE
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"thread stack: "
name|PTR_FMT
literal|"-"
name|PTR_FMT
argument_list|,
name|stack
argument_list|,
name|stack_top
argument_list|)
expr_stmt|;
if|#
directive|if
literal|1
name|id
operator|=
name|rfork_thread
argument_list|(
name|RFPROC
operator||
name|RFTHREAD
operator||
name|RFMEM
argument_list|,
name|stack_top
argument_list|,
name|func
argument_list|,
name|arg
argument_list|)
expr_stmt|;
elif|#
directive|elif
literal|1
name|id
operator|=
name|rfork_thread
argument_list|(
name|RFPROC
operator||
name|RFMEM
argument_list|,
name|stack_top
argument_list|,
name|func
argument_list|,
name|arg
argument_list|)
expr_stmt|;
elif|#
directive|elif
literal|1
name|id
operator|=
name|rfork_thread
argument_list|(
name|RFFDG
operator||
name|RFCFDG
argument_list|,
name|stack_top
argument_list|,
name|func
argument_list|,
name|arg
argument_list|)
expr_stmt|;
else|#
directive|else
name|id
operator|=
name|rfork
argument_list|(
name|RFFDG
operator||
name|RFCFDG
argument_list|)
expr_stmt|;
endif|#
directive|endif
name|err
operator|=
name|ngx_errno
expr_stmt|;
if|if
condition|(
name|id
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
name|err
argument_list|,
literal|"rfork() failed"
argument_list|)
expr_stmt|;
block|}
else|else
block|{
operator|*
name|tid
operator|=
name|id
expr_stmt|;
name|nthreads
operator|=
operator|(
name|usrstack
operator|-
name|stack_top
operator|)
operator|/
name|stack_size
expr_stmt|;
name|tids
index|[
name|nthreads
index|]
operator|=
name|id
expr_stmt|;
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_CORE
argument_list|,
name|log
argument_list|,
literal|0
argument_list|,
literal|"rfork()ed thread: %d"
argument_list|,
name|id
argument_list|)
expr_stmt|;
block|}
return|return
name|err
return|;
block|}
end_function

begin_function
DECL|function|ngx_init_threads (int n,size_t size,ngx_cycle_t * cycle)
name|ngx_int_t
name|ngx_init_threads
parameter_list|(
name|int
name|n
parameter_list|,
name|size_t
name|size
parameter_list|,
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
block|{
name|size_t
name|len
decl_stmt|;
name|char
modifier|*
name|red_zone
decl_stmt|,
modifier|*
name|zone
decl_stmt|;
name|max_threads
operator|=
name|n
expr_stmt|;
name|len
operator|=
sizeof|sizeof
argument_list|(
name|usrstack
argument_list|)
expr_stmt|;
if|if
condition|(
name|sysctlbyname
argument_list|(
literal|"kern.usrstack"
argument_list|,
operator|&
name|usrstack
argument_list|,
operator|&
name|len
argument_list|,
name|NULL
argument_list|,
literal|0
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
name|cycle
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"sysctlbyname(kern.usrstack) failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
comment|/* the main thread stack red zone */
name|red_zone
operator|=
name|usrstack
operator|-
operator|(
name|size
operator|+
name|rz_size
operator|)
expr_stmt|;
name|ngx_log_debug2
argument_list|(
name|NGX_LOG_DEBUG_CORE
argument_list|,
name|cycle
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"usrstack: "
name|PTR_FMT
literal|" red zone: "
name|PTR_FMT
argument_list|,
name|usrstack
argument_list|,
name|red_zone
argument_list|)
expr_stmt|;
name|zone
operator|=
name|mmap
argument_list|(
name|red_zone
argument_list|,
name|rz_size
argument_list|,
name|PROT_NONE
argument_list|,
name|MAP_ANON
argument_list|,
operator|-
literal|1
argument_list|,
literal|0
argument_list|)
expr_stmt|;
if|if
condition|(
name|zone
operator|==
name|MAP_FAILED
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|cycle
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"mmap("
name|PTR_FMT
literal|":"
name|SIZE_T_FMT
literal|", PROT_NONE, MAP_ANON) red zone failed"
argument_list|,
name|red_zone
argument_list|,
name|rz_size
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|zone
operator|!=
name|red_zone
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|cycle
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"red zone address was changed"
argument_list|)
expr_stmt|;
block|}
comment|/* create the threads errno array */
if|if
condition|(
operator|!
operator|(
name|errnos
operator|=
name|ngx_calloc
argument_list|(
name|n
operator|*
sizeof|sizeof
argument_list|(
name|int
argument_list|)
argument_list|,
name|cycle
operator|->
name|log
argument_list|)
operator|)
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
comment|/* create the threads tid array */
if|if
condition|(
operator|!
operator|(
name|tids
operator|=
name|ngx_calloc
argument_list|(
operator|(
name|n
operator|+
literal|1
operator|)
operator|*
sizeof|sizeof
argument_list|(
name|ngx_tid_t
argument_list|)
argument_list|,
name|cycle
operator|->
name|log
argument_list|)
operator|)
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|tids
index|[
literal|0
index|]
operator|=
name|ngx_pid
expr_stmt|;
name|nthreads
operator|=
literal|1
expr_stmt|;
name|last_stack
operator|=
name|zone
operator|+
name|rz_size
expr_stmt|;
name|usable_stack_size
operator|=
name|size
expr_stmt|;
name|stack_size
operator|=
name|size
operator|+
name|rz_size
expr_stmt|;
comment|/* allow the spinlock in libc malloc() */
name|__isthreaded
operator|=
literal|1
expr_stmt|;
name|ngx_threaded
operator|=
literal|1
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_gettid ()
specifier|static
specifier|inline
name|int
name|ngx_gettid
parameter_list|()
block|{
name|char
modifier|*
name|sp
decl_stmt|;
if|if
condition|(
name|stack_size
operator|==
literal|0
condition|)
block|{
return|return
literal|0
return|;
block|}
if|#
directive|if
operator|(
name|__i386__
operator|)
asm|__asm__
specifier|volatile
asm|("mov %%esp, %0" : "=q" (sp));
elif|#
directive|elif
operator|(
name|__amd64__
operator|)
asm|__asm__
specifier|volatile
asm|("mov %%rsp, %0" : "=q" (sp));
endif|#
directive|endif
return|return
operator|(
name|usrstack
operator|-
name|sp
operator|)
operator|/
name|stack_size
return|;
block|}
end_function

begin_function
DECL|function|ngx_thread_self ()
name|ngx_tid_t
name|ngx_thread_self
parameter_list|()
block|{
name|int
name|tid
decl_stmt|;
name|ngx_tid_t
name|pid
decl_stmt|;
name|tid
operator|=
name|ngx_gettid
argument_list|()
expr_stmt|;
if|if
condition|(
name|tids
operator|==
name|NULL
condition|)
block|{
return|return
name|ngx_pid
return|;
block|}
if|#
directive|if
literal|0
block_content|if (tids[tid] == 0) {         pid = ngx_pid;         tids[tid] = pid;         return pid;     }
endif|#
directive|endif
return|return
name|tids
index|[
name|tid
index|]
return|;
block|}
end_function

begin_function
DECL|function|ngx_mutex_init (ngx_log_t * log,uint flags)
name|ngx_mutex_t
modifier|*
name|ngx_mutex_init
parameter_list|(
name|ngx_log_t
modifier|*
name|log
parameter_list|,
name|uint
name|flags
parameter_list|)
block|{
name|int
name|nsem
decl_stmt|,
name|i
decl_stmt|;
name|ngx_mutex_t
modifier|*
name|m
decl_stmt|;
name|union
name|semun
name|op
decl_stmt|;
if|if
condition|(
operator|!
operator|(
name|m
operator|=
name|ngx_alloc
argument_list|(
sizeof|sizeof
argument_list|(
name|ngx_mutex_t
argument_list|)
argument_list|,
name|log
argument_list|)
operator|)
condition|)
block|{
return|return
name|NULL
return|;
block|}
name|m
operator|->
name|lock
operator|=
literal|0
expr_stmt|;
name|m
operator|->
name|log
operator|=
name|log
expr_stmt|;
if|if
condition|(
name|flags
operator|&
name|NGX_MUTEX_LIGHT
condition|)
block|{
name|m
operator|->
name|semid
operator|=
operator|-
literal|1
expr_stmt|;
return|return
name|m
return|;
block|}
name|nsem
operator|=
name|flags
operator|&
name|NGX_MUTEX_CV
condition|?
literal|2
else|:
literal|1
expr_stmt|;
name|m
operator|->
name|semid
operator|=
name|semget
argument_list|(
name|IPC_PRIVATE
argument_list|,
name|nsem
argument_list|,
name|SEM_R
operator||
name|SEM_A
argument_list|)
expr_stmt|;
if|if
condition|(
name|m
operator|->
name|semid
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
literal|"semget() failed"
argument_list|)
expr_stmt|;
return|return
name|NULL
return|;
block|}
name|op
operator|.
name|val
operator|=
literal|0
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|nsem
condition|;
name|i
operator|++
control|)
block|{
if|if
condition|(
name|semctl
argument_list|(
name|m
operator|->
name|semid
argument_list|,
name|i
argument_list|,
name|SETVAL
argument_list|,
name|op
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
literal|"semctl(SETVAL) failed"
argument_list|)
expr_stmt|;
if|if
condition|(
name|semctl
argument_list|(
name|m
operator|->
name|semid
argument_list|,
literal|0
argument_list|,
name|IPC_RMID
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
name|m
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"semctl(IPC_RMID) failed"
argument_list|)
expr_stmt|;
block|}
return|return
name|NULL
return|;
block|}
block|}
return|return
name|m
return|;
block|}
end_function

begin_function
DECL|function|ngx_mutex_done (ngx_mutex_t * m)
name|void
name|ngx_mutex_done
parameter_list|(
name|ngx_mutex_t
modifier|*
name|m
parameter_list|)
block|{
if|if
condition|(
name|semctl
argument_list|(
name|m
operator|->
name|semid
argument_list|,
literal|0
argument_list|,
name|IPC_RMID
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
name|m
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"semctl(IPC_RMID) failed"
argument_list|)
expr_stmt|;
block|}
name|ngx_free
argument_list|(
operator|(
name|void
operator|*
operator|)
name|m
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|ngx_mutex_do_lock (ngx_mutex_t * m,ngx_int_t try)
name|ngx_int_t
name|ngx_mutex_do_lock
parameter_list|(
name|ngx_mutex_t
modifier|*
name|m
parameter_list|,
name|ngx_int_t
name|try
parameter_list|)
block|{
name|uint32_t
name|lock
decl_stmt|,
name|new
decl_stmt|,
name|old
decl_stmt|;
name|ngx_uint_t
name|tries
decl_stmt|;
name|struct
name|sembuf
name|op
decl_stmt|;
if|if
condition|(
operator|!
name|ngx_threaded
condition|)
block|{
return|return
name|NGX_OK
return|;
block|}
if|#
directive|if
operator|(
name|NGX_DEBUG
operator|)
if|if
condition|(
name|try
condition|)
block|{
name|ngx_log_debug2
argument_list|(
name|NGX_LOG_DEBUG_CORE
argument_list|,
name|m
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"try lock mutex "
name|PTR_FMT
literal|" lock:%X"
argument_list|,
name|m
argument_list|,
name|m
operator|->
name|lock
argument_list|)
expr_stmt|;
block|}
else|else
block|{
name|ngx_log_debug2
argument_list|(
name|NGX_LOG_DEBUG_CORE
argument_list|,
name|m
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"lock mutex "
name|PTR_FMT
literal|" lock:%X"
argument_list|,
name|m
argument_list|,
name|m
operator|->
name|lock
argument_list|)
expr_stmt|;
block|}
endif|#
directive|endif
name|old
operator|=
name|m
operator|->
name|lock
expr_stmt|;
name|tries
operator|=
literal|0
expr_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
if|if
condition|(
name|old
operator|&
name|NGX_MUTEX_LOCK_BUSY
condition|)
block|{
if|if
condition|(
name|try
condition|)
block|{
return|return
name|NGX_AGAIN
return|;
block|}
if|if
condition|(
name|ngx_freebsd_hw_ncpu
operator|>
literal|1
operator|&&
name|tries
operator|++
operator|<
literal|1000
condition|)
block|{
comment|/* the spinlock is used only on the SMP system */
name|old
operator|=
name|m
operator|->
name|lock
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|m
operator|->
name|semid
operator|==
operator|-
literal|1
condition|)
block|{
name|sched_yield
argument_list|()
expr_stmt|;
name|tries
operator|=
literal|0
expr_stmt|;
name|old
operator|=
name|m
operator|->
name|lock
expr_stmt|;
continue|continue;
block|}
name|ngx_log_debug2
argument_list|(
name|NGX_LOG_DEBUG_CORE
argument_list|,
name|m
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"mutex "
name|PTR_FMT
literal|" lock:%X"
argument_list|,
name|m
argument_list|,
name|m
operator|->
name|lock
argument_list|)
expr_stmt|;
comment|/*              * The mutex is locked so we increase a number              * of the threads that are waiting on the mutex              */
name|lock
operator|=
name|old
operator|+
literal|1
expr_stmt|;
if|if
condition|(
operator|(
name|lock
operator|&
operator|~
name|NGX_MUTEX_LOCK_BUSY
operator|)
operator|>
name|nthreads
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|m
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"%d threads wait for mutex "
name|PTR_FMT
literal|", while only %d threads are available"
argument_list|,
name|lock
operator|&
operator|~
name|NGX_MUTEX_LOCK_BUSY
argument_list|,
name|m
argument_list|,
name|nthreads
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|ngx_atomic_cmp_set
argument_list|(
operator|&
name|m
operator|->
name|lock
argument_list|,
name|old
argument_list|,
name|lock
argument_list|)
condition|)
block|{
name|ngx_log_debug2
argument_list|(
name|NGX_LOG_DEBUG_CORE
argument_list|,
name|m
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"wait mutex "
name|PTR_FMT
literal|" lock:%X"
argument_list|,
name|m
argument_list|,
name|m
operator|->
name|lock
argument_list|)
expr_stmt|;
comment|/*                  * The number of the waiting threads has been increased                  * and we would wait on the SysV semaphore.                  * A semaphore should wake up us more efficiently than                  * a simple usleep().                  */
name|op
operator|.
name|sem_num
operator|=
literal|0
expr_stmt|;
name|op
operator|.
name|sem_op
operator|=
operator|-
literal|1
expr_stmt|;
name|op
operator|.
name|sem_flg
operator|=
name|SEM_UNDO
expr_stmt|;
if|if
condition|(
name|semop
argument_list|(
name|m
operator|->
name|semid
argument_list|,
operator|&
name|op
argument_list|,
literal|1
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
name|m
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"semop() failed while waiting "
literal|"on mutex "
name|PTR_FMT
argument_list|,
name|m
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
name|tries
operator|=
literal|0
expr_stmt|;
name|old
operator|=
name|m
operator|->
name|lock
expr_stmt|;
continue|continue;
block|}
name|old
operator|=
name|m
operator|->
name|lock
expr_stmt|;
block|}
else|else
block|{
name|lock
operator|=
name|old
operator||
name|NGX_MUTEX_LOCK_BUSY
expr_stmt|;
if|if
condition|(
name|ngx_atomic_cmp_set
argument_list|(
operator|&
name|m
operator|->
name|lock
argument_list|,
name|old
argument_list|,
name|lock
argument_list|)
condition|)
block|{
comment|/* we locked the mutex */
break|break;
block|}
name|old
operator|=
name|m
operator|->
name|lock
expr_stmt|;
block|}
if|if
condition|(
name|tries
operator|++
operator|>
literal|1000
condition|)
block|{
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_CORE
argument_list|,
name|m
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"mutex "
name|PTR_FMT
literal|" is contested"
argument_list|,
name|m
argument_list|)
expr_stmt|;
comment|/* the mutex is probably contested so we are giving up now */
name|sched_yield
argument_list|()
expr_stmt|;
name|tries
operator|=
literal|0
expr_stmt|;
name|old
operator|=
name|m
operator|->
name|lock
expr_stmt|;
block|}
block|}
name|ngx_log_debug2
argument_list|(
name|NGX_LOG_DEBUG_CORE
argument_list|,
name|m
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"mutex "
name|PTR_FMT
literal|" is locked, lock:%X"
argument_list|,
name|m
argument_list|,
name|m
operator|->
name|lock
argument_list|)
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_mutex_unlock (ngx_mutex_t * m)
name|ngx_int_t
name|ngx_mutex_unlock
parameter_list|(
name|ngx_mutex_t
modifier|*
name|m
parameter_list|)
block|{
name|uint32_t
name|lock
decl_stmt|,
name|new
decl_stmt|,
name|old
decl_stmt|;
name|struct
name|sembuf
name|op
decl_stmt|;
if|if
condition|(
operator|!
name|ngx_threaded
condition|)
block|{
return|return
name|NGX_OK
return|;
block|}
name|old
operator|=
name|m
operator|->
name|lock
expr_stmt|;
if|if
condition|(
operator|!
operator|(
name|old
operator|&
name|NGX_MUTEX_LOCK_BUSY
operator|)
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|m
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"tring to unlock the free mutex "
name|PTR_FMT
argument_list|,
name|m
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
comment|/* free the mutex */
for|for
control|(
init|;
condition|;
control|)
block|{
name|lock
operator|=
name|old
operator|&
operator|~
name|NGX_MUTEX_LOCK_BUSY
expr_stmt|;
if|if
condition|(
name|ngx_atomic_cmp_set
argument_list|(
operator|&
name|m
operator|->
name|lock
argument_list|,
name|old
argument_list|,
name|lock
argument_list|)
condition|)
block|{
break|break;
block|}
name|old
operator|=
name|m
operator|->
name|lock
expr_stmt|;
block|}
if|if
condition|(
name|m
operator|->
name|semid
operator|==
operator|-
literal|1
condition|)
block|{
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_CORE
argument_list|,
name|m
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"mutex "
name|PTR_FMT
literal|" is unlocked"
argument_list|,
name|m
argument_list|)
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
comment|/* check weather we need to wake up a waiting thread */
name|old
operator|=
name|m
operator|->
name|lock
expr_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
if|if
condition|(
name|old
operator|&
name|NGX_MUTEX_LOCK_BUSY
condition|)
block|{
comment|/* the mutex is just locked by another thread */
break|break;
block|}
if|if
condition|(
name|old
operator|==
literal|0
condition|)
block|{
break|break;
block|}
comment|/* there are the waiting threads */
name|lock
operator|=
name|old
operator|-
literal|1
expr_stmt|;
if|if
condition|(
name|ngx_atomic_cmp_set
argument_list|(
operator|&
name|m
operator|->
name|lock
argument_list|,
name|old
argument_list|,
name|lock
argument_list|)
condition|)
block|{
comment|/* wake up the thread that waits on semaphore */
name|op
operator|.
name|sem_num
operator|=
literal|0
expr_stmt|;
name|op
operator|.
name|sem_op
operator|=
literal|1
expr_stmt|;
name|op
operator|.
name|sem_flg
operator|=
name|SEM_UNDO
expr_stmt|;
if|if
condition|(
name|semop
argument_list|(
name|m
operator|->
name|semid
argument_list|,
operator|&
name|op
argument_list|,
literal|1
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
name|m
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"semop() failed while waking up on mutex "
name|PTR_FMT
argument_list|,
name|m
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
break|break;
block|}
name|old
operator|=
name|m
operator|->
name|lock
expr_stmt|;
block|}
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_CORE
argument_list|,
name|m
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"mutex "
name|PTR_FMT
literal|" is unlocked"
argument_list|,
name|m
argument_list|)
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

end_unit

