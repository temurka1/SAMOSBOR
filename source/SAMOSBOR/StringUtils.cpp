#include "pch.h"
#include "StringUtils.h"

std::vector<std::string_view> SAMOSBOR::core::utils::str_split(const std::string_view str, const char delim, std::uint32_t maxTokens)
{
    std::vector<std::string_view> ret(maxTokens);

    std::string_view::size_type start = 0;
    size_t pos = str.find_first_of(delim, start);

    while (pos != std::string_view::npos)
    {
        if (pos != start)
        {
            ret.push_back(str.substr(start, pos - start));
        }

        start = pos + 1;
        pos = str.find_first_of(delim, start);
    }

    if (start < str.length())
    {
        ret.push_back(str.substr(start, str.length() - start));
    }

    ret.shrink_to_fit();

    return ret;
}

void SAMOSBOR::core::utils::str_split_inplace(const std::string_view str, const char delim, std::vector<std::string_view>* tokens)
{
    std::string_view::size_type start = 0;
    size_t pos = str.find_first_of(delim, start);

    while (pos != std::string_view::npos)
    {
        if (pos != start)
        {
            tokens->push_back(str.substr(start, pos - start));
        }

        start = pos + 1;
        pos = str.find_first_of(delim, start);
    }

    if (start < str.length())
    {
        tokens->push_back(str.substr(start, str.length() - start));
    }
}
