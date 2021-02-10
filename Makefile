BOARD ?= nrf52dk_nrf52832
VERSION ?= 0.0.0
SIGN_KEY ?= $(ZEPHYR_BASE)/../bootloader/mcuboot/root-rsa-2048.pem


.PHONY: all clean flash menuconfig guiconfig init mcuboot flash_mcufoot signed flash_signed

all:
	west build -b $(BOARD)

clean:
	rm -rf build

flash:
	west flash

menuconfig:
	west build -t menuconfig

guiconfig:
	west build -t guiconfig

init:
	@echo "source $(HOME)/zephyrproject/zephyr/zephyr-env.sh"

mcuboot:
	west build -b $(BOARD) -s $(HOME)/zephyrproject/bootloader/mcuboot/boot/zephyr/ -d build/mcuboot

flash_mcuboot:
	west flash -d build/mcuboot

signed:
	west sign -t imgtool -- --version $(VERSION) --key $(SIGN_KEY) --slot-size 0x32000

flash_signed:
	west flash --hex-file build/zephyr/zephyr.signed.hex

#sign_update:
#	west sign -t imgtool -- --version $(VERSION) --key $(SIGN_KEY) --slot-size 0x32000 --pad

#flash_update:
#	west flash --bin-file build/zephyr/zephyr.signed.bin ## offset 0x03E000 ???

smp_svr_shell:
	west build -b $(BOARD) $(ZEPHYR_BASE)/samples/subsys/mgmt/mcumgr/smp_svr  -- -DOVERLAY_CONFIG='overlay-shell.conf'

smp_svr_ble:
	west build -b $(BOARD) $(ZEPHYR_BASE)/samples/subsys/mgmt/mcumgr/smp_svr  -- -DOVERLAY_CONFIG=overlay-bt.conf

## smp svr management
##
## create acm connection: ~/go/bin/mcumgr conn add acm0 type="serial" connstring="dev=/dev/ttyACM0,baud=115200,mtu=512"
## create hci conection:  ~/go/bin/mcumgr conn add hci0 type="ble"    connstring="ctlr_name=hci0,peer_name=Zephyr"
## Note: hci0 dev access requires sudo
## upload update:         ~/go/bin/mcumgr -c <conn_name> image upload build/zephyr/zephyr.signed.bin
## list images, hashes:   ~/go/bin/mcumgr -c <conn_name> image list
## test new image:        ~/go/bin/mcumgr -c <conn_name> image test <hash>
## reboot:                ~/go/bin/mcumgr -c <conn_name> reset
## confirm new image:     ~/go/bin/mcumgr -c <conn_name> image confirm
