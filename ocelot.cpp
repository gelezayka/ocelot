#include "ocelot.h"
#include "config.h"
#include "db.h"
#include "worker.h"
#include "events.h"
#include "schedule.h"
#include <sysexits.h>
#include <libutil.h>
#include <err.h>

#include "log.h"

static connection_mother *mother;
static worker *work;
struct stats stats;
const char *progname = "ocelot";
const char *pidfile = "/var/run/ocelot.pid";

char *config_file = NULL;
int bg = 1;
int debug = 0;

static void sig_handler(int sig)
{
	wlog(L_INFO, "Caught SIGINT/SIGTERM");
	if (work->signal(sig)) {
		exit(0);
	}
}

static void Usage() {
        fprintf(stderr, "usage: ocelot [OPTIONS]\n");
        fprintf(stderr, " -d            - run in debug mode\n");
        fprintf(stderr, " -f            - run in foreground\n");
        fprintf(stderr, " -c file       - define config for usage\n"
		"[-p<port>]\tTCP port number to listen on (default: %d)\n"
	    , 34000);
        exit(EX_USAGE);
}

void parse_cmd(int ac, char *av[], config &c) {
        int ch;
        /* Parse flags */
        while ((ch = getopt(ac, av, "h:d:f:p:c:")) != -1) {
                switch (ch) {
                case 'd':
                        debug = 1;
                        break;
                case 'f':
                        bg = 0;
			break;
                case 'p':
                        c.port = atoi(optarg);
                        break;
                case 'c':
                        config_file = strdup(optarg);
                        break;
                case 'h':
                case '?':
                default:
                        Usage();
                }
        }
}



int main(int argc, char **argv)
{
	config conf;

        /* parse args and option there */
	parse_cmd(argc, argv, conf);

        if ((debug == 1) || (bg == 0)) {
	    set_log_level(L_DEBUG);
            set_log_verbose(1);
            add_log_mask("DEBUG");
	}

        add_log_mask("CRIT");
	add_log_mask("WARN");
        add_log_mask("INFO");

	// we don't use printf so make cout/cerr a little bit faster
	std::ios_base::sync_with_stdio(false);

	
	signal(SIGINT, sig_handler);
	signal(SIGTERM, sig_handler);

	bool verbose = false; //debug > 0;

	mysql db(conf.mysql_db, conf.mysql_host, conf.mysql_username, conf.mysql_password, conf.clean_peer_on_start);
	if (!db.connected()) {
		wlog(L_INFO, "Exiting");
		return 0;
	}
	db.verbose_flush = verbose;


	if ((debug == 0) && (bg == 1)) 
	{
    	    int alien;
    	    daemon(0, 0);

    	    /* Create and lock pidfile */     
    	    struct pidfh *pfh = pidfile_open(pidfile, 0600, &alien);
    	    if (pfh == NULL) {
                if (errno == EEXIST)
                    errx(EXIT_FAILURE, "daemon already running at pid %d.", alien);
                else                                        
                    err(EXIT_FAILURE, "can't create pidfile");
    	    }
                                                                            
    	    pidfile_write(pfh);

	}
	std::vector<std::string> whitelist;
	//db.load_whitelist(whitelist);
	//std::cout << "Loaded " << whitelist.size() << " clients into the whitelist" << std::endl;
	if (whitelist.size() == 0) {
		wlog(L_INFO, "Assuming no whitelist desired, disabling");
	}

	user_list users_list;
	db.load_users(users_list);
	wlog(L_INFO, "Loaded %d users", users_list.size());

	torrent_list torrents_list;
	db.load_torrents(torrents_list);
	wlog(L_INFO, "Loaded %d torrents", torrents_list.size());

	db.load_peers(torrents_list, users_list);

	stats.open_connections = 0;
	stats.opened_connections = 0;
	stats.connection_rate = 0;
	stats.leechers = 0;
	stats.seeders = 0;
	stats.announcements = 0;
	stats.succ_announcements = 0;
	stats.scrapes = 0;
	stats.bytes_read = 0;
	stats.bytes_written = 0;
	stats.start_time = time(NULL);

	// Create worker object, which handles announces and scrapes and all that jazz
	work = new worker(torrents_list, users_list, whitelist, &conf, &db);

	// Create connection mother, which binds to its socket and handles the event stuff
	mother = new connection_mother(work, &conf, &db);

	return 0;
}
