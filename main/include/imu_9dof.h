#ifndef IMU_9DOF_H
#define IMU_9DOF_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ak09918.h"
#include "icm20600.h"
#include <driver/i2c_master.h>
#include <stdio.h>
#include <stdlib.h>


#define ICM_20600_I2C_PORT 0
#define ICM_20600_I2C_SDA 21
#define ICM_20600_I2C_SDL 22

#define I2C_FAST_MOOD 400000

void imu_9dof_i2c_init(icm20600_dev_t *icm_dev, ak09918_dev_t *ak_dev);
void imu_9dof_read(icm20600_dev_t *icm_dev, ak09918_dev_t *ak_dev);

#ifdef __cplusplus
}
#endif

#endif