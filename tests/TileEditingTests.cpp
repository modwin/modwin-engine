#include "Log.h"
#include "graphics/MapParser.h"
#include "graphics/MapSerializer.h"
#include "graphics/TileCoordinate.h"

#include <filesystem>
#include <iostream>
#include <string>

namespace
{
	bool Check(const bool condition, const std::string& message)
	{
		if (!condition)
		{
			std::cerr << "FAILED: " << message << '\n';
		}
		return condition;
	}

	bool MapsAreEqual(const Modwin::TileMap& left, const Modwin::TileMap& right)
	{
		if (left.GetWidth() != right.GetWidth() ||
			left.GetHeight() != right.GetHeight() ||
			left.GetTileWidth() != right.GetTileWidth() ||
			left.GetTileHeight() != right.GetTileHeight())
		{
			return false;
		}

		const auto& leftTilesets = left.GetTilesets();
		const auto& rightTilesets = right.GetTilesets();
		if (leftTilesets.size() != rightTilesets.size())
		{
			return false;
		}

		for (std::size_t index = 0; index < leftTilesets.size(); ++index)
		{
			const auto& a = leftTilesets[index];
			const auto& b = rightTilesets[index];
			if (a.firstId != b.firstId || a.lastId != b.lastId ||
				a.tileCount != b.tileCount || a.columnCount != b.columnCount ||
				a.rowCount != b.rowCount || a.tileWidth != b.tileWidth ||
				a.tileHeight != b.tileHeight || a.name != b.name || a.source != b.source)
			{
				return false;
			}
		}

		const auto& leftLayers = left.GetLayers();
		const auto& rightLayers = right.GetLayers();
		if (leftLayers.size() != rightLayers.size())
		{
			return false;
		}

		for (std::size_t index = 0; index < leftLayers.size(); ++index)
		{
			const auto& a = leftLayers[index];
			const auto& b = rightLayers[index];
			if (a.GetName() != b.GetName() || a.GetWidth() != b.GetWidth() ||
				a.GetHeight() != b.GetHeight() || a.GetTiles() != b.GetTiles())
			{
				return false;
			}
		}

		return true;
	}
}

int main(const int argc, char** argv)
{
	if (argc != 3)
	{
		std::cerr << "Expected a source map and test-output directory.\n";
		return 1;
	}

	Modwin::Log::Init();
	bool passed = true;

	const Modwin::MapViewport viewport{100.0F, 50.0F, 640.0F, 480.0F};
	const auto coordinate = Modwin::ScreenToTile(
		165.0F, 179.0F, viewport, 0.0F, 0.0F, 1.0F, 32, 32, 60, 34);
	passed &= Check(
		coordinate == Modwin::TileCoordinate{2, 4},
		"Screen coordinates should convert to tile column 2, row 4.");
	passed &= Check(
		!Modwin::ScreenToTile(99.0F, 50.0F, viewport, 0.0F, 0.0F, 1.0F, 32, 32, 60, 34)
			.has_value(),
		"Coordinates outside the viewport should be rejected.");

	const std::filesystem::path sourceMap = argv[1];
	auto original = Modwin::MapParser::LoadFromFile(sourceMap);
	passed &= Check(original.has_value(), "The source TMX map should parse.");
	if (!original.has_value())
	{
		return 1;
	}

	auto& layers = original->GetLayers();
	passed &= Check(!layers.empty(), "The source map should contain layers.");
	if (layers.empty())
	{
		return 1;
	}

	const Modwin::TileGid oldTile = layers.front().GetTile(0, 0);
	const Modwin::TileGid replacement = oldTile == 1U ? 2U : 1U;
	passed &= Check(layers.front().SetTile(0, 0, replacement), "A tile should be editable.");
	passed &= Check(
		!layers.front().SetTile(0, 0, replacement),
		"Writing the existing value should not report a change.");

	const std::filesystem::path outputDirectory = argv[2];
	std::filesystem::create_directories(outputDirectory);
	const std::filesystem::path savedMap = outputDirectory / "map-round-trip.tmx";
	passed &= Check(
		Modwin::MapSerializer::Save(*original, savedMap),
		"The modified map should save.");

	auto reloaded = Modwin::MapParser::LoadFromFile(savedMap);
	passed &= Check(reloaded.has_value(), "The saved map should reload.");
	if (reloaded.has_value())
	{
		passed &= Check(
			MapsAreEqual(*original, *reloaded),
			"The saved and reloaded maps should contain equivalent data.");
	}

	std::error_code cleanupError;
	std::filesystem::remove(savedMap, cleanupError);
	return passed ? 0 : 1;
}
