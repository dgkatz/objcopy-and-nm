#include <bfd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#define target "elf64-x86-64"

void dump_text_section(bfd *abfd)
{
  asection *txt_section = bfd_get_section_by_name(abfd, ".text");
  char *buf;
  //bfd_byte *data = NULL;
  bfd_size_type size = bfd_section_size(abfd, txt_section);
    //size = bfd_get_section_size_before_reloc(txt_section);
  //size = txt_section->size;                                                                                        

  //buf = (char *) malloc(size);
  buf = malloc(size);
  if (!bfd_get_section_contents(abfd, txt_section, buf, (file_ptr) 0, size))
  {
    write(1, "could not find section .text", strlen("could not find section .text"));
  }else{
    write(1, "found .txt", strlen("found .txt"));
  }

  int textfiledesc = open("text-output.txt", O_CREAT | O_RDWR | O_APPEND);
  write(textfiledesc, buf, size);
}

int main(int argc, const char* argv[])
{
  bfd *abfd;
  bfd_init();
  abfd = bfd_openr(argv[1], target);
  bfd_check_format(abfd, bfd_object);
  dump_text_section(abfd);
}
