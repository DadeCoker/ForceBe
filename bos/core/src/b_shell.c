/**
 *!
 * \file        b_shell.c
 * \version     v0.0.1
 * \date        2020/01/02
 * \author      Bean(notrynohigh@outlook.com)
 *******************************************************************************
 * @attention
 * 
 * Copyright (c) 2020 Bean
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO SHELL SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *******************************************************************************
 */
   
/*Includes ----------------------------------------------*/
#include "b_shell.h"
#include <string.h>
#if _NR_MICRO_SHELL_ENABLE
/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup SHELL
 * \{
 */

/** 
 * \defgroup SHELL_Private_TypesDefinitions
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup SHELL_Private_Defines
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup SHELL_Private_Macros
 * \{
 */
static void _bShellLSCmd(char argc, char *argv);  
static void _bShellTestCmd(char argc, char *argv);  
/**
 * \}
 */
   
/** 
 * \defgroup SHELL_Private_Variables
 * \{
 */
const static_cmd_st static_cmd[] =
{
    {"ls", _bShellLSCmd},
    {"test", _bShellTestCmd},
    {"\0", NULL}
};



/**
 * \}
 */
   
/** 
 * \defgroup SHELL_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup SHELL_Private_Functions
 * \{
 */
/**
 * @brief ls command
 */
void _bShellLSCmd(char argc, char *argv)
{
	unsigned int i = 0;
	if (argc > 1)
	{
		if (!strcmp("cmd", &argv[argv[1]]))
		{

			for (i = 0; nr_cmd_start_add[i].fp != NULL; i++)
			{
				shell_printf(nr_cmd_start_add[i].cmd);
				shell_printf("\r\n");
			}
		}
		else if (!strcmp("-v", &argv[argv[1]]))
		{
			shell_printf("ls version 1.0.\r\n");
		}
		else if (!strcmp("-h", &argv[argv[1]]))
		{
			shell_printf("useage: ls [options]\r\n");
			shell_printf("options: \r\n");
			shell_printf("\t -h \t: show help\r\n");
			shell_printf("\t -v \t: show version\r\n");
			shell_printf("\t cmd \t: show all commands\r\n");
		}
	}
	else
	{
		shell_printf("ls need more arguments!\r\n");
	}
}


static void _bShellTestCmd(char argc, char *argv)
{
	unsigned int i;
	shell_printf("test command:\r\n");
	for (i = 0; i < argc; i++)
	{
		shell_printf("paras %d: %s\r\n", i, &(argv[argv[i]]));
	}
}   
/**
 * \}
 */
   
/** 
 * \addtogroup SHELL_Exported_Functions
 * \{
 */


int bShellStart()
{
    shell_init();
    return 0;
}


int bShellParse(uint8_t *pbuf, uint16_t len)
{
    if(pbuf == NULL || len == 0)
    {
        return -1;
    }
    while(len--)
    {
        shell(*pbuf);
        pbuf++;
    }
    return 0;
}



/**
 * \}
 */

/**
 * \}
 */

/**
 * \}
 */
#endif

/************************ Copyright (c) 2019 Bean *****END OF FILE****/

