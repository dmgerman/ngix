begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|_NGX_STRING_H_INCLUDED_
end_ifndef

begin_define
DECL|macro|_NGX_STRING_H_INCLUDED_
define|#
directive|define
name|_NGX_STRING_H_INCLUDED_
end_define

begin_include
include|#
directive|include
file|<ngx_config.h>
end_include

begin_typedef
DECL|struct|__anon2bf58cf60108
typedef|typedef
struct|struct
block|{
DECL|member|len
name|size_t
name|len
decl_stmt|;
DECL|member|data
name|char
modifier|*
name|data
decl_stmt|;
DECL|typedef|ngx_str_t
block|}
name|ngx_str_t
typedef|;
end_typedef

begin_define
DECL|macro|ngx_string (str)
define|#
directive|define
name|ngx_string
parameter_list|(
name|str
parameter_list|)
value|{ sizeof(str) - 1, str }
end_define

begin_if
if|#
directive|if
operator|(
name|WIN32
operator|)
end_if

begin_define
DECL|macro|ngx_memzero
define|#
directive|define
name|ngx_memzero
value|ZeroMemory
end_define

begin_define
DECL|macro|ngx_strncasecmp
define|#
directive|define
name|ngx_strncasecmp
value|strnicmp
end_define

begin_define
DECL|macro|ngx_strcasecmp
define|#
directive|define
name|ngx_strcasecmp
value|stricmp
end_define

begin_define
DECL|macro|ngx_strncmp
define|#
directive|define
name|ngx_strncmp
value|strncmp
end_define

begin_define
DECL|macro|ngx_strcmp
define|#
directive|define
name|ngx_strcmp
value|strcmp
end_define

begin_define
DECL|macro|ngx_strlen
define|#
directive|define
name|ngx_strlen
value|strlen
end_define

begin_define
DECL|macro|ngx_snprintf
define|#
directive|define
name|ngx_snprintf
value|_snprintf
end_define

begin_define
DECL|macro|ngx_vsnprintf
define|#
directive|define
name|ngx_vsnprintf
value|_vsnprintf
end_define

begin_else
else|#
directive|else
end_else

begin_define
DECL|macro|ngx_memzero
define|#
directive|define
name|ngx_memzero
value|bzero
end_define

begin_define
DECL|macro|ngx_strncasecmp
define|#
directive|define
name|ngx_strncasecmp
value|strncasecmp
end_define

begin_define
DECL|macro|ngx_strcasecmp
define|#
directive|define
name|ngx_strcasecmp
value|strcasecmp
end_define

begin_define
DECL|macro|ngx_strncmp
define|#
directive|define
name|ngx_strncmp
value|strncmp
end_define

begin_define
DECL|macro|ngx_strcmp
define|#
directive|define
name|ngx_strcmp
value|strcmp
end_define

begin_define
DECL|macro|ngx_strlen
define|#
directive|define
name|ngx_strlen
value|strlen
end_define

begin_define
DECL|macro|ngx_snprintf
define|#
directive|define
name|ngx_snprintf
value|snprintf
end_define

begin_define
DECL|macro|ngx_vsnprintf
define|#
directive|define
name|ngx_vsnprintf
value|vsnprintf
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_define
DECL|macro|ngx_memcpy (dst,src,n)
define|#
directive|define
name|ngx_memcpy
parameter_list|(
name|dst
parameter_list|,
name|src
parameter_list|,
name|n
parameter_list|)
value|memcpy(dst, src, n)
end_define

begin_define
DECL|macro|ngx_cpymem (dst,src,n)
define|#
directive|define
name|ngx_cpymem
parameter_list|(
name|dst
parameter_list|,
name|src
parameter_list|,
name|n
parameter_list|)
value|memcpy(dst, src, n) + n
end_define

begin_function_decl
name|char
modifier|*
name|ngx_cpystrn
parameter_list|(
name|char
modifier|*
name|dst
parameter_list|,
name|char
modifier|*
name|src
parameter_list|,
name|size_t
name|n
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* _NGX_STRING_H_INCLUDED_ */
end_comment

end_unit

