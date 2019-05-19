#include "Driver_BSP.h"
#include "Driver_DBUS.h"
#include "math.h"
#include "macro.h"

void BSP_CAN_Init(void) {
    // GPIO
    GPIO_InitTypeDef GPIO_InitStructure;
    // CAN1
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_CAN1);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_CAN1);
    // CAN2
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_CAN2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_CAN2);

    // CAN
    CAN_InitTypeDef       CAN_InitStructure;
    CAN_FilterInitTypeDef CAN_FilterInitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);
    // CAN1
    CAN_InitStructure.CAN_ABOM      = DISABLE;
    CAN_InitStructure.CAN_AWUM      = DISABLE;
    CAN_InitStructure.CAN_BS1       = CAN_BS1_9tq;
    CAN_InitStructure.CAN_BS2       = CAN_BS2_5tq;
    CAN_InitStructure.CAN_Mode      = CAN_Mode_Normal;
    CAN_InitStructure.CAN_NART      = DISABLE;
    CAN_InitStructure.CAN_Prescaler = 3;
    CAN_InitStructure.CAN_RFLM      = DISABLE;
    CAN_InitStructure.CAN_SJW       = CAN_SJW_1tq;
    CAN_InitStructure.CAN_TTCM      = DISABLE;
    CAN_InitStructure.CAN_TXFP      = DISABLE;
    CAN_Init(CAN1, &CAN_InitStructure);
    CAN_FilterInitStructure.CAN_FilterActivation     = ENABLE;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
    CAN_FilterInitStructure.CAN_FilterIdHigh         = 0x0000;
    CAN_FilterInitStructure.CAN_FilterIdLow          = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh     = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow      = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMode           = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterNumber         = 0;
    CAN_FilterInitStructure.CAN_FilterScale          = CAN_FilterScale_32bit;
    CAN_FilterInit(&CAN_FilterInitStructure);
    // CAN2
    CAN_InitStructure.CAN_ABOM      = ENABLE;
    CAN_InitStructure.CAN_AWUM      = DISABLE;
    CAN_InitStructure.CAN_BS1       = CAN_BS1_9tq;
    CAN_InitStructure.CAN_BS2       = CAN_BS2_5tq;
    CAN_InitStructure.CAN_Mode      = CAN_Mode_Normal;
    CAN_InitStructure.CAN_NART      = DISABLE;
    CAN_InitStructure.CAN_Prescaler = 3;
    CAN_InitStructure.CAN_RFLM      = DISABLE;
    CAN_InitStructure.CAN_SJW       = CAN_SJW_1tq;
    CAN_InitStructure.CAN_TTCM      = DISABLE;
    CAN_InitStructure.CAN_TXFP      = DISABLE;
    CAN_Init(CAN2, &CAN_InitStructure);
    CAN_FilterInitStructure.CAN_FilterActivation     = ENABLE;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
    CAN_FilterInitStructure.CAN_FilterIdHigh         = 0x0000;
    CAN_FilterInitStructure.CAN_FilterIdLow          = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh     = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow      = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMode           = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterNumber         = 14;
    CAN_FilterInitStructure.CAN_FilterScale          = CAN_FilterScale_32bit;
    CAN_FilterInit(&CAN_FilterInitStructure);

    // NVIC
    NVIC_InitTypeDef NVIC_InitStructure;
    // CAN1
    NVIC_InitStructure.NVIC_IRQChannel                   = CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 8;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_Init(&NVIC_InitStructure);
    CAN_ITConfig(CAN1, CAN_IT_EWG | CAN_IT_EPV | CAN_IT_BOF | CAN_IT_LEC | CAN_IT_ERR | CAN_IT_WKU | CAN_IT_SLK, ENABLE);
    CAN1->IER |= 0xFF;
    // CAN2
    NVIC_InitStructure.NVIC_IRQChannel                   = CAN2_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 8;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_Init(&NVIC_InitStructure);
    CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);
}

void BSP_DBUS_Init(uint8_t *remoteBuffer) {
    // GPIO
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);
    // USART
    USART_InitTypeDef USART_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    USART_InitStructure.USART_BaudRate            = 100000;                         //波特率设置
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //硬件数据流控制
    USART_InitStructure.USART_Mode                = USART_Mode_Rx;                  //收发模式
    USART_InitStructure.USART_Parity              = USART_Parity_No;                //奇偶校验位
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;               //停止位
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;            //字长
    USART_Init(USART1, &USART_InitStructure);                                       //初始化串口
    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);                                  // DMA设置
    USART_Cmd(USART1, ENABLE);                                                      //使能串口
    // DMA
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
    DMA_InitStructure.DMA_Channel            = DMA_Channel_4;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->DR);
    DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)(remoteBuffer);
    DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_BufferSize         = DBUS_LENGTH + DBUS_BACK_LENGTH;
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_Medium;
    DMA_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;
    DMA_Init(DMA2_Stream2, &DMA_InitStructure);
    DMA_Cmd(DMA2_Stream2, ENABLE);
    // NVIC
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel                   = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 8;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_Init(&NVIC_InitStructure);
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
}

void BSP_USART3_Init(uint32_t baudRate) {
    // GPIO
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3); // GPIOD8复用为USART3
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3); // GPIOD9复用为USART3
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_9;  // GPIOD8与GPIOD9
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;             //复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;         //速度50MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;            //推挽复用输出
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;             //上拉
    GPIO_Init(GPIOD, &GPIO_InitStructure);                    //初始化
    // USART
    USART_InitTypeDef USART_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);                          //使能USART3时钟
    USART_InitStructure.USART_BaudRate            = baudRate;                       //波特率设置
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
    USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;  //收发模式
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;            //字长为8位数据格式
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;               //一个停止位
    USART_InitStructure.USART_Parity              = USART_Parity_No;                //无奇偶校验位
    USART_Init(USART3, &USART_InitStructure);                                       //初始化串口
    USART_Cmd(USART3, ENABLE);                                                      //使能串口
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);                                  //开启相关中断
    // NVIC
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel                   = USART3_IRQn; //串口3中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;           //抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;           //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;      // IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);                                     //根据指定的参数初始化VIC寄存器
}

void BSP_USART6_Init(uint32_t baudRate) {
    // GPIO
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource9, GPIO_AF_USART6);  // GPIOG9复用为USART6
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource14, GPIO_AF_USART6); // GPIOG14复用为USART6
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_14;  // GPIOG9与GPIOG14
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;              //复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;          //速度50MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;             //推挽复用输出
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;              //上拉
    GPIO_Init(GPIOG, &GPIO_InitStructure);                     //初始化
    // USART
    USART_InitTypeDef USART_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);                          //使能USART6时钟
    USART_InitStructure.USART_BaudRate            = baudRate;                       //波特率设置
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;            //字长为8位数据格式
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;               //一个停止位
    USART_InitStructure.USART_Parity              = USART_Parity_No;                //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;  //收发模式
    USART_Init(USART6, &USART_InitStructure);                                       //初始化串口
    USART_Cmd(USART6, ENABLE);                                                      //使能串口
    USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);                                  //开启相关中断
    // NVIC
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel                   = USART6_IRQn; //串口3中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;           //抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;           //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;      // IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);                                     //根据指定的参数初始化VIC寄存器
}

void BSP_UART7_Init(uint32_t baudRate) {
    // GPIO
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource7, GPIO_AF_UART7); // GPIOE7复用为UART7
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource8, GPIO_AF_UART7); // GPIOE8复用为UART7
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7 | GPIO_Pin_8; // GPIOE7与GPIOE8
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;            //复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        //速度50MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;           //推挽复用输出
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;            //上拉
    GPIO_Init(GPIOE, &GPIO_InitStructure);                   //初始化
    // USART
    USART_InitTypeDef USART_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART7, ENABLE);                           //使能UART7时钟
    USART_InitStructure.USART_BaudRate            = baudRate;                       //波特率设置
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;            //字长为8位数据格式
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;               //一个停止位
    USART_InitStructure.USART_Parity              = USART_Parity_No;                //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;  //收发模式
    USART_Init(UART7, &USART_InitStructure);                                        //初始化串口
    USART_Cmd(UART7, ENABLE);                                                       //使能串口
    USART_ITConfig(UART7, USART_IT_RXNE, ENABLE);                                   //开启相关中断
    // NVIC
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel            = UART7_IRQn; //串口7中断通道
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;          //子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd         = ENABLE;     // IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);                             //根据指定的参数初始化VIC寄存器
}

void BSP_UART8_Init(uint32_t baudRate) {
    // GPIO
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource0, GPIO_AF_UART8); // GPIOE0复用为UART8
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource1, GPIO_AF_UART8); // GPIOE1复用为UART8
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1; // GPIOE0与GPIOE1
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;            //复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        //速度50MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;           //推挽复用输出
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;            //上拉
    GPIO_Init(GPIOE, &GPIO_InitStructure);                   //初始化
    // USART
    USART_InitTypeDef USART_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART8, ENABLE);                           //使能UART8时钟
    USART_InitStructure.USART_BaudRate            = baudRate;                       //波特率设置
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;            //字长为8位数据格式
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;               //一个停止位
    USART_InitStructure.USART_Parity              = USART_Parity_No;                //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;  //收发模式
    USART_Init(UART8, &USART_InitStructure);                                        //初始化串口
    USART_Cmd(UART8, ENABLE);                                                       //使能串口
    USART_ITConfig(UART8, USART_IT_RXNE, ENABLE);                                   //开启相关中断
    // NVIC
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel            = UART8_IRQn; //串口8中断通道
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;          //子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd         = ENABLE;     // IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);                             //根据指定的参数初始化VIC寄存器
}

void BSP_Laser_Init(void) {
    // Laser
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOG, &GPIO_InitStructure);
}

void BSP_User_Power_Init(void) {
    // 24V User Power
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOH, &GPIO_InitStructure);
    GPIO_SetBits(GPIOH, GPIO_Pin_2);
    GPIO_SetBits(GPIOH, GPIO_Pin_3);
    GPIO_SetBits(GPIOH, GPIO_Pin_4);
    GPIO_SetBits(GPIOH, GPIO_Pin_5);
}

void BSP_IMU_Init(void) {
    // IIC
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOF, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOF, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOF, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOF, &GPIO_InitStructure);

    // IMU中断
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // NVIC
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel                   = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 8;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // EXTI
    EXTI_InitTypeDef EXTI_InitStructure;
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, GPIO_PinSource8);
    EXTI_InitStructure.EXTI_Line    = EXTI_Line8;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿中断
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
}

void BSP_TIM2_Init(void) {
    // GPIO
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM2); // GPIOA1复用为定时器2
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;             // GPIOB4
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;           //复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;      //速度100MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          //推挽复用输出
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;           //上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);                  //初始化
    // TIM
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure; // TIM 频率
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_TimeBaseInitStructure.TIM_Period        = 2 - 1;
    TIM_TimeBaseInitStructure.TIM_Prescaler     = 9000 - 1;
    TIM_TimeBaseInitStructure.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
    TIM_Cmd(TIM2, ENABLE);
    // NVIC
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel                   = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 9;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
}

/**
 * @brief DMA2驱动
 *
 * @param DMA_PeripheralBaseAddr 从哪里
 * @param DMA_Memory0BaseAddr    复制到哪里
 * @param DMA_BufferSize         还有长度
 */
void BSP_DMA2_Init(uint32_t DMA_PeripheralBaseAddr, uint32_t DMA_Memory0BaseAddr, uint32_t DMA_BufferSize) {
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel                   = DMA2_Stream1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    DMA_ITConfig(DMA2_Stream1, DMA_IT_TC, ENABLE);
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
    DMA_InitStructure.DMA_Channel            = DMA_Channel_5;
    DMA_InitStructure.DMA_PeripheralBaseAddr = DMA_PeripheralBaseAddr;
    DMA_InitStructure.DMA_Memory0BaseAddr    = DMA_Memory0BaseAddr;
    DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_BufferSize         = DMA_BufferSize;
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_Medium;
    DMA_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;
    DMA_Init(DMA2_Stream1, &DMA_InitStructure);
    DMA_Cmd(DMA2_Stream1, ENABLE);
}

/**
 * @brief 设置PWM所使用的端口
 * @param PWMx   PWM结构体
 * @param PWM_Px 使用的端口,如PWM_PORT_PD12
 */
void BSP_PWM_Set_Port(PWM_Type *PWMx, uint32_t PWM_Px) {
    PWMx->RCC_APBxPeriph_TIMx  = PWM_Px >> 28;
    uint32_t x                 = PWMx->RCC_APBxPeriph_TIMx;
    uint32_t TIMx_Base         = ((PWM_Px >> 24 & 0x0F) << 24) + ((x == 4 ? 8 : (x == 8 ? 0xc : (x == 1 ? 0 : 4))) << 8) + PERIPH_BASE;
    PWMx->TIMx                 = (TIM_TypeDef *) TIMx_Base;
    PWMx->GPIO_AF_TIMx         = PWM_Px >> 20 & 0xF;
    PWMx->RCC_AHB1Periph_GPIOx = PWM_Px >> 4 & 0x0FFF;
    uint32_t GPIOx_Base        = (AHB1PERIPH_BASE + log(PWMx->RCC_AHB1Periph_GPIOx) / log(2) * 0x400);
    PWMx->GPIOx                = (GPIO_TypeDef *) GPIOx_Base;
    PWMx->GPIO_PinSourcex      = PWM_Px & 0x0F;
    PWMx->GPIO_Pin_x           = 1 << (PWM_Px & 0x0F);
    PWMx->Channel              = PWM_Px >> 16 & 0xF;
    uint8_t y[4]               = {0x34, 0x38, 0x3C, 0x40};
    PWMx->CCRx                 = y[PWMx->Channel - 1];
}

/**
 * @brief 初始化PWM
 * @note  PI5,PI6,PI7,PI2对应时钟频率为180MHz,其余为90MHz
 * @param PWMx      PWM结构体
 * @param prescaler 预分频器. PWM频率   = TIM/prescaler
 * @param period    计数上限. PWM占空比 = compare/period
 * @param polarity  输出极性. TIM_OCPolarity_Low/TIM_OCPolarity_High
 */
void BSP_PWM_Init(PWM_Type *PWMx, uint16_t prescaler, uint32_t period, uint16_t polarity) {
    // InitStructure
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure; // TIM 频率
    GPIO_InitTypeDef        GPIO_InitStructure;        // TIM4_PWM GPIO口设置
    TIM_OCInitTypeDef       TIM_OCInitStructure;       // TIM4_PWM PWM模式

    // GPIO
    RCC_AHB1PeriphClockCmd(PWMx->RCC_AHB1Periph_GPIOx, ENABLE);               // 使能时钟
    GPIO_PinAFConfig(PWMx->GPIOx, PWMx->GPIO_PinSourcex, PWMx->GPIO_AF_TIMx); // GPIO复用为定时器
    GPIO_InitStructure.GPIO_Pin   = PWMx->GPIO_Pin_x;                         // GPIO
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;                             // 复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;                        // 速度100MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                            // 推挽复用输出
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;                             // 上拉
    GPIO_Init(PWMx->GPIOx, &GPIO_InitStructure);                              // 初始化

    // TIM
    RCC_APB1PeriphClockCmd(PWMx->RCC_APBxPeriph_TIMx, ENABLE);        // 时钟使能
    TIM_TimeBaseInitStructure.TIM_Prescaler     = prescaler - 1;      // 定时器分频
    TIM_TimeBaseInitStructure.TIM_CounterMode   = TIM_CounterMode_Up; // 向上计数模式
    TIM_TimeBaseInitStructure.TIM_Period        = period - 1;         // 自动重装载值
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;       // ClockDivision
    TIM_TimeBaseInit(PWMx->TIMx, &TIM_TimeBaseInitStructure);         // 初始化定时器

    // TIM_OC
    TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM2;        // 选择定时器模式:TIM脉冲宽度调制模式2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 比较输出使能
    TIM_OCInitStructure.TIM_OCPolarity  = polarity;               // 输出极性:TIM输出比较极性低
    TIM_OCInitStructure.TIM_Pulse       = 5;                      // Pulse
    if (PWMx->Channel == 1) {
        TIM_OC1Init(PWMx->TIMx, &TIM_OCInitStructure);          // 根据指定的参数初始化外设
        TIM_OC1PreloadConfig(PWMx->TIMx, TIM_OCPreload_Enable); // 使能TIM在CCR1上的预装载寄存器
    } else if (PWMx->Channel == 2) {
        TIM_OC2Init(PWMx->TIMx, &TIM_OCInitStructure);          // 根据指定的参数初始化外设
        TIM_OC2PreloadConfig(PWMx->TIMx, TIM_OCPreload_Enable); // 使能TIM在CCR1上的预装载寄存器
    } else if (PWMx->Channel == 3) {
        TIM_OC3Init(PWMx->TIMx, &TIM_OCInitStructure);          // 根据指定的参数初始化外设
        TIM_OC3PreloadConfig(PWMx->TIMx, TIM_OCPreload_Enable); // 使能TIM在CCR1上的预装载寄存器
    } else if (PWMx->Channel == 4) {
        TIM_OC4Init(PWMx->TIMx, &TIM_OCInitStructure);          // 根据指定的参数初始化外设
        TIM_OC4PreloadConfig(PWMx->TIMx, TIM_OCPreload_Enable); // 使能TIM在CCR1上的预装载寄存器
    }
    TIM_ARRPreloadConfig(PWMx->TIMx, ENABLE); // ARPE使能
    TIM_Cmd(PWMx->TIMx, ENABLE);              // 使能TIM
}

/**
 * @brief 设置占空比
 * @param PWMx    PWM结构体
 * @param compare 比较值. PWM占空比 = compare/period
 */
void PWM_Set_Compare(PWM_Type *PWMx, uint32_t compare) {
    *((uint32_t *) (((uint8_t *) PWMx->TIMx) + PWMx->CCRx)) = compare;
}

void BSP_LED_Init(void) {
    // 用户自定义LED*8
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin   = 0x01FE;            // GPIO_Pin_1-GPIO_Pin_8
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;     // 普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // 100MHz
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;      // 上拉
    GPIO_Init(GPIOG, &GPIO_InitStructure);             // 初始化
    GPIO_SetBits(GPIOG, 0x01FE);                       // GPIOG1-8设置高,灯灭
}

/**
 * @brief 设置LED亮暗
 * @param row 共8位表示8个灯,1为亮,0为暗
 */
void LED_Set_Row(uint16_t row) {
    GPIO_SetBits(GPIOG, ~(row << 1));
    GPIO_ResetBits(GPIOG, row << 1);
}

/**
 * @brief 设置LED亮起数量
 * @param progress 0-8
 */
void LED_Set_Progress(uint16_t progress) {
    // 设置
    MIAO(progress, 0, 8);
    LED_Set_Row((1 << progress) - 1);
}

/**
 * @brief 炫酷跑马灯
 * @note  每次调用本函数会更新LED状态,但没有延时
 *        建议每次调用后设置20ms延时
 */
uint16_t LEDHorseRow   = 0;
uint16_t LEDHorseState = 0;
void     LED_Run_Horse() {
    LEDHorseState = (LEDHorseState % 26) + 1;
    if (LEDHorseState <= 8)
        LEDHorseRow = (LEDHorseRow << 1) + 1;
    else if (LEDHorseState >= 14 && LEDHorseState <= 21)
        LEDHorseRow = LEDHorseRow - (1 << LEDHorseState - 14);
    LED_Set_Row(LEDHorseRow);
}

/**
 * @brief Windows XP开机动画跑马灯
 * @note  每次调用本函数会更新LED状态,但没有延时
 *        建议每次调用后设置200ms延时
 */
uint16_t LEDXPRow   = 0;
uint16_t LEDXPState = 0;
void     LED_Run_Horse_XP() {
    LEDXPState = (LEDXPState % 11) + 1;
    if (LEDXPState <= 3)
        LEDXPRow = (LEDXPRow << 1) + 1;
    else if (LEDXPState >= 4 && LEDXPState <= 8)
        LEDXPRow = LEDXPRow << 1;
    else
        LEDXPRow = LEDXPRow - (1 << LEDXPState - 4);
    LED_Set_Row(LEDXPRow);
}

void BSP_Button_Init(void) {
    //占坑
}

void BSP_Beep_Init(void) {
    GPIO_InitTypeDef        GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef       TIM_OCInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE); // TIM12时钟使能
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE); //使能GPIOH时钟

    GPIO_PinAFConfig(GPIOH, GPIO_PinSource6, GPIO_AF_TIM12); // GPIOH6复用为定时器12

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;        // GPIOH6
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;      //复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //速度100MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     //推挽复用输出
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;      //上拉
    GPIO_Init(GPIOH, &GPIO_InitStructure);             //初始化PH6

    TIM_TimeBaseStructure.TIM_Prescaler     = 90 - 1;             //定时器分频
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up; //向上计数模式
    TIM_TimeBaseStructure.TIM_Period        = 1;                  //自动重装载值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM_TimeBaseInit(TIM12, &TIM_TimeBaseStructure); //初始化定时器3

    //初始化TIM12 Channel1 PWM模式
    TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM1;        //选择定时器模式:TIM脉冲宽度调制模式2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_High;    //输出极性:TIM输出比较极性高
    TIM_OC1Init(TIM12, &TIM_OCInitStructure);                     //根据T指定的参数初始化外设TIM12 OC1

    TIM_OC1PreloadConfig(TIM12, TIM_OCPreload_Enable); //使能TIM12在CCR1上的预装载寄存器

    TIM_ARRPreloadConfig(TIM12, ENABLE); // ARPE使能

    TIM_Cmd(TIM12, ENABLE); //使能TIM3
}

/**
 * @brief 播放XP开机音乐
 * @note  如果播放完成返回1,任务应退出循环并自杀
 */
uint16_t BeepXPState = 0;
void     Beep_Sing_XP(void) {
    if (BeepXPState == BEEP_XP_LENGTH) {
        BEEP_CH     = 0;
        BeepXPState = 0;
        return 1;
    } else {
        BEEP_ARR = BeepXPScore[BeepXPState];
        BEEP_CH  = BeepXPScore[BeepXPState] / 2;
        BeepXPState++;
        return 0;
    }
}

/**
 * @brief 播放天空之城
 * @note  如果播放完成返回1,任务应退出循环并自杀
 */
uint16_t BeepSkyState = 0;
void     Beep_Sing_Sky(void) {
    if (BeepSkyState == Startup_Success_music_len_sky) {
        BEEP_CH      = 0;
        BeepSkyState = 0;
        return 1;
    } else {
        Sing(Mavic_Startup_music_XP[BeepSkyState]);
        BeepXPState++;
        return 0;
    }
}

void BSP_I2C2_Init(void) {
    //占坑
}
