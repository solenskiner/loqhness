/* copy over the next token from an input string, after
skipping leading blanks (or other whitespace?). The
token is terminated by the first appearance of tokchar,
or by the end of the source string.

The caller must supply sufficient space in token to
receive any token, Otherwise tokens will be truncated.

Returns: a pointer past the terminating tokchar.

This will happily return an infinity of empty tokens if
called with src pointing to the end of a string. Tokens
will never include a copy of tokchar.

A better name would be "strtkn", except that is reserved
for the system namespace. Change to that at your risk.

released to Public Domain, by C.B. Falconer.
Published 2006-02-20. Attribution appreciated.
*/

#include "misc_toksplit.h"

char * toksplit(char *string,  // Source of tokens
                     char delimier,        // token delimiting char
                     char *token,          // receiver of parsed token
                     int length)  {        // length token can receive not including final '\0' */
  if (string) {
    while (' ' == *string) *string++;

    while (*string && (delimier != *string)) {
      if (length) {
        *token++ = *string;
        --length;
      }
      string++;
    }
    if (*string && (delimier == *string)) string++;
  }
  *token = '\0';
  return string;
}
