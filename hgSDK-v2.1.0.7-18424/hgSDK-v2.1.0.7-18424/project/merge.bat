cd /d %~dp0

set dirname=%1
set dirname2=%dirname:~0,-4%

del %dirname2%_PSRAM.bin
copy %dirname% program.bin
BinScript.exe BinScript2.BinScript
copy project_merge.bin %dirname2%_PSRAM.bin

makecode_post.bat %1
