#include "parser/declarations/declaration.h"
#include "parser/declarations/assert-declaration.h"

Node* Declaration::construct() {
    return AssertDeclaration::construct();
}
