#pragma once
#include "../../common/logger/IOutputStream.h"
#include "core/semihosting.h"

class SemihostingOutputStream : public IOutputStream
{
public:
    void Write(const char* str) override
    {
        sh_writeString(str);
    }

    void Flush() override { }
};