local intercept = require("intercept")

local team = 0
local robot_id = 0

-- Asegúrate de que la pelota esté en movimiento antes de iniciar
intercept.reset()

local delay = 0
local delay_frames = 60

local state = "play"

local M = {}

function M.process()
    if delay > 0 then
        delay = delay - 1
        return
    end

    if state == "play" then
        intercept.run_once(robot_id, team)
        if intercept.is_done() then
            print("[Intercept] La pelota fue interceptada ✅")
            state = "wait"
            delay = delay_frames
        end

    elseif state == "wait" then
        state = "play"
        delay = delay_frames
        intercept.reset()
        print("[Intercept] Nuevo ciclo de interceptación iniciado 🔁")
    end
end

return M
