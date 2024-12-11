#if defined _INC_ITEM_BUILD_OPTIONS
    #endinput
#endif
#define _INC_ITEM_BUILD_OPTIONS

#include <I\item>

/**
 * # Header
 */

#if !defined MAX_ITEM_BUILD_OPTIONS
    #define MAX_ITEM_BUILD_OPTIONS (8)
#endif

#if !defined MAX_ITEM_BUILD_OPTION_NAME
    #define MAX_ITEM_BUILD_OPTION_NAME (8)
#endif

#define INVALID_ITEM_BUILD_OPTION_ID \
    (ItemBuildOpt:-1)

#define ITEM_BUILD_OPTION_FLAG(%0) \
    (ItemBuildOpt:(1 << _:(%0)))

static
    gItemBuildOptionName[MAX_ITEM_BUILD_OPTIONS][MAX_ITEM_BUILD_OPTION_NAME],
    ItemBuildOpt:gItemBuildOptions[MAX_ITEM_BUILDS],
    ItemBuildOpt:gItemBuildOptionCount
;

/**
 * # Functions
 */

forward ItemBuildOpt:CreateItemBuildOption(const name[]);
forward bool:HasItemBuildOptions(ItemBuild:buildid, ItemBuildOpt:options);
forward bool:SetItemBuildOptions(ItemBuild:buildid, ItemBuildOpt:options);
forward bool:GetItemBuildOptions(ItemBuild:buildid, ItemBuildOpt:options[], size = sizeof (options), &count = 0);
forward bool:SetItemBuildOptionName(ItemBuildOpt:optionid, const name[]);
forward bool:GetItemBuildOptionName(ItemBuildOpt:optionid, name[], size = sizeof (name));

/**
 * # External
 */

stock ItemBuildOpt:CreateItemBuildOption(const name[]) {
    if (_:gItemBuildOptionCount >= MAX_ITEM_BUILD_OPTIONS) {
        return INVALID_ITEM_BUILD_OPTION_ID;
    }

    new const
        ItemBuildOpt:optionid = gItemBuildOptionCount++
    ;

    strcopy(gItemBuildOptionName[_:optionid], name);

    return optionid;
}

stock bool:SetItemBuildOptionName(ItemBuildOpt:optionid, const name[]) {
    if (!(0 <= _:optionid < _:gItemBuildOptionCount)) {
        return false;
    }

    strcopy(gItemBuildOptionName[_:optionid], name);

    return true;
}

stock bool:GetItemBuildOptionName(ItemBuildOpt:optionid, name[], size = sizeof (name)) {
    if (!(0 <= _:optionid < _:gItemBuildOptionCount)) {
        return false;
    }

    strcopy(name, gItemBuildOptionName[_:optionid], size);

    return true;
}

stock bool:HasItemBuildOptions(ItemBuild:buildid, ItemBuildOpt:options) {
    if (!IsValidItemBuild(buildid)) {
        return false;
    }

    return ((gItemBuildOptions[buildid] & options) == options);
}

stock bool:SetItemBuildOptions(ItemBuild:buildid, ItemBuildOpt:options) {
    if (!IsValidItemBuild(buildid)) {
        return false;
    }

    gItemBuildOptions[buildid] ^= options;

    return true;
}

stock bool:GetItemBuildOptions(ItemBuild:buildid, ItemBuildOpt:options[], size = sizeof (options), &count = 0) {
    if (!IsValidItemBuild(buildid)) {
        return false;
    }

    new const
        ItemBuildOpt:itemBuildOptions = gItemBuildOptions[buildid]
    ;

    count = 0;

    for (new ItemBuildOpt:i; i != gItemBuildOptionCount && _:i < size; ++i) {
        if (_:itemBuildOptions & (1 << _:i)) {
            options[count++] = i;
        }
    }

    return (count > 0);
}