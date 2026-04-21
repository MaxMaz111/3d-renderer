#include "tile_mutex_manager.h"

namespace renderer::kernel {

TileMutexManager::TileMutexManager(Width width, Height height) {
  Init(width, height);
}

void TileMutexManager::ResetTo(Width width, Height height) {
  Init(width, height);
}

tbb::spin_mutex& TileMutexManager::GetMutexFor(Width x, Height y) {
  return tile_locks_[TileIndex(x, y)];
}

const tbb::spin_mutex& TileMutexManager::GetMutexFor(Width x, Height y) const {
  return tile_locks_[TileIndex(x, y)];
}

void TileMutexManager::Init(Width width, Height height) {
  const int w = width;
  const int h = height;
  tile_columns_ = (w + kTileSize - 1) / kTileSize;
  tile_rows_ = (h + kTileSize - 1) / kTileSize;
  tile_lock_count_ = tile_columns_ * tile_rows_;
  tile_locks_ = std::make_unique<tbb::spin_mutex[]>(tile_lock_count_);
}

int TileMutexManager::TileIndex(Width x, Height y) const {
  const int tile_x = x / kTileSize;
  const int tile_y = y / kTileSize;
  return tile_x + tile_y * tile_columns_;
}

}  // namespace renderer::kernel
