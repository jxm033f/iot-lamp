import serial
import socket
import time

#api and location information
api_key = "490a5a33da6ecfbad4c3dc91138d9a12"
lat = 40.712776
lon = -74.005974
url = "https://api.openweathermap.org/data/2.5/onecall?lat=%s&lon=%s&appid=%s&units=imperial" % (lat, lon, api_key)

UDP_IP = "160.39.190.130"
SHARED_UDP_PORT = 4210

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.connect((UDP_IP, SHARED_UDP_PORT))

# #arduino = serial.Serial('/dev/ttyUSB0', 9600)
# arduino = serial.Serial('/dev/cu.wchusbserial14410', 115200)
# time.sleep(2)

# def get_connection():
# 	response = requests.get(url)
# 	data = json.loads(response.text)
# 	current_temp = data["current"]["temp"]
# 	tensdigit = int(current_temp//10)
# 	return tensdigit

def loop():
    while True:
        data = sock.recv(2048)
        print(data)

        # if str(arduino.readline()).find("Turn on API") != -1:
        # 	digitTemp = get_connection()
        # 	tempLetter = ["B", "B", "G", "G", "Y", "Y", "R", "R"]
        # 	arduino.write(tempLetter[digitTemp].encode())



if __name__ == "__main__":
	loop()
