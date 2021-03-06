#ifndef VTPARSE_TABLE_H
#define VTPARSE_TABLE_H
typedef enum {
   VTPARSE_STATE_ANYWHERE = 0,
   VTPARSE_STATE_CSI_ENTRY = 1,
   VTPARSE_STATE_CSI_IGNORE = 2,
   VTPARSE_STATE_CSI_INTERMEDIATE = 3,
   VTPARSE_STATE_CSI_PARAM = 4,
   VTPARSE_STATE_DCS_ENTRY = 5,
   VTPARSE_STATE_DCS_IGNORE = 6,
   VTPARSE_STATE_DCS_INTERMEDIATE = 7,
   VTPARSE_STATE_DCS_PARAM = 8,
   VTPARSE_STATE_DCS_PASSTHROUGH = 9,
   VTPARSE_STATE_ESCAPE = 10,
   VTPARSE_STATE_ESCAPE_INTERMEDIATE = 11,
   VTPARSE_STATE_GROUND = 12,
   VTPARSE_STATE_OSC_STRING = 13,
   VTPARSE_STATE_SOS_PM_APC_STRING = 14,
} vtparse_state_t2;
typedef unsigned char vtparse_state_t;

typedef enum {
   VTPARSE_ACTION_CLEAR = 1,
   VTPARSE_ACTION_COLLECT = 2,
   VTPARSE_ACTION_CSI_DISPATCH = 3,
   VTPARSE_ACTION_ESC_DISPATCH = 4,
   VTPARSE_ACTION_EXECUTE = 5,
   VTPARSE_ACTION_HOOK = 6,
   VTPARSE_ACTION_IGNORE = 7,
   VTPARSE_ACTION_OSC_END = 8,
   VTPARSE_ACTION_OSC_PUT = 9,
   VTPARSE_ACTION_OSC_START = 10,
   VTPARSE_ACTION_PARAM = 11,
   VTPARSE_ACTION_PRINT = 12,
   VTPARSE_ACTION_PUT = 13,
   VTPARSE_ACTION_UNHOOK = 14,
} vtparse_action_t2;
typedef unsigned char vtparse_action_t;

typedef unsigned char state_change_t;
extern state_change_t STATE_TABLE[15][256];
extern vtparse_action_t ENTRY_ACTIONS[15];
extern vtparse_action_t EXIT_ACTIONS[15];
extern char *ACTION_NAMES[15];
extern char *STATE_NAMES[15];

#endif //#ifndef VTPARSE_TABLE_H