/*
 *			ShoeLace Header File
 *
 * (C) Copyright C E Chew
 *
 * Feel free to copy and use this software provided:
 *
 *	1. you do not pretend that you wrote it
 *	2. you leave this copyright notice intact.
 *
 * This file contains those definitions that are globally
 * applicable (typically Minix system dependent stuff). Parts
 * that are meant only for one section are placed in other files.
 *
 * Edit History:
 * 24-Apr-1990	Adjusted include order for 1.5.8
 * 08-Nov-1989	Changed names and included signature
 * 05-May-1989	Separated shoe.h into several files
 * 01-May-1989	Conditionalise enumerations
 * 26-Apr-1989	Added BPBOFFSET
 * 16-Apr-1989	Merged more additions from Bruce
 * 14-Apr-1989	Debugger support
 * 03-Nov-1988	Merged boot parameters with configuration parameters
 * 29-Oct-1988	Added boot.h as suggested by Bruce Evans
 * 15-Oct-1988	Function prototypes are now optional
 * 10-Oct-1988	Augment configuration data
 * 27-Sep-1988	Hooks for opening and reading files
 * 17-Sep-1988	Added hooks for gets() and further prototypes
 * 18-Apr-1988	Created from dos2out.c
 * 23-Apr-1988	Added partition table definitions
 */

#include <setjmp.h>
#ifdef _SETJMP_H			/* Minix 1.5.0 and above */
# define dev_nr dev_t
# define file_pos off_t
# define inode_nr ino_t
# define links nlink_t
# define i_modtime i_mtime
# define mask_bits mode_t
# define MAX_LINKS LINK_MAX
# define NAME_SIZE NAME_MAX
#include <linux/fs.h>
#else					/* Minix 1.4a and below */
# include <limits.h>
# include <minix/const.h>
# include <minix/type.h>
# include <fs/const.h>
# include <fs/type.h>
# include <fs/buf.h>
# include <fs/inode.h>
# include <fs/super.h>
#endif


#include "proto.h"

#undef printf
#undef putc
#undef getchar

#define printf printk

#ifndef	ENUMS
# define enum
#endif

/* Name the version */

#define VERSION		"v1.0 Nov 1989"

/* Names of special files */

#define BOOTLACENAME	"BootLace"
#define SHOELACENAME	"ShoeLace"

#define WINIBOOTFILE	"/etc/winiboot"
#define	BOOTLACEFILE	"/etc/bootlace"
#define SHOELACEFILE	"/shoelace"
#define DISKTABFILE	"/etc/disktab"
#define CONFIGFILE	"/etc/config"

/*
 * The following data type is used to store addresses of physical
 * memory. Memory addresses are specified as if they resided in
 * a linear space. Conversion to offset and segments is done
 * by the low level routines. The exception to this is the
 * low level bootstrap code in bootlace.s and shoeboot.c
 */

typedef unsigned long ADDRESS;

/* Declare physical disk parameters */

#define SECTOR_SIZE	512		/* size of a physical sector */
#define HARD_DISK	0x80		/* hard disk bit */

#if	BLOCK_SIZE/SECTOR_SIZE != 2
#error	readblock hard coded for 1 kb block sizes
#endif

/* Major device numbers */

#define DEV_RAM   0x100
#define DEV_FD0   0x200
#define DEV_HD0   0x300

/* Declare the boot signature */

#define SIGNATURE_BYTES { 0x55, 0xaa }	/* boot signature */

typedef unsigned char SIGNATURE[2];

/* Keyboard input buffer size */

#define INPUTBUFFERSIZE	80		/* size of keyboard input buffer */

/* Declare useful data structures */

typedef unsigned char buffer[BLOCK_SIZE];/* disk io buffer */

/* Library prototypes */

void printf P((char *, ...));
int atoi P((char *));
