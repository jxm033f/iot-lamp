import socket

#api and location information
api_key = "490a5a33da6ecfbad4c3dc91138d9a12"
lat = 40.712776
lon = -74.005974
url = "https://api.openweathermap.org/data/2.5/onecall?lat=%s&lon=%s&appid=%s&units=imperial" % (lat, lon, api_key)

LOCAL_UDP_IP = "192.168.1.2"
SHARED_UDP_PORT = 4210

arduino = serial.Serial('/dev/ttyUSB0', 9600)
time.sleep(2)

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((LOCAL_UDP_IP, SHARED_UDP_PORT))

def get_connection():
	response = requests.get(url)
	data = json.loads(response.text)
	current_temp = data["current"]["temp"]
	tensdigit = int(current_temp//10)
	return tensdigit

def loop():
    while True:
        data, addr = sock.recvfrom(2048)
        print(data)

        if str(arduino.readline()).find("Turn on API") != -1:
        	digitTemp = get_connection()
        	tempLetter = ["B", "B", "G", "G", "Y", "Y", "R", "R"]
        	arduino.write(tempLetter[digitTemp].encode())



if __name__ == "__main__":
    loop()
