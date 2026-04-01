#pragma once

#include "InputKey.h"

class InputProvider
{
public:
    /// @brief Returns a bitmask of the keys currently being held.
    /// @return A bitmask of the keys currently being held.
    InputKey GetCurrentKeys() const { return _currentKeys; }

    /// @brief Returns whether any of the keys in the given \p mask is currently being held.
    /// @param mask The mask to check for.
    /// @return \c true if any of the keys in the \p mask is being held, or \c false otherwise.
    bool Current(InputKey mask) const { return static_cast<bool>(_currentKeys & mask); }

    /// @brief Returns a bitmask of the keys that went from unpressed to pressed in the latest update.
    /// @return A bitmask of the keys that went from unpressed to pressed in the latest update.
    InputKey GetTriggeredKeys() const
    {
        return _triggeredKeys;
    }

    /// @brief Returns whether any of the keys in the given \p mask went from unpressed to pressed in the latest update.
    /// @param mask The mask to check for.
    /// @return \c true if any of the keys in the \p mask went from unpressed to pressed
    ///         in the latest update, or \c false otherwise.
    bool Triggered(InputKey mask) const { return static_cast<bool>(_triggeredKeys & mask); }

    /// @brief Returns a bitmask of the keys that went from pressed to unpressed in the latest update.
    /// @return A bitmask of the keys that went from pressed to unpressed in the latest update.
    InputKey GetReleasedKeys() const { return _releasedKeys; }

    /// @brief Returns whether any of the keys in the given \p mask went from pressed to unpressed in the latest update.
    /// @param mask The mask to check for.
    /// @return \c true if any of the keys in the \p mask went from pressed to unpressed
    ///         in the latest update, or \c false otherwise.
    bool Released(InputKey mask) const { return static_cast<bool>(_releasedKeys & mask); }

    /// @brief Updates the input provider.
    virtual void Update() = 0;

    /// @brief Resets the input provider.
    virtual void Reset()
    {
        _currentKeys = InputKey::None;
        _triggeredKeys = InputKey::None;
        _releasedKeys = InputKey::None;
    }

protected:
    InputKey _currentKeys;
    InputKey _triggeredKeys;
    InputKey _releasedKeys;

    InputProvider()
        : _currentKeys(InputKey::None), _triggeredKeys(InputKey::None), _releasedKeys(InputKey::None) { }
};
