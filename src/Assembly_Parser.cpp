#include "pch.h"
#include "Assembly_Parser.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace std;
using namespace boost::uuids;

using boost::property_tree::ptree;

namespace pooch::assembly
{
    Assembly_ParsedItem::Assembly_ParsedItem(uuid _id, uuid _parentId, string _filename, string _cswSlot)
        : id(_id), parentId(_parentId), filename(_filename)
    {
        this->cswSlot = _cswSlot.empty() ? 0 : stoi(_cswSlot);
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
            auto cswSlot = child.second.get<string>("csw_slot");

            result.push_back(shared_ptr<Assembly_ParsedItem>(new Assembly_ParsedItem(id, parentId, file, cswSlot)));
        }

        return result;
    }
}
