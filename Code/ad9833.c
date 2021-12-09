/*
 * ad9833.c
 *
 *  Created on: Aug 22, 2021
 *      Author: illam
 */

#include <main.h>

#define FCLK 25000000	//AD9833's Clock Frequency (25 MHz)
#define POW 268440000	// 2^28
#define VAL 10.73741824	// POW / FCLK

SPI_HandleTypeDef hspi3;

uint8_t sine[2] = { 0x20, 0x00 }, square[2] = { 0x20, 0x28 }, triang[2] = {
		0x20, 0x02 };
uint8_t init[2] = { 0x21, 0x00 }, reset[2] = { 0x20, 0x00 };

/*
 * Init AD9833
 */

void initAD9833() {
	sendData(hspi3, GPIOA, GPIO_PIN_4, init);
	setFreq(400);
	setPhase(0);
	sendData(hspi3, GPIOA, GPIO_PIN_4, reset);
}

/*
 * Sends 2 bytes of data
 */

void sendData(SPI_HandleTypeDef hspi, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin,
		uint8_t *data) {

	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, RESET);
	HAL_SPI_Transmit(&hspi, data, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, SET);
	asm("NOP");
}

/*
 * Adjust Signal Frequency
 */

void setFreq(float frequency) {
	uint32_t freq = round(frequency * VAL);
	uint16_t LSByte, MSByte;
	uint8_t lsb[2], msb[2];
	MSByte = (freq & 0xFFFC000) >> 14;
	LSByte = (freq & 0x3FFF);
	MSByte |= 0x4000;
	LSByte |= 0x4000;
	lsb[0] = (LSByte & 0xFF00) >> 8;
	lsb[1] = LSByte & 0xFF;
	msb[0] = (MSByte & 0xFF00) >> 8;
	msb[1] = MSByte & 0xFF;
	sendData(hspi3, GPIOA, GPIO_PIN_4, lsb);
	sendData(hspi3, GPIOA, GPIO_PIN_4, msb);
}

/*
 * Adjust Signal Phase
 */

void setPhase(float phase) {
	uint16_t phaseInteger = (round(phase * (4096 / 360)));
	uint8_t phaseBytes[2];
	phaseInteger |= 0XC000;
	phaseBytes[0] = (phaseInteger & 0xFF00) >> 8;
	phaseBytes[1] = phaseInteger & 0xFF;
	sendData(hspi3, GPIOA, GPIO_PIN_4, phaseBytes);
}

/*
 * Adjust Signal Shape
 */

void setShape(char *shape) {
	if (strncmp(shape, "SINE", 4) == 0)
		sendData(hspi3, GPIOA, GPIO_PIN_4, sine);
	else if (strncmp(shape, "TRIANG", 6) == 0)
		sendData(hspi3, GPIOA, GPIO_PIN_4, triang);
	else if (strncmp(shape, "SQUARE", 6) == 0)
		sendData(hspi3, GPIOA, GPIO_PIN_4, square);
}

