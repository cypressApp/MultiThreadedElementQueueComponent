
#pragma once

#include <iostream>
#include <semaphore>
#include <mutex>

std::counting_semaphore<1> semPush(0);
std::counting_semaphore<1> semPop(0);

/**
 * @brief The class is the blueprint of the elements of queue
 * 
 * The instant of this class is a node of linkedlist which contain data
 * and the pointer to the next node of the linkedlist 
 * 
 */
template<typename T>
class QueueElement{

public:

    /**
     * @brief the content of element
     */
    T data;


    /**
     * @brief pointer to the next element of the queue
     */    
    QueueElement<T> *next;

    QueueElement(T data);

};

template<typename T>
class CustomQueue{

    /**
     * @brief the oldest element in the queue
     */
    QueueElement<T> *head;
    
    
    /**
     * @brief the newest element in the queue
     */
    QueueElement<T> *tail;
    
    
    /**
     * @brief current size of queue
     */
    size_t size;


    /**
     * @brief maximum capacity of queue
     */
    size_t maxSize;


    /**
     * @brief mutex to handle syncronzation and safe access to the queue without any race condition
     */
    std::mutex push_pop_mtx;

    /**
     * @brief function call counter which is helpful to check if any race condition has been happened
     */
    int counter;
    
public:

    CustomQueue(size_t maxSize = 5);
    

    /**
     * @brief check if the queue is empty.
     * 
     * This function check "size" to recognize if the queue is empty. 
     * 
     */
    bool isEmpty();


    /**
     * @brief check if the queue is full
     * 
     * This function compare the current size of queue with the max queue capacity.
     * 
     */
    bool isFull();


    /**
     * @brief enqueue new element in to the list
     * 
     * This function handlers syncronization, queue overflow and timeout before enqueuing new element
     * 
     * @param data is the content of the new element
     * @param timeout(ms) if it is non-zero, enqueue operation should be done within the timeout duration
     * 
     */
    void push(T data , int timeout = 0);
    

    /**
     * @brief execute enqueuing operation
     * 
     * If the queue is not held by other threads and is not full
     * then this function enqueue the new element.
     *  
     * @param data is the content of new element
     * 
     */
    void execPush(T data);


    /**
     * @brief dequeue the oldest element in to the list
     * 
     * This function handlers syncronization, queue underflow and timeout before dequeuing the oldest element
     * 
     * @param timeout(ms) if it is non-zero, dequeue operation should be done within the timeout duration
     * @return the content of the oldest element
     * 
     */
    T pop(int timeout = 0);


    /**
     * @brief execute dequeuing operation
     * 
     * If the queue is not held by other threads and is not empty
     * then this function dequeue the oldest element.
     *  
     * @return is the content of the dequeued element
     * 
     */
    T execPop();    

    /**
     * @brief get the size of queue
     * 
     * @return size of queue
     * 
     */
    size_t getSize();

    /**
     * @brief get how many times push and pop functions have been called.
     * 
     * @return function call counter 
     * 
     */
    int getCounter();
    
};

#include "CustomClass.tpp"