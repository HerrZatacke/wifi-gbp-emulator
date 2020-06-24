#/bin/bash

rm -rf ./websocket-gbp-emulator/data/w
curl -L https://github.com/HerrZatacke/gb-printer-web/releases/download/1.0.2/data_w.zip --output ./data_w.zip
unzip ./data_w.zip -d ./websocket-gbp-emulator/data/w
rm ./data_w.zip
