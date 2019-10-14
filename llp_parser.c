/************************************************************************************
* FileName: llp_parser.c
*
* Author: John.Wang
*
* Date: 2019-03-15
*
* Descripiton: 
*************************************************************************************/
/************************************************************************************
* Include
*************************************************************************************/
#include "string.h"
#include "llp_struct.h"
#include "debug.h"
#include "stdlib.h"

/************************************************************************************
* Define and Declaration
*************************************************************************************/
//static char read_block_buffer[LLP_MAX_PARSER_BLOCK_LEN] = {0};
static char write_block_buffer[LLP_MAX_PARSER_BLOCK_LEN] = {0};
static char g_temp_buff[LLP_MAX_PARSER_LINE_LEN + 1] = {0};

/************************************************************************************
* Extern Function
*************************************************************************************/
extern llp_context_struct *llp_get_context(void);

/************************************************************************************
* Local Function
*************************************************************************************/
/************************************************************************************
* Function: llp_init_queue
* Author @ Date:
* Input:
* Return:
* Description:
*************************************************************************************/
static void llp_init_queue(llp_parse_queue *parse_q, char* block_buff)
{
	parse_q->raw_data = block_buff;
	parse_q->read_pos = parse_q->write_pos = 0;
	parse_q->valid_bytes = 0;
	parse_q->queue_size = LLP_MAX_PARSER_BLOCK_LEN;

	memset(block_buff, 0, LLP_MAX_PARSER_BLOCK_LEN);
}

/************************************************************************************
* Function: llp_write_queue
* Author @ Date:
* Input:
* Return:
* Description: write queue
*************************************************************************************/
static int llp_write_queue(llp_parse_queue *parse_q, char *content_p, int len)
{
	if (NULL == content_p)
	{
		return NULL_POINTER;
	}

	if (0 == len)
	{
		return Q_NODATA;
	}

	if ((LLP_MAX_PARSER_BLOCK_LEN - parse_q->write_pos) < len)
	{
		return Q_FULL;
	}

	memcpy((void *)&parse_q->raw_data[parse_q->write_pos], (void *)content_p, len);

	parse_q->write_pos += len;
	parse_q->valid_bytes += len;

	DBG_TRACE("valid_bytes=%d, write_pos=%d, len=%d \n", parse_q->valid_bytes, parse_q->write_pos, len);

	return len;
}

/************************************************************************************
* Function: llp_read_queue
* Author @ Date:
* Input:
* Return:
* Description: read data from loop queue
*************************************************************************************/
static int llp_read_queue(llp_parse_queue *parse_q, char *buff_p, int buff_size)
{
	int read_len = 0;
	
	if (NULL == buff_p || 0 == buff_size)
	{
		return 0;
	}

	if (0 == parse_q->valid_bytes)
	{
		return 0;
	}

	read_len = (parse_q->valid_bytes >= buff_size) 
					? buff_size 
					: parse_q->valid_bytes;

		
	memcpy((void *)buff_p, (void *)&parse_q->raw_data[parse_q->read_pos], read_len);

	parse_q->valid_bytes -= read_len;
	parse_q->read_pos -= read_len;

	return read_len;
}

/************************************************************************************
* Function: llp_parse_log_file
* Author @ Date:
* Input:
* Return:
* Description: get file name from given path
*************************************************************************************/
static int llp_get_file_name(char *path)
{
	llp_context_struct *llp_cntx_p = llp_get_context();
	char* p = path;
	int   name_len = 0;

	p += strlen(path);
	while( p > path )
	{
		if ( *p == '/')
		{
			break;
		}
		else
		{
			p--;
			name_len++;
		}
	}

	strncpy(llp_cntx_p->src_file_name, p, name_len);

	DBG_TRACE("path[%s], fileName[%s],len:%d"
		, path, llp_cntx_p->src_file_name, name_len);

	return name_len;
}


/************************************************************************************
* Function: llp_parse_msg_line
* Author @ Date:
* Input:
* Return:
* Description: 
*************************************************************************************/
static char* llp_parse_msg_line(char *msg_line_p, int msg_len, llp_msg_attribut_struct *msg_attr_p)
{
	char *head_p = NULL;
	char *tail_p = NULL;
	char temp_buff[10] = {0};
	
	if (msg_line_p == NULL ||  msg_attr_p == NULL)
	{
		DBG_TRACE("msg_line_p == NULL ||  msg_attr_p == NULL");
		return msg_line_p;
	}

	if (msg_len < MSG_HEAD_LEN)
	{
		DBG_TRACE("msg_len < MSG_HEAD_LEN");
		return msg_line_p;
	}

	head_p = strstr(msg_line_p, MSG_HEAD_STR);
	if (NULL == head_p)
	{
		DBG_TRACE("NULL == head_p");
		return msg_line_p;
	}

	tail_p = strchr(head_p, MSG_TAIL_CHR);
	if (NULL == tail_p)
	{
		DBG_TRACE("NULL == tail_p");
		return head_p;
	}
	else
	{
		memcpy(temp_buff, tail_p - MSG_SN_LEN, MSG_SN_LEN);
		msg_attr_p->serial_numb = strtoul(temp_buff, NULL, 16);
		DBG_TRACE("temp_buff[%s], msg_p->serial_numb[%x]", temp_buff, msg_attr_p->serial_numb);
	}
	
	return (char*)(tail_p + 1);
}


/************************************************************************************
* Function: llp_pick_out_msg_line
* Author @ Date:
* Input:
* Return:
* Description: 
*************************************************************************************/
static int llp_pick_out_msg_line(llp_parse_queue *parse_q,  char *dest_p, int dest_buff_size)
{
	int read_len = 0;
	int first_len = 0;
	char *rd_p = NULL;
	
	if ( parse_q == NULL || dest_p == NULL)
	{
		return read_len;
	}

	read_len = MIN_VALUE(parse_q->valid_bytes, dest_buff_size);
	rd_p = &parse_q->raw_data[parse_q->read_pos];
	if (read_len + parse_q->read_pos >= parse_q->queue_size)
	{
		first_len = parse_q->queue_size - parse_q->read_pos;
		memcpy(dest_p, rd_p, first_len);

		rd_p = &parse_q->raw_data[0];
		memcpy((void*)(dest_p + first_len), rd_p, read_len - first_len);
	}
	else
	{
		memcpy((void*)dest_p, rd_p, read_len);
	}

	parse_q->read_pos = read_len - first_len;
	parse_q->valid_bytes -= read_len;

	DBG_TRACE("read_len=%d \n", read_len);
	
	return read_len;
}


/************************************************************************************
* Function: llp_parse_log_file
* Author @ Date:
* Input:
* Return:
* Description: 
*************************************************************************************/
int llp_parse_read_frame(llp_parse_queue *parse_q, my_bool is_last)
{
	llp_context_struct * llp_cntx_p = llp_get_context();
	llp_msg_attribut_struct *msg_attr_p = NULL;
	char *pbuf = NULL, *qbuf = NULL;
	int log_length = 0;
	int rest_log_length = 0;
	char *rest_msg_p = NULL;
	
	if( parse_q == NULL )
	{
		DBG_TRACE("queue Null!!");
		return 0;
	}

	pbuf = &llp_cntx_p->log_line[0];
	msg_attr_p = &llp_cntx_p->msg_attr;
	
	log_length = llp_pick_out_msg_line(parse_q, pbuf, LLP_MAX_PARSER_LINE_LEN);
	rest_msg_p = llp_parse_msg_line(pbuf, log_length, msg_attr_p);
	rest_log_length = log_length - (rest_msg_p - pbuf);
	if (rest_msg_p > pbuf &&  rest_log_length > 0)
	{	
		memmove(pbuf, rest_msg_p, rest_log_length);
		memset((void *)(pbuf + rest_log_length), 0, LLP_MAX_PARSER_LINE_LEN - rest_log_length + 1);
	}

	return 1;
}

/************************************************************************************
* Function: llp_parse_input_file
* Author @ Date:
* Input:
* Return:
* Description: 
*************************************************************************************/
static int llp_read_file_data(llp_parse_queue *read_q, FILE *file_p)
{
	char *wr_p = &read_q->raw_data[read_q->write_pos];
	int read_size = LLP_MAX_PARSER_BLOCK_LEN - read_q->write_pos;

	if (read_size > LLP_MAX_TMEP_BUFF_LEN)
	{
		read_size = LLP_MAX_TMEP_BUFF_LEN;
	}
	
	if (1 == fread(wr_p, read_size, 1, file_p))
	{
		read_q->valid_bytes += read_size;
		read_q->write_pos += read_size;

		if (read_q->write_pos >= LLP_MAX_PARSER_BLOCK_LEN)
		{
			read_q->write_pos = 0;
		}
	}
	else
	{
		read_size = 0;
	}

	DBG_TRACE("read_size=%d, read_q->valid_bytes=%d \n", read_size, read_q->valid_bytes);

	return read_size;
}

/************************************************************************************
* Function: llp_parse_input_file
* Author @ Date:
* Input:
* Return:
* Description: 
*************************************************************************************/
static int llp_parse_input_file(char* path)
{
	llp_context_struct * llp_cntx_p = llp_get_context();
	llp_parse_queue	*read_q = NULL;
	FILE *read_fp;
	int str_len = 0;
	int parsed_len = 0;
	char *p = NULL;
	
	if ((read_fp = fopen(path, B_FILE_READ_ONLY)) == NULL)
	{
		DBG_TRACE("Open path=%s, failed,return!!", path);
    	return 0;
	}
	else
	{
		DBG_TRACE("path=%s, read_fp=%p", path, read_fp);
	}

	read_q = &llp_cntx_p->read_queue;
	llp_init_queue(read_q, write_block_buffer);
	llp_cntx_p->parsed_len = 0;
   	fseek(read_fp, 0, SEEK_SET);
	
	do 
	{
		if (llp_read_file_data(read_q, read_fp) > 0)
		{
			/* parse read data frame */
			parsed_len += llp_parse_read_frame(read_q, MY_FALSE);
			DBG_TRACE("===>continue");
		}
		else
		{
			/* parse read data frame */
			parsed_len +=llp_parse_read_frame(read_q, MY_TRUE);
			DBG_TRACE("===>break");
			break;
		}
	}
	while(1);
	
	fclose(read_fp);

	return parsed_len;
}

/************************************************************************************
* Function: llp_parse_log_file
* Author @ Date:
* Input:
* Return:
* Description: 
*************************************************************************************/
llp_pre_check_enum llp_parse_log_file(char* path)
{
	llp_pre_check_enum	err_code = CK_SUCCESS;

	llp_get_file_name(path);

	DBG_TRACE("\n ===========>parse start...\n");
	llp_parse_input_file(path);
	
	//DBG_TRACE("\n ===========>arrange start...\n");
	//llp_arrange_log_file(LLP_TEMP_FILE);
	
    return err_code;
}

