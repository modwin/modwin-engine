#ifndef MODWIN_ENGINE_TILELAYER_H
#define MODWIN_ENGINE_TILELAYER_H

#include <cstdint>
#include <graphics/SurfaceLayer.h>
#include <vector>
#include <string>

namespace Modwin
{
	struct Tileset
	{
		Tileset();

		int firstId, lastId;
		int rowCount, columnCount;
		int tileWidth;
		int tileCount;
		std::string name, source;

	};

	//TileSetVec = TileSetList
	using TilesetVec = std::vector<Tileset>;
	// TileVec2D = TileMap
	using TileGid = std::uint32_t;
	using TileVec2D = std::vector<std::vector<int>>;

	constexpr TileGid FlipHorizontal = 0x80000000;
	constexpr TileGid FlipVertical   = 0x40000000;
	constexpr TileGid FlipDiagonal   = 0x20000000;
	constexpr TileGid TileIdMask     = 0x1FFFFFFF;

	class TileLayer : public SurfaceLayer
	{
	public:
			TileLayer(int tileWidth, int rowCount, int columnCount, TileVec2D tileVec, TilesetVec tileSetVec);
			void Render() override;
			void Update() override;

			bool IsInBounds(int column, int row) const noexcept;
			int GetWidth() const noexcept;
			int GetHeight() const noexcept;
			const std::string& GetName() const noexcept;

			int CalculateTileForCorruption(int row, int column);
			bool ContainsTile(int row, int tileId);
			const TileVec2D& GetTileVec() const noexcept { return m_TileVec2D; }
			const TilesetVec& GetTileSetVec() const noexcept { return m_Tilesets; }
	private:

			int m_Width, m_Height = 0;
			std::vector<TileGid> m_Tiles;
			int m_ColumnCount, m_RowCount;
			TileVec2D m_TileVec2D;
			TilesetVec m_Tilesets;
	};

} // Modwin

#endif //MODWIN_ENGINE_TILELAYER_H
