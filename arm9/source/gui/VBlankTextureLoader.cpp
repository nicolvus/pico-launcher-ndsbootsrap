#include "common.h"
#include <string.h>
#include <libtwl/mem/memVram.h>
#include <libtwl/gfx/gfxStatus.h>
#include <libtwl/dma/dmaNitro.h>
#include "VBlankTextureLoader.h"

#define TEXLOADER_BYTES_PER_LINE        2048
#define TEXLOADER_G3_VBLANK_START_LINE  191
#define TEXLOADER_G3_VBLANK_END_LINE    213

void VBlankTextureLoader::VBlank()
{
    rtos_lockMutex(&_mutex);
    {
        while (true)
        {
            auto request = _requests.GetHead();
            if (!request)
            {
                _currentRequest = nullptr;
                break;
            }
            if (request->GetState() == VBlankTextureLoadRequestState::Canceled)
            {
                _currentRequest = nullptr;
                continue;
            }

            if (_currentRequest != request)
            {
                _currentRequest = request;
                _currentLoadingOffset = 0;
                _currentLoadingStage = LoadingStage::Texture;
            }

            if (_currentLoadingStage == LoadingStage::Texture)
            {
                if (!LoadTexture())
                {
                    break;
                }
            }

            if (_currentRequest->GetState() == VBlankTextureLoadRequestState::Canceled)
            {
                _currentRequest = nullptr;
                continue;
            }

            if (_currentLoadingStage == LoadingStage::Palette)
            {
                if (!LoadPalette())
                {
                    break;
                }
            }

            _currentRequest->_state = VBlankTextureLoadRequestState::LoadComplete;
            if (_currentRequest->_callbackFunc)
            {
                _currentRequest->_callbackFunc(_currentRequest, _currentRequest->_callbackArg);
            }
            _requests.Remove(_currentRequest);
            _currentRequest = nullptr;
        }
    }
    rtos_unlockMutex(&_mutex);
}

bool VBlankTextureLoader::LoadTexture()
{
    int size = _currentRequest->_textureDataLength - _currentLoadingOffset;
    if (size > 0)
    {
        int line = gfx_getVCount();
        if (line >= TEXLOADER_G3_VBLANK_END_LINE)
        {
            return false;
        }

        int maxLength = (TEXLOADER_G3_VBLANK_END_LINE - line) * TEXLOADER_BYTES_PER_LINE;
        auto vramDMapping = mem_getVramDMapping();
        mem_setVramDMapping(MEM_VRAM_D_LCDC);
        if (size <= maxLength)
        {
            dma_ntrCopy32(3,
                (u8*)_currentRequest->_textureData + _currentLoadingOffset,
                (u8*)0x6860000 + _currentRequest->_textureVramOffset + _currentLoadingOffset,
                size);
            mem_setVramDMapping(vramDMapping);
        }
        else
        {
            dma_ntrCopy32(3,
                (u8*)_currentRequest->_textureData + _currentLoadingOffset,
                (u8*)0x6860000 + _currentRequest->_textureVramOffset + _currentLoadingOffset,
                maxLength);
            mem_setVramDMapping(vramDMapping);
            _currentLoadingOffset += maxLength;
            return false;
        }
    }

    _currentLoadingOffset = 0;
    _currentLoadingStage = LoadingStage::Palette;
    return true;
}

bool VBlankTextureLoader::LoadPalette()
{
    int size = _currentRequest->_paletteDataLength - _currentLoadingOffset;
    if (size > 0)
    {
        int line = gfx_getVCount();
        if (line >= TEXLOADER_G3_VBLANK_END_LINE)
        {
            return false;
        }

        int maxLength = (TEXLOADER_G3_VBLANK_END_LINE - line) * TEXLOADER_BYTES_PER_LINE;
        auto vramEMapping = mem_getVramEMapping();
        mem_setVramEMapping(MEM_VRAM_E_LCDC);
        if (size <= maxLength)
        {
            dma_ntrCopy32(3,
                (u8*)_currentRequest->_paletteData + _currentLoadingOffset,
                (u8*)0x6880000 + _currentRequest->_paletteVramOffset + _currentLoadingOffset,
                size);
            mem_setVramEMapping(vramEMapping);
        }
        else
        {
            dma_ntrCopy32(3,
                (u8*)_currentRequest->_paletteData + _currentLoadingOffset,
                (u8*)0x6880000 + _currentRequest->_paletteVramOffset + _currentLoadingOffset,
                maxLength);
            mem_setVramEMapping(vramEMapping);
            _currentLoadingOffset += maxLength;
            return false;
        }
    }

    return true;
}

void VBlankTextureLoader::RequestLoad(VBlankTextureLoadRequest& request)
{
    rtos_lockMutex(&_mutex);
    {
        if (request._state == VBlankTextureLoadRequestState::NotLoaded)
        {
            _requests.InsertTail(&request);
            request._state = VBlankTextureLoadRequestState::LoadRequested;
        }
    }
    rtos_unlockMutex(&_mutex);
}

void VBlankTextureLoader::CancelLoad(VBlankTextureLoadRequest& request)
{
    rtos_lockMutex(&_mutex);
    {
        if (request._state == VBlankTextureLoadRequestState::LoadRequested || request._state == VBlankTextureLoadRequestState::Loading)
        {
            _requests.Remove(&request);
            request._state = VBlankTextureLoadRequestState::Canceled;
            if (_currentRequest == &request)
            {
                _currentRequest = nullptr;
                _currentLoadingOffset = 0;
                _currentLoadingStage = LoadingStage::Texture;
            }
        }
    }
    rtos_unlockMutex(&_mutex);
}
