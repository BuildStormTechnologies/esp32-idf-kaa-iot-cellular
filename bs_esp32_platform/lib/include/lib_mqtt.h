/**
* \copyright Copyright (c) 2019-2024, Buildstorm Pvt Ltd
*
* \file lib_mqtt.h
* \brief Mqtt library header file.
*
* The Mqtt library provides the API's for Mqtt operations
*
*
* The libraries have been tested on the ESP32 modules.
* Buildstorm explicitly denies responsibility for any hardware failures
* arising from the use of these libraries, whether directly or indirectly.

*
* EULA LICENSE:
* This library is licensed under end user license EULA agreement.
* The EULA is available at https://buildstorm.com/eula/
* For any support contact us at hello@buildstorm.com
*
*/

#ifndef _LIB_MQTT_H_
#define _LIB_MQTT_H_

/*-------------------- Includes -----------------------*/
#include "lib_system.h"
#include "lib_config.h"
#include "lib_msg.h"
#include "lib_utils.h"

/**
 * @enum mqttStates_et
 * An enum that represents states of MQTT module.
 * It will be in one of the following modes.
 */
typedef enum
{
    STATE_MQTT_IDLE,         /*!< Idle state */
    STATE_MQTT_START,        /*!< Initialize and Start MQTT */
    STATE_MQTT_DISCONNECTED, /*!< MQTT is in Disconnected state */
    STATE_MQTT_CONNECTED,    /*!< MQTT is Connected state */
    STATE_MQTT_ABORT,        /*!< Abort and close MQTT connection */
    STATE_MQTT_MAX,
} mqttStates_et;

/**
 * @enum mqttWrtStatus_et
 * An enum to represent MQTT status.
 */
typedef enum
{
    MQTT_OKAY,
    MQTT_READ_EMPTY,
    MQTT_WRITE_FULL,
    MQTT_ERROR_MAX,
} mqttWrtStatus_et;

/**
 * @brief Check if MQTT is connected to broker.
 * @param none
 * @returns Status of connection
 * @retval true if connected
 * @retval false if disconnected
 */
bool MQTT_isConnected();

/**
 * @brief Subscribe to a give topic.
 * @param [in] pTopicStr Topic to subscribe
 * @param [in] qos_e QOS Level
 * @returns Status to indicate if subscrption was successful or not
 * @retval true subscribed successfully
 * @retval false failed to subscribe
 */
bool MQTT_subscribe(char *pTopicStr, uint8_t qos_e);

/**
 * @brief Publish a given message.
 * @param [in] ps_msg Message to be published
 * @returns Status of publish request
 * @retval true if published
 * @retval false on error
 */
bool MQTT_publish(mqttMsg_st *ps_msg);

/**
 * @brief Check is messages are available on subsribed topics. The library
 * maintains a buffer to store received messages.
 * @param none
 * @returns Number of bytes available
 */
uint8_t MQTT_availableToRead();

/**
 * @brief Read the received message received.
 * @param [out] ps_msg
 * @returns Status of read message
 * @retval true on success
 * @retval false on failure
 */
bool MQTT_read(mqttMsg_st *ps_msg);

/**
 * @brief Print the status of MQTT library.
 * @param none
 * @returns none
 */

const char *MQTT_getThingName();
const char *MQTT_getUserName();

#endif //_LIB_MQTT_H_