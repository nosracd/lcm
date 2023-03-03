/*
 * The utf8_check() function scans the '\0'-terminated string starting
 * at s. It returns a pointer to the first byte of the first malformed
 * or overlong UTF-8 sequence found, or NULL if the string contains
 * only correct UTF-8. It also spots UTF-8 sequences that could cause
 * trouble if converted to UTF-16, namely surrogate characters
 * (U+D800..U+DFFF) and non-Unicode positions (U+FFFE..U+FFFF). This
 * routine is very likely to find a malformed sequence if the input
 * uses any other encoding than UTF-8. It therefore can be used as a
 * very effective heuristic for distinguishing between UTF-8 and other
 * encodings.
 *
 * I wrote this code mainly as a specification of functionality; there
 * are no doubt performance optimizations possible for certain CPUs.
 *
 * Markus Kuhn <http://www.cl.cam.ac.uk/~mgk25/> -- 2005-03-30
 * License: http://www.cl.cam.ac.uk/~mgk25/short-license.html
 */

/*
 * Original file: http://www.cl.cam.ac.uk/~mgk25/ucs/utf8_check.c
 *
 * This source has been modified to scan a string of 'length' bytes,
 * and does not terminate on '\0'. However, there should probably be
 * a '\0' at s + length (luaL_checklstring ensures this).
 *
 * This source file has been modified slightly by Tim Perkins.
 * The same license (as above) applies to this modified source file.
 */

#include <stdlib.h>
#include "stdint.h"

int utf8_check(const char *s, size_t length)
{
    size_t i = 0;
    while (i < length) {
        const uint8_t byte0 = s[i];
        const uint8_t byte1 = s[i + 1];
        if (byte0 < 0x80) /* 0xxxxxxx */
            i++;
        else if ((byte0 & 0xe0) == 0xc0) {
            /* 110XXXXx 10xxxxxx */
            if (i + 1 >= length)
                return 0;
            if ((byte1 & 0xc0) != 0x80 || (byte0 & 0xfe) == 0xc0) /* overlong? */
                return 0;
            else
                i += 2;
        } else if ((byte0 & 0xf0) == 0xe0) {
            /* 1110XXXX 10Xxxxxx 10xxxxxx */
            if (i + 2 >= length)
                return 0;
            const uint8_t byte2 = s[i + 2];
            if ((byte1 & 0xc0) != 0x80 || (byte2 & 0xc0) != 0x80 ||
                (byte0 == 0xe0 && (byte1 & 0xe0) == 0x80) || /* overlong? */
                (byte0 == 0xed && (byte1 & 0xe0) == 0xa0) || /* surrogate? */
                (byte0 == 0xef && byte1 == 0xbf &&
                 (byte2 & 0xfe) == 0xbe)) /* U+FFFE or U+FFFF? */
                return 0;
            else
                i += 3;
        } else if ((byte0 & 0xf8) == 0xf0) {
            /* 11110XXX 10XXxxxx 10xxxxxx 10xxxxxx */
            if (i + 3 >= length)
                return 0;
            const uint8_t byte2 = s[i + 2];
            const uint8_t byte3 = s[i + 3];
            if ((byte1 & 0xc0) != 0x80 || (byte2 & 0xc0) != 0x80 ||
                (byte3 & 0xc0) != 0x80 ||
                (byte0 == 0xf0 && (byte1 & 0xf0) == 0x80) ||    /* overlong? */
                (byte0 == 0xf4 && byte1 > 0x8f) || byte0 > 0xf4) /* > U+10FFFF? */
                return 0;
            else
                i += 4;
        } else
            return 0;
    }

    return 1;
}
