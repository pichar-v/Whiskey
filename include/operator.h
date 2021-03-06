#ifndef OPERATOR_H_
# define OPERATOR_H_

/**
 * @defgroup Operator Operator
 * @{
 */

/**
 * An enumeration of the operators
 */
typedef enum {
  wsky_Operator_COLON,
  wsky_Operator_SEMICOLON,

  wsky_Operator_COMMA,
  wsky_Operator_DOT,

  wsky_Operator_LEFT_PAREN,
  wsky_Operator_RIGHT_PAREN,

  wsky_Operator_LEFT_BRACKET,
  wsky_Operator_RIGHT_BRACKET,

  wsky_Operator_LEFT_BRACE,
  wsky_Operator_RIGHT_BRACE,

  wsky_Operator_LT,
  wsky_Operator_GT,

  wsky_Operator_LT_EQ,
  wsky_Operator_GT_EQ,

  wsky_Operator_EQUALS,
  wsky_Operator_NOT_EQUALS,

  wsky_Operator_PLUS,
  wsky_Operator_MINUS,
  wsky_Operator_STAR,
  wsky_Operator_SLASH,

  wsky_Operator_PLUS_EQ,
  wsky_Operator_MINUS_EQ,
  wsky_Operator_STAR_EQ,
  wsky_Operator_SLASH_EQ,

  wsky_Operator_AND,
  wsky_Operator_OR,
  wsky_Operator_NOT,

  wsky_Operator_ASSIGN,

  wsky_Operator_AT,
} wsky_Operator;


/**
 * Returns the string of the operator.
 */
const char *wsky_Operator_toString(wsky_Operator operator);

/**
 * @}
 */

#endif /* !OPERATOR_H_ */
