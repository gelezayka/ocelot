ACLOCAL_AMFLAGS = -I m4
CXX = clang++

bin_PROGRAMS = ocelot
ocelot_SOURCES = config.h db.cpp db.h events.cpp events.h misc_functions.cpp misc_functions.h\
	ocelot.cpp ocelot.h report.cpp report.h response.cpp response.h \
	schedule.cpp schedule.h user.cpp user.h worker.cpp worker.h log.cpp log.h md5.cpp md5.h \
	sha1.cpp sha1.h const_memory_range.h stdafx.cpp stdafx.h
nodist_ocelot_SOURCES = config.cpp

AM_CPPFLAGS = -std=c++11 -march=native -O2 -fvisibility=hidden -fvisibility-inlines-hidden -fomit-frame-pointer -fno-ident -pthread -Wall -Wfatal-errors -Wl,O1 -Wl,--as-needed $(BOOST_CPPFLAGS) -I$(MYSQLPP_INC_DIR) -I$(EV_INC_DIR) -I$(MYSQL_C_INC_DIR)
ocelot_LDFLAGS = -static-libstdc++ $(BOOST_IOSTREAMS_LIB) $(BOOST_SYSTEM_LIB) -L$(MYSQLPP_LIB_DIR) -pthread
ocelot_LDADD =  $(BOOST_IOSTREAMS_LIB) $(BOOST_SYSTEM_LIB) -lev -lmysqlpp -lutil
EXTRA_DIST = CHANGES LICENSE config.cpp.template
dist-hook:
	touch ${distdir}/configure
	patch -p2 -d ${distdir} --no-backup-if-mismatch < ../dist.patch
