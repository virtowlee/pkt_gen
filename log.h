#ifndef _LOG_H
#define _LOG_H 1

/* priorities already defined in sys/syslog.h */
#ifndef _SYS_SYSLOG_H
#define LOG_EMERG       0       /* system is unusable */
#define LOG_ALERT       1       /* action must be taken immediately */
#define LOG_CRIT        2       /* critical conditions */
#define LOG_ERR         3       /* error conditions */
#define LOG_WARNING     4       /* warning conditions */
#define LOG_NOTICE      5       /* normal but significant condition */
#define LOG_INFO        6       /* informational */
#define LOG_DEBUG       7       /* debug-level messages */
#endif

void OpenLog(char* filename, int level);
void CloseLog();
void SetLogLevel(int level);
void WriteLog(int priority, char* format, ...);

#endif /* log.h */
