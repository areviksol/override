### Disassembly of `main()`:

1. **Setup the Stack Frame**:
   ```asm
   0x08048879 <+0>:     push   %ebp
   0x0804887a <+1>:     mov    %esp,%ebp
   0x0804887c <+3>:     and    $0xfffffff0,%esp
   0x0804887f <+6>:     sub    $0x50,%esp
   ```
   - **`push %ebp`**: Save the old base pointer (for stack unwinding when the function returns).
   - **`mov %esp,%ebp`**: Set up the new base pointer for the current function's stack frame.
   - **`and $0xfffffff0,%esp`**: Align the stack to a 16-byte boundary (a common practice for performance).
   - **`sub $0x50,%esp`**: Allocate space on the stack for local variables (80 bytes).

2. **Move Argument into Local Variable**:
   ```asm
   0x08048882 <+9>:     mov    0xc(%ebp),%eax
   0x08048885 <+12>:    mov    %eax,0x1c(%esp)
   ```
   - **`mov 0xc(%ebp),%eax`**: Move the argument (likely a pointer or value passed to `main()`) into the `%eax` register.
   - **`mov %eax,0x1c(%esp)`**: Store the argument value (from `%eax`) into a specific location in the stack.

3. **Set Up for Further Function Calls**:
   ```asm
   0x08048889 <+16>:    mov    %gs:0x14,%eax
   0x0804888f <+22>:    mov    %eax,0x4c(%esp)
   0x08048893 <+26>:    xor    %eax,%eax
   0x08048895 <+28>:    push   %eax
   ```
   - **`mov %gs:0x14,%eax`**: This accesses the global segment register `%gs` and loads a value from offset `0x14` into `%eax`. This is commonly used to access the stack canary (a value that helps prevent buffer overflow exploits).
   - **`mov %eax,0x4c(%esp)`**: Store the value (stack canary) into a location on the stack.
   - **`xor %eax,%eax`**: Clear the `%eax` register (set it to zero).
   - **`push %eax`**: Push zero onto the stack. This could be for aligning arguments or clearing registers.

4. **Conditional Jump (`je`)**:
   ```asm
   0x08048896 <+29>:    xor    %eax,%eax
   0x08048898 <+31>:    je     0x804889d <main+36>
   ```
   - **`xor %eax,%eax`**: Again, zero out the `%eax` register.
   - **`je 0x804889d`**: If `%eax` is zero (it is), jump to the instruction at `0x804889d`. This is a short-circuit for skipping some code when `%eax` is zero.

5. **Call `puts` to Print Some Strings**:
   ```asm
   0x0804889d <+36>:    pop    %eax
   0x0804889e <+37>:    movl   $0x8048ad4,(%esp)
   0x080488a5 <+44>:    call   0x8048590 <puts@plt>
   ```
   - **`pop %eax`**: Pop the value from the stack into `%eax`.
   - **`movl $0x8048ad4,(%esp)`**: Load the address of a string into the stack (string at `0x8048ad4`).
   - **`call 0x8048590 <puts@plt>`**: Call `puts`, which will print the string at `0x8048ad4`.

6. **Call `puts` Again to Print Another String**:
   ```asm
   0x080488aa <+49>:    movl   $0x8048af8,(%esp)
   0x080488b1 <+56>:    call   0x8048590 <puts@plt>
   ```
   - Similar to the previous steps, this calls `puts` to print another string at `0x8048af8`.

7. **Call `puts` to Print Yet Another String**:
   ```asm
   0x080488b6 <+61>:    movl   $0x8048ad4,(%esp)
   0x080488bd <+68>:    call   0x8048590 <puts@plt>
   ```
   - This again prints a string at `0x8048ad4` using `puts`.

8. **Prepare Arguments for `printf`**:
   ```asm
   0x080488c2 <+73>:    mov    $0x8048b08,%eax
   0x080488c7 <+78>:    mov    %eax,(%esp)
   0x080488ca <+81>:    call   0x8048510 <printf@plt>
   ```
   - The string address `0x8048b08` is loaded into `%eax`, then passed as an argument to `printf`.

9. **Call `fgets` to Read Input**:
   ```asm
   0x080488d4 <+91>:    mov    0x804a060,%eax
   0x080488d8 <+95>:    mov    %eax,0x8(%esp)
   0x080488e0 <+103>:   lea    0x2c(%esp),%eax
   0x080488e4 <+107>:   mov    %eax,(%esp)
   0x080488e7 <+110>:   call   0x8048550 <fgets@plt>
   ```
   - This prepares for calling `fgets` to read input into the buffer at `0x2c(%esp)`.
   
10. **Print Another String Using `puts`**:
    ```asm
    0x080488ec <+115>:   movl   $0x8048ad4,(%esp)
    0x080488f3 <+122>:   call   0x8048590 <puts@plt>
    ```

11. **Check Authentication Result**:
    ```asm
    0x08048941 <+200>:   call   0x8048748 <auth>
    0x08048946 <+205>:   test   %eax,%eax
    0x08048948 <+207>:   jne    0x8048969 <main+240>
    ```
    - **`call 0x8048748 <auth>`**: Call the `auth` function to perform some authentication check.
    - **`test %eax,%eax`**: Check if the authentication was successful (i.e., `%eax` is non-zero).
    - **`jne 0x8048969`**: If the authentication is successful, jump to a section of the code that grants access to a shell.

12. **Call `puts` and `system` for Shell Access**:
    ```asm
    0x08048951 <+216>:   call   0x8048590 <puts@plt>
    0x08048956 <+221>:   movl   $0x8048b61,(%esp)
    0x0804895d <+228>:   call   0x80485a0 <system@plt>
    ```
    - **`call 0x8048590 <puts@plt>`**: Print a message indicating success.
    - **`movl $0x8048b61,(%esp)`**: Load the string `/bin/sh` into the stack (address `0x8048b61`).
    - **`call 0x80485a0 <system@plt>`**: Call `system("/bin/sh")`, which gives the attacker a shell.

---


## auth

---

## 🔍 Function Overview

The function prototype appears to be something like:

```c
int auth(char *input, int check_value);
```

It takes an input string (likely user-provided) and a second integer argument (`check_value`), and it returns 0 (success) or 1 (failure).

---

## 🧠 Step-by-Step Breakdown

### 🔹 Stack Setup

```asm
0x08048748 <+0>:     push   %ebp
0x08048749 <+1>:     mov    %esp,%ebp
0x0804874b <+3>:     sub    $0x28,%esp
```

Set up the stack frame and reserve 0x28 (40) bytes for local variables.

---

### 🔹 Sanitize input with `strcspn`

```asm
0x0804874e <+6>:     movl   $0x8048a63,0x4(%esp)  ; needle: "\n"
0x08048756 <+14>:    mov    0x8(%ebp),%eax        ; input string (arg1)
0x08048759 <+17>:    mov    %eax,(%esp)
0x0804875c <+20>:    call   strcspn
```

- `strcspn(input, "\n")` returns the offset of the newline character.
- Next:

```asm
0x08048761 <+25>:    add    0x8(%ebp),%eax
0x08048764 <+28>:    movb   $0x0,(%eax)
```

- Replaces the `\n` character with a null terminator (`\0`) — classic way to sanitize `fgets` input.

---

### 🔹 Get the length of the string (up to 0x20)

```asm
0x08048767 <+31>:    movl   $0x20,0x4(%esp)
0x0804876f <+39>:    mov    0x8(%ebp),%eax
0x08048772 <+42>:    mov    %eax,(%esp)
0x08048775 <+45>:    call   strnlen
0x0804877a <+50>:    mov    %eax,-0xc(%ebp)
```

- Store the length of the string into `-0xc(%ebp)` (let’s call this `len`).

---

### 🔹 If the string is too short (≤ 5), exit early

```asm
0x08048786 <+62>:    cmpl   $0x5,-0xc(%ebp)
0x0804878a <+66>:    jg     0x8048796 <auth+78>
0x0804878c <+68>:    mov    $0x1,%eax
0x08048791 <+73>:    jmp    0x8048877 <auth+303>
```

- If `len <= 5`, return 1 (failure).

---

### 🔹 Anti-debugging check via `ptrace`

```asm
0x080487b5 <+109>:   call   ptrace(PTRACE_TRACEME, 0, 0, 0)
0x080487ba <+114>:   cmp    $0xffffffff,%eax
0x080487bd <+117>:   jne    <skip_fail_msg>
```

- If `ptrace` fails (`== -1`), we’re **not** being traced — continue.
- If it **succeeds** (we're being debugged), it shows warning messages:

```asm
0x080487bf <+119>:   movl   $0x8048a68,(%esp) ; "Nope, you're being watched"
...
0x080487e3 <+155>:   mov    $0x1,%eax
```

So, if being debugged → display warning → return 1.

---

### 🔹 Compute a hash/ID from the 4th character

```asm
0x080487ed <+165>:   mov    0x8(%ebp),%eax      ; input
0x080487f0 <+168>:   add    $0x3,%eax           ; 4th byte
0x080487f3 <+171>:   movzbl (%eax),%eax         ; zero-extend byte
0x080487f6 <+174>:   movsbl %al,%eax            ; sign-extend
0x080487f9 <+177>:   xor    $0x1337,%eax
0x080487fe <+182>:   add    $0x5eeded,%eax
0x08048803 <+187>:   mov    %eax,-0x10(%ebp)    ; store as "accumulator"
```

→ This builds an **initial hash value** (`acc`) based on the 4th byte of the input.

---

### 🔁 Hash Loop

```asm
0x0804880d <+197>:   jmp    0x804885b <check_loop>
...
<check_loop>:
0x0804885b:   mov -0x14(%ebp), %eax             ; loop index (i)
0x0804885e:   cmp -0xc(%ebp), %eax              ; i < len
0x08048861:   jl  0x804880f                     ; jump to loop
```

### 🔁 Inside the Loop:

```asm
0x08048815:   movzbl (input+i), %eax            ; get input[i]
0x08048818:   cmp $0x1f, %al
0x0804881a:   jg <continue>
0x0804881c:   mov $0x1, %eax
0x08048821:   jmp <return>
```

- Rejects control characters (<= 0x1f)

Then:

```asm
0x0804882f:   mov %eax, %ecx
0x08048831:   xor -0x10(%ebp), %ecx             ; xor with accumulator
0x08048834:   mov $0x88233b2b, %edx
0x0804883b:   mul %edx                          ; obfuscated hash
...
0x08048854:   add %eax, -0x10(%ebp)             ; update accumulator
```

This is a hash-like function over each character, modifying the accumulator.

---

### ✅ Final Check

```asm
0x08048863:   mov 0xc(%ebp), %eax               ; expected value (arg2)
0x08048866:   cmp -0x10(%ebp), %eax             ; compare with accumulator
0x08048869:   je  <success>
0x0804886b:   mov $0x1, %eax                    ; fail
...
0x08048872:   mov $0x0, %eax                    ; success
```

So if the hash matches the second argument, return 0 (success). Otherwise, return 1 (fail).

---

## 🎯 Summary

### Arguments:

- `auth(char *input, int expected_hash)`
  - Cleans input, hashes it, compares to `expected_hash`.

### Protection:

- If input length ≤ 5 → reject
- If input has control characters → reject
- If being debugged → reject
- Uses obfuscated hash algorithm based on the 4th character and the rest of the string

### Return Values:

- ✅ 0 → success (input matches hash)
- ❌ 1 → failure

---


- Bypass this `auth()` by providing an input that matches a computed hash
- Or — more practically — **use a format string exploit or buffer overflow** to bypass `auth()`'s return value altogether and force `main()` to continue to `system("/bin/sh")`.
