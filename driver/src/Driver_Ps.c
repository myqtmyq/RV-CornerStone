#include "Driver_Ps.h"
#include "Driver_Magic.h"
#include "handle.h"
#include "debug.h"

enum Status_Type { WAITING, STARTED, VALIDED, INTERRUPTED };
enum Status_Type status = WAITING;

uint8_t len;
uint8_t id;
uint8_t data[34];
uint8_t counter = 0;

uint8_t psDebugEnabled = 0;

void Ps_Init_Config(void) {
    // GPIO端口设置
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);  //使能GPIOD时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART2, ENABLE); //使能USART2时钟

    //串口3对应引脚复用映射
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource5,
                     GPIO_AF_USART6); // GPIOG8复用为USART6
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource6,
                     GPIO_AF_USART6); // GPIOG9复用为USART6

    // USART6端口配置
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_14; // GPIOG9与GPIOG14
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;             //复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;         //速度50MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;            //推挽复用输出
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;             //上拉
    GPIO_Init(GPIOG, &GPIO_InitStructure);                    //初始化

    // USART6 初始化设置
    USART_InitStructure.USART_BaudRate            = bound;                          //波特率设置
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;            //字长为8位数据格式
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;               //一个停止位
    USART_InitStructure.USART_Parity              = USART_Parity_No;                //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;  //收发模式
    USART_Init(USART6, &USART_InitStructure);                                       //初始化串口3

    USART_Cmd(USART6, ENABLE); //使能串口3

    // USART_ClearFlag(USART6, USART_FLAG_TC);

    USART_ITConfig(USART6, USART_IT_RXNE, ENABLE); //开启相关中断

    // USART6 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel                   = USART6_IRQn; //串口3中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;           //抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;           //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;      // IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);                                     //根据指定的参数初始化VIC寄存器
}

void Ps_On_Received(uint8_t newByte) {
    // USART6->DR = counter;
    Ps_Append(newByte);
    if (psDebugEnabled) printf("[%d:%d]", counter, newByte);
    if (status == WAITING && newByte == SOF) {
        Ps_On_Start();
    } else if (status == STARTED && counter == 2) {
        Ps_Parse_Header(newByte);
    } else if (counter == 2 * len + 4 && newByte == 88) {
        Ps_Valid();
    }
}

void Ps_Append(uint8_t value) {
    data[counter] = value;
    counter++;
}

void Ps_Parse_Header(uint8_t value) {
    len = value >> 4;
    id  = value & 0x0f;
    if (psDebugEnabled) printf("len:%d\r\n", len);
}

void Ps_Valid(void) {
    uint8_t crc = data[2 * len + 2];
    if (CRC_Valid(data + 1, 2 * len + 1, crc)) {
        if (psDebugEnabled) printf("done%d\r\n", len);
        Ps_On_Done();
    } else {
        Ps_On_Interrupted();
    }
}

void Ps_Reset(void) {
    int i;
    for (i = 0; i < 34; i++) {
        data[i] = 0;
    }
    counter = 0;
    status  = WAITING;
    if (psDebugEnabled) printf("reset\r\n");
}

void Ps_On_Start(void) {
    if (psDebugEnabled) printf("start\r\n");
    status = STARTED;
}

void Ps_On_Done(void) {
    // todo:解析data
    status = VALIDED;
    Ps_DataAnalysis();
    Ps_Reset();
}

void Ps_On_Interrupted(void) {
    if (psDebugEnabled) printf("interrupted\r\n");
    status = INTERRUPTED;
    Ps_Reset();
}

static const unsigned char crc_table[] = {
    0x00, 0x31, 0x62, 0x53, 0xc4, 0xf5, 0xa6, 0x97, 0xb9, 0x88, 0xdb, 0xea, 0x7d, 0x4c, 0x1f, 0x2e, 0x43, 0x72, 0x21, 0x10, 0x87, 0xb6, 0xe5, 0xd4, 0xfa, 0xcb,
    0x98, 0xa9, 0x3e, 0x0f, 0x5c, 0x6d, 0x86, 0xb7, 0xe4, 0xd5, 0x42, 0x73, 0x20, 0x11, 0x3f, 0x0e, 0x5d, 0x6c, 0xfb, 0xca, 0x99, 0xa8, 0xc5, 0xf4, 0xa7, 0x96,
    0x01, 0x30, 0x63, 0x52, 0x7c, 0x4d, 0x1e, 0x2f, 0xb8, 0x89, 0xda, 0xeb, 0x3d, 0x0c, 0x5f, 0x6e, 0xf9, 0xc8, 0x9b, 0xaa, 0x84, 0xb5, 0xe6, 0xd7, 0x40, 0x71,
    0x22, 0x13, 0x7e, 0x4f, 0x1c, 0x2d, 0xba, 0x8b, 0xd8, 0xe9, 0xc7, 0xf6, 0xa5, 0x94, 0x03, 0x32, 0x61, 0x50, 0xbb, 0x8a, 0xd9, 0xe8, 0x7f, 0x4e, 0x1d, 0x2c,
    0x02, 0x33, 0x60, 0x51, 0xc6, 0xf7, 0xa4, 0x95, 0xf8, 0xc9, 0x9a, 0xab, 0x3c, 0x0d, 0x5e, 0x6f, 0x41, 0x70, 0x23, 0x12, 0x85, 0xb4, 0xe7, 0xd6, 0x7a, 0x4b,
    0x18, 0x29, 0xbe, 0x8f, 0xdc, 0xed, 0xc3, 0xf2, 0xa1, 0x90, 0x07, 0x36, 0x65, 0x54, 0x39, 0x08, 0x5b, 0x6a, 0xfd, 0xcc, 0x9f, 0xae, 0x80, 0xb1, 0xe2, 0xd3,
    0x44, 0x75, 0x26, 0x17, 0xfc, 0xcd, 0x9e, 0xaf, 0x38, 0x09, 0x5a, 0x6b, 0x45, 0x74, 0x27, 0x16, 0x81, 0xb0, 0xe3, 0xd2, 0xbf, 0x8e, 0xdd, 0xec, 0x7b, 0x4a,
    0x19, 0x28, 0x06, 0x37, 0x64, 0x55, 0xc2, 0xf3, 0xa0, 0x91, 0x47, 0x76, 0x25, 0x14, 0x83, 0xb2, 0xe1, 0xd0, 0xfe, 0xcf, 0x9c, 0xad, 0x3a, 0x0b, 0x58, 0x69,
    0x04, 0x35, 0x66, 0x57, 0xc0, 0xf1, 0xa2, 0x93, 0xbd, 0x8c, 0xdf, 0xee, 0x79, 0x48, 0x1b, 0x2a, 0xc1, 0xf0, 0xa3, 0x92, 0x05, 0x34, 0x67, 0x56, 0x78, 0x49,
    0x1a, 0x2b, 0xbc, 0x8d, 0xde, 0xef, 0x82, 0xb3, 0xe0, 0xd1, 0x46, 0x77, 0x24, 0x15, 0x3b, 0x0a, 0x59, 0x68, 0xff, 0xce, 0x9d, 0xac};

uint8_t CRC_Calculate(uint8_t *data, uint8_t len) {
    uint8_t crc = 0x00;
    uint8_t i;

    for (i = 0; i < len; i++) {
        if (psDebugEnabled) printf("crc_add: %x\r\n", *(data + i));
        crc = crc_table[crc ^ *(data + i)];
    }
    // DECO
    // crcCopy = crc;
    if (psDebugEnabled) printf("crc: %x\r\n", crc);
    return crc;
}

uint8_t CRC_Valid(uint8_t *data, uint8_t len, uint8_t crc) {
    return 1;
    // return CRC_Calculate(data, len) == crc;
}

void Ps_DataAnalysis(void) {
    int i = 0;
    if (psDebugEnabled) printf("rec: %d %d\r\n", data[2], data[3]);

    for (i = 0; i < len; i++) {
        PsData[i] = (data[2 * i + 2] << 8) | (data[2 * i + 3]);
    }
    Ps_DataUnused();
}

void Ps_DataUnused(void) {
    PsData[16] = 0; // 数据未被使用过
}

void Ps_DataUsed(void) {
    PsData[16] = 1; // 数据被使用过
}