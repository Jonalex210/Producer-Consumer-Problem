# Producer-Consumer Problem
 Create a semaphore-based solution to the producer-consumer problem

# prod_cons_JonathanNjeunje
prod_cons_JonathanNjeunje is a C program designed to do an simulate the producer-consumer problem solution with semaphore and mutex lock to protect their critical sections.


## Installatoion
Use the commande below to compile the .c file.
```bash
gcc -o prod_cons_JonathanNjeunje prod_cons_JonathanNjeunje.c -lpthread -lrt
```


## Usage
Run the command below to execute the program. Note that the program offers the following tweaks to vary the simulation.

- A "DEBUG" (boolean) constant [defualt value: true] to activate/deactivate debug lines. This lines shows the entrance/exit of each producer and consummer to/from their respective critical sections.

- A "PROD_SPEED" (int) constant in microseconds [default value: 1s] to simulate a given production speed.

- A "CONS_SPEED" (int) constant in microseconds [default value: 4s] to simulate a given consumption speed.

Note: after changing any of these parameters in the source code (c file) make sure to recompile before rexecuting the program with the underneath command.

```bash
./prod_cons_JonathanNjeunje
```


## Contributing
Any Contributions are welcome.


##Authors and acknowledgment
Jonathan Njeunje


##Licence 
[MIT](https://choosealicense.com/license/mit/)

