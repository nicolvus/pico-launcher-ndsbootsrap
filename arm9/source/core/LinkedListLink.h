#pragma once

/// @brief Link for a linked list.
struct LinkedListLink
{
    /// @brief Pointer to the next list link in the list.
    LinkedListLink* next = nullptr;

    /// @brief Pointer to the previous list link in the list.
    LinkedListLink* prev = nullptr;
};
