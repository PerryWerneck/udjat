
dnl Process this file with autoconf to produce a configure script.

dnl The minimum version of autoconf required.
AC_PREREQ(2.59)

dnl Initialise automake with the package name, version and
dnl bug-reporting address.
AC_INIT([udjat], [0.1], [perry.werneck@gmail.com])

app_cv_title="The Eye of Horus"
AC_DEFINE_UNQUOTED(PACKAGE_TITLE,"$app_cv_title")
AC_SUBST(PACKAGE_TITLE,"$app_cv_title")

dnl Place auxilliary scripts here.
AC_CONFIG_AUX_DIR([scripts])

dnl Compute the canonical host-system type
AC_CANONICAL_HOST

dnl Put macro definitions here (though they aren't used).
AC_CONFIG_HEADER([src/include/config.h])

dnl Initialise automake stuff.
AM_INIT_AUTOMAKE

dnl Check for iconv
AM_ICONV

dnl Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_SED
AC_PROG_LN_S

AC_LANG([C++])

dnl ---------------------------------------------------------------------------
dnl Version info
dnl ---------------------------------------------------------------------------

app_vrs_major=$(echo $VERSION | cut -d. -f1)
app_vrs_minor=$(echo $VERSION | cut -d. -f2)

AC_DEFINE_UNQUOTED(PACKAGE_MAJOR_VERSION, $app_vrs_major)
AC_DEFINE_UNQUOTED(PACKAGE_MINOR_VERSION, $app_vrs_minor)

AC_SUBST(PACKAGE_MAJOR_VERSION,$app_vrs_major)
AC_SUBST(PACKAGE_MINOR_VERSION,$app_vrs_minor)

dnl ---------------------------------------------------------------------------
dnl Check for OS specifics
dnl ---------------------------------------------------------------------------

CFLAGS="$CFLAGS -I$ac_pwd/src/include"

case "$host" in
	*-mingw32)
		CFLAGS="$CFLAGS -pthread -D_WIN32_WINNT=0x0600"
		LDFLAGS="$LDFLAGS -pthread -static-libgcc -static-libstdc++"
		AC_SUBST(SONAME,libudjat.dll)
		AC_SUBST(OSNAME,windows)
		;;

 	s390x-*)
		CFLAGS="$CFLAGS -pthread"
		LDFLAGS="$LDFLAGS -pthread"
		AC_SUBST(SONAME,libudjat.so)
		AC_SUBST(OSNAME,linux)
		;;


 	*)
		CFLAGS="$CFLAGS -pthread"
		LDFLAGS="$LDFLAGS -pthread"
		AC_SUBST(SONAME,libudjat.so.$app_vrs_major.$app_vrs_minor)
		AC_SUBST(OSNAME,linux)
		LIBS="$LIBS -ldl"


esac

dnl ---------------------------------------------------------------------------
dnl Check for tools
dnl ---------------------------------------------------------------------------

AC_PATH_TOOL([VALGRIND], [valgrind], [no])
AC_PATH_TOOL([AR], [ar], [ar])

dnl ---------------------------------------------------------------------------
dnl Check for doxygen
dnl ---------------------------------------------------------------------------

AC_PATH_TOOL([DOXYGEN], [doxygen])

dnl Help line for graphviz (optionally used by doxygen)
AC_ARG_ENABLE(graphviz,
	AS_HELP_STRING([--disable-graphviz], [prevent doxygen from using dot]),
	[wantgraphviz=$enableval], [wantgraphviz=yes])

AC_CHECK_PROGS([GRAPHVIZDOT], [dot])
AC_MSG_CHECKING([whether doxygen should use graphviz])
if test "x$wantgraphviz" = "xyes" ; then
	if test $GRAPHVIZDOT ; then
		AC_MSG_RESULT([yes])
		usegraphviz=yes
	else
		AC_MSG_RESULT([no, graphviz missing])
		usegraphviz=no
	fi
else
	AC_MSG_RESULT([no, disabled by configure])
	usegraphviz=no
fi

AC_SUBST(HAVE_DOT,`echo $usegraphviz | tr yesno YESNO`)

dnl ---------------------------------------------------------------------------
dnl Check for libintl
dnl ---------------------------------------------------------------------------

AC_PATH_TOOL([XGETTEXT], [xgettext], [no])
AC_PATH_TOOL([MSGCAT], [msgcat], [no])
AC_PATH_TOOL([MSGINIT], [msginit], [no])
AC_PATH_TOOL([MSGMERGE], [msgmerge], [no])
AC_PATH_TOOL([MSGFMT], [msgfmt], [no])

AC_CHECK_HEADER(libintl.h, [
	AC_DEFINE(HAVE_LIBINTL, 1)
])

dnl ---------------------------------------------------------------------------
dnl Check for C++ 2011 support
dnl ---------------------------------------------------------------------------
AC_DEFUN([AX_CHECK_COMPILE_FLAG],
[AC_PREREQ(2.59)dnl for _AC_LANG_PREFIX
AS_VAR_PUSHDEF([CACHEVAR],[ax_cv_check_[]_AC_LANG_ABBREV[]flags_$4_$1])dnl
AC_CACHE_CHECK([whether _AC_LANG compiler accepts $1], CACHEVAR, [
  ax_check_save_flags=$[]_AC_LANG_PREFIX[]FLAGS
  _AC_LANG_PREFIX[]FLAGS="$[]_AC_LANG_PREFIX[]FLAGS $4 $1"
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM()],
    [AS_VAR_SET(CACHEVAR,[yes])],
    [AS_VAR_SET(CACHEVAR,[no])])
  _AC_LANG_PREFIX[]FLAGS=$ax_check_save_flags])
AS_IF([test x"AS_VAR_GET(CACHEVAR)" = xyes],
  [m4_default([$2], :)],
  [m4_default([$3], :)])
AS_VAR_POPDEF([CACHEVAR])dnl
])dnl AX_CHECK_COMPILE_FLAGS

AX_CHECK_COMPILE_FLAG([-std=c++17], [CPPFLAGS="$CPPFLAGS -std=c++17"], [AC_MSG_ERROR(["C++ 17 is required"])])

dnl ---------------------------------------------------------------------------
dnl Set the ELF symbol visibility support
dnl ---------------------------------------------------------------------------
AC_DEFINE(HAVE_GNUC_VISIBILITY, 1, [supports GCC visibility attributes])

dnl ---------------------------------------------------------------------------
dnl Check for compiler visibility flag
dnl ---------------------------------------------------------------------------
AC_MSG_CHECKING(whether $CC accepts -fvisibility=hidden)
AC_CACHE_VAL(app_cv_fvisibility_ok,
[ac_save_cc="$CC"
CC="$CC -fvisibility=hidden"
AC_TRY_RUN([int main() { return 0; }],
  app_cv_fvisibility_ok=yes,
  app_cv_fvisibility_ok=no,
  app_cv_fvisibility_ok=no)
CC="$ac_save_cc"])
AC_MSG_RESULT($app_cv_fvisibility_ok)
if test $app_cv_fvisibility_ok = yes; then
  CFLAGS="$CFLAGS -fvisibility=hidden"
fi

dnl ---------------------------------------------------------------------------
dnl Check for pic
dnl ---------------------------------------------------------------------------
AC_ARG_ENABLE([pic],
	[AS_HELP_STRING([--disable-pic], [disable support for PIC libraries (required to compile on 64bits)])],
[
	app_cv_pic="$enableval"
],[

	case "$host" in
		*-mingw32)
			app_cv_pic="no"
		;;

 	s390x-*)
			app_cv_pic="yes"
		;;

	*)
		app_cv_pic="yes"
	esac
])

if test "$app_cv_pic" == "yes"; then
	CFLAGS="$CFLAGS -fPIC"
fi

dnl ---------------------------------------------------------------------------
dnl Libraries
dnl ---------------------------------------------------------------------------
PKG_CHECK_EXISTS

PKG_CHECK_MODULES( [LIBSSL], [libssl libcrypto], AC_DEFINE(HAVE_LIBSSL), AC_MSG_NOTICE([libssl not present.]) )
AC_SUBST(LIBSSL_LIBS)
AC_SUBST(LIBSSL_CFLAGS)

dnl ---------------------------------------------------------------------------
dnl Check for econf
dnl ---------------------------------------------------------------------------
PKG_CHECK_MODULES( [ECONF], [libeconf], AC_DEFINE(HAVE_ECONF), AC_MSG_NOTICE([libeconf not present.]) )

AC_SUBST(ECONF_LIBS)
AC_SUBST(ECONF_CFLAGS)

dnl ---------------------------------------------------------------------------
dnl Check for net-snmp
dnl ---------------------------------------------------------------------------

AC_PATH_TOOL([NETSNMPCONFIG], [net-snmp-config], [no])

if test x$NETSNMPCONFIG = xno; then

	AC_MSG_NOTICE([Building without SNMP support])

else

	AC_MSG_NOTICE([Building with SNMP support])

	app_cv_components="$app_cv_components snmp"

	AC_DEFINE(HAVE_NET_SNMP)

	AC_SUBST(SNMP_CFLAGS,"`$NETSNMPCONFIG --cflags`")
	AC_SUBST(SNMP_LIBS,"`$NETSNMPCONFIG --agent-libs`")
	AC_SUBST(SNMP_VERSION,"`$NETSNMPCONFIG --version`")
	AC_SUBST(SNMP_CONFPATH,"`$NETSNMPCONFIG --snmpconfpath | cut -d: -f1`")
	AC_SUBST(SNMP_AGENTDIR,"\${libdir}/net-snmp")

	AC_SUBST(SNMP_UDP_PORT,1610)
	AC_SUBST(SNMP_TCP_PORT,1610)

	AC_DEFINE_UNQUOTED(SNMP_UDP_PORT,1610)
	AC_DEFINE_UNQUOTED(SNMP_TCP_PORT,1610)

	AC_MSG_CHECKING(for netsnmp_enable_this_loghandler in net-snmp/library/snmp_logging.h)
		AC_EGREP_HEADER(netsnmp_enable_this_loghandler, net-snmp/library/snmp_logging.h,
		[ AC_DEFINE([HAVE_SNMP_LOG_HANDLER], [1], [Define to 1 if netsnmp_enable_this_loghandler is available.]) AC_MSG_RESULT(yes) ],
		AC_MSG_RESULT(no))

fi

dnl ---------------------------------------------------------------------------
dnl Check for D-Bus support
dnl ---------------------------------------------------------------------------

PKG_CHECK_MODULES( [DBUS], [dbus-1], AC_DEFINE(HAVE_DBUS), AC_MSG_NOTICE([ No DBUS support.]) )

AC_SUBST(DBUS_LIBS)
AC_SUBST(DBUS_CFLAGS)

dnl ---------------------------------------------------------------------------
dnl systemd integration
dnl ---------------------------------------------------------------------------

PKG_CHECK_MODULES(SYSTEMD, systemd, dummy=yes,AC_MSG_NOTICE(systemd not present.))

SYSTEMD_UNITDIR="`$PKG_CONFIG --variable=systemdsystemunitdir systemd`"
AC_SUBST(SYSTEMD_UNITDIR)

SYSTEMD_PRESETDIR="`$PKG_CONFIG --variable=systemdsystempresetdir systemd`"
AC_SUBST(SYSTEMD_PRESETDIR)

PKG_CHECK_MODULES( [SYSTEMD], [libsystemd], AC_DEFINE(HAVE_SYSTEMD), AC_MSG_NOTICE([libsystemd not present.]) )
AC_SUBST(SYSTEMD_CFLAGS)
AC_SUBST(SYSTEMD_LIBS)

dnl ---------------------------------------------------------------------------
dnl test for pugixml
dnl ---------------------------------------------------------------------------

PKG_CHECK_MODULES(PUGIXML, pugixml, AC_DEFINE(HAVE_PUGIXML),AC_MSG_ERROR(pugixml is required))

AC_SUBST(PUGIXML_LIBS)
AC_SUBST(PUGIXML_CFLAGS)

dnl ---------------------------------------------------------------------------
dnl Test for jsoncpp
dnl ---------------------------------------------------------------------------
PKG_CHECK_MODULES( [JSONCPP], [jsoncpp], AC_DEFINE(HAVE_JSONCPP), AC_MSG_ERROR([libjsoncpp not present.]) )

AC_SUBST(JSONCPP_LIBS)
AC_SUBST(JSONCPP_CFLAGS)

dnl ---------------------------------------------------------------------------
dnl Output config
dnl ---------------------------------------------------------------------------

AC_SUBST(BASEDIR,$ac_pwd)

dnl ---------------------------------------------------------------------------
dnl Configure which files to generate.
dnl ---------------------------------------------------------------------------

dnl AC_CONFIG_SUBDIRS([sample])
dnl https://www.gnu.org/software/autoconf-archive/ax_subdirs_configure.html
dnl AX_SUBDIRS_CONFIGURE( [subdirs], [mandatory arguments], [possibly merged arguments], [replacement arguments], [forbidden arguments])
dnl AX_SUBDIRS_CONFIGURE([sample],[UDJAT_CFLAGS=-I$ac_pwd/src/include],[UDJAT_LIBS=-L$ac_pwd/.bin/Debug/libudjat.so.$app_vrs_major.$app_vrs_minor],[--with-build-dir=$ac_pwd])

dnl ac_configure_args="UDJAT_CFLAGS=-I$ac_pwd/src/include UDJAT_LIBS=-L$ac_pwd/.bin/Debug/libudjat.so.$app_vrs_major.$app_vrs_minor --with-build-dir=$ac_pwd"

dnl AC_CONFIG_SUBDIRS([sample])

AC_CONFIG_FILES(doxygen/doxyfile)
AC_CONFIG_FILES(Makefile)

dnl ---------------------------------------------------------------------------
dnl Output the generated config.status script.
dnl ---------------------------------------------------------------------------

AC_OUTPUT

