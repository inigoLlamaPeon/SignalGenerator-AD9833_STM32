/*
 * ad9833.h
 *
 *  Created on: Aug 22, 2021
 *      Author: illam
 */
#include <main.h>

#ifndef INC_AD9833_H_
#define INC_AD9833_H_

void initAD9833();
void sendData(SPI_HandleTypeDef hspi, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t *data);
void setFreq(float frequency);
void setPhase(float phase);
void setShape(char *shape);
#endif /* INC_AD9833_H_ */
