#define MAX_PLAYERS (5)

#include <open.mp>
#include <pagination_dialog_async> // https://github.com/Hreesang/samp-pagination-dialog/tree/main

// Y
#include <YSI_Visual\y_dialog>
#include <YSI_Visual\y_commands>

// I
#include <I\I>

/**
 * # Header
 */

#define MAX_ITEM_BUILD_OPTIONS (7)
#define MAX_ITEMS_PER_PAGE (10)

enum (<<= 1) {
    ITEM_BUILD_FLAGS_NON,
    ITEM_BUILD_FLAGS_USE = (1 << 0),
    ITEM_BUILD_FLAGS_PUT,
    ITEM_BUILD_FLAGS_TAK,
    ITEM_BUILD_FLAGS_DRO,
    ITEM_BUILD_FLAGS_SPL,
    ITEM_BUILD_FLAGS_REN,
    ITEM_BUILD_FLAGS_DES
};

enum {
    ITEM_BUILD_OPTION_NON = -1,
    ITEM_BUILD_OPTION_USE,
    ITEM_BUILD_OPTION_PUT,
    ITEM_BUILD_OPTION_TAK,
    ITEM_BUILD_OPTION_DRO,
    ITEM_BUILD_OPTION_SPL,
    ITEM_BUILD_OPTION_REN,
    ITEM_BUILD_OPTION_DES,
    ITEM_BUILD_OPTION_INF,
    ITEM_BUILD_OPTION_CLO
};

static const
    gContainerOptionName[][] =
{
    "Use",
    "Put",
    "Take",
    "Drop",
    "Split",
    "Rent",
    "Destroy"
};

static
    ItemBuild:gBuildDeagle,
    ItemBuild:gBuildAK,
    ItemBuild:gBuildM4,
    ItemBuild:gBuildSniper,

    ItemBuild:gBuildPizza,
    ItemBuild:gBuildHamburger,
    

    ItemBuild:gBuildChip,
    ItemBuild:gBuildKey,
    ItemBuild:gBuildBlackSunglasses,

    ItemBuild:gItemBuildBigBox,
    ItemBuild:gItemBuildSmallBox
;

static
    Container:gContainerID = INVALID_CONTAINER_ID
;

/**
 * # Events
 */

public OnPlayerSelectContainerOption(playerid, Container:containerid, Item:itemid, option);

main(){}

stock bool:ShowContainerDialog(playerid, Container:containerid) {
    if (!IsPlayerConnected(playerid)) {
        return false;
    }

    if (!IsValidContainer(containerid)) {
        return false;
    }

    if (IsContainerEmpty(containerid)) {
        return false;
    }

    const
        MAX_DIALOG_ITEMS = 10,
        MAX_CONTAINER_ITEMS = 100
    ;

    new
        Item:items[MAX_CONTAINER_ITEMS],
        itemCount,
        Item:id,
        itemName[MAX_ITEM_NAME]
    ;

    new const
        List:list = list_new()
    ;

    foreach ((_:id) : ContainerItem(containerid)) {
        GetItemName(id, itemName);
        AddPaginationDialogItem(list, itemName);
        items[itemCount++] = id;
    }

    new
        response[E_ASYNC_PAGE_DIALOG],
        name[MAX_CONTAINER_NAME],
        capt[MAX_CONTAINER_NAME + 16]
    ;

    GetContainerName(containerid, name);
    format(capt, sizeof (capt), "%s (%i/%i)", name, GetContainerItemCount(containerid), GetContainerSize(containerid));

    task_await_arr(
        ShowPaginationDialogAsync(playerid, PAGE_DIALOG_STYLE_TABLIST, capt, list, "Select", "Close", .maxItems = MAX_DIALOG_ITEMS),
        response
    );

    new const
        listitem = response[E_ASYNC_PAGE_DIALOG_LISTITEM],
        Item:itemid = items[listitem]
    ;

    if (!IsValidItem(itemid)) {
        return true;
    }

    if (response[E_ASYNC_PAGE_DIALOG_RESPONSE]) {
        ShowContainerItemOptDialog(playerid, containerid, itemid);
    }

    return true;
}

void:ShowContainerItemOptDialog(playerid, Container:containerid, Item:itemid) {
    new
        body[128],
        listItems[MAX_ITEM_BUILD_OPTIONS + 2],
        itemCount
    ;
    
    new const
        f = GetItemBuildFlags(GetItemBuild(itemid))
    ;

    for (new i; i != MAX_ITEM_BUILD_OPTIONS; ++i) {
        if ((f & (1 << i)) == 0) {
            continue;
        }

        strcat(body, gContainerOptionName[i]);
        strcat(body, "\n");

        listItems[itemCount++] = i;
    }

    strcat(body, "Info\nBack");
    
    listItems[itemCount++] = ITEM_BUILD_OPTION_INF;
    listItems[itemCount++] = ITEM_BUILD_OPTION_CLO;

    inline const OnResponse(response, listitem, string:inputtext[]) {
        #pragma unused inputtext

        if (!response) {
            ShowContainerDialog(playerid, containerid);

            @return 1;
        }

        if (!CallLocalFunction("OnPlayerSelectContainerOption", "iiii", playerid, _:containerid, _:itemid, listItems[listitem])) {
            @return 1;
        }

        ShowContainerDialog(playerid, containerid);
    }

    Dialog_ShowCallback(playerid, using inline OnResponse, DIALOG_STYLE_TABLIST, "Options", body, "Select", "Back");
}

public OnPlayerSelectContainerOption(playerid, Container:containerid, Item:itemid, option) {
    switch (option) {
        case ITEM_BUILD_OPTION_USE: SendClientMessage(playerid, -1, "Use");
        case ITEM_BUILD_OPTION_PUT: SendClientMessage(playerid, -1, "Put");
        case ITEM_BUILD_OPTION_TAK: SendClientMessage(playerid, -1, "Take");
        case ITEM_BUILD_OPTION_DRO: SendClientMessage(playerid, -1, "Drop");
        case ITEM_BUILD_OPTION_SPL: SendClientMessage(playerid, -1, "Split");
        case ITEM_BUILD_OPTION_REN: SendClientMessage(playerid, -1, "Rent");
        case ITEM_BUILD_OPTION_DES: SendClientMessage(playerid, -1, "Destroy");
        case ITEM_BUILD_OPTION_INF: SendClientMessage(playerid, -1, "Info");
        case ITEM_BUILD_OPTION_CLO: SendClientMessage(playerid, -1, "Back");
    }

    // Test

    if (option == ITEM_BUILD_OPTION_TAK) {
        GivePlayerItem(playerid, itemid);

        return 0; // Don't show the container again
    }

    return 1; // Show the container again
}

public OnGameModeInit() {
    // Class
    AddPlayerClass(60, 0.0, 0.0, 3.1172, 0.0);

    // Builds
    gBuildDeagle          = BuildItem("Deagle"          , 348);
    gBuildAK              = BuildItem("AK"              , 355);
    gBuildM4              = BuildItem("M4"              , 356);
    gBuildSniper          = BuildItem("Sniper"          , 358);

    gBuildPizza           = BuildItem("Pizza"           , 2302);
    gBuildHamburger       = BuildItem("Hamburger"       , 2303);

    gBuildChip            = BuildItem("Chip"            , 1581);
    gBuildKey             = BuildItem("Key"             , 11746);
    gBuildBlackSunglasses = BuildItem("Black Sunglasses", 19138);

    gItemBuildBigBox      = BuildItem("Big Box"         , 1271 , 0.0, 0.0, 0.0, 0.3112, 6, 0.074, 0.313, -0.307, -110.200, -7.400, -007.600, .useCarryAnim = true);
    gItemBuildSmallBox    = BuildItem("Small Box"       , 2969 , 0.0, 0.0, 0.0, 0.0000, 6, 0.015, 0.084, -0.230, -110.400, -4.600, -101.200, .useCarryAnim = true);
    
    // Flags
    SetItemBuildFlags(gBuildDeagle         , ITEM_BUILD_FLAGS_USE | ITEM_BUILD_FLAGS_DRO                       );
    SetItemBuildFlags(gBuildAK             , ITEM_BUILD_FLAGS_USE | ITEM_BUILD_FLAGS_DRO                       );
    SetItemBuildFlags(gBuildM4             , ITEM_BUILD_FLAGS_USE | ITEM_BUILD_FLAGS_DRO                       );
    SetItemBuildFlags(gBuildSniper         , ITEM_BUILD_FLAGS_USE | ITEM_BUILD_FLAGS_DRO                       );
    SetItemBuildFlags(gBuildChip           , ITEM_BUILD_FLAGS_USE | ITEM_BUILD_FLAGS_DRO | ITEM_BUILD_FLAGS_SPL);
    SetItemBuildFlags(gBuildKey            , ITEM_BUILD_FLAGS_USE | ITEM_BUILD_FLAGS_DRO | ITEM_BUILD_FLAGS_SPL);
    SetItemBuildFlags(gBuildBlackSunglasses, ITEM_BUILD_FLAGS_PUT | ITEM_BUILD_FLAGS_DRO | ITEM_BUILD_FLAGS_SPL);
    SetItemBuildFlags(gBuildPizza          , ITEM_BUILD_FLAGS_USE | ITEM_BUILD_FLAGS_DRO | ITEM_BUILD_FLAGS_SPL);
    SetItemBuildFlags(gBuildHamburger      , ITEM_BUILD_FLAGS_USE | ITEM_BUILD_FLAGS_DRO | ITEM_BUILD_FLAGS_SPL);
    SetItemBuildFlags(gItemBuildBigBox     , ITEM_BUILD_FLAGS_TAK | ITEM_BUILD_FLAGS_DRO                       );
    SetItemBuildFlags(gItemBuildSmallBox   , ITEM_BUILD_FLAGS_TAK | ITEM_BUILD_FLAGS_DRO                       );

    return 1;
}

/**
 * # Commands
 */

YCMD:add(playerid, params[], help) {
    if (IsValidContainer(gContainerID)) {
        return SendClientMessage(playerid, -1, "Container alaready exists.");
    }

    gContainerID = CreateContainer("Container", 100);

    new const
        count = Iter_Count(ItemBuild)
    ;

    for (new i; i != 35; i++) {
        new const
            Item:itemid = CreateItem(ItemBuild:random(count))
        ;

        AddItemToContainer(gContainerID, itemid);
    }
    
    return 1;
}

YCMD:show(playerid, params[], help) {
    ShowContainerDialog(playerid, gContainerID);
    
    return 1;
}