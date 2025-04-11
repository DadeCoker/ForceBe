/**
 *!
 * \file        b_menu.c
 * \version     v0.0.1
 * \date        2020/03/04
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
#include "b_MENU.h"  
#if _MENU_ENABLE

/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup MENU
 * \{
 */


/** 
 * \defgroup MENU_Private_TypesDefinitions
 * \{
 */
  
/**
 * \}
 */
   
/** 
 * \defgroup MENU_Private_Defines
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup MENU_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup MENU_Private_Variables
 * \{
 */
static bMenuItem_t bMenuItemTable[_MENU_ITEM_NUMBER];
static uint8_t ItemIndex = 0;

static bMenuItem_t *pC_Item = NULL;
static uint32_t Pre_ID = 0;
/**
 * \}
 */
   
/** 
 * \defgroup MENU_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup MENU_Private_Functions
 * \{
 */
static int _bMenuFindIndex(uint32_t id)
{
    int i = 0;
    for(i = 0;i < ItemIndex;i++)
    {
        if(bMenuItemTable[i].id == id)
        {
            return i;
        }
    }
    return -1;
}    
/**
 * \}
 */
   
/** 
 * \addtogroup MENU_Exported_Functions
 * \{
 */

int bMenuAddSibling(uint32_t ref_id, uint32_t id, pCreateUI f)
{
    int i = 0;
    bMenuItem_t *ptmp, *pref = NULL;
    if(ItemIndex >= _MENU_ITEM_NUMBER)
    {
        return -1;
    }
    ptmp = &bMenuItemTable[ItemIndex];
    ptmp->id = id;
    ptmp->create_ui = f;
    ptmp->visible = 1;
    if(ref_id == id)
    {
        ptmp->child = ptmp->parent = ptmp->next = ptmp->prev = ptmp;
    }
    else
    {
        if((i = _bMenuFindIndex(ref_id)) < 0)
        {
            return -1;
        }
        pref = &bMenuItemTable[i];
        
        ptmp->prev = pref;
        pref->next->prev = ptmp;
        ptmp->next = pref->next;
        pref->next = ptmp;
        ptmp->parent = pref->parent;
        ptmp->child = ptmp;
    }
    if(pC_Item == NULL)
    {
        pC_Item = ptmp;
        Pre_ID = ptmp->id;
        if(pC_Item->create_ui)
        {
            pC_Item->create_ui(Pre_ID);
        }
    }
    ItemIndex += 1;
    return ItemIndex;
}


int bMenuAddChild(uint32_t ref_id, uint32_t id, pCreateUI f)
{
    int i = 0;
    bMenuItem_t *ptmp, *pref = NULL;
    if(ItemIndex >= _MENU_ITEM_NUMBER)
    {
        return -1;
    }
    
    ptmp = &bMenuItemTable[ItemIndex];
    ptmp->id = id;
    ptmp->create_ui = f;
    ptmp->visible = 1;    
    if((i = _bMenuFindIndex(ref_id)) < 0)
    {
        return -1;
    }
    pref = &bMenuItemTable[i];
    
    ptmp->child = ptmp->next = ptmp->prev = ptmp;  
    ptmp->parent = pref;
    pref->child = ptmp;
    ItemIndex += 1;
    return ItemIndex;
}


void bMenuAction(uint8_t cmd)
{
    bMenuItem_t *c = pC_Item;
    switch(cmd)
    {
        case MENU_UP:
            do
            {
                c = c->prev;
            }while(c->visible == 0);
            break;
        case MENU_DOWN:
            do
            {
                c = c->next;
            }while(c->visible == 0);
            break;
        case MENU_BACK:
            if(c->parent->visible != 0)
            {
                c = c->parent;
            }
            break;
        case MENU_ENTER:
            if(c->child->visible != 0)
            {
                c = c->child;
            }
            break;
        default:
            break;
    }
    if(c != pC_Item)
    {
        pC_Item = c;
        if(pC_Item->create_ui)
        {
            pC_Item->create_ui(Pre_ID);
        }
        Pre_ID = pC_Item->id;
    }
}

void bMenuJump(uint32_t id)
{
    int i = 0;
    bMenuItem_t *c;
    if((i = _bMenuFindIndex(id)) < 0)
    {
        return;
    }
    c = &bMenuItemTable[i];
    if(c->visible && c != pC_Item)
    {
        pC_Item = c;
        if(pC_Item->create_ui)
        {
            pC_Item->create_ui(Pre_ID);
        }
        Pre_ID = pC_Item->id; 
    }
}

int bMenuSetVisible(uint32_t id, uint8_t s)
{
    int i = 0;
    bMenuItem_t *c;
    if((i = _bMenuFindIndex(id)) < 0)
    {
        return -1;
    }
    c = &bMenuItemTable[i];
    if(s)
    {
        c->visible = 1;
    }
    else
    {
        c->visible = 0;
    }
    return 0;
}

uint32_t bMenuCurrentID()
{
    return Pre_ID; 
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


