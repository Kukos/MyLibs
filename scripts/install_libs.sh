#!/bin/bash

#Author: Michal Kukowski
#email: michalkukowski10@gmail.com

# This script install libs to user dir or default dir

# Full path of this script
THIS_DIR=`readlink -f "${BASH_SOURCE[0]}" 2>/dev/null||echo $0`

# This directory path
DIR=`dirname "${THIS}"`

source $DIR/log.sh

DEFAULT_DIR=~/mylibs
lib_dir=

# Check existing of argument
if [ ! -z "$1" ]; then
    lib_dir=$1
else
    lib_dir=$DEFAULT_DIR
fi

info "DIR = $lib_dir"

info "Compiling libs ..."

# Compile libs
this_dir=`pwd`
cd ../
make >/dev/null 2>&1 || error "make error"
cd $this_dir

success "Libs Compiled"

# Create dirs and copy libs
info "Installing libs to $lib_dir ..."
mkdir -p $lib_dir >/dev/null 2>&1 || error "mkdir error"

cp -R ../obj/* $lib_dir >/dev/null 2>&1 || error "cp error"

success "DONE"
