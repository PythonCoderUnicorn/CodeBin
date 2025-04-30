


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_GALAXY_NAME_LENGTH 20

// Function to process a Star Trek galaxy name.  Vulnerable to buffer overflow.
void process_galaxy_name(char *input_name) {
    char galaxy_name[MAX_GALAXY_NAME_LENGTH]; // Fixed-size buffer on the stack
    printf("Processing galaxy name: %s\n", input_name);

    // Vulnerability: strcpy does NOT check for buffer overflows!
    strcpy(galaxy_name, input_name);

    printf("Copied galaxy name: %s\n", galaxy_name);
    // Imagine further processing here... (e.g., storing in a database,
    // checking against a list of known galaxies, etc.)
}

int main() {
    // 1. Normal Case: Works as expected
    printf("\n--- Normal Case ---\n");
    process_galaxy_name("Milky Way");

    // 2. Vulnerability Case: Buffer Overflow
    printf("\n--- Vulnerability Case ---\n");
    process_galaxy_name("AndromedaGalaxyAndromedaGalaxyAndromeda"); // Input too long!

    printf("- - - break");

    // 3. User Input Case (with potential overflow)
    printf("\n--- User Input Case ---\n");
    char user_input[100];  // Larger buffer for user input
    printf("Enter a galaxy name: ");
    fgets(user_input, sizeof(user_input), stdin); // Use fgets for safer input
    // Remove the trailing newline character that fgets might add
    size_t len = strcspn(user_input, "\n");
    if (len < sizeof(user_input)) {
        user_input[len] = '\0';
    }
    process_galaxy_name(user_input);

    printf("\n--- End of Program ---\n");
    return 0;
}

/*
Explanation of the Vulnerability (Buffer Overflow)

The process_galaxy_name function is intended to copy a galaxy name into a local buffer called galaxy_name.

* galaxy_name is declared as a character array with a fixed size of MAX_GALAXY_NAME_LENGTH (20 characters in this example). This buffer is allocated on the stack.

* The strcpy function is used to copy the input_name into galaxy_name.

The Problem:

* strcpy does NOT perform any bounds checking. It blindly copies characters from the source (input_name) to the destination (galaxy_name) until it encounters a null terminator ('\0') in the source string.

* If the input_name is longer than MAX_GALAXY_NAME_LENGTH - 1 (to account for the null terminator), strcpy will write past the end of the galaxy_name buffer. This is a buffer overflow.

Consequences of a Buffer Overflow:

A buffer overflow can have serious consequences:

* Crash: The most common outcome is that the program crashes due to a segmentation fault or other memory error.
* Data Corruption: Overwriting memory can corrupt other data on the stack, leading to unpredictable program behavior.
* Code Execution: In the worst case, a malicious attacker can carefully craft an overly long input_name to overwrite the return address on the stack. This allows them to hijack the program's control flow and execute arbitrary code.  This is a major security vulnerability.

Star Trek Analogy:

Imagine the galaxy_name buffer as the cargo hold of a Starfleet shuttlecraft.

* Normal Operation: If the cargo (galaxy name) is small enough, it fits neatly into the cargo hold.
* Vulnerability: If the cargo (galaxy name) is too large, it spills out of the cargo hold, potentially damaging critical systems of the shuttlecraft (overwriting memory) or even causing it to crash (program crash).
* Malicious Attack: A clever enemy could use this overflow to smuggle in a program (malicious code) that takes control of the shuttlecraft (hijacks program execution).

How the Code Demonstrates the Vulnerability:

1.  Normal Case: When process_galaxy_name is called with "Milky Way", the name fits within the buffer, and the program behaves correctly.

2.  Vulnerability Case: When called with a very long string, strcpy overflows the galaxy_name buffer. This will likely cause a crash or corrupt memory.

3.  User Input Case: This shows how the vulnerability could be exploited with user-provided input. Even with fgets, if the input is too long, it can still cause issues if not handled carefully.

Mitigation:

* NEVER use strcpy!  Use strncpy instead.  strncpy takes a size argument to prevent overflows.
* Use fgets to read user input, as shown in the example, and handle the newline character.
* Carefully check the size of input data before copying it into a fixed-size buffer.
*/
