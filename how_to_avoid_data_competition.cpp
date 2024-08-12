#include<iostream>
#include<thread>
#include<mutex>
#define mode 1

#if mode==0
//what is date competition
int main(int n,char* argv[])
{
    //Avoid polluting namespaces
    using std::cout;
    using std::endl;
    using std::ref;
    using std::thread;

    //threads competing for one piece of data can lead to unpredictable results
    int target=0;
    auto get=[&target]()->int &{return target;}; 
    auto set=[&get](int count)
    {
        for (size_t i = 0; i < count; i++)
        {
            get()++;
            std::this_thread::sleep_for(std::chrono::microseconds(20));//Increase serendipity
        }
    }; 
    
    //Allocate thread pools
    thread th[10];
    for(auto &e:th)
    {
        e=move(thread(set,100));
    }

    for(auto &e:th)
    {
        e.join();
    }

    //check the value
    cout<<get();

    return 0;
}
#endif

#if mode==1
//how to avoid it
int main(int n,char* argv[])
{
    //Avoid polluting namespaces
    using std::cout;
    using std::endl;
    using std::ref;
    using std::mutex;
    using std::thread;
    
    //threads competing for one piece of data can lead to unpredictable results,
    //but there is mutex for lock 
    mutex m;
    int target=0;
    auto get=[&target]()->int &{return target;}; 
    auto set=[&get,&m](int count)
    {
        for (size_t i = 0; i < count; i++)
        {   
            m.lock();
            get()++;
            std::this_thread::sleep_for(std::chrono::microseconds(20));//Increase serendipity
            m.unlock();
        }
    }; 
    
    //Allocate thread pools
    thread th[10];
    for(auto &e:th)
    {
        e=move(thread(set,100));
    }

    for(auto &e:th)
    {
        e.join();
    }

    //check the value (10*100)
    cout<<get();
    return 0;
}
#endif