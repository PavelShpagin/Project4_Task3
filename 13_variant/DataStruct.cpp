
#include "DataStruct.h"

void DataStruct::write1(int value) {
	std::lock_guard<std::shared_mutex> lock(mutex1);
	field1 = value;
}

void DataStruct::write2(int value) {
	std::lock_guard<std::shared_mutex> lock(mutex2);
	field2 = value;
}

int DataStruct::read1() {
	std::shared_lock<std::shared_mutex> lock(mutex1);
	return field1;
}

int DataStruct::read2() {
	std::shared_lock<std::shared_mutex> lock(mutex2);
	return field2;
}

DataStruct::operator std::string() {
	std::shared_lock<std::shared_mutex> lock1(mutex1);
	std::shared_lock<std::shared_mutex> lock2(mutex2);
	return "Field1: " + std::to_string(field1) + ", Field2: " + std::to_string(field2);
}
