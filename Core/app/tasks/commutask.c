#include <stdio.h>
#include <string.h>

#include "cmsis_os2.h"
#include "main.h"
#include "FreeRTOS.h"
#include "usart.h"
#include "types/sensortype.h"
#include "drivers/can.h"

void startcommutask(void *argument) {
    sensordata_t *data ;
    char buf[64];
    for (;;) {
        osMessageQueueGet(commuqueueHandle,&data,NULL,osWaitForever);
        sprintf(buf,"comm:t:%.1f\r\n",data->temperature);
        //HAL_UART_Transmit(&huart1,(uint8_t*)buf,strlen(buf),100);


        uint16_t temp_int = (uint16_t)(data->temperature * 10);
        uint16_t volt_int = (uint16_t)(data->voltage * 100);
        uint8_t can_data[8];
        // 把温度、电压、电流打包成两个字节一组（放大10倍/100倍，整数传输）
        can_data[0] = (uint8_t)(temp_int & 0xFF);        // 温度低字节
        can_data[1] = (uint8_t)((temp_int >> 8) & 0xFF); // 温度高字节
        can_data[2] = (uint8_t)(volt_int & 0xFF);       // 电压低字节
        can_data[3] = (uint8_t)((volt_int >> 8) & 0xFF);// 电压高字节


        // 发出去
        can_send(0x123, can_data, 4);
        char test_buf[] = "send can test\r\n";
        // 调试：用uart1直接观察采集的数据
        // HAL_UART_Transmit(&huart1, (uint8_t*)test_buf, strlen(test_buf), 100);
        vPortFree(data);
    }

}
