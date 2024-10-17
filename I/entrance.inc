#if defined _INC_ENTRANCE
    #endinput
#endif
#define _INC_ENTRANCE

#include <I\button>
#include <I\button-plus>

#include <YSI_Data\y_iterate>
#include <YSI_Coding\y_hooks>

/**
 * # Header
 */

#if !defined MAX_ENTRANCES
    #define MAX_ENTRANCES (Entrance:1024)
#endif

#if !defined ENTRANCE_DEFAULT_BUTTON_KEY
    #define ENTRANCE_DEFAULT_BUTTON_KEY (KEY_SECONDARY_ATTACK)
#endif

#if !defined ENTRANCE_DEFAULT_BUTTON_PRIORITY
    #define ENTRANCE_DEFAULT_BUTTON_PRIORITY (0)
#endif

#if !defined ENTRANCE_DEFAULT_FORWARD_OFFSET
    #define ENTRANCE_DEFAULT_FORWARD_OFFSET (1.0)
#endif

#define INVALID_ENTRANCE_ID (Entrance:-1)

static const
    BTN_ATTRIBUTE_EXT_ENTRANCE_ID[] = "BTN_ATTRIBUTE_EXT_ENTRANCE_ID",
    BTN_ATTRIBUTE_INT_ENTRANCE_ID[] = "BTN_ATTRIBUTE_INT_ENTRANCE_ID"
;

static enum E_ENTRANCE_DATA {
    Float:E_ENTRANCE_EXT_X,
    Float:E_ENTRANCE_EXT_Y,
    Float:E_ENTRANCE_EXT_Z,
    Float:E_ENTRANCE_EXT_A,

    E_ENTRANCE_EXT_WORLD_ID,
    E_ENTRANCE_EXT_INTERIOR_ID,
    
    Float:E_ENTRANCE_INT_X,
    Float:E_ENTRANCE_INT_Y,
    Float:E_ENTRANCE_INT_Z,
    Float:E_ENTRANCE_INT_A,

    E_ENTRANCE_INT_WORLD_ID,
    E_ENTRANCE_INT_INTERIOR_ID,

    Button:E_ENTRANCE_EXT_BUTTON_ID,
    Button:E_ENTRANCE_INT_BUTTON_ID
};

static
    gEntranceData[MAX_ENTRANCES][E_ENTRANCE_DATA]
;

const static
    ENTRANCE_ITER_SIZE = _:MAX_ENTRANCES
;

new
    Iterator:Entrance<Entrance:ENTRANCE_ITER_SIZE>
;

/**
 * # Functions
 */

forward Entrance:CreateEntrance(Float:extX, Float:extY, Float:extZ, Float:extA, extWorld, extInterior, Float:intX, Float:intY, Float:intZ, Float:intA, intWorld, intInterior, KEY:buttonKey = ENTRANCE_DEFAULT_BUTTON_KEY, priority = ENTRANCE_DEFAULT_BUTTON_PRIORITY);
forward bool:IsValidEntrance(Entrance:entranceid);

forward bool:GetEntranceExteriorPos(Entrance:entranceid, &Float:x, &Float:y, &Float:z);
forward bool:GetEntranceExteriorFacingAngle(Entrance:entranceid, &Float:a);
forward GetEntranceExteriorVirtualWorld(Entrance:entranceid);
forward GetEntranceExteriorInterior(Entrance:entranceid);
forward bool:GetEntranceExteriorButton(Entrance:entranceid, &Button:buttonid);
forward bool:HasButtonEntranceExterior(Button:buttonid);
forward bool:GetButtonEntranceExterior(Button:buttonid, &Entrance:entranceid);
forward bool:SetPlayerEntranceExterior(playerid, Entrance:entranceid, Float:offset = ENTRANCE_DEFAULT_FORWARD_OFFSET);
forward bool:GetPlayerEntranceExterior(playerid, &Entrance:entranceid);
forward bool:GetPlayerEntranceExteriorButton(playerid, &Button:buttonid);

forward bool:GetEntranceInteriorPos(Entrance:entranceid, &Float:x, &Float:y, &Float:z);
forward bool:GetEntranceInteriorFacingAngle(Entrance:entranceid, &Float:a);
forward GetEntranceInteriorVirtualWorld(Entrance:entranceid);
forward GetEntranceInteriorInterior(Entrance:entranceid);
forward bool:GetEntranceInteriorButton(Entrance:entranceid, &Button:buttonid);
forward bool:HasButtonEntranceInterior(Button:buttonid);
forward bool:GetButtonEntranceInterior(Button:buttonid, &Entrance:entranceid);
forward bool:SetPlayerEntranceInterior(playerid, Entrance:entranceid, Float:offset = ENTRANCE_DEFAULT_FORWARD_OFFSET);
forward bool:GetPlayerEntranceInterior(playerid, &Entrance:entranceid);
forward bool:GetPlayerEntranceInteriorButton(playerid, &Button:buttonid);

/**
 * # Events
 */

forward OnPlayerEnterExteriorArea(playerid, Entrance:entranceid);

/**
 * # OnPlayerEnterExteriorArea
 *
 * # Returns
 *   - `0`: (Continue) Continues subsequent calls to `OnPlayerEnterButtonArea`.
 *   - `1`: (OK) Breaks subsequent calls to `OnPlayerEnterButtonArea`.
 */

forward OnPlayerEnterInteriorArea(playerid, Entrance:entranceid);

/**
 * # OnPlayerEnterInteriorArea
 *
 * # Returns
 *   - `0`: (Continue) Continues subsequent calls to `OnPlayerEnterButtonArea`.
 *   - `1`: (OK) Breaks subsequent calls to `OnPlayerEnterButtonArea`.
 */

forward OnPlayerLeaveExteriorArea(playerid, Entrance:entranceid);

/**
 * # OnPlayerLeaveExteriorArea
 *
 * # Returns
 *   - `0`: (Continue) Continues subsequent calls to `OnPlayerLeaveButtonArea`.
 *   - `1`: (OK) Breaks subsequent calls to `OnPlayerLeaveButtonArea`.
 */

forward OnPlayerLeaveInteriorArea(playerid, Entrance:entranceid);

/**
 * # OnPlayerLeaveInteriorArea
 *
 * # Returns
 *   - `0`: (Continue) Continues subsequent calls to `OnPlayerLeaveButtonArea`.
 *   - `1`: (OK) Breaks subsequent calls to `OnPlayerLeaveButtonArea`.
 */

forward OnPlayerEnterEntrance(playerid, Entrance:entranceid);

/**
 * # OnPlayerEnterEntrance
 *
 * # Returns
 *   - `0`: Allows the player to be set inside the entrance.
 *   - `1`: Prevents the player from being set inside the entrance.
 */

forward OnPlayerLeaveEntrance(playerid, Entrance:entranceid);

/**
 * # OnPlayerLeaveEntrance
 *
 * # Returns
 *   - `0`: Allows the player to be set outside the entrance.
 *   - `1`: Prevents the player from being set outside the entrance.
 */

forward OnPlayerEnteredEntrance(playerid, Entrance:entranceid);

/**
 * # OnPlayerEnteredEntrance
 *
 * # Returns
 *   - This callback does not handle returns.
 */

forward OnPlayerLeftEntrance(playerid, Entrance:entranceid);

/**
 * # OnPlayerLeftEntrance
 *
 * # Returns
 *   - This callback does not handle returns.
 */

/**
 * # External
 */

stock Entrance:CreateEntrance(Float:extX, Float:extY, Float:extZ, Float:extA, extWorld, extInterior, Float:intX, Float:intY, Float:intZ, Float:intA, intWorld, intInterior, KEY:buttonKey = ENTRANCE_DEFAULT_BUTTON_KEY, priority = ENTRANCE_DEFAULT_BUTTON_PRIORITY) {
    new const
        Entrance:entranceid = Entrance:Iter_Alloc(Entrance)
    ;

    if (_:entranceid == INVALID_ITERATOR_SLOT) {
        return INVALID_ENTRANCE_ID;
    }

    gEntranceData[entranceid][E_ENTRANCE_EXT_X] = extX;
    gEntranceData[entranceid][E_ENTRANCE_EXT_Y] = extY;
    gEntranceData[entranceid][E_ENTRANCE_EXT_Z] = extZ;
    gEntranceData[entranceid][E_ENTRANCE_EXT_A] = extA;
    gEntranceData[entranceid][E_ENTRANCE_EXT_WORLD_ID] = extWorld;
    gEntranceData[entranceid][E_ENTRANCE_EXT_INTERIOR_ID] = extInterior;

    gEntranceData[entranceid][E_ENTRANCE_INT_X] = intX;
    gEntranceData[entranceid][E_ENTRANCE_INT_Y] = intY;
    gEntranceData[entranceid][E_ENTRANCE_INT_Z] = intZ;
    gEntranceData[entranceid][E_ENTRANCE_INT_A] = intA;
    gEntranceData[entranceid][E_ENTRANCE_INT_WORLD_ID] = intWorld;
    gEntranceData[entranceid][E_ENTRANCE_INT_INTERIOR_ID] = intInterior;

    gEntranceData[entranceid][E_ENTRANCE_EXT_BUTTON_ID] = CreateButton(extX, extY, extZ, buttonKey, 1.0, extWorld, extInterior, priority);
    gEntranceData[entranceid][E_ENTRANCE_INT_BUTTON_ID] = CreateButton(intX, intY, intZ, buttonKey, 1.0, intWorld, intInterior, priority);

    SetButtonExtraData(gEntranceData[entranceid][E_ENTRANCE_EXT_BUTTON_ID], BTN_ATTRIBUTE_EXT_ENTRANCE_ID, _:entranceid);
    SetButtonExtraData(gEntranceData[entranceid][E_ENTRANCE_INT_BUTTON_ID], BTN_ATTRIBUTE_INT_ENTRANCE_ID, _:entranceid);

    return entranceid;
}

stock bool:IsValidEntrance(Entrance:entranceid) {
    if (!(0 <= _:entranceid < ENTRANCE_ITER_SIZE)) {
        return false;
    }

    return Iter_Contains(Entrance, entranceid);
}

/**
 * # Exterior
 */

stock bool:GetEntranceExteriorPos(Entrance:entranceid, &Float:x, &Float:y, &Float:z) {
    if (!IsValidEntrance(entranceid)) {
        return false;
    }

    x = gEntranceData[entranceid][E_ENTRANCE_EXT_X];
    y = gEntranceData[entranceid][E_ENTRANCE_EXT_Y];
    z = gEntranceData[entranceid][E_ENTRANCE_EXT_Z];

    return true;
}

stock bool:GetEntranceExteriorFacingAngle(Entrance:entranceid, &Float:a) {
    if (!IsValidEntrance(entranceid)) {
        return false;
    }

    a = gEntranceData[entranceid][E_ENTRANCE_EXT_A];

    return true;
}

stock GetEntranceExteriorVirtualWorld(Entrance:entranceid) {
    if (!IsValidEntrance(entranceid)) {
        return -1;
    }

    return gEntranceData[entranceid][E_ENTRANCE_EXT_WORLD_ID];
}

stock GetEntranceExteriorInterior(Entrance:entranceid) {
    if (!IsValidEntrance(entranceid)) {
        return -1;
    }

    return gEntranceData[entranceid][E_ENTRANCE_EXT_INTERIOR_ID];
}

stock bool:GetEntranceExteriorButton(Entrance:entranceid, &Button:buttonid) {
    if (!IsValidEntrance(entranceid)) {
        return false;
    }
    
    buttonid = gEntranceData[entranceid][E_ENTRANCE_EXT_BUTTON_ID];

    return true;
}

stock bool:HasButtonEntranceExterior(Button:buttonid) {
    return HasButtonExtraData(buttonid, BTN_ATTRIBUTE_EXT_ENTRANCE_ID);
}

stock bool:GetButtonEntranceExterior(Button:buttonid, &Entrance:entranceid) {
    return GetButtonExtraData(buttonid, BTN_ATTRIBUTE_EXT_ENTRANCE_ID, _:entranceid);
}

stock bool:SetPlayerEntranceExterior(playerid, Entrance:entranceid, Float:offset = ENTRANCE_DEFAULT_FORWARD_OFFSET) {
    if (!IsValidEntrance(entranceid)) {
        return false;
    }

    SetPlayerPos(playerid,
        gEntranceData[entranceid][E_ENTRANCE_EXT_X] + offset * floatsin(-gEntranceData[entranceid][E_ENTRANCE_EXT_A], degrees),
        gEntranceData[entranceid][E_ENTRANCE_EXT_Y] + offset * floatcos(-gEntranceData[entranceid][E_ENTRANCE_EXT_A], degrees),
        gEntranceData[entranceid][E_ENTRANCE_EXT_Z]
    );
    SetPlayerVirtualWorld(playerid, gEntranceData[entranceid][E_ENTRANCE_EXT_WORLD_ID]);
    SetPlayerInterior(playerid, gEntranceData[entranceid][E_ENTRANCE_EXT_INTERIOR_ID]);
    SetPlayerFacingAngle(playerid, gEntranceData[entranceid][E_ENTRANCE_EXT_A]);
    SetCameraBehindPlayer(playerid);

    CallLocalFunction("OnPlayerLeftEntrance", "ii", playerid, _:entranceid);

    return true;
}

stock bool:GetPlayerEntranceExterior(playerid, &Entrance:entranceid) {
    if (!IsPlayerConnected(playerid)) {
        return false;
    }

    if (!IsPlayerInAnyButton(playerid)) {
        return false;
    }

    new
        Button:buttonid
    ;

    if (GetPlayerButton(playerid, buttonid)) {
        return false;
    }

    if (!GetButtonEntranceExterior(buttonid, entranceid)) {
        return false;
    }

    return true;
}

stock bool:GetPlayerEntranceExteriorButton(playerid, &Button:buttonid) {
    if (!IsPlayerConnected(playerid)) {
        return false;
    }

    if (!IsPlayerInAnyButton(playerid)) {
        return false;
    }

    if (GetPlayerButton(playerid, buttonid)) {
        return false;
    }

    if (!HasButtonEntranceExterior(buttonid)) {
        return false;
    }

    return true;
}

/**
 * # Interior
 */

stock bool:GetEntranceInteriorPos(Entrance:entranceid, &Float:x, &Float:y, &Float:z) {
    if (!IsValidEntrance(entranceid)) {
        return false;
    }

    x = gEntranceData[entranceid][E_ENTRANCE_INT_X];
    y = gEntranceData[entranceid][E_ENTRANCE_INT_Y];
    z = gEntranceData[entranceid][E_ENTRANCE_INT_Z];

    return true;
}

stock bool:GetEntranceInteriorFacingAngle(Entrance:entranceid, &Float:a) {
    if (!IsValidEntrance(entranceid)) {
        return false;
    }

    a = gEntranceData[entranceid][E_ENTRANCE_INT_A];

    return true;
}

stock GetEntranceInteriorVirtualWorld(Entrance:entranceid) {
    if (!IsValidEntrance(entranceid)) {
        return -1;
    }

    return gEntranceData[entranceid][E_ENTRANCE_INT_WORLD_ID];
}

stock GetEntranceInteriorInterior(Entrance:entranceid) {
    if (!IsValidEntrance(entranceid)) {
        return -1;
    }

    return gEntranceData[entranceid][E_ENTRANCE_INT_INTERIOR_ID];
}

stock bool:GetEntranceInteriorButton(Entrance:entranceid, &Button:buttonid) {
    if (!IsValidEntrance(entranceid)) {
        return false;
    }

    buttonid = gEntranceData[entranceid][E_ENTRANCE_INT_BUTTON_ID];

    return true;
}

stock bool:HasButtonEntranceInterior(Button:buttonid) {
    return HasButtonExtraData(buttonid, BTN_ATTRIBUTE_INT_ENTRANCE_ID);
}

stock bool:GetButtonEntranceInterior(Button:buttonid, &Entrance:entranceid) {
    return GetButtonExtraData(buttonid, BTN_ATTRIBUTE_INT_ENTRANCE_ID, _:entranceid);
}

stock bool:SetPlayerEntranceInterior(playerid, Entrance:entranceid, Float:offset = ENTRANCE_DEFAULT_FORWARD_OFFSET) {
    if (!IsValidEntrance(entranceid)) {
        return false;
    }

    SetPlayerPos(playerid,
        gEntranceData[entranceid][E_ENTRANCE_INT_X] + offset * floatsin(-gEntranceData[entranceid][E_ENTRANCE_INT_A], degrees),
        gEntranceData[entranceid][E_ENTRANCE_INT_Y] + offset * floatcos(-gEntranceData[entranceid][E_ENTRANCE_INT_A], degrees),
        gEntranceData[entranceid][E_ENTRANCE_INT_Z]
    );
    SetPlayerVirtualWorld(playerid, gEntranceData[entranceid][E_ENTRANCE_INT_WORLD_ID]);
    SetPlayerInterior(playerid, gEntranceData[entranceid][E_ENTRANCE_INT_INTERIOR_ID]);
    SetPlayerFacingAngle(playerid, gEntranceData[entranceid][E_ENTRANCE_INT_A]);
    SetCameraBehindPlayer(playerid);

    CallLocalFunction("OnPlayerEnteredEntrance", "ii", playerid, _:entranceid);

    return true;
}

stock bool:GetPlayerEntranceInterior(playerid, &Entrance:entranceid) {
    if (!IsPlayerConnected(playerid)) {
        return false;
    }

    if (!IsPlayerInAnyButton(playerid)) {
        return false;
    }

    new
        Button:buttonid
    ;

    if (GetPlayerButton(playerid, buttonid)) {
        return false;
    }

    if (!GetButtonEntranceInterior(buttonid, entranceid)) {
        return false;
    }

    return true;
}

stock bool:GetPlayerEntranceInteriorButton(playerid, &Button:buttonid) {
    if (!IsPlayerConnected(playerid)) {
        return false;
    }

    if (!IsPlayerInAnyButton(playerid)) {
        return false;
    }

    if (GetPlayerButton(playerid, buttonid)) {
        return false;
    }

    if (!HasButtonEntranceInterior(buttonid)) {
        return false;
    }

    return true;
}

/**
 * # Calls
 */

hook OnButtonPress(playerid, Button:buttonid) {
    if (HasButtonEntranceExterior(buttonid)) {
        new
            Entrance:entranceid
        ;

        GetButtonEntranceExterior(buttonid, entranceid);

        if (CallLocalFunction("OnPlayerEnterEntrance", "ii", playerid, _:entranceid)) {
            return ~1;
        }

        SetPlayerEntranceInterior(playerid, entranceid);
        
        return ~1;
    }

    if (HasButtonEntranceInterior(buttonid)) {
        new
            Entrance:entranceid
        ;

        GetButtonEntranceInterior(buttonid, entranceid);

        if (CallLocalFunction("OnPlayerLeaveEntrance", "ii", playerid, _:entranceid)) {
            return ~1;
        }

        SetPlayerEntranceExterior(playerid, entranceid);

        return ~1;
    }

    return 0;
}

hook OnPlayerEnterButtonArea(playerid, Button:buttonid) {
    if (HasButtonEntranceExterior(buttonid)) {
        new
            Entrance:entranceid
        ;

        GetButtonEntranceExterior(buttonid, entranceid);

        if (CallLocalFunction("OnPlayerEnterExteriorArea", "ii", playerid, _:entranceid)) {
            return ~1;
        }

        return 0;
    }

    if (HasButtonEntranceInterior(buttonid)) {
        new
            Entrance:entranceid
        ;

        GetButtonEntranceInterior(buttonid, entranceid);

        if (CallLocalFunction("OnPlayerEnterInteriorArea", "ii", playerid, _:entranceid)) {
            return ~1;
        }
    }
    
    return 0;
}

hook OnPlayerLeaveButtonArea(playerid, Button:buttonid) {
    if (HasButtonEntranceExterior(buttonid)) {
        new
            Entrance:entranceid
        ;

        GetButtonEntranceExterior(buttonid, entranceid);

        if (CallLocalFunction("OnPlayerLeaveExteriorArea", "ii", playerid, _:entranceid)) {
            return ~1;
        }

        return 0;
    }

    if (HasButtonEntranceInterior(buttonid)) {
        new
            Entrance:entranceid
        ;

        GetButtonEntranceInterior(buttonid, entranceid);

        if (CallLocalFunction("OnPlayerLeaveInteriorArea", "ii", playerid, _:entranceid)) {
            return ~1;
        }
    }
    
    return 0;
}