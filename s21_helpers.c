#include "s21_decimal.h"

double s21_pow(double base, double e) {
    double result = 1.0;
    for (int i = 0; i < e; i++) result *= base;
    return result;
}

int get_exponent(s21_decimal value) {
    long int pow = 1;
    int point = 0;
    value.bits[3] = value.bits[3] >> START_EXP;
    for (int i = 0; i < 8; i++) {
        point += (int)((value.bits[3] & 1) * pow);
        pow *= 2;
        value.bits[3] = value.bits[3] >> 1;
    }
    return point;
}

void set_exponent(s21_decimal *value, int exp) {
    int sign = get_sign(*value);
    value->bits[3] = 0;
    exp <<= 16;
    value->bits[3] = exp | value->bits[3];
    if (sign) set_sign_1(value);
}

int find_first_1(s21_decimal value) {
    int result = END_THIRD_BYTE;
    for (; result > 0; result--) {
        if (get_bit(value, result)) break;
    }
    return result;
}

int is_null(s21_decimal value) {
    int ret = 0;
    if (value.bits[0] == 0 && value.bits[1] == 0 && value.bits[2] == 0) ret = 1;
    return ret;
}




