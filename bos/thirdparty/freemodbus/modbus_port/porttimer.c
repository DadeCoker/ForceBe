/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id$
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- static functions ---------------------------------*/
static void prvvTIMERExpiredISR( void );

/* ----------------------- Start implementation -----------------------------*/
BOOL
xMBPortTimersInit( USHORT usTim1Timerout50us )
{
	
	App_Timer0Cfg(usTim1Timerout50us * (SystemCoreClock/20000)); 
	return TRUE;
}

INLINE void
vMBPortTimersEnable( void )
{
    /* Enable the timer with the timeout passed to xMBPortTimersInit( ) */
	Bt_ClearIntFlag(TIM0,BtUevIrq); //
	Bt_M0_Cnt16Set(TIM0, 0);
	Bt_M0_Run(TIM0);        //TIM0 
}

INLINE void
vMBPortTimersDisable( void )
{
    /* Disable any pending timers. */
	Bt_ClearIntFlag(TIM0,BtUevIrq); //
	Bt_M0_Stop(TIM0);
	Bt_M0_Cnt16Set(TIM0, 0);
}

/* Create an ISR which is called whenever the timer has expired. This function
 * must then call pxMBPortCBTimerExpired( ) to notify the protocol stack that
 * the timer has expired.
 */
static void prvvTIMERExpiredISR( void )
{
    ( void )pxMBPortCBTimerExpired(  );
}

/*******************************************************************************
 * TIM0
 ******************************************************************************/
void Tim0_IRQHandler(void)
{   
    //Timer0
	if(TRUE == Bt_GetIntFlag(TIM0, BtUevIrq))
	{
		prvvTIMERExpiredISR();
		Bt_ClearIntFlag(TIM0,BtUevIrq); //
	}
}

