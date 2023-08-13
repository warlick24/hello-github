#include <stdio.h>

int main(void)
{
  int i = 0, j = 0;
  char databuff[128];
  
  printf("hello world!\n");
  printf("have a nice day\n");  

  for(i = 0, i < sizeof(databuff), i++)
  {
      databuff[i] = i;
  }

  return 0;
}
