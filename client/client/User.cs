﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace client
{
    public class User
    {
        public string Username { get; set; }
        public string Password { get; set; }
        public string Email { get; set; }
        public int TimePerQ { get; set; }
        public int Players { get; set; }
        public string RoomName { get; set; }
    }
}
