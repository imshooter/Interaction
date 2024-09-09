#if defined _INC_BUTTON
    #endinput
#endif
#define _INC_BUTTON

#include <streamer>

#include <YSI_Data\y_iterate>
#include <YSI_Coding\y_va>
#include <YSI_Coding\y_hooks>

/**
 * # Header
 */

#if !defined MAX_BUTTONS
    #define MAX_BUTTONS (Button:8192)
#endif

#if !defined MAX_BUTTON_POPUP_LENGTH
    #define MAX_BUTTON_POPUP_LENGTH (128)
#endif

#if !defined MAX_PLAYER_DYN_AREAS
    #define MAX_PLAYER_DYN_AREAS (256)
#endif

#if !defined MAX_AREA_BUTTONS
    #define MAX_AREA_BUTTONS (8)
#endif

#if !defined BUTTON_DEFAULT_KEY
    #define BUTTON_DEFAULT_KEY (KEY_SECONDARY_ATTACK)
#endif

#if !defined BUTTON_DEFAULT_DISTANCE
    #define BUTTON_DEFAULT_DISTANCE (4.0)
#endif

#if !defined BUTTON_STREAMER_IDENTIFIER
    #define BUTTON_STREAMER_IDENTIFIER (65536)
#endif

#define INVALID_BUTTON_ID (Button:-1)

static enum E_BUTTON_DATA {
    KEY:E_BUTTON_KEYS,
    E_BUTTON_PRIORITY,
    E_BUTTON_TEXT[MAX_BUTTON_POPUP_LENGTH],

    Float:E_BUTTON_X,
    Float:E_BUTTON_Y,
    Float:E_BUTTON_Z,
    Float:E_BUTTON_SIZE,

    E_BUTTON_WORLD_ID,
    E_BUTTON_INTERIOR_ID,

    STREAMER_TAG_AREA:E_BUTTON_AREA_ID,
    STREAMER_TAG_3D_TEXT_LABEL:E_BUTTON_TEXT3D_ID
};

static
    gButtonData[MAX_BUTTONS][E_BUTTON_DATA],
    gInButtonCount[MAX_PLAYERS char]
;

const static
    BUTTON_ITER_SIZE = _:MAX_BUTTONS
;

new
    Iterator:Button<Button:BUTTON_ITER_SIZE>
;

/**
 * # Functions
 */

forward Button:CreateButton(Float:x, Float:y, Float:z, KEY:buttonKey = BUTTON_DEFAULT_KEY, Float:size = 1.0, worldid = -1, interiorid = -1, priority = 0);
forward DestroyButton(Button:buttonid);
forward bool:IsValidButton(Button:buttonid);
forward SetButtonPos(Button:buttonid, Float:x, Float:y, Float:z);
forward GetButtonPos(Button:buttonid, &Float:x, &Float:y, &Float:z);
forward SetButtonPriority(Button:buttonid, priority);
forward GetButtonPriority(Button:buttonid);
forward SetButtonSize(Button:buttonid, Float:size);
forward GetButtonSize(Button:buttonid, &Float:size);
forward SetButtonVirtualWorld(Button:buttonid, worldid);
forward GetButtonVirtualWorld(Button:buttonid);
forward SetButtonInterior(Button:buttonid, interiorid);
forward GetButtonInterior(Button:buttonid);
forward SetButtonPopupText(Button:buttonid, const format[], OPEN_MP_TAGS:...);
forward GetButtonPopupText(Button:buttonid, output[], len = sizeof (output));
forward SetButtonTextLabel(Button:buttonid, const format[], colour = 0xFFFFFFFF, Float:distance = BUTTON_DEFAULT_DISTANCE, bool:testLOS = true, Float:fOffsetZ = 0.0, OPEN_MP_TAGS:...);
forward UpdateButtonTextLabel(Button:buttonid, const format[], colour = 0xFFFFFFFF, OPEN_MP_TAGS:...);
forward DestroyButtonTextLabel(Button:buttonid);
forward GetButtonDynamicArea(Button:buttonid, &STREAMER_TAG_AREA:areaid);
forward GetButtonDynamic3DTextLabel(Button:buttonid, &STREAMER_TAG_3D_TEXT_LABEL:textid);
forward bool:IsPlayerInAnyButton(playerid);
forward GetPlayerButton(playerid, &Button:buttonid);
forward GetPlayerNearbyButtons(playerid, Button:buttons[MAX_AREA_BUTTONS] = { INVALID_BUTTON_ID, ... }, &count = 0);

/**
 * # Events
 */

/**
 * # OnButtonCreate
 *
 * # Returns
 *   - This callback does not handle returns.
 */

forward OnButtonCreate(Button:buttonid);

/**
 * # OnButtonDestroy
 *
 * # Returns
 *   - This callback does not handle returns.
 */

forward OnButtonDestroy(Button:buttonid);

/**
 * # OnButtonPress
 *
 * # Returns
 *   - `0`: (Continue) Continues subsequent calls to `OnPlayerKeyStateChange`.
 *   - `1`: (OK) Breaks subsequent calls to `OnPlayerKeyStateChange`.
 */

forward OnButtonPress(playerid, Button:buttonid);

/**
 * # OnButtonRelease
 *
 * # Returns
 *   - `0`: (Continue) Continues subsequent calls to `OnPlayerKeyStateChange`.
 *   - `1`: (OK) Breaks subsequent calls to `OnPlayerKeyStateChange`.
 */

forward OnButtonRelease(playerid, Button:buttonid);

/**
 * # OnPlayerEnterButtonArea
 *
 * # Returns
 *   - `0`: (Continue) Continues subsequent calls to `OnPlayerEnterDynamicArea`.
 *   - `1`: (OK) Breaks subsequent calls to `OnPlayerEnterDynamicArea`.
 */

forward OnPlayerEnterButtonArea(playerid, Button:buttonid);

/**
 * # OnPlayerLeaveButtonArea
 *
 * # Returns
 *   - `0`: (Continue) Continues subsequent calls to `OnPlayerLeaveDynamicArea`.
 *   - `1`: (OK) Breaks subsequent calls to `OnPlayerLeaveDynamicArea`.
 */

forward OnPlayerLeaveButtonArea(playerid, Button:buttonid);

/**
 * # API
 */

stock Button:CreateButton(Float:x, Float:y, Float:z, KEY:buttonKey = BUTTON_DEFAULT_KEY, Float:size = 1.0, worldid = -1, interiorid = -1, priority = 0) {
    new const
        Button:buttonid = Button:Iter_Alloc(Button)
    ;

    if (_:buttonid == INVALID_ITERATOR_SLOT) {
        return INVALID_BUTTON_ID;
    }

    gButtonData[buttonid][E_BUTTON_KEYS] |= buttonKey;
    gButtonData[buttonid][E_BUTTON_PRIORITY] = priority;

    gButtonData[buttonid][E_BUTTON_X] = x;
    gButtonData[buttonid][E_BUTTON_Y] = y;
    gButtonData[buttonid][E_BUTTON_Z] = z;
    gButtonData[buttonid][E_BUTTON_SIZE] = size;
    
    gButtonData[buttonid][E_BUTTON_WORLD_ID] = worldid;
    gButtonData[buttonid][E_BUTTON_INTERIOR_ID] = interiorid;

    gButtonData[buttonid][E_BUTTON_AREA_ID] = CreateDynamicSphere(x, y, z, size, worldid, interiorid);

    new
        data[2]
    ;

    data[0] = BUTTON_STREAMER_IDENTIFIER;
    data[1] = _:buttonid;

    Streamer_SetArrayData(STREAMER_TYPE_AREA, gButtonData[buttonid][E_BUTTON_AREA_ID], E_STREAMER_EXTRA_ID, data);

    CallLocalFunction("OnButtonCreate", "i", _:buttonid);

    return buttonid;
}

stock DestroyButton(Button:buttonid) {
    if (!IsValidButton(buttonid)) {
        return 1;
    }

    DestroyDynamicArea(gButtonData[buttonid][E_BUTTON_AREA_ID]);
    DestroyDynamic3DTextLabel(gButtonData[buttonid][E_BUTTON_TEXT3D_ID]);

    static const
        BUTTON_DATA_CLEAN[E_BUTTON_DATA]
    ;

    gButtonData[buttonid] = BUTTON_DATA_CLEAN;

    Iter_Remove(Button, buttonid);

    CallLocalFunction("OnButtonDestroy", "i", _:buttonid);

    return 0;
}

stock bool:IsValidButton(Button:buttonid) {
    if (!(0 <= _:buttonid < BUTTON_ITER_SIZE)) {
        return false;
    }

    return Iter_Contains(Button, buttonid);
}

stock SetButtonPos(Button:buttonid, Float:x, Float:y, Float:z) {
    if (!IsValidButton(buttonid)) {
        return 1;
    }

    Streamer_SetFloatData(STREAMER_TYPE_AREA, gButtonData[buttonid][E_BUTTON_AREA_ID], E_STREAMER_X, x);
    Streamer_SetFloatData(STREAMER_TYPE_AREA, gButtonData[buttonid][E_BUTTON_AREA_ID], E_STREAMER_Y, y);
    Streamer_SetFloatData(STREAMER_TYPE_AREA, gButtonData[buttonid][E_BUTTON_AREA_ID], E_STREAMER_Z, z);

    if (IsValidDynamic3DTextLabel(gButtonData[buttonid][E_BUTTON_TEXT3D_ID])) {
        Streamer_SetFloatData(STREAMER_TYPE_3D_TEXT_LABEL, gButtonData[buttonid][E_BUTTON_TEXT3D_ID], E_STREAMER_X, x);
        Streamer_SetFloatData(STREAMER_TYPE_3D_TEXT_LABEL, gButtonData[buttonid][E_BUTTON_TEXT3D_ID], E_STREAMER_Y, y);
        Streamer_SetFloatData(STREAMER_TYPE_3D_TEXT_LABEL, gButtonData[buttonid][E_BUTTON_TEXT3D_ID], E_STREAMER_Z, z);
    }

    gButtonData[buttonid][E_BUTTON_X] = x;
    gButtonData[buttonid][E_BUTTON_Y] = y;
    gButtonData[buttonid][E_BUTTON_Z] = z;

    return 0;
}

stock GetButtonPos(Button:buttonid, &Float:x, &Float:y, &Float:z) {
    if (!IsValidButton(buttonid)) {
        return 1;
    }

    x = gButtonData[buttonid][E_BUTTON_X];
    y = gButtonData[buttonid][E_BUTTON_Y];
    z = gButtonData[buttonid][E_BUTTON_Z];

    return 0;
}

stock SetButtonPriority(Button:buttonid, priority) {
    if (!IsValidButton(buttonid)) {
        return 1;
    }

    gButtonData[buttonid][E_BUTTON_PRIORITY] = priority;

    return 0;
}

stock GetButtonPriority(Button:buttonid) {
    if (!IsValidButton(buttonid)) {
        return 0;
    }

    return gButtonData[buttonid][E_BUTTON_PRIORITY];
}

stock SetButtonSize(Button:buttonid, Float:size) {
    if (!IsValidButton(buttonid)) {
        return 1;
    }

    Streamer_SetFloatData(STREAMER_TYPE_AREA, gButtonData[buttonid][E_BUTTON_AREA_ID], E_STREAMER_SIZE, size);
        
    gButtonData[buttonid][E_BUTTON_SIZE] = size;

    return 0;
}

stock GetButtonSize(Button:buttonid, &Float:size) {
    if (!IsValidButton(buttonid)) {
        return 1;
    }
        
    size = gButtonData[buttonid][E_BUTTON_SIZE];

    return 0;
}

stock SetButtonVirtualWorld(Button:buttonid, worldid) {
    if (!IsValidButton(buttonid)) {
        return 1;
    }

    if (gButtonData[buttonid][E_BUTTON_WORLD_ID] != worldid) {
        Streamer_SetIntData(STREAMER_TYPE_AREA, gButtonData[buttonid][E_BUTTON_AREA_ID], E_STREAMER_WORLD_ID, worldid);

        if (IsValidDynamic3DTextLabel(gButtonData[buttonid][E_BUTTON_TEXT3D_ID])) {
            Streamer_SetIntData(STREAMER_TYPE_3D_TEXT_LABEL, gButtonData[buttonid][E_BUTTON_TEXT3D_ID], E_STREAMER_WORLD_ID, worldid);
        }

        gButtonData[buttonid][E_BUTTON_WORLD_ID] = worldid;
    }

    return 0;
}

stock GetButtonVirtualWorld(Button:buttonid) {
    if (!IsValidButton(buttonid)) {
        return -1;
    }

    return gButtonData[buttonid][E_BUTTON_WORLD_ID];
}

stock SetButtonInterior(Button:buttonid, interiorid) {
    if (!IsValidButton(buttonid)) {
        return 1;
    }

    if (gButtonData[buttonid][E_BUTTON_INTERIOR_ID] != interiorid) {
        Streamer_SetIntData(STREAMER_TYPE_AREA, gButtonData[buttonid][E_BUTTON_AREA_ID], E_STREAMER_INTERIOR_ID, interiorid);

        if (IsValidDynamic3DTextLabel(gButtonData[buttonid][E_BUTTON_TEXT3D_ID])) {
            Streamer_SetIntData(STREAMER_TYPE_3D_TEXT_LABEL, gButtonData[buttonid][E_BUTTON_TEXT3D_ID], E_STREAMER_INTERIOR_ID, interiorid);
        }

        gButtonData[buttonid][E_BUTTON_INTERIOR_ID] = interiorid;
    }

    return 0;
}

stock GetButtonInterior(Button:buttonid) {
    if (!IsValidButton(buttonid)) {
        return -1;
    }

    return gButtonData[buttonid][E_BUTTON_INTERIOR_ID];
}

stock SetButtonPopupText(Button:buttonid, const format[], OPEN_MP_TAGS:...) {
    if (!IsValidButton(buttonid)) {
        return 1;
    }

    new
        buffer[MAX_BUTTON_POPUP_LENGTH]
    ;

    Format(
        buffer,
        _,
        format,
        ___(2)
    );

    strpack(gButtonData[buttonid][E_BUTTON_TEXT], buffer);

    return 0;
}

stock GetButtonPopupText(Button:buttonid, output[], len = sizeof (output)) {
    if (!IsValidButton(buttonid)) {
        return 1;
    }

    strunpack(output, gButtonData[buttonid][E_BUTTON_TEXT], len);

    return 0;
}

stock SetButtonTextLabel(Button:buttonid, const format[], colour = 0xFFFFFFFF, Float:distance = BUTTON_DEFAULT_DISTANCE, bool:testLOS = true, Float:fOffsetZ = 0.0, OPEN_MP_TAGS:...) {
    if (!IsValidButton(buttonid)) {
        return 1;
    }

    if (IsValidDynamic3DTextLabel(gButtonData[buttonid][E_BUTTON_TEXT3D_ID])) {
        DestroyDynamic3DTextLabel(gButtonData[buttonid][E_BUTTON_TEXT3D_ID]);
    }

    new
        buffer[MAX_BUTTON_POPUP_LENGTH]
    ;

    Format(
        buffer,
        _,
        format,
        ___(6)
    );

    gButtonData[buttonid][E_BUTTON_TEXT3D_ID] = CreateDynamic3DTextLabel(
        buffer,
        colour,
        gButtonData[buttonid][E_BUTTON_X],
        gButtonData[buttonid][E_BUTTON_Y],
        gButtonData[buttonid][E_BUTTON_Z] + fOffsetZ,
        distance,
        _,
        _,
        testLOS,
        gButtonData[buttonid][E_BUTTON_WORLD_ID],
        gButtonData[buttonid][E_BUTTON_INTERIOR_ID],
        _,
        distance
    );

    return 0;
}

stock UpdateButtonTextLabel(Button:buttonid, const format[], colour = 0xFFFFFFFF, OPEN_MP_TAGS:...) {
    if (!IsValidButton(buttonid)) {
        return 1;
    }

    if (!IsValidDynamic3DTextLabel(gButtonData[buttonid][E_BUTTON_TEXT3D_ID])) {
        return 2;
    }

    new
        buffer[MAX_BUTTON_POPUP_LENGTH]
    ;

    Format(
        buffer,
        _,
        format,
        ___(3)
    );

    UpdateDynamic3DTextLabelText(gButtonData[buttonid][E_BUTTON_TEXT3D_ID], colour, buffer);

    return 0;
}

stock DestroyButtonTextLabel(Button:buttonid) {
    if (!IsValidButton(buttonid)) {
        return 1;
    }

    if (!IsValidDynamic3DTextLabel(gButtonData[buttonid][E_BUTTON_TEXT3D_ID])) {
        return 2;
    }

    DestroyDynamic3DTextLabel(gButtonData[buttonid][E_BUTTON_TEXT3D_ID]);
    gButtonData[buttonid][E_BUTTON_TEXT3D_ID] = STREAMER_TAG_3D_TEXT_LABEL:INVALID_3DTEXT_ID;

    return 0;
}

stock GetButtonDynamicArea(Button:buttonid, &STREAMER_TAG_AREA:areaid) {
    if (!IsValidButton(buttonid)) {
        return 1;
    }

    areaid = gButtonData[buttonid][E_BUTTON_AREA_ID];

    return 0;
}

stock GetButtonDynamic3DTextLabel(Button:buttonid, &STREAMER_TAG_3D_TEXT_LABEL:textid) {
    if (!IsValidButton(buttonid)) {
        return 1;
    }

    if (!IsValidDynamic3DTextLabel(gButtonData[buttonid][E_BUTTON_TEXT3D_ID])) {
        return 2;
    }

    textid = gButtonData[buttonid][E_BUTTON_TEXT3D_ID];

    return 0;
}

stock bool:IsPlayerInAnyButton(playerid) {
    if (!IsPlayerConnected(playerid)) {
        return false;
    }

    return (gInButtonCount{playerid} != 0);
}

stock GetPlayerButton(playerid, &Button:buttonid) {
    if (!IsPlayerConnected(playerid)) {
        return 1;
    }

    if (gInButtonCount{playerid} == 0) {
        return 2;
    }

    new
        STREAMER_TAG_AREA:areas[MAX_PLAYER_DYN_AREAS],
        data[2],
        Button:b,
        p,
        last = -1
    ;

    for (new i, size = GetPlayerDynamicAreas(playerid, areas); i != size; ++i) {
        Streamer_GetArrayData(STREAMER_TYPE_AREA, areas[i], E_STREAMER_EXTRA_ID, data);
    
        if (data[0] == BUTTON_STREAMER_IDENTIFIER) {
            b = Button:data[1];
            p = gButtonData[b][E_BUTTON_PRIORITY];

            if (p > last) {
                last = p;
                buttonid = b;
            }
        }
    }

    return (last == -1) ? 3 : 0;
}

stock GetPlayerNearbyButtons(playerid, Button:buttons[MAX_AREA_BUTTONS] = { INVALID_BUTTON_ID, ... }, &count = 0) {
    if (!IsPlayerConnected(playerid)) {
        return 1;
    }

    if (gInButtonCount{playerid} == 0) {
        return 2;
    }

    new
        STREAMER_TAG_AREA:areas[MAX_PLAYER_DYN_AREAS],
        data[2],
        Button:b,
        p,
        priorities[MAX_AREA_BUTTONS]
    ;

    count = 0;

    for (new i, x, size = GetPlayerDynamicAreas(playerid, areas); i != size && count != MAX_AREA_BUTTONS; ++i) {
        Streamer_GetArrayData(STREAMER_TYPE_AREA, areas[i], E_STREAMER_EXTRA_ID, data);

        if (data[0] == BUTTON_STREAMER_IDENTIFIER) {
            b = Button:data[1];
            p = gButtonData[b][E_BUTTON_PRIORITY];

            for (x = count++; x != 0 && priorities[x - 1] > p; --x) {
                buttons[x] = buttons[x - 1];
                priorities[x] = priorities[x - 1];
            }

            buttons[x] = b;
            priorities[x] = p;
        }
    }

    return (count == 0) ? 3 : 0;
}

/**
 * # Calls
 */

hook OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys) {
    if (gInButtonCount{playerid} == 0) {
        return 1;
    }

    new
        Button:buttonid
    ;

    if (GetPlayerButton(playerid, buttonid)) {
        return 1;
    }

    if (newkeys & gButtonData[buttonid][E_BUTTON_KEYS]) {
        if (CallLocalFunction("OnButtonPress", "ii", playerid, _:buttonid)) {
            return ~1;
        }

        return 1;
    }

    if (oldkeys & gButtonData[buttonid][E_BUTTON_KEYS]) {
        if (CallLocalFunction("OnButtonRelease", "ii", playerid, _:buttonid)) {
            return ~1;
        }
    }

    return 1;
}

hook OnPlayerEnterDynamicArea(playerid, STREAMER_TAG_AREA:areaid) {
    if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT && gInButtonCount{playerid} != (MAX_AREA_BUTTONS - 1)) {
        new
            data[2]
        ;
        
        Streamer_GetArrayData(STREAMER_TYPE_AREA, areaid, E_STREAMER_EXTRA_ID, data);

        if (data[0] == BUTTON_STREAMER_IDENTIFIER) {
            gInButtonCount{playerid}++;

            if (CallLocalFunction("OnPlayerEnterButtonArea", "ii", playerid, data[1])) {
                return ~1;
            }
        }
    }

    return 1;
}

hook OnPlayerLeaveDynamicArea(playerid, STREAMER_TAG_AREA:areaid) {
    if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT && gInButtonCount{playerid} != 0) {
        new
            data[2]
        ;
        
        Streamer_GetArrayData(STREAMER_TYPE_AREA, areaid, E_STREAMER_EXTRA_ID, data);

        if (data[0] == BUTTON_STREAMER_IDENTIFIER) {
            gInButtonCount{playerid}--;

            if (CallLocalFunction("OnPlayerLeaveButtonArea", "ii", playerid, data[1])) {
                return ~1;
            }
        }
    }

    return 1;
}

hook OnPlayerDisconnect(playerid, reason) {
    gInButtonCount{playerid} = 0;
    
    return 1;
}