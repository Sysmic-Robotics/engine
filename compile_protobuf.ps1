# compile_protobuf.ps1

# Ruta absoluta al compilador protoc.exe (ajústala si cambias de versión)
$protoc = "C:\protobuf_old\bin\protoc.exe"

# Carpeta donde se dejarán los .pb.h y .pb.cc
$dstDir = "./src/protobuf"

# Directorio raíz de los .proto
$srcSSL = "./src/protobuf/protos"

# Lista de archivos .proto de SSL Game Controller y otros
$sslProtos = @(
    "ssl_game_controller_auto_ref.proto",
    "ssl_game_controller_common.proto",
    "ssl_game_controller_geometry.proto",
    "ssl_game_controller_team.proto",
    "ssl_game_event_2019.proto",
    "ssl_referee_game_event.proto",
    "ssl_gc_common.proto",
    "ssl_referee.proto",
    "ssl_gc_common.proto",
    "ssl_simulation_config.proto",
    
    "ssl_simulation_control.proto",
    "ssl_simulation_error.proto",
    "ssl_simulation_robot_control.proto",
    "ssl_simulation_robot_feedback.proto",
    "ssl_simulation_synchronous.proto",
    "ssl_vision_detection.proto",
    "ssl_vision_geometry.proto",
    "ssl_vision_wrapper.proto"
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
            Write-Host "Falló la compilación de $proto" -ForegroundColor Yellow
        } else {
            Write-Host "Compilado: $proto"
        }
    }
}

Write-Host "⏳ Compilando archivos de RoboCup SSL..."
Compile-Protos $srcSSL $sslProtos

Write-Host "`n🎉 Compilación completa. Archivos generados en: $dstDir" -ForegroundColor Green
