/*
 *			BPB Header File
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
 * 02-May-1989	Hacked from laceup.c
 */

/* Define the locations of the components in the boot block */

#define B_DISKCODE_OFFSET	2	/* offset to diskcode */
#define B_BPB_OFFSET		3	/* offset to bpb */

/* Define size of disk code array */

#define	B_DISKCODE_S		1

typedef unsigned char DISKCODE[B_DISKCODE_S];

/* Define offsets and sizes into bpb array */

#define B_ID_P			0
#define		B_ID_S			8
#define B_SECTORSIZE_P		(B_ID_P+B_ID_S)
#define		B_SECTORSIZE_S		2
#define	B_BLOCKSIZE_P		(B_SECTORSIZE_P+B_SECTORSIZE_S)
#define		B_BLOCKSIZE_S		1
#define	B_RESERVED_P		(B_BLOCKSIZE_P+B_BLOCKSIZE_S)
#define		B_RESERVED_S		2
#define	B_FATS_P		(B_RESERVED_P+B_RESERVED_S)
#define		B_FATS_S		1
#define	B_ROOTSIZE_P		(B_FATS_P+B_FATS_S)
#define		B_ROOTSIZE_S		2
#define	B_DEVSIZE_P		(B_ROOTSIZE_P+B_ROOTSIZE_S)
#define		B_DEVSIZE_S		2
#define	B_MEDIA_P		(B_DEVSIZE_P+B_DEVSIZE_S)
#define		B_MEDIA_S		1
#define	B_FATSIZE_P		(B_MEDIA_P+B_MEDIA_S)
#define		B_FATSIZE_S		2
#define	B_TRACKSIZE_P		(B_FATSIZE_P+B_FATSIZE_S)
#define		B_TRACKSIZE_S		2
#define	B_CYLINDERSIZE_P	(B_TRACKSIZE_P+B_TRACKSIZE_S)
#define		B_CYLINDERSIZE_S	2
#define	B_HIDDEN_P		(B_CYLINDERSIZE_P+B_CYLINDERSIZE_S)
#define		B_HIDDEN_S		4
#define	B_FPT_P			(B_HIDDEN_P+B_HIDDEN_S)
#define		B_FPT_S			11
#define	B_BPB_S			(B_FPT_P+B_FPT_S)

/* Define the bpb array */

typedef unsigned char BPB[B_BPB_S];
