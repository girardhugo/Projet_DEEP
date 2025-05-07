/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    http://stm32f4-discovery.com/2014/09/library-37-bmp180-pressure-sensor-stm32f4
 * @version v1.0
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   BMP180 pressure sensor library for STM32F4xx
 *	
@verbatim
   ----------------------------------------------------------------------
    Copyright (C) Tilen Majerle, 2015
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.
     
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
   ----------------------------------------------------------------------
@endverbatim
 */
#ifndef BMP180_H
#define BMP180_H 100

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

#include "config.h"
#if USE_BMP180
#include "stm32g4_i2c.h"
#include "stm32g4xx.h"
#include "stm32g4_i2c.h"
#include "math.h"

/* Default I2C pin */
#ifndef BMP180_I2C
#define BMP180_I2C					I2C1
//#define BMP180_I2C_PINSPACK		I2C_PinsPack_1
#endif

/* Default I2C speed */
#ifndef BMP180_I2C_SPEED
#define BMP180_I2C_SPEED			STANDARD_MODE
#endif

/* BMP180 I2C address */
#ifndef BMP180_I2C_ADDRESS
#define BMP180_I2C_ADDRESS			0xEE
#endif

/* Registers */
#define	BMP180_REGISTER_CONTROL 	0xF4
#define	BMP180_REGISTER_RESULT 		0xF6
#define BMP180_REGISTER_EEPROM		0xAA

/* Commands */
#define	BMP180_COMMAND_TEMPERATURE 	0x2E
#define	BMP180_COMMAND_PRESSURE_0 	0x34
#define	BMP180_COMMAND_PRESSURE_1 	0x74
#define	BMP180_COMMAND_PRESSURE_2 	0xB4
#define	BMP180_COMMAND_PRESSURE_3 	0xF4

/* Minimum waiting delay, in microseconds */
#define BMP180_TEMPERATURE_DELAY	4500
#define BMP180_PRESSURE_0_DELAY		4500
#define BMP180_PRESSURE_1_DELAY		7500
#define BMP180_PRESSURE_2_DELAY		13000
#define BMP180_PRESSURE_3_DELAY		25000


typedef enum {
	BMP180_Result_Ok = 0x00,            /*!< Everything OK */
	BMP180_Result_DeviceNotConnected,   /*!< Device is not connected to I2C */
	BMP180_Result_LibraryNotInitialized /*!< Library is not initialized */
} BMP180_Result_t;

/**
 * @brief  Options pour les r�glages de sur�chantillonnage
 * @note   Ces r�glages varient en nombre d'�chantillons pour un r�sultat
 *         et en temps d'�chantillonnage pour un r�sultat
 */
typedef enum {
	BMP180_Oversampling_UltraLowPower = 0x00,      /*!< 1 sample for result */
	BMP180_Oversampling_Standard = 0x01,           /*!< 2 samples for result */
	BMP180_Oversampling_HighResolution = 0x02,     /*!< 3 samples for result */
	BMP180_Oversampling_UltraHighResolution = 0x03 /*!< 4 samples for result */
} BMP180_Oversampling_t;


typedef struct {
	float Altitude;                        /*!< Calculated altitude at given read pressure */
	uint32_t Pressure;                     /*!< Pressure in pascals */
	float Temperature;                     /*!< Temperature in degrees */
	uint16_t Delay;                        /*!< Number of microseconds, that sensor needs to calculate data that you request to */
	BMP180_Oversampling_t Oversampling; /*!< Oversampling for pressure calculation */
} BMP180_t;


//Fonction de d�mo...
void BSP_BMP180_demo(void);


/**
 * @brief  Initialise le capteur de pression BMP180
 * @param  *BMP180_Data: Pointeur vers la structure @ref BMP180_t
 * @retval Membre de @ref BMP180_Result_t
 */
BMP180_Result_t BSP_BMP180_Init(BMP180_t* BMP180_Data, GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin);

/**
 * @brief  D�marre le capteur de temp�rature sur le BMP180
 * @param  *BMP180_Data: Pointeur vers la structure @ref BMP180_t
 * @retval Membre de @ref BMP180_Result_t
 */
BMP180_Result_t BSP_BMP180_StartTemperature(BMP180_t* BMP180_Data);

/**
 * @brief  Lit la temp�rature sur capteur BMP180
 * @note   La temp�rature a une r�solution de 0,1 degr� Celsius
 * @param  *BMP180_Data: Pointeur vers la structure @ref BMP180_t
 * @retval Membre de @ref BMP180_Result_t
 */
BMP180_Result_t BSP_BMP180_ReadTemperature(BMP180_t* BMP180_Data);

/**
 * @brief  D�marre la mesure de pression sur le capteur BMP180
 * @param  *BMP180_Data: Pointeur vers la structure @ref BMP180_t
 * @param  Oversampling: Option de sur�chantillonnage pour le calcul de la pression.
 *            Ce param�tre peut �tre une valeur de l'�num�ration @ref BMP180_Oversampling_t
 * @note   Le temps de calcul d�pend du sur�chantillonnage s�lectionn�
 * @retval Membre de @ref BMP180_Result_t
 */
BMP180_Result_t BSP_BMP180_StartPressure(BMP180_t* BMP180_Data, BMP180_Oversampling_t Oversampling);

/**
 * @brief  Lit la pression depuis le capteur BMP180 et la calcule
 * @param  *BMP180_Data: Pointeur vers la structure @ref BMP180_t
 * @retval Membre de @ref BMP180_Result_t
 */
BMP180_Result_t BMP180_ReadPressure(BMP180_t* BMP180_Data);

/**
 * @brief  Calcule la pression au-dessus du niveau de la mer en pascals
 * 
 * Ceci est utile si vous lisez la pression du capteur � une altitude connue, et non l'altitude fournie par le capteur.
 * L'altitude du capteur est calcul�e en supposant que la pression au niveau de la mer est de 101325 Pascals.
 * Donc, si vous connaissez votre pression et utilisez l'altitude calcul�e, vous n'obtiendrez pas la pression r�elle au-dessus du niveau de la mer.
 * 
 * @warning Vous avez besoin de la pression calcul�e par le capteur et d'une altitude connue (d'un autre capteur, des donn�es GPS, etc.),
 *          et alors vous pourrez calculer la pression au-dessus du niveau de la mer.
 * @param  pressure: Pression � l'altitude connue en unit�s de pascals
 * @param  altitude: Altitude connue en unit�s de m�tres
 * @retval Pression au-dessus du niveau de la mer en unit�s de pascals
 */
uint32_t BSP_BMP180_GetPressureAtSeaLevel(uint32_t pressure, float altitude);


/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
#endif
