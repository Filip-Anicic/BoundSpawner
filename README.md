<img align="left" width="80" height="80" src="https://media.githubusercontent.com/media/Filip-Anicic/BoundSpawner/main/Resources/Icon128.png" alt="Ackerman's Roulette Plugin">

# BoundSpawner
Unreal Engine plugin that spawns a set amount of user specified meshes in a bounding box.

Built with C++ and Slate. Tested with UE 5.0

## Demo
https://github.com/Filip-Anicic/BoundSpawner/assets/23241818/fa95898e-d9f3-4ffb-acb0-7ea65a4699df

## How to install
> Note: Repository uses Git LFS, so download ZIP option on GitHub would not download the entire repository content.

1. Create an Unreal Engine 5.0+ project.
2. Open root of the project in the file browser.
3. Add folder called **Plugins**.
4. Clone repository with Git and place into Plugins folder.
5. Launch project and rebuild module when prompted by Unreal Engine.
6. If no toolbar icon is present, open the Plugins window, find and enable the BoundSpawner plugin.

## How to use
1. When enabled, open BoundSpawner window using the toolbar button or **Tools->BoundSpawner**.
2. Set Mesh to spawn, spawn range of position, rotation and scale.
3. Optional - Change the data table the tool will write to.
4. Optional - Change the material. 
5. Press **Spawn** button
> Note: The plugin requires a **Color** property in the material to change the color of the meshes

## Known bugs
- Data table window doesn't get refreshed after meshes have been spawned.
