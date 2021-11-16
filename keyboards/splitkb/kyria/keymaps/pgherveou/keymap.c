/* Copyright 2019 Thomas Baart <thomas@splitkb.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef __INTELLISENSE__
#    include "../../rev1/config.h"
#    include "../../rev1/rev1.h"
#    define QMK_KEYBOARD_H "rev1.h"
#    define OLED_ENABLE "yes"
#    define ENCODER_ENABLE "yes"
#    define RGBLIGHT_ENABLE "yes"
#    define COMBO_ENABLE "yes"
#endif

#include QMK_KEYBOARD_H
#include "timer.h"

#ifdef CONSOLE_ENABLE
#    include "print.h"
#endif

enum layers {
    _QWERTY = 0,
    _NAV,
    _SYM,
    _FUNCTION,
};

enum custom_keycodes { NAV_V = SAFE_RANGE, NAV_O, NAV_CAPS };

// Aliases for readability
#define SYM MO(_SYM)
#define NAV MO(_NAV)
#define FKEYS MO(_FUNCTION)

#define CTL_TAB MT(MOD_LCTL, KC_TAB)
#define SFT_PIP MT(MOD_LSFT, KC_PIPE)
#define SFT_MIN MT(MOD_RSFT, KC_MINUS)
#define GUI_DEL MT(MOD_LGUI, KC_DEL)
#define CTL_BSP MT(MOD_RCTL, KC_BSPC)
#define SPC_NAV LT(_NAV, KC_SPC)
#define SFT_COL MT(MOD_LSFT, KC_COLN)
#define ENT_SYM LT(_SYM, KC_ENT)
#define ALT_KRG LALT(KC_RIGHT)
#define ALT_KLT LALT(KC_LEFT)

static uint16_t retro_timer;
static bool     nav_caps_on = false;

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * Base Layer: QWERTY
 */
    [_QWERTY] = LAYOUT(
     KC_GRV  , KC_Q ,  KC_W   ,  KC_E  ,   KC_R ,   KC_T ,                                          KC_Y,   KC_U ,  KC_I ,   KC_O ,  KC_P , KC_EQL,
     CTL_TAB , KC_A ,  KC_S   ,  KC_D  ,   KC_F ,   KC_G ,                                          KC_H,   KC_J ,  KC_K ,   KC_L ,KC_SCLN, KC_QUOT,
     KC_LPRN , KC_Z ,  KC_X   ,  KC_C  ,   KC_V ,   KC_B , KC_LBRC, KC_MEH,      FKEYS  , KC_RBRC,  KC_N,   KC_M ,KC_COMM, KC_DOT ,KC_SLSH, KC_BSLS,
                               KC_LGUI , KC_LALT, SFT_COL, SPC_NAV, GUI_DEL,     CTL_BSP, ENT_SYM, SFT_MIN, KC_RGUI,XXXXXXX
    ),

/*
 * Nav Layer: Media, navigation
 */
    [_NAV] = LAYOUT(
      XXXXXXX, XXXXXXX, ALT_KRG, XXXXXXX,S(G(KC_Z)),XXXXXXX,                                           KC_HOME, KC_END,  KC_PGUP, NAV_O,  G(KC_V), XXXXXXX,
      G(KC_A), KC_LCTL, KC_LGUI, KC_LALT, KC_LSFT, XXXXXXX,                                            KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, XXXXXXX, XXXXXXX,
      C(KC_B), G(KC_Z), G(KC_X), G(KC_C), NAV_V,   ALT_KLT, XXXXXXX, XXXXXXX,        XXXXXXX, C(KC_P), C(KC_N), KC_PGDN, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                 XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     S(ALT_KLT), NAV_CAPS, S(ALT_KRG), _______,XXXXXXX
    ),

/*
 * Sym Layer: Numbers and symbols
 */
    [_SYM] = LAYOUT(
      KC_GRV ,   KC_1 ,   KC_2 ,   KC_3 ,   KC_4 ,   KC_5 ,                                            KC_6 ,   KC_7 ,   KC_8 ,   KC_9 ,   KC_0 , KC_PLUS,
     KC_TILD , KC_EXLM,  KC_AT , KC_HASH,  KC_DLR, KC_PERC,                                          KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_EQL,
     KC_PIPE , KC_BSLS, KC_COLN, KC_SCLN, KC_MINS, KC_LBRC, KC_LCBR, XXXXXXX,      XXXXXXX, KC_RCBR, KC_RBRC, KC_UNDS, KC_LT,   KC_GT,   KC_SLSH, KC_QUES,
                              G(KC_KP_0), _______, _______, KC_0,    _______,      _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
    ),

/*
 * Function Layer: Function keys
 */
    [_FUNCTION] = LAYOUT(
      XXXXXXX,  KC_F9 ,  KC_F10,  KC_F11,  KC_F12, XXXXXXX,                                         XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
      XXXXXXX,  KC_F5 ,  KC_F6 ,  KC_F7 ,  KC_F8 , LGUI(LSFT(KC_4)),                                         XXXXXXX, KC_RSFT, KC_RCTL, KC_LALT, KC_RGUI, XXXXXXX,
      XXXXXXX,  KC_F1 ,  KC_F2 ,  KC_F3 ,  KC_F4 , LGUI(LALT(KC_I)), XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                 XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,              XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
    ),

// /*
//  * Layer template
//  */
//     [_LAYERINDEX] = LAYOUT(
//       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                                          XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
//       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                                          XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
//       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
//                                  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
//     ),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode == SPC_NAV && nav_caps_on && !record->event.pressed){
        nav_caps_on = false;
        unregister_code16(KC_LSHIFT);
    }

    // work around MT limitation to send shifted keycode (:)
    // see https://beta.docs.qmk.fm/using-qmk/advanced-keycodes/mod_tap#changing-tap-function
    if (keycode == SFT_COL && record->event.pressed && record->tap.count >= 1) {
      tap_code16(KC_COLN);
      return false;
    }

    switch (keycode) {
        case SPC_NAV:
        case SFT_COL:
        case ENT_SYM:
            if (record->event.pressed) {
                retro_timer = timer_read();
            }
            break;
        case NAV_V:
            if (record->event.pressed) {
                SEND_STRING(SS_LGUI(SS_TAP(X_LEFT)) SS_LGUI(SS_LSFT(SS_TAP(X_RIGHT))));
            }
            break;
        case NAV_CAPS:
            if (record->event.pressed) {
                if (nav_caps_on) {
                    unregister_code16(KC_LSHIFT);
                 } else {
                    register_code16(KC_LSHIFT);
                }
                nav_caps_on = !nav_caps_on;
            }
            break;
        case NAV_O:
            if (record->event.pressed) {
              uint8_t mod_state = get_mods();
              unregister_mods(MOD_MASK_SHIFT);
              mod_state & MOD_MASK_SHIFT
                ? SEND_STRING(SS_TAP(X_UP) SS_LGUI(SS_TAP(X_RIGHT)) SS_TAP(X_ENTER))
                : SEND_STRING(SS_LGUI(SS_TAP(X_RIGHT)) SS_TAP(X_ENTER));
              set_mods(mod_state);
            }
            break;
    }
    return true;
};

#ifdef RETRO_TAPPING_PER_KEY
bool get_retro_tapping(uint16_t keycode, keyrecord_t *record) {
    // enable retro tapping only if it happens within max 2*term
    if (timer_elapsed(retro_timer) > 2*TAPPING_TERM) {
        return false;
    }
    switch (keycode) {
        case SPC_NAV:
        case ENT_SYM:
        case SFT_MIN:
            return true;
        default:
            return false;
    }
}
#endif

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_180; }

void oled_task_user(void) {
  // QMK Logo and version information
  // clang-format off
  static const char PROGMEM qmk_logo[] = {
    0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,
    0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,
    0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,0};
  // clang-format on

    if (is_keyboard_master()) {

        oled_write_P(qmk_logo, false);
        oled_write_P(PSTR("Kyria rev1.0\n\n"), false);

        // Host Keyboard Layer Status
        oled_write_P(PSTR("Layer: "), false);
        switch (get_highest_layer(layer_state | default_layer_state)) {
            case _QWERTY:
                oled_write_P(PSTR("QWERTY\n"), false);
                break;
            case _NAV:
                oled_write_P(PSTR("Nav\n"), false);
                break;
            case _SYM:
                oled_write_P(PSTR("Sym\n"), false);
                break;
            case _FUNCTION:
                oled_write_P(PSTR("Function\n"), false);
                break;
            default:
                oled_write_P(PSTR("Undefined\n"), false);
        }

        uint8_t mods = get_mods() | get_weak_mods();
        oled_write_P((mods & MOD_MASK_GUI) ? PSTR("GUI ") : PSTR("    "), false);
        oled_write_P((mods & MOD_MASK_ALT) ? PSTR("ALT ") : PSTR("    "), false);
        oled_write_P((mods & MOD_MASK_CTRL) ? PSTR("CTRL ") : PSTR("     "), false);
        oled_write_P((mods & MOD_MASK_SHIFT) ? PSTR("SHFT ") : PSTR("     "), false);
        oled_write_P(PSTR("\n"), false);

    } else {
        oled_write_raw_P(qmk_logo, false);
    }
}
#endif

#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {
    switch (get_highest_layer(layer_state | default_layer_state)) {
        case _QWERTY:
            if (index == 0) {
                if (get_mods() & MOD_MASK_GUI) {
                    if (clockwise) {
                        tap_code16(KC_TAB);
                    } else {
                        tap_code16(S(KC_TAB));
                    }
                } else {
                    if (clockwise) {
                        tap_code16(KC_RIGHT);
                    } else {
                        tap_code16(KC_LEFT);
                    }
                }

            } else if (index == 1) {
                // Page up/Page down
                if (clockwise) {
                    tap_code(KC_PGDN);
                } else {
                    tap_code(KC_PGUP);
                }
            }
            break;
        case _NAV:
            if (index == 1) {
                if (clockwise) {
                    tap_code16(G(A(KC_RIGHT)));
                } else {
                    tap_code16(G(A(KC_LEFT)));
                }
            }
            break;

        case _FUNCTION:
            if (index == 0) {
                if (clockwise) {
                    tap_code16(KC_UP);
                } else {
                    tap_code16(KC_DOWN);
                }
            }
            break;

        case _SYM:
            if (index == 0) {
                if (clockwise) {
                    tap_code16(G(KC_EQL));
                } else {
                    tap_code16(G(KC_MINS));
                }
            }
            break;

        default:
            break;
    }
    return false;
}
#endif

#ifdef RGBLIGHT_LAYERS
const rgblight_segment_t PROGMEM my_qwerty_layer[] = RGBLIGHT_LAYER_SEGMENTS({0, 20, HSV_OFF});
const rgblight_segment_t PROGMEM my_nav_layer[]    = RGBLIGHT_LAYER_SEGMENTS({0, 20, HSV_AZURE});
const rgblight_segment_t PROGMEM my_sym_layer[]    = RGBLIGHT_LAYER_SEGMENTS({0, 20, HSV_RED});
const rgblight_segment_t PROGMEM my_shift_layer[]  = RGBLIGHT_LAYER_SEGMENTS({0, 20, HSV_GREEN});
const rgblight_segment_t PROGMEM my_ctrl_layer[]   = RGBLIGHT_LAYER_SEGMENTS({4, 1, HSV_ORANGE}, {14, 1, HSV_ORANGE});
const rgblight_segment_t PROGMEM my_alt_layer[]    = RGBLIGHT_LAYER_SEGMENTS({5, 1, HSV_GOLD}, {15, 1, HSV_GOLD});
const rgblight_segment_t PROGMEM my_gui_layer[]    = RGBLIGHT_LAYER_SEGMENTS({9, 1, HSV_PINK}, {19, 1, HSV_PINK});
const rgblight_segment_t PROGMEM my_fn_layer[]     = RGBLIGHT_LAYER_SEGMENTS({0, 20, HSV_PURPLE});

const rgblight_segment_t *const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(my_qwerty_layer, my_nav_layer, my_sym_layer, my_fn_layer, my_shift_layer, my_ctrl_layer, my_alt_layer, my_gui_layer);

layer_state_t layer_state_set_user(layer_state_t state) {
    rgblight_set_layer_state(0, layer_state_cmp(state, _QWERTY));
    rgblight_set_layer_state(1, layer_state_cmp(state, _NAV));
    rgblight_set_layer_state(2, layer_state_cmp(state, _SYM));
    rgblight_set_layer_state(3, layer_state_cmp(state, _FUNCTION));
    return state;
}

void matrix_scan_user(void) {
    uint8_t mods = mod_config(get_mods());
    rgblight_set_layer_state(4, mods & MOD_MASK_SHIFT);
    rgblight_set_layer_state(5, mods & MOD_MASK_CTRL);
    rgblight_set_layer_state(6, mods & MOD_MASK_ALT);
    rgblight_set_layer_state(7, mods & MOD_MASK_GUI);
}

void keyboard_post_init_user(void) {
    rgblight_layers = my_rgb_layers;

    // rgblight_set_layer_state(3, true);
    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
}
#endif

#ifdef COMBO_ENABLE
const uint16_t PROGMEM COMBO_JK_KEYS[] = {KC_J, KC_K, COMBO_END};
const uint16_t PROGMEM COMBO_ER_KEYS[] = {KC_E, KC_R, COMBO_END};
const uint16_t PROGMEM COMBO_DF_KEYS[] = {KC_D, KC_F, COMBO_END};

enum combo_events { COMBO_JK, COMBO_ER, COMBO_DF, COMBO_LENGTH };
uint16_t COMBO_LEN = COMBO_LENGTH;

combo_t key_combos[] = {
    [COMBO_JK] = COMBO_ACTION(COMBO_JK_KEYS),
    [COMBO_ER] = COMBO_ACTION(COMBO_ER_KEYS),
    [COMBO_DF] = COMBO_ACTION(COMBO_DF_KEYS),
};

void process_combo_event(uint16_t combo_index, bool pressed) {
    switch (combo_index) {
        // jk -> escape
        case COMBO_JK:
            if (pressed) {
                tap_code16(KC_ESC);
            }
            break;
        // er -> ctrl + r
        case COMBO_ER:
            if (pressed) {
                tap_code16(C(KC_R));
            }
            break;
        // er -> ctrl+c
        case COMBO_DF:
            if (pressed) {
                tap_code16(C(KC_C));
            }
            break;
    }
}

#endif
