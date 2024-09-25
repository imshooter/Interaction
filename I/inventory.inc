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

forward bool:AddItemToInventory(playerid, Item:itemid, &index = -1, bool:call = true);
forward bool:RemoveItemFromInventory(playerid, index, &Item:itemid = INVALID_ITEM_ID, bool:call = true);
forward bool:SetInventorySize(playerid, size);
forward GetInventorySize(playerid);
forward bool:SetInventoryOrdered(playerid, bool:ordered);
forward bool:IsInventoryOrdered(playerid);
forward GetInventoryItemCount(playerid);
forward bool:IsInventoryEmpty(playerid);
forward bool:IsInventoryFull(playerid);
forward bool:IsInventorySlotUsed(playerid, index);
forward bool:GetInventorySlotItem(playerid, index, &Item:itemid);
forward bool:GetInventoryContainer(playerid, &Container:containerid);

/**
 * # Events
 */

forward OnItemAddToInventory(playerid, Item:itemid, index);

/**
 * # OnItemAddToInventory
 *
 * # Returns
 *   - This callback does not handle returns.
 */

forward OnItemRemoveFromInventory(playerid, Item:itemid, index);

/**
 * # OnItemRemoveFromInventory
 *
 * # Returns
 *   - This callback does not handle returns.
 */

/**
 * # Iter
 */

stock bool:Iter_Func@PlayerInventory(playerid) {
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

#define Iterator@PlayerInventory iteryield

/**
 * # External
 */

stock bool:AddItemToInventory(playerid, Item:itemid, &index = -1, bool:call = true) {
    if (!IsPlayerConnected(playerid)) {
        return false;
    }

    if (!AddItemToContainer(gOwnerContainerID[playerid], itemid, index, playerid, call)) {
        return false;
    }

    if (call) {
        CallLocalFunction("OnItemAddToInventory", "iii", playerid, _:itemid, index);
    }

    return true;
}

stock bool:RemoveItemFromInventory(playerid, index, &Item:itemid = INVALID_ITEM_ID, bool:call = true) {
    if (!IsPlayerConnected(playerid)) {
        return false;
    }

    if (!RemoveItemFromContainer(gOwnerContainerID[playerid], index, itemid, playerid, call)) {
        return false;
    }

    if (call) {
        CallLocalFunction("OnItemRemoveFromInventory", "iii", playerid, _:itemid, index);
    }

    return true;
}

stock bool:SetInventorySize(playerid, size) {
    if (!IsPlayerConnected(playerid)) {
        return false;
    }

    return SetContainerSize(gOwnerContainerID[playerid], size);
}

stock GetInventorySize(playerid) {
    if (!IsPlayerConnected(playerid)) {
        return 0;
    }

    return GetContainerSize(gOwnerContainerID[playerid]);
}

stock bool:SetInventoryOrdered(playerid, bool:ordered) {
    if (!IsPlayerConnected(playerid)) {
        return false;
    }

    return SetContainerOrdered(gOwnerContainerID[playerid], ordered);
}

stock bool:IsInventoryOrdered(playerid) {
    if (!IsPlayerConnected(playerid)) {
        return false;
    }

    return IsContainerOrdered(gOwnerContainerID[playerid]);
}

stock GetInventoryItemCount(playerid) {
    if (!IsPlayerConnected(playerid)) {
        return 0;
    }

    return GetContainerItemCount(gOwnerContainerID[playerid]);
}

stock bool:IsInventoryEmpty(playerid) {
    if (!IsPlayerConnected(playerid)) {
        return false;
    }

    return IsContainerEmpty(gOwnerContainerID[playerid]);
}

stock bool:IsInventoryFull(playerid) {
    if (!IsPlayerConnected(playerid)) {
        return false;
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
        printf("[INVENTORY]: (PlayerInvTearDownInternal) -> Failed to destroy inventory container (%i) for player (%i).", _:gOwnerContainerID[playerid], playerid);
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