/**
  ******************************************************************************
  * @file       sdk\include\chip\txw80x\adc_voltage_type.h
  * @author     HUGE-IC Application Team
  * @version    V1.0.0
  * @date       2022-03-18
  * @brief      This file contains all the adc_voltage_type.
  * @copyright  Copyright (c) 2016-2022 HUGE-IC
  ******************************************************************************
  * @attention
  * Only used for txw80x.
  * 
  *
  *
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_VOLTAGE_TYPE_H
#define __ADC_VOLTAGE_TYPE_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup Docxygenid_ADC_voltage_type_enum
  * @{
  */

/**
  * @brief Enumeration constant for ADC voltage type.
  * @note
  *       Enum number start from 0x101.
  */
enum adc_voltage_type {
    /*! RF temperature
     */
    ADC_CHANNEL_RF_TEMPERATURE = 0x101,

};


/** @} Docxygenid_ADC_voltage_type_enum_enum*/


#ifdef __cplusplus
}
#endif
#endif

/*************************** (C) COPYRIGHT 2016-2022 HUGE-IC ***** END OF FILE *****/

