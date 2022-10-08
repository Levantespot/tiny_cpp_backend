.PHONY : clean

objects = test_LogStream test_Timestamp test_Logger test_FileUtil \
	test_Logger_with_FileUtil test_Logger_with_LogFile test_AsyncLogging \
	test_Logger_with_Async bench_Logger_with_LogFile bench_Logger_with_Async

FLAGS = -Wall -g -O1
OUTPUT_DIR = build

all : $(objects)

bench_Logger_with_Async : bench_Logger_with_Async.cpp  ../Logger.a ../AsyncLogging.a
	g++ -o $(OUTPUT_DIR)/$@ $^ $(FLAGS) -pthread -p

bench_Logger_with_LogFile : bench_Logger_with_LogFile.cpp ../Logger.a ../LogFile.a
	g++ -o $(OUTPUT_DIR)/$@ $^ $(FLAGS) -pthread -p

test_Logger_with_Async : test_Logger_with_Async.cpp ../Logger.a ../AsyncLogging.a
	g++ -o $(OUTPUT_DIR)/$@ $^ $(FLAGS) -pthread

test_AsyncLogging: test_AsyncLogging.cpp ../AsyncLogging.a
	g++ -o $(OUTPUT_DIR)/$@ $^ $(FLAGS) -pthread

test_Logger_with_LogFile : test_Logger_with_LogFile.cpp ../Logger.a ../LogFile.a
	g++ -o $(OUTPUT_DIR)/$@ $^ $(FLAGS)

test_Logger_with_FileUtil : test_Logger_with_FileUtil.cpp ../Logger.a ../FileUtil.o
	g++ -o $(OUTPUT_DIR)/$@ $^ $(FLAGS)

test_FileUtil : test_FileUtil.cpp ../FileUtil.o
	g++ -o $(OUTPUT_DIR)/$@ $^ $(FLAGS) -pthread

test_Logger : test_Logger.cpp ../Logger.a
	g++ -o $(OUTPUT_DIR)/$@ $^ $(FLAGS)

test_LogStream : test_LogStream.cpp ../LogStream.o
	g++ -o $(OUTPUT_DIR)/$@ $^ $(FLAGS)

test_Timestamp : test_Timestamp.cpp ../Timestamp.o
	g++ -o $(OUTPUT_DIR)/$@ $^ $(FLAGS)

clean:
	- rm $(OUTPUT_DIR)/*