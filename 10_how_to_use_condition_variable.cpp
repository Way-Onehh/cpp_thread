#include<thread>
#include<iostream>
#include<condition_variable>
#include<mutex>
int main(int n,char**)
{
    using std::cout,std::endl,std::thread,std::condition_variable
         ,std::unique_lock,std::mutex,std::cin,std::string;
    
    const int workernum =10;
    thread workerThs[workernum];
    condition_variable cv;
    mutex m;
    auto workerf=[&m,&cv]()
    {   
        
        unique_lock uk(m,std::defer_lock);
        cout<<"等待中"<<endl;
        //cv.wait(uk);
        cout<<std::this_thread::get_id<<":开始"<<endl;
    };

    for(auto &e:workerThs){e=thread(workerf);}
    
  
    // string ch;
    // while (cin>>ch)
    // {
    //    if(ch=="quit")break;
    //    if(ch=="1")
    
    //    cv.notify_one();
    // }
    
    for(auto &e:workerThs){e.join();}
}
