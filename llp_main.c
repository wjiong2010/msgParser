/************************************************************************************
* FileName: llp_parser.c
*
* Author: John.Wang
*
* Date: 2019-03-15
*
* Descripiton: Log Line Parser 
*************************************************************************************/
/************************************************************************************
* Include
*************************************************************************************/
#include <string.h>
#include "llp_parser.h"
#include "llp_struct.h"
#include "debug.h"

/************************************************************************************
* Define and Function declaration
*************************************************************************************/
static llp_context_struct llp_cntx_st;
static llp_context_struct *llp_cntx_p = &llp_cntx_st;


/************************************************************************************
* Process
*************************************************************************************/
/************************************************************************************
* Function: llp_init_context
* Author @ Date:
* Input:
* Return:
* Description:
*************************************************************************************/
static void llp_init_context(void)
{
	memset(llp_cntx_p, 0, sizeof(llp_context_struct));
}

/************************************************************************************
* Function: llp_get_context
* Author @ Date:
* Input:
* Return:
* Description:
*************************************************************************************/
llp_context_struct *llp_get_context(void)
{
	return llp_cntx_p;
}

/************************************************************************************
* Function: llp_pre_check
* Author @ Date:
* Input:
* Return:
* Description:
*************************************************************************************/
static int llp_pre_check(char* path)
{
    FILE *fp = NULL;
    int  file_size = 0;
	DBG_TRACE("\nfvr_gen_pre_check:%s\n", path);
	
    if (strlen(path) > LLP_MAX_PATH_LEN)
    {
        DBG_TRACE("fvr_gen_pre_check ret=%d\n", CK_PATH_ERR);
        return CK_PATH_ERR;
    }

    if ((fp = fopen(path, T_FILE_READ_ONLY)) == NULL)
    {
        DBG_TRACE("fvr_gen_pre_check ret=%d\n", CK_FILE_OPEN_ERR);
        return CK_FILE_OPEN_ERR;
    }
	else
	{
		fclose(fp);
	}
	
#if 0
    /* check file size, configuration file must smaller than 10k */
    fseek(fp, 0L, SEEK_END);
    if ((file_size = ftell(fp)) > FVR_CK_MAX_FILE_SIZE)
    {
        fclose(fp);
        //printf("1-fvr_gen_pre_check file_size=%d\n", file_size);
        return CK_FILE_OVER_LEN;
    }
    else
    {
        //printf("2-fvr_gen_pre_check file_size=%d\n", file_size);
        fclose(fp);
    }
#endif

    return CK_SUCCESS;
}

/************************************************************************************
* Function: llp_print_input_arg
* Author @ Date:
* Input:
* Return:
* Description: 
*************************************************************************************/
static void llp_print_input_arg(int argc, char* argv[])
{
	char print_buf[128] = {0};
	int i, len;

	len = snprintf(print_buf, 128, "argc:%d argv:", argc);
    for (i = 0; i < argc; i++)
        len += snprintf((char *)(print_buf + len), 128 - len, "%s ", argv[i]);

	DBG_TRACE("%s", print_buf);
}


/************************************************************************************
* Function: main
* Author @ Date:
* Input:
* Return:
* Description: Entrance of the parser
*************************************************************************************/
int main(int argc, char* argv[])
{
	llp_pre_check_enum err_code = 0;

	llp_print_input_arg(argc, argv);
	
	llp_init_context();

    if (argc <= 1)
    {
        DBG_TRACE("Error! Unknow parse file!!\n");
        return 0;
    }
	
    if (CK_SUCCESS == llp_pre_check(argv[1]))
    {
        err_code = llp_parse_log_file(argv[1]);
    }
	
DBG_TRACE("Error! Unknow parse file!!\n");
    return 0;
}


