#if defined _INC_ITEM_PLUS
    #endinput
#endif
#define _INC_ITEM_PLUS

#include <I\item>
#include <PawnPlus>

#include <YSI_Coding\y_hooks>

/**
 * # Header
 */

#if !defined MAX_ITEM_KEY_LENGTH
    #define MAX_ITEM_KEY_LENGTH (32)
#endif

#if !defined MAX_ITEM_ATTRIBUTES
    #define MAX_ITEM_ATTRIBUTES (16)
#endif

static
    Map:gItemBuildMap,
    Map:gItemMap[MAX_ITEMS]
;

/**
 * # Functions
 */

forward bool:SetKeyItemBuild(const key[], ItemBuild:buildid);
forward bool:GetKeyItemBuild(const key[], &ItemBuild:buildid);
forward bool:HasKeyAnyItemBuild(const key[]);
forward bool:HasKeyItemBuild(const key[], ItemBuild:buildid);

forward bool:HasItemAnyExtraData(Item:itemid);
forward bool:HasItemExtraData(Item:itemid, const key[]);
forward bool:SetItemExtraData(Item:itemid, const key[], value);
forward bool:GetItemExtraData(Item:itemid, const key[], &value);
forward bool:GetItemExtraDataMap(Item:itemid, &Map:map);

/**
 * # Macros
 */

#define BindKeyToItemBuild SetKeyItemBuild
#define FetchItemBuildByKey GetKeyItemBuild
#define DoesKeyHaveItemBuild HasKeyAnyItemBuild
#define DoesKeyMatchItemBuild HasKeyItemBuild

forward bool:BindKeyToItemBuild(const key[], ItemBuild:buildid);
forward bool:FetchItemBuildByKey(const key[], &ItemBuild:buildid);
forward bool:DoesKeyHaveItemBuild(const key[]);
forward bool:DoesKeyMatchItemBuild(const key[], ItemBuild:buildid);

/**
 * # Item Build
 */

stock bool:SetKeyItemBuild(const key[], ItemBuild:buildid) {
    if (NOT_IN_RANGE(strlen(key), 1, MAX_ITEM_KEY_LENGTH)) {
        return false;
    }

    if (!IsValidItemBuild(buildid)) {
        return false;
    }

    if (!map_valid(gItemBuildMap)) {
        gItemBuildMap = map_new();
    }

    map_str_set(gItemBuildMap, key, _:buildid);

    return true;
}

stock bool:GetKeyItemBuild(const key[], &ItemBuild:buildid) {
    if (!HasKeyAnyItemBuild(key)) {
        return false;
    }

    return map_str_get_safe(gItemBuildMap, key, _:buildid);
}

stock bool:HasKeyAnyItemBuild(const key[]) {
    if (!map_valid(gItemBuildMap)) {
        return false;
    }

    return map_has_str_key(gItemBuildMap, key);
}

stock bool:HasKeyItemBuild(const key[], ItemBuild:buildid) {
    if (!IsValidItemBuild(buildid)) {
        return false;
    }

    new
        ItemBuild:b
    ;

    return (GetKeyItemBuild(key, b) && b == buildid);
}

/**
 * # Item
 */

stock bool:HasItemAnyExtraData(Item:itemid) {
    if (!IsValidItem(itemid)) {
        return false;
    }

    if (!map_valid(gItemMap[itemid])) {
        return false;
    }

    return (map_size(gItemMap[itemid]) != 0);
}

stock bool:HasItemExtraData(Item:itemid, const key[]) {
    if (!HasItemAnyExtraData(itemid)) {
        return false;
    }

    return map_has_str_key(gItemMap[itemid], key);
}

stock bool:SetItemExtraData(Item:itemid, const key[], value) {
    if (NOT_IN_RANGE(strlen(key), 1, MAX_ITEM_KEY_LENGTH)) {
        return false;
    }

    if (!IsValidItem(itemid)) {
        return false;
    }

    if (!map_valid(gItemMap[itemid])) {
        gItemMap[itemid] = map_new();
    }

    if (map_size(gItemMap[itemid]) >= MAX_ITEM_ATTRIBUTES) {
        return false;
    }

    map_str_set(gItemMap[itemid], key, value);

    return true;
}

stock bool:GetItemExtraData(Item:itemid, const key[], &value) {
    if (!HasItemExtraData(itemid, key)) {
        return false;
    }

    return map_str_get_safe(gItemMap[itemid], key, value);
}

stock bool:GetItemExtraDataMap(Item:itemid, &Map:map) {
    if (!HasItemAnyExtraData(itemid)) {
        return false;
    }

    map = gItemMap[itemid];

    return true;
}

/**
 * # Calls
 */

hook OnItemDestroy(Item:itemid) {
    if (map_valid(gItemMap[itemid])) {
        map_delete_deep(gItemMap[itemid]);
    }

    return 1;
}