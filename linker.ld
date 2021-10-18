ENTRY(_start)

SECTIONS {
  . = 1M;

  .multiboot : {
    * (.multiboot)
  }

  .text : {
    * (.text)
  }
}
