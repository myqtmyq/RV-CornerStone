/**
 * @brief 初始化全局变量(结构体)
 *
 */
#include "handle.h"
#include "config.h"

void Handle_Init(void) {
    //电机初始化
    Motor_Init(&Motor_Yaw, 1.0, 1);
    Motor_Init(&Motor_Pitch, 1.0, 1);
    Motor_Init(&Motor_Roll, 1.0, 1);
    Motor_Init(&Motor_Stir, 36, 1);
    Motor_Pitch.position     = 2740;
    Motor_Pitch.positionBias = 2740;
    Motor_Yaw.position       = 3538;
    Motor_Yaw.positionBias   = 3538;

    // 遥控器数据初始化
    DBUS_Init(&remoteData, &keyboardData, &mouseData);

    // 初始化串口调试数据
    Magic_Init(&magic, 0);

    // 通讯协议初始化
    Protocol_Init(&Judge);
    Protocol_Init(&Ps);
}
