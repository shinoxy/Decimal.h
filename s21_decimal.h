#ifndef PRIV_DECIMAL_SRC_S21_DECIMAL_H_  // would be replaced on gitlab version
#define PRIV_DECIMAL_SRC_S21_DECIMAL_H_

#define START_EXP 16  // Beginning of the Exponent Bits
#define SIGN 31  // Sign Bits
#define END_FIRST_BYTE 31  // End of [0]
#define START_SECOND_BYTE 32  // End of [0]
#define END_THIRD_BYTE 96  // End of [3]
#define END_SIXTH_BYTE 192  // End of [6]

#include <math.h>
#include <stdlib.h>

typedef struct {
  unsigned int bits[4];
} s21_decimal;

typedef struct {
    unsigned int bits[8];
} s21_big_decimal;

// Bit operations for big decimal
s21_big_decimal set_to_null_big(s21_big_decimal value);
int get_bit_big(s21_big_decimal value, int pos);
int get_sign_big(s21_big_decimal value);
void set_sign_1_big(s21_big_decimal *value);
void set_sign_0_big(s21_big_decimal *value);
void set_bit_0_big(s21_big_decimal *value, int pos);
void set_bit_1_big(s21_big_decimal *value, int pos);
int move_left_big(s21_big_decimal *value);
void move_right_big(s21_big_decimal *value);

// Bit operations for normal decimal
s21_decimal set_to_null(s21_decimal value);
int get_bit(s21_decimal value, int pos);
int get_sign(s21_decimal value);
void set_sign_1(s21_decimal *value);
void set_sign_0(s21_decimal *value);
void set_bit_0(s21_decimal *value, int pos);
void set_bit_1(s21_decimal *value, int pos);
int move_left(s21_decimal *value);
void move_right(s21_decimal *value);

// Helpers for big decimal
s21_big_decimal to_big_decimal(s21_decimal value);
int to_normal_decimal(s21_big_decimal value_big, s21_decimal *value);
int get_exponent_big(s21_big_decimal value);
int equating_exponent_big(s21_big_decimal *value_1, s21_big_decimal *value_2);
void duplicate_exponent_big(s21_big_decimal src, s21_big_decimal *dst);
void set_exponent_big(s21_big_decimal *value, int exp);
int find_first_1_big(s21_big_decimal value);
int is_equal_big(s21_big_decimal *value_1, s21_big_decimal *value_2);
int is_less_big(s21_big_decimal value_1, s21_big_decimal value_2);
int is_null_big(s21_big_decimal value);

// Helpers for normal decimal
int get_exponent(s21_decimal value);
void set_exponent(s21_decimal *value, int exp);
int find_first_1(s21_decimal value);
int is_null(s21_decimal value);
float s21_rand_r(float a, float b);

// Arithmetics
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int plus(s21_big_decimal value_1, s21_big_decimal value_2, s21_big_decimal *result);

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int minus(s21_big_decimal value_1, s21_big_decimal value_2, s21_big_decimal *result);

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int multiply(s21_big_decimal *value_1, s21_big_decimal *value_2, s21_big_decimal *result);

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int div_prev(s21_big_decimal value_1, s21_big_decimal value_2, s21_big_decimal *result);
int segmentation(s21_big_decimal value_1, s21_big_decimal value_2, s21_big_decimal *result);
void round_segmentation(s21_big_decimal *number, int round);


// Comparison
int s21_is_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_less(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater(s21_decimal value_1, s21_decimal value_2);
int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2);

// Converters
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
//int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);
int s21_from_decimal_to_double(s21_decimal src, double *dst);

// Other functions
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

#endif // PRIV_DECIMAL_SRC_S21_DECIMAL_H_

