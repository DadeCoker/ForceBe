/**
* @file
* @brief QP-nano public interface including backwards-compatibility layer
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
#ifndef QPN_H
#define QPN_H

/**
* @description
* This header file must be included directly or indirectly
* in all application modules (*.c files) that use QP-nano.
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "qpn_conf.h" /* QP-nano configuration file (from the application) */
#include "qfn_port.h" /* QF-nano port from the port directory */
#include "qassert.h"  /* embedded systems-friendly assertions */


/****************************************************************************/
/* QP API compatibility layer */
#ifndef QP_API_VERSION

/*! Macro that specifies the backwards compatibility with the
* QP-nano API version.
*/
/**
* @description
* For example, QP_API_VERSION=450 will cause generating the compatibility
* layer with QP-nano version 4.5.0 and newer, but not older than 4.5.0.
* QP_API_VERSION=0 causes generation of the compatibility layer "from the
* begining of time", which is the maximum backwards compatibilty. This is
* the default.@n
* @n
* Conversely, QP_API_VERSION=9999 means that no compatibility layer should
* be generated. This setting is useful for checking if an application
* complies with the latest QP-nano API.
*/
#define QP_API_VERSION 0

#endif  /* QP_API_VERSION */

/****************************************************************************/
#if (QP_API_VERSION < 580)

/*! @deprecated QMActive Control Block; instead use: ::QActiveCB. */
typedef QActiveCB QMActiveCB;

/*! @deprecated QMActive; instead use: ::QActive. */
typedef QActive QMActive;

/*! @deprecated QMsm state machine; instead use: ::QHsm. */
typedef QHsm QMsm;

/*! @deprecated QMActive constructor; instead use: QActive_ctor() */
#define QMActive_ctor QActive_ctor

/*! @deprecated QMsm state machine constructor; instead use: QHsm_ctor() */
#define QMsm_ctor     QHsm_ctor

#endif /* QP_API_VERSION < 540 */
/****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif  /* QPN_H */
