/* memory map */
MEMORY
{
    /* BOOTLOADER  (rx) : ORIGIN = 0x08000000, LENGTH = 0x00002000  /* 8 KB   */
    APPLICATION (rx) : ORIGIN = 0x08000000, LENGTH = 0x0000DC00  /* 55 KB  */
    RESERVED    (rx) : ORIGIN = 0x0800FC00, LENGTH = 0x00000400  /* 1 KB   */

    /* 8 KB total: 0x1F00 (7936 B) for data/bss, 0x0100 (256 B) for stack */
    RAM         (xrw): ORIGIN = 0x20000000, LENGTH = 0x00001F00  /* 7936 B */
    STACK       (xrw): ORIGIN = 0x20001F00, LENGTH = 0x00000100  /* 256 B  */
}

ENTRY(Reset_Handler)

SECTIONS
{
  /* ISR vectors */
  .vectors :
  {
    . = ALIGN(4);
    KEEP(*(.vectors))
    . = ALIGN(4);
    __Vectors_End  = .;
    __Vectors_Size = __Vectors_End - __gVectors;
  } >APPLICATION

  .text :
  {
    . = ALIGN(4);
    *(.text)
    *(.text*)
    *(.glue_7)
    *(.glue_7t)
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(4);
    /* the symbol '_etext' will be defined at the end of code section */
    _etext = .;
  } >APPLICATION

  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)
    *(.rodata*)
    . = ALIGN(4);
  } >APPLICATION

  .ARM.extab :
  {
     *(.ARM.extab* .gnu.linkonce.armextab.*)
  } >APPLICATION

  .ARM :
  {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } >APPLICATION

  .ARM.attributes :
  {
    *(.ARM.attributes)
  } >APPLICATION

  .preinit_array :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >APPLICATION

  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >APPLICATION

  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(.fini_array*))
    KEEP (*(SORT(.fini_array.*)))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >APPLICATION

  /* provide some necessary symbols for startup file to initialize data */
  _sidata = LOADADDR(.data);

  .data :
  {
    . = ALIGN(4);
    /* the symbol '_sdata' will be defined at the data section start */
    _sdata = .;
    *(.data)
    *(.data*)
    . = ALIGN(4);
    /* the symbol '_edata' will be defined at the data section end */
    _edata = .;
  } >RAM AT>APPLICATION

  . = ALIGN(4);
  .bss :
  {
    /* the symbol '_sbss' will be defined at the bss section start */
    _sbss = .;
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    /* the symbol '_ebss' will be defined at the bss section end */
    _ebss = .;
    __bss_end__ = _ebss;
  } >RAM

  . = ALIGN(8);
  PROVIDE ( end  = _ebss );
  PROVIDE ( _end = _ebss );

  /* Dedicated STACK region: size = LENGTH(STACK) = 0x100 (256 B) */
  .stack (NOLOAD) :
  {
    /* bottom of stack region = also end of heap, if you ever use it */
    . = ORIGIN(STACK);
    PROVIDE( _heap_end = . );

    /* top of stack (MSP reset value) */
    . = ORIGIN(STACK) + LENGTH(STACK);
    PROVIDE( _sp     = . );
    PROVIDE( _estack = . );
  } >STACK

  .RESERVED :
  {
    . = ALIGN(4);
    KEEP(*(.RESERVED))  /* If you define symbols there */
    . = ALIGN(4);
  } > RESERVED
}

/* input sections */
GROUP(libgcc.a libc.a libm.a libnosys.a)
