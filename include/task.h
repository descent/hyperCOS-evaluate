/*-****************************************************************************/
/*-                                                                           */
/*-            Copyright (c) of hyperCOS.                                     */
/*-                                                                           */
/*-  This software is copyrighted by and is the sole property of its authors. */
/*-  All rights, title, ownership, or other interests in the software remain  */
/*-  the property of the authors. The source code is for FREE short-term      */
/*-  evaluation, educational or non-commercial research only. Any commercial  */
/*-  application may only be used in accordance with the corresponding license*/
/*-  agreement. Any unauthorized use, duplication, transmission, distribution,*/
/*-  or disclosure of this software is expressly forbidden.                   */
/*-                                                                           */
/*-  Knowledge of the source code may NOT be used to develop a similar product*/
/*-                                                                           */
/*-  This Copyright notice may not be removed or modified without prior       */
/*-  written consent of the authors.                                          */
/*-                                                                           */
/*-  The authors reserves the right to modify this software                   */
/*-  without notice.                                                          */
/*-                                                                           */
/*-  To email the authors:                                                    */
/*-                                                                           */
/*-             socware.help@gmail.com                                        */
/*-                                                                           */
/*-****************************************************************************/

#ifndef TASK150313
#define TASK150313

#include "cfg.h"
#include "ll.h"
#include "wait.h"
#include "tmr.h"
#include "core.h"
#include "irq.h"
#include "dbg.h"
#include "cpu/reg.h"
#include "cpu/_cpu.h"

typedef void (*task_ent) (void *priv);

typedef enum {
	TASK_READY,
	TASK_WAIT,
	TASK_DEST,
} task_status_t;

typedef struct task {
	task_ent ent;		///< thread function
	const char *name;
	unsigned *stack;
	int stack_sz;		///< stack size requirements in bytes
	short slice;		///< initial timeslice
	short slice_cur;	///< available time slice before yield
	void *priv;
	lle_t ll;
	reg_t *context;
	tmr_t to;
	char status;		///< task_status_t
	char tm_out;
	short pri;		///< initial thread priority
} task_t;

extern void (*task_gc) (task_t * t);

/// \arg pri, 1~ (CFG_TPRI_NUM-1), less value has higher priority, 0 is invalid
/// \arg slice, -1 for no time-slice
task_t *task_init(task_t * t,
		  const char *name,
		  task_ent e,
		  int pri,
		  unsigned *stack, int stack_sz, int slice, void *priv);

#define task_new(_n,_e,_p,_sz,_sl,_pr) \
	task_init(_alloc(sizeof(task_t)), _n, _e, _p, _alloc(_sz), _sz, _sl, _pr)

extern task_t *_task_cur, *_task_pending;

/// requirecritical section protection
/// \return -1 on timeout
int task_suspend(ll_t * wq, wait_t w);

/// \param w sleep ticks
void task_sleep(wait_t w);

/// \param hint notify the scheduler to bypass thread with
/// priority higher than hint
void sch_schedule(unsigned hint);

/// \param yield the control to threads with the same priority
static inline void task_yield()
{
	sch_schedule(_task_cur->pri);
}

/// wake up thread, possible trigger an immediate context switch if t's
/// priority is higher than current
void sch_wake(task_t * t);

static inline int _task_wakeq(ll_t * wq, unsigned iflag)
{
	if (!ll_empty(wq)) {
		task_t *t = lle_get(ll_head(wq), task_t, ll);
		lle_del(&t->ll);
		sch_wake(t);
		irq_restore(iflag);
		return 0;
	} else {
		irq_restore(iflag);
		return 1;
	}
}

static inline void task_wakeq(ll_t * wq)
{
	unsigned iflag = irq_lock();
	_task_wakeq(wq, iflag);
}

void _task_load(reg_t * reg_next);

void _task_switch(reg_t * reg_next, reg_t ** reg_ori);

static inline void _task_switch_pending(task_t * tn)
{
	irq_dep_chk(irq_depth > 0);
	if (!_task_pending || _task_pending->pri > tn->pri)
		_task_pending = tn;
	cpu_req_switch();
}

/// \return _task_ori
reg_t **_task_switch_status(task_t * _tnext);

extern void (*task_ov) (task_t * t);

#endif