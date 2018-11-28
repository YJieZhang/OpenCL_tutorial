# OpenCL Tutorial
## High-Level Overview [video](https://www.youtube.com/watch?v=8D6yhpiQVVI)
### Device Modle
- Global Memory
- Constant Memory
- Compute Units
    - Local Memory
    - Processing Element: all instructions are executed on the PE.
        - Private Memory

### Memory Model
#### Global Memory
- Shared with all PE
- Host can access this memory too
  - memory map
  - Copy data to/from global memory
- This is OpenCL persistent storage
  - Other Memory regions are scratch space

#### Constant memory
- Shared with all processing elements
- Read-only memory
- Very efficient way to share data with all device PEs
- Not persistent

#### Local Memory
- Shared with all PEs in a CU
- Very efficient way to share date with all CU PEs
- Cannot be accessed by other CUs!
- Not persistent

#### Private Memory
- Accessible by a single PE
- No other PE can access this memory!
- Not persistent

### Execution Model
#### Kernel Functions
- OpenCL executes kernel functions on the device
    - Ordinary functions with a special signature
- Kernel calls have two parts:
    1) Ordinary function argument list
    ```objectivec
    int add(int x, int y);
    ```
    2) External execution parameters that control parallelism  
    Execution Parameters

#### Role of the Host in Kernel Execution
- Coordinates execution
    - Does not participate itself
- Provides arguments to the kernel
- Provides executions parameters to launch the kernel
#### NDRange: Identifying the Call
- Execution Parameters provide an index space
    - Each function invocation can access its index
- The index space is N-Dimensional  
global id  
global work offset  
global work size  

#### NDRange: Soem Definitions
- Work-item: invocation of the kernel for a particular index
- Global ID: globally unique id for a work-item (from index space)
- Global Work Size: the number of work-items (per dimension)
- Work Dimension: dimension of the index space

### Mapping Execution Model to Device Model
#### Mapping NDRange to Devices
- Remember the PE runs instructions
    - So work-items should run on PEs
- Assign multiple work-items to each PE
    - Handle global work size > number of PEs
- But, how to Use Local memory?
#### NDRange: Work-Groups
- Partition the global work into smaller pieces called work-groups
- Work-groups execute on CUs
    - Work-items within work-group dispatched to PEs within the CU
    - Local Memory shared by work-items within the work-group
- CU local memory shared by the work-group
    - that is all work-items in the work-group share local memory
- Work-group size has a physical meaning
    - It is device specific and related to performance
- Work-items can find out many things:
    - work-group id
    - work-groups size
    - global id
    - global work size
#### Work-Group Size
- Maximum work-group size is a device characteristic
    - You can query a device to determine this value
- Maximum work-group size is an integer
    - Handle N-dimensional work-groups in a special way
- How to determine the best work-group size?
    - Later on...
    
#### N-Dimensional Work-Groups
- You can have multiple dimensions
    - You can think of this geometrically
    - You can think of this as pulling N-dimensional tuples from a set
- The device maximum work-group size is scalar
    - But your work-groups can be N-dimensional sizes
    - e.g. max work-groups size is 32, but launching (8,2,1) dimensions

#### Final Kernel Call Points
- Host will provide execution dimensions to the device
    - This creates an index space
- Parameters can be values or global memory objects
- Global memory is persistent between calls
    - Constant, Local, Private memory is just scratch space
    - They are going to be reset per kernel call
- OpenCL implementation has considerable flexibility
    - How to map work-items to PEs?
    - How to schedule work?

### Host API
#### Platform
- A platform is an implementation of OpenCL
- Platforms are like drivers for particular devices
    - Platforms expose devices to you
- Example: A system with two GPUs and a Xeon Phi
    - A platform from AMD for one GPU
    - A platform from Intel for the CPU
    - A platform from NVIDIA for the other GPU
- Use the platform to discover devices available to you
#### Context
- You create a context for a particular platform
    - You cannot have multiple platforms in a context
- A context is a container
    - Contains devices
    - Contains memory
- Most operations are related to a context
    - Implicitly or explicitly
#### Programs
- Programs are just collections of kernels
    - You extract kernels from your program to call them
- OpenCL applications have to load kernels
    - Compile OpenCL C source code
    - Load binary representation
- Programs are device specific
#### Asynchronous Device Calls
The host manages devices asynchronously
- Host issues commands to the device
- Commands tell the device to do something
- Devices take commands and do as they say
- Host waits for commands to compute
    - This means the device has completed that action
- Commands can be dependent on other commands
- OpenCL commands are issued by `clEnqueue*` calls
    - A `cl_event` returned by `clEnqueue*` calls is used for dependencies

#### Command Queue
- A command queue is attached to a single device
- You can create as many command queues as you want
- `clEnqueue*` commands have a command-queue parameter

Host call `clEnqueue*` ->
command in command queue ->
command will run and control device ->
Host can find out that command completed

### Host API summary
- Host API controls the device
    - device cannot do anything themselves
- Asynchronous execution model
    - Important for speed
    - A bit different from traditional asynchronous APIs
    
## OpenCL C [video](https://www.youtube.com/watch?v=RKyhHonQMbw)
### OpenCL is not C
- No function pointers
- No recursion
- Function calls might be inlined
- OpenCL C is not a subset of C
    - OpenCL C has features not in C
- The specification outlines the full set of differences

## What is OpenCL C
- OpenCL device programming language
- The main actor in OpenCL programming
- OpenCL C is like C99
    - Despite differences, same flavor of language
- The other part of the OpenCL specification

### Types
#### OpenCL C vs C
- OpenCL C provides a concrete representation
    - Signed integers are two's complement
    - Types have fixed sizes
- OpenCL C provides vector types and operations
- OpenCl C provides image types
#### Integral Types
|type|size(bit)|
|:----:|:---:|
|char|8|
|short|16|
|int|32|
|long|64|

**Each of them has unsigned version**
#### Floating point types
|type|size(bit)|
|:----:|:---:|
|half|16|
|float|32|
|double|64|

**half and double types might not be supported**
#### Host and Device Types
#### Memory Regions
Keyword: `__globle, __constant, __local, __private`  
e.g. `__global int* x // meaning pointer to an integer in global memory`
#### Assigning Pointers
```objectivec
__global  int* x;
__global  int* y;
__private int* z;

x = y; // Okay!
x = z; // Error! Two different memory regions.
*x = *z; // Okay! Copy from private memory to global memory. But high cost!
```
### Vectors
#### Vectorization Approaches
- Use libraries
    - Just shifts the problem to somebody who isn't you
- Let the compiler to do it
    - Vectorizing compiler
- C++ classes
    - Not going to be terribly efficient
- Intrinsics
- Assembly language
#### OpenCL C Vector Types
charN, shortN, intN, longN, floatN, doubleN
ucharN, ushortN, uintN, ulongN, -, -
N = {1, 2, 4, 8, 16}
#### Vector Operations
```objectivec
float4 x, y, z;
z = x + y; // element-wise operation
// +, -, *, /, >, >=, <, <=, ==, !=, 

float x;
float4 y, z;
z = (float4)x + y; // Broadcast
```
#### Vector Components
```objectivec
vec.<component>
// {s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, sA, sB, sC, sD, sE, sF}
```
#### Why Use OpenCL Vector Types
```objectivec
// OpenCL C
float4 x, y;
float4 z = x + y;
```
OpenCL C compiler will compile it to:
```asm
vector_add_4xf32 r3, r1, r2
```
#### OpenCL C Vector Type Advantages
- Clear communication of vector operations
    - You and the compiler both know these are vectors
- Simplifies code: it looks nicer
- Excellent performance: compiler can do a great job

### Structures
#### OpenCL C Structures
- OpenCL C has structures and unions just like C
- There are good reasons to not use them because of performance!
- Be careful of data exchange!
    - Binary layout of struct must be same on device and host
    - Almost impossible to get right!
### Functions
#### OpenCL C Functions: Overview
- Ordinary C functions
- Recursion is forbidden
- Functions might be expanded and inlined by the compiler

### Kernels
#### Introducing Kernels
- Kernels are entry points to device execution
    - like `int main(int argc, char** argv)`
- Kernels are called by the host
    - Host will setup parameters for the call
    - Host will supply execution parameters for the call
    - Device runs function
- Kernel arguments are pointers to `__global` or just values
```objectivec
// Kernel Example
__kernel void add(__global float* x, __global float* y, __global float* z, uint32_t len) {
    size_t idx = get_global_id(0);
    if (idx >= len) {
        return;
    }
    z[idx] = x[idx] + y[idx];
}
```
#### Review: Execution Model
- Execution has dimensions
- Global work size
- Global offset
- Work-group size
#### Relevant Functions
```objectivec
get_global_id(n)     // Global work-item id in dimension n
get_global_offset(n) // Global offset in dimension n
get_local_id(n)      // Local work-item id in dimension n
```

#### Local Memory
- Memory shared by work-items within the work-group
    - **Might** be implemented in hardware
- Often key to top-performance
```objectivec
// Local memory example:
__kernel void foo(__global float* in, __global float* out, __local float* scratch, uint32_t len) {
    size_t global_idx = get_global_id(0);
    size_t local_idx = get_local_id(0);
    if (global_idx >= len) {
        return;
    }
    scartch[locak_idx] = in[global_idx];
    // do some local operation
    out[global_idx] = scartch[local_idx];
}

// Alternate
__kernel void foo(__global float* in, __global float* out, uint32_t len) {
    __local float scratch[1024];
    
    size_t global_idx = get_global_id(0);
    size_t local_idx = get_local_id(0);
    if (global_idx >= len) {
        return;
    }
    scartch[locak_idx] = in[global_idx];
    // do some local operation
    out[global_idx] = scartch[local_idx];
}
```
#### Constant Memory
- Read-only memory shared by all work-items
    - Very fast to read
    - But relatively small amount of space
- Useful in some circumstances
    - e.g. lookup tables
#### Kernel Limitations
- Kernels might execute concurrently on the device
    - But there is no mechanism for them to cooperate
- A single kernel is limited in what it can do
    - You might need to launch several kernels to get a job done
- Kernels cannot allocate memory
    - Everything is fixed prior to kernel execution
#### Kernel Attributes
```objectivec
vec_type_hint // hint to the compiler for vectorization
reqd_work_group_size // Forces a work-group size (useful for performance)
```
### Quick Topic
#### OpenCL Image Support
- OpenCL supports image operations
    - Load an image, do something, write an image
- I don't like images in OpenCL

#### Built-in OpenCL C Functions
- Work-item functions
    - Figure out the kernel launch parameters
- Math functions
- Integer functions
- Geometric functions
- ...
#### Synchronization
- This is a complex topic
- OpenCL C provides atomic operations
- OpenCL C parallelism is a bit tricky
#### Extensions
- The OpenCL C standard has extensions
- These are extra features that you can enable with `#pragma`
- As always, be careful!


## General GPU Architecture [video](https://www.youtube.com/watch?v=1Yr-E9w4tGI)
### GPUs for General Purpose Use? GPGPU
### OpenCL GPU Architecture
- Global memory implemented in hardware
- Constant memory implemented in hardware
- Which means physically exist.
#### GPU Compute Unit Model

#### AMD wavefront(or NVIDIA warp)
A **wavefront** is a collection of work-items, from the same work-group, executed together with a locked instruction pointer.  
`f = select(y, x, a < b)`  
`f = select(g(y), g(x), g(a) < g(b)) // not a good idea`
### GPU Memory
#### Memory Expense
- global > constant > local > private
- ALU efficiency
#### Trick: Increase ALU Use
Memory I/O is constant, so increase ALU operations to pay for it.

### Memory Latency Issues
#### Our Problem
- Memory transfer is very expensive
- Our goal is 100% ALU utilization
- Recall: work-items run on a processing element
- Recall: work-groups are scheduled on compute units
- Insight: overload the compute unit with work-groups!
#### Latency Hiding
The most interesting thing about GPU programming.
- Latency hiding works on wavefronts, not work-groups
- Memory access can be free!
#### Global Scheduler
When a work-group terminates, another is loaded to take its place.
#### Occupancy
Compute Units support a **fixed** maximum number of wavefronts.
- Calculating Occupancy
- Vendor documentation: maximum number of wavefronts per CU
- Number of wavefronts actually run
- Help to design significantly faster kernels

- Private memory and local memory limit the occupancy
### Back to OpenCL
- Reason in terms of work-items
- Set **work-group size** to be **wavefront size**
- Understand the following:
    - more resources per work-item -> fewer wavefronts -> less latency hiding
    - fewer resources per work-item -> more wavefronts -> more latency hiding
### Memory Channels
#### Global Memory Accesses
- Global memory partitioned into subsets
- Each subset is accessed through a channel
- All global memory has an address
- When you access memory the address maps to a channel
    - You actually need to know this for performance
#### Channel Conflicts
- Memory requests can be serialized
    - if a single channel gets multiple requests, they are handled serially
- Be careful of how you access memory
- Pigeonhole Principle?
- Some memory channel will always get multiple memory requests if number of CUs > number of memory channels
- There is one efficient access pattern
    - Adjacent work-items access adjacent memory (coalesced)
#### Extreme Memory Architecture Details
- Don't worry about the fine-grained details
    - You might care for extreme performance, but probably not
- Instead rely upon informative benchmarks
    - Know what is generally fast or slow
- You should know the following:
    - Work-items loading adjacent values is very fast
    - Work-items loading random values is very slow
- You can use high-level facts to design algorithms.


# General Middleware Objectives [video](https://www.youtube.com/watch?v=-mEQhf8MeUI)
- Reduce OpenCL management complexity
- Provide different abstractions than standard OpenCL
    - Can eliminate complexity without sacrificing **performance**
- Enable good software engineering
- Reduce time to market for developers
    - Reduced complexity means increased productivity
- Enable development of reusable libraries
- Such as **SYCL**, which specific to OpenCL 1.2
