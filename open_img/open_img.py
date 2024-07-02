import base64
from io import BytesIO
from PIL import Image

img = Image.open('2.png')
im_file = BytesIO()
img.save(im_file, format="PNG")
im_bytes = im_file.getvalue()  # im_bytes: image in binary format.
im_b64 = base64.b64encode(im_bytes)
file1 = open("2.txt", "w") 
file1.write(str(im_b64))
file1.close()



print('Success')

