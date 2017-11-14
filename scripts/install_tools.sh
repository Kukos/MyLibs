#!/bin/bash

#Author: Michal Kukowski
#email: michalkukowski10@gmail.com

# This script install needed tools to use & "play with" libs

source log.sh

# C / C++
C_PACKAGES=("gcc" "make")

# MISC TOOLS
TOOLS=("valgrind" "sed" "gawk", "wc", "bc")

installed=0

# Update current software to newest version
info "Updating software ..."
sudo apt-get update > /dev/null 2>&1
sudo apt-get upgrade --yes > /dev/null 2>&1
success "Update complete"

# Install C Packages
len=${#C_PACKAGES[@]}
for ((i = 0; i < ${len}; ++i)); do
    if ! dpkg -l | grep -qw "${C_PACKAGES[i]}"; then
        info "Installing ${C_PACKAGES[i]} ..."
        sudo apt-get --yes install "${C_PACKAGES[i]}" >/dev/null 2>&1 \
        || error "${C_PACKAGES[i]} "
        success "${C_PACKAGES[i]} Installed"
        ((installed++))
    fi
done

# Install TOOLS
len=${#TOOLS[@]}
for ((i = 0; i < ${len}; ++i)); do
    if ! dpkg -l | grep -qw "${TOOLS[i]}"; then
        info "Installing ${TOOLS[i]} ..."
        sudo apt-get --yes install "${TOOLS[i]}" >/dev/null 2>&1 \
        || error "${TOOLS[i]}"
        success "${TOOLS[i]} Installed"
        ((installed++))
    fi
done

# Summary
if [ $installed -eq 0 ]; then
    info "You have all needed packages"
else
	info "Installed $installed packages"
fi
