
#pragma once

#define APP_CONTEXT_VERSION_MAJOR 3
#define APP_CONTEXT_VERSION_MINOR 3

#define APP_NAME        "final project"
#define APP_RESIZABLE   0

#define APP_WIDTH  1024
#define APP_HEIGHT 768

#ifdef TYSOC_RESOURCES_PATH
	#define ENGINE_RESOURCES_PATH TYSOC_RESOURCES_PATH
#else
	#define ENGINE_RESOURCES_PATH "../res/"
#endif
