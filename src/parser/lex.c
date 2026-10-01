#include <kit.h>

tok_t* lex_statement(proj_t* proj) {
    tok_t* tok = lex_identifier(proj);
    if (!tok) return NULL;
    tok_t statement = {0};
    switch (tok->type) {
    case func_tok:
        free(tok);
        tok_t* tok = lex_identifier(proj);
        if (!tok||tok->type!=identifier_tok) return error(proj, "identifier was expected");
        statement.file = tok->file;
        statement.ptr  = tok->ptr;
        statement.len  = tok->len;
        statement.type = func_holder;
        index_append((void***)&statement.tokens, tok);

        tok_t* paren = lex_operator(proj);
        if (!paren) return error(proj, "operator was expected");
        if (paren->type==parentheses_open) {
            
        }
        free(paren);
        
        break;
    
    default:
        break;
    }
    return (proj->last_tok=allocpy(&statement, sizeof(tok_t)));
}