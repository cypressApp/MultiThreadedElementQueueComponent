#include <iostream>
#include <thread>
#include <chrono>
#include <iomanip>
#include <regex>
#include "CustomClass.hpp"
#include "autoconf.h"
#include <limits>

using namespace std;

size_t failPushCounter = 0;
size_t failPopCounter = 0;

template <typename T>
void pushThread(CustomQueue<T>& customQueue , T newData , int timeout){
    int result = customQueue.push(newData , timeout);
    if(result == CustomQueue<T>::OP_FAIL) {
        failPushCounter++;
    }else{
        cout << "\033[32m" << "push: " << newData << "\033[0m" << endl;
    }
}

template <typename T>
optional<T> popThread(CustomQueue<T>& customQueue , int timeout){
    optional<T> temp = customQueue.pop(timeout);
    if(temp == nullopt) failPopCounter++;
    return temp;
}

template <typename T>
void displaySizeThread(CustomQueue<T>& customQueue){
    cout << "\033[36m"  << "size: " + to_string(customQueue.getSize()) << "\033[0m" << endl;
}

template<typename T>
T getInput(string msg, T defaultValue){

    T temp = defaultValue;

    string input;
    bool validInput = false;

    do {
        cout << msg << " (" << temp << "): ";
        getline(cin, input);

        if (input.empty()) {
            validInput = true;
        } else {
            istringstream iss(input);
            if (iss >> temp && iss.eof()) {
                validInput = true;
            } else {
                cout << "\033[31m" << "Invalid format" << "\033[0m" << endl;
                temp = defaultValue;
            }
        }

    } while (!validInput);

    return temp;
}

optional<int> getPushValue(string command){
    
    regex pattern(R"(push\((\d+)\))");
    smatch match;

    if (regex_match(command, match, pattern)) {
        int value = stoi(match[1].str());
        return value;
    } else {
        cout << "\033[31m" << "Invalid format" << "\033[0m" << endl;
        return nullopt;
    }
}

int main(){

    size_t maxQueueCapacity = CONFIG_QUEUE_MAX_CAPIBILITY;
    int pushTimeout = CONFIG_PUSH_TIMEOUT;
    int popTimeout = CONFIG_POP_TIMEOUT;

    maxQueueCapacity = getInput<size_t>("Queue maxumim capacity" , CONFIG_QUEUE_MAX_CAPIBILITY);
    pushTimeout = getInput<size_t>("Push timeout (ms)" , CONFIG_PUSH_TIMEOUT);
    popTimeout = getInput<size_t>("Pop timeout (ms)" , CONFIG_POP_TIMEOUT);

    CustomQueue<int> customQueue(maxQueueCapacity);
    optional<int> pushCmd;
    string command;

    while(1){
        
        this_thread::sleep_for(chrono::milliseconds(10));
        cout << "> ";
        cin >> command;

        if(command == "pop()"){
            thread t1([&](){ 
                optional<int> temp =  popThread(customQueue , popTimeout);
                if(temp.has_value()){
                    cout << "\033[34m" << "pop: " << temp.value() << "\033[0m"  << endl; 
                }
            });
            t1.detach();
            continue;
        }else if(command == "size"){
            thread t1([&](){ displaySizeThread(customQueue); });
            t1.detach();
            continue;
        }

        pushCmd = getPushValue(command);
        if(pushCmd.has_value()){
            thread t1([&](){ 
                pushThread(customQueue , pushCmd.value() , pushTimeout); });
            t1.detach();
        }
    }

    return 0;
}   