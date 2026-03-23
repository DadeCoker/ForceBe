/**
* @file
* @brief Public QF-nano interface.
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
#ifndef QFN_H
#define QFN_H

/**
* @description
* This header file must be included in all modules that use QP-nano.
* Typically, this header file is included indirectly through the
* header file qpn_port.h.
*/

/****************************************************************************/
#ifndef QF_TIMEEVT_CTR_SIZE
    /*! macro to override the default QTimeEvtCtr size.
    * Valid values 0, 1, 2, or 4; default 0
    */
    #define QF_TIMEEVT_CTR_SIZE 0
#endif
#if (QF_TIMEEVT_CTR_SIZE == 0)
    /* no time events */
#elif (QF_TIMEEVT_CTR_SIZE == 1)
    typedef uint8_t QTimeEvtCtr;
#elif (QF_TIMEEVT_CTR_SIZE == 2)
    /*! type of the Time Event counter, which determines the dynamic
    * range of the time delays measured in clock ticks.
    */
    /**
    * @description
    * This typedef is configurable via the preprocessor switch
    * #QF_TIMEEVT_CTR_SIZE. The other possible values of this type are
    * as follows: @n
    * none when (QF_TIMEEVT_CTR_SIZE not defined or == 0), @n
    * uint8_t  when (QF_TIMEEVT_CTR_SIZE == 1); @n
    * uint16_t when (QF_TIMEEVT_CTR_SIZE == 2); and @n
    * uint32_t when (QF_TIMEEVT_CTR_SIZE == 4).
    */
    typedef uint16_t QTimeEvtCtr;
#elif (QF_TIMEEVT_CTR_SIZE == 4)
    typedef uint32_t QTimeEvtCtr;
#else
    #error "QF_TIMER_SIZE defined incorrectly, expected 1, 2, or 4"
#endif

#if (QF_TIMEEVT_CTR_SIZE != 0)
    /*! Timer structure the active objects */
    typedef struct {
        QTimeEvtCtr nTicks;   /*!< timer tick counter */
#ifdef QF_TIMEEVT_PERIODIC
        QTimeEvtCtr interval; /*!< timer interval */
#endif /* QF_TIMEEVT_PERIODIC */
    } QTimer;
#endif /* (QF_TIMEEVT_CTR_SIZE != 0) */

#ifndef QF_MAX_TICK_RATE
    /*! Default value of the macro configurable value in qpn_port.h */
    #define QF_MAX_TICK_RATE     1
#elif (QF_MAX_TICK_RATE > 4)
    #error "QF_MAX_TICK_RATE exceeds the 4 limit"
#endif

/****************************************************************************/
/*! QActive active object (based on QHsm-implementation) */
/**
* @description
* QActive is the base structure for derivation of active objects. Active
* objects in QF-nano are encapsulated tasks (each embedding a state machine
* and an event queue) that communicate with one another asynchronously by
* sending and receiving events. Within an active object, events are
* processed sequentially in a run-to-completion (RTC) fashion, while QF
* encapsulates all the details of thread-safe event exchange and queuing.
*
* @note ::QActive is not intended to be instantiated directly, but rather
* serves as the base structure for derivation of active objects in the
* application code.
*
* @usage
* The following example illustrates how to derive an active object from
* ::QActive. Please note that the ::QActive member super_ is defined as
* the __first__ member of the derived struct.
* @include qfn_qactive.c
*/
typedef struct QActive {
    QHsm super; /**< derives from the ::QHsm base class */

#if (QF_TIMEEVT_CTR_SIZE != 0)
    /*! Timer for the active object */
    QTimer tickCtr[QF_MAX_TICK_RATE];
#endif /* (QF_TIMEEVT_CTR_SIZE != 0) */

    /*! priority of the active object (1..8) */
    uint8_t prio;

    /*! offset to where next event will be inserted into the buffer */
    uint8_t volatile head;

    /*! offset of where next event will be extracted from the buffer */
    uint8_t volatile tail;

    /*! number of events currently present in the queue
    * (events in the ring buffer + 1 event in the state machine)
    */
    uint8_t volatile nUsed;

} QActive;

/*! Virtual table for the QActive class */
typedef struct {
    QHsmVtable super; /*!< inherits QHsmVtable */

#if (Q_PARAM_SIZE != 0)
    /*! virtual function to asynchronously post (FIFO) an event to an AO
    * (task context).
    */
    /** @sa QACTIVE_POST() and QACTIVE_POST_X() */
    bool (*post)(QActive * const me, uint_fast8_t const margin,
                 enum_t const sig, QParam const par);

    /*! virtual function to asynchronously post (FIFO) an event to an AO
    * (ISR context).
    */
    /** @sa QACTIVE_POST_ISR() and QACTIVE_POST_X_ISR() */
    bool (*postISR)(QActive * const me, uint_fast8_t const margin,
                    enum_t const sig, QParam const par);
#else
    bool (*post)   (QActive * const me, uint_fast8_t const margin,
                    enum_t const sig);
    bool (*postISR)(QActive * const me, uint_fast8_t const margin,
                    enum_t const sig);
#endif
} QActiveVtable;

/*! protected "constructor" of an QActive active object. */
void QActive_ctor(QActive * const me, QStateHandler initial);


/*! special value of margin that causes asserting failure in case
* event posting fails.
*/
#define QF_NO_MARGIN ((uint_fast8_t)0xFF)

#if (Q_PARAM_SIZE != 0)
    /*! Polymorphically posts an event to an active object (FIFO)
    * with delivery guarantee (task context).
    */
    /**
    * @description
    * This macro asserts if the queue overflows and cannot accept the event.
    *
    * @param[in,out] me_   pointer (see @ref oop)
    * @param[in]     sig_  signal of the event to post
    * @param[in]     par_  parameter of the event to post.
    *
    * @sa QACTIVE_POST_X(), QActive_postX_(),
    * QACTIVE_POST_ISR(), QActive_postXISR_().
    *
    * @usage
    * @include qfn_post.c
    */
    #define QACTIVE_POST(me_, sig_, par_) do {                      \
        QActive * const ao_ = QF_ACTIVE_CAST((me_));                \
        ((void)(*((QActiveVtable const *)(ao_->super.vptr))->post)( \
                ao_, QF_NO_MARGIN, (enum_t)(sig_), (QParam)(par_)));\
    } while (false)

    /*! Polymorphically posts an event to an active object (FIFO)
    * without delivery guarantee (task context).
    */
    /**
    * @description
    * This macro does not assert if the queue overflows and cannot accept
    * the event with the specified margin of free slots remaining.
    *
    * @param[in,out] me_     pointer (see @ref oop)
    * @param[in]     margin_ the minimum free slots in the queue, which
    *                must still be available after posting the event.
    *                The special value #QF_NO_MARGIN causes asserting failure
    *                in case event allocation fails.
    * @param[in]     sig_    signal of the event to post
    * @param[in]     par_    parameter of the event to post.
    *
    * @returns
    * 'true' if the posting succeeded, and 'false' if the posting failed
    * due to insufficient margin of free slots available in the queue.
    *
    * @usage
    * @include qfn_postx.c
    */
    #define QACTIVE_POST_X(me_, margin_, sig_, par_)            \
        ((*((QActiveVtable const *)((me_)->super.vptr))->post)( \
             (me_), (margin_), (enum_t)(sig_), (QParam)(par_)))

    /*! Polymorphically posts an event to an active object (FIFO)
    * with delivery guarantee (ISR context).
    */
    /**
    * @description
    * This macro asserts if the queue overflows and cannot accept the event.
    *
    * @param[in,out] me_   pointer (see @ref oop)
    * @param[in]     sig_  signal of the event to post
    * @param[in]     par_  parameter of the event to post.
    *
    * @sa QACTIVE_POST_X(), QActive_postX_().
    *
    * @usage
    * @include qfn_post.c
    */
    #define QACTIVE_POST_ISR(me_, sig_, par_) do {                     \
        QActive * const ao_ = QF_ACTIVE_CAST((me_));                   \
        ((void)(*((QActiveVtable const *)(ao_->super.vptr))->postISR)( \
                ao_, QF_NO_MARGIN, (enum_t)(sig_), (QParam)(par_)));   \
    } while (false)

    /*! Polymorphically posts an event to an active object (FIFO)
    * without delivery guarantee (ISR context).
    */
    /**
    * @description
    * This macro does not assert if the queue overflows and cannot accept
    * the event with the specified margin of free slots remaining.
    *
    * @param[in,out] me_     pointer (see @ref oop)
    * @param[in]     margin_ the minimum free slots in the queue, which
    *                must still be available after posting the event.
    *                The special value #QF_NO_MARGIN causes asserting failure
    *                in case event allocation fails.
    * @param[in]     sig_    signal of the event to post
    * @param[in]     par_    parameter of the event to post.
    *
    * @returns
    * 'true' if the posting succeeded, and 'false' if the posting failed
    * due to insufficient margin of free slots available in the queue.
    *
    * @usage
    * @include qfn_postx.c
    */
    #define QACTIVE_POST_X_ISR(me_, margin_, sig_, par_)   \
        ((*((QActiveVtable const *)(                       \
            QF_ACTIVE_CAST((me_))->super.vptr))->postISR)( \
                QF_ACTIVE_CAST((me_)), (margin_),          \
                (enum_t)(sig_), (QParam)(par_)))

    /*! Implementation of the task-level event posting */
    bool QActive_postX_(QActive * const me, uint_fast8_t margin,
                        enum_t const sig, QParam const par);

    /*! Implementation of the ISR-level event posting */
    bool QActive_postXISR_(QActive * const me, uint_fast8_t margin,
                           enum_t const sig, QParam const par);

#else /* no event parameter */

    #define QACTIVE_POST(me_, sig_) do {                            \
        QActive * const ao_ = QF_ACTIVE_CAST((me_));                \
        ((void)(*((QActiveVtable const *)(ao_->super.vptr))->post)( \
                ao_, QF_NO_MARGIN, (enum_t)(sig_)));                \
    } while (false)

    #define QACTIVE_POST_X(me_, margin_, sig_)                        \
        ((*((QActiveVtable const *)((me_)->super.vptr))->post)((me_), \
                                   (margin_), (sig_)))

    bool QActive_postX_(QActive * const me, uint_fast8_t margin,
                        enum_t const sig);

    #define QACTIVE_POST_ISR(me_, sig_) do {                           \
        QActive * const ao_ = QF_ACTIVE_CAST((me_));                   \
        ((void)(*((QActiveVtable const *)(ao_->super.vptr))->postISR)( \
                ao_, QF_NO_MARGIN, (enum_t)(sig_)));                   \
    } while (false)

    #define QACTIVE_POST_X_ISR(me_, margin_, sig_)                     \
        ((*((QActiveVtable const *)(                                   \
            QF_ACTIVE_CAST((me_))->super.vptr))->postISR)(             \
                QF_ACTIVE_CAST((me_)), (margin_), (enum_t)(sig_)))

    bool QActive_postXISR_(QActive * const me, uint_fast8_t margin,
                           enum_t const sig);
#endif

#if (QF_TIMEEVT_CTR_SIZE != 0)

    /*! Processes all armed time events at every clock tick. */
    void QF_tickXISR(uint_fast8_t const tickRate);

#ifdef QF_TIMEEVT_PERIODIC
    /*! Arm the QP-nano one-shot time event. */
    void QActive_armX(QActive * const me, uint_fast8_t const tickRate,
                      QTimeEvtCtr const nTicks, QTimeEvtCtr const interval);
#else
    /*! Arm the QP-nano one-shot time event. */
    void QActive_armX(QActive * const me, uint_fast8_t const tickRate,
                      QTimeEvtCtr const nTicks);
#endif

    /*! Disarm a time event. Since the tick counter */
    void QActive_disarmX(QActive * const me, uint_fast8_t const tickRate);

#endif /* (QF_TIMEEVT_CTR_SIZE != 0) */


/****************************************************************************/
/* QF-nano protected methods ...*/

/*! QF-nano initialization. */
void QF_init(uint_fast8_t maxActive);

/*! QF-nano termination. */
/**
* @description
* This function terminates QF and performs any necessary cleanup.
* In QF-nano this function is defined in the BSP. Many QF ports might not
* require implementing QF_stop() at all, because many embedded applications
* don't have anything to exit to.
*/
void QF_stop(void);

/*! Startup QF-nano callback. */
/**
* @description
* The time line for calling QF_onStartup() depends on the particular
* QF port. In most cases, QF_onStartup() is called from QF_run(), right
* before starting any multitasking kernel or the background loop.
*
* @sa QF initialization example for ::QActiveCB.
*/
void QF_onStartup(void);

void QF_Ready_run(void);
/*! Transfers control to QF-nano to run the application. */
int_t QF_run(void);


/****************************************************************************/
/*! QActive Control Block
*
* QActiveCB represents the read-only information that the QF-nano needs to
* manage the active object. QActiveCB objects are grouped in the array
* QF_active[], which typically can be placed in ROM.
*
* @usage
* The following example illustrates how to allocate and initialize the
* ::QActive control blocks in the array QF_active[].
* @include qfn_main.c
*/
typedef struct {
    QActive  *act;   /*!< pointer to the active object structure */
    QEvt     *queue; /*!< pointer to the event queue buffer */
    uint8_t   qlen;  /*!< the length of the queue ring buffer */
} QActiveCB;

/** active object control blocks */
/*lint -save -e9067 MISRA-C:2012 Rule 8.11, extern array declared without size */
extern QActiveCB const Q_ROM QF_active[];
/*lint -restore */

/*! number of active objects in the application (# elements in QF_active[]) */
extern uint_fast8_t QF_maxActive_;

/*! Ready set of QF-nano. */
extern uint_fast8_t volatile QF_readySet_;

#ifndef QF_LOG2

    /*! Lookup table for (log2(n) + 1), where n is the index into the table.
    * This lookup delivers the 1-based number of the most significant 1-bit
    * of a nibble.
    */
    extern uint8_t const Q_ROM QF_log2Lkup[16];

#endif


#ifdef QF_TIMEEVT_USAGE

    /*! Timer set of QF-nano. */
    extern uint_fast8_t volatile QF_timerSetX_[QF_MAX_TICK_RATE];

#endif  /* QF_TIMEEVT_USAGE */


/*! Lookup table for ~(1 << (n - 1)), where n is the index into the table. */
extern uint8_t const Q_ROM QF_invPow2Lkup[9];


/****************************************************************************/
/*! This macro encapsulates accessing the active object queue at a
* given index, which violates MISRA-C 2004 rules 17.4(req) and 11.4(adv).
* This macro helps to localize this deviation.
*/
#define QF_ROM_QUEUE_AT_(ao_, i_) (((QEvt *)Q_ROM_PTR((ao_)->queue))[(i_)])

/*! This macro encapsulates accessing the active object control block,
* which violates MISRA-C 2004 rule 11.4(adv). This macro helps to localize
* this deviation.
*/
#define QF_ROM_ACTIVE_GET_(p_) ((QActive *)Q_ROM_PTR(QF_active[(p_)].act))

/*! This macro encapsulates the upcast to QActive*
*
* This macro encapsulates up-casting a pointer to a subclass of ::QActive
* to the base class ::QActive, which violates MISRA-C 2004 rule 11.4(adv).
* This macro helps to localize this deviation.
*/
#define QF_ACTIVE_CAST(a_)     ((QActive *)(a_))

#endif /* QFN_H */

