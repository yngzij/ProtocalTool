#ifndef SINGLETON_H
#define SINGLETON_H

template <typename T>
class Singleton {
public:
    static T& Instance() {
        if (nullptr == value_ ) {
            value_ = new T();
        }
        return *value_;
    }

    static void destory() {
        delete value_;
    }

private:
    Singleton();
    ~Singleton();

    Singleton(const Singleton &);
    Singleton& operator = (const Singleton &);

private:
    static T *value_;
};

template <typename T>
T *Singleton<T>::value_ = nullptr;

#endif // SINGLETON_H
