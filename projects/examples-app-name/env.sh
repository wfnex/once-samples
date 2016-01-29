#!/bin/sh
#
# Copyright (C) 2013-2014, Nanjing WFNEX Technology Co., Ltd
#
CURRENT=`pwd`
export ROOTDIR=$CURRENT/../../

LD_LIBRARY_PATH=$ROOTDIR/libs:$ONCE_HOME/libs:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH;

export USER_ROOT=$CURRENT/../../


