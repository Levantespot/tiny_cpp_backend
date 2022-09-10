#ifndef TCB_BASE_NONCOPYABLE_H
#define TCB_BASE_NONCOPYABLE_H

namespace TCB
{

// 一个无法被拷贝和赋值的类，该类也不能实例化，只能作为基类供派生类继承
class noncopyable {
public:
    noncopyable (const noncopyable &) = delete;
    void operator=(const noncopyable &) = delete;
protected:
    noncopyable () = default;
    ~noncopyable () = default;
}; // class noncopyable

} // namespace TCB

#endif // #ifndef TCB_BASE_NONCOPYABLE_H