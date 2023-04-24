RIOTBASE ?= ../../../../RIOT-OS/RIOT

EXTERNAL_BOARD_DIRS ?= ../../boards

BOARD ?= idosens_base

include $(RIOTBASE)/tests/Makefile.tests_common

USEMODULE += od
USEMODULE += shell
USEMODULE += shell_cmds_default
USEMODULE += ps

DRIVER ?= sx1276

# use SX1276 by default
USEMODULE += $(DRIVER)

include $(RIOTBASE)/Makefile.include
