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

namespace client
{
    /// <summary>
    /// Interaction logic for CreateRoom.xaml
    /// </summary>
    public partial class CreateRoom : Page
    {
        private User user;
        public CreateRoom()
        {
            InitializeComponent();
            user = new User();
            this.DataContext = user;
        }

        private void CreateRoom_Click(object sender, RoutedEventArgs e)
        {
            if(Validation.GetHasError(PlayersAmountTextBox) || Validation.GetHasError(QuestionTimeTextBox) || Validation.GetHasError(RoomNameTextBox) || Validation.GetHasError(QuestionAmount))
            {
                MessageBox.Show("Please correct the highlighted fields.");
                return;
            }
            if(PlayersAmountTextBox.Text == "" || QuestionTimeTextBox.Text == "" || RoomNameTextBox.Text == "" || QuestionAmount.Text == "")
            {
                MessageBox.Show("Please correct the highlighted fields.");
                return;
            }
            byte[] request = Serializer.SearalizeCreateRoomRequest(int.Parse(PlayersAmountTextBox.Text), int.Parse(QuestionTimeTextBox.Text), RoomNameTextBox.Text, int.Parse(QuestionAmount.Text));
            byte[] response = MainWindow.communicator.sendAndReceive(request);
            int status = Deserializer.extractStatus(response);
 
            if (status == 1)
            {
                request = Serializer.SerializeSimpleRequest(Serializer.GET_ROOM_STATE_CODE);
                response = MainWindow.communicator.sendAndReceive(request);
                GetRoomStateResponse state = Deserializer.DesirializeGetRoomStateResponse(response);
                MessageBox.Show("Room created successfully!");
                int id;
                RoomInfo roomInfo = new RoomInfo(state.status, RoomNameTextBox.Text, int.Parse(PlayersAmountTextBox.Text), int.Parse(QuestionAmount.Text), int.Parse(QuestionTimeTextBox.Text));
                Room roomNow = new Room(roomInfo, Login.user);
                JoinRoomFrame.Navigate(roomNow);
                //request = Serializer.serializeGetRoomsRequest();
                //response = MainWindow.communicator.sendAndReceive(request);
                //id = -1;
                //List<RoomInfo> rooms = Deserializer.DeserializeGetRoomsResponse(response);
                //Room roomNow;
                //foreach (var room in rooms)
                //{
                //    if (room.name == RoomNameTextBox.Text)
                //    {
                //        id = room.id;
                //        roomNow = new Room(room, Login.user);
                //        request = Serializer.SearalizeJoinRoomRequest(id);
                //        response = MainWindow.communicator.sendAndReceive(request);
                //        if (Deserializer.extractStatus(response) != 1) MessageBox.Show("Error joining room");
                //        JoinRoomFrame.Navigate(roomNow);
                //    }
                //}
            }
        }

        private void Back_Click(object sender, RoutedEventArgs e)
        {
            RoomNameTextBox.Text = "";
            QuestionTimeTextBox.Text = "";
            QuestionAmount.Text = "";
            PlayersAmountTextBox.Text = "";
            this.DataContext = null;
            JoinRoomFrame.Navigate(new Trivia());
        }
    }
}
