#!/usr/bin/env bash

#
# Author: Tuna CICI
# GUI installer for *.wic formatted OS images, which are generated by Yocto.
#
# Requirements:
# dialog(1)
#

# TODO: Below dialog(1) screens and their behaviours (which are TBD)
# 1. Main menu
# 2. Install OS
# 3. Shell
# 3. Shutdown
# 4. Reboot

VERSION="v1.0.0-rc"
IMAGE_PATH="/home/root/images/core-image-full-cmdline.wic"

MAIN_MENU_TITLE="Welcome"
NETWORK_IF="enp0s3"

function display_install_os() {
    dialog --title "Install OS" --msgbox "TODO" 0 0
}

function display_shell() {
    bash
}

function display_shutdown() {
    dialog --title "Shutdown" --msgbox "TODO" 0 0
}

function display_reboot() {
    dialog --title "Reboot" --msgbox "TODO" 0 0
}

function display_show_warning() {
    dialog --title "Show warning" --msgbox "TODO" 0 0
}


function display_main_menu () {
    IPV4=$(ip -4 addr show "${NETWORK_IF}" \
        | grep -oP '(?<=inet\s)\d+(\.\d+){3}' | head -n 1)
    MENU_TITLE="OS Version: ${VERSION}\n${NETWORK_IF}: ${IPV4}"

    TEMP=$(mktemp)
    dialog --title "${MAIN_MENU_TITLE}" --menu "${MENU_TITLE}" 0 0 0 \
        1 "Install OS" \
        2 "Shell" \
        3 "Shutdown" \
        4 "Reboot" 2> "${TEMP}"

    if [ $? -ne 0 ]; then
        exit 1
    fi
    
    SELECTED=$(cat "${TEMP}")

    case "${SELECTED}" in
        1)
            display_install_os
        ;;
        2)
            display_shell
        ;;
        3)
            display_shutdown
        ;;
        4)
            display_reboot
        ;;
        *)
            display_show_warning
        ;;
    esac
}

while [ true ]; do
    display_main_menu
done
