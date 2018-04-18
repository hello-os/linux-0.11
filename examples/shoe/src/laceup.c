/*
 *		Write BootLace or WiniBoot to Volume
 *
 * (C) Copyright C E Chew
 *
 * Feel free to copy and use this software provided:
 *
 *	1. you do not pretend that you wrote it
 *	2. you leave this copyright notice intact.
 *
 * This code will scan an unmounted volume for ShoeLace and
 * will write the boot block code to the boot block on the
 * volume.
 *
 * Edit History:
 * 24-Apr-1990	Changed include file order for 1.5.8
 * 05-Dec-1989	Look in current directory for files.
 * 09-Nov-1989	Remember to cast (unsigned long) in call to tochars().
 *		Allow default wini partition to be specified.
 * 08-Nov-1989	Merged signature with shoe.c
 * 07-Nov-1989	Enhanced for WiniBoot
 * 02-May-1989	Fixed tgetstr bug, Shoepath definition bug, bpb
 *		initialisation
 * 26-Apr-1989	Support for /etc/disktab
 */

struct exec {
  unsigned char a_magic[2];
  unsigned char a_flags;
  unsigned char a_cpu;
  unsigned char a_hdrlen;
  unsigned char a_unused;
  unsigned short a_version;
  long a_text;
  long a_data;
  long a_bss;
  long a_noentry;
  long a_total;
  long a_sims;
  long a_trsize;
  long a_drsize;
  long a_tbase;
  long a_dbase;
};

#include "shoelace.h"
#undef printf
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "bpb.h"
#include "part.h"
#ifndef A_SEP
#define A_SEP 0x20
#endif 
#ifndef _SIZE_T
typedef unsigned int size_t;
#endif

#ifndef SEEK_SET
#define SEEK_SET	0
#endif

#define WINIBOOTEXTRA	1		/* extra bytes in WiniBoot */

extern char *getcwd P((char *, int));
/* extern int access P((char *, int)); */
extern void exit P((int));
extern int sync P((void));
extern void *malloc P((unsigned int));
extern void free P((void *));
extern int tgetent P((char *, char *));
extern int tgetnum P((char *));
extern char *tgetstr P((char *, char **));
extern int getopt P((int, char **, char *));

extern int optind;
extern char *optarg;

void winiboot P((char *, int));
void bootlace P((char *, char *));
void tochars P((unsigned long, unsigned char *, unsigned int));
unsigned long fromchars P((unsigned char *, unsigned int));
char *findfile P((char *));

extern char **environ;			/* environment pointer */

char bootsector[SECTOR_SIZE];		/* buffer to read in wini boot */
char bootblock[BLOCK_SIZE];		/* buffer to read in boot block */
char diskstr[1024];			/* string area */
char disktab[1024];			/* disktab buffer */
char termcapenv[] = "TERMCAP=";		/* termcap environment entry */
char *fake_env[] = {0, 0};		/* fake environment */

SIGNATURE signature = SIGNATURE_BYTES;	/* boot signature */

#define A_TEXTPOS(n) (n.a_hdrlen) 
#define BADMAG(n) ((n.a_magic[0]!=1) || (n.a_magic[1]!=3))
int
main F2(int, argc, char **, argv)

{
  int sw;				/* switch */
  char error;				/* error occurred */
  char wini;				/* wini boot */

  for (wini = error = 0; (sw = getopt(argc, argv, "w:")) != EOF; ) {
    switch (sw) {
    case 'w':
      if (optarg[0] == 0 || optarg[1] != 0)
        error = 1;
      else {
        switch (optarg[0]) {
	case '1': wini  = 1; break;
	case '2': wini  = 2; break;
	case '3': wini  = 3; break;
	case '4': wini  = 4; break;
	default:  error = 1; break;
	}
      }
      break;
    default:  error = 1; break;
    }
  }

  if (error || (wini && optind != argc-1) || (!wini && optind != argc-2)) {
    fprintf(stderr, "Usage: %s [-w partition] file [disktype]\n", argv[0]);
    exit(1);
  }

  if (wini)
    winiboot(argv[optind], wini);
  else
    bootlace(argv[optind], argv[optind+1]);

  return 0;
}

/*
 * Write BootLace
 *
 * Copy the BootLace into the boot block of the named file.
 */

void
bootlace F2(char *, Bootdev, char *, disktype)

{
  struct exec head;			/* shoelace header */
  int i;				/* index */
  int n;				/* disktab values */
  int v;				/* scanf return value */
  char *sp;				/* pointer into string area */
  char *fp;				/* disk parameters */
  BPB boothead;				/* boot header */
  FILE *Bootfp;				/* access channel */
  char *Bootfilename;			/* boot name */
  char *disktabfilename;		/* disk table name */

  Bootfilename = findfile(BOOTLACEFILE);
  disktabfilename = findfile(DISKTABFILE);

  if ((fake_env[0] = (char *) malloc(sizeof(termcapenv)-1 +
				     strlen(disktabfilename) + 1)) == 0) {
    fputs("No room for termcap environment entry\n", stderr);
    exit(1);
  }
  strcpy(fake_env[0], termcapenv);
  strcat(fake_env[0], disktabfilename);
  environ = &fake_env[0];
  switch (tgetent(disktab, disktype)) {
  case 0:
    fprintf(stderr, "Disk type %s not found in %s\n",
            disktype, strchr(fake_env[0], '=')+1);
    exit(1);
  case -1:
    fprintf(stderr, "%s not found\n", strchr(fake_env[0], '=')+1);
    exit(1);
  }

/* Open channel to boot image */  
  if ((Bootfp = fopen(Bootfilename, "r")) == NULL) {
    fprintf(stderr, "Cannot open %s\n", Bootfilename);
    exit(1);
  }

/* Read the header of the executable */
  if (fread((char *) &head, 1, sizeof(head), Bootfp) != sizeof(head)) {
    fprintf(stderr, "Cannot read header in %s\n", Bootfilename);
    exit(1);
  }

  if (BADMAG(head)) {
    fprintf(stderr, "%s not an executable\n", Bootfilename);
    exit(1);
  }

 /* if ((head.a_flags & A_SEP) != 0) {
    fputs("Executable not ComI&D\n", stderr);
    exit(1);
  } */

  if (head.a_text + head.a_data > sizeof(bootblock)-sizeof(signature)) {
    fputs("Code overflows boot block\n", stderr);
    exit(1);
  }

/* Read the boot block */
  memset((char *) bootblock, 0, sizeof(bootblock));
  if (fseek(Bootfp, (long) A_TEXTPOS(head), SEEK_SET)                 != 0 ||
      fread(bootblock, 1, (size_t) (head.a_text+head.a_data), Bootfp) == 0) {
    fprintf(stderr, "Cannot read boot block from %s\n", Bootfilename);
    exit(1);
  }
  fclose(Bootfp);

/* Check the bpb in the boot block */
  memcpy((char *) &boothead[0], &bootblock[B_BPB_OFFSET], sizeof(boothead));
  if (fromchars(&boothead[B_SECTORSIZE_P], B_SECTORSIZE_S) > SECTOR_SIZE) {
    fputs("Code overflows boot sector\n", stderr);
    exit(1);
  }

/* Insert the device structure information */
  strncpy((char *) &boothead[B_ID_P], SHOELACENAME, B_ID_S);
  
  if ((n = tgetnum("nt")) == -1) {
    fputs("Cannot find number of tracks per cylinder (nt)\n", stderr);
    exit(1);
  }
  tochars((unsigned long) n, &boothead[B_CYLINDERSIZE_P], B_CYLINDERSIZE_S);

  if ((n = tgetnum("se")) == -1) {
    fputs("Cannot find number of bytes per sector (ns)\n", stderr);
    exit(1);
  }
  tochars((unsigned long) n, &boothead[B_SECTORSIZE_P], B_SECTORSIZE_S);
  tochars((unsigned long) BLOCK_SIZE/n,
          &boothead[B_BLOCKSIZE_P],
          B_BLOCKSIZE_S);
  tochars((unsigned long) BLOCK_SIZE/n, &boothead[B_RESERVED_P], B_RESERVED_S);
  
  if ((n = tgetnum("ns")) == -1) {
    fputs("Cannot find number of sectors per track (ns)\n", stderr);
    exit(1);
  }
  tochars((unsigned long) n, &boothead[B_TRACKSIZE_P], B_TRACKSIZE_S);

  if ((n = tgetnum("md")) == -1) {
    fputs("Cannot find media description byte (md)\n", stderr);
    exit(1);
  }
  tochars((unsigned long) n, &boothead[B_MEDIA_P], B_MEDIA_S);
  
  sp = diskstr;
  if (tgetstr("fp", &sp) == (char *) 0) {
    fputs("Cannot find disk parameter table (fp)\n", stderr);
    exit(1);
  }
  for (sp = diskstr, i = 0; (fp = strtok(sp, " \t")) != 0; sp = 0, i++) {
    if (i == B_FPT_S) {
      fputs("Too many entries in disk parameter table (fp)\n", stderr);
      exit(1);
    }
    if (strncmp(fp, "0x", 2) == 0 || strncmp(fp, "0X", 2) == 0)
      v = sscanf(fp, "%x", &n);
    else if (strlen(fp) > 1 && fp[0] == '0')
      v = sscanf(fp, "%o", &n);
    else
      v = sscanf(fp, "%d", &n);
    if (v != 1 || n < 0 || n > 0xff) {
      fputs("Illegal entry in disk parameter table (fp)\n", stderr);
      exit(1);
    }
    boothead[B_FPT_P+i] = (unsigned char) n;
  }
  if (i != B_FPT_S) {
    fputs("Too few entries in disk parameter table (fp)\n", stderr);
    exit(1);
  }

/* Plant the bpb in the boot block */
  memcpy(&bootblock[B_BPB_OFFSET], (char *) &boothead[0], sizeof(boothead));

/* Show the user what the parameters are */
  printf("Identification:      %.*s\n",
         B_ID_S, &boothead[B_ID_P]);
  printf("Sector size:         %8lu bytes\n",
         fromchars(&boothead[B_SECTORSIZE_P], B_SECTORSIZE_S));
  printf("Block size:          %8lu sectors\n",
         fromchars(&boothead[B_BLOCKSIZE_P], B_BLOCKSIZE_S));
  printf("Boot sectors:        %8lu sectors\n",
         fromchars(&boothead[B_RESERVED_P], B_RESERVED_S));
  printf("Number of fats:      %8lu\n",
         fromchars(&boothead[B_FATS_P], B_FATS_S));
  printf("Root directory size: %8lu entries\n",
         fromchars(&boothead[B_ROOTSIZE_P], B_ROOTSIZE_S));
  printf("Volume size:         %8lu sectors\n",
         fromchars(&boothead[B_DEVSIZE_P], B_DEVSIZE_S));
  printf("Media descriptor:        0x%02lx\n",
         fromchars(&boothead[B_MEDIA_P], B_MEDIA_S));
  printf("Fat size:            %8lu sectors\n",
         fromchars(&boothead[B_FATSIZE_P], B_FATSIZE_S));
  printf("Track size:          %8lu sectors\n",
         fromchars(&boothead[B_TRACKSIZE_P], B_TRACKSIZE_S));
  printf("Cylinder size:       %8lu tracks\n",
         fromchars(&boothead[B_CYLINDERSIZE_P], B_CYLINDERSIZE_S));
  printf("Hidden sectors:      %8lu sectors\n",
         fromchars(&boothead[B_HIDDEN_P], B_HIDDEN_S));
  printf("Floppy table: ");
  for (i = 0; i < B_FPT_S; i++)
    printf(" 0x%02x", boothead[B_FPT_P+i]);
  printf("\n");
  
/* Insert the boot signature */
  for (i = sizeof(bootblock)-1; i >= SECTOR_SIZE; i--)
    bootblock[i] = bootblock[i-sizeof(signature)];
  memcpy(&bootblock[SECTOR_SIZE-sizeof(signature)],
         (char *) &signature[0],
	 sizeof(signature));

/* Write the boot block on the device */
  if ((Bootfp = fopen(Bootdev, "w"))                  == NULL ||
      fwrite(bootblock, 1, sizeof(bootblock), Bootfp) != sizeof(bootblock)) {
    fprintf(stderr, "Cannot write boot block to %s\n", Bootdev);
    exit(1);
  }

  sync();
}

/*
 * Write WiniBoot
 *
 * Copy the WiniBoot into the named file. The old partition entries
 * are left intact.
 */

void
winiboot F2(char *, Bootdev, int, bootpart)

{
  int i;				/* index */
  struct exec head;			/* shoelace header */
  FILE *Winifp;				/* wini boot file */
  char *Winifilename;			/* wini boot image */
  PARTITION part[P_PART_MAX];		/* partitions */

/* Open channel to wini image */  
  Winifilename = findfile(WINIBOOTFILE);
  if ((Winifp = fopen(Winifilename, "r")) == NULL) {
    fprintf(stderr, "Cannot open %s\n", Winifilename);
    exit(1);
  }

/* Read the header of the executable */
  if (fread((char *) &head, 1, sizeof(head), Winifp) != sizeof(head)) {
    fprintf(stderr, "Cannot read header in %s\n", Winifilename);
    exit(1);
  }

  if (BADMAG(head)) {
    fprintf(stderr, "%s not an executable\n", Winifilename);
    exit(1);
  }

  /* if ((head.a_flags & A_SEP) != 0) {
    fputs("Executable not ComI&D\n", stderr);
    exit(1);
  } Take a chance, comment this out. */

  if (head.a_text + head.a_data > P_PART_OFFSET - WINIBOOTEXTRA) {
    fputs("Code overflows boot sector\n", stderr);
    exit(1);
  }

/* Read the boot sector */
  memset((char *) bootsector, 0, sizeof(bootsector));
  if (fseek(Winifp, (long) A_TEXTPOS(head), SEEK_SET)                  != 0 ||
      fread(bootsector, 1, (size_t) (head.a_text+head.a_data), Winifp) == 0) {
    fprintf(stderr, "Cannot read boot sector from %s\n", Winifilename);
    exit(1);
  }
  fclose(Winifp);

/* Get old partition table */
  if ((Winifp = fopen(Bootdev, "r"))                       == NULL ||
      fseek(Winifp, (long) P_PART_OFFSET, SEEK_SET)        != 0    ||
      fread((char *) &part[0][0], 1, sizeof(part), Winifp) != sizeof(part)) {
    fprintf(stderr, "Cannot read old partition table from %s\n", Bootdev);
    exit(1);
  }
  fclose(Winifp);

/* Display partition table */
  puts("   Boot Hd Sec Cyl Type Hd Sec Cyl      Base      Size");
  for (i = 0; i < P_PART_MAX; i++) {
    printf("%c%d  %2lx  %2lx  %2lx  %2lx  %2lx  %2lx  %2lx  %2lx %9ld %9ld\n",
           i+1 == bootpart ? '*' : ' ', i+1,
	   fromchars(&part[i][P_BOOTFLAG_P], P_BOOTFLAG_S),
	   fromchars(&part[i][P_STARTHEAD_P], P_STARTHEAD_S),
	   fromchars(&part[i][P_STARTSECTOR_P], P_STARTSECTOR_S),
	   fromchars(&part[i][P_STARTCYLINDER_P], P_STARTCYLINDER_S),
	   fromchars(&part[i][P_TYPE_P], P_TYPE_S),
	   fromchars(&part[i][P_ENDHEAD_P], P_ENDHEAD_S),
	   fromchars(&part[i][P_ENDSECTOR_P], P_ENDSECTOR_S),
	   fromchars(&part[i][P_ENDCYLINDER_P], P_ENDCYLINDER_S),
	   fromchars(&part[i][P_HIDDEN_P], P_HIDDEN_S),
	   fromchars(&part[i][P_SIZE_P], P_SIZE_S));
  }

/* Insert default boot partition */
  bootsector[P_PART_OFFSET-1] = bootpart;

/* Copy partition table */
  memcpy(&bootsector[P_PART_OFFSET], (char *) &part[0][0], sizeof(part));

/* Insert signature */
  memcpy(&bootsector[SECTOR_SIZE-sizeof(signature)],
         (char *) &signature[0],
	 sizeof(signature));

/* Write wini boot */
  if ((Winifp = fopen(Bootdev, "w"))                    == NULL ||
      fwrite(bootsector, 1, sizeof(bootsector), Winifp) != sizeof(bootsector)) {
    fprintf(stderr, "Cannot write boot to %s\n", Bootdev);
    exit(1);
  }
  fclose(Winifp);

  sync();
}

/*
 * Convert long to char sequence
 *
 * This converts a long into a sequence of chars in little endian
 * order.
 */

void tochars F3(unsigned long, n, unsigned char *, p, unsigned int, b)

{
  while (b--) {
    *p++ = n;
    n >>= CHAR_BIT;
  }
}

/*
 * Convert char sequence to long
 *
 * This converts a character sequence in little endian order
 * to an unsigned long.
 */

unsigned long fromchars F2(unsigned char *, p, unsigned int, b)

{
  unsigned long n;
  unsigned int s;

  for (n = 0, s = 0; b--; n |= ((unsigned long) (*p++) << s), s += CHAR_BIT)
    ;
  return n;
}

/*
 * Find a file
 *
 * Look for the named file. If it is not found, look in the current
 * directory.
 */

char *findfile F1(char *, name)

{
  char *bname;				/* base name */
  char *cwd;				/* current directory */

  if (access(name, R_OK) == 0)
    return name;

  if ((bname = strrchr(name, '/')) != NULL && access(++bname, R_OK) == 0) {
    if ((cwd = (char *) malloc(PATH_MAX)) == NULL ||
        getcwd(cwd, PATH_MAX)             == NULL) {
      fprintf(stderr, "Cannot get current directory\n");
      exit(1);
    }
    if ((name = (char *) malloc(strlen(cwd) + strlen(bname) + 2)) == NULL) {
      fprintf(stderr, "Insufficient memory for %s\n", bname);
      exit(1);
    }
    strcpy(name, cwd);
    strcat(name, "/");
    strcat(name, bname);
    free((void *) cwd);
    return name;
  }

  fprintf(stderr, "%s not found\n", bname == NULL ? name : bname);
  exit(1);
  return (char *) NULL;
}
