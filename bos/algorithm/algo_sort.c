/**
 *!
 * \file        algo_sort.c
 * \version     v0.0.1
 * \date        2020/05/13
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *******************************************************************************
 */

/*Includes ----------------------------------------------*/
#include "inc/algo_sort.h"

#if (defined(_ALGO_SORT_ENABLE) && (_ALGO_SORT_ENABLE == 1))

/**
 * \addtogroup ALGORITHM
 * \{
 */

/**
 * \addtogroup SORT
 * \{
 */

/**
 * \defgroup SORT_Private_TypesDefinitions
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup SORT_Private_Defines
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup SORT_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup SORT_Private_Variables
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup SORT_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup SORT_Private_Functions
 * \{
 */
#define _BUBBLE_SORT(src)                    \
    for (i = 0; i < size - 1; i++)           \
    {                                        \
        for (j = 0; j < size - i - 1; j++)   \
        {                                    \
            if ((src)[j] > (src)[j + 1])     \
            {                                \
                tmp          = (src)[j];     \
                (src)[j]     = (src)[j + 1]; \
                (src)[j + 1] = tmp;          \
            }                                \
        }                                    \
    }

#define _SELECTION_SORT(src)           \
    for (i = 0; i < size - 1; i++)     \
    {                                  \
        k = i;                         \
        for (j = i + 1; j < size; j++) \
        {                              \
            if ((src)[j] < (src)[k])   \
            {                          \
                k = j;                 \
            }                          \
        }                              \
        tmp      = (src)[k];           \
        (src)[k] = (src)[i];           \
        (src)[i] = tmp;                \
    }

#define _INSERTION_SORT(src)                               \
    for (i = 1; i < size; i++)                             \
    {                                                      \
        if ((src)[i] < (src)[i - 1])                       \
        {                                                  \
            tmp = (src)[i];                                \
            for (j = i - 1; j >= 0 && (src)[j] > tmp; j--) \
            {                                              \
                (src)[j + 1] = (src)[j];                   \
            }                                              \
            (src)[j + 1] = tmp;                            \
        }                                                  \
    }
/**
 * \}
 */

/**
 * \addtogroup SORT_Exported_Functions
 * \{
 */

void BubbleSort(void *arr, int size, uint8_t type)
{
    int i, j;
    switch (type)
    {
        case _ALGO_SORT_I8:
        {
            int8_t tmp;
            _BUBBLE_SORT((int8_t *)arr);
        }
        break;
        case _ALGO_SORT_U8:
        {
            uint8_t tmp;
            _BUBBLE_SORT((uint8_t *)arr);
        }
        break;
        case _ALGO_SORT_I16:
        {
            int16_t tmp;
            _BUBBLE_SORT((int16_t *)arr);
        }
        break;
        case _ALGO_SORT_U16:
        {
            uint16_t tmp;
            _BUBBLE_SORT((uint16_t *)arr);
        }
        break;
        case _ALGO_SORT_I32:
        {
            int32_t tmp;
            _BUBBLE_SORT((int32_t *)arr);
        }
        break;
        case _ALGO_SORT_U32:
        {
            uint32_t tmp;
            _BUBBLE_SORT((uint32_t *)arr);
        }
        break;
    }
}

void SelectionSort(void *arr, int size, uint8_t type)
{
    int i, j, k;
    switch (type)
    {
        case _ALGO_SORT_I8:
        {
            int8_t tmp;
            _SELECTION_SORT((int8_t *)arr);
        }
        break;
        case _ALGO_SORT_U8:
        {
            uint8_t tmp;
            _SELECTION_SORT((uint8_t *)arr);
        }
        break;
        case _ALGO_SORT_I16:
        {
            int16_t tmp;
            _SELECTION_SORT((int16_t *)arr);
        }
        break;
        case _ALGO_SORT_U16:
        {
            uint16_t tmp;
            _SELECTION_SORT((uint16_t *)arr);
        }
        break;
        case _ALGO_SORT_I32:
        {
            int32_t tmp;
            _SELECTION_SORT((int32_t *)arr);
        }
        break;
        case _ALGO_SORT_U32:
        {
            uint32_t tmp;
            _SELECTION_SORT((uint32_t *)arr);
        }
        break;
    }
}

void InsertionSort(void *arr, int size, uint8_t type)
{
    int i, j;
    switch (type)
    {
        case _ALGO_SORT_I8:
        {
            int8_t tmp;
            _INSERTION_SORT((int8_t *)arr);
        }
        break;
        case _ALGO_SORT_U8:
        {
            uint8_t tmp;
            _INSERTION_SORT((uint8_t *)arr);
        }
        break;
        case _ALGO_SORT_I16:
        {
            int16_t tmp;
            _INSERTION_SORT((int16_t *)arr);
        }
        break;
        case _ALGO_SORT_U16:
        {
            uint16_t tmp;
            _INSERTION_SORT((uint16_t *)arr);
        }
        break;
        case _ALGO_SORT_I32:
        {
            int32_t tmp;
            _INSERTION_SORT((int32_t *)arr);
        }
        break;
        case _ALGO_SORT_U32:
        {
            uint32_t tmp;
            _INSERTION_SORT((uint32_t *)arr);
        }
        break;
    }
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

/************************ Copyright (c) 2020 Bean *****END OF FILE****/
