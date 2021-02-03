.PHONY: all clean flash init

all:
	west build -b nrf52_pca10040

clean:
	west build -t pristine

flash:
	west flash

menuconfig:
	west build -t menuconfig

guiconfig:
	west build -t guiconfig

init:
	@echo "source $(HOME)/repos/zephyrproject/zephyr/zephyr-env.sh"
