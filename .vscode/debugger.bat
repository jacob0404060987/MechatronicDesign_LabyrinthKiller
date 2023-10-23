set PATH=c:\Users\kubap\.vscode\HusarionTools\bin\;%PATH%
cd c:\Users\kubap\OneDrive\Pulpit\GIT\MechatronicDesign_LabyrinthKiller || exit 1
start /wait st-flash write myproject.bin 0x08010000 || exit 1
start st-util
arm-none-eabi-gdb %*