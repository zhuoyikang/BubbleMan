/*
 测试 SignalAgent
*/

package main

import (
	"fmt"
	"github.com/user/bubble/agent"
	"time"
)

type SignalHandler struct {
}

func (SignalHandler) SignalHub() {
	fmt.Printf("%s\n", "hub")
}

func (SignalHandler) SignalInt() {
	fmt.Printf("%s\n", "int")
}

func (SignalHandler) SignalTerm() {
	fmt.Printf("%s\n", "Term")
}

func main() {
	signalAgent := agent.MakeSignalAgent(SignalHandler{})
	signalAgent.Run()
	fmt.Printf("%s\n", "fe")
	time.Sleep(time.Second * 10)
}
