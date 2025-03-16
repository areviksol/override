# main

### Function Prologue
```asm
0x08048723 <+0>:    push   %ebp
0x08048724 <+1>:    mov    %esp,%ebp
```
- Standard function prologue. Save the caller's base pointer and set up the new stack frame.

```asm
0x08048726 <+3>:    push   %edi
0x08048727 <+4>:    push   %esi
0x08048728 <+5>:    push   %ebx
```
- Save `edi`, `esi`, and `ebx` which are callee-saved registers.

```asm
0x08048729 <+6>:    and    $0xfffffff0,%esp
```
- Align stack to 16 bytes (SSE and ABI compliance).

```asm
0x0804872c <+9>:    sub    $0x1d0,%esp
```
- Allocate `0x1d0` bytes on the stack for local variables. This is a large local buffer setup.

---

### Argument Saving
```asm
0x08048732 <+15>:   mov    0xc(%ebp),%eax        ; argv
0x08048735 <+18>:   mov    %eax,0x1c(%esp)
0x08048739 <+22>:   mov    0x10(%ebp),%eax       ; envp
0x0804873c <+25>:   mov    %eax,0x18(%esp)
```
- Saving `argv` and `envp` to the stack.

```asm
0x08048740 <+29>:   mov    %gs:0x14,%eax
0x08048746 <+35>:   mov    %eax,0x1cc(%esp)
```
- Save stack canary to `0x1cc(%esp)`.

---

### Zero Initialization of Local Buffer
```asm
0x0804874d <+42>:   xor    %eax,%eax
0x0804874f <+44>:   movl   $0x0,0x1b4(%esp)
...
0x08048786 <+99>:   movl   $0x0,0x1c8(%esp)
```
- Zero out several local variables (`0x1b4` to `0x1c8`).

```asm
0x08048791 <+110>:  lea    0x24(%esp),%ebx
0x08048795 <+114>:  mov    $0x0,%eax
0x0804879a <+119>:  mov    $0x64,%edx
0x0804879f <+124>:  mov    %ebx,%edi
0x080487a1 <+126>:  mov    %edx,%ecx
0x080487a3 <+128>:  rep stos %eax,%es:(%edi)
```
- Zero 0x64 (100) bytes at `0x24(%esp)` → buffer initialization.

---

### Zeroing out argv strings
```asm
0x080487a5 <+130>:  jmp    0x80487ea <main+199>
```
- Jump to loop condition.

```asm
0x080487a7 <+132>:  mov    0x1c(%esp),%eax
0x080487ab <+136>:  mov    (%eax),%eax           ; load current argv[i]
0x080487ad <+138>:  movl   $0xffffffff,0x14(%esp)
...
0x080487c2 <+159>:  repnz scas %es:(%edi),%al
```
- Determine string length of argv[i] using `scasb`.

```asm
0x080487c4 <+161>:  mov    %ecx,%eax
0x080487c6 <+163>:  not    %eax
0x080487c8 <+165>:  lea    -0x1(%eax),%edx       ; strlen(argv[i])
```

```asm
0x080487cb <+168>:  mov    0x1c(%esp),%eax
0x080487cf <+172>:  mov    (%eax),%eax           ; argv[i]
0x080487d1 <+174>:  mov    %edx,0x8(%esp)        ; length
0x080487d5 <+178>:  movl   $0x0,0x4(%esp)        ; fill with 0
0x080487dd <+186>:  mov    %eax,(%esp)           ; destination
0x080487e0 <+189>:  call   memset
```
- Call `memset(argv[i], 0, strlen(argv[i]))`

```asm
0x080487e5 <+194>:  addl   $0x4,0x1c(%esp)
0x080487ea <+199>:  mov    0x1c(%esp),%eax
0x080487ee <+203>:  mov    (%eax),%eax
0x080487f0 <+205>:  test   %eax,%eax
0x080487f2 <+207>:  jne    0x80487a7 <main+132>
```
- Loop until `argv[i] == NULL`

---

### Same logic for `envp`
```asm
0x080487f4 <+209>:  jmp    0x8048839 <main+278>
...
```
- Same structure as above, just for `envp`.

---

### Print welcome message
```asm
0x08048843 <+288>:  movl   $0x8048b38,(%esp)
0x0804884a <+295>:  call   puts
0x0804884f <+300>:  mov    $0x8048d4b,%eax
0x08048854 <+305>:  mov    %eax,(%esp)
0x08048857 <+308>:  call   printf
```
- Prints something like a prompt: `"Input command: "` (string at `0x8048d4b`)

---

### User input with `fgets`
```asm
0x0804885c <+313>:  movl   $0x1,0x1b4(%esp)       ; flag = 1
0x08048867 <+324>:  mov    0x804a040,%eax         ; stdin
0x0804886c <+329>:  mov    %eax,0x8(%esp)
0x08048870 <+333>:  movl   $0x14,0x4(%esp)
0x08048878 <+341>:  lea    0x1b8(%esp),%eax
0x0804887f <+348>:  mov    %eax,(%esp)
0x08048882 <+351>:  call   fgets
```
- Read user input into buffer at `0x1b8(%esp)` (20 bytes).

---

### Strip newline
```asm
0x080488a9 <+390>:  sub    $0x1,%eax
0x080488af <+396>:  movb   $0x0,0x1b8(%esp,%eax,1)
```
- Null-terminate at the newline (chomp input)

---

### Check for `"store"` command
```asm
0x080488c0 <+413>:  mov    $0x8048d5b,%eax         ; "store"
...
0x080488df <+444>:  test   %eax,%eax
0x080488e1 <+446>:  jne    0x80488f8 <main+469>
```
- If input == `"store"`, jump to store handler.

```asm
0x080488e3 <+448>:  lea    0x24(%esp),%eax
0x080488ea <+455>:  call   store_number
```

---

### `"read"` command check
```asm
0x080488f8 <+469>:  lea    0x1b8(%esp),%eax
...
0x0804892b <+520>:  call   read_number
```
- If input == `"read"`, call `read_number`.

---

### `"quit"` command check
```asm
0x08048942 <+543>:  mov    $0x8048d66,%eax        ; "quit"
...
0x08048963 <+576>:  je     0x80489cf <main+684>   ; exit
```

---

### Print result
```asm
0x08048965 <+578>:  cmpl   $0x0,0x1b4(%esp)
0x0804896d <+586>:  je     0x8048989 <main+614>
```
- Check success flag. Print result message accordingly.

---

### Clear input buffer
```asm
0x080489a1 <+638>:  lea    0x1b8(%esp),%eax
0x080489a8 <+645>:  movl   $0x0,(%eax)
...
```
- Clear the 20-byte input buffer after processing.

---

### Function epilogue
```asm
0x080489d0 <+685>:  mov    $0x0,%eax
0x080489d5 <+690>:  mov    0x1cc(%esp),%esi
0x080489dc <+697>:  xor    %gs:0x14,%esi
0x080489e3 <+704>:  je     0x80489ea <main+711>
0x080489e5 <+706>:  call   __stack_chk_fail
```
- Check stack canary. If it fails, terminate the program.

---

# store_number
---

### Function Prologue

```asm
0x08048630 <+0>:     push   %ebp
0x08048631 <+1>:     mov    %esp,%ebp
0x08048633 <+3>:     sub    $0x28,%esp
```
- Standard prologue: set up a new stack frame with 0x28 (40) bytes of local storage.

---

### Variable Initialization

```asm
0x08048636 <+6>:     movl   $0x0,-0x10(%ebp)     ; store = 0
0x0804863d <+13>:    movl   $0x0,-0xc(%ebp)      ; index = 0
```
- Two local variables are set to zero:  
  - `-0x10(%ebp)` → the value to store  
  - `-0xc(%ebp)` → the index where it should be stored

---

### Prompt for Value

```asm
0x08048644 <+20>:    mov    $0x8048ad3,%eax      ; " Number: "
0x08048649 <+25>:    mov    %eax,(%esp)
0x0804864c <+28>:    call   printf
```
- Prints prompt asking for the number.

```asm
0x08048651 <+33>:    call   0x80485e7 <get_unum>
0x08048656 <+38>:    mov    %eax,-0x10(%ebp)
```
- Calls `get_unum()` to read an unsigned int from input, stores it at `-0x10(%ebp)`.

---

### Prompt for Index

```asm
0x08048659 <+41>:    mov    $0x8048add,%eax      ; " Index: "
0x0804865e <+46>:    mov    %eax,(%esp)
0x08048661 <+49>:    call   printf
```

```asm
0x08048666 <+54>:    call   0x80485e7 <get_unum>
0x0804866b <+59>:    mov    %eax,-0xc(%ebp)
```
- Reads an index to store the number at.

---

### Index Bounds Check

```asm
0x0804866e <+62>:    mov    -0xc(%ebp),%ecx       ; index
0x08048671 <+65>:    mov    $0xaaaaaaab,%edx
0x08048676 <+70>:    mov    %ecx,%eax
0x08048678 <+72>:    mul    %edx
0x0804867a <+74>:    shr    %edx
0x0804867c <+76>:    mov    %edx,%eax
0x0804867e <+78>:    add    %eax,%eax
0x08048680 <+80>:    add    %edx,%eax
0x08048682 <+82>:    mov    %ecx,%edx
0x08048684 <+84>:    sub    %eax,%edx
0x08048686 <+86>:    test   %edx,%edx
0x08048688 <+88>:    je     0x8048697 <store_number+103>
```

This weird sequence is doing this:
```c
if (index % 3 == 0)
```
- If the index is divisible by 3, skip the "block" check.

---

### Memory Block Check (upper byte of value)

```asm
0x0804868a <+90>:    mov    -0x10(%ebp),%eax         ; value
0x0804868d <+93>:    shr    $0x18,%eax               ; extract highest byte
0x08048690 <+96>:    cmp    $0xb7,%eax
0x08048695 <+101>:   jne    0x80486c2 <store_number+146>
```

This checks:
```c
if ((value >> 24) != 0xb7)
```
Only values whose **highest byte is `0xb7`** are allowed **unless** index % 3 == 0.

---

### Print "store failed"

```asm
0x08048697 <+103>:   movl   $0x8048ae6,(%esp)       ; "*** ERROR! ***"
0x0804869e <+110>:   call   puts
0x080486a3 <+115>:   movl   $0x8048af8,(%esp)       ; "This index is reserved for wil..."
0x080486aa <+122>:   call   puts
0x080486af <+127>:   movl   $0x8048ae6,(%esp)
0x080486b6 <+134>:   call   puts
0x080486bb <+139>:   mov    $0x1,%eax
0x080486c0 <+144>:   jmp    0x80486d5 <store_number+165>
```

- Rejects the store. Returns 1 to indicate failure.

---

### Perform the Store

```asm
0x080486c2 <+146>:   mov    -0xc(%ebp),%eax         ; index
0x080486c5 <+149>:   shl    $0x2,%eax               ; index * 4
0x080486c8 <+152>:   add    0x8(%ebp),%eax           ; add base pointer (arg1)
0x080486cb <+155>:   mov    -0x10(%ebp),%edx         ; value
0x080486ce <+158>:   mov    %edx,(%eax)              ; *(arg + index*4) = value
```

- This is where the write happens:
  ```c
  destination[index] = value;
  ```

- `0x8(%ebp)` is the first function argument = pointer to the array (passed from `main`).

---

### Return

```asm
0x080486d0 <+160>:   mov    $0x0,%eax               ; return 0 on success
0x080486d5 <+165>:   leave
0x080486d6 <+166>:   ret
```

---

### Summary

This function:
- Prompts the user for a number and index.
- If `index % 3 != 0`, only values starting with `0xb7` are allowed.
- If check passes, the value is stored at `dest[index]`.

---

# read_number

---

### Function Prologue

```asm
0x080486d7 <+0>:     push   %ebp
0x080486d8 <+1>:     mov    %esp,%ebp
0x080486da <+3>:     sub    $0x28,%esp
```

- Standard function prologue.  
- Allocates 0x28 (40) bytes of stack space.

---

### Local Variable Initialization

```asm
0x080486dd <+6>:     movl   $0x0,-0xc(%ebp)
```

- Initializes a local variable at `-0xc(%ebp)` to 0.  
  Let's call it `index`.

---

### Prompt for Index

```asm
0x080486e4 <+13>:    mov    $0x8048add,%eax     ; " Index: "
0x080486e9 <+18>:    mov    %eax,(%esp)
0x080486ec <+21>:    call   printf
```

- Asks the user for an index.

---

### Read Index

```asm
0x080486f1 <+26>:    call   get_unum
0x080486f6 <+31>:    mov    %eax,-0xc(%ebp)
```

- Calls `get_unum()` to read the index input and stores it in `index`.

---

### Calculate Address to Read From

```asm
0x080486f9 <+34>:    mov    -0xc(%ebp),%eax      ; index
0x080486fc <+37>:    shl    $0x2,%eax            ; index * 4
0x080486ff <+40>:    add    0x8(%ebp),%eax       ; + array base address
```

- Calculates the address of `array[index]`:
  ```c
  value = array[index];
  ```

---

### Read the Value at That Address

```asm
0x08048702 <+43>:    mov    (%eax),%edx
```

- Reads the value from the computed address into `edx`.

---

### Format the Output

```asm
0x08048704 <+45>:    mov    $0x8048b1b,%eax      ; "%u = %u\n"
0x08048709 <+50>:    mov    %edx,0x8(%esp)       ; value
0x0804870d <+54>:    mov    -0xc(%ebp),%edx      ; index
0x08048710 <+57>:    mov    %edx,0x4(%esp)
0x08048714 <+61>:    mov    %eax,(%esp)
0x08048717 <+64>:    call   printf
```

- Prepares arguments and prints:
  ```c
  printf("%u = %u\n", index, value);
  ```

---

### Return 0

```asm
0x0804871c <+69>:    mov    $0x0,%eax
0x08048721 <+74>:    leave
0x08048722 <+75>:    ret
```

---

### Summary

This function:
1. Asks for an index.
2. Calculates the address: `array + index * 4`.
3. Reads and prints the value at that index with no bounds check.

---

# get_unum

---

### Function Prologue

```asm
0x080485e7 <+0>:     push   %ebp
0x080485e8 <+1>:     mov    %esp,%ebp
0x080485ea <+3>:     sub    $0x28,%esp
```

- Standard function prologue.
- Allocates 0x28 bytes of local stack space.

---

### Initialize local int variable to 0

```asm
0x080485ed <+6>:     movl   $0x0,-0xc(%ebp)
```

- Initializes a local variable at `-0xc(%ebp)` to 0.
- Let’s call this variable `input`.

---

### Flush input stream

```asm
0x080485f4 <+13>:    mov    0x804a060,%eax      ; FILE * stream = stdout
0x080485f9 <+18>:    mov    %eax,(%esp)
0x080485fc <+21>:    call   fflush
```

- Flushes the stream (`stdout`).
- Ensures buffered output is printed before waiting for user input.

---

### Call `scanf("%u", &input)`

```asm
0x08048601 <+26>:    mov    $0x8048ad0,%eax     ; "%u"
0x08048606 <+31>:    lea    -0xc(%ebp),%edx     ; &input
0x08048609 <+34>:    mov    %edx,0x4(%esp)      ; 2nd argument
0x0804860d <+38>:    mov    %eax,(%esp)         ; 1st argument
0x08048610 <+41>:    call   __isoc99_scanf
```

- Uses `scanf("%u", &input)` to read an unsigned integer from user input.

---

### Clear stdin

```asm
0x08048615 <+46>:    call   clear_stdin
```

- Likely loops until newline is reached, clearing out any trailing garbage or extra input.

---

### Return the value

```asm
0x0804861a <+51>:    mov    -0xc(%ebp),%eax
0x0804861d <+54>:    leave
0x0804861e <+55>:    ret
```

- Returns the `unsigned int` that was read.

---

### Summary

```c
unsigned int get_unum(void) {
    unsigned int input = 0;
    fflush(stdout);
    scanf("%u", &input);
    clear_stdin();
    return input;
}
```

**Key points:**
- Only allows unsigned decimal input (`%u`).
- No bounds checking.
- The result is returned directly.

---

So this is our **input primitive**—any time we see a call to `get_unum()`, you can input a 32-bit unsigned integer (0 to 4,294,967,295).

