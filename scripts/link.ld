OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386:i386)

ENTRY(_start)

SECTIONS {
  . = 1M;
  kernel_start = .;

  .multiboot BLOCK(4K) : ALIGN(4K) {
    *(.multiboot .multiboot.*)
  }
  .text BLOCK(4K) : ALIGN(4K)  {
    *(.text .text.*)
  }
  .rodata BLOCK(4K) : ALIGN(4K) {
    *(.rodata .rodata.*)
  }
  .data BLOCK(4K) : ALIGN(4K) {
    *(.data .data.*)
  }
  .bss BLOCK(4K) : ALIGN(4K) {
    *(COMMON)
    *(.bss .bss.*)
  }

  /DISCARD/ : {
    *(.eh_frame)
    *(.note .note.*)
  }

  kernel_end = .;
}