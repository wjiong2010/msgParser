/************************************************************************************
* FileName: llp_def.h
*
* Author: John.Wang
*
* Date: 2019-03-15
*
* Descripiton: Log Line Parser 
*************************************************************************************/
#ifndef __LLP_DEF_H__
#define __LLP_DEF_H__
/************************************************************************************
* Include
*************************************************************************************/
#include <stdio.h>
#include <stdlib.h>

/************************************************************************************
* Types 
*************************************************************************************/
typedef unsigned long int U64;
/************************************************************************************
* Macros
*************************************************************************************/
#define MIN_VALUE(a, b) ((a) >= (b) ? (b) : (a))

#define LLP_MAX_PATH_LEN  255

#define LLP_TEMP_FILE	"temp.txt"
#define LLP_LOG_FILE	"LOG_FILE.txt"

#define LLP_MAX_PARSER_BLOCK_LEN	10240	/* 10K bytes */
#define LLP_MAX_PARSER_LINE_LEN		1024
#define LLP_MAX_TMEP_BUFF_LEN    	2048

#define T_FILE_READ_ONLY		"r"
#define T_FILE_WRITE_ONLY		"w"
#define T_FILE_WRITE_ADD		"a+"
#define B_FILE_READ_ONLY		"rb"


#define LLP_LOG_LINE_PREFIX   	"LOG-"
#define LLP_LOG_LINE_PREFIX_LEN	4
#define LLP_FLAG 				"\r\n\r\n"
#define LLP_FLAG_LEN 			4

#define MSG_HEAD_STR			"[2019"
#define MSG_HEAD_LEN			5
#define MSG_TAIL_CHR			'$'	
#define MSG_SN_LEN				4	/* xxxx$ */

#define MSG_DATE_TIME_LEN		20
#define MAX_MSG_NUM				6000
/************************************************************************************
* Enums 
*************************************************************************************/
typedef enum
{
    CK_SUCCESS = 0,
    CK_PATH_ERR,
    CK_FILE_OPEN_ERR,
    CK_FILE_OVER_LEN,
    CK_FILE_END_ERR,
    CK_FILE_READ_ERR,
    CK_CMD_OVER_LEN,
    CK_NUM
} llp_pre_check_enum;

typedef enum
{
    Q_FULL = -1,
    Q_EMPTY = -2,
	NULL_POINTER = -3,
	Q_NODATA = -4
} llp_queue_ret_enum;

typedef enum
{
	MY_FALSE,
	MY_TRUE
} my_bool;
	
#endif /* __LLP_DEF_H__ */

