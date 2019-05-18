import React from "react";
import { render } from "react-dom";

import MonacoEditor from "react-monaco-editor";
import classnames from "classnames";
import CssBaseline from "@material-ui/core/CssBaseline";
import withStyles from "@material-ui/core/styles/withStyles";

import assemble from "./assembler";

const initial = `MOV R0 5
CALL fattoriale
DISPLAY R30
HALT

fattoriale:
MOV R1 2
MOV R20 1
MOV R30 1

inizio:
SUB R0 R1
JNEG fine
MUL R30 R1
POP R30
ADD R1 R20
POP R1
JMP inizio

fine:
RET`;

function Editor({ classes }) {
  const [code, setCode] = React.useState(initial);

  let assembled = "";
  try {
    assembled = assemble(code);
  } catch (e) {
    console.log(e);
  }

  return (
    <div className={classes.root}>
      <div className={classes.column}>
        <MonacoEditor theme="vs-dark" value={code} onChange={t => setCode(t)} />
      </div>
      <div className={classnames(classes.column, classes.code)}>
        <pre>{assembled}</pre>
      </div>
    </div>
  );
}
const StyledEditor = withStyles(theme => ({
  root: {
    display: "flex",
    width: "100%",
    height: "100%"
  },
  column: {
    flex: 1
  },
  code: {
    margin: theme.spacing.unit * 2,
    overflow: "auto"
  }
}))(Editor);

function App() {
  return (
    <>
      <CssBaseline />
      <StyledEditor />
    </>
  );
}

const element = document.getElementById("root");
render(<App />, element);
