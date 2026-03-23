/**
* @file
* @brief QV-nano implementation.
* @ingroup qvn
* @cond
******************************************************************************
* Last updated for version 6.6.0
* Last updated on  2019-07-30
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005-2019 Quantum Leaps, LLC. All rights reserved.
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
* along with this program. If not, see <www.gnu.org/licenses>.
*
* Contact information:
* <www.state-machine.com/licensing>
* <info@state-machine.com>
******************************************************************************
* @endcond
*/
#include "qpn_conf.h" /* QP-nano configuration file (from the application) */
#include "qfn_port.h" /* QF-nano port from the port directory */
#include "qassert.h"  /* embedded systems-friendly assertions */

Q_DEFINE_THIS_MODULE("qvn")

/* protection against including this source file in a wrong project */
#ifndef QVN_H
    #error "Source file included in a project NOT based on the QV-nano kernel"
#endif /* QVN_H */

void QF_Ready_run(void)
{
	uint_fast8_t p;
	QActive *a;
#ifdef QF_MAX_ACTIVE /* deprecated constant provided? */
#if (QF_MAX_ACTIVE < 1) || (8 < QF_MAX_ACTIVE)
#error "QF_MAX_ACTIVE not defined or out of range. Valid range is 1..8"
#endif
	QF_maxActive_ = (uint_fast8_t)QF_MAX_ACTIVE;
#else
	/** @pre the number of active objects must be initialized by calling:
	* QF_init(Q_DIM(QF_active));
	*/
	Q_REQUIRE_ID(100, ((uint_fast8_t)1 <= QF_maxActive_)
		&& (QF_maxActive_ <= (uint_fast8_t)8));
#endif

	/* set priorities all registered active objects... */
	for (p = (uint_fast8_t)1; p <= QF_maxActive_; ++p) {
		a = QF_ROM_ACTIVE_GET_(p);

		/* QF_active[p] must be initialized */
		Q_ASSERT_ID(810, a != (QActive *)0);

		a->prio = (uint8_t)p; /* set the priority of the active object */
	}

	/* trigger initial transitions in all registered active objects... */
	for (p = (uint_fast8_t)1; p <= QF_maxActive_; ++p) {
		a = QF_ROM_ACTIVE_GET_(p);
		QHSM_INIT(&a->super); /* take the initial transition in the SM */
	}

	QF_onStartup(); /* invoke startup callback */

}

/****************************************************************************/
/**
* @description
* QF_run() is typically called from your startup code after you initialize
* the QF and start at least one active object with QActive_start().
* This implementation of QF_run() is for the cooperative Vanilla kernel.
*
* @returns QF_run() typically does not return in embedded applications.
* However, when QP runs on top of an operating system,  QF_run() might
* return and in this case the return represents the error code (0 for
* success). Typically the value returned from QF_run() is subsequently
* passed on as return from main().
*/
int_t QF_run(void) {
    uint_fast8_t p;
    QActive *a;

    /* the event loop of the cooperative QV-nano kernel... */

    if (QF_readySet_ != (uint_fast8_t)0) {
        QActiveCB const Q_ROM *acb;

#ifdef QF_LOG2
        p = QF_LOG2(QF_readySet_);
#else
        /* hi nibble non-zero? */
        if ((QF_readySet_ & (uint_fast8_t)0xF0) != (uint_fast8_t)0) {
            p = (uint_fast8_t)(
                    (uint_fast8_t)Q_ROM_BYTE(QF_log2Lkup[QF_readySet_ >> 4])
                    + (uint_fast8_t)4);
        }
        else { /* hi nibble of QF_readySet_ is zero */
            p = (uint_fast8_t)Q_ROM_BYTE(QF_log2Lkup[QF_readySet_]);
        }
#endif /* QF_LOG2 */

        acb = &QF_active[p];
        a = QF_ROM_ACTIVE_GET_(p);

        /* some unsuded events must be available */
        Q_ASSERT_ID(820, a->nUsed > (uint8_t)0);

        --a->nUsed;
        Q_SIG(a) = QF_ROM_QUEUE_AT_(acb, a->tail).sig;
#if (Q_PARAM_SIZE != 0)
        Q_PAR(a) = QF_ROM_QUEUE_AT_(acb, a->tail).par;
#endif
        if (a->tail == (uint8_t)0) { /* wrap around? */
            a->tail = Q_ROM_BYTE(acb->qlen);
        }
        --a->tail;
        QF_INT_ENABLE();

        QHSM_DISPATCH(&a->super); /* dispatch to the HSM (RTC step) */

        QF_INT_DISABLE();
        /* empty queue? */
        if (a->nUsed == (uint8_t)0) {
            /* clear the bit corresponding to 'p' */
            QF_readySet_ &=
                (uint_fast8_t)~((uint_fast8_t)1 << (p - (uint_fast8_t)1));
        }
    }
    else {
        /* QV_onIdle() must be called with interrupts DISABLED because
        * the determination of the idle condition (no events in the
        * queues) can change at any time by an interrupt posting events
        * to a queue. QV_onIdle() MUST enable interrupts internally,
        * perhaps at the same time as putting the CPU into a power-saving
        * mode.
        */
        QV_onIdle();

        QF_INT_DISABLE();
    }
    return (int_t)0;
}

