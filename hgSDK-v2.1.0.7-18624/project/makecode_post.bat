set dirname=%1
set dirname=%dirname:~0,-4%

if not exist bakup md bakup
if not exist bakup\%dirname% md bakup\%dirname%

copy .\Obj\*.elf bakup\%dirname%\
copy .\Lst\*.map bakup\%dirname%\
copy makedis.bat bakup\%dirname%\
