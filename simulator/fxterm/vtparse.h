/*
 * VTParse - an implementation of Paul Williams' DEC compatible state machine parser
 *
 * Author: Joshua Haberman <joshua@reverberate.org>
 *
 * This code is in the public domain.
 */

#ifndef VTPARSE_H
#define VTPARSE_H
#include "vtparse_table.h"

#define MAX_INTERMEDIATE_CHARS 2
#define ACTION(state_change) (state_change & 0x0F)
#define STATE(state_change)  (state_change >> 4)

struct vtparse;

typedef struct _vtparse 
{
    vtparse_state_t    state;
    unsigned char      intermediate_chars[MAX_INTERMEDIATE_CHARS+1];
    char               ignore_flagged;
    int                params[16];
    int                num_params;
    void*              user_data;
} vtparse_t;

void vtparse_init(void);
void vtparse(unsigned char ch);


extern vtparse_t gParser;
void parser_callback(vtparse_action_t action, unsigned char ch);

#endif //#ifndef VTPARSE_H