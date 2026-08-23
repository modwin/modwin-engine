#include "modwin/abi/ModwinGameApi.h"

int main(void)
{
	ModwinGameApi game_api = {0};
	ModwinEntityCommand command = {0};
	game_api.abi_version = MODWIN_GAME_ABI_VERSION;
	game_api.struct_size = (uint32_t)sizeof(ModwinGameApi);
	command.struct_size = (uint32_t)sizeof(ModwinEntityCommand);
	command.type = MODWIN_COMMAND_SET_POSITION;

	return game_api.abi_version == 1U &&
		game_api.struct_size >= MODWIN_GAME_API_V1_SIZE &&
		command.struct_size >= MODWIN_ENTITY_COMMAND_V1_SIZE &&
		command.type == MODWIN_COMMAND_SET_POSITION &&
		MODWIN_INVALID_ENTITY_HANDLE == UINT64_C(0)
		? 0
		: 1;
}
