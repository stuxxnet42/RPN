# RPN
a functional reverse polish notation calculator

the modular design allows for simple implementation of new types

compile with -std=c++17

usage is:

echo "$SomeCalculation" | ./rpn {$mode}

where $mode is either 'i' for int, 'b' for bool or empty for float

example:
% echo "1 5 + 2.3 * cos" | ./rpn

0.330816



% echo "1 0 + 0 * not"  | ./rpn b

1

