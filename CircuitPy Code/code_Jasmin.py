# Write your code here :-)
import board
import time
import pwmio
from digitalio import DigitalInOut, Direction
import supervisor
from analogio import AnalogIn
# importing required libraries
# import numpy
# import math
# import math

# Define Pump
pres_0 = AnalogIn(board.A0)
pump_control0 = board.D2
pump_control1 = board.D3

# Define Chamber Pins
chamber1 = board.D14
chamber2 = board.D15
chamber3 = board.D16
chamber4 = board.D17
chamber5 = board.D18
chamber6 = board.D19
chamber7 = board.D20
chamber8 = board.D21
# Initialize Valves
valve1 = DigitalInOut(chamber1)
valve1.direction = Direction.OUTPUT
valve2 = DigitalInOut(chamber2)
valve2.direction = Direction.OUTPUT
valve3 = DigitalInOut(chamber3)
valve3.direction = Direction.OUTPUT
valve4 = DigitalInOut(chamber4)
valve4.direction = Direction.OUTPUT
valve5 = DigitalInOut(chamber5)
valve5.direction = Direction.OUTPUT
valve6 = DigitalInOut(chamber6)
valve6.direction = Direction.OUTPUT
valve7 = DigitalInOut(chamber7)
valve7.direction = Direction.OUTPUT
valve8 = DigitalInOut(chamber8)
valve8.direction = Direction.OUTPUT

# Initialize Pump
valve0c = DigitalInOut(pump_control0)
valve0c.direction = Direction.OUTPUT
valve1c = DigitalInOut(pump_control1)
valve1c.direction = Direction.OUTPUT


# Define timing variables
max_freq = 128
min_period = 1 / max_freq
min_freq = 0.5
max_period = 1 / min_freq
t2 = 0
time_error = 0.05

# Define range of pump speeds
min_pump_speed = 30  # a
max_pump_speed = 100  # b

# Set initial values
min_stroke = 0.0  # min
max_stroke = 12.5  # max
X0_prev = min_stroke
Y0_prev = min_stroke
Z0_prev = min_stroke
X1_prev = min_stroke
Y1_prev = min_stroke
Z1_prev = min_stroke

pwm_pump0 = pwmio.PWMOut(board.D8, frequency=8000, duty_cycle=0)  # Big pump
pwm_pump1 = pwmio.PWMOut(board.D13, frequency=8000, duty_cycle=0)  # Baby pump

def get_voltage(pin):
    return (pin.value * 3.3) / 65535

def get_pressure(V):
    return (V / 5.1 + 0.04) / 0.004

def duty2bits(duty):
    return int(duty * 65535 / 100)

# calculate pump speed depending on commanded stroke
def get_pump_Speed(stroke):
    a = min_pump_speed
    b = max_pump_speed
    minVal = min_stroke
    maxVal = max_stroke
    return ((b-a)*(stroke-minVal) / (maxVal-minVal)) + a

def all_off():
    valve1.value = False
    valve2.value = False
    valve3.value = False
    valve4.value = False
    valve5.value = False
    valve6.value = False
    valve7.value = False
    valve8.value = False
    pwm_pump0.duty_cycle = 0
    pwm_pump1.duty_cycle = 0
    valve0c.value = False
    valve1c.value = False

# ------ Hoxel 0 ------

def Z0_axis_pos(pump_val):
    pwm_pump0.duty_cycle = duty2bits(pump_val)
    valve1.value = True
    valve2.value = True
    valve3.value = True
    valve4.value = True

def Z0_axis_neg(pump_val):
    pwm_pump0.duty_cycle = duty2bits(pump_val)
    valve1.value = True
    valve2.value = True
    valve3.value = True
    valve4.value = True

def X0_axis_pos(pump_val):
    pwm_pump0.duty_cycle = duty2bits(pump_val)
    valve1.value = True
    valve2.value = True
    valve3.value = False
    valve4.value = False

def X0_axis_neg(pump_val):
    pwm_pump0.duty_cycle = duty2bits(pump_val)
    valve1.value = False
    valve2.value = False
    valve3.value = True
    valve4.value = True

def Y0_axis_pos(pump_val):
    pwm_pump0.duty_cycle = duty2bits(pump_val)
    valve1.value = True
    valve2.value = False
    valve3.value = False
    valve4.value = True

def Y0_axis_neg(pump_val):
    pwm_pump0.duty_cycle = duty2bits(pump_val)
    valve1.value = False
    valve2.value = True
    valve3.value = True
    valve4.value = False

def Twist0_pos(pump_val):
    pwm_pump0.duty_cycle = duty2bits(pump_val)
    valve1.value = True
    valve2.value = False
    valve3.value = True
    valve4.value = False

def Twist0_neg(pump_val):
    pwm_pump0.duty_cycle = duty2bits(pump_val)
    valve1.value = False
    valve2.value = True
    valve3.value = False
    valve4.value = True

# ------ Hoxel 1 ------

def Z1_axis_pos(pump_val):
    pwm_pump1.duty_cycle = duty2bits(pump_val)
    valve5.value = True
    valve6.value = True
    valve7.value = True
    valve8.value = True

def Z1_axis_neg(pump_val):
    pwm_pump1.duty_cycle = duty2bits(pump_val)
    valve5.value = True
    valve6.value = True
    valve7.value = True
    valve8.value = True

def X1_axis_pos(pump_val):
    pwm_pump1.duty_cycle = duty2bits(pump_val)
    valve5.value = True
    valve6.value = True
    valve7.value = False
    valve8.value = False

def X1_axis_neg(pump_val):
    pwm_pump1.duty_cycle = duty2bits(pump_val)
    valve5.value = False
    valve6.value = False
    valve7.value = True
    valve8.value = True

def Y1_axis_pos(pump_val):
    pwm_pump1.duty_cycle = duty2bits(pump_val)
    valve5.value = True
    valve6.value = False
    valve7.value = False
    valve8.value = True

def Y1_axis_neg(pump_val):
    pwm_pump1.duty_cycle = duty2bits(pump_val)
    valve5.value = False
    valve6.value = True
    valve7.value = True
    valve8.value = False

def Twist1_pos():
    pwm_pump1.duty_cycle = duty2bits(pump_val)
    valve5.value = True
    valve6.value = False
    valve7.value = True
    valve8.value = False

def Twist1_neg():
    pwm_pump1.duty_cycle = duty2bits(pump_val)
    valve5.value = False
    valve6.value = True
    valve7.value = False
    valve8.value = True

# Turn off Hoxel0
def hoxel0Off():
    pwm_pump0.duty_cycle = duty2bits(min_pump_speed)
    valve1.value = False
    valve2.value = False
    valve3.value = False
    valve4.value = False
    valve0c.value = False

# Turn off Hoxel1
def hoxel1Off():
    pwm_pump1.duty_cycle = duty2bits(min_pump_speed)
    valve5.value = False
    valve6.value = False
    valve7.value = False
    valve8.value = False
    valve1c.value = False

def moveHoxel0(X0, X0_prev, Y0, Y0_prev, Z0, Z0_prev):
    # if extending, use positive axes| if contracting, use negative axes
    if Z0 <= min_stroke:
        hoxel0Off()
    elif Z0 >= max_stroke:
        Z0_axis_pos(get_pump_Speed(max_stroke))
    elif Z0 >= Z0_prev:
        Z0_axis_pos(get_pump_Speed(Z0))
    elif Z0 < Z0_prev:
        Z0_axis_neg(get_pump_Speed(Z0))


def moveHoxel1(X1, X1_prev, Y1, Y1_prev, Z1, Z1_prev):
    # if extending, use positive axes | if contracting, use negative axes
    if Z1 <= min_stroke:
        hoxel1Off()
    elif Z1 >= max_stroke:
        Z1_axis_pos(get_pump_Speed(max_stroke))
    elif Z1 >= Z1_prev:
        Z1_axis_pos(get_pump_Speed(Z1))
    elif Z1 < Z1_prev:
        Z1_axis_neg(get_pump_Speed(Z1))


# Everything off
all_off()

# Set initial frequency
i = min_freq
t = 0.3
t2 = 0.3
valve0c.value = False
valve1c.value = False
time.sleep(1)
# Start vacuum
valve0c.value = True
valve1c.value = True
pwm_pump0.duty_cycle = duty2bits(0)
pwm_pump1.duty_cycle = duty2bits(0)
time.sleep(3)

renderChoice = 1  # 1 for mag(x,y,z) as norm Z0 for as norm 2

# Serial Comms
while True:
    if supervisor.runtime.serial_bytes_available:
        data = input()
        data_list = data.split(' ')
        print(data_list)
        # Set current values for each device direction
        X0 = float(data_list[0])
        Y0 = float(data_list[1])
        Z0 = float(data_list[2])
        X1 = float(data_list[3])
        Y1 = float(data_list[4])
        Z1 = float(data_list[5])

        # Change Z0 and Z1 depending on rendering choice
        #if renderChoice == 1:
            #Z0 =  math.sqrt(X0*X0 + Y0*Y0 + Z0*Z0)
            #Z1 =  math.sqrt(X1*X1 + Y1*Y1 + Z1*Z1)
        #else:
        #    Z0 = Z0
        #    Z1 = Z1

        # Hoxel 0:
        moveHoxel0(X0, X0_prev, Y0, Y0_prev, Z0, Z0_prev)
        # Hoxel 1:
        moveHoxel1(X1, X1_prev, Y1, Y1_prev, Z1, Z1_prev)

        # Set prev values for each device direction
        X0_prev = X0
        Y0_prev = Y0
        Z0_prev = Z0
        X1_prev = X1
        Y1_prev = Y1
        Z1_prev = Z1
        time.sleep(0.01)
