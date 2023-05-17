#include "s21_decimal.h"
#include <stdio.h>

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
    if (src >> 31 & 1) {
        src = -src;
        set_sign_1(dst);
    }
    dst->bits[0] = src;
    return 0;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
    int ret = 0;
    int exp = get_exponent(src);
    if (find_first_1(src) > END_FIRST_BYTE) {  // Если остались биты дальше 31-го бита, то это переполнение
        ret = 1;
    } else {
        *dst = (int)src.bits[0];
        if (exp > 0) {
            *dst /= (int)pow(10, exp);
        }
    }
    if (get_sign(src)) *dst *= -1;
    return ret;
}

//int s21_from_float_to_decimal(float src, s21_decimal *dst) {
//    s21_zero_decimal(dst);
//    int return_value = 0;
//    if (isinf(src) || isnan(src)) {
//        return_value = 1;
//    } else {
//        if (src != 0) {
//            int sign = *(int *)&src >> 31;
//            int exp = ((*(int *)&src & ~0x80000000) >> 23) - 127;
//            double temp = (double)fabs(src);
//            int off = 0;
//            for (; off < 28 && (int)temp / (int)pow(2, 21) == 0; temp *= 10, off++) {
//            }
//            temp = round(temp);
//            if (off <= 28 && (exp > -94 && exp < 96)) {
//                floatbits mant = {0};
//                temp = (float)temp;
//                for (; fmod(temp, 10) == 0 && off > 0; off--, temp /= 10) {
//                }
//                mant.fl = temp;
//                exp = ((*(int *)&mant.fl & ~0x80000000) >> 23) - 127;
//                dst->bits[exp / 32] |= 1 << exp % 32;
//                for (int i = exp - 1, j = 22; j >= 0; i--, j--)
//                    if ((mant.ui & (1 << j)) != 0) dst->bits[i / 32] |= 1 << i % 32;
//                dst->bits[3] = (sign << 31) | (off << 16);
//            }
//        }
//    }
//    return return_value;
//}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
    double dst_copy = (double)*dst;
    for (int i = 0; i < END_THIRD_BYTE; i++) {
        dst_copy += get_bit(src, i) * pow(2, i);
    }
    dst_copy = dst_copy * pow(10, - get_exponent(src));
    if (get_sign(src)) *dst *= -1;
    *dst = (float)dst_copy;
    return 0;
}

int s21_from_decimal_to_double(s21_decimal src, double *dst) {
    double temp = *dst;
    for (int i = 0; i < 96; i++) {
        temp += get_bit(src, i) * pow(2, i);
    }
    temp = temp * pow(10, -get_exponent(src));
    if (get_sign(src)) temp = temp * (-1);
    *dst = temp;
    return 0;
}