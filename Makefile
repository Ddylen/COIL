# Automatically generated Makefile
read :  reading.o read.o read.o read.o project.o movement.o Makefile.o header.o forwards.o
	 g++ -L/export/teach/1BRobot -o read  reading.o  read.o  read.o  read.o  project.o  movement.o  Makefile.o  header.o  forwards.o  -lrobot

reading.o: reading.cc header.h
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c reading.cc
read.o: read.o
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c read.o
read.o: read.cc header.h
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c read.cc
read.o: read
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c read
project.o: project.geany
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c project.geany
movement.o: movement.cc header.h
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c movement.cc
Makefile.o: Makefile
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c Makefile
header.o: header.h.gch
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c header.h.gch
forwards.o: forwards.cc header.h
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c forwards.cc
