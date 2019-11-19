#include <string.h>
// code borrowed from editline/examples/fileman.c

int is_space(char ch)
{
  return ((ch == ' ') || (ch == '\t') || (ch == '\n') || (ch == '\r') || (ch == 0));
}


char *strip_whitespace (char *string)
{
   register char *s, *t;

   for (s = string; is_space (*s); s++);

   if (*s == 0)
      return (s);

   t = s + strlen (s) - 1;
   while (t > s && is_space (*t))
      t--;
   *++t = '\0';

   return s;
}
