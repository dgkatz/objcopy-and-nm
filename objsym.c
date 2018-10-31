#include <bfd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>

#define INT_HEXSTRING_LENGTH2 (sizeof(int)*CHAR_BIT/4)

static char const HEXDIGITS2[0x10] =
  {'0', '1', '2', '3', '4', '5', '6', '7',
   '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
void int_to_hexstring2(int value, char result[INT_HEXSTRING_LENGTH2+1])
{
  int i;
  result[INT_HEXSTRING_LENGTH2] = '\0';

  for(i=INT_HEXSTRING_LENGTH2-1; value; i--, value >>= 4) {
    int d  = value & 0xf;
    result[i] = HEXDIGITS2[d];
  }

  for(;i>=0;i--){ result[i] = '0'; }
}

void write_symbols(bfd *abfd)
{
  char buf[INT_HEXSTRING_LENGTH2+1];

  unsigned int storage_needed;
  asymbol **symbol_table;
  symbol_info symbol_info;
  long number_of_symbols;
  long i;

  storage_needed = bfd_get_symtab_upper_bound(abfd);

  symbol_table = (asymbol **) malloc(storage_needed);
  number_of_symbols = bfd_canonicalize_symtab(abfd, symbol_table);

  for (i = 0; i < number_of_symbols; i++)
  {
    bfd_symbol_info(symbol_table[i], &symbol_info);
    int symbol_value = symbol_info.value;
    int_to_hexstring2(symbol_value, buf);
    write(1, buf, strlen(buf));
    write(1, "\t", strlen("\t"));
    write(1, symbol_table[i]->name, strlen(symbol_table[i]->name));
    write(1, "\n", strlen("\n"));
    memset(&buf[0], 0, sizeof(buf));
  }
}
