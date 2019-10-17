@echo off

del EVdance.zip
  WinRar U EVdance.zip *.sln EVdance\*.vcxproj *.idl EVdance\*.filters *.dsp *.aps EVdance\*.rc *.cpp *.h mkzip.bat
  WinRar U EVdance.zip Debug\images\*.* Release\images\*.* res\*.*
  WinRar U EVdance.zip EVdance\*.cpp EVdance\*.h EVdance\DiagramEditor\*.* EVdance\DiskObject\*.* EVdance\TextFile\*.* EVdance\res\*.* EVdance\*.rc
@echo on