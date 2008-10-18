AC_INIT([blobber], [0.0], [bmuller@butterfat.net])
AC_DEFINE([PACKAGE_URL],["http://trac.butterfat.net/public/blobber"],[project url])
AM_CONFIG_HEADER(blobber/config.h)
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

# provide flag --enable-doxygen
HAVE_DOXYGEN="no"
AC_ARG_ENABLE(doxygen, AC_HELP_STRING([[--enable-doxygen]], [enable doxygen api docs output]),
[if test "$enableval" = "yes"; then
  AC_PATH_PROG([DOXYGEN],[doxygen],[false])
  if test "${DOXYGEN}" = "false" ; then
    AC_MSG_ERROR([You chose the --enable-doxygen option but you don't seem to have doxygen installed.])
  else
    HAVE_DOXYGEN="yes"
    AC_SUBST([DOXYGEN])
    HAVE_DOT="no"
    AC_PATH_PROG([DOT],[dot],[false])
    if test "${DOT}" != "false" ; then
      HAVE_DOT="YES"
      AC_SUBST([DOT])
    fi
    AC_SUBST([HAVE_DOT])
  fi
fi])
AM_CONDITIONAL([HAVE_DOXYGEN],[test "${HAVE_DOXYGEN}" = "yes"])

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


# Check for v4l headers
nov4l1=true
AC_CHECK_HEADER([linux/videodev.h], [
  AC_DEFINE([HAVE_V4L1], [1], [have v4l version 1 support])
  nov4l1=false
  AM_CONDITIONAL(HAVE_V4L1, true)
],[
  AM_CONDITIONAL(HAVE_V4L1, false)
])


AC_CHECK_HEADER([linux/videodev2.h], [
  AC_DEFINE([HAVE_V4L2], [1], [have v4l version 2 support])
  AM_CONDITIONAL(HAVE_V4L2, true)
], [
  if $nov4l1 ; then
    AC_MSG_ERROR([Could not find v4l (version 1 or 2) header file - this likely means your kernel wasn't compiled with v4l support.  See http://linuxtv.org/v4lwiki/index.php/Main_Page for more info.])
  fi
  AM_CONDITIONAL(HAVE_V4L2, false)
])


# enable modules as necessary
allmods=false
AC_ARG_ENABLE(all-modules, AC_HELP_STRING([[--enable-all-modules]], [enable all modules]), [allmods=true])

greenscreen=false
AC_ARG_ENABLE(greenscreen-module, AC_HELP_STRING([[--enable-greenscreen-module]], [enable greenscreen module]), [greenscreen=true])
AM_CONDITIONAL(GREENSCREEN, $greenscreen || $allmods)

multicoloredtag=false
AC_ARG_ENABLE(multicoloredtag-module, AC_HELP_STRING([[--enable-multicoloredtag-module]], [enable multi-color tagging module]), [multicoloredtag=true])
AM_CONDITIONAL(MULTICOLOREDTAG, $multicoloredtag || $allmods)

pong=false
AC_ARG_ENABLE(pong-module, AC_HELP_STRING([[--enable-pong-module]], [enable pong game module]), [pong=true])
AM_CONDITIONAL(PONG, $pong || $allmods)

invaders=false
AC_ARG_ENABLE(invaders-module, AC_HELP_STRING([[--enable-invaders-module]], [enable invaders game module]), [invaders=true])
AM_CONDITIONAL(INVADERS, $invaders || $allmods)

flower=false
AC_ARG_ENABLE(flower-module, AC_HELP_STRING([[--enable-flower-module]], [enable flower game module]), [flower=true])
AM_CONDITIONAL(FLOWER, $flower || $allmods)

refreshrate=false
AC_ARG_ENABLE(refreshrate-module, AC_HELP_STRING([[--enable-refreshrate-module]], [enable refresh-rate info module]), [refreshrate=true])
AM_CONDITIONAL(REFRESHRATE, $refreshrate || $allmods)

movableshapes=false
AC_ARG_ENABLE(movableshapes-module, AC_HELP_STRING([[--enable-movableshapes-module]], [enable movableshapes game module]), [movableshapes=true])
AM_CONDITIONAL(MOVABLESHAPES, $movableshapes || $allmods)

multilasertag=false
AC_ARG_ENABLE(multilasertag-module, AC_HELP_STRING([[--enable-multilasertag-module]], [enable multilasertag drawing module]), [multilasertag=true])
AM_CONDITIONAL(MULTILASERTAG, $multilasertag || $allmods)

colorrandomizer=false
AC_ARG_ENABLE(colorrandomizer-module, AC_HELP_STRING([[--enable-colorrandomizer-module]], [enable colorrandomizer game module]), [colorrandomizer=true])
AM_CONDITIONAL(COLORRANDOMIZER, $colorrandomizer || $allmods)

AC_CONFIG_FILES([
 Makefile
 blobber/Makefile
 modules/Makefile
 libblobber.pc
 Doxyfile
])
AC_OUTPUT


echo " ***"
echo " *** You are now ready to build blobber"
echo " *** Enter the following commands:"
echo " ***"
echo " *** $> make"
echo " *** $> sudo make install"
echo " ***"
echo " *** Report bugs at http://trac.butterfat.net/public/blobber"
echo " *** Thanks for using free (as in speech and beer) software."
echo " ***"
echo
