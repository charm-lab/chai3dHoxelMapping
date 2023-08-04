# Jasmin's ~Official~ 3-DoF Hoxel Control Code -- WHC Demo Version 2
# Write your code here :-)
import board
import time
import pwmio
from digitalio import DigitalInOut, Direction
import supervisor
from analogio import AnalogIn


# Define Presure Input
pres_1 = AnalogIn(board.A8)
pres_2 = AnalogIn(board.A9)
pres_3 = AnalogIn(board.A10)
pres_4 = AnalogIn(board.A11)
pres_5 = AnalogIn(board.A12)
pres_6 = AnalogIn(board.A13)
pres_7 = AnalogIn(board.A14)
pres_8 = AnalogIn(board.A15)

ls_en = board.D40

# Define Solenoid Pins
sol1 = board.A0
sol2 = board.A1
sol3 = board.A2
sol4 = board.A3
sol5 = board.A4
sol6 = board.A5
sol7 = board.A6
sol8 = board.A7

enable_LS = DigitalInOut(ls_en)
enable_LS.direction = Direction.OUTPUT

# Initialize Valves
valve1 = DigitalInOut(sol1)
valve1.direction = Direction.OUTPUT
valve2 = DigitalInOut(sol2)
valve2.direction = Direction.OUTPUT
valve3 = DigitalInOut(sol3)
valve3.direction = Direction.OUTPUT
valve4 = DigitalInOut(sol4)
valve4.direction = Direction.OUTPUT
valve5 = DigitalInOut(sol5)
valve5.direction = Direction.OUTPUT
valve6 = DigitalInOut(sol6)
valve6.direction = Direction.OUTPUT
valve7 = DigitalInOut(sol7)
valve7.direction = Direction.OUTPUT
valve8 = DigitalInOut(sol8)
valve8.direction = Direction.OUTPUT

# Initialize Pumps
pwm_pump1 = pwmio.PWMOut(board.D9, frequency=8000, duty_cycle=0)
pwm_pump2 = pwmio.PWMOut(board.D16, frequency=8000, duty_cycle=0)
pwm_pump3 = pwmio.PWMOut(board.D7, frequency=8000, duty_cycle=0)
pwm_pump4 = pwmio.PWMOut(board.D6, frequency=8000, duty_cycle=0)
pwm_pump5 = pwmio.PWMOut(board.D5, frequency=8000, duty_cycle=0)
pwm_pump6 = pwmio.PWMOut(board.D4, frequency=8000, duty_cycle=0)
pwm_pump7 = pwmio.PWMOut(board.D3, frequency=8000, duty_cycle=0)
pwm_pump8 = pwmio.PWMOut(board.D2, frequency=8000, duty_cycle=0)

# Define timing variables
max_freq = 128
min_period = 1 / max_freq
min_freq = 0.5
max_period = 1 / min_freq
t2 = 0
time_error = 0.05

# Define range of pump speeds
min_pump_speed = 10  # a
max_pump_speed = 100  # b

# Set initial values
min_force = 0.1  # min
max_force = 150.0  # max
X0_prev = min_force
Y0_prev = min_force
Z0_prev = min_force
magF0_prev = 0.0
X1_prev = min_force
Y1_prev = min_force
Z1_prev = min_force
magF1_prev = 0.0


def get_voltage(pin):
    V_ref = 3.3
    resolution = 65535
    return (pin.value * V_ref) / (resolution)

# Outputs pressure in PSI, input is 3.3V scale voltage measured at pressure ADC
def get_pressure(V):
    V_sup = 5
    R1 = 19.6
    R2 = 10
    V_out = V * ((R1 + R2) / R1)

    # P in PSI
    P_min = 0
    P_max = 15

    return 6.89476 * ((V_out - (0.1 * V_sup)) * (P_max - P_min) / (0.8 * V_sup) + P_min)

def duty2bits(duty):
    return int(duty * 65535 / 100)

def pumps_on(d):
    pwm_pump1.duty_cycle = d
    pwm_pump2.duty_cycle = d
    pwm_pump3.duty_cycle = d
    pwm_pump4.duty_cycle = d
    pwm_pump5.duty_cycle = d
    pwm_pump6.duty_cycle = d
    pwm_pump7.duty_cycle = d
    pwm_pump8.duty_cycle = d

# Everything is off and air id released from the line
def exhaustHoxel0():
    pwm_pump1.duty_cycle = 0
    pwm_pump2.duty_cycle = 0
    pwm_pump3.duty_cycle = 0
    pwm_pump4.duty_cycle = 0

def exhaustHoxel1():
    pwm_pump5.duty_cycle = 0
    pwm_pump6.duty_cycle = 0
    pwm_pump7.duty_cycle = 0
    pwm_pump8.duty_cycle = 0

# calculate pump speed depending on commanded force
def get_pump_Speed(force):
    # a = min_pump_speed
    # b = max_pump_speed
    # minVal = 0  # min_force
    # maxVal = 20  # max_force
    # return ((b-a)*(force-minVal) / (maxVal-minVal)) + a

    # pump_speed cannot exceed 100
    force = 15 * abs(force)
    if force >= max_pump_speed:
        pump_Speed = max_pump_speed
    else:
        pump_Speed = force

    return pump_Speed

# ------ Hoxel Directions ------

def xPos(d):
    pwm_pump1.duty_cycle = 0
    pwm_pump2.duty_cycle = d
    pwm_pump3.duty_cycle = d
    pwm_pump4.duty_cycle = 0
    pwm_pump5.duty_cycle = 0
    pwm_pump6.duty_cycle = d
    pwm_pump7.duty_cycle = d
    pwm_pump8.duty_cycle = 0

def xNeg(d):
    pwm_pump1.duty_cycle = d
    pwm_pump2.duty_cycle = 0
    pwm_pump3.duty_cycle = 0
    pwm_pump4.duty_cycle = d
    pwm_pump5.duty_cycle = d
    pwm_pump6.duty_cycle = 0
    pwm_pump7.duty_cycle = 0
    pwm_pump8.duty_cycle = d

def yPos(d):
    pwm_pump1.duty_cycle = 0
    pwm_pump2.duty_cycle = 0
    pwm_pump3.duty_cycle = d
    pwm_pump4.duty_cycle = d
    pwm_pump5.duty_cycle = 0
    pwm_pump6.duty_cycle = 0
    pwm_pump7.duty_cycle = d
    pwm_pump8.duty_cycle = d

def yNeg(d):
    pwm_pump1.duty_cycle = d
    pwm_pump2.duty_cycle = d
    pwm_pump3.duty_cycle = 0
    pwm_pump4.duty_cycle = 0
    pwm_pump5.duty_cycle = d
    pwm_pump6.duty_cycle = d
    pwm_pump7.duty_cycle = 0
    pwm_pump8.duty_cycle = 0

def zSqueeze(d):
    pwm_pump1.duty_cycle = d
    pwm_pump2.duty_cycle = d
    pwm_pump3.duty_cycle = d
    pwm_pump4.duty_cycle = d
    pwm_pump5.duty_cycle = d
    pwm_pump6.duty_cycle = d
    pwm_pump7.duty_cycle = d
    pwm_pump8.duty_cycle = d

# Turn off Hoxel0
def hoxel0Off():
    pwm_pump1.duty_cycle = 0
    pwm_pump2.duty_cycle = 0
    pwm_pump3.duty_cycle = 0
    pwm_pump4.duty_cycle = 0

# Turn off Hoxel1
def hoxel1Off():
    pwm_pump5.duty_cycle = 0
    pwm_pump6.duty_cycle = 0
    pwm_pump7.duty_cycle = 0
    pwm_pump8.duty_cycle = 0


enable_LS.value = False
enable_LS.value = True
# Everything off
exhaustHoxel0()
exhaustHoxel1()

# Set initial frequency
i = min_freq
t = 0.3
t2 = 0.3
# valve0c.value = False
# valve1c.value = False
time.sleep(3)
# Start vacuum
# valve0c.value = True
# valve1c.value = True

time.sleep(3)
t_sleep = 0.5
duty = 0 # Initial value: 50000

# print("Start")

# Serial Comms
while True:
    # print("Working on")
    # Move Hoxel 0 and 1 in X
    xNeg(duty)
    time.sleep(t_sleep)
    xNeg(0)
    time.sleep(t_sleep)
    xPos(duty)
    time.sleep(t_sleep)
    xPos(0)
    time.sleep(t_sleep)

    # Move Hoxel 0 and 1 in Y
    yNeg(duty)
    time.sleep(t_sleep)
    yNeg(0)
    time.sleep(t_sleep)
    yPos(duty)
    time.sleep(t_sleep)
    yPos(0)
    time.sleep(t_sleep)


    # Move Hoxel 0 and 1 in Z for squeeze
    zSqueeze(duty)
    time.sleep(t_sleep)
    zSqueeze(0)
    time.sleep(t_sleep)

