#include "s21_decimal.h"

// Converters to big decimal and back

s21_big_decimal to_big_decimal(s21_decimal value) {
    s21_big_decimal value_big = {0};
    value_big.bits[7] = value.bits[3];
    value_big.bits[2] = value.bits[2];
    value_big.bits[1] = value.bits[1];
    value_big.bits[0] = value.bits[0];
    return value_big;
}

int to_normal_decimal(s21_big_decimal value_big, s21_decimal *value) {
    int result = 0;
    int exp = get_exponent_big(value_big);
    s21_big_decimal ten = {{0b1010,0}};
    while (exp > 28) {
        div_prev(value_big, ten, &value_big);
        exp--;
    }
    while (!(value_big.bits[3] == 0 && value_big.bits[4] == 0 &&
             value_big.bits[5] == 0 && value_big.bits[6] == 0)) {
        if (exp == 0) {
            result = 1;
            break;
        } else {
            div_prev(value_big, ten, &value_big);
            exp--;
        }
    }
    value->bits[3] = value_big.bits[7];
    value->bits[2] = value_big.bits[2];
    value->bits[1] = value_big.bits[1];
    value->bits[0] = value_big.bits[0];
    return result;
}

// Helpers

int get_exponent_big(s21_big_decimal value) {
    long int pow = 1;
    int point = 0;
    value.bits[7] = value.bits[7] >> START_EXP;
    for (int i = 0; i < 8; i++) {
        point += (int)((value.bits[7] & 1) * pow);
        pow *= 2;
        value.bits[7] = value.bits[7] >> 1;
    }
    return point;
}

int equating_exponent_big(s21_big_decimal *value_1, s21_big_decimal *value_2) {
    s21_big_decimal ten = {{0b1010,0}};
    s21_big_decimal result_value_1= {0}, less_exp_value = *value_1;
    int ret = 0, stop = 0, useNumber = 0;
    int exp_1 = get_exponent_big(*value_1);
    int exp_2 = get_exponent_big(*value_2);
    int exp_diff = exp_2 - exp_1;
    if (exp_1 > exp_2) {
        less_exp_value = *value_2;
        exp_diff = exp_1 - exp_2;
        useNumber = 1;
    }
    for (; exp_diff > 0; exp_diff--){
        stop = multiply(&less_exp_value, &ten, &result_value_1);
        less_exp_value = result_value_1;
        if (stop) break;
    }
    if (useNumber) {
        *value_2 = result_value_1;
        duplicate_exponent_big(*value_1, value_2);

    } else {
        *value_1 = result_value_1;
        duplicate_exponent_big(*value_2, value_1);
    }
    if(stop)ret++;
    return ret;
}

void duplicate_exponent_big(s21_big_decimal src, s21_big_decimal *dst) {
    int minus = get_sign_big(*dst);
    dst->bits[7] = 0;
    for (int i = 240; i < 249; i++) {
        if (get_bit_big(src, i)) {
            set_bit_1_big(dst, i);
        }
    }
    if (minus) set_sign_1_big(dst);
}


int find_first_1_big(s21_big_decimal value) {
    int result = END_SIXTH_BYTE;
    for (; result > 0; result--) {
        if (get_bit_big(value, result)) break;
    }
    return result;
}

int is_null_big(s21_big_decimal value) {
    int ret = 0;
    if (value.bits[0] == 0 && value.bits[1] == 0 && value.bits[2] == 0 &&
        value.bits[3] == 0 && value.bits[4] == 0 && value.bits[5] == 0 &&
        value.bits[6] == 0 && value.bits[7] == 0) ret = 1;
    return ret;
}

// Comparison big

int is_equal_big(s21_big_decimal *value_1, s21_big_decimal *value_2) {
    int result = 0;
    if (get_sign_big(*value_1) == get_sign_big(*value_2)) {
        if (get_exponent_big(*value_1) != get_exponent_big(*value_2)) equating_exponent_big(value_1, value_2);
        if (value_1->bits[0] == value_2->bits[0] && value_1->bits[1] == value_2->bits[1] &&
            value_1->bits[2] == value_2->bits[2] && value_1->bits[3] == value_2->bits[3] &&
            value_1->bits[4] == value_2->bits[4] && value_1->bits[5] == value_2->bits[5] &&
            value_1->bits[6] == value_2->bits[6] && value_1->bits[7] == value_2->bits[7]) {
            result = 1;
        }
    }
    return result;
}

int is_less_big(s21_big_decimal value_1, s21_big_decimal value_2) {
    int result = 0;
    if (get_sign_big(value_1) && !get_sign_big(value_2)) {
        result = 1;
    } else if (!get_sign_big(value_1) && get_sign_big(value_2)) {
        result = 0;
    } else {
        if (get_exponent_big(value_1) != get_exponent_big(value_2)) equating_exponent_big(&value_1, &value_2);
        if (!is_equal_big(&value_1, &value_2)) {
            for (int i = END_SIXTH_BYTE; i >= 0; i--) {
                if (get_bit_big(value_1, i) < get_bit_big(value_2, i)) result = 1;
            }
        }
    }
    return result;
}

void set_exponent_big(s21_big_decimal *value, int exp) {
    int sign = get_sign_big(*value);
    value->bits[7] = 0;
    exp <<= 16;
    value->bits[7] = exp | value->bits[7];
    if (sign) set_sign_1_big(value);
}

