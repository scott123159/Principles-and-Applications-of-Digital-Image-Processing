#Import some packages
import cv2
import numpy as np
import numpy
import sys
import pywt
import tqdm

#Don't create the __pycache__ folder
sys.dont_write_bytecode = True

#Import Qt and GUI related packages
from MainWindow import Ui_MainWindow
from PySide6.QtWidgets import QApplication, QMainWindow, QFileDialog
from PySide6.QtGui import QImage, QPixmap
#Global variables
img = None

step = None
SLIC_m = None
SLIC_ITERATIONS = None
SLIC_height, SLIC_width = None, None
SLIC_labimg = None
SLIC_distances = None
SLIC_clusters = None
SLIC_center_counts = None
SLIC_centers = None
def generate_pixels():
    indnp = numpy.mgrid[0:SLIC_height, 0:SLIC_width].swapaxes(0, 2).swapaxes(0, 1)
    for i in tqdm.tqdm(range(SLIC_ITERATIONS)):
        SLIC_distances = 1 * numpy.ones(img.shape[:2])
        for j in range(SLIC_centers.shape[0]):
            x_low, x_high = int(SLIC_centers[j][3] - step), int(SLIC_centers[j][3] + step)
            y_low, y_high = int(SLIC_centers[j][4] - step), int(SLIC_centers[j][4] + step)

            if x_low <= 0:
                x_low = 0
            # end
            if x_high > SLIC_width:
                x_high = SLIC_width
            # end
            if y_low <= 0:
                y_low = 0
            # end
            if y_high > SLIC_height:
                y_high = SLIC_height
            # end

            cropimg = SLIC_labimg[y_low: y_high, x_low: x_high]
            color_diff = cropimg - SLIC_labimg[int(SLIC_centers[j][4]), int(SLIC_centers[j][3])]
            color_distance = numpy.sqrt(numpy.sum(numpy.square(color_diff), axis=2))

            yy, xx = numpy.ogrid[y_low: y_high, x_low: x_high]
            pixdist = ((yy - SLIC_centers[j][4]) ** 2 + (xx - SLIC_centers[j][3]) ** 2) ** 0.5

            # SLIC_m is "m" in the paper, (m/S)*dxy
            dist = ((color_distance / SLIC_m) ** 2 + (pixdist / step) ** 2) ** 0.5

            distance_crop = SLIC_distances[y_low: y_high, x_low: x_high]
            idx = dist < distance_crop
            distance_crop[idx] = dist[idx]
            SLIC_distances[y_low: y_high, x_low: x_high] = distance_crop
            SLIC_clusters[y_low: y_high, x_low: x_high][idx] = j
        # end

        for k in range(len(SLIC_centers)):
            idx = (SLIC_clusters == k)
            colornp = SLIC_labimg[idx]
            distnp = indnp[idx]
            SLIC_centers[k][0:3] = numpy.sum(colornp, axis=0)
            sumy, sumx = numpy.sum(distnp, axis=0)
            SLIC_centers[k][3:] = sumx, sumy
            SLIC_centers[k] /= numpy.sum(idx)
        # end
    # end


# end

# At the end of the process, some stray labels may remain meaning some pixels
# may end up having the same label as a larger pixel but not be connected to it
# In the SLIC paper, it notes that these cases are rare, however this
# implementation seems to have a lot of strays depending on the inputs given

def create_connectivity():
    label = 0
    adj_label = 0
    lims = int(SLIC_width * SLIC_height / SLIC_centers.shape[0])

    new_clusters = -1 * numpy.ones(img.shape[:2]).astype(numpy.int64)
    elements = []
    for i in range(SLIC_width):
        for j in range(SLIC_height):
            if new_clusters[j, i] == -1:
                elements = []
                elements.append((j, i))
                for dx, dy in [(-1, 0), (0, -1), (1, 0), (0, 1)]:
                    x = elements[0][1] + dx
                    y = elements[0][0] + dy
                    if (x >= 0 and x < SLIC_width and
                            y >= 0 and y < SLIC_height and
                            new_clusters[y, x] >= 0):
                        adj_label = new_clusters[y, x]
                    # end
                # end
            # end

            count = 1
            counter = 0
            while counter < count:
                for dx, dy in [(-1, 0), (0, -1), (1, 0), (0, 1)]:
                    x = elements[counter][1] + dx
                    y = elements[counter][0] + dy

                    if (x >= 0 and x < SLIC_width and y >= 0 and y < SLIC_height):
                        if new_clusters[y, x] == -1 and SLIC_clusters[j, i] == SLIC_clusters[y, x]:
                            elements.append((y, x))
                            new_clusters[y, x] = label
                            count += 1
                        # end
                    # end
                # end

                counter += 1
            # end
            if (count <= lims >> 2):
                for counter in range(count):
                    new_clusters[elements[counter]] = adj_label
                # end

                label -= 1
            # end

            label += 1
        # end
    # end

    SLIC_new_clusters = new_clusters


# end

def display_contours(color):
    is_taken = numpy.zeros(img.shape[:2], numpy.bool)
    contours = []

    for i in range(SLIC_width):
        for j in range(SLIC_height):
            nr_p = 0
            for dx, dy in [(-1, 0), (-1, -1), (0, -1), (1, -1), (1, 0), (1, 1), (0, 1), (-1, 1)]:
                x = i + dx
                y = j + dy
                if x >= 0 and x < SLIC_width and y >= 0 and y < SLIC_height:
                    if is_taken[y, x] == False and SLIC_clusters[j, i] != SLIC_clusters[y, x]:
                        nr_p += 1
                    # end
                # end
            # end

            if nr_p >= 2:
                is_taken[j, i] = True
                contours.append([j, i])
            # end
        # end
    # end
    for i in range(len(contours)):
        img[contours[i][0], contours[i][1]] = color
    # end


# end

def find_local_minimum(center):
    min_grad = 1
    loc_min = center
    for i in range(center[0] - 1, center[0] + 2):
        for j in range(center[1] - 1, center[1] + 2):
            c1 = SLIC_labimg[j + 1, i]
            c2 = SLIC_labimg[j, i + 1]
            c3 = SLIC_labimg[j, i]
            if ((c1[0] - c3[0]) ** 2) ** 0.5 + ((c2[0] - c3[0]) ** 2) ** 0.5 < min_grad:
                min_grad = abs(c1[0] - c3[0]) + abs(c2[0] - c3[0])
                loc_min = [i, j]
            # end
        # end
    # end
    return loc_min


# end

def calculate_centers():
    centers = []
    for i in range(step, SLIC_width - int(step / 2), step):
        for j in range(step, SLIC_height - int(step / 2), step):
            nc = find_local_minimum(center=(i, j))
            color = SLIC_labimg[nc[1], nc[0]]
            center = [color[0], color[1], color[2], nc[0], nc[1]]
            centers.append(center)
        # end
    # end

    return centers

class HW6(QMainWindow):
    def __init__(self):
        super().__init__()
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)
        self.ui.actionExit.triggered.connect(self.close)
        self.ui.actionOpen.triggered.connect(self.read_image_file)
        self.ui.actionTrapezoidal.triggered.connect(self.trapezoidal)
        self.ui.actionWavy.triggered.connect(self.wavy_transformation)
        self.ui.actionCircular.triggered.connect(self.circular_transformation)
        self.ui.actionLoad_Images.triggered.connect(self.fuse_images)
        self.ui.actionSLIC.triggered.connect(self.slic)
        self.setFixedSize(800, 600)
        self.image = None

    def slic(self):
        if self.image is None:
            return
        global img, step, SLIC_width, SLIC_height, SLIC_centers, SLIC_ITERATIONS, SLIC_labimg, SLIC_labimg, SLIC_clusters, SLIC_center_counts, SLIC_centers, SLIC_m
        img = self.image.copy()
        step = int((img.shape[0] * img.shape[1] / int(1000)) ** 0.5)
        SLIC_m = int(40)
        SLIC_ITERATIONS = 4
        SLIC_height, SLIC_width = img.shape[:2]
        SLIC_labimg = cv2.cvtColor(img, cv2.COLOR_BGR2LAB).astype(numpy.float64)
        SLIC_distances = 1 * numpy.ones(img.shape[:2])
        SLIC_clusters = -1 * SLIC_distances
        SLIC_center_counts = numpy.zeros(len(calculate_centers()))
        SLIC_centers = numpy.array(calculate_centers())

        # main
        generate_pixels()
        create_connectivity()
        calculate_centers()
        display_contours([0.0, 0.0, 0.0])
        self.ui.image.setPixmap(QPixmap.fromImage(self.numpy_to_qimage(img)))

    def fuse_images(self):
        file_names, _ = QFileDialog.getOpenFileNames(self,
                                                     'Open Images',
                                                     '',
                                                     'Image Files (*.jpg *.jpeg *.png *.bmp)')
        if len(file_names) < 2:
            return

        """
        Perform image fusion using Discrete Wavelet Transform (DWT) for multiple images.

        :param images: List of input images (as numpy arrays).
        :return: Fused image as a numpy array.
        """
        images = [cv2.imread(path) for path in file_names]
        # Convert all images to grayscale and ensure they have the same size
        images_gray = [cv2.cvtColor(img, cv2.COLOR_BGR2GRAY) for img in images]
        shape = images_gray[0].shape
        assert all(img.shape == shape for img in images_gray), "All input images must have the same dimensions"

        # Perform 2D DWT on all images and collect coefficients
        coeffs_list = [pywt.dwt2(img, 'haar') for img in images_gray]

        # Initialize fused coefficients
        LL_fused = np.zeros_like(coeffs_list[0][0])
        LH_fused = np.zeros_like(coeffs_list[0][1][0])
        HL_fused = np.zeros_like(coeffs_list[0][1][1])
        HH_fused = np.zeros_like(coeffs_list[0][1][2])

        # Fusion rules: Average for LL and Maximum for LH, HL, HH
        for coeffs in coeffs_list:
            LL, (LH, HL, HH) = coeffs
            LL_fused += LL
            LH_fused = np.maximum(LH_fused, LH)
            HL_fused = np.maximum(HL_fused, HL)
            HH_fused = np.maximum(HH_fused, HH)

        # Average LL_fused by the number of images
        LL_fused /= len(images_gray)

        # Reconstruct the fused image using inverse DWT
        coeffs_fused = (LL_fused, (LH_fused, HL_fused, HH_fused))
        fused_image = pywt.idwt2(coeffs_fused, 'haar')

        # Normalize and convert to uint8
        fused_image = np.clip(fused_image, 0, 255).astype(np.uint8)
        self.setFixedSize(fused_image.shape[1], fused_image.shape[0])
        self.ui.image.setPixmap(QPixmap.fromImage(self.numpy_to_qimage(fused_image)))


    def read_image_file(self):
        file_name, _ = QFileDialog.getOpenFileName(self,
                                                   'Open Image',
                                                   '',
                                                   'Image Files (*.jpg *.jpeg *.png *.bmp)')

        #If no image was selected, then return it
        if not file_name:
            return
        self.image = cv2.cvtColor(cv2.imread(file_name), cv2.COLOR_BGR2RGB)
        self.setFixedSize(self.image.shape[1], self.image.shape[0])
        self.ui.image.setPixmap(QPixmap.fromImage(self.numpy_to_qimage(self.image)))

    def circular_transformation(self):
        """
        將影像轉換為圓形變換效果 (Circular Transformation)
        :param image: 輸入的灰階影像 (NumPy 陣列)
        :return: 圓形變形後的影像
        """
        # 獲取影像的中心與半徑
        height, width = self.image.shape[:2]
        center = (width // 2, height // 2)
        max_radius = min(center[0], center[1])  # 最大半徑為影像最小邊的一半

        # 使用 OpenCV 的 linearPolar 進行極坐標變換
        circular_image = cv2.linearPolar(
            self.image,
            center=center,
            maxRadius=max_radius,
            flags=cv2.WARP_FILL_OUTLIERS + cv2.WARP_INVERSE_MAP  # 反向映射
        )

        # 將結果旋轉 90 度，使其直立
        self.image = cv2.rotate(circular_image, cv2.ROTATE_90_CLOCKWISE)
        self.ui.image.setPixmap(QPixmap.fromImage(self.numpy_to_qimage(self.image)))

    def wavy_transformation(self):
        if self.image is None:
            return
        """
        對影像進行波浪變形
        :param image: 輸入的灰階影像 (NumPy 陣列)
        :param amplitude: 波浪的振幅 (越大波動越明顯)
        :param frequency: 波浪的頻率 (越大波浪越密集)
        :return: 波浪變形後的影像
        """
        amplitude = 15
        frequency = 0.01
        # 獲取影像尺寸
        height, width = self.image.shape[:2]

        # 建立網格座標
        x, y = np.meshgrid(np.arange(width), np.arange(height))

        # 計算波浪偏移 (x 和 y 方向分別變形)
        x_offset = amplitude * np.sin(2 * np.pi * frequency * y)
        y_offset = amplitude * np.sin(2 * np.pi * frequency * x)

        # 新的座標
        x_new = (x + x_offset).astype(np.float32)
        y_new = (y + y_offset).astype(np.float32)

        # 使用 OpenCV 的 remap 函數進行插值
        self.image = cv2.remap(self.image, x_new, y_new, interpolation=cv2.INTER_LINEAR,
                                      borderMode=cv2.BORDER_CONSTANT, borderValue=0)
        self.ui.image.setPixmap(QPixmap.fromImage(self.numpy_to_qimage(self.image)))

    def trapezoidal(self):
        if self.image is None:
            return
        height, width = self.image.shape[:2]
        # 定義原始圖像的四個角點
        original_points = np.float32([
            [0, 0],
            [width - 1, 0],
            [width - 1, height - 1],
            [0, height - 1]
        ])

        # 定義目標影像的四個角點 (設計為梯形)
        transformed_points = np.float32([
            [0, 0],
            [width - 1, 0],
            [width * 0.8, height * 0.7],
            [width * 0.2, height * 0.7]
        ])

        # 計算透視變換矩陣
        matrix = cv2.getPerspectiveTransform(original_points, transformed_points)

        # 進行透視變換
        self.image = cv2.warpPerspective(self.image, matrix, (width, height))
        self.ui.image.setPixmap(QPixmap.fromImage(self.numpy_to_qimage(self.image)))

    def numpy_to_qimage(self, np_array):
        """
        將 NumPy 陣列轉換為 QImage
        :param np_array: NumPy 陣列 (uint8)
        :return: QImage
        """
        # 灰階影像 (單通道)
        if len(np_array.shape) == 2:
            height, width = np_array.shape
            bytes_per_line = width
            return QImage(np_array.data, width, height, bytes_per_line, QImage.Format_Grayscale8)

        # 彩色影像 (RGB)
        elif len(np_array.shape) == 3 and np_array.shape[2] == 3:
            height, width, channels = np_array.shape
            bytes_per_line = width * channels
            return QImage(np_array.data, width, height, bytes_per_line, QImage.Format_RGB888)

        else:
            raise ValueError("Unsupported NumPy array shape or data type")

if __name__ == '__main__':
    app = QApplication([])
    window = HW6()
    window.show()
    app.exec()