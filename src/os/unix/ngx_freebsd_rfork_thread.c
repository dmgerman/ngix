begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (C) 2002-2004 Igor Sysoev, http://sysoev.ru/en/  */
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
comment|/*  * The threads implementation uses the rfork(RFPROC|RFTHREAD|RFMEM) syscall  * to create threads.  All threads use the stacks of the same size mmap()ed  * below the main stack.  Thus the current thread id is determinated via  * the stack pointer value.  *  * The mutex implementation uses the ngx_atomic_cmp_set() operation  * to acquire a mutex and the SysV semaphore to wait on a mutex and to wake up  * the waiting threads.  The light mutex does not use semaphore, so after  * spinning in the lock the thread calls sched_yield().  However the light  * mutecies are intended to be used with the "trylock" operation only.  * The SysV semop() is a cheap syscall, particularly if it has little sembuf's  * and does not use SEM_UNDO.  *  * The condition variable implementation uses signal #64.  The signal handler  * is SIG_IGN so the kill() is a cheap syscall.  The thread waits a signal  * in kevent().  The use of the EVFILT_SIGNAL is safe since FreeBSD 4.7.  *  * This threads implementation currently works on i386 (486+) and amd64  * platforms only.  */
end_comment

begin_decl_stmt
DECL|variable|ngx_freebsd_kern_usrstack
name|char
modifier|*
name|ngx_freebsd_kern_usrstack
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|ngx_thread_stack_size
name|size_t
name|ngx_thread_stack_size
decl_stmt|;
end_decl_stmt

begin_decl_stmt
DECL|variable|rz_size
specifier|static
name|size_t
name|rz_size
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
comment|/*  * __isthreaded enables the spinlocks in some libc functions, i.e. in malloc()  * and some other places.  Nevertheless we protect our malloc()/free() calls  * by own mutex that is more efficient than the spinlock.  *  * _spinlock() is a weak referenced stub in src/lib/libc/gen/_spinlock_stub.c  * that does nothing.  */
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
name|ngx_ncpu
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

begin_comment
comment|/*  * Before FreeBSD 5.1 _spinunlock() is a simple #define in  * src/lib/libc/include/spinlock.h that zeroes lock.  *  * Since FreeBSD 5.1 _spinunlock() is a weak referenced stub in  * src/lib/libc/gen/_spinlock_stub.c that does nothing.  */
end_comment

begin_ifndef
ifndef|#
directive|ifndef
name|_spinunlock
end_ifndef

begin_function
DECL|function|_spinunlock (ngx_atomic_t * lock)
name|void
name|_spinunlock
parameter_list|(
name|ngx_atomic_t
modifier|*
name|lock
parameter_list|)
block|{
operator|*
name|lock
operator|=
literal|0
expr_stmt|;
block|}
end_function

begin_endif
endif|#
directive|endif
end_endif

begin_function
DECL|function|ngx_create_thread (ngx_tid_t * tid,void * (* func)(void * arg),void * arg,ngx_log_t * log)
name|int
name|ngx_create_thread
parameter_list|(
name|ngx_tid_t
modifier|*
name|tid
parameter_list|,
name|void
modifier|*
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
name|ngx_thread_stack_size
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
name|ngx_set_errno
argument_list|(
literal|0
argument_list|)
expr_stmt|;
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
operator|(
name|ngx_rfork_thread_func_pt
operator|)
name|func
argument_list|,
name|arg
argument_list|)
expr_stmt|;
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
name|ngx_freebsd_kern_usrstack
operator|-
name|stack_top
operator|)
operator|/
name|ngx_thread_stack_size
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
name|char
modifier|*
name|red_zone
decl_stmt|,
modifier|*
name|zone
decl_stmt|;
name|size_t
name|len
decl_stmt|;
name|ngx_int_t
name|i
decl_stmt|;
name|struct
name|sigaction
name|sa
decl_stmt|;
name|max_threads
operator|=
name|n
operator|+
literal|1
expr_stmt|;
for|for
control|(
name|i
operator|=
literal|0
init|;
name|i
operator|<
name|n
condition|;
name|i
operator|++
control|)
block|{
name|ngx_memzero
argument_list|(
operator|&
name|sa
argument_list|,
sizeof|sizeof
argument_list|(
expr|struct
name|sigaction
argument_list|)
argument_list|)
expr_stmt|;
name|sa
operator|.
name|sa_handler
operator|=
name|SIG_IGN
expr_stmt|;
name|sigemptyset
argument_list|(
operator|&
name|sa
operator|.
name|sa_mask
argument_list|)
expr_stmt|;
if|if
condition|(
name|sigaction
argument_list|(
name|NGX_CV_SIGNAL
argument_list|,
operator|&
name|sa
argument_list|,
name|NULL
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
literal|"sigaction(%d, SIG_IGN) failed"
argument_list|,
name|NGX_CV_SIGNAL
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
block|}
name|len
operator|=
sizeof|sizeof
argument_list|(
name|ngx_freebsd_kern_usrstack
argument_list|)
expr_stmt|;
if|if
condition|(
name|sysctlbyname
argument_list|(
literal|"kern.usrstack"
argument_list|,
operator|&
name|ngx_freebsd_kern_usrstack
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
name|rz_size
operator|=
name|ngx_pagesize
expr_stmt|;
name|red_zone
operator|=
name|ngx_freebsd_kern_usrstack
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
name|ngx_freebsd_kern_usrstack
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
name|ngx_thread_stack_size
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
name|m
operator|->
name|semid
operator|=
name|semget
argument_list|(
name|IPC_PRIVATE
argument_list|,
literal|1
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
return|return
name|m
return|;
block|}
end_function

begin_function
DECL|function|ngx_mutex_destroy (ngx_mutex_t * m)
name|void
name|ngx_mutex_destroy
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
DECL|function|ngx_mutex_dolock (ngx_mutex_t * m,ngx_int_t try)
name|ngx_int_t
name|ngx_mutex_dolock
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
comment|/*                  * The number of the waiting threads has been increased                  * and we would wait on the SysV semaphore.                  * A semaphore should wake up us more efficiently than                  * a simple sched_yield() or usleep().                  */
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
literal|0
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
literal|"mutex waked up "
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
literal|0
argument_list|,
literal|"trying to unlock the free mutex "
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
if|#
directive|if
literal|0
block_content|ngx_log_debug2(NGX_LOG_DEBUG_CORE, m->log, 0,                    "unlock mutex " PTR_FMT " lock:%X", m, old);
endif|#
directive|endif
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
comment|/* check whether we need to wake up a waiting thread */
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
literal|"wake up mutex "
name|PTR_FMT
literal|""
argument_list|,
name|m
argument_list|)
expr_stmt|;
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
literal|0
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

begin_function
DECL|function|ngx_cond_init (ngx_log_t * log)
name|ngx_cond_t
modifier|*
name|ngx_cond_init
parameter_list|(
name|ngx_log_t
modifier|*
name|log
parameter_list|)
block|{
name|ngx_cond_t
modifier|*
name|cv
decl_stmt|;
if|if
condition|(
operator|!
operator|(
name|cv
operator|=
name|ngx_alloc
argument_list|(
sizeof|sizeof
argument_list|(
name|ngx_cond_t
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
name|cv
operator|->
name|signo
operator|=
name|NGX_CV_SIGNAL
expr_stmt|;
name|cv
operator|->
name|tid
operator|=
literal|0
expr_stmt|;
name|cv
operator|->
name|log
operator|=
name|log
expr_stmt|;
name|cv
operator|->
name|kq
operator|=
operator|-
literal|1
expr_stmt|;
return|return
name|cv
return|;
block|}
end_function

begin_function
DECL|function|ngx_cond_destroy (ngx_cond_t * cv)
name|void
name|ngx_cond_destroy
parameter_list|(
name|ngx_cond_t
modifier|*
name|cv
parameter_list|)
block|{
if|if
condition|(
name|close
argument_list|(
name|cv
operator|->
name|kq
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
name|cv
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"kqueue close() failed"
argument_list|)
expr_stmt|;
block|}
name|ngx_free
argument_list|(
name|cv
argument_list|)
expr_stmt|;
block|}
end_function

begin_function
DECL|function|ngx_cond_wait (ngx_cond_t * cv,ngx_mutex_t * m)
name|ngx_int_t
name|ngx_cond_wait
parameter_list|(
name|ngx_cond_t
modifier|*
name|cv
parameter_list|,
name|ngx_mutex_t
modifier|*
name|m
parameter_list|)
block|{
name|int
name|n
decl_stmt|;
name|ngx_err_t
name|err
decl_stmt|;
name|struct
name|kevent
name|kev
decl_stmt|;
name|struct
name|timespec
name|ts
decl_stmt|;
if|if
condition|(
name|cv
operator|->
name|kq
operator|==
operator|-
literal|1
condition|)
block|{
comment|/*          * We have to add the EVFILT_SIGNAL filter in the rfork()ed thread.          * Otherwise the thread would not get a signal event.          *          * However, we have not to open the kqueue in the thread,          * it is simply handy do it together.          */
name|cv
operator|->
name|kq
operator|=
name|kqueue
argument_list|()
expr_stmt|;
if|if
condition|(
name|cv
operator|->
name|kq
operator|==
operator|-
literal|1
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|cv
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"kqueue() failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
name|ngx_log_debug2
argument_list|(
name|NGX_LOG_DEBUG_CORE
argument_list|,
name|cv
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"cv kq:%d signo:%d"
argument_list|,
name|cv
operator|->
name|kq
argument_list|,
name|cv
operator|->
name|signo
argument_list|)
expr_stmt|;
name|kev
operator|.
name|ident
operator|=
name|cv
operator|->
name|signo
expr_stmt|;
name|kev
operator|.
name|filter
operator|=
name|EVFILT_SIGNAL
expr_stmt|;
name|kev
operator|.
name|flags
operator|=
name|EV_ADD
expr_stmt|;
name|kev
operator|.
name|fflags
operator|=
literal|0
expr_stmt|;
name|kev
operator|.
name|data
operator|=
literal|0
expr_stmt|;
name|kev
operator|.
name|udata
operator|=
name|NULL
expr_stmt|;
name|ts
operator|.
name|tv_sec
operator|=
literal|0
expr_stmt|;
name|ts
operator|.
name|tv_nsec
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|kevent
argument_list|(
name|cv
operator|->
name|kq
argument_list|,
operator|&
name|kev
argument_list|,
literal|1
argument_list|,
name|NULL
argument_list|,
literal|0
argument_list|,
operator|&
name|ts
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
name|cv
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"kevent() failed"
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
block|}
if|if
condition|(
name|ngx_mutex_unlock
argument_list|(
name|m
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|ngx_log_debug3
argument_list|(
name|NGX_LOG_DEBUG_CORE
argument_list|,
name|cv
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"cv "
name|PTR_FMT
literal|" wait, kq:%d, signo:%d"
argument_list|,
name|cv
argument_list|,
name|cv
operator|->
name|kq
argument_list|,
name|cv
operator|->
name|signo
argument_list|)
expr_stmt|;
for|for
control|(
init|;
condition|;
control|)
block|{
name|n
operator|=
name|kevent
argument_list|(
name|cv
operator|->
name|kq
argument_list|,
name|NULL
argument_list|,
literal|0
argument_list|,
operator|&
name|kev
argument_list|,
literal|1
argument_list|,
name|NULL
argument_list|)
expr_stmt|;
name|ngx_log_debug2
argument_list|(
name|NGX_LOG_DEBUG_CORE
argument_list|,
name|cv
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"cv "
name|PTR_FMT
literal|" kevent: %d"
argument_list|,
name|cv
argument_list|,
name|n
argument_list|)
expr_stmt|;
if|if
condition|(
name|n
operator|==
operator|-
literal|1
condition|)
block|{
name|err
operator|=
name|ngx_errno
expr_stmt|;
name|ngx_log_error
argument_list|(
operator|(
name|err
operator|==
name|NGX_EINTR
operator|)
condition|?
name|NGX_LOG_INFO
else|:
name|NGX_LOG_ALERT
argument_list|,
name|cv
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"kevent() failed while waiting condition variable "
name|PTR_FMT
argument_list|,
name|cv
argument_list|)
expr_stmt|;
if|if
condition|(
name|err
operator|==
name|NGX_EINTR
condition|)
block|{
break|break;
block|}
return|return
name|NGX_ERROR
return|;
block|}
if|if
condition|(
name|n
operator|==
literal|0
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|cv
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"kevent() returned no events "
literal|"while waiting condition variable "
name|PTR_FMT
argument_list|,
name|cv
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|kev
operator|.
name|filter
operator|!=
name|EVFILT_SIGNAL
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|cv
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"kevent() returned unexpected events: %d "
literal|"while waiting condition variable "
name|PTR_FMT
argument_list|,
name|kev
operator|.
name|filter
argument_list|,
name|cv
argument_list|)
expr_stmt|;
continue|continue;
block|}
if|if
condition|(
name|kev
operator|.
name|ident
operator|!=
operator|(
name|uintptr_t
operator|)
name|cv
operator|->
name|signo
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|cv
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"kevent() returned unexpected signal: %d "
argument_list|,
literal|"while waiting condition variable "
name|PTR_FMT
argument_list|,
name|kev
operator|.
name|ident
argument_list|,
name|cv
argument_list|)
expr_stmt|;
continue|continue;
block|}
break|break;
block|}
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_CORE
argument_list|,
name|cv
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"cv "
name|PTR_FMT
literal|" is waked up"
argument_list|,
name|cv
argument_list|)
expr_stmt|;
if|if
condition|(
name|ngx_mutex_lock
argument_list|(
name|m
argument_list|)
operator|==
name|NGX_ERROR
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
return|return
name|NGX_OK
return|;
block|}
end_function

begin_function
DECL|function|ngx_cond_signal (ngx_cond_t * cv)
name|ngx_int_t
name|ngx_cond_signal
parameter_list|(
name|ngx_cond_t
modifier|*
name|cv
parameter_list|)
block|{
name|ngx_log_debug3
argument_list|(
name|NGX_LOG_DEBUG_CORE
argument_list|,
name|cv
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"cv "
name|PTR_FMT
literal|" to signal "
name|PID_T_FMT
literal|" %d"
argument_list|,
name|cv
argument_list|,
name|cv
operator|->
name|tid
argument_list|,
name|cv
operator|->
name|signo
argument_list|)
expr_stmt|;
if|if
condition|(
name|kill
argument_list|(
name|cv
operator|->
name|tid
argument_list|,
name|cv
operator|->
name|signo
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
name|cv
operator|->
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"kill() failed while signaling condition variable "
name|PTR_FMT
argument_list|,
name|cv
argument_list|)
expr_stmt|;
return|return
name|NGX_ERROR
return|;
block|}
name|ngx_log_debug1
argument_list|(
name|NGX_LOG_DEBUG_CORE
argument_list|,
name|cv
operator|->
name|log
argument_list|,
literal|0
argument_list|,
literal|"cv "
name|PTR_FMT
literal|" is signaled"
argument_list|,
name|cv
argument_list|)
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
end_function

end_unit

