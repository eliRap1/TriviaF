# TriviaF – Multiplayer Trivia Game

A self‑contained trivia game project featuring a real-time **C++ server** and a **C# WPF client**. Built as part of my ongoing learning journey in systems programming, networking, and UI development.

---

## 🚀 Project Overview
Players can register, log in, create or join rooms, and answer trivia questions in real time. The client communicates with the server over TCP using a custom packet protocol.

---

## 🧠 Technologies
- **Client:** C# (.NET, WPF, XAML)
- **Server:** C++ (sockets, JSON)
- **Protocol:** TCP with 1-byte message type + length-prefix + serialized payload (JSON/binary)
- **Tools:** Visual Studio, Git

---

## 📂 Project Structure
client/ – C# WPF application files
├── App.xaml / App.xaml.cs
├── MainWindow.xaml / .cs
├── Login.xaml / .cs
├── JoinRoom.xaml / .cs
├── CreateRoom.xaml / .cs
├── Communicator.cs – handles socket I/O
├── Deserializer.cs – parses incoming messages
└── TriviaClient.csproj

trivia/ – C++ server implementation
├── main.cpp
├── Server.cpp / .h
├── ClientHandler.cpp / .h
└── (additional modules…)

.gitignore
README.md


---

## 🔧 Features
- Player signup & login
- Room creation and joining
- Real-time question/answer
- Score tracking per player
- Custom TCP-based communication
- GUI interface using WPF

---

## ⚙️ How to Run

### Client (C#)
1. Open `TriviaClient.sln` in Visual Studio
2. Build and run the application
3. Use the login screen, then create or join rooms

### Server (C++)
1. Navigate to the `trivia/` folder
2. Compile using:
   ```bash
   g++ main.cpp Server.cpp ClientHandler.cpp -o server
