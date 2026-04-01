#pragma once
#include "RomBrowserState.h"
#include "RomBrowserStateTrigger.h"

class RomBrowserStateMachine
{
public:
    void Fire(RomBrowserStateTrigger trigger);

    void Update();

    constexpr RomBrowserState GetPreviousState() const { return _prevState; }
    constexpr RomBrowserStateTrigger GetLastTrigger() const { return _lastTrigger; }
    constexpr RomBrowserState GetCurrentState() const { return _curState; }

    constexpr bool HasStateChanged() const { return _stateChanged; }

private:
    RomBrowserStateTrigger _lastTrigger = RomBrowserStateTrigger::None;
    RomBrowserState _prevState = RomBrowserState::Start;
    RomBrowserState _curState = RomBrowserState::Start;
    RomBrowserStateTrigger _newTrigger = RomBrowserStateTrigger::None;
    bool _stateChanged = false;

    bool FireDirect(RomBrowserStateTrigger trigger);
};
