const fs = require("fs");
const nearley = require("nearley");

const grammar = require("./grammar");
const parser = new nearley.Parser(nearley.Grammar.fromCompiled(grammar));
parser.feed(fs.readFileSync("./test2.c", "utf-8"));

const nRegisters = 32;
function getFreeRegister(context) {
  for (let i = 0; i < nRegisters; i++) {
    if (!context.scope.find(v => v.register === i)) {
      return i;
    }
  }
}

function convertAST(node, context) {
  if (node.type === "function") {
    context.functions.push(node);
    console.log(`${node.ident}:`);

    const nContext = { ...context };
    nContext.scope = [...context.scope];
    node.params.forEach(v => {
      nContext.scope.push({ ...v, register: getFreeRegister(nContext) });
    });
    node.body.variables.forEach(v => {
      nContext.scope.push({ ...v, register: getFreeRegister(nContext) });
    });

    node.body.statements.forEach(s => convertAST(s, nContext));
  } else if (node.stype) {
    switch (node.stype) {
      case "assign":
        const register = convertAST()
        break;
    }
  }
}

const result = parser.results[0];
const context = {
  code: [],
  functions: [],
  scope: []
};
result.map(node => convertAST(node, context));
