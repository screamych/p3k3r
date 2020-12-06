#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <arpa/inet.h>

#include "crc.h"
#include "z85.h"
/* Version of crc/z85 encode as 0 */
#define ver ")"

/* Error message */
static void usage()
{
  fprintf(stderr, "Usage: swp-encoder SWITCH-NAME PORT-NAME\n");
}

/* All switches/ports must be encode in lower case */
char* toLowerCase(const char *string)
{
  char *tmp = (char*) malloc(strlen(string) + 1);
  char *lc = tmp;
  
  while (*string != 0)
  {
    *tmp++ = tolower(*string++);
  }
  
  *tmp = 0; 
  return lc;
}

/* Concat switch and port for checksum */  
void strConcat(const char *swname, const char *portname, char *concat)
{
  strcpy(concat, swname);
  strcat(concat, portname);
}

/* Get rawbytes for z85_encode */
char* getRawbytes(uint32_t swcrc, uint32_t portcrc, uint32_t chksum)
{
  char *buf = malloc(32);

  memcpy(buf, &swcrc, sizeof(swcrc));
  memcpy(buf + 4, &portcrc, sizeof(portcrc));
  memcpy(buf + 8, &chksum, sizeof(chksum));

  return buf;
}

int main(int argc, const char *argv[])
{
  char enc85[64] = "\0";
  char concat[] = "\0";
  char *raw;
  const char *swname, *portname;
  uint32_t swcrc, portcrc, chksum;
  /* char *test = "\xcf\x06\x5e\xf2\x9c\x81\x98\xac\xe7\x43\x0b\x1f";*/

  if (argc != 3) 
  {
    usage();
    return -1;
  }

  swname   = argv[1];
  portname = argv[2];

  swname = toLowerCase(swname);
  portname = toLowerCase(portname);
  
  strConcat(swname, portname, concat);
  /* printf("%s\n%s\n%s\n", swname, portname, concat); */

  /* Calculate CRC32 for switch, port description and checksum */
  swcrc = htonl(crc32((uint8_t *)swname, strlen(swname)));
  portcrc = htonl(crc32((uint8_t *)portname, strlen(portname)));
  chksum = htonl(crc32((uint8_t *)concat, strlen(concat)));
  /* fprintf(stdout, "%x%x%x\n", swcrc, portcrc, chksum); */

  raw = getRawbytes(swcrc, portcrc, chksum);
  /* printf("[%s]\n[%s]\n", raw, test); */

  /* Encode p3k3r to z85 */
  Z85_encode(raw, enc85, 12);
  printf("%s%s\n", ver, enc85);

  free(raw);

  return 0;
}
