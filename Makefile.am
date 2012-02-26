AUTOMAKE_OPTIONS = subdir-objects 

AM_CPPFLAGS = $(GTCUE_CFLAGS)
GTCue_LDADD = $(GTCUE_LIBS)

bin_PROGRAMS = GTCue
GTCue_SOURCES =	src/main.h \
		src/main.cpp \
		src/JackClient/JackClient.h \
		src/JackClient/JackClient.cpp

dist_noinst_SCRIPTS = autogen.sh
