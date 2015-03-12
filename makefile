#file: Makefile
#
# test program for CMS RPC
#
# 14/01/14  Created by Y.Benhammou


DAQ_HOME_DIR = /home/gifdaq/Desktop/GIF_DAQ
DAQ_BIN_DIR = $(DAQ_HOME_DIR)/bin
DAQ_INC_DIR = $(DAQ_HOME_DIR)/include
DAQ_SRC_DIR = $(DAQ_HOME_DIR)/src
DAQ_OBJ_DIR = $(DAQ_HOME_DIR)/obj

ROOT_INC  = $(ROOTSYS)/include
ROOTLIBS     := $(shell root-config --libs)

LFLAGS     = -L$(DAQ_HOME_DIR)/lib -L/usr/lib \
             $(ROOTLIBS) 

CFLAGS     = -ggdb -fPIC -DLINUX -Wall -funsigned-char \
             -I$(DAQ_INC_DIR) -I$(ROOT_INC)

all: daq

daq: 	daq.o v1718.o v1190a.o DataReader.o IniFile.o
	g++ $(CFLAGS) $(DAQ_OBJ_DIR)/daq.o \
	$(DAQ_OBJ_DIR)/v1718.o \
	$(DAQ_OBJ_DIR)/v1190a.o \
	$(DAQ_OBJ_DIR)/DataReader.o \
	$(DAQ_OBJ_DIR)/IniFile.o \
        -o $(DAQ_BIN_DIR)/daq \
        $(LFLAGS)  \
        -l CAENVME -l curses

clean:
	-rm $(DAQ_BIN_DIR)/daq
	-rm $(DAQ_OBJ_DIR)/*.o

remove:
	-rm $(DAQ_BIN_DIR)/daq
	-rm $(DAQ_OBJ_DIR)/*.o

daq.o:
	g++ -c $(CFLAGS) $(DAQ_SRC_DIR)/daq.cxx -o $(DAQ_OBJ_DIR)/daq.o
v1718.o:
	g++ -c $(CFLAGS) $(DAQ_SRC_DIR)/v1718.cxx -o $(DAQ_OBJ_DIR)/v1718.o
v1190a.o:
	g++ -c $(CFLAGS) $(DAQ_SRC_DIR)/v1190a.cxx -o $(DAQ_OBJ_DIR)/v1190a.o
DataReader.o:
	g++ -c $(CFLAGS) $(DAQ_SRC_DIR)/DataReader.cxx -o $(DAQ_OBJ_DIR)/DataReader.o
IniFile.o:
	g++ -c $(CFLAGS) $(DAQ_SRC_DIR)/IniFile.cxx -o $(DAQ_OBJ_DIR)/IniFile.o