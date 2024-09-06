#if defined _INC_I
    #endinput
#endif
#define _INC_I

#include <I\button>
#include <I\item>
#include <I\container>
#include <I\inventory>

#include <I\item-plus>
#include <I\button-plus>

#include <I\entrance>

/**
 * # Extra
 */

stock ReturnItemBuildName(ItemBuild:buildid) {
    new
        output[MAX_ITEM_BUILD_NAME]
    ;

    GetItemBuildName(buildid, output);

    return output;
}

stock ReturnItemExtraName(Item:itemid) {
    new
        output[MAX_ITEM_EXTRA_NAME]
    ;

    GetItemExtraName(itemid, output);

    return output;
}

stock ReturnItemUUID(Item:itemid) {
    new
        output[UUID_LEN]
    ;

    GetItemUUID(itemid, output);

    return output;
}

stock ReturnItemName(Item:itemid) {
    new
        output[MAX_ITEM_NAME]
    ;

    GetItemName(itemid, output);

    return output;
}

stock ReturnContainerName(Container:containerid) {
    new
        output[MAX_CONTAINER_NAME]
    ;

    GetContainerName(containerid, output);

    return output;
}