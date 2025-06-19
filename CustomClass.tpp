
#pragma once

#include <iostream>
#include <future>
#include <chrono>
#include <thread>
#include <vector>
#include "CustomClass.hpp"

using namespace std;

template<typename T>
QueueElement<T>::QueueElement(T data){
    this->data = data;
    next = nullptr;
}

template<typename T>
CustomQueue<T>::CustomQueue(size_t maxSize){
    this->maxSize = maxSize;
    size = 0;
    head = nullptr;
    tail = nullptr;
    counter = 0;
}

template<typename T>
bool CustomQueue<T>::isEmpty(){
    return size == 0;
}

template<typename T>
bool CustomQueue<T>::isFull(){
    return size == maxSize;
}

template<typename T>
void CustomQueue<T>::execPush(T data){

    if(isEmpty()){
        head = new QueueElement(data);
        tail = head;
        size++;
        return;
    }
    
    QueueElement<T> *temp = new QueueElement(data);
    tail->next = temp;
    tail = temp;
    
    size++;

}

template<typename T>
void CustomQueue<T>::push(T data , int timeout){

    unique_lock<mutex> lock(push_pop_mtx);

    counter++;

    bool tempIsEmpty = isEmpty();

    auto start = chrono::steady_clock::now();
    while(isFull()){
        //cout << "Queue overflow!" << endl;
        lock.unlock();
        if(timeout > 0){
            while (isFull()) {
                if (chrono::steady_clock::now() - start >= chrono::milliseconds(timeout)) {
                    if(tempIsEmpty){
                        semPop.release();
                    }
                    return;
                }
            }
        }else{
            semPush.acquire();   
        }
        lock.lock();
    }

    if(timeout == 0){
        execPush(data);
    }else{
        if (chrono::steady_clock::now() - start <= chrono::milliseconds(timeout)) {
            execPush(data);
        } 
    }

    if(tempIsEmpty){
        semPop.release();
    }
    
    lock.unlock();

}

template<typename T>
T CustomQueue<T>::execPop(){

    int data = head->data;
    
    QueueElement<T> *temp = head;
    
    if(head == tail){
        head = nullptr;
        tail = nullptr;
    }else{
        head = temp->next;
    }
    
    delete(temp);
    
    size--;
    
    return data;

}

template<typename T>
T CustomQueue<T>::pop(int timeout){

    unique_lock<mutex> lock(push_pop_mtx);
    T result = -1;    
    counter++;

    bool tempIsFull = isFull();

    auto start = chrono::steady_clock::now();
    while(isEmpty()){
        // cout << "Queue underflow" << endl;
        lock.unlock();
        if(timeout > 0){
            while (isEmpty()) {
                if (chrono::steady_clock::now() - start >= chrono::milliseconds(timeout)) {
                    if(tempIsFull){
                        semPush.release();
                    }
                    return -1;
                }
            }
        }else{
            semPop.acquire();
        }
        lock.lock();
    }

    if(timeout == 0){
        result = execPop();
    }else{
        if (chrono::steady_clock::now() - start < chrono::milliseconds(timeout)) {
            result = execPop();
        }         
    }

    if(tempIsFull){
        semPush.release();
    }

    lock.unlock();

    return result;
}

template<typename T>
size_t CustomQueue<T>::getSize(){
    return size;
}

template<typename T>
int CustomQueue<T>::getCounter(){
    return counter;
}