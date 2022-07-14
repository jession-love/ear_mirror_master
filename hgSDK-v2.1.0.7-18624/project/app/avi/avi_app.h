#ifndef __AVI_APP_H
#define __AVI_APP_H
void  avi_read_thread(void *d);
void avi_app_init();
void *avi_file_parse(char *filepath);
void avi_file_free(struct avifp *a_f);
void set_web_avi_running(void *d);
void  web_avi_read_thread(void *d);






#endif
