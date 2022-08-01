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

#ifndef __DISK91_LORAE5_H__
#define __DISK91_LORAE5_H__

#include <Arduino.h>
#include <SoftwareSerial.h>

// ==========================================================
// Setup options

#ifdef SEEED_WIO_TERMINAL
    // Supports 2 Serial
    #define __SERIAL1
    #define __SERIAL2
    #define DSKLORAE5_SEARCH_WIO        0x0B        // Search for the LoRa-E5 on the different port of the WIO
    #define DSKLORAE5_SWSERIAL_WIO_P1   0x1A        // LoRa-E5 on WIO Software serial 1 (RX:BCM3 / TX:BCM2)
    #define DSKLORAE5_SWSERIAL_WIO_P2   0x1B        // LoRa-E5 on WIO Software serial 2 (RX:BCM27 / TW:BCM22)
    #define DSKLORAE5_DISABLE_FSTR                  // Disable Flash String support
#endif

#ifdef __SERIAL1
    #define DSKLORAE5_HWSERIAL1         0x01        // LoRa-E5 on serial port 1
#endif

#ifdef __SERIAL2
    #define DSKLORAE5_HWSERIAL2         0x02        // LoRa-E5 on serial port 2
#endif

#define DSKLORAE5_HWSEARCH          0x00        // Search for LoRa-E5 on the different HWSerial
#define DSKLORAE5_HWSERIAL_CUSTOM   0x0A
#define DSKLORAE5_SWSERIAL_CUSTOM   0x10        // LoRa-E5 on a software serial to be specified
#define DSKLORAE5_SWSERIAL_PINS     0x11        // LoRa-E5 on a software serial to be created by library with pins

#define DSKLORAE5_ZONE_UNDEFINED    0x00
#define DSKLORAE5_ZONE_EU868        0x01
#define DSKLORAE5_ZONE_US915        0x02
#define DSKLORAE5_ZONE_AS923_1      0x03
#define DSKLORAE5_ZONE_AS923_2      0x04
#define DSKLORAE5_ZONE_AS923_3      0x05
#define DSKLORAE5_ZONE_AS923_4      0x06
#define DSKLORAE5_ZONE_KR920        0x07
#define DSKLORAE5_ZONE_IN865        0x08
#define DSKLORAE5_ZONE_AU915        0x09

#define DSKLORAE5_SF_UNCHANGED      0xFF          // Keep the previous DataRate
#define DSKLORAE5_DW_UNCHANGED      0xFF          // Keep the previous Tx Power
#define DSKLORAE5_RT_UNCHANGED      0xFF          // Keep the previous Retries

#define DSKLORAE5_INVALID_RSSI      -999
#define DSKLORAE5_INVALID_SNR       -99

// ==========================================================
// Some internal param
#define __DSKLORAE5_ENABLE                        // comment to remove traces and save some memory & Flash
#define __DSKLORAE5_TRACE_MAX_BUF_SZ  128         // max size for a trace line
#define __DSKLORAE5_ATCMD_BUFF_SZ      64         // max size to store ok/err response pattern
#define __DSKLORAE5_ATRESP_BUFF_SZ    128         // max size to store a response from an AT command

#define __DSKLORAE5_UNSET              -1         // default value to unset a parameter
#define __DSKLORAE5_UNSET_POWER      -100  

#define __HWSERIAL_T                  Uart         // type to be used for hardware serial, apprantly different are existing
#define __DSKLORAE5_DEFAULT_AT_TMOUT  2000         // default time for AT command timeout in Ms
#define __DSKLORAE5_JOIN_TIMEOUT     12000         // Specific timeout for Join procedure in Ms
#define __DSKLORAE5_TX_TIMEOUT_BASE   8000         // Specific timeout for Tx w/o ack ( tx time will be added )
#define __DSKLORAE5_TX_TIMEOUT_ACK    3500         // Time to add for Ack / Downlink frames
#define __DSKLORAE5_JOINREQ_PAYLOADSZ   24         // Estimation of a Join Req frame as a normal frame with a given payload of 24bytes
                                                   //   looking for a source to get a better estimate

#define __DSKLORAE5_NONDCZONE_DC         0         // Eventually force a duty cycle when a zone does not have one


// ==========================================================
// Disk91_LoRaE5
class Disk91_LoRaE5 {

private:
    bool                  runningCommand;   // true when an AT command is already running
    uint32_t              startTime;        // store start of AT command for timeout management
    uint32_t              maxDuration;      // At command timeout
                                            // True when the AT process monitor a end condition
    bool                  withEndingCondition;
                                            // Buffers to store a copy of the ok/err patterns
    char                  bufOkResp[__DSKLORAE5_ATCMD_BUFF_SZ];
    char                  bufErrResp[__DSKLORAE5_ATCMD_BUFF_SZ];
    char                  bufEnding[__DSKLORAE5_ATCMD_BUFF_SZ];
    char                  bufResponse[__DSKLORAE5_ATRESP_BUFF_SZ];
    uint8_t               bufInt8;          // Just a single byte buffer for returning value from callback
    uint8_t    *          _rxBuffer;        // store the rxbuffer for async processing
    uint8_t    *          _rxSize;          // store the rxbuffer size pointer
    uint8_t    *          _rxPort;          // store the rxport pointer for async process
    uint16_t              respIndex;        // processing response, index in buffer
                                            // Function for processing AT response line
    bool                  (*lineProcessing)(Disk91_LoRaE5 *);
    bool                  statusCommand;    // state of the AT command execution

    bool processATResponse();
    static bool processRead(Disk91_LoRaE5 *);

protected:
    uint16_t              atTimeout;        // At command timeout (in ms)
    bool                  hasJoined;        // true when the module has joined network
    bool                  isJoining;        // true when the join procedure is in progress
    bool                  isBusy;           // true when the communication failed potentially due to duty-cycle
    int8_t                lastSf;           // Last SpreadFactor for communication __DSKLORAE5_UNSET when not configured
    int8_t                lastPower;        // Last Power for communication __DSKLORAE5_UNSET_POWER when not configured
    int8_t                lastRetry;        // Last Retry for communication __DSKLORAE5_UNSET when not configured
    uint8_t               lastSize;         // last uplink size
    uint32_t              lastSendMs;       // last message sent timestamp
    int16_t               lastRssi;         // last ack / downlink RSSI
    int16_t               lastSnr;          // last ack / downlink SNR
    uint16_t              currentSeqId;     // Next communication estimated SeqId (the module does not allow to access it, so ... a bit a mess)
    bool                  downlinkPending;  // true when a downlink communication is pending 
    bool                  downlinkReceived; // true when a downlink has been received
    bool                  hasAcked;         // true when the uplink has been acked
    uint8_t               currentZone;      // The current zone set into the module for optimization
    uint32_t              estimatedDCMs;    // Next communication autorization for when DC management is not delegated to module (DC zone only)

    __HWSERIAL_T        * e5Uart;           // link to hw serial for E5 communications
    Serial_             * debugUart;        // link to serial used for debugginf
    SoftwareSerial      * e5SwUart;         // link to the sw serial for E5 communications
    bool                  isHwSerial;       // true when a hw serial is used

    void tracef(const char *format, ...);         // Debug traces
    #ifndef DSKLORAE5_DISABLE_FSTR
    void tracef(const __FlashStringHelper *format, ...);
    #endif

    // bool sendATCommand(                     // Send an AT command to the E5 module
    //     const char * cmd,                       // command to be sent
    //     const char * okResp,                    // expected char string to be find in response to consider the AT execution a success, * is a joker
    //     const char * errResp,                   // expected char string to be find in response to consider the AT execution a failure, * is a joker, null is err if not ok
    //     const char * ending,                    // expected char string to be find in response to consider end of AT command, null means timeout (slower)
    //     uint32_t timeoutMs,                     // timeout for getting a response  from E5
    //     bool async,                             // async processing with the E5 loop.
    //     bool (*lineProcessing)(Disk91_LoRaE5 *) // callback function to process each line of the E5 response
    // ); 

    bool storeOneByte(                      // Store 1 byte of data into the E5 eeprom memory
        uint8_t adr, uint8_t v);  
    bool readOneByte(                       // Read 1 byte of data from the E5 eeprom memory
        uint8_t adr, uint8_t * v); 

                                            // Estimate the duty-cycle resting time
    uint32_t estimateTxDuration(uint8_t sf, uint8_t payloadSz, uint8_t retries);
    bool testPresence();                    // return true when the LoRaE5 board has been found on the selected port
    void end();                             // close the library
    
    static bool processTx(Disk91_LoRaE5 * wrap);   // Process each of the line recaived from e5 when a tx request has been made

    bool sendReceive(    // send a message on LoRaWan, return true when sent is a success 
        uint8_t     port,               // LoRaWan port
        uint8_t *   data,               // Data / payload to be transmitted
        uint8_t     sz,                 // Size of the data, when 0 Join only is proceeded
        bool        acked,              // Ack / Downlink request
                                        // Downlink Callback, return size of downlink, 0 when ack, -1 for none, as a parameter the buffer 
                                        //  can't yet be used... set NULL
        int8_t      (*rxCallback)(uint8_t *),
        uint8_t *   rxBuffer,           // Downlink buffer, make sure it will be large enought, no verification
        uint8_t *   rxSize,             // uint8_t containing the rxBuffer size and returning the downlink message size
        uint8_t *   rxPort,             // uint8_t pointer for returnin the downlink port
        uint8_t     sf,                 // Spread Factor , use DSKLORAE5_SF_UNCHANGED to keep the previous one
        uint8_t     pwr,                // Transmission power, use DSKLORAE5_DW_UNCHANGED to keep the previous one
        uint8_t     retries,            // Number of retry, use DSKLORAE5_RT_UNCHANGED to keep the previous one. retry = 0 means 1 uplink, no retry
        bool        async               // When true, the processing is made synchronously
    );

public:
    Disk91_LoRaE5(
        bool       nothing = false       // if anyone can explain me why w/o param the constructer generate compilation error ?!?
    );

    Disk91_LoRaE5(
        uint16_t   atTimeoutMs,          // Default timeout in Ms for AT command execution
        Serial_  * logSerial = NULL      // When set, the library debug is enabled               
    );

    Disk91_LoRaE5(
        Serial_  * logSerial = NULL      // When set, the library debug is enabled               
    );

    ~Disk91_LoRaE5();

    bool begin(  
        uint8_t portType = DSKLORAE5_HWSEARCH,    // where to find the LoRa-E5 board  
        __HWSERIAL_T * hwSerial = NULL,           // for HWSERIAL_CUSTOM, link the associated Serial
        SoftwareSerial * swSerial = NULL,         // for SWSERIAL_CUTSOM, link the associated SoftwareSerial
        int16_t ssRxPort = -1,                    // for SWSERIAL_PINS, specify the RX & TX Pin, the SSerial will be initialized
        int16_t ssTxPort = -1
    );

    bool setup(                     // Setup the LoRaWAN stack with the stored credentials
        bool      selfDC = false,   // when true, the duty cycle management is not managed by the module but the user application
        bool      withADR = false   // when true, the ADR is turned ON
    );

    bool setup(                     // Setup the LoRaWAN stack
        uint8_t   zone,             // radio zone selection
        // uint8_t   deveui[],         // deviceEUI in the normal order for the bytes
        // uint8_t   appeui[],         // applicationEUI in the normal order for the bytes
        // uint8_t   appkey[],         // applicationKEY in the normal order for the bytes
        char   deveui[],           // deviceEUI in the normal order for the bytes
        char   appeui[],           // applicationEUI in the normal order for the bytes
        char   appkey[],           // applicationKEY in the normal order for the bytes
        bool      selfDC = false,   // when true, the duty cycle management is not managed by the module but the user application
        bool      withADR = false   // when true, the ADR is turned ON
    );

    bool setup_sensecap(                          // Setup the LoRaWAN stack
        uint8_t   zone,                  // radio zone selection
        bool      selfDC = false,        // when true, the duty cycle management is not managed by the module but the user application
        bool      withADR  = false       // when true, the ADR is turned ON
    );

    bool persistConfig(             // Store the LoRaWan configuration into module EEPROM for restoring later 
                                    //   this allows to update WIO firmware without loosing the LoRaWan credentials
            uint8_t   zone,         // radio zone selection
            // uint8_t   deveui[],     // deviceEUI in the normal order for the bytes
            // uint8_t   appeui[],     // applicationEUI in the normal order for the bytes
            // uint8_t   appkey[]      // applicationKEY in the normal order for the bytes
            char   deveui[],       // deviceEUI in the normal order for the bytes
            char   appeui[],       // applicationEUI in the normal order for the bytes
            char   appkey[]        // applicationKEY in the normal order for the bytes
    );

    bool haveStoredConfig();    // Returns true when a configuration has already been stored in the E5 memory
    bool clearStoredConfig();   // Purge the stored configuration for E5

    bool sendReceive_sync(      // send a message on LoRaWan expert an ack at least, eventually a downlink, return true when sent is a success and expect a ack
        uint8_t     port,               // LoRaWan port
        uint8_t *   data,               // Data / payload to be transmitted
        uint8_t     sz,                 // Size of the data, when 0 Join only is proceeded
        uint8_t *   rxBuffer,           // Downlink buffer, make sure it will be large enought, no verification
        uint8_t *   rxSize,             // uint8_t containing the rxBuffer size and returning the downlink message size
        uint8_t *   rxPort,             // uint8_t pointer for returnin the downlink port
        uint8_t     sf = 9,             // Spread Factor , use DSKLORAE5_SF_UNCHANGED to keep the previous one
        uint8_t     pwr = 14,           // Transmission power, use DSKLORAE5_DW_UNCHANGED to keep the previous one
        uint8_t     retries = 0         // Number of retry, use DSKLORAE5_RT_UNCHANGED to keep the previous one. retry = 0 means 1 uplink, no retry
    );

    bool send_sync(    // send a message on LoRaWan, return true when sent is a success 
        uint8_t     port,               // LoRaWan port
        uint8_t *   data,               // Data / payload to be transmitted
        uint8_t     sz,                 // Size of the data, when 0 Join only is proceeded
        bool        acked = false,      // Ack / Downlink request
        uint8_t     sf = 9,             // Spread Factor , use DSKLORAE5_SF_UNCHANGED to keep the previous one
        uint8_t     pwr = 14,           // Transmission power, use DSKLORAE5_DW_UNCHANGED to keep the previous one
        uint8_t     retries = 0         // Number of retry, use DSKLORAE5_RT_UNCHANGED to keep the previous one. retry = 0 means 1 uplink, no retry
    );

    bool join_sync(    // Join only (join is automatically executed on send but you can decide to join sepratly)
        uint8_t     sf = 9,             // Spread Factor , use DSKLORAE5_SF_UNCHANGED to keep the previous one
        uint8_t     pwr = 14            // Transmission power, use DSKLORAE5_DW_UNCHANGED to keep the previous one
    );

    bool sendATCommand(                     // Send an AT command to the E5 module
        const char * cmd,                       // command to be sent
        const char * okResp,                    // expected char string to be find in response to consider the AT execution a success, * is a joker
        const char * errResp,                   // expected char string to be find in response to consider the AT execution a failure, * is a joker, null is err if not ok
        const char * ending,                    // expected char string to be find in response to consider end of AT command, null means timeout (slower)
        uint32_t timeoutMs,                     // timeout for getting a response  from E5
        bool async,                             // async processing with the E5 loop.
        bool (*lineProcessing)(Disk91_LoRaE5 *) // callback function to process each line of the E5 response
    ); 

    bool isJoined();                    // return true when the device has joined the network
    bool isAcked();                     // return true when the previous uplink has been confirmed as received
    bool isDownlinkPending();           // return true when the previous uplink got a downlink pending response
    bool isDownlinkReceived();          // return true when the previous uplink got a downlink response
    int16_t getRssi();                  // return last Ack RSSI when the previous uplink has been confirmed as received or DSKLORAE5_INVALID_RSSI
    int16_t getSnr();                   // return last Ack SNR when the previous uplink has been confirmed as received or DSKLORAE5_INVALID_SNR

};


#endif // __DISK91_LORAE5_H__