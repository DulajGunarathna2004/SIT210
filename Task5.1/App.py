import tkinter as tk
from tkinter import ttk
import RPi.GPIO as GPIO

# Define GPIO pins for LEDs
LED_PINS = {
    "red": 11,
    "green": 12,
    "blue": 13
}

# GPIO setup
GPIO.setmode(GPIO.BCM)
for pin in LED_PINS.values():
    GPIO.setup(pin, GPIO.OUT)
    GPIO.output(pin, GPIO.LOW)

# GUI setup
root = tk.Tk()
root.geometry("300x200")
root.title("LED Controller")

frame = ttk.Frame(root, padding=10)
frame.pack(expand=True)

def blink_light(color):
    # Turn off all LEDs
    for pin in LED_PINS.values():
        GPIO.output(pin, GPIO.LOW)
    
    # Turn on selected LED
    if color in LED_PINS:
        GPIO.output(LED_PINS[color], GPIO.HIGH)

# Create buttons
ttk.Button(frame, text="Red", width=20, command=lambda: blink_light("red")).pack(pady=5)
ttk.Button(frame, text="Green", width=20, command=lambda: blink_light("green")).pack(pady=5)
ttk.Button(frame, text="Blue", width=20, command=lambda: blink_light("blue")).pack(pady=5)
ttk.Button(frame, text="Exit", width=20, command=root.destroy).pack(pady=10)

# Run the GUI loop
try:
    root.mainloop()
finally:
    GPIO.cleanup()
