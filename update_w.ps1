if (Test-Path ./wifi-gbp-emulator/data/w) {
  Remove-Item ./wifi-gbp-emulator/data/w -Recurse -Force -Confirm:$false
}
if (Test-Path ./wifi-gbp-emulator/data/pathmap.txt) {
  Remove-Item ./wifi-gbp-emulator/data/pathmap.txt -Recurse -Force -Confirm:$false
}
Invoke-WebRequest -Uri https://github.com/HerrZatacke/gb-printer-web/releases/latest/download/data_w.zip -OutFile ./data_w.zip
Expand-Archive -Path ./data_w.zip -DestinationPath ./wifi-gbp-emulator/data
Remove-Item ./data_w.zip
