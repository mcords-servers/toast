#ifndef LEX_H
#define LEX_H

#include "kit.h"

bool lex_spaces(proj_t* proj);
tok_t* lex_operator(proj_t* proj);
tok_t* lex_identifier(proj_t* proj);

#endif