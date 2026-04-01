#pragma once
#include "LinkedListLink.h"

/// @brief Class implementing a linked list with the link inside the list items.
/// @tparam T The type of list items.
/// @tparam member The link inside the items type.
template <typename T, LinkedListLink T::*member>
class LinkedList
{
public:
    /// @brief Linked list iterator.
    class Iterator
    {
        LinkedListLink* _itemLink;

    public:
        /// @brief Creates an iterator pointing to the given item.
        /// @param itemLink The link of the item this iterator points to.
        explicit Iterator(LinkedListLink* itemLink)
            : _itemLink(itemLink) { }

        /// @brief Checks whether this iterator is inequal to rhs.
        /// @param rhs The iterator to compare to.
        /// @return True if the iterators are inequal, or false otherwise.
        constexpr bool operator!=(Iterator rhs) const
        {
            return _itemLink != rhs._itemLink;
        }

        /// @brief Gets the list item this iterator points to.
        /// @return A reference to the list item.
        constexpr T& operator*() const
        {
            return *LinkToItem(_itemLink);
        }

        /// @brief Advances this iterator to the next list item.
        void operator++()
        {
            _itemLink = _itemLink->next;
        }
    };

    /// @brief Inserts the given item at the head of the list.
    /// @param item The item to insert.
    void InsertHead(T* item)
    {
        LinkedListLink* link = &(item->*member);
        link->next = _head;
        link->prev = nullptr;
        if (_head)
        {
            _head->prev = link;
        }
        _head = link;
        if (!_count++)
        {
            _tail = link;
        }
    }

    /// @brief Inserts the given item at the tail of the list.
    /// @param item The item to insert.
    void InsertTail(T* item)
    {
        LinkedListLink* link = &(item->*member);
        link->next = nullptr;
        link->prev = _tail;
        if (_tail)
        {
            _tail->next = link;
        }
        _tail = link;
        if (!_count++)
        {
            _head = link;
        }
    }

    /// @brief Inserts the given item after the other item.
    /// @param item The item to insert.
    /// @param after The item after which will be inserted.
    void InsertAfter(T* item, T* after)
    {
        LinkedListLink* itemLink = &(item->*member);
        LinkedListLink* afterLink = &(after->*member);
        itemLink->next = afterLink->next;
        itemLink->prev = afterLink;
        if (afterLink->next)
        {
            afterLink->next->prev = itemLink;
        }
        _count++;
    }

    /// @brief Inserts the given item before the other item.
    /// @param item The item to insert.
    /// @param after The item before which will be inserted.
    void InsertBefore(T* item, T* before)
    {
        LinkedListLink* itemLink = &(item->*member);
        LinkedListLink* beforeLink = &(before->*member);
        itemLink->next = beforeLink;
        itemLink->prev = beforeLink->prev;
        if (beforeLink->prev)
        {
            beforeLink->prev->next = itemLink;
        }
        _count++;
    }

    /// @brief Removes the given item from the list.
    /// @param item The item to remove.
    void Remove(T* item)
    {
        LinkedListLink* itemLink = &(item->*member);
        if (!itemLink->prev)
        {
            _head = itemLink->next;
        }
        else
        {
            itemLink->prev->next = itemLink->next;
        }

        if (!itemLink->next)
        {
            _tail = itemLink->prev;
        }
        else
        {
            itemLink->next->prev = itemLink->prev;
        }
        itemLink->prev = nullptr;
        itemLink->next = nullptr;
        _count--;
    }

    /// @brief Gets the head of the list.
    /// @return The head of the list, or null if the list is empty.
    constexpr T* GetHead() const { return _head ? LinkToItem(_head) : nullptr; }

    /// @brief Gets the tail of the list.
    /// @return The tail of the list, or null if the list is empty.
    constexpr T* GetTail() const { return _tail ? LinkToItem(_tail) : nullptr; }

    /// @brief Gets the item that comes after the given item.
    /// @param item The item to get the next for.
    /// @return The next item, or null if item is the last item in the list.
    constexpr T* GetNext(T* item) const
    {
        LinkedListLink* itemLink = &(item->*member);
        return itemLink->next ? LinkToItem(itemLink->next) : nullptr;
    }

    /// @brief Gets the item that comes before the given item.
    /// @param item The item to get the previous for.
    /// @return The previous item, or null if item is the first item in the list.
    constexpr T* GetPrevious(T* item) const
    {
        LinkedListLink* itemLink = &(item->*member);
        return itemLink->prev ? LinkToItem(itemLink->prev) : nullptr;
    }

    /// @brief Gets the amount of items in the list.
    /// @return The amount of items in the list.
    constexpr u32 GetCount() const { return _count; }

    /// @brief Gets an iterator to the start of the list.
    /// @return An iterator to start of the list.
    constexpr Iterator begin() const { return Iterator(_head); }

    /// @brief Gets an iterator to the end of the list.
    /// @return An iterator to end of the list.
    constexpr Iterator end() const { return Iterator(nullptr); }

private:
    LinkedListLink* _head = nullptr;
    LinkedListLink* _tail = nullptr;
    u32 _count = 0;

    /// @brief Converts a list link pointer to an item pointer.
    /// @param link The list link pointer.
    /// @return The item pointer.
    static constexpr T* LinkToItem(LinkedListLink* link)
    {
        return (T*)(((u8*)link)-((size_t)&(((T*)nullptr)->*member)));
    }
};