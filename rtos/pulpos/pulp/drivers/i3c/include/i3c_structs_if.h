/* parasoft suppress item  MISRA2012-DIR-4_8 "Consider hiding implementation of structure" */
/**********************************************************************
 * Copyright (C) 2014-2018 Cadence Design Systems, Inc.
 * All rights reserved worldwide
 * The material contained herein is the proprietary and confidential
 * information of Cadence or its licensors, and is supplied subject to, and may
 * be used only by Cadence's customer in accordance with a previously executed
 * license and maintenance agreement between Cadence and that customer.
 **********************************************************************
 * WARNING: This file is auto-generated using api-generator utility.
 *          api-generator: 12.02.13bb8d5
 *          Do not edit it manually.
 **********************************************************************
 * Cadence Core Driver for I3C Master Interface
 **********************************************************************/
#ifndef I3C_STRUCTS_IF_H
#define I3C_STRUCTS_IF_H

#include "cdn_stdtypes.h"
#include "i3c_if.h"

/** @defgroup DataStructure Dynamic Data Structures
 *  This section defines the data structures used by the driver to provide
 *  hardware information, modification and dynamic operation of the driver.
 *  These data structures are defined in the header file of the core driver
 *  and utilized by the API.
 *  @{
 */

/**********************************************************************
 * Structures and unions
 **********************************************************************/
/** This structure holds configuration of interrupts. */
struct I3C_InterruptConfig_s
{
    /** If cmdResponseOverflow interrupt should be enabled. */
    bool cmdResponseOverflow;
    /** If cmdResponseUnderflow interrupt should be enabled. */
    bool cmdResponseUnderflow;
    /** If cmdResponseThreshold interrupt should be enabled. */
    bool cmdResponseThreshold;
    /** If cmdDescriptorOverflow interrupt should be enabled. */
    bool cmdDescriptorOverflow;
    /** If cmdDescriptorEmpty interrupt should be enabled. */
    bool cmdDescriptorEmpty;
    /** If cmdDescriptorThreshold interrupt should be enabled. */
    bool cmdDescriptorThreshold;
    /** If rxDataFifoUnderflow interrupt should be enabled. */
    bool rxDataFifoUnderflow;
    /** If rxFifoThreshold interrupt should be enabled. */
    bool rxFifoThreshold;
    /** If ibiResponseOverflow interrupt should be enabled. */
    bool ibiResponseOverflow;
    /** If ibiResponseUnderflow interrupt should be enabled. */
    bool ibiResponseUnderflow;
    /** If ibiResponseThreshold interrupt should be enabled. */
    bool ibiResponseThreshold;
    /** If ibiDataUnderflow interrupt should be enabled. */
    bool ibiDataUnderflow;
    /** If ibiDataThreshold interrupt should be enabled. */
    bool ibiDataThreshold;
    /** If txDataFifoOverflow interrupt should be enabled. */
    bool txDataFifoOverflow;
    /** If txFifoThreshold interrupt should be enabled. */
    bool txFifoThreshold;
    /** If immComplete interrupt should be enabled. */
    bool immComplete;
    /** If mastershipDone interrupt should be enabled. */
    bool mastershipDone;
    /** If halted interrupt should be enabled. */
    bool halted;
};

/** This structure holds configuration of slave-mode interrupts. */
struct I3C_SlaveInterruptConfig_s
{
    /** If sdrWrComplete interrupt should be enabled */
    bool sdrWrComplete;
    /** If sdrWrComplete interrupt should be enabled */
    bool sdrRdComplete;
    /** If ddrWrComplete interrupt should be enabled */
    bool ddrWrComplete;
    /** If ddrRdComplete interrupt should be enabled */
    bool ddrRdComplete;
    /** If sdrTxDataFifoOverflow interrupt should be enabled */
    bool sdrTxDataFifoOverflow;
    /** If sdrRxDataFifoUnderflow interrupt should be enabled */
    bool sdrRxDataFifoUnderflow;
    /** If ddrTxDataFifoOverflow interrupt should be enabled */
    bool ddrTxDataFifoOverflow;
    /** If ddrRxDataFifoUnderflow interrupt should be enabled */
    bool ddrRxDataFifoUnderflow;
    /** If sdrTxFifoThreshold interrupt should be enabled. */
    bool sdrTxFifoThreshold;
    /** If sdrRxFifoThreshold interrupt should be enabled. */
    bool sdrRxFifoThreshold;
    /** If ddrTxFifoThreshold interrupt should be enabled. */
    bool ddrTxFifoThreshold;
    /** If ddrRxFifoThreshold interrupt should be enabled. */
    bool ddrRxFifoThreshold;
    /** If masterReadAbort interrupt should be enabled */
    bool masterReadAbort;
    /** If ddrFail interrupt should be enabled */
    bool ddrFail;
    /** If sdrFail interrupt should be enabled */
    bool sdrFail;
    /** If dynamicAddrUpdated interrupt should be enabled. */
    bool dynamicAddrUpdated;
    /** If ibiDone interrupt should be enabled */
    bool ibiDone;
    /** If ibiNack interrupt should be enabled */
    bool ibiNack;
    /** If hotJoinDone interrupt should be enabled */
    bool hotJoinDone;
    /** If hotJoinNack interrupt should be enabled */
    bool hotJoinNack;
    /** If eventUpdate interrupt should be enabled */
    bool eventUpdate;
    /** If protocolError interrupt should be enabled */
    bool protocolError;
    /** If testMode interrupt should be enabled */
    bool testMode;
};

/** Configuration of threshold level. */
struct I3C_ThresholdConfig_s
{
    /** Threshold level for data TX FIFO. */
    uint16_t txFifoThreshold;
    /** Threshold level for data RX FIFO. */
    uint16_t rxFifoThreshold;
    /** Threshold level for IBIR FIFO. */
    uint8_t ibirFifoThreshold;
    /** Threshold level for CMDR FIFO. */
    uint8_t cmdrFifoThreshold;
};

/** Configuration parameters passed to probe & init functions. */
struct I3C_Config_s
{
    /** Base address of the register space. */
    MIPI_I3cRegs* regsBase;
    /** System clock frequency [Hz]. */
    uint32_t sysClk;
    /** SCL clock frequency of the slowest I2C Slave [Hz]. */
    uint32_t i2cFreq;
    /** SDR Speed frequency [Hz]. */
    uint32_t sdrFreq;
    /** Required SCL low period [ns]. */
    uint32_t sclLowPeriod;
    /** Maximal frequency supported by slave [Hz]. */
    uint32_t maxSclFreq;
    /** Array of I3C/I2C Device configuration descriptors. */
    I3C_DeviceDescriptor* devs;
    /** Number of the I3C/I2C Devices in the configuration structure. Minimum number of required configurations is 2 and the maximum is 64. */
    uint8_t numDevs;
    /** Pointer to array of IBI configuration structures. */
    I3C_IbiSirConfiguration* ibiSirCfgs;
    /** Number of I3C IBI configured devices. */
    uint8_t numSirCfgs;
    /** Maximum number of items that can be put in the Command FIFO. */
    uint16_t cmdFifoSize;
    /** Threshold level for command FIFO. */
    uint16_t cmdFifoThreshold;
    /** Maximum number of bytes that can be put in the TX FIFO. */
    uint16_t txFifoSize;
    /** Maximum number of bytes that can be put in the RX FIFO. */
    uint16_t rxFifoSize;
    /** Threshold configuration. Levels for threshold interrupts. */
    I3C_ThresholdConfig thresholdConfig;
    /** Configuration for enabling and disabling interrupts. */
    I3C_InterruptConfig interruptConfig;
    /** Slave interrupt configuration. */
    I3C_SlaveInterruptConfig slaveInterruptConfig;
};

/** System requirements returned by probe. */
struct I3C_SysReq_s
{
    /** Size of memory required for driver's private data. */
    uint32_t memReq;
};

/** Configuration parameters passed to probe & init functions. */
struct I3C_Callbacks_s
{
    /** This function will be called while attempting to write a response to full command response FIFO. */
    I3C_InterruptEvent cmdResponseOverflow;
    /** This function will be called while attempting to read response from empty response FIFO. */
    I3C_InterruptEvent cmdResponseUnderflow;
    /** This function will be called when command response FIFO threshold is reached. */
    I3C_InterruptEvent cmdResponseThreshold;
    /** This function will be called while attempting to write a command to full command descriptor FIFO. */
    I3C_InterruptEvent cmdDescriptorOverflow;
    /** This function will be called while attempting to read data from empty command descriptor FIFO. */
    I3C_InterruptEvent cmdDescriptorEmpty;
    /** This function will be called when command descriptor FIFO threshold is reached. */
    I3C_InterruptEvent cmdDescriptorThreshold;
    /** This function will be called when driver attempts to read from empty RX FIFO. */
    I3C_InterruptEvent rxDataFifoUnderflow;
    /** This function will be called when receive data FIFO threshold is reached. */
    I3C_InterruptEvent rxFifoThreshold;
    /** This function will be called while attempting to write a command to full ibi response FIFO. */
    I3C_InterruptEvent ibiResponseOverflow;
    /** This function will be called while attempting to read data from empty ibi response FIFO. */
    I3C_InterruptEvent ibiResponseUnderflow;
    /** This function will be called when ibi response FIFO threshold is reached. */
    I3C_InterruptEvent ibiResponseThreshold;
    /** This function will be called while attempting to read data from empty ibi data FIFO. */
    I3C_InterruptEvent ibiDataUnderflow;
    /** This function will be called when ibi data FIFO threshold is reached. */
    I3C_InterruptEvent ibiDataThreshold;
    /** This function will be called when transmit data FIFO threshold is reached. */
    I3C_InterruptEvent txFifoThreshold;
    /** This function will be called when driver attempts to write data to full TX FIFO. */
    I3C_InterruptEvent txDataFifoOverflow;
    /** This function will be called when Immediate Command(s) processing is completed. */
    I3C_InterruptEvent immComplete;
    /** This function will be called when Mastership Request interrupt occurs. */
    I3C_InterruptEvent mastershipDone;
    /** This function will be called when Halted interrupt occurs. */
    I3C_InterruptEvent halted;
    /** This function will be called when all commands form command buffer are sent. */
    I3C_CommandBufferEmptyEvent commandBufferEmpty;
    /** This function will be called when command is processed without errors. */
    I3C_CommandCompleteEvent commandComplete;
    /** This function will be called when DAA procedure is finished. */
    I3C_DaaCompleteEvent daaComplete;
    /** This function will be called when mastership request occurs. */
    I3C_MastershipRequestEvent mastershipRequest;
    /** This function will be called when Hot-Join occurs. */
    I3C_HotJoinEvent hotjoin;
    /** This function will be called when Hot-Join occurs. */
    I3C_InbandInterruptEvent inbandInterrupt;
    /** This function will be called when SDR Write is completed ane master operates in slave mode. */
    I3C_InterruptEvent slaveSdrWrComplete;
    /** This function will be called when SDR Read is completed ane master operates in slave mode. */
    I3C_InterruptEvent slaveSdrRdComplete;
    /** This function will be called when DDR Write is completed ane master operates in slave mode. */
    I3C_InterruptEvent slaveDdrWrComplete;
    /** This function will be called when DDR Read is completed ane master operates in slave mode. */
    I3C_InterruptEvent slaveDdrRdComplete;
    /** This function will be called when TestMode command arrives. */
    I3C_InterruptEvent testMode;
};

/** I3C/I2C Device characteristics. */
struct I3C_DeviceDescriptor_s
{
    /** I2C Legacy Virtual Register Index. Not used for I3C Devices. */
    uint8_t i2cIdx;
    /** I2C Device operation speed. If true then Device works in FM+ Speed. Otherwise it works in FM Speed. Not used for I3C Devices. */
    bool i2cFmPlusSpeed;
    /** Reserved for 15 available codes for describing the Device capabilities and functions on the sensors system. Not used if Device is in I3C Mode. */
    uint8_t i2cRsvd;
    /** If true, then addr contains 10-bit address for of the Legacy I2C Slave. If false, then the address is 7-bit. Not used for I3C Devices. */
    bool i2c10bAddr;
    /** Is I3C Device Capable of performing High Data Rate transfers. Not used in I2C Mode. */
    bool hdrCapable;
    /** If true then it is a Legacy I2C Device. Otherwise it is a I3C Device */
    bool legacyI2CDev;
    /** Is the Device active. */
    bool isActive;
    /** 7-bit Device address in case of I3C Device. 7-bit or 10-bit Device address in case of Legacy I2C Slave. */
    uint16_t addr;
    /** Provisional ID value bits 47:32. Bits 47:33 - MIPI Manufacturer ID. Bit 32 - Type Selector (0 - Vendor Fixed Value, 1 - Random Value). */
    uint16_t provIdHi;
    /** Provisional ID value bits 31:0. If Type Selector (Provisional ID bit 32) is set to 1, then: Bits 31:16 Part ID, Bits 15:12 - Instance ID, Bits 11:0 - left for definition with additional meaning. Please refer to the MIPI I3C specification chapter 5.1.4.1.1 for additional information. If Type Selector is set to 0, then this field will receive random 32-bit value from the Device. */
    uint32_t provIdLo;
    /** Bus Characteristic Register. I3C only. */
    uint8_t bcr;
    /** Device Characteristics Register. I3C only. */
    uint8_t dcr;
    /** For driver's internal use only. */
    volatile uint32_t* priv;
};

/** I3C/I2C Slave descriptor . */
struct I3C_SlaveDescriptor_s
{
    /** Slave's Dynamic Address assigned by the Master. */
    uint8_t da;
    /** Slave's Device Characteristics Register value. */
    uint8_t dcrType;
    /** Slave's Bus Characteristics Register value. */
    uint8_t bcrType;
    /** Slave's original I2C Static Address (0 if no address). */
    uint8_t sa;
};

/** Structure for holding multiple I3C/I2C Slave descriptors. */
struct I3C_SlaveDescriptors_s
{
    /** Number of Slaves. */
    uint8_t slave_count;
    /** Array of slave descriptors. */
    I3C_SlaveDescriptor descriptors[I3C_ABSOLUTE_MAX_DEVS_NUM];
};

/** Configuration of IBI capable I3C device */
struct I3C_IbiSirConfiguration_s
{
    /** IBI response that will be sent to requesting device. */
    I3C_IbiResponse ibiResp;
    /** 7-bit Device address in case of I3C Device. */
    uint16_t addr;
    /** Expected IBI payload size */
    uint8_t ibiPayloadSize;
    /** Speed limit of payload transfer from IBI requesting device. */
    I3C_IbiDataSpeedLimit ibiPayloadSpeedLimit;
    /** Role device configured for IBI. */
    I3C_IbiRequestingDeviceRole ibiReqDevRole;
};

/** Strucutre describing IBI. */
struct I3C_Ibi_s
{
    /** The type of an IBI request. */
    uint8_t ibi_type;
    /** Number of bytes transferred. */
    uint8_t xfer_bytes;
    /** Indicates if error occured. */
    bool error;
    /** ID of slave issuing IBI */
    uint8_t slv_id;
    /** Indicates if IBI was acked or nacked. */
    bool acked;
};

/** Structure describing command. */
struct I3C_CommandDescriptor_s
{
    /** CommomCommandCode of a command. */
    I3C_CommonCommandCode commomCommandCode;
    /** Pointer to data of command. */
    uint8_t* payload;
    /** Size of the payload when payload provided or 0 if there is no payload to be transferred. */
    uint16_t payloadSize;
};

/** Structure describing command. */
struct I3C_cmd_t_s
{
    /** ID of command */
    uint8_t id;
    /** Two command words combined. */
    uint64_t cmd;
    /** Pointer to data of command. */
    volatile uint8_t* payload_sdr;
    /** Pointer to data of command. */
    volatile uint16_t* payload_ddr;
    /** Size of the payload when payload provided or 0 if there is no payload to be transferred. */
    uint16_t payload_size;
    /** Used internally for most of the CCC's. */
    uint32_t priv;
    /** Field indicates if it is a write command. */
    bool is_wr_cmd;
    /** Indicates if command has been written to command fifo. */
    bool in_cmd_fifo;
    /** Number of  bytes that was either send or received. */
    uint16_t bytes_fared;
    /** Points which byte of payload is read or written. Used as index of payload_sdr or payload_ddr array */
    uint16_t payload_iterator;
    /** CRC of DDR. */
    uint8_t ddr_crc;
    /** Reading from/to FIFO in progress.. */
    bool transfer_in_progress;
};

/** List of commands to send. */
struct I3C_cmd_list_element_s
{
    /** Command. */
    I3C_cmd_t cmd;
    /** Indicates if this node is used. */
    bool used;
};

/** List of commands to send. */
struct I3C_cmd_list_s
{
    /** Maximum number of descriptors in the buffer. */
    size_t capacity;
    /** Number of descriptors in the buffer. */
    I3C_cmd_list_element* buffer;
    /** Number of descriptors in the buffer. */
    size_t count;
};

/** information about CSR */
struct I3C_CsrData_s
{
    /** CSR address. */
    uint16_t csr;
    /** Specifies if CSR address is 16-bit (true) or 8-bit (false). */
    bool csr16;
};

/** Information about payload */
struct I3C_PayloadData_s
{
    /** Pointer to SDR buffer containing payload to be sent */
    uint8_t* sdr_payload;
    /** Pointer to DDR buffer containing payload to be sent */
    uint16_t* ddr_payload;
    /** Size of the payload [B]. */
    uint16_t payloadSize;
};

/** Structure contains information about transmission parameters */
struct I3C_TransmissionParameters_s
{
    /** Determines if the Message is addressed to the specific I3C Device (false) or to all I3C Devices (true). */
    bool broadcast;
    /** If true, the Command will be send via priority queue. */
    bool immediate;
    /** If true, the Repeated Start Between Commands is enabled. */
    bool rsbc;
};

/** Driver's Private Data. */
struct I3C_PrivData_s
{
    MIPI_I3cRegs* regs_base;
    bool isr_en;
    bool use_fifos;
    uint8_t num_gpo;
    uint8_t num_gpi;
    uint8_t max_devs;
    uint8_t max_ibi_devs;
    I3C_Callbacks callbacks;
    I3C_DeviceDescriptor devs[I3C_ABSOLUTE_MAX_DEVS_NUM];
    /** Interrupt configuration. */
    I3C_InterruptConfig interrupt_config;
    /** Slave interrupt configuration. */
    I3C_SlaveInterruptConfig slaveInterruptConfig;
    /** Threshold configuration. */
    I3C_ThresholdConfig threshold_config;
    I3C_DeviceRole dev_role;
    I3C_BusMode bus_mode;
    bool cmd_comp;
    /** Indicates if software command FIFO is empty. */
    bool cmd_empty;
    bool cmd_abort;
    bool cmd_in_progress;
    bool imd_in_progress;
    uint16_t cmd_fifo_used;
    /** Indicates if hardware command FIFO is empty. */
    bool cmd_fifo_empty;
    uint16_t tx_fifo_used;
    uint16_t rx_fifo_used;
    bool wait_for_rx;
    uint16_t cmd_fifo_size;
    uint16_t ibi_fifo_size;
    uint16_t cmd_fifo_threshold;
    uint16_t tx_fifo_size;
    uint16_t rx_fifo_size;
    I3C_cmd_t imd_cmd;
    I3C_cmd_list cmd_list;
    I3C_cmd_list_element CMD_LIST_BUFFER[I3C_CMD_BUFFER_SIZE];
    /** Indicates if tx fifo write command is in progress */
    volatile I3C_cmd_t* tx_cmd_in_progress;
    /** Indicates if rx fifo read command is in progress */
    volatile I3C_cmd_t* rx_cmd_in_progress;
    /** Store next CMD ID */
    uint8_t next_cmd_id;
    /** Stores last IBI */
    I3C_Ibi ibi;
    /** Mask which indicates which devices are active. */
    uint16_t devs_active;
    /** Mask which indicates which devices hoined the bus. */
    uint16_t hj_devices;
};

/** Information about ASF in I3C controller. */
struct I3C_AsfInfo_s
{
    /** ASF register start addresses */
    volatile uint32_t* regs_base;
};

/** Information about max. read length. */
struct I3C_MaxReadLength_s
{
    /** Max. read length for regular payload. */
    uint16_t payload_length;
    /** Max. read length for In-Band Interrupt payload. */
    uint8_t ibi_payload_length;
};

/** Information about max. data speed. */
struct I3C_MaxDataSpeed_s
{
    /** Max. read spead */
    uint8_t read_speed;
    /** Max. write spead. */
    uint8_t write_speed;
};

/**
 *  @}
 */

#endif	/* I3C_STRUCTS_IF_H */
