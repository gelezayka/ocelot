#ifndef OCELOT_CONFIG_H
#define OCELOT_CONFIG_H

#include <string>

class config {
	public:
		bool clean_peer_on_start;
		std::string host;
		unsigned int port;
		unsigned int max_connections;
		unsigned int max_read_buffer;
		unsigned int max_request_size;
		unsigned int timeout_interval;
		unsigned int schedule_interval;
		unsigned int max_middlemen;

		unsigned int announce_interval;
		unsigned int peers_timeout;

		unsigned int reap_peers_interval;
		unsigned int del_reason_lifetime;

		// MySQL
		std::string mysql_db;
		std::string mysql_host;
		std::string mysql_username;
		std::string mysql_password;
		std::string site_password;
		std::string report_password;
		std::string torrent_pass_private_key;

		config();
};

#endif
