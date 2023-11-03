#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <random>
#include <algorithm>
#include <fstream>
#include "DataStruct.cpp"
#include "timeCount.cpp"

using namespace std;

typedef vector<string>::iterator Iterator;

void generateFile(int actionCount, string fileName, int field0RFreq, int field1RFreq, int field0WFreq, int field1WFreq, int stringFreq) 
{
    int fieldCount = 2;
    vector<string> actions;
    random_device rd;
    mt19937 g(rd());

    for (int i = 0; i < round(field0RFreq * actionCount / 100); i++) {
        actions.push_back("read 0");
    }
    for (int i = 0; i < round(field0WFreq * actionCount / 100); i++) {
        actions.push_back("write 0 1");
    }

    for (int i = 0; i < round(field1RFreq * actionCount / 100); i++) {
        actions.push_back("read 1");
    }
    for (int i = 0; i < round(field1WFreq * actionCount / 100); i++) {
        actions.push_back("write 1 1");
    }

    for (int i = 0; i < round(stringFreq * actionCount / 100); i++) {
        actions.push_back("string");
    }

    shuffle(actions.begin(), actions.end(), g);

    ofstream file(fileName);
    if (file.is_open()) {
        for (const string& action : actions) {
            file << action << "\n";
        }
        file.close();
    }
    else {
        throw runtime_error("Failed to open file for writing.");
    }
}

void execute(DataStruct& data, Iterator first, Iterator last, long long& totalTime)
{
    for (auto it = first; it != last; ++it) {
        istringstream iss(*it);
        string action;
        iss >> action;

        if (action == "string") {
            totalTime += timeCount([&data] {
                (string)data;
                });

        }
        else if (action == "read") {
            
            int fieldNumber;
            iss >> fieldNumber;

            if (fieldNumber == 0) {
                totalTime += timeCount([&data] {
                    data.read1();
                    });
            }
            else if (fieldNumber == 1) {
                totalTime += timeCount([&data] {
                    data.read2();
                    });
            }
            else {
                throw runtime_error("Unknown number of field.");
                break;
            }
        }
        else if (action == "write") {
            int fieldNumber, value;
            iss >> fieldNumber >> value;

            if (fieldNumber == 0) {
                totalTime += timeCount([&data, value] {
                    data.write1(value);
                    });
            }
            else if (fieldNumber == 1) {
                totalTime += timeCount([&data, value] {
                    data.write2(value);
                    });
            }
            else {
                throw runtime_error("Unknown number of field.");
                break;
            }
        }
        else {
            throw runtime_error("Unknown type of action.");
            break;
        }
    }
}

vector<string> transformDataInArr(const string& fileName)
{
    ifstream file(fileName);

    if (!file.is_open()) {
        throw runtime_error("Failed to open file for reading.");
    }

    vector<string> lines;
    string line;

    while (getline(file, line)) {
        if (line.empty()) {
            break;
        }
        lines.push_back(line);
    }

    file.close();
    return lines;
}

void transformedTimeOut(long long time)
{

    if (time < 10000) {

        cout<<"Time: " << time << " ns" << endl;
    }
    else {
        double time_in_seconds = static_cast<double>(time) / 1e9;
        cout<<"Time: " << time_in_seconds << " sec" << endl;
    }
}

void launchThreads(Iterator first, Iterator last, int threadsNum)
{
    if (threadsNum < 1) {
        throw runtime_error("Bad value for threads number.");
    }

    DataStruct data;
    unsigned long const length = distance(first, last);
    unsigned long const block_size = length / threadsNum;
    vector<jthread> threads(threadsNum - 1);
    vector<long long> results(threadsNum, 0);
    Iterator block_start = first;
    for (int i = 0; i < (threadsNum - 1); ++i)
    {
        Iterator block_end = block_start;
        advance(block_end, block_size);
        threads[i] = jthread(execute, ref(data), block_start, block_end, ref(results[i]));
        block_start = block_end;
    }
    execute(data, block_start, last, results[threadsNum - 1]);

    transformedTimeOut(*max_element(results.begin(), results.end()));
}



 int main()
{
    try {
        cout << "Generating data..." << endl;
        generateFile(90000, "taskFreq.txt", 10, 50, 10, 10, 20);
        generateFile(90000, "equalFreq.txt", 20, 20, 20, 20, 20);
        generateFile(90000, "difFreq.txt", 5, 5, 25, 25, 40);

        auto taskActionFreq = transformDataInArr("taskFreq.txt");
        auto equalActionFreq = transformDataInArr("equalFreq.txt");
        auto difActionFreq = transformDataInArr("difFreq.txt");
        cout << "Finsih generating data!" << endl;

        cout << "Starting executing actions..." << endl<<endl;
        for (int i = 1; i < 4; ++i) {
            cout << "Executing in " << i << " threads" << endl;
            cout << "Actions with task frequency: " << endl;
            launchThreads(taskActionFreq.begin(), taskActionFreq.end(), i);
            cout << "Actions with equal frequency: " << endl;
            launchThreads(equalActionFreq.begin(), equalActionFreq.end(), i);
            cout << "Actions with different from task frequency: " << endl;
            launchThreads(difActionFreq.begin(), difActionFreq.end(), i);
            cout << endl;
        }
    }   
    catch (const exception& e)
    {
        cerr << e.what() << endl;
    }
     return 0;
 }