# ESP32 IDF KAA IoT Platform
The Buildstorm platform simplifies the process of integrating KAA IoT core features onto ESP32 Hardware, covering basic IoT functionalities like pub/sub to advanced features such as OTA updates and device provisioning, the platform handles it all. This simplified approach saves developers time and effort, empowering them to concentrate on creating innovative IoT solutions without encountering complex integration challenges.

Expanding the capabilities of ESP32 hardware to include Cellular IoT functionalities unlocks new possibilities for IoT projects. With cellular connectivity, devices can communicate over long distances, even in remote or mobile environments where traditional Wi-Fi connections are impractical. This broader reach enables applications such as asset tracking, remote monitoring, and industrial automation.

The platform's architecture is designed around core IDF APIs, ensuring compatibility, reliability, and access to the full capabilities of ESP32 hardware provided by the ESP-IDF ecosystem.

The platform ensures smooth functionality by enabling the non-blocking operation of all APIs. Each user action, like HTTP requests, MQTT publishing, Commands, and OTA updates, is logged as an event and seamlessly managed in the background. To maintain this seamless operation, the platform efficiently handles tasks through a dedicated system task, allowing users to concurrently run other application tasks.

This establishes a robust foundation for your upcoming IoT project.

Supported features:

1. MQTT pub/sub
2. Telemetry updates
3. Metadata management
4. Command processing
5. OTA firmware updates

## Suppported Modems
1. Quectel EC200U
2. Quectel BG95,96
3. Quectel M66, M95

## References
1. [KAA IoT Platform](https://buildstorm.com/solutions/kaa-iot-cellular-solution/)
2. [KAA IoT Platform](https://buildstorm.com/solutions/kaa-iot-cellular-solution/)
2. [Exploring KAA IoT Commands](https://buildstorm.com/blog/kaa-iot-commands/)
3. [ESP32 OTA Updates with KAA IoT](https://buildstorm.com/blog/kaa-iot-ota-updates/)


## Requirements
1. This library uses esp-idf v5.2.0

---

---
## Example Setup
1. Update the following WiFi and KAA parameters in `app_config.h` of the example
2. This configuration will be same accross all examples

```
#define TEST_WIFI_SSID "YOUR WIFI SSID"
#define TEST_WIFI_PASSWORD "YOUR PASSWORD"

#define KAA_URI "mqtt://mqtt.cloud.kaaiot.com"
#define KAA_PORT 1883

#define KAA_APPLICATION_VERSION "Your Device Application Version"
#define KAA_ENDPOINT_TOKEN "Your Device Endpoint Token"


#define MODEM_POWERKEY_GPIO_PIN 5
#define MODEM_RESETKEY_GPIO_PIN 4
#define MODEM_RX_UART_PIN 16
#define MODEM_TX_UART_PIN 17
#define MODEM_UART_NUM 2

#define APN "airtelgprs.com"
#define USERID ""
#define PASSWORD ""
```

---

## Soc Compatibility

| Name     | BLE       | OTA       |
| -------- | --------- | --------- |
| ESP32    | Supported | Supported |
| ESP32 S3 | Supported | Supported |
