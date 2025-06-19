Multi-threaded Element Queue component

### Features:

Thread-safe push and pop
Optional timeout for push and pop operations
Blocking behavior when full/empty and timeout = 0
Bounded queue with maximum size

### Initialization:
```
CustomQueue<int> queue(n);  // Create a queue with a max size of n
```
### Pushing Elements:
```
queue.push('data');         // Push immediately (blocks if full)
queue.push('data', 1000);   // Push with 1000 ms timeout
```
### If the queue is full:

With `timeout = 0`, it blocks until space is available.

With `timeout > 0`, it waits up to the specified time and returns if unsuccessful.

### Popping Elements:
```
int value = queue.pop();     // Pop immediately (blocks if empty)
int value = queue.pop(500);  // Pop with 500 ms timeout
```
If the queue is empty:

With `timeout = 0`, it blocks until an element is available.

With `timeout > 0`, it waits up to the specified time, `returns -1` if nothing is available.

### Other Utilities:
```
queue.getSize();     // Returns current number of elements
queue.getCounter();  // Returns total number of push/pop operations
```
### Unit Testing
Tests are located in the `tests/` directory and use GoogleTest. Run tests with:
```
ctest --verbose
```
### Dependencies
C++20 or later
GoogleTest (for testing)

### Project Structure:
```
.
├── main.cpp              # Entry point for testing
├── CustomClass.hpp       # Queue class declaration
├── CustomClass.tpp       # Template implementation
├── tests/                # GoogleTest unit tests
└── build/                # CMake build output
```

### License: 
MIT License
