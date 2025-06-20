#include <iostream>
#include <thread>
#include <iomanip>
#include "CustomClass.hpp"
#include "autoconf.h"

using namespace std;

size_t failPushCounter = 0;
size_t failPopCounter = 0;

template <typename T>
void pushThread(CustomQueue<T>& customQueue){
    failPushCounter = 0;
    for(int i = 0 ; i < 1000000 ; i++){
   //     this_thread::sleep_for(chrono::seconds(2));
        int result = customQueue.push(i , 1);
        if(result == CustomQueue<T>::OP_FAIL) failPushCounter++;
    }
}

template <typename T>
void popThread(CustomQueue<T>& customQueue){
    failPopCounter = 0;
    for(int i = 0 ; i < 1000000 ; i++){
        optional<T> temp = customQueue.pop(1);
        if(temp == nullopt) failPopCounter++;
    }
}

template <typename T>
void displaySizeThread(CustomQueue<T>& customQueue){
    size_t displayCounter = 0;
    while(displayCounter++ < 8){
        this_thread::sleep_for(chrono::milliseconds(500));
        cout << left
             << setw(20) << "counter = " + to_string(customQueue.getCounter())
             << setw(20) << "size = " + to_string(customQueue.getSize())
             << setw(20) << "push fail = " + to_string(failPushCounter)
             << setw(20) << "pop fail = " + to_string(failPopCounter)
             << endl; 
    }
}

int main(){

    size_t maxQueueCapacity = 4000000;
    // size_t pushTimeout = 1;
    // size_t popTimeout = 1;

    // cout << "Max queue capacity: ";
    // cin >> maxQueueCapacity;

    CustomQueue<int> customQueue(maxQueueCapacity);
    
    thread t1([&](){ pushThread(customQueue); });
    thread t2([&](){ popThread(customQueue); });
    thread t3([&](){ displaySizeThread(customQueue); });

    // thread t1(&CustomQueue<int>::push, &customQueue , 10 , 1);
    // t1.join();

    t1.join();
    t2.join();    
    t3.join();

    return 0;
}   