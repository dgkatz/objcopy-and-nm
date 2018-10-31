#include <bfd.h>
#include <dlfcn.h>
#include "objsym.c"

#define rdtsc(var)                                              \
  {                                                             \
  int var##_lo, var##_hi;                                     \
  asm volatile("rdtsc" : "=a"(var##_lo), "=d"(var##_hi));     \
  var = var##_hi;                                             \
  var <<= 32;                                                 \
  var |= var##_lo;                                            \
  }
#define target "elf64-x86-64"
#define INT_HEXSTRING_LENGTH (sizeof(int)*CHAR_BIT/4)

static char const HEXDIGITS[0x10] =
  {'0', '1', '2', '3', '4', '5', '6', '7',
   '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
void int_to_hexstring(int value, char result[INT_HEXSTRING_LENGTH+1])
{
  int i;
  result[INT_HEXSTRING_LENGTH] = '\0';

  for(i=INT_HEXSTRING_LENGTH-1; value; i--, value >>= 4) {
    int d  = value & 0xf;
    result[i] = HEXDIGITS[d];
  }

  for(;i>=0;i--){ result[i] = '0'; }
}

int main(int argc, const char* argv[])
{
  int statfiledesc = open("stats.txt", O_CREAT | O_RDWR | O_APPEND);
  float times[50];
  write(statfiledesc, "GETSYMS\n", strlen("GETSYMS\n"));
  write(statfiledesc, argv[2], strlen(argv[2]));
  int i;
  char buf[INT_HEXSTRING_LENGTH+1];
  bfd *abfd;
  bfd_init();
  abfd = bfd_openr(argv[1], target);
  bfd_check_format(abfd, bfd_object);
  for (i = 0; i < 50; i++)
    {
      unsigned long long start, finish;
      void* lib_handle;
      int mode = (strcmp(argv[2], "RTLD_LAZY") != 0) ? RTLD_NOW : RTLD_LAZY;
      void (*write_symbols)(bfd * abfd);

      rdtsc(start);
      lib_handle = dlopen("./libobjdata.so", mode);
      if (!lib_handle) {
	return 0;
      }
      rdtsc(finish);
      int timestat = (finish-start)/2600;
      times[i] = timestat;
      int_to_hexstring(timestat, buf);
      write(statfiledesc, "\n", strlen("\n"));
      write(statfiledesc, buf, strlen(buf));

      write_symbols = dlsym(lib_handle, "write_symbols");
      (*write_symbols)(abfd);
      memset(&buf[0], 0, sizeof(buf));
      dlclose(lib_handle);
    }
  write(statfiledesc, "\nAVG: ", strlen("\nAVG: "));
  int avg = timestataverage1(times);
  int_to_hexstring(avg, buf);
  write(statfiledesc, buf, strlen(buf));
}

int timestataverage1(float timestats[50]){
  int i;
  float sum = 0;
  float average = 0;
  for(i = 0; i < 50; i++){
    sum = sum + timestats[i];
  }
  average = sum/50;

  return average;
}
