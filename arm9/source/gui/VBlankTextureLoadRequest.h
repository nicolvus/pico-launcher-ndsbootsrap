#pragma once
#include "core/LinkedListLink.h"
#include "VBlankTextureLoadRequestState.h"

class VBlankTextureLoadRequest
{
    friend class VBlankTextureLoader;

public:
    typedef void (*CallbackFunc)(const VBlankTextureLoadRequest* request, void* arg);

    VBlankTextureLoadRequest() { }

    VBlankTextureLoadRequest(const void* textureData, u32 textureDataLength, u32 textureVramOffset,
        const void* paletteData, u32 paletteDataLength, u32 paletteVramOffset,
        CallbackFunc callbackFunc, void* callbackArg)
        : _textureData(textureData), _textureDataLength(textureDataLength), _textureVramOffset(textureVramOffset)
        , _paletteData(paletteData), _paletteDataLength(paletteDataLength), _paletteVramOffset(paletteVramOffset)
        , _callbackFunc(callbackFunc), _callbackArg(callbackArg) { }

    VBlankTextureLoadRequestState GetState() const
    {
        return _state;
    }

private:
    VBlankTextureLoadRequestState _state = VBlankTextureLoadRequestState::NotLoaded;
    LinkedListLink _listLink;
    const void* _textureData = nullptr;
    u32 _textureDataLength = 0;
    u32 _textureVramOffset = 0;
    const void* _paletteData = nullptr;
    u32 _paletteDataLength = 0;
    u32 _paletteVramOffset = 0;
    CallbackFunc _callbackFunc = nullptr;
    void* _callbackArg = nullptr;
};
