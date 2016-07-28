-- RAZ2-Client, Su-27SM external panel and PPI export file for DCS World by VAT "Berkuts"


function LuaExportStart()
	package.path  = package.path..";.\\LuaSocket\\?.lua"
	package.cpath = package.cpath..";.\\LuaSocket\\?.dll"
	socket = require("socket")

	Raz2Client = socket.udp()
	Raz2Client:setsockname("127.0.0.1", 8092)
	Raz2Client:setpeername("127.0.0.1", 8090)
	Raz2Client:settimeout(0)

	Panel_udp = socket.udp()
	Panel_udp:setpeername("127.0.0.1", 20083)
	Panel_udp:settimeout(0)

	PPI = socket.udp()
	PPI:setpeername("127.0.0.1", 20082)
	PPI:settimeout(0)
end

function LuaExportStop()
	 Raz2Client:close()
	 Panel_udp:close()
	 PPI:close()
end

function LuaExportActivityNextEvent(t)
	local tNext = math.floor(t)
	socket.try(Raz2Client:send(string.format("%d",tNext)))
	tNext = tNext + 1
	return tNext
end

function LuaExportBeforeNextFrame()
	local Flaps = 0
	local Brake = 0
	local Gear = 0
	local Flare = 0
	local Noseflap = 0
	Flaps = LoGetMechInfo().flaps.value
	Brake = LoGetMechInfo().speedbrakes.value 
	Gear = LoGetMechInfo().gear.value 
	Noseflap = LoGetMechInfo().noseflap.value
	Flare = LoGetSnares().flare
	local Alt = LoGetAltitudeAboveGroundLevel()
	
	local MCPState = LoGetMCPState()

	local AUTOON_F = (MCPState.MasterWarning and 1) or 0

	local altBar = LoGetAltitudeAboveSeaLevel()
	local IAS = LoGetIndicatedAirSpeed()
	local TAS = LoGetTrueAirSpeed()
	local ENG_L = LoGetEngineInfo().RPM.left
	local ENG_R = LoGetEngineInfo().RPM.right
	local ENG_T_L = LoGetEngineInfo().Temperature.left
	local ENG_T_R = LoGetEngineInfo().Temperature.right
	local fuel_int = LoGetEngineInfo().fuel_internal
	local fuel_ext = LoGetEngineInfo().fuel_external
	local pitch, bank, yaw = LoGetADIPitchBankYaw()
	local AOA = LoGetAngleOfAttack()
	local GGG = LoGetAccelerationUnits().y
	local VSP = LoGetVerticalVelocity()
	local MAC = LoGetMachNumber()
	local navi = LoGetNavigationInfo()
	local HSI  = LoGetControlPanel_HSI()
	local ball = LoGetSlipBallPosition()
	local PRS = LoGetBasicAtmospherePressure()
	local glide = LoGetGlideDeviation()
	local side =  LoGetSideDeviation()
	local route = LoGetRoute()
	local PlaneID = LoGetPlayerPlaneId()
    local obj = LoGetObjectById(PlaneID)
	local dist = math.sqrt((obj.Position.x-route.goto_point.world_point.x)*(obj.Position.x-route.goto_point.world_point.x)+(obj.Position.z-route.goto_point.world_point.z)*(obj.Position.z-route.goto_point.world_point.z))
	if (dist~=nil) then dist = dist else dist = -1 	end
		
	--Отправка в панель по UDP
	socket.try(Panel_udp:send(string.format("%0.2f %0.2f %0.2f %0.2f %d", Gear, Flaps, Brake, Noseflap, AUTOON_F)))	
	
	--Отправка в PPI
	socket.try(PPI:send(string.format("%0.d_%.2f-%.2f %.2f+%0.d\n", Alt, Flaps, Brake, Gear, Flare)))
end
