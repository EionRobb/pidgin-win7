#Customisable stuff here
WIN32_COMPILER ?= /usr/bin/i586-mingw32-gcc
WIN32_WINDRES ?= i586-mingw32-windres
WIN32_DLLTOOL ?= i586-mingw32-dlltool


LIBPURPLE_CFLAGS = -I/usr/include/libpurple -I/usr/local/include/libpurple -DPURPLE_PLUGINS -DENABLE_NLS -DHAVE_ZLIB
PIDGIN_CFLAGS = -I/usr/include/pidgin -I/usr/local/include/pidgin
GLIB_CFLAGS = -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include -I/usr/local/include/glib-2.0 -I/usr/local/lib/glib-2.0/include -I/usr/local/include
WIN32_DEV_DIR = /root/pidgin/win32-dev
WIN32_PIDGIN_DIR = /root/pidgin/pidgin-2.3.0_win32

ifeq ($(OS),Windows_NT)
WIN32_DEV_DIR = ../win32-dev
WIN32_COMPILER = ${WIN32_DEV_DIR}/mingw-4.7.2/bin/gcc
WIN32_WINDRES = ${WIN32_DEV_DIR}/mingw-4.7.2/bin/windres
WIN32_DLLTOOL = ${WIN32_DEV_DIR}/mingw-4.7.2/bin/dlltool
WIN32_PIDGIN_DIR = ../pidgin-2.10.11

LIBPURPLE_CFLAGS = -I${WIN32_PIDGIN_DIR}/libpurple -I${WIN32_PIDGIN_DIR}/libpurple/win32 -DPURPLE_PLUGINS -DENABLE_NLS -DHAVE_ZLIB
PIDGIN_CFLAGS = -I${WIN32_PIDGIN_DIR}/pidgin -I${WIN32_PIDGIN_DIR}/pidgin/win32
GLIB_CFLAGS = -I${WIN32_DEV_DIR}/gtk_2_0/include/glib-2.0 -I${WIN32_DEV_DIR}/gtk_2_0/lib/glib-2.0/include

endif

WIN32_CFLAGS = -I${WIN32_DEV_DIR}/gtk_2_0/include/glib-2.0 -I${WIN32_PIDGIN_DIR}/libpurple/win32 -I${WIN32_PIDGIN_DIR}/pidgin/win32 -I${WIN32_DEV_DIR}/gtk_2_0/include -I${WIN32_DEV_DIR}/gtk_2_0/include/glib-2.0 -I${WIN32_DEV_DIR}/gtk_2_0/lib/glib-2.0/include -Wno-format -I/usr/include/wine/windows -Wno-write-strings -Wno-strict-aliasing -I${WIN32_DEV_DIR}/gtk_2_0/include/gtk-2.0 -I${WIN32_DEV_DIR}/gtk_2_0/include/cairo -I${WIN32_DEV_DIR}/gtk_2_0/include/atk-1.0 -I${WIN32_DEV_DIR}/gtk_2_0/include/pango-1.0 -I${WIN32_DEV_DIR}/gtk_2_0/lib/gtk-2.0/include -Wno-attributes
WIN32_LIBS = -L${WIN32_DEV_DIR}/gtk_2_0/lib -L${WIN32_PIDGIN_DIR}/libpurple -L${WIN32_PIDGIN_DIR}/pidgin -lglib-2.0 -lgobject-2.0 -lgtk-win32-2.0 -lgdk-win32-2.0 -lintl -lws2_32 -lpurple -lole32 -lshell32 -lshell32_win7 -luuid -L. -lpidgin -lgdk_pixbuf-2.0 -lgdk-win32-2.0 -lgdi32 -ldwmapi

all:	pidgin-win7.dll

clean:
	rm -f pidgin-win7.dll libdwmapi.a libshell32_win7.a

libdwmapi.a:	dwmapi.def
	${WIN32_DLLTOOL} -d dwmapi.def -l libdwmapi.a -k

libshell32_win7.a:	shell32_win7.def
	${WIN32_DLLTOOL} -d shell32_win7.def -l libshell32_win7.a -k

pidgin-win7.res:	pidgin-win7.rc
	${WIN32_WINDRES} -i $< -O coff -o $@

pidgin-win7.dll:	pidgin-win7.c pidgin-win7.h pidgin-win7.res libdwmapi.a libshell32_win7.a
	${WIN32_COMPILER} ${LIBPURPLE_CFLAGS} ${PIDGIN_CFLAGS} -Wall -I. -O2 -pipe $< pidgin-win7.res -o $@ -shared -static-libgcc ${WIN32_CFLAGS} ${WIN32_LIBS} -Wl,--strip-all -mms-bitfields -Wl,--enable-auto-image-base -Wl,--warn-once
#	upx $@

pidgin-win7-debug.dll:	pidgin-win7.c pidgin-win7.h pidgin-win7.res libdwmapi.a libshell32_win7.a
	${WIN32_COMPILER} ${LIBPURPLE_CFLAGS} ${PIDGIN_CFLAGS} -Wall -I. -g -O2 -pipe $< pidgin-win7.res -o $@ -shared -static-libgcc ${WIN32_CFLAGS} ${WIN32_LIBS} -mms-bitfields -Wl,--enable-auto-image-base

