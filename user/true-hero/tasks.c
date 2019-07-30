/**
 * @brief 真*英雄
 * @version 1.2.0
 */
#include "main.h"

void Task_Safe_Mode(void *Parameters) {
    while (1) {
        if (remoteData.switchRight == 2) {
            vTaskSuspendAll();
            while (1) {
                Can_Send(CAN1, 0x200, 0, 0, 0, 0);
                Can_Send(CAN1, 0x1FF, 0, 0, 0, 0);
                vTaskDelay(2);
            }
        }
        vTaskDelay(2);
    }
    vTaskDelete(NULL);
}

void Task_Control(void *Parameters) {
    TickType_t LastWakeTime = xTaskGetTickCount();
    while (1) {
        // Keyboard_Check(&CTRL, &remoteData, KEY_CTRL);

        if (remoteData.switchLeft == 1 || remoteData.switchLeft == 3) {
            controlMode = 1; //遥控器模式
        } else if (remoteData.switchLeft == 2) {
            controlMode = 2; //键鼠模式
        }
        vTaskDelayUntil(&LastWakeTime, 10);
    }
    vTaskDelete(NULL);
}

void Task_Gimbal(void *Parameters) {
    //云台
    TickType_t LastWakeTime = xTaskGetTickCount(); //时钟
    //初始化云台PID
    PID_Init(&PID_Cloud_YawAngle, 25, 0, 0, 4000, 0);
    PID_Init(&PID_Cloud_YawSpeed, 40, 0, 0, 5000, 0);
    PID_Init(&PID_Cloud_PitchAngle, 30, 0, 0, 5000, 0);
    PID_Init(&PID_Cloud_PitchSpeed, 8, 0.1, 0, 4000, 1000);

    float yawTargetAngle   = 0;
    float pitchTargetAngle = 0;
    int   yawSpeedTarget   = 0;
    int   pitchSpeedTarget = 0;

    float yawAngleTarget   = 0;
    float pitchAngleTarget = 0;

    while (1) {
        if (controlMode == 1) {
            if (ABS(remoteData.rx) > 30) yawAngleTarget += remoteData.rx / 660.0f * 180 * 0.005;
            if (ABS(remoteData.ry) > 30) pitchAngleTarget += remoteData.ry / 660.0f * 100 * 0.005;
        } else if (controlMode == 2) {
            yawAngleTarget += mouseData.x * 0.02;
            pitchAngleTarget += (-mouseData.y) * 0.01;
        }

        MIAO(yawAngleTarget, -130, 130);
        MIAO(pitchAngleTarget, 0, 73);

        PID_Calculate(&PID_Cloud_YawAngle, yawAngleTarget, Motor_Yaw.angle);
        PID_Calculate(&PID_Cloud_YawSpeed, PID_Cloud_YawAngle.output, Motor_Yaw.speed);

        PID_Calculate(&PID_Cloud_PitchAngle, pitchAngleTarget, Motor_Pitch.angle);
        PID_Calculate(&PID_Cloud_PitchSpeed, PID_Cloud_PitchAngle.output, Motor_Pitch.speed);

        // 输出电流
        Can_Send(CAN1, 0x1FF, PID_Cloud_YawSpeed.output, PID_Cloud_PitchSpeed.output, 0, 0);

        vTaskDelayUntil(&LastWakeTime, 5);
    }
}

void Task_Chassis(void *Parameters) {
    // 任务
    TickType_t LastWakeTime = xTaskGetTickCount(); // 时钟
    float      interval     = 0.005;               // 任务运行间隔 s
    int        intervalms   = interval * 1000;     // 任务运行间隔 ms

    // 底盘运动
    float vx = 0;
    float vy = 0;
    float vw = 0;
    float targetPower;

    // 反馈值
    float motorAngle;
    float filter[6] = {0, 0, 0, 0, 0, 0};
    int   filterp   = 0;
    float motorSpeedStable;
    float power = 0;

    // 底盘跟随PID
    float followDeadRegion = 3.0;
    PID_Init(&PID_Follow_Angle, 20, 0, 0, 2000, 0);
    PID_Init(&PID_Follow_Speed, 0.3, 0, 0, 1000, 0);

    // 麦轮速度PID
    PID_Init(&PID_LFCM, 20, 0.2, 0, 15000, 7500);
    PID_Init(&PID_LBCM, 20, 0.2, 0, 15000, 7500);
    PID_Init(&PID_RBCM, 20, 0.2, 0, 15000, 7500);
    PID_Init(&PID_RFCM, 20, 0.2, 0, 15000, 7500);

    // 初始化底盘
    Chassis_Init(&ChassisData);

    //地盘运动斜坡函数
    float xRampProgress = 0;
    float xRampStart    = 0;
    float xTargetRamp   = 0;
    float yRampProgress = 0;
    float yRampStart    = 0;
    float yTargetRamp   = 0;

    while (1) {
        //     Keyboard_Check(&D, &remoteData, KEY_D);
        //     Keyboard_Check(&A, &remoteData, KEY_A);
        //     Keyboard_Check(&S, &remoteData, KEY_S);
        //     Keyboard_Check(&W, &remoteData, KEY_W);
        // 设置反馈值
        motorAngle = Motor_Yaw.angle;                   // 电机角度
        power      = Judge.powerHeatData.chassis_power; // 裁判系统功率

        // 对电机角速度进行平均值滤波
        filter[filterp] = motorAngle;
        filterp         = filterp == 5 ? 0 : filterp + 1;
        int i;
        for (i = 0; i < 6; i++) {
            motorSpeedStable += filter[i];
        }
        motorSpeedStable = motorSpeedStable / 6.0f;

        // 根据运动模式计算PID
        PID_Calculate(&PID_Follow_Angle, 0, motorAngle);                             // 计算航向角角度PID
        PID_Calculate(&PID_Follow_Speed, PID_Follow_Angle.output, motorSpeedStable); // 计算航向角角速度PID

        // // 设置底盘总体移动速度
        // if (controlMode == 1) {
        //     vx = -remoteData.lx / 660.0f * 4;
        //     vy = remoteData.ly / 660.0f * 12;
        // } else if (controlMode == 2) {
        //     xTargetRamp = RAMP(xRampStart, 660, xRampProgress);
        //     if (xRampProgress < 1) {
        //         xRampProgress += 0.005f;
        //     }
        //     yTargetRamp = RAMP(yRampStart, 660, yRampProgress);
        //     if (yRampProgress < 1) {
        //         yRampProgress += 0.005f;
        //     }
        //     vx = (A.state - D.state) * xTargetRamp / 660.0f * 4;
        //     vy = (W.state - S.state) * yTargetRamp / 660.0f * 12;

        //     if (W.state == 0 && S.state == 0) {
        //         yRampStart = 0;
        //     } else if (A.state == 0 && D.state == 0) {
        //         xRampStart = 0;
        //     }
        // }
        vw = ABS(PID_Follow_Angle.error) < followDeadRegion ? 0 : (-1 * PID_Follow_Speed.output * DPS2RPS);

        // 麦轮解算及限速
        targetPower = 80.0 - (60.0 - ChassisData.powerBuffer) / 60.0 * 80.0; // 设置目标功率
        Chassis_Update(&ChassisData, vx, vy, vw);                            // 更新麦轮转速
        // Chassis_Fix(&ChassisData, motorAngle);                               // 修正旋转后底盘的前进方向
        Chassis_Calculate_Rotor_Speed(&ChassisData);                         // 麦轮解算
        Chassis_Limit_Rotor_Speed(&ChassisData, CHASSIS_ROTOR_SPEED);        // 设置转子速度上限 (rad/s)
        Chassis_Limit_Power(&ChassisData, 80, targetPower, power, interval); // 根据功率限幅

        // 计算输出电流PID
        PID_Calculate(&PID_LFCM, ChassisData.rotorSpeed[0], Motor_LF.speed * RPM2RPS);
        PID_Calculate(&PID_LBCM, ChassisData.rotorSpeed[1], Motor_LB.speed * RPM2RPS);
        PID_Calculate(&PID_RBCM, ChassisData.rotorSpeed[2], Motor_RB.speed * RPM2RPS);
        PID_Calculate(&PID_RFCM, ChassisData.rotorSpeed[3], Motor_RF.speed * RPM2RPS);

        // 输出电流值到电调
        // Can_Send(CAN1, 0x200, PID_LFCM.output, PID_LBCM.output, PID_RBCM.output, PID_RFCM.output);

        // 底盘运动更新频率
        vTaskDelayUntil(&LastWakeTime, intervalms);

        // 调试信息T
        // DebugData.debug1 = motorAngle;
        // DebugData.debug2 = -1 * PID_Follow_Speed.output;
        // DebugData.debug3 = ChassisData.powerScale * 1000;
        // DebugData.debug4 = Judge.powerHeatData.chassis_power;
        // DebugData.debug5 = motorAngle;
        // DebugData.debug6 = -1 * PID_Follow_Speed.output;
        // DebugData.debug6 = PID_LFCM.output;
        // DebugData.debug7 = ChassisData.rotorSpeed[0];
        // DebugData.debug8 = PID_Follow_Speed.output;
    }

    vTaskDelete(NULL);
}

void Task_Debug_Magic_Send(void *Parameters) {
    TickType_t LastWakeTime = xTaskGetTickCount();
    while (1) {
        taskENTER_CRITICAL(); // 进入临界段
        // printf();
        taskEXIT_CRITICAL(); // 退出临界段
        vTaskDelayUntil(&LastWakeTime, 500);
    }
    vTaskDelete(NULL);
}

/**
 * @brief 发射机构代码
 *
 * @param Parameters
 */
void Task_Fire(void *Parameters) {
    TickType_t LastWakeTime = xTaskGetTickCount();
    //模式
    int   shootMode         = 1; // 0为纯手动控制大拨弹   1为自动控制大拨弹
    int   state             = 0;
    int   counter1          = 0;
    int   counter2          = 0;
    int   counter3          = 0;
    int   stop              = 0;
    int   lastStop          = 0;
    float speedRampProgress = 0;
    float speedRampStart    = 0;
    float speedTargetRamp   = 0;

    //射击参数
    int maxShootHeat = Judge.robotState.shooter_heat1_cooling_limit;

    GPIO_SetBits(GPIOF, GPIO_Pin_0); //微动开关IO口输出高电平

    float rpm2rps = 3.14 / 60;

    // PID 初始化
    PID_Init(&PID_LeftFrictSpeed, 30, 0.1, 0, 10000, 5000);
    PID_Init(&PID_RightFrictSpeed, 30, 0.1, 0, 10000, 5000);
    PID_Init(&PID_Stir2006Speed, 30, 0.1, 0, 5000, 3000);   //半径38
    PID_Init(&PID_Stir3510Speed, 100, 10, 0, 15000, 10000); //半径110

    while (1) {

        // Keyboard_Check(&V, &remoteData, KEY_V);
        // Keyboard_Check(&C, &remoteData, KEY_C);
        // Keyboard_Check(&X, &remoteData, KEY_X);
        // Keyboard_Check(&Z, &remoteData, KEY_Z);
        // Keyboard_Check(&G, &remoteData, KEY_G);
        // Keyboard_Check(&F, &remoteData, KEY_F);
        // Keyboard_Check(&R, &remoteData, KEY_R);
        // Keyboard_Check(&E, &remoteData, KEY_E);
        // Keyboard_Check(&Q, &remoteData, KEY_Q);
        // Keyboard_Check(&SHIFT, &remoteData, KEY_SHIFT);
        // if (remoteData.mouse.pressRight == 1 && CTRL.state == 0) {
        //     pressRight.state = 1;
        // } else if (remoteData.mouse.pressRight == 1 && CTRL.state == 1) {
        //     pressRight.state = 0;
        // }
        // if (controlMode == 2) {
        //     if (R.state == 1 && CTRL.state == 0) {
        //         shootMode = 1;
        //     } else if (R.state == 1 && CTRL.state == 1) {
        //         shootMode = 0;
        //     }
        // } else if (controlMode == 1) {
        //     shootMode = 1;
        // }
        // 2006,3510拨弹轮
        if (shootMode == 1) {
            if (controlMode == 1) {
                if (remoteData.switchLeft == 1) {
                    PID_LeftFrictSpeed.output_I  = 0;
                    PID_RightFrictSpeed.output_I = 0;
                    PID_Calculate(&PID_LeftFrictSpeed, 0, Motor_LeftFrict.speed * rpm2rps);   // 左摩擦轮停止
                    PID_Calculate(&PID_RightFrictSpeed, 0, Motor_RightFrict.speed * rpm2rps); // 右摩擦轮停止

                } else if (remoteData.switchLeft == 3) {
                    PID_Calculate(&PID_RightFrictSpeed, 230, Motor_RightFrict.speed * rpm2rps); // 右摩擦轮
                    PID_Calculate(&PID_LeftFrictSpeed, -230, Motor_LeftFrict.speed * rpm2rps);  // 左摩擦轮

                    LASER_ON; // 激光开启
                }
            }
            if (controlMode == 2) {
                // if (pressRight.state == 0) {
                //     PID_LeftFrictSpeed.output_I  = 0;
                //     PID_RightFrictSpeed.output_I = 0;
                //     PID_Calculate(&PID_LeftFrictSpeed, 0, Motor_LeftFrict.speed * rpm2rps);   // 左摩擦轮停止
                //     PID_Calculate(&PID_RightFrictSpeed, 0, Motor_RightFrict.speed * rpm2rps); // 右摩擦轮停止

                //     LASER_OFF; // 激光关闭
                // } else if (pressRight.state == 1) {
                //     PID_Calculate(&PID_RightFrictSpeed, 230, Motor_RightFrict.speed * rpm2rps); // 右摩擦轮
                //     PID_Calculate(&PID_LeftFrictSpeed, -230, Motor_LeftFrict.speed * rpm2rps);  // 左摩擦轮
                //     LASER_ON;                                                                   // 激光开启
                // }

                if (keyboardData.E == 1 && keyboardData.Ctrl == 0) {
                    PWM_Set_Compare(&PWM_Magazine_Servo, 17);
                } else if (keyboardData.E == 1 && keyboardData.Ctrl == 1) {
                    PWM_Set_Compare(&PWM_Magazine_Servo, 10);
                }
            }

            speedTargetRamp = RAMP(speedRampStart, 35, speedRampProgress);
            if (speedRampProgress < 1) {
                speedRampProgress += 0.1f;
            }
            if (stop == 0) {
                if (controlMode == 1) {
                    if (remoteData.switchRight == 3 && Judge.powerHeatData.shooter_heat1 < (maxShootHeat - 100)) {
                        PID_Calculate(&PID_Stir2006Speed, speedTargetRamp, Motor_Stir2006.speed * rpm2rps);
                        PID_Stir3510Speed.output_I = 0;
                        PID_Stir3510Speed.output   = 0;
                        state                      = 1;
                    } else if (remoteData.switchRight == 1) {
                        state = 0;
                    }
                }
                // if (controlMode == 2) {
                //     if (remoteData.mouse.pressLeft == 1 && Judge.powerHeatData.shooter_heat1 < (maxShootHeat - 100)) {
                //         PID_Calculate(&PID_Stir2006Speed, speedTargetRamp, Motor_Stir2006.speed * rpm2rps);
                //         PID_Stir3510Speed.output_I = 0;
                //         PID_Stir3510Speed.output   = 0;
                //         state                      = 1;
                //     } else if (remoteData.mouse.pressLeft == 0) {
                //         state = 0;
                //     }
                // }
                if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == 0 && state == 0) {
                    PID_Calculate(&PID_Stir2006Speed, speedTargetRamp, Motor_Stir2006.speed * rpm2rps);
                    PID_Calculate(&PID_Stir3510Speed, 4, Motor_Stir3510.speed * rpm2rps);
                } else if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == 1 && state == 0) {
                    PID_Stir2006Speed.output_I = 0;
                    PID_Stir3510Speed.output_I = 0;
                    PID_Calculate(&PID_Stir2006Speed, 0, Motor_Stir2006.speed * rpm2rps);
                    PID_Calculate(&PID_Stir3510Speed, 0, Motor_Stir3510.speed * rpm2rps);
                    speedRampProgress = 0;
                }
            }
        }

        //堵转检测
        if (PID_Stir2006Speed.output > 3000) {
            stop = 1;
        } else {
            stop = 0;
        }

        if (stop == 1 && counter1 < 10) {
            counter1 += 1;
            PID_Stir2006Speed.output = -800;
            PID_Stir3510Speed.output = 0;
        } else if (counter1 == 10 && counter2 < 200) {
            lastStop                   = 0;
            PID_Stir2006Speed.output_I = 0;
            PID_Stir2006Speed.output   = 0;
            PID_Stir3510Speed.output   = 0;
            counter2 += 1;
        }

        if (counter2 == 200) {
            counter1 = 0;
            counter2 = 0;
        }

        // Can_Send(CAN2, 0x200, PID_LeftFrictSpeed.output, PID_RightFrictSpeed.output, PID_Stir2006Speed.output, PID_Stir3510Speed.output);

        vTaskDelayUntil(&LastWakeTime, 10);

        DebugData.debug1 = keyboardData.keyCode;
        DebugData.debug2 = mouseData.x;
        DebugData.debug3 = mouseData.y;
        DebugData.debug4 = mouseData.z;
        DebugData.debug5 = mouseData.pressLeft;
        DebugData.debug6 = mouseData.pressRight;
        DebugData.debug7 = keyboardData.C;
        DebugData.debug8 = keyboardData.Z;
    }
    vTaskDelete(NULL);
}

void Task_Blink(void *Parameters) {
    TickType_t LastWakeTime = xTaskGetTickCount();
    while (1) {
        LED_Run_Horse_XP(); // XP开机动画,建议延时200ms
        // LED_Run_Horse(); // 跑马灯,建议延时20ms
        vTaskDelayUntil(&LastWakeTime, 200);
    }

    vTaskDelete(NULL);
}

void Task_Startup_Music(void *Parameters) {
    TickType_t LastWakeTime = xTaskGetTickCount();
    while (1) {
        if (KTV_Play(Music_Earth)) break;
        vTaskDelayUntil(&LastWakeTime, 120);
    }
    vTaskDelete(NULL);
}

void Task_Sys_Init(void *Parameters) {

    // 初始化全局变量
    Handle_Init();

    // 初始化硬件
    BSP_Init();

    // 初始化陀螺仪
    Gyroscope_Init(&Gyroscope_EulerData);

    //键鼠状态初始化
    // Keyboard_Init(&V);
    // Keyboard_Init(&C);
    // Keyboard_Init(&X);
    // Keyboard_Init(&Z);
    // Keyboard_Init(&G);
    // Keyboard_Init(&F);
    // Keyboard_Init(&R);
    // Keyboard_Init(&E);
    // Keyboard_Init(&Q);
    // Keyboard_Init(&CTRL);
    // Keyboard_Init(&SHIFT);
    // Keyboard_Init(&D);
    // Keyboard_Init(&A);
    // Keyboard_Init(&S);
    // Keyboard_Init(&W);
    // Keyboard_Init(&pressRight);

    // 调试任务
#if DEBUG_ENABLED
    // xTaskCreate(Task_Debug_Magic_Receive, "Task_Debug_Magic_Receive", 500, NULL, 6, NULL);
    // xTaskCreate(Task_Debug_Magic_Send, "Task_Debug_Magic_Send", 500, NULL, 6, NULL);
    // xTaskCreate(Task_Debug_RTOS_State, "Task_Debug_RTOS_State", 500, NULL, 6, NULL);
    // xTaskCreate(Task_Debug_Gyroscope_Sampling, "Task_Debug_Gyroscope_Sampling", 400, NULL, 6,
    // NULL);
#endif

    // 低级任务
    xTaskCreate(Task_Safe_Mode, "Task_Safe_Mode", 500, NULL, 7, NULL);
    xTaskCreate(Task_Blink, "Task_Blink", 400, NULL, 3, NULL);
    // xTaskCreate(Task_Startup_Music, "Task_Startup_Music", 400, NULL, 3, NULL);

    // 等待遥控器开启
    while (!remoteData.state) {
    }

    // 运动控制任务
    // xTaskCreate(Task_Chassis, "Task_Chassis", 400, NULL, 5, NULL);
    // xTaskCreate(Task_Gimbal, "Task_Gimbal", 500, NULL, 5, NULL);
    xTaskCreate(Task_Fire, "Task_Fire", 400, NULL, 5, NULL);

    //模式切换任务
    xTaskCreate(Task_Control, "Task_Control", 400, NULL, 4, NULL);

    // 完成使命
    vTaskDelete(NULL);
    vTaskDelay(10);
}