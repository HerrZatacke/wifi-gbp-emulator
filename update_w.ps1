if (Test-Path ./websocket-gbp-emulator/data/w) {
  Remove-Item ./websocket-gbp-emulator/data/w -Recurse -Force -Confirm:$false
}
Invoke-WebRequest -Uri https://github.com/HerrZatacke/gb-printer-web/releases/latest/download/data_w.zip -OutFile ./data_w.zip
Expand-Archive -Path ./data_w.zip -DestinationPath ./websocket-gbp-emulator/data/w
Remove-Item ./data_w.zip
