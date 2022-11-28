ENTRY(_start)

SECTIONS {
  . = 1M;

  /* TODO seperate .multiboot is not working */
  .text : {
    * (.multiboot)
    * (.text)
  }
}
