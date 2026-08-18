#include "editor/EditorController.h"

#include <SDL3/SDL.h>
#include <imgui.h>

namespace Modwin
{
	void EditorController::Update(
		EditorState& state,
		MapDocument& document,
		const MapViewport& viewport) const
	{
		state.hoveredColumn = -1;
		state.hoveredRow = -1;

		if (state.mode != EditorMode::Edit || ImGui::GetIO().WantCaptureMouse)
		{
			return;
		}

		auto& layers = document.map.GetLayers();
		if (layers.empty() || state.activeLayerIndex >= layers.size())
		{
			return;
		}

		float mouseX = 0.0F;
		float mouseY = 0.0F;
		const SDL_MouseButtonFlags mouseButtons = SDL_GetMouseState(&mouseX, &mouseY);
		TileLayer& activeLayer = layers[state.activeLayerIndex];

		const auto coordinate = ScreenToTile(
			mouseX,
			mouseY,
			viewport,
			state.camera.x,
			state.camera.y,
			state.camera.zoom,
			document.map.GetTileWidth(),
			document.map.GetTileHeight(),
			activeLayer.GetWidth(),
			activeLayer.GetHeight());
		if (!coordinate.has_value())
		{
			return;
		}

		state.hoveredColumn = coordinate->column;
		state.hoveredRow = coordinate->row;

		if ((mouseButtons & SDL_BUTTON_LMASK) == 0)
		{
			return;
		}

		TileGid replacement = 0;
		if (state.activeTool == EditorTool::Paint)
		{
			if (state.selectedTile == 0)
			{
				return;
			}
			replacement = state.selectedTile;
		}

		if (activeLayer.SetTile(coordinate->column, coordinate->row, replacement))
		{
			document.MarkDirty();
		}
	}
}
