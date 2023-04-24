MINITEL ?= 1

# name of your application
APPLICATION = minitel_$(MINITEL)

# If no BOARD is found in the environment, use this default:
BOARD ?= idosens_base

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= /home/switch/minitel_projet/RIOT-master

CFLAGS += -DMINITEL_NUM=$(MINITEL)

#Feastures
FEATURES_REQUIRED += periph_uart periph_uart_modecfg
FEATURES_OPTIONAL += periph_rtc

# INCLUDES
INCLUDES += -I$(CURDIR)/include

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1


# Default radio driver is Semtech SX1276, can also take sx1272 as value
DRIVER ?= sx1276

# Modules needed
USEMODULE += lora_p2p
USEMODULE += fmt
USEMODULE += xtimer
USEMODULE += $(DRIVER)

include $(RIOTBASE)/Makefile.include
