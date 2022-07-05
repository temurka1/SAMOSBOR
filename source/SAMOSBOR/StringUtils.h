#pragma once

namespace SAMOSBOR::core::utils
{
    /// <summary>
    /// Splits string
    /// </summary>
    std::vector<std::string_view> string_split(const std::string_view& str, const std::string_view& delims);

    /// <summary>
    /// Splits string into buffer
    /// </summary>
    void string_split_into(const std::string_view& str, const std::string_view& delims, std::vector<std::string_view>& buffer);

    /// <summary>
    /// Converts string to hex representation
    /// </summary>
    std::string string_to_hex(const std::string_view& input);

    /// <summary>
    /// Converts string with hex representation into normal string
    /// </summary>
    std::string hex_to_string(const std::string_view& input);
}