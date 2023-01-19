from flask import Flask, request
import json
import mariadb

app = Flask(__name__)

@app.route('/post_data', methods=['POST'])
def process_data():
    content_type = request.headers.get('Content-Type')
    if (content_type != 'application/json'):
        return
    
        
@app.route('/post_device', methods=['POST'])
def process_device():
    content_type = request.headers.get('Content-Type')
    if (content_type != 'application/json'):
        return "text"
    return "text"
    data = request.json
    
    

@app.route('/get_data', methods=['GET'])
def get_data():
    # Fetch data from the database
    return "text"

if __name__ == 'main':
    app.run(host='192.168.4.1', port=8000, ssl_context=("server.crt", "server.key"))