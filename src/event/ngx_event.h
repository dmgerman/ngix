begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_ifndef
ifndef|#
directive|ifndef
name|_NGX_EVENT_H_INCLUDED_
end_ifndef

begin_define
DECL|macro|_NGX_EVENT_H_INCLUDED_
define|#
directive|define
name|_NGX_EVENT_H_INCLUDED_
end_define

begin_include
include|#
directive|include
file|<ngx_config.h>
end_include

begin_include
include|#
directive|include
file|<ngx_types.h>
end_include

begin_include
include|#
directive|include
file|<ngx_socket.h>
end_include

begin_include
include|#
directive|include
file|<ngx_log.h>
end_include

begin_include
include|#
directive|include
file|<ngx_alloc.h>
end_include

begin_include
include|#
directive|include
file|<ngx_array.h>
end_include

begin_typedef
DECL|typedef|ngx_event_t
typedef|typedef
name|struct
name|ngx_event_s
name|ngx_event_t
typedef|;
end_typedef

begin_struct
DECL|struct|ngx_event_s
struct|struct
name|ngx_event_s
block|{
DECL|member|data
name|void
modifier|*
name|data
decl_stmt|;
DECL|member|event_handler
name|int
function_decl|(
modifier|*
name|event_handler
function_decl|)
parameter_list|(
name|ngx_event_t
modifier|*
name|ev
parameter_list|)
function_decl|;
DECL|member|close_handler
name|int
function_decl|(
modifier|*
name|close_handler
function_decl|)
parameter_list|(
name|ngx_event_t
modifier|*
name|ev
parameter_list|)
function_decl|;
DECL|member|context
name|void
modifier|*
name|context
decl_stmt|;
DECL|member|action
name|char
modifier|*
name|action
decl_stmt|;
DECL|member|prev
name|ngx_event_t
modifier|*
name|prev
decl_stmt|;
comment|/* queue in select(), poll(), mutex(),        */
DECL|member|next
name|ngx_event_t
modifier|*
name|next
decl_stmt|;
comment|/*   aio_read(), aio_write()                  */
DECL|member|timer_handler
name|int
function_decl|(
modifier|*
name|timer_handler
function_decl|)
parameter_list|(
name|ngx_event_t
modifier|*
name|ev
parameter_list|)
function_decl|;
DECL|member|timer_prev
name|ngx_event_t
modifier|*
name|timer_prev
decl_stmt|;
DECL|member|timer_next
name|ngx_event_t
modifier|*
name|timer_next
decl_stmt|;
DECL|member|timer_delta
name|u_int
name|timer_delta
decl_stmt|;
DECL|member|timer
name|u_int
name|timer
decl_stmt|;
DECL|member|log
name|ngx_log_t
modifier|*
name|log
decl_stmt|;
DECL|member|available
name|int
name|available
decl_stmt|;
comment|/* kqueue only:                              */
comment|/*   accept: number of sockets that wait     */
comment|/*           to be accepted                  */
comment|/*   read:   bytes to read                   */
comment|/*   write:  available space in buffer       */
comment|/* otherwise:                                */
comment|/*   accept: 1 if accept many, 0 otherwise   */
comment|/* flags - int are probably faster on write then bits ??? */
DECL|member|listening
name|unsigned
name|listening
range|:
literal|1
decl_stmt|;
DECL|member|write
name|unsigned
name|write
range|:
literal|1
decl_stmt|;
DECL|member|ready
name|unsigned
name|ready
range|:
literal|1
decl_stmt|;
DECL|member|timedout
name|unsigned
name|timedout
range|:
literal|1
decl_stmt|;
DECL|member|process
name|unsigned
name|process
range|:
literal|1
decl_stmt|;
DECL|member|read_discarded
name|unsigned
name|read_discarded
range|:
literal|1
decl_stmt|;
DECL|member|unexpected_eof
name|unsigned
name|unexpected_eof
range|:
literal|1
decl_stmt|;
if|#
directive|if
operator|(
name|HAVE_DEFERRED_ACCEPT
operator|)
DECL|member|accept_filter
name|unsigned
name|accept_filter
range|:
literal|1
decl_stmt|;
endif|#
directive|endif
if|#
directive|if
operator|(
name|HAVE_KQUEUE
operator|)
DECL|member|eof
name|unsigned
name|eof
range|:
literal|1
decl_stmt|;
DECL|member|error
name|int
name|error
decl_stmt|;
endif|#
directive|endif
block|}
struct|;
end_struct

begin_typedef
DECL|enum|__anon2c5e4f220103
typedef|typedef
enum|enum
block|{
DECL|enumerator|NGX_SELECT_EVENT
name|NGX_SELECT_EVENT
init|=
literal|0
block|,
if|#
directive|if
operator|(
name|HAVE_POLL
operator|)
DECL|enumerator|NGX_POLL_EVENT
name|NGX_POLL_EVENT
block|,
endif|#
directive|endif
if|#
directive|if
operator|(
name|HAVE_KQUEUE
operator|)
DECL|enumerator|NGX_KQUEUE_EVENT
name|NGX_KQUEUE_EVENT
block|,
endif|#
directive|endif
DECL|typedef|ngx_event_type_e
block|}
name|ngx_event_type_e
typedef|;
end_typedef

begin_typedef
DECL|struct|__anon2c5e4f220208
typedef|typedef
struct|struct
block|{
DECL|member|add
name|int
function_decl|(
modifier|*
name|add
function_decl|)
parameter_list|(
name|ngx_event_t
modifier|*
name|ev
parameter_list|,
name|int
name|event
parameter_list|,
name|u_int
name|flags
parameter_list|)
function_decl|;
DECL|member|del
name|int
function_decl|(
modifier|*
name|del
function_decl|)
parameter_list|(
name|ngx_event_t
modifier|*
name|ev
parameter_list|,
name|int
name|event
parameter_list|)
function_decl|;
DECL|member|process
name|int
function_decl|(
modifier|*
name|process
function_decl|)
parameter_list|(
name|ngx_log_t
modifier|*
name|log
parameter_list|)
function_decl|;
DECL|member|read
name|int
function_decl|(
modifier|*
name|read
function_decl|)
parameter_list|(
name|ngx_event_t
modifier|*
name|ev
parameter_list|,
name|char
modifier|*
name|buf
parameter_list|,
name|size_t
name|size
parameter_list|)
function_decl|;
comment|/*     int  (*write)(ngx_event_t *ev, char *buf, size_t size); */
DECL|typedef|ngx_event_actions_t
block|}
name|ngx_event_actions_t
typedef|;
end_typedef

begin_comment
comment|/* NGX_LEVEL_EVENT (default)  select, poll, kqueue                                 requires to read whole data NGX_ONESHOT_EVENT          kqueue NGX_CLEAR_EVENT            kqueue */
end_comment

begin_if
if|#
directive|if
operator|(
name|HAVE_KQUEUE
operator|)
end_if

begin_define
DECL|macro|NGX_READ_EVENT
define|#
directive|define
name|NGX_READ_EVENT
value|EVFILT_READ
end_define

begin_define
DECL|macro|NGX_WRITE_EVENT
define|#
directive|define
name|NGX_WRITE_EVENT
value|EVFILT_WRITE
end_define

begin_define
DECL|macro|NGX_TIMER_EVENT
define|#
directive|define
name|NGX_TIMER_EVENT
value|(-EVFILT_SYSCOUNT - 1)
end_define

begin_define
DECL|macro|NGX_ONESHOT_EVENT
define|#
directive|define
name|NGX_ONESHOT_EVENT
value|EV_ONESHOT
end_define

begin_define
DECL|macro|NGX_CLEAR_EVENT
define|#
directive|define
name|NGX_CLEAR_EVENT
value|EV_CLEAR
end_define

begin_else
else|#
directive|else
end_else

begin_define
DECL|macro|NGX_READ_EVENT
define|#
directive|define
name|NGX_READ_EVENT
value|0
end_define

begin_define
DECL|macro|NGX_WRITE_EVENT
define|#
directive|define
name|NGX_WRITE_EVENT
value|1
end_define

begin_define
DECL|macro|NGX_TIMER_EVENT
define|#
directive|define
name|NGX_TIMER_EVENT
value|2
end_define

begin_define
DECL|macro|NGX_ONESHOT_EVENT
define|#
directive|define
name|NGX_ONESHOT_EVENT
value|1
end_define

begin_define
DECL|macro|NGX_CLEAR_EVENT
define|#
directive|define
name|NGX_CLEAR_EVENT
value|2
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_if
if|#
directive|if
operator|(
name|USE_KQUEUE
operator|)
end_if

begin_define
DECL|macro|ngx_init_events
define|#
directive|define
name|ngx_init_events
value|ngx_kqueue_init
end_define

begin_define
DECL|macro|ngx_process_events
define|#
directive|define
name|ngx_process_events
value|ngx_kqueue_process_events
end_define

begin_define
DECL|macro|ngx_add_event
define|#
directive|define
name|ngx_add_event
value|ngx_kqueue_add_event
end_define

begin_define
DECL|macro|ngx_del_event
define|#
directive|define
name|ngx_del_event
value|ngx_kqueue_del_event
end_define

begin_define
DECL|macro|ngx_event_recv
define|#
directive|define
name|ngx_event_recv
value|ngx_event_recv_core
end_define

begin_else
else|#
directive|else
end_else

begin_define
DECL|macro|ngx_init_events
define|#
directive|define
name|ngx_init_events
value|(ngx_event_init[ngx_event_type])
end_define

begin_define
DECL|macro|ngx_process_events
define|#
directive|define
name|ngx_process_events
value|ngx_event_actions.process
end_define

begin_define
DECL|macro|ngx_add_event
define|#
directive|define
name|ngx_add_event
value|ngx_event_actions.add
end_define

begin_define
DECL|macro|ngx_del_event
define|#
directive|define
name|ngx_del_event
value|ngx_event_actions.del
end_define

begin_define
DECL|macro|ngx_event_recv
define|#
directive|define
name|ngx_event_recv
value|ngx_event_recv_core
end_define

begin_endif
endif|#
directive|endif
end_endif

begin_decl_stmt
specifier|extern
name|ngx_event_t
modifier|*
name|ngx_read_events
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|ngx_event_t
modifier|*
name|ngx_write_events
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|ngx_connection_t
modifier|*
name|ngx_connections
decl_stmt|;
end_decl_stmt

begin_if
if|#
directive|if
operator|!
operator|(
name|USE_KQUEUE
operator|)
end_if

begin_decl_stmt
specifier|extern
name|ngx_event_actions_t
name|ngx_event_actions
decl_stmt|;
end_decl_stmt

begin_decl_stmt
specifier|extern
name|ngx_event_type_e
name|ngx_event_type
decl_stmt|;
end_decl_stmt

begin_endif
endif|#
directive|endif
end_endif

begin_function_decl
name|void
name|ngx_pre_thread
parameter_list|(
name|ngx_array_t
modifier|*
name|ls
parameter_list|,
name|ngx_pool_t
modifier|*
name|pool
parameter_list|,
name|ngx_log_t
modifier|*
name|log
parameter_list|)
function_decl|;
end_function_decl

begin_function_decl
name|void
name|ngx_worker
parameter_list|(
name|ngx_log_t
modifier|*
name|log
parameter_list|)
function_decl|;
end_function_decl

begin_endif
endif|#
directive|endif
end_endif

begin_comment
comment|/* _NGX_EVENT_H_INCLUDED_ */
end_comment

end_unit

