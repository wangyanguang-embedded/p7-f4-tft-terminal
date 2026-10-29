


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "adc.h"
#include "drivers/ds18b20.h"
#include "cmsis_os2.h"
#include "FreeRTOS.h"
#include "i2c.h"
#include "drivers/ina219.h"
#include "main.h"
#include "usart.h"

#include "drivers/mpu6050.h"
#include "types/sensortype.h"

void startsensortask(void *argument) {
    float voltage;
    mpu6050_init();


    for (;;) {

        int16_t ax, ay, az;
        mpu6050_readaccel(&ax, &ay, &az);

        //adc采集电压
        HAL_ADC_Start(&hadc1);
        HAL_ADC_PollForConversion(&hadc1, 100);
        uint32_t adc = HAL_ADC_GetValue(&hadc1);
        voltage = adc * 3.3f / 4095.0f;

        // 读INA219电流
        float current = ina219_read_current();
        float power = ina219_read_power();
        float v= ina219_read_bus_voltage();

        // 打印调试
        char buf[64];
        sprintf(buf, "i:%.1f p:%.1f v:%.1f\r\n", current, power,voltage);
        HAL_UART_Transmit(&huart2, (uint8_t*)buf, strlen(buf), 100);



        //动态申请内存
        sensordata_t* data = pvPortMalloc(sizeof(sensordata_t));
        data->temperature = ds18b20_read_temp();
        data->vibration = (float)abs(ax)/16384.0f;//震动用x轴加速度
        data->voltage = voltage;
        data->current = current;
        data->timestamp = osKernelGetTickCount();//纪录数据的获得时间
        osMessageQueuePut(sensorqueueHandle,&data,0,osWaitForever);
        osDelay(500);
    }
}
