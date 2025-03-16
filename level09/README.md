# main


### Function Prologue

```asm
0x0000000000000aa8 <+0>:     push   %rbp
```
- **Save the old base pointer**: The `%rbp` register is pushed onto the stack to save the previous frame pointer before the function begins.

```asm
0x0000000000000aa9 <+1>:     mov    %rsp,%rbp
```
- **Set the current base pointer**: The stack pointer `%rsp` is moved into `%rbp` to create the stack frame for the current function (`main`).

### Calling `puts` to Print a String

```asm
0x0000000000000aac <+4>:     lea    0x15d(%rip),%rdi        # 0xc10
```
- **Load effective address**: The instruction calculates the address of the string located at `0x15d` bytes offset from the current instruction pointer (`%rip`) and stores it in `%rdi`. This is the string that will be passed as an argument to `puts`.
- **Note**: The address is calculated using RIP-relative addressing, meaning the value is calculated based on the current instruction pointer.

```asm
0x0000000000000ab3 <+11>:    callq  0x730 <puts@plt>
```
- **Call `puts` function**: The string address in `%rdi` is passed to `puts`, which prints the string to the standard output.

### Calling `handle_msg` Function

```asm
0x0000000000000ab8 <+16>:    callq  0x8c0 <handle_msg>
```
- **Call `handle_msg` function**: After printing the string, the program proceeds to call the function `handle_msg`. No arguments are explicitly passed, so it likely works with the state of the program or other global variables.

### Function Return

```asm
0x0000000000000abd <+21>:    mov    $0x0,%eax
```
- **Set the return value to `0`**: The register `%eax` is set to `0`, which indicates that the `main` function will return 0 (typically indicating successful execution).

```asm
0x0000000000000ac2 <+26>:    pop    %rbp
```
- **Restore the old base pointer**: The value of `%rbp` is popped from the stack to restore the previous function's stack frame.

```asm
0x0000000000000ac3 <+27>:    retq
```
- **Return from `main`**: The `retq` instruction is used to return from the `main` function and transfer control back to the operating system or the calling function. Since the return value is set to `0`, it indicates successful execution.

---

### Summary of the `main` Function:

1. It prints a string using `puts`.
2. Then, it calls the `handle_msg` function.
3. It sets the return value to `0` (success) and returns.

# handle_msg

### Function Prologue

```asm
0x00000000000008c0 <+0>:     push   %rbp
```
- **Save the old base pointer**: The `%rbp` register is pushed onto the stack to preserve the previous function's frame pointer.

```asm
0x00000000000008c1 <+1>:     mov    %rsp,%rbp
```
- **Set the current base pointer**: The stack pointer `%rsp` is moved into `%rbp`, setting up the current stack frame for `handle_msg`.

```asm
0x00000000000008c4 <+4>:     sub    $0xc0,%rsp
```
- **Allocate space on the stack**: The stack pointer is adjusted downwards by `0xc0` (192 bytes), creating space for local variables.

### Clearing Memory

```asm
0x00000000000008cb <+11>:    lea    -0xc0(%rbp),%rax
```
- **Load effective address**: This instruction calculates the address of the local buffer (at offset `-0xc0` from `%rbp`) and stores it in `%rax`.

```asm
0x00000000000008d2 <+18>:    add    $0x8c,%rax
```
- **Adjust the address**: It adds `0x8c` (140 bytes) to the address in `%rax`.

```asm
0x00000000000008d8 <+24>:    movq   $0x0,(%rax)
0x00000000000008df <+31>:    movq   $0x0,0x8(%rax)
0x00000000000008e7 <+39>:    movq   $0x0,0x10(%rax)
0x00000000000008ef <+47>:    movq   $0x0,0x18(%rax)
0x00000000000008f7 <+55>:    movq   $0x0,0x20(%rax)
```
- **Clear local variables**: These instructions set the first five entries of the buffer (pointed to by `%rax`) to `0` (zero-initialize memory).

### Setting a Value in Local Stack

```asm
0x00000000000008ff <+63>:    movl   $0x8c,-0xc(%rbp)
```
- **Set a value in a local variable**: This stores the value `0x8c` (140 in decimal) into the local stack variable at offset `-0xc` from `%rbp`.

### Calling `set_username`

```asm
0x0000000000000906 <+70>:    lea    -0xc0(%rbp),%rax
```
- **Load the address of the local buffer**: This calculates the address of the local buffer again, now with the stack offset `-0xc0` from `%rbp`.

```asm
0x000000000000090d <+77>:    mov    %rax,%rdi
```
- **Pass the address to `set_username`**: The address of the buffer is moved into `%rdi`, which is the first argument register for the `set_username` function.

```asm
0x0000000000000910 <+80>:    callq  0x9cd <set_username>
```
- **Call `set_username`**: The function `set_username` is called with the address of the buffer in `%rdi`.

### Calling `set_msg`

```asm
0x0000000000000915 <+85>:    lea    -0xc0(%rbp),%rax
```
- **Load the address of the local buffer again**: The address of the buffer is recalculated.

```asm
0x000000000000091c <+92>:    mov    %rax,%rdi
```
- **Pass the address to `set_msg`**: The address of the buffer is moved into `%rdi`, which is the first argument register for `set_msg`.

```asm
0x000000000000091f <+95>:    callq  0x932 <set_msg>
```
- **Call `set_msg`**: The function `set_msg` is called with the address of the buffer in `%rdi`.

### Printing a String with `puts`

```asm
0x0000000000000924 <+100>:   lea    0x295(%rip),%rdi        # 0xbc0
```
- **Load string address**: The instruction calculates the address of a string located at offset `0x295` from the current instruction pointer (`%rip`) and stores it in `%rdi`. This string will be passed to `puts`.

```asm
0x000000000000092b <+107>:   callq  0x730 <puts@plt>
```
- **Call `puts` function**: The address of the string is passed to `puts` to print the string.

### Function Epilogue

```asm
0x0000000000000930 <+112>:   leaveq
```
- **Restore the stack frame**: The `leaveq` instruction is used to restore the previous stack frame by setting `%rsp` back to `%rbp` and popping `%rbp`.

```asm
0x0000000000000931 <+113>:   retq
```
- **Return from `handle_msg`**: The `retq` instruction transfers control back to the caller, i.e., the `main` function.

---

### Summary of `handle_msg` Function:

1. **Clears a local buffer** in the stack.
2. Sets a value (`0x8c`) into a local variable.
3. Calls `set_username` with the address of the local buffer.
4. Calls `set_msg` with the same buffer.
5. Prints a string using `puts`.
6. Returns control to the caller.

# set_username

### Function Prologue

```asm
0x00000000000009cd <+0>:     push   %rbp
0x00000000000009ce <+1>:     mov    %rsp,%rbp
```
- **Save the previous stack frame**: The base pointer (`%rbp`) is saved to preserve the previous function's stack frame.
- **Set up the new stack frame**: The current stack pointer (`%rsp`) is moved into `%rbp` to set up the new stack frame.

```asm
0x00000000000009d1 <+4>:     sub    $0xa0,%rsp
```
- **Allocate space on the stack**: The stack pointer is decreased by `0xa0` (160 bytes), creating space for local variables.

### Storing and Preparing for String Operations

```asm
0x00000000000009d8 <+11>:    mov    %rdi,-0x98(%rbp)
```
- **Store the username argument**: The username (passed in `%rdi`) is stored in the local variable at offset `-0x98` from `%rbp`.

```asm
0x00000000000009df <+18>:    lea    -0x90(%rbp),%rax
0x00000000000009e6 <+25>:    mov    %rax,%rsi
0x00000000000009e9 <+28>:    mov    $0x0,%eax
0x00000000000009ee <+33>:    mov    $0x10,%edx
```
- **Prepare for `rep stos` instruction**: These instructions set up registers for the `rep stos` instruction, which is a string operation. It will store a value in a memory region.

```asm
0x00000000000009f3 <+38>:    mov    %rsi,%rdi
0x00000000000009f6 <+41>:    mov    %rdx,%rcx
0x00000000000009f9 <+44>:    rep stos %rax,%es:(%rdi)
```
- **Zero out a memory region**: The `rep stos` instruction is used to fill memory with a value. In this case, it will zero out `0x10` (16 bytes) of memory starting at the address stored in `%rsi`.

### Printing Strings

```asm
0x00000000000009fc <+47>:    lea    0x1e1(%rip),%rdi        # 0xbe4
0x0000000000000a03 <+54>:    callq  0x730 <puts@plt>
```
- **Print a message**: This instruction loads the address of a string into `%rdi` and calls the `puts` function to print the string.

```asm
0x0000000000000a08 <+59>:    lea    0x1d0(%rip),%rax        # 0xbdf
0x0000000000000a0f <+66>:    mov    %rax,%rdi
0x0000000000000a12 <+69>:    mov    $0x0,%eax
0x0000000000000a17 <+74>:    callq  0x750 <printf@plt>
```
- **Print a formatted message**: This loads a string into `%rdi` and calls `printf` to print the formatted string.

### Fetching User Input

```asm
0x0000000000000a1c <+79>:    mov    0x201595(%rip),%rax        # 0x201fb8
0x0000000000000a23 <+86>:    mov    (%rax),%rax
0x0000000000000a26 <+89>:    mov    %rax,%rdx
```
- **Fetch some data**: The address `0x201595` from `%rip` is loaded into `%rax`. This is likely an address of a global or static variable.

```asm
0x0000000000000a29 <+92>:    lea    -0x90(%rbp),%rax
0x0000000000000a30 <+99>:    mov    $0x80,%esi
0x0000000000000a35 <+104>:   mov    %rax,%rdi
```
- **Prepare for input**: The address of the buffer (`-0x90(%rbp)`) is loaded into `%rax`, and the `fgets` function will be called to read input into this buffer.

```asm
0x0000000000000a38 <+107>:   callq  0x770 <fgets@plt>
```
- **Call `fgets` to get input**: This function reads up to `0x80` bytes of input from the user and stores it in the buffer at `-0x90(%rbp)`.

### Processing User Input

```asm
0x0000000000000a3d <+112>:   movl   $0x0,-0x4(%rbp)
0x0000000000000a44 <+119>:   jmp    0xa6a <set_username+157>
```
- **Initialize a counter**: A local variable at `-0x4(%rbp)` is initialized to `0`. This counter is likely used for processing the user input.

```asm
0x0000000000000a46 <+121>:   mov    -0x4(%rbp),%eax
0x0000000000000a49 <+124>:   cltq
0x0000000000000a4b <+126>:   movzbl -0x90(%rbp,%rax,1),%ecx
```
- **Process input byte-by-byte**: This part loads a byte from the buffer (`-0x90(%rbp,%rax,1)`) and stores it in `%ecx`.

```asm
0x0000000000000a53 <+134>:   mov    -0x98(%rbp),%rdx
```
- **Load username address**: The username address stored earlier (`-0x98(%rbp)`) is moved into `%rdx`.

```asm
0x0000000000000a5a <+141>:   mov    -0x4(%rbp),%eax
0x0000000000000a5d <+144>:   cltq
0x0000000000000a5f <+146>:   mov    %cl,0x8c(%rdx,%rax,1)
```
- **Store input byte**: The byte from the user input (`%cl`) is stored in the username at the calculated offset (`0x8c(%rdx,%rax,1)`).

```asm
0x0000000000000a66 <+153>:   addl   $0x1,-0x4(%rbp)
```
- **Increment the counter**: The counter is incremented by `1` to process the next byte.

```asm
0x0000000000000a6a <+157>:   cmpl   $0x28,-0x4(%rbp)
0x0000000000000a6e <+161>:   jg     0xa81 <set_username+180>
```
- **Check for maximum length**: The counter is compared to `0x28` (40), and if it exceeds this value, it jumps to the end of the loop.

```asm
0x0000000000000a70 <+163>:   mov    -0x4(%rbp),%eax
0x0000000000000a73 <+166>:   cltq
0x0000000000000a75 <+168>:   movzbl -0x90(%rbp,%rax,1),%eax
0x0000000000000a7d <+176>:   test   %al,%al
0x0000000000000a7f <+178>:   jne    0xa46 <set_username+121>
```
- **Process each byte**: These instructions continue the loop to process each byte of the user input until it reaches the end (i.e., null byte `\0`).

### Finalizing the Username

```asm
0x0000000000000a81 <+180>:   mov    -0x98(%rbp),%rax
0x0000000000000a88 <+187>:   lea    0x8c(%rax),%rdx
0x0000000000000a8f <+194>:   lea    0x165(%rip),%rax        # 0xbfb
0x0000000000000a96 <+201>:   mov    %rdx,%rsi
0x0000000000000a99 <+204>:   mov    %rax,%rdi
```
- **Prepare for final print**: After processing the input, the username is printed using `printf`.

```asm
0x0000000000000aa1 <+212>:   callq  0x750 <printf@plt>
```
- **Print the final result**: The username is printed.

### Function Epilogue

```asm
0x0000000000000aa6 <+217>:   leaveq
0x0000000000000aa7 <+218>:   retq
```
- **Clean up the stack**: The stack frame is restored, and control is returned to the caller.

---

### Summary of `set_username`:

1. It clears a local memory region.
2. It prints some strings.
3. It fetches user input into a buffer.
4. It processes each byte of the input, storing it into a "username" buffer.
5. If the input is valid (terminated with `\0`), it prints the username.

# set_msg

### Function Prologue

```asm
0x0000000000000932 <+0>:     push   %rbp
0x0000000000000933 <+1>:     mov    %rsp,%rbp
```
- **Save the previous stack frame**: The current function's prologue saves the base pointer (`%rbp`) to preserve the calling function's stack frame.
- **Set up a new stack frame**: The stack pointer (`%rsp`) is moved into `%rbp` to create a new stack frame.

```asm
0x0000000000000936 <+4>:     sub    $0x410,%rsp
```
- **Allocate space on the stack**: The stack pointer is adjusted to allocate space (0x410 bytes) for local variables and other necessary data in this function.

### Storing Arguments and Preparing for String Operations

```asm
0x000000000000093d <+11>:    mov    %rdi,-0x408(%rbp)
```
- **Store the argument**: The first argument (`%rdi`) is stored at offset `-0x408(%rbp)`. This argument likely represents a message.

```asm
0x0000000000000944 <+18>:    lea    -0x400(%rbp),%rax
0x000000000000094b <+25>:    mov    %rax,%rsi
0x000000000000094e <+28>:    mov    $0x0,%eax
0x0000000000000953 <+33>:    mov    $0x80,%edx
```
- **Prepare for string operation**: These instructions prepare the registers for the `rep stos` operation, which is typically used to fill a memory region. Here it likely sets up the buffer for a string operation.

```asm
0x0000000000000958 <+38>:    mov    %rsi,%rdi
0x000000000000095b <+41>:    mov    %rdx,%rcx
0x000000000000095e <+44>:    rep stos %rax,%es:(%rdi)
```
- **Fill memory**: The `rep stos` instruction is used to fill a memory block with a value. It likely initializes or clears a section of the memory.

### Printing Messages

```asm
0x0000000000000961 <+47>:    lea    0x265(%rip),%rdi        # 0xbcd
0x0000000000000968 <+54>:    callq  0x730 <puts@plt>
```
- **Print a string**: The address of a string is loaded into `%rdi`, and the `puts` function is called to print it.

```asm
0x000000000000096d <+59>:    lea    0x26b(%rip),%rax        # 0xbdf
0x0000000000000974 <+66>:    mov    %rax,%rdi
0x0000000000000977 <+69>:    mov    $0x0,%eax
0x000000000000097c <+74>:    callq  0x750 <printf@plt>
```
- **Print another message**: Another string is loaded into `%rdi`, and this time, `printf` is called to print it, possibly with formatting.

### Fetching User Input

```asm
0x0000000000000981 <+79>:    mov    0x201630(%rip),%rax        # 0x201fb8
0x0000000000000988 <+86>:    mov    (%rax),%rax
0x000000000000098b <+89>:    mov    %rax,%rdx
```
- **Load some global data**: The address `0x201630` from `%rip` is loaded into `%rax`, and the value at that address is then moved into `%rax` again. This data is likely related to the message handling.

```asm
0x000000000000098e <+92>:    lea    -0x400(%rbp),%rax
0x0000000000000995 <+99>:    mov    $0x400,%esi
0x000000000000099a <+104>:   mov    %rax,%rdi
```
- **Prepare for user input**: The address of a buffer (`-0x400(%rbp)`) is loaded into `%rax`, and `fgets` will be called to read up to `0x400` (1024) bytes into that buffer.

```asm
0x000000000000099d <+107>:   callq  0x770 <fgets@plt>
```
- **Call `fgets` to get input**: `fgets` reads a string from the user input and stores it into the buffer located at `-0x400(%rbp)`.

### Processing User Input

```asm
0x00000000000009a2 <+112>:   mov    -0x408(%rbp),%rax
0x00000000000009a9 <+119>:   mov    0xb4(%rax),%eax
0x00000000000009af <+125>:   movslq %eax,%rdx
```
- **Process the input**: The function accesses a field from the argument (`-0x408(%rbp)`), likely a structure or object, and moves it into `%rax`. This data is further manipulated in the following lines.

```asm
0x00000000000009b2 <+128>:   lea    -0x400(%rbp),%rcx
0x00000000000009b9 <+135>:   mov    -0x408(%rbp),%rax
0x00000000000009c0 <+142>:   mov    %rcx,%rsi
0x00000000000009c3 <+145>:   mov    %rax,%rdi
```
- **Copy data**: The function prepares to copy data from one buffer to another, possibly copying a string into the message buffer.

```asm
0x00000000000009c6 <+148>:   callq  0x720 <strncpy@plt>
```
- **Call `strncpy`**: This function is used to copy a string from one location to another, with a limit on the number of characters copied.

### Function Epilogue

```asm
0x00000000000009cb <+153>:   leaveq
0x00000000000009cc <+154>:   retq
```
- **Clean up**: The function cleans up the stack frame and returns control to the caller.

---

### Summary of `set_msg`:

1. The function takes an argument (likely a message).
2. It initializes some memory regions and prints messages to the user.
3. It uses `fgets` to read a string from the user input into a buffer.
4. The function processes the input and copies it into another location, possibly related to the message.
5. It then cleans up and returns.

# secret_backdoor

### Function Prologue

```asm
0x000000000000088c <+0>:     push   %rbp
0x000000000000088d <+1>:     mov    %rsp,%rbp
```
- **Save previous stack frame**: The current function's prologue saves the base pointer (`%rbp`) and sets up a new stack frame by copying the stack pointer (`%rsp`) into `%rbp`.

```asm
0x0000000000000890 <+4>:     add    $0xffffffffffffff80,%rsp
```
- **Allocate space**: The stack pointer is adjusted by `0xffffffffffffff80`, which effectively reserves space for local variables in the function.

### Fetching Data from a Global Address

```asm
0x0000000000000894 <+8>:     mov    0x20171d(%rip),%rax        # 0x201fb8
0x000000000000089b <+15>:    mov    (%rax),%rax
0x000000000000089e <+18>:    mov    %rax,%rdx
```
- **Fetch address and data**: The value at the memory address `0x20171d(%rip)` is loaded into `%rax`, and then the value at the address in `%rax` is moved into `%rax` again. Finally, `%rax` is copied into `%rdx`. This seems to load some kind of pointer or address into `%rdx`.

### Reading User Input

```asm
0x00000000000008a1 <+21>:    lea    -0x80(%rbp),%rax
0x00000000000008a5 <+25>:    mov    $0x80,%esi
0x00000000000008aa <+30>:    mov    %rax,%rdi
0x00000000000008ad <+33>:    callq  0x770 <fgets@plt>
```
- **Prepare for input**: The address `-0x80(%rbp)` is loaded into `%rax`, which points to a buffer on the stack. The size `0x80` (128 bytes) is moved into `%esi`, and the address of the buffer (`%rax`) is moved into `%rdi`.
- **Call `fgets`**: The `fgets` function is called to read up to 128 bytes of input from the user into the buffer at `-0x80(%rbp)`.

### Executing a Command

```asm
0x00000000000008b2 <+38>:    lea    -0x80(%rbp),%rax
0x00000000000008b6 <+42>:    mov    %rax,%rdi
0x00000000000008b9 <+45>:    callq  0x740 <system@plt>
```
- **Prepare to execute the input**: The address of the buffer (`-0x80(%rbp)`) is loaded into `%rax`, and this buffer (likely containing a command) is moved into `%rdi`.
- **Call `system`**: The `system` function is called, which will execute the command in the buffer. This allows the user to execute arbitrary shell commands.

### Function Epilogue

```asm
0x00000000000008be <+50>:    leaveq
0x00000000000008bf <+51>:    retq
```
- **Clean up**: The stack frame is cleaned up, and control is returned to the caller.

---

### Summary of `secret_backdoor`:

1. **Fetches some data**: The function loads some global or static data into `%rdx`.
2. **Reads user input**: It then reads up to 128 bytes of user input into a buffer on the stack using `fgets`.
3. **Executes the input**: The input is treated as a shell command and passed to `system`, which executes it.
4. **Returns**: The function cleans up the stack and returns control.

This function effectively implements a backdoor where a user can input a command, and the program will execute it in the system shell.

