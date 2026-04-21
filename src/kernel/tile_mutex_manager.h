#pragma once

#include <memory>
#include <tbb/spin_mutex.h>

#include "util/size.h"

namespace renderer::kernel {

class TileMutexManager {
  static constexpr int kTileSize = 16;

 public:
  TileMutexManager() = default;
  TileMutexManager(Width width, Height height);

  void ResetTo(Width width, Height height);
  tbb::spin_mutex& GetMutexFor(Width x, Height y);
  const tbb::spin_mutex& GetMutexFor(Width x, Height y) const;

 private:
  void Init(Width width, Height height);
  int TileIndex(Width x, Height y) const;

  int tile_columns_ = 0;
  int tile_rows_ = 0;
  int tile_lock_count_ = 0;
  std::unique_ptr<tbb::spin_mutex[]> tile_locks_;
};

}  // namespace renderer::kernel
