// libTorrent - BitTorrent library
// Copyright (C) 2005-2011, Jari Sundell
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// In addition, as a special exception, the copyright holders give
// permission to link the code of portions of this program with the
// OpenSSL library under certain conditions as described in each
// individual source file, and distribute linked combinations
// including the two.
//
// You must obey the GNU General Public License in all respects for
// all of the code used other than OpenSSL.  If you modify file(s)
// with this exception, you may extend this exception to your version
// of the file(s), but you are not obligated to do so.  If you do not
// wish to do so, delete this exception statement from your version.
// If you delete this exception statement from all source files in the
// program, then also delete it here.
//
// Contact:  Jari Sundell <jaris@ifi.uio.no>
//
//           Skomakerveien 33
//           3185 Skoppum, NORWAY

#ifndef LIBTORRENT_RC4_H
#define LIBTORRENT_RC4_H

#include "config.h"

#include <cstdint>

namespace torrent {

class RC4 {
public:
  RC4() : m_i(0), m_j(0) { }

  RC4(const unsigned char key[], int len) : m_i(0), m_j(0) {
    init(key, len);
  }

  void init(const unsigned char key[], int len) {
    // Key Scheduling Algorithm (KSA)
    for (int i = 0; i < 256; i++)
      m_state[i] = i;

    int j = 0;
    for (int i = 0; i < 256; i++) {
      j = (j + m_state[i] + key[i % len]) & 0xff;
      std::swap(m_state[i], m_state[j]);
    }

    m_i = 0;
    m_j = 0;
  }

  void crypt(const void* indata, void* outdata, unsigned int length) {
    const unsigned char* input = static_cast<const unsigned char*>(indata);
    unsigned char* output = static_cast<unsigned char*>(outdata);

    for (unsigned int k = 0; k < length; k++) {
      // Pseudo-random Generation Algorithm (PRGA)
      m_i = (m_i + 1) & 0xff;
      m_j = (m_j + m_state[m_i]) & 0xff;
      std::swap(m_state[m_i], m_state[m_j]);
      output[k] = input[k] ^ m_state[(m_state[m_i] + m_state[m_j]) & 0xff];
    }
  }

  void crypt(void* data, unsigned int length) {
    crypt(data, data, length);
  }

private:
  uint8_t m_state[256];
  uint8_t m_i;
  uint8_t m_j;
};

};

#endif
