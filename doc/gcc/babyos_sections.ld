/**
修改链接文件，添加BabyOS需要的Section信息。
可参考例程仓库 full_func_test_gcc 分支的链接文件
*/
##需要添加到.data段信息：

PROVIDE(__start_b_hal_uart = .);
KEEP(*(SORT(.b_hal_uart*)))
PROVIDE(__stop_b_hal_uart = .);


##在链接文件再添加如下信息：
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

  .b_hal_gpio :
  {
    PROVIDE(__start_b_hal_gpio = .);
    KEEP(*(SORT(.b_hal_gpio*)))
    PROVIDE(__stop_b_hal_gpio = .);
  } > FLASH

}
