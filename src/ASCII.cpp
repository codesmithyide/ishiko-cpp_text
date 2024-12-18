// SPDX-FileCopyrightText: 2000-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#include "ASCII.hpp"
#include "CString.hpp"
#include "TextErrorCategory.hpp"
#include <Ishiko/BasePlatform.hpp>

using namespace Ishiko;

#if ISHIKO_OS == ISHIKO_OS_LINUX
const char* ASCII::LineEnding = "\n";
#elif ISHIKO_OS == ISHIKO_OS_WINDOWS
const char* ASCII::LineEnding = "\r\n";
#else
#error Unsupported or unrecognized OS
#endif

bool ASCII::IsAlpha(char c) noexcept
{
    return ((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z'));
}

bool ASCII::IsNumeric(char c) noexcept
{
    return ((c >= '0') && (c <= '9'));
}

bool ASCII::IsAlphanumeric(char c) noexcept
{
    return (IsAlpha(c) || IsNumeric(c));
}

bool ASCII::IsHexDigit(char c) noexcept
{
    return (IsNumeric(c) || ((c >= 'a') && (c <= 'f')) || ((c >= 'A') && (c <= 'F')));
}

bool ASCII::IsWhitespace(char c) noexcept
{
    return ((c == ' ') || (c == '\r') || (c == '\n') || (c == '\t') || (c == '\v'));
}

std::vector<std::string> ASCII::GetLines(const std::string& str)
{
    std::vector<std::string> result;

    std::string::const_iterator it = str.begin();
    std::string::const_iterator end = str.end();
    std::string::const_iterator previousIt = it;
    while (it != end)
    {
        if ((*it != '\r') && (*it != '\n'))
        {
            ++it;
        }
        else
        {
            result.emplace_back(previousIt, it);
            if (*it == '\r')
            {
                ++it;
                if ((it != end) && (*it == '\n'))
                {
                    ++it;
                }
            }
            else
            {
                // TODO: do I need a variant where I keep the separator?
                //result.emplace_back(1, *it);
                ++it;
            }
            previousIt = it;
        }
    }
    if (previousIt != it)
    {
        result.emplace_back(previousIt, it);
    }

    return result;
}

std::vector<std::string> ASCII::Split(const std::string& str, char separator, bool conflateAdjacentSeparators)
{
    std::vector<std::string> result;

    std::string::const_iterator it = str.begin();
    std::string::const_iterator end = str.end();
    std::string::const_iterator previousIt = it;
    while (it != end)
    {
        if (*it != separator)
        {
            ++it;
        }
        else
        {
            if (!conflateAdjacentSeparators || (previousIt != it))
            {
                result.emplace_back(previousIt, it);
            }
            // TODO: do I need a variant where I keep the separator?
            //result.emplace_back(1, *it);
            ++it;
            previousIt = it;
        }
    }
    if (!conflateAdjacentSeparators || (previousIt != it))
    {
        result.emplace_back(previousIt, it);
    }
    
    return result;
}

void ASCII::ToLowerCase(std::string& str)
{
    for (char& c : str)
    {
        if ((c >= 'A') && (c <= 'Z'))
        {
            c = 'a' + c - 'A';
        }
    }
}

void ASCII::ToUpperCase(std::string& str)
{
    for (char& c : str)
    {
        if ((c >= 'a') && (c <= 'z'))
        {
            c = 'A' + c - 'a';
        }
    }
}

void ASCII::Trim(std::string& str)
{
    if (!str.empty())
    {
        std::string::const_iterator begin = str.begin();
        std::string::const_iterator it = str.begin();
        std::string::const_iterator end = str.end();
        while ((it != end) && (*it == ' '))
        {
            ++it;
        }
        std::string::const_iterator startIt = it;
        it = end;
        while ((it != startIt) && (*(it - 1) == ' '))
        {
            --it;
        }
        str.erase(it, end);
        str.erase(begin, startIt);
    }
}

bool ASCII::RemovePrefix(const std::string& prefix, std::string& str) noexcept
{
    if (CString::StartsWith(str.c_str(), prefix.c_str()))
    {
        str.erase(0, prefix.length());
        return true;
    }
    else
    {
        return false;
    }
}

bool ASCII::RemoveSuffix(const std::string& suffix, std::string& str) noexcept
{
    // TODO: we know the string length so we can do better than use EndsWidth
    if (CString::EndsWith(str.c_str(), suffix.c_str()))
    {
        str.erase(str.size() - suffix.size());
        return true;
    }
    else
    {
        return false;
    }
}

std::string ASCII::ToHexString(Byte byte)
{
    std::string result;
    result.resize(2);
    char c = ((byte >> 4) & 0x0F);
    result[0] = ((c < 10) ? (c + '0') : (c + 'a' - 10));
    c = (byte & 0x0F);
    result[1] = ((c < 10) ? (c + '0') : (c + 'a' - 10));
    return result;
}

std::string ASCII::ToHexString(uint16_t number)
{
    std::string result;
    result.resize(4);
    char c = ((number >> 12) & 0x000F);
    result[0] = ((c < 10) ? (c + '0') : (c + 'a' - 10));
    c = ((number >> 8) & 0x000F);
    result[1] = ((c < 10) ? (c + '0') : (c + 'a' - 10));
    c = ((number >> 4) & 0x000F);
    result[2] = ((c < 10) ? (c + '0') : (c + 'a' - 10));
    c = (number & 0x000F);
    result[3] = ((c < 10) ? (c + '0') : (c + 'a' - 10));
    return result;
}

std::string ASCII::ToHexString(const Byte* bytes, size_t count)
{
    std::string result;
    result.resize(2 * count);
    const Byte* end = (bytes + count);
    size_t i = 0;
    while (bytes != end)
    {
        char c = ((*bytes >> 4) & 0x0F);
        result[i++] = ((c < 10) ? (c + '0') : (c + 'a' - 10));
        c = (*bytes & 0x0F);
        result[i++] = ((c < 10) ? (c + '0') : (c + 'a' - 10));
        ++bytes;
    }
    return result;
}

void ASCII::Convert(const std::string& str, int8_t& number, Error& error) noexcept
{
    Convert(str.begin(), str.end(), number, error);
}

void ASCII::Convert(std::string::const_iterator begin, std::string::const_iterator end, int8_t& number,
    Error& error) noexcept
{
    if (begin == end)
    {
        // TODO: better error
        Fail(TextErrorCategory::Value::generic, error);
        return;
    }

    bool negative = false;
    if (*begin == '+')
    {
        ++begin;
        if (begin == end)
        {
            // TODO: better error
            Fail(TextErrorCategory::Value::generic, error);
            return;
        }
    }
    else if (*begin == '-')
    {
        negative = true;
        ++begin;
        if (begin == end)
        {
            // TODO: better error
            Fail(TextErrorCategory::Value::generic, error);
            return;
        }
    }
    int result = 0;
    while ((begin != end) && (result < 128))
    {
        char c = *begin;
        if (!IsNumeric(c))
        {
            // TODO: better error
            Fail(TextErrorCategory::Value::generic, error);
            return;
        }
        result = ((10 * result) + (c - '0'));
        ++begin;
    }
    if (negative)
    {
        result = -result;
    }
    if ((result <= -129) || (result >= 128))
    {
        // TODO: better error
        Fail(TextErrorCategory::Value::generic, error);
        return;
    }
    number = static_cast<int8_t>(result);
}

void ASCII::Convert(const std::string& str, uint8_t& number, Error& error) noexcept
{
    Convert(str.begin(), str.end(), number, error);
}

void ASCII::Convert(std::string::const_iterator begin, std::string::const_iterator end, uint8_t& number,
    Error& error) noexcept
{
    if (begin == end)
    {
        // TODO: better error
        Fail(TextErrorCategory::Value::generic, error);
        return;
    }

    if (*begin == '+')
    {
        ++begin;
        if (begin == end)
        {
            // TODO: better error
            Fail(TextErrorCategory::Value::generic, error);
            return;
        }
    }
    else if (*begin == '-')
    {
        // TODO: better error
        Fail(TextErrorCategory::Value::generic, error);
        return;
    }
    unsigned int result = 0;
    while ((begin != end) && (result < 256))
    {
        char c = *begin;
        if (!IsNumeric(c))
        {
            // TODO: better error
            Fail(TextErrorCategory::Value::generic, error);
            return;
        }
        result = ((10 * result) + (c - '0'));
        ++begin;
    }
    if (result >= 256)
    {
        // TODO: better error
        Fail(TextErrorCategory::Value::generic, error);
        return;
    }
    number = static_cast<uint8_t>(result);
}

void ASCII::Convert(const std::string& str, int16_t& number, Error& error) noexcept
{
    Convert(str.begin(), str.end(), number, error);
}

void ASCII::Convert(std::string::const_iterator begin, std::string::const_iterator end, int16_t& number,
    Error& error) noexcept
{
    if (begin == end)
    {
        // TODO: better error
        Fail(TextErrorCategory::Value::generic, error);
        return;
    }

    bool negative = false;
    if (*begin == '+')
    {
        ++begin;
        if (begin == end)
        {
            // TODO: better error
            Fail(TextErrorCategory::Value::generic, error);
            return;
        }
    }
    else if (*begin == '-')
    {
        negative = true;
        ++begin;
        if (begin == end)
        {
            // TODO: better error
            Fail(TextErrorCategory::Value::generic, error);
            return;
        }
    }
    int result = 0;
    while ((begin != end) && (result < 32768))
    {
        char c = *begin;
        if (!IsNumeric(c))
        {
            // TODO: better error
            Fail(TextErrorCategory::Value::generic, error);
            return;
        }
        result = ((10 * result) + (c - '0'));
        ++begin;
    }
    if (negative)
    {
        result = -result;
    }
    if ((result <= -32769) || (result >= 32768))
    {
        // TODO: better error
        Fail(TextErrorCategory::Value::generic, error);
        return;
    }
    number = static_cast<int16_t>(result);
}

void ASCII::Convert(const std::string& str, std::ios_base::fmtflags base, uint16_t& number, Error& error) noexcept
{
    Convert(str.begin(), str.end(), base, number, error);
}

void ASCII::Convert(std::string::const_iterator begin, std::string::const_iterator end, std::ios_base::fmtflags base,
    uint16_t& number, Error& error) noexcept
{
    if (begin == end)
    {
        // TODO: better error
        Fail(TextErrorCategory::Value::generic, error);
        return;
    }

    if (*begin == '+')
    {
        ++begin;
        if (begin == end)
        {
            // TODO: better error
            Fail(TextErrorCategory::Value::generic, error);
            return;
        }
    }
    else if (*begin == '-')
    {
        // TODO: better error
        Fail(TextErrorCategory::Value::generic, error);
        return;
    }
    unsigned int result = 0;
    if (base == std::ios::dec)
    {
        while ((begin != end) && (result < 65536))
        {
            char c = *begin;
            if (!IsNumeric(c))
            {
                // TODO: better error
                Fail(TextErrorCategory::Value::generic, error);
                return;
            }
            result = ((10 * result) + (c - '0'));
            ++begin;
        }
    }
    else if (base == std::ios::hex)
    {
        while ((begin != end) && (result < 65536))
        {
            char c = *begin;
            if (IsNumeric(c))
            {
                result = ((16 * result) + (c - '0'));
            }
            else if ((c >= 'a') && (c <= 'f'))
            {
                result = ((16 * result) + (c - 'a' + 10));
            }
            else if ((c >= 'A') && (c <= 'F'))
            {
                result = ((16 * result) + (c - 'A' + 10));
            }
            else
            {
                // TODO: better error
                Fail(TextErrorCategory::Value::generic, error);
                return;
            }
            ++begin;
        }
    }
    if (result >= 65536)
    {
        // TODO: better error
        Fail(TextErrorCategory::Value::generic, error);
        return;
    }
    number = static_cast<uint16_t>(result);
}

// TODO: add tests
void ASCII::Convert(const std::string& str, uint64_t& number)
{
    Convert(str.begin(), str.end(), number);
}

// TODO: add tests
void ASCII::Convert(const std::string& str, uint64_t& number, Error& error) noexcept
{
    Convert(str.begin(), str.end(), number, error);
}

// TODO: add tests
void ASCII::Convert(std::string::const_iterator begin, std::string::const_iterator end, uint64_t& number)
{
    if (begin == end)
    {
        // TODO: better error
        Throw(TextErrorCategory::Value::generic, __FILE__, __LINE__);
        return;
    }

    if (*begin == '+')
    {
        ++begin;
        if (begin == end)
        {
            // TODO: better error
            Throw(TextErrorCategory::Value::generic, __FILE__, __LINE__);
            return;
        }
    }
    else if (*begin == '-')
    {
        // TODO: better error
        Throw(TextErrorCategory::Value::generic, __FILE__, __LINE__);
        return;
    }
    // TODO: handle overflow
    uint64_t result = 0;
    while (begin != end)
    {
        char c = *begin;
        if (!IsNumeric(c))
        {
            // TODO: better error
            Throw(TextErrorCategory::Value::generic, __FILE__, __LINE__);
            return;
        }
        result = ((10 * result) + (c - '0'));
        ++begin;
    }
    number = static_cast<uint64_t>(result);
}

// TODO: add tests
void ASCII::Convert(std::string::const_iterator begin, std::string::const_iterator end, uint64_t& number,
    Error& error) noexcept
{
    if (begin == end)
    {
        // TODO: better error
        Fail(TextErrorCategory::Value::generic, error);
        return;
    }

    if (*begin == '+')
    {
        ++begin;
        if (begin == end)
        {
            // TODO: better error
            Fail(TextErrorCategory::Value::generic, error);
            return;
        }
    }
    else if (*begin == '-')
    {
        // TODO: better error
        Fail(TextErrorCategory::Value::generic, error);
        return;
    }
    // TODO: handle overflow
    uint64_t result = 0;
    while (begin != end)
    {
        char c = *begin;
        if (!IsNumeric(c))
        {
            // TODO: better error
            Fail(TextErrorCategory::Value::generic, error);
            return;
        }
        result = ((10 * result) + (c - '0'));
        ++begin;
    }
    number = static_cast<uint64_t>(result);
}

void ASCII::Convert(boost::string_view::const_iterator begin, boost::string_view::const_iterator end, uint64_t& number)
{
    if (begin == end)
    {
        // TODO: better error
        Throw(TextErrorCategory::Value::generic, __FILE__, __LINE__);
        return;
    }

    if (*begin == '+')
    {
        ++begin;
        if (begin == end)
        {
            // TODO: better error
            Throw(TextErrorCategory::Value::generic, __FILE__, __LINE__);
            return;
        }
    }
    else if (*begin == '-')
    {
        // TODO: better error
        Throw(TextErrorCategory::Value::generic, __FILE__, __LINE__);
        return;
    }
    // TODO: handle overflow
    uint64_t result = 0;
    while (begin != end)
    {
        char c = *begin;
        if (!IsNumeric(c))
        {
            // TODO: better error
            Throw(TextErrorCategory::Value::generic, __FILE__, __LINE__);
            return;
        }
        result = ((10 * result) + (c - '0'));
        ++begin;
    }
    number = static_cast<uint64_t>(result);
}

void ASCII::Convert(boost::string_view::const_iterator begin, boost::string_view::const_iterator end, uint64_t& number,
    Error& error) noexcept
{
    if (begin == end)
    {
        // TODO: better error
        Fail(TextErrorCategory::Value::generic, error);
        return;
    }

    if (*begin == '+')
    {
        ++begin;
        if (begin == end)
        {
            // TODO: better error
            Fail(TextErrorCategory::Value::generic, error);
            return;
        }
    }
    else if (*begin == '-')
    {
        // TODO: better error
        Fail(TextErrorCategory::Value::generic, error);
        return;
    }
    // TODO: handle overflow
    uint64_t result = 0;
    while (begin != end)
    {
        char c = *begin;
        if (!IsNumeric(c))
        {
            // TODO: better error
            Fail(TextErrorCategory::Value::generic, error);
            return;
        }
        result = ((10 * result) + (c - '0'));
        ++begin;
    }
    number = static_cast<uint64_t>(result);
}
