#include<iostream>
#include<thread>
#include<mutex>
#define mode 1

#if mode == 0
//this isnt deadlock,due to same order that dont become loop
int main(int n ,char ** arg)
{
    //Avoid polluting namespaces
    using std::cout;
    using std::endl;
    using std::thread;
    using std::mutex;

    //Allocate two mutexs, two threads and
    //a lamba function(ensure sameorder to lock) ,for test 
    mutex m1,m2;
    thread th1,th2;
    int times;
    auto Sameorder=[&m1,&m2](int count)
    {
        for (size_t i = 0; i < count; i++)
        {
            m1.lock();
            m2.lock();
            cout<<"this is "<<i<<"time "<<std::this_thread::get_id()<<endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
            m2.unlock(); 
            m1.unlock();
        }
    };

    //threads start to test
    th1=move(thread(Sameorder,100));
    th2=move(thread(Sameorder,100));
    th1.join();
    th2.join();
    
    return 0;
}
#endif

#if mode == 1
//this is deadlock,two mutexs are given to different two threads;
int main(int n ,char ** arg)
{
    //Avoid polluting namespaces
    using std::cout;
    using std::endl;
    using std::thread;
    using std::mutex;

    //Allocate two mutexs, two threads and
    //two lamba function(ensure differentorder to get a mutex then wait a while) ,for test 
    mutex m1,m2;
    thread th1,th2;
    int times;
    auto differentorder1=[&m1,&m2](int count)
    {
        for (size_t i = 0; i < count; i++)
        {
            m1.lock();
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
            m2.lock();
            cout<<"this is "<<i<<"time "<<std::this_thread::get_id()<<endl;
            m2.unlock(); 
            m1.unlock();
        }
    };
    auto differentorder2=[&m1,&m2](int count)
    {
        for (size_t i = 0; i < count; i++)
        {
            m2.lock();
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
            m1.lock();
            cout<<"this is "<<i<<"time "<<std::this_thread::get_id()<<endl;
            m2.unlock(); 
            m1.unlock();
        }
    };


    //threads start to test
    th1=move(thread(differentorder1,100));
    th2=move(thread(differentorder2,100));
    th1.join();
    th2.join();
    
    return 0;
}
#endif

