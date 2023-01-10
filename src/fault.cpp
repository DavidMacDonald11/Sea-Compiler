#include "fault.h"
#include "util.h"

namespace Fault {
    static str act(Component& c, const str& message, const str& label) {
        c.mark();
        str newMessage = replaceStr(message, "\n", "\\n");
        return fmt::format("{}: {}\n{}", label, newMessage, c.raw());
    }

    vector<str> warnings;
    vector<str> errors;
    str failure;

    str toString() {
        str string;

        if(warnings.size() > 0) string.append(join(warnings, "\n"));
        if(errors.size() > 0) string.append(join(errors, "\n"));
        if(failure != "") string.append(failure + "\n");

        return string;
    }

    void warn(Component& c, const str& message) {
        warnings.push_back(act(c, message, "Warning"));
    }

    void error(Component& c, const str& message) {
        errors.push_back(act(c, message, "Error"));
    }

    CompilerFailure fail(Component& c, const str& message) {
        failure = act(c, message, "Failure");
        return CompilerFailure();
    }

    void check() {
        if(warnings.size() + errors.size() == 0) return;
        throw CompilerFailure();
    }

    void reset() {
        warnings.clear();
        errors.clear();
        failure.clear();
    }
}
