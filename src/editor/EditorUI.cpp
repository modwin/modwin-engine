#include "editor/EditorUI.h"
#include "graphics/MapSerializer.h"

#include <algorithm>
#include <cstdint>
#include <imgui.h>
#include <string>

namespace Modwin
{
	MapViewport EditorUI::Draw(
		EditorState& state, MapDocument& document, TextureManager& textures) const
	{
		constexpr float sidebarWidth = 360.0F;
		const ImVec2 displaySize = ImGui::GetIO().DisplaySize;
		const float actualSidebarWidth = std::min(sidebarWidth, displaySize.x);
		const MapViewport viewport{
			actualSidebarWidth,
			0.0F,
			std::max(0.0F, displaySize.x - actualSidebarWidth),
			std::max(0.0F, displaySize.y)};

		ImGui::SetNextWindowPos(ImVec2(0.0F, 0.0F));
		ImGui::SetNextWindowSize(ImVec2(actualSidebarWidth, displaySize.y));
		constexpr ImGuiWindowFlags sidebarFlags =
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoSavedSettings;
		ImGui::Begin("Modwin Editor", nullptr, sidebarFlags);

		if (ImGui::RadioButton("Edit", state.mode == EditorMode::Edit))
		{
			state.mode = EditorMode::Edit;
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("Play", state.mode == EditorMode::Play))
		{
			state.mode = EditorMode::Play;
		}
		ImGui::SameLine();
		ImGui::TextUnformatted(document.IsDirty() ? "Unsaved changes" : "Saved");
		ImGui::TextWrapped("Map: %s", document.sourcePath.string().c_str());
		if (ImGui::Button("Save") &&
			MapSerializer::Save(document.map, document.sourcePath))
		{
			document.MarkSaved();
		}

		if (state.mode == EditorMode::Play)
		{
			ImGui::TextUnformatted("Gameplay input is active.");
			ImGui::End();
			return viewport;
		}

		ImGui::SeparatorText("Tool");
		if (ImGui::RadioButton("Paint", state.activeTool == EditorTool::Paint))
		{
			state.activeTool = EditorTool::Paint;
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("Erase", state.activeTool == EditorTool::Erase))
		{
			state.activeTool = EditorTool::Erase;
		}
		if (state.hoveredColumn >= 0 && state.hoveredRow >= 0)
		{
			ImGui::Text("Hovered tile: %d, %d", state.hoveredColumn, state.hoveredRow);
		}
		else
		{
			ImGui::TextDisabled("Hovered tile: outside map");
		}

		ImGui::SeparatorText("Camera");
		ImGui::DragFloat2("Position", &state.camera.x, 4.0F, 0.0F, 100000.0F, "%.0f");
		ImGui::SliderFloat("Zoom", &state.camera.zoom, 0.25F, 4.0F, "%.2fx");
		if (ImGui::Button("Reset camera"))
		{
			state.camera = {};
		}

		auto& layers = document.map.GetLayers();
		ImGui::SeparatorText("Layers");
		if (layers.empty())
		{
			ImGui::TextDisabled("This map has no tile layers.");
		}
		else
		{
			state.activeLayerIndex = std::min(state.activeLayerIndex, layers.size() - 1U);
			for (std::size_t index = 0; index < layers.size(); ++index)
			{
				const bool selected = index == state.activeLayerIndex;
				if (ImGui::Selectable(layers[index].GetName().c_str(), selected))
				{
					state.activeLayerIndex = index;
				}
			}
		}

		const auto& tilesets = document.map.GetTilesets();
		ImGui::SeparatorText("Tile palette");
		if (tilesets.empty())
		{
			ImGui::TextDisabled("This map has no tilesets.");
			ImGui::End();
			return viewport;
		}

		state.activeTilesetIndex = std::min(state.activeTilesetIndex, tilesets.size() - 1U);
		const char* previewName = tilesets[state.activeTilesetIndex].name.c_str();
		if (ImGui::BeginCombo("Tileset", previewName))
		{
			for (std::size_t index = 0; index < tilesets.size(); ++index)
			{
				const bool selected = index == state.activeTilesetIndex;
				if (ImGui::Selectable(tilesets[index].name.c_str(), selected))
				{
					state.activeTilesetIndex = index;
				}
				if (selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

		const Tileset& tileset = tilesets[state.activeTilesetIndex];
		SDL_Texture* texture = textures.GetTexture(tileset.name);
		if (texture == nullptr || tileset.columnCount <= 0 || tileset.rowCount <= 0)
		{
			ImGui::TextDisabled("The selected tileset texture is unavailable.");
			ImGui::End();
			return viewport;
		}

		constexpr float paletteTileSize = 40.0F;
		for (int localIndex = 0; localIndex < tileset.tileCount; ++localIndex)
		{
			const int column = localIndex % tileset.columnCount;
			const int row = localIndex / tileset.columnCount;
			const ImVec2 uv0(
				static_cast<float>(column) / static_cast<float>(tileset.columnCount),
				static_cast<float>(row) / static_cast<float>(tileset.rowCount));
			const ImVec2 uv1(
				static_cast<float>(column + 1) / static_cast<float>(tileset.columnCount),
				static_cast<float>(row + 1) / static_cast<float>(tileset.rowCount));
			const TileGid tileGid = tileset.firstId + static_cast<TileGid>(localIndex);

			ImGui::PushID(localIndex);
			const bool selected = state.selectedTile == tileGid;
			if (selected)
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
			}

			if (ImGui::ImageButton(
				"tile",
				reinterpret_cast<ImTextureID>(texture),
				ImVec2(paletteTileSize, paletteTileSize),
				uv0,
				uv1))
			{
				state.selectedTile = tileGid;
				state.activeTool = EditorTool::Paint;
			}

			if (selected)
			{
				ImGui::PopStyleColor();
			}
			ImGui::PopID();

			if ((localIndex + 1) % tileset.columnCount != 0)
			{
				ImGui::SameLine();
			}
		}

		ImGui::Text("Selected GID: %u", static_cast<unsigned int>(state.selectedTile));
		ImGui::End();
		return viewport;
	}

	CloseDecision EditorUI::DrawCloseConfirmation(
		MapDocument& document, const bool openPopup) const
	{
		if (openPopup)
		{
			ImGui::OpenPopup("Unsaved changes");
		}

		CloseDecision decision = CloseDecision::None;
		if (ImGui::BeginPopupModal(
			"Unsaved changes", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::TextUnformatted("The active map contains unsaved changes.");
			ImGui::TextUnformatted("Save before closing Modwin Engine?");
			ImGui::Separator();

			if (ImGui::Button("Save and close") &&
				MapSerializer::Save(document.map, document.sourcePath))
			{
				document.MarkSaved();
				decision = CloseDecision::SaveAndClose;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Discard changes"))
			{
				decision = CloseDecision::DiscardAndClose;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
			{
				decision = CloseDecision::Cancel;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		return decision;
	}
}
