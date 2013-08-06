begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (C) Igor Sysoev  * Copyright (C) Nginx, Inc.  */
end_comment

begin_ifndef
ifndef|#
directive|ifndef
name|_NGX_FILE_H_INCLUDED_
end_ifndef

begin_define
DECL|macro|_NGX_FILE_H_INCLUDED_
define|#
directive|define
name|_NGX_FILE_H_INCLUDED_
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

begin_struct
DECL|struct|ngx_file_s
struct|struct
name|ngx_file_s
block|{
DECL|member|fd
name|ngx_fd_t
name|fd
decl_stmt|;
DECL|member|name
name|ngx_str_t
name|name
decl_stmt|;
DECL|member|info
name|ngx_file_info_t
name|info
decl_stmt|;
DECL|member|offset
name|off_t
name|offset
decl_stmt|;
DECL|member|sys_offset
name|off_t
name|sys_offset
decl_stmt|;
DECL|member|log
name|ngx_log_t
modifier|*
name|log
decl_stmt|;
if|#
directive|if
operator|(
name|NGX_HAVE_FILE_AIO
operator|)
DECL|member|aio
name|ngx_event_aio_t
modifier|*
name|aio
decl_stmt|;
endif|#
directive|endif
DECL|member|valid_info
name|unsigned
name|valid_info
range|:
literal|1
decl_stmt|;
DECL|member|directio
name|unsigned
name|directio
range|:
literal|1
decl_stmt|;
block|}
struct|;
end_struct

begin_define
DECL|macro|NGX_MAX_PATH_LEVEL
define|#
directive|define
name|NGX_MAX_PATH_LEVEL
value|3
end_define

begin_typedef
DECL|typedef|ngx_path_manager_pt
typedef|typedef
name|time_t
function_decl|(
modifier|*
name|ngx_path_manager_pt
function_decl|)
parameter_list|(
name|void
modifier|*
name|data
parameter_list|)
function_decl|;
end_typedef

begin_typedef
DECL|typedef|ngx_path_loader_pt
typedef|typedef
name|void
function_decl|(
modifier|*
name|ngx_path_loader_pt
function_decl|)
parameter_list|(
name|void
modifier|*
name|data
parameter_list|)
function_decl|;
end_typedef

begin_typedef
DECL|struct|__anon27d578230108
typedef|typedef
struct|struct
block|{
DECL|member|name
name|ngx_str_t
name|name
decl_stmt|;
DECL|member|len
name|size_t
name|len
decl_stmt|;
DECL|member|level
name|size_t
name|level
index|[
literal|3
index|]
decl_stmt|;
DECL|member|manager
name|ngx_path_manager_pt
name|manager
decl_stmt|;
DECL|member|loader
name|ngx_path_loader_pt
name|loader
decl_stmt|;
DECL|member|data
name|void
modifier|*
name|data
decl_stmt|;
DECL|member|conf_file
name|u_char
modifier|*
name|conf_file
decl_stmt|;
DECL|member|line
name|ngx_uint_t
name|line
decl_stmt|;
DECL|typedef|ngx_path_t
block|}
name|ngx_path_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon27d578230208
typedef|typedef
struct|struct
block|{
DECL|member|name
name|ngx_str_t
name|name
decl_stmt|;
DECL|member|level
name|size_t
name|level
index|[
literal|3
index|]
decl_stmt|;
DECL|typedef|ngx_path_init_t
block|}
name|ngx_path_init_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon27d578230308
typedef|typedef
struct|struct
block|{
DECL|member|file
name|ngx_file_t
name|file
decl_stmt|;
DECL|member|offset
name|off_t
name|offset
decl_stmt|;
DECL|member|path
name|ngx_path_t
modifier|*
name|path
decl_stmt|;
DECL|member|pool
name|ngx_pool_t
modifier|*
name|pool
decl_stmt|;
DECL|member|warn
name|char
modifier|*
name|warn
decl_stmt|;
DECL|member|access
name|ngx_uint_t
name|access
decl_stmt|;
DECL|member|log_level
name|unsigned
name|log_level
range|:
literal|8
decl_stmt|;
DECL|member|persistent
name|unsigned
name|persistent
range|:
literal|1
decl_stmt|;
DECL|member|clean
name|unsigned
name|clean
range|:
literal|1
decl_stmt|;
DECL|typedef|ngx_temp_file_t
block|}
name|ngx_temp_file_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon27d578230408
typedef|typedef
struct|struct
block|{
DECL|member|access
name|ngx_uint_t
name|access
decl_stmt|;
DECL|member|path_access
name|ngx_uint_t
name|path_access
decl_stmt|;
DECL|member|time
name|time_t
name|time
decl_stmt|;
DECL|member|fd
name|ngx_fd_t
name|fd
decl_stmt|;
DECL|member|create_path
name|unsigned
name|create_path
range|:
literal|1
decl_stmt|;
DECL|member|delete_file
name|unsigned
name|delete_file
range|:
literal|1
decl_stmt|;
DECL|member|log
name|ngx_log_t
modifier|*
name|log
decl_stmt|;
DECL|typedef|ngx_ext_rename_file_t
block|}
name|ngx_ext_rename_file_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon27d578230508
typedef|typedef
struct|struct
block|{
DECL|member|size
name|off_t
name|size
decl_stmt|;
DECL|member|buf_size
name|size_t
name|buf_size
decl_stmt|;
DECL|member|access
name|ngx_uint_t
name|access
decl_stmt|;
DECL|member|time
name|time_t
name|time
decl_stmt|;
DECL|member|log
name|ngx_log_t
modifier|*
name|log
decl_stmt|;
DECL|typedef|ngx_copy_file_t
block|}
name|ngx_copy_file_t
typedef|;
end_typedef

begin_typedef
DECL|typedef|ngx_tree_ctx_t
typedef|typedef
name|struct
name|ngx_tree_ctx_s
name|ngx_tree_ctx_t
typedef|;
end_typedef

begin_typedef
DECL|typedef|ngx_tree_init_handler_pt
typedef|typedef
name|ngx_int_t
function_decl|(
modifier|*
name|ngx_tree_init_handler_pt
function_decl|)
parameter_list|(
name|void
modifier|*
name|ctx
parameter_list|,
name|void
modifier|*
name|prev
parameter_list|)
function_decl|;
end_typedef

begin_typedef
DECL|typedef|ngx_tree_handler_pt
typedef|typedef
name|ngx_int_t
function_decl|(
modifier|*
name|ngx_tree_handler_pt
function_decl|)
parameter_list|(
name|ngx_tree_ctx_t
modifier|*
name|ctx
parameter_list|,
name|ngx_str_t
modifier|*
name|name
parameter_list|)
function_decl|;
end_typedef

begin_struct
DECL|struct|ngx_tree_ctx_s
struct|struct
name|ngx_tree_ctx_s
block|{
DECL|member|size
name|off_t
name|size
decl_stmt|;
DECL|member|fs_size
name|off_t
name|fs_size
decl_stmt|;
DECL|member|access
name|ngx_uint_t
name|access
decl_stmt|;
DECL|member|mtime
name|time_t
name|mtime
decl_stmt|;
DECL|member|init_handler
name|ngx_tree_init_handler_pt
name|init_handler
decl_stmt|;
DECL|member|file_handler
name|ngx_tree_handler_pt
name|file_handler
decl_stmt|;
DECL|member|pre_tree_handler
name|ngx_tree_handler_pt
name|pre_tree_handler
decl_stmt|;
DECL|member|post_tree_handler
name|ngx_tree_handler_pt
name|post_tree_handler
decl_stmt|;
DECL|member|spec_handler
name|ngx_tree_handler_pt
name|spec_handler
decl_stmt|;
DECL|member|data
name|void
modifier|*
name|data
decl_stmt|;
DECL|member|alloc
name|size_t
name|alloc
decl_stmt|;
DECL|member|log
name|ngx_log_t
modifier|*
name|log
decl_stmt|;
block|}
struct|;
end_struct

begin_function_decl
name|ngx_int_t
name|ngx_get_full_name
parameter_list|(
name|ngx_pool_t
modifier|*
name|pool
parameter_list|,
name|ngx_str_t
modifier|*
name|prefix
parameter_list|,
name|ngx_str_t
modifier|*
name|name
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ssize_t
name|ngx_write_chain_to_temp_file
parameter_list|(
name|ngx_temp_file_t
modifier|*
name|tf
parameter_list|,
name|ngx_chain_t
modifier|*
name|chain
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_int_t
name|ngx_create_temp_file
parameter_list|(
name|ngx_file_t
modifier|*
name|file
parameter_list|,
name|ngx_path_t
modifier|*
name|path
parameter_list|,
name|ngx_pool_t
modifier|*
name|pool
parameter_list|,
name|ngx_uint_t
name|persistent
parameter_list|,
name|ngx_uint_t
name|clean
parameter_list|,
name|ngx_uint_t
name|access
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|ngx_create_hashed_filename
parameter_list|(
name|ngx_path_t
modifier|*
name|path
parameter_list|,
name|u_char
modifier|*
name|file
parameter_list|,
name|size_t
name|len
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_int_t
name|ngx_create_path
parameter_list|(
name|ngx_file_t
modifier|*
name|file
parameter_list|,
name|ngx_path_t
modifier|*
name|path
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_err_t
name|ngx_create_full_path
parameter_list|(
name|u_char
modifier|*
name|dir
parameter_list|,
name|ngx_uint_t
name|access
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_int_t
name|ngx_add_path
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|ngx_path_t
modifier|*
modifier|*
name|slot
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_int_t
name|ngx_create_paths
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
name|ngx_int_t
name|ngx_ext_rename_file
parameter_list|(
name|ngx_str_t
modifier|*
name|src
parameter_list|,
name|ngx_str_t
modifier|*
name|to
parameter_list|,
name|ngx_ext_rename_file_t
modifier|*
name|ext
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_int_t
name|ngx_copy_file
parameter_list|(
name|u_char
modifier|*
name|from
parameter_list|,
name|u_char
modifier|*
name|to
parameter_list|,
name|ngx_copy_file_t
modifier|*
name|cf
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_int_t
name|ngx_walk_tree
parameter_list|(
name|ngx_tree_ctx_t
modifier|*
name|ctx
parameter_list|,
name|ngx_str_t
modifier|*
name|tree
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|ngx_atomic_uint_t
name|ngx_next_temp_number
parameter_list|(
name|ngx_uint_t
name|collision
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|char
modifier|*
name|ngx_conf_set_path_slot
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|ngx_command_t
modifier|*
name|cmd
parameter_list|,
name|void
modifier|*
name|conf
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|char
modifier|*
name|ngx_conf_merge_path_value
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|ngx_path_t
modifier|*
modifier|*
name|path
parameter_list|,
name|ngx_path_t
modifier|*
name|prev
parameter_list|,
name|ngx_path_init_t
modifier|*
name|init
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|char
modifier|*
name|ngx_conf_set_access_slot
parameter_list|(
name|ngx_conf_t
modifier|*
name|cf
parameter_list|,
name|ngx_command_t
modifier|*
name|cmd
parameter_list|,
name|void
modifier|*
name|conf
parameter_list|)
function_decl|;
end_function_decl

begin_decl_stmt
specifier|extern
name|ngx_atomic_t
modifier|*
name|ngx_temp_number
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|ngx_atomic_int_t
name|ngx_random_number
decl_stmt|;
end_decl_stmt

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* _NGX_FILE_H_INCLUDED_ */
end_comment

end_unit

