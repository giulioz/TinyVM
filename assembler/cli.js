const fs = require("fs");
const assemble = require("./assembler");

console.log("CVM Bytecode assembler\n2019, Giulio Zausa\n");

console.log(assemble(fs.readFileSync(process.argv[2], "utf-8")));
