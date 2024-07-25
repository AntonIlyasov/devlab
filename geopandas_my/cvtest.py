import cv2
import io
import time

def zoom_at(img, zoom, coord=None ):
            """
            Simple image zooming without boundary checking.
            Centered at "coord", if given, else the image center.

            img: numpy.ndarray of shape (h,w,:)
            zoom: float
            coord: (float, float)
            """
            # Translate to zoomed coordinates
            h, w, _ = [zoom * i for i in img.shape ]
            
            if coord is None: cx, cy = w/2, h/2
            else: cx, cy = [zoom*c for c in coord ]
            
            img = cv2.resize( img, (0, 0), fx=zoom, fy=zoom)
            img = img[ int(round(cy - h/zoom * .5)) : int(round(cy + h/zoom * .5)),
                    int(round(cx - w/zoom * .5)) : int(round(cx + w/zoom * .5)),
                    : ]
            
            return img



cap = cv2.VideoCapture(0)
# cap.set(cv2.CAP_PROP_FRAME_WIDTH, 1920)
# cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 1080)
# cap.set(cv2.CAP_PROP_FPS, 30)

cap.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter.fourcc('M', 'J', 'P', 'G'))
cap.set(cv2.CAP_PROP_FPS, 30)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 3840)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 2160)

while(True):
    ret, frame = cap.read()

    print(frame.shape)
    frame = zoom_at(frame, 4)
    frame = cv2.resize(frame, (1920, 1080))
    print(frame.shape)
    # Display the frame
    cv2.imshow('Crop', frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()

