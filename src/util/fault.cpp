#include "util/fault.h"
#include "lexer/token.h"

namespace Fault {
    static str act(Token& token, const str& message, const str& label) {
        token.mark();
        return fmt::format("{}: {}\n{}", label, message, token.line.raw());
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

    void warn(Token& token, const str& message) {
        warnings.push_back(act(token, message, "Warning"));
    }

    void error(Token& token, const str& message) {
        errors.push_back(act(token, message, "Error"));
    }

    CompilerFailure fail(Token& token, const str& message) {
        failure = act(token, message, "Failure");
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
