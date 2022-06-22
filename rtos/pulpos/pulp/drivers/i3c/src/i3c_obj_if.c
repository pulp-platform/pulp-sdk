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

#include "i3c_obj_if.h"

/* parasoft suppress item METRICS-41-3 "Number of blocks of comments per statement" */

I3C_OBJ *I3C_GetInstance(void)
{
    static I3C_OBJ driver =
    {
        .probe = I3C_Probe,
        .init = I3C_Init,
        .isr = I3C_Isr,
        .start = I3C_Start,
        .stop = I3C_Stop,
        .destroy = I3C_Destroy,
        .enableCore = I3C_EnableCore,
        .disableCore = I3C_DisableCore,
        .setBusMode = I3C_SetBusMode,
        .getBusMode = I3C_GetBusMode,
        .setBcr = I3C_SetBcr,
        .setDcr = I3C_SetDcr,
        .setPid = I3C_SetPid,
        .configureDevices = I3C_ConfigureDevices,
        .configureDevice = I3C_ConfigureDevice,
        .configurePrescalers = I3C_ConfigurePrescalers,
        .clearRrOfDevice = I3C_ClearRrOfDevice,
        .getNewDevices = I3C_GetNewDevices,
        .configureInterrupts = I3C_ConfigureInterrupts,
        .configureThresholds = I3C_ConfigureThresholds,
        .cmdExec = I3C_CmdExec,
        .cmdExecImmediate = I3C_CmdExecImmediate,
        .enableMcs = I3C_EnableMcs,
        .disableMcs = I3C_DisableMcs,
        .manualCommandStart = I3C_ManualCommandStart,
        .cmdCount = I3C_CmdCount,
        .cmdMaxCount = I3C_CmdMaxCount,
        .cmdClearAll = I3C_CmdClearAll,
        .devPrint = I3C_DevPrint,
        .cmdAddPrivWrite = I3C_CmdAddPrivWrite,
        .cmdAddPrivRead = I3C_CmdAddPrivRead,
        .cmdAddPrivI2CWrite = I3C_CmdAddPrivI2CWrite,
        .cmdAddPrivI2CRead = I3C_CmdAddPrivI2CRead,
        .cmdAddDdrWrite = I3C_CmdAddDdrWrite,
        .cmdAddDdrRead = I3C_CmdAddDdrRead,
        .cmdAddSetSlaveEvents = I3C_CmdAddSetSlaveEvents,
        .cmdAddEnterActivityState = I3C_CmdAddEnterActivityState,
        .cmdAddResetDaa = I3C_CmdAddResetDaa,
        .cmdAddEnterDaa = I3C_CmdAddEnterDaa,
        .cmdAddSetMaxWriteLength = I3C_CmdAddSetMaxWriteLength,
        .cmdAddGetMaxWriteLength = I3C_CmdAddGetMaxWriteLength,
        .cmdAddSetMaxReadLength = I3C_CmdAddSetMaxReadLength,
        .cmdAddGetMaxReadLength = I3C_CmdAddGetMaxReadLength,
        .cmdAddGetMaxDataSpeed = I3C_CmdAddGetMaxDataSpeed,
        .getSlavesList = I3C_GetSlavesList,
        .cmdAddDefineSlavesList = I3C_CmdAddDefineSlavesList,
        .cmdAddEnterTestMode = I3C_CmdAddEnterTestMode,
        .cmdAddEnterHdrMode = I3C_CmdAddEnterHdrMode,
        .cmdSetDaFromSa = I3C_CmdSetDaFromSa,
        .cmdAddSetNewDa = I3C_CmdAddSetNewDa,
        .cmdAddGetProvisionalId = I3C_CmdAddGetProvisionalId,
        .cmdAddGetBcr = I3C_CmdAddGetBcr,
        .cmdAddGetDcr = I3C_CmdAddGetDcr,
        .cmdAddGetStatus = I3C_CmdAddGetStatus,
        .cmdAddGetAccMst = I3C_CmdAddGetAccMst,
        .ibiConfigureDevices = I3C_IbiConfigureDevices,
        .ibiModifyDeviceConfig = I3C_IbiModifyDeviceConfig,
        .ibiGetAddressOfIssuer = I3C_IbiGetAddressOfIssuer,
        .ibiGetData = I3C_IbiGetData,
        .hjConfigureResponse = I3C_HjConfigureResponse,
        .configureSlaveInterrupts = I3C_ConfigureSlaveInterrupts,
        .slaveModeConfigure = I3C_SlaveModeConfigure,
        .slaveModeReqSdrRead = I3C_SlaveModeReqSdrRead,
        .slaveModeReqSdrWrite = I3C_SlaveModeReqSdrWrite,
        .slaveModeReqDdrRead = I3C_SlaveModeReqDdrRead,
        .slaveModeReqDdrWrite = I3C_SlaveModeReqDdrWrite,
        .slaveModeRequestHotJoin = I3C_SlaveModeRequestHotJoin,
        .slaveModeMastershipReq = I3C_SlaveModeMastershipReq,
        .getAsfInfo = I3C_GetAsfInfo,
        .checkOperationMode = I3C_CheckOperationMode,
    };

    return &driver;
}
