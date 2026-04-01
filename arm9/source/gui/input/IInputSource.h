#pragma once
#include "InputKey.h"

/// @brief Interface for a source of key input.
class IInputSource
{
public:
    virtual ~IInputSource() { }

    virtual InputKey Sample() const = 0;
};
