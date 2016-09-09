/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/*
 * This file is a part of the libzmf project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <libzmf/libzmf.h>
#include "ZMF4Header.h"
#include "ZMF4Parser.h"
#include "ZMF2Header.h"
#include "ZMF2Parser.h"
#include "libzmf_utils.h"

namespace libzmf
{

namespace
{

enum ZMFVersion
{
  UNKNOWN_VERSION = 0,
  ZMF2,
  ZMF4
};

ZMFVersion getVersion(librevenge::RVNGInputStream *input)
{
  try
  {
	
    ZMFVersion version = UNKNOWN_VERSION;	
	
    //If not structured, ZMF4
    if (!input->isStructured())
    {
        version = ZMF4;
    }
    else 
    {
        //If it has a file named Header with version 02, ZMF2
        boost::scoped_ptr<librevenge::RVNGInputStream> versionStream(input->getSubStreamByName("Header"));
        if (versionStream)
        {
            if (0x02 == readU8(versionStream.get()))
                version = ZMF2;
        }
        else
        {
            version = ZMF4;
        }

    }
	return version;
  }
  
  catch (...)
  {
    return UNKNOWN_VERSION;
  }
}


RVNGInputStreamPtr getContentStream(librevenge::RVNGInputStream *input, ZMFVersion version)
{
  if (!input)
    return RVNGInputStreamPtr();

  RVNGInputStreamPtr input_(input, ZMFDummyDeleter());

  //Perform version check here
  
  if (version == ZMF4)
  {
    if (input_->isStructured())
    {
    if (input_->existsSubStream("content.zmf"))
      input_.reset(input_->getSubStreamByName("content.zmf"));
    else
      return RVNGInputStreamPtr();
    }
  }
  else //ZMF2
  {
    return input_;
  }
}

bool ZMFDocument::isSupported(librevenge::RVNGInputStream *input) try
{
 
  version = getVersion(input);
  RVNGInputStreamPtr input_ = getContentStream(input, version);
  if (!input_)
    return false;
  switch (version)
  {
	case ZMF2:
	  ZMF2Header header;
	  break;
	case ZMF4:
	  ZMF4Header header;
	  break;
	default:
	  return false;
  }
  return header.load(input_) && header.isSupported();
}

catch (...)
{
  return false;
}

bool ZMFDocument::parse(librevenge::RVNGInputStream *input, librevenge::RVNGDrawingInterface *painter) try
{
    
  RVNGInputStreamPtr input_ = getContentStream(input);
  if (!input_)
    return false;

  ZMFCollector collector(painter);
  
  switch (getVersion(input))
  {
	case ZMF2:
	  ZMF2Parser parser(input_, &collector);
	  break;
	case ZMF4:
	  ZMF4Parser parser(input_, &collector);
	  break;
	default:
	  return false;
  }

  return parser.parse();
}

catch (...)
{
  return false;
}

}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
