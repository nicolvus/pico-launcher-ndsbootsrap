#pragma once
#include <libtwl/rtos/rtosMutex.h>
#include "VBlankTextureLoadRequest.h"
#include "core/LinkedList.h"

/// @brief Class for loading texture data during vblank.
class VBlankTextureLoader
{
public:
    VBlankTextureLoader()
    {
        rtos_createMutex(&_mutex);
    }

    void VBlank();

    void RequestLoad(VBlankTextureLoadRequest& request);
    void CancelLoad(VBlankTextureLoadRequest& request);

private:
    enum class LoadingStage
    {
        Texture,
        Palette
    };

    rtos_mutex_t _mutex;
    LinkedList<VBlankTextureLoadRequest, &VBlankTextureLoadRequest::_listLink> _requests;
    VBlankTextureLoadRequest* _currentRequest = nullptr;
    u32 _currentLoadingOffset = 0;
    LoadingStage _currentLoadingStage;

    bool LoadTexture();
    bool LoadPalette();
};
