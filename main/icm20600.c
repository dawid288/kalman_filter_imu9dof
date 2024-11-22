#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>
#include <stdlib.h>
#include <driver/i2c_master.h>
#include "icm20600.h"
#include "imu_9dof.h"

void icm20600_read(icm20600_dev_t* dev, uint8_t reg, uint8_t* data, size_t data_size)
{
    ESP_ERROR_CHECK(i2c_master_transmit_receive(dev->icm20600_device_handle, &reg, 1, data, data_size, -1));
}

void icm20600_write(icm20600_dev_t* dev, uint8_t reg, uint8_t data)
{
    uint8_t data_wr[2];
    data_wr[0] = reg;
    data_wr[1] = data;
    ESP_ERROR_CHECK(i2c_master_transmit(dev->icm20600_device_handle, data_wr, 2, -1));
}

uint8_t icm20600_get_id(icm20600_dev_t* dev)
{
    uint8_t id;
    icm20600_read(dev, ICM20600_WHO_AM_I_REG, &id, 1);

    return id;
}

void icm20600_init(icm20600_dev_t* dev)
{
    i2c_device_config_t dev_cfg
        = { .dev_addr_length = I2C_ADDR_BIT_LEN_7, .device_address = ICM_20600_I2C_ADDR, .scl_speed_hz = 100000 };

    i2c_master_dev_handle_t dev_handle;
    ESP_ERROR_CHECK(i2c_master_bus_add_device(dev->icm20600_master_handle, &dev_cfg, &dev_handle));

    dev->icm20600_device_handle = dev_handle;

    uint8_t ID = icm20600_get_id(dev);
    if (ID == I_AM_ICM20600)
        printf("found icm20600 device: 0x%x\r\n", ID);
    else
        printf("Error, icm20600 not found: 0x%x\r\n", ID);

    icm20600_write(dev, ICM20600_PWR_MGMT_1_REG, ICM20600_CLOCK_SOURCE_AUTO);

    icm20600_write(dev, ICM20600_PWR_MGMT_2_REG, ICM20600_ALL_DEVICES_ON);

    icm20600_write(dev, ICM20600_SMPLRT_DIV_REG, 0x04);

    icm20600_write(dev, ICM20600_CONFIG_REG, 0x05);

    icm20600_write(dev, ICM20600_GYRO_CONFIG_REG, FS_250_dps);
    dev->init.gyro_init.gyro_fs = FS_250_dps;

    icm20600_write(dev, ICM20600_ACCEL_CONFIG_REG, FS_2g);
    dev->init.accel_init.accel_fs = FS_2g;

    icm20600_write(dev, ICM20600_ACCEL_CONFIG2_REG, ACC_RATE_1K_BW_44);

    icm20600_write(dev, ICM20600_FIFO_EN_REG, 0x00);
}

void icm20600_read_acc_gyro(icm20600_dev_t* dev)
{
    uint8_t acc_gyro_buffer[6] = { 0 };
    int16_t raw_data[3] = { 0 };
    float data[3];
    float sensitivity = 0;

    icm20600_read(dev, ICM20600_ACCEL_XOUT_H_REG, acc_gyro_buffer, 6);

    switch (dev->init.accel_init.accel_fs)
    {
        case FS_2g:
            sensitivity = ICMP20600_ACC_SENSITIVITY_2g;
            break;

        case FS_4g:
            sensitivity = ICMP20600_ACC_SENSITIVITY_4g;
            break;

        case FS_8g:
            sensitivity = ICMP20600_ACC_SENSITIVITY_8g;
            break;

        case FS_16g:
            sensitivity = ICMP20600_ACC_SENSITIVITY_16g;
            break;

        default:
            break;
    }

    for (uint8_t i = 0; i < 3; i++)
    {
        raw_data[i] = (int16_t) ((acc_gyro_buffer[2 * i] << 8) | acc_gyro_buffer[2 * i + 1]);
        data[i] = raw_data[i] * sensitivity;
    }

    dev->axes_accel.accel_x = data[0];
    dev->axes_accel.accel_y = data[1];
    dev->axes_accel.accel_z = data[2];

    icm20600_read(dev, ICM20600_GYRO_XOUT_H_REG, acc_gyro_buffer, 6);

    switch (dev->init.gyro_init.gyro_fs)
    {
        case FS_250_dps:
            sensitivity = ICMP20600_GYRO_SENSITIVITY_250_dps;
            break;

        case FS_500_dps:
            sensitivity = ICMP20600_GYRO_SENSITIVITY_500_dps;
            break;

        case FS_1000_dps:
            sensitivity = ICMP20600_GYRO_SENSITIVITY_1000_dps;
            break;

        case FS_2000_dps:
            sensitivity = ICMP20600_GYRO_SENSITIVITY_2000_dps;
            break;

        default:
            break;
    }

    for (uint8_t i = 0; i < 3; i++)
    {
        raw_data[i] = (int16_t) ((acc_gyro_buffer[2 * i] << 8) | acc_gyro_buffer[2 * i + 1]);
        data[i] = raw_data[i] * sensitivity;
    }

    dev->axes_gyro.gyro_x = data[0];
    dev->axes_gyro.gyro_y = data[1];
    dev->axes_gyro.gyro_z = data[2];
}
