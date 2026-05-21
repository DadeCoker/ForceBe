/**
 * @file      ansi_port.h
 * @author    Ji Youzhou
 * @version   V0.1
 * @date      28 Oct 2019
 * @brief     [brief]
 * *****************************************************************************
 * @attention
 * 
 * MIT License
 * 
 * Copyright (C) 2019 Ji Youzhou. or its affiliates.  All Rights Reserved.
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
#ifndef __ansi_port_h
#define __ansi_port_h

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "ansi.h"

#if _NR_MICRO_SHELL_ENABLE

struct nr_ansi_struct;
typedef struct nr_ansi_struct ansi_st;

void nr_ansi_ctrl_common_slover(ansi_st *ansi);
void nr_ansi_in_newline(ansi_st *ansi);
void nr_ansi_in_backspace(ansi_st *ansi);
void nr_ansi_in_up(ansi_st *ansi);
void nr_ansi_in_down(ansi_st *ansi);
void nr_ansi_in_left(ansi_st *ansi);
void nr_ansi_in_right(ansi_st *ansi);
void nr_ansi_in_tab(ansi_st *ansi);
void nr_ansi_in_enter(ansi_st *ansi);
void ansi_show_char(char ch);
#endif

#ifdef __cplusplus
}
#endif

#endif
/******************* (C) COPYRIGHT 2019 Ji Youzhou *****END OF FILE*****************/
