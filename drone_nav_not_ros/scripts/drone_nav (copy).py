import cv2
import numpy as np
import yaml

# загрузка внутренних параметров камеры
with open('../config/calibration.yaml') as f:
    loadeddict = yaml.safe_load(f)
mtx = loadeddict.get('camera_matrix')
dist = loadeddict.get('dist_coeff')
mtx = np.array(mtx)
dist = np.array(dist)


def drone_nav():
    aruco_dict = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_5X5_250)
    tvec = np.array([0, 0, 0], dtype=np.float32)
    rvec = np.array([0, 0, 0], dtype=np.float32)

    marker_size = 0.1
    image_size = (800, 600)

    cap = cv2.VideoCapture(0, cv2.CAP_V4L2)
    parameters = cv2.aruco.DetectorParameters_create()
    print("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")

    while True:
        ret, frame = cap.read()
        h,  w = frame.shape[:2]
        newcameramtx, roi=cv2.getOptimalNewCameraMatrix(mtx,dist,(w,h),1,(w,h))

        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

        corners, ids, rejectedImgPoints = cv2.aruco.detectMarkers(gray, aruco_dict, parameters=parameters)

        # если хоть какой то маркер обнаружен
        # то есть в списке ids есть номер хотя бы одного маркера
        if np.all(ids != None):
            im_with_aruco_board = cv2.aruco.drawDetectedMarkers(frame, corners, ids, (0, 255, 0))

            # оценка положения для одного маркера
            rvec, tvec, _objPoints = cv2.aruco.estimatePoseSingleMarkers(corners, marker_size, newcameramtx, dist)

            # получаем матрицу поворота из полученного вектора вращения
            mat = cv2.Rodrigues(rvec[0])

            # выводим в консоль положение маркера в СК камеры
            print("tvec = ", tvec[0])

            # выводим в консоль положение камеры в СК маркера
            #  для этого умножаем транспонированную матрицу поворота с отриц. знаком
            #  на вектор положения маркера в СК камеры
            print("tvec_rot = ", -mat[0].transpose()*tvec[0])

            # для визуализации процесса рисуем оси СК маркера на изображении
            im_with_aruco_board = cv2.aruco.drawAxis(im_with_aruco_board, mtx, dist, rvec[0], tvec[0], 0.5)
        
        else:
            im_with_aruco_board = frame

        cv2.namedWindow("im_with_aruco_board", cv2.WINDOW_NORMAL)
        cv2.resizeWindow("im_with_aruco_board", 800, 600)
        cv2.imshow("im_with_aruco_board", im_with_aruco_board)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()



if __name__ == '__main__':
    try:
        drone_nav()
    except:
        pass