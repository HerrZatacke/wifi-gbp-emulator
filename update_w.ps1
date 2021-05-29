Add-Type -AssemblyName PresentationFramework

$continue = [System.Windows.MessageBox]::Show('Attention!
Installing the newest version of the web-app will remove all images stored on your printer.

Continue?', 'Confirmation', 'YesNo');

if ($continue -eq 'Yes') {
  if (Test-Path ./wifi-gbp-emulator/data/w)
  {
    Remove-Item ./wifi-gbp-emulator/data/w -Recurse -Force -Confirm:$false
  }
  Invoke-WebRequest -Uri https://github.com/HerrZatacke/gb-printer-web/releases/latest/download/data_w.zip -OutFile ./data_w.zip
  Expand-Archive -Path ./data_w.zip -DestinationPath ./wifi-gbp-emulator/data/w
  Remove-Item ./data_w.zip
}
