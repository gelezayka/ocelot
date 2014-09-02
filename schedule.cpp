#include "ocelot.h"
#include "config.h"
#include "db.h"
#include "worker.h"
#include "events.h"
#include "schedule.h"

#include "log.h"

schedule::schedule(connection_mother * mother_obj, worker* worker_obj, config* conf_obj, mysql * db_obj) : mother(mother_obj), work(worker_obj), conf(conf_obj), db(db_obj) {
	counter = 0;
	last_opened_connections = 0;

	next_reap_peers = time(NULL) + conf->reap_peers_interval + 40;
}
//---------- Schedule - gets called every schedule_interval seconds
void schedule::handle(ev::timer &watcher, int events_flags) {
	stats.connection_rate = (stats.opened_connections - last_opened_connections) / conf->schedule_interval;
	if (counter % 20 == 0) {
		wlog(L_INFO, "Schedule run #%d - open: %d, opened: %d, speed: %d/s", counter, 
			stats.open_connections, stats.opened_connections, stats.connection_rate);
	}

	if (work->get_status() == CLOSING && db->all_clear()) {
		wlog(L_CRIT, "all clear, shutting down");
		exit(0);
	}

	last_opened_connections = stats.opened_connections;

	db->flush();


	time_t cur_time = time(NULL);

	if (cur_time > next_reap_peers) {
		work->start_reaper();
		next_reap_peers = cur_time + conf->reap_peers_interval;
	}

	counter++;
}
