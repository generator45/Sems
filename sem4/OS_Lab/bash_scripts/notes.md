# JP NOTES
## Lecture 0

### File and Process Analysis Commands

- `od –cx <filename>` → See the dump of the file.
- `objdump –d <executable file or object file>` → View assembly of a file.
- `gdb <executable file with path>` → Step-by-step follow the file execution.
- `strace <executable file with path>` → List system calls made when the program executes.
- `ltrace <executable file with path>` → List library calls made when the program executes.
- `man <command or call>` → View manual page information (e.g., `man ls`, `man 2 printf`, `man 3 printf`, `man ps`).
- Other useful tools: `ps`.

---

## Lecture 1

### Process Creation & Execution

- Child process gets return value `0` from `fork` command; the parent gets the process ID of the new child.
- **Summary:** The variables `x` and `i` of the child process are completely independent from the parent process.
  - **Processes are isolated!**

### `execv(prog, params)`

- Replaces the current process with itself (executes with the process ID of the current program).

#### What Happens After `execv`?

- **If **``** succeeds:**

  - The original program is **completely replaced**.
  - The new program **starts from **`` as if started normally.
  - The original program’s **code, data, and stack are lost**.
  - **Control never returns** to the original process.

- **If **``** fails:**

  - It returns `-1`, and the original process **continues execution** from the next line after `execv`.
  - `errno` is set to indicate the error.

- **First argument of **`` → Name of the program to execute.

### `wait()` System Call

#### Key Takeaways:

- `wait(&wstatus)` **blocks** the parent until the child process terminates.
- `wstatus` **stores child process exit information**:
  - `WIFEXITED(wstatus)` → Checks if the child exited normally.
  - `WEXITSTATUS(wstatus)` → Retrieves the exit code.
  - `WIFSIGNALED(wstatus)` → Checks if the child was killed by a signal.
  - `WTERMSIG(wstatus)` → Gets the signal that terminated the child.
- If the parent **never calls **``, the child becomes a **zombie process** (until the parent exits).

---

### Man Pages

- `syscalls`
- `funccalls`

### Process States & Management

- `ctrl+z` → Suspend a process.
- `fg`, `bg` → Manage foreground/background processes.
- `+` → Foreground process.
- `R` → Running.
- `S` → Sleeping.
- `s` → Session Leader.
- `l` → Multithreaded.
- `Z` → Zombie (process exited but metadata is saved by Linux).

**Session Leader:**

- Initializes a session (often the shell or a parent process of related processes).
- Controls `ctrl+C`.

**Nice Values:**

- **Only superusers** can make a process faster (by giving **negative** nice values).
- Regular users **can only slow down** a command.

---

## 22/1/25

### Static vs Dynamic Linking

- **Library calls contain system calls.**
- **Static linking** → All library code linked into a single file.
- **Dynamic linking** → References to linked code, creating a shorter file.
- `libc.so` **references the actual library executing the code**, saving memory.

### Compilation & Linking

#### @Compile Time (Code Compilation)

- When using `printf`, the compiler **does not include the actual implementation**, only a **reference**.

Example:

```c
printf("Hello, World!\n");
```

- The compiler generates a **call to **``, but does not include its code.

#### @Linking (Dynamic Linking)

- The linker **does not copy** `printf` into the program.
- Instead, it **adds metadata** so `printf` is resolved at runtime from `libc.so.6`.

### Process Groups

- **If two processes start in the same command line**, they are assigned the **same **``.
- `kill` can terminate process groups.

---

## Process Scheduling

- **If arrival times are the same**, **Shortest Job First (SJF)** is used.
- **Job suspension (preemption):** A job may be suspended for another job based on the scheduling algorithm.
- **STCF (Shortest Time to Completion First):** If jobs reach the same state, apply SJF.
- **Response time** → Time between hitting enter and the system's first response.
- **Interactive jobs** → Require **better response times**.
- **Round Robin (RR)** → Causes **heavy context switching** (smaller time slices increase switches).
- **Overlapping Disk I/O & CPU** → Saves execution time.
- **Nice value** is **not related to SJF**.
- **Users decide priority.**

---

## 5/02/2025

### Process Priorities

- **FIFO & RR** → High-priority tasks (**95% CPU usage**).
- **SCHED\_OTHER** → Regular tasks (**5% CPU usage**).

### `chrt` Command

- **Manipulates real-time attributes** of a process.

#### Syntax:

```sh
chrt [options] priority command argument ...
```

- Can **change process policies** to **increase CPU priority**.
- **Dynamic priority changes** can be made based on **CPU time used**.

---

## CPU Scheduling Tick

- `HZ` → Kernel constant defining **how many times per second** the CPU is interrupted.

# Lec-02 Process Management


-  Policy – its about using the mechanism to achieve a certain end  
- Mechanism – it’s a set of tools, capabilities, ‘library’  

### What processes exist on the system at any time (on Linux)?
Use:
- `ps –ax`, or
- Examine `/proc/` directory  

All jobs under one (virtual) terminal (like pts/2) are in one session.  
A job is a fancy word for the process (or processes) on a single command.

All processes have a priority that determines how it will be scheduled.  
- Indicated by a **‘nice’ value** in Linux.  
- A process that is nice to others has reduced scheduling chance.  
  - **Higher nice value** ⇒ **Lower scheduling chance**  
  
To see nice values:
- `ps -axl`
- Commands to manage priority:  
  - `nice(1)`, `nice(2)`, `renice(1)`  
  - `getpriority(2)`, `setpriority(2)`  
  - See **priority_ops.c**

### System Calls and Context Switching
- Syscall makes you jump to **kernel state**, and after completion, `sysret` switches context back to **user mode**.
- A process has attributes stored in the **PCB (Process Control Block)**, including:
  - Process ID, parent process info  
  - List of open files  
  - Info on memory occupied  
  - Register values, including PC, flags (Context of a process)  
  - Scheduling information (time used, priority, etc.)
- **Context is stored in the PCB when task is not running (e.g., task switch).**  
- **Process table stores a mapping:** `pid --> PCB`

### Kernel and User Mode Context
- **User Stack:** Holds function calls, local variables, etc.
- **Kernel Stack:** Used instead when executing in kernel mode.
- When a syscall is made, the **context of the process** is stored in **this kernel stack**.
- **Kernel stack may optionally hold the PCB.**

### Process States
#### Terminated state and Zombie Processes
- **Child process may terminate before the parent.**
- The parent is expected to issue `wait()` to get child exit info.
- Until the parent reads the exit status, the child enters the **terminated (zombie) state**.
- Once the parent reads the exit status, the child PCB is removed.

#### Orphaned Processes
- **Parent exits early, but child continues to run** ⇒ orphaned process.
- Handling varies by OS:
  - **Some OSes:** Orphaned children terminate.
  - **Linux:** Process continues, but parent is changed (Reparenting process).
  - **New parent:** Usually `systemd`.

### Context Switching
#### Goals of Context Switching (A ➔ B)
- **A terminates**, and **B is ready**.
- **A is running but becomes blocked**, and **B is ready**.
- **A uses its time quota**, and **B is ready**.
- **CPU is given to another process efficiently.**

#### Scheduling and Run Queue
- **Processes in Ready state** ⇒ candidates for CPU.
- **Run Queue:** Organizes and selects processes for CPU.
- **TAT (Turnaround Time):** `Completion Time - Arrival Time`.
- **Response Time:** Time to first CPU access.
- **STCF:** Shortest time to complete first.
- **Round Robin:** Time slicing, balances response time vs. overhead.

### Multi-Level Feedback Queue Scheduling
- **Priority changes over time**.
- **Rules:**
  1. If **P > Q**, run **P**.
  2. If **P = Q**, run in **Round Robin**.
  3. New processes start with **highest priority**.
  4. Reduce priority **as process uses more time**.
  5. Periodically **boost priority** of all processes.

### Real-Time Scheduling
- **SCHED_DEADLINE:**
  - **Periodic execution.**
  - **Deadline constraint.**
  - **Guaranteed run time before deadline.**

### Notion of a "Tick"
- **Unit of time in the kernel.**
- Also called **Jiffy** in Linux.
- **Time to interrupt the CPU.**
- Controlled by an external timer (e.g., PIC).
- **Kernel constant HZ:**
  - **HZ = 1000** means CPU is interrupted **every ms**.
  - Used for **time accounting and scheduling**.
- **Each timer interrupt:**
  - Tracks process time usage.
  - Calls the **scheduler if needed**.

-------------------------------------------------------------------------------------




if two processes are started in the same command line then they are assigned the same  "PGID"
kill can be used to kill groups

Check if a program is statically or dynamically linked:
Each foreground command gets a new PGID (like your ps command).
Each background job (started separately) gets a unique PGID.
Processes started together in a single job share a PGID.
GNOME session and other system processes follow their own session management.
ldd my_program