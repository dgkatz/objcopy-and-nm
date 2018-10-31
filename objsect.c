#include <bfd.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>

#define INT_HEXSTRING_LENGTH3 (sizeof(int)*CHAR_BIT/4)

static char const HEXDIGITS3[0x10] =
  {'0', '1', '2', '3', '4', '5', '6', '7',
   '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
void int_to_hexstring3(int value, char result[INT_HEXSTRING_LENGTH3+1])
{
  int i;
  result[INT_HEXSTRING_LENGTH3] = '\0';

  for(i=INT_HEXSTRING_LENGTH3-1; value; i--, value >>= 4) {
    int d  = value & 0xf;
    result[i] = HEXDIGITS3[d];
  }

  for(;i>=0;i--){ result[i] = '0'; }
}

void write_section(bfd *abfd, asection *section, void *obj)
{
  char buf[INT_HEXSTRING_LENGTH3+1];

  write(1,section->name,strlen(section->name));
  write(1,"\n\t VMA: ", strlen("\n\t VMA: "));

  int vma = bfd_get_section_vma(abfd, section); 
  int_to_hexstring3(vma, buf);
  write(1, buf, strlen(buf));
  write(1,"\n\t Size: ", strlen("\n\t Size: "));

  memset(&buf[0], 0, sizeof(buf));
  
  int size = bfd_section_size(abfd, section);
  int_to_hexstring3(size, buf);
  write(1, buf, strlen(buf));
  write(1,"\n\t Position: ", strlen("\n\t Position: "));
  
  memset(&buf[0], 0, sizeof(buf));

  int position = section->filepos;
  int_to_hexstring3(position, buf);
  write(1, buf, strlen(buf));
  write(1,"\n", strlen("\n"));

}

void write_sections(bfd *abfd)
{
  // iterates over each section in abfd, calling write_section on each
  bfd_map_over_sections(abfd, write_section, NULL);
}
