import socket
import struct
import json
import pandas as pd
from fastcrc import crc16

# Globale Variable für DataFrame
df = pd.DataFrame()

# Funktion zum Berechnen der CRC16-Prüfsumme mithilfe der FastCRC-Bibliothek
def calculate_crc(data: bytes) -> int:
    return crc16.modbus(data)

# Funktion zum Dekodieren der empfangenen Daten gemäß der TDataAll Struktur
def decode_data(data: bytes):
    # Format-String für die TDataAll Struktur
    # Aktualisierter Format-String für TDataRC mit neuen bool-Attributen
    unpack_format = '<Hll10sBB5s5s16H3HBBB3B2H2H3B3B3B3B2B'
    
    # Entpacken der Daten gemäß der Struktur, dabei die letzten 2 Bytes (CRC16) ausschließen
    unpacked_data = struct.unpack(unpack_format, data[:-2])

    # Dekodierte Daten in einer strukturierten Form
    decoded_data = {
        "TDataGlobal": {
            "start_millis": unpacked_data[1],
            "end_millis": unpacked_data[2],
            "fw_version": unpacked_data[3].decode('ascii').rstrip('\x00')  # Dekodieren und Null-Bytes entfernen
        },
        "TDataStatus": {
            "status_pattern": unpacked_data[4],
            "error_pattern": unpacked_data[5]
        },
        "TDataGroup": {
            "domain": unpacked_data[6].decode('ascii').rstrip('\x00'),
            "subdomain": unpacked_data[7].decode('ascii').rstrip('\x00')
        },
        "TDataRC": {
            "channels": unpacked_data[8:24],  # Extrahieren der 16 Kanäle
            "gimbal_min": unpacked_data[24],
            "gimbal_max": unpacked_data[25],
            "gimbal_mid": unpacked_data[26],
            "fail_save": bool(unpacked_data[27]),
            "lost_frame": bool(unpacked_data[28]),
            "is_armed": bool(unpacked_data[29])
        },
        "TDataSurface": {
            "pinTOF": unpacked_data[30],
            "pinRXLidar": unpacked_data[31],
            "pinTXLidar": unpacked_data[32],
            "pidTOF": unpacked_data[33:36],  # P, I, D Werte für TOF
            "pidLIDAR": unpacked_data[36:39],  # P, I, D Werte für LIDAR
            "minHeight": unpacked_data[39],
            "maxHeight": unpacked_data[40],
            "sensorConditionMin": unpacked_data[41],
            "sensorConditionMax": unpacked_data[42]
        },
        "TDataOFlow": {
            "pidRGain": unpacked_data[43:46],  # P, I, D Werte für Roll
            "pidPGain": unpacked_data[46:49],  # P, I, D Werte für Pitch
            "pidYGain": unpacked_data[49:52],  # P, I, D Werte für Yaw
            "biasRPY": unpacked_data[52:55],  # Bias für Roll/Pitch/Yaw
            "setPointSlipRP": unpacked_data[55:57]  # Setpoints für Slip Roll/Pitch
        }
    }
    
    return decoded_data

# Funktion zum Speichern der empfangenen Daten in das Pandas DataFrame
def save_data_to_dataframe(decoded_data):
    global df
    # Konvertiere die Daten in ein Pandas DataFrame und füge sie dem bestehenden DataFrame hinzu
    new_df = pd.json_normalize(decoded_data)
    df = pd.concat([df, new_df], ignore_index=True)

# Funktion zur Ausgabe der formatierten TDataRC-Daten
def print_tdata_rc(decoded_data):
    tdata_rc = decoded_data.get("TDataRC", {})
    formatted_rc = json.dumps(tdata_rc, indent=4)
    print("TDataRC structure:")
    print(formatted_rc)

# UDP-Server-Konfiguration
UDP_IP = "0.0.0.0"  # Empfang auf allen verfügbaren Netzwerkschnittstellen
UDP_PORT = 4210     # Port für den UDP-Server
BUFFER_SIZE = 1024  # Maximale Größe des Puffers für empfangene Daten

# Erstellen eines UDP-Sockets
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

print(f"Listening for UDP packets on port {UDP_PORT}...")

while True:
    # Empfangen von UDP-Daten
    data, addr = sock.recvfrom(BUFFER_SIZE)
    
    # Extrahieren der CRC16-Prüfsumme (letzten 2 Bytes)
    received_crc = struct.unpack('<H', data[-2:])[0]
    # Berechnen der CRC16-Prüfsumme für die empfangenen Daten (ohne die letzten 2 Bytes)
    calculated_crc = calculate_crc(data[:-2])

    # Prüfen, ob die CRC-Prüfsumme übereinstimmt
    if received_crc != calculated_crc:
        print(f"CRC mismatch! Received: {hex(received_crc)}, Calculated: {hex(calculated_crc)}")
        continue

    # Dekodieren der Daten in eine lesbare Struktur
    decoded_data = decode_data(data)
    
    # Speichern der Daten in das DataFrame
    save_data_to_dataframe(decoded_data)

    # Ausgabe der formatierten TDataRC-Daten im Terminal
    print_tdata_rc(decoded_data)
