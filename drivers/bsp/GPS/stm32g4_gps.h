/**
 *******************************************************************************
 * @file	stm32g4_gps.h
 * @author	Nirgal			&& Luc H�rault
 * @date	1 f�vr. 2018	&& Juin 2024 --> portage sur g431
 * @brief	Module pour utiliser le GPS
 *******************************************************************************
 */

#ifndef GPS_H_
#define GPS_H_
#include "config.h"
#if USE_GPS
#include "stm32g4_utils.h"

typedef struct
{
	uint16_t 	id;
	uint32_t 	time;		//[HHMMSS]
	uint32_t 	seconds;	//[sec since 0:00:00]
	double 		latitude_rad;		//rad
	double 		longitude_rad;		//rad
	double		latitude_deg;		//deg
	double		longitude_deg;		//deg
	int16_t		lat_degrees;	//Partie enti�re des degr�s
	double		lat_minutes;	//Minutes d'angle, avec d�cimales
	int16_t		long_degrees;	//Partie enti�re des degr�s
	double		long_minutes;	//Minutes d'angle, avec d�cimales
	bool		north;
	bool 		east;
	uint16_t	ground_speed;
	uint32_t 	date32;
	uint8_t checksum;
}gps_datas_t;

typedef enum
{
	NO_TRAME_RECEIVED = 0,	//Une trame est en cours de r�ception
	CHECKSUM_INVALID,
	TRAME_INVALID,
	TRAME_UNKNOW,			//Une trame inconnue a �t� re�ue
	TRAME_GPRMC,			//Une trame GPRMC a �t� re�ue
	TRAME_GPGGA				//Une trame GPGGA a �t� re�ue
}nmea_frame_e;

nmea_frame_e BSP_GPS_process_rx(uint8_t c, gps_datas_t * gps_datas);
void BSP_GPS_test(void);
void BSP_GPS_demo(void);

#endif //USE_GPS
#endif /* GPS_H_ */

