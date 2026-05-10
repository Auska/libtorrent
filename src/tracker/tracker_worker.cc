#include "config.h"

#include <climits>
#include <random>

#include "tracker_worker.h"

namespace torrent {

TrackerWorker::TrackerWorker(TrackerInfo info, int flags)
  : m_info(info) {

  m_state.m_flags = flags;
}

TrackerWorker::~TrackerWorker() = default;

void
TrackerWorker::rotate_peer_id() {
  std::random_device rd;
  std::mt19937 mt(rd());
  using bytes_randomizer = std::independent_bits_engine<std::mt19937, CHAR_BIT, uint8_t>;
  bytes_randomizer bytes(mt);

  std::string suffix;
  suffix.resize(20 - std::string(PEER_NAME).size());
  std::generate(suffix.begin(), suffix.end(), std::ref(bytes));

  std::string new_id = PEER_NAME + suffix;
  m_info.local_id.assign(new_id.c_str());
}

}  // namespace torrent
