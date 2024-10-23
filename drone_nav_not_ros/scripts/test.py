import yaml

# Считываем данные из YAML файла
with open('../config/marker_poses_to_odom.yaml', 'r') as file:
    data = yaml.safe_load(file)

# Извлекаем данные
list_of_values = data['data']

# Выводим загруженные данные
for idx, values in enumerate(list_of_values):
    print(f"Набор {idx}: {values}")
