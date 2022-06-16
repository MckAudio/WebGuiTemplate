SRC=src/main.cpp src/mainwindow.cpp src/webview.cpp
INCL=-Ideps/httplib `pkg-config --cflags gtkmm-4.0 webkit2gtk-5.0`
DEB_FLAGS=-O0 -DDEBUG -ggdb3 -Wall
OUT=bin/mck-webkit-test
default: mkdir
	g++ ${DEB_FLAGS} ${INCL} ${SRC} -o ${OUT} `pkg-config --libs gtkmm-4.0 webkit2gtk-5.0` -lpthread

mkdir:
	mkdir bin || true