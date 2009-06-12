AC_INIT([blobber], [0.0], [bmuller@butterfat.net])
AC_DEFINE([PACKAGE_URL],["http://trac.butterfat.net/public/blobber"],[project url])
AM_CONFIG_HEADER(blobber/config.h)
AM_INIT_AUTOMAKE()

AC_CONFIG_MACRO_DIR([m4])

AC_PROG_CXX
AC_PROG_CXXCPP
AC_LANG_CPLUSPLUS
AC_CANONICAL_HOST
AC_PROG_INSTALL
AM_PROG_LIBTOOL

AC_HEADER_STDC

# these might eventually have configure switches so they can be user specified so leave here
AC_DEFINE([DEFAULT_DEVICE], ["/dev/video0"], [The default device])
AC_DEFINE([DEFAULT_CRITERIA_WINDOW_SIZE], ["15"], [The default window size for criteria for POI])

# provide flag --enable-debug
AC_ARG_ENABLE(debug, AC_HELP_STRING([[--enable-debug]], [enable debugging output to stdout]),
[if test "$enableval" = "yes"; then
    DEBUG="yes"
    CPPFLAGS="$CPPFLAGS -DDEBUG"
fi]
)

# provide flag --enable-doxygen
HAVE_DOXYGEN="no"
AC_ARG_ENABLE(doxygen, AC_HELP_STRING([[--enable-doxygen]], [enable doxygen api docs output with make dox]),
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
v4l1=false
AC_ARG_ENABLE(v4l1, AC_HELP_STRING([[--enable-v4l1]], [enable v4l version 1 support]),
  [AC_CHECK_HEADER([linux/videodev.h], [AC_DEFINE([HAVE_V4l1], [1], [[have v4l version 1 support]])] [v4l1=true], [ 
    AC_MSG_ERROR([Could not find v4l version 1 header file - please reconfigure without --enable-v4l1])]
  )]
)
AM_CONDITIONAL(HAVE_V4L1, $v4l1)

AC_CHECK_HEADER([linux/videodev2.h], [AC_DEFINE([HAVE_V4L2], [1], [[have v4l version 2 support]])] , [
 AC_MSG_ERROR([[Could not find v4l2 header file - this likely means your kernel wasn't compiled with v4l support.  See http://linuxtv.org/v4lwiki/index.php/Main_Page for more info.]])
])

# enable libv4lconvert
AC_CHECK_HEADER([libv4lconvert.h], [], AC_MSG_ERROR([Could not find libv4lconvert header file - please install the libv4lconvert development headers.]))
AC_CHECK_LIB([v4lconvert], [v4lconvert_create], [], AC_MSG_ERROR([Could not find libv4lconvert library - please install the libv4lconvert library]))
 
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

duckhunt=false
AC_ARG_ENABLE(duckhunt-module, AC_HELP_STRING([[--enable-duckhunt-module]], [enable duckhunt game module]), [duckhunt=true])
AM_CONDITIONAL(DUCKHUNT, $duckhunt || $allmods)

theremin=false
AC_ARG_ENABLE(theremin-module, AC_HELP_STRING([[--enable-theremin-module]], [enable theremin sound module]), 
  [theremin=true])
if ( $theremin || $allmods ) then
  AC_CHECK_LIB(asound, snd_pcm_open, [theremin=true], [AC_MSG_ERROR([
    Could not find ALSA library, which is required for the theremin module. ])  
  ])
fi
AM_CONDITIONAL(THEREMIN, $theremin)

shapesnsounds=false
AC_ARG_ENABLE(shapesnsounds-module, AC_HELP_STRING([[--enable-shapesnsounds-module]], [enable shapes and sound module]), 
  [shapesnsounds=true])
if ( $shapesnsounds || $allmods ) then
  AC_CHECK_LIB(asound, snd_pcm_open, [shapesnsounds=true], [AC_MSG_ERROR([
    Could not find ALSA library, which is required for the shapesnsounds module. ])  
  ])
fi
AM_CONDITIONAL(SHAPESNSOUNDS, $shapesnsounds)

pointer=false
AC_ARG_ENABLE(pointer-module, AC_HELP_STRING([[--enable-pointer-module]], [enable pointer module]), [ 
  pointer=true])
if ( $pointer || $allmods ) then
  AC_CHECK_HEADER([X11/Xlib.h], [pointer=true],  
    [AC_MSG_ERROR([Could not find X11/Xlib.h required for pointer module.])])
  AC_CHECK_HEADER([X11/extensions/XTest.h], [pointer=true],
    [AC_MSG_ERROR([Could not find X11/extensions/XTest.h required for pointer module.])])
  AC_CHECK_LIB([Xtst], [XTestFakeMotionEvent], [pointer=true], 
    [AC_MSG_ERROR([Could not find Xtst library function XTestFakeMotionEvent.])])
  AC_CHECK_LIB([Xtst], [XTestFakeButtonEvent], [pointer=true],
    [AC_MSG_ERROR([Could not find Xtst library function XTestFakeButtonEvent.])])
fi 
AM_CONDITIONAL(POINTER, $pointer)

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
