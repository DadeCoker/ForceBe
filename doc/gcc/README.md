使用gcc编译时，需要在链接文件中加入BabyOS的段

```C
/* Define output sections */
SECTIONS
{
  ......
  /* BabyOS Section -------------*/
  .driver_init :
  {
    . = ALIGN(4);
    PROVIDE(__start_driver_init = .);
    KEEP(*(SORT(.driver_init*)))
    PROVIDE(__stop_driver_init = .);
    . = ALIGN(4);
  } > FLASH

  .driver_init_0 :
  {
    . = ALIGN(4);
    PROVIDE(__start_driver_init_0 = .);
    KEEP(*(SORT(.driver_init_0*)))
    PROVIDE(__stop_driver_init_0 = .);
    . = ALIGN(4);
  } > FLASH

  .bos_polling :
  {
    . = ALIGN(4);
    PROVIDE(__start_bos_polling = .);
    KEEP(*(SORT(.bos_polling*)))
    PROVIDE(__stop_bos_polling = .);
    . = ALIGN(4);
  } > FLASH

  .b_mod_shell :
  {
    . = ALIGN(4);
    PROVIDE(__start_b_mod_shell = .);
    KEEP(*(SORT(.b_mod_shell*)))
    PROVIDE(__stop_b_mod_shell = .);
    . = ALIGN(4);
  } > FLASH

  .b_mod_state :
  {
    . = ALIGN(4);
    PROVIDE(__start_b_mod_state = .);
    KEEP(*(SORT(.b_mod_state*)))
    PROVIDE(__stop_b_mod_state = .);
    . = ALIGN(4);
  } > FLASH      
      
  /* BabyOS Section ---------end----*/
  ......
}
```

