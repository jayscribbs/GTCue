AUTOMAKE_OPTIONS = subdir-objects 

AM_CPPFLAGS = $(GTCUE_CFLAGS)
GTCue_LDADD = $(GTCUE_LIBS)

bin_PROGRAMS = GTCue
GTCue_SOURCES =	src/main.h \
		src/main.cpp \
		src/Configuration.h\
		src/Configuration.cpp\
		src/JackClient/JackClient.h \
		src/JackClient/JackClient.cpp\
		src/Gui/MainWindow.h\
		src/Gui/MainWindow.cpp

dist_noinst_SCRIPTS = autogen.sh
