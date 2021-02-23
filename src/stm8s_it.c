/**
  ******************************************************************************
  * @file stm8s_it.c
  * @author STMicroelectronics - MCD Application Team
  * @version V2.0.0
  * @date 15-March-2011
  * @brief Main Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm8s_it.h"
#include "system.h"
#include "driver.h"

#define SPI_TX_BUF_SZ SPI_RX_BUF_SZ

uint8_t	SPI_rxbuf[SPI_RX_BUF_SZ + 8]; // let this be padded up so rx ISR doesn't have to check each at each byte rx

//uint8_t SPI_txbuf[SPI_TX_BUF_SZ];
uint8_t SPI_rxcnt;
uint8_t SPI_txcnt;

/** @addtogroup STM8S_IT STM8S ISR
  * @brief STM8S ISR Template
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* Public functions ----------------------------------------------------------*/
#ifdef _COSMIC_
/**
  * @brief  Dummy interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(NonHandledInterrupt, 25)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif /*_COSMIC_*/

/**
  * @brief  TRAP interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER_TRAP(TRAP_IRQHandler)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
/**
  * @brief  Top Level Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TLI_IRQHandler, 0)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief  Auto Wake Up Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(AWU_IRQHandler, 1)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief  Clock Controller Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(CLK_IRQHandler, 2)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief  External Interrupt PORTA Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTA_IRQHandler, 3)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief  External Interrupt PORTB Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTB_IRQHandler, 4)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief  External Interrupt PORTC Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTC_IRQHandler, 5)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief  External Interrupt PORTD Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTD_IRQHandler, 6)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief  External Interrupt PORTE Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTE_IRQHandler, 7)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#ifdef STM8S903
/**
  * @brief  External Interrupt PORTF Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(EXTI_PORTF_IRQHandler, 8)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif /*STM8S903*/

#ifdef STM8S208
/**
  * @brief  CAN RX Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(CAN_RX_IRQHandler, 8)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief  CAN TX Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(CAN_TX_IRQHandler, 9)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif /*STM8S208 || STM8AF52Ax */

/**
  * @brief  SPI Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(SPI_IRQHandler, 10)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
    uint8_t spi_tx;

    if (SPI->SR & SPI_SR_RXNE)
    {
        if ( ++SPI_rxcnt >=  SPI_RX_BUF_SZ )
        {
            SPI_rxcnt = 0;
        }

        // Clearing the RXNE bit is performed by reading the SPI_DR register
        SPI_rxbuf[ SPI_rxcnt ] = 
          spi_tx = SPI->DR;

// this is for some acceleromter in a SPI tutorial ... 	address |= 0x80  + address |= 0x40 -> multibyte read
        if ( spi_tx == 0xF2  /* ( 0x80 & bdata) && (0x40 & bdata) */ ) // data read request
        {
            SPI->DR = 0x80; // returns first byte is junk ,start of new sequence

            // expectes to read 6 bytes
            while (! (SPI->SR & SPI_SR_TXE) ) ;
            SPI_rxbuf[ ++SPI_rxcnt ] = 
              spi_tx = SPI->DR;
            SPI->DR = (spi_tx & 0x1F);

            while (! (SPI->SR & SPI_SR_TXE) ) ;
            SPI_rxbuf[ ++SPI_rxcnt ] = 
              spi_tx = SPI->DR;
            SPI->DR = (spi_tx & 0x1F);

            while (! (SPI->SR & SPI_SR_TXE) ) ;
            SPI_rxbuf[ ++SPI_rxcnt ] = 
              spi_tx = SPI->DR;
            SPI->DR = (spi_tx & 0x1F);

            while (! (SPI->SR & SPI_SR_TXE) ) ;
            SPI_rxbuf[ ++SPI_rxcnt ] = 
              spi_tx = SPI->DR;
            SPI->DR = (spi_tx & 0x1F);

            while (! (SPI->SR & SPI_SR_TXE) ) ;
            SPI_rxbuf[ ++SPI_rxcnt ] = 
              spi_tx = SPI->DR;
            SPI->DR = (spi_tx & 0x1F);

            while (! (SPI->SR & SPI_SR_TXE) ) ;
            SPI_rxbuf[ ++SPI_rxcnt ] = 
              spi_tx = SPI->DR;
            SPI->DR = (spi_tx & 0x1F);
        }
        else
        {
            SPI->DR = spi_tx;
        }
    }
}

/**
  * @brief  Timer1 Update/Overflow/Trigger/Break Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_BRK_IRQHandler, 11)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */

    static int toggle;
    toggle ^= 1;
#if 1
    if ( toggle )
    {
        GPIOD->ODR &=  ~(1<<LED); // clear test pin
    }
    else
    {
        GPIOD->ODR |=  (1<<LED); // set test pin
    }
#endif
    TIM1_ClearITPendingBit(TIM1_IT_UPDATE);
    TIM1_ClearFlag(TIM1_FLAG_UPDATE);
}

/**
  * @brief  Timer1 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_CAP_COM_IRQHandler, 12)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */

    if ( 0 != TIM1_GetFlagStatus(TIM1_FLAG_CC3) )
    {
//        GPIOD->ODR &=  ~(1<<LED); // clear test pin
        Driver_on_capture_fall();

        TIM1_ClearITPendingBit(TIM1_IT_CC3);
        TIM1_ClearFlag(TIM1_FLAG_CC3);
    }
    else
    {
//        GPIOD->ODR |=  (1<<LED); // set test pin
        Driver_on_capture_rise();
    }

       // no idea why this flag not getting set .. oh well
//    if ( 0 != TIM1_GetFlagStatus(TIM1_FLAG_CC4) )
    {
        TIM1_ClearITPendingBit(TIM1_IT_CC4);
        TIM1_ClearFlag(TIM1_FLAG_CC4);
    }
}

#ifdef STM8S903
/**
  * @brief  Timer5 Update/Overflow/Break/Trigger Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM5_UPD_OVF_BRK_TRG_IRQHandler, 13)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
/**
  * @brief  Timer5 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM5_CAP_COM_IRQHandler, 14)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

#else /*STM8S208, STM8S207, STM8S105 or STM8S103 or STM8AF62Ax or STM8AF52Ax or STM8AF626x */
/**
  * @brief  Timer2 Update/Overflow/Break Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM2_UPD_OVF_BRK_IRQHandler, 13)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
#if 0 // tmp test
    static uint8_t toggle;
    if (toggle ^= 1){
        GPIOD->ODR |= (1 << LED);
    } else {
        GPIOD->ODR &= ~(1 << LED);
    }
#endif

//GPIOD->ODR |= (1 << LED);

    Driver_on_PWM_edge();

//GPIOD->ODR &= ~(1 << LED);

    // must reset the tmer interrupt flag
    TIM2->SR1 &= ~TIM2_SR1_UIF;
}

/**
  * @brief  Timer2 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM2_CAP_COM_IRQHandler, 14)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif /*STM8S903*/

#if defined (STM8S208) || defined(STM8S207) || defined(STM8S105)
/**
  * @brief  Timer3 Update/Overflow/Break Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM3_UPD_OVF_BRK_IRQHandler, 15)
{
#if 0 // tmp test
    static uint8_t toggle;
    if (toggle ^= 1){
        GPIOG->ODR |=  (1<<0); // set test pin
    } else {
        GPIOG->ODR &=  ~(1<<0); // clear test pin
    }
#endif
//    GPIOG->ODR |=  (1<<0); // set test pin

    Driver_Step();

//    GPIOG->ODR &=  ~(1<<0); // clear test pin

    // must reset the tmer interrupt flag
    TIM3->SR1 &= ~TIM3_SR1_UIF;
}

/**
  * @brief  Timer3 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM3_CAP_COM_IRQHandler, 16)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif /*STM8S208, STM8S207 or STM8S105 or STM8AF62Ax or STM8AF52Ax or STM8AF626x */

#if defined (STM8S208) || defined(STM8S207) || defined(STM8S103) || defined (STM8AF62Ax) ||\
    defined (STM8AF52Ax) || defined (STM8S903)
/**
  * @brief  UART1 TX Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART1_TX_IRQHandler, 17)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief  UART1 RX Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif /*STM8S105*/

/**
  * @brief  I2C Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(I2C_IRQHandler, 19)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

#if defined (STM8S105) || defined (STM8AF626x)
/**
  * @brief  UART2 TX interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART2_TX_IRQHandler, 20)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  }

/**
  * @brief  UART2 RX interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART2_RX_IRQHandler, 21)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  }
#endif /* STM8S105*/

#if defined(STM8S207) || defined(STM8S208) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
/**
  * @brief  UART3 TX interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART3_TX_IRQHandler, 20)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  }

/**
  * @brief  UART3 RX interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART3_RX_IRQHandler, 21)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  }
#endif /*STM8S208 or STM8S207 or STM8AF52Ax or STM8AF62Ax */

#if defined(STM8S207) || defined(STM8S208) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
/**
  * @brief  ADC2 interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(ADC2_IRQHandler, 22)
{

    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
#else /*STM8S105, STM8S103 or STM8S903 or STM8AF626x */
/**
  * @brief  ADC1 interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(ADC1_IRQHandler, 22)
{
#if 0 // tmp test
    static uint8_t toggle;
    if (toggle ^= 1){
        GPIOC->ODR |= (1 << 4);
    } else {
        GPIOC->ODR &= ~(1 << 4);
    }
#endif

GPIOC->ODR |= (1 << 4);

    Driver_on_ADC_conv();

GPIOC->ODR &= ~(1 << 4);

    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
    ADC1_ClearFlag(ADC1_FLAG_EOC);
}
#endif /*STM8S208 or STM8S207 or STM8AF52Ax or STM8AF62Ax */

#ifdef STM8S903
/**
  * @brief  Timer6 Update/Overflow/Trigger Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM6_UPD_OVF_TRG_IRQHandler, 23)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#else /*STM8S208, STM8S207, STM8S105 or STM8S103 or STM8AF62Ax or STM8AF52Ax or STM8AF626x */
/**
  * @brief  Timer4 Update/Overflow Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM4_UPD_OVF_IRQHandler, 23)
{
#if 0 // tmp test
    static uint8_t toggle;
    if (toggle ^= 1){
        GPIOD->ODR |= (1 << LED);
    } else {
        GPIOD->ODR &= ~(1 << LED);
    }
#endif

//   GPIOD->ODR |=  (1<<LED); // set test pin

    Driver_Update();

//   GPIOD->ODR &=  ~(1<<LED); // clear test pin

// must reset the tmer interrupt flag
    TIM4->SR1 &= ~TIM4_SR1_UIF;
}
#endif /*STM8S903*/

/**
  * @brief  Eeprom EEC Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EEPROM_EEC_IRQHandler, 24)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
