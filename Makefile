SRC=src/main.cpp src/mainwindow.cpp src/webview.cpp
INCL=-Ideps/httplib `pkg-config --cflags gtkmm-4.0 webkit2gtk-5.0`
LIBS=`pkg-config --libs gtkmm-4.0 webkit2gtk-5.0` -lpthread
CFLAGS=-std=c++20
REL_FLAGS=-O3
DEB_FLAGS=-O0 -DDEBUG -ggdb3 -Wall
BIN_NAME=mck-webkit-test
OUT_DEB=bin/debug
OUT_REL=bin/release
release: mkdir-rel
	g++ ${CFLAGS} ${REL_FLAGS} ${INCL} ${SRC} -o ${OUT_REL}/${BIN_NAME} ${LIBS}

debug: mkdir-deb
	g++ ${CFLAGS} ${DEB_FLAGS} ${INCL} ${SRC} -o ${OUT_DEB}/${BIN_NAME} ${LIBS}

mkdir-deb:
	mkdir -p ${OUT_DEB} || true

mkdir-rel:
	mkdir -p ${OUT_REL} || true


run: debug
	WEBKIT_INSPECTOR_SERVER=127.0.0.1:1234 ./${OUT_DEB}/${BIN_NAME}