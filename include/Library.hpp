#include <any>
#include <dlfcn.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

#ifndef _LIBRARY_HPP_
#define _LIBRARY_HPP_

class LibraryErr : public std::exception
{
public:
    LibraryErr(const std::string &msg): message(msg){};
    const std::string &getMessage() const { return message; }

private:
    const std::string message;
};

template <typename T>
class Library
{
public:
    Library(std::string path): _path(path), _ptr(nullptr), obj(nullptr)
    {
        auto dlclose_deleter = [](void *ptr) {
            if (ptr) dlclose(ptr);
        };
        _ptr = std::shared_ptr<void>(dlopen(path.c_str(), RTLD_LAZY), dlclose_deleter);
        if (!_ptr) { throw LibraryErr("unable to open .so"); }
        T *(*symb)(void) = (T * (*)(void)) dlsym(_ptr.get(), "entrypoint");
        if (symb == 0x0) { throw LibraryErr("missing symbols"); }
        obj = std::shared_ptr<T>(symb());
        if (!obj) { throw LibraryErr("error using lib"); }
    };
    Library(Library &other)
    {
        _path = other._path;
        obj = other.obj;
        _ptr = other._ptr;
    };
    Library(Library &&other) noexcept
    {
        _path = std::move(other._path);
        obj = std::move(other.obj);
        _ptr = std::move(other._ptr);
    };

    ~Library(){};

    std::shared_ptr<T> operator->() { return obj; }

    Library &operator=(Library other)
    {
        std::swap(_path, other._path);
        std::swap(obj, other.obj);
        std::swap(_ptr, other._ptr);
        return *this;
    };

public:
    std::string _path;
    std::shared_ptr<void> _ptr;
    std::shared_ptr<T> obj;
};

#endif    //_LIBRARY_HPP_
