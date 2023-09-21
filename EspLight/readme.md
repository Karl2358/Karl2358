The startup configuration is located in the 'main' section, while all other components are implemented in the 'lib' section. Here's a list of the components:

    HttpServer: This is a simple HTTP server designed to display log files.
    LightController: It calculates PWM values based on RGB/HSB inputs.
    LightModel: This component represents the model of the RGB/RGBW light state.
    Logger: It serves as a logging system, capable of logging data to files or serial connections.
    MqttClient: This is an MQTT client used for sending and receiving light-related data.
    MqttController: It provides control functions for adding and removing MQTT clients.
    OTA (Over-The-Air): This component is responsible for handling over-the-air updates.
    PWMController: It offers an interface for controlling PWM (Pulse Width Modulation).
    StatusLed: A non-blocking LED interface with customizable duration settings.
    StorageController: This component handles the loading and storage of data from flash memory.
    WifiService: It provides a straightforward integration for Wi-Fi connectivity.
