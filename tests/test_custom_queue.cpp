#include <gtest/gtest.h>
#include "../CustomClass.hpp"

using namespace std;

using namespace std::chrono;
using namespace std::this_thread;

template <typename T>
void pushThread(CustomQueue<T>& customQueue){
    for(int i = 0 ; i < 1000000 ; i++){
        customQueue.push(i , 1);
    }
}

template <typename T>
void popThread(CustomQueue<T>& customQueue){
    for(int i = 0 ; i < 1000000 ; i++){
        int temp = customQueue.pop(0);
    }
}

template <typename T>
void displaySizeThread(CustomQueue<T>& customQueue){
    
    sleep_for(milliseconds(3000));
    EXPECT_EQ(customQueue.getCounter(), 2000000); 
    EXPECT_EQ(customQueue.getSize() , 0);
    
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
    t3.join();

}