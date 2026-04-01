#include "common.h"
#include "gui/views/View.h"
#include "input/InputProvider.h"
#include "FocusManager.h"

void FocusManager::Focus(View* newFocus)
{
    if (!newFocus)
    {
        Unfocus();
        return;
    }

    if (_currentFocus)
        _currentFocus->SetFocused(false);
    newFocus->SetFocused(true);
    _currentFocus = newFocus;
}

void FocusManager::Unfocus()
{
    if (_currentFocus)
        _currentFocus->SetFocused(false);
    _currentFocus = nullptr;
}

void FocusManager::Update(const InputProvider& inputProvider)
{
    if (!_currentFocus || !_currentFocus->GetParent())
        return; // todo

    View* newFocus = nullptr;
    if (inputProvider.Triggered(InputKey::DpadUp))
        newFocus = _currentFocus->GetParent()->MoveFocus(_currentFocus, FocusMoveDirection::Up, _currentFocus);
    else if (inputProvider.Triggered(InputKey::DpadDown))
        newFocus = _currentFocus->GetParent()->MoveFocus(_currentFocus, FocusMoveDirection::Down, _currentFocus);
    else if (inputProvider.Triggered(InputKey::DpadLeft))
        newFocus = _currentFocus->GetParent()->MoveFocus(_currentFocus, FocusMoveDirection::Left, _currentFocus);
    else if (inputProvider.Triggered(InputKey::DpadRight))
        newFocus = _currentFocus->GetParent()->MoveFocus(_currentFocus, FocusMoveDirection::Right, _currentFocus);
    else
        _currentFocus->HandleInput(inputProvider, *this);

    if (newFocus)
        Focus(newFocus);
}

bool FocusManager::IsFocusInside(const View* view) const
{
    auto focusView = _currentFocus;
    while (focusView)
    {
        if (view == focusView)
            return true;
        focusView = focusView->GetParent();
    }
    return false;
}
