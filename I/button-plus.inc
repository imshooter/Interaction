#if defined _INC_PP_BUTTON
    #endinput
#endif
#define _INC_PP_BUTTON

#include <I\button>
#include <PawnPlus>

#include <YSI_Coding\y_hooks>

/**
 * # Header
 */

#if !defined MAX_BUTTON_KEY_LENGTH
    #define MAX_BUTTON_KEY_LENGTH (32)
#endif

#if !defined MAX_BUTTON_ATTRIBUTES
    #define MAX_BUTTON_ATTRIBUTES (16)
#endif

static
    Map:gButtonExtraData[MAX_BUTTONS]
;

/**
 * # Functions
 */

forward bool:HasButtonAnyExtraData(Button:buttonid);
forward bool:HasButtonExtraData(Button:buttonid, const key[]);
forward bool:SetButtonExtraData(Button:buttonid, const key[], value);
forward bool:GetButtonExtraData(Button:buttonid, const key[], &value);
forward bool:GetButtonExtraDataMap(Button:buttonid, &Map:map);

/**
 * # External
 */

stock bool:HasButtonAnyExtraData(Button:buttonid) {
    if (!IsValidButton(buttonid)) {
        return false;
    }

    if (!map_valid(gButtonExtraData[buttonid])) {
        return false;
    }

    return (map_size(gButtonExtraData[buttonid]) != 0);
}

stock bool:HasButtonExtraData(Button:buttonid, const key[]) {
    if (!HasButtonAnyExtraData(buttonid)) {
        return false;
    }

    return map_has_str_key(gButtonExtraData[buttonid], key);
}

stock bool:SetButtonExtraData(Button:buttonid, const key[], value) {
    if (!(0 < strlen(key) < MAX_BUTTON_KEY_LENGTH)) {
        return false;
    }

    if (!IsValidButton(buttonid)) {
        return false;
    }

    if (!map_valid(gButtonExtraData[buttonid])) {
        gButtonExtraData[buttonid] = map_new();
    }

    if (map_size(gButtonExtraData[buttonid]) >= MAX_BUTTON_ATTRIBUTES) {
        return false;
    }

    map_str_set(gButtonExtraData[buttonid], key, value);

    return true;
}

stock bool:GetButtonExtraData(Button:buttonid, const key[], &value) {
    if (!HasButtonExtraData(buttonid, key)) {
        return false;
    }

    return map_str_get_safe(gButtonExtraData[buttonid], key, value);
}

stock bool:GetButtonExtraDataMap(Button:buttonid, &Map:map) {
    if (!HasButtonAnyExtraData(buttonid)) {
        return false;
    }

    map = gButtonExtraData[buttonid];

    return true;
}

/**
 * # Calls
 */

hook OnButtonDestroy(Button:buttonid) {
    if (map_valid(gButtonExtraData[buttonid])) {
        map_delete_deep(gButtonExtraData[buttonid]);
    }

    return 1;
}