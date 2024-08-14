#ifndef SINGLETON_H
#define SINGLETON_H
#include <global.h>

template <typename T>
class Singleton
{
protected:
    Singleton() = default;
    Singleton(const Singleton<T>& ) = delete ;
    Singleton& operator = (const Singleton<T>& ) = delete ;

    static std::shared_ptr<T> _instance;

public:
    static std::shared_ptr<T> GetInstance()
    {
        static std::once_flag s_flag;
        std::call_once(s_flag,[&](){
            // 这里不用 make_shared 是因为 make_shared 需要访问类的构造函数，而这个单例类的构造函数是 protected 的，无法被智能指针访问
            _instance = std::shared_ptr<T> (new T);
        });
        return _instance;
    }

    void PrintAddress()
    {
        std::cout << _instance.get() << std::endl;
    }

    ~Singleton()
    {
        std::cout << "这个单例被析构" << std::endl;
    }
};



template <typename T>
std::shared_ptr<T> Singleton<T>::_instance = nullptr;



#endif // SINGLETON_H
