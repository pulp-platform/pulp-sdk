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

#ifndef I3C_IF_H
#define I3C_IF_H

#include "cdn_errno.h"
#include "cdn_stdtypes.h"
#include "mipi_i3c_regs.h"

/** @defgroup ConfigInfo  Configuration and Hardware Operation Information
 *  The following definitions specify the driver operation environment that
 *  is defined by hardware configuration or client code. These defines are
 *  located in the header file of the core driver.
 *  @{
 */

/**********************************************************************
* Defines
**********************************************************************/
/** Max number of I3C devices on I3C bus according to specification. */
#define	I3C_ABSOLUTE_MAX_DEVS_NUM 11U

#define	I3C_CMD_BUFFER_SIZE 20U

/** Send Command using DDR Mode */
#define	I3C_CMD_DDR_MODE (((1U) & 1U) << 31U)

/** Specifies if Repeated Start Between Commands is enabled. */
#define	I3C_CMD_RSBC ((uint64_t)1U << 25U)

/** Specifies if Device addres is 10-bit or 7-bit long. This is applicable only for the Legacy I2C Devices. */
#define	I3C_CMD_DEV_ADDR_HAS_MSB ((uint64_t)1U << 24U)

/** Read Command. */
#define	I3C_CMD_DIR_READ (1U)

/** Write Command. */
#define	I3C_CMD_DIR_WRITE (0U)

/** Max value of DDR TX transfer command code. */
#define	I3C_HDR_DDR_TX_CMD_CODE_MAX (0x7FU)

/** Min value of DDR RX transfer command code. */
#define	I3C_HDR_DDR_RX_CMD_CODE_MIN (0x80U)

/** Max value of DDR RX transfer command code. */
#define	I3C_HDR_DDR_RX_CMD_CODE_MAX (0x8FU)

/** Initial value for CRC word RX DDR transfer while in slave mode. */
#define	I3C_HDR_DDR_INITIAL_CRC_RX (0x1FU)

/** Initial value for CRC for TX DDR transfer while in slave mode. */
#define	I3C_HDR_DDR_INITIAL_CRC_TX (0x01U)

/** Initial value for Command ID */
#define	I3C_CMD_ID_INIT_VALUE (0xFAU)

/** First command ID */
#define	I3C_CMD_ID_FIRST (0x01U)

/** Two cmmands are reserved for HW, this is max-ID */
#define	I3C_CMD_ID_MAX (0xFDU)

/** Reserved ID for DISEC command issued by hardware after Hot-Join */
#define	I3C_CMD_ID_HW_DISEC (0xFEU)

/** Reserved ID for ENTDAA command issued by hardware after Hot-Join */
#define	I3C_CMD_ID_HW_ENTDAA (0xFFU)

/** Number of slaves per SIR register */
#define	I3C_NUM_OF_SLVS_PER_SIR (2U)

/**
 *  @}
 */


/** @defgroup DataStructure Dynamic Data Structures
 *  This section defines the data structures used by the driver to provide
 *  hardware information, modification and dynamic operation of the driver.
 *  These data structures are defined in the header file of the core driver
 *  and utilized by the API.
 *  @{
 */

/**********************************************************************
 * Forward declarations
 **********************************************************************/
typedef struct I3C_InterruptConfig_s I3C_InterruptConfig;
typedef struct I3C_SlaveInterruptConfig_s I3C_SlaveInterruptConfig;
typedef struct I3C_ThresholdConfig_s I3C_ThresholdConfig;
typedef struct I3C_Config_s I3C_Config;
typedef struct I3C_SysReq_s I3C_SysReq;
typedef struct I3C_Callbacks_s I3C_Callbacks;
typedef struct I3C_DeviceDescriptor_s I3C_DeviceDescriptor;
typedef struct I3C_SlaveDescriptor_s I3C_SlaveDescriptor;
typedef struct I3C_SlaveDescriptors_s I3C_SlaveDescriptors;
typedef struct I3C_IbiSirConfiguration_s I3C_IbiSirConfiguration;
typedef struct I3C_Ibi_s I3C_Ibi;
typedef struct I3C_CommandDescriptor_s I3C_CommandDescriptor;
typedef struct I3C_cmd_t_s I3C_cmd_t;
typedef struct I3C_cmd_list_element_s I3C_cmd_list_element;
typedef struct I3C_cmd_list_s I3C_cmd_list;
typedef struct I3C_CsrData_s I3C_CsrData;
typedef struct I3C_PayloadData_s I3C_PayloadData;
typedef struct I3C_TransmissionParameters_s I3C_TransmissionParameters;
typedef struct I3C_PrivData_s I3C_PrivData;
typedef struct I3C_AsfInfo_s I3C_AsfInfo;
typedef struct I3C_MaxReadLength_s I3C_MaxReadLength;
typedef struct I3C_MaxDataSpeed_s I3C_MaxDataSpeed;


/**********************************************************************
 * Enumerations
 **********************************************************************/
/** I3C Bus Mode. */
typedef enum
{
    /** Only I3C Devices are connected to the bus. */
    I3C_BUS_MODE_PURE = 0U,
    /** Bus mode is invalid */
    I3C_BUS_MODE_INVALID = 1U,
    /** There are I3C and I2C FM+ Devices connected to the bus. */
    I3C_BUS_MODE_MIXED_FAST = 2U,
    /** There are I3C, I2C FM+ and I2C FM Devices connected to the bus. */
    I3C_BUS_MODE_MIXED_SLOW = 3U
} I3C_BusMode;

/** I3C Device Role. */
typedef enum
{
    /** Device is a Master. */
    I3C_DEV_ROLE_MASTER = 0U,
    /** Device is a Secondary Master. */
    I3C_DEV_ROLE_SECONDARY_MASTER = 1U
} I3C_DeviceRole;

/** I3C Controller Operation Mode. */
typedef enum
{
    /** Device operates as a Slave. */
    I3C_OP_MODE_SLAVE = 0U,
    /** Device operates as a Master . */
    I3C_OP_MODE_MASTER = 1U
} I3C_OperationMode;

/** Inband Interrupt type. */
typedef enum
{
    /** Device operates as a Slave. */
    I3C_IBI_INBAND = 0U,
    /** Device operates as a Master . */
    I3C_IBI_HOTJOIN = 1U,
    /** Device operates as a Master . */
    I3C_IBI_MASTERSHIP = 2U
} I3C_IbiType;

/** Command(s) execution status. */
typedef enum
{
    /** No error. */
    I3C_CMD_ERR_NONE = 0U,
    /** Framing error. */
    I3C_CMD_ERR_DDR_FRAMING_ERROR = 1U,
    /** Parity error - incorrect parity bit received. */
    I3C_CMD_ERR_DDR_PARITY_ERROR = 2U,
    /** RX FIFO overflowed. */
    I3C_CMD_ERR_DDR_RX_OVF_ERROR = 3U,
    /** Unknown Device Address used in the Command. */
    I3C_CMD_ERR_UNKNOWN_DA = 4U,
    /** Read was aborted by the Master. */
    I3C_CMD_ERR_READ_ABORTED = 5U,
    /** Command was NACKed. */
    I3C_CMD_ERR_NACK = 6U
} I3C_CommandStatus;

/** Priority of the Command. This bit is applicable only for non-CCC commands. */
typedef enum
{
    /** Send Command via normal queue. */
    I3C_CMD_PRIORITY_NORMAL = 0U,
    /** Send Command via priority queue. */
    I3C_CMD_PRIORITY_IMMEDIATE = 1U
} I3C_CommandPriority;

/** I3C Transmit Mode. Applicable only for private Commands (not CCC). */
typedef enum
{
    /** Transmit single slave CSR address. */
    I3C_CMD_XMIT_MODE_SINGLE_CSR = 0U,
    /** Transmit multi-byte incrementing CSR address. */
    I3C_CMD_XMIT_MODE_MULTI_BYTE_INC = 1U,
    /** Transmit multi-byte static CSR address. */
    I3C_CMD_XMIT_MODE_MULTI_BYTE_STATIC = 2U,
    /** CCC/CSR Address field is not used. Applicable only for Legacy I2C messaging. */
    I3C_CMD_XMIT_MODE_NCA = 3U
} I3C_TransmitMode;

/** I3C Common Command Codes. */
typedef enum
{
    /** Enable Events Command. CCC Type: Broadcast. */
    I3C_CCC_ENEC_BC = (0x00U),
    /** Disable Events Command. CCC Type: Broadcast. */
    I3C_CCC_DISEC_BC = (0x01U),
    /** Enter Activity State 0. CCC Type: Broadcast. */
    I3C_CCC_ENTAS0_BC = (0x02U),
    /** Enter Activity State 1. CCC Type: Broadcast. */
    I3C_CCC_ENTAS1_BC = (0x03U),
    /** Enter Activity State 2. CCC Type: Broadcast. */
    I3C_CCC_ENTAS2_BC = (0x04U),
    /** Enter Activity State 3. CCC Type: Broadcast. */
    I3C_CCC_ENTAS3_BC = (0x05U),
    /** Reset Dynamic Address Assignment. CCC Type: Broadcast. */
    I3C_CCC_RSTDAA_BC = (0x06U),
    /** Enter Dynamic Address Assignment. CCC Type: Broadcast. */
    I3C_CCC_ENTDAA_BC = (0x07U),
    /** Define List of Slaves. CCC Type: Broadcast. */
    I3C_CCC_DEFSLVS_BC = (0x08U),
    /** Set Max Write Length. CCC Type: Broadcast. */
    I3C_CCC_SETMWL_BC = (0x09U),
    /** Set Max Read Length. CCC Type: Broadcast. */
    I3C_CCC_SETMRL_BC = (0x0AU),
    /** Enter Test Mode. CCC Type: Broadcast. */
    I3C_CCC_ENTTM_BC = (0x0BU),
    /** Enter HDR Mode 0. CCC Type: Broadcast. */
    I3C_CCC_ENTHDR0_BC = (0x20U),
    /** Enter HDR Mode 1. CCC Type: Broadcast. */
    I3C_CCC_ENTHDR1_BC = (0x21U),
    /** Enter HDR Mode 2. CCC Type: Broadcast. */
    I3C_CCC_ENTHDR2_BC = (0x22U),
    /** Enter HDR Mode 3. CCC Type: Broadcast. */
    I3C_CCC_ENTHDR3_BC = (0x23U),
    /** Enter HDR Mode 4. CCC Type: Broadcast. */
    I3C_CCC_ENTHDR4_BC = (0x24U),
    /** Enter HDR Mode 5. CCC Type: Broadcast. */
    I3C_CCC_ENTHDR5_BC = (0x25U),
    /** Enter HDR Mode 6. CCC Type: Broadcast. */
    I3C_CCC_ENTHDR6_BC = (0x26U),
    /** Enter HDR Mode 7. CCC Type: Broadcast. */
    I3C_CCC_ENTHDR7_BC = (0x27U),
    /** Exchange Timing Information. CCC Type: Broadcast. */
    I3C_CCC_XTIME_BC = (0x28U),
    /** Enable Events Command. CCC Type: Direct. */
    I3C_CCC_ENEC_DC = (0x80U),
    /** Disable Events Command. CCC Type: Direct. */
    I3C_CCC_DISEC_DC = (0x81U),
    /** Enter Activity State 0. CCC Type: Direct. */
    I3C_CCC_ENTAS0_DC = (0x82U),
    /** Enter Activity State 1. CCC Type: Direct. */
    I3C_CCC_ENTAS1_DC = (0x83U),
    /** Enter Activity State 2. CCC Type: Direct. */
    I3C_CCC_ENTAS2_DC = (0x84U),
    /** Enter Activity State 3. CCC Type: Direct. */
    I3C_CCC_ENTAS3_DC = (0x85U),
    /** Reset Dynamic Address Assignment. CCC Type: Direct. */
    I3C_CCC_RSTDAA_DC = (0x86U),
    /** Set Dynamic Address from Static Address. CCC Type: Direct Set. */
    I3C_CCC_SETDASA_DC = (0x87U),
    /** Set New Dynamic Address. CCC Type: Direct Set. */
    I3C_CCC_SETNEWDA_DC = (0x88U),
    /** Set Max Write Length. CCC Type: Direct Set. */
    I3C_CCC_SETMWL_DC = (0x89U),
    /** Set Max Read Length. CCC Type: Direct Set. */
    I3C_CCC_SETMRL_DC = (0x8AU),
    /** Get Max Write Length. CCC Type: Direct Get. */
    I3C_CCC_GETMWL_DC = (0x8BU),
    /** Get Max Read Length. CCC Type: Direct Get. */
    I3C_CCC_GETMRL_DC = (0x8CU),
    /** Get Provisional ID. CCC Type: Direct Get. */
    I3C_CCC_GETPID_DC = (0x8DU),
    /** Get Bus Characteristics Register. CCC Type: Direct Get. */
    I3C_CCC_GETBCR_DC = (0x8EU),
    /** Get Device Characteristics Register. CCC Type: Direct Get. */
    I3C_CCC_GETDCR_DC = (0x8FU),
    /** Get Device Status. CCC Type: Direct Get. */
    I3C_CCC_GETSTATUS_DC = (0x90U),
    /** Request and Get Accept Mastership. CCC Type: Direct Set. */
    I3C_CCC_GETACCMST_DC = (0x91U),
    /** Set Bridge Targets. CCC Type: Direct Set. */
    I3C_CCC_SETBRGTGT_DC = (0x93U),
    /** Get Max Data Speed (max SDR Mode). CCC Type: Direct Get. */
    I3C_CCC_GETMXDS_DC = (0x94U),
    /** Get HDR Capability (supported HDR Modes). CCC Type: Direct Get. */
    I3C_CCC_GETHDRCAP_DC = (0x95U),
    /** Exchange Timing Information. CCC Type: Direct Get. */
    I3C_CCC_XTIME_DC = (0x98U)
} I3C_CommonCommandCode;

/** High Data Rate Modes for I3C Devices. */
typedef enum
{
    /** Double Data Rate Mode. */
    I3C_HDR_MODE_DDR = (0x00U),
    /** Ternary symbol Pure Bus Mode. */
    I3C_HDR_MODE_TSP = (0x01U),
    /** Ternary symbol legacy Mode. */
    I3C_HDR_MODE_TSL = (0x02U)
} I3C_HdrMode;

/** Slave Events. */
typedef enum
{
    /** Interrupt request Events. */
    I3C_SLAVE_EVENT_INTERRUPT_REQS = (1U << 0U),
    /** Mastership change request Event. */
    I3C_SLAVE_EVENT_MASTERSHIP_REQ = (1U << 1U),
    /** Hot Join Event. */
    I3C_SLAVE_EVENT_HOT_JOIN = (1U << 3U)
} I3C_SlaveEvent;

/** I3C Slave Activity State. */
typedef enum
{
    /** 0 milliseconds - latency-free operation. */
    I3C_ACTIVITY_STATE_0 = (0U),
    /** 1 millisecond. */
    I3C_ACTIVITY_STATE_1 = (1U),
    /** 100 milliseconds. */
    I3C_ACTIVITY_STATE_2 = (2U),
    /** 10 seconds - lowest-activity operation. */
    I3C_ACTIVITY_STATE_3 = (3U)
} I3C_ActivityState;

/** Role of device requesting IBI. */
typedef enum
{
    /** IBI requesting device is Slave. */
    I3C_IBI_REQUESTING_DEV_ROLE_SLAVE = (0U),
    /** IBI requesting device is Master. */
    I3C_IBI_REQUESTING_DEV_ROLE_MASTER = (1U)
} I3C_IbiRequestingDeviceRole;

/** Speed limitation of IBI payload transfer. */
typedef enum
{
    /** IBI payload transfer speed will not be limited. */
    I3C_IBI_DATA_SPEED_NOT_LIMITED = (0U),
    /** IBI payload transfer speed will be limited. */
    I3C_IBI_DATA_SPEED_LIMITED = (1U)
} I3C_IbiDataSpeedLimit;

/** Intended response to IBI from requesing device. */
typedef enum
{
    /** IBI requests will be NACKed */
    I3C_IBI_RESP_NACK = (0U),
    /** IBI requests will be ACKed */
    I3C_IBI_RESP_ACK = (1U)
} I3C_IbiResponse;

/** Intended response to Hot Join request. */
typedef enum
{
    /** Hot Join request will be NACKed. */
    I3C_HJ_RESP_NACK = (0U),
    /** Hot Join request will be ACKed. */
    I3C_HJ_RESP_ACK = (1U)
} I3C_HjResponse;

/** Should slave that is requesting Hot Join be disabled from. */
typedef enum
{
    /** Requesting Hot Join by slave will not be disabled. */
    I3C_HJ_REQ_SLAVE_NOT_DISABLE = (0U),
    /** Requesting Hot Join by slave will be disabled. */
    I3C_HJ_REQ_SLAVE_DISABLE = (1U)
} I3C_HjDisableSet;

/**********************************************************************
 * Callbacks
 **********************************************************************/
/** This type of event refers to events that cause change of Status register value. */
typedef void (*I3C_StatusEvent)(I3C_PrivData* pd, I3C_CommandStatus sts);

/** Interrupt event. */
typedef void (*I3C_InterruptEvent)(I3C_PrivData* pd);

/** This event occurs when all commands from software command buffer ware sent. */
typedef void (*I3C_CommandBufferEmptyEvent)(I3C_PrivData* pd);

/** This event occurs when command is processed without errors. */
typedef void (*I3C_CommandCompleteEvent)(I3C_PrivData* pd);

/** This event occurs when DAA Procedure is finished. */
typedef void (*I3C_DaaCompleteEvent)(I3C_PrivData* pd);

/** This event occurs when DAA Procedure is finished. */
typedef void (*I3C_MastershipRequestEvent)(I3C_PrivData* pd);

/** This event occurs when Hot-Join is finished. */
typedef void (*I3C_HotJoinEvent)(I3C_PrivData* pd);

/** This event occurs when IBI is finished. */
typedef void (*I3C_InbandInterruptEvent)(I3C_PrivData* pd);

/**
 *  @}
 */

/** @defgroup DriverFunctionAPI Driver Function API
 *  Prototypes for the driver API functions. The user application can link statically to the
 *  necessary API functions and call them directly.
 *  @{
 */

/**********************************************************************
 * API methods
 **********************************************************************/

/**
 * Returns the memory requirements for a driver instance.
 * @param[in] config Driver and hardware configuration.
 * @param[out] sysReq Returns the memory requirements for given configuration.
 * @return 0 On success.
 * @return EINVAL If config contains invalid values or not supported configuration.
 */
uint32_t I3C_Probe(const I3C_Config* config, I3C_SysReq* sysReq);

/**
 * Instantiates the I3C Core Driver, given the required blocks of
 * memory (this includes initializing the instance and the underlying
 * hardware). If a client configuration is required (likely to always
 * be true), it is passed in also. Returns an instance pointer, which
 * the client must maintain and pass to all other driver functions.
 * (except probe).
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] config Specifies driver/hardware configuration.
 * @param[in] callbacks Event Handlers and Callbacks.
 * @return 0 On success
 * @return EINVAL If illegal/inconsistent values in 'config' doesn't support feature(s) required by 'config' parameters.
 * @return EIO if operation failed
 */
uint32_t I3C_Init(I3C_PrivData* pd, const I3C_Config* config, const I3C_Callbacks* callbacks);

/**
 * I3C Core Driver's ISR. Platform-specific code is responsible for
 * ensuring this gets called when the corresponding hardware's
 * interrupt is asserted. Registering the ISR should be done after
 * calling init, and before calling start. The driver's ISR will not
 * attempt to lock any locks, but will perform client callbacks. If
 * the client wishes to defer processing to non-interrupt time, it is
 * responsible for doing so. This function must not be called after
 * calling destroy and releasing private data memory.
 * @param[in] pd Pointer to driver's private data object filled by init.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_Isr(I3C_PrivData* pd);

/**
 * Start the I3C driver, enabling interrupts. This is called after the
 * client has successfully initialized the driver and hooked the
 * driver's ISR (the isr member of this struct) to the IRQ.
 * @param[in] pd Pointer to driver's private data object.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_Start(I3C_PrivData* pd);

/**
 * The client may call this to disable the hardware (disabling its IRQ
 * at the source and disconnecting it if applicable). Also, a best-
 * effort is made to cancel any pending transactions.
 * @param[in] pd Pointer to driver's private data object.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_Stop(I3C_PrivData* pd);

/**
 * This performs an automatic stop and then de-initializes the driver.
 * The client may not make further requests on this instance.
 * @param[in] pd Pointer to driver's private data object.
 * @return 0 on success
 * @return EIO if operation failed
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_Destroy(I3C_PrivData* pd);

/**
 * Enables the I3C Master Core.
 * @param[in] pd Pointer to driver's private data object.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_EnableCore(I3C_PrivData* pd);

/**
 * Waits for all operations to be completed and then it disables the
 * I3C Master Core.
 * @param[in] pd Pointer to driver's private data object.
 * @return 0 on success
 * @return EIO if operation failed
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_DisableCore(I3C_PrivData* pd);

/**
 * Sets Bus Mode.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] mode Bus Mode.
 * @return 0 on success
 * @return EIO if operation failed
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_SetBusMode(I3C_PrivData* pd, I3C_BusMode mode);

/**
 * Reads Bus Mode.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] mode Pointer to Bus Mode object.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_GetBusMode(I3C_PrivData* pd, I3C_BusMode* mode);

/**
 * Sets BCR value in one of the retaining registers (DeviceIDx).
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] bcr BCR value.
 * @param[in] devId Retaining register number.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_SetBcr(I3C_PrivData* pd, uint8_t bcr, uint8_t devId);

/**
 * Sets DCR value in one of the retaining registers (DeviceIDx).
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] dcr DCR value.
 * @param[in] devId Retaining register number.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_SetDcr(I3C_PrivData* pd, uint8_t dcr, uint8_t devId);

/**
 * Sets Provisional ID value in one of the retaining registers
 * (DeviceIDx).
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] pid Ponter to buffer containing 48-bit Provisional ID value.
 * @param[in] devId Retaining register number.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_SetPid(I3C_PrivData* pd, const uint8_t* pid, uint8_t devId);

/**
 * Configures Devices connected to the I3C Bus (including Master)
 * accordingly to the capabilities of these Devices. Configuration of
 * the Devices must be provided in following order: Master, I3C Slaves
 * with Dynamic Address support, I3C Slaves with static addresses and
 * at the end Legacy I2C Devices. Position of the Device configuration
 * in each group determines priority of the Device. Configuration of
 * at least two Devices (including Master) need to be provided. I3C
 * Master Core must be disabled before calling this function.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] devs Pointer to the structure containing configuration and capabilities of the Devices.
 * @param[in] numDevs Total number of the Devices.
 * @return 0 on success
 * @return EIO if operation failed
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_ConfigureDevices(I3C_PrivData* pd, I3C_DeviceDescriptor* devs, uint8_t numDevs);

/**
 * Configures Device connected to the I3C Bus (including Master)
 * accordingly to the capabilities of this device. I3C Master Core
 * must be disabled before calling this function.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] dev Pointer to the structure containing configuration and capabilities of the Device.
 * @param[in] index Index of the device corresponding to index of devs array (field of the I3C_PrivData structure).
 * @return 0 on success
 * @return EIO if operation failed
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_ConfigureDevice(I3C_PrivData* pd, I3C_DeviceDescriptor* dev, uint8_t index);

/**
 * Calculates prescaler values for Legacy I2C and SDR frequencies.
 * Please make sure core is disabled.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] config Specifies driver/hardware configuration.
 * @return 0 on success
 * @return EIO if operation failed
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_ConfigurePrescalers(I3C_PrivData* pd, const I3C_Config* config);

/**
 * This function clears retaining registers set for a chosen device.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] device_no Number corresponding to Device position in retaining register.
 * @return 0 on success
 * @return EIO if operation failed
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_ClearRrOfDevice(I3C_PrivData* pd, uint8_t device_no);

/**
 * Returns devices which joined (by HJ) the bus since last check.
 * @param[in] pd Pointer to driver's private data object.
 * @param[out] hj_devices This mask indicates which devices joined the bus.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_GetNewDevices(I3C_PrivData* pd, uint16_t* hj_devices);

/**
 * Enables interrupts chosen by user.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] interruptConfig Interrupt configuration.
 * @return 0 on success
 * @return EIO if operation failed
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_ConfigureInterrupts(I3C_PrivData* pd, const I3C_InterruptConfig* interruptConfig);

/**
 * Sets thresholds to levels selected by user.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] thresholdConfig Threshold configuration.
 * @return 0 on success
 * @return EIO if operation failed
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_ConfigureThresholds(I3C_PrivData* pd, const I3C_ThresholdConfig* thresholdConfig);

/**
 * Executes all queued Commands.
 * @param[in] pd Pointer to driver's private data object.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 * @return EBUSY if Commands are already being processed
 * @return EOPNOTSUPP if Command list is empty
 */
uint32_t I3C_CmdExec(I3C_PrivData* pd);

/**
 * Executes Immediate Command.
 * @param[in] pd Pointer to driver's private data object.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_CmdExecImmediate(I3C_PrivData* pd);

/**
 * Enables Manual Command Start.
 * @param[in] pd Pointer to driver's private data object.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_EnableMcs(I3C_PrivData* pd);

/**
 * Disabled Manual Command Start.
 * @param[in] pd Pointer to driver's private data object.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_DisableMcs(I3C_PrivData* pd);

/**
 * Executes commands stored in HW.
 * @param[in] pd Pointer to driver's private data object.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_ManualCommandStart(I3C_PrivData* pd);

/**
 * Returns number of queued Commands.
 * @param[in] pd Pointer to driver's private data object.
 * @return 0 if input parameters are invalid or if there are no queued Commands
 */
uint32_t I3C_CmdCount(const I3C_PrivData* pd);

/**
 * Returns maximum number of Commands that can be put in queue.
 * @param[in] pd Pointer to driver's private data object.
 * @return 0 if input parameters are invalid or if there are no queued Commands
 */
uint32_t I3C_CmdMaxCount(const I3C_PrivData* pd);

/**
 * Clears Commands queue.
 * @param[in] pd Pointer to driver's private data object.
 * @return EINVAL if input parameters are invalid
 * @return 0 on success
 */
uint32_t I3C_CmdClearAll(I3C_PrivData* pd);

/**
 * Prints info on I3C devices on the bus.
 * @param[in] pd Pointer to driver's private data object.
 * @return EINVAL if input parameters are invalid
 * @return 0 on success
 */
uint32_t I3C_DevPrint(const I3C_PrivData* pd);

/**
 * Adds SDR Mode private Write Command to the Commands queue.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] da Device Address to which data will be transferred.
 * @param[in] csr_data CSR information with CSR address and address lenght.
 * @param[in] payload_data Pointer to structure with payload information
 * @param[in] xmitMmode Transmission Mode.
 * @return EINVAL if input parameters are invalid
 * @return EBUSY if there is no space for new Command or payload
 * @return 0 on success
 */
uint32_t I3C_CmdAddPrivWrite(I3C_PrivData* pd, uint16_t da, const I3C_CsrData* csr_data, const I3C_PayloadData* payload_data, I3C_TransmitMode xmitMmode);

/**
 * Adds SDR Mode private Read Command to the Commands queue.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] da Device Address from which data will be transferred.
 * @param[in] csr_data CSR information with CSR address and address lenght.
 * @param[in] payload_data Pointer to structure with payload information
 * @param[in] xmitMmode Transmission Mode.
 * @return EINVAL if input parameters are invalid
 * @return EBUSY if there is no space for new Command or payload
 * @return 0 on success
 */
uint32_t I3C_CmdAddPrivRead(I3C_PrivData* pd, uint16_t da, const I3C_CsrData* csr_data, const I3C_PayloadData* payload_data, I3C_TransmitMode xmitMmode);

/**
 * Adds Legacy I2C SDR Mode private Write Command to the Commands
 * queue.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] da Device Address to which data will be transferred.
 * @param[in] payload Pointer to buffer containing payload to be sent.
 * @param[in] payloadSize Size of the payload [B].
 * @return EINVAL if input parameters are invalid
 * @return EBUSY if there is no space for new Command or payload
 * @return 0 on success
 */
uint32_t I3C_CmdAddPrivI2CWrite(I3C_PrivData* pd, uint16_t da, uint8_t* payload, uint16_t payloadSize);

/**
 * Adds Legacy I2C SDR Mode private Read Command to the Commands
 * queue.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] da Device Address from which data will be transferred.
 * @param[in] payload Pointer to buffer containing payload to be sent.
 * @param[in] payloadSize Size of the payload [B].
 * @return EINVAL if input parameters are invalid
 * @return EBUSY if there is no space for new Command or payload
 * @return 0 on success
 */
uint32_t I3C_CmdAddPrivI2CRead(I3C_PrivData* pd, uint16_t da, uint8_t* payload, uint16_t payloadSize);

/**
 * Adds DDR Mode private Write Command to the Commands queue. I3C Core
 * Driver will automatically calculate CRC, Preambles and Parity Bits
 * for the DDR Command and its Payload.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] da Device Address to which data will be transferred.
 * @param[in] command DDR Command.
 * @param[in] hdrMode Specifies which one of the HDR modes to use.
 * @param[in] payload_data Pointer to structure with payload information
 * @return EINVAL if input parameters are invalid
 * @return EBUSY if there is no space for new Command or payload
 * @return 0 on success
 */
uint32_t I3C_CmdAddDdrWrite(I3C_PrivData* pd, uint8_t da, uint8_t command, I3C_HdrMode hdrMode, I3C_PayloadData* payload_data);

/**
 * Adds DDR Mode private Read Command to the Commands queue. I3C Core
 * Driver will automatically check CRC, Preambles and Parity Bits for
 * the DDR Command and its Payload.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] da Device Address from which data will be transferred.
 * @param[in] command DDR Command.
 * @param[in] hdrMode Specifies which one of the HDR modes to use.
 * @param[in] payload_data Pointer to structure with payload information
 * @return EINVAL if input parameters are invalid
 * @return EBUSY if there is no space for new Command or payload
 * @return 0 on success
 */
uint32_t I3C_CmdAddDdrRead(I3C_PrivData* pd, uint8_t da, uint8_t command, I3C_HdrMode hdrMode, I3C_PayloadData* payload_data);

/**
 * Adds to queue Command to that sends message to I3C Device (or to
 * all I3C Devices in case of a broadcast) which event interrupts
 * should be enabled or disabled.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] eventsMask Mask of Events to be enabled or disabled.
 * @param[in] enable If true, specified events will be enabled; if false, specified events will be disabled.
 * @param[in] devAddr Address of the I3C Device in case of Direct Message.
 * @param[in] tx_params Transmission parameters - command descriptors
 * @return 0 on success
 * @return EPROTO if slave event mask is not valid
 * @return EINVAL if input parameters are invalid
 * @return EBUSY if there is no space for new Command or payload
 */
uint32_t I3C_CmdAddSetSlaveEvents(I3C_PrivData* pd, uint8_t eventsMask, bool enable, uint8_t devAddr, const I3C_TransmissionParameters* tx_params);

/**
 * Creates Enter Activity State Command to be sent to the I3C Slaves.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] state Activity State value.
 * @param[in] devAddr Address of the I3C Device in case of Direct Message.
 * @param[in] tx_params Transmission parameters - command descriptor
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 * @return EBUSY if there is no space for new Command or payload
 */
uint32_t I3C_CmdAddEnterActivityState(I3C_PrivData* pd, I3C_ActivityState state, uint8_t devAddr, const I3C_TransmissionParameters* tx_params);

/**
 * This function adds CCC for Dynamic Address Assignment process to
 * the Commands queue.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] devAddr Address of the I3C Device in case of Direct Command. This parameter is ignored in case of Broadcast Command.
 * @param[in] tx_params Transmission parameters - command descriptor
 * @return 0 on success
 * @return EBUSY if Commands are being processed by the Core Driver
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_CmdAddResetDaa(I3C_PrivData* pd, uint8_t devAddr, const I3C_TransmissionParameters* tx_params);

/**
 * This function adds CCC for Dynamic Address Assignment process to
 * the Commands queue.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] tx_params Transmission parameters - command descriptor
 * @return 0 on success
 * @return EBUSY if Commands are being processed by the Core Driver
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_CmdAddEnterDaa(I3C_PrivData* pd, const I3C_TransmissionParameters* tx_params);

/**
 * This function adds CCC for setting the Maximum Write Length for all
 * or specific device.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] length Maximum Write Length to be set.
 * @param[in] devAddr Address of the I3C Device in case of Direct Message.
 * @param[in] tx_params Transmission parameters - command descriptor
 * @return 0 on success
 * @return EBUSY if Commands are being processed by the Core Driver
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_CmdAddSetMaxWriteLength(I3C_PrivData* pd, uint16_t length, uint8_t devAddr, const I3C_TransmissionParameters* tx_params);

/**
 * This function adds CCC for obtaining the Maximum Write Length value
 * for a specific device.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] length Pointer to 16-bit variable to which Maximum Write Length will be written.
 * @param[in] devAddr Address of the I3C Device to which Command will be sent.
 * @param[in] tx_params Transmission parameters - command descriptor
 * @return 0 on success
 * @return EBUSY if Commands are being processed by the Core Driver
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_CmdAddGetMaxWriteLength(I3C_PrivData* pd, uint16_t* length, uint8_t devAddr, const I3C_TransmissionParameters* tx_params);

/**
 * This function adds CCC for setting the Maximum Read Length for all
 * or specific device.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] max_read_length Maximum Read Length to be set.
 * @param[in] length_size Number of bytes required to store Maximum Read Length. Alloved values are 2 or 3.
 * @param[in] devAddr Address of the I3C Device in case of Direct Message.
 * @param[in] tx_params Transmission parameters - command descriptors
 * @return 0 on success
 * @return EBUSY if Commands are being processed by the Core Driver
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_CmdAddSetMaxReadLength(I3C_PrivData* pd, I3C_MaxReadLength max_read_length, uint8_t length_size, uint8_t devAddr, const I3C_TransmissionParameters* tx_params);

/**
 * This function adds CCC for obtaining the Maximum Read Length value
 * for a specific device.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] max_read_length Pointer to a structure where Maximum Read Length.
 * @param[in] length_size Number of bytes required to store Maximum Read Length. Alloved values are 2 or 3.
 * @param[in] devAddr Address of the I3C Device to which Command will be sent.
 * @param[in] tx_params Transmission parameters - command descriptor
 * @return 0 on success
 * @return EBUSY if Commands are being processed by the Core Driver
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_CmdAddGetMaxReadLength(I3C_PrivData* pd, I3C_MaxReadLength* max_read_length, uint8_t length_size, uint8_t devAddr, const I3C_TransmissionParameters* tx_params);

/**
 * This function adds CCC for obtaining the Maximum Data Speed (SCL
 * Frequency of the slave)
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] max_data_speed Pointer to structure with max data speeds.
 * @param[in] devAddr Address of the I3C Device to which Command will be sent.
 * @param[in] tx_params Transmission parameters - command descriptor
 * @return 0 on success
 * @return EBUSY if Commands are being processed by the Core Driver
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_CmdAddGetMaxDataSpeed(I3C_PrivData* pd, I3C_MaxDataSpeed* max_data_speed, uint8_t devAddr, const I3C_TransmissionParameters* tx_params);

/**
 * This function gets information about Slaves present on the bus.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] slaveDescs Pointer the structure of Slave descriptors. Memory will be written with slave count and slave data.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_GetSlavesList(const I3C_PrivData* pd, I3C_SlaveDescriptors* slaveDescs);

/**
 * This function adds CCC for informing Secondary Masters about Slaves
 * present on the bus.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] slaveDescs Pointer the structure of Slave descriptors.
 * @param[in] tx_params Transmission parameters - command descriptor
 * @return 0 on success
 * @return EBUSY if Commands are being processed by the Core Driver
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_CmdAddDefineSlavesList(I3C_PrivData* pd, I3C_SlaveDescriptors* slaveDescs, const I3C_TransmissionParameters* tx_params);

/**
 * Prepares CCC broadcast Command for all I3C Devices that the Master
 * is entering specific Test Mode.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] mode Test mode code. 0x00 - Exit Test Mode. 0x01 - Vendor Test Mode. 0x02 - General Test Mode. 0x03-0x0F - MIPI Reserved. 0x10-0xFF - Vendor Definable.
 * @param[in] tx_params Transmission parameters - command descriptor
 * @return 0 on success
 * @return EBUSY if Commands are being processed by the Core Driver
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_CmdAddEnterTestMode(I3C_PrivData* pd, uint8_t mode, const I3C_TransmissionParameters* tx_params);

/**
 * Sends broadcast message to all I3C Devices that the Bus is being
 * switched over to the indicated HDR Mode.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] mode
 * @param[in] tx_params Transmission parameters - command descriptor
 * @return 0 on success
 * @return EBUSY if Commands are being processed by the Core Driver
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_CmdAddEnterHdrMode(I3C_PrivData* pd, I3C_HdrMode mode, const I3C_TransmissionParameters* tx_params);

/**
 * Sends Command that addresses specific Device with Dynamic Address
 * using its I2C Address.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] address Dynamic Address to be set.
 * @param[in] devAddr Address of the I3C Device to which Command will be sent.
 * @param[in] tx_params Transmission parameters - command descriptor
 * @return 0 on success
 * @return EBUSY if Commands are being processed by the Core Driver
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_CmdSetDaFromSa(I3C_PrivData* pd, uint8_t address, uint8_t devAddr, const I3C_TransmissionParameters* tx_params);

/**
 * Sends Command that sets a new Dynamic Address for specific I3C
 * Slave Device.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] da Dynamic Address to be set.
 * @param[in] devAddr Address of the I3C Device to which Command will be sent.
 * @param[in] tx_params Transmission parameters - command descriptor
 * @return 0 on success
 * @return EBUSY if Commands are being processed by the Core Driver
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_CmdAddSetNewDa(I3C_PrivData* pd, uint8_t da, uint8_t devAddr, const I3C_TransmissionParameters* tx_params);

/**
 * This function adds CCC for obtaining provisional ID of device.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] buff Pointer to variable to which 48-bit Provisional ID will be written.
 * @param[in] devAddr Address of the I3C Device to which Command will be sent.
 * @param[in] tx_params Transmission parameters - command descriptor
 * @return 0 on success
 * @return EBUSY if Commands are being processed by the Core Driver
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_CmdAddGetProvisionalId(I3C_PrivData* pd, uint8_t* buff, uint8_t devAddr, const I3C_TransmissionParameters* tx_params);

/**
 * This function adds CCC for obtaining the Bus Characteristics
 * Register.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] bcr Pointer to variable to which 8-bit DCR value will be written.
 * @param[in] devAddr Address of the I3C Device to which Command will be sent.
 * @param[in] tx_params Transmission parameters - command descriptor
 * @return 0 on success
 * @return EBUSY if Commands are being processed by the Core Driver
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_CmdAddGetBcr(I3C_PrivData* pd, uint8_t* bcr, uint8_t devAddr, const I3C_TransmissionParameters* tx_params);

/**
 * This function adds CCC for obtaining the Device Characteristics
 * Register.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] dcr Pointer to variable to which 8-bit BCR value will be written.
 * @param[in] devAddr Address of the I3C Device to which Command will be sent.
 * @param[in] tx_params Transmission parameters - command descriptor
 * @return 0 on success
 * @return EBUSY if Commands are being processed by the Core Driver
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_CmdAddGetDcr(I3C_PrivData* pd, uint8_t* dcr, uint8_t devAddr, const I3C_TransmissionParameters* tx_params);

/**
 * This function adds CCC for obtaining status of the I3C Device.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] status Pointer to variable to which 16-bit status value will be written.
 * @param[in] devAddr Address of the I3C Device to which Command will be sent.
 * @param[in] tx_params Transmission parameters - command descriptor
 * @return 0 on success
 * @return EBUSY if Commands are being processed by the Core Driver
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_CmdAddGetStatus(I3C_PrivData* pd, uint16_t* status, uint8_t devAddr, const I3C_TransmissionParameters* tx_params);

/**
 * This function adds CCC for granting Mastership.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] recData Pointer to data received from device that GETACCMST CCC is sent to. After command execution this should hold address of receiving device.
 * @param[in] devAddr Address of the I3C Device to which Command will be sent.
 * @param[in] tx_params Transmission parameters - command descriptor
 * @return 0 on success
 * @return EBUSY if Commands are being processed by the Core Driver
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_CmdAddGetAccMst(I3C_PrivData* pd, uint8_t* recData, uint8_t devAddr, const I3C_TransmissionParameters* tx_params);

/**
 * This function configures devices issuing IBI.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] ibi_sir_cfg Pointer to array that will hold configuration of IBI related devices.
 * @param[in] num_sir_cfgs Size of configuration array.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_IbiConfigureDevices(const I3C_PrivData* pd, const I3C_IbiSirConfiguration* ibi_sir_cfg, uint32_t num_sir_cfgs);

/**
 * This function modifies device configuration issuing IBI.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] ibi_sir_cfg Pointer to struct that holds configuration of IBI device.
 * @param[in] da Device DA.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 * @return ENOENT if device with DA not found
 */
uint32_t I3C_IbiModifyDeviceConfig(const I3C_PrivData* pd, const I3C_IbiSirConfiguration* ibi_sir_cfg, uint8_t da);

/**
 * This function extracts address of device that issued IBI.
 * @param[in] pd Pointer to driver's private data object.
 * @return 0 on error
 * @return valid I3C address on success
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_IbiGetAddressOfIssuer(I3C_PrivData* pd);

/**
 * This function extracts IBI payload.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] buf Pointer to buffer that will hold received payload.
 * @param[in] payload_size Size of payload.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_IbiGetData(I3C_PrivData* pd, uint8_t* buf, uint16_t payload_size);

/**
 * This function configures response of I3C conftroller to Hot Join
 * request.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] hj_response Should I3C controller ACK or NACK Hot Join request.
 * @param[in] hj_disable_set Should I3C controller disable slaves from requesting Hot Join.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_HjConfigureResponse(I3C_PrivData* pd, I3C_HjResponse hj_response, I3C_HjDisableSet hj_disable_set);

/**
 * Enables chosen interrupts in slave mode.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] slaveInterruptConfig Interrupt configuration.
 * @return 0 on success
 * @return EIO if operation failed
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_ConfigureSlaveInterrupts(I3C_PrivData* pd, const I3C_SlaveInterruptConfig* slaveInterruptConfig);

/**
 * Configures Hardware and Software to operate in Slave mode.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] slaveInterruptConfig Pointer to slave mode interrupt configuration.
 * @return 0 on success
 * @return EIO if operation failed
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_SlaveModeConfigure(I3C_PrivData* pd, const I3C_SlaveInterruptConfig* slaveInterruptConfig);

/**
 * Adds SDR Read request to the queue.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] buf Pointer to buffer where data will be stored.
 * @param[in] num_bytes Number of bytes to read.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_SlaveModeReqSdrRead(I3C_PrivData* pd, uint8_t* buf, uint16_t num_bytes);

/**
 * Adds SDR Write request to the queue.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] buf Pointer to buffer with data to be sent.
 * @param[in] num_bytes Number of bytes to send.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_SlaveModeReqSdrWrite(I3C_PrivData* pd, const uint8_t* buf, uint16_t num_bytes);

/**
 * Adds DDR Read request to the queue.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] buf Pointer to buffer where data will be stored.
 * @param[in] num_words Number of bytes to read.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 * @return ENOENT if no DDR data is available for read
 * @return EPROTO if reveived command code is not to DDR Write
 */
uint32_t I3C_SlaveModeReqDdrRead(I3C_PrivData* pd, uint16_t* buf, uint16_t num_words);

/**
 * Adds DDR Write request to the queue.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] buf_in Pointer to buffer with data to be sent.
 * @param[in] num_words_in Number of 16-bit words to send.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 * @return ENOTSUP if DDR FIFOs are not implemented
 */
uint32_t I3C_SlaveModeReqDdrWrite(I3C_PrivData* pd, const uint16_t* buf_in, uint16_t num_words_in);

/**
 * Sends Hot Join request.
 * @param[in] pd Pointer to driver's private data object.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_SlaveModeRequestHotJoin(I3C_PrivData* pd);

/**
 * Sends Mastership request.
 * @param[in] pd Pointer to driver's private data object.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_SlaveModeMastershipReq(I3C_PrivData* pd);

/**
 * Retrieves ASF information from I3C controller.
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] asf_info Pointer to ASF information structure.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_GetAsfInfo(I3C_PrivData* pd, I3C_AsfInfo* asf_info);

/**
 * Checks controller operation mode
 * @param[in] pd Pointer to driver's private data object.
 * @param[out] opMode Pointer to OperationMode structure.
 * @return 0 on success
 * @return EINVAL if input parameters are invalid
 */
uint32_t I3C_CheckOperationMode(const I3C_PrivData* pd, I3C_OperationMode* opMode);

/**
 *  @}
 */


#endif	/* I3C_IF_H */
