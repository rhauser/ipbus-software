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


#include <uhal/log/log_inserters.time.hpp>
#include <uhal/log/log_inserters.integer.hpp>

#include <uhal/log/log.hpp>


namespace uhal
{

  // Template specialization for printing the year field of a time struct as a 4 digit number
  template<>
  void TimeSpecializationHelper< year >::print ( const tm* aTm , const uint32_t& )
  {
    // the tm struct stores years as int number of years since 1900
    log_inserter ( Integer ( aTm->tm_year+1900 ) );
  }

  // Template specialization for printing the year field of a time struct as a 2 digit number
  template<>
  void TimeSpecializationHelper< yr >::print ( const tm* aTm , const uint32_t& )
  {
    static const char* lCharacterMapping (	"00010203040506070809"
                                            "10111213141516171819"
                                            "20212223242526272829"
                                            "30313233343536373839"
                                            "40414243444546474849"
                                            "50515253545556575859"
                                            "60616263646566676869"
                                            "70717273747576777879"
                                            "80818283848586878889"
                                            "90919293949596979899" );
    put ( lCharacterMapping + ( ( aTm->tm_year%100 ) <<1 ) , 2 );
  }

  // Template specialization for printing the month field of a time struct as a three character string
  template<>
  void TimeSpecializationHelper< strmth >::print ( const tm* aTm , const uint32_t& )
  {
    // the tm struct stores months as int with values 0 - 11 (not 1 - 12)
    static const char* lCharacterMapping ( "Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec" );
    put ( lCharacterMapping + ( aTm->tm_mon<<2 ) , 3 );
  }

  // Template specialization for printing the month field of a time struct as a 2 digit number
  template<>
  void TimeSpecializationHelper< mth >::print ( const tm* aTm , const uint32_t& )
  {
    // the tm struct stores months as int with values 0 - 11 (not 1 - 12)
    static const char* lCharacterMapping ( "010203040506070809101112" );
    put ( lCharacterMapping + ( aTm->tm_mon<<1 ) , 2 );
  }

  // Template specialization for printing the day field of a time struct as a 2 digit number
  template<>
  void TimeSpecializationHelper< day >::print ( const tm* aTm , const uint32_t& )
  {
    // the tm struct stores days as int with values 1 - 31 (so I add a dummy entry to start of LUT)
    static const char* lCharacterMapping (	"xx010203040506070809"
                                            "10111213141516171819"
                                            "20212223242526272829"
                                            "3031" );
    put ( lCharacterMapping + ( aTm->tm_mday<<1 ) , 2 );
  }

  // Template specialization for printing the hour field of a time struct as a 2 digit number
  template<>
  void TimeSpecializationHelper< hr >::print ( const tm* aTm , const uint32_t& )
  {
    // the tm struct stores hours as int with values 0-23
    static const char* lCharacterMapping (	"00010203040506070809"
                                            "10111213141516171819"
                                            "20212223" );
    put ( lCharacterMapping + ( aTm->tm_hour<<1 ) , 2 );
  }

  // Template specialization for printing the minute field of a time struct as a 2 digit number
  template<>
  void TimeSpecializationHelper< min >::print ( const tm* aTm , const uint32_t& )
  {
    // the tm struct stores minutes as int with values 0-59
    static const char* lCharacterMapping (	"00010203040506070809"
                                            "10111213141516171819"
                                            "20212223242526272829"
                                            "30313233343536373839"
                                            "40414243444546474849"
                                            "50515253545556575859" );
    put ( lCharacterMapping + ( aTm->tm_min<<1 ) , 2 );
  }

  // Template specialization for printing the second field of a time struct as a 2 digit number
  template<>
  void TimeSpecializationHelper< sec >::print ( const tm* aTm , const uint32_t& )
  {
    // the tm struct stores minutes as int with values 0-61 to take into account the double leap second
    static const char* lCharacterMapping (	"00010203040506070809"
                                            "10111213141516171819"
                                            "20212223242526272829"
                                            "30313233343536373839"
                                            "40414243444546474849"
                                            "50515253545556575859"
                                            "6061" );
    put ( lCharacterMapping + ( aTm->tm_sec<<1 ) , 2 );
  }

  template<>
  void TimeSpecializationHelper< usec >::print ( const tm* , const uint32_t& aUsec )
  {
    // the uSeconds are just a 32bit number so print it as an integer
    log_inserter ( Integer ( aUsec , IntFmt<dec , fixed , 6>() ) );
  }

  void log_inserter ( const _Time< timeval , TimeFmt<usec,' ',null,' ',null,' ',null,' ',null,' ',null,' ',null> >& aTime )
  {
    log_inserter ( Integer ( uint32_t ( aTime.value().tv_usec ) , IntFmt<dec , fixed , 6>() ) );
  }

  timeval Now()
  {
    timeval lTime;
    gettimeofday ( &lTime, NULL );
    return lTime;
  }

}