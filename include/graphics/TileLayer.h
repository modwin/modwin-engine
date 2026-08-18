#ifndef MODWIN_ENGINE_TILELAYER_H
#define MODWIN_ENGINE_TILELAYER_H

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace Modwin
{
	using TileGid = std::uint32_t;

	// Tiled stores transformation flags in the highest bits of each global tile ID.
	inline constexpr TileGid FlipHorizontal = 0x80000000U;
	inline constexpr TileGid FlipVertical = 0x40000000U;
	inline constexpr TileGid FlipDiagonal = 0x20000000U;
	inline constexpr TileGid TileIdMask = 0x1FFFFFFFU;

	struct Tileset
	{
		TileGid firstId = 0;
		TileGid lastId = 0;
		int tileCount = 0;
		int columnCount = 0;
		int rowCount = 0;
		int tileWidth = 0;
		int tileHeight = 0;
		std::string name;
		std::string source;
	};

	using TilesetVec = std::vector<Tileset>;

	class TileLayer
	{
	public:
		TileLayer(std::string name, int width, int height, int tileWidth, std::vector<TileGid> tiles);

		[[nodiscard]] bool IsInBounds(int column, int row) const noexcept;
		[[nodiscard]] TileGid GetTile(int column, int row) const;
		[[nodiscard]] bool SetTile(int column, int row, TileGid tileGid) noexcept;
		[[nodiscard]] bool ContainsTile(int row, TileGid tileId) const noexcept;

		[[nodiscard]] int GetWidth() const noexcept;
		[[nodiscard]] int GetHeight() const noexcept;
		[[nodiscard]] int GetTileWidth() const noexcept;
		[[nodiscard]] const std::string& GetName() const noexcept;
		[[nodiscard]] const std::vector<TileGid>& GetTiles() const noexcept;

		void Update();
		void Render(const TilesetVec& tilesets) const;

	private:
		[[nodiscard]] std::size_t GetIndex(int column, int row) const noexcept;

		std::string m_Name;
		int m_TileWidth = 0;
		int m_Width = 0;
		int m_Height = 0;
		std::vector<TileGid> m_Tiles;
	};
}

#endif // MODWIN_ENGINE_TILELAYER_H
