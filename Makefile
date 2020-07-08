HOME = Chain-IC
CXX = g++

#INCLUDES = -I($(HOME)/src_new
#LIBINCLUDES = -L $(HOME)/lib
#INC = $(INCLUDES) #$(LIBINCLUDES)

CXXFLAGS = -g -Wall -std=c++11
SRCS = Main.cpp chainSIM.cpp src_new/chain.cpp
OBJS = $(subst .cpp,.o,$(SRCS))

all: Main

Main: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o Main

Main.o: Main.cpp chainSIM.h src_new/chain.h
	$(CXX) $(CXXFLAGS) -c Main.cpp

chainSIM.o: chainSIM.cpp chainSIM.h src_new/chain.h
	$(CXX) $(CXXFLAGS) -c chainSIM.cpp

chain.o: src_new/chain.cpp src_new/chain.h
	$(CXX) $(CXXFLAGS) -c src_new/chain.cpp

clean:
	rm -rf *.o Main


#all: Main.out

#Main.out: Main.cpp chainSIM.h chainSIM.cpp src_new/chain.h src_new/chain.cpp
#	g++ Main.cpp chainSIM.cpp src_new/chain.cpp -o Main.out

#clean:
#	rm -rf *.o Main.out
