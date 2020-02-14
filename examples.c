#include <stdio.h>

int main(void) {
  printf("%d\n", (26) & 0x1f);
  printf("%d\n", 0x1f);
  printf("%c, %d\n", 'q', 'q');

  int val = 113 & 0x1f;
  printf("%d, %c\n", val, val);
}
