# Producer-Consumer Problem
Producer Consumer Problem, also known as Bounded-Buffer Problem, takes a number of producers and consumers along with a buffer of limited size. The buffer is where the producer stores its products and it is also where the consumers get their consumptions. The problem states that a single space on the buffer could only be accessed by a single entity (producer/consumer) at a time.

---
## Solution
The solution is implemented using the C programming language and POSIX Thread or Pthread API.
### Pthread Subroutines
##### The following are the pthread subroutines that is used in the program:
- Thread Management - for creating (pthread_create) and joining (pthread_join) producer  and consumer threads
- Mutexes - for creating a mutex that handles the locking and unlocking of buffer array
- Condition Variable - for checking whether the buffer array has space left or whether it has a content

### Other libraries
- unistd.h - contains the sleep() used in the producer and consumer functions
### Variables
##### Global Variables
- ``` BUFFER_SIZE : int``` - the predetermined size of the buffer array
- ``` bufferHasContent : int ``` - serves as a tracker for the buffer content
- ``` putIndex : int ``` - the index where the producer puts the products
- ``` getIndex : int ``` - the index where the consumer gets the products
- ``` bufferMutex : pthread_mutex_t ``` - mutual exclusion for the buffer array to ensure that only one thread has the access to it
- ``` bufferFull : pthread_cond_t ``` - condition variable for the buffer that checks if there is a content in the buffer. Used as a condition in consumer()
- ``` bufferEmpty : pthread_cond_t ``` - condition variable for the buffer that checks if there is a space in the buffer. Used as a condition in the producer().
### Function Flows
##### main : int
- parse cli arguments
- initialized variables
- create threads for producers
- create threads for consumers
- join threads
- destroy variables
##### producer : void*
- parse producer id
- produces product
- lock buffer
- wait for a space in the buffer
- put the product in the buffer
- print statement
- signal other threads for the product
- release lock on buffer
##### consumer : void*
- parse consumer id
- lock buffer
- wait for a content in the buffer
- consume product
- print statement
- signal other threads for the consumption
- release lock on buffer

