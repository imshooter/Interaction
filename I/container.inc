#if defined _INC_CONTAINER
    #endinput
#endif
#define _INC_CONTAINER

#include <I\item>
#include <PawnPlus>

#include <YSI_Data\y_iterate>
#include <YSI_Coding\y_hooks>

/**
 * # Header
 */

#if !defined MAX_CONTAINERS
    #define MAX_CONTAINERS (Container:4096)
#endif

#if !defined MAX_CONTAINER_NAME
    #define MAX_CONTAINER_NAME (16)
#endif

#if !defined MAX_CONTAINER_SLOTS
    #define MAX_CONTAINER_SLOTS (128)
#endif

#define INVALID_CONTAINER_ID (Container:-1)

static enum E_CONTAINER_DATA {
    E_CONTAINER_NAME[MAX_CONTAINER_NAME + 1],
    Pool:E_CONTAINER_POOL
};

static
    gContainerData[MAX_CONTAINERS][E_CONTAINER_DATA]
;

static
    Container:gItemContainerID[MAX_ITEMS] = { INVALID_CONTAINER_ID, ... },
    gItemContainerSlotID[MAX_ITEMS] = { -1, ... }
;

const static
    CONTAINER_ITER_SIZE = _:MAX_CONTAINERS
;

new
    Iterator:Container<Container:CONTAINER_ITER_SIZE>
;

/**
 * # Functions
 */

stock Container:CreateContainer(const name[], size, bool:ordered = true);
stock bool:IsValidContainer(Container:containerid);
stock DestroyContainer(Container:containerid, bool:clear = true);
stock SetContainerName(Container:containerid, const name[]);
stock GetContainerName(Container:containerid, output[], len = sizeof (output));
stock SetContainerSize(Container:containerid, size);
stock GetContainerSize(Container:containerid);
stock SetContainerOrdered(Container:containerid, bool:ordered);
stock bool:IsContainerOrdered(Container:containerid);
stock GetContainerItemCount(Container:containerid);
stock AddItemToContainer(Container:containerid, Item:itemid, &index = -1, playerid = INVALID_PLAYER_ID, bool:call = true);
stock RemoveItemFromContainer(Container:containerid, index, &Item:itemid = INVALID_ITEM_ID, playerid = INVALID_PLAYER_ID, bool:call = true);
stock TransferContainerItem(Container:containerid1, Container:containerid2, index, &Item:itemid = INVALID_ITEM_ID);
stock bool:IsContainerEmpty(Container:containerid);
stock bool:IsContainerFull(Container:containerid);
stock bool:IsContainerSlotUsed(Container:containerid, index);
stock GetContainerSlotItem(Container:containerid, index, &Item:itemid);
stock GetItemContainerData(Item:itemid, &Container:containerid = INVALID_CONTAINER_ID, &index = -1);

/**
 * # Events
 */

forward OnContainerCreate(Container:containerid);
forward OnContainerDestroy(Container:containerid);
forward OnItemAddToContainer(Container:containerid, Item:itemid, index, playerid);
forward OnItemRemoveFromContainer(Container:containerid, Item:itemid, index, playerid);

/**
 * # Iter
 */

stock bool:Iter_Func@ContainerItem(Container:containerid) {
    if (!IsValidContainer(containerid)) {
        return false;
    }

    for (new Iter:it = pool_iter(gContainerData[containerid][E_CONTAINER_POOL]); iter_inside(it); iter_move_next(it)) {
        yield return iter_get_value(it);
    }

    return true;
}

#define Iterator@ContainerItem iteryield

/**
 * # External
 */

stock Container:CreateContainer(const name[], size, bool:ordered = true) {
    if (!(1 <= size <= MAX_CONTAINER_SLOTS)) {
        return INVALID_CONTAINER_ID;
    }

    new const
        Container:containerid = Container:Iter_Alloc(Container)
    ;

    if (_:containerid == INVALID_ITERATOR_SLOT) {
        return INVALID_CONTAINER_ID;
    }

    strcopy(gContainerData[containerid][E_CONTAINER_NAME], name);
    gContainerData[containerid][E_CONTAINER_POOL] = pool_new(size, ordered);

    CallLocalFunction("OnContainerCreate", "i", _:containerid);

    return containerid;
}

stock bool:IsValidContainer(Container:containerid) {
    if (!(0 <= _:containerid < CONTAINER_ITER_SIZE)) {
        return false;
    }

    return Iter_Contains(Container, containerid);
}

stock DestroyContainer(Container:containerid, bool:clear = true) {
    if (!IsValidContainer(containerid)) {
        return 1;
    }

    if (clear) {
        new
            Item:itemid
        ;

        foreach ((_:itemid) : ContainerItem(containerid)) {
            gItemContainerID[itemid] = INVALID_CONTAINER_ID;
            gItemContainerSlotID[itemid] = -1;

            DestroyItem(itemid);
        }
    }

    pool_delete(gContainerData[containerid][E_CONTAINER_POOL]);

    Iter_Remove(Container, containerid);

    CallLocalFunction("OnContainerDestroy", "i", _:containerid);

    return 0;
}

stock SetContainerName(Container:containerid, const name[]) {
    if (!IsValidContainer(containerid)) {
        return 1;
    }

    strcopy(gContainerData[containerid][E_CONTAINER_NAME], name);

    return 0;
}

stock GetContainerName(Container:containerid, output[], len = sizeof (output)) {
    if (!IsValidContainer(containerid)) {
        return 1;
    }

    strcopy(output, gContainerData[containerid][E_CONTAINER_NAME], len);

    return 0;
}

stock SetContainerSize(Container:containerid, size) {
    if (!IsValidContainer(containerid)) {
        return 1;
    }

    pool_resize(gContainerData[containerid][E_CONTAINER_POOL], size);

    return 0;
}

stock GetContainerSize(Container:containerid) {
    if (!IsValidContainer(containerid)) {
        return 0;
    }

    return pool_capacity(gContainerData[containerid][E_CONTAINER_POOL]);
}

stock SetContainerOrdered(Container:containerid, bool:ordered) {
    if (!IsValidContainer(containerid)) {
        return 1;
    }

    pool_set_ordered(gContainerData[containerid][E_CONTAINER_POOL], ordered);

    return 0;
}

stock bool:IsContainerOrdered(Container:containerid) {
    if (!IsValidContainer(containerid)) {
        return undefined;
    }

    return pool_is_ordered(gContainerData[containerid][E_CONTAINER_POOL]);
}

stock GetContainerItemCount(Container:containerid) {
    if (!IsValidContainer(containerid)) {
        return 0;
    }

    return pool_size(gContainerData[containerid][E_CONTAINER_POOL]);
}

stock AddItemToContainer(Container:containerid, Item:itemid, &index = -1, playerid = INVALID_PLAYER_ID, bool:call = true) {
    if (!IsValidContainer(containerid)) {
        return 1;
    }

    if (!IsValidItem(itemid)) {
        return 2;
    }

    if (gItemContainerID[itemid] != INVALID_CONTAINER_ID) {
        return 3;
    }

    if (pool_size(gContainerData[containerid][E_CONTAINER_POOL]) >= pool_capacity(gContainerData[containerid][E_CONTAINER_POOL])) {
        return 4;
    }

    gItemContainerID[itemid] = containerid;
    gItemContainerSlotID[itemid] = index = pool_add(gContainerData[containerid][E_CONTAINER_POOL], _:itemid);

    if (call) {
        CallLocalFunction("OnItemAddToContainer", "iiii", _:containerid, _:itemid, index, playerid);
    }

    return 0;
}

stock RemoveItemFromContainer(Container:containerid, index, &Item:itemid = INVALID_ITEM_ID, playerid = INVALID_PLAYER_ID, bool:call = true) {
    if (!IsValidContainer(containerid)) {
        return 1;
    }

    if (!GetContainerSlotItem(containerid, index, itemid)) {
        return 2;
    }

    gItemContainerID[itemid] = INVALID_CONTAINER_ID;
    gItemContainerSlotID[itemid] = -1;

    pool_remove(gContainerData[containerid][E_CONTAINER_POOL], index);

    if (call) {
        CallLocalFunction("OnItemRemoveFromContainer", "iiii", _:containerid, _:itemid, index, playerid);
    }

    return 0;
}

stock TransferContainerItem(Container:containerid1, Container:containerid2, index, &Item:itemid = INVALID_ITEM_ID) {
    if (!IsValidContainer(containerid1)) {
        return 1;
    }

    if (!IsValidContainer(containerid2)) {
        return 2;
    }

    if (IsContainerSlotUsed(containerid1, index)) {
        return 3;
    }

    if (IsContainerSlotUsed(containerid2, index)) {
        return 4;
    }

    RemoveItemFromContainer(containerid1, index, itemid);
    AddItemToContainer(containerid2, itemid);

    return 0;
}

stock bool:IsContainerEmpty(Container:containerid) {
    if (!IsValidContainer(containerid)) {
        return undefined;
    }

    return (pool_size(gContainerData[containerid][E_CONTAINER_POOL]) == 0);
}

stock bool:IsContainerFull(Container:containerid) {
    if (!IsValidContainer(containerid)) {
        return undefined;
    }

    return (pool_size(gContainerData[containerid][E_CONTAINER_POOL]) >= pool_capacity(gContainerData[containerid][E_CONTAINER_POOL]));
}

stock bool:IsContainerSlotUsed(Container:containerid, index) {
    if (!IsValidContainer(containerid)) {
        return undefined;
    }

    return pool_has(gContainerData[containerid][E_CONTAINER_POOL], index);
}

stock GetContainerSlotItem(Container:containerid, index, &Item:itemid) {
    if (!IsValidContainer(containerid)) {
        return 1;
    }

    if (!pool_has(gContainerData[containerid][E_CONTAINER_POOL], index)) {
        return 2;
    }

    pool_get_safe(gContainerData[containerid][E_CONTAINER_POOL], index, _:itemid);

    return 0;
}

stock GetItemContainerData(Item:itemid, &Container:containerid = INVALID_CONTAINER_ID, &index = -1) {
    if (!IsValidItem(itemid)) {
        return 1;
    }

    containerid = gItemContainerID[itemid];
    index = gItemContainerSlotID[itemid];

    return 0;
}

/**
 * # Calls
 */

hook OnItemDestroy(Item:itemid) {
    if (gItemContainerID[itemid] != INVALID_CONTAINER_ID) {
        RemoveItemFromContainer(gItemContainerID[itemid], gItemContainerSlotID[itemid]);
    }
    
    return 1;
}