# name of your application
APPLICATION = hello_world_lora


# Path to the RIOT base directory
RIOTBASE ?= ../../../../RIOT-OS/RIOT



# Path to my boards
EXTERNAL_BOARD_DIRS ?= ../../boards

# Path to my modules
EXTERNAL_MODULE_DIRS ?= ../../modules



# Default board
BOARD ?= idosens_base

# Default radio driver is Semtech SX1276, can also take sx1272 as value
DRIVER ?= sx1276



#Feastures
FEATURES_REQUIRED += periph_uart periph_uart_modecfg
FEATURES_OPTIONAL += periph_rtc

# Modules needed
USEMODULE += lora_p2p
USEMODULE += fmt
USEMODULE += xtimer
USEMODULE += $(DRIVER)

include $(RIOTBASE)/Makefile.include
