#pragma once

class StringUtil
{
public:
    /// @brief Copies the null terminated string in src to the
    ///        dst buffer of size dstSize. If dstSize > 0 the dst
    ///        buffer is guarenteed to be null terminated, even
    ///        if src is truncated to fit in dst.
    /// @param dst The destination buffer.
    /// @param src The source string (null terminated).
    /// @param dstLength The length of the destination buffer in characters.
    static u32 Copy(char* dst, const char* src, u32 dstLength);

    /// @brief Copies the null terminated string in src to the
    ///        dst buffer of size dstSize. If dstSize > 0 the dst
    ///        buffer is guarenteed to be null terminated, even
    ///        if src is truncated to fit in dst.
    /// @param dst The destination buffer.
    /// @param src The source string (null terminated).
    /// @param dstLength The length of the destination buffer in char16_t characters.
    static u32 Copy(char16_t* dst, const char16_t* src, u32 dstLength);

    /// @brief Copies the null terminated string in src to the
    ///        dst buffer of size dstSize. If dstSize > 0 the dst
    ///        buffer is guarenteed to be null terminated, even
    ///        if src is truncated to fit in dst.
    /// @param dst The destination buffer.
    /// @param src The source string (null terminated).
    /// @param dstLength The length of the destination buffer in char16_t characters.
    static u32 Copy(char16_t* dst, const char* src, u32 dstLength);
};
