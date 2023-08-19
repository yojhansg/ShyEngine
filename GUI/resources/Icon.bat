@echo off
setlocal

:: Define la extensión de archivo para la cual cambiarás el icono
set "extension=.shyproject"

:: Define la ubicación del nuevo icono
set "nuevoIcono=C:\Users\sryoj\Documents\TFG\GUI\resources\Icon.ico"

:: Crea un archivo .reg temporal
>"%temp%\ChangeIcon.reg" (
    echo Windows Registry Editor Version 5.00
    echo.
    echo [HKEY_CLASSES_ROOT\%extension%\DefaultIcon]
    echo @="%nuevoIcono%"
)

:: Ejecuta el archivo .reg temporal para realizar el cambio de icono
regedit /s "%temp%\ChangeIcon.reg"

:: Elimina el archivo .reg temporal
del "%temp%\ChangeIcon.reg"

:: Actualiza el caché de iconos
ie4uinit.exe -ClearIconCache

:: Muestra un mensaje de éxito
echo Icono cambiado para archivos %extension% a %nuevoIcono%
pause

:: Cierra el script
exit /b