#include "mapping.hpp"

Mappings::Mappings() :
    basics{
        { { ':' }, { Action::CMD } },
        { { 'h' }, { Action::LEFT } },
        { { 'j' }, { Action::DOWN } },
        { { 'k' }, { Action::UP } },
        { { 'l' }, { Action::RIGHT } },
        { { 'r' }, { Action::REPLACE } },
        { { '0' }, { Action::FIRSTCOL } },
        { { KEY_RESIZE }, { Action::RESIZE } },
    },
    compounds{
        { { 'g', '0' }, { Action::FIRSTCOL } },
    }
{}

Mappings::returnType Mappings::operator ()(int c)
{
    auto x = basics.find({ c });
    if (x != basics.end()) {
        return x->second;
    }
    return { Action::NONE };
}
