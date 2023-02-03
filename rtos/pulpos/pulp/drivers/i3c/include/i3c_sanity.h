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

/* parasoft-begin-suppress METRICS-18-3 "Follow the Cyclomatic Complexity limit of 10" */
/* parasoft-begin-suppress METRICS-36-3 "A function should not be called from more than 5 different functions" */
/* parasoft-begin-suppress METRICS-39-3 "The value of VOCF metric for a function should not be higher than 4" */
/* parasoft-begin-suppress METRICS-41-3 "Number of blocks of comments per statement" */

/**
 * This file contains sanity API functions. The purpose of sanity functions
 * is to check input parameters validity. They take the same parameters as
 * original API functions and return 0 on success or CDN_EINVAL on wrong parameter
 * value(s).
 */

#ifndef I3C_SANITY_H
#define I3C_SANITY_H

#include "cdn_errno.h"
#include "cdn_stdtypes.h"
#include "i3c_if.h"

static inline uint32_t I3C_AsfInfoSF(const I3C_AsfInfo *obj);
static inline uint32_t I3C_CallbacksSF(const I3C_Callbacks *obj);
static inline uint32_t I3C_ConfigSF(const I3C_Config *obj);
static inline uint32_t I3C_CsrDataSF(const I3C_CsrData *obj);
static inline uint32_t I3C_DeviceDescriptorSF(const I3C_DeviceDescriptor *obj);
static inline uint32_t I3C_IbiSirConfigurationSF(const I3C_IbiSirConfiguration *obj);
static inline uint32_t I3C_InterruptConfigSF(const I3C_InterruptConfig *obj);
static inline uint32_t I3C_MaxDataSpeedSF(const I3C_MaxDataSpeed *obj);
static inline uint32_t I3C_MaxReadLengthSF(const I3C_MaxReadLength *obj);
static inline uint32_t I3C_PayloadDataSF(const I3C_PayloadData *obj);
static inline uint32_t I3C_PrivDataSF(const I3C_PrivData *obj);
static inline uint32_t I3C_SlaveDescriptorsSF(const I3C_SlaveDescriptors *obj);
static inline uint32_t I3C_SlaveInterruptConfigSF(const I3C_SlaveInterruptConfig *obj);
static inline uint32_t I3C_ThresholdConfigSF(const I3C_ThresholdConfig *obj);
static inline uint32_t I3C_TransmissionParametersSF(const I3C_TransmissionParameters *obj);

static inline uint32_t I3C_SanityFunction1(const I3C_Config* config, const I3C_SysReq* sysReq);
static inline uint32_t I3C_SanityFunction2(const I3C_PrivData* pd, const I3C_Config* config, const I3C_Callbacks* callbacks);
static inline uint32_t I3C_SanityFunction3(const I3C_PrivData* pd);
static inline uint32_t I3C_SanityFunction9(const I3C_PrivData* pd, const I3C_BusMode mode);
static inline uint32_t I3C_SanityFunction10(const I3C_PrivData* pd, const I3C_BusMode* mode);
static inline uint32_t I3C_SanityFunction13(const I3C_PrivData* pd, const uint8_t* pid);
static inline uint32_t I3C_SanityFunction14(const I3C_PrivData* pd, const I3C_DeviceDescriptor* devs);
static inline uint32_t I3C_SanityFunction16(const I3C_PrivData* pd, const I3C_Config* config);
static inline uint32_t I3C_SanityFunction18(const I3C_PrivData* pd, const uint16_t* hj_devices);
static inline uint32_t I3C_SanityFunction19(const I3C_PrivData* pd, const I3C_InterruptConfig* interruptConfig);
static inline uint32_t I3C_SanityFunction20(const I3C_PrivData* pd, const I3C_ThresholdConfig* thresholdConfig);
static inline uint32_t I3C_SanityFunction30(const I3C_PrivData* pd, const I3C_CsrData* csr_data, const I3C_PayloadData* payload_data, const I3C_TransmitMode xmitMmode);
static inline uint32_t I3C_SanityFunction34(const I3C_PrivData* pd, const I3C_HdrMode hdrMode, const I3C_PayloadData* payload_data);
static inline uint32_t I3C_SanityFunction36(const I3C_PrivData* pd, const I3C_TransmissionParameters* tx_params);
static inline uint32_t I3C_SanityFunction37(const I3C_PrivData* pd, const I3C_ActivityState state, const I3C_TransmissionParameters* tx_params);
static inline uint32_t I3C_SanityFunction41(const I3C_PrivData* pd, const uint16_t* length, const I3C_TransmissionParameters* tx_params);
static inline uint32_t I3C_SanityFunction43(const I3C_PrivData* pd, const I3C_MaxReadLength* max_read_length, const I3C_TransmissionParameters* tx_params);
static inline uint32_t I3C_SanityFunction44(const I3C_PrivData* pd, const I3C_MaxDataSpeed* max_data_speed, const I3C_TransmissionParameters* tx_params);
static inline uint32_t I3C_SanityFunction45(const I3C_PrivData* pd, const I3C_SlaveDescriptors* slaveDescs);
static inline uint32_t I3C_SanityFunction46(const I3C_PrivData* pd, const I3C_SlaveDescriptors* slaveDescs, const I3C_TransmissionParameters* tx_params);
static inline uint32_t I3C_SanityFunction48(const I3C_PrivData* pd, const I3C_HdrMode mode, const I3C_TransmissionParameters* tx_params);
static inline uint32_t I3C_SanityFunction51(const I3C_PrivData* pd, const uint8_t* buff, const I3C_TransmissionParameters* tx_params);
static inline uint32_t I3C_SanityFunction56(const I3C_PrivData* pd, const I3C_IbiSirConfiguration* ibi_sir_cfg);
static inline uint32_t I3C_SanityFunction60(const I3C_PrivData* pd, const I3C_HjResponse hj_response, const I3C_HjDisableSet hj_disable_set);
static inline uint32_t I3C_SanityFunction61(const I3C_PrivData* pd, const I3C_SlaveInterruptConfig* slaveInterruptConfig);
static inline uint32_t I3C_SanityFunction69(const I3C_PrivData* pd, const I3C_AsfInfo* asf_info);
static inline uint32_t I3C_SanityFunction70(const I3C_PrivData* pd, const I3C_OperationMode* opMode);

#define	I3C_ProbeSF I3C_SanityFunction1
#define	I3C_InitSF I3C_SanityFunction2
#define	I3C_IsrSF I3C_SanityFunction3
#define	I3C_StartSF I3C_SanityFunction3
#define	I3C_StopSF I3C_SanityFunction3
#define	I3C_DestroySF I3C_SanityFunction3
#define	I3C_EnableCoreSF I3C_SanityFunction3
#define	I3C_DisableCoreSF I3C_SanityFunction3
#define	I3C_SetBusModeSF I3C_SanityFunction9
#define	I3C_GetBusModeSF I3C_SanityFunction10
#define	I3C_SetBcrSF I3C_SanityFunction3
#define	I3C_SetDcrSF I3C_SanityFunction3
#define	I3C_SetPidSF I3C_SanityFunction13
#define	I3C_ConfigureDevicesSF I3C_SanityFunction14
#define	I3C_ConfigureDeviceSF I3C_SanityFunction14
#define	I3C_ConfigurePrescalersSF I3C_SanityFunction16
#define	I3C_ClearRrOfDeviceSF I3C_SanityFunction3
#define	I3C_GetNewDevicesSF I3C_SanityFunction18
#define	I3C_ConfigureInterruptsSF I3C_SanityFunction19
#define	I3C_ConfigureThresholdsSF I3C_SanityFunction20
#define	I3C_CmdExecSF I3C_SanityFunction3
#define	I3C_CmdExecImmediateSF I3C_SanityFunction3
#define	I3C_EnableMcsSF I3C_SanityFunction3
#define	I3C_DisableMcsSF I3C_SanityFunction3
#define	I3C_ManualCommandStartSF I3C_SanityFunction3
#define	I3C_CmdCountSF I3C_SanityFunction3
#define	I3C_CmdMaxCountSF I3C_SanityFunction3
#define	I3C_CmdClearAllSF I3C_SanityFunction3
#define	I3C_DevPrintSF I3C_SanityFunction3
#define	I3C_CmdAddPrivWriteSF I3C_SanityFunction30
#define	I3C_CmdAddPrivReadSF I3C_SanityFunction30
#define	I3C_CmdAddPrivI2CWriteSF I3C_SanityFunction13
#define	I3C_CmdAddPrivI2CReadSF I3C_SanityFunction13
#define	I3C_CmdAddDdrWriteSF I3C_SanityFunction34
#define	I3C_CmdAddDdrReadSF I3C_SanityFunction34
#define	I3C_CmdAddSetSlaveEventsSF I3C_SanityFunction36
#define	I3C_CmdAddEnterActivityStateSF I3C_SanityFunction37
#define	I3C_CmdAddResetDaaSF I3C_SanityFunction36
#define	I3C_CmdAddEnterDaaSF I3C_SanityFunction36
#define	I3C_CmdAddSetMaxWriteLengthSF I3C_SanityFunction36
#define	I3C_CmdAddGetMaxWriteLengthSF I3C_SanityFunction41
#define	I3C_CmdAddSetMaxReadLengthSF I3C_SanityFunction36
#define	I3C_CmdAddGetMaxReadLengthSF I3C_SanityFunction43
#define	I3C_CmdAddGetMaxDataSpeedSF I3C_SanityFunction44
#define	I3C_GetSlavesListSF I3C_SanityFunction45
#define	I3C_CmdAddDefineSlavesListSF I3C_SanityFunction46
#define	I3C_CmdAddEnterTestModeSF I3C_SanityFunction36
#define	I3C_CmdAddEnterHdrModeSF I3C_SanityFunction48
#define	I3C_CmdSetDaFromSaSF I3C_SanityFunction36
#define	I3C_CmdAddSetNewDaSF I3C_SanityFunction36
#define	I3C_CmdAddGetProvisionalIdSF I3C_SanityFunction51
#define	I3C_CmdAddGetBcrSF I3C_SanityFunction51
#define	I3C_CmdAddGetDcrSF I3C_SanityFunction51
#define	I3C_CmdAddGetStatusSF I3C_SanityFunction41
#define	I3C_CmdAddGetAccMstSF I3C_SanityFunction51
#define	I3C_IbiConfigureDevicesSF I3C_SanityFunction56
#define	I3C_IbiModifyDeviceConfigSF I3C_SanityFunction56
#define	I3C_IbiGetAddressOfIssuerSF I3C_SanityFunction3
#define	I3C_IbiGetDataSF I3C_SanityFunction13
#define	I3C_HjConfigureResponseSF I3C_SanityFunction60
#define	I3C_ConfigureSlaveInterruptsSF I3C_SanityFunction61
#define	I3C_SlaveModeConfigureSF I3C_SanityFunction61
#define	I3C_SlaveModeReqSdrReadSF I3C_SanityFunction13
#define	I3C_SlaveModeReqSdrWriteSF I3C_SanityFunction13
#define	I3C_SlaveModeReqDdrReadSF I3C_SanityFunction18
#define	I3C_SlaveModeReqDdrWriteSF I3C_SanityFunction18
#define	I3C_SlaveModeRequestHotJoinSF I3C_SanityFunction3
#define	I3C_SlaveModeMastershipReqSF I3C_SanityFunction3
#define	I3C_GetAsfInfoSF I3C_SanityFunction69
#define	I3C_CheckOperationModeSF I3C_SanityFunction70


/**
 * Function to validate struct InterruptConfig
 *
 * @param[in] obj pointer to struct to be verified
 * @returns 0 for valid
 * @returns CDN_EINVAL for invalid
 */
static inline uint32_t I3C_InterruptConfigSF(const I3C_InterruptConfig *obj)
{
    uint32_t ret = 0;

    if (obj == NULL)
    {
        ret = CDN_EINVAL;
    }

    return ret;
}


/**
 * Function to validate struct SlaveInterruptConfig
 *
 * @param[in] obj pointer to struct to be verified
 * @returns 0 for valid
 * @returns CDN_EINVAL for invalid
 */
static inline uint32_t I3C_SlaveInterruptConfigSF(const I3C_SlaveInterruptConfig *obj)
{
    uint32_t ret = 0;

    if (obj == NULL)
    {
        ret = CDN_EINVAL;
    }

    return ret;
}


/**
 * Function to validate struct ThresholdConfig
 *
 * @param[in] obj pointer to struct to be verified
 * @returns 0 for valid
 * @returns CDN_EINVAL for invalid
 */
static inline uint32_t I3C_ThresholdConfigSF(const I3C_ThresholdConfig *obj)
{
    uint32_t ret = 0;

    if (obj == NULL)
    {
        ret = CDN_EINVAL;
    }

    return ret;
}


/**
 * Function to validate struct Config
 *
 * @param[in] obj pointer to struct to be verified
 * @returns 0 for valid
 * @returns CDN_EINVAL for invalid
 */
static inline uint32_t I3C_ConfigSF(const I3C_Config *obj)
{
    uint32_t ret = 0;

    if (obj == NULL)
    {
        ret = CDN_EINVAL;
    }

    return ret;
}


/**
 * Function to validate struct Callbacks
 *
 * @param[in] obj pointer to struct to be verified
 * @returns 0 for valid
 * @returns CDN_EINVAL for invalid
 */
static inline uint32_t I3C_CallbacksSF(const I3C_Callbacks *obj)
{
    uint32_t ret = 0;

    if (obj == NULL)
    {
        ret = CDN_EINVAL;
    }

    return ret;
}


/**
 * Function to validate struct DeviceDescriptor
 *
 * @param[in] obj pointer to struct to be verified
 * @returns 0 for valid
 * @returns CDN_EINVAL for invalid
 */
static inline uint32_t I3C_DeviceDescriptorSF(const I3C_DeviceDescriptor *obj)
{
    uint32_t ret = 0;

    if (obj == NULL)
    {
        ret = CDN_EINVAL;
    }

    return ret;
}


/**
 * Function to validate struct SlaveDescriptors
 *
 * @param[in] obj pointer to struct to be verified
 * @returns 0 for valid
 * @returns CDN_EINVAL for invalid
 */
static inline uint32_t I3C_SlaveDescriptorsSF(const I3C_SlaveDescriptors *obj)
{
    uint32_t ret = 0;

    if (obj == NULL)
    {
        ret = CDN_EINVAL;
    }

    return ret;
}


/**
 * Function to validate struct IbiSirConfiguration
 *
 * @param[in] obj pointer to struct to be verified
 * @returns 0 for valid
 * @returns CDN_EINVAL for invalid
 */
static inline uint32_t I3C_IbiSirConfigurationSF(const I3C_IbiSirConfiguration *obj)
{
    uint32_t ret = 0;

    if (obj == NULL)
    {
        ret = CDN_EINVAL;
    }
    else
    {
        if (
            (obj->ibiResp != I3C_IBI_RESP_NACK) &&
            (obj->ibiResp != I3C_IBI_RESP_ACK)
        )
        {
            ret = CDN_EINVAL;
        }
        if (
            (obj->ibiPayloadSpeedLimit != I3C_IBI_DATA_SPEED_NOT_LIMITED) &&
            (obj->ibiPayloadSpeedLimit != I3C_IBI_DATA_SPEED_LIMITED)
        )
        {
            ret = CDN_EINVAL;
        }
        if (
            (obj->ibiReqDevRole != I3C_IBI_REQUESTING_DEV_ROLE_SLAVE) &&
            (obj->ibiReqDevRole != I3C_IBI_REQUESTING_DEV_ROLE_MASTER)
        )
        {
            ret = CDN_EINVAL;
        }
    }

    return ret;
}


/**
 * Function to validate struct CsrData
 *
 * @param[in] obj pointer to struct to be verified
 * @returns 0 for valid
 * @returns CDN_EINVAL for invalid
 */
static inline uint32_t I3C_CsrDataSF(const I3C_CsrData *obj)
{
    uint32_t ret = 0;

    if (obj == NULL)
    {
        ret = CDN_EINVAL;
    }

    return ret;
}


/**
 * Function to validate struct PayloadData
 *
 * @param[in] obj pointer to struct to be verified
 * @returns 0 for valid
 * @returns CDN_EINVAL for invalid
 */
static inline uint32_t I3C_PayloadDataSF(const I3C_PayloadData *obj)
{
    uint32_t ret = 0;

    if (obj == NULL)
    {
        ret = CDN_EINVAL;
    }

    return ret;
}


/**
 * Function to validate struct TransmissionParameters
 *
 * @param[in] obj pointer to struct to be verified
 * @returns 0 for valid
 * @returns CDN_EINVAL for invalid
 */
static inline uint32_t I3C_TransmissionParametersSF(const I3C_TransmissionParameters *obj)
{
    uint32_t ret = 0;

    if (obj == NULL)
    {
        ret = CDN_EINVAL;
    }

    return ret;
}


/**
 * Function to validate struct PrivData
 *
 * @param[in] obj pointer to struct to be verified
 * @returns 0 for valid
 * @returns CDN_EINVAL for invalid
 */
static inline uint32_t I3C_PrivDataSF(const I3C_PrivData *obj)
{
    uint32_t ret = 0;

    if (obj == NULL)
    {
        ret = CDN_EINVAL;
    }
    else
    {
        if (
            (obj->dev_role != I3C_DEV_ROLE_MASTER) &&
            (obj->dev_role != I3C_DEV_ROLE_SECONDARY_MASTER)
        )
        {
            ret = CDN_EINVAL;
        }
        if (
            (obj->bus_mode != I3C_BUS_MODE_PURE) &&
            (obj->bus_mode != I3C_BUS_MODE_INVALID) &&
            (obj->bus_mode != I3C_BUS_MODE_MIXED_FAST) &&
            (obj->bus_mode != I3C_BUS_MODE_MIXED_SLOW)
        )
        {
            ret = CDN_EINVAL;
        }
    }

    return ret;
}


/**
 * Function to validate struct AsfInfo
 *
 * @param[in] obj pointer to struct to be verified
 * @returns 0 for valid
 * @returns CDN_EINVAL for invalid
 */
static inline uint32_t I3C_AsfInfoSF(const I3C_AsfInfo *obj)
{
    uint32_t ret = 0;

    if (obj == NULL)
    {
        ret = CDN_EINVAL;
    }

    return ret;
}


/**
 * Function to validate struct MaxReadLength
 *
 * @param[in] obj pointer to struct to be verified
 * @returns 0 for valid
 * @returns CDN_EINVAL for invalid
 */
static inline uint32_t I3C_MaxReadLengthSF(const I3C_MaxReadLength *obj)
{
    uint32_t ret = 0;

    if (obj == NULL)
    {
        ret = CDN_EINVAL;
    }

    return ret;
}


/**
 * Function to validate struct MaxDataSpeed
 *
 * @param[in] obj pointer to struct to be verified
 * @returns 0 for valid
 * @returns CDN_EINVAL for invalid
 */
static inline uint32_t I3C_MaxDataSpeedSF(const I3C_MaxDataSpeed *obj)
{
    uint32_t ret = 0;

    if (obj == NULL)
    {
        ret = CDN_EINVAL;
    }

    return ret;
}


/**
 * A common function to check the validity of API functions with
 * following parameter types
 * @param[in] config Driver and hardware configuration.
 * @param[out] sysReq Returns the memory requirements for given configuration.
 * @return 0 success
 * @return CDN_EINVAL invalid parameters
 */
static inline uint32_t I3C_SanityFunction1(const I3C_Config* config, const I3C_SysReq* sysReq)
{
    /* Declaring return variable */
    uint32_t ret = 0;

    if (sysReq == NULL)
    {
        ret = CDN_EINVAL;
    }
    else if (I3C_ConfigSF(config) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else
    {
        /*
         * All 'if ... else if' constructs shall be terminated with an 'else' statement
         * (MISRA2012-RULE-15_7-3)
         */
    }

    return ret;
}


/**
 * A common function to check the validity of API functions with
 * following parameter types
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] config Specifies driver/hardware configuration.
 * @param[in] callbacks Event Handlers and Callbacks.
 * @return 0 success
 * @return CDN_EINVAL invalid parameters
 */
static inline uint32_t I3C_SanityFunction2(const I3C_PrivData* pd, const I3C_Config* config, const I3C_Callbacks* callbacks)
{
    /* Declaring return variable */
    uint32_t ret = 0;

    if (I3C_PrivDataSF(pd) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else if (I3C_ConfigSF(config) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else if (I3C_CallbacksSF(callbacks) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else
    {
        /*
         * All 'if ... else if' constructs shall be terminated with an 'else' statement
         * (MISRA2012-RULE-15_7-3)
         */
    }

    return ret;
}


/**
 * A common function to check the validity of API functions with
 * following parameter types
 * @param[in] pd Pointer to driver's private data object filled by init.
 * @return 0 success
 * @return CDN_EINVAL invalid parameters
 */
static inline uint32_t I3C_SanityFunction3(const I3C_PrivData* pd)
{
    /* Declaring return variable */
    uint32_t ret = 0;

    if (I3C_PrivDataSF(pd) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }

    return ret;
}


/**
 * A common function to check the validity of API functions with
 * following parameter types
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] mode Bus Mode.
 * @return 0 success
 * @return CDN_EINVAL invalid parameters
 */
static inline uint32_t I3C_SanityFunction9(const I3C_PrivData* pd, const I3C_BusMode mode)
{
    /* Declaring return variable */
    uint32_t ret = 0;

    if (I3C_PrivDataSF(pd) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else if (
        (mode != I3C_BUS_MODE_PURE) &&
        (mode != I3C_BUS_MODE_INVALID) &&
        (mode != I3C_BUS_MODE_MIXED_FAST) &&
        (mode != I3C_BUS_MODE_MIXED_SLOW)
    )
    {
        ret = CDN_EINVAL;
    }
    else
    {
        /*
         * All 'if ... else if' constructs shall be terminated with an 'else' statement
         * (MISRA2012-RULE-15_7-3)
         */
    }

    return ret;
}


/**
 * A common function to check the validity of API functions with
 * following parameter types
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] mode Pointer to Bus Mode object.
 * @return 0 success
 * @return CDN_EINVAL invalid parameters
 */
static inline uint32_t I3C_SanityFunction10(const I3C_PrivData* pd, const I3C_BusMode* mode)
{
    /* Declaring return variable */
    uint32_t ret = 0;

    if (mode == NULL)
    {
        ret = CDN_EINVAL;
    }
    else if (I3C_PrivDataSF(pd) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else if (
        (*mode != I3C_BUS_MODE_PURE) &&
        (*mode != I3C_BUS_MODE_INVALID) &&
        (*mode != I3C_BUS_MODE_MIXED_FAST) &&
        (*mode != I3C_BUS_MODE_MIXED_SLOW)
    )
    {
        ret = CDN_EINVAL;
    }
    else
    {
        /*
         * All 'if ... else if' constructs shall be terminated with an 'else' statement
         * (MISRA2012-RULE-15_7-3)
         */
    }

    return ret;
}


/**
 * A common function to check the validity of API functions with
 * following parameter types
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] pid Ponter to buffer containing 48-bit Provisional ID value.
 * @return 0 success
 * @return CDN_EINVAL invalid parameters
 */
static inline uint32_t I3C_SanityFunction13(const I3C_PrivData* pd, const uint8_t* pid)
{
    /* Declaring return variable */
    uint32_t ret = 0;

    if (pid == NULL)
    {
        ret = CDN_EINVAL;
    }
    else if (I3C_PrivDataSF(pd) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else
    {
        /*
         * All 'if ... else if' constructs shall be terminated with an 'else' statement
         * (MISRA2012-RULE-15_7-3)
         */
    }

    return ret;
}


/**
 * A common function to check the validity of API functions with
 * following parameter types
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] devs Pointer to the structure containing configuration and capabilities of the Devices.
 * @return 0 success
 * @return CDN_EINVAL invalid parameters
 */
static inline uint32_t I3C_SanityFunction14(const I3C_PrivData* pd, const I3C_DeviceDescriptor* devs)
{
    /* Declaring return variable */
    uint32_t ret = 0;

    if (I3C_PrivDataSF(pd) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else if (I3C_DeviceDescriptorSF(devs) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else
    {
        /*
         * All 'if ... else if' constructs shall be terminated with an 'else' statement
         * (MISRA2012-RULE-15_7-3)
         */
    }

    return ret;
}


/**
 * A common function to check the validity of API functions with
 * following parameter types
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] config Specifies driver/hardware configuration.
 * @return 0 success
 * @return CDN_EINVAL invalid parameters
 */
static inline uint32_t I3C_SanityFunction16(const I3C_PrivData* pd, const I3C_Config* config)
{
    /* Declaring return variable */
    uint32_t ret = 0;

    if (I3C_PrivDataSF(pd) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else if (I3C_ConfigSF(config) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else
    {
        /*
         * All 'if ... else if' constructs shall be terminated with an 'else' statement
         * (MISRA2012-RULE-15_7-3)
         */
    }

    return ret;
}


/**
 * A common function to check the validity of API functions with
 * following parameter types
 * @param[in] pd Pointer to driver's private data object.
 * @param[out] hj_devices This mask indicates which devices joined the bus.
 * @return 0 success
 * @return CDN_EINVAL invalid parameters
 */
static inline uint32_t I3C_SanityFunction18(const I3C_PrivData* pd, const uint16_t* hj_devices)
{
    /* Declaring return variable */
    uint32_t ret = 0;

    if (hj_devices == NULL)
    {
        ret = CDN_EINVAL;
    }
    else if (I3C_PrivDataSF(pd) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else
    {
        /*
         * All 'if ... else if' constructs shall be terminated with an 'else' statement
         * (MISRA2012-RULE-15_7-3)
         */
    }

    return ret;
}


/**
 * A common function to check the validity of API functions with
 * following parameter types
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] interruptConfig Interrupt configuration.
 * @return 0 success
 * @return CDN_EINVAL invalid parameters
 */
static inline uint32_t I3C_SanityFunction19(const I3C_PrivData* pd, const I3C_InterruptConfig* interruptConfig)
{
    /* Declaring return variable */
    uint32_t ret = 0;

    if (I3C_PrivDataSF(pd) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else if (I3C_InterruptConfigSF(interruptConfig) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else
    {
        /*
         * All 'if ... else if' constructs shall be terminated with an 'else' statement
         * (MISRA2012-RULE-15_7-3)
         */
    }

    return ret;
}


/**
 * A common function to check the validity of API functions with
 * following parameter types
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] thresholdConfig Threshold configuration.
 * @return 0 success
 * @return CDN_EINVAL invalid parameters
 */
static inline uint32_t I3C_SanityFunction20(const I3C_PrivData* pd, const I3C_ThresholdConfig* thresholdConfig)
{
    /* Declaring return variable */
    uint32_t ret = 0;

    if (I3C_PrivDataSF(pd) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else if (I3C_ThresholdConfigSF(thresholdConfig) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else
    {
        /*
         * All 'if ... else if' constructs shall be terminated with an 'else' statement
         * (MISRA2012-RULE-15_7-3)
         */
    }

    return ret;
}


/**
 * A common function to check the validity of API functions with
 * following parameter types
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] csr_data CSR information with CSR address and address lenght.
 * @param[in] payload_data Pointer to structure with payload information
 * @param[in] xmitMmode Transmission Mode.
 * @return 0 success
 * @return CDN_EINVAL invalid parameters
 */
static inline uint32_t I3C_SanityFunction30(const I3C_PrivData* pd, const I3C_CsrData* csr_data, const I3C_PayloadData* payload_data, const I3C_TransmitMode xmitMmode)
{
    /* Declaring return variable */
    uint32_t ret = 0;

    if (I3C_PrivDataSF(pd) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else if (I3C_CsrDataSF(csr_data) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else if (I3C_PayloadDataSF(payload_data) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else if (
        (xmitMmode != I3C_CMD_XMIT_MODE_SINGLE_CSR) &&
        (xmitMmode != I3C_CMD_XMIT_MODE_MULTI_BYTE_INC) &&
        (xmitMmode != I3C_CMD_XMIT_MODE_MULTI_BYTE_STATIC) &&
        (xmitMmode != I3C_CMD_XMIT_MODE_NCA)
    )
    {
        ret = CDN_EINVAL;
    }
    else
    {
        /*
         * All 'if ... else if' constructs shall be terminated with an 'else' statement
         * (MISRA2012-RULE-15_7-3)
         */
    }

    return ret;
}


/**
 * A common function to check the validity of API functions with
 * following parameter types
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] hdrMode Specifies which one of the HDR modes to use.
 * @param[in] payload_data Pointer to structure with payload information
 * @return 0 success
 * @return CDN_EINVAL invalid parameters
 */
static inline uint32_t I3C_SanityFunction34(const I3C_PrivData* pd, const I3C_HdrMode hdrMode, const I3C_PayloadData* payload_data)
{
    /* Declaring return variable */
    uint32_t ret = 0;

    if (I3C_PrivDataSF(pd) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else if (
        (hdrMode != I3C_HDR_MODE_DDR) &&
        (hdrMode != I3C_HDR_MODE_TSP) &&
        (hdrMode != I3C_HDR_MODE_TSL)
    )
    {
        ret = CDN_EINVAL;
    }
    else if (I3C_PayloadDataSF(payload_data) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else
    {
        /*
         * All 'if ... else if' constructs shall be terminated with an 'else' statement
         * (MISRA2012-RULE-15_7-3)
         */
    }

    return ret;
}


/**
 * A common function to check the validity of API functions with
 * following parameter types
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] tx_params Transmission parameters - command descriptors
 * @return 0 success
 * @return CDN_EINVAL invalid parameters
 */
static inline uint32_t I3C_SanityFunction36(const I3C_PrivData* pd, const I3C_TransmissionParameters* tx_params)
{
    /* Declaring return variable */
    uint32_t ret = 0;

    if (I3C_PrivDataSF(pd) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else if (I3C_TransmissionParametersSF(tx_params) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else
    {
        /*
         * All 'if ... else if' constructs shall be terminated with an 'else' statement
         * (MISRA2012-RULE-15_7-3)
         */
    }

    return ret;
}


/**
 * A common function to check the validity of API functions with
 * following parameter types
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] state Activity State value.
 * @param[in] tx_params Transmission parameters - command descriptor
 * @return 0 success
 * @return CDN_EINVAL invalid parameters
 */
static inline uint32_t I3C_SanityFunction37(const I3C_PrivData* pd, const I3C_ActivityState state, const I3C_TransmissionParameters* tx_params)
{
    /* Declaring return variable */
    uint32_t ret = 0;

    if (I3C_PrivDataSF(pd) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else if (
        (state != I3C_ACTIVITY_STATE_0) &&
        (state != I3C_ACTIVITY_STATE_1) &&
        (state != I3C_ACTIVITY_STATE_2) &&
        (state != I3C_ACTIVITY_STATE_3)
    )
    {
        ret = CDN_EINVAL;
    }
    else if (I3C_TransmissionParametersSF(tx_params) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else
    {
        /*
         * All 'if ... else if' constructs shall be terminated with an 'else' statement
         * (MISRA2012-RULE-15_7-3)
         */
    }

    return ret;
}


/**
 * A common function to check the validity of API functions with
 * following parameter types
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] length Pointer to 16-bit variable to which Maximum Write Length will be written.
 * @param[in] tx_params Transmission parameters - command descriptor
 * @return 0 success
 * @return CDN_EINVAL invalid parameters
 */
static inline uint32_t I3C_SanityFunction41(const I3C_PrivData* pd, const uint16_t* length, const I3C_TransmissionParameters* tx_params)
{
    /* Declaring return variable */
    uint32_t ret = 0;

    if (length == NULL)
    {
        ret = CDN_EINVAL;
    }
    else if (I3C_PrivDataSF(pd) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else if (I3C_TransmissionParametersSF(tx_params) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else
    {
        /*
         * All 'if ... else if' constructs shall be terminated with an 'else' statement
         * (MISRA2012-RULE-15_7-3)
         */
    }

    return ret;
}


/**
 * A common function to check the validity of API functions with
 * following parameter types
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] max_read_length Pointer to a structure where Maximum Read Length.
 * @param[in] tx_params Transmission parameters - command descriptor
 * @return 0 success
 * @return CDN_EINVAL invalid parameters
 */
static inline uint32_t I3C_SanityFunction43(const I3C_PrivData* pd, const I3C_MaxReadLength* max_read_length, const I3C_TransmissionParameters* tx_params)
{
    /* Declaring return variable */
    uint32_t ret = 0;

    if (I3C_PrivDataSF(pd) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else if (I3C_MaxReadLengthSF(max_read_length) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else if (I3C_TransmissionParametersSF(tx_params) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else
    {
        /*
         * All 'if ... else if' constructs shall be terminated with an 'else' statement
         * (MISRA2012-RULE-15_7-3)
         */
    }

    return ret;
}


/**
 * A common function to check the validity of API functions with
 * following parameter types
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] max_data_speed Pointer to structure with max data speeds.
 * @param[in] tx_params Transmission parameters - command descriptor
 * @return 0 success
 * @return CDN_EINVAL invalid parameters
 */
static inline uint32_t I3C_SanityFunction44(const I3C_PrivData* pd, const I3C_MaxDataSpeed* max_data_speed, const I3C_TransmissionParameters* tx_params)
{
    /* Declaring return variable */
    uint32_t ret = 0;

    if (I3C_PrivDataSF(pd) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else if (I3C_MaxDataSpeedSF(max_data_speed) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else if (I3C_TransmissionParametersSF(tx_params) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else
    {
        /*
         * All 'if ... else if' constructs shall be terminated with an 'else' statement
         * (MISRA2012-RULE-15_7-3)
         */
    }

    return ret;
}


/**
 * A common function to check the validity of API functions with
 * following parameter types
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] slaveDescs Pointer the structure of Slave descriptors. Memory will be written with slave count and slave data.
 * @return 0 success
 * @return CDN_EINVAL invalid parameters
 */
static inline uint32_t I3C_SanityFunction45(const I3C_PrivData* pd, const I3C_SlaveDescriptors* slaveDescs)
{
    /* Declaring return variable */
    uint32_t ret = 0;

    if (I3C_PrivDataSF(pd) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else if (I3C_SlaveDescriptorsSF(slaveDescs) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else
    {
        /*
         * All 'if ... else if' constructs shall be terminated with an 'else' statement
         * (MISRA2012-RULE-15_7-3)
         */
    }

    return ret;
}


/**
 * A common function to check the validity of API functions with
 * following parameter types
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] slaveDescs Pointer the structure of Slave descriptors.
 * @param[in] tx_params Transmission parameters - command descriptor
 * @return 0 success
 * @return CDN_EINVAL invalid parameters
 */
static inline uint32_t I3C_SanityFunction46(const I3C_PrivData* pd, const I3C_SlaveDescriptors* slaveDescs, const I3C_TransmissionParameters* tx_params)
{
    /* Declaring return variable */
    uint32_t ret = 0;

    if (I3C_PrivDataSF(pd) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else if (I3C_SlaveDescriptorsSF(slaveDescs) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else if (I3C_TransmissionParametersSF(tx_params) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else
    {
        /*
         * All 'if ... else if' constructs shall be terminated with an 'else' statement
         * (MISRA2012-RULE-15_7-3)
         */
    }

    return ret;
}


/**
 * A common function to check the validity of API functions with
 * following parameter types
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] mode
 * @param[in] tx_params Transmission parameters - command descriptor
 * @return 0 success
 * @return CDN_EINVAL invalid parameters
 */
static inline uint32_t I3C_SanityFunction48(const I3C_PrivData* pd, const I3C_HdrMode mode, const I3C_TransmissionParameters* tx_params)
{
    /* Declaring return variable */
    uint32_t ret = 0;

    if (I3C_PrivDataSF(pd) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else if (
        (mode != I3C_HDR_MODE_DDR) &&
        (mode != I3C_HDR_MODE_TSP) &&
        (mode != I3C_HDR_MODE_TSL)
    )
    {
        ret = CDN_EINVAL;
    }
    else if (I3C_TransmissionParametersSF(tx_params) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else
    {
        /*
         * All 'if ... else if' constructs shall be terminated with an 'else' statement
         * (MISRA2012-RULE-15_7-3)
         */
    }

    return ret;
}


/**
 * A common function to check the validity of API functions with
 * following parameter types
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] buff Pointer to variable to which 48-bit Provisional ID will be written.
 * @param[in] tx_params Transmission parameters - command descriptor
 * @return 0 success
 * @return CDN_EINVAL invalid parameters
 */
static inline uint32_t I3C_SanityFunction51(const I3C_PrivData* pd, const uint8_t* buff, const I3C_TransmissionParameters* tx_params)
{
    /* Declaring return variable */
    uint32_t ret = 0;

    if (buff == NULL)
    {
        ret = CDN_EINVAL;
    }
    else if (I3C_PrivDataSF(pd) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else if (I3C_TransmissionParametersSF(tx_params) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else
    {
        /*
         * All 'if ... else if' constructs shall be terminated with an 'else' statement
         * (MISRA2012-RULE-15_7-3)
         */
    }

    return ret;
}


/**
 * A common function to check the validity of API functions with
 * following parameter types
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] ibi_sir_cfg Pointer to array that will hold configuration of IBI related devices.
 * @return 0 success
 * @return CDN_EINVAL invalid parameters
 */
static inline uint32_t I3C_SanityFunction56(const I3C_PrivData* pd, const I3C_IbiSirConfiguration* ibi_sir_cfg)
{
    /* Declaring return variable */
    uint32_t ret = 0;

    if (I3C_PrivDataSF(pd) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else if (I3C_IbiSirConfigurationSF(ibi_sir_cfg) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else
    {
        /*
         * All 'if ... else if' constructs shall be terminated with an 'else' statement
         * (MISRA2012-RULE-15_7-3)
         */
    }

    return ret;
}


/**
 * A common function to check the validity of API functions with
 * following parameter types
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] hj_response Should I3C controller ACK or NACK Hot Join request.
 * @param[in] hj_disable_set Should I3C controller disable slaves from requesting Hot Join.
 * @return 0 success
 * @return CDN_EINVAL invalid parameters
 */
static inline uint32_t I3C_SanityFunction60(const I3C_PrivData* pd, const I3C_HjResponse hj_response, const I3C_HjDisableSet hj_disable_set)
{
    /* Declaring return variable */
    uint32_t ret = 0;

    if (I3C_PrivDataSF(pd) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else if (
        (hj_response != I3C_HJ_RESP_NACK) &&
        (hj_response != I3C_HJ_RESP_ACK)
    )
    {
        ret = CDN_EINVAL;
    }
    else if (
        (hj_disable_set != I3C_HJ_REQ_SLAVE_NOT_DISABLE) &&
        (hj_disable_set != I3C_HJ_REQ_SLAVE_DISABLE)
    )
    {
        ret = CDN_EINVAL;
    }
    else
    {
        /*
         * All 'if ... else if' constructs shall be terminated with an 'else' statement
         * (MISRA2012-RULE-15_7-3)
         */
    }

    return ret;
}


/**
 * A common function to check the validity of API functions with
 * following parameter types
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] slaveInterruptConfig Interrupt configuration.
 * @return 0 success
 * @return CDN_EINVAL invalid parameters
 */
static inline uint32_t I3C_SanityFunction61(const I3C_PrivData* pd, const I3C_SlaveInterruptConfig* slaveInterruptConfig)
{
    /* Declaring return variable */
    uint32_t ret = 0;

    if (I3C_PrivDataSF(pd) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else if (I3C_SlaveInterruptConfigSF(slaveInterruptConfig) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else
    {
        /*
         * All 'if ... else if' constructs shall be terminated with an 'else' statement
         * (MISRA2012-RULE-15_7-3)
         */
    }

    return ret;
}


/**
 * A common function to check the validity of API functions with
 * following parameter types
 * @param[in] pd Pointer to driver's private data object.
 * @param[in] asf_info Pointer to ASF information structure.
 * @return 0 success
 * @return CDN_EINVAL invalid parameters
 */
static inline uint32_t I3C_SanityFunction69(const I3C_PrivData* pd, const I3C_AsfInfo* asf_info)
{
    /* Declaring return variable */
    uint32_t ret = 0;

    if (I3C_PrivDataSF(pd) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else if (I3C_AsfInfoSF(asf_info) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else
    {
        /*
         * All 'if ... else if' constructs shall be terminated with an 'else' statement
         * (MISRA2012-RULE-15_7-3)
         */
    }

    return ret;
}


/**
 * A common function to check the validity of API functions with
 * following parameter types
 * @param[in] pd Pointer to driver's private data object.
 * @param[out] opMode Pointer to OperationMode structure.
 * @return 0 success
 * @return CDN_EINVAL invalid parameters
 */
static inline uint32_t I3C_SanityFunction70(const I3C_PrivData* pd, const I3C_OperationMode* opMode)
{
    /* Declaring return variable */
    uint32_t ret = 0;

    if (opMode == NULL)
    {
        ret = CDN_EINVAL;
    }
    else if (I3C_PrivDataSF(pd) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else
    {
        /*
         * All 'if ... else if' constructs shall be terminated with an 'else' statement
         * (MISRA2012-RULE-15_7-3)
         */
    }

    return ret;
}

/* parasoft-end-suppress METRICS-41-3 */
/* parasoft-end-suppress METRICS-39-3 */
/* parasoft-end-suppress METRICS-36-3 */
/* parasoft-end-suppress METRICS-18-3 */

#endif	/* I3C_SANITY_H */
