const fs = require("fs");
const nearley = require("nearley");

const grammar = require("./grammar");
const parser = new nearley.Parser(nearley.Grammar.fromCompiled(grammar));
parser.feed(fs.readFileSync("./test.c", "utf-8"));

console.log(JSON.stringify(parser.results, null, 2));
