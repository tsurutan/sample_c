#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>

#include "ourhdr.h"

typedef int Myfunc(const char *, const struct stat *, int);

static Myfunc myfunc;
static int myftw(char *, Myfunc *);
static int dopath(Myfunc *, char *);

static long nreg, ndir, nblk, nchr, nfifo, nslink, nsock, ntot;

int main(int argc, char **argv) {
  int ret;
  if(argc != 2) {
    printf("error");
    exit(1);
  }

  ret = myftw(argv[1], myfunc);

  if((ntot = nreg + ndir + nblk + nchr + nfifo + nslink + nsock) == 0) {
    ntot = 1;
  }

  printf("regular files  = %7ld, %5.2f %%\n", nreg, nreg * 100.0 / ntot);
  printf("directories    = %7ld, %5.2f %%\n", ndir, ndir * 100.0 / ntot);
  printf("block special  = %7ld, %5.2f %%\n", nblk, nblk * 100.0 / ntot);
  printf("char special   = %7ld, %5.2f %%\n", nchr, nchr * 100.0 / ntot);
  printf("FIFOs          = %7ld, %5.2f %%\n", nfifo, nfifo * 100.0 / ntot);
  printf("symbolic links = %7ld, %5.2f %%\n", nslink, nslink * 100.0 / ntot);
  printf("sockets        = %7ld, %5.2f %%\n", nsock, nsock * 100.0 / ntot);

  exit(ret);
}

#define FTW_F 1
#define FTW_D 2
#define FTW_DNR 3
#define FTW_NS 4

static char *fullpath;

static int myftw(char *pathname, Myfunc *func) {
  fullpath = path_alloc(NULL);
  /* strcpy(fullpath, pathname); */
  return(dopath(func, pathname));
}

static int dopath(Myfunc* func, char *pathname) {
  struct stat statbuf;
  struct dirent *dirp;
  DIR *dp;
  int ret;
  char *ptr;

  if(lstat(pathname, &statbuf) < 0) {
    return(func(pathname, &statbuf, FTW_NS));
  }

  if(S_ISDIR(statbuf.st_mode) == 0) {
    return (func(pathname, &statbuf, FTW_F));
  }

  if((ret = func(pathname, &statbuf, FTW_D)) != 0) {
    return(ret);
  }

  chdir(pathname);

  write(1, "==========\n", strlen("==========\n"));
  write(1, pathname, strlen(pathname));
  if((dp = opendir(".")) == NULL) {
    return(func(pathname, &statbuf, FTW_DNR));
  }

  while((dirp = readdir(dp)) != NULL) {
    if(strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0) { continue; }
    write(1, dirp->d_name, strlen(dirp->d_name));
    write(1, "\n", strlen("\n"));
    if((ret=dopath(func, dirp->d_name)) != 0) { break; }
  }
  chdir("..");
  if(closedir(dp) < 0) {
    printf("error colose dir");
    exit(1);
  }

  return(ret);
}

static int myfunc(const char *pathname, const struct stat *statptr, int type) {
  switch(type) {
    case FTW_F:
      switch(statptr->st_mode & S_IFMT) {
        case S_IFREG: nreg++; break;
        case S_IFBLK: nblk++; break;
        case S_IFCHR: nchr++; break;
        case S_IFIFO: nfifo++; break;
        case S_IFLNK: nslink++; break;
        case S_IFSOCK: nsock++; break;
        case S_IFDIR:
                       printf("error S_IFDIR for %s", pathname);
                       break;
      }
    case FTW_D:
      ndir++;
      break;
    case FTW_DNR:
      printf("cant read directory %s", pathname);
      exit(1);
      break;
    case FTW_NS:
      printf("stat error for %s", pathname);
      exit(1);
      break;
    default:
      printf("wunknown type %d for pathname %s", type, pathname);
      exit(1);
      break;
  }
  return(0);
}
