@{%
    const merge = d => d.join('');
    const nth = n => d => d[n];
%}
@builtin "whitespace.ne"

file -> (function _):*                                       {% d => d[0].map(e => e[0]) %}

function -> type __ ident _ "(" (params | null) ")" _ "{" _ body "}"
      {% d => ({type: "function", rtype: d[0], ident: d[2], params: d[5][0], body: d[10]}) %}

type -> "void"                                               {% nth(0) %}
      | "int"                                                {% nth(0) %}
ident -> [a-z,A-Z] [a-z,A-Z,0-9]:*                           {% d => d[0] + merge(d[1]) %}
params -> type __ ident                                      {% d => [{type: "param", vtype: d[0], ident: d[2]}] %}
		| type __ ident _ "," _ params                       {% d => [{type: "param", vtype: d[0], ident: d[2]}, ...d[6]] %}
body -> null                                                 {% nth(0) %}
      | statements                                           {% d => ({statements: d[0]}) %}
      | vardecl _ statements                                 {% d => ({variables: d[0], statements: d[2]}) %}
vardecl -> type __ ident _ assign ";"                        {% d => [{vtype: d[0], ident: d[2], init: d[4]}] %}
         | type __ ident _ assign ";" _ vardecl              {% d => [{vtype: d[0], ident: d[2], init: d[4]}, ...d[7]] %}
assign -> null
        | "=" _ expr _                                       {% nth(2) %}
statements -> statement:*                                    {% nth(0) %}
statement -> ident _ assign ";" _                            {% d => ({stype: "assign", ident: d[0], value: d[2]}) %}
           | expr ";" _                                      {% d => ({stype: "expr", value: d[0]}) %}
           | "return" __ expr _ ";" _                        {% d => ({stype: "return", value: d[2]}) %}
           | "if" _ "(" _ condition _ ")" _ "{" _ body "}" _ else
		     {% d => ({stype: "if", condition: d[4], body: d[10], elseBody: d[13]}) %}
           | "while" _ "(" _ condition _ ")" _ "{" _ body "}" _
		     {% d => ({stype: "while", condition: d[4], body: d[10]}) %}
           | "for" _ "(" _ forInit _ ";" _ condition _ ";" _ forInit ")" _ "{" _ body "}" _
		     {% d => ({stype: "for", init: d[4], condition: d[8], increment: d[12], body: d[17], elseBody: d[13]}) %}
			 
condition -> expr _ cmp _ expr                               {% d => ({left: d[0], right: d[4], cmp: d[2]}) %}
forInit -> null                                              {% nth(0) %}
         | ident _ assign                                    {% d => ({ident: d[0], value: d[2]}) %}
else -> null
      | "else" _ "{" _ body "}" _                            {% nth(4) %}

expr -> expr2                                                {% nth(0) %}
	  | expr2 _ op _ expr                                    {% d => ({etype: "op", op: d[2], left: d[0], right: d[4]}) %}
	  
expr2 -> value                                               {% d => ({etype: "value", value: d[0]}) %}
       | ident                                               {% d => ({etype: "ident", value: d[0]}) %}
       | ident _ "(" _ (fparams | null) ")"                  {% d => ({etype: "call", name: d[0], params: d[4][0]}) %}
	  
op -> "+"                              {% nth(0) %}
    | "-"                              {% nth(0) %}
	| "*"                              {% nth(0) %}
	| "/"                              {% nth(0) %}
cmp -> "=="                            {% nth(0) %}
     | ">"                             {% nth(0) %}
	 | "<"                             {% nth(0) %}
	 | "<="                            {% nth(0) %}
	 | ">="                            {% nth(0) %}

fparams -> expr                                              {% d => [d[0]] %}
		 | expr _ "," _ fparams                              {% d => [d[0], ...d[4]] %}

value -> int                                                 {% d => ({vtype: "int", value: parseFloat(d[0])}) %}

int -> digit                           {% id %}
     | digit19 digits                  {% merge %}
     | "-" digit                       {% merge %}
     | "-" digit19 digits              {% merge %}
digits -> digit                        {% id %}
        | digit digits                 {% merge %}
digit -> [0-9]                         {% id %}
digit19 -> [1-9]                       {% id %}
