/*****************************************************************************
* Product: Simple Blinky example
* Last updated for version 5.4.0
* Last updated on  2015-05-18
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, www.state-machine.com.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Alternatively, this program may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GNU General Public License and are specifically designed for
* licensees interested in retaining the proprietary status of their code.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <www.gnu.org/licenses/>.
*
* Contact information:
* <www.state-machine.com/licensing>
* <info@state-machine.com>
*****************************************************************************/
#include "qpn.h"
#include "blinky.h"
#include "bsp.h"

//Q_DEFINE_THIS_FILE

/*..........................................................................*/
typedef struct BlinkyTag {  /* the Blinky active object */
    QActive super;          /* inherit QActive */
} Blinky;

/* hierarchical state machine ... */
static QState Blinky_initial(Blinky * const me);
static QState Blinky_off    (Blinky * const me);
static QState Blinky_on     (Blinky * const me);

/* Global objects ----------------------------------------------------------*/
Blinky AO_Blinky;   /* the single instance of the Blinky AO */

/*..........................................................................*/
void Blinky_ctor(void) {
    Blinky * const me = &AO_Blinky;
    QActive_ctor(&me->super, Q_STATE_CAST(&Blinky_initial));
}

/* HSM definition ----------------------------------------------------------*/
QState Blinky_initial(Blinky * const me) {
    QActive_armX((QActive *)me, 0U,
                 BSP_TICKS_PER_SEC/2U, BSP_TICKS_PER_SEC/2U);
    return Q_TRAN(&Blinky_off);
}
/*..........................................................................*/
QState Blinky_off(Blinky * const me) {
    QState status;
    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG: {
            BSP_ledOff();
            status = Q_HANDLED();
            break;
        }
        case Q_TIMEOUT_SIG: {
            status = Q_TRAN(&Blinky_on);
            break;
        }
        default: {
            status = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status;
}
/*..........................................................................*/
QState Blinky_on(Blinky * const me) {
    QState status;
    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG: {
            BSP_ledOn();
            status = Q_HANDLED();
            break;
        }
        case Q_TIMEOUT_SIG: {
            status = Q_TRAN(&Blinky_off);
            break;
        }
        default: {
            status = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status;
}
