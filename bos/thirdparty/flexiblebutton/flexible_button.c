/**
 * @File:    flexible_button.c
 * @Author:  MurphyZhao
 * @Date:    2018-09-29
 *
 * Copyright (c) 2018-2019 MurphyZhao <d2014zjt@163.com>
 *               https://github.com/murphyzhao
 * All rights reserved.
 * License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Change logs:
 * Date        Author       Notes
 * 2018-09-29  MurphyZhao   First add
 * 2019-08-02  MurphyZhao   Migrate code to github.com/murphyzhao account
 * 2019-12-26  MurphyZhao   Refactor code and implement multiple clicks
 *
 */

#include "flexible_button.h"

#if (defined(_FLEXIBLEBUTTON_ENABLE) && (_FLEXIBLEBUTTON_ENABLE == 1))

#define EVENT_SET_AND_EXEC_CB(btn, evt)         \
    do                                          \
    {                                           \
        btn->event = evt;                       \
        if (flex_callback)                      \
            flex_callback((flex_button_t*)btn); \
    } while (0)

/**
 * BTN_IS_PRESSED
 *
 * 1: is pressed
 * 0: is not pressed
 */
#define BTN_IS_PRESSED(i) (g_btn_status_reg.btn[((i) / 8)] & (1 << ((i) % 8)))

enum FLEX_BTN_STAGE
{
    FLEX_BTN_STAGE_DEFAULT        = 0,
    FLEX_BTN_STAGE_DOWN           = 1,
    FLEX_BTN_STAGE_MULTIPLE_CLICK = 2
};

#define TYPE_N_BYTE (32)

typedef struct
{
    uint8_t btn[TYPE_N_BYTE];
} btn_type_t;

static flex_button_t* btn_head = NULL;

static uint16_t max_multiple_clicks_interval = MAX_MULTIPLE_CLICKS_INTERVAL;
static uint16_t short_press_start_tick       = (FLEX_MS_TO_SCAN_CNT(FLEX_BTN_SHORT_XMS));
static uint16_t long_press_start_tick        = (FLEX_MS_TO_SCAN_CNT(FLEX_BTN_LONG_XMS));
static uint16_t long_hold_start_tick         = (FLEX_MS_TO_SCAN_CNT(FLEX_BTN_LLONG_XMS));

static flex_button_response_callback flex_callback      = NULL;
static flex_button_read_function     flex_read_function = NULL;

/**
 * g_logic_level
 *
 * The logic level of the button pressed,
 * Each bit represents a button.
 *
 * First registered button, the logic level of the button pressed is
 * at the low bit of g_logic_level.
 */
btn_type_t g_logic_level;

/**
 * g_btn_status_reg
 *
 * The status register of all button, each bit records the pressing state of a button.
 *
 * First registered button, the pressing state of the button is
 * at the low bit of g_btn_status_reg.
 */
btn_type_t g_btn_status_reg;

static uint8_t button_cnt = 0;

/**
 * @brief Register a user button
 *
 * @param button: button structure instance
 * @return Number of keys that have been registered, or -1 when error
 */
int32_t flex_button_register(flex_button_t* button)
{
    static uint8_t init_flag = 0;
    flex_button_t* curr      = btn_head;

    if (init_flag == 0)
    {
        init_flag = 1;
        memset(&g_logic_level, 0, sizeof(btn_type_t));
        memset(&g_btn_status_reg, 0, sizeof(btn_type_t));
    }

    if (!button)
    {
        return -1;
    }

    while (curr)
    {
        if (curr == button)
        {
            return -1; /* already exist. */
        }
        curr = curr->next;
    }

    /**
     * First registered button is at the end of the 'linked list'.
     * btn_head points to the head of the 'linked list'.
     */
    button->next      = btn_head;
    button->status    = FLEX_BTN_STAGE_DEFAULT;
    button->event     = FLEX_BTN_PRESS_NONE;
    button->scan_cnt  = 0;
    button->click_cnt = 0;
    btn_head          = button;

    /**
     * First registered button, the logic level of the button pressed is
     * at the low bit of g_logic_level.
     */
    g_logic_level.btn[(button_cnt / 8)] |= (button->pressed_logic_level << (button_cnt % 8));
    button_cnt++;

    return button_cnt;
}

/**
 * @brief Read all key values in one scan cycle
 *
 * @param void
 * @return none
 */
static void flex_button_read(void)
{
    uint8_t        i;
    flex_button_t* target;

    /* The button that was registered first, the button value is in the low position of raw_data */
    btn_type_t raw_data;

    memset(&raw_data, 0, sizeof(btn_type_t));

    for (target = btn_head, i = button_cnt - 1; (target != NULL) && (flex_read_function != NULL);
         target = target->next, i--)
    {
        raw_data.btn[(i / 8)] = raw_data.btn[(i / 8)] | ((flex_read_function)(target) << (i % 8));
    }

    for (i = 0; i < TYPE_N_BYTE; i++)
    {
        g_btn_status_reg.btn[i] = (~raw_data.btn[i]) ^ g_logic_level.btn[i];
    }
}

/**
 * @brief Handle all key events in one scan cycle.
 *        Must be used after 'flex_button_read' API
 *
 * @param void
 * @return Activated button count
 */
static uint8_t flex_button_process(void)
{
    uint8_t        i;
    uint8_t        active_btn_cnt = 0;
    flex_button_t* target;

    for (target = btn_head, i = button_cnt - 1; target != NULL; target = target->next, i--)
    {
        if (target->status > FLEX_BTN_STAGE_DEFAULT)
        {
            target->scan_cnt++;
            if (target->scan_cnt >= ((1 << (sizeof(target->scan_cnt) * 8)) - 1))
            {
                target->scan_cnt = long_hold_start_tick;
            }
        }

        switch (target->status)
        {
            case FLEX_BTN_STAGE_DEFAULT: /* stage: default(button up) */
                if (BTN_IS_PRESSED(i))   /* is pressed */
                {
                    target->scan_cnt  = 0;
                    target->click_cnt = 0;

                    EVENT_SET_AND_EXEC_CB(target, FLEX_BTN_PRESS_DOWN);

                    /* swtich to button down stage */
                    target->status = FLEX_BTN_STAGE_DOWN;
                }
                else
                {
                    target->event = FLEX_BTN_PRESS_NONE;
                }
                break;

            case FLEX_BTN_STAGE_DOWN:  /* stage: button down */
                if (BTN_IS_PRESSED(i)) /* is pressed */
                {
                    if (target->click_cnt > 0) /* multiple click */
                    {
                        if (target->scan_cnt > max_multiple_clicks_interval)
                        {
                            EVENT_SET_AND_EXEC_CB(target,
                                                  target->click_cnt < FLEX_BTN_PRESS_REPEAT_CLICK
                                                      ? target->click_cnt
                                                      : FLEX_BTN_PRESS_REPEAT_CLICK);

                            /* swtich to button down stage */
                            target->status    = FLEX_BTN_STAGE_DOWN;
                            target->scan_cnt  = 0;
                            target->click_cnt = 0;
                        }
                    }
                    else if (target->scan_cnt >= long_hold_start_tick)
                    {
                        if (target->event != FLEX_BTN_PRESS_LONG_HOLD)
                        {
                            EVENT_SET_AND_EXEC_CB(target, FLEX_BTN_PRESS_LONG_HOLD);
                        }
                    }
                    else if (target->scan_cnt >= long_press_start_tick)
                    {
                        if (target->event != FLEX_BTN_PRESS_LONG_START)
                        {
                            EVENT_SET_AND_EXEC_CB(target, FLEX_BTN_PRESS_LONG_START);
                        }
                    }
                    else if (target->scan_cnt >= short_press_start_tick)
                    {
                        if (target->event != FLEX_BTN_PRESS_SHORT_START)
                        {
                            EVENT_SET_AND_EXEC_CB(target, FLEX_BTN_PRESS_SHORT_START);
                        }
                    }
                }
                else /* button up */
                {
                    if (target->scan_cnt >= long_hold_start_tick)
                    {
                        EVENT_SET_AND_EXEC_CB(target, FLEX_BTN_PRESS_LONG_HOLD_UP);
                        target->status = FLEX_BTN_STAGE_DEFAULT;
                    }
                    else if (target->scan_cnt >= long_press_start_tick)
                    {
                        EVENT_SET_AND_EXEC_CB(target, FLEX_BTN_PRESS_LONG_UP);
                        target->status = FLEX_BTN_STAGE_DEFAULT;
                    }
                    else if (target->scan_cnt >= short_press_start_tick)
                    {
                        EVENT_SET_AND_EXEC_CB(target, FLEX_BTN_PRESS_SHORT_UP);
                        target->status = FLEX_BTN_STAGE_DEFAULT;
                    }
                    else
                    {
                        /* swtich to multiple click stage */
                        target->status = FLEX_BTN_STAGE_MULTIPLE_CLICK;
                        target->click_cnt++;
                    }
                }
                break;

            case FLEX_BTN_STAGE_MULTIPLE_CLICK: /* stage: multiple click */
                if (BTN_IS_PRESSED(i))          /* is pressed */
                {
                    /* swtich to button down stage */
                    target->status   = FLEX_BTN_STAGE_DOWN;
                    target->scan_cnt = 0;
                }
                else
                {
                    if (target->scan_cnt > max_multiple_clicks_interval)
                    {
                        EVENT_SET_AND_EXEC_CB(target,
                                              target->click_cnt < FLEX_BTN_PRESS_REPEAT_CLICK
                                                  ? target->click_cnt
                                                  : FLEX_BTN_PRESS_REPEAT_CLICK);

                        /* swtich to default stage */
                        target->status = FLEX_BTN_STAGE_DEFAULT;
                    }
                }
                break;
        }

        if (target->status > FLEX_BTN_STAGE_DEFAULT)
        {
            active_btn_cnt++;
        }
    }

    return active_btn_cnt;
}

/**
 * flex_button_event_read
 *
 * @brief Get the button event of the specified button.
 *
 * @param button: button structure instance
 * @return button event
 */
flex_button_event_t flex_button_event_read(flex_button_t* button)
{
    return (flex_button_event_t)(button->event);
}

/**
 * flex_button_scan
 *
 * @brief Start key scan.
 *        Need to be called cyclically within the specified period.
 *        Sample cycle: 5 - 20ms
 *
 * @param void
 * @return Activated button count
 */
uint8_t flex_button_scan(void)
{
    flex_button_read();
    return flex_button_process();
}

int32_t flex_button_reg_function(flex_button_response_callback cb, flex_button_read_function read_f)
{
    if (cb == NULL || read_f == NULL)
    {
        return -1;
    }
    flex_callback      = cb;
    flex_read_function = read_f;
    return 0;
}

#endif
