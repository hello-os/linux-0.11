/*
 *			ANSI Prototyping
 *
 * This file declares macros that are useful for conditionally
 * providing ANSI function prototypes.
 */

#ifndef	__PROTO_H__
# define	__PROTO_H__
# ifdef		__STDC__
#   define	P(x)		x
#   define	F4V(t1,n1,t2,n2,t3,n3,t4,n4)	(t1 n1, t2 n2, t3 n3, t4 n4 \
						 VA_ALIST)
#   define	F3V(t1,n1,t2,n2,t3,n3)		(t1 n1, t2 n2, t3 n3 \
						 VA_ALIST)
#   define	F2V(t1,n1,t2,n2)		(t1 n1, t2 n2 \
						 VA_ALIST)
#   define	F1V(t1,n1)			(t1 n1 \
						 VA_ALIST)
#   define	F4(t1,n1,t2,n2,t3,n3,t4,n4)	(t1 n1, t2 n2, t3 n3, t4 n4)
#   define	F3(t1,n1,t2,n2,t3,n3)		(t1 n1, t2 n2, t3 n3)
#   define	F2(t1,n1,t2,n2)			(t1 n1, t2 n2)
#   define	F1(t1,n1)			(t1 n1)
#   define	F0()				(void)
# else
#   define	P(x)		()
#   define	F4V(t1,n1,t2,n2,t3,n3,t4,n4)	(n1, n2, n3, n4 \
						 VA_ALIST) \
						t1 n1; t2 n2; t3 n3; t4 n4;
#   define	F3V(t1,n1,t2,n2,t3,n3)		(n1, n2, n3 \
						 VA_ALIST) \
						t1 n1; t2 n2; t3 n3;
#   define	F2V(t1,n1,t2,n2)		(n1, n2 \
						 VA_ALIST) \
						t1 n1; t2 n2;
#   define	F1V(t1,n1)			(n1 \
						 VA_ALIST) \
						t1 n1;
#   define	F4(t1,n1,t2,n2,t3,n3,t4,n4)	(n1, n2, n3, n4) \
						t1 n1; t2 n2; t3 n3; t4 n4;
#   define	F3(t1,n1,t2,n2,t3,n3)		(n1, n2, n3) \
						t1 n1; t2 n2; t3 n3;
#   define	F2(t1,n1,t2,n2)			(n1, n2) \
						t1 n1; t2 n2;
#   define	F1(t1,n1)			(n1) \
						t1 n1;
#   define	F0()				()
# endif
#endif
