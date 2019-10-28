// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 254.
//!+

// Chat is a server that lets clients chat with each other.
package main

import (
	"bufio"
	"bytes"
	"flag"
	"fmt"
	"log"
	"net"
	"strings"
	"time"
	"os"
	"math/rand"
	"reflect"
)

//!+broadcaster
type client chan<- string // an outgoing message channel
var users map[string]net.Conn //Map of users
var moderator string
var isFirstConn bool
var (
	entering = make(chan client)
	leaving  = make(chan client)
	messages = make(chan string) // all incoming client messages
)


func getUsers() string {
	//Display info of connected users
	var buf bytes.Buffer
	buf.WriteString("server > Users on this channel:\n")
	buf.WriteString("--------------\n")
	for k, _ := range users {
		buf.WriteString(k + " ")
	}
	buf.WriteString("\n--------------\n")
	return buf.String()
}

func getUserInfo(user string) string {
	if _, ok := users[user]; ok { 
		return ("server > " + user + "IP: " + users[user].RemoteAddr().String())
	} else {
		return "Unavailable user"
	}

}

func getTime() string {
	var buf bytes.Buffer
	buf.WriteString("server > Current time: ")
	buf.WriteString(time.Now().Format(time.RFC850))
	return buf.String()
}

func getMod() string {
	var buf bytes.Buffer
	buf.WriteString("server > The moderator is ")
	buf.WriteString(moderator)
	return buf.String()
}

func sendPrivateMessage(origin, destination string, data []string) {
	if _, ok := users[destination]; ok { 
		fmt.Fprintln(users[destination], origin+"> "+strings.Join(data, " "))
	} else {
		fmt.Fprintln(users[destination], "Invalid username")
	}

}

func kickUser(kicker, banned string) string {
	if _, ok := users[banned]; ok { 
		if strings.Compare(kicker, moderator)!=0{
			return "Only the moderator can kick users from the channel"
		} else{
			users[banned].Close()
			delete(users, banned)
			return "server >" + banned + "has been kicked out"
		}
	} else {
		return "User doesn't exist"
	}
}


func setModerator(user string){
	moderator = user
}

func isModerator(user string) bool {
	return strings.Compare(user,moderator) == 0
}

func SelectNewModerator(mapI interface{}) interface{} {
	keys := reflect.ValueOf(mapI).MapKeys()
	return keys[rand.Intn(len(keys))].Interface()
}

func broadcaster() {
	clients := make(map[client]bool) // all connected clients
	for {
		select {
		case msg := <-messages:
			// Broadcast incoming message to all
			// clients' outgoing message channels.
			for cli := range clients {
				cli <- msg
			}

		case cli := <-entering:
			clients[cli] = true

		case cli := <-leaving:
			delete(clients, cli)
			close(cli)
		}
	}
}

//!-broadcaster

//!+handleConn
func handleConn(conn net.Conn, user string) {
users[user] = conn
ch := make(chan string) // outgoing client messages
if isFirstConn {
	setModerator(user)
	isFirstConn = false
}
go clientWriter(conn, ch, user)
ch <- "server > User " + user + " has joined the channel!"
if strings.Compare(user, moderator) == 0 {
	ch <- "server >" + user + " you are the moderator!"
}
entering <- ch
input := bufio.NewScanner(conn)
for input.Scan() {
	data := input.Text()
	data_len := len(data)
	if data_len <= 0 {
		messages <- user + "> " + input.Text()
		continue
	}
	if strings.Compare(string(data[0]), "/") != 0 {
		messages <- user + "> " + input.Text()
		continue
	}
	command := strings.Split(
		strings.Trim(data, " "),
		" ")
	if len(command) == 1 {
		switch command[0] {
		case "/users":
			ch <- getUsers()
		case "/time":
			ch <- getTime()
		case "/moderator":
			ch <- getMod()
		default:
			messages <- user + "> " + data
		}
	} else if len(command) == 2 {
		if strings.Compare(command[0], "/user") == 0 {
			ch <- getUserInfo(command[1])
		} 
		if strings.Compare(command[0], "/kick") == 0 {
			ch <- kickUser(user, command[1])
		} else {
			messages <- user + "> " + data
		}
	} else if len(command) >= 3 {
		if strings.Compare(command[0], "/msg") == 0 {
			sendPrivateMessage(user, command[1], command[2:])
		}

	}

}

if isModerator(user){
	moderator = SelectNewModerator(users).(string)
}
messages <- "server > " + moderator + " is now the moderator!"
delete(users, user)

fmt.Println("server >", user, "left")

leaving <- ch
messages <- user + " has left"

conn.Close()
}

func clientWriter(conn net.Conn, ch <-chan string, user string) {
	for msg := range ch {
		fmt.Fprintln(conn, msg) // NOTE: ignoring network errors
	}
}

//!-handleConn

//!+main
func main() {
	var host string
	var port string
	flag.StringVar(&host, "host", "localhost", "server address")
	flag.StringVar(&port, "port", "8000", "port to listen")
	flag.Parse()
	isFirstConn = true
	listener, err := net.Listen("tcp", host+":"+port)
	if err != nil {
		log.Fatal(err)
		os.Exit(-1);
	}
	users = make(map[string]net.Conn)
	fmt.Println("Server initializing...")
	fmt.Println(time.Now().UTC())
	fmt.Println("server> Go server running on:", host, port)
	fmt.Println("Remember the guidelines of this channel:")
	fmt.Println("- No swearing allowed, be nice to each other")
	fmt.Println("- No harrasment or bullying")
	fmt.Println("- No sexual undertones allowed")
	fmt.Println("- Failure to comply will result in inmediate banning by the moderator")
	go broadcaster()
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err)
			continue
		}
		user, _ := bufio.NewReader(conn).ReadString('\n')
		user = user[:len(user)-1]
		//Test is user exists
		if _, exists := users[user]; exists {
			fmt.Fprintf(conn, "server > Not a valid username, try another one\n")
			conn.Close()
		} else {
			go handleConn(conn, user)
		}
	}
}

//!-main
