/*
 测试 TcpAgent
*/

package main

import (
	"github.com/user/bubble/agent"
)

func main() {
	agent := agent.MakeTcpAgent("127.0.0.1:3004")
	agent.Run()
}
