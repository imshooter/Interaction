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

forward Entrance:CreateEntrance(Float:extX, Float:extY, Float:extZ, Float:extA, extWorld, extInterior, Float:intX, Float:intY, Float:intZ, Float:intA, intWorld, intInterior);
forward bool:IsValidEntrance(Entrance:entranceid);

forward bool:GetEntranceExteriorPos(Entrance:entranceid, &Float:x, &Float:y, &Float:z);
forward bool:GetEntranceExteriorFacingAngle(Entrance:entranceid, &Float:a);
forward GetEntranceExteriorVirtualWorld(Entrance:entranceid);
forward GetEntranceExteriorInterior(Entrance:entranceid);
forward bool:GetExteriorButton(Entrance:entranceid, &Button:buttonid);
forward bool:HasButtonExteriorEntrance(Button:buttonid);
forward bool:GetButtonExteriorEntrance(Button:buttonid, &Entrance:entranceid);
forward bool:GetPlayerExteriorButton(playerid, &Button:buttonid);
forward bool:SetPlayerExteriorEntrance(playerid, Entrance:entranceid, Float:offset = ENTRANCE_DEFAULT_FORWARD_OFFSET);
forward bool:GetPlayerExteriorEntrance(playerid, &Entrance:entranceid);

forward bool:GetEntranceInteriorPos(Entrance:entranceid, &Float:x, &Float:y, &Float:z);
forward bool:GetEntranceInteriorFacingAngle(Entrance:entranceid, &Float:a);
forward GetEntranceInteriorVirtualWorld(Entrance:entranceid);
forward GetEntranceInteriorInterior(Entrance:entranceid);
forward bool:GetInteriorButton(Entrance:entranceid, &Button:buttonid);
forward bool:HasButtonInteriorEntrance(Button:buttonid);
forward bool:GetButtonInteriorEntrance(Button:buttonid, &Entrance:entranceid);
forward bool:GetPlayerInteriorButton(playerid, &Button:buttonid);
forward bool:SetPlayerInteriorEntrance(playerid, Entrance:entranceid, Float:offset = ENTRANCE_DEFAULT_FORWARD_OFFSET);
forward bool:GetPlayerInteriorEntrance(playerid, &Entrance:entranceid);

/**
 * # Events
 */

/**
 * # OnPlayerEnterExteriorArea
 *
 * # Returns
 *  -  `0`: Breaks subsequent calls to `OnPlayerEnterButtonArea` (Consequently `OnPlayerEnterDynamicArea` too).
 *  -  `1`: Continues with subsequent calls to `OnPlayerEnterButtonArea` (Consequently `OnPlayerEnterDynamicArea` too).
 *
 *  - `~0`: Breaks subsequent calls to `OnPlayerEnterExteriorArea` and `OnPlayerEnterButtonArea` (Consequently `OnPlayerEnterDynamicArea` too).
 *  - `~1`: Breaks subsequent calls to `OnPlayerEnterExteriorArea` and continues subsequent calls to `OnPlayerEnterButtonArea` (Consequently `OnPlayerEnterDynamicArea` too).
 */

forward OnPlayerEnterExteriorArea(playerid, Entrance:entranceid);

/**
 * # OnPlayerEnterInteriorArea
 *
 * # Returns
 *  -  `0`: Breaks subsequent calls to `OnPlayerEnterButtonArea` (Consequently `OnPlayerEnterDynamicArea` too).
 *  -  `1`: Continues with subsequent calls to `OnPlayerEnterButtonArea` (Consequently `OnPlayerEnterDynamicArea` too).
 *
 *  - `~0`: Breaks subsequent calls to `OnPlayerEnterInteriorArea` and `OnPlayerEnterButtonArea` (Consequently `OnPlayerEnterDynamicArea` too).
 *  - `~1`: Breaks subsequent calls to `OnPlayerEnterInteriorArea` and continues subsequent calls to `OnPlayerEnterButtonArea` (Consequently `OnPlayerEnterDynamicArea` too).
 */
forward OnPlayerEnterInteriorArea(playerid, Entrance:entranceid);

/**
 * # OnPlayerLeaveExteriorArea
 *
 * # Returns
 *  -  `0`: Breaks subsequent calls to `OnPlayerLeaveButtonArea` (Consequently `OnPlayerLeaveDynamicArea` too).
 *  -  `1`: Continues with subsequent calls to `OnPlayerLeaveButtonArea` (Consequently `OnPlayerLeaveDynamicArea` too).
 *
 *  - `~0`: Breaks subsequent calls to `OnPlayerLeaveExteriorArea` and `OnPlayerLeaveButtonArea` (Consequently `OnPlayerLeaveDynamicArea` too).
 *  - `~1`: Breaks subsequent calls to `OnPlayerLeaveExteriorArea` and continues subsequent calls to `OnPlayerLeaveButtonArea` (Consequently `OnPlayerLeaveDynamicArea` too).
 */

forward OnPlayerLeaveExteriorArea(playerid, Entrance:entranceid);

/**
 * # OnPlayerLeaveInteriorArea
 *
 * # Returns
 *  -  `0`: Breaks subsequent calls to `OnPlayerLeaveButtonArea` (Consequently `OnPlayerLeaveDynamicArea` too).
 *  -  `1`: Continues with subsequent calls to `OnPlayerLeaveButtonArea` (Consequently `OnPlayerLeaveDynamicArea` too).
 *
 *  - `~0`: Breaks subsequent calls to `OnPlayerLeaveInteriorArea` and `OnPlayerLeaveButtonArea` (Consequently `OnPlayerLeaveDynamicArea` too).
 *  - `~1`: Breaks subsequent calls to `OnPlayerLeaveInteriorArea` and continues subsequent calls to `OnPlayerLeaveButtonArea` (Consequently `OnPlayerLeaveDynamicArea` too).
 */

forward OnPlayerLeaveInteriorArea(playerid, Entrance:entranceid);

/**
 * # External
 */

stock Entrance:CreateEntrance(Float:extX, Float:extY, Float:extZ, Float:extA, extWorld, extInterior, Float:intX, Float:intY, Float:intZ, Float:intA, intWorld, intInterior) {
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

    gEntranceData[entranceid][E_ENTRANCE_EXT_BUTTON_ID] = CreateButton(extX, extY, extZ, .worldid = extWorld, .interiorid = extInterior);
    gEntranceData[entranceid][E_ENTRANCE_INT_BUTTON_ID] = CreateButton(intX, intY, intZ, .worldid = intWorld, .interiorid = intInterior);

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
        return 0;
    }

    return gEntranceData[entranceid][E_ENTRANCE_EXT_WORLD_ID];
}

stock GetEntranceExteriorInterior(Entrance:entranceid) {
    if (!IsValidEntrance(entranceid)) {
        return 0;
    }

    return gEntranceData[entranceid][E_ENTRANCE_EXT_INTERIOR_ID];
}

stock bool:GetExteriorButton(Entrance:entranceid, &Button:buttonid) {
    if (!IsValidEntrance(entranceid)) {
        return false;
    }
    
    buttonid = gEntranceData[entranceid][E_ENTRANCE_EXT_BUTTON_ID];

    return true;
}

stock bool:HasButtonExteriorEntrance(Button:buttonid) {
    return HasButtonExtraData(buttonid, BTN_ATTRIBUTE_EXT_ENTRANCE_ID);
}

stock bool:GetButtonExteriorEntrance(Button:buttonid, &Entrance:entranceid) {
    return GetButtonExtraData(buttonid, BTN_ATTRIBUTE_EXT_ENTRANCE_ID, _:entranceid);
}

stock bool:GetPlayerExteriorButton(playerid, &Button:buttonid) {
    if (!GetPlayerButton(playerid, buttonid)) {
        return false;
    }

    return HasButtonPlusExtraData(buttonid, BTN_ATTRIBUTE_EXT_ENTRANCE_ID);
}

stock bool:SetPlayerExteriorEntrance(playerid, Entrance:entranceid, Float:offset = ENTRANCE_DEFAULT_FORWARD_OFFSET) {
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

    return true;
}

stock bool:GetPlayerExteriorEntrance(playerid, &Entrance:entranceid) {
    new
        Button:buttonid
    ;

    if (!GetPlayerButton(playerid, buttonid)) {
        return false;
    }

    return GetButtonExtraData(buttonid, BTN_ATTRIBUTE_EXT_ENTRANCE_ID, _:entranceid);
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
        return 0;
    }

    return gEntranceData[entranceid][E_ENTRANCE_INT_WORLD_ID];
}

stock GetEntranceInteriorInterior(Entrance:entranceid) {
    if (!IsValidEntrance(entranceid)) {
        return 0;
    }

    return gEntranceData[entranceid][E_ENTRANCE_INT_INTERIOR_ID];
}

stock bool:GetInteriorButton(Entrance:entranceid, &Button:buttonid) {
    if (!IsValidEntrance(entranceid)) {
        return false;
    }

    buttonid = gEntranceData[entranceid][E_ENTRANCE_INT_BUTTON_ID];

    return true;
}

stock bool:HasButtonInteriorEntrance(Button:buttonid) {
    return HasButtonExtraData(buttonid, BTN_ATTRIBUTE_INT_ENTRANCE_ID);
}

stock bool:GetButtonInteriorEntrance(Button:buttonid, &Entrance:entranceid) {
    return GetButtonExtraData(buttonid, BTN_ATTRIBUTE_INT_ENTRANCE_ID, _:entranceid);
}

stock bool:GetPlayerInteriorButton(playerid, &Button:buttonid) {
    if (!GetPlayerButton(playerid, buttonid)) {
        return false;
    }

    return HasButtonPlusExtraData(buttonid, BTN_ATTRIBUTE_INT_ENTRANCE_ID);
}

stock bool:SetPlayerInteriorEntrance(playerid, Entrance:entranceid, Float:offset = ENTRANCE_DEFAULT_FORWARD_OFFSET) {
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

    return true;
}

stock bool:GetPlayerInteriorEntrance(playerid, &Entrance:entranceid) {
    new
        Button:buttonid
    ;

    if (!GetPlayerButton(playerid, buttonid)) {
        return false;
    }

    return GetButtonExtraData(buttonid, BTN_ATTRIBUTE_INT_ENTRANCE_ID, _:entranceid);
}

/**
 * # Calls
 */

hook OnButtonPress(playerid, Button:buttonid) {
    if (HasButtonExteriorEntrance(buttonid)) {
        new
            Entrance:entranceid
        ;

        if (GetButtonExteriorEntrance(buttonid, entranceid) && SetPlayerInteriorEntrance(playerid, entranceid)) {
            return ~0;
        }

        return 1;
    }

    if (HasButtonInteriorEntrance(buttonid)) {
        new
            Entrance:entranceid
        ;

        if (GetButtonInteriorEntrance(buttonid, entranceid) && SetPlayerExteriorEntrance(playerid, entranceid)) {
            return ~0;
        }
    }

    return 1;
}

hook OnPlayerEnterButtonArea(playerid, Button:buttonid) {
    if (HasButtonExteriorEntrance(buttonid)) {
        new
            Entrance:entranceid
        ;

        GetButtonExteriorEntrance(buttonid, entranceid);

        if (!CallLocalFunction("OnPlayerEnterExteriorArea", "ii", playerid, _:entranceid)) {
            return ~0;
        }

        return 1;
    }

    if (HasButtonInteriorEntrance(buttonid)) {
        new
            Entrance:entranceid
        ;

        GetButtonInteriorEntrance(buttonid, entranceid);

        if (!CallLocalFunction("OnPlayerEnterInteriorArea", "ii", playerid, _:entranceid)) {
            return ~0;
        }
    }
    
    return 1;
}

hook OnPlayerLeaveButtonArea(playerid, Button:buttonid) {
    if (HasButtonExteriorEntrance(buttonid)) {
        new
            Entrance:entranceid
        ;

        GetButtonExteriorEntrance(buttonid, entranceid);

        if (!CallLocalFunction("OnPlayerLeaveExteriorArea", "ii", playerid, _:entranceid)) {
            return ~0;
        }

        return 1;
    }

    if (HasButtonInteriorEntrance(buttonid)) {
        new
            Entrance:entranceid
        ;

        GetButtonInteriorEntrance(buttonid, entranceid);

        if (!CallLocalFunction("OnPlayerLeaveInteriorArea", "ii", playerid, _:entranceid)) {
            return ~0;
        }
    }
    
    return 1;
}