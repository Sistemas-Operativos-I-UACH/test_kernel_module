#include <stdio.h>
#include <unistd.h>

/*

  Command execution example  


*/ 


int main(void) {
  int major = 420;
  char *binaryPath = "/bin/ls";   //  /usr/bin/mknod
  char *arg1 = "-lh";             //  c
  char arg2[20];                  //  major
  sprintf(arg2, "%d", major);
  printf("Execute: %s %s %s\n", binaryPath, arg1, arg2);
  execl(binaryPath, binaryPath, arg1, arg2, NULL);
 
  return 0;
}
