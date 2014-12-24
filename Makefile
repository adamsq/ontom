#############################################################################
# Makefile for building: ontom
# Generated by qmake (2.01a) (Qt 4.8.2) on: Mon Aug 18 09:29:26 2014
# Project:  ontom.pro
# Template: app
# Command: /home/tom/workspace/qt-4.8.2-arm/bin/qmake -o Makefile ontom.pro
#############################################################################

####### Compiler, tools and options

CC            = arm-arago-linux-gnueabi-gcc -lts
CXX           = arm-arago-linux-gnueabi-gcc -lts
#DEFINES      = -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_CORE_LIB -DQT_SHARED
VER           = `svn info | grep 'Revision' | awk '{print $2}'`
DEFINES       =
CFLAGS        = -g -pipe -O2 -Wall -Wno-unused-parameter -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -g -pipe -O2 -Wall -Wno-unused-parameter -W -D_REENTRANT $(DEFINES)
INCPATH       = -I../qt-4.8.2-arm/mkspecs/qws/linux-arm-gcc -I.
LINK          = arm-arago-linux-gnueabi-gcc -lts -lrt
LFLAGS        = -Wl,-O1
LIBS          = $(SUBLIBS)  -L/home/tom/workspace/qt-4.8.2-arm/lib  \
		-L/home/tom/workspace/tslib/lib -L/home/tom/workspace/qt-4.8.2-arm/lib \
		-L. -lD8U -lD8usb -lpthread
AR            = arm-arago-linux-ar cqs
RANLIB        = 
QMAKE         = /home/tom/workspace/qt-4.8.2-arm/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = arm-arago-linux-gnueabi-strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = mongoose.c \
		tom.c ajax.c bms.c config.c log.c charge.c zeus.c \
		Hachiko.c blueprint.c
OBJECTS       = mongoose.o \
		tom.o ajax.o bms.o config.o log.o charge.o Hachiko.o \
		blueprint.o
DIST          = ../qt-4.8.2-arm/mkspecs/common/unix.conf \
		../qt-4.8.2-arm/mkspecs/common/linux.conf \
		../qt-4.8.2-arm/mkspecs/common/gcc-base.conf \
		../qt-4.8.2-arm/mkspecs/common/gcc-base-unix.conf \
		../qt-4.8.2-arm/mkspecs/common/g++-base.conf \
		../qt-4.8.2-arm/mkspecs/common/g++-unix.conf \
		../qt-4.8.2-arm/mkspecs/common/qws.conf \
		../qt-4.8.2-arm/mkspecs/qconfig.pri \
		../qt-4.8.2-arm/mkspecs/modules/qt_webkit_version.pri \
		../qt-4.8.2-arm/mkspecs/features/qt_functions.prf \
		../qt-4.8.2-arm/mkspecs/features/qt_config.prf \
		../qt-4.8.2-arm/mkspecs/features/exclusive_builds.prf \
		../qt-4.8.2-arm/mkspecs/features/default_pre.prf \
		../qt-4.8.2-arm/mkspecs/features/release.prf \
		../qt-4.8.2-arm/mkspecs/features/default_post.prf \
		../qt-4.8.2-arm/mkspecs/features/warn_on.prf \
		../qt-4.8.2-arm/mkspecs/features/qt.prf \
		../qt-4.8.2-arm/mkspecs/features/unix/thread.prf \
		../qt-4.8.2-arm/mkspecs/features/moc.prf \
		../qt-4.8.2-arm/mkspecs/features/resources.prf \
		../qt-4.8.2-arm/mkspecs/features/uic.prf \
		../qt-4.8.2-arm/mkspecs/features/yacc.prf \
		../qt-4.8.2-arm/mkspecs/features/lex.prf \
		../qt-4.8.2-arm/mkspecs/features/include_source_dir.prf \
		ontom.pro
QMAKE_TARGET  = ontom
DESTDIR       = 
TARGET        = ontom

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	@echo "        ARM-GCC                  $<"
	@$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

config:
	@echo "        ARM-LD                   $@"

all: Makefile $(TARGET)

$(TARGET):  $(OBJECTS)  
	@echo "        ARM-LD                   $@"
	@$(LINK) $(LFLAGS) -Map=ontom.map -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)
	@echo "        ARM-STIP                 $@"
#	@$(STRIP) $@

Makefile: ontom.pro  ../qt-4.8.2-arm/mkspecs/qws/linux-arm-g++/qmake.conf ../qt-4.8.2-arm/mkspecs/common/unix.conf \
		../qt-4.8.2-arm/mkspecs/common/linux.conf \
		../qt-4.8.2-arm/mkspecs/common/gcc-base.conf \
		../qt-4.8.2-arm/mkspecs/common/gcc-base-unix.conf \
		../qt-4.8.2-arm/mkspecs/common/g++-base.conf \
		../qt-4.8.2-arm/mkspecs/common/g++-unix.conf \
		../qt-4.8.2-arm/mkspecs/common/qws.conf \
		../qt-4.8.2-arm/mkspecs/qconfig.pri \
		../qt-4.8.2-arm/mkspecs/modules/qt_webkit_version.pri \
		../qt-4.8.2-arm/mkspecs/features/qt_functions.prf \
		../qt-4.8.2-arm/mkspecs/features/qt_config.prf \
		../qt-4.8.2-arm/mkspecs/features/exclusive_builds.prf \
		../qt-4.8.2-arm/mkspecs/features/default_pre.prf \
		../qt-4.8.2-arm/mkspecs/features/release.prf \
		../qt-4.8.2-arm/mkspecs/features/default_post.prf \
		../qt-4.8.2-arm/mkspecs/features/warn_on.prf \
		../qt-4.8.2-arm/mkspecs/features/qt.prf \
		../qt-4.8.2-arm/mkspecs/features/unix/thread.prf \
		../qt-4.8.2-arm/mkspecs/features/moc.prf \
		../qt-4.8.2-arm/mkspecs/features/resources.prf \
		../qt-4.8.2-arm/mkspecs/features/uic.prf \
		../qt-4.8.2-arm/mkspecs/features/yacc.prf \
		../qt-4.8.2-arm/mkspecs/features/lex.prf \
		../qt-4.8.2-arm/mkspecs/features/include_source_dir.prf \
		/home/tom/workspace/qt-4.8.2-arm/lib/libQtGui.prl \
		/home/tom/workspace/qt-4.8.2-arm/lib/libQtNetwork.prl \
		/home/tom/workspace/qt-4.8.2-arm/lib/libQtCore.prl
	$(QMAKE) -o Makefile ontom.pro
../qt-4.8.2-arm/mkspecs/common/unix.conf:
../qt-4.8.2-arm/mkspecs/common/linux.conf:
../qt-4.8.2-arm/mkspecs/common/gcc-base.conf:
../qt-4.8.2-arm/mkspecs/common/gcc-base-unix.conf:
../qt-4.8.2-arm/mkspecs/common/g++-base.conf:
../qt-4.8.2-arm/mkspecs/common/g++-unix.conf:
../qt-4.8.2-arm/mkspecs/common/qws.conf:
../qt-4.8.2-arm/mkspecs/qconfig.pri:
../qt-4.8.2-arm/mkspecs/modules/qt_webkit_version.pri:
../qt-4.8.2-arm/mkspecs/features/qt_functions.prf:
../qt-4.8.2-arm/mkspecs/features/qt_config.prf:
../qt-4.8.2-arm/mkspecs/features/exclusive_builds.prf:
../qt-4.8.2-arm/mkspecs/features/default_pre.prf:
../qt-4.8.2-arm/mkspecs/features/release.prf:
../qt-4.8.2-arm/mkspecs/features/default_post.prf:
../qt-4.8.2-arm/mkspecs/features/warn_on.prf:
../qt-4.8.2-arm/mkspecs/features/qt.prf:
../qt-4.8.2-arm/mkspecs/features/unix/thread.prf:
../qt-4.8.2-arm/mkspecs/features/moc.prf:
../qt-4.8.2-arm/mkspecs/features/resources.prf:
../qt-4.8.2-arm/mkspecs/features/uic.prf:
../qt-4.8.2-arm/mkspecs/features/yacc.prf:
../qt-4.8.2-arm/mkspecs/features/lex.prf:
../qt-4.8.2-arm/mkspecs/features/include_source_dir.prf:
/home/tom/workspace/qt-4.8.2-arm/lib/libQtGui.prl:
/home/tom/workspace/qt-4.8.2-arm/lib/libQtNetwork.prl:
/home/tom/workspace/qt-4.8.2-arm/lib/libQtCore.prl:
qmake:  FORCE
	@$(QMAKE) -o Makefile ontom.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/ontom1.0.0 || $(MKDIR) .tmp/ontom1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/ontom1.0.0/ && $(COPY_FILE) --parents mongoose.h tom.h .tmp/ontom1.0.0/ && $(COPY_FILE) --parents mongoose.c tom.c .tmp/ontom1.0.0/ && (cd `dirname .tmp/ontom1.0.0` && $(TAR) ontom1.0.0.tar ontom1.0.0 && $(COMPRESS) ontom1.0.0.tar) && $(MOVE) `dirname .tmp/ontom1.0.0`/ontom1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/ontom1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all:
compiler_moc_header_clean:
compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all:
compiler_uic_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: 

ut: config.c log.c unit_test.c
	gcc -g -o $@ $^

zeus_unit_test: zeus_unit_test.c log.c config.c
	/usr/bin/gcc -g -o $@ -o $^

onconfig-arm: onconfig.c
	$(CC) $(CFLAGS) $(INCPATH) -o $@ $^

onconfig-i386: onconfig.c
	gcc -o $@ $^

####### Compile

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

