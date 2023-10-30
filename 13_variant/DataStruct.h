#pragma once

#include <iostream>
#include <string>
#include <mutex>
#include <shared_mutex>

class DataStruct {
public:
	//write into first element
	void write1(int value);

	//write into second element
	void write2(int value);

	//read the first element
	int read1();

	//read the second element
	int read2();

	//conversion to string
	operator std::string();

private:
	int field1 = 0;
	int field2 = 0;
	//30% read (10% read + 20% string) 10% write in total
	std::shared_mutex mutex1;
	//70% read (50% read + 20% string) 10% write in total
	std::shared_mutex mutex2;
};