#!/bin/bash
make Debug
if [ "$?" != "0" ]; then
	exit -1
fi

sudo .bin/Debug/udjat -f

	
