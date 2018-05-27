# The Threads Bar
The Threads Bar problem is a common exercise used in the study of operating systems which extends the producer-consumer
classical problem. The development of this application has as objective to explore the use of threads, their mechanisms
of synchronization and access control to critical regions.

## Problem Description
A bar has decided to release a specific number of free rounds for its *${N}* customers present at the establishment. This
bar has also *${X}* waiters working when the event stats. 

Each waiter can serve a limited number *${C}* of clients at each run. As the rounds are released, each waiter can only
go to the kitchen and prepare orders (which takes a random time) when its order capacity *${C}* is reached or when there is no more customers to be
served. 

As for the clients, each can make up to one order per round. If a client decides to order at a certain round, he has to
wait for a waiter to serve it before consuming the order (which also takes a random time).

A new round can only start when every customer that ordered where served.

## Proposed Solution
The project solves the threads bar concurrency problem by proposing the use of barriers. Barriers are used to
delimit arbitrary stages in each round and synchronize each agent. The defined stages are:

1. Ordering stage;
2. Serving stage;
3. Consuming stage;

In stage 1 each client decides whether or not to order at current round and perform the necessary actions. For stage 2 
the waiters start working by preparing each order and delivering it to each client. And finally, at round 3 the bar
waits for the orders to be consumed before starting a new round.

As the console screen is used to print agent messages and application related info it can also be considered as a
critical region. To make sure each message is printed properly this implementation presents an utility class to designed
to compose string messages before calling the standard output stream and avoid concurrency problems.

## File Tree
```bash
.
├── bin
├── include
│   ├── Bar.h
│   ├── Barrier.h
│   ├── Client.h
│   ├── Console.h
│   └── Waiter.h
├── obj
│   ├── Bar.d
│   ├── Client.d
│   ├── main.d
│   └── Waiter.d
├── src
│   ├── Bar.cpp
│   ├── Client.cpp
│   ├── main.cpp
│   └── Waiter.cpp
├── LICENSE
├── makefile
└── README.md
```

## Compiling and Running
To compile the program simply access the project root directory from terminal and input:
``` bash
$ make
```
To remove the compiled objects and clean the project directory tree, access the project root directory from terminal and
input:
``` bash
$ make clean
```
To run the project executable file, access the project root directory from terminal and input:
``` bash
$ make run
```