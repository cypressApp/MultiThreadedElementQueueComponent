
#pragma once

#include <iostream>
#include <semaphore>

std::counting_semaphore<1> semPush(0);
std::counting_semaphore<1> semPop(0);

template<typename T>
class QueueElement{

public:

    T data;
    QueueElement<T> *next;

    QueueElement(T data);

};

template<typename T>
class CustomQueue{

    QueueElement<T> *head;
    QueueElement<T> *tail;
    size_t size;
    size_t maxSize;

public:

    CustomQueue(size_t maxSize = 5);
    bool isEmpty();
    bool isFull();
    void execPush(T data);
    void push(T data , int timeout = 0);
    T execPop();
    T pop(int timeout = 0);
    T peek();
    size_t getSize();
    
};

#include "CustomClass.tpp"