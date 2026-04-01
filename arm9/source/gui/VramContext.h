#pragma once
#include "IVramManager.h"

class VramContext
{
public:
    VramContext(IVramManager* bgVramManager, IVramManager* objVramManager,
        IVramManager* texVramManager, IVramManager* texPlttVramManager)
        : _bgVramManager(bgVramManager), _objVramManager(objVramManager)
        , _texVramManager(texVramManager), _texPlttVramManager(texPlttVramManager)
    { }

    IVramManager* GetBgVramManager() const { return _bgVramManager; }
    IVramManager* GetObjVramManager() const { return _objVramManager; }
    IVramManager* GetTexVramManager() const { return _texVramManager; }
    IVramManager* GetTexPlttVramManager() const { return _texPlttVramManager; }

private:
    IVramManager* _bgVramManager;
    IVramManager* _objVramManager;
    IVramManager* _texVramManager;
    IVramManager* _texPlttVramManager;
};
