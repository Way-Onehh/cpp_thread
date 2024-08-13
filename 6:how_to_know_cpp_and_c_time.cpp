#include<iostream>
#include<thread>
//include time library
#include<chrono>
#include<time.h>
//
int main(int n,char ** argv)
{
    //in c++ chrono
    //to use C++ features create time easyly.(support ns to h)
    using std::chrono::operator""s;
    using std::chrono::operator""ms;
    //other
    using std::cout;
    using std::endl;

    //In chrono, time_point type is used ,by thread library
    cout<<"sleeping start "<<endl;
    std::this_thread::sleep_for(1s);
    std::this_thread::sleep_for(1000ms);
    cout<<"sleeping end"<<endl;
    
    //c++ time_point is integer type and have two forms ,one steady_clock ,one system_clock
    std::chrono::system_clock::time_point t= std::chrono::system_clock::now();
    cout<<*(long long*)(&t)<<endl;

    //In c time.h library ,time_t type is used. It can turn into char type to output.
    //system_clock::time_point can turn into time_t;
    time_t &&ct=std::chrono::system_clock::to_time_t(t);
    cout<<ctime(&ct)<<endl; 

    //By the way ，tm type can get elements in time_t. 
    tm &ttm=*localtime(&ct);
    cout<<ttm.tm_year+1900<<endl;//tips: tm_year is 1900--now,so it should add 1900.
    return 0;
}
