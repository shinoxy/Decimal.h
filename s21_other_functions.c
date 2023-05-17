#include "s21_decimal.h"

int s21_floor(s21_decimal value, s21_decimal *result) {
    *result = set_to_null(*result);
    s21_truncate(value, result);
    if (get_sign(value) && !s21_is_equal(*result, value)) {
        s21_sub(*result, ((s21_decimal){{1, 0, 0, 0}}), result);
    }
    return 0;
}

int s21_negate(s21_decimal dec, s21_decimal *result) {
    *result = dec;
    int sign = get_sign(dec);
    if (sign == 1) {
        set_sign_0(result);
    } else {
        set_sign_1(result);
    }
    return 0;
}

int s21_round(s21_decimal value, s21_decimal *result) {
    *result = set_to_null(*result);
    double fl_to_round = 0.0;
    long double difference = 0;
    s21_from_decimal_to_double(value, &fl_to_round);
    fl_to_round = round(fl_to_round);
    if (fl_to_round < 0.0) {
        set_sign_1(result);

        fl_to_round *= -1;
    }
    for (int i = 0; fl_to_round >= 1 && i < END_THIRD_BYTE; i++) {
        fl_to_round = floor(fl_to_round) / 2;
        difference = fl_to_round - floor(fl_to_round);
        if (difference > 0.0) {
            set_bit_1(result, i);
        } else {
            set_bit_0(result, i);
        }
    }
    set_exponent(result, 0);
    return 0;
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
    *result = set_to_null(*result);
    float difference = 0;
    int exp = get_exponent(value);
    if (exp) {
        double fl_to_int = 0.0;
        s21_from_decimal_to_double(value, &fl_to_int);
        fl_to_int = trunc(fl_to_int);
        if (fl_to_int < 0.0) {
            set_sign_1(result);

            fl_to_int *= -1;
        }
        for (int i = 0; fl_to_int >= 1 && i < END_THIRD_BYTE; i++) {
            fl_to_int = floor(fl_to_int) / 2;
            difference = fl_to_int - floor(fl_to_int);
            if (difference > 0.0) {
                set_bit_1(result, i);
            } else {
                set_bit_0(result, i);
            }
        }
        set_exponent(result, 0);
    } else {
        for (int i = 0; i < 4; ++i) {
            result->bits[i] = value.bits[i];
        }
    }
    return 0;
}