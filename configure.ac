#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([test_mp3], [0.1], [m.kudimov at gmail.com])
AC_CONFIG_SRCDIR([src/to_mp3.cpp])
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CXX

# Checks for libraries.
AC_CHECK_LIB(pthread, pthread_create, [], [ echo "ERROR!: libpthread not found!"; exit -1], [])
AC_CHECK_LIB(mp3lame, lame_init, [LIBS=$LIBS], [ echo "ERROR!: libmp3lame not found!"; exit -1], [-Wl,-static -lmp3lame -Wl,-Bdynamic])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
