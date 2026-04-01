#pragma once

/// @brief Interface for a color palette.
class IPalette
{
public:
    virtual ~IPalette() = 0;

    /// @brief Gets the colors of the palette.
    /// @param dst A pointer to an array the colors should be written to.
    virtual void GetColors(u16* dst) const = 0;

    /// @brief Gets a hash that represents this color palette.
    /// @return The hash.
    virtual u32 GetHashCode() const = 0;
};

inline IPalette::~IPalette() { }
