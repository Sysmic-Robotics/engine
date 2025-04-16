-- Función para verificar si el robot tiene la pelota en frente
function isBallInFront(robot, ball)
    -- Obtener las coordenadas del robot y la pelota
    local robotX, robotY = robot.x, robot.y
    local ballX, ballY = ball.x, ball.y

    -- Calcular la distancia entre el robot y la pelota
    local distance = math.sqrt((ballX - robotX)^2 + (ballY - robotY)^2)

    -- Verificar si la pelota está dentro de un rango frente al robot
    local angleToBall = math.atan(ballY - robotY, ballX - robotX)
    local angleDifference = math.abs(angleToBall - robot.orientation)

    -- Definir un umbral para considerar que la pelota está en frente
    local distanceThreshold = 1.0 -- Ajustar según el rango deseado
    local angleThreshold = math.rad(15) -- 15 grados de tolerancia

    return distance <= distanceThreshold and angleDifference <= angleThreshold
end