#pragma once
#include "Point.h"

/// @brief Class representing a 2d rectangle.
class Rectangle
{
    int _x;
    int _y;
    int _width;
    int _height;

public:
    constexpr Rectangle(const Point& point, int width, int height)
        : _x(point.x), _y(point.y), _width(width), _height(height) { }

    constexpr Rectangle(const Point& topLeft, const Point& bottomRight)
        : _x(topLeft.x), _y(topLeft.y)
        , _width(bottomRight.x - topLeft.x), _height(bottomRight.y - topLeft.y) { }

    constexpr Rectangle(int x, int y, int width, int height)
        : _x(x), _y(y), _width(width), _height(height) { }
    
    constexpr int GetX() const { return _x; }
    constexpr int GetY() const { return _y; }
    constexpr int GetWidth() const { return _width; }
    constexpr int GetHeight() const { return _height; }

    constexpr int GetLeft() const { return _x; }
    constexpr int GetRight() const { return _x + _width; }
    constexpr int GetTop() const { return _y; }
    constexpr int GetBottom() const { return _y + _height; }

    constexpr Point GetTopLeft() const { return Point(_x, _y); }
    constexpr Point GetBottomRight() const { return Point(_x + _width, _y + _height); }

    constexpr Point GetTopCenter() const { return Point(_x + (_width >> 1), _y); }
    constexpr Point GetCenter() const { return Point(_x + (_width >> 1), _y + (_height >> 1)); }
    constexpr Point GetBottomCenter() const { return Point(_x + (_width >> 1), _y); }

    /// @brief Checks if the given rectangle overlaps with this rectangle.
    /// @param other The rectangle to check.
    /// @return True if the given rectangle overlaps with this rectangle,
    ///         or false otherwise.
    constexpr bool OverlapsWith(const Rectangle& other) const
    {
        return GetLeft() < other.GetRight() && other.GetLeft() < GetRight()
            && GetTop() < other.GetBottom() && other.GetTop() < GetBottom();
    }

    /// @brief Checks if the given point is contained in this rectangle.
    /// @param point The point to check.
    /// @return True if the given point is contained in this rectangle,
    ///         or false otherwise.
    constexpr bool Contains(const Point& point) const
    {
        return point.x >= GetLeft() && point.x < GetRight()
            && point.y >= GetTop() && point.y < GetBottom();
    }

    /// @brief Checks if the given rectangle is fully contained in this rectangle.
    /// @param other The rectangle to check.
    /// @return True if the given rectangle is fully contained in this rectangle,
    ///         or false otherwise.
    constexpr bool Contains(const Rectangle& other) const
    {
        return other.GetLeft() >= GetLeft() && other.GetRight() <= GetRight()
            && other.GetTop() >= GetTop() && other.GetBottom() <= GetBottom();
    }

    constexpr Rectangle OffsetBy(const Point& point) const
    {
        return Rectangle(_x + point.x, _y + point.y, _width, _height);
    }
};