#pragma once
#include "unknownCover.h"
#include "StaticFileCover.h"

/// @brief Fallback question mark cover.
class UnknownFileCover : public StaticFileCover
{
public:
    UnknownFileCover()
        : StaticFileCover(unknownCoverBitmap, unknownCoverPal) { }
};
