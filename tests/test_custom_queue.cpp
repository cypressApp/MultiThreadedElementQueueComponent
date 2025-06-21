#include <gtest/gtest.h>
#include "../CustomClass.hpp"

using namespace std;

using namespace std::chrono;
using namespace std::this_thread;

size_t failPushCounter = 0;
size_t failPopCounter = 0;

template <typename T>
void pushThread(CustomQueue<T>& customQueue){
    for(int i = 0 ; i < (customQueue.getMaxQueueCapacity() / 4) ; i++){
        int result = customQueue.push(i , 1 , false);
        if(result) failPushCounter++;
    }
}

template <typename T>
void popThread(CustomQueue<T>& customQueue){

    for(int i = 0 ; i < (customQueue.getMaxQueueCapacity() / 4) ; i++){
        optional<T> temp = customQueue.pop(0 , false);
        if(temp == nullopt) failPopCounter++;
    }
}

template <typename T>
void verifyOperation(CustomQueue<T>& customQueue){
    
    sleep_for(milliseconds(3000));
    EXPECT_EQ(customQueue.getCounter(), 2000000); 
    EXPECT_EQ(customQueue.getSize() , 0);
    
}

template <typename T>
void displaySizeThread(CustomQueue<T>& customQueue){
    size_t displayCounter = 0;
    while(displayCounter++ < 8){
        this_thread::sleep_for(chrono::milliseconds(500));
        cout << left
             << setw(20) << "counter = " + to_string(customQueue.getCounter())
             << setw(18) << " | size = " + to_string(customQueue.getSize())
             << setw(20) << " | push fail = " + to_string(failPushCounter)
             << setw(20) << " | pop fail = " + to_string(failPopCounter)
             << endl; 
    }
}


TEST(QueueTest, PushAndPop) {
    CustomQueue<int> q(10);
    q.push(42);
    EXPECT_EQ(q.pop(), 42);
}


TEST(QueueTest2, PushAndPop2) {
    CustomQueue<int> q(10);
    q.push(105);
    EXPECT_EQ(q.pop(), 105);
}

TEST(TestCounter , UltimateCounterValue){
    size_t maxQueueCapacity = 4000000;

    CustomQueue<int> customQueue(maxQueueCapacity);
    
    thread t1([&](){ pushThread(customQueue); });
    thread t2([&](){ popThread(customQueue); });
    thread t3([&](){ displaySizeThread(customQueue); });

    t1.join();
    t2.join(); 
    t3.detach();

}