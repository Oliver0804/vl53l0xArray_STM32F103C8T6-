import pygame
import math
import serial  # 確保已安裝pyserial包

# 初始化串列連接（根據需要調整'/dev/cu.usbserial-A50285BI'和波特率）
ser = serial.Serial('/dev/cu.usbserial-A50285BI', 230400, timeout=1)

# 初始化 Pygame 和顯示視窗
pygame.init()
width, height = 400, 400
screen = pygame.display.set_mode((width, height))
pygame.display.set_caption('雷達顯示')
clock = pygame.time.Clock()

# 雷達設置
center = (width // 2, height // 2)
radius = 150
colors = {
    'background': (0, 0, 0),
    'grid': (0, 255, 0),
    'sector': (200, 200, 0)  # 不同綠色的區塊
}

# 範例距離數據（將從串行更新）
distances = [1600, 1505, 1546, 1594, 1572, 1544, 1480, 1564]  # 初始占位符值

def draw_radar(screen, center, radius, distances):
    # 清除屏幕
    screen.fill(colors['background'])
    
    # 绘制雷达圈和线
    for i in range(1, 6):
        pygame.draw.circle(screen, colors['grid'], center, radius * i / 5, 1)
    for i in range(8):
        angle = math.radians(45 * i - 90)  # 调整-90度从顶部开始
        x = center[0] + radius * math.cos(angle)
        y = center[1] + radius * math.sin(angle)
        pygame.draw.line(screen, colors['grid'], center, (x, y), 1)
    
    # 绘制距离扇形
    for i, distance in enumerate(distances):
        norm_distance = min(distance / 2000, 1)  # 正常化距离以适应雷达
        # 根据距离设置不同的颜色
        if norm_distance < 1/3:
            color = (255, 0, 0)  # 红色
        elif norm_distance < 2/3:
            color = (255, 255, 0)  # 黄色
        else:
            color = (0, 255, 0)  # 绿色
        center_angle = math.radians(45 * i - 90 + 22.5)  # 扇形中心点，偏移量为半个间隔
        start_angle = center_angle - math.radians(12.5)  # 扇形开始角度
        end_angle = center_angle + math.radians(12.5)  # 扇形结束角度
        # 定义扇形的三个点：中心点、起始点、结束点
        start_point = (center[0] + norm_distance * radius * math.cos(start_angle), center[1] + norm_distance * radius * math.sin(start_angle))
        end_point = (center[0] + norm_distance * radius * math.cos(end_angle), center[1] + norm_distance * radius * math.sin(end_angle))
        # 使用 pygame.draw.polygon 绘制填充的扇形
        pygame.draw.polygon(screen, color, [center, start_point, end_point])



# 從串行端口讀取一個封包的函數
def read_packet(ser):
    # 等待封包的開始
    while True:
        if ser.read(1) == b'\xAA':  # 封包頭的第一個位元組
            if ser.read(1) == b'\x55':  # 封包頭的第二個位元組
                break  # 檢測到封包開始

    # 讀取封包的其餘部分（根據您的數據格式進行調整）
    # 示例：讀取8個距離值，每個2字節長
    data = ser.read(16)  # 根據您的數據格式調整數字
    ###print(data)
    # 將位元組轉換為距離值（假設為16位整數，需要調整）
    distances = [int.from_bytes(data[i:i+2], 'big') for i in range(0, len(data), 2)]  # 將'little'改為'big'來對調MSB和LSB
    ###print(distances)
    return distances

# 主循環
running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    # 從串行更新距離
    if ser.in_waiting >= 18:  # 檢查是否有足夠的數據進行一次包（根據大小調整）
        distances = read_packet(ser)

    # 繪製雷達（假設您已經定義了draw_radar函數）
    draw_radar(screen, center, radius, distances)
    pygame.display.flip()
    clock.tick(60)  # 限制為每秒60幀

ser.close()  # 關閉串行連接
pygame.quit()