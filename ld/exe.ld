ENTRY(__grimace_exe_start)
SECTIONS
{
    . = DEFINED(GRIMACE_TEXT_ADDRESS) ? GRIMACE_TEXT_ADDRESS : 0x11000;
    .text : { *(.text .text.*) }
    . = ALIGN(0x1000);
    .rdata : {
        *(.rodata .rodata.*)
        . = ALIGN(4);
        PROVIDE_HIDDEN(__init_array_start = .);
        KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE_HIDDEN(__init_array_end = .);
        . = ALIGN(4);
        PROVIDE_HIDDEN(__fini_array_start = .);
        KEEP(*(SORT_BY_INIT_PRIORITY(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE_HIDDEN(__fini_array_end = .);
        . = ALIGN(4);
        PROVIDE_HIDDEN(__ctors_priority_start = .);
        KEEP(*(SORT_BY_INIT_PRIORITY(.ctors.*)))
        PROVIDE_HIDDEN(__ctors_priority_end = .);
        . = ALIGN(4);
        PROVIDE_HIDDEN(__ctors_start = .);
        KEEP(*(.ctors))
        PROVIDE_HIDDEN(__ctors_end = .);
        . = ALIGN(4);
        PROVIDE_HIDDEN(__dtors_priority_start = .);
        KEEP(*(SORT_BY_INIT_PRIORITY(.dtors.*)))
        PROVIDE_HIDDEN(__dtors_priority_end = .);
        . = ALIGN(4);
        PROVIDE_HIDDEN(__dtors_start = .);
        KEEP(*(.dtors))
        PROVIDE_HIDDEN(__dtors_end = .);
    }
    . = ALIGN(0x1000);
    .data : { *(.iat) *(SORT_BY_NAME(.iat.*)) *(.data .data.* .sdata .sdata.*) }
    .bss (NOLOAD) : { *(.sbss .sbss.* .bss .bss.* COMMON) }
    /DISCARD/ : { *(.MIPS.abiflags) *(.reginfo) *(.pdr) *(.gnu.attributes) *(.mdebug.*) *(.rel.pdr) *(.ARM.exidx*) *(.ARM.extab*) *(.ARM.attributes) *(.comment) *(.eh_frame) *(.note*) }
}
