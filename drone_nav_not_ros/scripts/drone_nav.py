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

camera_rotations = []
camera_poses = []

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
    marker_poses = [[[coord / 1000.0 for coord in pose[0]]]
                                for pose in marker_poses]
    print("\nmarker_poses:")
    for idx, values in enumerate(marker_poses):
        print(f"Набор {idx}: {values}")

def drone_nav():
    global camera_rotations
    global camera_poses

    aruco_dict = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_4X4_50)
    tvec = np.array([0, 0, 0], dtype=np.float32)
    rvec = np.array([0, 0, 0], dtype=np.float32)

    marker_size = 0.03
    image_size = (640, 480)

    cap = cv2.VideoCapture(0, cv2.CAP_V4L2)
    parameters = cv2.aruco.DetectorParameters_create()
    print("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")

    while True:
        ret, frame = cap.read()
        h,  w = frame.shape[:2]
        # newcameramtx, roi=cv2.getOptimalNewCameraMatrix(mtx,dist,(w,h),1,(w,h))

        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

        corners, ids, rejectedImgPoints = cv2.aruco.detectMarkers(gray, aruco_dict, parameters=parameters)

        # если хоть какой то маркер обнаружен
        # то есть в списке ids есть номер хотя бы одного маркера
        if np.all(ids != None):
            im_with_aruco_board = cv2.aruco.drawDetectedMarkers(frame, corners, ids, (0, 255, 0))

            # оценка положения для одного маркера
            rvec, tvec, _objPoints = cv2.aruco.estimatePoseSingleMarkers(corners, marker_size, mtx, dist)

            for i in range(0, len(ids)):
                # print()
                # print("\ni = ", i)

                # для визуализации процесса рисуем оси СК маркера на изображении
                cv2.aruco.drawAxis(im_with_aruco_board, mtx, dist, rvec[i], tvec[i], 0.01)

                # получаем матрицу поворота из полученного вектора вращения
                r12 = cv2.Rodrigues(rvec[i])    # rvec[i] i - индекс маркера, rvec - список данных всех маркеров
                # print("r12[0] = ", r12[0])

                # вектор переноса маркера относительно камеры
                t12 = tvec[i]
                # print("t12 = ", t12)

                # print("marker_rotations = ", marker_rotations)
                # print("marker_poses = ", marker_poses)

                # получаем матрицу поворота камеры относительно маркера
                r21 = r12[0].transpose()

                # print("r21 = ", r21)

                # получаем вектор переноса камеры относительно маркера
                t21 = -np.dot(r21, t12.T).reshape(1, 3)
                # print("t21 = ", t21)

                # print("ids[i][0] = ", ids[i][0])

                # получаем camera_rotations, camera_poses
                t02 = np.array(marker_poses[ids[i][0]])
                r02 = cv2.Rodrigues(np.array(marker_rotations[ids[i][0]]))
                # print("t02 = ", t02)
                # print("r02[0] = ", r02[0])

                # Вычисление вектора переноса камеры в СК мира
                t01 = t02 + np.dot(r02[0], t21.T).reshape(1, 3)
                # print("t01 = ", t01)

                # Вычисление вектора поворота камеры в СК мира
                r01 = np.dot(r02[0], r21)
                # print("r01 = ", r01)

                camera_rotations.append(r01)
                camera_poses.append(t01)
            
            camera_rotations_array = np.array(camera_rotations)
            camera_poses_array = np.array(camera_poses)
            # print("camera_rotations_array:\n", camera_rotations_array)
            # print("camera_poses_array:\n", camera_poses_array)

            average_camera_rotation = np.mean(camera_rotations_array, axis=0)
            average_camera_pose = np.mean(camera_poses_array, axis=0)

            # print("average_camera_rotation:\n", average_camera_rotation)
            print("average_camera_pose:\n", average_camera_pose)
            print()

            camera_rotations.clear()
            camera_poses.clear()
        
        else:
            im_with_aruco_board = frame
        
        cv2.line(im_with_aruco_board, (0, 240), (640, 240), (0, 0, 255), 1)
        cv2.line(im_with_aruco_board, (320, 0), (320, 480), (0, 0, 255), 1)
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