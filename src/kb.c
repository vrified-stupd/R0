#include "descriptorTables.h"
#include "isr.h"
#include "string.h"
#include "vga.h"
#include "kb.h"
#include <stdint.h>
#include <stdbool.h>
#include "string.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

static bool shiftPressed = false;
static bool capsLockEnabled = false;
static bool ctrlPressed = false;
static bool mute = false;


static char inputBuffer[MAX_INPUT_LENGTH];
static int inputIndex = 0;

static volatile bool inputReady = false;

// Regular Chars
static char kb[] =
{
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', 
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',   
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',          
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,            
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0                       
};

// Chars for when using Shift/Caps Lock
static char kbShift[] = 
{
    0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
    0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void keyboardHandler() 
{
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);

    // Handle shift, caps lock, and control key state changes
    if (scancode == LSHIFT || scancode == RSHIFT) {          // Check for Shift press. Makes handler switch from kb to kbShift
        shiftPressed = true;
    } else if (scancode == LSHIFTR || scancode == RSHIFTR) { // Check for Shift release. Makes handler switch from kbShift to kb
        shiftPressed = false;
    } else if (scancode == CAPSLCK) {                        // Caps Lock toggle
        capsLockEnabled = !capsLockEnabled; 
    } else if (scancode == CTRL) {                           // Check for Ctrl press. Does nothing for now
        ctrlPressed = true;
    } else if (scancode == CTRLR) {                          // Check for Ctrl release. Does nothing for now
        ctrlPressed = false;
    } else if (scancode == F1) {                             // Check for F1 press. Does nothing for now
        print("F1");
    } else if (scancode == F2) {                             // Check for F2 press. Does nothing for now
        print("F2");
    } else if (scancode == F3) {                             // Check for F3 press. Does nothing for now
        print("F3");
    } else if (scancode == F4) {                             // Check for F4 press. Does nothing for now
        print("F4");
    } else if (scancode == F5) {                             // Check for F5 press. Does nothing for now
        print("F5");
    } else if (scancode == F6) {                             // Check for F6 press. Does nothing for now
        print("F6");
    } else if (scancode == F7) {                             // Check for F7 press. Does nothing for now
        print("F7");
    } else if (scancode == F8) {                             // Check for F8 press. Does nothing for now
        print("F8");
    } else if (scancode == F9) {                             // Check for F9 press. Does nothing for now
        print("F9");
    } else if (scancode == F10) {                            // Check for F10 press. Does nothing for now
        print("F10");
    } else if (scancode == F11) {                            // Check for F11 press. Does nothing for now
        print("F11");
    } else if (scancode == F12) {                            // Check for F12 press. Does nothing for now
        print("F12");
    } else if (scancode == ESC) {                            // Check for ESC press. Does nothing for now
        print("ESC");
    } else if (scancode < 0x80) {                            // Regular key press
        char ascii;

        if (shiftPressed) {
            ascii = kbShift[scancode];
        } else if (capsLockEnabled && (kb[scancode] >= 'a' && kb[scancode] <= 'z')) {
            ascii = kbShift[scancode]; // Use uppercase for alphabets
        } else {
            ascii = kb[scancode];
        }

        if (ascii) {
            if (ascii == '\n') { // Enter key
                inputReady = true;
                inputBuffer[inputIndex] = '\0'; // Null-terminate the input
                print("\n"); // Move to the next line on screen
            } else if (ascii == '\b') { // Backspace key
                if (inputIndex > 0) {
                    inputIndex--;
                    inputBuffer[inputIndex] = '\0';
                    print("\b \b"); // Handle backspace on the screen
                }
            } else if (inputIndex < MAX_INPUT_LENGTH - 1) { // Regular character
                inputBuffer[inputIndex++] = ascii;
                char str[2] = { ascii, '\0' };
                print(str); // Echo the character
            }
        }
    }
}

void init_keyboard() 
{
    register_interrupt_handler(IRQ1, keyboardHandler); // Register KB
    print("KBD Initialized\n");
}

void scanf(char* buffer, int maxLength) {
    inputReady = false;
    inputIndex = 0;

    // Wait until the user presses Enter
    while (!inputReady);

    // Copy the input to the provided buffer
    for (int i = 0; i < maxLength - 1 && inputBuffer[i] != '\0'; i++) {
        buffer[i] = inputBuffer[i];
    }

    // Null-terminate the user's buffer
    buffer[maxLength - 1] = '\0';
}