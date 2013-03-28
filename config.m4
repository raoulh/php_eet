dnl $Id$
dnl config.m4 for extension eet

PHP_ARG_WITH(eet, for eet support,
[  --with-eet             Include eet support])

if test "$PHP_EET" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-eet -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/eet.h"  # you most likely want to change this
  dnl if test -r $PHP_EET/$SEARCH_FOR; then # path given as parameter
  dnl   EET_DIR=$PHP_EET
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for eet files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       EET_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$EET_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the eet distribution])
  dnl fi

  dnl # --with-eet -> add include path
  dnl PHP_ADD_INCLUDE($EET_DIR/include)

  dnl # --with-eet -> check for lib and symbol presence
  dnl LIBNAME=eet # you may want to change this
  dnl LIBSYMBOL=eet # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $EET_DIR/lib, EET_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_EETLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong eet lib version or lib not found])
  dnl ],[
  dnl   -L$EET_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(EET_SHARED_LIBADD)

  PHP_SUBST(EET_SHARED_LIBADD)
  AC_DEFINE(HAVE_EET, 1, [ ])

  PHP_NEW_EXTENSION(eet, \
    eet.c \
    , $ext_shared)

  EXT_EET_HEADERS="php_eet.h"

  ifdef([PHP_INSTALL_HEADERS], [
    PHP_INSTALL_HEADERS(ext/eet, $EXT_EET_HEADERS)
  ])

  AC_MSG_CHECKING(for pkg-config)

  if test ! -f "$PKG_CONFIG"; then
    PKG_CONFIG=`which pkg-config`
  fi

  if test -f "$PKG_CONFIG"; then
    AC_MSG_RESULT(found)
    AC_MSG_CHECKING(for eet)

    if $PKG_CONFIG --exists eet; then
        eet_version_full=`$PKG_CONFIG --modversion eet`
        AC_MSG_RESULT([found $eet_version_full])
        EET_LIBS="$LDFLAGS `$PKG_CONFIG --libs eet`"
        EET_INCS="$CFLAGS `$PKG_CONFIG --cflags-only-I eet`"
        PHP_EVAL_INCLINE($EET_INCS)
        PHP_EVAL_LIBLINE($EET_LIBS, EET_SHARED_LIBADD)
        AC_DEFINE(HAVE_EET, 1, [whether eet exists in the system])
    else
        AC_MSG_RESULT(not found)
        AC_MSG_ERROR(Ooops ! no eet detected in the system)
    fi
  else
    AC_MSG_RESULT(not found)
    AC_MSG_ERROR(Ooops ! no pkg-config found .... )
  fi

fi
