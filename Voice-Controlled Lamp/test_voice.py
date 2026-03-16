import speech_recognition as sr

r = sr.Recognizer()

with sr.Microphone() as source:
    print("Adjusting for noise...")
    r.adjust_for_ambient_noise(source, duration=1)
    print("Speak now...")

    try:
        audio = r.listen(source, timeout=10, phrase_time_limit=4)
        print("Audio captured successfully")
    except sr.WaitTimeoutError:
        print("No speech detected")
        raise SystemExit

try:
    text = r.recognize_google(audio, language="en-US")
    print("You said:", text)
except sr.UnknownValueError:
    print("Speech captured but not understood")
except sr.RequestError as e:
    print("API error:", e)