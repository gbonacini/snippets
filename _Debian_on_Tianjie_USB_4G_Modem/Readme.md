Howto Install Debian on Tianjin USB 4G Modem
============================================

* On a Linux PC, install following dependencies:
```shell
  apt install android-tools-adb android-tools-fastboot 
```
* Insert USB Modem in one USB port of the same computer;
* Set an ip for the device using network address 192.168.100.0, different from 192.168.100.1 that is the address of the http server, for example 192.168.100.111/24 ;
* Access to the following page of device's http server to set the device itself in debug mode:
```shell
  http://192.168.100.1/usbdebug.html 
```
* Launch the command:
```shell
 adb devices 
```
   this command will start add server if not already running, if the modem is recognised properly, following output will be printed:
```shell
  List of devices attached
  0123456789ABCDEF	device
```
* Then launch these other commands:
```shell
   adb reboot bootloader
   fastboot devices
```
* The latter will print something like this:
```shell
   12e698e	fastboot
```
* From https://github.com/OpenStick/OpenStick/releases , download the following 2 files:
```shell
  debian.zip
  base-generic.zip
```
* Expand base-generic.zip:
```shell
 unzip base-generic.zip
```
run the script:
```shell
  base/flash.sh
```
* note - this is its content:
```shell
#!/bin/sh
# 
# Copyright (C) 2021-2022 OpenStick Project
# Author : HandsomeYingyan <handsomeyingyan@gmail.com>
#

fastboot erase boot
fastboot flash aboot aboot.bin
fastboot reboot
sleep 5
fastboot oem dump fsc && fastboot get_staged fsc.bin
fastboot oem dump fsg && fastboot get_staged fsg.bin
fastboot oem dump modemst1 && fastboot get_staged modemst1.bin
fastboot oem dump modemst2 && fastboot get_staged modemst2.bin
fastboot erase boot
fastboot reboot bootloader
sleep 5


# Now We Got All Of Them To Rock Linux!
fastboot flash partition gpt_both0.bin
fastboot flash hyp hyp.mbn
fastboot flash rpm rpm.mbn
fastboot flash sbl1 sbl1.mbn
fastboot flash tz tz.mbn
fastboot flash fsc fsc.bin
fastboot flash fsg fsg.bin
fastboot flash modemst1 modemst1.bin
fastboot flash modemst2 modemst2.bin
fastboot flash aboot aboot.bin
fastboot flash cdt sbc_1.0_8016.bin
fastboot erase boot
fastboot erase rootfs

fastboot reboot
echo 'all done please flash your os!'
sleep 5
This is the expected result:
  ./flash.sh 
Erasing 'boot'                                     OKAY [  0.106s]
Finished. Total time: 0.114s
Sending 'aboot' (280 KB)                           OKAY [  0.015s]
Writing 'aboot'                                    OKAY [  0.035s]
Finished. Total time: 0.062s
Rebooting                                          OKAY [  0.005s]
Finished. Total time: 0.055s
                                                   OKAY [  0.002s]
Finished. Total time: 0.002s
Uploading 'fsc.bin'                                OKAY [  0.002s]
Finished. Total time: 0.002s
                                                   OKAY [  0.031s]
Finished. Total time: 0.031s
Uploading 'fsg.bin'                                OKAY [  0.078s]
Finished. Total time: 0.078s
                                                   OKAY [  0.030s]
Finished. Total time: 0.030s
Uploading 'modemst1.bin'                           OKAY [  0.073s]
Finished. Total time: 0.074s
                                                   OKAY [  0.030s]
Finished. Total time: 0.030s
Uploading 'modemst2.bin'                           OKAY [  0.074s]
Finished. Total time: 0.074s
Erasing 'boot'                                     OKAY [  0.101s]
Finished. Total time: 0.108s
Rebooting into bootloader                          OKAY [  0.005s]
Finished. Total time: 0.055s
Sending 'partition' (33 KB)                        OKAY [  0.005s]
Writing 'partition'                                OKAY [  1.196s]
Finished. Total time: 1.213s
Sending 'hyp' (12 KB)                              OKAY [  0.004s]
Writing 'hyp'                                      OKAY [  0.005s]
Finished. Total time: 0.019s
Sending 'rpm' (512 KB)                             OKAY [  0.020s]
Writing 'rpm'                                      OKAY [  0.050s]
Finished. Total time: 0.080s
Sending 'sbl1' (512 KB)                            OKAY [  0.021s]
Writing 'sbl1'                                     OKAY [  0.052s]
Finished. Total time: 0.083s
Sending 'tz' (591 KB)                              OKAY [  0.024s]
Writing 'tz'                                       OKAY [  0.058s]
Finished. Total time: 0.092s
Sending 'fsc' (1 KB)                               OKAY [  0.004s]
Writing 'fsc'                                      OKAY [  0.005s]
Finished. Total time: 0.019s
Sending 'fsg' (1536 KB)                            OKAY [  0.060s]
Writing 'fsg'                                      OKAY [  0.139s]
Finished. Total time: 0.210s
Sending 'modemst1' (1536 KB)                       OKAY [  0.060s]
Writing 'modemst1'                                 OKAY [  0.155s]
Finished. Total time: 0.226s
Sending 'modemst2' (1536 KB)                       OKAY [  0.059s]
Writing 'modemst2'                                 OKAY [  0.139s]
Finished. Total time: 0.210s
Sending 'aboot' (280 KB)                           OKAY [  0.015s]
Writing 'aboot'                                    OKAY [  0.033s]
Finished. Total time: 0.058s
Sending 'cdt' (0 KB)                               OKAY [  0.004s]
Writing 'cdt'                                      OKAY [  0.005s]
Finished. Total time: 0.019s
Erasing 'boot'                                     OKAY [  0.025s]
Finished. Total time: 0.032s
Erasing 'rootfs'                                   OKAY [  0.544s]
Finished. Total time: 0.551s
Rebooting                                          OKAY [  0.004s]
Finished. Total time: 0.054s
all done please flash your os!
```
* Expand debian.zip:
```shell
unzip debian.zip
```
Run the following script:
```shell
  debian/flash.sh
```
* note - this is its content:
```shell
#!/bin/sh
# 
# Copyright (C) 2021-2022 OpenStick Project
# Author : HandsomeYingyan <handsomeyingyan@gmail.com>
#
# get in new aboot
# now we flash debian
fastboot -S 200M flash rootfs rootfs.img
fastboot flash boot boot.img
fastboot reboot
This is the expected result, some minutes are required to complete:
./flash.sh
Sending sparse 'rootfs' 1/5 (203958 KB)            OKAY [  7.372s]
Writing 'rootfs'                                   OKAY [ 72.523s]
Sending sparse 'rootfs' 2/5 (197981 KB)            OKAY [  7.275s]
Writing 'rootfs'                                   OKAY [ 23.023s]
Sending sparse 'rootfs' 3/5 (204797 KB)            OKAY [  7.308s]
Writing 'rootfs'                                   OKAY [ 21.119s]
Sending sparse 'rootfs' 4/5 (204552 KB)            OKAY [  7.257s]
Writing 'rootfs'                                   OKAY [ 22.973s]
Sending sparse 'rootfs' 5/5 (86909 KB)             OKAY [  3.085s]
Writing 'rootfs'                                   OKAY [ 74.434s]
Finished. Total time: 246.385s
Sending 'boot' (13270 KB)                          OKAY [  0.490s]
Writing 'boot'                                     OKAY [  1.193s]
Finished. Total time: 1.699s
Rebooting                                          OKAY [  0.004s]
```
* Open a root shell to start device's configuration:
```shell
  adb shell
  root@openstick:/# 
```
