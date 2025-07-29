# demo.py
import cherry
import requests
import numpy as np
import json

if __name__ == "__main__":
    print(f"Cherry DEFAULT_TIMEOUT: {cherry.DEFAULT_TIMEOUT}")
    print(f"Cherry MAX_CONNECTIONS: {cherry.MAX_CONNECTIONS}")

    if cherry.connect("localhost", 8080, timeout=5.0):
        print("Successfully connected via cherry module!")
    else:
        print("Failed to connect.")

    session = cherry.Session(user="admin", password="1234")

    if session.start():
        print("Cherry session started successfully!")

    bytes_sent = session.send(b"Hello Cherry!")
    print(f"Sent {bytes_sent} bytes via Cherry Session.")

    received = session.receive(buffer_size=1024)
    print(f"Received message: {received.decode('utf-8')}")

    session.stop()

    # Use external library: requests
    response = requests.get("https://jsonplaceholder.typicode.com/todos/1")
    print("Fetched data using requests:", response.json())

    # Use external library: numpy
    array = np.array([1, 2, 3, 4, 5])
    print("NumPy array squared:", np.square(array))

    # Serialize data using json
    serialized = json.dumps({"status": "success", "data": array.tolist()})
    print("Serialized JSON data:", serialized)

    cherry.disconnect()

    # cause a deliberate error to test cherry's error handling
    raise Exception("This is a test error to trigger cherry's error handling.")
