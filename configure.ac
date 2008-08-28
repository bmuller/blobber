AC_INIT([blobber], [0.0], [bmuller@butterfat.net])
AC_DEFINE([PACKAGE_URL],["http://trac.butterfat.net/public/blobber"],[project url])
AM_CONFIG_HEADER(src/config.h)
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

# option for compiler to be nitpicky about code
nitpick=false
AC_ARG_ENABLE([nitpicking],
 AC_HELP_STRING([--enable-nitpicking],[make compiler somewhat overly fastidious about the code it deals with]),
 [ test "$enableval" = "no" || nitpick=true ]
)
if $nitpick ; then
 CPP_NITPICK="-Wall -Wextra -Wundef -Wshadow -Wunsafe-loop-optimizations -Wconversion -Wmissing-format-attribute"
 CPP_NITPICK="$CPP_NITPICK -Wredundant-decls -ansi -Wmissing-noreturn"
 CPPFLAGS="$CPPFLAGS $CPP_NITPICK"
fi

PKG_CHECK_MODULES([GTK],[gtkmm-2.4 >= 2.8.0],,[
 AC_MSG_ERROR([No GTK C++ library found (version 2.8.0 or higher). You can get one from http://www.gtkmm.org])
])

AC_CONFIG_FILES([
 Makefile
 src/Makefile
])
AC_OUTPUT


echo " ***"
echo " *** You are now ready to build blobber"
echo " *** Enter the following commands:"
echo " ***"
echo " *** $> make"
echo " ***"
echo " *** Report bugs at http://trac.butterfat.net/public/blobber"
echo " *** Thanks for using free (as in speech and beer) software."
echo " ***"
echo
