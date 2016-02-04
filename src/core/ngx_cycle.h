begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (C) Igor Sysoev  * Copyright (C) Nginx, Inc.  */
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

begin_ifndef
ifndef|#
directive|ifndef
name|NGX_CYCLE_POOL_SIZE
end_ifndef

begin_define
DECL|macro|NGX_CYCLE_POOL_SIZE
define|#
directive|define
name|NGX_CYCLE_POOL_SIZE
value|NGX_DEFAULT_POOL_SIZE
end_define

begin_endif
endif|#
directive|endif
end_endif

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

begin_typedef
DECL|typedef|ngx_shm_zone_t
typedef|typedef
name|struct
name|ngx_shm_zone_s
name|ngx_shm_zone_t
typedef|;
end_typedef

begin_typedef
DECL|typedef|ngx_shm_zone_init_pt
typedef|typedef
name|ngx_int_t
function_decl|(
modifier|*
name|ngx_shm_zone_init_pt
function_decl|)
parameter_list|(
name|ngx_shm_zone_t
modifier|*
name|zone
parameter_list|,
name|void
modifier|*
name|data
parameter_list|)
function_decl|;
end_typedef

begin_struct
DECL|struct|ngx_shm_zone_s
struct|struct
name|ngx_shm_zone_s
block|{
DECL|member|data
name|void
modifier|*
name|data
decl_stmt|;
DECL|member|shm
name|ngx_shm_t
name|shm
decl_stmt|;
DECL|member|init
name|ngx_shm_zone_init_pt
name|init
decl_stmt|;
DECL|member|tag
name|void
modifier|*
name|tag
decl_stmt|;
DECL|member|noreuse
name|ngx_uint_t
name|noreuse
decl_stmt|;
comment|/* unsigned  noreuse:1; */
block|}
struct|;
end_struct

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
name|new_log
decl_stmt|;
DECL|member|log_use_stderr
name|ngx_uint_t
name|log_use_stderr
decl_stmt|;
comment|/* unsigned  log_use_stderr:1; */
DECL|member|files
name|ngx_connection_t
modifier|*
modifier|*
name|files
decl_stmt|;
DECL|member|free_connections
name|ngx_connection_t
modifier|*
name|free_connections
decl_stmt|;
DECL|member|free_connection_n
name|ngx_uint_t
name|free_connection_n
decl_stmt|;
DECL|member|modules
name|ngx_module_t
modifier|*
modifier|*
name|modules
decl_stmt|;
DECL|member|modules_n
name|ngx_uint_t
name|modules_n
decl_stmt|;
DECL|member|modules_used
name|ngx_uint_t
name|modules_used
decl_stmt|;
comment|/* unsigned  modules_used:1; */
DECL|member|reusable_connections_queue
name|ngx_queue_t
name|reusable_connections_queue
decl_stmt|;
DECL|member|listening
name|ngx_array_t
name|listening
decl_stmt|;
DECL|member|paths
name|ngx_array_t
name|paths
decl_stmt|;
DECL|member|config_dump
name|ngx_array_t
name|config_dump
decl_stmt|;
DECL|member|open_files
name|ngx_list_t
name|open_files
decl_stmt|;
DECL|member|shared_memory
name|ngx_list_t
name|shared_memory
decl_stmt|;
DECL|member|connection_n
name|ngx_uint_t
name|connection_n
decl_stmt|;
DECL|member|files_n
name|ngx_uint_t
name|files_n
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
DECL|member|conf_param
name|ngx_str_t
name|conf_param
decl_stmt|;
DECL|member|conf_prefix
name|ngx_str_t
name|conf_prefix
decl_stmt|;
DECL|member|prefix
name|ngx_str_t
name|prefix
decl_stmt|;
DECL|member|lock_file
name|ngx_str_t
name|lock_file
decl_stmt|;
DECL|member|hostname
name|ngx_str_t
name|hostname
decl_stmt|;
block|}
struct|;
end_struct

begin_typedef
DECL|struct|__anon277a87030108
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
DECL|member|timer_resolution
name|ngx_msec_t
name|timer_resolution
decl_stmt|;
DECL|member|worker_processes
name|ngx_int_t
name|worker_processes
decl_stmt|;
DECL|member|debug_points
name|ngx_int_t
name|debug_points
decl_stmt|;
DECL|member|rlimit_nofile
name|ngx_int_t
name|rlimit_nofile
decl_stmt|;
DECL|member|rlimit_core
name|off_t
name|rlimit_core
decl_stmt|;
DECL|member|priority
name|int
name|priority
decl_stmt|;
DECL|member|cpu_affinity_auto
name|ngx_uint_t
name|cpu_affinity_auto
decl_stmt|;
DECL|member|cpu_affinity_n
name|ngx_uint_t
name|cpu_affinity_n
decl_stmt|;
DECL|member|cpu_affinity
name|uint64_t
modifier|*
name|cpu_affinity
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
DECL|member|working_directory
name|ngx_str_t
name|working_directory
decl_stmt|;
DECL|member|lock_file
name|ngx_str_t
name|lock_file
decl_stmt|;
DECL|member|pid
name|ngx_str_t
name|pid
decl_stmt|;
DECL|member|oldpid
name|ngx_str_t
name|oldpid
decl_stmt|;
DECL|member|env
name|ngx_array_t
name|env
decl_stmt|;
DECL|member|environment
name|char
modifier|*
modifier|*
name|environment
decl_stmt|;
DECL|typedef|ngx_core_conf_t
block|}
name|ngx_core_conf_t
typedef|;
end_typedef

begin_define
DECL|macro|ngx_is_init_cycle (cycle)
define|#
directive|define
name|ngx_is_init_cycle
parameter_list|(
name|cycle
parameter_list|)
value|(cycle->conf_ctx == NULL)
end_define

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
name|ngx_str_t
modifier|*
name|name
parameter_list|,
name|ngx_log_t
modifier|*
name|log
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
name|ngx_int_t
name|ngx_signal_process
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|,
name|char
modifier|*
name|sig
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
name|char
modifier|*
modifier|*
name|ngx_set_environment
parameter_list|(
name|ngx_cycle_t
modifier|*
name|cycle
parameter_list|,
name|ngx_uint_t
modifier|*
name|last
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

begin_function_decl
name|uint64_t
name|ngx_get_cpu_affinity
parameter_list|(
name|ngx_uint_t
name|n
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_shm_zone_t
modifier|*
name|ngx_shared_memory_add
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|ngx_str_t
modifier|*
name|name
parameter_list|,
name|size_t
name|size
parameter_list|,
name|void
modifier|*
name|tag
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

begin_decl_stmt
specifier|extern
name|ngx_uint_t
name|ngx_dump_config
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|ngx_uint_t
name|ngx_quiet_mode
decl_stmt|;
end_decl_stmt

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* _NGX_CYCLE_H_INCLUDED_ */
end_comment

end_unit

