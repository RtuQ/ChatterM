--init.lua
print("set up wifi mode")
wifi.setmode(wifi.STATION)
station_cfg = {}
station_cfg.ssid = "360"
station_cfg.pwd = "12345678"
station_cfg.save = true
wifi.sta.config(station_cfg)
wifi.sta.connect()
wifi.sta.autoconnect(1)

wifi_connetion=0
tmr.alarm(1,1000,1,function()
if wifi.sta.getip() == nil then
print("IP unavaiable,Waiting...")
else
tmr.stop(1)
print("Config done,IP is"..wifi.sta.getip())
dofile("mqttconfig.lua")
end
end)




