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
using System.Threading;
using System.Windows.Threading;

namespace client
{
    /// <summary>
    /// Interaction logic for Room.xaml
    /// </summary>
    public partial class Room : Page
    {
        enum RoomStatus { Waiting, Active, Closed };
        private RoomInfo room1;
        private DispatcherTimer updatePlayersTimer;
        private bool gameStarted = false;
        private bool adminLeft = false;
        public Room(RoomInfo room, string admin = "")
        {
            InitializeComponent();
            this.room1 = room;
            roomName.Text = room.name;
            roomId.Text = room.id.ToString();
            questionAmount.Text = room.numOfQuestionsInGame.ToString();
            timePerQuestion.Text = $"{room.timePerQuestion} sec";
            CheckPlayers(null, EventArgs.Empty);
            if (admin != "")
            {
                adminName.Text = admin;
                start.Visibility = Visibility.Visible;
            }
                updatePlayersTimer = new DispatcherTimer(); // POOLING THREAD /credit to my mekif z teacher
            updatePlayersTimer.Interval = TimeSpan.FromSeconds(3);
            updatePlayersTimer.Tick += CheckPlayers;
            updatePlayersTimer.Start();

        }
        private void CheckPlayers(object sender, EventArgs e)
        {
            byte[] request;
            byte[] response;
            request = Serializer.SerializeSimpleRequest(Serializer.GET_ROOM_STATE_CODE);
            response = MainWindow.communicator.sendAndReceive(request);
            GetRoomStateResponse state = Deserializer.DesirializeGetRoomStateResponse(response);
            List<string> players = state.players;
            playersList.ItemsSource = players;
            try
            {
                adminName.Text = players[0];
                if (Login.user == players[0])
                {
                    start.Visibility = Visibility.Visible;
                }
            }
            catch
            {
            }
            if(state.hasGameBegun)
            {
                MessageBox.Show("Game has started!");
                triviaF.Navigate(new Game(this.room1));
                updatePlayersTimer.Stop();

            }
            if(state.status == 0)
            {
                MessageBox.Show("Admin left the room!/ Closing room");
                Back_Click(null, new RoutedEventArgs());
                updatePlayersTimer.Stop();
            }
        }
        public static int CheckPlayersList(int roomId)
        {
            byte[] request = Serializer.SearalizeGetPlayersInRoomRequest(roomId);
            byte[] response = MainWindow.communicator.sendAndReceive(request);
            List<string> players = Deserializer.DeserializeGetPlayersInRoomResponse(response);
            return players.Count;
        }
        private void Back_Click(object sender, RoutedEventArgs e)
        {
            if(Login.user == adminName.Text)
            {
                byte[] request1 = Serializer.SerializeSimpleRequest(Serializer.CLOSE_ROOM_CODE);
                byte[] response1 = MainWindow.communicator.sendAndReceive(request1);
                updatePlayersTimer.Stop();
                triviaF.Navigate(new Trivia());
            }
            else
            {
                byte[] request = Serializer.SerializeSimpleRequest(Serializer.LEAVE_ROOM_CODE);
                byte[] response = MainWindow.communicator.sendAndReceive(request);
                updatePlayersTimer.Stop();
                triviaF.Navigate(new Trivia());
            }

        }

        private void start_Click(object sender, RoutedEventArgs e)
        {
            adminName.Text = Login.user + " (Admin)";
            byte[] request = Serializer.SerializeSimpleRequest(Serializer.START_GAME_CODE);
            byte[] response = MainWindow.communicator.sendAndReceive(request);
            //MessageBox.Show("Status: "+Deserializer.extractStatus(response).ToString());
            gameStarted = true;
        }
    }
}
