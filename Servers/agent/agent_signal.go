package agent

import (
	"os"
	"os/signal"
	"syscall"
)

/*------------------------------------------------------------------------------
 信号处理
------------------------------------------------------------------------------*/

type SignalHandler interface {
	SignalHub()
	SignalInt()
	SignalTerm()
}

type SignalAgent struct {
	signalChan chan os.Signal
	handler    SignalHandler
}

func MakeSignalAgent(handler SignalHandler) SignalAgent {
	agent := SignalAgent{}
	agent.signalChan = make(chan os.Signal, 1)
	agent.handler = handler
	return agent
}

func (agent *SignalAgent) Run() {
	signal.Notify(agent.signalChan,
		syscall.SIGHUP,
		syscall.SIGTERM,
		syscall.SIGINT)
	go func() {
		for {
			msg := <-agent.signalChan
			switch msg {
			case syscall.SIGHUP:
				agent.handler.SignalHub()
			case syscall.SIGINT:
				agent.handler.SignalInt()
			case syscall.SIGTERM:
				agent.handler.SignalTerm()
			}
		}
	}()
}
