import serial
import speech_recognition as sr
import time


ser = serial.Serial('COM6', 9600, timeout=1)
time.sleep(2)

recognizer = sr.Recognizer()
recognizer.energy_threshold = 300
recognizer.dynamic_energy_threshold = True
recognizer.pause_threshold = 0.8


MIC_INDEX = 1  

print("Voice lamp control started")

try:
    while True:
        with sr.Microphone() as source:
            print("Speak command...")

            recognizer.adjust_for_ambient_noise(source, duration=1)

            try:
                audio = recognizer.listen(source, timeout=10, phrase_time_limit=4)
            except sr.WaitTimeoutError:
                print("No speech detected. Try speaking louder or closer to the microphone.")
                continue

        try:
            text = recognizer.recognize_google(audio, language="en-US")
            text = text.lower().strip()

            print("You said:", text)

            if "turn on" in text or text == "on":
                ser.write(b'1')
                print("Lamp ON")

            elif "turn off" in text or text == "off":
                ser.write(b'0')
                print("Lamp OFF")

            else:
                print("Command not recognized")

        except sr.UnknownValueError:
            print("Could not understand audio")

        except sr.RequestError as e:
            print("API error:", e)

except KeyboardInterrupt:
    print("\nProgram stopped")

finally:
    ser.close()
    print("Serial closed")