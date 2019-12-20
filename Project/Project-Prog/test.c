#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "call.h"

int main(void)
{
  char path[] = "/dir5/dir1/file1";
  int a = open_t(path);
  printf("%d\n",a);
  return 0;
}
