#include <open.mp>

// I
#include <I\item>
#include <I\extensions\item-build-options>

// Builds
static
    ItemBuild:gItemBuildPizza
;

main(){}

public OnGameModeInit() {
    gItemBuildPizza = BuildItem("Pizza", 2702);

    // Options
    new const
        ItemBuildOpt:U = CreateItemBuildOption("Use"),
        ItemBuildOpt:P = CreateItemBuildOption("Put"),
        ItemBuildOpt:E = CreateItemBuildOption("Equip"),
        ItemBuildOpt:D = CreateItemBuildOption("Drop"),
        ItemBuildOpt:S = CreateItemBuildOption("Split"),
        ItemBuildOpt:I = CreateItemBuildOption("Info"),
        ItemBuildOpt:C = CreateItemBuildOption("Close")
    ;

    #pragma unused P, E

    // Set
    SetItemBuildOptions(gItemBuildPizza, OPTION_FLAG(U) | OPTION_FLAG(D) | OPTION_FLAG(S) | OPTION_FLAG(I) | OPTION_FLAG(C));

    // Show
    new
        ItemBuildOpt:options[MAX_ITEM_BUILD_OPTIONS],
        optionCount,
        optionName[MAX_ITEM_BUILD_OPTION_NAME]
    ;

    GetItemBuildOptions(gItemBuildPizza, options, .count = optionCount);
    
    for (new i; i != optionCount; i++) {
        GetItemBuildOptionName(options[i], optionName);
        printf("%i. %s", i+1, optionName);
    }

    // Has
    printf("Has options? %c", HasItemBuildOptions(gItemBuildPizza, OPTION_FLAG(U) | OPTION_FLAG(I) | OPTION_FLAG(C)) ? 'Y' : 'N');
    printf("Has options? %c", HasItemBuildOptions(gItemBuildPizza, OPTION_FLAG(P) | OPTION_FLAG(I) | OPTION_FLAG(C)) ? 'Y' : 'N');

    return 1;
}