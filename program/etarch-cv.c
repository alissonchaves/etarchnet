/*
 * Library: libcrc
 * File:    examples/tstcrc.c
 * Author:  Lammert Bies
 *
 * This file is licensed under the MIT License as stated below
 *
 * Copyright (c) 1999-2016 Lammert Bies
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Description
 * -----------
 * The file tstx_crc.c contains a small sample program which demonstrates the
 * use of the functions for calculating the CRC-CCITT, CRC-16 and CRC-32 values
 * of data. The program calculates the three different CRC's for a file who's
 * name is either provided at the command line, or data typed in right the
 * program has started.
 */

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../include/checksum.h"

#define MAX_STRING_SIZE    2048

/*
 * int main( int argc, char *argv[] );
 *
 * The function main() is the entry point of the example program which
 * calculates several CRC values from the contents of files, or data from
 * stdin.
 */

int main(int argc, char *argv[]) {

    unsigned char *ptr;
    unsigned char prev_byte;
    uint16_t crc_16_val;
    uint16_t crc_16_modbus_val;
    uint16_t crc_ccitt_ffff_val;
    uint16_t crc_ccitt_0000_val;
    uint16_t crc_ccitt_1d0f_val;
    uint16_t crc_dnp_val;
    uint16_t crc_sick_val;
    uint16_t crc_kermit_val;
    uint32_t crc_32_val;
    uint16_t low_byte;
    uint16_t high_byte;


    if (argc > 1) {
        ptr = (unsigned char *) argv[1];
        while (*ptr && *ptr != '\r' && *ptr != '\n') ptr++;
        *ptr = 0;

        crc_16_val = 0x0000;
        crc_16_modbus_val = 0xffff;
        crc_dnp_val = 0x0000;
        crc_sick_val = 0x0000;
        crc_ccitt_0000_val = 0x0000;
        crc_ccitt_ffff_val = 0xffff;
        crc_ccitt_1d0f_val = 0x1d0f;
        crc_kermit_val = 0x0000;
        crc_32_val = 0xffffffffL;


        prev_byte = 0;
        ptr = (unsigned char *) argv[1];

        while (*ptr) {

            crc_16_val = update_crc_16(crc_16_val, *ptr);
            crc_16_modbus_val = update_crc_16(crc_16_modbus_val, *ptr);
            crc_dnp_val = update_crc_dnp(crc_dnp_val, *ptr);
            crc_sick_val = update_crc_sick(crc_sick_val, *ptr, prev_byte);
            crc_ccitt_0000_val = update_crc_ccitt(crc_ccitt_0000_val, *ptr);
            crc_ccitt_ffff_val = update_crc_ccitt(crc_ccitt_ffff_val, *ptr);
            crc_ccitt_1d0f_val = update_crc_ccitt(crc_ccitt_1d0f_val, *ptr);
            crc_kermit_val = update_crc_kermit(crc_kermit_val, *ptr);
            crc_32_val = update_crc_32(crc_32_val, *ptr);

            prev_byte = *ptr;
            ptr++;
        }


        crc_32_val ^= 0xffffffffL;

        crc_dnp_val = ~crc_dnp_val;
        low_byte = (crc_dnp_val & 0xff00) >> 8;
        high_byte = (crc_dnp_val & 0x00ff) << 8;
        crc_dnp_val = low_byte | high_byte;

        low_byte = (crc_sick_val & 0xff00) >> 8;
        high_byte = (crc_sick_val & 0x00ff) << 8;
        crc_sick_val = low_byte | high_byte;

        low_byte = (crc_kermit_val & 0xff00) >> 8;
        high_byte = (crc_kermit_val & 0x00ff) << 8;
        crc_kermit_val = low_byte | high_byte;

//		printf( "%s0x08", crc_32_val     );
//                printf( "%08" PRIX32 "%04" PRIX16, crc_32_val, crc_16_val     );
        char crca[3] = "E0";
        char crcb[20];
        char crc32[20];
        char crc16[20];
        sprintf(crc32, "%08" PRIX32, crc_32_val);
        sprintf(crc16, "%04" PRIX32, crc_16_val);
        strcat(crcb, &crc32[2]);
        strcat(crca, crcb);
        strcat(crca, crc16);

        printf("%s", crca);
//

    }

//	return 0;

}  /* main (tstcrc.c) */
