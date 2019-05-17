#include <iostream>

using namespace std;

#define RCC_APB1Periph_TIM2              ((uint32_t)0x00000001)
#define RCC_APB1Periph_TIM4              ((uint32_t)0x00000004)
#define RCC_APB1Periph_TIM5              ((uint32_t)0x00000008)
#define RCC_APB2Periph_TIM8              ((uint32_t)0x00000002)

#define PERIPH_BASE           ((uint32_t)0x40000000)
#define APB1PERIPH_BASE       PERIPH_BASE
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x00010000)
#define TIM2_BASE             (APB1PERIPH_BASE + 0x0000)
#define TIM4_BASE             (APB1PERIPH_BASE + 0x0800)
#define TIM5_BASE             (APB1PERIPH_BASE + 0x0C00)
#define TIM8_BASE             (APB2PERIPH_BASE + 0x0400)


#define RCC_AHB1Periph_GPIOA             ((uint32_t)0x00000001)
#define RCC_AHB1Periph_GPIOD             ((uint32_t)0x00000008)
#define RCC_AHB1Periph_GPIOH             ((uint32_t)0x00000080)
#define RCC_AHB1Periph_GPIOI             ((uint32_t)0x00000100)


#define GPIO_PinSource0            ((uint8_t)0x00)
#define GPIO_PinSource1            ((uint8_t)0x01)
#define GPIO_PinSource2            ((uint8_t)0x02)
#define GPIO_PinSource3            ((uint8_t)0x03)
#define GPIO_PinSource4            ((uint8_t)0x04)
#define GPIO_PinSource5            ((uint8_t)0x05)
#define GPIO_PinSource6            ((uint8_t)0x06)
#define GPIO_PinSource7            ((uint8_t)0x07)
#define GPIO_PinSource8            ((uint8_t)0x08)
#define GPIO_PinSource9            ((uint8_t)0x09)
#define GPIO_PinSource10           ((uint8_t)0x0A)
#define GPIO_PinSource11           ((uint8_t)0x0B)
#define GPIO_PinSource12           ((uint8_t)0x0C)
#define GPIO_PinSource13           ((uint8_t)0x0D)
#define GPIO_PinSource14           ((uint8_t)0x0E)
#define GPIO_PinSource15           ((uint8_t)0x0F)


#define GPIO_Pin_0                 ((uint16_t)0x0001)  /* Pin 0 selected */
#define GPIO_Pin_1                 ((uint16_t)0x0002)  /* Pin 1 selected */
#define GPIO_Pin_2                 ((uint16_t)0x0004)  /* Pin 2 selected */
#define GPIO_Pin_3                 ((uint16_t)0x0008)  /* Pin 3 selected */
#define GPIO_Pin_4                 ((uint16_t)0x0010)  /* Pin 4 selected */
#define GPIO_Pin_5                 ((uint16_t)0x0020)  /* Pin 5 selected */
#define GPIO_Pin_6                 ((uint16_t)0x0040)  /* Pin 6 selected */
#define GPIO_Pin_7                 ((uint16_t)0x0080)  /* Pin 7 selected */
#define GPIO_Pin_8                 ((uint16_t)0x0100)  /* Pin 8 selected */
#define GPIO_Pin_9                 ((uint16_t)0x0200)  /* Pin 9 selected */
#define GPIO_Pin_10                ((uint16_t)0x0400)  /* Pin 10 selected */
#define GPIO_Pin_11                ((uint16_t)0x0800)  /* Pin 11 selected */
#define GPIO_Pin_12                ((uint16_t)0x1000)  /* Pin 12 selected */
#define GPIO_Pin_13                ((uint16_t)0x2000)  /* Pin 13 selected */
#define GPIO_Pin_14                ((uint16_t)0x4000)  /* Pin 14 selected */
#define GPIO_Pin_15                ((uint16_t)0x8000)  /* Pin 15 selected */
#define GPIO_Pin_All               ((uint16_t)0xFFFF)  /* All pins selected */


#define GPIO_AF_TIM2          ((uint8_t)0x01)  /* TIM2 Alternate Function mapping */
#define GPIO_AF_TIM4          ((uint8_t)0x02)  /* TIM4 Alternate Function mapping */
#define GPIO_AF_TIM5          ((uint8_t)0x02)  /* TIM5 Alternate Function mapping */
#define GPIO_AF_TIM8          ((uint8_t)0x03)  /* TIM8 Alternate Function mapping */

#define PWM_PD12 0x4028008c

int main(){
    /*
     * RCC_APB1Periph_TIM4 4
     * TIM4(1) 4
     * GPIO_AF_TIMx 4
     * TIM4(2) 4
     * ADHI 12
     * 1234 4
     */
    
    printf("#define PWM_PD12 0x%08x\n",((RCC_APB1Periph_TIM4&0x0F)<<28)+(((TIM4_BASE>>8)&0x0FFF)<<16)+(GPIO_AF_TIM4<<20)+((RCC_AHB1Periph_GPIOD&0x0FFF)<<4)+(GPIO_PinSource12&0x0F));
    printf("#define PWM_PD13 0x%08x\n",((RCC_APB1Periph_TIM4&0x0F)<<28)+(((TIM4_BASE>>8)&0x0FFF)<<16)+(GPIO_AF_TIM4<<20)+((RCC_AHB1Periph_GPIOD&0x0FFF)<<4)+(GPIO_PinSource13&0x0F));
    printf("#define PWM_PD14 0x%08x\n",((RCC_APB1Periph_TIM4&0x0F)<<28)+(((TIM4_BASE>>8)&0x0FFF)<<16)+(GPIO_AF_TIM4<<20)+((RCC_AHB1Periph_GPIOD&0x0FFF)<<4)+(GPIO_PinSource14&0x0F));
    printf("#define PWM_PD15 0x%08x\n",((RCC_APB1Periph_TIM4&0x0F)<<28)+(((TIM4_BASE>>8)&0x0FFF)<<16)+(GPIO_AF_TIM4<<20)+((RCC_AHB1Periph_GPIOD&0x0FFF)<<4)+(GPIO_PinSource15&0x0F));
    printf("#define PWM_PH10 0x%08x\n",((RCC_APB1Periph_TIM5&0x0F)<<28)+(((TIM5_BASE>>8)&0x0FFF)<<16)+(GPIO_AF_TIM5<<20)+((RCC_AHB1Periph_GPIOH&0x0FFF)<<4)+(GPIO_PinSource10&0x0F));
    printf("#define PWM_PH11 0x%08x\n",((RCC_APB1Periph_TIM5&0x0F)<<28)+(((TIM5_BASE>>8)&0x0FFF)<<16)+(GPIO_AF_TIM5<<20)+((RCC_AHB1Periph_GPIOH&0x0FFF)<<4)+(GPIO_PinSource11&0x0F));
    printf("#define PWM_PH12 0x%08x\n",((RCC_APB1Periph_TIM5&0x0F)<<28)+(((TIM5_BASE>>8)&0x0FFF)<<16)+(GPIO_AF_TIM5<<20)+((RCC_AHB1Periph_GPIOH&0x0FFF)<<4)+(GPIO_PinSource12&0x0F));
    printf("#define PWM_PI0 0x%08x\n", ((RCC_APB1Periph_TIM5&0x0F)<<28)+(((TIM5_BASE>>8)&0x0FFF)<<16)+(GPIO_AF_TIM5<<20)+((RCC_AHB1Periph_GPIOI&0x0FFF)<<4)+(GPIO_PinSource0&0x0F));
    
    printf("#define PWM_PA0 0x%08x\n",((RCC_APB1Periph_TIM2&0x0F)<<28)+(((TIM2_BASE>>8)&0x0FFF)<<16)+(GPIO_AF_TIM2<<20)+((RCC_AHB1Periph_GPIOA&0x0FFF)<<4)+(GPIO_PinSource0&0x0F));
    printf("#define PWM_PA1 0x%08x\n",((RCC_APB1Periph_TIM2&0x0F)<<28)+(((TIM2_BASE>>8)&0x0FFF)<<16)+(GPIO_AF_TIM2<<20)+((RCC_AHB1Periph_GPIOA&0x0FFF)<<4)+(GPIO_PinSource1&0x0F));
    printf("#define PWM_PA2 0x%08x\n",((RCC_APB1Periph_TIM2&0x0F)<<28)+(((TIM2_BASE>>8)&0x0FFF)<<16)+(GPIO_AF_TIM2<<20)+((RCC_AHB1Periph_GPIOA&0x0FFF)<<4)+(GPIO_PinSource2&0x0F));
    printf("#define PWM_PA3 0x%08x\n",((RCC_APB1Periph_TIM2&0x0F)<<28)+(((TIM2_BASE>>8)&0x0FFF)<<16)+(GPIO_AF_TIM2<<20)+((RCC_AHB1Periph_GPIOA&0x0FFF)<<4)+(GPIO_PinSource3&0x0F));
    printf("#define PWM_PI5 0x%08x\n",((RCC_APB2Periph_TIM8&0x0F)<<28)+(((TIM8_BASE>>8)&0x0FFF)<<16)+(GPIO_AF_TIM8<<20)+((RCC_AHB1Periph_GPIOI&0x0FFF)<<4)+(GPIO_PinSource5&0x0F));
    printf("#define PWM_PI6 0x%08x\n",((RCC_APB2Periph_TIM8&0x0F)<<28)+(((TIM8_BASE>>8)&0x0FFF)<<16)+(GPIO_AF_TIM8<<20)+((RCC_AHB1Periph_GPIOI&0x0FFF)<<4)+(GPIO_PinSource6&0x0F));
    printf("#define PWM_PI7 0x%08x\n",((RCC_APB2Periph_TIM8&0x0F)<<28)+(((TIM8_BASE>>8)&0x0FFF)<<16)+(GPIO_AF_TIM8<<20)+((RCC_AHB1Periph_GPIOI&0x0FFF)<<4)+(GPIO_PinSource7&0x0F));
    printf("#define PWM_PI2 0x%08x\n",((RCC_APB2Periph_TIM8&0x0F)<<28)+(((TIM8_BASE>>8)&0x0FFF)<<16)+(GPIO_AF_TIM8<<20)+((RCC_AHB1Periph_GPIOI&0x0FFF)<<4)+(GPIO_PinSource2&0x0F));

    printf("\nrestore PWM_PD12:\n");
    uint32_t     RCC_APBxPeriph_TIMx  = PWM_PD12 >> 28;
    uint32_t     TIMx_BASE            = ((PWM_PD12 >> 16 & 0x0F0F) << 8) + PERIPH_BASE;
    uint8_t      GPIO_AF_TIMx         = PWM_PD12 >> 20 & 0xF;
    uint32_t     RCC_AHB1Periph_GPIOx = PWM_PD12 >> 4 & 0x0FFF;
    uint32_t     GPIO_PinSourcex      = PWM_PD12 & 0x0F;
    uint16_t     GPIO_Pin_x           = 1 << (PWM_PD12 & 0x0F);
    printf("RCC_APBxPeriph_TIMx:0x%08x/0x%08x\n", RCC_APBxPeriph_TIMx, RCC_APB1Periph_TIM4);
    printf("TIMx_BASE:0x%04x/0x%04x\n", TIMx_BASE, TIM4_BASE);
    printf("GPIO_AF_TIM2:0x%02x/0x%02x\n", GPIO_AF_TIMx, GPIO_AF_TIM4);
    printf("RCC_AHB1Periph_GPIOx:0x%08x/0x%08x\n", RCC_AHB1Periph_GPIOx, RCC_AHB1Periph_GPIOD);
    printf("GPIO_PinSourcex:0x%08x/0x%08x\n", GPIO_PinSourcex, GPIO_PinSource12);
    printf("GPIO_Pin_x:0x%04x/0x%04x\n", GPIO_Pin_x, GPIO_Pin_12);

     return 0;
}


