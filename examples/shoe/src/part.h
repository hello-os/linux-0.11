/*
 *			Partition Header File
 *
 * (C) Copyright C E Chew
 *
 * Feel free to copy and use this software provided:
 *
 *	1. you do not pretend that you wrote it
 *	2. you leave this copyright notice intact.
 *
 * Edit History:
 *
 * 02-May-1989	Hacked from shoe.h
 */

/* Define location of partition table in partition block */

#define P_PART_OFFSET		446	/* partition table offset */
#define P_PART_MAX		4	/* maximum number of partitions */

/* Define offsets and sizes into partition array */

#define	P_BOOTFLAG_P			0
#define		P_BOOTFLAG_S		1
#define	P_STARTHEAD_P			(P_BOOTFLAG_P+P_BOOTFLAG_S)
#define		P_STARTHEAD_S		1
#define	P_STARTSECTOR_P			(P_STARTHEAD_P+P_STARTHEAD_S)
#define		P_STARTSECTOR_S		1
#define	P_STARTCYLINDER_P		(P_STARTSECTOR_P+P_STARTSECTOR_S)
#define		P_STARTCYLINDER_S	1
#define	P_TYPE_P			(P_STARTCYLINDER_P+P_STARTCYLINDER_S)
#define		P_TYPE_S		1
#define	P_ENDHEAD_P			(P_TYPE_P+P_TYPE_S)
#define		P_ENDHEAD_S		1
#define	P_ENDSECTOR_P			(P_ENDHEAD_P+P_ENDHEAD_S)
#define		P_ENDSECTOR_S		1
#define	P_ENDCYLINDER_P			(P_ENDSECTOR_P+P_ENDSECTOR_S)
#define		P_ENDCYLINDER_S		1
#define	P_HIDDEN_P			(P_ENDCYLINDER_P+P_ENDCYLINDER_S)
#define		P_HIDDEN_S		4
#define	P_SIZE_P			(P_HIDDEN_P+P_HIDDEN_S)
#define		P_SIZE_S		4
#define	P_PART_S			(P_SIZE_P+P_SIZE_S)

/* Define the partition array */

typedef unsigned char PARTITION[P_PART_S];
