/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/*
 * This file is a part of the libzmf project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ZMF2Header.h"
#define SIGNATURE_ZMF2 0x12345678

namespace libzmf
{

ZMF2Header::ZMF2Header()
  : h_version(0)
  , h_signature(0)
{
}

bool ZMF2Header::load(const RVNGInputStreamPtr &input)
{
  seek(input, 10);
  
  h_version = readU8(input, false);
  h_signature = readU32(input, false);

  if (!isSupported())
	return false;

  return true;
  
}

uint8_t ZMF2Header::get_version() const
{
  return h_version;
}

uint32_t ZMF2Header::get_signature() const
{
  return h_signature;
}

bool ZMF2Header::isSupported() const
{  
  return SIGNATURE_ZMF2 == h_signature;
}

}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
