#include <errno.h>
#include <stdarg.h>
#include "ourhdr.h"

static void err_doit(int , const char *, va_list);

/*Nonfatal error related to a system call
 print a message and return*/
void err_ret(const  char *fmt, ...)
{
	va_list	ap;
	va_start(ap, fmt);
	err_doit(1, fmt, ap);
	va_end(ap);
	return;
}

/*Fatal error related to a system call*/
void err_sys(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	err_doit(1, fmt, ap);
	va_end(ap);
	exit(1);
}

/*fatal error related to a system call.
 print a mesg, dump core , and terminate*/
void err_dump(const char *fmt, ...)
{
	va_list		ap;
	va_start(ap, fmt);
	err_doit(1, fmt, ap);
	va_end(ap);
	abort();		/*dump core and terminate*/
	exit(1);
}

/*non fatal error unrelated to a system call
print a msg and return*/
void err_msg(const char *fmt, ...)
{
	va_list		ap;
	va_start(ap, fmt);
	err_doit(0, fmt, ap);
	va_end(ap);
	return;
}

/* Fatal error unrelated to a system call
print a msg and terminate*/
void err_quit(const char * fmt, ...)
{
	va_list		ap;
	va_start(ap, fmt);
	err_doit(0, fmt, ap);
	va_end(ap);
	exit(1);
}

/*Print a message and return to caller
 * Caller specifies "errnoflag"*/
 static void err_doit(int errflag, const char * fmt, va_list ap)
 {
	 int	errno_save;
	 char	buf[MAXLINE];
	 errno_save = errno;	/*value caller might want printed*/
	 vsprintf(buf, fmt, ap);
	 if(errflag)
		 sprintf(buf + strlen(buf), ": %s", strerror(errno_save));
	strcat(buf, "\n");
	fflush(stdout);			/*in case stdout and stderr are the same*/
	fputs(buf, stderr);
	fflush(NULL);		/*flushes all stdio output stream*/
	return;
 }

#ifdef PATH_MAX
static int pathmax = PATH_MAX;
#else
static int pathmax = 0;
#endif

#define PATH_MAX_GUESS	1024
 char *path_alloc(int *size)
 {
	 char *ptr;
	 if (pathmax == 0)
	 {
		 errno = 0;
		 if ((pathmax = pathconf("/", _PC_PATH_MAX)) < 0)
		 {
			 if(errno == 0)
				 pathmax = PATH_MAX_GUESS;
			 else
				 err_sys("pathconf error for _PC_PATH_MAX");
		 }
		 else pathmax++;	/*add one since it's relative to root*/
	 }
	 if((ptr = malloc(pathmax + 1)) == NULL)
		 err_sys("malloc error for pathname");
	 if(size != NULL)
		 *size = pathmax + 1;
	 return(ptr);
 }

