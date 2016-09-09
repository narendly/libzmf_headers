/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/*
 * This file is a part of the libzmf project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ZMF2Parser.h"

namespace
{

  double um2in(uint32_t micrometers)
  {
    return micrometers / 1000 / 25.4;
  }

}

namespace libzmf
{

ZMF2Parser::ZMF2Parser(const RVNGInputStreamPtr &input, ZMFCollector *collector)
  : m_input(input)
  , m_collector(collector)
  , m_header() //whose reference is this?
  , m_pageWidth(0.0)
  , m_pageHeight(0.0)
{
}

bool ZMF2Parser::parse()
{
  if (!m_header.load(m_input))
    return false;

  m_collector->startDocument();

  // TODO: Analyze ZMF2 components
  // 1. BitmapDB.zmf
  // 2. Comment.txt
  // 3. Preview.bmp
  // 4. TextStyles.zmf
  // 5. Callisto_doc.zmf
  // 6. Callisto_pages.zmf
  
  // seek(m_input, m_header.startContentOffset());
  // readDocumentSettings();
  // readStylesheet();

  // while (!m_input->isEnd())
  // {
    // readPage();
  // }

  m_collector->endDocument();

  return true;
}

}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
