#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>

void showdata(char *buff, int datalen)
{
    int i = 0;
    printf("data:\n");
    for(i = 0, i < datalen, i++)
    {
        printf("0x%02x ", buff[i]);
        if((++i % 8 ) == 0)
        {
            printf("\n");
        }
    }
}


int eeprom_read(int which_chain, char addr, char *buf, char len)
{
    return 0;
}

int eeprom_write(int which_chain, char addr, char *buf, char len)
{
    return 0;
}