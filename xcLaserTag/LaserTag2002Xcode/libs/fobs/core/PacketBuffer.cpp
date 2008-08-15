/******************************************************************************
* FOBS C++ File
* Copyright (c) 2004 Omnividea Multimedia S.L
* Coded by José San Pedro Wandelmer
*
*    This file is part of FOBS.
*
*    FOBS is free software; you can redistribute it and/or modify
*    it under the terms of the GNU Lesser General Public License as
*    published by the Free Software Foundation; either version 2.1 
*    of the License, or (at your option) any later version.
*
*    FOBS is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU Lesser General Public License for more details.
*
*    You should have received a copy of the GNU Lesser General Public
*    License along with FOBS; if not, write to the Free Software
*    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
******************************************************************************/

extern "C"
{
#include <string.h>
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
}

#include "PacketBuffer.h"

const int PacketBuffer::maxPktNumber = 300;

struct PacketBuffer::Packet
{
    bool isAllocated;
    AVPacket pkt;
};


void PacketBuffer::dupPacket(AVPacket *pkt, int index)
{
    av_dup_packet(pkt);
    memcpy(&(buffer[index].pkt), pkt, sizeof(AVPacket));
}


PacketBuffer::PacketBuffer()
{
    buffer = new Packet[maxPktNumber];
    for(int i = 0; i < maxPktNumber; i++)
    {
        buffer[i].isAllocated = false;
    }
    index0=indexF=0;
    countV = 0;
}

PacketBuffer::~PacketBuffer()
{
    clear();
    delete[] buffer;
}

int PacketBuffer::count()
{
    return countV;
}

void PacketBuffer::clear()
{
    for(int i = 0; i < maxPktNumber; i++)
    {
        if(buffer[i].isAllocated) 
        {
            av_free_packet(&buffer[i].pkt);
            buffer[i].isAllocated = false;
        }
    }
    index0 = indexF = 0;
    countV = 0;
}

AVPacket* PacketBuffer::readNext()
{
    if(count() == 0) return NULL;
    AVPacket *pkt = &(buffer[index0].pkt);
    return pkt;
}

void PacketBuffer::deleteNext()
{
    AVPacket *pkt = extractNext();
    if(pkt == NULL) return;
    av_free_packet(pkt);
}

AVPacket* PacketBuffer::extractNext()
{
    if(count() == 0) return NULL;
    AVPacket *pkt = &(buffer[index0].pkt);
    buffer[index0].isAllocated = false;
    index0++;
    if(index0 == maxPktNumber) index0 = 0;
    countV--;
    return pkt;
}

bool PacketBuffer::append(AVPacket* pkt)
{
    bool res = false;
    if(count() == maxPktNumber)
    {
        av_free_packet(&(buffer[index0].pkt));
        dupPacket(pkt, index0);
        index0++;
        if(index0 == maxPktNumber) index0 = 0;
        indexF++;
        if(indexF == maxPktNumber) indexF = 0;
        res = true;
    }
    else
    {
        dupPacket(pkt, indexF);
        buffer[indexF].isAllocated = true;
        indexF++;
        if(indexF == maxPktNumber) indexF = 0;
        countV++;
    }
    return res;
}
bool PacketBuffer::prepend(AVPacket* pkt)
{
    bool res = false;
    if(count() != maxPktNumber)
    {
        if(index0 == 0) index0 = maxPktNumber -1;
        else index0--;
        //dupPacket(pkt, indexF);
        memcpy(&(buffer[index0].pkt), pkt, sizeof(AVPacket));
        buffer[index0].isAllocated = true;
        countV++;
    }    
    return res;

}