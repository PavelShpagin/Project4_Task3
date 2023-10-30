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
	int field1;
	int field2;
	std::mutex mutex1;
	std::shared_mutex mutex2;
};