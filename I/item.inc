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

#if !defined ITEM_KEY_USE_ITEM
    #define ITEM_KEY_USE_ITEM (KEY_SECONDARY_ATTACK)
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
    E_ITEM_BUILD_NAME[MAX_ITEM_BUILD_NAME],
    E_ITEM_BUILD_MODEL_ID,
    E_ITEM_BUILD_FLAGS,

    Float:E_ITEM_BUILD_ROT_OFFSET_X,
    Float:E_ITEM_BUILD_ROT_OFFSET_Y,
    Float:E_ITEM_BUILD_ROT_OFFSET_Z,

    Float:E_ITEM_BUILD_OBJ_OFFSET_Z,
    Float:E_ITEM_BUILD_BTN_OFFSET_Z,

    Float:E_ITEM_BUILD_ATTACH_X,
    Float:E_ITEM_BUILD_ATTACH_Y,
    Float:E_ITEM_BUILD_ATTACH_Z,
    Float:E_ITEM_BUILD_ATTACH_ROT_X,
    Float:E_ITEM_BUILD_ATTACH_ROT_Y,
    Float:E_ITEM_BUILD_ATTACH_ROT_Z,
    E_ITEM_BUILD_MATERIAL_COLOUR_1,
    E_ITEM_BUILD_MATERIAL_COLOUR_2,
    bool:E_ITEM_BUILD_USE_CARRY_ANIM,
    E_ITEM_BUILD_ATTACH_BONE_ID
};

static
    gItemBuildData[MAX_ITEM_BUILDS][E_ITEM_BUILD_DATA]
;

static enum E_ITEM_DATA {
    ItemBuild:E_ITEM_BUILD_ID,
    E_ITEM_MODEL_ID,
    
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
    E_ITEM_EXTRA_NAME[MAX_ITEM_NAME]
};

static
    gItemData[MAX_ITEMS][E_ITEM_DATA]
;

static
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

forward ItemBuild:BuildItem(const name[], modelid, Float:worldRotX = 0.0, Float:worldRotY = 0.0, Float:worldRotZ = 0.0, Float:objectOffsetZ = 0.0, Float:attachX = 0.0, Float:attachY = 0.0, Float:attachZ = 0.0, Float:attachRotX = 0.0, Float:attachRotY = 0.0, Float:attachRotZ = 0.0, materialColour1 = 0, materialColour2 = 0, bool:useCarryAnim = false, bone = 6, Float:buttonOffsetZ = ITEM_FLOOR_OFFSET);
forward bool:IsValidItemBuild(ItemBuild:buildid);
forward bool:GetItemBuildName(ItemBuild:buildid, output[], len = sizeof (output));
forward GetItemBuildModel(ItemBuild:buildid);
forward bool:SetItemBuildFlags(ItemBuild:buildid, flags);
forward GetItemBuildFlags(ItemBuild:buildid);
forward bool:HasItemBuildFlags(ItemBuild:buildid, flags);

forward Item:CreateItem(ItemBuild:buildid, modelid = -1, const uuid[] = "");
forward bool:IsValidItem(Item:itemid);
forward bool:DestroyItem(Item:itemid);
forward ItemBuild:GetItemBuild(Item:itemid, &ItemBuild:buildid = INVALID_ITEM_BUILD_ID);
forward bool:SetItemModel(Item:itemid, modelid);
forward GetItemModel(Item:itemid);
forward bool:SetItemExtraName(Item:itemid, const format[], OPEN_MP_TAGS:...);
forward bool:GetItemExtraName(Item:itemid, output[], len = sizeof (output));
forward bool:GetItemUUID(Item:itemid, output[], len = sizeof (output));
forward bool:GetItemName(Item:itemid, output[], len = sizeof (output));

forward bool:CreateItemInWorld(Item:itemid, Float:x, Float:y, Float:z, Float:rotationX, Float:rotationY, Float:rotationZ, KEY:buttonKey = ITEM_KEY_PICK_UP_ITEM, worldid = -1, interiorid = -1, Float:distance = ITEM_DEFAULT_DISTANCE, priority = ITEM_DEFAULT_BUTTON_PRIORITY);
forward bool:IsItemInWorld(Item:itemid);
forward bool:RemoveItemFromWorld(Item:itemid);
forward bool:SetItemPos(Item:itemid, Float:x, Float:y, Float:z);
forward bool:GetItemPos(Item:itemid, &Float:x, &Float:y, &Float:z);
forward bool:SetItemRot(Item:itemid, Float:rotationX, Float:rotationY, Float:rotationZ, bool:useRotOffsets = true);
forward bool:GetItemRot(Item:itemid, &Float:rotationX, &Float:rotationY, &Float:rotationZ);
forward bool:SetItemVirtualWorld(Item:itemid, worldid);
forward GetItemVirtualWorld(Item:itemid);
forward bool:SetItemInterior(Item:itemid, interiorid);
forward GetItemInterior(Item:itemid);
forward bool:SetItemLabelText(Item:itemid, const format[], colour = 0xFFFFFFFF, Float:distance = ITEM_DEFAULT_DISTANCE, bool:testLOS = true, Float:fOffsetZ = 0.5, OPEN_MP_TAGS:...);
forward bool:GetItemButton(Item:itemid, &Button:buttonid);
forward bool:HasButtonItem(Button:buttonid);
forward bool:GetButtonItem(Button:buttonid, &Item:itemid);
forward bool:GetPlayerNearbyItems(playerid, Item:items[MAX_AREA_BUTTONS] = { INVALID_ITEM_ID, ... }, &count = 0);
forward bool:SetPlayerItem(playerid, Item:itemid);
forward bool:GiveWorldItemToPlayer(playerid, Item:itemid);

forward bool:PlayerPickUpItem(playerid, Item:itemid);
forward bool:PlayerDropItem(playerid);
forward bool:RemoveCurrentItem(playerid);
forward bool:PlayerUseItem(playerid);
forward GetItemHolder(Item:itemid);
forward GetItemInteractor(Item:itemid);
forward Item:GetPlayerItem(playerid, &Item:itemid = INVALID_ITEM_ID);
forward Item:GetPlayerInteractItem(playerid, &Item:itemid = INVALID_ITEM_ID);

/**
 * # Events
 */

forward OnItemCreate(Item:itemid);
forward OnItemDestroy(Item:itemid);
forward OnItemCreateInWorld(Item:itemid);
forward OnItemRemoveFromWorld(Item:itemid);
forward OnPlayerEnterItemArea(playerid, Item:itemid);
forward OnPlayerLeaveItemArea(playerid, Item:itemid);
forward OnPlayerPickUpItem(playerid, Item:itemid);
forward OnPlayerPickUpItemWithItem(playerid, Item:itemid1, Item:itemid2);
forward OnPlayerPickedUpItem(playerid, Item:itemid);
forward OnPlayerDropItem(playerid, Item:itemid);
forward OnPlayerDroppedItem(playerid, Item:itemid);
forward OnPlayerGetItem(playerid, Item:itemid);
forward OnPlayerGotItem(playerid, Item:itemid);
forward OnPlayerUseItem(playerid, Item:itemid);

/**
 * # Item Build
 */

stock ItemBuild:BuildItem(const name[], modelid, Float:worldRotX = 0.0, Float:worldRotY = 0.0, Float:worldRotZ = 0.0, Float:objectOffsetZ = 0.0, Float:attachX = 0.0, Float:attachY = 0.0, Float:attachZ = 0.0, Float:attachRotX = 0.0, Float:attachRotY = 0.0, Float:attachRotZ = 0.0, materialColour1 = 0, materialColour2 = 0, bool:useCarryAnim = false, bone = 6, Float:buttonOffsetZ = ITEM_FLOOR_OFFSET) {
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

    gItemBuildData[buildid][E_ITEM_BUILD_ATTACH_X] = attachX;
    gItemBuildData[buildid][E_ITEM_BUILD_ATTACH_Y] = attachY;
    gItemBuildData[buildid][E_ITEM_BUILD_ATTACH_Z] = attachZ;
    gItemBuildData[buildid][E_ITEM_BUILD_ATTACH_ROT_X] = attachRotX;
    gItemBuildData[buildid][E_ITEM_BUILD_ATTACH_ROT_Y] = attachRotY;
    gItemBuildData[buildid][E_ITEM_BUILD_ATTACH_ROT_Z] = attachRotZ;
    gItemBuildData[buildid][E_ITEM_BUILD_MATERIAL_COLOUR_1] = materialColour1;
    gItemBuildData[buildid][E_ITEM_BUILD_MATERIAL_COLOUR_2] = materialColour2;
    gItemBuildData[buildid][E_ITEM_BUILD_USE_CARRY_ANIM] = useCarryAnim;
    gItemBuildData[buildid][E_ITEM_BUILD_ATTACH_BONE_ID] = bone;

    return buildid;
}

stock bool:IsValidItemBuild(ItemBuild:buildid) {
    if (!(0 <= _:buildid < ITEM_BUILD_ITER_SIZE)) {
        return false;
    }

    return Iter_Contains(ItemBuild, buildid);
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

stock Item:CreateItem(ItemBuild:buildid, modelid = -1, const uuid[] = "") {
    if (!IsValidItemBuild(buildid)) {
        return INVALID_ITEM_ID;
    }

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
    gItemData[itemid][E_ITEM_MODEL_ID] = modelid >= 0 ? modelid : GetItemBuildModel(buildid);

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
    RemoveCurrentItem(GetItemHolder(itemid));

    Iter_Remove(Item, itemid);

    CallLocalFunction("OnItemDestroy", "i", _:itemid);

    return true;
}

stock ItemBuild:GetItemBuild(Item:itemid, &ItemBuild:buildid = INVALID_ITEM_BUILD_ID) {
    if (!IsValidItem(itemid)) {
        buildid = INVALID_ITEM_BUILD_ID;

        return buildid;
    }

    buildid = gItemData[itemid][E_ITEM_BUILD_ID];

    return buildid;
}

stock bool:SetItemModel(Item:itemid, modelid) {
    if (!IsValidItem(itemid)) {
        return false;
    }

    gItemData[itemid][E_ITEM_MODEL_ID] = modelid;

    return true;
}

stock GetItemModel(Item:itemid) {
    if (!IsValidItem(itemid)) {
        return -1;
    }

    return gItemData[itemid][E_ITEM_MODEL_ID];
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

stock bool:CreateItemInWorld(Item:itemid, Float:x, Float:y, Float:z, Float:rotationX, Float:rotationY, Float:rotationZ, KEY:buttonKey = ITEM_KEY_PICK_UP_ITEM, worldid = -1, interiorid = -1, Float:distance = ITEM_DEFAULT_DISTANCE, priority = ITEM_DEFAULT_BUTTON_PRIORITY) {
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
        Button:buttonid = CreateButton(x, y, z + gItemBuildData[buildid][E_ITEM_BUILD_BTN_OFFSET_Z], buttonKey, 1.0, worldid, interiorid, priority)
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
    gItemData[itemid][E_ITEM_OBJECT_ID] = CreateDynamicObject(gItemBuildData[buildid][E_ITEM_BUILD_MODEL_ID], x, y, z + gItemBuildData[buildid][E_ITEM_BUILD_OBJ_OFFSET_Z], rotationX, rotationY, rotationZ, worldid, interiorid, .streamdistance = distance, .drawdistance = distance);

    SetButtonExtraData(buttonid, BUTTON_ATTRIBUTE_ITEM_ID, _:itemid);

    Iter_Add(ItemWorld, itemid);

    CallLocalFunction("OnItemCreateInWorld", "i", _:itemid);

    return true;
}

stock bool:IsItemInWorld(Item:itemid) {
    if (!IsValidItem(itemid)) {
        return false;
    }

    return Iter_Contains(ItemWorld, itemid);
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

stock bool:SetItemPos(Item:itemid, Float:x, Float:y, Float:z) {
    if (!IsItemInWorld(itemid)) {
        return false;
    }

    gItemData[itemid][E_ITEM_X] = x;
    gItemData[itemid][E_ITEM_Y] = y;
    gItemData[itemid][E_ITEM_Z] = z;

    SetButtonPos(gItemData[itemid][E_ITEM_BUTTON_ID], x, y, z + gItemBuildData[buildid][E_ITEM_BUILD_BTN_OFFSET_Z]);
    SetDynamicObjectPos(gItemData[itemid][E_ITEM_OBJECT_ID], x, y, z + gItemBuildData[buildid][E_ITEM_BUILD_OBJ_OFFSET_Z]);

    return true;
}

stock bool:GetItemPos(Item:itemid, &Float:x, &Float:y, &Float:z) {
    if (!IsItemInWorld(itemid)) {
        return false;
    }

    x = gItemData[itemid][E_ITEM_X];
    y = gItemData[itemid][E_ITEM_Y];
    z = gItemData[itemid][E_ITEM_Z];

    return true;
}

stock bool:SetItemRot(Item:itemid, Float:rotationX, Float:rotationY, Float:rotationZ, bool:useRotOffsets = true) {
    if (!IsItemInWorld(itemid)) {
        return false;
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

    return true;
}

stock bool:GetItemRot(Item:itemid, &Float:rotationX, &Float:rotationY, &Float:rotationZ) {
    if (!IsItemInWorld(itemid)) {
        return false;
    }

    rotationX = gItemData[itemid][E_ITEM_ROT_X];
    rotationY = gItemData[itemid][E_ITEM_ROT_Y];
    rotationZ = gItemData[itemid][E_ITEM_ROT_Z];

    return true;
}

stock bool:SetItemVirtualWorld(Item:itemid, worldid) {
    if (!IsItemInWorld(itemid)) {
        return false;
    }

    SetButtonVirtualWorld(gItemData[itemid][E_ITEM_BUTTON_ID], worldid);
    gItemData[itemid][E_ITEM_WORLD_ID] = worldid;

    return true;
}

stock GetItemVirtualWorld(Item:itemid) {
    if (!IsItemInWorld(itemid)) {
        return -1;
    }

    return gItemData[itemid][E_ITEM_WORLD_ID];
}

stock bool:SetItemInterior(Item:itemid, interiorid) {
    if (!IsItemInWorld(itemid)) {
        return false;
    }

    SetButtonInterior(gItemData[itemid][E_ITEM_BUTTON_ID], interiorid);
    gItemData[itemid][E_ITEM_INTERIOR_ID] = interiorid;

    return true;
}

stock GetItemInterior(Item:itemid) {
    if (!IsItemInWorld(itemid)) {
        return -1;
    }

    return gItemData[itemid][E_ITEM_WORLD_ID];
}

stock bool:SetItemLabelText(Item:itemid, const format[], colour = 0xFFFFFFFF, Float:distance = ITEM_DEFAULT_DISTANCE, bool:testLOS = true, Float:fOffsetZ = 0.5, OPEN_MP_TAGS:...) {
    if (!IsItemInWorld(itemid)) {
        return false;
    }

    SetButtonTextLabel(gItemData[itemid][E_ITEM_BUTTON_ID], format, colour, distance, testLOS, fOffsetZ, ___(6));

    return true;
}

stock bool:GetItemButton(Item:itemid, &Button:buttonid) {
    if (!IsItemInWorld(itemid)) {
        return false;
    }

    buttonid = gItemData[itemid][E_ITEM_BUTTON_ID];

    return true;
}

stock bool:HasButtonItem(Button:buttonid) {
    return HasButtonExtraData(buttonid, BUTTON_ATTRIBUTE_ITEM_ID);
}

stock bool:GetButtonItem(Button:buttonid, &Item:itemid) {
    return GetButtonExtraData(buttonid, BUTTON_ATTRIBUTE_ITEM_ID, _:itemid);
}

stock bool:GetPlayerNearbyItems(playerid, Item:items[MAX_AREA_BUTTONS] = { INVALID_ITEM_ID, ... }, &count = 0) {
    if (!IsPlayerConnected(playerid)) {
        return false;
    }

    if (!IsPlayerInAnyButton(playerid)) {
        return false;
    }

    new
        Button:buttons[MAX_AREA_BUTTONS],
        buttonCount,
        Item:buttonItem
    ;

    GetPlayerNearbyButtons(playerid, buttons, buttonCount);

    count = 0;

    for (new i; i != buttonCount; ++i) {
        if (GetButtonItem(buttons[i], buttonItem)) {
            items[count++] = buttonItem;
        }
    }

    return (count != 0);
}

stock bool:SetPlayerItem(playerid, Item:itemid) {
    if (!IsPlayerConnected(playerid)) {
        return false;
    }

    if (!IsValidItem(itemid)) {
        return false;
    }

    PlayerGetItemInternal(playerid, itemid);

    return true;
}

stock bool:GiveWorldItemToPlayer(playerid, Item:itemid) {
    if (!IsPlayerConnected(playerid)) {
        return false;
    }

    if (!IsItemInWorld(itemid)) {
        return false;
    }

    PlayerGetItemInternal(playerid, itemid);

    return true;
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

    gInteractItemID[playerid] = INVALID_ITEM_ID;
    gItemInteractorID[itemid] = INVALID_PLAYER_ID;

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

    RemoveCurrentItem(playerid);
    ApplyAnimation(playerid, "BOMBER", "BOM_PLANT_2IDLE", 4.0, false, false, false, false, 0);

    Streamer_Update(playerid);

    CallLocalFunction("OnPlayerDroppedItem", "ii", playerid, _:itemid);
}

stock bool:PlayerPickUpItem(playerid, Item:itemid) {
    if (!IsValidItem(itemid)) {
        return false;
    }

    if (IsValidItem(gHoldItemID[playerid])) {
        return false;
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

    return true;
}

stock bool:PlayerDropItem(playerid) {
    if (!IsPlayerConnected(playerid)) {
        return false;
    }

    if (!IsValidItem(gHoldItemID[playerid])) {
        return false;
    }

    if (CallLocalFunction("OnPlayerDropItem", "ii", playerid, _:gHoldItemID[playerid])) {
        return true;
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

    return true;
}

stock bool:RemoveCurrentItem(playerid) {
    if (!IsPlayerConnected(playerid)) {
        return false;
    }

    if (!IsValidItem(gHoldItemID[playerid])) {
        return false;
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

    return true;
}

stock bool:PlayerUseItem(playerid) {
    if (!IsPlayerConnected(playerid)) {
        return false;
    }

    if (!IsIdleAnimationInternal(GetPlayerAnimationIndex(playerid))) {
        return false;
    }

    if (gInteractItemID[playerid] != INVALID_ITEM_ID) {
        return false;
    }

    if (gHoldItemID[playerid] == INVALID_ITEM_ID) {
        return false;
    }

    CallLocalFunction("OnPlayerUseItem", "ii", playerid, _:gHoldItemID[playerid]);

    return true;
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

stock Item:GetPlayerItem(playerid, &Item:itemid = INVALID_ITEM_ID) {
    if (!IsPlayerConnected(playerid)) {
        itemid = INVALID_ITEM_ID;
        
        return itemid;
    }

    itemid = gHoldItemID[playerid];

    return itemid;
}

stock Item:GetPlayerInteractItem(playerid, &Item:itemid = INVALID_ITEM_ID) {
    if (!IsPlayerConnected(playerid)) {
        itemid = INVALID_ITEM_ID;

        return itemid;
    }

    itemid = gInteractItemID[playerid];

    return itemid;
}

/**
 * # Internal
 */

static stock void:PlayerGetItemInternal(playerid, Item:itemid) {
    RemoveItemFromWorld(itemid);
    RemoveCurrentItem(GetItemHolder(itemid));

    if (CallLocalFunction("OnPlayerGetItem", "ii", playerid, _:itemid)) {
        return;
    }

    new const
        ItemBuild:buildid = gItemData[itemid][E_ITEM_BUILD_ID]
    ;

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

    CallLocalFunction("OnPlayerGotItem", "ii", playerid, _:itemid);
}

static stock IsIdleAnimationInternal(index) {
    switch (index) {
        case 0, 320, 471, 1164, 1183, 1188, 1189: {
            return 1;
        }
    }

    return 0;
}

/**
 * # Calls
 */

hook OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys) {
    if (!IsPlayerInAnyVehicle(playerid) && GetPlayerState(playerid) != PLAYER_STATE_SPECTATING) {
        if (newkeys & ITEM_KEY_DROP_ITEM) {
            PlayerDropItem(playerid);
        }

        if (newkeys & ITEM_KEY_USE_ITEM) {
            PlayerUseItem(playerid);
        }
    }

    return 1;
}

hook OnButtonPress(playerid, Button:buttonid) {
    if (HasButtonItem(buttonid)) {
        new
            Item:itemid
        ;

        GetButtonItem(buttonid, itemid);

        if (gItemHolderID[itemid] == INVALID_PLAYER_ID && gItemInteractorID[itemid] == INVALID_PLAYER_ID) {
            if (IsValidItem(gHoldItemID[playerid])) {
                CallLocalFunction("OnPlayerPickUpItemWithItem", "iii", playerid, _:itemid, _:gHoldItemID[playerid]);

                return ~1;
            }

            if (CallLocalFunction("OnPlayerPickUpItem", "ii", playerid, _:itemid)) {
                return ~1;
            }

            PlayerPickUpItem(playerid, itemid);
        }
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

hook OnPlayerConnect(playerid) {
    gHoldItemID[playerid] = INVALID_ITEM_ID;
    gInteractItemID[playerid] = INVALID_ITEM_ID;
    stop gInteractTimer[playerid];
}

hook OnPlayerDisconnect(playerid, reason) {
    stop gInteractTimer[playerid];
}