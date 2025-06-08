# compile_protobuf.ps1

# Requiere que 'protoc.exe' esté en el PATH o se configure manualmente más abajo
# Puedes cambiar esto si necesitas usar una ruta específica a protoc.exe
$protoc = "C:/protoc/bin/protoc.exe"

# Directorios
$dstDir = "./src/protobuf"

# Archivos de RoboCup SSL
$srcSSL = "./src/protobuf/protos"
$sslProtos = @(
    "messages_robocup_ssl_detection.proto",
    "messages_robocup_ssl_wrapper.proto",
    "messages_robocup_ssl_geometry.proto"
)

# Archivos de GrSim
$srcGrSim = "./src/protobuf/protos"
$grsimProtos = @(
    "grSim_Commands.proto",
    "grSim_Packet.proto",
    "grSim_Replacement.proto"
)

# Crear carpeta destino si no existe
if (-Not (Test-Path $dstDir)) {
    New-Item -ItemType Directory -Path $dstDir | Out-Null
}

# Función para compilar .proto
function Compile-Protos($srcDir, $protoFiles) {
    foreach ($proto in $protoFiles) {
        $protoPath = Join-Path $srcDir $proto
        if (-Not (Test-Path $protoPath)) {
            Write-Host "Archivo no encontrado: $protoPath" -ForegroundColor Red
            continue
        }

        & $protoc -I="$srcDir" --cpp_out="$dstDir" "$protoPath"
        if ($LASTEXITCODE -ne 0) {
            Write-Host " Falló la compilación de $proto" -ForegroundColor Yellow
        } else {
            Write-Host "Compilado: $proto"
        }
    }
}

# Compilar todos los .proto
Write-Host "Compilando archivos de RoboCup SSL..."
Compile-Protos $srcSSL $sslProtos

Write-Host "Compilando archivos de GrSim..."
Compile-Protos $srcGrSim $grsimProtos

Write-Host "Compilación completa. Archivos generados en: $dstDir" -ForegroundColor Green
