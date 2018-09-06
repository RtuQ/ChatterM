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
if wifi_connetion ==1 then
print("IP unavaiable,Waiting...")
wifi_connetion = 0
end
else
if wifi_connetion == 0 then
print("Config done,IP is"..wifi.sta.getip())
wifi_connetion = 1
end
end
end)


uart_s =1
hum =0
temp =0
uart.on("data",2,
  function(data)
    if uart_s ==1 then
    hum = data
    uart_s = -uart_s
    else
    if uart_s ~=1 then
    temp = data
    uart_s = 1
    end
    end
    print("hum:",hum)
    print("temp:",temp)
end, 0)

tmr.alarm(2,10000,1,function()
if wifi_connetion ==1 then
str =",;humidity,"..hum..";temperature,"..temp
print("h:",hum)
print("t:",temp)
sk=net.createConnection(net.TCP, 0)
sk:on("receive", function(sck, c) print(c) end )
sk:on("disconnection", function() print("tcp:disconnection") end )
sk:connect(80,"183.230.40.33") 
sk:on("connection", function(sck,c)
print("tcp:connection")
sk:send(
"POST /devices/38867638/datapoints?type=5 HTTP/1.0\r\n"
.."api-key: YBdbjfO2pHztjIonQF5XYOzuGi0=\r\n"
.."Host: api.heclouds.com\r\n"
.."Content-Length: "..string.len(str).."\r\n\r\n"
..str.."\r\n\r\n")
end)
end
end)



