#include "eval.h"

// Function to find precedence of
// operators.
DT precedence(char op)
{
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    return 0;
}

// Function to perform arithmetic operations.
DT applyOp(DT a, DT b, char op)
{
    switch (op)
    {
    case '+':
        return a + b;
    case '-':
        return a - b;
    case '*':
        return a * b;
    case '/':
        return a / b;
    }
    return 0;
}

// Function that returns value of
// expression after evaluation.
DT evaluate(char *tokens)
{
    int i;

    Stack *values = init_stack(sizeof(DT));
    Stack *ops = init_stack(sizeof(char));
    int len = strlen(tokens);
    float rev_value = 1;

    for (i = 0; i < len; i++)
    {
        // Current token is a whitespace,
        // skip it.
        char current_char = tokens[i];
        if (current_char == ' ')
            continue;

        // Current token is an opening
        // brace, push it to ops stack
        else if (current_char == '(')
        {

            stack_push(ops, &current_char);
        }

        // Current token is a number, push
        // it to stack for numbers.
        else if (isdigit(tokens[i]))
        {
            DT val = 0;

            // There may be more than one
            // digits in number.
            while (i < len &&
                   isdigit(tokens[i]))
            {
                val = (val * 10) + (tokens[i] - '0');
                i++;
            }

            DT fract = 0;
            int fract_len = 1;
            if (tokens[i] == '.')
            {
                i++;
                while (i < len && isdigit(tokens[i]))
                {
                    fract = (fract * 10) + (tokens[i] - '0');
                    fract_len *= 10;
                    i++;
                }
            }

            val += fract / fract_len;
            val *= rev_value;
            rev_value = 1;

            stack_push(values, &val);

            i--;
        }

        // Closing brace encountered, solve
        // entire brace.
        else if (current_char == ')')
        {
            char top;
            stack_top(ops, &top);
            while (ops->size > 0 && top != '(')
            {
                DT val1, val2;
                char op;
                stack_pop(values, &val2);
                stack_pop(values, &val1);
                stack_pop(ops, &op);
                DT result = applyOp(val1, val2, op);
                stack_push(values, &result);
                stack_top(ops, &top);
            }

            // Pop opening brace.
            if (ops->size > 0)
                ops->size = ops->size - 1;
        }

        // Current token is an operator.
        else
        {
            if (tokens[i] == '-' && (i == 0 || tokens[i - 1] == '('))
            {
                rev_value = -1;
                continue;
            }

            char top;
            stack_top(ops, &top);
            while (ops->size > 0 && precedence(top) >= precedence(current_char))
            {
                DT val1, val2;
                char op;
                stack_pop(values, &val2);
                stack_pop(values, &val1);
                stack_pop(ops, &op);
                DT result = applyOp(val1, val2, op);
                stack_push(values, &result);
                stack_top(ops, &top);
            }

            // Push current token to 'ops'.
            stack_push(ops, &current_char);
        }
    }

    // Entire expression has been parsed at this
    // point, apply remaining ops to remaining
    // values.
    while (ops->size > 0)
    {
        DT val1, val2;
        char op;
        stack_pop(values, &val2);
        stack_pop(values, &val1);
        stack_pop(ops, &op);
        DT result = applyOp(val1, val2, op);
        stack_push(values, &result);
    }

    // Top of 'values' contains result, return it.
    DT result;
    stack_pop(values, &result);
    free(ops);
    free(values);
    return result;
}