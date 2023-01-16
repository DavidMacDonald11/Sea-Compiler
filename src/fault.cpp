#include <map>
#include "fault.h"
#include "util.h"

using CompilerFailure = Fault::CompilerFailure;

static std::map<nat, str> stageMap {
    {0, "Lexing"},
    {1, "Substitution"},
    {2, "Parsing"},
    {3, "Publishing"},
    {4, "Transpiling"}
};


Fault::Fault()
: stage(0), warnings(), errors(), failure() {}

str Fault::toString() {
    str string;

    if(warnings.size() > 0) string.append(join(warnings, "\n"));
    if(errors.size() > 0) string.append(join(errors, "\n"));
    if(failure != "") string.append(failure + "\n");

    return string;
}

void Fault::warn(Component& c, const str& message) {
    warnings.push_back(act(c, message, "Warning"));
}

void Fault::error(Component& c, const str& message) {
    errors.push_back(act(c, message, "Error"));
}

CompilerFailure Fault::fail(Component& c, const str& message) {
    failure = act(c, message, "Failure");
    return CompilerFailure();
}

void Fault::check() {
    if(warnings.size() + errors.size() == 0) return;
    throw CompilerFailure();
}

str Fault::act(Component& c, const str& message, const str& label) {
    c.mark();

    str stageLabel = fmt::format("{} {}", stageMap[stage], label);
    str newMessage = replaceStr(message, "\n", "\\n");

    return fmt::format("{}: {}\n{}", stageLabel, newMessage, c.raw());
}
