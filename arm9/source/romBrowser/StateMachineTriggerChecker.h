#pragma once

/// @brief Helper class for specifying state machine state transitions in an intuitive way.
/// @tparam TState The state enum type.
/// @tparam TTrigger The trigger enum type.
template <typename TState, typename TTrigger>
class StateMachineTriggerChecker
{
    const TState _fromState;
    const TTrigger _trigger;
    bool _isValid;
    TState _newState;

    TState _from;
    TTrigger _fromTrigger;
public:
    /// @brief Constructs a StateMachineTriggerChecker for a state machine
    ///        that is currently in state fromState and received the given trigger.
    /// @param fromState The current state of the state machine.
    /// @param trigger The received trigger.
    explicit constexpr StateMachineTriggerChecker(TState fromState, TTrigger trigger)
        : _fromState(fromState), _trigger(trigger), _isValid(false), _newState(fromState) { }

    /// @brief Specifies the start of a state transition. Multiple triggers can be
    ///        specified for a given from state.
    /// @param from The start state of the following trigger definitions.
    /// @return this
    constexpr StateMachineTriggerChecker& In(TState from)
    {
        if (!_isValid)
            _from = from;
        return *this;
    }

    /// @brief Specifies the trigger for a state transition. Should be used
    ///        after In has been used at least once.
    /// @param trigger The trigger of the state transition.
    /// @return this
    constexpr StateMachineTriggerChecker& Trigger(TTrigger trigger)
    {
        if (!_isValid)
            _fromTrigger = trigger;
        return *this;
    }

    /// @brief Specifies the end state for a state transition. Should be used
    ///        after every Trigger.
    /// @param to The end state of the state transition.
    /// @return this
    constexpr StateMachineTriggerChecker& GoesTo(TState to)
    {
        if (!_isValid && _fromState == _from && _trigger == _fromTrigger)
        {
            _isValid = true;
            _newState = to;
        }
        return *this;
    }

    /// @brief Ends the state transition definitions and returns
    ///        the check result.
    /// @param newState The new state for the state machine.
    /// @return true if the received trigger was valid for the
    ///         state of the state machine, or false otherwise.
    constexpr bool Check(TState& newState) const
    {
        newState = _newState;
        return _isValid;
    }
};
