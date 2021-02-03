.PHONY: all clean flash init

all:
	west build -b nrf52dk_nrf52832

clean:
	west build -t pristine

flash:
	west flash

menuconfig:
	west build -t menuconfig

guiconfig:
	west build -t guiconfig

init:
	@echo "source $(HOME)/zephyrproject/zephyr/zephyr-env.sh"

mcuboot:
	west build -b nrf52dk_nrf52832 -s $(HOME)/zephyrproject/bootloader/mcuboot/boot/zephyr/

sign:
	west sign -t imgtool -- --key $(HOME)/zephyrproject/bootloader/mcuboot/root-rsa-2048.pem

flash_signed:
	west flash --hex-file build/zephyr/zephyr.signed.hex
