//to use recursive_mutex and recursive_time_mutex
#include<iostream>
#include<thread>
#include<functional>
#include<mutex>
#define mode 1

#if mode==0
int main(int n,char* argv[])
{
    //Avoid polluting namespaces
    using std::cout;
    using std::endl;
    using std::thread;
    using std::mutex;

    mutex m;
    //这里用function来延迟创建一下，赖写英文了。
    std::function<void(int)> f=[&f,&m](int n)
    {
        m.lock();
        if(n==0) return;
        cout<<n<<endl;
        f(n-1); 
        m.unlock();
    };

    f(10);

    return 0;
}
#endif

#if mode==1
int main(int n,char* argv[])
{
    //Avoid polluting namespaces
    using std::cout;
    using std::endl;
    using std::thread;
    using std::recursive_mutex;

    recursive_mutex m;
    //这里用function来延迟创建一下，赖写英文了。
    std::function<void(int)> f=[&f,&m](int n)
    {
        m.lock();
        if(n==0) return;
        cout<<n<<endl;
        f(n-1); 
        m.unlock();
    };

    f(10);

    return 0;
}
#endif