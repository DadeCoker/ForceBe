/**
* @file
* @brief QEP-nano implementation.
* @ingroup qep
* @cond
******************************************************************************
* Last updated for version 6.6.0
* Last updated on  2019-10-04
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

Q_DEFINE_THIS_MODULE("qepn")

/****************************************************************************/
char_t const Q_ROM QP_versionStr[7] = QP_VERSION_STR;

/****************************************************************************/
/****************************************************************************/
/*! empty signal for internal use only */
#define QEP_EMPTY_SIG_        ((QSignal)0)

/*! maximum depth of state nesting in HSMs (including the top level),
* must be >= 2
*/
#define QHSM_MAX_NEST_DEPTH_  ((int_fast8_t)5)

/*! helper function to execute a transition chain in HSM */
static int_fast8_t QHsm_tran_(QHsm * const me,
                              QStateHandler path[QHSM_MAX_NEST_DEPTH_]);

/****************************************************************************/
/**
* @description
* Performs the first step of HSM initialization by assigning the initial
* pseudostate to the currently active state of the state machine.
*
* @param[in,out] me      pointer (see @ref oop)
* @param[in]     initial pointer to the top-most initial state-handler
*                        function in the derived state machine
* @note
* Must be called only by the constructors of the derived state machines.
*
* @note
* Must be called only ONCE before QHSM_INIT().
*
* @usage
* The following example illustrates how to invoke QHsm_ctor() in the
* "constructor" of a derived state machine:
* @include qepn_qhsm_ctor.c
*/
void QHsm_ctor(QHsm * const me, QStateHandler initial) {
    static QHsmVtable const vtable = { /* QHsm virtual table */
        &QHsm_init_,
        &QHsm_dispatch_
    };
    me->vptr  = &vtable;
    me->state = Q_STATE_CAST(&QHsm_top);
    me->temp  = initial;
}

/****************************************************************************/
/**
* @description
* Executes the top-most initial transition in a HSM.
*
* @param[in,out] me pointer (see @ref oop)
*
* @note
* Must be called only ONCE after the QHsm_ctor().
*/
void QHsm_init_(QHsm * const me) {
    QStateHandler t = me->state;
    QState r;

    /** @pre the virtual pointer must be initialized, the top-most initial
    * transition must be initialized, and the initial transition must not
    * be taken yet.
    */
    Q_REQUIRE_ID(200, (me->vptr != (QHsmVtable const *)0)
                      && (me->temp != Q_STATE_CAST(0))
                      && (t == Q_STATE_CAST(&QHsm_top)));

    r = (*me->temp)(me); /* execute the top-most initial transition */

    /* the top-most initial transition must be taken */
    Q_ASSERT_ID(210, r == (QState)Q_RET_TRAN);

    /* drill down into the state hierarchy with initial transitions... */
    do {
        QStateHandler path[QHSM_MAX_NEST_DEPTH_];
        int_fast8_t ip = (int_fast8_t)0; /* transition entry path index */

        path[0] = me->temp;
        Q_SIG(me) = (QSignal)QEP_EMPTY_SIG_;
        (void)(*me->temp)(me);
        while (me->temp != t) {
            ++ip;
            Q_ASSERT_ID(220, ip < (int_fast8_t)Q_DIM(path));
            path[ip] = me->temp;
            (void)(*me->temp)(me);
        }
        me->temp = path[0];

        /* retrace the entry path in reverse (desired) order... */
        Q_SIG(me) = (QSignal)Q_ENTRY_SIG;
        do {
            (void)(*path[ip])(me); /* enter path[ip] */
            --ip;
        } while (ip >= (int_fast8_t)0);

        t = path[0]; /* current state becomes the new source */

        Q_SIG(me) = (QSignal)Q_INIT_SIG;
        r = (*t)(me);
    } while (r == (QState)Q_RET_TRAN);

    me->state = t; /* change the current active state */
    me->temp  = t; /* mark the configuration as stable */
}

/****************************************************************************/
/**
* @description
* QHsm_top() is the ultimate root of state hierarchy in all HSMs derived
* from ::QHsm.
*
* @param[in] me pointer (see @ref oop)
*
* @returns
* Always returns #Q_RET_IGNORED, which means that the top state ignores
* all events.
*
* @note
* The parameter @p me to this state handler is not used. It is provided for
* conformance with the state-handler function signature ::QStateHandler.
*/
QState QHsm_top(void const * const me) {
    (void)me; /* suppress the "unused parameter" compiler warning */
    return (QState)Q_RET_IGNORED; /* the top state ignores all events */
}

/****************************************************************************/
/**
* @description
* Dispatches an event for processing to a hierarchical state machine (HSM).
* The processing of an event represents one run-to-completion (RTC) step.
*
* @param[in,out] me pointer (see @ref oop)
*
* @note
* This function should be called only via the virtual table (see
* QHSM_DISPATCH()) and should NOT be called directly in the applications.
*/
void QHsm_dispatch_(QHsm * const me) {
    QStateHandler t = me->state;
    QStateHandler s;
    QState r;
    int_fast8_t iq; /* helper transition entry path index */

    /** @pre the current state must be initialized and
    * the state configuration must be stable
    */
    Q_REQUIRE_ID(400, (t != Q_STATE_CAST(0))
                      && (t == me->temp));

    /* process the event hierarchically... */
    do {
        s = me->temp;
        r = (*s)(me); /* invoke state handler s */

        if (r == (QState)Q_RET_UNHANDLED) { /* unhandled due to a guard? */
            iq = (int_fast8_t)Q_SIG(me); /* save the original signal */
            Q_SIG(me) = (QSignal)QEP_EMPTY_SIG_; /* find the superstate */
            r = (*s)(me); /* invoke state handler s */
            Q_SIG(me) = (QSignal)iq; /* restore the original signal */
        }
    } while (r == (QState)Q_RET_SUPER);

    /* transition taken? */
    if (r >= (QState)Q_RET_TRAN) {
        QStateHandler path[QHSM_MAX_NEST_DEPTH_]; /* transition entry path */
        int_fast8_t ip; /* transition entry path index */

        path[0] = me->temp; /* save the target of the transition */
        path[1] = t;
        path[2] = s;

        /* exit current state to transition source s... */
        for (; t != s; t = me->temp) {
            Q_SIG(me) = (QSignal)Q_EXIT_SIG; /* find superstate of t */

            /* take the exit action and check if it was handled? */
            if ((*t)(me) == (QState)Q_RET_HANDLED) {
                Q_SIG(me) = (QSignal)QEP_EMPTY_SIG_;
                (void)(*t)(me); /* find superstate of t */
            }
        }

        ip = QHsm_tran_(me, path); /* take the state transition */

        /* retrace the entry path in reverse (desired) order... */
        Q_SIG(me) = (QSignal)Q_ENTRY_SIG;
        for (; ip >= (int_fast8_t)0; --ip) {
            (void)(*path[ip])(me); /* enter path[ip] */
        }
        t = path[0];      /* stick the target into register */
        me->temp = t; /* update the current state */

        /* drill into the target hierarchy... */
        Q_SIG(me) = (QSignal)Q_INIT_SIG;
        while ((*t)(me) == (QState)Q_RET_TRAN) {
            ip = (int_fast8_t)0;

            path[0] = me->temp;
            Q_SIG(me) = (QSignal)QEP_EMPTY_SIG_;
            (void)(*me->temp)(me); /* find the superstate */
            while (me->temp != t) {
                ++ip;
                path[ip] = me->temp;
                (void)(*me->temp)(me); /* find the superstate */
            }
            me->temp = path[0];

            /* entry path must not overflow */
            Q_ASSERT_ID(410, ip < QHSM_MAX_NEST_DEPTH_);

            /* retrace the entry path in reverse (correct) order... */
            Q_SIG(me) = (QSignal)Q_ENTRY_SIG;
            do {
                (void)(*path[ip])(me); /* enter path[ip] */
                --ip;
            } while (ip >= (int_fast8_t)0);

            t = path[0];
            Q_SIG(me) = (QSignal)Q_INIT_SIG;
        }
    }

    me->state = t; /* change the current active state */
    me->temp  = t; /* mark the configuration as stable */
}

/****************************************************************************/
/**
* @description
* Static helper function to execute transition sequence in a hierarchical
* state machine (HSM).
*
* @param[in,out] me   pointer (see @ref oop)
* @param[in,out] path array of pointers to state-handler functions
*                     to execute the entry actions
* @returns
* the depth of the entry path stored in the @p path parameter.
*/
static int_fast8_t QHsm_tran_(QHsm * const me,
                              QStateHandler path[QHSM_MAX_NEST_DEPTH_])
{
    int_fast8_t ip = (int_fast8_t)(-1); /* transition entry path index */
    int_fast8_t iq; /* helper transition entry path index */
    QStateHandler t = path[0];
    QStateHandler s = path[2];
    QState r;

    /* (a) check source==target (transition to self) */
    if (s == t) {
        Q_SIG(me) = (QSignal)Q_EXIT_SIG;
        (void)(*s)(me);      /* exit the source */
        ip = (int_fast8_t)0; /* enter the target */
    }
    else {
        Q_SIG(me) = (QSignal)QEP_EMPTY_SIG_;
        (void)(*t)(me); /* find superstate of target */
        t = me->temp;

        /* (b) check source==target->super */
        if (s == t) {
            ip = (int_fast8_t)0; /* enter the target */
        }
        else {
            Q_SIG(me) = (QSignal)QEP_EMPTY_SIG_;
            (void)(*s)(me); /* find superstate of source */

            /* (c) check source->super==target->super */
            if (me->temp == t) {
                Q_SIG(me) = (QSignal)Q_EXIT_SIG;
                (void)(*s)(me);     /* exit the source */
                ip = (int_fast8_t)0; /* enter the target */
            }
            else {
                /* (d) check source->super==target */
                if (me->temp == path[0]) {
                    Q_SIG(me) = (QSignal)Q_EXIT_SIG;
                    (void)(*s)(me); /* exit the source */
                }
                else {
                    /* (e) check rest of source==target->super->super..
                    * and store the entry path along the way
                    */
                    iq = (int_fast8_t)0; /* indicate that LCA not found */
                    ip = (int_fast8_t)1; /* enter target and its superstate */
                    path[1] = t; /* save the superstate of target */
                    t = me->temp; /* save source->super */

                    /* find target->super->super... */
                    Q_SIG(me) = (QSignal)QEP_EMPTY_SIG_;
                    r = (*path[1])(me);
                    while (r == (QState)Q_RET_SUPER) {
                        ++ip;
                        path[ip] = me->temp; /* store the entry path */
                        if (me->temp == s) { /* is it the source? */
                            iq = (int_fast8_t)1; /* indicate that LCA found */

                            /* entry path must not overflow */
                            Q_ASSERT_ID(510, ip < QHSM_MAX_NEST_DEPTH_);
                            --ip; /* do not enter the source */
                            r = (QState)Q_RET_HANDLED; /* terminate loop */
                        }
                        /* it is not the source, keep going up */
                        else {
                            r = (*me->temp)(me); /* superstate of t */
                        }
                    }

                    /* the LCA not found yet? */
                    if (iq == (int_fast8_t)0) {

                        /* entry path must not overflow */
                        Q_ASSERT_ID(520, ip < QHSM_MAX_NEST_DEPTH_);

                        Q_SIG(me) = (QSignal)Q_EXIT_SIG;
                        (void)(*s)(me); /* exit the source */

                        /* (f) check the rest of source->super
                        *                  == target->super->super...
                        */
                        iq = ip;
                        r = (QState)Q_RET_IGNORED; /* LCA NOT found */
                        do {
                            if (t == path[iq]) { /* is this the LCA? */
                                r = (QState)Q_RET_HANDLED; /* LCA found */

                                /* do not enter LCA */
                                ip = (int_fast8_t)(iq - (int_fast8_t)1);
                                iq = (int_fast8_t)(-1); /* terminate loop */
                            }
                            else {
                                --iq; /* try lower superstate of target */
                            }
                        } while (iq >= (int_fast8_t)0);

                        /* LCA not found? */
                        if (r != (QState)Q_RET_HANDLED) {
                            /* (g) check each source->super->...
                            * for each target->super...
                            */
                            r = (QState)Q_RET_IGNORED; /* keep looping */
                            do {
                                /* exit t unhandled? */
                                Q_SIG(me) = (QSignal)Q_EXIT_SIG;
                                if ((*t)(me) == (QState)Q_RET_HANDLED) {
                                    Q_SIG(me) = (QSignal)QEP_EMPTY_SIG_;
                                    (void)(*t)(me); /* find super of t */
                                }
                                t = me->temp; /* set to super of t */
                                iq = ip;
                                do {
                                    /* is this LCA? */
                                    if (t == path[iq]) {
                                        /* do not enter LCA */
                                        ip = (int_fast8_t)(iq-(int_fast8_t)1);
                                        /* cause breaking out of inner loop */
                                        iq = (int_fast8_t)(-1);
                                        r = (QState)Q_RET_HANDLED; /* break */
                                    }
                                    else {
                                        --iq;
                                    }
                                } while (iq >= (int_fast8_t)0);
                            } while (r != (QState)Q_RET_HANDLED);
                        }
                    }
                }
            }
        }
    }
    return ip;
}

/****************************************************************************/
/**
* @description
* Finds the child state of the given @c parent, such that this child state
* is an ancestor of the currently active state. The main purpose of this
* function is to support **shallow history** transitions in state machines
* derived from QHsm.
*
* @param[in] me     pointer (see @ref oop)
* @param[in] parent pointer to the state-handler function
*
* @returns
* the child of a given @c parent state, which is an ancestor of the current
* active state. For the corner case when the currently active state is the
* given @c parent state, function returns the @c parent state.
*
* @note
* This function is designed to be called during state transitions, so it
* does not necessarily start in a stable state configuration. However, the
* function establishes stable state configuration upon exit.
*
* @sa
* QHsm_childState()
*/
QStateHandler QHsm_childState_(QHsm * const me,
                               QStateHandler const parent)
{
    QStateHandler child = me->state; /* start with the current state */
    bool isFound = false; /* start with the child not found */
    QState r;

    /* establish stable state configuration */
    me->temp = me->state;
    do {
        /* is this the parent of the current child? */
        if (me->temp == parent) {
            isFound = true; /* child is found */
            r = (QState)Q_RET_IGNORED; /* break out of the loop */
        }
        else {
            child = me->temp;
            Q_SIG(me) = (QSignal)QEP_EMPTY_SIG_;
            r = (*me->temp)(me); /* find the superstate */
        }
    } while (r != (QState)Q_RET_IGNORED); /* QHsm_top() state not reached */
    me->temp = me->state; /* establish stable state configuration */

    /** @post the child must be found */
    Q_ENSURE_ID(810, isFound != false);
#ifdef Q_NASSERT
    (void)isFound; /* avoid compiler warning about unused variable */
#endif

    return child; /* return the child */
}
