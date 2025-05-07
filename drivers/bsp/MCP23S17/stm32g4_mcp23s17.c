/**
 *******************************************************************************
 * @file	stm32g4_mcp23s17.c
 * @author	vchav
 * @date	May 15, 2024
 * @brief	Module pour utiliser le GPIO Expander MCP23S17
 *******************************************************************************
 */

/*
 * Certains commentaires dans ce fichier font r�f�rence �la datasheet du composant.
 * Voici le lien vers la datasheet correspondante: https://docs.rs-online.com/938e/0900766b8171fc73.pdf
 *
 * Le MCP23S17 se compose de plusieurs registres 8 bits sur lesquels on peut:
 * - configurer les I/O en tant qu'entr�es ou sorties,
 * - param�trer des interruptions,
 * - lire l'�tat des GPIO,
 * - D�finir l'�tat des GPIO,
 * - ...
 *
 */
#include "config.h"
#if USE_MCP23S17
#include "stm32g4_mcp23s17.h"
#include "stm32g4_utils.h"
#include "stm32g4_spi.h"
#include "stm32g4_gpio.h"
#include "stm32g4_sys.h"
#include <stdio.h>


#define READ_OPCODE  0b01000001 // voir datasheet page 15
#define WRITE_OPCODE 0b01000000


static void MCP23S17_CSPinSet(int PinValue);
static uint8_t MCP23S17_readSPI(uint8_t address);
static void MCP23S17_writeSPI(uint8_t address, uint8_t data);

/**
 * @brief Fonction de d�mo pour prendre en main le module
 * @param direction: MCP23S17_DIRECTION_OUTPUT si vous voulez tester les GPIO en output ; MCP23S17_DIRECTION_INPUT pour l'inverse
 */
void BSP_MCP23S17_demo(MCP23S17_direction_e direction)
{
	MCP23S17_pin_e pin;
	BSP_MCP23S17_init();

	// On configure tous les GPIO en input ou en output
	for (MCP23S17_port_e port=0; port<=MCP23S17_PORT_B; port++)
	{
		for(uint8_t j=0; j<=7; j++)
		{
			pin = 1<<j;
			BSP_MCP23S17_setGPIODirection(port, pin, direction);
		}
	}
	while(1)
	{
		if(direction)
		{	// D�mo pour tester les GPIO en input
			uint8_t portAValue = BSP_MCP23S17_readGPIO(MCP23S17_PORT_A);
			printf("Etat des broches du port A : 0b");
			for (int8_t i = 7; i >= 0; i--) {
				printf("%d", (portAValue >> i) & 1);
			}
			printf("\n");
			uint8_t portBValue = BSP_MCP23S17_readGPIO(MCP23S17_PORT_B);
			printf("Etat des broches du port B : 0b");
			for (int8_t i = 7; i >= 0; i--) {
				printf("%d", (portBValue >> i) & 1);
			}
			printf("\n");
			HAL_Delay(1000); // Ce d�lai bourrin n'est pas recommand� dans vos codes. Il est l� pour la d�mo !
		}
		else
		{	// D�mo pour tester les GPIO en output
			bool toggle = false;
			MCP23S17_pinState_e state;
			// Cette d�mo n'a pas encore �t� test�e

			toggle = !toggle;
			for (MCP23S17_port_e port=0; port<=MCP23S17_PORT_B; port++)
			{
				for(uint8_t j=0; j<=7; j++)
				{
					pin = 1<<j;
					state = toggle?MCP23S17_PIN_STATE_HIGH:MCP23S17_PIN_STATE_LOW;
					BSP_MCP23S17_writeGPIO(port, pin, state);
				}
				HAL_Delay(100);
			}
		}
	}
}

/**
 * @brief Permet de modifier la valeur en sortie du CS
 * @param PinValue: Valeur � attribuer un CS
 */
static void MCP23S17_CSPinSet(int PinValue){
   HAL_GPIO_WritePin(MCP23S17_CS_PORT, MCP23S17_CS_PIN, PinValue);
}


/**
 * @brief Fonction pour extraire/lire des donn�es du MCP23S17
 * @param address: l'adresse du registre que l'on souhaite lire
 * @return l'octet lu
 */
static uint8_t MCP23S17_readSPI(uint8_t address)
{
	uint8_t result = 0;

    // On ouvre la communication avec CS=0
    MCP23S17_CSPinSet(0);
    // On envoie la commande pour lire
    BSP_SPI_WriteNoRegister(MCP23S17_SPI, READ_OPCODE);
    // On envoie l'adresse du registre dans lequel on souhaite lire
    BSP_SPI_WriteNoRegister(MCP23S17_SPI, address);
    // On �coute ce que nous dit le MCP23S17
    result = BSP_SPI_ReadNoRegister(MCP23S17_SPI);
    // On ferme la communication avec CS=1
    MCP23S17_CSPinSet(1);

    return result;
}

/**
 * @brief Fonction pour envoyer des donn�es via le bus SPI.
 * @param address: adresse du registre où envoyer la donn�e
 * @param data: la donn�e � envoyer
 */
static void MCP23S17_writeSPI(uint8_t address, uint8_t data)
{
    // On ouvre la communication avec CS=0
	MCP23S17_CSPinSet(0);
	// On envoie la commande pour �crire
	BSP_SPI_WriteNoRegister(MCP23S17_SPI, WRITE_OPCODE);
	// On envoie l'adresse du registre dans lequel on souhaite �crire
	BSP_SPI_WriteNoRegister(MCP23S17_SPI, address);
	// On envoie la donn�e
	BSP_SPI_WriteNoRegister(MCP23S17_SPI, data);
    // On ferme la communication avec CS=1
	MCP23S17_CSPinSet(1);
}


/**
 * @brief Fonction pour initialiser le GPIO Expander MCP23S17 ainsi que le bus SPI et le Chip Select (CS).
 */
void BSP_MCP23S17_init(void)
{
	// Init le port de communication SPI et le GPIO CS
	BSP_GPIO_pin_config(MCP23S17_CS_PORT,MCP23S17_CS_PIN,GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_MEDIUM,GPIO_NO_AF);
	BSP_SPI_Init(MCP23S17_SPI, FULL_DUPLEX, MASTER, SPI_BAUDRATEPRESCALER_128);

	MCP23S17_writeSPI(MCP23S17_IOCON, (IOCON_SEQOP_BIT));
}



/**
 * @brief Fonction qui permet de d�finir la direction d'un pin (pour plus d'infos: Datasheet page 18)
 * @param port: le port du pin (MCP23S17_PORT_A ou MCP23S17_PORT_B)
 * @param pin: le pin � traiter
 * @param direction: la direction que l'on souhaite lui donner. Soit MCP23S17_DIRECTION_OUTPUT soit MCP23S17_DIRECTION_INPUT
 */
void BSP_MCP23S17_setGPIODirection(MCP23S17_port_e port, MCP23S17_pin_e pin, MCP23S17_direction_e direction)
{
	// On r�cup�re l'octet contenant l'�tat des pins du port souhait�
	uint8_t value = MCP23S17_readSPI(MCP23S17_IODIR + (char) port);

	// On modifie l'�tat du pins souhait� dans l'octet contenant l'�tat des pins
	if(direction == MCP23S17_DIRECTION_OUTPUT){
		value = value & (~pin);
	}else{
		value = value | pin;
	}
	// On envoie l'octet contenant le nouvel �tat des pins
	MCP23S17_writeSPI(MCP23S17_IODIR + port, value);
}


/**
 * @brief 	Fonction pour contr�ler la r�sistance pull-up d'un pin.
 * 			Si un bit est d�fini � 1 et que la broche correspondante est configur�e en entr�e,
 * 			la broche du port correspondante est tir�e vers le haut en interne avec une r�sistance de 100 kOhm.
 * 			Plus d'infos page 22 de la datasheet.
 * @param port: le port du pin (voir MCP23S17_port_e)
 * @param pin: le pin � traiter (voir MCP23S17_pin_e)
 * @param state: MCP23S17_PULL_UP_STATE_HIGH pour d�finir le bit � 1, MCP23S17_PULL_UP_STATE_LOW pour le d�finir � 0
 */
void BSP_MCP23S17_setPullUp_onPin(MCP23S17_port_e port, MCP23S17_pin_e pin, MCP23S17_pullUpState_e state)
{
	// On r�cup�re l'octet contenant l'�tat des r�sistances du port souhait�
	uint8_t value = MCP23S17_readSPI(MCP23S17_GPPU + (char) port);

	// On modifie l'�tat de la r�sistance souhait� dans l'octet contenant l'�tat des r�sistances
	if(state == MCP23S17_PULL_UP_STATE_LOW){
		value = value & (~pin); // met � 0
	}else{
		value = value | pin; // met � 1
	}
	// On envoie l'octet contenant le nouvel �tat des r�sistances
	MCP23S17_writeSPI(MCP23S17_GPPU + port, value);
}

/**
 * @brief 	Fonction pour contr�ler les r�sistances pull-up des pins d'un port.
 * 			Si un bit est d�fini � 1 et que la broche correspondante est configur�e en entr�e,
 * 			la broche du port correspondante est tir�e vers le haut en interne avec une r�sistance de 100 kOhm.
 * 			Plus d'infos page 22 de la datasheet.
 * @param port: le port � traiter (voir MCP23S17_port_e)
 * @param state: MCP23S17_PULL_UP_STATE_HIGH pour d�finir le bit � 1, MCP23S17_PULL_UP_STATE_LOW pour le d�finir � 0
 */
void BSP_MCP23S17_setPullUp_onPort(MCP23S17_port_e port, MCP23S17_pullUpState_e state)
{
	uint8_t value = (state == MCP23S17_PULL_UP_STATE_LOW)?0x00000000:0x11111111; // Je mets volontairement 0x00000000 et pas 0 pour aider � mieux comprendre ce qu'on envoie

	// On envoie l'octet contenant le nouvel �tat des r�sistances
	MCP23S17_writeSPI(MCP23S17_GPPU + port, value);
}


/**
 * @brief 	Fonction pour activer ou d�sactiver l'interruption MCP23S17_IT_ON_CHANGE sur un pin.
 * 			Plus d'infos page 19 de la datasheet.
 * @param port: le port du pin
 * @param pin: le pin � traiter
 * @param state: MCP23S17_IT_ENABLE pour activer l'it, MCP23S17_IT_DISABLE pour la d�sactiver
 */
void BSP_MCP23S17_setITState_onPin(MCP23S17_port_e port, MCP23S17_pin_e pin, MCP23S17_itState_e state)
{
	// On r�cup�re l'octet contenant l'�tat des it du port souhait�
	uint8_t value = MCP23S17_readSPI(MCP23S17_GPINTEN + port);

	// On modifie l'�tat de l'it souhait� dans l'octet contenant l'�tat des it
	if(state == MCP23S17_IT_DISABLE)
		value = value & (~pin);
	else
		value = value | pin;

	// On envoie l'octet contenant le nouvel �tat des it
	MCP23S17_writeSPI(MCP23S17_GPINTEN +  port, value);
}

/**
 * @brief 	Fonction pour activer ou d�sactiver les interruptions MCP23S17_IT_ON_CHANGE sur tous les pins d'un port.
 * 			Plus d'infos page 19 de la datasheet.
 * @param port: le port � traiter
 * @param state: MCP23S17_IT_ENABLE pour activer les it, MCP23S17_IT_DISABLE pour les d�sactiver
 */
void BSP_MCP23S17_setITState_onPort(MCP23S17_port_e port, MCP23S17_itState_e state)
{
	uint8_t value = (state == MCP23S17_IT_DISABLE)?0x00000000:0x11111111; // Je mets volontairement 0x00000000 et pas 0 pour aider � mieux comprendre ce qu'on envoie

	// On envoie l'octet contenant le nouvel �tat des it
	MCP23S17_writeSPI(MCP23S17_GPINTEN +  port, value);
}


/**
 * @brief Permet de d�finir si les interruptions des ports A et B sont en miroir ou pas.
 *        Si le miroir est activ�, les broches INTA et INTB sont "reli�es" de sorte qu’une interruption sur l’un ou l’autre port provoquera l’activation des deux broches.
 *        Si le miroir est d�sactiv�, les broches INT sont "s�par�es". Les conditions d’interruption sur un port entraîneront l’activation de sa broche INT respective.
 *        Pour plus d'informations, voir la page 20 de la datasheet.
 * @param mirror : true pour activer le miroir, false pour le d�sactiver
 */
void BSP_MCP23S17_setMirrorIT(bool mirror)
{
	// On r�cup�re le registre IOCON
	uint8_t iocon = MCP23S17_readSPI(MCP23S17_IOCON);

	// On modifie le bit qui d�finit si INTA et INTB sont li�s
    if (mirror)
        iocon = iocon | IOCON_MIRROR_BIT;
    else
        iocon = iocon & ~IOCON_MIRROR_BIT;

    // On envoie le registre modifi�
    MCP23S17_writeSPI(MCP23S17_IOCON, iocon);

}


/**
 * @brief Fonction qui permet de d�finir la polarit� des interruptions.
 *        La polarit� d’interruption (INTPOL) d�finit la polarit� de la broche INT.
 *        Ce bit n’est fonctionnel que lorsque le bit ODR est effac�.
 *        Ce qui est le cas ici car il n'a pas �t� initialis� � 1 dans BSP_MCP23S17_init(). Plus d'infos --> datasheet page 21
 * @param polarity : polarit� � appliquer (voir MCP23S17_polarity_e)
 */
void  BSP_MCP23S17_setITPolarity(MCP23S17_polarity_e polarity)
{
	// On r�cup�re le registre IOCON
	uint8_t iocon = MCP23S17_readSPI(MCP23S17_IOCON);

	// On modifie le bit de polarit� des IT (bit 1)
    if (polarity == ACTIVE_LOW)
        iocon = iocon & ~IOCON_POLARITY_BIT;
    else
        iocon = iocon | IOCON_POLARITY_BIT;

    // On envoie le registre modifi�
    MCP23S17_writeSPI(MCP23S17_IOCON, iocon);
}

/**
 * @brief Permet de d�finir la valeur par d�faut d'un pin.
 *        Cette valeur par d�faut va servir � d�clencher des interruptions (voir page 20 de la datasheet).
 * @param port : port du pin demand�
 * @param pin : pin pour lequel on souhaite d�finir une valeur par d�faut
 * @param state : valeur/�tat par d�faut � appliquer
 */
void BSP_MCP23S17_setDefaultValue_onPin(MCP23S17_port_e port, MCP23S17_pin_e pin, MCP23S17_pinState_e state)
{
	// On r�cup�re les �tats des valeurs par d�faut
	uint8_t value = MCP23S17_readSPI(MCP23S17_DEFVAL + port);

	// On modifie la valeur par d�faut du pin souhait�
	if (state == MCP23S17_PIN_STATE_LOW){
		value = value & (~pin); // met � 0
	}else{
		value = value | pin; // met � 1
	}

	// On envoie les donn�es modifi�s
	MCP23S17_writeSPI(MCP23S17_DEFVAL +  port, value);
}

/**
 * @brief Permet de d�finir la valeur par d�faut de tous les pins d'un port.
 *        Cette valeur par d�faut va servir � d�clencher des interruptions (voir page 20 de la datasheet).
 * @param port : port où vont être appliqu�es les valeurs par d�faut
 * @param state : valeur/�tat par d�faut � appliquer
 */
void BSP_MCP23S17_setDefaultValue_onPort(MCP23S17_port_e port, MCP23S17_pinState_e state)
{
    uint8_t value = (state == MCP23S17_PIN_STATE_LOW) ? 0x00 : 0xFF; // Je mets volontairement 0x00 et pas 0 pour aider � mieux comprendre ce qu'on envoie
    // On envoie les donn�es
    MCP23S17_writeSPI(MCP23S17_DEFVAL + port, value);
}

/**
 * @brief Fonction qui permet de d�finir le contr�le d'interruption sur un pin
 * @param port : port du pin (MCP23S17_PORT_A ou MCP23S17_PORT_B)
 * @param pin : le pin sur lequel on souhaite d�finir le contr�le (voir MCP23S17_pin_e)
 * @param type : type de contr�le que l'on souhaite d�finir (voir MCP23S17_itControlType_e)
 */
void BSP_MCP23S17_setITControl_onPin(MCP23S17_port_e port, MCP23S17_pin_e pin, MCP23S17_itControlType_e type)
{
    // On r�cup�re les �tats des contr�les d'interruption du port souhait�
    uint8_t value = MCP23S17_readSPI(MCP23S17_INTCON + port);

    // On modifie la m�thode de contr�le du pin souhait�
    if (type == MCP23S17_IT_ON_CHANGE) {
        value &= ~pin;
    } else {
        value |= pin;
    }

    // On envoie les donn�es modifi�es
    MCP23S17_writeSPI(MCP23S17_INTCON + port, value);
}

/**
 * @brief Fonction qui permet de d�finir le contr�le d'interruption sur tous les pins d'un port
 * @param port : MCP23S17_PORT_A ou MCP23S17_PORT_B
 * @param type : type de contr�le (voir MCP23S17_itControlType_e)
 */
void BSP_MCP23S17_setITControl_onPort(MCP23S17_port_e port, MCP23S17_itControlType_e type)
{
    uint8_t value = (type == MCP23S17_IT_ON_CHANGE) ? 0x00 : 0xFF;
    // On envoie les donn�es
    MCP23S17_writeSPI(MCP23S17_INTCON + port, value);
}

/**
 * @brief Fonction qui permet d'�crire un �tat haut ou bas sur un pin
 * @param port: port du pin (MCP23S17_PORT_A ou MCP23S17_PORT_B)
 * @param pin: pin � "�crire" (voir MCP23S17_pin_e)
 * @param state: �tat que l'on souhaite appliquer au pin (voir MCP23S17_pinState_e)
 */
void BSP_MCP23S17_writeGPIO(MCP23S17_port_e port, MCP23S17_pin_e pin, MCP23S17_pinState_e state)
{
	// On r�cup�re les �tats des GPIO du port
	uint8_t value = BSP_MCP23S17_readGPIO(port);

	// On modifie le pin souhait� avec l'�tat souhait�
	if (state == MCP23S17_PIN_STATE_LOW){
		value = value & (~pin);
	}else{
		value = value | pin;
	}

	// On envoie les donn�es modifi�es
	MCP23S17_writeSPI(MCP23S17_OLAT +  port, value);
}

/**
 * @brief Fonction pour r�cup�rer l'�tat des GPIO d'un port
 * @param port: MCP23S17_PORT_A ou MCP23S17_PORT_B
 * @return l'�tat les GPIO sur 8 bits (1 bit par GPIO)
 */
uint8_t BSP_MCP23S17_readGPIO(MCP23S17_port_e port)
{
    return MCP23S17_readSPI(MCP23S17_GPIO + port);
}

#endif

