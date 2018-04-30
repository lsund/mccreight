/*
  Copyright (c) 2003 by Stefan Kurtz and The Institute for
  Genomic Research.  This is OSI Certified Open Source Software.
  Please see the file LICENSE for licensing information and
  the file ACKNOWLEDGEMENTS for names of contributors to the
  code base.
*/

//\Ignore{

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include "types.h"
#include "megabytes.h"
#include "debug.h"

#define MAXMAPPEDFILES 32

//}

/*EE
  This file contains functions to map files to memory, to store pointers to
  the corresponding secondary memory, and to maintain the size of the mapped
  memory. The arguments \texttt{file} and \texttt{line} (if they occur)
  are always the filename and the linenumber the function is called from.
  To supply the arguments, we recommend to call the corresponding functions
  via some useful macros, as defined in the file \texttt{spaceman.h}.
  \begin{enumerate}
  \item
  The function \texttt{creatememorymap} should be called via the macro
  \texttt{CREATEMEMORYMAP}.
  \item
  The function \texttt{deletememorymap} should be called
  via the macro \texttt{DELETEMEMORYMAP}.
  \end{enumerate}
*/

/*
  Pointers to the memory areas, initialized to NULL, i.e. not occupied.
*/

static void *memoryptr[MAXMAPPEDFILES] = {NULL};

static Uint currentspace = 0,              // currently mapped num of bytes
            spacepeak = 0,                 // maximally mapped num of bytes
            mappedbytes[MAXMAPPEDFILES] = {0};  // size of the memory map

/*
  The following two tables store important information to
  generate meaningfull error messages.
*/


/*
  file where the mmap was created
*/

static char *filemapped[MAXMAPPEDFILES] = {NULL};

/*
  line where the mmap was created
*/

static Uint linemapped[MAXMAPPEDFILES] = {0};

/*
  The following two functions \texttt{mmaddspace} and \texttt{mmsubtractspace}
  maintain the variables \texttt{currentspace} and \texttt{spacepeak}.
*/

static void mmaddspace(Uint space)
{
  currentspace += space;
  if(currentspace > spacepeak)
  {
    spacepeak = currentspace;
    DEBUG1(2,"# mmap spacepeak = %.2f reached\n",MEGABYTES(spacepeak));
  }
}

static void mmsubtractspace(Uint space)
{
  currentspace -= space;
}

/*EE
  The following function opens a file, and stores the size of the
  file in \texttt{numofbytes}. It returns the file descriptor of the
  file, or a negative error code if something went wrong.
*/

Sint simplefileOpen(char *filename,Uint *numofbytes)
{
  int fd;
  struct stat buf;

  if((fd = open(filename,O_RDONLY)) == -1)
  {
     fprintf(stderr, "cannot open file \"%s\"",filename);
     return -1;
  }
  if(fstat(fd,&buf) == -1)
  {
     fprintf(stderr, "cannot access status for file \"%s\"",filename);
     return -2;
  }
  if ( buf.st_size > (~((Uint)0)) )
  {
    fprintf(stderr, "maximum file size exceeded for \"%s\"",filename);
    return -2;
  }

  *numofbytes = (Uint) buf.st_size;
  return (Sint) fd;
}

/*EE
  The following function creates a memory map for a given file
  descriptor \texttt{fd}. \texttt{writemap} is true iff the map
  should be writable, and \texttt{numofbytes} is the
  size of the file to be mapped.
*/

/*@null@*/ void *creatememorymapforfiledesc(char *file,Uint line,Sint fd,
                                            Bool writemap,Uint numofbytes)
{
  DEBUG1(2,"# creatememorymapforfiledesc %ld\n",(Slong) fd);
  if(numofbytes == 0)
  {
    fprintf(stderr, "creatememorymap: file is empty");
    return NULL;
  }
  if(fd < 0)
  {
    fprintf(stderr, "creatememorymap: filedescriptor %ld negative",(Slong) fd);
    return NULL;
  }
  if(fd >= MAXMAPPEDFILES)
  {
    fprintf(stderr, "creatememorymap: filedescriptor %ld is too large",(Slong) fd);
    return NULL;
  }
  if(memoryptr[fd] != NULL)
  {
    fprintf(stderr, "creatememorymap: filedescriptor %ld already in use",(Slong) fd);
    return NULL;
  }
  mmaddspace(numofbytes);
  DEBUG2(2,"# memorymap:fd=%ld: %lu bytes\n",(Slong) fd,
                                             (Ulong) numofbytes);
  mappedbytes[fd] = numofbytes;
  memoryptr[fd]
    = (void *) mmap(0,
                    (size_t) numofbytes,
                    writemap ? (PROT_READ | PROT_WRITE) : PROT_READ,
                    MAP_PRIVATE,
                    fd,
                    (off_t) 0);
  if(memoryptr[fd] == (void *) MAP_FAILED)
  {
    fprintf(stderr, "memorymapping for filedescriptor %ld failed",(Slong) fd);
    return NULL;
  }
  filemapped[fd] = file;
  linemapped[fd] = line;
  return memoryptr[fd];
}

/*EE
  The following function returns a memory map for a given filename, or
  \texttt{NULL} if something went wrong.
*/

/*@null@*/ void *creatememorymap(char *file,Uint line,char *filename,
                                 Bool writemap,Uint *numofbytes)
{
  int fd;

  DEBUG3(2,"\n# creatememorymap(file=%s,line=%ld) for file %s:\n",
              file,(Slong) line,
              filename);
  fd = simplefileOpen(filename,numofbytes);
  if(fd < 0)
  {
    return NULL;
  }
  return creatememorymapforfiledesc(file,line,fd,writemap,*numofbytes);
}

/*EE
  The following function unmaps the memorymap referenced by
  \texttt{mappedfile}. It returns a negative value if the
  \texttt{mappedfile} is \texttt{NULL}, or the corresponding
  filedescriptor cannot be found, or the \texttt{munmap} operation
  fails.
*/

Sint deletememorymap(char *file,Uint line,void *mappedfile)
{
  int fd;

  if(mappedfile == NULL)
  {
    fprintf(stderr, "%s: l. %ld: deletememorymap: mappedfile is NULL",
             file,(Slong) line);
    return -1;
  }
  for(fd=0; fd<MAXMAPPEDFILES; fd++)
  {
    if(memoryptr[fd] == mappedfile)
    {
      break;
    }
  }
  if(fd == MAXMAPPEDFILES)
  {
    fprintf(stderr, "%s: l. %ld: deletememorymap: cannot find filedescriptor for given address",
           file,(Slong) line);
    return -2;
  }
  if(munmap(memoryptr[fd],(size_t) mappedbytes[fd]) != 0)
  {
    fprintf(stderr, "%s: l. %ld: deletememorymap: munmap failed:"
           " mapped in file \"%s\",line %lu",
            file,
            (Slong) line,
            filemapped[fd],
            (Ulong) linemapped[fd]);
    return -3;
  }
  DEBUG4(2,"# file \"%s\", line %ld: deletememorymap:fd=%ld (%lu) bytes:\n",
          file,
          (Slong) line,
          (Slong) fd,
          (Ulong) mappedbytes[fd]);
  DEBUG2(2,"# mapped in file \"%s\", line %lu\n",filemapped[fd],
                                                 (Ulong) linemapped[fd]);
  memoryptr[fd] = NULL;
  mmsubtractspace(mappedbytes[fd]);
  mappedbytes[fd] = 0;
  if(close(fd) != 0)
  {
    fprintf(stderr, "cannot close file \"%s\"",filemapped[fd]);
    return -4;
  }
  return 0;
}

/*EE
  The following function checks if all files previously mapped, have
  been unmapped. If there is a file that was not unmapped, then
  an error is reported accordingly. We recommend to call this function
  before the program terminates. This easily allows to discover
  space leaks.
*/

void mmcheckspaceleak(void)
{
  int fd;

  for(fd=0; fd<MAXMAPPEDFILES; fd++)
  {
    if(memoryptr[fd] != NULL)
    {
      fprintf(stderr,"space leak: memory for filedescriptor %ld not freed\n",
              (Slong) fd);
      fprintf(stderr,"mapped in file \"%s\", line %lu\n",
              filemapped[fd],
              (Ulong) linemapped[fd]);
      exit(EXIT_FAILURE);
    }
  }
}

/*EE
  The following function frees the space for all memory maps
  which have not already been unmapped.
*/

Sint mmwrapspace(void)
{
  int fd;

  for(fd=0; fd<MAXMAPPEDFILES; fd++)
  {
    if(memoryptr[fd] != NULL)
    {
      if(munmap(memoryptr[fd],(size_t) mappedbytes[fd]) != 0)
      {
        fprintf(stderr, "mmwrapspace: munmap failed: mapped in file \"%s\", line %lu",
               filemapped[fd],
               (Ulong) linemapped[fd]);
        return -1;
      }
      memoryptr[fd] = NULL;
      DEBUG4(2,"# file \"%s\", line %ld: munmap:fd=%ld (%lu) bytes:\n",
              __FILE__,(Slong) __LINE__,(Slong) fd,
              (Ulong) mappedbytes[fd]);
      DEBUG2(2,"# mapped in file \"%s\", line %lu\n",filemapped[fd],
                                                     (Ulong) linemapped[fd]);
      if(close(fd) != 0)
      {
        fprintf(stderr, "cannot close file \"%s\"",filemapped[fd]);
        return -4;
      }
      mmsubtractspace(mappedbytes[fd]);
      memoryptr[fd] = NULL;
      mappedbytes[fd] = 0;
      filemapped[fd] = NULL;
      linemapped[fd] = 0;
    }
  }
  return 0;
}

/*EE
  The following function shows the space peak in megabytes on \texttt{stderr}.
*/

void mmshowspace(void)
{
  fprintf(stderr,"# mmap space peak in megabytes: %.2f\n",MEGABYTES(spacepeak));
}

/*EE
  The following function returns the space peak in bytes.
*/

Uint mmgetspacepeak(void)
{
  return spacepeak;
}
