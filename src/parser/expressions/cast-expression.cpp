#include "parser/declarations/type-name.h"
#include "parser/expressions/allocation-expression.h"
#include "parser/expressions/cast-expression.h"

vector<Component*> CastExpression::nodes() const {
    return {&expression, &type};
}

CastExpression::CastExpression(Node& expression, Node& type)
: expression(expression), type(type) {}

CastExpression::~CastExpression() {
    delete &expression;
    delete &type;
}

Node* CastExpression::construct() {
    Node* node = AllocationExpression::construct();

    while(parser->next().has({"as"})) {
        parser->take();
        bool hasParen = parser->next().has({"("});

        if(hasParen) parser->take();
        Node* type = TypeName::construct();
        node = new CastExpression(*node, *type);
        if(hasParen) parser->expectingHas({")"});
    }

    return node;
}

Transpiler::Line CastExpression::transpile() {
    Transpiler::Line line = type.transpile().add("(", ")");
    Transpiler::Line expression = self.expression.transpile().add("(", ")");
    return line.add("", expression.toString());
}

