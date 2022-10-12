.PHONY : clean

objects_files = Timestamp.o FileUtil.o LogStream.o Logger.o LogFile.o AsyncLogging.o \
				CurrentThread.o
static_libs =  Logger.a LogFile.a AsyncLogging.a

FLAGS = -g -Wall -O2

all : $(objects_files) $(static_libs)

CurrentThread.o : CurrentThread.cpp
	g++ -c -o $@ $^ $(FLAGS) -pthread

AsyncLogging.a : AsyncLogging.o LogFile.o Timestamp.o FileUtil.o
	ar rcs $@ $^

AsyncLogging.o: AsyncLogging.cpp
	g++ -c -o $@ $^ $(FLAGS) -pthread

LogFile.a : LogFile.o Timestamp.o FileUtil.o
	ar rcs $@ $^

LogFile.o : LogFile.cpp
	g++ -c -o $@ $^ $(FLAGS) -pthread

Logger.a: Logger.o LogStream.o Timestamp.o CurrentThread.o
	ar rcs $@ $^

Timestamp.o : Timestamp.cpp
	g++ -c -o $@ $^ $(FLAGS)

FileUtil.o : FileUtil.cpp
	g++ -c -o $@ $^ $(FLAGS)

LogStream.o : LogStream.cpp 
	g++ -c -o $@ $^ $(FLAGS)

Logger.o : Logger.cpp
	g++ -c -o $@ $^ $(FLAGS)

clean:
	- rm $(objects_files) $(static_libs)