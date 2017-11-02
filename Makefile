# Automatically generated Makefile
a.out :  upperlevel.o TEST.o reading.o movement.o loading.o
	 g++ -L/export/teach/1BRobot -o a.out  upperlevel.o  TEST.o  reading.o  movement.o  loading.o  -lrobot

upperlevel.o: upperlevel.cc header.h
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c upperlevel.cc
TEST.o: TEST.cc header.h
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c TEST.cc
reading.o: reading.cc header.h
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c reading.cc
movement.o: movement.cc header.h
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c movement.cc
loading.o: loading.cc header.h
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c loading.cc
