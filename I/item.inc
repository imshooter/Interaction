#if defined _INC_ITEM
    #endinput
#endif
#define _INC_ITEM

#include <I\button>
#include <I\button-plus>

#include <uuid>
#include <mathutil>

#include <YSI_Data\y_iterate>
#include <YSI_Coding\y_hooks>
#include <YSI_Coding\y_timers>

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

#if !defined ITEM_KEY_PICK_UP_ITEM
    #define ITEM_KEY_PICK_UP_ITEM (KEY_SECONDARY_ATTACK)
#endif

#if !defined ITEM_KEY_DROP_ITEM
    #define ITEM_KEY_DROP_ITEM (KEY_NO)
#endif

#if !defined ITEM_DEFAULT_DISTANCE
    #define ITEM_DEFAULT_DISTANCE (100.0)
#endif

#if !defined ITEM_DEFAULT_BUTTON_PRIORITY
    #define ITEM_DEFAULT_BUTTON_PRIORITY (1)
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

    Float:E_ITEM_BUILD_ROT_OFFSET_X,
    Float:E_ITEM_BUILD_ROT_OFFSET_Y,
    Float:E_ITEM_BUILD_ROT_OFFSET_Z,

    Float:E_ITEM_BUILD_OBJ_OFFSET_Z,
    Float:E_ITEM_BUILD_BTN_OFFSET_Z,

    E_ITEM_BUILD_ATTACH_BONE_ID,
    Float:E_ITEM_BUILD_ATTACH_X,
    Float:E_ITEM_BUILD_ATTACH_Y,
    Float:E_ITEM_BUILD_ATTACH_Z,
    Float:E_ITEM_BUILD_ATTACH_ROT_X,
    Float:E_ITEM_BUILD_ATTACH_ROT_Y,
    Float:E_ITEM_BUILD_ATTACH_ROT_Z,
    E_ITEM_BUILD_MATERIAL_COLOUR_1,
    E_ITEM_BUILD_MATERIAL_COLOUR_2,
    bool:E_ITEM_BUILD_USE_CARRY_ANIM
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

static stock
    Item:gInteractItemID[MAX_PLAYERS] = { INVALID_ITEM_ID, ... },
    Item:gHoldItemID[MAX_PLAYERS]     = { INVALID_ITEM_ID, ... },
    gItemInteractorID[MAX_ITEMS]      = { INVALID_PLAYER_ID, ... },
    gItemHolderID[MAX_ITEMS]          = { INVALID_PLAYER_ID, ... },
    Timer:gInteractTimer[MAX_PLAYERS] = { Timer:0, ... }
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

stock ItemBuild:BuildItem(const name[], modelid, Float:worldRotX = 0.0, Float:worldRotY = 0.0, Float:worldRotZ = 0.0, Float:objectOffsetZ = 0.0, bone = 0, Float:attachX = 0.0, Float:attachY = 0.0, Float:attachZ = 0.0, Float:attachRotX = 0.0, Float:attachRotY = 0.0, Float:attachRotZ = 0.0, materialColour1 = 0, materialColour2 = 0, bool:useCarryAnim = false, Float:buttonOffsetZ = ITEM_FLOOR_OFFSET);
stock bool:IsValidItemBuild(ItemBuild:buildid);
stock GetItemBuildName(ItemBuild:buildid, output[], len = sizeof (output));
stock GetItemBuildModel(ItemBuild:buildid);
stock SetItemBuildFlags(ItemBuild:buildid, flags);
stock GetItemBuildFlags(ItemBuild:buildid);
stock bool:HasItemBuildFlags(ItemBuild:buildid, flags);

stock Item:CreateItem(ItemBuild:buildid, const uuid[] = "");
stock bool:IsValidItem(Item:itemid);
stock DestroyItem(Item:itemid);
stock GetItemBuild(Item:itemid, &ItemBuild:buildid);
stock SetItemExtraName(Item:itemid, const format[], OPEN_MP_TAGS:...);
stock GetItemExtraName(Item:itemid, output[], len = sizeof (output));
stock GetItemUUID(Item:itemid, output[], len = sizeof (output));
stock GetItemName(Item:itemid, output[], len = sizeof (output));

stock CreateItemInWorld(Item:itemid, Float:x, Float:y, Float:z, Float:rotationX, Float:rotationY, Float:rotationZ, KEY:buttonKey = ITEM_KEY_PICK_UP_ITEM, worldid = -1, interiorid = -1, Float:distance = ITEM_DEFAULT_DISTANCE, priority = ITEM_DEFAULT_BUTTON_PRIORITY);
stock bool:IsItemInWorld(Item:itemid);
stock RemoveItemFromWorld(Item:itemid);
stock SetItemPos(Item:itemid, Float:x, Float:y, Float:z);
stock GetItemPos(Item:itemid, &Float:x, &Float:y, &Float:z);
stock SetItemRot(Item:itemid, Float:rotationX, Float:rotationY, Float:rotationZ, bool:useRotOffsets = true);
stock GetItemRot(Item:itemid, &Float:rotationX, &Float:rotationY, &Float:rotationZ);
stock SetItemVirtualWorld(Item:itemid, worldid);
stock GetItemVirtualWorld(Item:itemid);
stock SetItemInterior(Item:itemid, interiorid);
stock GetItemInterior(Item:itemid, &interiorid);
stock SetItemTextLabel(Item:itemid, const format[], colour = 0xFFFFFFFF, Float:distance = ITEM_DEFAULT_DISTANCE, bool:testLOS = true, Float:fOffsetZ = 0.5, OPEN_MP_TAGS:...);
stock GetItemButton(Item:itemid, &Button:buttonid);
stock bool:HasButtonItem(Button:buttonid);
stock bool:GetButtonItem(Button:buttonid, &Item:itemid);
stock GetPlayerNearbyItems(playerid, Item:items[MAX_AREA_BUTTONS] = { INVALID_ITEM_ID, ... }, &count = 0);
stock GivePlayerItem(playerid, Item:itemid);
stock GiveWorldItemToPlayer(playerid, Item:itemid);

stock PlayerPickUpItem(playerid, Item:itemid);
stock PlayerDropItem(playerid);
stock RemoveCurrentItem(playerid);
stock GetItemHolder(Item:itemid);
stock GetItemInteractor(Item:itemid);
stock GetPlayerItem(playerid, &Item:itemid);
stock GetPlayerInteractItem(playerid, &Item:itemid);

/**
 * # Events
 */

forward OnItemCreate(Item:itemid);

/**
 * # OnItemCreate
 *
 * # Returns
 *   - This callback does not handle returns.
 */

forward OnItemDestroy(Item:itemid);

/**
 * # OnItemDestroy
 *
 * # Returns
 *   - This callback does not handle returns.
 */

forward OnItemCreateInWorld(Item:itemid);

/**
 * # OnItemCreateInWorld
 *
 * # Returns
 *   - This callback does not handle returns.
 */

forward OnItemRemoveFromWorld(Item:itemid);

/**
 * # OnItemRemoveFromWorld
 *
 * # Returns
 *   - This callback does not handle returns.
 */

forward OnPlayerEnterItemArea(playerid, Item:itemid);

/**
* # OnPlayerEnterItemArea
*
* # Returns
*   - `0`: (Continue) Continues subsequent calls to `OnPlayerEnterButtonArea`.
*   - `1`: (OK) Breaks subsequent calls to `OnPlayerEnterButtonArea`.
*/

forward OnPlayerLeaveItemArea(playerid, Item:itemid);

/**
 * # OnPlayerLeaveItemArea
 *
 * # Returns
 *   - `0`: (Continue) Continues subsequent calls to `OnPlayerLeaveButtonArea`.
 *   - `1`: (OK) Breaks subsequent calls to `OnPlayerLeaveButtonArea`.
 */

forward OnPlayerPickUpItem(playerid, Item:itemid, Item:items[], count);

/**
 * # OnPlayerPickUpItem
 *
 * # Returns
 *   - `0`: Allows the player the animation of picking up the item from the world.
 *   - `1`: Prevents the animation from picking up the item in the world.
 */

forward OnPlayerPickedUpItem(playerid, Item:itemid);

/**
 * # OnPlayerPickedUpItem
 *
 * # Returns
 *   - `0`: Allows the item to be attached to the player.
 *   - `1`: Prevents the item from being attached to the player.
 */

forward OnPlayerDroppedItem(playerid, Item:itemid);

/**
 * # OnPlayerDroppedItem
 *
 * # Returns
 *   - This callback does not handle returns.
 */

forward OnItemAttachToPlayer(playerid, Item:itemid);

/**
 * # OnItemAttachToPlayer
 *
 * # Returns
 *   - This callback does not handle returns.
 */

forward OnItemDetachFromPlayer(playerid, Item:itemid);

/**
 * # OnItemDetachFromPlayer
 *
 * # Returns
 *   - This callback does not handle returns.
 */

/**
 * # Item Build
 */

stock ItemBuild:BuildItem(const name[], modelid, Float:worldRotX = 0.0, Float:worldRotY = 0.0, Float:worldRotZ = 0.0, Float:objectOffsetZ = 0.0, bone = 0, Float:attachX = 0.0, Float:attachY = 0.0, Float:attachZ = 0.0, Float:attachRotX = 0.0, Float:attachRotY = 0.0, Float:attachRotZ = 0.0, materialColour1 = 0, materialColour2 = 0, bool:useCarryAnim = false, Float:buttonOffsetZ = ITEM_FLOOR_OFFSET) {
    new const
        ItemBuild:buildid = ItemBuild:Iter_Alloc(ItemBuild)
    ;

    if (_:buildid == INVALID_ITERATOR_SLOT) {
        return INVALID_ITEM_BUILD_ID;
    }

    strcopy(gItemBuildData[buildid][E_ITEM_BUILD_NAME], name);
    gItemBuildData[buildid][E_ITEM_BUILD_MODEL_ID] = modelid;

    gItemBuildData[buildid][E_ITEM_BUILD_ROT_OFFSET_X] = worldRotX;
    gItemBuildData[buildid][E_ITEM_BUILD_ROT_OFFSET_Y] = worldRotY;
    gItemBuildData[buildid][E_ITEM_BUILD_ROT_OFFSET_Z] = worldRotZ;

    gItemBuildData[buildid][E_ITEM_BUILD_OBJ_OFFSET_Z] = objectOffsetZ;
    gItemBuildData[buildid][E_ITEM_BUILD_BTN_OFFSET_Z] = buttonOffsetZ;

    gItemBuildData[buildid][E_ITEM_BUILD_ATTACH_BONE_ID] = bone;
    gItemBuildData[buildid][E_ITEM_BUILD_ATTACH_X] = attachX;
    gItemBuildData[buildid][E_ITEM_BUILD_ATTACH_Y] = attachY;
    gItemBuildData[buildid][E_ITEM_BUILD_ATTACH_Z] = attachZ;
    gItemBuildData[buildid][E_ITEM_BUILD_ATTACH_ROT_X] = attachRotX;
    gItemBuildData[buildid][E_ITEM_BUILD_ATTACH_ROT_Y] = attachRotY;
    gItemBuildData[buildid][E_ITEM_BUILD_ATTACH_ROT_Z] = attachRotZ;
    gItemBuildData[buildid][E_ITEM_BUILD_MATERIAL_COLOUR_1] = materialColour1;
    gItemBuildData[buildid][E_ITEM_BUILD_MATERIAL_COLOUR_2] = materialColour2;
    gItemBuildData[buildid][E_ITEM_BUILD_USE_CARRY_ANIM] = useCarryAnim;

    return buildid;
}

stock bool:IsValidItemBuild(ItemBuild:buildid) {
    if (!(0 <= _:buildid < ITEM_BUILD_ITER_SIZE)) {
        return false;
    }

    return Iter_Contains(ItemBuild, buildid);
}

stock GetItemBuildName(ItemBuild:buildid, output[], len = sizeof (output)) {
    if (!IsValidItemBuild(buildid)) {
        return 1;
    }

    strcopy(output, gItemBuildData[buildid][E_ITEM_BUILD_NAME], len);

    return 0;
}

stock GetItemBuildModel(ItemBuild:buildid) {
    if (!IsValidItemBuild(buildid)) {
        return 0;
    }

    return gItemBuildData[buildid][E_ITEM_BUILD_MODEL_ID];
}

stock SetItemBuildFlags(ItemBuild:buildid, flags) {
    if (!IsValidItemBuild(buildid)) {
        return 1;
    }

    gItemBuildData[buildid][E_ITEM_BUILD_FLAGS] ^= flags;

    return 0;
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

    CallLocalFunction("OnItemCreate", "i", _:itemid);

    return itemid;
}

stock bool:IsValidItem(Item:itemid) {
    if (!(0 <= _:itemid < ITEM_ITER_SIZE)) {
        return false;
    }

    return Iter_Contains(Item, itemid);
}

stock DestroyItem(Item:itemid) {
    if (!IsValidItem(itemid)) {
        return 1;
    }

    RemoveItemFromWorld(itemid);
    RemoveCurrentItem(GetItemHolder(itemid));

    Iter_Remove(Item, itemid);

    CallLocalFunction("OnItemDestroy", "i", _:itemid);

    return 0;
}

stock GetItemBuild(Item:itemid, &ItemBuild:buildid) {
    if (!IsValidItem(itemid)) {
        return 1;
    }

    buildid = gItemData[itemid][E_ITEM_BUILD_ID];

    return 0;
}

stock SetItemExtraName(Item:itemid, const format[], OPEN_MP_TAGS:...) {
    if (!IsValidItem(itemid)) {
        return 1;
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

    return 0;
}

stock GetItemExtraName(Item:itemid, output[], len = sizeof (output)) {
    if (!IsValidItem(itemid)) {
        return 1;
    }

    strcopy(output, gItemData[itemid][E_ITEM_EXTRA_NAME], len);

    return 0;
}

stock GetItemUUID(Item:itemid, output[], len = sizeof (output)) {
    if (!IsValidItem(itemid)) {
        return 1;
    }

    strcopy(output, gItemData[itemid][E_ITEM_UUID], len);

    return 0;
}

stock GetItemName(Item:itemid, output[], len = sizeof (output)) {
    if (!IsValidItem(itemid)) {
        return 1;
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

    return 0;
}

/**
 * # Item World
 */

stock CreateItemInWorld(Item:itemid, Float:x, Float:y, Float:z, Float:rotationX, Float:rotationY, Float:rotationZ, KEY:buttonKey = ITEM_KEY_PICK_UP_ITEM, worldid = -1, interiorid = -1, Float:distance = ITEM_DEFAULT_DISTANCE, priority = ITEM_DEFAULT_BUTTON_PRIORITY) {
    if (!IsValidItem(itemid)) {
        return 1;
    }
    
    if (Iter_Contains(ItemWorld, itemid)) {
        return 2;
    }

    new const
        ItemBuild:buildid = gItemData[itemid][E_ITEM_BUILD_ID]
    ;

    if (!IsValidItemBuild(buildid)) {
        return 3;
    }

    new const
        Button:buttonid = CreateButton(x, y, z + gItemBuildData[buildid][E_ITEM_BUILD_BTN_OFFSET_Z], buttonKey, 1.0, worldid, interiorid, priority)
    ;

    if (buttonid == INVALID_BUTTON_ID) {
        return 4;
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
    gItemData[itemid][E_ITEM_OBJECT_ID] = CreateDynamicObject(gItemBuildData[buildid][E_ITEM_BUILD_MODEL_ID], x, y, z + gItemBuildData[buildid][E_ITEM_BUILD_OBJ_OFFSET_Z], rotationX, rotationY, rotationZ, worldid, interiorid, .streamdistance = distance, .drawdistance = distance);

    SetButtonExtraData(buttonid, BUTTON_ATTRIBUTE_ITEM_ID, _:itemid);

    Iter_Add(ItemWorld, itemid);

    CallLocalFunction("OnItemCreateInWorld", "i", _:itemid);

    return 0;
}

stock bool:IsItemInWorld(Item:itemid) {
    if (!IsValidItem(itemid)) {
        return false;
    }

    return Iter_Contains(ItemWorld, itemid);
}

stock RemoveItemFromWorld(Item:itemid) {
    if (!IsValidItem(itemid)) {
        return 1;
    }

    if (!IsItemInWorld(itemid)) {
        return 2;
    }

    DestroyButton(gItemData[itemid][E_ITEM_BUTTON_ID]);
    DestroyDynamicObject(gItemData[itemid][E_ITEM_OBJECT_ID]);

    gItemData[itemid][E_ITEM_BUTTON_ID] = INVALID_BUTTON_ID;
    gItemData[itemid][E_ITEM_OBJECT_ID] = STREAMER_TAG_OBJECT:INVALID_STREAMER_ID;

    Iter_Remove(ItemWorld, itemid);

    CallLocalFunction("OnItemRemoveFromWorld", "i", _:itemid);

    return 0;
}

stock SetItemPos(Item:itemid, Float:x, Float:y, Float:z) {
    if (!IsValidItem(itemid)) {
        return 1;
    }

    if (!IsItemInWorld(itemid)) {
        return 2;
    }

    if (SetButtonPos(gItemData[itemid][E_ITEM_BUTTON_ID], x, y, z + gItemBuildData[buildid][E_ITEM_BUILD_BTN_OFFSET_Z])) {
        return 3;
    }

    gItemData[itemid][E_ITEM_X] = x;
    gItemData[itemid][E_ITEM_Y] = y;
    gItemData[itemid][E_ITEM_Z] = z;

    SetDynamicObjectPos(gItemData[itemid][E_ITEM_OBJECT_ID], x, y, z + gItemBuildData[buildid][E_ITEM_BUILD_OBJ_OFFSET_Z]);

    return 0;
}

stock GetItemPos(Item:itemid, &Float:x, &Float:y, &Float:z) {
    if (!IsValidItem(itemid)) {
        return 1;
    }

    if (!IsItemInWorld(itemid)) {
        return 2;
    }

    x = gItemData[itemid][E_ITEM_X];
    y = gItemData[itemid][E_ITEM_Y];
    z = gItemData[itemid][E_ITEM_Z];

    return 0;
}

stock SetItemRot(Item:itemid, Float:rotationX, Float:rotationY, Float:rotationZ, bool:useRotOffsets = true) {
    if (!IsValidItem(itemid)) {
        return 1;
    }

    if (!IsItemInWorld(itemid)) {
        return 2;
    }

    if (useRotOffsets) {
        SetDynamicObjectRot(gItemData[itemid][E_ITEM_OBJECT_ID],
            rotationX + gBuildData[gItemData[itemid][E_ITEM_BUILD_ID]][E_BUILD_WORLD_OFFSET_X],
            rotationY + gBuildData[gItemData[itemid][E_ITEM_BUILD_ID]][E_BUILD_WORLD_OFFSET_Y],
            rotationZ + gBuildData[gItemData[itemid][E_ITEM_BUILD_ID]][E_BUILD_WORLD_OFFSET_Z]
        );
    } else {
        SetDynamicObjectRot(gItemData[itemid][E_ITEM_OBJECT_ID], rotationX, rotationY, rotationZ);
    }

    gItemData[itemid][E_ITEM_ROT_X] = rotationX;
    gItemData[itemid][E_ITEM_ROT_Y] = rotationY;
    gItemData[itemid][E_ITEM_ROT_Z] = rotationZ;

    return 0;
}

stock GetItemRot(Item:itemid, &Float:rotationX, &Float:rotationY, &Float:rotationZ) {
    if (!IsValidItem(itemid)) {
        return 1;
    }

    if (!IsItemInWorld(itemid)) {
        return 2;
    }

    rotationX = gItemData[itemid][E_ITEM_ROT_X];
    rotationY = gItemData[itemid][E_ITEM_ROT_Y];
    rotationZ = gItemData[itemid][E_ITEM_ROT_Z];

    return 0;
}

stock SetItemVirtualWorld(Item:itemid, worldid) {
    if (!IsValidItem(itemid)) {
        return 1;
    }

    if (!IsItemInWorld(itemid)) {
        return 2;
    }

    SetButtonVirtualWorld(gItemData[itemid][E_ITEM_BUTTON_ID], worldid);
    gItemData[itemid][E_ITEM_WORLD_ID] = worldid;

    return 0;
}

stock GetItemVirtualWorld(Item:itemid) {
    if (!IsValidItem(itemid)) {
        return 1;
    }

    if (!IsItemInWorld(itemid)) {
        return 2;
    }

    worldid = gItemData[itemid][E_ITEM_WORLD_ID];

    return 0;
}

stock SetItemInterior(Item:itemid, interiorid) {
    if (!IsValidItem(itemid)) {
        return 1;
    }

    if (!IsItemInWorld(itemid)) {
        return 2;
    }

    SetButtonInterior(gItemData[itemid][E_ITEM_BUTTON_ID], interiorid);
    gItemData[itemid][E_ITEM_INTERIOR_ID] = interiorid;

    return 0;
}

stock GetItemInterior(Item:itemid, &interiorid) {
    if (!IsValidItem(itemid)) {
        return 1;
    }

    if (!IsItemInWorld(itemid)) {
        return 2;
    }

    interiorid = gItemData[itemid][E_ITEM_WORLD_ID];

    return 0;
}

stock SetItemTextLabel(Item:itemid, const format[], colour = 0xFFFFFFFF, Float:distance = ITEM_DEFAULT_DISTANCE, bool:testLOS = true, Float:fOffsetZ = 0.5, OPEN_MP_TAGS:...) {
    if (!IsValidItem(itemid)) {
        return 1;
    }

    if (!IsItemInWorld(itemid)) {
        return 2;
    }

    SetButtonTextLabel(gItemData[itemid][E_ITEM_BUTTON_ID], format, colour, distance, testLOS, fOffsetZ, ___(6));

    return 0;
}

stock GetItemButton(Item:itemid, &Button:buttonid) {
    if (!IsValidItem(itemid)) {
        return 1;
    }

    if (!IsItemInWorld(itemid)) {
        return 2;
    }

    buttonid = gItemData[itemid][E_ITEM_BUTTON_ID];

    return 0;
}

stock bool:HasButtonItem(Button:buttonid) {
    return HasButtonExtraData(buttonid, BUTTON_ATTRIBUTE_ITEM_ID);
}

stock bool:GetButtonItem(Button:buttonid, &Item:itemid) {
    return GetButtonExtraData(buttonid, BUTTON_ATTRIBUTE_ITEM_ID, _:itemid);
}

stock GetPlayerNearbyItems(playerid, Item:items[MAX_AREA_BUTTONS] = { INVALID_ITEM_ID, ... }, &count = 0) {
    if (!IsPlayerInAnyButton(playerid)) {
        return 1;
    }

    new
        Button:buttons[MAX_AREA_BUTTONS],
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

    return (count == 0) ? 2 : 0;
}

stock GivePlayerItem(playerid, Item:itemid) {
    if (!IsPlayerConnected(playerid)) {
        return 1;
    }

    if (!IsValidItem(itemid)) {
        return 2;
    }

    AttachItemInternal(playerid, itemid);

    return 0;
}

stock GiveWorldItemToPlayer(playerid, Item:itemid) {
    if (!IsPlayerConnected(playerid)) {
        return 1;
    }

    if (!IsItemInWorld(itemid)) {
        return 2;
    }

    AttachItemInternal(playerid, itemid);

    return 0;
}

/**
 * # Interaction
 */

stock timer PickUpItemDelay[400](playerid, bool:animation) {
    if (animation) {
        ApplyAnimation(playerid, "BOMBER", "BOM_PLANT_2IDLE", 4.0, false, false, false, false, 0);
    }

    new const
        Item:itemid = gInteractItemID[playerid]
    ;

    if (CallLocalFunction("OnPlayerPickedUpItem", "ii", playerid, _:itemid)) {
        return;
    }

    GiveWorldItemToPlayer(playerid, itemid);
}

stock timer DropItemDelay[400](playerid) {
    if (!IsValidItem(gHoldItemID[playerid])) {
        return;
    }

    new
        Float:x,
        Float:y,
        Float:z,
        Float:a
    ;

    GetPlayerPos(playerid, x, y, z);
    GetPlayerFacingAngle(playerid, a);

    new const
        Item:itemid = gHoldItemID[playerid]
    ;

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

    // Detach
    RemoveCurrentItem(playerid);
    ApplyAnimation(playerid, "BOMBER", "BOM_PLANT_2IDLE", 4.0, false, false, false, false, 0);
    
    // Update
    Streamer_Update(playerid);

    // Calls
    CallLocalFunction("OnPlayerDroppedItem", "ii", playerid, _:itemid);
    CallLocalFunction("OnItemDetachFromPlayer", "ii", playerid, _:itemid);
}

stock PlayerPickUpItem(playerid, Item:itemid) {
    if (!IsValidItem(itemid)) {
        return 1;
    }

    if (IsValidItem(gHoldItemID[playerid])) {
        return 2;
    }

    if (gItemInteractorID[itemid] != INVALID_PLAYER_ID) {
        return 3;
    }

    new
        Float:x,
        Float:y,
        Float:z
    ;

    GetPlayerPos(playerid, x, y, z);
    ClearAnimations(playerid, SYNC_NONE);

    SetPlayerPos(playerid, x, y, z);
    SetPlayerFacingAngle(playerid, GetAngleToPoint(x, y, gItemData[itemid][E_ITEM_X], gItemData[itemid][E_ITEM_Y]));

    new const
        ItemBuild:buildid = gItemData[itemid][E_ITEM_BUILD_ID]
    ;

    gInteractItemID[playerid] = itemid;
    gItemInteractorID[itemid] = playerid;

    if ((z - gItemData[itemid][E_ITEM_Z]) < 0.3) {
        if (gItemBuildData[buildid][E_ITEM_BUILD_USE_CARRY_ANIM]) {
            ApplyAnimation(playerid, "CARRY", "LIFTUP105", 5.0, false, false, false, false, 400);
        } else {
            ApplyAnimation(playerid, "CASINO", "SLOT_PLYR", 4.0, false, false, false, false, 0);
        }

        gInteractTimer[playerid] = defer PickUpItemDelay(playerid, false);
    } else {
        if (gItemBuildData[buildid][E_ITEM_BUILD_USE_CARRY_ANIM]) {
            ApplyAnimation(playerid, "CARRY", "LIFTUP", 5.0, false, false, false, false, 400);
        } else {
            ApplyAnimation(playerid, "BOMBER", "BOM_PLANT_IN", 5.0, false, false, false, false, 450);
        }

        gInteractTimer[playerid] = defer PickUpItemDelay(playerid, true);
    }

    return 0;
}

stock PlayerDropItem(playerid) {
    if (!IsPlayerConnected(playerid)) {
        return 1;
    }

    if (!IsValidItem(gHoldItemID[playerid])) {
        return 2;
    }

    if (CallLocalFunction("OnPlayerDropItem", "ii", playerid, _:gHoldItemID[playerid])) {
        return 3;
    }

    new const
        Item:itemid = gHoldItemID[playerid],
        ItemBuild:buildid = gItemData[itemid][E_ITEM_BUILD_ID]
    ;

    gInteractItemID[playerid] = itemid;
    gItemInteractorID[itemid] = playerid;

    if (gItemBuildData[buildid][E_ITEM_BUILD_USE_CARRY_ANIM]) {
        ApplyAnimation(playerid, "CARRY", "PUTDWN", 5.0, false, false, false, false, 0);
    } else {
        ApplyAnimation(playerid, "BOMBER", "BOM_PLANT_IN", 5.0, true, false, false, false, 450);
    }

    gInteractTimer[playerid] = defer DropItemDelay(playerid);

    return 0;
}

stock RemoveCurrentItem(playerid) {
    if (!IsPlayerConnected(playerid)) {
        return 1;
    }

    if (!IsValidItem(gHoldItemID[playerid])) {
        return 2;
    }

    new const
        Item:itemid = gHoldItemID[playerid],
        ItemBuild:buildid = gItemData[itemid][E_ITEM_BUILD_ID]
    ;

    if (IsPlayerAttachedObjectSlotUsed(playerid, ITEM_ATTACH_INDEX)) {
        RemovePlayerAttachedObject(playerid, ITEM_ATTACH_INDEX);
    }
    
    if (gItemBuildData[buildid][E_ITEM_BUILD_USE_CARRY_ANIM]) {
        SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
    }

    gHoldItemID[playerid] = INVALID_ITEM_ID;
    gItemHolderID[itemid] = INVALID_PLAYER_ID;
    gInteractItemID[playerid] = INVALID_ITEM_ID;
    gItemInteractorID[itemid] = INVALID_PLAYER_ID;

    return 0;
}

stock GetItemHolder(Item:itemid) {
    if (!IsValidItem(itemid)) {
        return INVALID_PLAYER_ID;
    }

    return gItemHolderID[itemid];
}

stock GetItemInteractor(Item:itemid) {
    if (!IsValidItem(itemid)) {
        return INVALID_PLAYER_ID;
    }

    return gItemInteractorID[itemid];
}

stock GetPlayerItem(playerid, &Item:itemid) {
    if (!IsPlayerConnected(playerid)) {
        return 1;
    }

    itemid = gHoldItemID[playerid];

    return 0;
}

stock GetPlayerInteractItem(playerid, &Item:itemid) {
    if (!IsPlayerConnected(playerid)) {
        return 1;
    }

    itemid = gInteractItemID[playerid];

    return 0;
}

/**
 * # Internal
 */

static stock void:AttachItemInternal(playerid, Item:itemid) {
    new const
        ItemBuild:buildid = gItemData[itemid][E_ITEM_BUILD_ID]
    ;

    RemoveItemFromWorld(itemid);
    RemoveCurrentItem(GetItemHolder(itemid));

    gItemData[itemid][E_ITEM_X] = 0.0;
    gItemData[itemid][E_ITEM_Y] = 0.0;
    gItemData[itemid][E_ITEM_Z] = 0.0;

    gHoldItemID[playerid] = itemid;
    gItemHolderID[itemid] = playerid;
    gInteractItemID[playerid] = INVALID_ITEM_ID;
    gItemInteractorID[itemid] = INVALID_PLAYER_ID;

    SetPlayerAttachedObject(playerid,
        ITEM_ATTACH_INDEX,
        gItemBuildData[buildid][E_ITEM_BUILD_MODEL_ID],
        gItemBuildData[buildid][E_ITEM_BUILD_ATTACH_BONE_ID],
        gItemBuildData[buildid][E_ITEM_BUILD_ATTACH_X],
        gItemBuildData[buildid][E_ITEM_BUILD_ATTACH_Y],
        gItemBuildData[buildid][E_ITEM_BUILD_ATTACH_Z],
        gItemBuildData[buildid][E_ITEM_BUILD_ATTACH_ROT_X],
        gItemBuildData[buildid][E_ITEM_BUILD_ATTACH_ROT_Y],
        gItemBuildData[buildid][E_ITEM_BUILD_ATTACH_ROT_Z],
        _,
        _,
        _,
        gItemBuildData[buildid][E_ITEM_BUILD_MATERIAL_COLOUR_1],
        gItemBuildData[buildid][E_ITEM_BUILD_MATERIAL_COLOUR_2]
    );

    if (gItemBuildData[buildid][E_ITEM_BUILD_USE_CARRY_ANIM]) {
        SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);
    }

    CallLocalFunction("OnItemAttachToPlayer", "ii", playerid, _:itemid);
}

/**
 * # Calls
 */

hook OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys) {
    if (IsPlayerInAnyVehicle(playerid)) {
        return 1;
    }

    if (GetPlayerState(playerid) == PLAYER_STATE_SPECTATING) {
        return 1;
    }

    if (newkeys & ITEM_KEY_DROP_ITEM) {
        PlayerDropItem(playerid);
    }

    return 1;
}

hook OnButtonPress(playerid, Button:buttonid) {
    if (HasButtonItem(buttonid)) {
        new
            Item:items[MAX_AREA_BUTTONS],
            itemCount
        ;

        GetPlayerNearbyItems(playerid, items, itemCount);

        if (CallLocalFunction("OnPlayerPickUpItem", "iiai", playerid, _:items[0], _:items, itemCount)) {
            return ~1;
        }

        PlayerPickUpItem(playerid, items[0]);

        return ~1;
    }

    return 0;
}

hook OnPlayerEnterButtonArea(playerid, Button:buttonid) {
    if (HasButtonItem(buttonid)) {
        new
            Item:itemid
        ;

        GetButtonItem(buttonid, itemid);

        if (CallLocalFunction("OnPlayerEnterItemArea", "ii", playerid, _:itemid)) {
            return ~1;
        }
    }
    
    return 0;
}

hook OnPlayerLeaveButtonArea(playerid, Button:buttonid) {
    if (HasButtonItem(buttonid)) {
        new
            Item:itemid
        ;

        GetButtonItem(buttonid, itemid);

        if (CallLocalFunction("OnPlayerLeaveItemArea", "ii", playerid, _:itemid)) {
            return ~1;
        }
    }
    
    return 0;
}

hook OnPlayerDisconnect(playerid, reason) {
    if (gHoldItemID[playerid] != INVALID_ITEM_ID) {
        /**
         * The Item will still exist if the player disconnects while holding an item.
         */

        RemoveCurrentItem(playerid);
    }
    
    return 1;
}