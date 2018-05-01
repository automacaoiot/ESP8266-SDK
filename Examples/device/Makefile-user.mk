## Local build configuration
## Parameters configured here will override default and ENV values.
## Uncomment and change examples:

## Add your source directories here separated by space
# MODULES = app
# EXTRA_INCDIR = include

LIBRARY_COMMON = ../../Libraries/Common
LIBRARY_RESOURCE = ../../Libraries/Resources

#MODULES ?= app 
MODULES ?= app $(LIBRARY_COMMON)/Device $(LIBRARY_COMMON)/BaseDevice $(LIBRARY_COMMON)/Json $(LIBRARY_COMMON)/Config $(LIBRARY_RESOURCE)/LedRGB \
$(LIBRARY_COMMON)/CallBackEvents $(LIBRARY_COMMON)/WifiSetting $(LIBRARY_COMMON)/Request


EXTRA_INCDIR ?= include $(LIBRARY_COMMON)/Device $(LIBRARY_COMMON)/BaseDevice $(LIBRARY_COMMON)/Json $(LIBRARY_COMMON)/Config $(LIBRARY_RESOURCE)/LedRGB \
$(LIBRARY_COMMON)/CallBackEvents $(LIBRARY_COMMON)/WifiSetting $(LIBRARY_COMMON)/Request

## ESP_HOME sets the path where ESP tools and SDK are located.
## Windows:
# ESP_HOME = c:/Espressif

## MacOS / Linux:
# ESP_HOME = /opt/esp-open-sdk

## SMING_HOME sets the path where Sming framework is located.
## Windows:
 #SMING_HOME = c:/tools/sming/Sming

## MacOS / Linux
# SMING_HOME = /opt/sming/Sming

## COM port parameter is reqruied to flash firmware correctly.
## Windows: 
# COM_PORT = COM3

## MacOS / Linux:
# COM_PORT = /dev/tty.usbserial

## Com port speed
# COM_SPEED	= 115200

## Configure flash parameters (for ESP12-E and other new boards):
SPI_MODE = dio
SPI_SIZE = 4M
SPI_SPEED=80

## SPIFFS options
DISABLE_SPIFFS = 1
