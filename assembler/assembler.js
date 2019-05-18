const nearley = require("nearley");
const grammar = require("./grammar");

module.exports = function assemble(code) {
  const parser = new nearley.Parser(nearley.Grammar.fromCompiled(grammar));
  parser.feed(code);

  const opMap = {
    HALT: 0,
    DISPLAY: 1,
    PRINT_STACK: 2,
    PUSH: 10,
    POP: 11,
    MOV: 12,
    CALL: 20,
    RET: 21,
    JMP: 22,
    JZ: 23,
    JPOS: 24,
    JNEG: 25,
    ADD: 30,
    SUB: 31,
    MUL: 32,
    DIV: 33
  };

  const result = parser.results[0];
  const bytecode = fixLabels(
    result.reduce(opToCodes, {
      code: [],
      labels: [],
      index: 0
    })
  );

  function opToCodes(state, op) {
    if (op.op === "LABEL") {
      state.code.push(`; ${op.name}:`);
      state.labels.push({ name: op.name, index: state.index });
    } else {
      state.code.push(`${opMap[op.op]} ; [${state.index}] ${op.op}`);
      state.index++;

      switch (op.op) {
        case "DISPLAY":
        case "PUSH":
        case "POP":
          state.code.push(op.reg);
          state.index++;
          break;

        case "PRINT_STACK":
          state.code.push(op.value);
          state.index++;
          break;

        case "ADD":
        case "SUB":
        case "MUL":
        case "DIV":
          state.code.push(op.regA);
          state.code.push(op.regB);
          state.index++;
          state.index++;
          break;

        case "CALL":
        case "JMP":
        case "JZ":
        case "JPOS":
        case "JNEG":
          state.code.push({ type: "LABEL", label: op.label });
          state.index++;
          break;

        case "MOV":
          state.code.push(op.reg);
          state.code.push(op.value);
          state.index++;
          state.index++;
          break;
      }
    }

    return state;
  }

  function fixLabels(state) {
    return {
      ...state,
      code: state.code.map(op => {
        if (typeof op === "object") {
          return state.labels.find(l => l.name === op.label).index;
        } else {
          return op;
        }
      })
    };
  }

  return bytecode.code.join("\n");
};
