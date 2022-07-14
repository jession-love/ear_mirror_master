#ifndef __PARSE_CMD_H
#define __PARSE_CMD_H
int get_arg_num(char *buf,char ***argv_r);
void arg_free(char **arg);

int str_isdigit(char *is);
int is_ip(char *ip);
#endif