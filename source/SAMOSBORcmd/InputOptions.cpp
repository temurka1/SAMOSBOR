#include "pch.h"
#include "InputOptions.h"
#include <boost/program_options.hpp>

using namespace std;
using namespace boost::program_options;

InputOptions::InputOptions(int argc, char* argv[])
{
    options_description desc("allowed options");

    desc.add_options()
        ("help", "produce help message")
        ("output, o", value<string>(), "output assembly STP file")
        ("length, l", value<float>()->default_value(0.0f), "extension length")
        ("structure, s", value<string>(), "assembly structure file");

    _vm = new variables_map();

    store(command_line_parser(argc, argv).options(desc).allow_unregistered().run(), *_vm);
    notify(*_vm);

    if (_vm->count("help"))
    {
        cout << desc << "\n";
    }
}

InputOptions::~InputOptions()
{
    delete _vm;
}

float InputOptions::GetExtensionLength() const
{
    return _vm->at("length").as<float>();
}

string InputOptions::GetAssemblyOutputFile() const
{
    if (_vm->count("output"))
    {
        return _vm->at("output").as<string>();
    }

    throw runtime_error("No output file specified");
}

string InputOptions::GetAssemblyStructureFile() const
{
    if (_vm->count("structure"))
    {
        return _vm->at("structure").as<string>();
    }

    throw runtime_error("No assembly structure file specified");
}