/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/*
 * This file is a part of the libzmf project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef ZMF2PARSER_H_INCLUDED
#define ZMF2PARSER_H_INCLUDED

#include <librevenge/librevenge.h>
#include "libzmf_utils.h"
#include "ZMF2Header.h"
#include "ZMFCollector.h"

namespace libzmf
{

class ZMF2Parser
{
  // disable copying
  ZMF2Parser(const ZMF2Parser &other);
  ZMF2Parser &operator=(const ZMF2Parser &other);

public:
  ZMF2Parser(const RVNGInputStreamPtr &input, ZMFCollector *collector);
  bool parse();

private:
  enum ObjectType
  {
    PAGE = 0x3,
    LAYER = 0x4,
	RECTANGLE = 0x8,
    IMAGE = 0x9,
    COLOR = 0xa,
    //TODO: add more components
	TEXT_FRAME = 0x13,
    BITMAP_DEF = 0x201
  }; 
  
struct ObjectHeader
  {
    ObjectType type;
    uint32_t size;
    uint32_t nextObjectOffset;
  };

  //ObjectHeader readObjectHeader();

  ZMF2Header m_header;
  ZMFCollector *m_collector;
  
  double m_pageWidth;
  double m_pageHeight;
  
  const RVNGInputStreamPtr m_input;
};

}

#endif // ZMF2PARSER_H_INCLUDED

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
