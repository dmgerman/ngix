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

begin_include
include|#
directive|include
file|<ngx_event.h>
end_include

begin_include
include|#
directive|include
file|<ngx_imap.h>
end_include

begin_function
DECL|function|ngx_pop3_parse_command (ngx_imap_session_t * s)
name|ngx_int_t
name|ngx_pop3_parse_command
parameter_list|(
name|ngx_imap_session_t
modifier|*
name|s
parameter_list|)
block|{
name|u_char
name|ch
decl_stmt|,
modifier|*
name|p
decl_stmt|,
modifier|*
name|c
decl_stmt|;
name|ngx_str_t
modifier|*
name|arg
decl_stmt|;
DECL|enum|__anon294162d30103
enum|enum
block|{
DECL|enumerator|sw_start
name|sw_start
init|=
literal|0
block|,
DECL|enumerator|sw_spaces_before_argument
name|sw_spaces_before_argument
block|,
DECL|enumerator|sw_argument
name|sw_argument
block|,
DECL|enumerator|sw_almost_done
name|sw_almost_done
block|,
DECL|enumerator|sw_done
name|sw_done
block|}
name|state
enum|;
name|state
operator|=
name|s
operator|->
name|state
expr_stmt|;
name|p
operator|=
name|s
operator|->
name|buffer
operator|->
name|pos
expr_stmt|;
while|while
condition|(
name|p
operator|<
name|s
operator|->
name|buffer
operator|->
name|last
operator|&&
name|state
operator|<
name|sw_done
condition|)
block|{
name|ch
operator|=
operator|*
name|p
operator|++
expr_stmt|;
switch|switch
condition|(
name|state
condition|)
block|{
comment|/* POP3 command */
case|case
name|sw_start
case|:
if|if
condition|(
name|ch
operator|==
literal|' '
operator|||
name|ch
operator|==
name|CR
operator|||
name|ch
operator|==
name|LF
condition|)
block|{
name|c
operator|=
name|s
operator|->
name|buffer
operator|->
name|start
expr_stmt|;
if|if
condition|(
name|p
operator|-
literal|1
operator|-
name|c
operator|==
literal|4
condition|)
block|{
if|if
condition|(
name|c
index|[
literal|0
index|]
operator|==
literal|'U'
operator|&&
name|c
index|[
literal|1
index|]
operator|==
literal|'S'
operator|&&
name|c
index|[
literal|2
index|]
operator|==
literal|'E'
operator|&&
name|c
index|[
literal|3
index|]
operator|==
literal|'R'
condition|)
block|{
name|s
operator|->
name|command
operator|=
name|NGX_POP3_USER
expr_stmt|;
block|}
if|else if
condition|(
name|c
index|[
literal|0
index|]
operator|==
literal|'P'
operator|&&
name|c
index|[
literal|1
index|]
operator|==
literal|'A'
operator|&&
name|c
index|[
literal|2
index|]
operator|==
literal|'S'
operator|&&
name|c
index|[
literal|3
index|]
operator|==
literal|'S'
condition|)
block|{
name|s
operator|->
name|command
operator|=
name|NGX_POP3_PASS
expr_stmt|;
block|}
if|else if
condition|(
name|c
index|[
literal|0
index|]
operator|==
literal|'Q'
operator|&&
name|c
index|[
literal|1
index|]
operator|==
literal|'U'
operator|&&
name|c
index|[
literal|2
index|]
operator|==
literal|'I'
operator|&&
name|c
index|[
literal|3
index|]
operator|==
literal|'T'
condition|)
block|{
name|s
operator|->
name|command
operator|=
name|NGX_POP3_QUIT
expr_stmt|;
if|#
directive|if
literal|0
block_content|} else if (c[0] == 'N'&& c[1] == 'O'&& c[2] == 'O'&& c[3] == 'P')                     {                         s->command = NGX_POP3_NOOP;
endif|#
directive|endif
block|}
else|else
block|{
name|s
operator|->
name|state
operator|=
name|sw_start
expr_stmt|;
return|return
name|NGX_IMAP_PARSE_INVALID_COMMAND
return|;
block|}
block|}
else|else
block|{
name|s
operator|->
name|state
operator|=
name|sw_start
expr_stmt|;
return|return
name|NGX_IMAP_PARSE_INVALID_COMMAND
return|;
block|}
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
literal|' '
case|:
name|state
operator|=
name|sw_spaces_before_argument
expr_stmt|;
break|break;
case|case
name|CR
case|:
name|state
operator|=
name|sw_almost_done
expr_stmt|;
break|break;
case|case
name|LF
case|:
name|state
operator|=
name|sw_done
expr_stmt|;
break|break;
block|}
break|break;
block|}
if|if
condition|(
name|ch
argument_list|<
literal|'A'
operator|||
name|ch
argument_list|>
literal|'Z'
condition|)
block|{
name|s
operator|->
name|state
operator|=
name|sw_start
expr_stmt|;
return|return
name|NGX_IMAP_PARSE_INVALID_COMMAND
return|;
block|}
break|break;
comment|/* the spaces before the argument */
case|case
name|sw_spaces_before_argument
case|:
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
literal|' '
case|:
break|break;
case|case
name|CR
case|:
name|state
operator|=
name|sw_almost_done
expr_stmt|;
name|s
operator|->
name|arg_end
operator|=
name|p
operator|-
literal|1
expr_stmt|;
break|break;
case|case
name|LF
case|:
name|state
operator|=
name|sw_done
expr_stmt|;
name|s
operator|->
name|arg_end
operator|=
name|p
operator|-
literal|1
expr_stmt|;
break|break;
default|default:
if|if
condition|(
name|s
operator|->
name|args
operator|.
name|nelts
operator|>
literal|2
condition|)
block|{
name|s
operator|->
name|state
operator|=
name|sw_start
expr_stmt|;
return|return
name|NGX_IMAP_PARSE_INVALID_COMMAND
return|;
block|}
name|state
operator|=
name|sw_argument
expr_stmt|;
name|s
operator|->
name|arg_start
operator|=
name|p
operator|-
literal|1
expr_stmt|;
break|break;
block|}
break|break;
comment|/* the argument */
case|case
name|sw_argument
case|:
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
literal|' '
case|:
case|case
name|CR
case|:
case|case
name|LF
case|:
if|if
condition|(
operator|!
operator|(
name|arg
operator|=
name|ngx_array_push
argument_list|(
operator|&
name|s
operator|->
name|args
argument_list|)
operator|)
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|arg
operator|->
name|len
operator|=
name|p
operator|-
name|s
operator|->
name|arg_start
expr_stmt|;
name|arg
operator|->
name|data
operator|=
name|s
operator|->
name|arg_start
expr_stmt|;
name|s
operator|->
name|arg_start
operator|=
name|NULL
expr_stmt|;
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
literal|' '
case|:
name|state
operator|=
name|sw_spaces_before_argument
expr_stmt|;
break|break;
case|case
name|CR
case|:
name|state
operator|=
name|sw_almost_done
expr_stmt|;
break|break;
case|case
name|LF
case|:
name|state
operator|=
name|sw_done
expr_stmt|;
break|break;
block|}
break|break;
default|default:
break|break;
block|}
break|break;
comment|/* end of request line */
case|case
name|sw_almost_done
case|:
switch|switch
condition|(
name|ch
condition|)
block|{
case|case
name|LF
case|:
name|state
operator|=
name|sw_done
expr_stmt|;
break|break;
default|default:
name|s
operator|->
name|state
operator|=
name|sw_start
expr_stmt|;
return|return
name|NGX_IMAP_PARSE_INVALID_COMMAND
return|;
block|}
break|break;
comment|/* suppress warning */
case|case
name|sw_done
case|:
break|break;
block|}
block|}
name|s
operator|->
name|buffer
operator|->
name|pos
operator|=
name|p
expr_stmt|;
if|if
condition|(
name|state
operator|==
name|sw_done
condition|)
block|{
if|if
condition|(
name|s
operator|->
name|arg_start
condition|)
block|{
if|if
condition|(
operator|!
operator|(
name|arg
operator|=
name|ngx_array_push
argument_list|(
operator|&
name|s
operator|->
name|args
argument_list|)
operator|)
condition|)
block|{
return|return
name|NGX_ERROR
return|;
block|}
name|arg
operator|->
name|len
operator|=
name|s
operator|->
name|arg_end
operator|-
name|s
operator|->
name|arg_start
expr_stmt|;
name|arg
operator|->
name|data
operator|=
name|s
operator|->
name|arg_start
expr_stmt|;
name|s
operator|->
name|arg_start
operator|=
name|NULL
expr_stmt|;
block|}
name|s
operator|->
name|state
operator|=
name|sw_start
expr_stmt|;
return|return
name|NGX_OK
return|;
block|}
else|else
block|{
name|s
operator|->
name|state
operator|=
name|state
expr_stmt|;
return|return
name|NGX_AGAIN
return|;
block|}
block|}
end_function

end_unit

