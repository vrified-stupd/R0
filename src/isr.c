#include <stdint.h>
#include "isr.h"
#include "vga.h"
#include <stdbool.h>

bool badInterrupt = false;
isr_t interrupt_handlers[256];

void register_interrupt_handler(uint8_t n, isr_t handler)
{
    interrupt_handlers[n] = handler;
}

// This gets called from our ASM interrupt handler stub.
void isr_handler(registers_t regs)
{
    if (interrupt_handlers[regs.int_no] != 0)
    {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(regs);
    }
    else
    {
        if(regs.int_no != 6)
        {
            print("unhandled interrupt: ");
            printInt(regs.int_no);
            print("\n");
        }
        else
        {
            if(!badInterrupt)
                print("ew.\n");
            badInterrupt = true;
        }
    }
}

// This gets called from our ASM interrupt handler stub.
void irq_handler(registers_t regs)
{
    // Send an EOI (end of interrupt) signal to the PICs.
    // If this interrupt involved the slave.
    if (regs.int_no >= 40)
    {
        // Send reset signal to slave.
        outb(0xA0, 0x20);
    }
    // Send reset signal to master. (As well as slave, if necessary).
    outb(0x20, 0x20);

    if (interrupt_handlers[regs.int_no] != 0)
    {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(regs);
    }

}