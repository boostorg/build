/*
 * Copyright 1993, 1995 Christopher Seiwald.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

/*
 * filesys.h - FILENAME struct and OS specific file routines 
 */

/*
 * FILENAME - a name of a file, broken into <grist>dir/base/suffix(member)
 *
 * <grist> is salt to distinguish between targets that otherwise would
 * have the same name:  it never appears in the bound name of a target.
 * (member) is an archive member name: the syntax is arbitrary, but must 
 * agree in file_parse(), file_build() and the Jambase.
 *
 * On VMS, we keep track of whether the original path was a directory
 * (without a file), so that $(VAR:D) can climb to the parent.
 */

#ifndef FILESYS_DWA20011025_H
# define FILESYS_DWA20011025_H

# include "strings.h"

typedef struct _filename FILENAME;
typedef struct _filepart FILEPART;

struct _filepart {
	char	*ptr;
	int	len;
};

struct _filename {
	FILEPART	part[6];
# ifdef OS_VMS
	int		parent;
# endif

# define f_grist	part[0]
# define f_root		part[1]
# define f_dir		part[2]
# define f_base		part[3]
# define f_suffix	part[4]
# define f_member	part[5]

} ;

void file_build( FILENAME *f, string *file, int binding );
void file_build1( FILENAME *f, string *file );

void file_parse( char *file, FILENAME *f );
void file_parent( FILENAME *f );

void file_dirscan( char *dir, void (*func)( char *f, int s, time_t t ) );
void file_archscan( char *arch, void (*func)( char *f, int s, time_t t ) );

int file_time( char *filename, time_t *time );

#endif // FILESYS_DWA20011025_H
