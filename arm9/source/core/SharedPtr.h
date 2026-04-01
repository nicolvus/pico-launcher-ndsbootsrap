#pragma once

static inline u32 arm_getCpsr()
{
    u32 cpsr;
    asm volatile("mrs %0, cpsr" : "=r" (cpsr));
    return cpsr;
}

static inline void arm_setCpsrControl(u32 cpsrControl)
{
    asm volatile("msr cpsr_c, %0" :: "r" (cpsrControl) : "cc");
}

static inline u32 arm_disableIrqs(void)
{
    u32 oldCpsr = arm_getCpsr();
    arm_setCpsrControl(oldCpsr | 0x80);
    return oldCpsr;
}

static inline void arm_restoreIrqs(u32 oldCpsr)
{
    arm_setCpsrControl(oldCpsr);
}

template <class T>
class SharedPtr
{
    T* _pointer;
    vu32* _refCount;

public:
    SharedPtr()
        : _pointer(nullptr), _refCount(nullptr) { (void)sizeof(T); }

    explicit SharedPtr(T* pointer)
        : _pointer(pointer), _refCount(pointer ? new u32(1) : nullptr) { (void)sizeof(T); }

    SharedPtr(const SharedPtr& other)
    {
        u32 irq = arm_disableIrqs();
        _pointer = other._pointer;
        _refCount = other._refCount;
        if (_pointer)
        {
            (*_refCount)++;
        }
        arm_restoreIrqs(irq);
    }

    SharedPtr(SharedPtr&& other)
        : _pointer(other._pointer), _refCount(other._refCount)
    {
        other._pointer = nullptr;
        other._refCount = nullptr;
    }

    ~SharedPtr()
    {
        Reset();
    }

    [[gnu::noinline]]
    SharedPtr& operator=(const SharedPtr& other)
    {
        u32 irq = arm_disableIrqs();
        T* pointer = _pointer;
        if (pointer)
        {
            vu32* refCount = _refCount;
            u32 newValue = *refCount - 1;
            *refCount = newValue;
            _pointer = other._pointer;
            _refCount = other._refCount;
            if (_pointer)
            {
                (*_refCount)++;
            }
            arm_restoreIrqs(irq);
            if (newValue == 0)
            {
                delete pointer;
                delete refCount;
            }
        }
        else
        {
            _pointer = other._pointer;
            _refCount = other._refCount;
            if (_pointer)
            {
                (*_refCount)++;
            }
            arm_restoreIrqs(irq);
        }
        return *this;
    }

    [[gnu::noinline]]
    SharedPtr& operator=(SharedPtr&& other)
    {
        u32 irq = arm_disableIrqs();
        T* pointer = _pointer;
        if (pointer)
        {
            vu32* refCount = _refCount;
            u32 newValue = *refCount - 1;
            *refCount = newValue;
            _pointer = other._pointer;
            _refCount = other._refCount;
            other._pointer = nullptr;
            other._refCount = nullptr;
            arm_restoreIrqs(irq);
            if (newValue == 0)
            {
                delete pointer;
                delete refCount;
            }
        }
        else
        {
            _pointer = other._pointer;
            _refCount = other._refCount;
            other._pointer = nullptr;
            other._refCount = nullptr;
            arm_restoreIrqs(irq);
        }
        return *this;
    }

    [[gnu::noinline]]
    void Reset()
    {
        u32 irq = arm_disableIrqs();
        T* pointer = _pointer;
        if (pointer)
        {
            vu32* refCount = _refCount;
            u32 newValue = *refCount - 1;
            *refCount = newValue;
            _pointer = nullptr;
            _refCount = nullptr;
            arm_restoreIrqs(irq);
            if (newValue == 0)
            {
                delete pointer;
                delete refCount;
            }
        }
        else
        {
            arm_restoreIrqs(irq);
        }
    }

    constexpr T& operator*() const { return *_pointer; }
    constexpr T* operator->() const { return _pointer; }

    constexpr T* GetPointer() const { return _pointer; }
    constexpr u32 GetRefCount() const { return _refCount ? *_refCount : 0; }
    constexpr bool IsValid() const { return _pointer; }
};
