#include "fault.h"
#include "util.h"

using CompilerFailure = Fault::CompilerFailure;

static str act(Component& c, const str& message, const str& label) {
    c.mark();
    str newMessage = replaceStr(message, "\n", "\\n");
    return fmt::format("{}: {}\n{}", label, newMessage, c.raw());
}


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

void Fault::reset() {
    warnings.clear();
    errors.clear();
    failure.clear();
}
