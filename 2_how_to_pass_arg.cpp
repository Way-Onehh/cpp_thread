#include<iostream>
#include<thread>

int main(int n,char* argv[])
{
    //Avoid polluting namespaces
    using std::cout;
    using std::endl;
    using std::ref;
    using std::thread;
    
    //pass ref type
        int testtarget;
        auto printaddr = [](int & t){cout<<&t<<endl;};
        auto printvalue = [](int t){cout<<t<<endl;};
        printaddr(testtarget);
        //error way
        //thread th1(a,testtarget);
        //th1.join();
        thread th2(printaddr,ref(testtarget));
        th2.join();
    
    //dont attempt pass temporary value by pointer and refence ，
    //and ensure that the variables in the thread are valid (RAII is recommended)  
        thread th3;
        {
            int &testtarget=*new int;//Ensure that the variables are temporary
            testtarget=1;
            auto delayuse = [&printvalue](int & t)
            {
                std::this_thread::sleep_for(std::chrono::seconds(1)); //Ensure that the variables are temporary
                printvalue(t);
            };
            th3=move(thread(delayuse,ref(testtarget)));
            delete &testtarget;//Ensure that the variables are temporary
        }
        th3.join();


    return 0;
}