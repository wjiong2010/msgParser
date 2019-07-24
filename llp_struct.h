/************************************************************************************
* FileName: llp_struct.h
*
* Author: John.Wang
*
* Date: 2019-03-15
*
* Descripiton: Log Line Parser 
*************************************************************************************/
#ifndef __LLP_STRUCT_H__
#define __LLP_STRUCT_H__
/************************************************************************************
* Include
*************************************************************************************/
#include "llp_def.h"

/************************************************************************************
* Structs 
*************************************************************************************/
typedef struct 
{
	int read_pos;
	int write_pos;
	int valid_bytes;
	int queue_size;
	char *raw_data;
} llp_parse_queue;

typedef struct 
{
	int type;
	char date_time[MSG_DATE_TIME_LEN + 1];
	U64 serial_numb;
} llp_msg_attribut_struct;


typedef struct
{
	FILE *read_fp;
	FILE *write_fp;
	char src_file_name[LLP_MAX_PATH_LEN + 1];
	int  parsed_len;
	char parsed_frame[LLP_MAX_PARSER_LINE_LEN];
	char log_line[LLP_MAX_PARSER_LINE_LEN + 1];
	llp_parse_queue read_queue;
	llp_msg_attribut_struct msg_attr;
	int  sn_array[MAX_MSG_NUM];
	
} llp_context_struct;



#endif /* __LLP_STRUCT_H__ */


