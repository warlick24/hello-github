void showdata(char *buff, int datalen);
int eeprom_read(int which_chain, char addr, char *buf, char len);
int eeprom_write(int which_chain, char addr, char *buf, char len);
