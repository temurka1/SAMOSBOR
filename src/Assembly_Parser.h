#pragma once

namespace pooch::assembly
{
	struct Assembly_ParsedItem
	{
		boost::uuids::uuid id;
		boost::uuids::uuid parentId;

		std::string filename;
		int cswSlot;

		Assembly_ParsedItem(boost::uuids::uuid id, boost::uuids::uuid parentId, std::string filename, std::string cswSlot);
		~Assembly_ParsedItem();
	};

	class Assembly_Parser
	{
	public:
		std::vector<std::shared_ptr<Assembly_ParsedItem>> Parse(std::string const& filename) const;
	};
}

