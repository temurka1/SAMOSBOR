#pragma once

namespace SAMOSBOR::core::utils
{
	std::vector<std::string_view> str_split(std::string_view str, const char delim, uint32_t maxTokens = 64);
	void str_split_inplace(std::string_view str, const char delim, std::vector<std::string_view>* tokens);
}