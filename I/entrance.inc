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

#define INVALID_ENTRANCE_ID (Entrance:-1)

static const
    ATTRIBUTE_EXT_ENTRANCE_ID[] = "ATTRIBUTE_EXT_ENTRANCE_ID",
    ATTRIBUTE_INT_ENTRANCE_ID[] = "ATTRIBUTE_INT_ENTRANCE_ID"
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
    entrance[MAX_ENTRANCES][E_ENTRANCE_DATA]
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
forward bool:GetPlayerExteriorButton(playerid, &Button:buttonid);
forward bool:SetPlayerExteriorEntrance(playerid, Entrance:entranceid, Float:offset = 1.0);
forward bool:GetPlayerExteriorEntrance(playerid, &Entrance:entranceid);

forward bool:GetEntranceInteriorPos(Entrance:entranceid, &Float:x, &Float:y, &Float:z);
forward bool:GetEntranceInteriorFacingAngle(Entrance:entranceid, &Float:a);
forward GetEntranceInteriorVirtualWorld(Entrance:entranceid);
forward GetEntranceInteriorInterior(Entrance:entranceid);
forward bool:GetInteriorButton(Entrance:entranceid, &Button:buttonid);
forward bool:GetPlayerInteriorButton(playerid, &Button:buttonid);
forward bool:SetPlayerInteriorEntrance(playerid, Entrance:entranceid, Float:offset = 1.0);
forward bool:GetPlayerInteriorEntrance(playerid, &Entrance:entranceid);

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

    entrance[entranceid][E_ENTRANCE_EXT_X] = extX;
    entrance[entranceid][E_ENTRANCE_EXT_Y] = extY;
    entrance[entranceid][E_ENTRANCE_EXT_Z] = extZ;
    entrance[entranceid][E_ENTRANCE_EXT_A] = extA;
    entrance[entranceid][E_ENTRANCE_EXT_WORLD_ID] = extWorld;
    entrance[entranceid][E_ENTRANCE_EXT_INTERIOR_ID] = extInterior;

    entrance[entranceid][E_ENTRANCE_INT_X] = intX;
    entrance[entranceid][E_ENTRANCE_INT_Y] = intY;
    entrance[entranceid][E_ENTRANCE_INT_Z] = intZ;
    entrance[entranceid][E_ENTRANCE_INT_A] = intA;
    entrance[entranceid][E_ENTRANCE_INT_WORLD_ID] = intWorld;
    entrance[entranceid][E_ENTRANCE_INT_INTERIOR_ID] = intInterior;

    entrance[entranceid][E_ENTRANCE_EXT_BUTTON_ID] = CreateButton(extX, extY, extZ, .worldid = extWorld, .interiorid = extInterior);
    entrance[entranceid][E_ENTRANCE_INT_BUTTON_ID] = CreateButton(intX, intY, intZ, .worldid = intWorld, .interiorid = intInterior);

    SetButtonExtraData(entrance[entranceid][E_ENTRANCE_EXT_BUTTON_ID], ATTRIBUTE_EXT_ENTRANCE_ID, _:entranceid);
    SetButtonExtraData(entrance[entranceid][E_ENTRANCE_INT_BUTTON_ID], ATTRIBUTE_INT_ENTRANCE_ID, _:entranceid);

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

    x = entrance[entranceid][E_ENTRANCE_EXT_X];
    y = entrance[entranceid][E_ENTRANCE_EXT_Y];
    z = entrance[entranceid][E_ENTRANCE_EXT_Z];

    return true;
}

stock bool:GetEntranceExteriorFacingAngle(Entrance:entranceid, &Float:a) {
    if (!IsValidEntrance(entranceid)) {
        return false;
    }

    a = entrance[entranceid][E_ENTRANCE_EXT_A];

    return true;
}

stock GetEntranceExteriorVirtualWorld(Entrance:entranceid) {
    if (!IsValidEntrance(entranceid)) {
        return 0;
    }

    return entrance[entranceid][E_ENTRANCE_EXT_WORLD_ID];
}

stock GetEntranceExteriorInterior(Entrance:entranceid) {
    if (!IsValidEntrance(entranceid)) {
        return 0;
    }

    return entrance[entranceid][E_ENTRANCE_EXT_INTERIOR_ID];
}

stock bool:GetExteriorButton(Entrance:entranceid, &Button:buttonid) {
    if (!IsValidEntrance(entranceid)) {
        return false;
    }
    
    buttonid = entrance[entranceid][E_ENTRANCE_EXT_BUTTON_ID];

    return true;
}

stock bool:GetPlayerExteriorButton(playerid, &Button:buttonid) {
    if (!GetPlayerButton(playerid, buttonid)) {
        return false;
    }

    return HasButtonPlusExtraData(buttonid, ATTRIBUTE_EXT_ENTRANCE_ID);
}

stock bool:SetPlayerExteriorEntrance(playerid, Entrance:entranceid, Float:offset = 1.0) {
    if (!IsValidEntrance(entranceid)) {
        return false;
    }

    SetPlayerPos(playerid,
        entrance[entranceid][E_ENTRANCE_EXT_X] + offset * floatsin(-entrance[entranceid][E_ENTRANCE_EXT_A], degrees),
        entrance[entranceid][E_ENTRANCE_EXT_Y] + offset * floatcos(-entrance[entranceid][E_ENTRANCE_EXT_A], degrees),
        entrance[entranceid][E_ENTRANCE_EXT_Z]
    );

    SetPlayerFacingAngle(playerid, entrance[entranceid][E_ENTRANCE_EXT_A]);
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

    return GetButtonExtraData(buttonid, ATTRIBUTE_EXT_ENTRANCE_ID, _:entranceid);
}

/**
 * # Interior
 */

stock bool:GetEntranceInteriorPos(Entrance:entranceid, &Float:x, &Float:y, &Float:z) {
    if (!IsValidEntrance(entranceid)) {
        return false;
    }

    x = entrance[entranceid][E_ENTRANCE_INT_X];
    y = entrance[entranceid][E_ENTRANCE_INT_Y];
    z = entrance[entranceid][E_ENTRANCE_INT_Z];

    return true;
}

stock bool:GetEntranceInteriorFacingAngle(Entrance:entranceid, &Float:a) {
    if (!IsValidEntrance(entranceid)) {
        return false;
    }

    a = entrance[entranceid][E_ENTRANCE_INT_A];

    return true;
}

stock GetEntranceInteriorVirtualWorld(Entrance:entranceid) {
    if (!IsValidEntrance(entranceid)) {
        return 0;
    }

    return entrance[entranceid][E_ENTRANCE_INT_WORLD_ID];
}

stock GetEntranceInteriorInterior(Entrance:entranceid) {
    if (!IsValidEntrance(entranceid)) {
        return 0;
    }

    return entrance[entranceid][E_ENTRANCE_INT_INTERIOR_ID];
}

stock bool:GetInteriorButton(Entrance:entranceid, &Button:buttonid) {
    if (!IsValidEntrance(entranceid)) {
        return false;
    }

    buttonid = entrance[entranceid][E_ENTRANCE_INT_BUTTON_ID];

    return true;
}

stock bool:GetPlayerInteriorButton(playerid, &Button:buttonid) {
    if (!GetPlayerButton(playerid, buttonid)) {
        return false;
    }

    return HasButtonPlusExtraData(buttonid, ATTRIBUTE_INT_ENTRANCE_ID);
}

stock bool:SetPlayerInteriorEntrance(playerid, Entrance:entranceid, Float:offset = 1.0) {
    if (!IsValidEntrance(entranceid)) {
        return false;
    }

    SetPlayerPos(playerid,
        entrance[entranceid][E_ENTRANCE_INT_X] + offset * floatsin(-entrance[entranceid][E_ENTRANCE_INT_A], degrees),
        entrance[entranceid][E_ENTRANCE_INT_Y] + offset * floatcos(-entrance[entranceid][E_ENTRANCE_INT_A], degrees),
        entrance[entranceid][E_ENTRANCE_INT_Z]
    );

    SetPlayerFacingAngle(playerid, entrance[entranceid][E_ENTRANCE_INT_A]);
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

    return GetButtonExtraData(buttonid, ATTRIBUTE_INT_ENTRANCE_ID, _:entranceid);
}

/**
 * # Calls
 */

hook OnButtonPress(playerid, Button:buttonid) {
    static
        Entrance:entranceid
    ;

    if (GetButtonExtraData(buttonid, ATTRIBUTE_EXT_ENTRANCE_ID, _:entranceid) && SetPlayerInteriorEntrance(playerid, entranceid)) {
        return ~1;
    }

    if (GetButtonExtraData(buttonid, ATTRIBUTE_INT_ENTRANCE_ID, _:entranceid) && SetPlayerExteriorEntrance(playerid, entranceid)) {
        return ~1;
    }

    return 1;
}