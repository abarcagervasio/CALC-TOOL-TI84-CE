#ifndef INTEGRAL_H_
#define INTEGRAL_H_

#include "identities.h"

#define ID_NUM_INTEGRAL 12
extern pcas_id_t id_integral[ID_NUM_INTEGRAL];

extern pcas_id_t id_int_power_rule;
extern pcas_id_t id_int_const_mult;

/* Evaluates all OP_INTEGRAL nodes in the AST */
bool eval_integral_nodes(pcas_ast_t *e);

/* Wraps expression e in an OP_INTEGRAL node with respect to variable x and solves it */
void integral(pcas_ast_t *e, pcas_ast_t *respect_to);

#endif