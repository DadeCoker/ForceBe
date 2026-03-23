#include "b_mod_qpn.h"

/* Local-scope objects -----------------------------------------------------*/
static QEvt l_blinkyQSto[10]; /* Event queue storage for Blinky */

QActiveCB const Q_ROM QF_active[] = {
    { (QActive *)0,           (QEvt *)0,        0U                      },
	{ (QActive *)&AO_Blinky, l_blinkyQSto, Q_DIM(l_blinkyQSto) }
};

void qpn_init(void)
{
	Blinky_ctor(); /* instantiate all Blinky AO */

	QF_init(Q_DIM(QF_active)); /* initialize the QF-nano framework */
	BSP_init();      /* initialize the Board Support Package */

	QF_Ready_run();
}

int qpn_run(void) {
	return QF_run(); /* transfer control to QF-nano */
}
