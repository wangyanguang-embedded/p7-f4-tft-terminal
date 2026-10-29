#include "cmsis_os2.h"
#include "main.h"
#include "FreeRTOS.h"
#include "types/sensortype.h"
#define filter_size 5//滑动窗口大小

void startfiltertask(void *argument) {
    sensordata_t *data ;

    //滤波缓存
    float temp_buf[filter_size] = {0};
    float volt_buf[filter_size] = {0};
    float vib_buf[filter_size] = {0};
    float curr_buf[filter_size] = {0};
    uint8_t idx = 0;
    uint8_t count = 0;

    for (;;) {
        osMessageQueueGet(sensorqueueHandle,&data,NULL,osWaitForever);

        // 存入缓存
        temp_buf[idx] = data->temperature;
        volt_buf[idx] = data->voltage;
        vib_buf[idx] = data->vibration;
        curr_buf[idx] = data->current;

        idx++;
        if (idx >= filter_size) idx = 0;
        if (count < filter_size) count++;

        // 算平均值
        float temp_sum = 0, volt_sum = 0, vib_sum = 0, curr_sum = 0;
        for (uint8_t i = 0; i < count; i++) {
            temp_sum += temp_buf[i];
            volt_sum += volt_buf[i];
            vib_sum += vib_buf[i];
            curr_sum += curr_buf[i];
        }

        data->temperature = temp_sum / count;
        data->voltage = volt_sum / count;
        data->vibration = vib_sum / count;
        data->current = curr_sum / count;

        //发送两个队列
        sensordata_t *data2 =pvPortMalloc(sizeof(sensordata_t));
        *data2 = *data;
        osMessageQueuePut(displayqueueHandle,&data,0,osWaitForever);
        //复制一份数据


        osMessageQueuePut(commuqueueHandle,&data2,0,0);


    }
}
