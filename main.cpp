#include <iostream>
#include <thread>
#include "CustomClass.hpp"
#include "autoconf.h"

using namespace std;

template <typename T>
void pushThread(CustomQueue<T>& customQueue){
    for(int i = 0 ; i < 1000000 ; i++){
   //     this_thread::sleep_for(chrono::seconds(2));
        customQueue.push(i , 0);// (i + 1) * 10 , 1);
    }
}

template <typename T>
void popThread(CustomQueue<T>& customQueue){
    for(int i = 0 ; i < 1000000 ; i++){
        int temp = customQueue.pop(0);
       // cout << "pop: " << temp << endl;
    }
}

template <typename T>
void displaySizeThread(CustomQueue<T>& customQueue){
    while(1){
        this_thread::sleep_for(chrono::milliseconds(500));
        cout << "counter = " << customQueue.getCounter() << " , size = " << customQueue.getSize() << endl;  
    }
}

int main(){

    size_t maxQueueCapacity = 4000000;

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