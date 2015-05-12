/*  Implementation of getline taken of the internet and than hacked slightly by Paul Horton
 *  Useful for compiling code on solaris since getline appears to be a GNU/Linux specific function
 */

/* this code is an ad hoc fix for solaris not having the standard getline function */
#include <sys/types.h>
#include <utime.h>
#include <errno.h>
#include <cstdlib>
#include <cstdio>
//#include <config.h>

#ifndef HAVE_GETLINE
/* The interface here is that of GNU libc's getline */
//static ssize_t
inline ssize_t
getline (char **lineptr, size_t *n, FILE *stream)
{
#define EXPAND_CHUNK 16

 int n_read = 0;
 char *line = *lineptr;

 /* originally this block used g_return_val_if_fail to log failures, but the compiler could not find g_return_val_if_fail... */
 if( lineptr == NULL )  return -1;
 if( n == NULL )        return -1;
 if( stream == NULL )   return -1;
 if( (*lineptr == NULL) && (*n != 0) )  return -1;

 
#ifdef HAVE_FLOCKFILE
 flockfile (stream);
#endif  
 
 while (1)
   {
     int c;
     
#ifdef HAVE_FLOCKFILE
     c = getc_unlocked (stream);
#else
     c = getc (stream);
#endif      

     if (c == EOF)
       {
         if (n_read > 0)
          line[n_read] = '\0';
         break;
       }

     if (n_read + 2 >= (int) *n)
       {
        size_t new_size;

        if (*n == 0)
          new_size = 16;
        else
          new_size = *n * 2;

        if (*n >= new_size)    /* Overflowed size_t */
          line = NULL;
        else{
	  //          line = *lineptr ? realloc (*lineptr, new_size) : malloc (new_size);
          line = (char*) (  *lineptr ? realloc (*lineptr, new_size) : malloc (new_size)  );
	}

        if (line)
          {
            *lineptr = line;
            *n = new_size;
          }
        else
          {
            if (*n > 0)
              {
                (*lineptr)[*n - 1] = '\0';
                n_read = *n - 2;
              }
            break;
          }
       }

     line[n_read] = c;
     n_read++;

     if (c == '\n')
       {
         line[n_read] = '\0';
         break;
       }
   }

#ifdef HAVE_FLOCKFILE
 funlockfile (stream);
#endif

 return n_read - 1;
}
#endif /* ! HAVE_GETLINE */
