#if defined _INC_INVENTORY
    #endinput
#endif
#define _INC_INVENTORY

#include <I\item>
#include <I\container>

#include <YSI_Data\y_iterate>
#include <YSI_Coding\y_hooks>
#include <YSI_Coding\y_timers>

/**
 * # Header
 */

#if !defined MAX_INVENTORY_SLOTS
    #define MAX_INVENTORY_SLOTS (128)
#endif

static
    Container:gOwnerContainerID[MAX_PLAYERS] = { INVALID_CONTAINER_ID, ... },
    gContainerOwnerID[MAX_CONTAINERS] = { INVALID_PLAYER_ID, ... }
;

/**
 * # Functions
 */

forward AddItemToInventory(playerid, Item:itemid, &index = -1, bool:call = true);
forward RemoveItemFromInventory(playerid, index, &Item:itemid = INVALID_ITEM_ID, bool:call = true);
forward bool:IsInventoryEmpty(playerid);
forward bool:IsInventoryFull(playerid);
forward bool:IsInventorySlotUsed(playerid, index);
forward bool:GetInventorySlotItem(playerid, index, &Item:itemid);
forward bool:GetInventoryContainer(playerid, &Container:containerid);

/**
 * # Events
 */

forward OnItemAddToInventory(playerid, Item:itemid, index);
forward OnItemRemoveFromInventory(playerid, Item:itemid, index);

/**
 * # Iter
 */

stock bool:Iter_Func@InventoryItem(playerid) {
    if (!IsPlayerConnected(playerid)) {
        return false;
    }

    new
        Item:itemid
    ;

    foreach ((_:itemid) : ContainerItem(gOwnerContainerID[playerid])) {
        yield return (_:itemid);
    }

    return true;
}

#define Iterator@InventoryItem iteryield

/**
 * # External
 */

stock AddItemToInventory(playerid, Item:itemid, &index = -1, bool:call = true) {
    if (!IsPlayerConnected(playerid)) {
        return 1;
    }

    new const
        ret = AddItemToContainer(gOwnerContainerID[playerid], itemid, index, playerid, call)
    ;

    if (ret) {
        return (ret + 1);
    }

    if (call) {
        CallLocalFunction("OnItemAddToInventory", "iii", playerid, _:itemid, index);
    }

    return 0;
}

stock RemoveItemFromInventory(playerid, index, &Item:itemid = INVALID_ITEM_ID, bool:call = true) {
    if (!IsPlayerConnected(playerid)) {
        return 1;
    }

    new const
        ret = RemoveItemFromContainer(gOwnerContainerID[playerid], index, itemid, playerid, call)
    ;

    if (ret) {
        return (ret + 1);
    }

    if (call) {
        CallLocalFunction("OnItemRemoveFromInventory", "iii", playerid, _:itemid, index);
    }

    return 0;
}

stock bool:IsInventoryEmpty(playerid) {
    if (!IsPlayerConnected(playerid)) {
        return undefined;
    }

    return IsContainerEmpty(gOwnerContainerID[playerid]);
}

stock bool:IsInventoryFull(playerid) {
    if (!IsPlayerConnected(playerid)) {
        return undefined;
    }

    return IsContainerFull(gOwnerContainerID[playerid]);
}

stock bool:IsInventorySlotUsed(playerid, index) {
    if (!IsPlayerConnected(playerid)) {
        return false;
    }

    return IsContainerSlotUsed(gOwnerContainerID[playerid], index);
}

stock bool:GetInventorySlotItem(playerid, index, &Item:itemid) {
    if (!IsPlayerConnected(playerid)) {
        return false;
    }

    return GetContainerSlotItem(gOwnerContainerID[playerid], index, itemid);
}

stock bool:GetInventoryContainer(playerid, &Container:containerid) {
    if (!IsPlayerConnected(playerid)) {
        return false;
    }

    containerid = gOwnerContainerID[playerid];

    return true;
}

/**
 * # Calls
 */

static void:PlayerInvSetupInternal(playerid) {
    new const
        Container:containerid = CreateContainer("Inventory", MAX_CONTAINER_SLOTS)
    ;

    if (!IsValidContainer(containerid)) {
        printf("[INVENTORY]: (PlayerInvSetupInternal) -> Failed to create inventory container (%i) for player (%i).", _:containerid, playerid);

        return;
    }

    gOwnerContainerID[playerid] = containerid;
    gContainerOwnerID[containerid] = playerid;
}

static timer PlayerInvTearDownInternal[1](playerid) {
    new const
        Container:containerid = gOwnerContainerID[playerid]
    ;

    if (!DestroyContainer(gOwnerContainerID[playerid])) {
        printf("[INVENTORY]: (PlayerInvSetupInternal) -> Failed to destroy inventory container (%i) for player (%i).", _:gOwnerContainerID[playerid], playerid);
    }

    gOwnerContainerID[playerid] = INVALID_CONTAINER_ID;
    gContainerOwnerID[containerid] = INVALID_PLAYER_ID;
}

hook OnPlayerConnect(playerid) {
    PlayerInvSetupInternal(playerid);
    
    return 1;
}

hook OnPlayerDisconnect(playerid, reason) {
    defer PlayerInvTearDownInternal(playerid);
    
    return 1;
}