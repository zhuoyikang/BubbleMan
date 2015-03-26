package agent

import (
	"fmt"
	"net"
	"os"
)

type Session struct {
	Conn net.Conn
}

type TcpAgent struct {
	ip       string
	listener net.Listener
}

func MakeTcpAgent(ip string) TcpAgent {
	agent := TcpAgent{ip: ip}
	return agent
}

func (agent *TcpAgent) Run() {
	listener, err := net.Listen("tcp", agent.ip)
	if err != nil {
		fmt.Printf("error listening: %s\n", err.Error())
		os.Exit(1)
	}
	defer listener.Close()
	agent.listener = listener

	for {
		conn, err := listener.Accept()
		if err != nil {
			fmt.Printf("Error Accept %s\n", err.Error())
			return
		}
		fmt.Printf("%v\n", conn)
	}
}
