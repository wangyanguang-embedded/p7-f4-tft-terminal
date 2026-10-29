//
// Created by 86158 on 2026/10/11.
//

#ifndef P7_F4_FOURTASKS_SENSORTYPE_H
#define P7_F4_FOURTASKS_SENSORTYPE_H

typedef struct {
    float temperature;//温度
    float vibration;//震动
    float voltage;//电压
    float current;//电流
    uint32_t timestamp;//时间戳
}sensordata_t;



#endif //P7_F4_FOURTASKS_SENSORTYPE_H
