#include "s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    s21_big_decimal value_1_big = to_big_decimal(value_1);
    s21_big_decimal value_2_big = to_big_decimal(value_2);
    s21_big_decimal result_big = to_big_decimal(*result);

    int exp_1 = get_exponent_big(value_1_big);
    int exp_2 = get_exponent_big(value_2_big);
    if (exp_1 != exp_2) equating_exponent_big(&value_1_big, &value_2_big);

    int sign_1 = get_sign_big(value_1_big);
    int sign_2 = get_sign_big(value_2_big);

    if (sign_1 > sign_2) { // If negative and positive
        minus(value_2_big, value_1_big, &result_big);
    } else if (sign_1 < sign_2) {  // If positive and negative
        minus(value_1_big, value_2_big, &result_big);
    } else {  // If signs are equal
        plus(value_1_big, value_2_big, &result_big);
        if (sign_1 == 1 && sign_2 == 1) set_sign_1_big(&result_big);
    }
    int res = to_normal_decimal(result_big, result);
    return res;
}

int plus(s21_big_decimal value_1, s21_big_decimal value_2, s21_big_decimal *result) {
    set_sign_0_big(&value_1);
    set_sign_0_big(&value_2);
    int buff = 0, ret = 0;
    s21_big_decimal null = {0};
    *result = null;
    for (int i = 0; i < END_SIXTH_BYTE; i++) {
        if (get_bit_big(value_1, i) || get_bit_big(value_2, i)) {
            if (!buff) set_bit_1_big(result, i);
        } else if (get_bit_big(value_1, i) == 1 && get_bit_big(value_2, i) == 1) {
            if (buff) {
                set_bit_1_big(result, i);
                buff--;
            }
            buff++;
        } else {
            if (buff) {
                set_bit_1_big(result, i);
                buff--;
            }
        }
    }
    if (buff) ret = 1;
    return ret;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    s21_big_decimal value_1_big = to_big_decimal(value_1);
    s21_big_decimal value_2_big = to_big_decimal(value_2);
    s21_big_decimal result_big = to_big_decimal(*result);

    int exp_1 = get_exponent_big(value_1_big);
    int exp_2 = get_exponent_big(value_2_big);
    if (exp_1 != exp_2) equating_exponent_big(&value_1_big, &value_2_big);

    int sign_1 = get_sign_big(value_1_big);
    int sign_2 = get_sign_big(value_2_big);

    if (sign_1 > sign_2) { // If negative and positive
        plus(value_1_big, value_2_big, &result_big);
        set_sign_1_big(&result_big);
    } else if (sign_1 < sign_2) {  // If positive and negative
        plus(value_1_big, value_2_big, &result_big);
    } else {
        if (sign_1 == 1 && sign_2 == 1) {  // Both negative
            minus(value_2_big, value_1_big, &result_big);
            set_sign_1_big(&result_big);
        } else {  // Both positive
            minus(value_1_big, value_2_big, &result_big);
        }
    }
    int res = to_normal_decimal(result_big, result);
    return res;
}

int minus(s21_big_decimal value_1, s21_big_decimal value_2, s21_big_decimal *result) {
    set_sign_0_big(&value_1);
    set_sign_0_big(&value_2);
    if (is_equal_big(&value_1, &value_2)) {
        *result = set_to_null_big(*result);
    } else {
        if (!is_less_big(value_1, value_2)) {
            s21_big_decimal tmp = value_1;
            value_1 = value_2;
            value_2 = tmp;
            set_sign_1_big(result);
        }
        int buff = 0;
        int end_of_number = find_first_1_big(value_1);
        for (int i = 0; i < end_of_number; i++) {
            int diff = get_bit_big(value_1, i) - get_bit_big(value_2, i) - buff;
            if (diff < 0) {
                diff += 2;
                buff = 1;
            } else {
                buff = 0;
            }
            if (diff == 1) {
                set_bit_1_big(result, i);
            } else {
                set_bit_0_big(result, i);
            }
        }
    }
    return 0;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    s21_big_decimal value_1_big = to_big_decimal(value_1);
    s21_big_decimal value_2_big = to_big_decimal(value_2);
    s21_big_decimal result_big = to_big_decimal(*result);
    if (get_sign_big(value_1_big) + get_sign_big(value_2_big) == 1) set_sign_1_big(&result_big);  // Если разные знаки, то будет минус
    int index_mul = 0;
    s21_decimal null = {0};
    if (s21_is_equal(value_1, null)) {
        for (int k = 0; k < find_first_1_big(value_2_big); k++) {
            s21_big_decimal tmp = {0};
            for (int i = 0; i < 96; i++) {
                if (get_bit_big(value_1_big, i) && get_bit_big(value_2_big, index_mul)) {
                    set_bit_1_big(&tmp, i);
                }
            }
            for (int g = 0; g < k; g++) move_left_big(&tmp);
            plus(result_big, tmp, &result_big);
            index_mul++;
            if (index_mul > 31) {
                index_mul = 0;
            }
        }
    }
    int res = to_normal_decimal(result_big, result);
    return res;
}

int multiply(s21_big_decimal *value_1, s21_big_decimal *value_2, s21_big_decimal *result) {
    int index_mul = 0;
    int equality_points = 0;  // Is value_1 == 0;
    for (int i = 0; i < 8; i++) {
        if (value_1->bits[i] == 0) equality_points++;
    }
    if (equality_points == 8) {
        int first_symbol_2 = find_first_1_big(*value_2);
        for (int k = 0; k < first_symbol_2; k++) {
            s21_big_decimal tmp = {0};
            for (int i = 0; i < END_SIXTH_BYTE - 1; i++) {
                if (get_bit_big(*value_1, i) && get_bit_big(*value_2, index_mul)) {
                    set_bit_1_big(&tmp, i);
                }
            }
            for (int g = 0; g < k; g++) move_left_big(&tmp);
            plus(*result, tmp, result);
            index_mul++;
            if (index_mul > 31) {
                index_mul = 0;
            }
        }
    }
    return 1;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result){
    s21_big_decimal value_1_big = to_big_decimal(value_1);
    s21_big_decimal value_2_big = to_big_decimal(value_2);
    s21_big_decimal result_big = to_big_decimal(*result);
    int ret = div_prev(value_1_big, value_2_big, &result_big);
    if (!ret) ret = to_normal_decimal(result_big, result);
    return ret;
}

int div_prev(s21_big_decimal value_1, s21_big_decimal value_2, s21_big_decimal *result) {
    int ret = 0, sign = 0;
    s21_big_decimal null = set_to_null_big(null);
    if (get_sign_big(value_1) != get_sign_big(value_2)) {
        value_1.bits[7]=value_1.bits[7]&~(1<<31);
        value_2.bits[7]=value_2.bits[7]&~(1<<31);
        sign = 1;
    }
    if (is_null_big(value_2)) {
        ret = 3;
        *result = set_to_null_big(*result);
    } else {
        int exp_1 = get_exponent_big(value_1);
        int exp_2 = get_exponent_big(value_2);
        if (is_null_big(value_1)) {
            ret = 1;
            *result = null;
        }
        if (!ret) {
            s21_big_decimal value_1_copy = value_1, value_2_copy = value_2;
            if (exp_1 != exp_2) equating_exponent_big(&value_1_copy, &value_2_copy);
            if (is_equal_big(&value_1, &value_2)) {
                ret = 1;
                result->bits[0] = 1;
            }
        }
        if (!ret) {
            for (int i = exp_2; i != 0; i--){
                if (exp_1) {
                    set_exponent_big(&value_1, (exp_1 - 1));
                } else {
                    s21_big_decimal ten = {{0b1010,0}};
                    s21_big_decimal result_copy={0};
                    multiply(&value_1, &ten, &result_copy);
                    value_1 = result_copy;
                }
            }
            value_2.bits[7] = 0;
            round_segmentation(result, segmentation(value_1, value_2, result));
            if (sign) result->bits[7]= result->bits[7] | 1 << 31;
            ret = 1;
        }
    }
    return ret - 1;
}

int segmentation(s21_big_decimal value_1, s21_big_decimal value_2, s21_big_decimal *result) {
    s21_big_decimal result_copy = {0}, remainder = {0}, null = {0};
    result_copy.bits[7] = result->bits[7];
    *result = result_copy;
    int stop = 0, exponentUP = 0;
    int target_symbol = find_first_1_big(value_1) - 1;
    int exp_result = get_exponent_big(value_1);
    while(!stop){
        if (target_symbol + 1) if (value_1.bits[target_symbol / 32] & 1 << target_symbol % 32)remainder.bits[0]= remainder.bits[0] | 1;
        if ((is_less_big(value_2, remainder) || is_equal_big(&value_2, &remainder))) {
            result_copy.bits[0] |= 1;
            minus(remainder, value_2, &remainder);
        }
        move_left_big(&result_copy);
        move_left_big(&remainder);
        target_symbol--;
        if ((is_null_big(remainder) && target_symbol + 1 <= 0) || exp_result > 28) stop = 1;
        if (target_symbol + 1 <= 0 && !stop) {
            exponentUP=1;
            move_right_big(&result_copy);
            s21_big_decimal ten = {{0b1010,0}};
            plus(*result, result_copy, result);
            if (exp_result == 28) stop++;
            if (exp_result < 28) {
                if (!is_null_big(*result)) multiply(result, &ten, result);
                move_right_big(&remainder);
                multiply(&remainder, &ten, &value_1);
                remainder = null;
                target_symbol = find_first_1_big(value_1) - 1;
                exp_result++;
                result_copy=null;
            }
        }
    }
    move_right_big(&result_copy);
    move_right_big(&remainder);
    if (!exponentUP) *result = result_copy;
    set_exponent_big(result, exp_result);
    move_right_big(&value_2);
    int round = 0;
    if (is_less_big(value_2, remainder) || is_equal_big(&value_2, &remainder)) round = 1;
    if (!round) {
        round = -1;
    } else {
        round = is_less_big(value_2, remainder);
    }
    if (is_null_big(remainder)) round = 2;
    return round;

}

void round_segmentation(s21_big_decimal *number, int round) {
    s21_big_decimal one = {{1,0}};
    int exp = get_exponent_big(*number);
    set_exponent_big(&one, exp);
    if (round == 0 || round == 1) plus(*number, one, number);
    if (round == 0) number->bits[0] = number->bits[0] &~ 1;
    set_exponent_big(number, exp);
}