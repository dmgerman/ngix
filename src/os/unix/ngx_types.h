begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (C) Igor Sysoev  */
end_comment

begin_ifndef
ifndef|#
directive|ifndef
name|_NGX_TYPES_H_INCLUDED_
end_ifndef

begin_define
DECL|macro|_NGX_TYPES_H_INCLUDED_
define|#
directive|define
name|_NGX_TYPES_H_INCLUDED_
end_define

begin_include
include|#
directive|include
file|<ngx_config.h>
end_include

begin_typedef
DECL|typedef|ngx_fd_t
typedef|typedef
name|int
name|ngx_fd_t
typedef|;
end_typedef

begin_typedef
DECL|typedef|ngx_file_info_t
typedef|typedef
name|struct
name|stat
name|ngx_file_info_t
typedef|;
end_typedef

begin_typedef
DECL|typedef|ngx_file_uniq_t
typedef|typedef
name|ino_t
name|ngx_file_uniq_t
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon274e547a0108
typedef|typedef
struct|struct
block|{
DECL|member|dir
name|DIR
modifier|*
name|dir
decl_stmt|;
DECL|member|de
name|struct
name|dirent
modifier|*
name|de
decl_stmt|;
DECL|member|info
name|struct
name|stat
name|info
decl_stmt|;
DECL|member|info_valid
name|unsigned
name|info_valid
decl_stmt|;
DECL|typedef|ngx_dir_t
block|}
name|ngx_dir_t
typedef|;
end_typedef

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* _NGX_TYPES_H_INCLUDED_ */
end_comment

end_unit

