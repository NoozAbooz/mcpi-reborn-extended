#!/bin/bash

curl -s --compressed "https://mcpirevival.tk/mcpi-packages/debs/KEY.gpg" | sudo apt-key add -
sudo curl -s --compressed -o /etc/apt/sources.list.d/mcpi-revival.list "deb https://mcpirevival.tk/mcpi-packages/debs/mcpi-revival.list"
sudo apt update
