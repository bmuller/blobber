AC_INIT([nerdtag], [0.0], [bmuller@butterfat.net])
AC_DEFINE([PACKAGE_URL],["http://trac.butterfat.net/public/nerdtag"],[project url])
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE([no-dist])

AM_INIT_AUTOMAKE()

AC_PROG_CXX
AC_PROG_CXXCPP
AC_LANG_CPLUSPLUS
AC_CANONICAL_HOST
AC_PROG_INSTALL
AM_PROG_LIBTOOL

AC_HEADER_STDC

# provide flag --enable-debug
AC_ARG_ENABLE(debug, AC_HELP_STRING([[--enable-debug]], [enable debugging output to stdout]),
[if test "$enableval" = "yes"; then
    DEBUG="yes"
    CPPFLAGS="$CPPFLAGS -DDEBUG"
fi]
)

PKG_CHECK_MODULES([GTK],[gtkmm-2.4 >= 2.8.0],,[
 AC_MSG_ERROR([No GTK C++ library found (version 2.8.0 or higher). You can get one from http://www.gtkmm.org])
])

AC_CONFIG_FILES([
 Makefile
 stactiverecord/Makefile
 examples/Makefile
 tests/Makefile
])
AC_OUTPUT


echo " ***"
echo " *** You are now ready to build StactiveRecord"
echo " *** Enter the following commands:"
echo " ***"
echo " *** $> make"
echo " *** $> su root"
echo " *** $> make install"
echo " ***"
echo " *** Report bugs at https://www.butterfat.net/tracker/butterfat/index"
echo " *** Thanks for using free (as in speech, not beer) software."
echo " ***"
echo
