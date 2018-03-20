# expression-parser
Just playing with parsing and evaluating mathematical expressions in C

# Grammar

The language has a somewhat minimal and restrictive grammar, for now.

```
ws:
    0 or more whitespace characters (\r, \n, \t, space)

int:
    1 or more decimal digits (0-9)

operator:
    exactly 1 of either +, -, *, or /

expression:
    EITHER: ws int ws
    OR:     ws ( expression operator expression ) ws
```

In other words, only positive integers can be represented, and every operation
must be surrounded in parentheses. Arbitrary whitespace may seperate symbols.

An example expression: `(( 4 + 123 )*12345)`

# Future improvements:

- Allow for symbolic variables, and compile expressions into assembly/machine
code.

- Expand grammar to allow for negative numbers, make parentheses optional, etc.
