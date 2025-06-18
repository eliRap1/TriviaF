using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;

namespace client
{
    /// <summary>
    /// Interaction logic for JoinRoom.xaml
    /// </summary>
    public partial class JoinRoom : Page
    {
        private DispatcherTimer updateTimer;
        public JoinRoom()
        {
            InitializeComponent();
            byte[] request = Serializer.serializeGetRoomsRequest();
            byte[] response = MainWindow.communicator.sendAndReceive(request);

            List<RoomInfo> rooms = Deserializer.DeserializeGetRoomsResponse(response);

            foreach (var room in rooms)
            {
                AddRoomCard(room);
            }
            updateTimer = new DispatcherTimer();
            updateTimer.Interval = TimeSpan.FromSeconds(3);
            updateTimer.Tick += Refresh;
            updateTimer.Start();

        }

        private void Refresh(object sender, EventArgs e)
        {
            RoomsPanel.Children.Clear();
            byte[] request = Serializer.serializeGetRoomsRequest();
            byte[] response = MainWindow.communicator.sendAndReceive(request);

            List<RoomInfo> rooms = Deserializer.DeserializeGetRoomsResponse(response);

            foreach (var room in rooms)
            {
                AddRoomCard(room);
            }
        }
        private void AddRoomCard(RoomInfo room)
        {
            // create card container
            int currPlayers = Room.CheckPlayersList(room.id);
            Border roomCard = new Border
            {
                Background = Brushes.White,
                CornerRadius = new CornerRadius(12),
                Margin = new Thickness(0, 0, 0, 15),
                Padding = new Thickness(16),
                Width = 700,
                HorizontalAlignment = HorizontalAlignment.Center,
            };

            DockPanel dock = new DockPanel();

            // left side text
            StackPanel textPanel = new StackPanel();
            TextBlock nameText = new TextBlock
            {
                Text = $"Room Name: {room.name}",
                FontSize = 20,
                FontWeight = FontWeights.Bold,
                Foreground = Brushes.Navy
            };
            TextBlock idText = new TextBlock
            {
                Text = $"Room ID: {room.id}",
                FontSize = 14,
                Foreground = Brushes.Gray
            };
            TextBlock playersText = new TextBlock
            {
                Text = $"Players: {currPlayers} / {room.maxPlayers}",
                FontSize = 14,
                Foreground = Brushes.Gray
            };

            textPanel.Children.Add(nameText);
            textPanel.Children.Add(idText);
            textPanel.Children.Add(playersText);
            DockPanel.SetDock(textPanel, Dock.Left);

            // right side join 
            Button joinButton = new Button
            {
                Content = "Join",
                Width = 80,
                Height = 30,
                Background = Brushes.DeepSkyBlue,
                Foreground = Brushes.White,
                FontWeight = FontWeights.Bold,
                BorderThickness = new Thickness(0),
                Cursor = Cursors.Hand,
                Margin = new Thickness(10, 0, 0, 0)
            };
            joinButton.Click += (sender, eventArgs) =>
            {
                if(currPlayers >= room.maxPlayers)
                {
                    MessageBox.Show("Room is full!");
                    return;
                }
                MessageBox.Show($"Joining room: {room.name}");
                byte[] request = Serializer.SearalizeJoinRoomRequest(room.id);
                byte[] response = MainWindow.communicator.sendAndReceive(request);
                if(Deserializer.extractStatus(response) == 1)
                {
                    JoinRoomFrame.Navigate(new Room(room));
                }
                else
                {
                    MessageBox.Show("Error joining room");
                }
            };
            DockPanel.SetDock(joinButton, Dock.Right);

            dock.Children.Add(textPanel);
            dock.Children.Add(joinButton);
            roomCard.Child = dock;

            RoomsPanel.Children.Add(roomCard);
        }

        private void BackButton_Click(object sender, RoutedEventArgs e)
        {
            JoinRoomFrame.Navigate(new Trivia());
        }
    }
}
