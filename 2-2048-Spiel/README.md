# 2048Clone (C/C++)

![Alt text](./example.jpg?raw=true "Example")

## Controls
```
ARROW BUTTONS will move the tiles according to the selected direction
ESC KEY       will exit the game and return to terminal
n   KEY       will restart the entire game
```
 

### Prerequisites

What things you need to compile the game

```
g++ Compiler
*IF* UNIT TESTs and MEMLEAK checking is wanted, then the following requirements also have to be met:
gTest version >= 1.8.0
VALGRIND
```

### Compiling and Running

A step by step series of examples that tell you how to get a development env running

Say what the step will be

```
make          <--> This will just compile the code
make all      <--> This will compile, stylecheck and UNIT TESTs the code
make test     <--> This will execute the UNIT TESTS
make valgrind	<--> This will check the code for memory leaks
```

After compiling with one of the options mentioned above, run the game with

```
./GameMain
```
