/*
---------------------------------------------------------------------------

    This file is part of uHAL.

    uHAL is a hardware access library and programming framework
    originally developed for upgrades of the Level-1 trigger of the CMS
    experiment at CERN.

    uHAL is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    uHAL is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with uHAL.  If not, see <http://www.gnu.org/licenses/>.


      Andrew Rose, Imperial College, London
      email: awr01 <AT> imperial.ac.uk

      Marc Magrans de Abril, CERN
      email: marc.magrans.de.abril <AT> cern.ch

---------------------------------------------------------------------------
*/

#include "uhal/ProtocolIPbusCore.hpp"

#include "boost/date_time/posix_time/posix_time.hpp"

#include <cstring>


std::ostream& operator<< ( std::ostream& aStr , const uhal::eIPbusTransactionType& aIPbusTransactionType )
{
  switch ( aIPbusTransactionType )
  {
    case uhal::B_O_T:
      aStr << "\"Byte Order Transaction\"";
      break;
    case uhal::R_A_I:
      aStr << "\"Reserved Address Information\"";
      break;
    case uhal::NI_READ:
      aStr << "\"Non-incrementing Read\"";
      break;
    case uhal::READ:
      aStr << "\"Incrementing Read\"";
      break;
    case uhal::NI_WRITE:
      aStr << "\"Non-incrementing Write\"";
      break;
    case uhal::WRITE:
      aStr << "\"Incrementing Write\"";
      break;
    case uhal::RMW_SUM:
      aStr << "\"Read-Modify-Write Sum\"";
      break;
    case uhal::RMW_BITS:
      aStr << "\"Read-Modify-Write Bits\"";
      break;
  }

  return aStr;
}


namespace uhal
{


  IPbusCore::IPbusCore ( const std::string& aId, const URI& aUri , const uint32_t& aMaxSendSize , const uint32_t& aMaxReplySize , const boost::posix_time::time_duration& aTimeoutPeriod ) :
    ClientInterface ( aId , aUri , aTimeoutPeriod ),
    mTransactionCounter ( 0x00000000 ),
    mMaxSendSize ( aMaxSendSize<<2 ),
    mMaxReplySize ( aMaxReplySize<<2 )
  {}


  IPbusCore::~IPbusCore()
  {}


  // void IPbusCore::preamble ( boost::shared_ptr< Buffers > lBuffers )
  // {
  //
  // log ( Debug() , "preamble" );
  // ByteOrderTransaction();
  // }




  // void IPbusCore::Dispatch( )
  // {
  //
  // log ( Debug() , "Dispatch" );

  // if ( lBuffers )
  // {
  // if ( lBuffers->sendCounter() )
  // {
  // predispatch ( boost::shared_ptr< Buffers > lBuffers );
  // mTransportProtocol->Dispatch ( lBuffers );
  // lBuffers = NULL; //Not deleting the underlying buffer, just flagging that we need a new buffer next time
  // mTransportProtocol->Flush();
  // }
  // }
  // }

  exception::exception* IPbusCore::validate ( uint8_t* aSendBufferStart ,
      uint8_t* aSendBufferEnd ,
      std::deque< std::pair< uint8_t* , uint32_t > >::iterator aReplyStartIt ,
      std::deque< std::pair< uint8_t* , uint32_t > >::iterator aReplyEndIt )
  {
    eIPbusTransactionType lSendIPbusTransactionType , lReplyIPbusTransactionType;
    uint32_t lSendWordCount , lReplyWordCount;
    uint32_t lSendTransactionId , lReplyTransactionId;
    uint8_t lSendResponseGood , lReplyResponseGood;

    do
    {
      if ( ! implementExtractHeader ( * ( ( uint32_t* ) ( aSendBufferStart ) ) , lSendIPbusTransactionType , lSendWordCount , lSendTransactionId , lSendResponseGood ) )
      {
        uhal::exception::IPbusCoreUnparsableTransactionHeader* lExc = new uhal::exception::IPbusCoreUnparsableTransactionHeader();
        log ( *lExc , "Unable to parse send header " , Integer ( * ( ( uint32_t* ) ( aSendBufferStart ) ), IntFmt< hex , fixed >() ) );
        return lExc;
      }

      if ( ! implementExtractHeader ( * ( ( uint32_t* ) ( aReplyStartIt->first ) ) , lReplyIPbusTransactionType , lReplyWordCount , lReplyTransactionId , lReplyResponseGood ) )
      {
        uhal::exception::IPbusCoreUnparsableTransactionHeader* lExc = new uhal::exception::IPbusCoreUnparsableTransactionHeader();
        log ( *lExc , "Unable to parse reply header " , Integer ( * ( ( uint32_t* ) ( aReplyStartIt->first ) ), IntFmt< hex , fixed >() ) );
        return lExc;
      }

      if ( lReplyResponseGood )
      {
        uhal::exception::IPbusCoreResponseCodeSet* lExc = new uhal::exception::IPbusCoreResponseCodeSet();
        log ( *lExc , "Returned Header, " , Integer ( * ( ( uint32_t* ) ( aReplyStartIt->first ) ), IntFmt< hex , fixed >() ),
              " ( transaction id = " , Integer ( lReplyTransactionId, IntFmt< hex , fixed >() ) ,
              ", transaction type = " , Integer ( ( uint8_t ) ( ( lReplyIPbusTransactionType >> 3 ) ), IntFmt< hex , fixed >() ) ,
              ", word count = " , Integer ( lReplyWordCount ) ,
              " ) had response field = " , Integer ( lReplyResponseGood, IntFmt< hex , fixed >() ) , " indicating an error" );
        return lExc;
      }

      if ( lSendIPbusTransactionType != lReplyIPbusTransactionType )
      {
        uhal::exception::IPbusTransactionTypeIncorrect* lExc = new uhal::exception::IPbusTransactionTypeIncorrect();
        log ( *lExc , "Returned Transaction Type " , lReplyIPbusTransactionType , " does not match that sent " , lSendIPbusTransactionType );
        return lExc;
      }

      if ( lSendTransactionId != lReplyTransactionId )
      {
        uhal::exception::IPbusTransactionIdIncorrect* lExc = new uhal::exception::IPbusTransactionIdIncorrect();
        log ( *lExc , "Sent Header was " , Integer ( * ( ( uint32_t* ) ( aSendBufferStart ) ) , IntFmt< hex , fixed >() ) ,
              " whilst Return Header was " , Integer ( * ( ( uint32_t* ) ( aReplyStartIt->first ) ) , IntFmt< hex , fixed >() ) );
        return lExc;
      }

      switch ( lSendIPbusTransactionType )
      {
        case B_O_T:
        case R_A_I:
          aSendBufferStart += ( 1<<2 );
          break;
        case NI_READ:
        case READ:
          aSendBufferStart += ( 2<<2 );
          break;
        case NI_WRITE:
        case WRITE:
          aSendBufferStart += ( ( 2+lSendWordCount ) <<2 );
          break;
        case RMW_SUM:
          aSendBufferStart += ( 3<<2 );
          break;
        case RMW_BITS:
          aSendBufferStart += ( 4<<2 );
          break;
      }

      switch ( lReplyIPbusTransactionType )
      {
        case B_O_T:
        case NI_WRITE:
        case WRITE:
          aReplyStartIt++;
          break;
        case R_A_I:
        case NI_READ:
        case READ:
        case RMW_SUM:
        case RMW_BITS:
          aReplyStartIt+=2;
          break;
      }
    }
    while ( ( aSendBufferEnd - aSendBufferStart != 0 ) && ( aReplyEndIt - aReplyStartIt != 0 ) );

    // log ( Info() , "IPbus has validated the packet paylod" );
    log ( Debug() , "Validation Complete!" );
    return NULL;
  }
  // ----------------------------------------------------------------------------------------------------------------------------------------------------------------




  // // ----------------------------------------------------------------------------------------------------------------------------------------------------------------
  // // NOTE! THIS FUNCTION MUST BE THREAD SAFE: THAT IS:
  // // IT MUST ONLY USE LOCAL VARIABLES
  // //            --- OR ---
  // // IT MUST MUTEX PROTECT ACCESS TO MEMBER VARIABLES!
  // // ----------------------------------------------------------------------------------------------------------------------------------------------------------------
  // bool IPbusCore::Validate ( boost::shared_ptr< Buffers > lBuffers )
  // {
  //
  // bool lRet = Validate ( lBuffers->getSendBuffer() ,
  // lBuffers->getSendBuffer() +lBuffers->sendCounter() ,
  // lBuffers->getReplyBuffer().begin() ,
  // lBuffers->getReplyBuffer().end() );

  // if ( lRet )
  // {
  // lBuffers->validate ( boost::shared_ptr< Buffers > lBuffers );
  // delete lBuffers; //We have now checked the returned data and marked as valid the underlying memory. We can, therefore, delete the local storage and from this point onward, the validated memory will only exist if the user kept their own copy
  // }

  // return lRet;
  // }

  // ----------------------------------------------------------------------------------------------------------------------------------------------------------------
  ValHeader IPbusCore::implementBOT()
  {
    log ( Debug() , "Byte Order Transaction" );
    // IPbus send packet format is:
    // HEADER
    uint32_t lSendByteCount ( 1 << 2 );
    // IPbus reply packet format is:
    // HEADER
    uint32_t lReplyByteCount ( 1 << 2 );
    uint32_t lSendBytesAvailable;
    uint32_t  lReplyBytesAvailable;
    boost::shared_ptr< Buffers > lBuffers = checkBufferSpace ( lSendByteCount , lReplyByteCount , lSendBytesAvailable , lReplyBytesAvailable );
    lBuffers->send ( implementCalculateHeader ( B_O_T , 0 , mTransactionCounter++ , requestTransactionInfoCode() ) );
    std::pair < ValHeader , _ValHeader_* > lReply ( CreateValHeader() );
    lReply.second->IPbusHeaders.push_back ( 0 );
    lBuffers->add ( lReply.first );
    lBuffers->receive ( lReply.second->IPbusHeaders.back() );
    return lReply.first;
  }
  //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



  //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  ValHeader IPbusCore::implementWrite ( const uint32_t& aAddr, const uint32_t& aSource )
  {
    log ( Debug() , "Write " , Integer ( aSource , IntFmt<hex,fixed>() ) , " to address " , Integer ( aAddr , IntFmt<hex,fixed>() ) );
    // IPbus packet format is:
    // HEADER
    // BASE ADDRESS
    // WORD
    uint32_t lSendByteCount ( 3 << 2 );
    // IPbus reply packet format is:
    // HEADER
    uint32_t lReplyByteCount ( 1 << 2 );
    uint32_t lSendBytesAvailable;
    uint32_t  lReplyBytesAvailable;
    boost::shared_ptr< Buffers > lBuffers = checkBufferSpace ( lSendByteCount , lReplyByteCount , lSendBytesAvailable , lReplyBytesAvailable );
    lBuffers->send ( implementCalculateHeader ( WRITE , 1 , mTransactionCounter++ , requestTransactionInfoCode()
                                              ) );
    lBuffers->send ( aAddr );
    lBuffers->send ( aSource );
    std::pair < ValHeader , _ValHeader_* > lReply ( CreateValHeader() );
    lReply.second->IPbusHeaders.push_back ( 0 );
    lBuffers->add ( lReply.first );
    lBuffers->receive ( lReply.second->IPbusHeaders.back() );
    return lReply.first;
  }

  ValHeader IPbusCore::implementWriteBlock ( const uint32_t& aAddr, const std::vector< uint32_t >& aSource, const defs::BlockReadWriteMode& aMode )
  {
    if ( aSource.size() == 0 )
    {
      log ( Warning() , "Zero-sized block transaction requested. uHAL has made the executive decision to ignore this: the instruction will not be sent and the returned header is >>assumed<< to be validated." );
      ValHeader lReply ( CreateValHeader().first );
      lReply.valid ( true );
      return lReply;
      //throw exception::IPbusCoreZeroSizeTransaction();
    }

    log ( Debug() , "Write block of size " , Integer ( aSource.size() ) , " to address " , Integer ( aAddr , IntFmt<hex,fixed>() ) );
    // IPbus packet format is:
    // HEADER
    // BASE ADDRESS
    // WORD
    // WORD
    // ....
    uint32_t lSendHeaderByteCount ( 2 << 2 );
    // IPbus reply packet format is:
    // HEADER
    uint32_t lReplyByteCount ( 1 << 2 );
    uint32_t lSendBytesAvailable;
    uint32_t  lReplyBytesAvailable;
    eIPbusTransactionType lType ( ( aMode == defs::INCREMENTAL ) ? WRITE : NI_WRITE );
    int32_t lPayloadByteCount ( aSource.size() << 2 );
    uint8_t* lSourcePtr ( ( uint8_t* ) ( & ( aSource.at ( 0 ) ) ) );
    uint32_t lAddr ( aAddr );
    std::pair < ValHeader , _ValHeader_* > lReply ( CreateValHeader() );
    boost::shared_ptr< Buffers > lBuffers;

    while ( lPayloadByteCount > 0 )
    {
      lBuffers = checkBufferSpace ( lSendHeaderByteCount+lPayloadByteCount , lReplyByteCount , lSendBytesAvailable , lReplyBytesAvailable );
      uint32_t lSendBytesAvailableForPayload ( std::min ( 4*getMaxTransactionWordCount(), lSendBytesAvailable - lSendHeaderByteCount ) & 0xFFFFFFFC );
      //      log ( Info() , std::string ( 100,'-' ) );
      //      log ( Info() , ThisLocation(), ", Buffer: " , Pointer ( & ( *lBuffers ) ) );
      //      log ( Info() , "lSendBytesAvailable: " , Integer ( lSendBytesAvailable )  ,
      //            " | lSendBytesAvailableForPayload (bytes): " , Integer ( lSendBytesAvailableForPayload )  ,
      //            " | lSendBytesAvailableForPayload (words): " , Integer ( lSendBytesAvailableForPayload>>2 ) ,
      //            " | lPayloadByteCount = " , Integer ( lPayloadByteCount ) );
      lBuffers->send ( implementCalculateHeader ( lType , lSendBytesAvailableForPayload>>2 , mTransactionCounter++ , requestTransactionInfoCode() ) );
      lBuffers->send ( lAddr );
      lBuffers->send ( lSourcePtr , lSendBytesAvailableForPayload );
      lSourcePtr += lSendBytesAvailableForPayload;
      lPayloadByteCount -= lSendBytesAvailableForPayload;

      if ( aMode == defs::INCREMENTAL )
      {
        lAddr += ( lSendBytesAvailableForPayload>>2 );
      }

      lReply.second->IPbusHeaders.push_back ( 0 );
      lBuffers->receive ( lReply.second->IPbusHeaders.back() );
    }

    lBuffers->add ( lReply.first ); //we store the valmem in the last chunk so that, if the reply is split over many chunks, the valmem is guaranteed to still exist when the other chunks come back...
    return lReply.first;
  }
  //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



  //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  ValWord< uint32_t > IPbusCore::implementRead ( const uint32_t& aAddr, const uint32_t& aMask )
  {
    log ( Debug() , "Read one unsigned word from address " , Integer ( aAddr , IntFmt<hex,fixed>() ) );
    // IPbus packet format is:
    // HEADER
    // BASE ADDRESS
    uint32_t lSendByteCount ( 2 << 2 );
    // IPbus reply packet format is:
    // HEADER
    // WORD
    uint32_t lReplyByteCount ( 2 << 2 );
    uint32_t lSendBytesAvailable;
    uint32_t  lReplyBytesAvailable;
    boost::shared_ptr< Buffers > lBuffers = checkBufferSpace ( lSendByteCount , lReplyByteCount , lSendBytesAvailable , lReplyBytesAvailable );
    lBuffers->send ( implementCalculateHeader ( READ , 1 , mTransactionCounter++ , requestTransactionInfoCode()
                                              ) );
    lBuffers->send ( aAddr );
    std::pair < ValWord<uint32_t> , _ValWord_<uint32_t>* > lReply ( CreateValWord ( 0 , aMask ) );
    lBuffers->add ( lReply.first );
    lReply.second->IPbusHeaders.push_back ( 0 );
    lBuffers->receive ( lReply.second->IPbusHeaders.back() );
    lBuffers->receive ( lReply.second->value );
    return lReply.first;
  }

  ValVector< uint32_t > IPbusCore::implementReadBlock ( const uint32_t& aAddr, const uint32_t& aSize, const defs::BlockReadWriteMode& aMode )
  {
    if ( aSize == 0 )
    {
      log ( Warning() , "Zero-sized block transaction requested. uHAL has made the executive decision to ignore this: the instruction will not be sent and the returned packet is >>assumed<< to be validated." );
      ValVector<uint32_t> lReply ( CreateValVector ( aSize ).first );
      lReply.valid ( true );
      return lReply;
      //       throw exception::IPbusCoreZeroSizeTransaction();
    }

    log ( Debug() , "Read unsigned block of size " , Integer ( aSize ) , " from address " , Integer ( aAddr , IntFmt<hex,fixed>() ) );
    // IPbus packet format is:
    // HEADER
    // BASE ADDRESS
    uint32_t lSendByteCount ( 2 << 2 );
    // IPbus reply packet format is:
    // HEADER
    // WORD
    // WORD
    // ....
    uint32_t lReplyHeaderByteCount ( 1 << 2 );
    uint32_t lSendBytesAvailable;
    uint32_t  lReplyBytesAvailable;
    std::pair < ValVector<uint32_t> , _ValVector_<uint32_t>* > lReply ( CreateValVector ( aSize ) );
    uint8_t* lReplyPtr = ( uint8_t* ) ( & ( lReply.second->value[0] ) );
    eIPbusTransactionType lType ( ( aMode == defs::INCREMENTAL ) ? READ : NI_READ );
    int32_t lPayloadByteCount ( aSize << 2 );
    uint32_t lAddr ( aAddr );
    boost::shared_ptr< Buffers > lBuffers;

    while ( lPayloadByteCount > 0 )
    {
      lBuffers = checkBufferSpace ( lSendByteCount , lReplyHeaderByteCount+lPayloadByteCount , lSendBytesAvailable , lReplyBytesAvailable );
      uint32_t lReplyBytesAvailableForPayload ( std::min ( 4*getMaxTransactionWordCount(), lReplyBytesAvailable - lReplyHeaderByteCount ) & 0xFFFFFFFC );
      lBuffers->send ( implementCalculateHeader ( lType , lReplyBytesAvailableForPayload>>2 , mTransactionCounter++ , requestTransactionInfoCode()
                                                ) );
      lBuffers->send ( lAddr );
      lReply.second->IPbusHeaders.push_back ( 0 );
      lBuffers->receive ( lReply.second->IPbusHeaders.back() );
      lBuffers->receive ( lReplyPtr , lReplyBytesAvailableForPayload );
      lReplyPtr += lReplyBytesAvailableForPayload;
      lPayloadByteCount -= lReplyBytesAvailableForPayload;

      if ( aMode == defs::INCREMENTAL )
      {
        lAddr += ( lReplyBytesAvailableForPayload>>2 );
      }
    }

    lBuffers->add ( lReply.first ); //we store the valmem in the last chunk so that, if the reply is split over many chunks, the valmem is guaranteed to still exist when the other chunks come back...
    return lReply.first;
  }
  //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


  //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  ValWord< uint32_t > IPbusCore::implementRMWbits ( const uint32_t& aAddr , const uint32_t& aANDterm , const uint32_t& aORterm )
  {
    log ( Debug() , "Read/Modify/Write bits (and=" , Integer ( aANDterm , IntFmt<hex,fixed>() ) , ", or=" , Integer ( aORterm , IntFmt<hex,fixed>() ) , ") from address " , Integer ( aAddr , IntFmt<hex,fixed>() ) );
    // IPbus packet format is:
    // HEADER
    // BASE ADDRESS
    // AND TERM
    // OR TERM
    uint32_t lSendByteCount ( 4 << 2 );
    // IPbus reply packet format is:
    // HEADER
    // WORD
    uint32_t lReplyByteCount ( 2 << 2 );
    uint32_t lSendBytesAvailable;
    uint32_t  lReplyBytesAvailable;
    boost::shared_ptr< Buffers > lBuffers = checkBufferSpace ( lSendByteCount , lReplyByteCount , lSendBytesAvailable , lReplyBytesAvailable );
    lBuffers->send ( implementCalculateHeader ( RMW_BITS , 1 , mTransactionCounter++ , requestTransactionInfoCode()
                                              ) );
    lBuffers->send ( aAddr );
    lBuffers->send ( aANDterm );
    lBuffers->send ( aORterm );
    std::pair < ValWord<uint32_t> , _ValWord_<uint32_t>* > lReply ( CreateValWord ( 0 ) );
    lBuffers->add ( lReply.first );
    lReply.second->IPbusHeaders.push_back ( 0 );
    lBuffers->receive ( lReply.second->IPbusHeaders.back() );
    lBuffers->receive ( lReply.second->value );
    return lReply.first;
  }
  //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


  //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  ValWord< uint32_t > IPbusCore::implementRMWsum ( const uint32_t& aAddr , const int32_t& aAddend )
  {
    log ( Debug() , "Read/Modify/Write sum (addend=" , Integer ( aAddend , IntFmt<hex,fixed>() ) , ") from address " , Integer ( aAddr , IntFmt<hex,fixed>() ) );			// IPbus packet format is:
    // HEADER
    // BASE ADDRESS
    // ADDEND
    uint32_t lSendByteCount ( 3 << 2 );
    // IPbus reply packet format is:
    // HEADER
    // WORD
    uint32_t lReplyByteCount ( 2 << 2 );
    uint32_t lSendBytesAvailable;
    uint32_t  lReplyBytesAvailable;
    boost::shared_ptr< Buffers > lBuffers = checkBufferSpace ( lSendByteCount , lReplyByteCount , lSendBytesAvailable , lReplyBytesAvailable );
    lBuffers->send ( implementCalculateHeader ( RMW_SUM , 1 , mTransactionCounter++ , requestTransactionInfoCode()
                                              ) );
    lBuffers->send ( aAddr );
    lBuffers->send ( static_cast< uint32_t > ( aAddend ) );
    std::pair < ValWord<uint32_t> , _ValWord_<uint32_t>* > lReply ( CreateValWord ( 0 ) );
    lBuffers->add ( lReply.first );
    lReply.second->IPbusHeaders.push_back ( 0 );
    lBuffers->receive ( lReply.second->IPbusHeaders.back() );
    lBuffers->receive ( lReply.second->value );
    return lReply.first;
  }
  //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------




  uint32_t IPbusCore::getMaxSendSize()
  {
    return mMaxSendSize;
  }

  uint32_t IPbusCore::getMaxReplySize()
  {
    return mMaxReplySize;
  }


  void IPbusCore::dispatchExceptionHandler()
  {
    log ( Info() , ThisLocation() );
    mTransactionCounter = 0;
    ClientInterface::dispatchExceptionHandler();
  }

}


