#include "Definitions.h"
#include <stdio.h>
#include <iostream>

using namespace std;

HANDLE keyHandle = 0;
char* deviceName = (char*)"EPOS4";
char* protocolStackName = (char*)"MAXON SERIAL V2";
char* interfaceName = (char*)"USB";
char* portName = (char*)"USB0";
bool findDevSet;
DWORD errorCode = 0;

WORD nodeId = 1;
//const char* parameterFileName = "C:\\Users\\ymoto\OneDrive\\ドキュメント\\MAXON_programming\\Parameter.dcf";
BOOL showDlg = TRUE;
BOOL showMsg = FALSE;
BOOL result = FALSE;

long PositionIs = 0;        // 角度の実測値
BOOL Absolute = FALSE;      // TRUE： 絶対　FALSE：相対
BOOL Immediately = TRUE;    // TRUE：すぐに開始　FALSE：最後の位置決めが終了するまで待つ

int ii = 0;
long currentMust = 0;
BOOL suc = 0;
WORD AnalogValue = 0;
long VoltageValue = 0;

int main(int argc, char* argv[]) {
    //ポートをEPOS4に接続
    keyHandle = VCS_OpenDevice(deviceName, protocolStackName, interfaceName, portName, &errorCode);

    if ((BOOL)keyHandle > 0) {
        // 使える状態にする
        VCS_ClearFault(keyHandle, nodeId, &errorCode);  // デバイスの状態を"fault"から"disable"にする
        VCS_SetEnableState(keyHandle, nodeId, &errorCode);  // デバイスの状態を"enable"状態にする
        // VCS_ResetPositionMarkerCounter(keyHandle, nodeId, &errorCode);



        
        ////////////////////////////// profile position mode
        VCS_SetOperationMode(keyHandle, nodeId, 1, &errorCode); //動作モードを設定　コマンドライブラリのp.67参照

        // profile position modeのパラメータを設定
        DWORD ProfileVelocity = 2500;
        DWORD ProfileAcceleration = 200;
        DWORD ProfileDeceleration = 200;
        VCS_SetPositionProfile(keyHandle, nodeId, ProfileVelocity, ProfileAcceleration, ProfileDeceleration, &errorCode);

        for (ii = 1; ii <= 5; ii++) {

            VCS_GetPositionIs(keyHandle, nodeId, &PositionIs, &errorCode);  // 現在の角度
            printf("現在の角度は %ld です\n", PositionIs);

            //  VCS_MoveToPosition(keyHandle, nodeId, pTargetPosition, Absolute, Immediately, &errorCode);
            VCS_MoveToPosition(keyHandle, nodeId, 4096, Absolute, Immediately, &errorCode);
            Sleep(3000);
        }
        //////////////////////////////// profile position modeここまで
        


        /*
        //////////////////////////////// profile velocity mode
        VCS_SetOperationMode(keyHandle, nodeId, 3, &errorCode); //動作モードを設定　コマンドライブラリのp.67参照

        // profile velocity modeのパラメータを設定
        DWORD ProfileAcceleration = 200;
        DWORD ProfileDeceleration = 200;
        VCS_SetVelocityProfile(keyHandle, nodeId, ProfileAcceleration, ProfileDeceleration, &errorCode);

        // velocityの単位をMilliに設定．コマンドライブラリ64ページを参照
        VCS_SetVelocityUnits(keyHandle, nodeId, 0xA4, (char)-3, &errorCode);

        //  VCS_MoveWithVelocity(HANDLE KeyHandle, WORD NodeId, long TargetVelocity, DWORD* pErrorCode);
        VCS_MoveWithVelocity(keyHandle, nodeId, 273, &errorCode);
        Sleep(10000);
        VCS_MoveWithVelocity(keyHandle, nodeId, 0, &errorCode);
        ///////////////////////////////// profile velocity modeここまで
        */

        /*
        //////////////////////////////// Cyclic synchronous current mode
        VCS_SetOperationMode(keyHandle, nodeId, -3, &errorCode); //動作モードを設定　コマンドライブラリのp.67参照

        suc = VCS_GetCurrentMustEx(keyHandle, nodeId, &currentMust, &errorCode);
        printf_s("currentMust = %ld\n", currentMust);
        printf_s("suc = %d\n", (int)suc);
        currentMust = (long)4.0;
        VCS_SetCurrentMustEx(keyHandle, nodeId, currentMust, &errorCode);
        suc = VCS_GetCurrentMustEx(keyHandle, nodeId, &currentMust, &errorCode);
        printf_s("currentMust = %ld\n", currentMust);
        printf_s("suc = %d\n", (int)suc);
        VCS_ActivateAnalogCurrentSetpoint(keyHandle, nodeId, 1, (float)50.0, (short)0, &errorCode);   // analog入力端子1を使用，80 mA, offset 0 mA
        VCS_EnableAnalogCurrentSetpoint(keyHandle, nodeId, &errorCode);
        Sleep(5000);

        VCS_DisableAnalogCurrentSetpoint(keyHandle, nodeId, &errorCode);
        VCS_DeactivateAnalogCurrentSetpoint(keyHandle, nodeId, 1, &errorCode);

        ///////////////////////////////// Cyclic synchronous current modeここまで
        */


        /*
        ///////////////////////////////// Analog read
        for (ii = 1; ii <= 100; ii++) {
            VCS_GetAnalogInput(keyHandle, nodeId, 1, &AnalogValue, &errorCode);
            VCS_GetAnalogInputVoltage(keyHandle, nodeId, 1, &VoltageValue, &errorCode);
            printf_s("Analog Inputの値は%d\n", AnalogValue);
            printf_s("Analog Input Voltageの値は%ld [mV]\n", VoltageValue);
            Sleep(1000);
        }
        ///////////////////////////////// Analog readここまで
        */


        /*
        ///////////////////////////////// Analog out 5番のAnOUT1ならoutput numberは１
        VCS_AnalogOutputConfiguration(keyHandle, nodeId, 1, 0, &errorCode);
        //VCS_SetAnalogOutput(keyHandle, nodeId, 1, 4000, &errorCode);
        VCS_SetAnalogOutputVoltage(keyHandle, nodeId, 1, 4000, &errorCode);
        //VCS_SetAnalogOutput(keyHandle, nodeId, 5, 4000, &errorCode);
        Sleep(10000);
        VCS_SetAnalogOutput(keyHandle, nodeId, 1, 2000, &errorCode);
        Sleep(10000);
        VCS_SetAnalogOutput(keyHandle, nodeId, 1, 0, &errorCode);
        ///////////////////////////////// Analog outここまで
        */




        VCS_ResetDevice(keyHandle, nodeId, &errorCode);
        VCS_SetDisableState(keyHandle, nodeId, &errorCode);
    }
    VCS_CloseDevice(keyHandle, &errorCode);

}


/*
// 参考にしたプログラム　＜https://qiita.com/jamjam/items/4b9aff43aa0dd0a9dba4＞
HANDLE keyHandle = 0;
char* deviceName = (char*)"EPOS4";
char* protocolStackName = (char*)"MAXON SERIAL V2";
char* interfaceName = (char*)"USB";
char* portName = (char*)"USB0";

DWORD errorCode = 0;
DWORD timeout = 0;
WORD nodeId = 1;

WORD State = 0;
DWORD ProfileVelocity = 1000;
DWORD ProfileAcceleration = 1000;
DWORD ProfileDeceleration = 1000;
long PositionIs = 0;
long TargetPosition = 0;

DWORD move(DWORD pTargetPosition, DWORD pProfileVelocity)
{
    BOOL Absolute = 0;
    BOOL Immediately = TRUE;
    VCS_SetDisableState(keyHandle, nodeId, &errorCode);
    VCS_SetEnableState(keyHandle, nodeId, &errorCode);
    VCS_MoveToPosition(keyHandle, nodeId, pTargetPosition, Absolute, Immediately, &errorCode);
    void* Profile_velocity;
    DWORD NbOfBytesWritten = 0;
    VCS_SetObject(keyHandle, nodeId, 0x6081, 0x00, &Profile_velocity, 4, &NbOfBytesWritten, &errorCode);
    return errorCode;
}

int main()
{
    printf("start EPOS4\n");

    keyHandle = VCS_OpenDevice(deviceName, protocolStackName, interfaceName, portName, &errorCode);

    if ((BOOL)keyHandle) {

        // enable_state
        VCS_ClearFault(keyHandle, nodeId, &errorCode);
        VCS_SetEnableState(keyHandle, nodeId, &errorCode);

        // Initialisation
        VCS_SetDisableState(keyHandle, nodeId, &errorCode);
        VCS_SetOperationMode(keyHandle, nodeId, 1, &errorCode);
        ProfileVelocity = 2500;
        ProfileAcceleration = 200;
        ProfileDeceleration = 200;
        VCS_SetPositionProfile(keyHandle, nodeId, ProfileVelocity, ProfileAcceleration, ProfileDeceleration, &errorCode);

        VCS_GetPositionIs(keyHandle, nodeId, &PositionIs, &errorCode);
        printf("\norg positionIs--- %ld\n", PositionIs);

        //move

        move(10000, 50);

        VCS_GetTargetPosition(keyHandle, nodeId, &TargetPosition, &errorCode);
        printf("\nTargetPosition--- %ld\n", TargetPosition);
        Sleep(3000);

        VCS_GetPositionIs(keyHandle, nodeId, &PositionIs, &errorCode);
        printf("\nnew positionIs--- %ld\n", PositionIs);

        move(-50000, 2000);

        VCS_GetTargetPosition(keyHandle, nodeId, &TargetPosition, &errorCode);
        printf("\nTargetPosition--- %ld\n", TargetPosition);
        Sleep(3000);

        VCS_GetPositionIs(keyHandle, nodeId, &PositionIs, &errorCode);
        printf("\nnew positionIs--- %ld\n", PositionIs);
        Sleep(3000);

        printf("\nReset state");
        VCS_ResetDevice(keyHandle, nodeId, &errorCode);
        VCS_SetDisableState(keyHandle, nodeId, &errorCode);
    }
    VCS_CloseDevice(keyHandle, &errorCode);
}
*/


#include "Definitions.h"
#include <stdio.h>
#include <iostream>

using namespace std;

HANDLE keyHandleA = 0;
HANDLE keyHandleD = 0;
BOOL success_key = 0;
BOOL setProtA = 0;
BOOL setProtD = 0;
BOOL success_prot = 0;
char* deviceName = (char*)"EPOS4";
char* protocolStackName = (char*)"MAXON SERIAL V2";
char* interfaceName = (char*)"USB";
char* portNameA = (char*)"USB1";
char* portNameD = (char*)"USB0";
bool findDevSet;
DWORD errorCode = 0;
DWORD Baudrate = 1000000;  // [bit/s]
DWORD Timeout = 500;       // [ms]
WORD MotorType = 11;        // EC motor block commutated
DWORD NominalCurrent;       // Maximal continuous current
DWORD MaxOutputCurrent;     // Maximal peak current
WORD ThermalTimeConstant;   // Thermal time constant windint
BYTE NbOfPolePairs;         // Number of pole pairs
WORD sensorType = 2;        // incremental encoder 1 without index (2-channel)
int motorNo = 0;            // Set motor as 633399 or 658677
WORD nodeId = 1;
BOOL showDlg = TRUE;
BOOL showMsg = FALSE;
BOOL result = FALSE;

BOOL Absolute = FALSE;      // TRUE: Absolute　FALSE: Relative
BOOL Immediately = TRUE;    // TRUE: Start immediately　FALSE: Wait until the lasti positioning is completed

int ii = 0;
long currentMust = 0;
BOOL suc = 0;
WORD AnalogValue = 0;
long VoltageValue = 0;      // For analog read
long OutVoltage = 0;        // For analog out
long PositionIsA = 0;       // Actual angle[qc] of motor A
long PositionIsD = 0;       // Actual angle[qc] of motor D
long AngleOriginA = 0;      // Angle origin of motor A
long AngleOriginD = 0;      // Angle origin of motor D
float angleA = 0;           // Actual angle (0-360 [degree]) of motor A
float angleD = 0;           // Actual angle (0-360 [degree]) of motor D
int rotA = 0;               // Number of times motor A has rotated
int rotD = 0;               // Number of times motor D has rotated
long deltaTheta_qc = 0;     // Angle difference [qc]: A-D
float deltaTheta = 0;       // Angle difference [degree]

int main(int argc, char* argv[]) {
    //Connect EPOS4 to PC
    keyHandleA = VCS_OpenDevice(deviceName, protocolStackName, interfaceName, portNameA, &errorCode);
    keyHandleD = VCS_OpenDevice(deviceName, protocolStackName, interfaceName, portNameD, &errorCode);
    success_key = (BOOL)keyHandleA * (BOOL)keyHandleD;

    if (success_key) {

        // Set protocol stack setting
        setProtA = VCS_SetProtocolStackSettings(keyHandleA, Baudrate, Timeout, &errorCode);
        setProtD = VCS_SetProtocolStackSettings(keyHandleD, Baudrate, Timeout, &errorCode);
        success_prot = setProtA * setProtD;

        if (success_prot) {
            VCS_ClearFault(keyHandleA, nodeId, &errorCode);  // Change the device status from "fault" to "disable
            VCS_ClearFault(keyHandleD, nodeId, &errorCode);  // Change the device status from "fault" to "disable

            // Configuration
            // Set motor type as "EC motor block commutated"
            VCS_SetMotorType(keyHandleA, nodeId, MotorType, &errorCode);
            VCS_SetMotorType(keyHandleD, nodeId, MotorType, &errorCode);


            // Set motor as EC motor 633399(for testing) or 658677(for experiment)
            printf_s("Which motor you use?\n1: 633399, 2: 658677\n");
            scanf_s("%d", &motorNo);
            if (motorNo == 1) {
                // Motor is EC motor 633399
                NominalCurrent = 5140;
                MaxOutputCurrent = 5000;
                ThermalTimeConstant = 414;
                NbOfPolePairs = 7;
            }
            else if (motorNo == 2) {
                // Motor is EC motor 658677
                NominalCurrent = 2610;
                MaxOutputCurrent = 2500;
                ThermalTimeConstant = 414;
                NbOfPolePairs = 7;
            }
            else {
                printf_s("Exit program.\n");
                VCS_ResetDevice(keyHandleA, nodeId, &errorCode);
                VCS_ResetDevice(keyHandleD, nodeId, &errorCode);
                VCS_SetDisableState(keyHandleA, nodeId, &errorCode);
                VCS_SetDisableState(keyHandleD, nodeId, &errorCode);
                VCS_CloseDevice(keyHandleA, &errorCode);
                VCS_CloseDevice(keyHandleD, &errorCode);
                return 0;
            }
            VCS_SetEcMotorParameter(keyHandleA, nodeId, NominalCurrent, MaxOutputCurrent, ThermalTimeConstant, NbOfPolePairs, &errorCode);
            VCS_SetEcMotorParameter(keyHandleD, nodeId, NominalCurrent, MaxOutputCurrent, ThermalTimeConstant, NbOfPolePairs, &errorCode);

            // Set sensor type
            VCS_SetSensorType(keyHandleA, nodeId, sensorType, &errorCode);
            VCS_SetSensorType(keyHandleD, nodeId, sensorType, &errorCode);
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            // Meomo for analog out
            /*
            ///////////////////////////////// When using AnOUT1 on pin 5, the output number is 1
            VCS_AnalogOutputConfiguration(keyHandle, nodeId, 1, 0, &errorCode);
            //VCS_SetAnalogOutput(keyHandle, nodeId, 1, 4000, &errorCode);
            VCS_SetAnalogOutputVoltage(keyHandle, nodeId, 1, 4000, &errorCode);
            //VCS_SetAnalogOutput(keyHandle, nodeId, 5, 4000, &errorCode);
            Sleep(10000);
            VCS_SetAnalogOutput(keyHandle, nodeId, 1, 2000, &errorCode);
            Sleep(10000);
            VCS_SetAnalogOutput(keyHandle, nodeId, 1, 0, &errorCode);
            /////////////////////////////////
            */


            VCS_GetPositionIs(keyHandleA, nodeId, &PositionIsA, &errorCode);  // Current angle of motor A
            VCS_GetPositionIs(keyHandleD, nodeId, &PositionIsD, &errorCode);  // Current angle of motor D
            AngleOriginA = PositionIsA;
            AngleOriginD = PositionIsD;
            printf_s("Let the current angleA %ld [qc] be 0 deg.\n", AngleOriginA);
            printf_s("Let the current angleD %ld [qc] be 0 deg.\n", AngleOriginD);
            deltaTheta_qc = 0;
            deltaTheta = 0;

            //////////////////////////////// Cyclic synchronous current mode
            VCS_SetOperationMode(keyHandleD, nodeId, -3, &errorCode); //Set operation mode. See p.67 of Command Library
            VCS_ActivateCurrentMode(keyHandleD, nodeId, &errorCode);
            VCS_ActivateAnalogCurrentSetpoint(keyHandleD, nodeId, 1, (float)100.0, (short)0, &errorCode);   // Use analog input terminal 1, 100 mA, offset 0 mA
            VCS_EnableAnalogCurrentSetpoint(keyHandleD, nodeId, &errorCode);
            VCS_SetEnableState(keyHandleD, nodeId, &errorCode);

            while (1) {
                VCS_GetAnalogInputVoltage(keyHandleD, nodeId, 2, &VoltageValue, &errorCode);
                if (VoltageValue > 1000) {
                    // Current angle of motor
                    VCS_GetPositionIs(keyHandleA, nodeId, &PositionIsA, &errorCode);
                    VCS_GetPositionIs(keyHandleD, nodeId, &PositionIsD, &errorCode);
                    // Convert units from qc to deg
                    angleA = ((float)PositionIsA - (float)AngleOriginA) / 16384.0 * 360.0;
                    angleD = ((float)PositionIsD - (float)AngleOriginD) / 16384.0 * 360.0;
                    // Number of rotations
                    rotA = (int)angleA / 360;
                    rotD = (int)angleD / 360;
                    // Convert rotation angle to 0-360 deg
                    angleA = angleA - (float)(rotA * 360);
                    angleD = angleD - (float)(rotD * 360);
                    if (angleA < 0) {
                        angleA = angleA + 360.0;
                    }
                    if (angleD < 0) {
                        angleD = angleD + 360.0;
                    }
                    // Calculate angle difference
                    deltaTheta_qc = (PositionIsA - AngleOriginA) + (PositionIsD - AngleOriginD);
                    deltaTheta = (float)deltaTheta_qc / 16384.0 * 360.0;
                    //printf("The current angle of motor A is %f deg.\n", angleA);
                    //printf("The current angle of motor D is %f deg.\n", angleD);
                    printf("deltaTheta = %ld [qc] = %f [degree]\n", deltaTheta_qc, deltaTheta);

                    // Analog out and analog in

                    //OutVoltage = 10000 / 360 * ((long)angleA) - 5000;
                    //OutVoltage = 10000 / 360 * ((long)angleD) - 5000;
                    //VCS_SetAnalogOutputVoltage(keyHandleA, nodeId, 1, 4000, &errorCode);
                    //VCS_SetAnalogOutputVoltage(keyHandleD, nodeId, 1, 4000, &errorCode);
                    //VCS_GetAnalogInput(keyHandle, nodeId, 1, &AnalogValue, &errorCode);
                    //VCS_GetAnalogInputVoltage(keyHandle, nodeId, 1, &VoltageValue, &errorCode);
                    //printf_s("Analog Inputの値は%d\n", AnalogValue);
                    //printf_s("Analog Input Voltageの値は%ld [mV]\n", VoltageValue);
                    Sleep(1000);
                }
                else {
                    // safety button
                    VCS_DeactivateAnalogCurrentSetpoint(keyHandleD, nodeId, 1, &errorCode);
                    VCS_DisableAnalogCurrentSetpoint(keyHandleD, nodeId, &errorCode);
                    printf_s("Operation is stopped because the switch is pressed.\n");
                    break;
                }
            }

            VCS_ResetDevice(keyHandleA, nodeId, &errorCode);
            VCS_ResetDevice(keyHandleD, nodeId, &errorCode);
            VCS_SetDisableState(keyHandleA, nodeId, &errorCode);
            VCS_SetDisableState(keyHandleD, nodeId, &errorCode);
        }
    }


    VCS_CloseDevice(keyHandleA, &errorCode);
    VCS_CloseDevice(keyHandleD, &errorCode);
}


