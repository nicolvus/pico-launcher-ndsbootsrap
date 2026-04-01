#pragma once
#include <string.h>
#include "StringUtil.h"

template <typename CharType, int MaxLength>
class String
{
    CharType _buffer[MaxLength + 1];

public:
    String()
    {
        _buffer[0] = 0;
    }

    template<class T, class = std::enable_if_t<std::is_same_v<T, CharType>>>
    String(const T* const & str)
    {
        StringUtil::Copy(_buffer, str, MaxLength + 1);
    }

    explicit String(const char* str)
    {
        StringUtil::Copy(_buffer, str, MaxLength + 1);
    }

    template<std::size_t N>
    constexpr String(const CharType(&str)[N])
    {
        if (N * sizeof(CharType) <= sizeof(_buffer))
            memcpy(_buffer, str, N * sizeof(CharType));
        else
        {
            memcpy(_buffer, str, MaxLength * sizeof(CharType));
            _buffer[MaxLength] = 0;
        }
    }

    template<class T, class = std::enable_if_t<std::is_same_v<T, CharType>>>
    void operator=(const T* const & str)
    {
        StringUtil::Copy(_buffer, str, MaxLength + 1);
    }

    template<std::size_t N>
    constexpr void operator=(const CharType(&str)[N])
    {
        if (N * sizeof(CharType) <= sizeof(_buffer))
            memcpy(_buffer, str, N * sizeof(CharType));
        else
        {
            memcpy(_buffer, str, MaxLength * sizeof(CharType));
            _buffer[MaxLength] = 0;
        }
    }

    constexpr operator const CharType*() const { return _buffer; }

    constexpr const CharType* GetString() const { return _buffer; }
};