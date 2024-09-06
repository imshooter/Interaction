#if defined _INC_PP_ITEM
    #endinput
#endif
#define _INC_PP_ITEM

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
    Map:gItemBuildData,
    Map:gItemExtraData[MAX_ITEMS]
;

/**
 * # Functions
 */

forward bool:SetKeyItemBuild(const key[], ItemBuild:buildid);
forward bool:GetKeyItemBuild(const key[], &ItemBuild:buildid);
forward bool:HasKeyItemBuild(const key[]);
forward bool:IsItemBuildMatchKey(ItemBuild:buildid, const key[]);

forward bool:HasItemAnyExtraData(Item:itemid);
forward bool:HasItemExtraData(Item:itemid, const key[]);
forward bool:SetItemExtraData(Item:itemid, const key[], value);
forward bool:GetItemExtraData(Item:itemid, const key[], &value);
forward bool:GetItemExtraDataMap(Item:itemid, &Map:map);

/**
 * # Item Build
 */

stock bool:SetKeyItemBuild(const key[], ItemBuild:buildid) {
    if (!(0 <= strlen(key) < MAX_ITEM_KEY_LENGTH)) {
        return false;
    }

    if (!IsValidItemBuild(buildid)) {
        return false;
    }

    if (!map_valid(gItemBuildData)) {
        gItemBuildData = map_new();
    }

    map_str_set(gItemBuildData, key, _:buildid);

    return true;
}

stock bool:GetKeyItemBuild(const key[], &ItemBuild:buildid) {
    if (!HasKeyItemBuild(key)) {
        return false;
    }

    return map_str_get_safe(gItemBuildData, key, _:buildid);
}

stock bool:HasKeyItemBuild(const key[]) {
    if (!map_valid(gItemBuildData)) {
        return false;
    }

    return map_has_str_key(gItemBuildData, key);
}

stock bool:IsItemBuildMatchKey(ItemBuild:buildid, const key[]) {
    if (!IsValidItemBuild(buildid)) {
        return false;
    }

    new
        ItemBuild:buildid2
    ;

    if (!GetKeyItemBuild(key, buildid2)) {
        return false;
    }

    return (buildid == buildid2);
}

/**
 * # Item
 */

stock bool:HasItemAnyExtraData(Item:itemid) {
    if (!IsValidItem(itemid)) {
        return false;
    }

    if (!map_valid(gItemExtraData[itemid])) {
        return false;
    }

    return (map_size(gItemExtraData[itemid]) != 0);
}

stock bool:HasItemExtraData(Item:itemid, const key[]) {
    if (!HasItemAnyExtraData(itemid)) {
        return false;
    }

    return map_has_str_key(gItemExtraData[itemid], key);
}

stock bool:SetItemExtraData(Item:itemid, const key[], value) {
    if (!(1 <= strlen(key) <= MAX_ITEM_KEY_LENGTH)) {
        return false;
    }

    if (!IsValidItem(itemid)) {
        return false;
    }

    if (!map_valid(gItemExtraData[itemid])) {
        gItemExtraData[itemid] = map_new();
    }

    if (map_size(gItemExtraData[itemid]) >= MAX_ITEM_ATTRIBUTES) {
        return false;
    }

    map_str_set(gItemExtraData[itemid], key, value);

    return true;
}

stock bool:GetItemExtraData(Item:itemid, const key[], &value) {
    if (!HasItemExtraData(itemid, key)) {
        return false;
    }

    return map_str_get_safe(gItemExtraData[itemid], key, value);
}

stock bool:GetItemExtraDataMap(Item:itemid, &Map:map) {
    if (!HasItemAnyExtraData(itemid)) {
        return false;
    }

    map = gItemExtraData[itemid];

    return true;
}

/**
 * # Calls
 */

hook OnItemDestroy(Item:itemid) {
    if (map_valid(gItemExtraData[itemid])) {
        map_delete_deep(gItemExtraData[itemid]);
    }

    return 1;
}