/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/*
 * This file is a part of the libzmf project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ZMF4Parser.h"

namespace
{

double um2in(uint32_t micrometers)
{
  return micrometers / 1000 / 25.4;
}

}

namespace libzmf
{

ZMF4Parser::ZMF4Parser(const RVNGInputStreamPtr &input, ZMFCollector *collector)
  : m_input(input)
  , m_collector(collector)
  , m_header()
  , m_pageWidth(0.0)
  , m_pageHeight(0.0)
{
}

bool ZMF4Parser::parse()
{
  if (!m_header.load(m_input) || !m_header.isSupported())
    return false;

  m_collector->startDocument();

  if (m_header.startBitmapOffset() > 0)
  {
    seek(m_input, m_header.startBitmapOffset());

    readBitmap();
  }
  else
  {
    seek(m_input, m_header.startContentOffset());
  }

  readDocumentSettings();

  readStylesheet();

  while (!m_input->isEnd())
  {
    readPage();
  }

  m_collector->endDocument();

  return true;
}

ZMF4Parser::ObjectHeader ZMF4Parser::readObjectHeader()
{
  ObjectHeader header;

  header.size = readU32(m_input);
  header.type = static_cast<ObjectType>(readU8(m_input));
  header.nextObjectOffset = m_input->tell() + (header.size - 5);

  return header;
}

void ZMF4Parser::readBitmap()
{
  skip(m_input, 2);

  uint32_t size = readU32(m_input);

  skip(m_input, size - 2 - 4);
}

void ZMF4Parser::readDocumentSettings()
{
  ObjectHeader header = readObjectHeader();

  if (header.type != DOCUMENT_SETTINGS)
    throw GenericException();

  skip(m_input, 63);

  m_pageWidth = um2in(readU32(m_input));
  m_pageHeight = um2in(readU32(m_input));

  seek(m_input, header.nextObjectOffset);
}

void ZMF4Parser::readStylesheet()
{
  ObjectHeader header = readObjectHeader();

  if (header.type != STYLESHEET)
    throw GenericException();

  seek(m_input, header.nextObjectOffset);
}

void ZMF4Parser::readPage()
{
  ObjectHeader startPageHeader = readObjectHeader();

  if (startPageHeader.type != PAGE_START)
    throw GenericException();

  m_collector->startPage(m_pageWidth, m_pageHeight);

  seek(m_input, startPageHeader.nextObjectOffset);

  while (true)
  {
    ObjectHeader header = readObjectHeader();

    switch (header.type)
    {
    case MASTER_PAGE:
      seek(m_input, header.nextObjectOffset);
      break;
    case PAGE_END:
      m_collector->endPage();
      seek(m_input, header.nextObjectOffset);
      return;
    case LAYER_START:
      readLayer(header);
      break;
    default:
      throw GenericException();
    }
  }
}

void ZMF4Parser::readLayer(const ZMF4Parser::ObjectHeader &layerStartObjHeader)
{
  if (layerStartObjHeader.type != LAYER_START)
    throw GenericException();

  m_collector->startLayer();

  seek(m_input, layerStartObjHeader.nextObjectOffset);

  while (true)
  {
    ObjectHeader header = readObjectHeader();

    switch (header.type)
    {
    case LAYER_END:
      seek(m_input, header.nextObjectOffset);
      m_collector->endLayer();
      return;
    default:
      seek(m_input, header.nextObjectOffset);
    }
  }
}

}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
