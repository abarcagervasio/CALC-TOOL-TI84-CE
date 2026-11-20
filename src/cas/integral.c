#include "cas.h"

/* Basic elementary antiderivatives (Note: '_' is minus in PineappleCAS string syntax) */
pcas_id_t id_integral[ID_NUM_INTEGRAL] = {
    {"intgrl(X,X)", "X^2/2"},
    {"intgrl(1/X,X)", "ln(abs(X))"},
    {"intgrl(X^_1,X)", "ln(abs(X))"},

    {"intgrl(e^X,X)", "e^X"},
    {"intgrl(ln(X),X)", "X*ln(X)_X"},

    {"intgrl(sin(X),X)", "_cos(X)"},
    {"intgrl(cos(X),X)", "sin(X)"},
    {"intgrl(tan(X),X)", "_ln(abs(cos(X)))"},

    {"intgrl(1/cos(X)^2,X)", "tan(X)"},
    {"intgrl(1/sin(X)^2,X)", "_1/tan(X)"},

    {"intgrl(1/(1+X^2),X)", "atan(X)"},
    {"intgrl(1/sqrt(1_X^2),X)", "asin(X)"},
};

/* Rules requiring constant checks */
pcas_id_t id_int_power_rule = {"intgrl(X^A,X)", "X^(A+1)/(A+1)"};
pcas_id_t id_int_const_mult = {"intgrl(C*A,X)", "C*intgrl(A,X)"};

/* Helper declared in derivative.c */
extern bool is_constant(pcas_ast_t *e, pcas_ast_t *respect_to);

/* Helper: checks if a constant exponent node equals -1 (integer or -1/1) */
static bool is_minus_one(pcas_ast_t *n) {
    if (is_ast_int(n, -1))
        return true;
    return false;
}

bool eval_integral_nodes(pcas_ast_t *e) {
    pcas_ast_t *expr, *respect_to;
    pcas_ast_t *child;
    bool changed = false;

    if (e->type != NODE_OPERATOR)
        return false;

    /* Recursively evaluate any inner integral nodes first */
    for (child = ast_ChildGet(e, 0); child != NULL; child = child->next)
        changed |= eval_integral_nodes(child);

    if (!isoptype(e, OP_INTEGRAL))
        return changed;

    expr = ast_ChildGet(e, 0);
    respect_to = ast_Copy(ast_ChildGet(e, 1));

    /* Simplify integrand first so roots like 4x√(X^3) become X^(3/4) */
    simplify(expr, SIMP_ALL);

    /* 1. Constant Rule: integral(C, x) = C * x */
    if (is_constant(expr, respect_to)) {
        pcas_ast_t *mult = ast_MakeOperator(OP_MULT);
        ast_ChildAppend(mult, ast_Copy(expr));
        ast_ChildAppend(mult, ast_Copy(respect_to));
        simplify(mult, SIMP_ALL);
        replace_node(e, mult);
        ast_Cleanup(respect_to);
        return true;
    }

    /* 2. Sum / Linearity Rule: integral(A + B, x) = integral(A, x) + integral(B, x) */
    if (isoptype(expr, OP_ADD)) {
        pcas_ast_t *sum = ast_MakeOperator(OP_ADD);
        for (child = ast_ChildGet(expr, 0); child != NULL; child = child->next) {
            pcas_ast_t *sub_int = ast_MakeOperator(OP_INTEGRAL);
            ast_ChildAppend(sub_int, ast_Copy(child));
            ast_ChildAppend(sub_int, ast_Copy(respect_to));
            ast_ChildAppend(sum, sub_int);
        }
        replace_node(e, sum);
        ast_Cleanup(respect_to);
        return true;
    }

    /* 3. Constant Multiple Rule: integral(C * f(x), x) = C * integral(f(x), x) */
    if (isoptype(expr, OP_MULT)) {
        pcas_ast_t *const_part = ast_MakeOperator(OP_MULT);
        pcas_ast_t *var_part = ast_MakeOperator(OP_MULT);
        bool has_const = false;
        bool has_var = false;

        for (child = ast_ChildGet(expr, 0); child != NULL; child = child->next) {
            if (is_constant(child, respect_to)) {
                ast_ChildAppend(const_part, ast_Copy(child));
                has_const = true;
            } else {
                ast_ChildAppend(var_part, ast_Copy(child));
                has_var = true;
            }
        }

        if (has_const && has_var) {
            pcas_ast_t *sub_int = ast_MakeOperator(OP_INTEGRAL);
            simplify(var_part, SIMP_ALL);
            ast_ChildAppend(sub_int, var_part);
            ast_ChildAppend(sub_int, ast_Copy(respect_to));

            ast_ChildAppend(const_part, sub_int);
            simplify(const_part, SIMP_COMMUTATIVE);
            replace_node(e, const_part);
            ast_Cleanup(respect_to);
            return true;
        } else {
            ast_Cleanup(const_part);
            ast_Cleanup(var_part);
        }
    }

    /* 4. Power Rule: integral(x^n, x) = x^(n+1)/(n+1) for integer & fractional constants n != -1 */
    if (isoptype(expr, OP_POW) &&
        ast_Compare(ast_ChildGet(expr, 0), respect_to) &&
        is_constant(ast_ChildGet(expr, 1), respect_to) &&
        !is_minus_one(ast_ChildGet(expr, 1))) {

        pcas_ast_t *n = ast_ChildGet(expr, 1);
        pcas_ast_t *n_plus_1 = ast_MakeOperator(OP_ADD);
        ast_ChildAppend(n_plus_1, ast_Copy(n));
        ast_ChildAppend(n_plus_1, ast_MakeNumber(num_FromInt(1)));
        simplify(n_plus_1, SIMP_ALL);

        pcas_ast_t *new_pow = ast_MakeBinary(OP_POW, ast_Copy(respect_to), ast_Copy(n_plus_1));
        pcas_ast_t *div = ast_MakeBinary(OP_DIV, new_pow, n_plus_1);
        simplify(div, SIMP_ALL);

        replace_node(e, div);
        ast_Cleanup(respect_to);
        return true;
    }

    /* 5. Table lookup for elementary functions (sin, cos, e^x, 1/x, etc.) */
    if (id_ExecuteTable(e, id_integral, ID_NUM_INTEGRAL, false)) {
        changed = true;
    }

    ast_Cleanup(respect_to);
    return changed;
}

void integral(pcas_ast_t *e, pcas_ast_t *respect_to) {
    pcas_ast_t *int_node = ast_MakeOperator(OP_INTEGRAL);

    ast_ChildAppend(int_node, ast_Copy(e));
    ast_ChildAppend(int_node, ast_Copy(respect_to));

    while (eval_integral_nodes(int_node));

    simplify(int_node, SIMP_ALL);

    /* Append "+ C" constant of integration */
    pcas_ast_t *with_c = ast_MakeOperator(OP_ADD);
    ast_ChildAppend(with_c, int_node);
    ast_ChildAppend(with_c, ast_MakeSymbol('C'));

    replace_node(e, with_c);
}
