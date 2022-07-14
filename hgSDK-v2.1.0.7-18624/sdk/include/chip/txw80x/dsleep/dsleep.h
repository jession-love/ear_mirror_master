#ifndef _DSLEEP_H_
#define _DSLEEP_H_
#include <typesdef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DSLEEP_RO_START                     ((uint32)&__dsleep_ro_start__)
#define DSLEEP_CODE_START                   ((uint32)&__dsleep_code_start__)
#define DSLEEP_CODE_END                     ((uint32)&__dsleep_code_end__)
#define DSLEEP_CODE_SIZE                    (DSLEEP_CODE_END - DSLEEP_CODE_START)

extern uint32 __dsleep_ro_start__;
extern uint32 __dsleep_code_start__;
extern uint32 __dsleep_code_end__;

void rf_dsleep_recover(void);
void dsleep_enter(struct lmac_ops *ops);
void dsleep_wakeup(void);

#ifdef __cplusplus
}
#endif

#endif