import datetime as dt
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import serial
import serial_utils

ser = serial.Serial(timeout=2)
ser.baudrate = 115200

comPort = serial_utils.findPort()
print(comPort)
ser.port = comPort

ser.open()

# Create figure for plotting
fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)
xs = []
ys = []

# This function is called periodically from FuncAnimation
def animate(i, xs, ys):

    # Add x and y to lists
    xs.append(dt.datetime.now().strftime('%H:%M:%S.%f'))


    line = ser.read_until()
    try:
        value = int(line[0:-1])
        print(value)
    except ValueError:
        print("No value")
        if (not ys):
            value = 0
        else: 
            value = ys[i]
            
    ys.append((value/4065)*3)

    # Limit x and y lists to 20 items
    xs = xs[-20:]
    ys = ys[-20:]



    # Draw x and y lists
    ax.clear()
    ax.plot(xs, ys)

    # Format plot
    plt.xticks(rotation=45, ha='right')
    plt.subplots_adjust(bottom=0.30)
    plt.title('ADC readings from sensor node.')
    plt.ylabel('Voltage')

# Set up plot to call animate() function periodically
ani = animation.FuncAnimation(fig, animate, fargs=(xs, ys), interval=1000)
plt.show()