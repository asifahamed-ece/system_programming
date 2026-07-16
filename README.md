![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![POSIX](https://img.shields.io/badge/POSIX-000000?style=for-the-badge&logo=linux&logoColor=white)
![System Programming](https://img.shields.io/badge/System_Programming-4A154B?style=for-the-badge&logo=gnu&logoColor=white)

# 🐧 Linux System Programming - Concepts & Implementation Guide

A comprehensive technical guide covering fundamental concepts of Linux System Programming, including file operations, process management, thread synchronization, and inter-process communication.

📖 Table of Contents
- File Management & I/O Operations
- Process Management
- Thread Programming & Synchronization
- Inter-Process Communication (IPCs)

---

## File Management & I/O Operations

### Core Concepts

**System Calls vs Library Functions**  
Linux provides two layers of file operations:  
- **System Calls** (`open`, `read`, `write`): Direct interface to the kernel. No buffering, immediate hardware interaction.  
- **Library Functions** (`fopen`, `fread`, `fwrite`): Higher-level C standard library functions with user-space buffering.

> **Key Point:** System calls trigger a context switch from User Mode to Kernel Mode. The kernel performs the actual I/O operation and returns control to the user process.

---

**File Descriptors**  
A File Descriptor (fd) is a non-negative integer that the kernel uses to identify an open file.  
- `0` = Standard Input (stdin)  
- `1` = Standard Output (stdout)  
- `2` = Standard Error (stderr)  
- `3+` = User-opened files

> **Key Point:** File descriptors are process-local. The same fd number in different processes refers to different files.

---

**Low-Level I/O: `open()`, `read()`, `write()`, `close()`**

```c
int open(const char *pathname, int flags, mode_t mode);
ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);
int close(int fd);
```

> **Key Points:**  
> - `open()` returns a file descriptor. Use flags like `O_RDONLY`, `O_WRONLY`, `O_RDWR`, `O_CREAT`, `O_TRUNC`.  
> - `read()` and `write()` return the number of bytes actually transferred. Always check for `-1` (error) or partial reads/writes.  
> - `close()` releases the file descriptor. Always close files to prevent resource leaks.

---

**File Metadata: `stat()`**

```c
int stat(const char *pathname, struct stat *statbuf);
```

> **Key Points:**  
> - Retrieves file information without opening the file.  
> - The `struct stat` contains:  
>   - `st_size`: File size in bytes  
>   - `st_mode`: File type and permissions (use `S_ISREG()`, `S_ISDIR()` macros)  
>   - `st_uid`, `st_gid`: Owner and group IDs  
>   - `st_atime`, `st_mtime`, `st_ctime`: Access, modify, and change timestamps  
>   - `st_ino`: Inode number (unique file identifier)  
> - Use Case: Checking file size, permissions, or type before processing.

---

**Random Access: `lseek()`**

```c
off_t lseek(int fd, off_t offset, int whence);
```

> **Key Points:**  
> - Moves the file offset pointer without reading data.  
> - `whence` options:  
>   - `SEEK_SET`: Offset from beginning of file  
>   - `SEEK_CUR`: Offset from current position  
>   - `SEEK_END`: Offset from end of file  
> - Trick: `lseek(fd, 0, SEEK_END)` returns the file size without using `stat()`.  
> - Use Case: Jumping to a specific byte in a file (like seeking an address in flash memory).

---

**File Permissions: `chmod()`, `fchmod()`**

```c
int chmod(const char *pathname, mode_t mode);
int fchmod(int fd, mode_t mode);
```

> **Key Points:**  
> - Permissions are represented in octal (base-8):  
>   - `7` = rwx (read + write + execute = 4+2+1)  
>   - `6` = rw- (read + write = 4+2)  
>   - `5` = r-x (read + execute = 4+1)  
>   - `4` = r-- (read only)  
> - Format: `0` (e.g., `0755` = rwxr-xr-x)  
> - `chmod()` uses file path; `fchmod()` uses file descriptor (faster if file is already open).  
> - Use Case: Setting executable permissions on scripts, restricting access to sensitive files.

---

**File Descriptor Duplication: `dup()`, `dup2()`**

```c
int dup(int oldfd);
int dup2(int oldfd, int newfd);
```

> **Key Points:**  
> - Creates a copy of a file descriptor. Both fd's point to the same file and share the same offset.  
> - `dup()` returns the lowest available fd.  
> - `dup2()` forces `newfd` to be a copy of `oldfd`. If `newfd` was open, it's silently closed first.  
> - This is how shell redirection (`>`, `2>&1`) works under the hood.  
> - Use Case: Redirecting stdout to a file, or duplicating pipes for inter-process communication.

---

## Process Management

### Core Concepts

**Process Creation: `fork()`**

```c
pid_t fork(void);
```

> **Key Points:**  
> - Creates a new process (child) by duplicating the calling process (parent).  
> - Returns `0` to the child, the child's PID to the parent, or `-1` on error.  
> - After `fork()`, both processes execute the next instruction concurrently.  
> - The child gets a **copy** of the parent's memory, file descriptors, and environment.  
> - Use Case: Running multiple tasks simultaneously, such as a web server handling multiple clients.

---

**Process Execution: `exec()` Family**

```c
int execl(const char *path, const char *arg, ...);
int execlp(const char *file, const char *arg, ...);
int execle(const char *path, const char *arg, ..., char *const envp[]);
int execv(const char *path, char *const argv[]);
int execvp(const char *file, char *const argv[]);
int execvpe(const char *file, char *const argv[], char *const envp[]);
```

> **Key Points:**  
> - Replaces the current process image with a new program.  
> - Does **not** create a new process; the PID remains the same.  
> - On success, `exec()` never returns; on error, returns `-1`.  
> - Common pattern: `fork()` + `exec()` to start a new program in a child process.  
> - Use Case: Launching other programs (e.g., shell running `ls`, `gcc`).

---

**Waiting for Processes: `wait()`, `waitpid()`**

```c
pid_t wait(int *status);
pid_t waitpid(pid_t pid, int *status, int options);
```

> **Key Points:**  
> - `wait()` blocks until any child process terminates.  
> - `waitpid()` waits for a specific child (by PID) or any child with options.  
> - The `status` argument captures the child's exit status (use `WIFEXITED`, `WEXITSTATUS` macros).  
> - Without `wait()`, terminated children become **zombies** (no resources but still in process table).  
> - Use Case: Synchronizing with child completion, collecting exit codes.

---

**Process Termination: `exit()`, `_exit()`**

```c
void exit(int status);
void _exit(int status);
```

> **Key Points:**  
> - `exit()` performs clean shutdown (flushes stdio buffers, calls `atexit` handlers).  
> - `_exit()` terminates immediately, bypassing cleanup (used in child after `fork()`).  
> - The `status` (0 = success, non-zero = error) is returned to the parent via `wait()`.  
> - Use Case: Exiting a program with a return code; `_exit()` in child processes after `exec()` failure.

---

**Process States**

- **Running**: Currently executing on CPU.  
- **Ready**: Runnable but waiting for CPU.  
- **Blocked**: Waiting for I/O or a resource.  
- **Zombie**: Terminated but not yet reaped by parent (shows as `Z` in `ps` output).  
- **Orphan**: Parent exited before child; adopted by `init` (PID 1).  

> **Fix for Zombies:** Parent must call `wait()` or `waitpid()` to reap them.  
> **Fix for Orphans:** None needed; kernel adopts them automatically.

---

## Thread Programming & Synchronization

### Core Concepts

**Processes vs Threads**

| Feature | Process (`fork`) | Thread (`pthread`) |
|---------|------------------|---------------------|
| Memory Space | Separate | Shared |
| Creation Speed | Slow (copies memory) | Fast |
| Communication | Requires IPC (pipes, shared memory) | Direct (shared variables) |
| Isolation | High (one crash doesn't affect others) | Low (one crash kills all threads) |

> **Key Point:** Threads are lightweight processes that share the same memory space. They are ideal for parallelizing tasks within a single application.

---

**Thread Creation: `pthread_create()`**

```c
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                   void *(*start_routine)(void *), void *arg);
```

> **Key Points:**  
> - Creates a new thread that runs `start_routine(arg)`.  
> - The thread runs concurrently with the main program.  
> - Return value is `0` on success, error code on failure.  
> - Use `pthread_join()` to wait for a thread to finish (similar to `wait()` for processes).  
> - **Key Concept:** Thread creation order ≠ execution order. The OS scheduler decides which thread runs when. Threads interleave at the microsecond level.

---

**Race Conditions**

A race condition occurs when multiple threads access shared data concurrently, and the final result depends on the timing of their execution.

**Example:**
```c
int counter = 0;
// Thread 1: counter++;
// Thread 2: counter++;
// Expected: counter = 2
// Actual (without synchronization): counter might be 1!
```

> Why it happens: The `counter++` operation is not atomic. It involves:  
> 1. Read counter from memory  
> 2. Increment it  
> 3. Write it back  
> If two threads interleave between steps, one increment is lost.

---

**Mutex (Mutual Exclusion)**

A mutex is a synchronization primitive that ensures only one thread can access a critical section at a time.

```c
pthread_mutex_t mutex;
pthread_mutex_init(&mutex, NULL);
pthread_mutex_lock(&mutex);    // Acquire the lock
// Critical section (only one thread here at a time)
pthread_mutex_unlock(&mutex);  // Release the lock
pthread_mutex_destroy(&mutex);
```

> **Key Points:**  
> - `pthread_mutex_lock()` blocks if another thread holds the lock.  
> - Always pair `lock()` with `unlock()`. Forgetting to unlock causes deadlock.  
> - Mutexes protect shared resources (variables, data structures, hardware).  
> - **Analogy:** A mutex is like a key to a bathroom. Only one person can hold the key at a time. Others must wait outside.

---

## Inter-Process Communication (IPCs)

### Core Concepts

**Why IPCs?**  
Processes have separate memory spaces. They cannot directly share variables. IPCs provide mechanisms for processes to communicate and synchronize.

**Common IPC Mechanisms:**  
- **Pipes:** Unidirectional byte stream (like a UART connection)  
- **Shared Memory:** Fastest IPC (processes map the same physical memory)  
- **Message Queues:** Structured message passing  
- **Semaphores:** Synchronization primitive (counter-based)  
- **Signals:** Asynchronous notifications (like hardware interrupts)

---

**Anonymous Pipes: `pipe()`**

```c
int pipe(int pipefd[2]);
```

> **Key Points:**  
> - Creates a unidirectional data channel (half-duplex).  
> - `pipefd[0]` is the read end; `pipefd[1]` is the write end.  
> - Data written to `pipefd[1]` can be read from `pipefd[0]`.  
> - Pipes are anonymous (no name in the filesystem). Only related processes (parent-child) can use them.

**Usage Pattern:**  
1. Parent calls `pipe()` to create the pipe.  
2. Parent calls `fork()` to create a child.  
3. Child closes the read end (`pipefd[0]`) and writes to `pipefd[1]`.  
4. Parent closes the write end (`pipefd[1]`) and reads from `pipefd[0]`.

> **Key Concept:** Always close the end of the pipe you're not using! If you don't, `read()` will block forever waiting for more data.  
> **Analogy:** A pipe is like a UART connection between two microcontrollers. One transmits (TX), the other receives (RX).

---

## 🔧 Best Practices

**Error Handling**  
- Always check return values of system calls. A return of `-1` indicates an error.  
- Use `perror()` or `strerror(errno)` to print human-readable error messages.  
- **Example:**
  ```c
  if (open("file.txt", O_RDONLY) == -1) {
      perror("open failed");
      exit(EXIT_FAILURE);
  }
  ```

**Resource Management**  
- Always close file descriptors when done. Leaked fds can exhaust the system limit.  
- Always reap child processes using `wait()` or `waitpid()` to prevent zombies.  
- Always unlock mutexes to prevent deadlocks.

**Concurrency**  
- Protect shared resources with mutexes or other synchronization primitives.  
- Minimize critical sections (code protected by locks) to maximize parallelism.  
- Avoid holding locks while performing I/O (can cause performance bottlenecks).

---

## 📚 Further Reading

- *Linux System Programming* by Robert Love  
- *The Linux Programming Interface* by Michael Kerrisk  
- *Advanced Programming in the UNIX Environment* by W. Richard Stevens

---

*Built with a focus on deep conceptual understanding and practical implementation.*
