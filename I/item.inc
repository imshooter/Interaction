#if defined _INC_ITEM
    #endinput
#endif
#define _INC_ITEM

#include <I\button>
#include <I\button-plus>

#include <uuid>

#include <YSI_Data\y_iterate>
#include <YSI_Coding\y_hooks>

/**
 * # Header
 */

#if !defined MAX_ITEM_BUILDS
    #define MAX_ITEM_BUILDS (ItemBuild:128)
#endif

#if !defined MAX_ITEMS
    #define MAX_ITEMS (Item:8192)
#endif

#if !defined MAX_ITEM_BUILD_NAME
    #define MAX_ITEM_BUILD_NAME (32)
#endif

#if !defined MAX_ITEM_EXTRA_NAME
    #define MAX_ITEM_EXTRA_NAME (32)
#endif

#if !defined MAX_ITEM_NAME
    #define MAX_ITEM_NAME (MAX_ITEM_BUILD_NAME + MAX_ITEM_EXTRA_NAME)
#endif

#if !defined ITEM_KEY_DROP
    #define ITEM_KEY_DROP (KEY_NO)
#endif

#if !defined ITEM_DEFAULT_KEY
    #define ITEM_DEFAULT_KEY (BUTTON_DEFAULT_KEY)
#endif

#if !defined ITEM_DEFAULT_DISTANCE
    #define ITEM_DEFAULT_DISTANCE (50.0)
#endif

#define INVALID_ITEM_BUILD_ID (ItemBuild:-1)
#define INVALID_ITEM_ID (Item:-1)

#define ITEM_ATTACH_INDEX (0)
#define ITEM_FLOOR_OFFSET (0.96)

static const
    BUTTON_ATTRIBUTE_ITEM_ID[] = "BUTTON_ATTRIBUTE_ITEM_ID"
;

static enum E_ITEM_BUILD_DATA {
    E_ITEM_BUILD_NAME[MAX_ITEM_BUILD_NAME + 1],
    E_ITEM_BUILD_MODEL_ID,
    E_ITEM_BUILD_FLAGS,
    Float:E_ITEM_BUILD_BUTTON_OFFSET_Z
};

static
    gItemBuildData[MAX_ITEM_BUILDS][E_ITEM_BUILD_DATA]
;

static enum E_ITEM_DATA {
    ItemBuild:E_ITEM_BUILD_ID,
    
    // WORLD:

    Float:E_ITEM_X,
    Float:E_ITEM_Y,
    Float:E_ITEM_Z,

    Float:E_ITEM_ROT_X,
    Float:E_ITEM_ROT_Y,
    Float:E_ITEM_ROT_Z,

    E_ITEM_WORLD_ID,
    E_ITEM_INTERIOR_ID,

    Button:E_ITEM_BUTTON_ID,
    STREAMER_TAG_OBJECT:E_ITEM_OBJECT_ID,

    // EXTRA:

    E_ITEM_UUID[UUID_LEN],
    E_ITEM_EXTRA_NAME[MAX_ITEM_NAME + 1]
};

static
    gItemData[MAX_ITEMS][E_ITEM_DATA]
;

const static
    ITEM_BUILD_ITER_SIZE = _:MAX_ITEM_BUILDS,
    ITEM_ITER_SIZE = _:MAX_ITEMS
;

new
    Iterator:ItemBuild<ItemBuild:ITEM_BUILD_ITER_SIZE>,
    Iterator:Item<Item:ITEM_ITER_SIZE>,
    Iterator:ItemWorld<Item:ITEM_ITER_SIZE>
;

/**
 * # Functions
 */

forward ItemBuild:BuildItem(const name[], modelid, Float:buttonOffsetZ = ITEM_FLOOR_OFFSET);
forward bool:IsValidItemBuild(ItemBuild:buildid);
forward bool:GetItemBuildName(ItemBuild:buildid, output[], len = sizeof (output));
forward GetItemBuildModel(ItemBuild:buildid);
forward bool:SetItemBuildFlags(ItemBuild:buildid, flags);
forward GetItemBuildFlags(ItemBuild:buildid);
forward bool:HasItemBuildFlags(ItemBuild:buildid, flags);

forward Item:CreateItem(ItemBuild:buildid, const uuid[] = "");
forward bool:IsValidItem(Item:itemid);
forward bool:GetItemBuild(Item:itemid, &ItemBuild:buildid);
forward bool:SetItemExtraName(Item:itemid, const format[], OPEN_MP_TAGS:...);
forward bool:GetItemExtraName(Item:itemid, output[], len = sizeof (output));
forward bool:GetItemUUID(Item:itemid, output[], len = sizeof (output));

forward bool:CreateItemInWorld(Item:itemid, Float:x, Float:y, Float:z, Float:rotationX, Float:rotationY, Float:rotationZ, KEY:buttonKey = ITEM_DEFAULT_KEY, worldid = -1, interiorid = -1, Float:distance = ITEM_DEFAULT_DISTANCE);
forward bool:RemoveItemFromWorld(Item:itemid);
forward bool:IsItemInWorld(Item:itemid);
forward bool:SetItemTextLabel(Item:itemid, const format[], colour = 0xFFFFFFFF, Float:distance = ITEM_DEFAULT_DISTANCE, bool:testLOS = true, Float:fOffsetZ = 0.5, OPEN_MP_TAGS:...);
forward bool:GetItemButton(Item:itemid, &Button:buttonid);
forward bool:SetItemButtonPos(Item:itemid, Float:x, Float:y, Float:z, bool:allowStack = false);
forward bool:GetItemButtonPos(Item:itemid, &Float:x, &Float:y, &Float:z);
forward bool:GetButtonItem(Button:buttonid, &Item:itemid);
forward bool:GetPlayerNearbyItems(playerid, Item:items[MAX_PLAYER_BUTTONS] = { INVALID_ITEM_ID, ... }, &count = 0);

/**
 * # Events
 */

/**
 * # OnItemCreate
 *
 * # Returns
 *  - `~0` or `~1`: Breaks subsequent calls to `OnItemCreate`.
 */

forward OnItemCreate(Item:itemid);

/**
 * # OnItemDestroy
 *
 * # Returns
 *  - `~0` or `~1`: Breaks subsequent calls to `OnItemDestroy`.
 */

forward OnItemDestroy(Item:itemid);

/**
 * # OnItemCreateInWorld
 *
 * # Returns
 *  - `~0` or `~1`: Breaks subsequent calls to `OnItemCreateInWorld`.
 */

forward OnItemCreateInWorld(Item:itemid);

/**
 * # OnItemRemoveFromWorld
 *
 * # Returns
 *  - `~0` or `~1`: Breaks subsequent calls to `OnItemRemoveFromWorld`.
 */

forward OnItemRemoveFromWorld(Item:itemid);

/**
 * # OnPlayerPickUpItem
 *
 * # Returns
 *  -  `0` or  `1`: Breaks subsequent calls to `OnButtonPress` (Consequently `OnPlayerKeyStateChange` too).
 *  - `~0` or `~1~: Breaks subsequent calls to `OnPlayerPickUpItem` and `OnButtonPress` (Consequently `OnPlayerKeyStateChange` too).
 *
 * # NOTE:
 *  - Return `0` to `CANCEL` the player from picking up the item in the world
 *  - Return `1` for the player pick up the item in the world
 */

forward OnPlayerPickUpItem(playerid, Item:itemid, Item:items[], count);

/**
 * # Item Build
 */

stock ItemBuild:BuildItem(const name[], modelid, Float:buttonOffsetZ = ITEM_FLOOR_OFFSET) {
    new const
        ItemBuild:buildid = ItemBuild:Iter_Alloc(ItemBuild)
    ;

    if (_:buildid == INVALID_ITERATOR_SLOT) {
        return INVALID_ITEM_BUILD_ID;
    }

    strcopy(gItemBuildData[buildid][E_ITEM_BUILD_NAME], name);
    gItemBuildData[buildid][E_ITEM_BUILD_MODEL_ID] = modelid;
    gItemBuildData[buildid][E_ITEM_BUILD_BUTTON_OFFSET_Z] = buttonOffsetZ;

    return buildid;
}

stock bool:IsValidItemBuild(ItemBuild:buildid) {
    return (0 <= _:buildid < ITEM_BUILD_ITER_SIZE) && Iter_Contains(ItemBuild, buildid);
}

stock bool:GetItemBuildName(ItemBuild:buildid, output[], len = sizeof (output)) {
    if (!IsValidItemBuild(buildid)) {
        return false;
    }

    strcopy(output, gItemBuildData[buildid][E_ITEM_BUILD_NAME], len);

    return true;
}

stock GetItemBuildModel(ItemBuild:buildid) {
    if (!IsValidItemBuild(buildid)) {
        return 0;
    }

    return gItemBuildData[buildid][E_ITEM_BUILD_MODEL_ID];
}

stock bool:SetItemBuildFlags(ItemBuild:buildid, flags) {
    if (!IsValidItemBuild(buildid)) {
        return false;
    }

    gItemBuildData[buildid][E_ITEM_BUILD_FLAGS] ^= flags;

    return true;
}

stock GetItemBuildFlags(ItemBuild:buildid) {
    if (!IsValidItemBuild(buildid)) {
        return 0;
    }

    return gItemBuildData[buildid][E_ITEM_BUILD_FLAGS];
}

stock bool:HasItemBuildFlags(ItemBuild:buildid, flags) {
    if (!IsValidItemBuild(buildid)) {
        return false;
    }

    return ((gItemBuildData[buildid][E_ITEM_BUILD_FLAGS] & flags) == gItemBuildData[buildid][E_ITEM_BUILD_FLAGS]);
}

/**
 * # Item
 */

stock Item:CreateItem(ItemBuild:buildid, const uuid[] = "") {
    new const
        Item:itemid = Item:Iter_Alloc(Item)
    ;

    if (_:itemid == INVALID_ITERATOR_SLOT) {
        return INVALID_ITEM_ID;
    }

    if (isnull(uuid)) {
        UUID(gItemData[itemid][E_ITEM_UUID]);
    } else {
        strcopy(gItemData[itemid][E_ITEM_UUID], uuid);
    }

    gItemData[itemid][E_ITEM_BUILD_ID] = buildid;

    // Item World
    gItemData[itemid][E_ITEM_BUTTON_ID] = INVALID_BUTTON_ID;
    gItemData[itemid][E_ITEM_OBJECT_ID] = STREAMER_TAG_OBJECT:INVALID_STREAMER_ID;

    CallLocalFunction("OnItemCreate", "i", _:itemid);

    return itemid;
}

stock bool:IsValidItem(Item:itemid) {
    if (!(0 <= _:itemid < ITEM_ITER_SIZE)) {
        return false;
    }

    return Iter_Contains(Item, itemid);
}

stock bool:DestroyItem(Item:itemid) {
    if (!IsValidItem(itemid)) {
        return false;
    }

    RemoveItemFromWorld(itemid);

    Iter_Remove(Item, itemid);

    CallLocalFunction("OnItemDestroy", "i", _:itemid);

    return true;
}

stock bool:GetItemBuild(Item:itemid, &ItemBuild:buildid) {
    if (!IsValidItem(itemid)) {
        return false;
    }

    buildid = gItemData[itemid][E_ITEM_BUILD_ID];

    return true;
}

stock bool:SetItemExtraName(Item:itemid, const format[], OPEN_MP_TAGS:...) {
    if (!IsValidItem(itemid)) {
        return false;
    }

    new
        buffer[MAX_ITEM_EXTRA_NAME]
    ;

    Format(
        buffer,
        _,
        format,
        ___(2)
    );

    strcopy(gItemData[itemid][E_ITEM_EXTRA_NAME], buffer);

    return true;
}

stock bool:GetItemExtraName(Item:itemid, output[], len = sizeof (output)) {
    if (!IsValidItem(itemid)) {
        return false;
    }

    strcopy(output, gItemData[itemid][E_ITEM_EXTRA_NAME], len);

    return true;
}

stock bool:GetItemUUID(Item:itemid, output[], len = sizeof (output)) {
    if (!IsValidItem(itemid)) {
        return false;
    }

    strcopy(output, gItemData[itemid][E_ITEM_UUID], len);

    return true;
}

stock bool:GetItemName(Item:itemid, output[], len = sizeof (output)) {
    if (!IsValidItem(itemid)) {
        return false;
    }

    new const
        ItemBuild:buildid = gItemData[itemid][E_ITEM_BUILD_ID]
    ;

    strcopy(output, gItemBuildData[buildid][E_ITEM_BUILD_NAME], len);

    if (!isnull(gItemData[itemid][E_ITEM_EXTRA_NAME])) {
        strcat(output, " (", len);
        strcat(output, gItemData[itemid][E_ITEM_EXTRA_NAME], len);
        strcat(output,  ")", len);
    }

    return true;
}

/**
 * # Item World
 */

stock bool:CreateItemInWorld(Item:itemid, Float:x, Float:y, Float:z, Float:rotationX, Float:rotationY, Float:rotationZ, KEY:buttonKey = ITEM_DEFAULT_KEY, worldid = -1, interiorid = -1, Float:distance = ITEM_DEFAULT_DISTANCE) {
    if (!IsValidItem(itemid)) {
        return false;
    }
    
    if (Iter_Contains(ItemWorld, itemid)) {
        return false;
    }

    new const
        ItemBuild:buildid = gItemData[itemid][E_ITEM_BUILD_ID]
    ;

    if (!IsValidItemBuild(buildid)) {
        return false;
    }

    new const
        Button:buttonid = CreateButton(x, y, z + gItemBuildData[buildid][E_ITEM_BUILD_BUTTON_OFFSET_Z], buttonKey)
    ;

    if (buttonid == INVALID_BUTTON_ID) {
        return false;
    }

    gItemData[itemid][E_ITEM_X] = x;
    gItemData[itemid][E_ITEM_Y] = y;
    gItemData[itemid][E_ITEM_Z] = z;

    gItemData[itemid][E_ITEM_ROT_X] = rotationX;
    gItemData[itemid][E_ITEM_ROT_Y] = rotationY;
    gItemData[itemid][E_ITEM_ROT_Z] = rotationZ;

    gItemData[itemid][E_ITEM_WORLD_ID] = worldid;
    gItemData[itemid][E_ITEM_INTERIOR_ID] = interiorid;

    

    gItemData[itemid][E_ITEM_BUTTON_ID] = buttonid;
    gItemData[itemid][E_ITEM_OBJECT_ID] = CreateDynamicObject(gItemBuildData[buildid][E_ITEM_BUILD_MODEL_ID], x, y, z, rotationX, rotationY, rotationZ, worldid, interiorid, .streamdistance = distance, .drawdistance = distance);

    SetButtonExtraData(buttonid, BUTTON_ATTRIBUTE_ITEM_ID, _:itemid);

    Iter_Add(ItemWorld, itemid);

    CallLocalFunction("OnItemCreateInWorld", "i", _:itemid);

    return true;
}

stock bool:RemoveItemFromWorld(Item:itemid) {
    if (!IsItemInWorld(itemid)) {
        return false;
    }

    DestroyButton(gItemData[itemid][E_ITEM_BUTTON_ID]);
    DestroyDynamicObject(gItemData[itemid][E_ITEM_OBJECT_ID]);

    gItemData[itemid][E_ITEM_BUTTON_ID] = INVALID_BUTTON_ID;
    gItemData[itemid][E_ITEM_OBJECT_ID] = STREAMER_TAG_OBJECT:INVALID_STREAMER_ID;

    Iter_Remove(ItemWorld, itemid);

    CallLocalFunction("OnItemRemoveFromWorld", "i", _:itemid);

    return true;
}

stock bool:IsItemInWorld(Item:itemid) {
    if (!IsValidItem(itemid)) {
        return false;
    }

    return Iter_Contains(ItemWorld, itemid);
}

stock bool:SetItemTextLabel(Item:itemid, const format[], colour = 0xFFFFFFFF, Float:distance = ITEM_DEFAULT_DISTANCE, bool:testLOS = true, Float:fOffsetZ = 0.5, OPEN_MP_TAGS:...) {
    if (!IsItemInWorld(itemid)) {
        return false;
    }

    return SetButtonTextLabel(gItemData[itemid][E_ITEM_BUTTON_ID], format, colour, distance, testLOS, fOffsetZ, ___(6));
}

stock bool:GetItemButton(Item:itemid, &Button:buttonid) {
    if (!IsItemInWorld(itemid)) {
        return false;
    }

    buttonid = gItemData[itemid][E_ITEM_BUTTON_ID];
    
    return true;
}

stock bool:SetItemButtonPos(Item:itemid, Float:x, Float:y, Float:z, bool:allowStack = false) {
    if (!IsItemInWorld(itemid)) {
        return false;
    }

    return SetButtonPos(gItemData[itemid][E_ITEM_BUTTON_ID], x, y, z, allowStack);
}

stock bool:GetItemButtonPos(Item:itemid, &Float:x, &Float:y, &Float:z) {
    if (!IsItemInWorld(itemid)) {
        return false;
    }

    return GetButtonPos(gItemData[itemid][E_ITEM_BUTTON_ID], x, y, z);
}

stock bool:HasButtonItem(Button:buttonid) {
    return HasButtonExtraData(buttonid, BUTTON_ATTRIBUTE_ITEM_ID);
}

stock bool:GetButtonItem(Button:buttonid, &Item:itemid) {
    return GetButtonExtraData(buttonid, BUTTON_ATTRIBUTE_ITEM_ID, _:itemid);
}

stock bool:GetPlayerNearbyItems(playerid, Item:items[MAX_PLAYER_BUTTONS] = { INVALID_ITEM_ID, ... }, &count = 0) {
    if (!IsPlayerInAnyButton(playerid)) {
        return false;
    }

    new
        Button:buttons[MAX_PLAYER_BUTTONS],
        buttonCount,
        Item:buttonItem
    ;

    GetPlayerNearbyButtons(playerid, buttons, buttonCount);

    count = 0;

    for (new i; i != buttonCount; ++i) {
        buttonItem = INVALID_ITEM_ID;
        GetButtonItem(buttons[i], buttonItem);

        if (Iter_Contains(ItemWorld, buttonItem)) {
            items[count++] = buttonItem;
        }
    }

    return (count > 0);
}

stock PlayerDropItem(playerid, Item:itemid) {
    if (!IsPlayerConnected(playerid)) {
        return false;
    }

    if (!IsValidItem(itemid)) {
        return false;
    }

    new
        Float:x,
        Float:y,
        Float:z,
        Float:a
    ;

    GetPlayerPos(playerid, x, y, z);
    GetPlayerFacingAngle(playerid, a);

    CreateItemInWorld(
        itemid,
        (x + (0.85 * floatsin(-a, degrees))),
        (y + (0.85 * floatcos(-a, degrees))),
        (z - ITEM_FLOOR_OFFSET),
        0.0,
        0.0,
        a,
        _,
        GetPlayerVirtualWorld(playerid),
        GetPlayerInterior(playerid)
    );

    Streamer_Update(playerid);

    return true;
}

/**
 * # Calls
 */

hook OnButtonPress(playerid, Button:buttonid) {
    if (!HasButtonItem(buttonid)) {
        return 1;
    }

    new
        Item:items[MAX_PLAYER_BUTTONS],
        itemCount
    ;

    GetPlayerNearbyItems(playerid, items, itemCount);

    if (!CallLocalFunction("OnPlayerPickUpItem", "iiai", playerid, _:items[0], _:items, itemCount)) {
        return ~0;
    }

    return ~0;
}