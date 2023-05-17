#include "s21_decimal.h"

// BIG DECIMAL

s21_big_decimal set_to_null_big(s21_big_decimal value) {
    for (int i = 0; i <= 7; i++) value.bits[i] = 0;
    return value;
}

int get_bit_big(s21_big_decimal value, int pos) {
    int bit = 0;
    unsigned int mask = 1U << (pos % 32);
    if ((value.bits[pos/32] & mask) > 0) bit = 1;
    return bit;
}

int get_sign_big(s21_big_decimal value) {
    return (int)((value.bits[7] >> SIGN) & 1);  // 0 is positive, 1 is negative
}

void set_sign_1_big(s21_big_decimal *value) {
    value->bits[7] |= (1U << SIGN);
}

void set_sign_0_big(s21_big_decimal *value) {
    value->bits[7] |= (0U << SIGN);
}

void set_bit_0_big(s21_big_decimal *value, int pos) {
    value->bits[pos/32] &= ~(0U << pos % 32);
}

void set_bit_1_big(s21_big_decimal *value, int pos) {
    value->bits[pos/32] |= (1U << pos % 32);
}

int move_left_big(s21_big_decimal *value) {
    for (int i = 6; i >= 0; i--) {
        if (value->bits[i] & 1 << 31)
            value->bits[i + 1] = value->bits[i + 1] | 1;
        value->bits[i] = value->bits[i] << 1;
    }
    return (int)(value->bits[7] & 1U);
}

void move_right_big(s21_big_decimal *value) {
    int next_byte_first_bit = 32;
    int current_byte_last_bit = 31;
    for (int i = 0; i < 7; i++) {
        value->bits[i] = value->bits[i] >> 1;
        if (get_bit_big(*value, next_byte_first_bit)) set_bit_1_big(value, current_byte_last_bit);
        next_byte_first_bit += 32;
        current_byte_last_bit += 32;
    }
}

// NORMAL DECIMAL

s21_decimal set_to_null(s21_decimal value) {
    for (int i = 0; i <= 3; i++) value.bits[i] = 0;
    return value;
}

int get_bit(s21_decimal value, int pos) {
    int bit = 0;
    unsigned int mask = 1U << (pos % 32);
    if ((value.bits[pos/32] & mask) > 0) bit = 1;
    return bit;
}

int get_sign(s21_decimal value) {
    return (int)((value.bits[3] >> SIGN) & 1);  // 0 is positive, 1 is negative
}

void set_sign_1(s21_decimal *value) {
    value->bits[3] |= (1U << SIGN);
}

void set_sign_0(s21_decimal *value) {
    value->bits[3] |= (0U << SIGN);
}

void set_bit_0(s21_decimal *value, int pos) {
    value->bits[pos/32] &= ~(0U << pos % 32);
}

void set_bit_1(s21_decimal *value, int pos) {
    value->bits[pos/32] |= (1U << pos % 32);
}

int move_left(s21_decimal *value) {
    for (int i = 2; i >= 0; i--) {
        if (value->bits[i] & 1 << 31)
            value->bits[i + 1] = value->bits[i + 1] | 1;
        value->bits[i] = value->bits[i] << 1;
    }
    return (int)(value->bits[3] & 1U);
}

void move_right(s21_decimal *value) {
    int next_byte_first_bit = 32;
    int current_byte_last_bit = 31;
    for (int i = 0; i < 3; i++) {
        value->bits[i] = value->bits[i] >> 1;
        if (get_bit(*value, next_byte_first_bit)) set_bit_1(value, current_byte_last_bit);
        next_byte_first_bit += 32;
        current_byte_last_bit += 32;
    }
}