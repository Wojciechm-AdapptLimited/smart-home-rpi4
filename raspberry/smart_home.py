from flask import Flask, request, jsonify
from flask_cors import CORS, cross_origin
import mariadb

app = Flask(__name__)
cors = CORS(app)
app.config['CORS_HEADERS'] = 'Content-Type'

def connect_to_db():
    try:
        conn = mariadb.connect(
            user="wojciech",
            password="1234",
            host="localhost",
            port=3306,
            database="smart_home"
        )
    except mariadb.Error as e:
        return None, str(e)
    return conn, None


@app.route('/post_data', methods=['POST'])
def process_data():    
    if request.content_type != 'application/json':
        return jsonify(error='Content-Type must be application/json'), 400
    
    conn, err = connect_to_db()
    if err:
        return jsonify(error=err), 500
    cur = conn.cursor()
    
    data = request.json
    device_id = None
       
    cur.execute("SELECT id FROM device WHERE name=%s", (data["device"],))
    if cur.rowcount != 0:
        device_id = cur.fetchone()[0]
    else:
        cur.close()
        conn.close()
        return jsonify(error='Device not found'), 400
    
    for name, value in data["sensors"].items():
        cur.execute("SELECT id FROM sensor WHERE name=%s AND device=%s", (name, device_id))
        if cur.rowcount != 0:
            cur.execute("INSERT INTO measurement(sensor, time, value) VALUES (%s, CURRENT_TIMESTAMP, %s)", (cur.fetchone()[0], value))
        else:
            cur.close()
            conn.close()
            return jsonify(error='Sensor Type invalid'), 400
        
    cur.close()
    conn.commit()
    conn.close()
    return jsonify(message='Data processed successfully'), 200
        
@app.route('/post_device', methods=['POST'])
def process_device():
    if request.content_type != 'application/json':
        return jsonify(error='Content-Type must be application/json'), 400
    
    conn, err = connect_to_db()
    if err:
        return jsonify(error=err), 500
    cur = conn.cursor()
    
    data = request.json
    type_id = None
    
    cur.execute("SELECT count(*) FROM device WHERE name=%s", (data["device"],))
        
    if cur.fetchone()[0] != 0:
        cur.close()
        conn.close()
        return jsonify(error='Device already registered'), 400
       
    cur.execute("SELECT id FROM device_type WHERE name=?", (data["type"],))
    if cur.rowcount != 0:
        type_id = cur.fetchone()[0]
    else:
        cur.close()
        conn.close()
        return jsonify(error='Type invalid'), 400
    
    cur.execute("INSERT INTO device(name, type) VALUES (?, ?)", (data["device"], type_id))
    device_id = cur.lastrowid;
    
    for name, type_name in data["sensors"].items():
        cur.execute("SELECT id FROM sensor_type WHERE name=?", (type_name,))
        if cur.rowcount != 0:
            cur.execute("INSERT INTO sensor(name, device, type) VALUES (?, ?, ?)", (name, device_id, cur.fetchone()[0]))
        else:
            cur.close()
            conn.close()
            return jsonify(error='Sensor Type invalid'), 400
        
    cur.close()
    conn.commit()
    conn.close()
    return jsonify(message='Data processed successfully'), 200


@cross_origin
@app.route('/get_data/<id>', methods=['GET'])
def get_data(id):
    conn, err = connect_to_db()
    if err:
        return jsonify(error=err), 500
    cur = conn.cursor()
    
    cur.execute("SELECT s.name, m.value, m.time, (SELECT st.name FROM sensor_type st WHERE st.id = s.type) FROM measurement m INNER JOIN sensor s ON m.sensor = s.id WHERE m.time = (SELECT max(m2.time) FROM measurement m2 WHERE m2.sensor = m.sensor)");
    measurements = []
    
    for sensor, value, time, s_type in cur:
        measurements.append({"sensor": sensor, "value": value, "time": time, "type": s_type})
    
    return jsonify(results=measurements), 200

@cross_origin
@app.route('/', methods=['GET'])
def home():
    conn, err = connect_to_db()
    if err:
        return jsonify(error=err), 500
    cur = conn.cursor()
    
    cur.execute("SELECT d.id, d.name, dt.name FROM device d INNER JOIN device_type dt ON d.type = dt.id");
    devices = []
    
    for id, device, d_type in cur:
        devices.append({"id": id, "device": device, "type": d_type})
        
    return jsonify(devices=devices), 200

if __name__ == 'main':
    app.run(host='0.0.0.0', port=8000)