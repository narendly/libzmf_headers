/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/*
 * This file is a part of the libzmf project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef ZMF4PARSER_H_INCLUDED
#define ZMF4PARSER_H_INCLUDED

#include <librevenge/librevenge.h>
#include "libzmf_utils.h"
#include "ZMF4Header.h"
#include "ZMFCollector.h"

namespace libzmf
{

class ZMF4Parser
{
  // disable copying
  ZMF4Parser(const ZMF4Parser &other);
  ZMF4Parser &operator=(const ZMF4Parser &other);

public:
  ZMF4Parser(const RVNGInputStreamPtr &input, ZMFCollector *collector);
  bool parse();

private:
  enum ObjectType
  {
    PAGE_START = 0x21,
    MASTER_PAGE = 0x22,
    PAGE_END = 0x23,
    LAYER_START = 0x24,
    LAYER_END = 0x25,
    DOCUMENT_SETTINGS = 0x27,
    STYLESHEET = 0x28
  };

  struct ObjectHeader
  {
    ObjectType type;
    uint32_t size;
    uint32_t nextObjectOffset;
  };

  ObjectHeader readObjectHeader();

  void readBitmap();

  void readDocumentSettings();
  void readStylesheet();
  void readPage();
  void readLayer(const ObjectHeader &layerStartObjHeader);

  const RVNGInputStreamPtr m_input;
  ZMFCollector *m_collector;

  ZMF4Header m_header;

  double m_pageWidth;
  double m_pageHeight;
};

}

#endif // ZMF4PARSER_H_INCLUDED

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
