#include "pch.h"
#include "Assembly_Parser.h"

using namespace std;
using namespace boost::uuids;
using namespace SAMOSBOR::assembly;

using boost::property_tree::ptree;

Assembly_ParsedItem::Assembly_ParsedItem(uuid _id, uuid _parentId, string _filename, std::vector<std::string> _cswSlots)
    : id(_id), parentId(_parentId), filename(_filename), cswSlots(_cswSlots)
{
}

Assembly_ParsedItem::~Assembly_ParsedItem()
{
}

vector<shared_ptr<Assembly_ParsedItem>> Assembly_Parser::Parse(string const& filename) const
{
    ptree pt;
    read_json(filename, pt);

    vector<shared_ptr<Assembly_ParsedItem>> result;

    BOOST_FOREACH(const ptree::value_type & child, pt.get_child("data"))
    {
        auto id = boost::lexical_cast<uuid>(child.second.get<string>("id"));
        auto parentId = boost::lexical_cast<uuid>(child.second.get<string>("parent_id"));
        auto file = child.second.get<string>("file");

        vector<string> cswSlots;

        BOOST_FOREACH(const ptree::value_type & cswSlot, child.second.get_child("csw_slots"))
        {
            cswSlots.push_back(cswSlot.second.get_value<string>());
        }

        result.push_back(shared_ptr<Assembly_ParsedItem>(new Assembly_ParsedItem(id, parentId, file, cswSlots)));
    }

    return result;
}
