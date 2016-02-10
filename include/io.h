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

#ifndef IO150303
#define IO150303

#define BI_TMSK(_n)		((1U<<(_n))-1)

#define BI_FMSK(_ms,_ls)	(BI_TMSK((_ms)-(_ls)+1)<<(_ls))

#define BI_BMSK(_n)		(1U << (_n))

#define BI_WID(_ms,_ls)	((_ms)-(_ls)+1)

#define BI_ALN(_i, _n)	((_i) & ~BI_TMSK(_n))

#define BI_RUP(_i, _n)	BI_ALN((_i)+BI_TMSK(_n), _n)

#define BI_TRU(_i, _n)	((_i) & BI_TMSK(_n))

#define BI_G_FLD(_i,_ms,_ls)		\
	(((_i)<<(31-(_ms))) >> (31- (_ms) + (_ls)))

#define BI_O_FLD(_i,_ms,_ls,_v)	\
	((_i)|((BI_TRU(_v,BI_WID(_ms,_ls))<<(_ls))))

#define BI_C_FLD(_i,_ms,_ls)		((_i) & ~BI_FMSK(_ms,_ls))

#define BI_R_FLD(_i,_ms,_ls,_v)	\
	((((_i)&~BI_FMSK(_ms,_ls)))|((BI_TRU(_v,BI_WID(_ms,_ls))<<(_ls))))

#define BI_G_BIT(_i,_n)		BI_G_FLD(_i, _n, _n)

#define BI_O_BIT(_i,_n)		((_i) |(1 << (_n)))

#define BI_Z_BIT(_i,_n)		((_i) & ~BI_BMSK(_n))

#define BI_R_BIT(_i,_n,_v)		\
	(((_i)&~BI_BMSK(_n))|(((_v)&0x1)<<(_n)))

static inline void writel(unsigned v, void *a)
{
	(*((volatile unsigned *)(a)) = (v));
}

#define readl(_a)	_readl((void*)(_a))

static inline unsigned _readl(void *a)
{
	return *((volatile unsigned *)(a));
}

static inline void writew(unsigned v, void *a)
{
	(*((volatile unsigned short *)(a)) = (v));
}

#define readw(_a)	_readw((void*)(_a))

static inline unsigned _readw(void *a)
{
	return *((volatile unsigned short *)(a));
}

static inline void writeb(unsigned v, void *a)
{
	(*((volatile unsigned char *)(a)) = (v));
}

#define readb(_a)	_readb((void*)(_a))

static inline unsigned _readb(void *a)
{
	return *((volatile unsigned char *)(a));
}

#define reg(_a)	*((volatile unsigned*)(_a))

#endif