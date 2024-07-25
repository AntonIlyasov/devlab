import random
import string
import os
from docx import Document
from docx.shared import Inches

def generate_string(length):
    all_symbols = string.ascii_lowercase + string.digits
    str1 = ''.join(random.choice(all_symbols) for _ in range(6))
    str2 = ''.join(random.choice(all_symbols) for _ in range(6))
    str3 = ''.join(random.choice(all_symbols) for _ in range(6))
    result = str1 + '-' + str2 + '-' + str3
    return result

def create_hostapd_file(id):
    input  = open('hostapdR.conf', 'r')
    output = open('hostapd.conf', 'w')
    output.write(input.read())
    output.write('ssid=orangepi-' + id)
    input.close()
    output.close()

def send_hostapd_file():
    os.system('scp hostapd.conf orangepi@192.168.0.1:~')

def create_word_file(id):
    document = Document()
    document.add_heading(str(id), 0)
    document.save('hostapd.docx')

def delete_hostapd_file():
    os.system('rm hostapd.conf')

def main():
    id = generate_string(6)
    print(id)
    create_hostapd_file(id)
    send_hostapd_file()
    create_word_file(id)
    delete_hostapd_file()


if __name__ == '__main__':
    main()