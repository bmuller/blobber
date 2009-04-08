#! /bin/sh
libtoolize -f -c && aclocal && autoheader && automake -ac && autoconf && ./configure "$@"
