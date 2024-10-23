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

marker_rotations = []
marker_poses = []

def get_marker_rotations():
    global marker_rotations
    with open('../config/marker_rotations_to_odom.yaml', 'r') as file:
        data = yaml.safe_load(file)
    marker_rotations = data['data']
    print("marker_rotations:")
    for idx, values in enumerate(marker_rotations):
        print(f"Набор {idx}: {values}")

def get_marker_poses():
    global marker_poses
    with open('../config/marker_poses_to_odom.yaml', 'r') as file:
        data = yaml.safe_load(file)
    marker_poses = data['data']
    print("\nmarker_poses:")
    for idx, values in enumerate(marker_poses):
        print(f"Набор {idx}: {values}")

def drone_nav():
    aruco_dict = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_4X4_50)
    tvec = np.array([0, 0, 0], dtype=np.float32)
    rvec = np.array([0, 0, 0], dtype=np.float32)

    marker_size = 0.1
    image_size = (640, 480)

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
            r12 = cv2.Rodrigues(rvec[0])    # rvec[0] 0 - индекс маркера, rvec - список данных всех маркеров
            # print("\nr12 = ", r12)

            # вектор переноса маркера относительно камеры
            t12 = tvec[0]
            print("\nt12 = ", t12)

            # получаем вектор переноса камеры относительно маркера
            t21 = -np.dot(r12[0].transpose(), t12.T).reshape(1, 3)
            print("t21 = ", t21)

            # для визуализации процесса рисуем оси СК маркера на изображении
            cv2.aruco.drawAxis(im_with_aruco_board, newcameramtx, dist, rvec[0], tvec[0], 0.05)
        
        else:
            im_with_aruco_board = frame

        cv2.namedWindow("im_with_aruco_board", cv2.WINDOW_NORMAL)
        cv2.resizeWindow("im_with_aruco_board", 640, 480)
        cv2.imshow("im_with_aruco_board", im_with_aruco_board)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()



if __name__ == '__main__':
    try:
        get_marker_rotations()
        get_marker_poses()
        drone_nav()
    except:
        pass