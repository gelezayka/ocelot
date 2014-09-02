#include <string>

class user {
	private:
		int id;
		bool leechstatus;
		bool protect_ip;
		std::string auth_key;
		struct {
			unsigned int leeching;
			unsigned int seeding;
		} stats;
	public:
		user(int uid, bool leech, bool protect, std::string);
		int get_id();
		bool is_protected();
		void set_protected(bool status);
		bool can_leech();
		void set_leechstatus(bool status);
		void decr_leeching();
		void decr_seeding();
		void incr_leeching();
		void incr_seeding();
		unsigned int get_leeching();
		unsigned int get_seeding();
		std::string get_auth_key();
		void set_auth_key(std::string key);
};
