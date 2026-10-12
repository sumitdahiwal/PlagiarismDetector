from kivy.app import App
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.textinput import TextInput
from kivy.uix.button import Button
from kivy.uix.label import Label

class NotesApp(App):
    def build(self):
        self.layout = BoxLayout(orientation='vertical', padding=10, spacing=10)

        self.label = Label(text="My Notes", font_size=24)
        self.text_input = TextInput(hint_text="Write your notes here...", size_hint=(1, 0.8))

        save_button = Button(text="Save Note")
        clear_button = Button(text="Clear Text")

        save_button.bind(on_press=self.save_note)
        clear_button.bind(on_press=self.clear_text)

        self.layout.add_widget(self.label)
        self.layout.add_widget(self.text_input)
        self.layout.add_widget(save_button)
        self.layout.add_widget(clear_button)

        return self.layout

    def save_note(self, instance):
        note_text = self.text_input.text
        if note_text.strip():
            with open("my_notes.txt", "a") as file:
                file.write(note_text + "\n\n")
            self.label.text = "Note Saved!"
        else:
            self.label.text = "Cannot save empty note."

    def clear_text(self, instance):
        self.text_input.text = ""
        self.label.text = "Cleared!"

if __name__ == '__main__':
    NotesApp().run()
