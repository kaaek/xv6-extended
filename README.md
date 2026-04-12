# xv6-extended

A teaching-oriented xv6 (x86) variant with a few practical extensions: a login screen before entering the shell, wildcard support in ls using *, a system-call counter exposed through get_syscall_count(), plus a user test program getcounttest.

**Running:** Build the kernel and user programs, then boot in QEMU:

```bash
make
make qemu
```

You should then see the login prompt from init before the shell starts. Default credentials are `root:toor` or `user:password`

# Login Screen (Added)

The login flow is implemented in `init`:
- `init` forks a child process for authentication.
- The child repeatedly prompts for Username and Password.
- Credentials are checked against a small in-memory table.
- On success, `init` execs `sh`; on failure, it prints an error and retries.
- Since authentication happens before exec("sh"), unauthenticated users cannot reach the shell loop.

# Wildcard Support in ls

`ls` now detects whether an argument contains `*`:
- If no wildcard is present, it follows normal `ls(path)` behavior.
- If `*` is present, it uses `ls_wild(pattern)`, scans the current directory, and prints entries whose names match the pattern.

**Pattern matching rules:**
- `*` matches zero or more characters.
- Other characters match literally.
- The current implementation matches names in the current directory only (no recursive globbing).

Examples inside xv6:
```bash
ls *.c
ls _*
```

Try them!

# System Call Counter + getcounttest

The kernel tracks total successful system calls since boot:
- A global counter is incremented in the `syscall` dispatcher.
- `get_syscall_count()` returns that global count.

The user program `getcounttest` prints the current value:
- Run inside xv6 shell: `getcounttest`
- This is a simple global counter, so it reports system-wide activity, not per-process usage.