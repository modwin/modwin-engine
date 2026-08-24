#ifndef MODWIN_GAME_API_H
#define MODWIN_GAME_API_H

#include <stddef.h>
#include <stdint.h>

#if defined(_WIN32)
#define MODWIN_ABI_CALL __cdecl
#else
#define MODWIN_ABI_CALL
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define MODWIN_GAME_ABI_VERSION 1U
#define MODWIN_INVALID_ENTITY_HANDLE UINT64_C(0)

typedef uint64_t ModwinEntityHandle;

typedef int32_t ModwinResult;
enum
{
	MODWIN_RESULT_OK = 0,
	MODWIN_RESULT_INVALID_ARGUMENT = 1,
	MODWIN_RESULT_INCOMPATIBLE_ABI = 2,
	MODWIN_RESULT_NOT_FOUND = 3,
	MODWIN_RESULT_INTERNAL_ERROR = 4
};

typedef uint32_t ModwinLogLevel;
enum
{
	MODWIN_LOG_TRACE = 0,
	MODWIN_LOG_INFO = 1,
	MODWIN_LOG_WARNING = 2,
	MODWIN_LOG_ERROR = 3
};

// Stable physical key identifiers. Values follow the USB keyboard usage page,
// which also makes the current SDL implementation a direct conversion.
typedef uint32_t ModwinKeyCode;
enum
{
	MODWIN_KEY_A = 4,
	MODWIN_KEY_D = 7,
	MODWIN_KEY_S = 22,
	MODWIN_KEY_W = 26,
	MODWIN_KEY_SPACE = 44
};

typedef uint32_t ModwinEntityFlags;
enum
{
	MODWIN_ENTITY_HAS_KINEMATIC_BODY = 1U << 0
};

typedef uint32_t ModwinFlipMode;
enum
{
	MODWIN_FLIP_NONE = 0,
	MODWIN_FLIP_HORIZONTAL = 1U << 0,
	MODWIN_FLIP_VERTICAL = 1U << 1
};

typedef uint32_t ModwinEntityCommandType;
enum
{
	MODWIN_COMMAND_SET_KINEMATIC_INPUT = 1,
	MODWIN_COMMAND_SET_ANIMATION = 2,
	MODWIN_COMMAND_SET_POSITION = 3
};

typedef struct ModwinVec2
{
	float x;
	float y;
} ModwinVec2;

typedef struct ModwinMapInfo
{
	uint32_t struct_size;
	int32_t width;
	int32_t height;
	int32_t tile_width;
	int32_t tile_height;
} ModwinMapInfo;

// All strings are UTF-8 and borrowed for the duration of the call. The engine
// copies values it retains. The creating side owns all allocated memory.
typedef struct ModwinEntityCreateInfo
{
	uint32_t struct_size;
	uint32_t flags;
	ModwinVec2 position;
	ModwinVec2 sprite_size;
	float maximum_speed;
	float acceleration;
	float deceleration;
	uint32_t flip_mode;
	const char* texture_id;
} ModwinEntityCreateInfo;

// A fixed-size command avoids per-property interop calls. Command meanings:
// KINEMATIC_INPUT: value0=x, value1=y
// ANIMATION: data0=row, data1=frame count, data2=frame duration ms
// POSITION: value0=x, value1=y
typedef struct ModwinEntityCommand
{
	uint32_t struct_size;
	uint32_t type;
	ModwinEntityHandle entity;
	float value0;
	float value1;
	float value2;
	float value3;
	uint32_t data0;
	uint32_t data1;
	uint32_t data2;
	uint32_t data3;
} ModwinEntityCommand;

typedef struct ModwinEngineApi
{
	uint32_t abi_version;
	uint32_t struct_size;
	void* engine_context;

	void (MODWIN_ABI_CALL *log)(
		void* engine_context, uint32_t level, const char* utf8_message);
	uint8_t (MODWIN_ABI_CALL *input_key_down)(
		void* engine_context, uint32_t key_code);
	uint8_t (MODWIN_ABI_CALL *input_key_pressed)(
		void* engine_context, uint32_t key_code);
	uint8_t (MODWIN_ABI_CALL *input_key_released)(
		void* engine_context, uint32_t key_code);

	ModwinResult (MODWIN_ABI_CALL *load_texture)(
		void* engine_context, const char* texture_id, const char* resource_directory);
	ModwinEntityHandle (MODWIN_ABI_CALL *create_entity)(
		void* engine_context, const ModwinEntityCreateInfo* create_info);
	ModwinResult (MODWIN_ABI_CALL *destroy_entity)(
		void* engine_context, ModwinEntityHandle entity);
	ModwinResult (MODWIN_ABI_CALL *get_entity_position)(
		void* engine_context, ModwinEntityHandle entity, ModwinVec2* position);
	ModwinResult (MODWIN_ABI_CALL *submit_entity_commands)(
		void* engine_context, const ModwinEntityCommand* commands, uint32_t command_count);
} ModwinEngineApi;

typedef struct ModwinGameApi
{
	uint32_t abi_version;
	uint32_t struct_size;
	void* game_context;

	ModwinResult (MODWIN_ABI_CALL *initialize)(
		void* game_context, const ModwinEngineApi* engine_api);
	ModwinResult (MODWIN_ABI_CALL *shutdown)(void* game_context);
	ModwinResult (MODWIN_ABI_CALL *enter_play)(
		void* game_context, const ModwinMapInfo* map_info);
	ModwinResult (MODWIN_ABI_CALL *exit_play)(void* game_context);
	ModwinResult (MODWIN_ABI_CALL *capture_input)(
		void* game_context, uint8_t accept_player_input);
	ModwinResult (MODWIN_ABI_CALL *fixed_update)(
		void* game_context, float delta_seconds);
	ModwinResult (MODWIN_ABI_CALL *update)(
		void* game_context, float delta_seconds);
} ModwinGameApi;

// Required version-1 prefixes. New optional fields may be appended to a
// structure without changing these values or breaking an older host/module.
#define MODWIN_MAP_INFO_V1_SIZE \
	((uint32_t)(offsetof(ModwinMapInfo, tile_height) + \
		sizeof(((ModwinMapInfo*)0)->tile_height)))
#define MODWIN_ENTITY_CREATE_INFO_V1_SIZE \
	((uint32_t)(offsetof(ModwinEntityCreateInfo, texture_id) + \
		sizeof(((ModwinEntityCreateInfo*)0)->texture_id)))
#define MODWIN_ENTITY_COMMAND_V1_SIZE \
	((uint32_t)(offsetof(ModwinEntityCommand, data3) + \
		sizeof(((ModwinEntityCommand*)0)->data3)))
#define MODWIN_ENGINE_API_V1_SIZE \
	((uint32_t)(offsetof(ModwinEngineApi, submit_entity_commands) + \
		sizeof(((ModwinEngineApi*)0)->submit_entity_commands)))
#define MODWIN_GAME_API_V1_SIZE \
	((uint32_t)(offsetof(ModwinGameApi, update) + \
		sizeof(((ModwinGameApi*)0)->update)))

// Signature for a future native shared-library entry point. Managed runtime
// adapters can construct the same ModwinGameApi table from managed delegates.
typedef ModwinResult (MODWIN_ABI_CALL *ModwinGetGameApiFn)(
	uint32_t requested_abi_version, ModwinGameApi* game_api);

#ifdef __cplusplus
}
#endif

#endif // MODWIN_GAME_API_H
