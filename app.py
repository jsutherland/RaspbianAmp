from flask import Flask
import subprocess

app = Flask(__name__)

@app.route('/')
def hello_world():
    return 'Hello, World!'

@app.route('/vol/<int:volume>')
def set_vol(volume):
    if int(volume) > 100: volume = 80
    volume = str(volume) + "%"
    output = subprocess.run(["dsptoolkit", "set-volume", volume], stdout=subprocess.PIPE)
    return str(output)

@app.route('/vol/relative/<volume>')
def set_relative_volume(volume):
    current_volume = int(get_vol())
    target_volume = current_volume + int(volume)
    set_vol(target_volume)
    return str(target_volume)

@app.route('/vol/get')
def get_vol():
    output = subprocess.run(["dsptoolkit", "get-volume"], stdout=subprocess.PIPE)
    volume = str(output).split("/")[1].strip().replace("%",'')
    return str(volume)

@app.route('/reset')
def reset():
    output = subprocess.run(["dsptoolkit", "reset"], stdout=subprocess.PIPE)
    return str(output)
