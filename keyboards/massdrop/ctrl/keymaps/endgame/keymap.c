#include "keymap.h"

static uint16_t idle_timer;             // Idle LED timeout timer
static uint8_t idle_second_counter;     // Idle LED seconds counter, counts seconds not milliseconds

//Associate our tap dance key with its functionality
qk_tap_dance_action_t tap_dance_actions[] = {
    [TD_LGUI_ML] = ACTION_TAP_DANCE_LAYER_TOGGLE(KC_LGUI, _ML),
    [TD_LCTRL_TERM] = ACTION_TAP_DANCE_DOUBLE(KC_LCTRL, LCA(KC_T)),
    [TD_RCTRL_TERM] = ACTION_TAP_DANCE_DOUBLE(KC_RCTRL, LCA(KC_T)),
    [TD_LSFT_CAPS] = ACTION_TAP_DANCE_DOUBLE(KC_LSFT, KC_CAPS),
    [TD_F_FIND] = ACTION_TAP_DANCE_DOUBLE(KC_F, LCTL(KC_F)),
    [TD_C_COPY] = ACTION_TAP_DANCE_DOUBLE(KC_C, LCTL(KC_C)),
    [TD_X_CUT] = ACTION_TAP_DANCE_DOUBLE(KC_X, LCTL(KC_X)),
    [TD_V_PASTE] = ACTION_TAP_DANCE_DOUBLE(KC_V, LCTL(KC_V)),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
    [DEFAULT] = LAYOUT(
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,             KC_PSCR, KC_SLCK, KC_PAUS, \
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,   KC_INS,  KC_HOME, KC_PGUP, \
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,   KC_DEL,  KC_END,  KC_PGDN, \
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT, \
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,                              KC_UP, \
        KC_LCTL, KC_LGUI, KC_LALT,                   KC_SPC,                             KC_RALT, MO(1),   KC_APP,  KC_RCTL,            KC_LEFT, KC_DOWN, KC_RGHT \
    ),
    */
    [_KL] = LAYOUT(
        KC_ESC,            KC_F1,          KC_F2,   KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8,   KC_F9,   KC_F10,  KC_F11,    KC_F12,                       KC_PSCR, KC_SLCK, KC_PAUS, \
        KC_GRV,            KC_1,           KC_2,    KC_3,  KC_4,  KC_5,  KC_6,  KC_7,  KC_8,    KC_9,    KC_0,    KC_MINS,   KC_EQL,            KC_BSPC,   KC_INS,  KC_HOME, KC_PGUP, \
        KC_TAB,            KC_Q,           KC_W,    KC_E,  KC_R,  KC_T,  KC_Y,  KC_U,  KC_I,    KC_O,    KC_P,    KC_LBRC,   KC_RBRC,           KC_BSLS,   KC_DEL,  KC_END,  KC_PGDN, \
        KC_CAPS,           KC_A,           KC_S,    KC_D,  KC_F,  KC_G,  KC_H,  KC_J,  KC_K,    KC_L,    KC_SCLN, KC_QUOT,   KC_ENT, \
        TD(TD_LSFT_CAPS),  KC_Z,           KC_X,    KC_C,  KC_V,  KC_B,  KC_N,  KC_M,  KC_COMM, KC_DOT,  KC_SLSH, KC_SFTENT,                                        KC_UP, \
        TD(TD_LCTRL_TERM), TD(TD_LGUI_ML), KC_LALT,               KC_SPC,                       KC_RALT, MO(_FL), KC_APP,    TD(TD_RCTRL_TERM),            KC_LEFT, KC_DOWN, KC_RGHT \
    ),
    [_FL] = LAYOUT(
        _______, DM_PLY1, DM_PLY2, _______,  _______, DM_REC1, DM_REC2, _______,  _______,  DM_RSTP, _______, KC_WAKE, KC_SLEP,            KC_MUTE, _______, _______, \
        _______, RGB_M_P, RGB_M_B, RGB_M_R,  RGB_M_SW, RGB_M_SN, RGB_M_K, RGB_M_X, RGB_M_G,  ROUT_FM, ROUT_TG, ROUT_VD, ROUT_VI, _______,   KC_MSTP, KC_MPLY, KC_VOLU, \
        _______, RGB_SPD, RGB_VAI, RGB_SPI,  RGB_HUI, RGB_SAI, _______, U_T_AUTO, U_T_AGCR, _______, _______, _______, _______, _______,   KC_MPRV, KC_MNXT, KC_VOLD, \
        _______, RGB_RMOD,RGB_VAD, RGB_MOD,  RGB_HUD, RGB_SAD, _______, _______,  _______,  _______, _______, _______, _______, \
        _______, RGB_TOG, _______, COPY_ALL, _______, MD_BOOT, TG_NKRO, _______,  _______,  _______, _______, _______,                              KC_BRIU, \
        _______, _______, _______,                    _______,                              _______, _______, _______, _______,            _______, KC_BRID, _______ \
    ),
    [_ML] = LAYOUT(
        _______, KC_ACL0,        KC_ACL1, KC_ACL2, _______, _______, _______, _______, _______, _______, _______, _______, _______,            _______, _______, _______, \
        _______, KC_BTN4,        KC_BTN3, KC_BTN5, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,   _______, _______, _______, \
        _______, KC_BTN1,        KC_MS_U, KC_BTN2, KC_WH_U, _______, _______, _______, _______, _______, _______, _______, _______, _______,   _______, _______, _______, \
        _______, KC_MS_L,        KC_MS_D, KC_MS_R, KC_WH_D, _______, _______, _______, _______, _______, _______, _______, _______, \
        _______, _______,        _______, SEL_CPY, _______, _______, TG_NKRO, _______, _______, _______, _______, _______,                              _______, \
        _______, TD(TD_LGUI_ML), _______,                   _______,                            _______, MO(_FL), TG(_ML), _______,            _______, _______, _______ \
    ),
    /*
    [X] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,            _______, _______, _______, \
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,   _______, _______, _______, \
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,   _______, _______, _______, \
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
        _______, _______, _______, _______, _______, _______, TG_NKRO, _______, _______, _______, _______, _______,                              _______, \
        _______, _______, _______,                   _______,                            _______, _______, _______, _______,            _______, _______, _______ \
    ),
    [RGB] = LAYOUT(
        ESC: 0,   F1: 1,    F2: 2,    F3: 3,    F4: 4,    F5: 5,    F6: 6,    F7: 7,    F8: 8,    F9: 9,    F10: 10,  F11: 11,  F12: 12,            PSCR: 13, SLCK: 14, PAUS: 15, \
        GRV: 16,  1: 17,    2: 18,    3: 19,    4: 20,    5: 21,    6: 22,    7: 23,    8: 24,    9: 25,    0: 26,    MINS: 27, EQL: 28,  BSPC: 29, INS: 30,  HOME: 31, PGUP: 32, \
        TAB: 33,  Q: 34,    W: 35,    E: 36,    R: 37,    T: 38,    Y: 39,    U: 40,    I: 41,    O: 42,    P: 43,    LBRC: 44, RBRC: 45, BSLS: 46, DEL: 47,  END: 48,  PGDN: 49, \
        CAPS: 50, A: 51,    S: 52,    D: 53,    F: 54,    G: 55,    H: 56,    J: 57,    K: 58,    L: 59,    SCLN: 60, QUOT: 61, ENT: 62, \
        LSFT: 63, Z: 64,    X: 65,    C: 66,    V: 67,    B: 68,    N: 69,    M: 70,    COMM: 71, DOT: 72,  SLSH: 73, RSFT: 74,                               UP: 75, \
        LCTL: 76, LGUI: 77, LALT: 78,                   SPC: 79,                                  RALT: 80, Fn: 81,   APP: 82,  RCTL: 83,           LEFT: 84, DOWN: 85, RGHT: 86 \
    ),
	[MATRIX] = LAYOUT(
	    0,       1,       2,       3,       4,       5,       6,       7,       8,       9,       10,      11,      12,                 13,      14,      15,      \
	    16,      17,      18,      19,      20,      21,      22,      23,      24,      25,      26,      27,      28,      29,        30,      31,      32,      \
	    33,      34,      35,      36,      37,      38,      39,      40,      41,      42,      43,      44,      45,      46,        47,      48,      49,      \
	    50,      51,      52,      53,      54,      55,      56,      57,      58,      59,      60,      61,      62,      \
	    63,      64,      65,      66,      67,      68,      69,      70,      71,      72,      73,      74,                                   75,      \
	    76,      77,      78,                        79,                                 80,      81,      82,      83,                 84,      85,      86      \
	),
    */
};

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {
    // Enable or disable debugging
    debug_enable=true;
    debug_matrix=true;
    debug_keyboard=true;
    debug_mouse=true;

    idle_second_counter = 0;
    rgb_time_out_seconds = RGB_DEFAULT_TIME_OUT;
    rgb_time_out_enable = false;                    // Disable RGB timeout by default. Enable using toggle key.
    rgb_time_out_user_value = false;                // Has to have the same initial value as rgb_time_out_enable
    rgb_enabled_flag = true;                        // Initially, keyboard RGB is enabled. Change appropriately if config.h initializes RGB disabled.
    rgb_time_out_fast_mode_enabled = false;         // RGB timeout fast mode disabled initially.
    rgb_time_out_saved_flag = rgb_matrix_get_flags();
};

void keyboard_post_init_user(void) {
    rgb_matrix_enable();
}

// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {
    if(rgb_time_out_enable && rgb_enabled_flag) {
        if (timer_elapsed(idle_timer) > MILLISECONDS_IN_SECOND) {
            idle_second_counter++;
            idle_timer = timer_read();
        }

        if (idle_second_counter >= rgb_time_out_seconds) {
            rgb_time_out_saved_flag = rgb_matrix_get_flags();
            rgb_matrix_set_flags(LED_FLAG_NONE);
            rgb_matrix_disable_noeeprom();
            rgb_enabled_flag = false;
            idle_second_counter = 0;
        }
    }
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static uint32_t key_timer;

    if (rgb_time_out_enable) {
        idle_timer = timer_read();
        // Reset the seconds counter. Without this, something like press> leave x seconds> press, would be x seconds on the effective counter not 0 as it should.
        idle_second_counter = 0;
        if (!rgb_enabled_flag) {
            rgb_matrix_enable_noeeprom();
            rgb_matrix_set_flags(rgb_time_out_saved_flag);
            rgb_enabled_flag = true;
        }
    }

    switch (keycode) {
        case U_T_AUTO:
            if (record->event.pressed && MODS_SHIFT && MODS_CTRL) {
                TOGGLE_FLAG_AND_PRINT(usb_extra_manual, "USB extra port manual mode");
            }
            return false;
        case U_T_AGCR:
            if (record->event.pressed && MODS_SHIFT && MODS_CTRL) {
                TOGGLE_FLAG_AND_PRINT(usb_gcr_auto, "USB GCR auto mode");
            }
            return false;
        case DBG_TOG:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_enable, "Debug mode");
            }
            return false;
        case DBG_MTRX:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_matrix, "Debug matrix");
            }
            return false;
        case DBG_KBD:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_keyboard, "Debug keyboard");
            }
            return false;
        case DBG_MOU:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_mouse, "Debug mouse");
            }
            return false;
        case MD_BOOT:
            if (record->event.pressed) {
                key_timer = timer_read32();
            } else {
                if (timer_elapsed32(key_timer) >= 500) {
                    reset_keyboard();
                }
            }
            return false;
        case RGB_TOG:
            if (record->event.pressed) {
                rgb_time_out_enable = rgb_time_out_user_value;
                switch (rgb_matrix_get_flags()) {
                    case LED_FLAG_ALL: {
                        rgb_matrix_set_flags(LED_FLAG_KEYLIGHT | LED_FLAG_MODIFIER);
                        rgb_matrix_set_color_all(0, 0, 0);
                    }
                    break;
                    case LED_FLAG_KEYLIGHT | LED_FLAG_MODIFIER: {
                        rgb_matrix_set_flags(LED_FLAG_UNDERGLOW);
                        rgb_matrix_set_color_all(0, 0, 0);
                    }
                    break;
                    case LED_FLAG_UNDERGLOW: {
                        // This line is for LED idle timer. It disables the toggle so you can turn off LED completely if you like
                        rgb_time_out_enable = false;
                        rgb_matrix_set_flags(LED_FLAG_NONE);
                        rgb_matrix_disable_noeeprom();
                    }
                    break;
                    default: {
                        rgb_matrix_set_flags(LED_FLAG_ALL);
                        rgb_matrix_enable_noeeprom();
                    }
                    break;
                }
            }
            return false;
        // ======================================================== CUSTOM KEYCOADS BELOW ========================================================
        case COPY_ALL:
            if (record->event.pressed) {
                // Selects all and text and copy
                SEND_STRING(SS_LCTRL("ac"));
            }
            return false;
        case SEL_CPY:
            // Select word under cursor and copy. Double mouse click then ctrl+c
            if (record->event.pressed) {
                tap_code16(KC_BTN1);
                tap_code16(KC_BTN1);
                SEND_STRING(SS_LCTRL("c"));
            }
            return false;
        case ROUT_TG:
            // Toggle idle LED timeout on or off
            if (record->event.pressed) {
                rgb_time_out_enable = !rgb_time_out_enable;
                rgb_time_out_user_value = rgb_time_out_enable;
            }
            return false;
        case ROUT_VI:
            // Increase idle LED timeout value in seconds
            if (record->event.pressed) {
                // Only increase if current value is lower than RGB_TIME_OUT_MAX. Don't care what value the result will be
                // Modity RGB_TIME_OUT_STEP for bigger or smaller increments
                if (!rgb_time_out_fast_mode_enabled && rgb_time_out_seconds <= RGB_TIME_OUT_MAX) {
                    rgb_time_out_seconds += RGB_TIME_OUT_STEP;
                }
            }
            return false;
        case ROUT_VD:
                // Decrease idle LED timeout value in seconds
            if (record->event.pressed) {
                // Only decrease if current value is higher than minimum value and the result is larger than zero
                // Modity RGB_TIME_OUT_STEP for bigger or smaller decrements
                if (!rgb_time_out_fast_mode_enabled && rgb_time_out_seconds > RGB_TIME_OUT_MIN) {
                    rgb_time_out_seconds -= RGB_TIME_OUT_STEP;
                }
            }
            return false;
        case ROUT_FM:
            if (record->event.pressed) {
                if (rgb_time_out_fast_mode_enabled) {
                    rgb_time_out_seconds = rgb_time_out_saved_seconds;
                } else {
                    rgb_time_out_saved_seconds = rgb_time_out_seconds;
                    rgb_time_out_seconds = RGB_FAST_MODE_TIME_OUT;
                }
                rgb_time_out_fast_mode_enabled = !rgb_time_out_fast_mode_enabled;
            }
            return false;
        default:
            return true; //Process all other keycodes normally
    }
}

#ifdef _______
#undef _______
#define _______ {HSV_WHITE}
#define RED {HSV_RED}
#define CORAL {HSV_CORAL}
#define ORANGE {HSV_ORANGE}
#define GOLDEN {HSV_GOLDENROD}
#define GOLD {HSV_GOLD}
#define YELLOW {HSV_YELLOW}
#define CHART {HSV_CHARTREUSE}
#define GREEN {HSV_GREEN}
#define SPRING {HSV_SPRINGGREEN}
#define TURQ {HSV_TURQUOISE}
#define TEAL {HSV_TEAL}
#define CYAN {HSV_CYAN}
#define AZURE {HSV_AZURE}
#define BLUE {HSV_BLUE}
#define PURPLE {HSV_PURPLE}
#define MAGENT {HSV_MAGENTA}
#define PINK {HSV_PINK}

const uint8_t PROGMEM ledmap[][DRIVER_LED_TOTAL][3] = {
    /*
    [RGB] = LAYOUT(
        ESC: 0,   F1: 1,    F2: 2,    F3: 3,    F4: 4,    F5: 5,    F6: 6,    F7: 7,    F8: 8,    F9: 9,    F10: 10,  F11: 11,  F12: 12,            PSCR: 13, SLCK: 14, PAUS: 15, \
        GRV: 16,  1: 17,    2: 18,    3: 19,    4: 20,    5: 21,    6: 22,    7: 23,    8: 24,    9: 25,    0: 26,    MINS: 27, EQL: 28,  BSPC: 29, INS: 30,  HOME: 31, PGUP: 32, \
        TAB: 33,  Q: 34,    W: 35,    E: 36,    R: 37,    T: 38,    Y: 39,    U: 40,    I: 41,    O: 42,    P: 43,    LBRC: 44, RBRC: 45, BSLS: 46, DEL: 47,  END: 48,  PGDN: 49, \
        CAPS: 50, A: 51,    S: 52,    D: 53,    F: 54,    G: 55,    H: 56,    J: 57,    K: 58,    L: 59,    SCLN: 60, QUOT: 61, ENT: 62, \
        LSFT: 63, Z: 64,    X: 65,    C: 66,    V: 67,    B: 68,    N: 69,    M: 70,    COMM: 71, DOT: 72,  SLSH: 73, RSFT: 74,                               UP: 75, \
        LCTL: 76, LGUI: 77, LALT: 78,                     SPC: 79,                                RALT: 80, Fn: 81,   APP: 82,  RCTL: 83,           LEFT: 84, DOWN: 85, RGHT: 86 \
    )
    _ML] = LAYOUT(
        _______, KC_ACL0,        KC_ACL1, KC_ACL2, _______, _______, _______, _______, _______, _______, _______, _______, _______,            _______, _______, _______, \
        _______, KC_BTN4,        KC_BTN3, KC_BTN5, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,   _______, _______, _______, \
        _______, KC_BTN1,        KC_MS_U, KC_BTN2, KC_WH_U, _______, _______, _______, _______, _______, _______, _______, _______, _______,   _______, _______, _______, \
        _______, KC_MS_L,        KC_MS_D, KC_MS_R, KC_WH_D, _______, _______, _______, _______, _______, _______, _______, _______, \
        _______, _______,        _______, SEL_CPY, _______, _______, TG_NKRO, _______, _______, _______, _______, _______,                              _______, \
        _______, TD(TD_LGUI_ML), _______,                   _______,                            _______, MO(_FL), TG(_ML), _______,            _______, _______, _______ \
    ),
    */
    [1] = {
        _______, CORAL,   CORAL,   _______, _______, CORAL,   CORAL,  _______,  _______, CORAL,   _______, YELLOW,  YELLOW,           TEAL,    _______, _______, \
        _______, ORANGE,  _______, _______, _______, _______, _______, _______, _______, GREEN,   GREEN,   GREEN,   GREEN,   _______, TEAL,    TEAL,    TEAL, \
        _______, ORANGE,  ORANGE,  ORANGE,  ORANGE,  ORANGE,  _______, AZURE,   AZURE,   _______, _______, _______, _______, _______, TEAL,    TEAL,    TEAL, \
        _______, ORANGE,  ORANGE,  ORANGE,  ORANGE,  ORANGE,  _______, _______, _______, _______, _______, _______, _______, \
        _______, ORANGE,  _______, CORAL,   _______, AZURE,    AZURE,  _______, _______, _______, _______, AZURE,                              SPRING,  \
        _______, _______, _______,                   _______,                            _______, PINK,    _______, AZURE,            _______, SPRING,  {141, 100, 100} \
    },
    [2] = {
        _______, GOLD,    GOLD,    GOLD,   _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, \
        _______, MAGENT,  MAGENT,  MAGENT, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
        _______, MAGENT,  GOLD,    MAGENT, GOLD,    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
        _______, GOLD,    GOLD,    GOLD,   GOLD,    _______, _______, _______, _______, _______, _______, _______, _______, \
        _______, _______, _______, CORAL,  _______, _______, GOLD,    _______, _______, _______, _______, _______,                            YELLOW,  \
        _______, PINK,    _______,                  _______,                                     _______, _______, PINK,    _______, _______, YELLOW,  _______ \
    },
};

#undef _______
#define _______ KC_TRNS
#endif

void set_layer_color(int layer) {
    for (int i = 0; i < DRIVER_LED_TOTAL; i++) {
        HSV hsv = {
            .h = pgm_read_byte(&ledmap[layer][i][0]),
            .s = pgm_read_byte(&ledmap[layer][i][1]),
            .v = pgm_read_byte(&ledmap[layer][i][2]),
        };
        if ((hsv.h || hsv.s || hsv.v) && !(hsv.h == 0 && hsv.s == 0 && hsv.v == 255)) {
            RGB rgb = hsv_to_rgb( hsv );
            float f = (float)rgb_matrix_config.hsv.v / UINT8_MAX;
            rgb_matrix_set_color( i, f * rgb.r, f * rgb.g, f * rgb.b );
        }
    }
}

void rgb_matrix_indicators_user(void) {
    if (g_suspend_state || disable_layer_color) { return; }
    if (biton32(layer_state) == 1 || biton32(layer_state) ==2) {
        set_layer_color(biton32(layer_state));
    }
}

