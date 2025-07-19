import uproot
import numpy as np
from tqdm import tqdm  # 显示进度条

def read_data(filename):
    electron_edep = []
    cluster_count = []
    gamma_detected = []

    with open(filename, 'r') as f:
        for line in tqdm(f, total=19890093):  # total lines
            parts = line.strip().split()
            if len(parts) == 3:  # 只处理完整的三列数据
                try:
                    electron_edep.append(float(parts[0]))
                    cluster_count.append(int(parts[1]))
                    gamma_detected.append(int(parts[2]))
                except ValueError:
                    continue  # 跳过格式错误的行

    return {
        'ElectronEdep': np.array(electron_edep, dtype='f8'),
        'ClusterCount': np.array(cluster_count, dtype='i4'),
        'GammaDetected': np.array(gamma_detected, dtype='i4')
    }

print("正在读取数据...")
data = read_data('event_data19890093.txt')

print("正在写入ROOT文件...")
with uproot.recreate("event_data19890093.root") as f:
    f["tree"] = data

print(f"转换完成！共处理了{len(data['ElectronEdep'])}行有效数据。")
