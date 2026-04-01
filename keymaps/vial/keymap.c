/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      KC_ESC,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,  KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_TAB,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                         KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                         KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, KC_RSFT,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_RCTL, KC_ENT, TL_LOWR,     TL_UPPR, KC_SPC, KC_LGUI
                                      //`--------------------------'  `--------------------------'
  ),

  [1] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      KC_GRV,  KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_DEL,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
    KC_TAB,LSFT(KC_1),LSFT(KC_2),LSFT(KC_3),LSFT(KC_4),LSFT(KC_5),        LSFT(KC_6),LSFT(KC_7),LSFT(KC_8),LSFT(KC_9),LSFT(KC_0),XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT,  KC_EQL, KC_MINS, KC_PLUS, KC_LCBR, KC_RCBR,                      KC_LBRC, KC_RBRC, KC_SCLN, KC_COLN, KC_BSLS, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_RCTL, KC_ENT, _______,     _______, KC_SPC, KC_LGUI
                                      //`--------------------------'  `--------------------------'
  ),

  [2] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,                        KC_F7,   KC_F8,   KC_F9,   KC_F10,   KC_F11,  KC_F12,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_CAPS,                      KC_LEFT, KC_DOWN, KC_UP  ,KC_RIGHT, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LALT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, KC_PGDN, KC_PGUP, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_RCTL, KC_ENT, _______,     _______, KC_SPC, KC_LGUI
                                      //`--------------------------'  `--------------------------'
  ),

  [3] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      QK_BOOT, RGB_TOG, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      RGB_MOD, RGB_HUI, RGB_SAI, RGB_VAI, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
     RGB_RMOD, RGB_HUD, RGB_SAD, RGB_VAD, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_RCTL, KC_ENT, _______,     _______, KC_SPC, KC_LGUI
                                      //`--------------------------'  `--------------------------'
  )
};

#ifdef OLED_ENABLE
#include <string.h>

// Keylog buffer - circular buffer for last 10 keypresses
#define KEYLOG_SIZE 11
#define KEYCODE_NAME_LEN 8
static char    keylog_buffer[KEYLOG_SIZE][KEYCODE_NAME_LEN] = {{0}};
static uint8_t keylog_idx                                   = 0;
static uint8_t keylog_count                                 = 0;

// State tracking for OLED rendering - only render when something changes
static uint8_t last_keylog_count    = 0;
static uint8_t last_layer           = 255;  // Invalid initial value
static bool    last_caps_lock       = false;
static bool    oled_needs_update    = true; // Force initial render

// Custom key name lookup for basic keys
const char *get_key_name(uint16_t keycode) {
    switch (keycode) {
        case KC_A:
            return "a";
        case KC_B:
            return "b";
        case KC_C:
            return "c";
        case KC_D:
            return "d";
        case KC_E:
            return "e";
        case KC_F:
            return "f";
        case KC_G:
            return "g";
        case KC_H:
            return "h";
        case KC_I:
            return "i";
        case KC_J:
            return "j";
        case KC_K:
            return "k";
        case KC_L:
            return "l";
        case KC_M:
            return "m";
        case KC_N:
            return "n";
        case KC_O:
            return "o";
        case KC_P:
            return "p";
        case KC_Q:
            return "q";
        case KC_R:
            return "r";
        case KC_S:
            return "s";
        case KC_T:
            return "t";
        case KC_U:
            return "u";
        case KC_V:
            return "v";
        case KC_W:
            return "w";
        case KC_X:
            return "x";
        case KC_Y:
            return "y";
        case KC_Z:
            return "z";
        case KC_1:
            return "1";
        case KC_2:
            return "2";
        case KC_3:
            return "3";
        case KC_4:
            return "4";
        case KC_5:
            return "5";
        case KC_6:
            return "6";
        case KC_7:
            return "7";
        case KC_8:
            return "8";
        case KC_9:
            return "9";
        case KC_0:
            return "0";
        case KC_ENT:
            return "ent";
        case KC_ESC:
            return "esc";
        case KC_BSPC:
            return "bspc";
        case KC_TAB:
            return "tab";
        case KC_SPC:
            return "spc";
        case KC_MINS:
            return "-";
        case KC_EQL:
            return "=";
        case KC_LBRC:
            return "[";
        case KC_RBRC:
            return "]";
        case KC_BSLS:
            return "\\";
        case KC_NUHS:
            return "#";
        case KC_SCLN:
            return ";";
        case KC_QUOT:
            return "'";
        case KC_GRV:
            return "`";
        case KC_COMM:
            return ",";
        case KC_DOT:
            return ".";
        case KC_SLSH:
            return "/";
        case KC_LSFT:
            return "lsft";
        case KC_RSFT:
            return "rsft";
        case KC_LCTL:
            return "lctl";
        case KC_RCTL:
            return "rctl";
        case KC_LALT:
            return "lalt";
        case KC_RALT:
            return "ralt";
        case KC_LGUI:
            return "lgui";
        case KC_RGUI:
            return "rgui";
        case KC_DEL:
            return "del";
        case KC_INS:
            return "ins";
        case KC_HOME:
            return "home";
        case KC_END:
            return "end";
        case KC_PGUP:
            return "pgup";
        case KC_PGDN:
            return "pgdn";
        case KC_LEFT:
            return "left";
        case KC_RGHT:
            return "rght";
        case KC_UP:
            return "up";
        case KC_DOWN:
            return "down";
        case KC_F1:
            return "f1";
        case KC_F2:
            return "f2";
        case KC_F3:
            return "f3";
        case KC_F4:
            return "f4";
        case KC_F5:
            return "f5";
        case KC_F6:
            return "f6";
        case KC_F7:
            return "f7";
        case KC_F8:
            return "f8";
        case KC_F9:
            return "f9";
        case KC_F10:
            return "f10";
        case KC_F11:
            return "f11";
        case KC_F12:
            return "f12";
        default:
            return NULL;
    }
}

void add_keylog(uint16_t keycode) {
    const char *name = get_key_name(keycode);

    if (name == NULL) {
        // For unknown keys, just show the hex code
        snprintf(keylog_buffer[keylog_idx], KEYCODE_NAME_LEN, "%04X", keycode);
    } else {
        strncpy(keylog_buffer[keylog_idx], name, KEYCODE_NAME_LEN - 1);
        keylog_buffer[keylog_idx][KEYCODE_NAME_LEN - 1] = '\0';
    }

    // Advance index
    keylog_idx = (keylog_idx + 1) % KEYLOG_SIZE;
    if (keylog_count < KEYLOG_SIZE) {
        keylog_count++;
    }
    
    // Mark OLED as needing update since keylog changed
    oled_needs_update = true;
}

static void render_keylog_status(void) {
    // Clear the OLED first to prevent ghosting
    oled_clear();

    // Show layer name without header
    switch (get_highest_layer(layer_state)) {
        case 0:
            oled_write_P(PSTR("base\n"), false);
            break;
        case 1:
            oled_write_P(PSTR("alt\n"), false);
            break;
        case 2:
            oled_write_P(PSTR("num\n"), false);
            break;
        case 3:
            oled_write_P(PSTR("ext\n"), false);
            break;
        default:
            oled_write_P(PSTR("mod\n"), false);
            break;
    }

    // Show Caps Lock - just the word highlighted when on
    led_t led_usb_state = host_keyboard_led_state();
    oled_write_P(PSTR("caps\n"), led_usb_state.caps_lock);

    // Blank line as separator
    oled_write_P(PSTR("-----"), false);

    // Show last keys with header
    oled_write_P(PSTR("log:\n\n"), false);

    // Display keys vertically (newest first at top, oldest last at bottom)
    // Start from the most recent key and work backwards to oldest
    uint8_t newest_idx = (keylog_idx + KEYLOG_SIZE - 1) % KEYLOG_SIZE;
    for (int8_t i = 0; i < keylog_count; i++) {
        uint8_t buf_idx = (newest_idx + KEYLOG_SIZE - i) % KEYLOG_SIZE;
        oled_write(keylog_buffer[buf_idx], false);
        oled_write_P(PSTR("\n"), false);
    }
}

static void render_logo(void) {
    static const char PROGMEM qmk_logo[] = {
        0x03, 0x03, 0xf0, 0xf8, 0x3c, 0x1c, 0x0c, 0x0c, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x0c, 0x0c, 0x1c, 0x3c, 0xf8, 0xf0, 0x03, 0x03, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xfe, 0x18, 0x7c, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x80, 0xf0, 0x3e, 0x03, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x03, 0x01, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x80, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x3f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc3, 0x7e, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0xf0, 0xc0, 0x40, 0x40, 0xc0, 0x80, 0x80, 0x80, 0xff, 0x80, 0x80, 0xe7, 0x98, 0x98, 0xe7, 0x80, 0x80, 0xff, 0x80, 0x80, 0x80, 0xc0, 0x40, 0x40, 0xc0, 0xf0, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x02, 0x02, 0x06, 0x0c, 0x08, 0x18, 0x12, 0xf2, 0xa2, 0xae, 0xae, 0xa2, 0xf2, 0x12, 0x18, 0x08, 0x0c, 0x06, 0x02, 0x02, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xaa, 0xaa, 0xaa, 0xbf, 0xf1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x40, 0x5c, 0x58, 0x50, 0x40, 0x40, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0xc0, 0xc0, 0x0f, 0x1f, 0x3c, 0x38, 0x30, 0x30, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x30, 0x30, 0x38, 0x3c, 0x1f, 0x0f, 0xc0, 0xc0,
    };

    oled_write_raw_P(qmk_logo, sizeof(qmk_logo));
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (is_keyboard_master()) {
        return OLED_ROTATION_270;
    }
    return OLED_ROTATION_270;
}

bool oled_task_user(void) {
    // Get current state
    uint8_t current_layer = get_highest_layer(layer_state);
    led_t led_usb_state = host_keyboard_led_state();
    bool current_caps_lock = led_usb_state.caps_lock;
    bool oled_is_on = is_oled_on();
    
    // Check if anything changed (for master half)
    bool master_state_changed = (current_layer != last_layer) || 
                                 (current_caps_lock != last_caps_lock) || 
                                 (keylog_count != last_keylog_count);
    
    // For slave half: check if OLED just woke up (was off, now on)
    static bool last_oled_state = true;
    bool slave_needs_render = !is_keyboard_master() && (oled_is_on && !last_oled_state);
    last_oled_state = oled_is_on;
    
    // Update tracking variables
    last_layer = current_layer;
    last_caps_lock = current_caps_lock;
    last_keylog_count = keylog_count;
    
    // Only render if state changed or we need an update
    if (!master_state_changed && !oled_needs_update && !slave_needs_render) {
        return false;
    }
    
    // Clear the update flag
    oled_needs_update = false;
    
    // Check if OLED is active (not timed out) before rendering
    if (!oled_is_on) {
        return false;
    }
    
    if (is_keyboard_master()) {
        render_keylog_status();
    } else {
        render_logo();
    }
    return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        add_keylog(keycode);
    }
    return true;
}
#endif // OLED_ENABLE
