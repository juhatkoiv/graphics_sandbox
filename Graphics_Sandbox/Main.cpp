#include "Precompiled/Precompiled.h"

#include "Editor/Editor.h"
#include "MainLoop.h"

/*
* ================
* GOALS:
* ================
* - Four scenes:
*   + Basic material scene with spheres
*   + Very plain scene with spehres and boxes to cast shadows
*   + Voxelated terrain with different materials (similar to questing project) and vegetation.
*   + Vegetation scene with grass, trees, hay, rocks, water, trees
*   + Snowy scene with footprints. High fidelity.
* - Each scene has:
*   + Directional lights, global lights etc ( BRDF )
*   + Shadows
*	+ Some dynamic light sources as point lights (spirits, fireflies...) that also glow.
*   + Environmental effects, wind, rain, fog, and volumetric effects.
*   + Normal mapping, height mapping etc.
*   + Various full screen effects.
*   + SSAO
* - The tech supports:
*   + Building, saving and loading a scene.
*   + Editor to make adjustments to scene.
*   + ECS - components (data only) and some systems.
*   + Renderer: OpenGL first. Then pick either: Vulkan or DX12.
*   + Resource system: Load and unload resources.
*	+ Serialization: Save and load data.
*	+ Don't care about editor code quality, just make it work.
* 
* =====
* TASKS
* =====
* - Renderer encoding and and architecture improvements
*   + simplify configurations and data flow to renderer
*   + move pass data and pass definition data to same data.
*   + Strealmine data flow to renderer > encoding and data packing
* - Renderer improvements
*   + Remove single entity rendering functions
*   + Fix 3d outline - Render effects vs materials
* - Multi sources shader suuport: SPIRV, HLSL, GLSL
*	+ Shader argument list serialization
*   + Implement binding shader args
*   + implement support to either convert hlsl to glsl > glsl to hlsl > spirv etc. Investigate pre-existing solutions here.
*   + Implement support to read shader arguments from shaders and tag them as configurable or not.
*   + Implement editor support to change shader arguments
* - Implement missing rendering support (not in any particular order) - OpenGL first.
*   + Noise textures
*	+ z-prepass
*   + Deferred shading
*		- Depth prepass
*		- Opengl multiple render targets
*			- Color attachment 1, 2, 3, depth
*			- GBuffer: Position, Normal, Albedo, Depth
* 			- or any material data that makes sense
*		- Lighting pass
*		- Post processing pass
*   + Buffer update, application, per frame > constant buffers
*   + MSAA / FXAA
*   + BRDF
*   + spot lights
*	+ decals
*   + shadows
*   + global illumination - AO,
*   + Blur, SSAO, Bloom, Depth of field, Motion blur, Volumetric lighting, GI
*   + Bump maps
*	+ Debug rendering: Lines, points, normals, tangents, bitangents, bounding boxes, bounding spheres, frustums, cameras, lights, shadows, 
* - Remove hard dependency from glm
* - Experiment with multithreaded rendering - requires decoupling the glfw context from the window.
* - Implement other API
* - If bored: 
*	+ Add clear scene
*	+ Add
*   + Add possibility to add, remove and configure components.
*   + Add entity transform by mouse (current coordinates and absolute)
*   + Render render graph
*	  - Show data used by passes
*	  - Show if each queues, feature, pass is enabled or not.
*   + Render settings should not be in camera -> global is fine
* ============================
* Architectural considerations
* ============================
* - STEAMLINING DATA:
* 	- Data is already cached in components, resource system and gfx device.
* 	- Generating the frame data each frame would have the following benefits:
* 		1) It decouples the entities from renderer
* 		2) Extending further data conversions becomes more straightforward.
* 		3) Culling and render orders of meshes are all handled in one place.
* 		4) Reduces application complexity by removing additional state variables. Less branching, fewer coupled use cases.
*	- Draw data:
*		- Encodes the data for the renderer as a various bitfields, representing metadata.
*		- The data is then passed to the renderer, which then decodes the data and renders it.
*		- Pre allocate all GPU resources, and then update them as needed.
*		- Aim for 64 byte structs.
*		==> Simplifies CPU logic, and allows for more efficient GPU resource management.
*		- Conceptual data organization may be intuitively undertadable, but the actual data organization should be optimized for the GPU.
*			- Are there gaps in the data?
*			- Do we need 32 bit floats for normals etc.?
*
* ============================
* Observations / BUGS:
* ============================
*	TODO - Materials vs full screen shaders: shader != material.
*	TODO - Move buffers into BufferRef, move resources to ResourceRef
*   TODO - WASD camera movement is sluggish.
*   TODO - Handle transparent ordering in Renderer.
*/

int main( int argc, char** argv )
{
	MainLoop::Args args
	{
		"Graphics Sandbox",
		1600,
		1000
	};

	MainLoop mainLoop( args );
	mainLoop.run();
	return 0;
}
