# Functions

### Button

> * **CreateButton**`(Float:x, Float:y, Float:z, KEY:buttonKey = BUTTON_DEFAULT_KEY, Float:size = 1.0, worldid = -1, interiorid = -1, priority = 0, bool:allowStack = false)`
> * **DestroyButton**`(Button:buttonid)`
> * **IsValidButton**`(Button:buttonid)`
> * **SetButtonPos**`(Button:buttonid, Float:x, Float:y, Float:z, bool:allowStack = false)`
> * **GetButtonPos**`(Button:buttonid, &Float:x, &Float:y, &Float:z)`
> * **SetButtonSize**`(Button:buttonid, Float:size)`
> * **GetButtonSize**`(Button:buttonid, &Float:size)`
> * **SetButtonVirtualWorld**`(Button:buttonid, worldid)`
> * **GetButtonVirtualWorld**`(Button:buttonid)`
> * **SetButtonInterior**`(Button:buttonid, interiorid)`
> * **GetButtonInterior**`(Button:buttonid)`
> * **SetButtonPopupText**`(Button:buttonid, const format[], OPEN_MP_TAGS:...)`
> * **GetButtonPopupText**`(Button:buttonid, output[], len = sizeof (output))`
> * **SetButtonTextLabel**`(Button:buttonid, const format[], colour = 0xFFFFFFFF, Float:distance = BUTTON_DEFAULT_DISTANCE, bool:testLOS = true, Float:fOffsetZ = 0.0, OPEN_MP_TAGS:...)`
> * **DestroyButtonTextLabel**`(Button:buttonid)`
> * **UpdateButtonTextLabel**`(Button:buttonid, const format[], colour = 0xFFFFFFFF, OPEN_MP_TAGS:...)`
> * **GetButtonDynamicArea**`(Button:buttonid, &STREAMER_TAG_AREA:areaid)`
> * **GetButtonDynamic3DTextLabel**`(Button:buttonid, &STREAMER_TAG_3D_TEXT_LABEL:textid)`
> * **GetClosestButtonToPoint**`(Float:x, Float:y, Float:z, Float:size = 1.0, worldid = -1, &Button:buttonid = INVALID_BUTTON_ID)`
> * **IsPlayerInAnyButton**`(playerid)`
> * **GetPlayerButton**`(playerid, &Button:buttonid)`
> * **GetPlayerNearbyButtons**`(playerid, Button:buttons[MAX_PLAYER_BUTTONS] = { INVALID_BUTTON_ID, ... }, &count = 0)`

### Item

> * **BuildItem**`(const name[], modelid, Float:buttonOffsetZ = ITEM_FLOOR_OFFSET)`
> * **IsValidItemBuild**`(ItemBuild:buildid)`
> * **GetItemBuildName**`(ItemBuild:buildid, output[], len = sizeof (output))`
> * **GetItemBuildModel**`(ItemBuild:buildid)`

> * **CreateItem**`(ItemBuild:buildid, const uuid[] = "")`
> * **IsValidItem**`(Item:itemid)`
> * **GetItemBuild**`(Item:itemid, &ItemBuild:buildid)`
> * **SetItemExtraName**`(Item:itemid, const name[])`
> * **GetItemExtraName**`(Item:itemid, output[], len = sizeof (output))`
> * **GetItemUUID**`(Item:itemid, output[], len = sizeof (output))`

> * **CreateItemInWorld**`(Item:itemid, Float:x, Float:y, Float:z, Float:rotationX, Float:rotationY, Float:rotationZ, KEY:buttonKey = ITEM_DEFAULT_KEY, worldid = -1, interiorid = -1, Float:distance = ITEM_DEFAULT_DISTANCE)`
> * **RemoveItemFromWorld**`(Item:itemid)`
> * **IsItemInWorld**`(Item:itemid)`
> * **SetItemTextLabel**`(Item:itemid, const format[], colour = 0xFFFFFFFF, Float:distance = ITEM_DEFAULT_DISTANCE, bool:testLOS = true, Float:fOffsetZ = 0.5, OPEN_MP_TAGS:...)`
> * **GetItemButton**`(Item:itemid, &Button:buttonid)`
> * **SetItemButtonPos**`(Item:itemid, Float:x, Float:y, Float:z, bool:allowStack = false)`
> * **GetItemButtonPos**`(Item:itemid, &Float:x, &Float:y, &Float:z)`
> * **GetButtonItem**`(Button:buttonid, &Item:itemid)`
> * **GetPlayerNearbyItems**`(playerid, Item:items[MAX_PLAYER_BUTTONS] = { INVALID_ITEM_ID, ... }, &count = 0)`

### Entrance

> * **CreateEntrance**`(Float:extX, Float:extY, Float:extZ, Float:extA, extWorld, extInterior, Float:intX, Float:intY, Float:intZ, Float:intA, intWorld, intInterior)`
> * **IsValidEntrance**`(Entrance:entranceid)`

> * **GetEntranceExteriorPos**`(Entrance:entranceid, &Float:x, &Float:y, &Float:z)`
> * **GetEntranceExteriorFacingAngle**`(Entrance:entranceid, &Float:a)`
> * **GetEntranceExteriorVirtualWorld**`(Entrance:entranceid)`
> * **GetEntranceExteriorInterior**`(Entrance:entranceid)`
> * **GetExteriorButton**`(Entrance:entranceid, &Button:buttonid)`
> * **GetPlayerExteriorButton**`(playerid, &Button:buttonid)`
> * **SetPlayerExteriorEntrance**`(playerid, Entrance:entranceid, Float:offset = 1.0)`
> * **GetPlayerExteriorEntrance**`(playerid, &Entrance:entranceid)`

> * **GetEntranceInteriorPos**`(Entrance:entranceid, &Float:x, &Float:y, &Float:z)`
> * **GetEntranceInteriorFacingAngle**`(Entrance:entranceid, &Float:a)`
> * **GetEntranceInteriorVirtualWorld**`(Entrance:entranceid)`
> * **GetEntranceInteriorInterior**`(Entrance:entranceid)`
> * **GetInteriorButton**`(Entrance:entranceid, &Button:buttonid)`
> * **GetPlayerInteriorButton**`(playerid, &Button:buttonid)`
> * **SetPlayerInteriorEntrance**`(playerid, Entrance:entranceid, Float:offset = 1.0)`
> * **GetPlayerInteriorEntrance**`(playerid, &Entrance:entranceid)`

# Button Callbacks

### OnButtonCreate
> * #### Parameters:
>    * `TODO`
> * #### Returns:
>    * `~0` or `~1`: Breaks subsequent calls to `OnButtonCreate`.

### OnButtonDestroy
> * #### Parameters:
>    * `TODO`
> * #### Returns:
>    * `~0` or `~1`: Breaks subsequent calls to `OnButtonDestroy`.

#### OnButtonPress
> * #### Parameters:
>    * `TODO`
> * #### Returns:
>    * ` 0`: Breaks subsequent calls to `OnPlayerKeyStateChange`.
>    * ` 1`: Continues with subsequent calls to `OnPlayerKeyStateChange`.
>    * `~0`: Breaks subsequent calls to `OnButtonPress` and `OnPlayerKeyStateChange`.
>    * `~1`: Breaks subsequent calls to `OnButtonPress` and continues subsequent calls to `OnPlayerKeyStateChange`.

#### OnButtonRelease
> * #### Parameters:
>    * `TODO`
> * #### Returns:
>    * ` 0`: Breaks subsequent calls to `OnPlayerKeyStateChange`.
>    * ` 1`: Continues with subsequent calls to `OnPlayerKeyStateChange`.
>    * `~0`: Breaks subsequent calls to `OnButtonRelease` and `OnPlayerKeyStateChange`.
>    * `~1`: Breaks subsequent calls to `OnButtonRelease` and continues subsequent calls to `OnPlayerKeyStateChange`.

#### OnPlayerEnterButtonArea
> * #### Parameters:
>    * `TODO`
> * #### Returns:
>    * ` 0`: Breaks subsequent calls to `OnPlayerEnterDynamicArea`.
>    * ` 1`: Continues with subsequent calls to `OnPlayerEnterDynamicArea`.
>    * `~0`: Breaks subsequent calls to `OnPlayerEnterButtonArea` and `OnPlayerEnterDynamicArea`.
>    * `~1`: Breaks subsequent calls to `OnPlayerEnterButtonArea` and continues subsequent calls to `OnPlayerEnterDynamicArea`.

#### OnPlayerLeaveButtonArea
> * #### Parameters:
>    * `TODO`
> * #### Returns:
>    * ` 0`: Breaks subsequent calls to `OnPlayerLeaveDynamicArea`.
>    * ` 1`: Continues with subsequent calls to `OnPlayerLeaveDynamicArea`.
>    * `~0`: Breaks subsequent calls to `OnPlayerLeaveButtonArea` and `OnPlayerLeaveDynamicArea`.
>    * `~1`: Breaks subsequent calls to `OnPlayerLeaveButtonArea` and continues subsequent calls to `OnPlayerLeaveDynamicArea`.

# Item Callbacks

#### OnItemCreate
> * #### Parameters:
>    * `TODO`
> * #### Returns:
>    * `~0` or `~1`: Breaks subsequent calls to `OnItemCreate`.

#### OnItemDestroy
> * #### Parameters:
>    * `TODO`
> * #### Returns:
>    * `~0` or `~1`: Breaks subsequent calls to `OnItemDestroy`.

#### OnItemCreateInWorld
> * #### Parameters:
>    * `TODO`
> * #### Returns:
>    * `~0` or `~1`: Breaks subsequent calls to `OnItemCreateInWorld`.

#### OnItemRemoveFromWorld
> * #### Parameters:
>    * `TODO`
> * #### Returns:
>    * `~0` or `~1`: Breaks subsequent calls to `OnItemRemoveFromWorld`.

#### OnPlayerPickUpItem
> * #### Parameters:
>    * `TODO`
> * #### Returns:
>    * ` 0` or ` 1`: Breaks subsequent calls to `OnButtonPress` (Consequently `OnPlayerKeyStateChange` too).
>    * `~0` or `~1`: Breaks subsequent calls to `OnPlayerPickUpItem` and `OnButtonPress` (Consequently `OnPlayerKeyStateChange` too).
> * #### NOTE:
>    * Return `0` to `CANCEL` the player from picking up the item in the world.
>    * Return `1` for the player pick up the item in the world.
