/**
* @file
* @brief QF-nano implementation.
* @ingroup qfn
* @cond
******************************************************************************
* Last updated for version 6.7.0
* Last updated on  2019-12-30
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

Q_DEFINE_THIS_MODULE("qfn")

/* Global-scope objects *****************************************************/

/**
* @description
* This variable stores the number of active objects in the application.
* This is the number of elements (dimension of) the QF_active[] array.
*/
uint_fast8_t QF_maxActive_;

/**
* @description
* The QF-nano ready set keeps track of active objects that are ready to run.
* The ready set represents each active object as a bit, with the bits
* assigned according to priorities of the active objects. The bit is set
* if the corresponding active object is ready to run (i.e., has one or
* more events in its event queue) and zero if the event queue is empty.
* The QF-nano ready set is one byte-wide, which corresponds to 8 active
* objects maximum.
*/
uint_fast8_t volatile QF_readySet_;

#ifdef QF_TIMEEVT_USAGE
/**
* @description
* The QF-nano timer set keeps track of the armed time events. The timer set
* represents the timeout down-counter of each active object as a bit, with
* the bits assigned according to priorities of the active objects. The bit
* is set if the corresponding timeout down-counter is not zero (i.e., is
* counting down) and zero if the down-counter is zero. The QF-nano time event
* set is one byte-wide, which corresponds to 8 active objects maximum.@n
* @n
* The main use of the QF_timerSetX_ is to quickly determine that all time
* events are disarmed by testing (QF_timerSetX_[tickRate] == 0).
* If so, the CPU can go to longer sleep mode, in which the system clock
* tick ISR is turned off.
*
* @note The test (QF_timerSet_[tickRate] == 0) must be always performed
* inside a CRITICAL SECTION.
*/
uint_fast8_t volatile QF_timerSetX_[QF_MAX_TICK_RATE];
#endif

#ifndef QF_LOG2
uint8_t const Q_ROM QF_log2Lkup[16] = {
    (uint8_t)0, (uint8_t)1, (uint8_t)2, (uint8_t)2,
    (uint8_t)3, (uint8_t)3, (uint8_t)3, (uint8_t)3,
    (uint8_t)4, (uint8_t)4, (uint8_t)4, (uint8_t)4,
    (uint8_t)4, (uint8_t)4, (uint8_t)4, (uint8_t)4
};
#endif /* QF_LOG2 */

/****************************************************************************/
void QActive_ctor(QActive * const me, QStateHandler initial) {
    static QActiveVtable const vtable = { /* QActive virtual table */
        { &QHsm_init_,
          &QHsm_dispatch_ },
        &QActive_postX_,
        &QActive_postXISR_
    };

    /**
    * @note QActive inherits QActive, so by the @ref oop convention
    * it should call the constructor of the superclass, i.e., QActive_ctor().
    * However, this would pull in the QActiveVtable, which in turn will pull
    * in the code for QHsm_init_() and QHsm_dispatch_() implemetations,
    * which is expensive. To avoid this code size penalty, in case ::QHsm is
    * not used in a given project, the call to QHsm_ctor() avoids pulling
    * in the code for QHsm.
    */
    QHsm_ctor(&me->super, initial);
    me->super.vptr = &vtable.super; /* hook the vptr to QActive vtable */
}

/****************************************************************************/
/**
* @description
* Direct event posting is the simplest asynchronous communication method
* available in QF-nano.
*
* @attention
* This function should be called only via the macro QACTIVE_POST()
* or QACTIVE_POST_X(). This function should be only used in the
* __task__ context.
*
* @param[in,out] me     pointer (see @ref oop)
* @param[in]     margin number of required free slots in the queue after
*                       posting the event. The special value #QF_NO_MARGIN
*                       means that this function will assert if posting fails.
* @param[in]     sig    signal of the event to be posted
* @param[in]     par    parameter of the event to be posted
*
* @usage
* @include qfn_postx.c
*/
#if (Q_PARAM_SIZE != 0)
bool QActive_postX_(QActive * const me, uint_fast8_t margin,
                    enum_t const sig, QParam const par)
#else
bool QActive_postX_(QActive * const me, uint_fast8_t margin,
                    enum_t const sig)
#endif
{
    QActiveCB const Q_ROM *acb = &QF_active[me->prio];
    uint_fast8_t qlen = (uint_fast8_t)Q_ROM_BYTE(acb->qlen);
    bool can_post;

    QF_INT_DISABLE();

    if (margin == QF_NO_MARGIN) {
        if (qlen > (uint_fast8_t)me->nUsed) {
            can_post = true; /* can post */
        }
        else {
            can_post = false; /* cannot post */
#ifndef Q_NASSERT
            QF_INT_ENABLE();
            /* must be able to post event : Q_ERROR_ID(310) */
            Q_onAssert(Q_this_module_, (int_t)310);
#endif
        }
    }
    else if ((qlen - (uint_fast8_t)me->nUsed) > margin) {
        can_post = true; /* can post */
    }
    else {
        can_post = false; /* cannot post */
    }

    if (can_post) { /* can post the event? */
        /* insert event into the ring buffer (FIFO) */
        QF_ROM_QUEUE_AT_(acb, me->head).sig = (QSignal)sig;
#if (Q_PARAM_SIZE != 0)
        QF_ROM_QUEUE_AT_(acb, me->head).par = par;
#endif
        if (me->head == (uint8_t)0) {
            me->head = (uint8_t)qlen; /* wrap the head */
        }
        --me->head;
        ++me->nUsed;

        /* is this the first event? */
        if (me->nUsed == (uint8_t)1) {

            /* set the corresponding bit in the ready set */
            QF_readySet_ |= (uint_fast8_t)
                ((uint_fast8_t)1 << (me->prio - (uint8_t)1));

#ifdef qkn_h
            if (QK_sched_() != (uint_fast8_t)0) {
                QK_activate_(); /* activate the next active object */
            }
#endif
        }
    }
    QF_INT_ENABLE();

    return can_post;
}

/****************************************************************************/
/**
* @description
* Direct event posting is the simplest asynchronous communication method
* available in QF-nano.
*
* @attention
* This function should be called only via the macro QACTIVE_POST_ISR()
* or QACTIVE_POST_X_ISR(). This function should be only used in the
* __ISR__ context.
*
* @param[in,out] me     pointer (see @ref oop)
* @param[in]     margin number of required free slots in the queue after
*                       posting the event. The special value #QF_NO_MARGIN
*                       means that this function will assert if posting fails.
* @param[in]     sig    signal of the event to be posted
* @param[in]     par    parameter of the event to be posted
*
* @usage
* @include qfn_postx.c
*/
#if (Q_PARAM_SIZE != 0)
bool QActive_postXISR_(QActive * const me, uint_fast8_t margin,
                       enum_t const sig, QParam const par)
#else
bool QActive_postXISR_(QActive * const me, uint_fast8_t margin,
                       enum_t const sig)
#endif
{
#ifdef QF_ISR_NEST
#ifdef QF_ISR_STAT_TYPE
    QF_ISR_STAT_TYPE stat;
#endif
#endif
    QActiveCB const Q_ROM *acb = &QF_active[me->prio];
    uint_fast8_t qlen = (uint_fast8_t)Q_ROM_BYTE(acb->qlen);
    bool can_post;

#ifdef QF_ISR_NEST
#ifdef QF_ISR_STAT_TYPE
    QF_ISR_DISABLE(stat);
#else
    QF_INT_DISABLE();
#endif
#endif

    if (margin == QF_NO_MARGIN) {
        if (qlen > (uint_fast8_t)me->nUsed) {
            can_post = true; /* can post */
        }
        else {
            can_post = false; /* cannot post */
#ifndef Q_NASSERT
            QF_INT_ENABLE();
            /* must be able to post event : Q_ERROR_ID(410) */
            Q_onAssert(Q_this_module_, (int_t)410);
#endif
        }
    }
    else if ((qlen - (uint_fast8_t)me->nUsed) > margin) {
        can_post = true; /* can post */
    }
    else {
        can_post = false; /* cannot post */
    }

    if (can_post) { /* can post the event? */
        /* insert event into the ring buffer (FIFO) */
        QF_ROM_QUEUE_AT_(acb, me->head).sig = (QSignal)sig;
#if (Q_PARAM_SIZE != 0)
        QF_ROM_QUEUE_AT_(acb, me->head).par = par;
#endif
        if (me->head == (uint8_t)0) {
            me->head = (uint8_t)qlen; /* wrap the head */
        }
        --me->head;
        ++me->nUsed;
        /* is this the first event? */
        if (me->nUsed == (uint8_t)1) {
            /* set the bit */
            QF_readySet_ |= (uint_fast8_t)
                ((uint_fast8_t)1 << (me->prio - (uint8_t)1));
        }
    }

#ifdef QF_ISR_NEST
#ifdef QF_ISR_STAT_TYPE
    QF_ISR_RESTORE(stat);
#else
    QF_INT_ENABLE();
#endif
#endif

    return can_post;
}

/****************************************************************************/
/**
* @description
* The function QF_init() initializes the number of active objects to be
* managed by the framework and clears the internal QF-nano variables as well
* as all registered active objects to zero, which is needed in case when
* the startup code does not clear the uninitialized data (in violation of
* the C Standard).
*
* @note
* The intended use of the function is to call as follows:
* QF_init(Q_DIM(QF_active));
*/
void QF_init(uint_fast8_t maxActive) {
    QActive *a;
    uint_fast8_t p;
    uint_fast8_t n;

    /** @pre the number of active objects must be in range */
    Q_REQUIRE_ID(100, ((uint_fast8_t)1 < maxActive)
                      && (maxActive <= (uint_fast8_t)9));
    QF_maxActive_ = (uint_fast8_t)maxActive - (uint_fast8_t)1;

#ifdef QF_TIMEEVT_USAGE
    for (n = (uint_fast8_t)0; n < (uint_fast8_t)QF_MAX_TICK_RATE; ++n) {
        QF_timerSetX_[n] = (uint_fast8_t)0;
    }
#endif /* QF_TIMEEVT_USAGE */

    QF_readySet_ = (uint_fast8_t)0;

#ifdef qkn_h
    QK_attr_.actPrio = (uint_fast8_t)8; /* QK-nano scheduler locked */

#ifdef QF_ISR_NEST
    QK_attr_.intNest = (uint_fast8_t)0;
#endif

#ifdef QK_SCHED_LOCK
    QK_attr_.lockPrio   = (uint_fast8_t)0;
    QK_attr_.lockHolder = (uint_fast8_t)0;
#endif

#endif /* #ifdef qkn_h */

    /* clear all registered active objects... */
    for (p = (uint_fast8_t)1; p <= QF_maxActive_; ++p) {
        a = QF_ROM_ACTIVE_GET_(p);

        /* QF_active[p] must be initialized */
        Q_ASSERT_ID(110, a != (QActive *)0);

        a->head    = (uint8_t)0;
        a->tail    = (uint8_t)0;
        a->nUsed   = (uint8_t)0;
#if (QF_TIMEEVT_CTR_SIZE != 0)
        for (n = (uint_fast8_t)0; n < (uint_fast8_t)QF_MAX_TICK_RATE; ++n) {
            a->tickCtr[n].nTicks   = (QTimeEvtCtr)0;
#ifdef QF_TIMEEVT_PERIODIC
            a->tickCtr[n].interval = (QTimeEvtCtr)0;
#endif /* def QF_TIMEEVT_PERIODIC */
        }
#endif /* (QF_TIMEEVT_CTR_SIZE != 0) */
    }

#ifdef QV_INIT /* initialization of the QV-nano kernel defined? */
    QV_INIT(); /* port-specific initialization of the QV-nano kernel */
#elif defined QK_INIT /* initialization of the QK-nano kernel defined? */
    QK_INIT(); /* port-specific initialization of the QK-nano kernel */
#endif
}

/****************************************************************************/
/****************************************************************************/
#if (QF_TIMEEVT_CTR_SIZE != 0)

/****************************************************************************/
/**
* @description
* This function must be called periodically from a time-tick ISR or from
* an ISR so that QF-nano can manage the timeout events assigned to the given
* system clock tick rate.
*
* @param[in]  tickRate  system clock tick rate serviced in this call.
*
* @note Each system tick rate posts timeout events with a different signal
* as follows:@n
* tickRate==0  Q_TIMEOUT_SIG@n
* tickRate==1  Q_TIMEOUT1_SIG@n
* tickRate==2  Q_TIMEOUT2_SIG@n
* tickRate==3  Q_TIMEOUT3_SIG
*
* @note The calls to QF_tickXISR() with different tick rate parameter can
* preempt each other. For example, higher clock tick rates might be serviced
* from interrupts that can preempt lower-priority interrupts.
*/
void QF_tickXISR(uint_fast8_t const tickRate) {
    uint_fast8_t p = QF_maxActive_;
    do {
        QActive *a = QF_ROM_ACTIVE_GET_(p);
        QTimer *t = &a->tickCtr[tickRate];

        if (t->nTicks != (QTimeEvtCtr)0) {
            --t->nTicks;
            if (t->nTicks == (QTimeEvtCtr)0) {

#ifdef QF_TIMEEVT_PERIODIC
                if (t->interval != (QTimeEvtCtr)0) {
                    t->nTicks = t->interval; /* re-arm the periodic timer */
                }
#endif /* QF_TIMEEVT_PERIODIC */

#ifdef QF_TIMEEVT_USAGE
                QF_timerSetX_[tickRate] &= (uint_fast8_t)
                    ~((uint_fast8_t)1 << (p - (uint_fast8_t)1));
#endif /* QF_TIMEEVT_USAGE */

#if (Q_PARAM_SIZE != 0)
                QACTIVE_POST_ISR(a, (enum_t)Q_TIMEOUT_SIG + (enum_t)tickRate,
                                 (QParam)0);
#else
                QACTIVE_POST_ISR(a, (enum_t)Q_TIMEOUT_SIG + (enum_t)tickRate);
#endif /* (Q_PARAM_SIZE != 0) */
            }
        }
        --p;
    } while (p != (uint_fast8_t)0);
}

/****************************************************************************/
/**
* @description
* Arms a time event to fire in a specified number of clock ticks at the
* specified tick rate. The timeout signal gets directly posted (using the
* FIFO policy) into the event queue of the active object calling this
* function.
*
* @param[in,out] me       pointer (see @ref oop)
* @param[in]     tickRate tick rate .
* @param[in]     nTicks   number of clock ticks (at the associated rate)
*                         to rearm the time event with.
*
* @note Each system tick rate posts timeout events with a different signal
* as follows:@n
* tickRate==0  Q_TIMEOUT_SIG@n
* tickRate==1  Q_TIMEOUT1_SIG@n
* tickRate==2  Q_TIMEOUT2_SIG@n
* tickRate==3  Q_TIMEOUT3_SIG
*
* @note After posting, a one-shot time event gets automatically disarmed.
*
* @note A time event can be disarmed at any time by calling the
* QActive_disarmX() function.
*
* @usage
* The following example shows how to arm a time event from a state
* machine of an active object:
* @include qfn_armx.c
*/
#ifdef QF_TIMEEVT_PERIODIC
void QActive_armX(QActive * const me, uint_fast8_t const tickRate,
                  QTimeEvtCtr const nTicks, QTimeEvtCtr const interval)
#else
void QActive_armX(QActive * const me, uint_fast8_t const tickRate,
                  QTimeEvtCtr const nTicks)
#endif
{
    QF_INT_DISABLE();
    me->tickCtr[tickRate].nTicks = nTicks;
#ifdef QF_TIMEEVT_PERIODIC
    me->tickCtr[tickRate].interval = interval;
#endif /* QF_TIMEEVT_PERIODIC */

#ifdef QF_TIMEEVT_USAGE
    /* set a bit in QF_timerSetX_[] to rememer that the timer is running */
    QF_timerSetX_[tickRate] |= (uint_fast8_t)
        ((uint_fast8_t)1 << (me->prio - (uint8_t)1));
#endif
    QF_INT_ENABLE();
}

/****************************************************************************/
/**
* @description
* The time event of the active object gets disarmed (stopped).
*
* @param[in,out] me       pointer (see @ref oop)
* @param[in]     tickRate tick rate
*
* @note You should __not__ assume that the timeout event will not
* arrive after you disarm the time event. The timeout event could be
* already in the event queue.
*/
void QActive_disarmX(QActive * const me, uint_fast8_t const tickRate) {
    QF_INT_DISABLE();
    me->tickCtr[tickRate].nTicks = (QTimeEvtCtr)0;
#ifdef QF_TIMEEVT_PERIODIC
    me->tickCtr[tickRate].interval = (QTimeEvtCtr)0;
#endif /* QF_TIMEEVT_PERIODIC */

#ifdef QF_TIMEEVT_USAGE
    /* clear a bit in QF_timerSetX_[] to rememer that timer is not running */
    QF_timerSetX_[tickRate] &= (uint_fast8_t)
        ~((uint_fast8_t)1 << (me->prio - (uint8_t)1));
#endif
    QF_INT_ENABLE();
}
#endif /* #if (QF_TIMEEVT_CTR_SIZE != 0) */
