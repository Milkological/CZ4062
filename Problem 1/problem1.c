#include<stdio.h>
#include<stdint.h>
#include<string.h>
#include<unistd.h>
#include<assert.h>
#include<stdlib.h>

#define CHUNK 1024

int j;

int main(int argc, char ** argv) {
    char buf[CHUNK];

    if (argc != 2) {
        fprintf(stderr, "usage: %s file", argv[0]);
        exit(1);
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("fopen");
        exit(2);
    }

    size_t nread = fread(buf, 1, sizeof buf, file);
    if (nread > 0) {
        if (nread > 16) {
            switch(buf[8]) {
                case 0x41: // 'A'
                    printf("CASE_01 [nread>16] crashing: null pointer dereference\n");
                    int *b = NULL;
                    *b = 99;
                    break;
                case 0x61: // 'a'
                    printf("CASE_02 [nread>16] timeout\n");
                    sleep(2);
                    break;
                case 0x2A: // '*'
                    printf("CASE_03 [nread>16] assert_failure: \n");
                    assert(0);
                    break;
                case 0x3F: // '?'
                    if (buf[7] == 0x3C) { // '<'
                        uint8_t j = 0, k = 0;
                        uint8_t limit = buf[7];
                        for (j=0; j < limit; j++) {
                            k++;
                        }
                        printf("CASE_04 [nread>16] loop: k=%d\n", k);
                        exit(0);
                    } else if (buf[7] > 0x64) {
                        printf("CASE_04 [nread>16] exit 6\n");
                        exit(6);
                    } else {
                        if ((buf[6] + buf[7]) == 0x64) {
                            printf("CASE_05 [nread>16] equality condition\n");
                            exit(0);
                        } else {
                            printf("CASE_06 [nread>16] inequality condition\n");
                            exit(0);
                        }
                    }
                    break;
                case 0x77:
                    if (strcmp(buf+16, "american fuzzy lop") == 0) {
                        printf("CASE_07 Oh no, I've been caught, excellent!\n");
                        abort();
                    } else if (strcmp(buf+16, "afl") == 0) {
                        printf("CASE_08 oops, I surrender\n");
                        abort();
                    } else  {
                        printf("CASE_09 nothing found\n");
                    }
                    break;
                case 0x7F:
                    if (buf[2048]) {
                        printf("CASE_10 can you catch me?\n");
                    } else {
                        printf("CASE_11 and me?\n");
                    }
                    ;
                    printf("CASE_12 so you've survived others, Here is a bonus value: %d\n", 12 / (buf[8] - 0x7F));
                    break;
                default:
                    printf("CASE_13 [nread>16] common\n");
                    break;
            }
        } else {
            switch(buf[8]) {
                case 0x41: // 'A'
                    printf("CASE_14 [nread<=16] crashing: null pointer dereference\n");
                    int *b = NULL;
                    *b = 99;
                    break;
                case 0x61: // 'a'
                    printf("CASE_15 [nread<=16] timeout\n");
                    sleep(2);
                    break;
                case 0x2A: // '*'
                    printf("CASE_16 [nread<=16] assert_failure: \n");
                    assert(0);
                    break;
                case 0x3F: // '?'
                    if (buf[7] == 0x3C) { // '<'
                        uint8_t j = 0, k = 0;
                        uint8_t limit = buf[7];
                        for (j=0; j < limit; j++) {
                            k++;
                        }
                        printf("CASE_17 [nread<=16] loop: k=%d\n", k);
                        exit(0);
                    } else if (buf[7] > 0x64) {
                        printf("CASE_18 [nread<=16] exit 6\n");
                        exit(6);
                    } else {
                        if ((buf[6] + buf[7]) == 0x64) {
                            printf("CASE_19 [nread<=16] equality condition\n");
                            exit(0);
                        } else {
                            printf("CASE_20 [nread<=16] inequality condition\n");
                            exit(0);
                        }
                    }
                    break;
                default:
                    printf("CASE_21 [nread<16] common\n");
                    break;
            }
        }
    }
    return 0;
}
