AC_PREREQ([2.63])

AC_INIT([GTCue], [0.0], [jayscribbs@gmail.com], GTCue)

AM_INIT_AUTOMAKE([no-define])

AC_CONFIG_HEADERS([config.h])

#Check for Programs
AC_PROG_CXX

#Check for libraries
#gtkmm, sndfile, and jack
PKG_CHECK_MODULES([GTCUE],[gtkmm-3.0 >= 3.2.0] [sndfile >= 1.0.24] [jack >= 1.9.7]) 

#Set compilation flags
#CFLAGS=$GTCUE_CFLAGS
#LIBS=$GTCUE_LIBS

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
