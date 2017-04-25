#!/bin/bash

# Script to log on stdout

# Autor Michal Kukowski
# email: michalkukowski10@gmail.com

RED="\033[1;31m"
GREEN="\033[0;32m"
BLUE="\033[0;36m"
NO_COLOR="\033[0m"

error()
{
    echo -e "${RED}[ERROR]     $1${NO_COLOR}"
    exit 1
}

info()
{
    echo -e "${BLUE}[INFO]      $1${NO_COLOR}"
}

success()
{
    echo -e "${GREEN}[SUCCESS]   $1${NO_COLOR}"
}
