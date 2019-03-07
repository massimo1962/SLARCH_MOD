/***************************************************************************
 * quality.c
 *
 * Log handling routines for libslink
 *
 * Written by Stefano Pintore, INGV
 *
 * modified: 2013.332
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

#include "libslink.h"


/* Logging parameters */
typedef struct SLqc_s
{
  const char * qcfilename;
  int  latency_threshold;
  FILE * qcfile;
} SLqc;

SLqc qcs={NULL,0,NULL};

void sl_qcinit(const char *qcfile) {
  //int qcfd;
      /* Open the qc file */
  //if ( ( qcs.qcfd = slp_openfile (qcfile, 'a')) < 0 )
if ( ( qcs.qcfile = fopen (qcfile, "a")) < 0 )      
      
    {
      sl_log (0, 0, "cannot open qc file for writing \n");
      //return -1;
    }
  qcs.qcfilename=qcfile;
//  sl_log (0, 0,  "Appending data to qc file \n");
    
}


int
sl_qcsave ( const char *content)
{
  //SLstream *curstream;
  char line[100];
  int linelen;
  
  char message[100];
  //curstream = slconn->streams;
  
  /* Open the qc file */
  if (qcs.qcfile < 0 )
    {
      snprintf(message,100,"Unable to write in %s \n",qcs.qcfilename);
      sl_log (0, 0,  message);
      return -1;
    }
  
//  sl_log (0, 0,  "Appending data to qc file: %s \n", qcs.qcfilename);
  fprintf(qcs.qcfile, "%s \n", content);

  return 0;
  
  
}