#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <stdarg.h>

#include <syslog.h>
#include <pthread.h>

#include "log.h"

int log_lock(void);
int log_unlock(void);

struct { int code; int facility; const char *str; const char *cfg; } l_strings[] = {
        { L_CRIT, LOG_LOCAL1, "[crit]", "CRIT" },
        { L_INFO, LOG_LOCAL1, "[info]", "INFO" },
        { L_WARN, LOG_LOCAL1, "[warn]", "WARN" },
        { L_DEBUG1, LOG_LOCAL1, "[debug]", "DEBUG1" },
        { L_PCAP, LOG_LOCAL2, "[pcap]", "PCAP" },
        { L_CACHE, LOG_LOCAL3, "[cache]", "CACHE" },
        { L_DHCP, LOG_LOCAL4, "[dhcp]", "DHCP" },
        { L_RADIUS, LOG_LOCAL5, "[radius]", "RADIUS" },
        { L_RADACC, LOG_LOCAL5, "[radacc]", "RADACC" },
        { L_MIB, LOG_LOCAL6, "[mib]", "MIB" },
        { L_DEBUG, LOG_LOCAL7, "[debug]", "DEBUG" },
        { 0, 0, NULL, NULL },
    };

pthread_mutex_t logs_mtx = PTHREAD_MUTEX_INITIALIZER;

int LOG_LEVEL = 0;
int verbose = 0;
int LOG_MASK = 0;

void
add_log_mask(const char *level)
{
        int i;
        for(i=0 ; l_strings[i].cfg ; i++)
            if(strcmp(l_strings[i].cfg, level) == 0)
		    LOG_MASK = LOG_MASK | l_strings[i].code;
}

void
set_log_mask(int l)
{
    LOG_MASK = l;
}

void
set_log_level(int l)
{
    LOG_LEVEL = l;
}

void
set_log_verbose(int l) 
{
    verbose = l;
}



int 
log_lock(void) 
{
        return pthread_mutex_lock(&logs_mtx);
}

int 
log_unlock(void)
{
        return pthread_mutex_unlock(&logs_mtx);
}

static int log_open = 0;

static 
const char *l2string(int code)
{
        int i;
        for(i=0 ; l_strings[i].str ; i++) {
            if(l_strings[i].code == code)
                return l_strings[i].str;
        }
        return "NaN";
}

static 
int l2facility(int code)
{
        int i;
        for(i=0 ; l_strings[i].str ; i++) {
            if(l_strings[i].code == code)
                return l_strings[i].facility;
        }
        return LOG_LOCAL1;
}

void 
WLOG(int type,const char *f, ...)
{
        va_list ap;
	char buf[MAX_LOG_BUF];
        log_lock();

	if(log_open == 0) {
	    if(verbose > 0) {
		openlog(progname, LOG_PERROR | LOG_PID, LOG_DAEMON);
	    } else {
		openlog(progname, LOG_PID, LOG_DAEMON);
	    }
	    log_open = 1;
	}
        if(LOG_MASK & type) {
		snprintf(buf, MAX_LOG_BUF, "%s: %s", l2string(type), f);
                va_start(ap, f);
		vsyslog(LOG_INFO | l2facility(type), buf, ap);
                va_end(ap); 
        }
        log_unlock();
}

