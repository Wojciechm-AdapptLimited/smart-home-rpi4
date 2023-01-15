from flask import Flask, request
import json

app = Flask(__name__)

@app.route('/get_data', methods=['GET'])
def get_data():
    # Fetch data from the database
    data = {
        "temperature": 25.0,
        "humidity": 60.0,
        "soilMoisture1": 40.0,
        "soilMoisture2": 50.0,
        "soilMoisture3": 30.0
    }
    return json.dumps(data)

if __name__ == 'main':
    app.run(host='0.0.0.0', port=8000)
