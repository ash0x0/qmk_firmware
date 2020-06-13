#include QMK_KEYBOARD_H
#include "hid_protocol.h"

uint8_t raw_hid_buffer[RAW_EPSIZE];

void raw_hid_perform_send(void) {
    raw_hid_send(raw_hid_buffer, RAW_EPSIZE);
}

/* This is a test function for Raw HID, which is currently not implemented for this keyboard */
/**
void raw_hid_receive(uint8_t *data, uint8_t length) {
    uint8_t response[RAW_EPSIZE];
    memset(response+1, 'C', 1);
    memset(response+2, 'T', 1);
    memset(response+3, 'R', 1);
    memset(response+4, 'L', 1);
    raw_hid_send(data, length);
}
*/

bool is_led_timeout = true;

void change_led_state(bool is_off) {
    is_led_timeout = is_off;
    int led_state = rgb_matrix_get_flags();

    if (is_led_timeout) {
        if (led_state != LED_FLAG_NONE) {
            rgb_matrix_set_flags(LED_FLAG_NONE);
            rgb_matrix_disable_noeeprom();
        }
    } else {
        if (led_state != LED_FLAG_NONE) {
            rgb_matrix_set_flags(led_state);
            rgb_matrix_enable_noeeprom();
        }
    }
}

void raw_hid_say_hello(void) {
    const char *ctrl = CTRL_HID_GREETING_VERSION;
    uint8_t i = 0;
    while (ctrl[i] != 0 && i + 2 < RAW_EPSIZE) {
        raw_hid_buffer[1 + i] = ctrl[i];
        i++;
    }
    raw_hid_buffer[i] = CTRL_HID_EOM;
}

void raw_hid_lights_toggle(void) {
    change_led_state(!is_led_timeout);

    raw_hid_buffer[1] = CTRL_HID_OK;
    raw_hid_buffer[2] = (uint8_t) is_led_timeout;
    raw_hid_buffer[3] = CTRL_HID_EOM;
}

void raw_hid_led(uint8_t *data) {
    //rgb_matrix_set_color(data[1], data[2], data[3], data[4]);
    const uint8_t led = data[1];

    if (led >= DRIVER_LED_TOTAL) {
        raw_hid_buffer[1] = CTRL_HID_NOK;
        raw_hid_buffer[2] = DRIVER_LED_TOTAL;
        raw_hid_buffer[3] = CTRL_HID_EOM;
        return;
    }

    rgb_matrix_led_state[led].r = data[2];
    rgb_matrix_led_state[led].g = data[3];
    rgb_matrix_led_state[led].b = data[4];

    raw_hid_buffer[1] = CTRL_HID_OK;
    raw_hid_buffer[2] = CTRL_HID_EOM;
}

void raw_hid_leds(uint8_t *data) {
    const uint8_t first_led = data[1];
    const uint8_t number_leds = data[2];

    uint8_t i = 0;
    while (i < number_leds && first_led + i < DRIVER_LED_TOTAL && i * 3 + 5 < RAW_EPSIZE) {
        rgb_matrix_led_state[first_led + i].r = data[3 + i * 3 + 0];
        rgb_matrix_led_state[first_led + i].g = data[3 + i * 3 + 1];
        rgb_matrix_led_state[first_led + i].b = data[3 + i * 3 + 2];
        i++;
    }

    raw_hid_buffer[1] = CTRL_HID_OK;
    raw_hid_buffer[2] = i;
    raw_hid_buffer[3] = CTRL_HID_EOM;
}

void raw_hid_rgbmatrix_mode(uint8_t *data) {
    const uint8_t mode = data[1];
    if (mode >= RGB_MATRIX_EFFECT_MAX) {
        raw_hid_buffer[1] = CTRL_HID_NOK;
        raw_hid_buffer[2] = RGB_MATRIX_EFFECT_MAX - 1;
        raw_hid_buffer[3] = CTRL_HID_EOM;
        return;
    }
    rgb_matrix_mode_noeeprom(mode);
    raw_hid_buffer[1] = CTRL_HID_OK;
    raw_hid_buffer[2] = CTRL_HID_EOM;
}

void raw_hid_receive(uint8_t *data, uint8_t length) {
    switch (*data) {
        case CTRL_HID_HELLO:
            raw_hid_say_hello();
            break;
        case CTRL_HID_LIGHTS_TOGGLE:
            raw_hid_lights_toggle();
            break;
        case CTRL_HID_LED:
            raw_hid_led(data);
            break;
        case CTRL_HID_LEDS:
            raw_hid_leds(data);
            break;
        case CTRL_HID_RGBMATRIX_MODE:
            raw_hid_rgbmatrix_mode(data);
            break;
    }
    raw_hid_perform_send();
}
