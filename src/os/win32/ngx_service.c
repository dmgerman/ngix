begin_unit|revision:1.0.0;language:C;cregit-version:0.0.1
begin_comment
comment|/*  * Copyright (C) Igor Sysoev  */
end_comment

begin_define
DECL|macro|NGX_SERVICE_CONTROL_SHUTDOWN
define|#
directive|define
name|NGX_SERVICE_CONTROL_SHUTDOWN
value|128
end_define

begin_define
DECL|macro|NGX_SERVICE_CONTROL_REOPEN
define|#
directive|define
name|NGX_SERVICE_CONTROL_REOPEN
value|129
end_define

begin_decl_stmt
DECL|variable|st
name|SERVICE_TABLE_ENTRY
name|st
index|[]
init|=
block|{
block|{
literal|"nginx"
block|,
name|service_main
block|}
block|,
block|{
name|NULL
block|,
name|NULL
block|}
block|}
decl_stmt|;
end_decl_stmt

begin_function
DECL|function|ngx_service (ngx_log_t * log)
name|ngx_int_t
name|ngx_service
parameter_list|(
name|ngx_log_t
modifier|*
name|log
parameter_list|)
block|{
comment|/* primary thread */
comment|/* StartServiceCtrlDispatcher() shouxpdl be called within 30 seconds */
if|if
condition|(
name|StartServiceCtrlDispatcher
argument_list|(
name|st
argument_list|)
operator|==
literal|0
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_EMERG
argument_list|,
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"StartServiceCtrlDispatcher() failed"
argument_list|)
expr_stmt|;
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
DECL|function|service_main (u_int argc,char ** argv)
name|void
name|service_main
parameter_list|(
name|u_int
name|argc
parameter_list|,
name|char
modifier|*
modifier|*
name|argv
parameter_list|)
block|{
name|SERVICE_STATUS
name|status
decl_stmt|;
name|SERVICE_STATUS_HANDLE
name|service
decl_stmt|;
comment|/* thread spawned by SCM */
name|service
operator|=
name|RegisterServiceCtrlHandlerEx
argument_list|(
literal|"nginx"
argument_list|,
name|service_handler
argument_list|,
name|ctx
argument_list|)
expr_stmt|;
if|if
condition|(
name|service
operator|==
name|INVALID_HANDLE_VALUE
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_EMERG
argument_list|,
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"RegisterServiceCtrlHandlerEx() failed"
argument_list|)
expr_stmt|;
return|return;
block|}
name|status
operator|.
name|dwServiceType
operator|=
name|SERVICE_WIN32_OWN_PROCESS
expr_stmt|;
name|status
operator|.
name|dwCurrentState
operator|=
name|SERVICE_START_PENDING
expr_stmt|;
name|status
operator|.
name|dwControlsAccepted
operator|=
name|SERVICE_ACCEPT_STOP
operator||
name|SERVICE_ACCEPT_PARAMCHANGE
expr_stmt|;
name|status
operator|.
name|dwWin32ExitCode
operator|=
name|NO_ERROR
expr_stmt|;
name|status
operator|.
name|dwServiceSpecificExitCode
operator|=
literal|0
expr_stmt|;
name|status
operator|.
name|dwCheckPoint
operator|=
literal|1
expr_stmt|;
name|status
operator|.
name|dwWaitHint
operator|=
literal|2000
expr_stmt|;
comment|/* SetServiceStatus() should be called within 80 seconds */
if|if
condition|(
name|SetServiceStatus
argument_list|(
name|service
argument_list|,
operator|&
name|status
argument_list|)
operator|==
literal|0
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_EMERG
argument_list|,
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"SetServiceStatus() failed"
argument_list|)
expr_stmt|;
return|return;
block|}
comment|/* init */
name|status
operator|.
name|dwCurrentState
operator|=
name|SERVICE_RUNNING
expr_stmt|;
name|status
operator|.
name|dwCheckPoint
operator|=
literal|0
expr_stmt|;
name|status
operator|.
name|dwWaitHint
operator|=
literal|0
expr_stmt|;
if|if
condition|(
name|SetServiceStatus
argument_list|(
name|service
argument_list|,
operator|&
name|status
argument_list|)
operator|==
literal|0
condition|)
block|{
name|ngx_log_error
argument_list|(
name|NGX_LOG_EMERG
argument_list|,
name|log
argument_list|,
name|ngx_errno
argument_list|,
literal|"SetServiceStatus() failed"
argument_list|)
expr_stmt|;
return|return;
block|}
comment|/* call master or worker loop */
comment|/*      * master should use event notification and look status      * single should use iocp to get notifications from service handler      */
block|}
end_function

begin_function
DECL|function|service_handler (u_int control,u_int type,void * data,void * ctx)
name|u_int
name|service_handler
parameter_list|(
name|u_int
name|control
parameter_list|,
name|u_int
name|type
parameter_list|,
name|void
modifier|*
name|data
parameter_list|,
name|void
modifier|*
name|ctx
parameter_list|)
block|{
comment|/* primary thread */
switch|switch
condition|(
name|control
condition|)
block|{
case|case
name|SERVICE_CONTROL_INTERROGATE
case|:
name|status
operator|=
name|NGX_IOCP_INTERROGATE
expr_stmt|;
break|break;
case|case
name|SERVICE_CONTROL_STOP
case|:
name|status
operator|=
name|NGX_IOCP_STOP
expr_stmt|;
break|break;
case|case
name|SERVICE_CONTROL_PARAMCHANGE
case|:
name|status
operator|=
name|NGX_IOCP_RECONFIGURE
expr_stmt|;
break|break;
case|case
name|NGX_SERVICE_CONTROL_SHUTDOWN
case|:
name|status
operator|=
name|NGX_IOCP_REOPEN
expr_stmt|;
break|break;
case|case
name|NGX_SERVICE_CONTROL_REOPEN
case|:
name|status
operator|=
name|NGX_IOCP_REOPEN
expr_stmt|;
break|break;
default|default:
return|return
name|ERROR_CALL_NOT_IMPLEMENTED
return|;
block|}
if|if
condition|(
name|ngx_single
condition|)
block|{
if|if
condition|(
name|PostQueuedCompletionStatus
argument_list|(
name|iocp
argument_list|,
operator|...
name|status
argument_list|,
operator|...
argument_list|)
operator|==
literal|0
condition|)
block|{
name|err
operator|=
name|ngx_errno
expr_stmt|;
name|ngx_log_error
argument_list|(
name|NGX_LOG_ALERT
argument_list|,
name|log
argument_list|,
name|err
argument_list|,
literal|"PostQueuedCompletionStatus() failed"
argument_list|)
expr_stmt|;
return|return
name|err
return|;
block|}
block|}
else|else
block|{
name|Event
block|}
return|return
name|NO_ERROR
return|;
block|}
end_function

end_unit

