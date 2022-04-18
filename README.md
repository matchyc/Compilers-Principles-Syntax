# Compilers-Principles-Syntax

Syntactic analysis

This project finished a syntax analyzer in compiler using C++.

- OS：Windows 10 20H2
- Language：C++
- IDE： CLion 2020.3.3
  Build #CL-203.7717.62, built on March 16, 2021
  Runtime version: 11.0.10+8-b1145.96 amd64
- Compiler：GCC version 9.2.0 (MinGW.org GCC Build-2)
- Dependencies： Cmake、C++11

Construction

```
    syntactic_analysis
    ├─.idea
    │  └─inspectionProfiles
    ├─cmake-build-debug
    │  ├─CMakeFiles
    │  │  ├─3.17.5
    │  │  │  ├─CompilerIdC
    │  │  │  │  └─tmp
    │  │  │  └─CompilerIdCXX
    │  │  │      └─tmp
    │  │  ├─CMakeTmp
    │  │  └─syntactic_analysis.dir
    │  │      └─source
    │  └─Testing
    │      └─Temporary
    ├─header
    └─source
```

## Usage

### Input

Basically, it's a calculator for simple  arithmetic calculation. (+, -, *, /)

input.txt:

- line 1: number of productions `m`
- line 2 - line m + 1: `m` lines of productions
- line m + 2: non-terminals
- line m + 3: terminals
```
8
E->E+T
E->E-T
E->T
T->T*F
T->T/F
T->F
F->(E)
F->num
E T F $
num + - * / ( ) $
```

### Output

First Set, Follow Set, DFA, Analysis Table.

Such as:
```
First set:
+---+-----+
|   |FIRST|
+---+-----+
|E  |( n  |
+---+-----+
|T  |( n  |
+---+-----+
|F  |( n  |
+---+-----+
```

```
DFA
Item set 29
T->T/F· )
T->T/F· +
T->T/F· -
T->T/F· *
T->T/F· /

Total item set: 30
```

Table

```
state|                          action                        |         goto
     |num    +      -      *      /      (      )      $      |    E      T      F
----------------------------------------------------------------------------------------------------
0    |S2                                 S1                   |   3       5       4
1    |S7                                 S6                   |   8       10      9
2    |       R8     R8     R8     R8                   R8     |   0       0       0
3    |       S11    S12                                ACC    |   0       0       0
4    |       R6     R6     R6     R6                   R6     |   0       0       0
5    |       R3     R3     S13    S14                  R3     |   0       0       0
6    |S7                                 S6                   |   15      10      9
7    |       R8     R8     R8     R8            R8            |   0       0       0
8    |       S17    S18                         S16           |   0       0       0
9    |       R6     R6     R6     R6            R6            |   0       0       0
10   |       R3     R3     S19    S20           R3            |   0       0       0
11   |S2                                 S1                   |   0       21      4
12   |S2                                 S1                   |   0       22      4
13   |S2                                 S1                   |   0       0       23
14   |S2                                 S1                   |   0       0       24
15   |       S17    S18                         S25           |   0       0       0
16   |       R7     R7     R7     R7                   R7     |   0       0       0
17   |S7                                 S6                   |   0       26      9
18   |S7                                 S6                   |   0       27      9
19   |S7                                 S6                   |   0       0       28
20   |S7                                 S6                   |   0       0       29
21   |       R1     R1     S13    S14                  R1     |   0       0       0
22   |       R2     R2     S13    S14                  R2     |   0       0       0
23   |       R4     R4     R4     R4                   R4     |   0       0       0
24   |       R5     R5     R5     R5                   R5     |   0       0       0
25   |       R7     R7     R7     R7            R7            |   0       0       0
26   |       R1     R1     S19    S20           R1            |   0       0       0
27   |       R2     R2     S19    S20           R2            |   0       0       0
28   |       R4     R4     R4     R4            R4            |   0       0       0
29   |       R5     R5     R5     R5            R5            |   0       0       0
```

### Enter the expression

```
28   |       R4     R4     R4     R4            R4            |   0       0       0
29   |       R5     R5     R5     R5            R5            |   0       0       0
Please enter a math expression:
```

### analyze
```
Please enter a math expression:
1+1
+----+------------+-----------+-----+----------------+
|step|symbol_stack|state_stack|input|action          |
+----+------------+-----------+-----+----------------+
|0   |n           |0 2        |+1$  |shift 2         |
+----+------------+-----------+-----+----------------+
|1   |F           |0 4        |+1$  |reduce by F->n  |
+----+------------+-----------+-----+----------------+
|2   |T           |0 5        |+1$  |reduce by T->F  |
+----+------------+-----------+-----+----------------+
|3   |E           |0 3        |+1$  |reduce by E->T  |
+----+------------+-----------+-----+----------------+
|4   |E+          |0 3 11     |1$   |shift 11        |
+----+------------+-----------+-----+----------------+
|5   |E+n         |0 3 11 2   |$    |shift 2         |
+----+------------+-----------+-----+----------------+
|6   |E+F         |0 3 11 4   |$    |reduce by F->n  |
+----+------------+-----------+-----+----------------+
|7   |E+T         |0 3 11 21  |$    |reduce by T->F  |
+----+------------+-----------+-----+----------------+
|8   |E           |0 3        |$    |reduce by E->E+T|
+----+------------+-----------+-----+----------------+
Accept!

```

## Error Handling

This program could handle basic syntax error in arithmetic expression.

```c
Please enter a math expression:
1^2
+----+------------+-----------+-----+-------+
|step|symbol_stack|state_stack|input|action |
+----+------------+-----------+-----+-------+
|0   |n           |0 2        |^2$  |shift 2|
+----+------------+-----------+-----+-------+
error
Error Location:^2

Process finished with exit code -2
```