# CondorSSL
AI software for RobocupSSL

![logo2](https://github.com/Sysmic-Robotics/CondorSSL/blob/main/docs/logo.jpg)

## Installation

lua for embedding:
```sudo apt install liblua5.3-dev```

# Compilacion windows

1 Descargar MinGW https://winlibs.com/ y extraer en el directorio C: (Aca es solo extraer el .zip en C:)
2 Instalar Qt6 en el directorio C:

3 Añadir esto a .vscode settings.json

    "cmake.configureSettings": {
        "CMAKE_PREFIX_PATH": "C:/Qt/6.8.2/mingw_64/lib/cmake",

    },

y colocar la ruta de tu cmake, en este caso mi version de qt es la 6.8.2 pero puede cambiar para ti

4 En la extension del VScode click en configure y scan for kits (depth = 5). Selecciona la carpeta donde instalaste el mingw (deberia ser C:/mingw)



