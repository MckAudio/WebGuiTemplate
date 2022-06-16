SRC=src/main.cpp src/mainwindow.cpp src/webview.cpp
INCL=-Ideps/httplib `pkg-config --cflags jsoncpp gtkmm-4.0 webkit2gtk-5.0`
LIBS=`pkg-config --libs jsoncpp gtkmm-4.0 webkit2gtk-5.0` -lpthread
CFLAGS=-std=c++20
DEB_FLAGS=-O0 -DDEBUG -ggdb3 -Wall
OUT=bin/mck-webkit-test
default: mkdir
	g++ ${CFLAGS} ${DEB_FLAGS} ${INCL} ${SRC} -o ${OUT} ${LIBS}

mkdir:
	mkdir -p bin || true

run:
	WEBKIT_INSPECTOR_SERVER=127.0.0.1:1234 ./${OUT}