# Interaction Archived
This project implements a set of frameworks to create a more immersive experience in SA-MP/open.mp servers by replacing spinning pickups with physical buttons and providing an interactive interface for items, inventories, and containers.

# **Button**
A simple framework using dynamic areas and key checks to simulate physical buttons. This is ideal for interactions without commands, such as entrances, or other interactions in the world.

*    Buttons can be placed in the world and require the player to press a specific key.
*    Supports dynamic text to show information when near the button.

# **Item**
A flexible script for handling items in the game world. Items can be picked up or dropped, and can be attached to the player character (Read the callbacks documentation at `item.inc`). This script replaces standard pickups with more customizable and interactive items.

*    Supports item interactions like picking up and dropping items.
*    Items can be placed on player model bones when picked up, adding visual representation.

# **Container**
A script that allows the creation of virtual inventories (containers) to store items. Containers can either be interactable in the game world using buttons or used entirely via script functions.

*    Supports interaction with containers via buttons or scripts.
*    Players can switch between their inventory and containers for easy item swapping or looting.

# **Inventory**
Extends item functionality by adding a virtual item system. Players can store and retrieve multiple items, with complete control over inventory actions provided through various functions and callbacks.

*    Full control over inventory management.
*    Virtual items can be stored, retrieved, and manipulated.

# **Installation**
*    Download and include the necessary .inc files in your project.
*    Use the functions and callbacks provided to implement buttons, items, inventories, and containers in your server.

```pwn
#include <I\I>
```
Or include individual:
```pwn
#include <I\button>
```

# **License**
This project is open-source and can be modified to suit the needs of your server.
