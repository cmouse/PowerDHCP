AM_CXXFLAGS = -I../includes $(BOOST_PROGRAM_OPTIONS_CFLAGS) -DSYSCONFDIR=\"${sysconfdir}\" -DLIBDIR=\"${libdir}\" $(PIE_CFLAGS) $(PTHREAD_CFLAGS)
noinst_LTLIBRARIES=libpowerdhcp.la
libpowerdhcp_la_SOURCES=configuration.cc initialize.cc misc.cc ../includes/powerhdcp.hh ../includes/definitions.hh ../includes/configuration.hh ../includes/logging.hh
