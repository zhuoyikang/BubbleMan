msgbin:
	./proto_t.awk  proto.txt
	g++ -o msgbin MsgTest.cpp MsgBin.cpp MsgGen.cpp  ; ./msgbin

socknet:
	cp /Users/zhuoyikang/bubble_go/src/bubble/*.txt ./
	./proto_t.awk  proto.txt
	g++ -DSELF_BUBBLE_MAIN -o BubbleApp BubbleApp.cpp SockNet.cpp MsgBin.cpp MsgGen.cpp MsgQueue.cpp ; ./BubbleApp

run:
	./BubbleApp

g:
	./proto_t.awk  proto.txt

.PHONY:msgbin socknet g
