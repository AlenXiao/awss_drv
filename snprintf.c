#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

struct test_type {
    uint16_t fc;
    uint16_t fc1;
    uint8_t addr1[6];
    uint8_t addr2[6];
    uint8_t addr3[6];
    uint16_t seq;
    uint8_t addr4[6];
};
    

int main()
{
    char pr_buf[sizeof(struct test_type) * 3 + 1];
    struct test_type ty = {0x00};
    char *data_buf = (char *)&ty;

    int i;
    int len = 0;
    memset(pr_buf, 0, sizeof(pr_buf));
    for (i = 0; i < sizeof(struct test_type); i ++) {
        len += snprintf(pr_buf + len, 4, "%02X ", data_buf[i]);
    }
    printf("len:%u, slen:%lu, %s\r\n", len, strlen(pr_buf), pr_buf);

    return 0;
}
