#include<iostream>
#include<thread>

int main(int n,char* argv[])
{
    //Avoid polluting namespaces
    using std::cout;
    using std::endl;
    using std::thread;

    //Function entry to print something and thread  id; 
    auto &&la= [](int times)
    {
        for (size_t i = 0; i < times; i++)
        {
        cout<<"wr is handsome man"<<" : "<<std::this_thread::get_id()<<endl;
        }
    };
    
    //Create thread by Function entry , pass the parameters, and wait the thread executing.
    auto &&th = thread(la,100);
    la(10);
    th.join();


    return 0;
}