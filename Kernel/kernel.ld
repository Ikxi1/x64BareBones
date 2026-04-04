OUTPUT_FORMAT(elf64-x86-64)
ENTRY(_loader)

SECTIONS
{
    . = 0x100000;

    /* Code */
    .text :
    {
        text = .;
        *(.text*)
    }

    /* Read-only data */
    . = ALIGN(0x1000);
    .rodata :
    {
        rodata = .;
        *(.rodata*)
    }

    /* Writable data */
    . = ALIGN(0x1000);
    .data :
    {
        data = .;
        *(.data*)
    }

    /* BSS (no file content) */
    . = ALIGN(0x1000);
    .bss :
    {
        bss = .;
        *(.bss*)
        *(COMMON)
    }

    . = ALIGN(0x1000);
    endOfKernel = .;
}