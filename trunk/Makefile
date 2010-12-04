#Customisable stuff here
WIN32_COMPILER = /usr/bin/i586-mingw32-gcc
WIN32_WINDRES = i586-mingw32-windres

LIBPURPLE_CFLAGS = -I/usr/include/libpurple -I/usr/local/include/libpurple -DPURPLE_PLUGINS -DENABLE_NLS -DHAVE_ZLIB
GLIB_CFLAGS = -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include -I/usr/local/include/glib-2.0 -I/usr/local/lib/glib-2.0/include -I/usr/local/include
WIN32_DEV_DIR = /root/pidgin/win32-dev
WIN32_PIDGIN_DIR = /root/pidgin/pidgin-2.3.0_win32
WIN32_CFLAGS = -I${WIN32_DEV_DIR}/gtk_2_0/include/glib-2.0 -I${WIN32_PIDGIN_DIR}/libpurple/win32 -I${WIN32_DEV_DIR}/gtk_2_0/include -I${WIN32_DEV_DIR}/gtk_2_0/include/glib-2.0 -I${WIN32_DEV_DIR}/gtk_2_0/lib/glib-2.0/include -Wno-format -I/usr/include/wine/windows -Wno-write-strings -Wno-strict-aliasing
WIN32_LIBS = -L${WIN32_DEV_DIR}/gtk_2_0/lib -L${WIN32_PIDGIN_DIR}/libpurple -lglib-2.0 -lgobject-2.0 -lintl -lws2_32 -lpurple -lole32 -L. -lshell32

all:	pidgin-win7.dll

clean:
	rm -f pidgin-win7.dll

pidgin-win7.dll:	pidgin-win7.c
	${WIN32_COMPILER} ${LIBPURPLE_CFLAGS} -Wall -I. -g -O2 -pipe $< -o $@ -shared -mno-cygwin ${WIN32_CFLAGS} ${WIN32_LIBS} -Wl,--strip-all
	upx $@

pidgin-win7-debug.dll:	pidgin-win7.c
	${WIN32_COMPILER} ${LIBPURPLE_CFLAGS} -Wall -I. -g -O2 -pipe $< -o $@ -shared -mno-cygwin ${WIN32_CFLAGS} ${WIN32_LIBS}

