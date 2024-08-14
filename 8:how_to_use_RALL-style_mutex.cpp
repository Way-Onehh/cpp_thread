#include<iostream>
#include<thread>
#include<mutex>
#include<chrono>
#include<functional>

template<class lock_t,class adapter,typename...Arg>
void test(lock_t &m,Arg&&... args)
{
    //Avoid polluting namespaces
    using std::cout;
    using std::endl;
    using std::thread;

    thread th
    (
        [&m,&args...]()
        {   
            while (1)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                adapter lg(m,args...);
                cout<<"th get lock"<<endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
        }
    );
    
    while (1)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        adapter lg(m,args...);
        cout<<"main get lock"<<endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}



int main(int n,char* argv[])
{

    //what to use
    using std::lock_guard;//easy to use,but few feature.
    using std::unique_lock;//more feature.
    using std::timed_mutex;
    using std::mutex;
    timed_mutex m;

    //这是一个模板封装的函数，但是他没有体现，unique_lock<> 标志的作用 所以手动补充以下
    //refer_lock(_t) () 延迟上锁，应对不能马上锁上的环境，自己决定上锁时间，然后析构解锁 (t对应时间锁)
    //try_to_lock(_t)() 尝试上锁，通过operator bool owns_lock 来处理分支，然后析构解锁 (t对应时间锁)
    //adopt_lock(_t) () 已经上锁，防止双锁但是又想要析构解锁(t对应时间锁)

    //test<timed_mutex,lock_guard<timed_mutex>>(m);
    //test<timed_mutex,unique_lock<timed_mutex>>(m,std::adopt_lock_t());
    //test<timed_mutex,unique_lock<timed_mutex>>(m,std::try_to_lock_t());
    //test<timed_mutex,unique_lock<timed_mutex>>(m,std::adopt_lock_t());
    return 0;
}