
SECTIONS
{
  . = ALIGN(4);
  .mem_section_dummy_ram :
  {
  }
  .b_hal_uart :
  {
    PROVIDE(__start_b_hal_uart = .);
    KEEP(*(SORT(.b_hal_uart*)))
    PROVIDE(__stop_b_hal_uart = .);
  } >RAM
}

SECTIONS
{
  . = ALIGN(4);
  .mem_section_dummy_rom :
  {
  }
  .driver_init :
  {
    PROVIDE(__start_driver_init = .);
    KEEP(*(SORT(.driver_init*)))
    PROVIDE(__stop_driver_init = .);
  } > FLASH
  .bos_polling :
  {
    PROVIDE(__start_bos_polling = .);
    KEEP(*(SORT(.bos_polling*)))
    PROVIDE(__stop_bos_polling = .);
  } > FLASH

  .b_mod_shell :
  {
    PROVIDE(__start_b_mod_shell = .);
    KEEP(*(SORT(.b_mod_shell*)))
    PROVIDE(__stop_b_mod_shell = .);
  } > FLASH
}
