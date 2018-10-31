#include <bfd.h>
#include "objsym.c"

#define target "elf64-x86-64"


int main(int argc, const char* argv[])
{
  bfd *abfd;
  bfd_init();
  abfd = bfd_openr(argv[1], target);
  bfd_check_format(abfd, bfd_object);
  write_symbols(abfd);
}
