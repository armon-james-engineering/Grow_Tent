# Grow_Tent
Control system for indoor garden.

A control system for indoor gardening grow tents. The system controls the various parameteres needed for keeping a grow tent (which is basically a glorified green house) 
functioning.

The project is coded using embedded C. It utilizes much of the STM32 HAL libraries as this was quicker for development purposes. Future editions of the system may be rewritten using a bare metal approach to save on memory footprint and more importantly, to improve coding skills.

The system monitors the following variables:
- Light
- Temperature
- Humidity
- Airflow

Build Components Include:
- STM32L432 Microcontroller
- DHT22 Temperature and Humidity Sensor
- SD Card and RTC module
- 128 x 64 Monochrome LCD

