local M = {}

function M.execute(team, robots)
    print("[SimplePlay] Ejecutando jugada simple ⚽")
    -- Aquí puedes definir una jugada básica
    for id, position in pairs(robots) do
        print("Moviendo robot " .. id .. " a posición (" .. position.x .. ", " .. position.y .. ")")
        -- Llama a funciones de tactics o skills para mover robots
    end
end

return M