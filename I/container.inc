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

#if !defined MAX_CONTAINER_SIZE
    #define MAX_CONTAINER_SIZE (100)
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

forward Container:CreateContainer(const name[], size, bool:ordered = false);
forward bool:DestroyContainer(Container:containerid, bool:clear = true);
forward bool:IsValidContainer(Container:containerid);
forward bool:SetContainerName(Container:containerid, const name[]);
forward bool:GetContainerName(Container:containerid, output[], size = sizeof (output));
forward bool:SetContainerSize(Container:containerid, size);
forward GetContainerSize(Container:containerid);
forward bool:SetContainerOrdered(Container:containerid, bool:ordered);
forward bool:IsContainerOrdered(Container:containerid);
forward bool:AddItemToContainer(Container:containerid, Item:itemid, &index = -1, addToIndex = -1, playerid = INVALID_PLAYER_ID, bool:call = true);
forward bool:RemoveItemFromContainer(Container:containerid, index, &Item:itemid = INVALID_ITEM_ID, playerid = INVALID_PLAYER_ID, bool:call = true);
forward GetContainerItemCount(Container:containerid);
forward bool:IsContainerEmpty(Container:containerid);
forward bool:IsContainerFull(Container:containerid);
forward bool:IsContainerSlotUsed(Container:containerid, index);
forward bool:GetContainerSlotItem(Container:containerid, index, &Item:itemid);
forward bool:GetItemContainerData(Item:itemid, &Container:containerid = INVALID_CONTAINER_ID, &index = -1);

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

    for (new const Iter:iterator = pool_iter(gContainerData[containerid][E_CONTAINER_POOL]); iter_inside(iterator); iter_move_next(iterator)) {
        yield return iter_get_value(iterator);
    }

    return true;
}

#define Iterator@ContainerItem iteryield

/**
 * # External
 */

stock Container:CreateContainer(const name[], size, bool:ordered = false) {
    if (!(1 <= size <= MAX_CONTAINER_SIZE)) {
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

stock bool:DestroyContainer(Container:containerid, bool:clear = true) {
    if (!IsValidContainer(containerid)) {
        return false;
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

    return true;
}

stock bool:IsValidContainer(Container:containerid) {
    if (!UCMP(_:containerid, CONTAINER_ITER_SIZE)) {
        return false;
    }

    return Iter_Contains(Container, containerid);
}

stock bool:SetContainerName(Container:containerid, const name[]) {
    if (!IsValidContainer(containerid)) {
        return false;
    }

    strcopy(gContainerData[containerid][E_CONTAINER_NAME], name);

    return true;
}

stock bool:GetContainerName(Container:containerid, output[], size = sizeof (output)) {
    if (!IsValidContainer(containerid)) {
        return false;
    }

    strcopy(output, gContainerData[containerid][E_CONTAINER_NAME], size);

    return true;
}

stock bool:SetContainerSize(Container:containerid, size) {
    if (!IsValidContainer(containerid)) {
        return false;
    }

    pool_resize(gContainerData[containerid][E_CONTAINER_POOL], size);

    return true;
}

stock GetContainerSize(Container:containerid) {
    if (!IsValidContainer(containerid)) {
        return 0;
    }

    return pool_capacity(gContainerData[containerid][E_CONTAINER_POOL]);
}

stock bool:SetContainerOrdered(Container:containerid, bool:ordered) {
    if (!IsValidContainer(containerid)) {
        return false;
    }

    pool_set_ordered(gContainerData[containerid][E_CONTAINER_POOL], ordered);

    return true;
}

stock bool:IsContainerOrdered(Container:containerid) {
    if (!IsValidContainer(containerid)) {
        return false;
    }

    return pool_is_ordered(gContainerData[containerid][E_CONTAINER_POOL]);
}

stock bool:AddItemToContainer(Container:containerid, Item:itemid, &index = -1, addToIndex = -1, playerid = INVALID_PLAYER_ID, bool:call = true) {
    if (!IsValidContainer(containerid)) {
        return false;
    }

    if (!IsValidItem(itemid)) {
        return false;
    }

    if (gItemContainerID[itemid] != INVALID_CONTAINER_ID) {
        return false;
    }

    new const
        Pool:p = gContainerData[containerid][E_CONTAINER_POOL]
    ;

    if (addToIndex >= 0 && !pool_has(p, addToIndex)) {
        pool_set(p, addToIndex, _:itemid);
        index = addToIndex;
    } else {
        index = pool_add(p, _:itemid);
    }

    gItemContainerID[itemid] = containerid;
    gItemContainerSlotID[itemid] = index;

    if (call) {
        CallLocalFunction("OnItemAddToContainer", "iiii", _:containerid, _:itemid, index, playerid);
    }

    return true;
}

stock bool:RemoveItemFromContainer(Container:containerid, index, &Item:itemid = INVALID_ITEM_ID, playerid = INVALID_PLAYER_ID, bool:call = true) {
    if (!IsValidContainer(containerid)) {
        return false;
    }

    if (!GetContainerSlotItem(containerid, index, itemid)) {
        return false;
    }

    gItemContainerID[itemid] = INVALID_CONTAINER_ID;
    gItemContainerSlotID[itemid] = -1;

    pool_remove(gContainerData[containerid][E_CONTAINER_POOL], index);

    if (call) {
        CallLocalFunction("OnItemRemoveFromContainer", "iiii", _:containerid, _:itemid, index, playerid);
    }

    return true;
}

stock GetContainerItemCount(Container:containerid) {
    if (!IsValidContainer(containerid)) {
        return 0;
    }

    return pool_size(gContainerData[containerid][E_CONTAINER_POOL]);
}

stock bool:IsContainerEmpty(Container:containerid) {
    if (!IsValidContainer(containerid)) {
        return false;
    }

    return (pool_size(gContainerData[containerid][E_CONTAINER_POOL]) == 0);
}

stock bool:IsContainerFull(Container:containerid) {
    if (!IsValidContainer(containerid)) {
        return false;
    }

    return (pool_size(gContainerData[containerid][E_CONTAINER_POOL]) >= pool_capacity(gContainerData[containerid][E_CONTAINER_POOL]));
}

stock bool:IsContainerSlotUsed(Container:containerid, index) {
    if (!IsValidContainer(containerid)) {
        return false;
    }

    return pool_has(gContainerData[containerid][E_CONTAINER_POOL], index);
}

stock bool:GetContainerSlotItem(Container:containerid, index, &Item:itemid) {
    if (!IsValidContainer(containerid)) {
        return false;
    }

    if (!pool_has(gContainerData[containerid][E_CONTAINER_POOL], index)) {
        return false;
    }

    return pool_get_safe(gContainerData[containerid][E_CONTAINER_POOL], index, _:itemid);
}

stock bool:GetItemContainerData(Item:itemid, &Container:containerid = INVALID_CONTAINER_ID, &index = -1) {
    if (!IsValidItem(itemid)) {
        return false;
    }

    containerid = gItemContainerID[itemid];
    index = gItemContainerSlotID[itemid];

    return true;
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