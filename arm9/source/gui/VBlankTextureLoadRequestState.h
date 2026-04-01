#pragma once

enum class VBlankTextureLoadRequestState
{
    NotLoaded,
    LoadRequested,
    Loading,
    LoadComplete,
    Canceled
};
