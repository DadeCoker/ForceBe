/**
 * @file      ansi.h
 * @author    Ji Youzhou
 * @version   V0.1
 * @date      28 Oct 2019
 * @brief     [brief]
 * *****************************************************************************
 * @attention
 *
 * MIT License
 *
 * Copyright (C) 2019 Ji Youzhou
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ansi_h
#define __ansi_h

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "nr_micro_shell_config.h"

#if (defined(_NR_MICRO_SHELL_ENABLE) && (_NR_MICRO_SHELL_ENABLE == 1))

#define NR_ANSI_CTRL_MAX_LEN 20
#define NR_ANSI_MAX_EX_DATA_NUM 1

enum
{
    ANSI_ENABLE_SHOW,
    ANSI_DISABLE_SHOW
};

typedef struct nr_ansi_struct
{
    short        p;
    unsigned int counter;
    char         current_line[NR_ANSI_LINE_SIZE];

    char combine_buf[NR_ANSI_CTRL_MAX_LEN];
    char cmd_num;
    char combine_state;
} ansi_st;

typedef void (*ansi_fun_t)(ansi_st *);

#define NR_ANSI_SET_TEXT(cmd) ((const char *)"\033["##cmd##"m") /** the form of set text font */

/** set the color of background */
#define NR_ANSI_BBLACK "40"
#define NR_ANSI_BRED "41"
#define NR_ANSI_BGREEN "42"
#define NR_ANSI_BGRAY "43"
#define NR_ANSI_BBLUE "44"
#define NR_ANSI_BPURPLE "45"
#define NR_ANSI_BAQUAM "46"
#define NR_ANSI_BWHITE "47"

/** set the color of character */
#define NR_ANSI_FBLACK "30"
#define NR_ANSI_FRED "31"
#define NR_ANSI_FGREEN "32"
#define NR_ANSI_FGRAY "33"
#define NR_ANSI_FBLUE "34"
#define NR_ANSI_FPURPLE "35"
#define NR_ANSI_FAQUAM "36"
#define NR_ANSI_FWHITE "37"

/** special effect */
#define NR_ANSI_NORMAL "0"
#define NR_ANSI_BRIGHT "1"
#define NR_ANSI_UNDERLINE "4"
#define NR_ANSI_FLASH "5"
#define NR_ANSI_INVERSE "7"
#define NR_ANSI_INVISABLE "8"

/****************************************************/

/** clear code */
#define NR_ANSI_CLEAR_RIGHT "\033[K"
#define NR_ANSI_CLEAR_LEFT "\033[1K"
#define NR_ANSI_CLEAR_WHOLE "\033[2K"

#define NR_ANSI_CLEAR_SCREEN "\033[2J"

#define NR_ANSI_HIDE_COURSER "\033[?25l"
#define NR_ANSI_SHOW_COURSER "\033[?25h"

#if defined(__GNUC__)
#define NR_ANSI_SET_FONT(cmd) "null"
#define NR_ANSI_CLR_R_NCHAR(cmd) "null"
#define NR_ANSI_CLR_R_MV_L_NCHAR(cmd) "null"

/** move course code */
#define NR_ANSI_MV_L_N(n) "null"
#define NR_ANSI_MV_R_N(n) "null"

#else
#define NR_ANSI_SET_FONT(cmd) ((const char *)"\033["##cmd##"I")
#define NR_ANSI_CLR_R_NCHAR(cmd) ((const char *)"\033["## #cmd##"X")
#define NR_ANSI_CLR_R_MV_L_NCHAR(cmd) ((const char *)"\033["## #cmd##"P")

/** move course code */
#define NR_ANSI_MV_L_N(n) ((const char *)"\033["## #n##"D")
#define NR_ANSI_MV_R_N(n) ((const char *)"\033["## #n##"C")
#endif

#define NR_ANSI_NORMAL "0"
#define NR_ANSI_SONG "1"
#define NR_ANSI_HEI "2"
#define NR_ANSI_KAI "3"

/** special characters functions \b,\n,\r,\t*/
#define nr_ansi_in_bsb_function nr_ansi_in_backspace
#define nr_ansi_in_bsn_function nr_ansi_in_newline
#define nr_ansi_in_bsr_function nr_ansi_in_enter
#define nr_ansi_in_bst_function nr_ansi_in_tab

/** control characters functions */
#define nr_ansi_in_m_function nr_ansi_ctrl_common_slover
#define nr_ansi_in_I_function nr_ansi_ctrl_common_slover
#define nr_ansi_in_A_function nr_ansi_in_up
#define nr_ansi_in_B_function nr_ansi_in_down
#define nr_ansi_in_C_function nr_ansi_in_right
#define nr_ansi_in_D_function nr_ansi_in_left
#define nr_ansi_in_X_function nr_ansi_ctrl_common_slover
#define nr_ansi_in_K_function nr_ansi_ctrl_common_slover
#define nr_ansi_in_M_function nr_ansi_ctrl_common_slover
#define nr_ansi_in_P_function nr_ansi_ctrl_common_slover
#define nr_ansi_in_J_function nr_ansi_ctrl_common_slover
#define nr_ansi_in_at_function nr_ansi_ctrl_common_slover
#define nr_ansi_in_L_function nr_ansi_ctrl_common_slover
#define nr_ansi_in_l_function nr_ansi_ctrl_common_slover
#define nr_ansi_in_h_function nr_ansi_ctrl_common_slover
#define nr_ansi_in_n_function nr_ansi_ctrl_common_slover
#define nr_ansi_in_H_function nr_ansi_ctrl_common_slover
#define nr_ansi_in_s_function nr_ansi_ctrl_common_slover
#define nr_ansi_in_u_function nr_ansi_ctrl_common_slover

char ansi_get_char(char x, ansi_st *ansi);
void ansi_init(ansi_st *ansi);
void ansi_clear_current_line(ansi_st *ansi);

extern ansi_st nr_ansi;

#endif

#ifdef __cplusplus
}
#endif

#endif
/******************* (C) COPYRIGHT 2019 Ji Youzhou *****END OF FILE*****************/
