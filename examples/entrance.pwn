#include <open.mp>

// I
#include <I\entrance>

/**
 * # Header
 */

static
    Entrance:gBincoEntranceID,
    Entrance:gCluckinBellEntranceID,
    Entrance:gZIPEntranceID,
    Entrance:gWellStackedPizzaEntranceID
;

main(){}

public OnGameModeInit() {
    AddPlayerClass(60, 2104.8264, 2226.3044, 11.0234, 180.0000);

    // Native entrances
    DisableInteriorEnterExits();

    // Entrances

    gBincoEntranceID = CreateEntrance(
        2101.9431, 2257.4814, 11.0234  , 270.0000, 0, 0 , // Exterior
        207.6035 , -111.1129, 1005.1328, 0.0000  , 0, 15  // Interior
    );

    gCluckinBellEntranceID = CreateEntrance(
        2101.9773, 2228.9023, 11.0234  , 270.0000, 0, 0 , // Exterior
        364.8506 , -11.5878 , 1001.8516, 0.0000  , 0, 9   // Interior
    );

    gZIPEntranceID = CreateEntrance(
        2090.5540, 2224.5737, 11.0234  , 180.0000, 0, 0 , // Exterior
        161.3103 , -96.8705 , 1001.8047, 0.0000  , 0, 18  // Interior
    );

    gWellStackedPizzaEntranceID = CreateEntrance(
        2083.4136, 2224.6208, 11.0234  , 180.0000, 0, 0 , // Exterior
        372.2722 , -133.4687, 1001.4922, 0.0000  , 0, 5   // Interior
    );

    // Icons

    CreateDynamicPickup(1318, 1, 2101.9431, 2257.4814, 11.0234  , 0, 0 );
    CreateDynamicPickup(1318, 1, 207.6035 , -111.1129, 1005.1328, 0, 15);

    CreateDynamicPickup(1318, 1, 2101.9773, 2228.9023, 11.0234  , 0, 0 );
    CreateDynamicPickup(1318, 1, 364.8506 , -11.5878 , 1001.8516, 0, 9 );

    CreateDynamicPickup(1318, 1, 2090.5540, 2224.5737, 11.0234  , 0, 0 );
    CreateDynamicPickup(1318, 1, 161.3103 , -96.8705 , 1001.8047, 0, 18);

    CreateDynamicPickup(1318, 1, 2083.4136, 2224.6208, 11.0234  , 0, 0 );
    CreateDynamicPickup(1318, 1, 372.2722 , -133.4687, 1001.4922, 0, 5 );

    return 1;
}

public OnPlayerEnterExteriorArea(playerid, Entrance:entranceid) {
    if (entranceid == gBincoEntranceID) {
        GameTextForPlayer(playerid, "Press ~k~~VEHICLE_ENTER_EXIT~ to enter the Binco.", 3000, 15);

        return 0; // (Hook) ~0
    }

    if (entranceid == gCluckinBellEntranceID) {
        GameTextForPlayer(playerid, "Press ~k~~VEHICLE_ENTER_EXIT~ to enter the Cluckin Bell.", 3000, 15);

        return 0; // (Hook) ~0
    }

    if (entranceid == gZIPEntranceID) {
        GameTextForPlayer(playerid, "Press ~k~~VEHICLE_ENTER_EXIT~ to enter the ZIP.", 3000, 15);

        return 0; // (Hook) ~0
    }

    if (entranceid == gWellStackedPizzaEntranceID) {
        GameTextForPlayer(playerid, "Press ~k~~VEHICLE_ENTER_EXIT~ to enter the The Well Stacked Pizza Co.", 3000, 15);

        return 0; // (Hook) ~0
    }

    return 1;
}

public OnPlayerEnterInteriorArea(playerid, Entrance:entranceid) {
    if (entranceid == gBincoEntranceID) {
        GameTextForPlayer(playerid, "Press ~k~~VEHICLE_ENTER_EXIT~ to leave the Binco.", 3000, 15);

        return 0; // (Hook) ~0
    }

    if (entranceid == gCluckinBellEntranceID) {
        GameTextForPlayer(playerid, "Press ~k~~VEHICLE_ENTER_EXIT~ to leave the Cluckin Bell.", 3000, 15);

        return 0; // (Hook) ~0
    }

    if (entranceid == gZIPEntranceID) {
        GameTextForPlayer(playerid, "Press ~k~~VEHICLE_ENTER_EXIT~ to leave the ZIP.", 3000, 15);

        return 0; // (Hook) ~0
    }

    if (entranceid == gWellStackedPizzaEntranceID) {
        GameTextForPlayer(playerid, "Press ~k~~VEHICLE_ENTER_EXIT~ to leave the The Well Stacked Pizza Co.", 3000, 15);

        return 0; // (Hook) ~0
    }

    return 1;
}