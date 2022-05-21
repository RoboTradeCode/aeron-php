dnl config.m4 for extension aeron

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary.

dnl If your extension references something external, use 'with':

PHP_ARG_WITH([aeron],
  [for aeron support],
  [AS_HELP_STRING([--with-aeron],
    [Include aeron support])])

dnl Otherwise use 'enable':

dnl PHP_ARG_ENABLE([aeron],
dnl   [whether to enable aeron support],
dnl   [AS_HELP_STRING([--enable-aeron],
dnl     [Enable aeron support])],
dnl   [no])

if test "$PHP_AERON" != "no"; then
  dnl Write more examples of tests here...

  dnl Remove this code block if the library does not support pkg-config.
  dnl PKG_CHECK_MODULES([LIBFOO], [foo])
  dnl PHP_EVAL_INCLINE($LIBFOO_CFLAGS)
  dnl PHP_EVAL_LIBLINE($LIBFOO_LIBS, AERON_SHARED_LIBADD)

  dnl If you need to check for a particular library version using PKG_CHECK_MODULES,
  dnl you can use comparison operators. For example:
  dnl PKG_CHECK_MODULES([LIBFOO], [foo >= 1.2.3])
  dnl PKG_CHECK_MODULES([LIBFOO], [foo < 3.4])
  dnl PKG_CHECK_MODULES([LIBFOO], [foo = 1.2.3])

  dnl Remove this code block if the library supports pkg-config.
  dnl --with-aeron -> check with-path
  SEARCH_PATH="/usr/local /usr"     # you might want to change this
  SEARCH_FOR="/include/aeron/aeronc.h"  # you most likely want to change this
  if test -r $PHP_AERON/$SEARCH_FOR; then # path given as parameter
    AERON_DIR=$PHP_AERON
  else # search default path list
    AC_MSG_CHECKING([for aeron files in default path])
    for i in $SEARCH_PATH ; do
      if test -r $i/$SEARCH_FOR; then
        AERON_DIR=$i
        AC_MSG_RESULT(found in $i)
      fi
    done
  fi

  if test -z "$AERON_DIR"; then
    AC_MSG_RESULT([not found])
    AC_MSG_ERROR([Please reinstall the aeron distribution])
  fi

  dnl Remove this code block if the library supports pkg-config.
  dnl --with-aeron -> add include path
  PHP_ADD_INCLUDE($AERON_DIR/include)

  dnl Remove this code block if the library supports pkg-config.
  dnl --with-aeron -> check for lib and symbol presence
  LIBNAME=aeron # you may want to change this
  LIBSYMBOL=aeron_init # you most likely want to change this

  dnl If you need to check for a particular library function (e.g. a conditional
  dnl or version-dependent feature) and you are using pkg-config:
  dnl PHP_CHECK_LIBRARY($LIBNAME, $LIBSYMBOL,
  dnl [
  dnl   AC_DEFINE(HAVE_AERON_FEATURE, 1, [ ])
  dnl ],[
  dnl   AC_MSG_ERROR([FEATURE not supported by your aeron library.])
  dnl ], [
  dnl   $LIBFOO_LIBS
  dnl ])

  dnl If you need to check for a particular library function (e.g. a conditional
  dnl or version-dependent feature) and you are not using pkg-config:
  PHP_CHECK_LIBRARY($LIBNAME, $LIBSYMBOL,
  [
    PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $AERON_DIR/$PHP_LIBDIR, AERON_SHARED_LIBADD)
    AC_DEFINE(HAVE_AERON_FEATURE, 1, [ ])
  ],[
    AC_MSG_ERROR([FEATURE not supported by your aeron library.])
  ],[
    -L$AERON_DIR/$PHP_LIBDIR -lm
  ])

  PHP_SUBST(AERON_SHARED_LIBADD)

  dnl In case of no dependencies
  AC_DEFINE(HAVE_AERON, 1, [ Have aeron support ])

  PHP_NEW_EXTENSION(aeron, aeron.c, $ext_shared)
fi
