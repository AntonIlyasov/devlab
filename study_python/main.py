sleep_time_hours = int(input())
sleep_time_min   = int(input())
sleep_min_input = int(input())

sleep_hours = int(sleep_min_input / 60)
sleep_min = int(sleep_min_input % 60)
print(sleep_hours)
print(sleep_min)