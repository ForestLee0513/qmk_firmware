// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later
#include QMK_KEYBOARD_H
#include "as5600.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        JS_10,  JS_9,   JS_11,  JS_12,
        JS_1,   JS_3,   JS_5,   KC_TRNS,
        JS_0,   JS_2,   JS_4,   JS_6
    )
};

static uint16_t current_position = 0;
int16_t joystick_position = 0;
int16_t full_rotation_angle = 4096;     // AS5600 12-bit resolution (0-4095)
int16_t full_joystick_value = 32767;    // Equivalent to max value of int16. +full_joystick_value is +1.0 axis output. -full_joystick_value is -1.0 axis output.
joystick_config_t joystick_axes[JOYSTICK_AXIS_COUNT] = {
    [0] = JOYSTICK_AXIS_VIRTUAL,
    [1] = JOYSTICK_AXIS_VIRTUAL   // Y축 추가
};

void keyboard_post_init_user(void) {
    as5600_init();
    current_position = as5600_get_rawangle();

    joystick_set_axis(1, 0);
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    uint16_t ra = as5600_get_rawangle();
    int16_t delta = (int16_t)(ra - current_position);

    // Handle wraparound: if delta is too large, it wrapped around
    if (delta > 2048) {
        delta -= 4096;
    } else if (delta < -2048) {
        delta += 4096;
    }

    // Convert delta to joystick position
    joystick_position += delta * full_joystick_value / full_rotation_angle;
    joystick_set_axis(0, joystick_position);

    // Update current position
    current_position = ra;

    return false;
}
