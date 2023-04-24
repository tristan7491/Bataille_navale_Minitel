# name of your application
APPLICATION = hello-world

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= ../../../../RIOT-OS/RIOT

EXTERNAL_BOARD_DIRS ?= ../../boards

# If no BOARD is found in the environment, use this default:
BOARD ?= native


FEATURES_REQUIRED +=  periph_uart periph_uart_modecfg

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

include $(RIOTBASE)/Makefile.include
