#include <algorithm>
#include <iterator>
#include <map>
#include "parser/node.h"
#include "lexer/source_line.h"
#include "util/component.h"

Parser* Node::parser = nullptr;

str Node::toString() const { return tree("    "); }

str Node::tree(str prefix) const { 
    str string = className(this);
    vector<Component*> nodes = self.nodes();

    for(nat i = 0; i < nodes.size(); i++) {
        bool atLast = (i == nodes.size() - 1);

        str symbol = atLast ? "└──" : "├──";
        str newPrefix = fmt::format("{}{}    ", prefix, atLast ? "" : "│");

        string.append(fmt::format("\n{}{} {}", prefix, symbol, nodes[i]->toString()));
    }

    return string;
}

vector<SourceLine*> Node::lines() const {
    std::map<nat, SourceLine*> lineMap;
    vector<SourceLine*> lines;

    for(Component* c : nodes()) {
        for(SourceLine* line : c->lines()) {
            lineMap[line->num] = line;
        }
    }

    lines.reserve(lineMap.size());

    for(std::pair<nat, SourceLine*> pair : lineMap) {
        lines.push_back(pair.second);
    }

    return lines;
}

str Node::raw() const { 
    str string;
    for(const SourceLine* line : lines()) string.append(line->raw());

    return string; 
}

void Node::mark() { 
    for(Component* c : nodes()) c->mark();
}


vector<Component*> PrimaryNode::nodes() const { return {&token};}

str PrimaryNode::tree(str) const {
    return fmt::format("{} ── {}", className(this), token.toString());
}

PrimaryNode::PrimaryNode(Token& token)
: token(token) {}
