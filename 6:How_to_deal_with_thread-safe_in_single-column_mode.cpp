#include<thread>
#include<mutex>
#include<iostream>
#include<functional>


int  main(int n , char** args)
{
    //懒汉模式 ok。他是线程安全的
    {
    struct Log
    {
        static Log & getinstance()
        {
            static  Log clog;
            return clog;
        }

        std::function<void(const char *)> Logout=[](const char * str)
        {
            std::cout<<str<<std::endl;
        };
       
    private:
        Log(){}
        Log(const Log&) =delete;
        Log(Log && )=delete;
    };
        Log::getinstance().Logout("123");
    }
    
    //饿汉模式
    {
        //线程不安全示范
        {  
        struct Log
        {
            static Log & getinstance()
            {
                static  Log * clog=nullptr;
                if(!clog) clog=new Log ;//要保证对象在程序中一直存在，所以这个内存给系统去回收。我就不delete了
                return *clog;
            }

            std::function<void(const char *)> Logout=[](const char * str)
            {
                std::cout<<str<<std::endl;
            };
        
        private:
            Log(){}
            Log(const Log&) =delete;
            Log(Log && )=delete;
        };
        Log::getinstance().Logout("123");  
        }
        //线程安全示范
        {
        struct Log
        {
            static Log & getinstance()
            {
                static  Log * clog=nullptr;
                static  std::mutex m;   //保证所有线程拿到的是第一个。  
                m.lock();
                if(!clog) clog=new Log ;//要保证对象在程序中一直存在，所以这个内存给系统去回收。我就不delete了
                m.unlock();
                return *clog;
            }

            std::function<void(const char *)> Logout=[](const char * str)
            {
                std::cout<<str<<std::endl;
            };
        
        private:
            Log(){}
            Log(const Log&) =delete;
            Log(Log && )=delete;
        };
        }
        //减阻塞模式，双检查锁
        {
        struct Log
        {
            static Log & getinstance()
            {
                static  Log * clog=nullptr;
                static  std::mutex m;   //保证所有线程拿到的是第一个。  
                
                if(!clog) //只有空才阻塞 
                {//第一次判断为空的线程
                m.lock();//其中一个线程会回去分配，阻塞其他的然后在检测以下发现不是空，就可以啥也不干。
                if(!clog) clog=new Log ;     //要保证对象在程序中一直存在，所以这个内存给系统去回收。我就不delete了
                m.unlock();
                return *clog;
                }
            }

            std::function<void(const char *)> Logout=[](const char * str)
            {
                std::cout<<str<<std::endl;
            };
        
        private:
            Log(){}
            Log(const Log&) =delete;
            Log(Log && )=delete;
        };

        Log::getinstance().Logout("123");  
        }
        //减阻塞模式，双检查锁, volatile防止重排序
        {
        struct Log
        {
            static Log & getinstance()
            {
                static  Log * volatile clog=nullptr; //不是 volatile  Log *  clog 
                static  std::mutex m;   //保证所有线程拿到的是第一个。  
                
                if(!clog) //只有空才阻塞 
                {//第一次判断为空的线程
                m.lock();//其中一个线程会回去分配，阻塞其他的然后在检测以下发现不是空，就可以啥也不干。
                if(!clog) clog=new Log ;     //要保证对象在程序中一直存在，所以这个内存给系统去回收。我就不delete了
                m.unlock();
                return *clog;
                }
            }

            std::function<void(const char *)> Logout=[](const char * str)
            {
                std::cout<<str<<std::endl;
            };
        
        private:
            Log(){}
            Log(const Log&) =delete;
            Log(Log && )=delete;
        };

        Log::getinstance().Logout("123"); 
        }
        //使用call_once, volatile防止重排序
        {
        struct Log
        {
            static Log & getinstance()
            {
                static  Log * volatile clog=nullptr; //不是 volatile  Log *  clog 
                static  std::once_flag once;
                auto init= [&]()
                {
                    clog=new Log;
                    std::cout<<"完成"<<std::endl;
                };

                std::call_once(once,init);

                return *clog;
            }
            

            std::function<void(const char *)> Logout=[](const char * str)
            {
                std::cout<<str<<std::endl;
            };
        
        private:
            Log(){}
            Log(const Log&) =delete;
            Log(Log && )=delete;
        };

        Log::getinstance().Logout("123"); 
        Log::getinstance().Logout("123"); 
        }
    }
}
//auto _main =std::bind(main); 