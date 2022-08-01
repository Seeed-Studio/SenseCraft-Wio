/*!
 * @file disk91_LoRaE5.h
 *
 * Disk91 LoRa E5 library, get a simpler access to LoRa-E5 module for your Arduino
 * developments
 * 
 * Written by Paul Pinault (Disk91.com)
 *
 * BSD license, all text here must be included in any redistribution.
 */
#include "disk91_LoRaE5.h"
#include <stdio.h>

// =============================================================================
// Helpers

void Disk91_LoRaE5::tracef(const char *format, ...) {
    #ifdef __DSKLORAE5_ENABLE
        if ( this->debugUart == NULL ) return;

        va_list args;
        char 	fmtBuffer[__DSKLORAE5_TRACE_MAX_BUF_SZ]; 
        va_start(args,format);
        vsnprintf(fmtBuffer,__DSKLORAE5_TRACE_MAX_BUF_SZ,format,args);
        va_end(args);
        this->debugUart->print(fmtBuffer);
    #endif
}

#ifndef DSKLORAE5_DISABLE_FSTR
void Disk91_LoRaE5::tracef(const __FlashStringHelper *format, ...) {
    #ifdef __DSKLORAE5_ENABLE
        if ( this->debugUart == NULL ) return;

        va_list args;
        char 	fmtBuffer[__DSKLORAE5_TRACE_MAX_BUF_SZ]; 
        va_start(args,format);
        vsnprintf_P(fmtBuffer,__DSKLORAE5_TRACE_MAX_BUF_SZ,format,args);
        va_end(args);
        this->debugUart->print(fmtBuffer);
    #endif
}
#else
  #define F(x) x
#endif

// compare str with a ref string and return true when
// str starts with ref. Case sensitive. ref can contain
// a joker char *
bool startsWith(const char * str, const char * ref) {
  if ( strlen(str) >= strlen(ref) ) {
    // possible 
    int i;
    for ( i = 0 ; i < strlen(ref) ; i++ ) {
        if ( ref[i] != '*' && str[i] != ref[i] ) {
                break;
        }
    }
    return ( i == strlen(ref) );
  }
  return false;
}

// search for index of char after the ref string in the str
// return -1 when not found
int indexOf(const char * str, const char * ref) {
  int sStr = strlen(str);
  int sRef = strlen(ref);
  int e;
  for ( int d = 0 ; d < (sStr - sRef) ; d++ ) {
    if ( str[d] == ref[0] ) {
      for ( e = 1 ; e < sRef ; e++ ) {
        if ( str[d+e] != ref[e] ) {
          break;
        }
      }
      if ( e == sRef ) {
        return d+e;
      }
    }
  }
  return -1;
}

// copy a hex string into a table
// returns the hexbyte in dst tab and the number of data in sz
// sz is also the max capacity of dst
// stops when non hexchar found
bool extractHexStr(const char * src, uint8_t * dst, uint8_t * sz) {
   uint8_t tNum = 0;
   uint8_t tSz = 0;
   for ( int i = 0 ; ; i++ ) {
    
     if ( src[i] >= '0' && src[i] <= '9' ) {
        tNum += src[i] - '0';
     } else if ( src[i] >= 'a' && src[i] <= 'f' ) {
        tNum += 10 + src[i] - 'a';
     } else if ( src[i] >= 'A' && src[i] <= 'F' ) {
        tNum += 10 + src[i] - 'A';
     } else break;

     if ( (i & 1) == 0 ) {
       // 1st digit
       tNum *= 16;
     } else {
        // 2nd digit - store
        dst[tSz] = tNum;
        tNum = 0;
        tSz++;
        if (tSz >= *sz) break;
     }
    
   }
   *sz = tSz;
   return true;
}

// copy a float number into dst string
bool extractNumber(const char * src, char *dst, int maxSz) {
    int idx = 0;
    for ( idx = 0 ; idx < maxSz-1 ; idx ++ ) {
       if ( src[idx] != ',' && src[idx] != ' ' && src[idx] != '\0' ) {
         dst[idx] = src[idx];
       } else {
         break;
       }
    }
    if ( idx < maxSz-1 ) {
       dst[idx] = '\0';  
       return true;
    }
    return false;
}


bool Disk91_LoRaE5::processRead( Disk91_LoRaE5 * wrap) {
    if ( startsWith(wrap->bufResponse,"+EEPROM: **, **") ) {
     int s = indexOf(wrap->bufResponse,"PROM: ");
     if ( s > 0 ) {
        uint8_t values[1];
        uint8_t sz = 1;
        if ( extractHexStr(&wrap->bufResponse[s+4], values,&sz) ) {
          wrap->bufInt8 = values[0];
          return true;
        }
     }
   }
   return false;
}

bool Disk91_LoRaE5::storeOneByte(uint8_t adr, uint8_t v) {
    char _cmd[128];
    sprintf(_cmd,"AT+EEPROM=%02X,%02X", adr, v);
    return sendATCommand(_cmd,"+EEPROM: ","","",this->atTimeout,false,NULL);     
}

bool Disk91_LoRaE5::readOneByte(uint8_t adr, uint8_t * v) {
    char _cmd[128];
    sprintf(_cmd,"AT+EEPROM=%02X",adr);
    if ( sendATCommand(_cmd,"+EEPROM: ","","",this->atTimeout,false,processRead) ) {
      *v = this->bufInt8;
      return true;          
    }
    return false;
}


// =============================================================================
// Persistance
struct s_persist {
    uint8_t version;
    // uint8_t deveui[8];
    // uint8_t appeui[8];
    // uint8_t appkey[16];
    char deveui[16];
    char appeui[16];
    char appkey[32];
    uint8_t zone;
    uint8_t recomp[6];
};

bool Disk91_LoRaE5::persistConfig(  // Store the LoRaWan configuration into module EEPROM for restoring later 
        uint8_t   zone,             // radio zone selection
        // uint8_t   deveui[],         // deviceEUI in the normal order for the bytes
        // uint8_t   appeui[],         // applicationEUI in the normal order for the bytes
        // uint8_t   appkey[]          // applicationKEY in the normal order for the bytes
        char   deveui[16],         // deviceEUI in the normal order for the bytes
        char   appeui[16],         // applicationEUI in the normal order for the bytes
        char   appkey[32]
) {
    bool ret = true;
    struct s_persist p;
    uint8_t * pp =  (uint8_t *)&p;
    p.version = 0x01;
    p.zone = zone;
    //bcopy(zone,p.zone,20);
    bcopy(deveui,p.deveui,8);
    bcopy(appeui,p.appeui,8);
    bcopy(appkey,p.appkey,16);
    // p.deveui = deveui;
    // p.appeui = appeui;
    // p.appkey = appkey;
    for (int i = 0; i < sizeof(struct s_persist); i++)
    {
      ret &= this->storeOneByte(i, pp[i]);
    }
    return ret;
}

bool Disk91_LoRaE5::clearStoredConfig( // Purge the stored configuration for E5
) {
    bool ret = true;
    for ( int i = 0 ; i < sizeof(struct s_persist) ; i++ ) {
        ret &= this->storeOneByte(i,0x00);
    }
    return ret;
}


bool Disk91_LoRaE5::haveStoredConfig( // Returns true when a configuration has already been stored in the E5 memory
) {
    bool ret = true;
    struct s_persist p;
    uint8_t * pp =  (uint8_t *)&p;
    for ( int i = 0 ; i < sizeof(struct s_persist) ; i++ ) {
        ret &= this->readOneByte(i,&pp[i]);
    }
    if ( !ret || p.version != 0x01 ) {
        return false;
    }
    return true;
}

// =============================================================================
// Constructor / Destructors

Disk91_LoRaE5::Disk91_LoRaE5(
        uint16_t  atTimeoutMs,               // Default timeout for the AT command processing
        Serial_ * logSerial                  // When set, the library debug is enabled               
){
    this->debugUart = logSerial;
    this->runningCommand = false;
    this->atTimeout = atTimeoutMs;
    this->currentZone = DSKLORAE5_ZONE_UNDEFINED;
    this->estimatedDCMs = 0;
}

Disk91_LoRaE5::Disk91_LoRaE5(
        Serial_ * logSerial                  // When set, the library debug is enabled               
){
    this->debugUart = logSerial;
    this->runningCommand = false;
    this->atTimeout = __DSKLORAE5_DEFAULT_AT_TMOUT;
    this->currentZone = DSKLORAE5_ZONE_UNDEFINED;
    this->estimatedDCMs = 0;
}

Disk91_LoRaE5::Disk91_LoRaE5(
    bool nothing                            // Strange complilation behavior, no param is not accepted
){
    this->debugUart = NULL;
    this->runningCommand = false;
    this->atTimeout = __DSKLORAE5_DEFAULT_AT_TMOUT;
    this->currentZone = DSKLORAE5_ZONE_UNDEFINED;
    this->estimatedDCMs = 0;
}


Disk91_LoRaE5::~Disk91_LoRaE5(){
      this->end();              
}

bool Disk91_LoRaE5::begin(  
        uint8_t portType,                   // where to find the LoRa-E5 board  
        __HWSERIAL_T   * hwSerial,          // for HWSERIAL_CUSTOM, link the associated Serial
        SoftwareSerial * swSerial,          // for SWSERIAL_CUTSOM, link the associated SoftwareSerial
        int16_t ssRxPort,                   // for SWSERIAL_PINS, specify the RX & TX Pin, the SSerial will be initialized
        int16_t ssTxPort
){
    switch ( portType ) {
        #ifdef __SERIAL
        case DSKLORAE5_HWSERIAL:
            this->e5Uart = &Serial;
            this->isHwSerial = true;
            break;
        #endif    
        #ifdef __SERIAL1
        case DSKLORAE5_HWSERIAL1:
            this->e5Uart = &Serial1;
            this->isHwSerial = true;
            break;
        #endif    
        #ifdef __SERIAL2
        case DSKLORAE5_HWSERIAL2:
            this->e5Uart = &Serial2;
            this->isHwSerial = true;
            break;
        #endif    
        case DSKLORAE5_HWSERIAL_CUSTOM:
            if ( hwSerial == NULL ) {
                this->tracef(F("LoRaE5 - invalid hw serial selection\r\n"));
                return false;
            }
            this->e5Uart = hwSerial;
            this->isHwSerial = true;
            break;
        case DSKLORAE5_SWSERIAL_CUSTOM:
            if ( swSerial == NULL ) {
                this->tracef(F("LoRaE5 - invalid sw serial selection\r\n"));
                return false;
            }
            this->e5SwUart = swSerial;
            this->isHwSerial = false;
            break;
        case DSKLORAE5_SWSERIAL_PINS:
            if ( ssRxPort == -1 || ssTxPort == -1 ) {
                this->tracef(F("LoRaE5 - invalid sw serial pins\r\n"));
                return false;
            }
            this->e5SwUart = new SoftwareSerial(ssRxPort, ssTxPort,false);
            this->isHwSerial = false;
            break;
        #ifdef DSKLORAE5_SWSERIAL_WIO_P1
        case DSKLORAE5_SWSERIAL_WIO_P1:
            this->e5SwUart = new SoftwareSerial(BCM3, BCM2,false);
            this->isHwSerial = false;
            break;
        #endif
        #ifdef DSKLORAE5_SWSERIAL_WIO_P2
        case DSKLORAE5_SWSERIAL_WIO_P2:
            this->e5SwUart = new SoftwareSerial(BCM27, BCM22,false);
            this->isHwSerial = false;
            break;
        #endif

        case DSKLORAE5_HWSEARCH:
            #ifdef __SERIAL
                this->e5Uart = &Serial;
                this->isHwSerial = true;
                if ( testPresence() ) break;
                this->e5Uart = NULL;
            #endif
            #ifdef __SERIAL1
                this->e5Uart = &Serial1;
                this->isHwSerial = true;
                if ( testPresence() ) break;
                this->e5Uart = NULL;
            #endif
            #ifdef __SERIAL2
                this->e5Uart = &Serial2;
                this->isHwSerial = true;
                if ( testPresence() ) break;
                this->e5Uart = NULL;
            #endif
            this->tracef(F("LoRaE5 - not found\r\n"));
            return false;
            break;

        #ifdef DSKLORAE5_SEARCH_WIO
        case DSKLORAE5_SEARCH_WIO:
            #ifdef __SERIAL1
                this->e5Uart = &Serial1;
                this->isHwSerial = true;
                if ( testPresence() ) break;
                this->e5Uart = NULL;
            #endif
            #ifdef __SERIAL2
                this->e5Uart = &Serial2;
                this->isHwSerial = true;
                if ( testPresence() ) break;
                this->e5Uart = NULL;
            #endif
            #ifdef DSKLORAE5_SWSERIAL_WIO_P1
                this->e5SwUart = new SoftwareSerial(BCM3, BCM2,false);
                this->isHwSerial = false;
                if ( testPresence() ) break;
                delete this->e5SwUart;
                this->e5SwUart = NULL;
            #endif
            #ifdef DSKLORAE5_SWSERIAL_WIO_P2
                this->e5SwUart = new SoftwareSerial(BCM27, BCM22,false);
                this->isHwSerial = false;
                if ( testPresence() ) break;
                delete this->e5SwUart;
                this->e5SwUart = NULL;
            #endif
            this->tracef(F("LoRaE5 - not found\r\n"));
            return false;
            break;
        #endif
    }
    // Init Serial
    uint32_t start = millis();
    if ( this->isHwSerial ) {
        this->e5Uart->begin(9600);
        while ( !(*this->e5Uart) && (millis() - start) < 1000 );
    } else {
        this->e5SwUart->begin(9600);
        while ( !(*this->e5SwUart) && (millis() - start) < 1000 );
    }
    if ( (millis() - start) >= 1000 ) {
        this->tracef(F("LoRaE5 - Impossible to connect Serial\r\n"));
        return false;
    }
    // Init contexte
    this->runningCommand = false;
    this->hasJoined = false;
    this->isJoining = false;
    this->lastSf = __DSKLORAE5_UNSET;
    this->lastPower = __DSKLORAE5_UNSET_POWER;
    this->lastRetry = 0;
    this->currentSeqId = 1;
    this->downlinkPending = false;

    // Verify module response
    this->runningCommand = false;
    if ( !sendATCommand("AT","+AT: OK","","",1000,false,NULL) ) {
        if ( !this->isHwSerial ) {
            this->e5SwUart->end();
        }
        return false;
    }
    
    this->tracef(F("LoRaE5 - initialization OK\r\n"));
    return true;
}

void Disk91_LoRaE5::end() {
    if ( this->e5SwUart != NULL ) {
        delete this->e5SwUart;
    }
}

// =============================================================================
// Setup & test
bool Disk91_LoRaE5::testPresence() {
    uint32_t start = millis();
    if ( this->isHwSerial ) {
        this->e5Uart->begin(9600);
        while ( !(*this->e5Uart) && (millis() - start) < 1000 );
    } else {
        this->e5SwUart->begin(9600);
        while ( !(*this->e5SwUart) && (millis() - start) < 1000 );
    }
    if ( (millis() - start) >= 1000 ) return false;
    this->runningCommand = false;
    if ( !sendATCommand("AT","+AT: OK","","",this->atTimeout,false,NULL) ) {
        // retry
        if ( !sendATCommand("AT","+AT: OK","","",this->atTimeout,false,NULL) ) {
            return false;
        }
    }
    if ( !this->isHwSerial ) {
        this->e5SwUart->end();
    }
    return true;
}

bool Disk91_LoRaE5::setup(  // Setup the LoRaWAN stack with the stored credentials
    bool      selfDC,       // when true, the duty cycle management is not managed by the module but the user application
    bool      withADR       // when true, the ADR is turned ON
) {
    bool ret = true;
    struct s_persist p;
    uint8_t * pp =  (uint8_t *)&p;
    for ( int i = 0 ; i < sizeof(struct s_persist) ; i++ ) {
        ret &= this->readOneByte(i,&pp[i]);
    }
    if ( !ret ) {
        this->tracef(F("LoRaE5 - Failed to read credentials"));
    }
    if ( p.version != 0x01 ) return false; // basic control, can be improved
    return this->setup(
        p.zone,
        p.deveui,
        p.appeui,
        p.appkey,
        selfDC,
        withADR
    );
}


bool Disk91_LoRaE5::setup(  // Setup the LoRaWAN stack
    uint8_t   zone,         // radio zone selection
    // uint8_t   deveui[],     // deviceEUI in the normal order for the bytes
    // uint8_t   appeui[],     // applicationEUI in the normal order for the bytes
    // uint8_t   appkey[],     // applicationKEY in the normal order for the bytes
    char   deveui[],     // deviceEUI in the normal order for the bytes
    char   appeui[],     // applicationEUI in the normal order for the bytes
    char   appkey[],     // applicationKEY in the normal order for the bytes
    bool      selfDC,       // when true, the duty cycle management is not managed by the module but the user application
    bool      withADR       // when true, the ADR is turned ON
) {
    char _cmd[128];
    bool ret = true;
    sendATCommand("AT+UART=TIMEOUT,0","+UART: TIMEOUT","","",this->atTimeout,false, NULL);
    // Setup region
    if ( this->currentZone != zone  ) { 
        this->currentZone = zone;

        if ( zone == DSKLORAE5_ZONE_EU868 ) {
            ret &= sendATCommand("AT+DR=EU868","+DR: EU868","+DR: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=3,867.1,0,5","+CH: 3,8671","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=4,867.3,0,5","+CH: 4,8673","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=5,867.5,0,5","+CH: 5,8675","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=6,867.7,0,5","+CH: 6,8677","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=7,867.9,0,5","+CH: 7,8679","+CH: ERR","",this->atTimeout,false,NULL);
            if ( selfDC ) {
                ret &= sendATCommand("AT+LW=DC,OFF","+LW: DC, OFF","+LW: ERR","",this->atTimeout,false,NULL); // manually managed to avoid conflicts
                ret &= sendATCommand("AT+LW=JDC,OFF","+LW: JDC, OFF","+LW: ERR","",this->atTimeout,false,NULL); // manually managed to avoid conflicts  
            }
        } else if ( zone == DSKLORAE5_ZONE_US915 ) {
            ret &= sendATCommand("AT+DR=US915","+DR: US915","+DR: ERR","",this->atTimeout,false,NULL);
            // unvalidate the subband other than 2
            for ( int i=0 ; i < 72 ; i++ ) {
                if ( i < 8 || i > 15 ) {
                    sprintf(_cmd,"AT+CH=%d,OFF",i);
                    ret &= sendATCommand(_cmd,"+CH: CH","+CH: ERR","",this->atTimeout,false,NULL);  
                }
            }
        } else if ( zone == DSKLORAE5_ZONE_AS923_1 ) {
            /*  According to https://github.com/helium/router
                923.6 MHz
                923.8 MHz
                924.0 MHz
                924.2 MHz
                924.4 MHz
            */
            ret &= sendATCommand("AT+DR=AS923","+DR: AS923","+DR: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=0,923.2,0,5","+CH: 0,9232","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=1,923.4,0,5","+CH: 1,9234","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=2,923.6,0,5","+CH: 2,9236","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=3,923.8,0,5","+CH: 3,9238","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=4,924.0,0,5","+CH: 4,9240","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=5,924.2,0,5","+CH: 5,9242","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=6,924.4,0,5","+CH: 6,9244","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+RXWIN2=923.2,DR2","+RXWIN2: 9232","+RXWIN2: ERR","",this->atTimeout,false,NULL);
        } else if ( zone == DSKLORAE5_ZONE_AS923_2 ) {
            ret &= sendATCommand("AT+DR=AS923","+DR: AS923","+DR: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=0,921.4,0,5","+CH: 0,9214","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=1,921.6,0,5","+CH: 1,9216","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=2,921.8,0,5","+CH: 2,9218","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=3,922.0,0,5","+CH: 3,9220","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=4,922.2,0,5","+CH: 4,9222","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=5,922.4,0,5","+CH: 5,9224","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=6,922.6,0,5","+CH: 6,9226","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+RXWIN2=921.4,DR2","+RXWIN2: 9214","+RXWIN2: ERR","",this->atTimeout,false,NULL);
          } else if ( zone == DSKLORAE5_ZONE_AS923_3 ) {
            ret &= sendATCommand("AT+DR=AS923","+DR: AS923","+DR: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=0,916.6,0,5","+CH: 0,9166","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=1,916.8,0,5","+CH: 1,9168","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=2,917.0,0,5","+CH: 2,9170","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=3,917.2,0,5","+CH: 3,9172","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=4,917.4,0,5","+CH: 4,9174","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=5,917.6,0,5","+CH: 5,9176","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=6,917.8,0,5","+CH: 6,9178","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+RXWIN2=916.6,DR2","+RXWIN2: 9166","+RXWIN2: ERR","",this->atTimeout,false,NULL);
        } else if ( zone == DSKLORAE5_ZONE_AS923_4 ) {
            ret &= sendATCommand("AT+DR=AS923","+DR: AS923","+DR: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=0,917.3,0,5","+CH: 0,9173","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=1,917.5,0,5","+CH: 1,9175","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=2,917.7,0,5","+CH: 2,9177","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=3,917.9,0,5","+CH: 3,9179","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=4,918.1,0,5","+CH: 4,9181","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=5,918.3,0,5","+CH: 5,9183","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=6,918.5,0,5","+CH: 6,9185","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+RXWIN2=917.3,DR2","+RXWIN2: 9173","+RXWIN2: ERR","",this->atTimeout,false,NULL);
        } else if ( zone == DSKLORAE5_ZONE_KR920 ) {
            ret &= sendATCommand("AT+DR=KR920","+DR: KR920","+DR: ERR","",this->atTimeout,false,NULL);
        } else if ( zone == DSKLORAE5_ZONE_IN865 ) {
            ret &= sendATCommand("AT+DR=IN865","+DR: IN865","+DR: ERR","",this->atTimeout,false,NULL);
        } else if ( zone == DSKLORAE5_ZONE_AU915 ) {
            ret &= sendATCommand("AT+DR=AU915","+DR: AU915","+DR: ERR","",this->atTimeout,false,NULL);  
            // unvalidate the subband other than 2
            for ( int i=0 ; i < 72 ; i++ ) {
                if ( i < 8 || i > 15 ) {
                    sprintf(_cmd,"AT+CH=%d,OFF",i);
                    ret &= sendATCommand(_cmd,"+CH: CH","+CH: ERR","",this->atTimeout,false,NULL);  
                }
            }
        } else {
            this->tracef(F("LoRaE5 - Invalid zone setting\r\n"));
            return false;
        }
    }
    if ( !ret ) {
        this->tracef(F("LoRaE5 - Failed to configure zone\r\n"));
        return false;
    }
    if ( ! withADR ) {
        ret &= sendATCommand("AT+ADR=OFF","+ADR: OFF","+ADR: ON","",this->atTimeout,false,NULL);
    } else {
        ret &= sendATCommand("AT+ADR=ON","+ADR: ON","+ADR: OFF","",this->atTimeout,false,NULL);
    }

    // Setup Ids
    // sprintf(_cmd,"AT+ID=DevEUI,%02X%02X%02X%02X%02X%02X%02X%02X",
    //   deveui[0],
    //   deveui[1],
    //   deveui[2],
    //   deveui[3],
    //   deveui[4],
    //   deveui[5],
    //   deveui[6],
    //   deveui[7]
    // );
    sprintf(_cmd, "AT+ID=DevEUI,%s", deveui);
    ret &= sendATCommand(_cmd, "+ID: DevEui", "+ID: ERR", "", this->atTimeout, false, NULL);

    // sprintf(_cmd,"AT+ID=AppEUI,%02X%02X%02X%02X%02X%02X%02X%02X",
    //   appeui[0],
    //   appeui[1],
    //   appeui[2],
    //   appeui[3],
    //   appeui[4],
    //   appeui[5],
    //   appeui[6],
    //   appeui[7]
    // );
    sprintf(_cmd, "AT+ID=AppEUI,%s", appeui);
    ret &= sendATCommand(_cmd, "+ID: AppEui", "+ID: ERR", "", this->atTimeout, false, NULL);

    // sprintf(_cmd,"AT+KEY=APPKEY,%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
    //   appkey[0],
    //   appkey[1],
    //   appkey[2],
    //   appkey[3],
    //   appkey[4],
    //   appkey[5],
    //   appkey[6],
    //   appkey[7],
    //   appkey[8],
    //   appkey[9],
    //   appkey[10],
    //   appkey[11],
    //   appkey[12],
    //   appkey[13],
    //   appkey[14],
    //   appkey[15]
    // );
    sprintf(_cmd, "AT+KEY=APPKEY,%s", appkey);
    ret &= sendATCommand(_cmd,"+KEY: APPKEY","+KEY: ERR","",this->atTimeout,false,NULL);
    ret &= sendATCommand("AT+MODE=LWOTAA","+MODE: LWOTAA","+MODE: ERR","",this->atTimeout,false,NULL);    
    if ( !ret ) {
        this->tracef(F("LoRaE5 - Failed to configure credentials\r\n"));
        return false;
    }
    this->tracef(F("LoRaE5 - setup OK\r\n"));
    return true;
}

bool Disk91_LoRaE5::setup_sensecap(  // Setup the LoRaWAN stack
    uint8_t   zone,         // radio zone selection
    bool      selfDC,       // when true, the duty cycle management is not managed by the module but the user application
    bool      withADR       // when true, the ADR is turned ON
) {
    char _cmd[128];
    bool ret = true;
    sendATCommand("AT+UART=TIMEOUT,0","+UART: TIMEOUT","","",this->atTimeout,false, NULL);
    // Setup region
    if ( this->currentZone != zone  ) { 
        this->currentZone = zone;

        if ( zone == DSKLORAE5_ZONE_EU868 ) {
            ret &= sendATCommand("AT+DR=EU868","+DR: EU868","+DR: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=3,867.1,0,5","+CH: 3,8671","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=4,867.3,0,5","+CH: 4,8673","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=5,867.5,0,5","+CH: 5,8675","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=6,867.7,0,5","+CH: 6,8677","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=7,867.9,0,5","+CH: 7,8679","+CH: ERR","",this->atTimeout,false,NULL);
            if ( selfDC ) {
                ret &= sendATCommand("AT+LW=DC,OFF","+LW: DC, OFF","+LW: ERR","",this->atTimeout,false,NULL); // manually managed to avoid conflicts
                ret &= sendATCommand("AT+LW=JDC,OFF","+LW: JDC, OFF","+LW: ERR","",this->atTimeout,false,NULL); // manually managed to avoid conflicts  
            }
        } else if ( zone == DSKLORAE5_ZONE_US915 ) {
            ret &= sendATCommand("AT+DR=US915","+DR: US915","+DR: ERR","",this->atTimeout,false,NULL);
            // unvalidate the subband other than 2
            for ( int i=0 ; i < 72 ; i++ ) {
                if ( i < 8 || i > 15 ) {
                    sprintf(_cmd,"AT+CH=%d,OFF",i);
                    ret &= sendATCommand(_cmd,"+CH: CH","+CH: ERR","",this->atTimeout,false,NULL);  
                }
            }
        } else if ( zone == DSKLORAE5_ZONE_AS923_1 ) {
            /*  According to https://github.com/helium/router
                923.6 MHz
                923.8 MHz
                924.0 MHz
                924.2 MHz
                924.4 MHz
            */
            ret &= sendATCommand("AT+DR=AS923","+DR: AS923","+DR: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=0,923.2,0,5","+CH: 0,9232","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=1,923.4,0,5","+CH: 1,9234","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=2,923.6,0,5","+CH: 2,9236","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=3,923.8,0,5","+CH: 3,9238","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=4,924.0,0,5","+CH: 4,9240","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=5,924.2,0,5","+CH: 5,9242","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=6,924.4,0,5","+CH: 6,9244","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+RXWIN2=923.2,DR2","+RXWIN2: 9232","+RXWIN2: ERR","",this->atTimeout,false,NULL);
        } else if ( zone == DSKLORAE5_ZONE_AS923_2 ) {
            ret &= sendATCommand("AT+DR=AS923","+DR: AS923","+DR: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=0,921.4,0,5","+CH: 0,9214","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=1,921.6,0,5","+CH: 1,9216","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=2,921.8,0,5","+CH: 2,9218","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=3,922.0,0,5","+CH: 3,9220","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=4,922.2,0,5","+CH: 4,9222","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=5,922.4,0,5","+CH: 5,9224","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=6,922.6,0,5","+CH: 6,9226","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+RXWIN2=921.4,DR2","+RXWIN2: 9214","+RXWIN2: ERR","",this->atTimeout,false,NULL);
          } else if ( zone == DSKLORAE5_ZONE_AS923_3 ) {
            ret &= sendATCommand("AT+DR=AS923","+DR: AS923","+DR: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=0,916.6,0,5","+CH: 0,9166","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=1,916.8,0,5","+CH: 1,9168","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=2,917.0,0,5","+CH: 2,9170","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=3,917.2,0,5","+CH: 3,9172","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=4,917.4,0,5","+CH: 4,9174","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=5,917.6,0,5","+CH: 5,9176","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=6,917.8,0,5","+CH: 6,9178","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+RXWIN2=916.6,DR2","+RXWIN2: 9166","+RXWIN2: ERR","",this->atTimeout,false,NULL);
        } else if ( zone == DSKLORAE5_ZONE_AS923_4 ) {
            ret &= sendATCommand("AT+DR=AS923","+DR: AS923","+DR: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=0,917.3,0,5","+CH: 0,9173","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=1,917.5,0,5","+CH: 1,9175","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=2,917.7,0,5","+CH: 2,9177","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=3,917.9,0,5","+CH: 3,9179","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=4,918.1,0,5","+CH: 4,9181","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=5,918.3,0,5","+CH: 5,9183","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+CH=6,918.5,0,5","+CH: 6,9185","+CH: ERR","",this->atTimeout,false,NULL);
            ret &= sendATCommand("AT+RXWIN2=917.3,DR2","+RXWIN2: 9173","+RXWIN2: ERR","",this->atTimeout,false,NULL);
        } else if ( zone == DSKLORAE5_ZONE_KR920 ) {
            ret &= sendATCommand("AT+DR=KR920","+DR: KR920","+DR: ERR","",this->atTimeout,false,NULL);
        } else if ( zone == DSKLORAE5_ZONE_IN865 ) {
            ret &= sendATCommand("AT+DR=IN865","+DR: IN865","+DR: ERR","",this->atTimeout,false,NULL);
        } else if ( zone == DSKLORAE5_ZONE_AU915 ) {
            ret &= sendATCommand("AT+DR=AU915","+DR: AU915","+DR: ERR","",this->atTimeout,false,NULL);  
            // unvalidate the subband other than 2
            for ( int i=0 ; i < 72 ; i++ ) {
                if ( i < 8 || i > 15 ) {
                    sprintf(_cmd,"AT+CH=%d,OFF",i);
                    ret &= sendATCommand(_cmd,"+CH: CH","+CH: ERR","",this->atTimeout,false,NULL);  
                }
            }
        } else {
            this->tracef(F("LoRaE5 - Invalid zone setting\r\n"));
            return false;
        }
    }
    if ( !ret ) {
        this->tracef(F("LoRaE5 - Failed to configure zone\r\n"));
        return false;
    }
    if ( ! withADR ) {
        ret &= sendATCommand("AT+ADR=OFF","+ADR: OFF","+ADR: ON","",this->atTimeout,false,NULL);
    } else {
        ret &= sendATCommand("AT+ADR=ON","+ADR: ON","+ADR: OFF","",this->atTimeout,false,NULL);
    }

    ret &= sendATCommand("AT+MODE=LWOTAA","+MODE: LWOTAA","+MODE: ERR","",this->atTimeout,false,NULL);    
    if ( !ret ) {
        this->tracef(F("LoRaE5 - Failed to configure credentials\r\n"));
        return false;
    }
    this->tracef(F("LoRaE5 - setup OK\r\n"));
    return true;
}

// =============================================================================
// Send Data

bool Disk91_LoRaE5::sendReceive_sync(  // send a message on LoRaWan expert an ack at least, eventually a downlink, return true when sent is a success and expect a ack
        uint8_t     port,               // LoRaWan port
        uint8_t *   data,               // Data / payload to be transmitted
        uint8_t     sz,                 // Size of the data, when 0 Join only is proceeded
        uint8_t *   rxBuffer,           // Downlink buffer, make sure it will be large enought, no verification
        uint8_t *   rxSize,             // uint8_t containing the rxBuffer size and returning the downlink message size
        uint8_t *   rxPort,             // uint8_t pointer for returnin the downlink port
        uint8_t     sf,                 // Spread Factor , use DSKLORAE5_SF_UNCHANGED to keep the previous one
        uint8_t     pwr,                // Transmission power, use DSKLORAE5_DW_UNCHANGED to keep the previous one
        uint8_t     retries             // Number of retry, use DSKLORAE5_RT_UNCHANGED to keep the previous one. retry = 0 means 1 uplink, no retry
){
    this->sendReceive( port, data, sz, true, NULL, rxBuffer, rxSize, rxPort, sf, pwr, retries, false );
}


bool Disk91_LoRaE5::send_sync(    // send a message on LoRaWan, return true when sent is a success 
        uint8_t     port,               // LoRaWan port
        uint8_t *   data,               // Data / payload to be transmitted
        uint8_t     sz,                 // Size of the data, when 0 Join only is proceeded
        bool        acked,              // Ack / Downlink request
        uint8_t     sf,                 // Spread Factor , use DSKLORAE5_SF_UNCHANGED to keep the previous one
        uint8_t     pwr,                // Transmission power, use DSKLORAE5_DW_UNCHANGED to keep the previous one
        uint8_t     retries             // Number of retry, use DSKLORAE5_RT_UNCHANGED to keep the previous one. retry = 0 means 1 uplink, no retry
){
    this->sendReceive( port, data, sz, acked, NULL, NULL, NULL, NULL, sf, pwr, retries, false );
}

bool Disk91_LoRaE5::join_sync(    // send a message on LoRaWan, return true when sent is a success 
        uint8_t     sf,             // Spread Factor , use DSKLORAE5_SF_UNCHANGED to keep the previous one
        uint8_t     pwr             // Transmission power, use DSKLORAE5_DW_UNCHANGED to keep the previous one
){
    this->sendReceive( 0, NULL, 0, false, NULL, NULL, NULL, NULL, sf, pwr, 0, false );
}


bool Disk91_LoRaE5::sendReceive(    // send a message on LoRaWan, return true when sent is a success 
    uint8_t     port,               // LoRaWan port
    uint8_t *   data,               // Data / payload to be transmitted
    uint8_t     sz,                 // Size of the data, when 0 Join only is proceeded
    bool        acked,              // Ack / Downlink request
                                    // Downlink Callback, return size of downlink, 0 when ack, -1 for none, as a parameter the buffer 
    int8_t      (*rxCallback)(uint8_t *),
    uint8_t *   rxBuffer,           // Downlink buffer, make sure it will be large enought, no verification
    uint8_t *   rxSize,             // uint8_t containing the rxBuffer size and returning the downlink message size
    uint8_t *   rxPort,             // uint8_t pointer for returnin the downlink port
    uint8_t     sf,                 // Spread Factor , use DSKLORAE5_SF_UNCHANGED to keep the previous one
    uint8_t     pwr,                // Transmission power, use DSKLORAE5_DW_UNCHANGED to keep the previous one
    uint8_t     retries,            // Number of retry, use DSKLORAE5_RT_UNCHANGED to keep the previous one. retry = 0 means 1 uplink, no retry
    bool        async               // When true, the processing is made synchronously
) {
  char _cmd[128];

  if ( sz == 0 && this->hasJoined && !acked ) return true; // basicaly nothing to do
  if ( this->runningCommand ) return false; // re-entering

  if ( pwr != DSKLORAE5_DW_UNCHANGED && this->lastPower != pwr ) {
    // set power (E5 automatically set to max if higher than max allowed)
    sprintf(_cmd,"AT+POWER=%d",pwr);
    if ( sendATCommand(_cmd,"+POWER:","+POWER: ERR","",this->atTimeout,false,NULL) ) {
      this->lastPower = pwr;
    } else {
      this->tracef(F("LoRaE5 - Failed to set power\r\n"));
      return false;
    }
  }
  if ( this->lastPower == __DSKLORAE5_UNSET_POWER ) {
      this->tracef(F("LoRaE5 - Tx Power not set\r\n"));
      return false;
  }

  if ( sf != DSKLORAE5_SF_UNCHANGED && this->lastSf != sf ) {
    // set sf 
    boolean retDr = true;
    if ( this->currentZone == DSKLORAE5_ZONE_EU868 
      || this->currentZone == DSKLORAE5_ZONE_AS923_1 || this->currentZone == DSKLORAE5_ZONE_AS923_2 || this->currentZone == DSKLORAE5_ZONE_AS923_3 || this->currentZone == DSKLORAE5_ZONE_AS923_4
      || this->currentZone == DSKLORAE5_ZONE_KR920 || this->currentZone == DSKLORAE5_ZONE_IN865 || this->currentZone == DSKLORAE5_ZONE_AU915 ) {
      // DR0 - SF12 / DR5 - SF7
      switch (sf) {
        case 7:
             retDr = sendATCommand("AT+DR=DR5","+DR: ***** DR5","+DR: ERR","",this->atTimeout,false,NULL);
             break;
        case 8:
             retDr = sendATCommand("AT+DR=DR4","+DR: ***** DR4","+DR: ERR","",this->atTimeout,false,NULL);
             break;
        case 9:
             retDr = sendATCommand("AT+DR=DR3","+DR: ***** DR3","+DR: ERR","",this->atTimeout,false,NULL);
             break;
        case 10:
             retDr = sendATCommand("AT+DR=DR2","+DR: ***** DR2","+DR: ERR","",this->atTimeout,false,NULL);
             break;
        case 11:
             retDr = sendATCommand("AT+DR=DR1","+DR: ***** DR1","+DR: ERR","",this->atTimeout,false,NULL);
             break;
        case 12:
             retDr = sendATCommand("AT+DR=DR0","+DR: ***** DR0","+DR: ERR","",this->atTimeout,false,NULL);
             break;
        default:
             this->tracef(F("LoRaE5 - Invalid SF\r\n"));
             return false;
      }
    } else if ( this->currentZone == DSKLORAE5_ZONE_US915 ) {
      // DR0 - SF10 / DR3 - SF7
      switch (sf) {
        case 7:
             retDr = sendATCommand("AT+DR=DR3","+DR: ***** DR3","+DR: ERR","",this->atTimeout,false,NULL);
             break;
        case 8:
             retDr = sendATCommand("AT+DR=DR2","+DR: ***** DR2","+DR: ERR","",this->atTimeout,false,NULL);
             break;
        case 9:
             retDr = sendATCommand("AT+DR=DR1","+DR: ***** DR1","+DR: ERR","",this->atTimeout,false,NULL);
             break;
        case 10:
             retDr = sendATCommand("AT+DR=DR0","+DR: ***** DR0","+DR: ERR","",this->atTimeout,false,NULL);
             break;
        default:
             this->tracef(F("LoRaE5 - Invalid SF\r\n"));
             return false;
      }
    } else {
      retDr = false;
    }
    if ( ! retDr ) {
           this->tracef(F("LoRaE5 - Impossible to set SF\r\n"));
           return false;       
    } else {
      this->lastSf = sf;
    }
  }
  if ( this->lastSf == __DSKLORAE5_UNSET ) {
      this->tracef(F("LoRaE5 - Invalid SF\r\n"));
      return false;
  }


  if ( retries != DSKLORAE5_RT_UNCHANGED && this->lastRetry != retries ) {
    // set retries
    sprintf(_cmd,"AT+RETRY=%d",retries+1);
    if ( sendATCommand(_cmd,"+RETRY:","+RETRY: ERR","",this->atTimeout,false,NULL) ) {
      this->lastRetry = retries;
    } else {
      this->tracef(F("LoRaE5 - Invalid Retry\r\n"));
      return false;
    }
  }

  // Set Pport
  sprintf(_cmd,"AT+PORT=%d",port);
  if ( !sendATCommand(_cmd,"+PORT:","+PORT: ERR","",this->atTimeout,false,NULL) ) {
     this->tracef(F("LoRaE5 - Port config refused\r\n"));
     return false;
  }

  
  this->lastSendMs = millis();
  if ( ! this->hasJoined ) {
    // we first need to join the network
    // make it simple, the first frame will be lost during join
    // 1% based on SF and data size (24 Bytes)
    // @TODO also consider ack
    this->estimatedDCMs += 100*this->estimateTxDuration(this->lastSf,__DSKLORAE5_JOINREQ_PAYLOADSZ,1);
    bool ret = true;
    if ( async && sz == 0) {
        // We just want a join w/o uplink so we can make it async
        ret = sendATCommand("AT+JOIN","+JOIN: Network joined","+JOIN: Join failed","+JOIN: Done",__DSKLORAE5_JOIN_TIMEOUT,true,NULL);
        this->isJoining = true;
    } else {
        // We need to have s sync Join, case is simpler
        ret = sendATCommand("AT+JOIN","+JOIN: Network joined","+JOIN: Join failed","+JOIN: Done",__DSKLORAE5_JOIN_TIMEOUT,false,NULL);
        this->isJoining = false;
        if ( ret ) {
            this->hasJoined = true;
        }
        return ret; // job finished
    }
  } 
  if ( this->hasJoined && sz > 0 ) {
      // now we can send the uplink message
      uint32_t txDuration = this->estimateTxDuration(this->lastSf,sz,this->lastRetry+1);
      this->estimatedDCMs += 100*txDuration;
      if (acked) {
        sprintf(_cmd,"AT+CMSGHEX=");
      } else {
        sprintf(_cmd,"AT+MSGHEX=");
      }
      int k = strlen(_cmd);
      for ( int i = 0 ; i < sz && k < 115 ; i++ ) {
        sprintf(&_cmd[k],"%02X",data[i]);
        k+=2;
      }
      this->hasAcked = false;
      this->downlinkPending = false;
      this->downlinkReceived = false;
      this->lastSize = sz;
      this->lastRssi = 0;
      this->lastSnr = 0;
      this->isBusy = false;
      bool ret;  
      if (acked) {
          this->_rxBuffer = rxBuffer;
          this->_rxSize = rxSize;
          this->_rxPort = rxPort;
          ret = sendATCommand(_cmd,"+CMSGHEX: Done","","",__DSKLORAE5_TX_TIMEOUT_BASE+txDuration+__DSKLORAE5_TX_TIMEOUT_ACK*(this->lastRetry+1),async,processTx);     
      } else {
          ret = sendATCommand(_cmd,"+MSGHEX: Done","","",__DSKLORAE5_TX_TIMEOUT_BASE+txDuration,async,processTx);           
      }
      if ( !async && this->isBusy ) ret = false;
      return ret;
  } 
}



// ---------------------------------------------------------------------
// Manage transmission response asynchronously
// example :
// 12:23:29.618 -> AT+CMSGHEX=20591A02324505490C07
// 12:23:29.721 -> +CMSGHEX: Start
// 12:23:29.721 -> +CMSGHEX: Wait ACK
// 12:23:33.559 -> +CMSGHEX: ACK Received
// 12:23:33.592 -> +CMSGHEX: PORT: 2; RX: "3E9999010101"
// 12:23:33.627 -> +CMSGHEX: RXWIN1, RSSI -84, SNR 6.0
// 12:23:33.627 -> +CMSGHEX: Done
bool Disk91_LoRaE5::processTx(Disk91_LoRaE5 * wrap) {
  if ( startsWith(wrap->bufResponse,"+CMSGHEX: RXWIN*, RSSI") || startsWith(wrap->bufResponse,"+MSGHEX: RXWIN*, RSSI")) {
     int s = indexOf(wrap->bufResponse,"RSSI ");
     wrap->lastRssi = 0;
     wrap->lastSnr = 0;
     if ( s > 0 ) {
        char sRssi[10];
        if ( extractNumber(&wrap->bufResponse[s], sRssi,10) ) {
           wrap->lastRssi = (int16_t)atof(sRssi);
        }
     }
     s = indexOf(wrap->bufResponse,"SNR ");
     if ( s > 0 ) {
        char sSnr[10];
        if ( extractNumber(&wrap->bufResponse[s], sSnr,10) ) {
           wrap->lastSnr = (int16_t)atof(sSnr);
        }
     }
     wrap->hasAcked = true;
  } else if (startsWith(wrap->bufResponse,"+CMSGHEX: Done") || startsWith(wrap->bufResponse,"+MSGHEX: Done")) {
    uint32_t elapsedTime = millis() - wrap->startTime;
    if ( wrap->hasAcked ) {
      // will see later if we want to use this
      uint8_t retries = elapsedTime / wrap->estimateTxDuration(wrap->lastSf,wrap->lastSize,1); // really approximative approach
    } else {
      //Serial.printf("Add Data for seq(%d) rssi(%d) snr(%d) [Lost]\r\n",loraContext.currentSeqId,(int16_t)0, (int16_t)0);
    }
    // update the Duty cycle based on real retry estimate
    wrap->currentSeqId = (wrap->currentSeqId + 1) & 0xFFFF ;
  } else if ( startsWith(wrap->bufResponse,"+CMSGHEX: FPEND") ||  startsWith(wrap->bufResponse,"+MSGHEX: FPEND")) {
    // downlink pending
    wrap->downlinkPending = true;
  } else if ( startsWith(wrap->bufResponse,"+CMSGHEX: PORT: *; RX: ") || startsWith(wrap->bufResponse,"+MSGHEX: PORT: *; RX: ") ) {
    // downlink content
    int s = indexOf(wrap->bufResponse,"PORT: ");
    int port=0;
    if ( s > 0 ) {
       char sPort[10];
       if ( extractNumber(&wrap->bufResponse[s], sPort,10) ) {
          port = atoi(sPort);
          if ( wrap->_rxPort != NULL ) *wrap->_rxPort = port;
       }
    }
    s = indexOf(wrap->bufResponse,"RX: \"");
    if ( s > 0 & wrap->_rxBuffer != NULL && wrap->_rxSize != NULL ) {
       uint8_t downlink[__DSKLORAE5_ATRESP_BUFF_SZ];
       uint8_t sz = __DSKLORAE5_ATRESP_BUFF_SZ;
       if ( extractHexStr(&wrap->bufResponse[s], downlink, &sz) ) {
           if ( sz > *wrap->_rxSize ) sz = *wrap->_rxSize;
           bcopy(downlink, wrap->_rxBuffer,sz);
           *wrap->_rxSize = sz;
       }
    }
    wrap->hasAcked = true;
    wrap->downlinkReceived = true;
  } else if (startsWith(wrap->bufResponse,"+CMSGHEX: LoRaWAN modem is busy") || startsWith(wrap->bufResponse,"+MSGHEX: LoRaWAN modem is busy")) {
    wrap->isBusy = true;
  } else {
    // unprocessed lines
  }
  return false;
}


// estimate the duty-cycle, this is really approximative and to compensate the absence of this 
// infomration from the LoRaE5
uint32_t Disk91_LoRaE5::estimateTxDuration(uint8_t sf, uint8_t payloadSz, uint8_t retries) {
   if ( this->currentZone == DSKLORAE5_ZONE_EU868 ) {
      // EU868
      // 1% based on SF and data size (10 Bytes)
      // @TODO also consider ack
      // @TODO make this more generic considering payload size
      if ( retries == 0 ) retries = 1;
      switch (sf) {
          case 7:  return (retries*(  4630+payloadSz* 225))/100; 
          case 8:  return (retries*(  9270+payloadSz* 256))/100; 
          case 9:  return (retries*( 16490+payloadSz* 450))/100;
          case 10: return (retries*( 32970+payloadSz* 819))/100; 
          case 11: return (retries*( 65950+payloadSz*1802))/100; 
          default:
          case 12: return (retries*(115510+payloadSz*3277))/100;
      }
   } else {
      // No Duty Cycle zones, set a minimum time
      return __DSKLORAE5_NONDCZONE_DC;
   }
}

// =============================================================================
// Manage Commands

/**
 * Execute an AT command with a timeout
 * Search for okResp or errResp to determine is the command is a success or a fail
 * When ending is defined, it search for this sentence to consider end of response
 * Can be executed as a sync or async command.
 * okResp and errResp can use joker char with '*'
 * The okResp / errResp surch is a startsWith operation
 * When lineProcessing function is given, each of the line are transmitted to a custom function for processing, when return true, processing is stopped (like for ending)
 */
bool Disk91_LoRaE5::sendATCommand(
    const char * cmd, 
    const char * okResp, 
    const char * errResp, 
    const char * ending, 
    uint32_t timeoutMs, 
    bool async, 
    bool (*lineProcessing)(Disk91_LoRaE5 *) 
) {
  if ( this->runningCommand ) {
    this->tracef(F("LoRaE5 - AT commande already processing\r\n"));
    return false;
  }
  this->runningCommand = true;
  this->startTime = millis();
  this->maxDuration = timeoutMs;
  strcpy(this->bufOkResp,okResp);
  strcpy(this->bufErrResp,errResp);
  if ( ending != NULL && strlen(ending) > 0 ) {
    strcpy(this->bufEnding,ending);
    this->withEndingCondition = true;
  } else {
    this->withEndingCondition = false;
  }
  this->respIndex = 0;
  this->lineProcessing = lineProcessing;
  if ( this->isHwSerial ) {
      this->e5Uart->printf("%s\r\n",cmd);
  } else {
      this->e5SwUart->printf("%s\r\n",cmd);
  }

  this->tracef("LoRaE5 - send ");
  this->tracef(cmd);
  this->tracef(F("\r\n"));
  if ( !async ) {
    while ( ! processATResponse() );
    return this->statusCommand;
  }
  return true;
}

/**
 * Process command response
 * return true when nothing more to be done
 */
bool Disk91_LoRaE5::processATResponse() {

  // nothing to be done
  if ( !this->runningCommand ) return true;

  // manage timeout
  uint32_t duration  = millis() - this->startTime;   // overflow after 50D. risk taken.
  if ( duration > this->maxDuration ) {
    this->runningCommand = false;
    this->statusCommand = false;
    //this->tracef(F("LoRaE5 - timeout\r\n"));
    return true;
  }
  // process serial line response
  while ( (this->isHwSerial  && this->e5Uart->available() > 0)
       || (!this->isHwSerial && this->e5SwUart->available() > 0)
  ) {
      char c;
      if ( this->isHwSerial ) {
          c = this->e5Uart->read();
      } else {
          c = this->e5SwUart->read();
      }
      if ( (c == '\0' || c == '\r' || c == '\n' ) ) {
        if ( this->respIndex > 0 ) {
          // process line response
          this->bufResponse[this->respIndex] = '\0';
          this->tracef(F("LoRaE5 - recv "));
          this->tracef(this->bufResponse);
          this->tracef(F("\r\n"));
          int i;
          if ( this->lineProcessing != NULL ) {
            if ( this->lineProcessing(this) ) {
              this->runningCommand = false;
              this->respIndex = 0;
              return true;
            }
          }
          if ( strlen(this->bufErrResp) > 0 && startsWith(this->bufResponse,this->bufErrResp) ) {
              // Error String found
              if ( ! this->withEndingCondition ) {
                this->runningCommand = false;
              }
              this->statusCommand = false;
              this->respIndex = 0;
              this->tracef(F("LoRaE5 - AT ending with error\r\n"));
              return !this->withEndingCondition;
          }
          if ( strlen(this->bufOkResp) > 0 && startsWith(this->bufResponse,this->bufOkResp) ) {
              // Success String found
              if ( ! this->withEndingCondition ) {
                this->runningCommand = false;
              }
              this->statusCommand = true;
              this->respIndex = 0;
              return !this->withEndingCondition;
          }
          if ( this->withEndingCondition && startsWith(this->bufResponse,this->bufEnding) ) {
              // this is the end
              this->runningCommand = false;
              this->respIndex = 0;
              return true;
          }
        }
        this->respIndex = 0;
      } else {
        if ( c >=32 && c <=127 ) {
          if ( this->respIndex < __DSKLORAE5_ATRESP_BUFF_SZ ) {
            this->bufResponse[this->respIndex] = c;
            this->respIndex++;    
          } else {
            this->tracef(F("LoRaE5 - Response size overflow\r\n"));
            this->respIndex = 0;
          }
        }
      }
  }
  return false;
}

// =============================================================================
// Getters
bool Disk91_LoRaE5::isAcked() {
    return this->hasAcked;
}

bool Disk91_LoRaE5::isJoined() {
    return this->hasJoined;
}

bool Disk91_LoRaE5::isDownlinkPending() {
    return this->downlinkPending;
}

bool Disk91_LoRaE5::isDownlinkReceived() {
    return this->downlinkReceived;
}

int16_t Disk91_LoRaE5::getRssi() {
    return (this->hasAcked )?this->lastRssi:DSKLORAE5_INVALID_RSSI;
}

int16_t Disk91_LoRaE5::getSnr() {
    return (this->hasAcked )?this->lastSnr:DSKLORAE5_INVALID_SNR;
}