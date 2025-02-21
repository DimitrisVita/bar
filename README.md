# Bar Simulation Project

This project simulates the operation of a bar with multiple visitors and a receptionist. Visitors sit at tables, place orders, and rest for a while before leaving. The receptionist processes the visitors' orders. This project is for practicing using semaphores.

## Execution Instructions

1. **Compile the project:**
    ```sh
    make all
    ```

2. **Start the simulation:**
    ```sh
    ./smem_init
    ```

3. **Monitor the bar status:**
    ```sh
    ./monitor -s <shmid>
    ```

## Semaphores

Semaphores are used for process synchronization and avoiding racing conditions. In this project, the following semaphores are used:

- `mutex`: Used to protect shared memory from concurrent accesses. When a process wants to read or write to shared memory, it must first acquire the `mutex` semaphore.

- `sit`: Used to control the availability of chairs. When a visitor wants to sit, they must acquire the `sit` semaphore, which is decremented by 1. When the visitor leaves, the semaphore is incremented by 1. Its initial value is equal to: `NUM_TABLES * CHAIRS_PER_TABLE`.

- `order`: Used to synchronize the visitor's order with the receptionist. The visitor waits to acquire the `order` semaphore to receive their order.

- `wakeup`: Used to wake up the receptionist when a new visitor arrives. The visitor increments the `wakeup` semaphore to notify the receptionist.

- `log`: Used to synchronize logging to the log file. When a process wants to log a message, it must first acquire the `log` semaphore.

## Statistics

- `Average visit duration`: The average time visitors spend in the bar. Calculated from the moment the visitor arrives at the bar until they leave.
- `Average wait duration`: The average time visitors wait to find a chair. Calculated from the moment the visitor arrives at the bar until they find a chair.

## Files

- `visitor.c`: Code for visitors who sit, place orders, and leave.
- `receptionist.c`: Code for the receptionist who processes visitors' orders.
- `smem_utils.c`: Utility functions for managing shared memory and signals.
- `smem_init.c`: Initialization of shared memory and starting processes.
- `monitor.c`: Display statistics and table status.
- `common_types.h`: Common data structures and constants.
- `Makefile`: Build instructions for the project.

## Functions

### visitor.c

- `find_chair(SharedMemory* shm)`: Search for an empty chair.
- `leave_chair(SharedMemory* shm)`: Leave the chair.

### smem_utils.c

- `create_shmem()`: Create a shared memory segment.
- `attach_shmem(int shmid)`: Attach a shared memory segment to the process.
- `detach_shmem(SharedMemory* shm)`: Detach a shared memory segment from the process.
- `destroy_shmem(int shmid)`: Destroy a shared memory segment.
- `init_semaphores(SharedMemory* shm)`: Initialize semaphores.
- `log_message(SharedMemory* shm, const char* format, ...)`: Log a message to the log file.

### smem_init.c

- `init_statistics(SharedMemory* shm)`: Initialize statistics.
- `init_tables(SharedMemory* shm)`: Initialize tables.

### monitor.c

- `print_statistics(SharedMemory* shm)`: Print statistics.
- `print_tables(SharedMemory* shm)`: Print table status.

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.
