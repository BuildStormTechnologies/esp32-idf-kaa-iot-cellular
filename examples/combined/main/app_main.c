/**
 * \copyright Copyright (c) 2021-2024, Buildstorm Pvt Ltd
 *
 * \file app_main.c
 * \brief app_main.c file.
 *
 * The app_main.c is the main entry of the application.
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lib_system.h"
#include "lib_print.h"
#include "lib_delay.h"
#include "app_config.h"
#include "lib_gpio.h"
#include "lib_metadata.h"
#include "lib_telemetry.h"
#include "lib_commands.h"

#define thisModule APP_MODULE_MAIN
uint32_t ledBlinkCount_u32 = 0;
uint32_t nextBlinkTime_u32 = 0;

void app_eventsCallBackHandler(systemEvents_et event_e)
{
    switch (event_e)
    {
    case EVENT_WIFI_CONNECTED:
        print_verbose("EVENT_WIFI_CONNECTED");
        break;
    case EVENT_WIFI_DISCONNECTED:
        print_verbose("EVENT_WIFI_DISCONNECTED");
        break;

    case EVENT_MQTT_CONNECTED:
        print_verbose("EVENT_MQTT_CONNECTED");
        break;
    case EVENT_MQTT_DISCONNECTED:
        print_verbose("EVENT_MQTT_DISCONNECTED");
        break;

    default:
        break;
    }
}

void cmd_callBackHander(const char *pCmd, const char* pCmdId, const char *pPayload)
{
    print_verbose("id:%s [%s]:%s",pCmdId,pCmd,pPayload);
    if(strcmp(pCmd,"LED")==0)
    {
        ledBlinkCount_u32 = util_getNumFromStringU32(pPayload);
        if((ledBlinkCount_u32 == 0) || (ledBlinkCount_u32 == 1))
        {
           GPIO_pinWrite(LED_PIN,ledBlinkCount_u32); // 0-LED OFF,  1-> LED ON
           ledBlinkCount_u32 = 0;
        }
        else //N- Blink led N-times
        {
            ledBlinkCount_u32 = ledBlinkCount_u32*2; // double it for ON->OFF cycle
        }
    }
    COMMAND_updateStatus(pCmd,pCmdId,pPayload,200,"Ok");
}

void app_handleLedCmd()
{
    if (ledBlinkCount_u32)
    {
        if (millis() > nextBlinkTime_u32)
        {
            ledBlinkCount_u32--;
            nextBlinkTime_u32 = millis() + 500;
            GPIO_pinWrite(LED_PIN, ledBlinkCount_u32 & 0x01);
        }
    }
}

void metadata_updateCallBack(const char *pKeyStr, const void *pValue, valueType_et valuetype)
{
    switch (valuetype)
    {
    case VALUE_TYPE_INT:
        print_verbose("[%s]: %d", pKeyStr, *(int *)pValue);
        break;

    case VALUE_TYPE_FLOAT:
        print_verbose("[%s]: %.2f", pKeyStr, *(float *)pValue);
        break;

    case VALUE_TYPE_STRING:
        print_verbose("[%s]: %s", pKeyStr, pValue);
        break;

    default:
        break;
    }
}

void app_task(void *param)
{
    static uint32_t nextPubTime = 0;
    uint32_t temp_u32 = 20;
    float humidity = 10.0;

    if(nextPubTime == 0)
    {
       nextPubTime = millis();
       METADATA_update("platform", (void *)"ESP32", VALUE_TYPE_STRING);
       METADATA_update("bootTime", &nextPubTime, VALUE_TYPE_INT);
    }

    while (1)
    {
        if (SYSTEM_getMode() == SYSTEM_MODE_NORMAL)
        {
            if (KAA_isConnected())
            {
                if ((millis() > nextPubTime))
                {
                    TELEMETRY_update("temp", &temp_u32, VALUE_TYPE_INT);
                    TELEMETRY_update("hum", &humidity, VALUE_TYPE_FLOAT);
                    print_verbose("[temp]:%d [hum]: %.2f\n", temp_u32, humidity);

                    temp_u32++;
                    humidity = humidity + 1.1;
                    nextPubTime = millis() + 5000;
                }
                app_handleLedCmd();
            }
        }

        TASK_DELAY_MS(200);
    }
}

/**
 * @brief    entry point of the project
 * @param    None
 * @return   None
 */
void app_main()
{

    systemInitConfig_st sysConfig = {
        .systemEventCallbackHandler = app_eventsCallBackHandler,
        .pAppVersionStr = APP_VERSION,
        //.pWifiSsidStr = TEST_WIFI_SSID,
        //.pWifiPwdStr = TEST_WIFI_PASSWORD,
        .maxMetaElements_u8 = 4,
        .maxTelemetryElements_u8 = 2,
        .maxCommands_u8 = 2,
       /* Modem Configuration */
       .s_modemConfig ={
            .model = QUECTEL_EC200U,
            .uartPortNum_u8 = MODEM_UART_NUM,
            .rxPin_u8 = MODEM_RX_UART_PIN,
            .txPin_u8 = MODEM_TX_UART_PIN,
            .pwKeyPin_u8 = MODEM_POWERKEY_GPIO_PIN,
            .resetKeyPin_u8 = MODEM_RESETKEY_GPIO_PIN,
            .pApn = APN,
            .pApnUsrName = USERID,
            .pApnPwd = PASSWORD,
            },
        .s_mqttClientConfig = {
            .pUriStr = KAA_URI,
            .port_u16 = KAA_PORT,
            .pKaaApplicationVersionStr = KAA_APPLICATION_VERSION,
            .pAccessTokenStr = KAA_ENDPOINT_TOKEN}};

    if (SYSTEM_init(&sysConfig) == TRUE)
    {
        SYSTEM_start();

        METADATA_register("testMeta", VALUE_TYPE_STRING, metadata_updateCallBack);
        COMMAND_register("LED", cmd_callBackHander);

        GPIO_pinMode(LED_PIN, GPIO_MODE_OUTPUT, GPIO_INTR_DISABLE, NULL);
        GPIO_pinWrite(LED_PIN, LOW);

        BaseType_t err = xTaskCreate(&app_task, "app_task", TASK_APP_STACK_SIZE, NULL, TASK_APP_PRIORITY, NULL);
        if (pdPASS != err)
        {
            print_error("\n Error 0x%X in creating app_task \n restarting system\n\r\n\r", err);
            fflush(stdout);
            esp_restart();
        }
    }
    else
    {
        while (1)
        {
            print_verbose("System Init failed, verify the init config ....");
            TASK_DELAY_MS(5000);
        }
    }
}
