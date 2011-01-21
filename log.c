#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <errno.h>
#include "log.h"

int logLevel;
FILE* fd = NULL;

void OpenLog(char* filename, int level)
{
	fd = fopen(filename, "a");
	if ( fd == NULL )
		fprintf(stderr, "Open log %s failed: %d\n", filename, errno);
	logLevel = level;
}

void CloseLog()
{
	if (fd) {
		fclose(fd);
		fd = NULL;
	}
}

void SetLogLevel(int level)
{
	logLevel = level;
}

void WriteLog(int priority, char* format, ...)
{
	va_list argptr;
	
	if ( fd == NULL )
		return;
 	
	if ( priority <= logLevel )
	{
		time_t now=time(NULL);	
		struct tm *ts=localtime(&now);
		if (ts)
			fprintf(fd, "%02d-%02d %02d:%02d:%02d # ",
				ts->tm_mon+1, ts->tm_mday,
				ts->tm_hour, ts->tm_min, ts->tm_sec);
 
		va_start(argptr, format);
		vfprintf(fd, format, argptr);
		va_end(argptr);
		fflush(fd);
	}
}

