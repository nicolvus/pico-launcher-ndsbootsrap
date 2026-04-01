#pragma once
#include "../StaticFileCover.h"
#include "folderCover.h"

/// @brief Generic cover for a folder.
class FolderFileCover : public StaticFileCover
{
public:
    FolderFileCover()
        : StaticFileCover(folderCoverBitmap, folderCoverPal) { }
};
