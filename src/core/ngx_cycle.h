begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (C) Igor Sysoev  */
end_comment

begin_ifndef
ifndef|#
directive|ifndef
name|_NGX_CYCLE_H_INCLUDED_
end_ifndef

begin_define
DECL|macro|_NGX_CYCLE_H_INCLUDED_
define|#
directive|define
name|_NGX_CYCLE_H_INCLUDED_
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

begin_define
DECL|macro|NGX_DEBUG_POINTS_STOP
define|#
directive|define
name|NGX_DEBUG_POINTS_STOP
value|1
end_define

begin_define
DECL|macro|NGX_DEBUG_POINTS_ABORT
define|#
directive|define
name|NGX_DEBUG_POINTS_ABORT
value|2
end_define

begin_struct
DECL|struct|ngx_cycle_s
struct|struct
name|ngx_cycle_s
block|{
DECL|member|conf_ctx
name|void
modifier|*
modifier|*
modifier|*
modifier|*
name|conf_ctx
decl_stmt|;
DECL|member|pool
name|ngx_pool_t
modifier|*
name|pool
decl_stmt|;
DECL|member|log
name|ngx_log_t
modifier|*
name|log
decl_stmt|;
DECL|member|new_log
name|ngx_log_t
modifier|*
name|new_log
decl_stmt|;
DECL|member|listening
name|ngx_array_t
name|listening
decl_stmt|;
DECL|member|pathes
name|ngx_array_t
name|pathes
decl_stmt|;
DECL|member|open_files
name|ngx_list_t
name|open_files
decl_stmt|;
DECL|member|connection_n
name|ngx_uint_t
name|connection_n
decl_stmt|;
DECL|member|connections
name|ngx_connection_t
modifier|*
name|connections
decl_stmt|;
DECL|member|read_events
name|ngx_event_t
modifier|*
name|read_events
decl_stmt|;
DECL|member|write_events
name|ngx_event_t
modifier|*
name|write_events
decl_stmt|;
DECL|member|old_cycle
name|ngx_cycle_t
modifier|*
name|old_cycle
decl_stmt|;
DECL|member|conf_file
name|ngx_str_t
name|conf_file
decl_stmt|;
DECL|member|root
name|ngx_str_t
name|root
decl_stmt|;
block|}
struct|;
end_struct

begin_typedef
DECL|struct|__anon2a0454090108
typedef|typedef
struct|struct
block|{
DECL|member|daemon
name|ngx_flag_t
name|daemon
decl_stmt|;
DECL|member|master
name|ngx_flag_t
name|master
decl_stmt|;
DECL|member|worker_processes
name|ngx_int_t
name|worker_processes
decl_stmt|;
DECL|member|debug_points
name|ngx_int_t
name|debug_points
decl_stmt|;
DECL|member|priority
name|int
name|priority
decl_stmt|;
DECL|member|username
name|char
modifier|*
name|username
decl_stmt|;
DECL|member|user
name|ngx_uid_t
name|user
decl_stmt|;
DECL|member|group
name|ngx_gid_t
name|group
decl_stmt|;
DECL|member|pid
name|ngx_str_t
name|pid
decl_stmt|;
DECL|member|newpid
name|ngx_str_t
name|newpid
decl_stmt|;
if|#
directive|if
operator|(
name|NGX_THREADS
operator|)
DECL|member|worker_threads
name|ngx_int_t
name|worker_threads
decl_stmt|;
DECL|member|thread_stack_size
name|size_t
name|thread_stack_size
decl_stmt|;
endif|#
directive|endif
DECL|typedef|ngx_core_conf_t
block|}
name|ngx_core_conf_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon2a0454090208
typedef|typedef
struct|struct
block|{
DECL|member|pool
name|ngx_pool_t
modifier|*
name|pool
decl_stmt|;
comment|/* pcre's malloc() pool */
DECL|typedef|ngx_core_tls_t
block|}
name|ngx_core_tls_t
typedef|;
end_typedef

begin_function_decl
name|ngx_cycle_t
modifier|*
name|ngx_init_cycle
parameter_list|(
name|ngx_cycle_t
modifier|*
name|old_cycle
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_int_t
name|ngx_create_pidfile
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|,
name|ngx_cycle_t
modifier|*
name|old_cycle
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|ngx_delete_pidfile
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|ngx_reopen_files
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|,
name|ngx_uid_t
name|user
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_pid_t
name|ngx_exec_new_binary
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|,
name|char
modifier|*
specifier|const
modifier|*
name|argv
parameter_list|)
function_decl|;
end_function_decl

begin_decl_stmt
specifier|extern
specifier|volatile
name|ngx_cycle_t
modifier|*
name|ngx_cycle
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|ngx_array_t
name|ngx_old_cycles
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|ngx_module_t
name|ngx_core_module
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|ngx_uint_t
name|ngx_test_config
decl_stmt|;
end_decl_stmt

begin_if
if|#
directive|if
operator|(
name|NGX_THREADS
operator|)
end_if

begin_decl_stmt
specifier|extern
name|ngx_tls_key_t
name|ngx_core_tls_key
decl_stmt|;
end_decl_stmt

begin_endif
endif|#
directive|endif
end_endif

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* _NGX_CYCLE_H_INCLUDED_ */
end_comment

end_unit

