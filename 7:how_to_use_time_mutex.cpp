#include<iostream>
#include<thread>
#include<mutex>
#include<chrono>

//You don't need to pay attention to this complicated code, just look at the comments
int main(int n,char* argv[])
{
    using std::cout;
    using std::endl;
    using std::thread;
    using std::timed_mutex;
    //How to create c++ time value
    using std::chrono::operator""ms;
    using std::chrono::operator""s;
    
    timed_mutex tm;
    thread th;
    auto looplock=[&tm]()
    {
        std::this_thread::sleep_for(20ms);
        while (1)
        {
            std::chrono::steady_clock::time_point t1=std::chrono::steady_clock::now();
            //Look this. "Try_lock_for(1s)" make mutex to control time;
            if(tm.try_lock_for(1s))
            {
                cout<<"success to lock"<<endl;
                tm.unlock(); 
                std::this_thread::sleep_for(1s - (std::chrono::steady_clock::now()-t1));
            }
            else
            {
                cout<<"fail to lock"<<endl;
            }     
        }
    };
    th =thread(looplock);

    while (1)
    {
        tm.lock();
        std::this_thread::sleep_for(2.5s);
        tm.unlock();
        std::this_thread::sleep_for(2.5s);
    }

    //Here's a small detail. The threads are not waiting for joining,
    //because all the threads are in an endless loop, and it is impossible 
    //to joining at all, so they can only be closed by youself.Normally, 
    //it will be checked after the main thread ends. Whether the child 
    //thread joins or not
    return 0;
}