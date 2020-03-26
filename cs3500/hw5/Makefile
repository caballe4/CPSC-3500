#Jack Gularte and Anthony Caballero
#Makefile: use to run and submit p3
#cpsc3500
#2/17/19

CXX = g++
STD = -std=c++11
DEBUG = -g
CFLAGS = -Walll -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)
THR = -pthread

p3:
	$(CXX) $(LFLAGS) -o p3 p3.cpp $(STD) ($THR)
p3_submit:
	/home/fac/zhuy/class/SubmitHW/submit3500 p3 p3.tar

clean:
	/rm*.o*~