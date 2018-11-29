# ARM Mali GPUs [online doc](https://www.baidu.com)
## 1. Introduction
### 1.1 About ARM Mali GPUs
|Architecture|OpenCL Implementation|GPUs|
|:---:|:---:|:---:|
|Bifrost| quad-style vectorization. Scalar instructions execute in parallel. Not required to vectorize code| Mali-G71|
|Midgard| SIMD style vectorization. Instructions operate on multiple data elements. Must explicitly vectorize code| Mali-T60 series, 720, 760, 820, 830, 860, 880|
|Utgard| Do not support OpenCL| -|
### 1.2 About OpenCL
Open Computing Language
### 1.3 About the Mali GPU OpenCL driver and support
The Mali GPU OpenCL driver supports different versions of OpenCL.
- OpenCL version 1.2, Full Profile.
- Binary-compatibility with OpenCL 1.0 and OpenCL 1.1 applications.

## 2. Parallel Processing Concepts
### 2.1 About parallel processing
Parallel processing is the simultaneous processing of multiple computations.  
OpenCL is an open standard language that enables developers to run general purpose computing tasks on GPUs,
 application processors and other types of processors.

### 2.2 Types of parallelism
Data parallelism, task parallelism, and pipelines are the main types of parallelism.
#### 2.2.1 Data parallelism
In data parallelism, data is divided into data elements that a processor can process in parallel.
OpenCL is primarily used for data parallel processing  
Example: rendering graphics
#### 2.2.2 Task parallelism
In task parallelism, the application is broken down into smaller tasks that execute in parallel.  
Task parallelism is also known as functional parallelism.  
Example: playing an on-line video, decode sound and video in parallel.
#### 2.2.3 Pipelines
Pipelines process data in a series of stages. In a pipeline, the stages can operate simultaneously but they do not 
process the same data.  
Example: Instruction pipeline in CPU.
### 2.3 Mixing different types of parallelism
Different types of parallelism can be mixed together.

### 2.4 Embarrassingly parallel applications
If an application can be parallelized across a large number of processors easily, it is said to be embarrassingly parallel.  
OpenCL is ideally suited for developing and executing embarrassingly parallel applications.

### 2.5 Limitations of parallel processing and Amdahl's law
If an application parallelizes perfectly, executing the application on ten processors makes it run ten times faster.
However, applications rarely parallelize perfectly because part of the application is serial. This serial component
imposes a limit on the amount of parallelization the application can use.
**Amdahl's law** describes the maximum speedup that parallel processing can achieve.
The formula for Amdahl's law is:  
Speedup = 1 / (S + (P / N))  
**S**: Fraction of serial part.  
**P**: Fraction of parallelizable part.  
**N**: Number of processors.  
**Note that**, if S is large, Speedup gets less improvement even when N is large.  
For high performance with a large number of processors, the application must have a very small serial component. These
sorts of applicstions are said to be embarrassingly parallel.
### 2.6 Concurrency (并发)
Concurrent applications have multiple operations in progress at the same time. These can operate in parallel or in serial
through the use of time sharing system.  
In a concurrent application, multiple tasks attempt to share the same data. Access to this data must be
managed to prevent complex problems such as race **conditions**, **deadlocks**, and **livelocks**.  
Concurrent applications require concurrent data structures which enable multiple tasks to gain access to the data with
no concurrency problems.  
Data parallel applications use concurrent data structures. These are the sorts of data structures that you
typically use in OpenCL.  
OpenCL includes atomic operations to help manage interactions between threads. Atomic operations
provide one thread exclusive access to a data item while it modifies it. The atomic operation enables one
thread to read, modify, and write the data item with the guarantee that no other thread can modify the
data item at the same time.  
**Note that**, OpenCL does not guarantee the order of operation of threads. Threads can start and finish in any order.

## 3. OpenCL Concepts
### 3.1 Using OpenCL
OpenCL specifies an API for parallel programming that is designed for portability:
- It uses an abstract memory and execution model.
- There is no requirement to know the application processor instruction set.  

**Kernels** that written in a language called **OpenCL C** are executed on OpenCL devices.
The OpenCL C include vector types and built-in functions that enable you to use the features of accelerators.

### 3.2 OpenCL applicatons
OpenCL applications consist of two parts:
- Host code:
    - Call the OpenCL APIs.
    - Compile the OpenCL kernels.
    - Allocate memory buffer to pass data into and out of kernels.
    - Set up command queues.
    - Set up dependencies between tasks.
    - Set up the N-Dimensional Range (NDRanges) that the kernels execute over.
- OpenCl kernels:
    - Written in OpenCL C language.
    - Perform the parallel processing.
    - Run on compute devices such as GPU shader cores.
### 3.3 OpenCL execution model
The OpenCL execution model includes:
- Host Application: (set up commands)
    - memory commands
    - kernel execution commands
    - synchronization
- Context: (manage resources)
    - kernels
    - devices
    - program
    - memory
- OpenCL Kernels: (run on devices in parallel)
    - An instance of kernel is created for each element in the NDRange.
    - Each element in NDRange process independently in parallel.

### 3.4 OpenCL data processing
The data processed by OpenCL is in an index space of work-items.  
The work-items are organized in an NDRange.  
One kernel instance is executed for each work-item in the index space.
Work-items can be grouped into work-group for processing.
### 3.5 OpenCL work-groups
- **Properties of work-groups**:
    - Independent of each other.
    - Can execute in parallel with each other.
    - The work-items in a work-group can share data buffers. (**NEED** Synchronization)
- **Limitations between work-groups**:
    - Not directly share data, they can share data using global memory (very slow).
- **work-items in a work-groups**
    - Access shared memory.
    - Use local atomic operations.
    - Perform barrier operations to synchronize execution points.

### 3.6 OpenCL identifiers
**global ID**: Every work-item has a unique global ID that identify it within the index space.  
**work-group ID**: Every work-group has a unique work-group ID.  
**local ID**: Within each work-group, each work-item has a unique local ID.
### 3.7 The OpenCL memory model
- Global memory
    - all work-items can access
    - host access it by `read`, `write`, and `map` commands
    - consistent across work-items in a single work-group, no guarantee between different work-groups
    - keyword `__global`
- Constant memory
    - allocate and initialized by the host
    - all work-items can access, but read-only
    - keyword `__constant`
- Local memory
    - specific to a work-group
    - work-items in the work-group can access
    - consistent to all work-items in the work-group
    - keyword `__local`
    - **Note that**, work-items execute in an undefined order (may need barrier)
- Private memory
    - specific to a work-item
    - not visible to other work-items
### 3.8 The Mali GPU OpenCL memory model
- Desktop workstation GPUs
    - Processors have their own dedicated memory
    - Physically separate global, local, and private memories
    - GPU has its own memory
    - Data transfer between processor memory and GPU memory
- Mali GPUs
    - Mali GPUs have a unified memory system with the application processor
    - Mali GPUs use global memory backed with caches in place of local or private memories
    - Local or private memory is allocated in global memory. Moving data from global to local or private memory
    typically does not improve performance
    - Copying data is not required, provided it is allocated by OpenCL in the context manner
    - Each compute device, that is shader core, has its own data caches
### 3.9 OpenCL concepts summary
- OpenCL primarily uses data parallel processing
- Computations in OpenCL are performed by pieces of code called kernels that execute on compute devices.
- The data processed by OpenCL is in an index space of work-items. The work-items are organized in an **NDRange**.
- One kernel instance is executed for each work-item in the index space.
- Kernel instances can execute in parallel
- Work-items are grouped into work-groups. The work-items in a work-group share local memory (may need barrier)
- Work-groups typically don't directly share data with each other. They can share date using global memory and atomic
operations, but very **slow**
- Multiple work-groups can be issued in parallel
## 4. Developing an OpenCL Application
### 4.1 Software and hardware requirements for Mali GPU OpenCL development
Development need:
- A compatible OS
- The Mali GPU OpenCL driver
- A platform with a Mali (Midgard or Bifrost) GPU
### 4.2 Development stages for OpenCL
Development stages of OpenCL application:
- **Determine what to parallelize**  
Identify the parts of the application that can run in parallel (profile)
- **Write kernels**  
Write kernels that perform the computations. Partition kernels so that the least amount of data is transferred between
them. Loading large amounts of data is often the most expensive part of an operation.
- **Write infrastructures for kernels**  
Write infrastructures code that sets up the data and prepares the kernels for execution.
- **Execute the kernels**  
Enqueue the kernels for execution and read back the results.
## 5. Execution Stages of an OpenCL Applicaton
### 5.1 About the execution stages
- **Platform setup**  
    - Use the platform API to obtain information about the hardware, and find out available devices.
    - Set up the OpenCL context.
- **Runtime setup**  
    - Create a command queue.
    - Compile and build program objects using:  
    `clCreateProgramWithSource()`, `clCreateProgramWithBinary()`, `clBuildProgram()`
    - Create the kernel and memory objects using:  
    `clCreateKernel()`, `clCreateKernelsInProgram()`, and OpenCL allocate memory API, `map()`, `unmap()`
    - Enqueue and execute the kernels:  
        - `clSetKernelArg()`
        - determine the work-group size and the index space
        - Enqueue the kernel for execution in the command queue
    - Enqueue the command that used to read the result from work-items to host
    - Clean up the unused objects
### 5.2 Finding the available compute devices
`clGetPlatformIDs()`
`clGetDeviceIDs()`  

**Note that**, Mali GPU may has several shader core groups (some are asymmetric, that is different number of cores),
and each core group is treated as a separate OpenCL device.
### 5.3 Initializing and creating OpenCL contexts
`clCreateContext()`  

The context groups the devices together to enable memory objects to be shared across different compute devices.
### 5.4 Creating a command queue
`clCreateCommandQueue()`  

Multiple devices:
- Create a command queue for each device.
- Divide up the work.
- Submit commands separately to each device.
### 5.5 Creating OpenCL program objects
`clCreateProgramWithSource()`  
`clCreateProgramWithBinary()`, `clGetProgramInfo()`  

Compilation need time!!
### 5.6 Building a program executable
`clBuildProgram()`
### 5.7 Creating kernel and memory objects
- Creating kernel objects  
`clCreateKernel()`  
`clCreateKernelsInProgram()`  
- Creating memory objects  
`clCreateBuffer()`, `clCreateImage()`  
Package the data in a memory object (**Buffer** or **Image**), and send the program data to the kernels.
### 5.8 Executing the kernel
- Determining the data dimensions
- Determining the **optimal global work size**
    - A single work-item can process multiple data items (reduce global work size).
    - The global work size must be large if you want to ensure high performance (several thousand).
    - The **ideal number** depends on the number of shader cores in your device.
- Determining the local work-group size
    - OpenCL device has **maximum** work-group size for a **specific kernel** (`clGetKernelWorkGroupInfo()`)  
    Simplify the kernel such as the size larger than 128.
    - Maximum work-group size `clGetDeviceInfo()`
- Enqueuing kernel execution  
`clEnqueueNDRangeKernel()`
- Executing kernels  
command queue has property `CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE`
### 5.9 Reading the results
Map the memory buffer into host memory: `clEnqueueMapBuffer()`  
`clFinish()` must be called to make the buffer available.
### 5.10 Cleaning up unused objects
```objectivec
// decrement the reference count
// Ensure the reference counts reach zero when exit
clReleaseMemOjbect()
clReleaseKernel()
clReleaseProgram()
clReleaseCommandQueue()
clReleaseContext()
// to obtain the reference count, for example:
clGetMemObjectInfo()
```

## 6. Converting Existing Code to OpenCL
### 6.1 Profiling application
### 6.2 Analyzing code for parallelization
### 6.3 Parallel processing techniques in OpenCL
### 6.4 Using parallel processing with non-parallelizable code
### 6.5 Dividing data for OpenCL

## ARM Mali GPU T860 MP4 Specification (RK3399)
```
mkdir build && cd build && cmake ..
make
./gpuInfo
```

|items|value|
|:---:|:---:|
|OpenCL Version|OpenCL 1.2|
|CUs|4|
|Max work-group size|256|
|Max memory allocate size|480MB|
|Global memory size|1.88GB|
|Global memory cache size|25KB|
|Global memory cache line size|64B|
|Local memory size|32KB|
