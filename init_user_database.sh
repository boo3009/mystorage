#!/bin/bash

#   list all needed variables
USER="$(whoami)"
DATABASE="mystorage"
HOST="localhost"
HEADER_FILE="db.h"

#   create database and user (via unix_socket) and grant privileges
sudo mariadb -u root -e "
CREATE DATABASE IF NOT EXISTS "$DATABASE";
CREATE USER IF NOT EXISTS '$USER'@'$HOST' IDENTIFIED VIA unix_socket;
GRANT ALL PRIVILEGES ON "$DATABASE".* TO '$USER'@'$HOST';
FLUSH PRIVILEGES;"

#   is exit status of previous command is 0 then we succed
if [ $? -eq 0 ]; then
	echo "Success."
else
	echo "Failed."
	exit 1;
fi

#   now lets correct '#define USER' line in 'db.h' file
sed -i "s/WHOAMI/$USER/g" "$HEADER_FILE"
echo "User corrected in "db.h" file."
