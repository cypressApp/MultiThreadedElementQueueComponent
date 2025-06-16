
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

    bool tempIsEmpty = isEmpty();

    if(isFull()){
        cout << "Queue overflow!" << endl;
        semPush.acquire();
    }
    
    if(timeout == 0){
        execPush(data);
    }else{
        auto pushWithTimeout = std::async(std::launch::async, &CustomQueue<T>::execPush , this , data);
        if (pushWithTimeout.wait_for(std::chrono::seconds(timeout)) == std::future_status::ready) {
            std::cout << "Push: " << data << " ########### " << endl;
        } else {
            std::cout << "Timeout occurred!" << endl;
        }
    }

    if(tempIsEmpty){
        semPop.release();
    }
    
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

    bool tempIsFull = isFull();

    if(isEmpty()){
        cout << "Queue underflow" << endl;
        semPop.acquire();
    }

    if(timeout == 0){
        return execPop();
    }else{
        auto popWithTimeout = std::async(std::launch::async, &CustomQueue<T>::execPop , this);
        if (popWithTimeout.wait_for(std::chrono::seconds(timeout)) == std::future_status::ready) {
            return popWithTimeout.get();
        } else {
            std::cout << "Timeout occurred!" << endl;
        }        
    }

    if(tempIsFull){
        semPush.release();
    }

    return -1;
}

template<typename T>
T CustomQueue<T>::peek(){

    if(isEmpty()){
        cout << "Queue is empty" << endl;
        return -1;
    }

    return head->data;
}

template<typename T>
size_t CustomQueue<T>::getSize(){
    return size;
}