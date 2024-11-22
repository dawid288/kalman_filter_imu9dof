#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>
#include <stdlib.h>
#include <driver/i2c_master.h>
#include "ak09918.h"
#include "imu_9dof.h"

void ak09918_read(ak09918_dev_t* dev, uint8_t reg, uint8_t* data, size_t data_size)
{
    ESP_ERROR_CHECK(i2c_master_transmit_receive(dev->ak09918_device_handle, &reg, 1, data, data_size, -1));
}

void ak09918_write(ak09918_dev_t* dev, uint8_t reg, uint8_t data)
{
    uint8_t data_wr[2];
    data_wr[0] = reg;
    data_wr[1] = data;
    ESP_ERROR_CHECK(i2c_master_transmit(dev->ak09918_device_handle, data_wr, 2, -1));
}

uint8_t ak09918_get_id(ak09918_dev_t* dev)
{
    uint8_t id;
    ak09918_read(dev, AK09918_WIA2_REG, &id, 1);

    return id;
}

void ak09918_set_mode(ak09918_dev_t* dev, AK09918_mode_type_t mode)
{
    uint8_t ctrl2;
    ak09918_read(dev, AK09918_CNTL2_REG, &ctrl2, 1);

    ctrl2 &= ~AK09918_MODE_MASK;
    ctrl2 |= mode;

    ak09918_write(dev, AK09918_CNTL2_REG, ctrl2);
    dev->comp_mode = mode;
}

void ak09918_init(ak09918_dev_t* dev)
{
    i2c_device_config_t dev_cfg
        = { .dev_addr_length = I2C_ADDR_BIT_LEN_7, .device_address = AK09918_I2C_ADDR, .scl_speed_hz = I2C_FAST_MOOD };

    i2c_master_dev_handle_t dev_handle;
    ESP_ERROR_CHECK(i2c_master_bus_add_device(dev->ak09918_master_handle, &dev_cfg, &dev_handle));

    dev->ak09918_device_handle = dev_handle;

    uint8_t ID = ak09918_get_id(dev);
    if (ID == I_AM_AK09918)
        printf("found ak09918 device: 0x%x\r\n", ID);
    else
        printf("Error, ak09918 not found: 0x%x\r\n", ID);

    ak09918_set_mode(dev, AK09918_CONTINUOUS_10HZ);
    vTaskDelay(10 / portTICK_PERIOD_MS);

    ak09918_write(dev, AK09918_CNTL3_REG, 0x00);
    vTaskDelay(10 / portTICK_PERIOD_MS);
}

void ak09918_read_mag(ak09918_dev_t* dev)
{
    uint8_t mag_buffer[8] = { 0 };
    int16_t raw_data[3] = { 0 };
    float data[3];
    uint8_t status;

    ak09918_read(dev, AK09918_ST1_REG, &status, 1);
    if (status & AK09918_DRDY_BIT)
    {
        ak09918_read(dev, AK09918_HXL_REG, mag_buffer, 8);
        if (!(mag_buffer[7] & AK09918_HOFL_BIT))
        {
            for (uint8_t i = 0; i < 3; i++)
            {
                raw_data[i] = (int16_t) (mag_buffer[2 * i] | (mag_buffer[2 * i + 1] << 8));
                data[i] = raw_data[i] * AK09918_SENSIVITITY;
            }

            dev->comp_axes.comp_x = data[0];
            dev->comp_axes.comp_y = data[1];
            dev->comp_axes.comp_z = data[2];
        }
    }
}
