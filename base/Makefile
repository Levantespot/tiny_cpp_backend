.PHONY : clean

objects = Timestamp.o FileUtil.o LogStream.o Logger.o Logger.a

FLAGS = -g -Wall
# LIBS = -pthread

all : $(objects)

Logger.a: Logger.o LogStream.o Timestamp.o
	ar rcs $@ $^

Timestamp.o : Timestamp.cpp
	g++ -c -o $@ $^ $(FLAGS)

FileUtil.o : FileUtil.cpp
	g++ -c -o $@ $^ $(FLAGS)

LogStream.o : LogStream.cpp 
	g++ -c -o $@ $^ $(FLAGS)

Logger.o : Logger.cpp LogStream.o Timestamp.o
	g++ -c -o $@ $^ $(FLAGS) -pthread

clean:
	- rm $(objects)