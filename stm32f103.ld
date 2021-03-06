/* Linker script to configure memory regions. */
MEMORY {
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 0x10000  /* 64k */
    RAM (rw)   : ORIGIN = 0x20000000, LENGTH = 0x5000   /* 20k  */
}
 
 
/* Library configurations */
/* GROUP(libgcc.a libc.a libm.a libSTM32F10X_MD.a) */ 
 
/* Linker script to place sections and symbol values. Should be used together
 * with other linker script that defines memory regions FLASH and RAM.
 * It references following symbols, which must be defined in code:
 *   Reset_Handler : Entry of reset handler
 *
 * It defines following symbols, which code can use without definition:
 *   __exidx_start
 *   __exidx_end
 *   __etext
 *   __data_start__
 *   __preinit_array_start
 *   __preinit_array_end
 *   __init_array_start
 *   __init_array_end
 *   __fini_array_start
 *   __fini_array_end
 *   __data_end__
 *   __bss_start__
 *   __bss_end__
 *   __end__
 *   __HeapLimit
 *   __StackLimit
 *   __StackTop
 *   __stack
 */
ENTRY(Reset_Handler)
 
 
SECTIONS {
 
    .text : {
        KEEP(*(.isr_vector))
        
        *(.text)
        *(.text.*)
 
        KEEP(*(.init))
        KEEP(*(.fini))
 
        /* .ctors */
        *crtbegin.o(.ctors)
        *crtbegin?.o(.ctors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
        *(SORT(.ctors.*))
        *(.ctors)
 
        /* .dtors */
        *crtbegin.o(.dtors)
        *crtbegin?.o(.dtors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
        *(SORT(.dtors.*))
        *(.dtors)
 
        *(.rodata*)
 
        KEEP(*(.eh_frame*))
    } > FLASH
 
    .ARM.extab : {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASH
 
    __exidx_start = .;
    .ARM.exidx : {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > FLASH
    __exidx_end = .;
 
 
    _sidata = .;
    
    
    .data : AT ( _sidata ) {
        _sdata = .;
        
        *(vtable)
        *(.data*)
 
        . = ALIGN(4);
        /* preinit data */
        PROVIDE (__preinit_array_start = .);
        *(.preinit_array)
        PROVIDE (__preinit_array_end = .);
 
        . = ALIGN(4);
        /* init data */
        PROVIDE (__init_array_start = .);
        *(SORT(.init_array.*))
        *(.init_array)
        PROVIDE (__init_array_end = .);
 
 
        . = ALIGN(4);
        /* finit data */
        PROVIDE (__fini_array_start = .);
        *(SORT(.fini_array.*))
        *(.fini_array)
        PROVIDE (__fini_array_end = .);
 
        . = ALIGN(4);
        /* All data end */
        _edata = .;
 
    } > RAM
 
    .bss : {
        _sbss = .;
        *(.bss*)
        *(COMMON)
        _ebss = .;
    } > RAM
    
    .heap : {
        __HeapStart = .;
        *(.heap*)
        __HeapLimit = .;
    } > RAM
 
    /* .stack_dummy section doesn't contains any symbols. It is only
     * used for linker to calculate size of stack sections, and assign
     * values to stack symbols later */
    .stack : {
        *(.stack)
    } > RAM
 
    /* Set stack top to end of RAM, and stack limit move down by
     * size of stack_dummy section */
    __StackTop      = ORIGIN(RAM) + LENGTH(RAM);
    __StackLimit    = __StackTop - SIZEOF(.stack);
    _estack         = __StackTop;
 
    
    /* Check if data + heap + stack exceeds RAM limit */
    ASSERT(__StackLimit >= __HeapLimit, "RAM overflowed with stack")
}


