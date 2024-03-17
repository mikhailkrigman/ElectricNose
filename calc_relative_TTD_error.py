TTD_means = [8237.017,
             10015.021,
             12032.505,
             15174.884,
             20239.047,
             26961.685,
             39088.213,
             46746.840,
             55369.405,
             81910.363]

TTD_Test = [8000,
            10000,
            12000,
            15000,
            20000,
            27000,
            39000,
            47000,
            56000,
            82000]

errors = []

for i in range(10):
    relative_error = (abs(TTD_means[i] - TTD_Test[i]) / TTD_Test[i]) * 100
    errors.append(relative_error)


abw = [2.96, 
 0.15,
 0.27,
 1.17,
 1.20, 
 0.14,
 0.23,
 0.54,
 1.13,
 0.11]

def secToTime(sec: int):
    hours = int(sec / 3600)
    minutes = int((sec - 3600 * hours) / 60)
    seconds = int(sec - 3600 * hours - 60 * minutes)
    print((hours + 15) % 24, minutes, seconds, sep=":")

secToTime(23775)
secToTime(77022)