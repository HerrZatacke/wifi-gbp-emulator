#/bin/bash

rm -rf ./wifi-gbp-emulator/data/w
rm -rf ./wifi-gbp-emulator/data/pathmap.txt
curl -L https://github.com/HerrZatacke/gb-printer-web/releases/latest/download/data_w.zip --output ./data_w.zip
unzip ./data_w.zip -d ./wifi-gbp-emulator/data
rm ./data_w.zip
