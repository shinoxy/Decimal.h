#include "s21_decimal.h"

int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
    s21_big_decimal value_1_big = to_big_decimal(value_1);
    s21_big_decimal value_2_big = to_big_decimal(value_2);
    return is_equal_big(&value_1_big, &value_2_big);
}

int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2) {
    return !s21_is_equal(value_1, value_2);
}

int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
    s21_big_decimal value_1_big = to_big_decimal(value_1);
    s21_big_decimal value_2_big = to_big_decimal(value_2);
    int result = is_less_big(value_1_big, value_2_big);
    return result;
}

int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
    return !s21_is_less(value_1, value_2);
}

int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2) {
    int result = 0;
    if (s21_is_less(value_1, value_2) || s21_is_equal(value_1, value_2)) result = 1;
    return result;
}

int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2) {
    int result = 0;
    if (s21_is_greater(value_1, value_2) || s21_is_equal(value_1, value_2)) result = 1;
    return result;
}