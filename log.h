#ifndef _LOG_H_
#define _LOG_H_

#define	MAX_LOG_BUF	1024

#define L_CRIT          0x0001
#define L_INFO          0x0002
#define L_WARN          0x0004
#define L_DEBUG1        0x0008
#define L_PCAP          0x0010
#define L_CACHE         0x0020
#define L_DHCP          0x0040
#define L_RADIUS        0x0080
#define L_RADACC        0x0100
#define L_MIB         	0x0200
#define L_DEBUG         0x0400

extern int LOG_MASK;
extern const char *progname;

void add_log_mask(const char *level);
void set_log_mask(int l);
void set_log_level(int l);
void set_log_verbose(int l);

#define wlog(type, f, ...) do { if (LOG_MASK & type) WLOG(type, f, ## __VA_ARGS__); } while(0)

void WLOG(int type,const char *f, ...);

#endif
